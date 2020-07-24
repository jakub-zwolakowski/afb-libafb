/*
 * Copyright (C) 2015-2020 IoT.bzh Company
 * Author: José Bollo <jose.bollo@iot.bzh>
 *
 * $RP_BEGIN_LICENSE$
 * Commercial License Usage
 *  Licensees holding valid commercial IoT.bzh licenses may use this file in
 *  accordance with the commercial license agreement provided with the
 *  Software or, alternatively, in accordance with the terms contained in
 *  a written agreement between you and The IoT.bzh Company. For licensing terms
 *  and conditions see https://www.iot.bzh/terms-conditions. For further
 *  information use the contact form at https://www.iot.bzh/contact.
 *
 * GNU General Public License Usage
 *  Alternatively, this file may be used under the terms of the GNU General
 *  Public license version 3. This license is as published by the Free Software
 *  Foundation and appearing in the file LICENSE.GPLv3 included in the packaging
 *  of this file. Please review the following information to ensure the GNU
 *  General Public License requirements will be met
 *  https://www.gnu.org/licenses/gpl-3.0.html.
 * $RP_END_LICENSE$
 */

#include "libafb-config.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>

#include "core/afb-apiname.h"
#include "core/afb-apiset.h"
#include "apis/afb-api-ws.h"
#include "misc/afb-fdev.h"
#include "misc/afb-socket.h"
#include "misc/afb-socket-fdev.h"
#include "wsapi/afb-stub-ws.h"
#include "sys/verbose.h"
#include "sys/fdev.h"
#include "sys/x-socket.h"
#include "sys/x-errno.h"

struct api_ws_server
{
	struct afb_apiset *apiset;	/* the apiset for calling */
	struct fdev *fdev;		/* fdev handler */
	uint16_t offapi;		/* api name of the interface */
	char uri[];			/* the uri of the server socket */
};

/******************************************************************************/
/***       C L I E N T                                                      ***/
/******************************************************************************/

static struct fdev *reopen_client(void *closure)
{
	const char *uri = closure;
	return afb_socket_fdev_open(uri, 0);
}

int afb_api_ws_add_client(const char *uri, struct afb_apiset *declare_set, struct afb_apiset *call_set, int strong)
{
	struct afb_stub_ws *stubws;
	struct fdev *fdev;
	const char *api;
	int rc;

	/* check the api name */
	api = afb_socket_api(uri);
	if (api == NULL || !afb_apiname_is_valid(api)) {
		ERROR("invalid (too long) ws client uri %s", uri);
		rc = X_EINVAL;
		goto error;
	}

	/* open the socket */
	fdev = afb_socket_fdev_open(uri, 0);
	if (!fdev)
		rc = X_ENOMEM;
	else {
		/* create the client stub */
		stubws = afb_stub_ws_create_client(fdev, api, call_set);
		if (!stubws) {
			ERROR("can't setup client ws service to %s", uri);
			fdev_unref(fdev);
			rc = X_ENOMEM;
		} else {
			if (afb_stub_ws_client_add(stubws, declare_set) >= 0) {
#if 1
				/* it is asserted here that uri is never released */
				afb_stub_ws_client_robustify(stubws, reopen_client, (void*)uri, NULL);
#else
				/* it is asserted here that uri is released, so use a copy */
				afb_stub_ws_client_robustify(stubws, reopen_client, strdup(uri), free);
#endif
				return 0;
			}
			ERROR("can't add the client to the apiset for service %s", uri);
			afb_stub_ws_unref(stubws);
			rc = X_ENOMEM;
		}
	}
error:
	return strong ? rc : 0;
}

int afb_api_ws_add_client_strong(const char *uri, struct afb_apiset *declare_set, struct afb_apiset *call_set)
{
	return afb_api_ws_add_client(uri, declare_set, call_set, 1);
}

int afb_api_ws_add_client_weak(const char *uri, struct afb_apiset *declare_set, struct afb_apiset *call_set)
{
	return afb_api_ws_add_client(uri, declare_set, call_set, 0);
}

/*****************************************************************************/
/***       S E R V E R                                                      ***/
/******************************************************************************/

static void api_ws_server_accept(struct api_ws_server *apiws)
{
	int fd;
	struct sockaddr addr;
	socklen_t lenaddr;
	struct fdev *fdev;
	struct afb_stub_ws *server;

	lenaddr = (socklen_t)sizeof addr;
	fd = accept(fdev_fd(apiws->fdev), &addr, &lenaddr);
	if (fd < 0) {
		ERROR("can't accept connection to %s: %m", apiws->uri);
	} else {
		fdev = afb_fdev_create(fd);
		if (!fdev) {
			ERROR("can't hold accepted connection to %s", apiws->uri);
			close(fd);
		} else {
			server = afb_stub_ws_create_server(fdev, &apiws->uri[apiws->offapi], apiws->apiset);
			if (server)
				afb_stub_ws_set_on_hangup(server, afb_stub_ws_unref);
			else
				ERROR("can't serve accepted connection to %s", apiws->uri);
		}
	}
}

static int api_ws_server_connect(struct api_ws_server *apiws);

static void api_ws_server_listen_callback(void *closure, uint32_t revents, struct fdev *fdev)
{
	struct api_ws_server *apiws = closure;

	if ((revents & EPOLLHUP) != 0)
		api_ws_server_connect(apiws);
	else if ((revents & EPOLLIN) != 0)
		api_ws_server_accept(apiws);
}

static void api_ws_server_disconnect(struct api_ws_server *apiws)
{
	fdev_unref(apiws->fdev);
	apiws->fdev = 0;
}

static int api_ws_server_connect(struct api_ws_server *apiws)
{
	/* ensure disconnected */
	api_ws_server_disconnect(apiws);

	/* request the service object name */
	apiws->fdev = afb_socket_fdev_open(apiws->uri, 1);
	if (!apiws->fdev)
		ERROR("can't create socket %s", apiws->uri);
	else {
		/* listen for service */
		fdev_set_events(apiws->fdev, EPOLLIN);
		fdev_set_callback(apiws->fdev, api_ws_server_listen_callback, apiws);
		return 0;
	}
	return -1;
}

/* create the service */
int afb_api_ws_add_server(const char *uri, struct afb_apiset *declare_set, struct afb_apiset *call_set)
{
	int rc;
	const char *api;
	struct api_ws_server *apiws;
	size_t luri, lapi, extra;

	/* check the size */
	luri = strlen(uri);
	if (luri > 4000) {
		ERROR("can't create socket %s", uri);
		rc = X_E2BIG;
		goto error;
	}

	/* check the api name */
	api = afb_socket_api(uri);
	if (api == NULL || !afb_apiname_is_valid(api)) {
		ERROR("invalid api name in ws uri %s", uri);
		rc = X_EINVAL;
		goto error;
	}

	/* check api name */
	rc = afb_apiset_get_api(call_set, api, 1, 0, NULL);
	if (rc < 0) {
		ERROR("Can't provide ws-server for URI %s API %s", uri, api);
		goto error;
	}

	/* make the structure */
	lapi = strlen(api);
	extra = luri == (api - uri) + lapi ? 0 : lapi + 1;
	apiws = malloc(sizeof * apiws + 1 + luri + extra);
	if (!apiws) {
		ERROR("out of memory");
		rc = X_ENOMEM;
		goto error;
	}

	apiws->apiset = afb_apiset_addref(call_set);
	apiws->fdev = 0;
	strcpy(apiws->uri, uri);
	if (!extra)
		apiws->offapi = (uint16_t)(api - uri);
	else {
		apiws->offapi = (uint16_t)(luri + 1);
		strcpy(&apiws->uri[apiws->offapi], api);
	}

	/* connect for serving */
	rc = api_ws_server_connect(apiws);
	if (rc >= 0)
		return 0;

	afb_apiset_unref(apiws->apiset);
	free(apiws);
error:
	return rc;
}
