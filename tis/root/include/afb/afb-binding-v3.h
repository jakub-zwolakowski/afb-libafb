/*
 * Copyright (C) 2015-2021 IoT.bzh Company
 * Author: José Bollo <jose.bollo@iot.bzh>
 *
 * SPDX-License-Identifier: LGPL-3.0-only
 */

/**< @file afb/afb-binding-v3.h */

#pragma once

/******************************************************************************/

#include "afb-errno.h"
#include "afb-verbosity.h"
#include "afb-auth.h"
#include "afb-event-x2.h"
#include "afb-req-x2.h"
#include "afb-session.h"
#include "afb-api-x3.h"

/******************************************************************************/

/**
 * @page validity-v3 Validity of a binding v3
 *
 * A binding V3 MUST have at least two exported symbols of name:
 *
 *   - @ref afbBindingV3root
 *   - @ref afbBindingV3  and/or  @ref afbBindingV3entry
 *
 * @ref afbBindingV3root is automatically created when **AFB_BINDING_VERSION == 3**
 * without programmer action, as a hidden variable linked as *weak*.
 *
 * The symbols @ref afbBindingV3  and  **afbBindingV3entry** are under control
 * of the programmer.
 *
 * The symbol @ref afbBindingV3 if defined is used, as in binding v2, to describe
 * an API that will be declared during pre-initialization of bindings.
 *
 * The symbol @ref afbBindingV3entry if defined will be called during
 * pre-initialization.
 *
 * If @ref afbBindingV3entry and @ref afbBindingV3 are both defined, it is an
 * error to fill the field @ref preinit of @ref afbBindingV3.
 *
 * @see afb_binding_v3
 * @see afbBindingV3root
 * @see afbBindingV3entry
 * @see afbBindingV3
 */

/**
 * Description of one verb as provided for binding API version 3
 */
struct afb_verb_v3
{
	/** name of the verb, NULL only at end of the array */
	const char *verb;

	/** callback function implementing the verb */
	void (*callback)(struct afb_req_x2 *req);

	/** required authorization, can be NULL */
	const struct afb_auth *auth;

	/** some info about the verb, can be NULL */
	const char *info;

	/**< data for the verb callback */
	void *vcbdata;

	/** authorization and session requirements of the verb */
	uint16_t session;

	/** is the verb glob name */
	uint16_t glob: 1;
};

/**
 * Description of the bindings of type version 3
 */
struct afb_binding_v3
{
	/** api name for the binding, can't be NULL */
	const char *api;

	/** textual specification of the binding, can be NULL */
	const char *specification;

	/** some info about the api, can be NULL */
	const char *info;

	/** array of descriptions of verbs terminated by a NULL name, can be NULL */
	const struct afb_verb_v3 *verbs;

	/** callback at load of the binding */
	int (*preinit)(struct afb_api_x3 *api);

	/** callback for starting the service */
	int (*init)(struct afb_api_x3 *api);

	/** callback for handling events */
	void (*onevent)(struct afb_api_x3 *api, const char *event, struct json_object *object);

	/** userdata for afb_api_x3 */
	void *userdata;

	/** space separated list of provided class(es) */
	const char *provide_class;

	/** space separated list of required class(es) */
	const char *require_class;

	/** space separated list of required API(es) */
	const char *require_api;

	/** avoids concurrent requests to verbs */
	unsigned noconcurrency: 1;
};

/**
 * Default root binding's api that can be used when no explicit context is
 * available.
 *
 * When @ref afbBindingV3 is defined, this variable records the corresponding
 * api handler. Otherwise, it points to an fake handles that allows logging
 * and api creation.
 *
 * @see afbBindingV3entry
 * @see afbBindingV3
 * @see @ref validity-v3
 */
#if !defined(AFB_BINDING_NO_ROOT) /* use with caution, see @ref validity-v3 */
#if AFB_BINDING_VERSION == 3
struct afb_api_x3 *afbBindingV3root __attribute__((weak));
#endif
#endif

/**
 * Pre-initialization function.
 *
 * If this function is defined and exported in the produced binding
 * (shared object), it will be called during pre-initialization with the
 * rootapi defined by \ref afbBindingV3root
 *
 * @param rootapi the root api (equals to afbBindingV3root)
 *
 * @return the function must return a negative integer on error to abort the
 * initialization of the binding. Any positive or zero returned value is a
 * interpreted as a success.

 * @see afbBindingV3root
 * @see afbBindingV3
 * @see @ref validity-v3
 */
extern int afbBindingV3entry(struct afb_api_x3 *rootapi);

/**
 * Static definition of the root api of the binding.
 *
 * This symbol if defined describes the API of the binding.
 *
 * If it is not defined then the function @ref afbBindingV3entry must be defined
 *
 * @see afbBindingV3root
 * @see afbBindingV3entry
 * @see @ref validity-v3
 */
extern const struct afb_binding_v3 afbBindingV3;

/******************************************************************************/
#if AFB_BINDING_VERSION == 3

typedef struct afb_verb_v3			afb_verb_t;
typedef struct afb_binding_v3			afb_binding_t;

#define afbBindingExport			afbBindingV3
#define afbBindingRoot				afbBindingV3root
#define afbBindingEntry				afbBindingV3entry

#define afb_get_root_api()			(afbBindingV3root)
#define afb_get_daemon()			afb_get_root_api()
#define afb_get_service()			afb_get_root_api()
#define afb_get_logmask()			(afb_get_root_api()->logmask)

static inline int _afb_get_verbosity_(int verbomask)
{
	int result = AFB_SYSLOG_LEVEL_ERROR;
	while ((verbomask >>= result) && result < AFB_SYSLOG_LEVEL_DEBUG)
		result++;
	return result - AFB_SYSLOG_LEVEL_ERROR;
}
#define afb_get_verbosity()			_afb_get_verbosity_(afb_get_logmask())

#define afb_daemon_get_event_loop()		afb_api_get_event_loop(afbBindingV3root)
#define afb_daemon_get_user_bus()		afb_api_get_user_bus(afbBindingV3root)
#define afb_daemon_get_system_bus()		afb_api_get_system_bus(afbBindingV3root)
#define afb_daemon_broadcast_event(...)		afb_api_broadcast_event(afbBindingV3root,__VA_ARGS__)
#define afb_daemon_make_event(...)		afb_api_make_event(afbBindingV3root,__VA_ARGS__)
#define afb_daemon_verbose(...)			afb_api_verbose(afbBindingV3root,__VA_ARGS__)
#define afb_daemon_rootdir_get_fd()		afb_api_rootdir_get_fd(afbBindingV3root)
#define afb_daemon_rootdir_open_locale(...)	afb_api_rootdir_open_locale(afbBindingV3root,__VA_ARGS__)
#define afb_daemon_queue_job(...)		afb_api_queue_job(afbBindingV3root,__VA_ARGS__)
#define afb_daemon_require_api(...)		afb_api_require_api(afbBindingV3root,__VA_ARGS__)
#define afb_daemon_add_alias(...)		afb_api_add_alias(afbBindingV3root,__VA_ARGS__)

#define afb_service_call(...)			afb_api_call(afbBindingV3root,__VA_ARGS__)
#define afb_service_call_sync(...)		afb_api_call_sync(afbBindingV3root,__VA_ARGS__)
#define afb_service_call_legacy(...)		afb_api_call_legacy(afbBindingV3root,__VA_ARGS__)
#define afb_service_call_sync_legacy(...)	afb_api_call_sync_legacy(afbBindingV3root,__VA_ARGS__)

#endif
/******************************************************************************/
