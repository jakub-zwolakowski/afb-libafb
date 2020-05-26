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

#include <json-c/json.h>

#define AFB_BINDING_VERSION 0
#include <afb/afb-binding.h>

#include "core/afb-calls.h"
#include "core/afb-evt.h"
#include "core/afb-export.h"
#include "core/afb-hook.h"
#include "core/afb-msg-json.h"
#include "core/afb-session.h"
#include "core/afb-xreq.h"
#include "core/afb-error-text.h"

#include "core/afb-sched.h"
#include "sys/verbose.h"
#include "sys/x-errno.h"

#define CALLFLAGS            (afb_req_subcall_api_session|afb_req_subcall_catch_events)

/************************************************************************/

struct modes
{
	unsigned hooked: 1;
	unsigned sync: 1;
};

#if WITH_AFB_CALL_SYNC
#define mode_sync  ((struct modes){ .hooked=0, .sync=1 })
#endif
#define mode_async  ((struct modes){ .hooked=0, .sync=0 })

#if WITH_AFB_HOOK
#if WITH_AFB_CALL_SYNC
#define mode_hooked_sync  ((struct modes){ .hooked=1, .sync=1 })
#endif
#define mode_hooked_async  ((struct modes){ .hooked=1, .sync=0 })
#endif

union callback {
	void *any;
	union {
		void (*x3)(void*, struct json_object*, const char*, const char *, struct afb_req_x2*);
	} subcall;
	union {
		void (*x3)(void*, struct json_object*, const char*, const char*, struct afb_api_x3*);
	} call;
};

struct callreq
{
	struct afb_xreq xreq;

	struct afb_export *export;

	struct modes mode;

	int flags;

	union {
#if WITH_AFB_CALL_SYNC
		struct {
			struct afb_sched_lock *afb_sched_lock;
			int returned;
			int status;
			struct json_object **object;
			char **error;
			char **info;
		};
#endif
		struct {
			union callback callback;
			void *closure;
			union {
				void (*final)(void*, struct json_object*, const char*, const char*, union callback, struct afb_export*,struct afb_xreq*);
			};
		};
	};
};

/******************************************************************************/

static inline int errstr2errno(const char *error)
{
	return error ? X_EAGAIN : 0;
}

/******************************************************************************/
#if WITH_AFB_CALL_SYNC

static int store_reply(
		struct json_object *iobject, const char *ierror, const char *iinfo,
		struct json_object **sobject, char **serror, char **sinfo)
{
	if (serror) {
		if (!ierror)
			*serror = NULL;
		else if (!(*serror = strdup(ierror))) {
			ERROR("can't report error %s", ierror);
			json_object_put(iobject);
			iobject = NULL;
			iinfo = NULL;
		}
	}

	if (sobject)
		*sobject = iobject;
	else
		json_object_put(iobject);

	if (sinfo) {
		if (!iinfo)
			*sinfo = NULL;
		else if (!(*sinfo = strdup(iinfo)))
			ERROR("can't report info %s", iinfo);
	}

	return errstr2errno(ierror);
}

static void sync_leave(struct callreq *callreq)
{
	struct afb_sched_lock *afb_sched_lock = __atomic_exchange_n(&callreq->afb_sched_lock, NULL, __ATOMIC_RELAXED);
	if (afb_sched_lock)
		afb_sched_leave(afb_sched_lock);
}

static void sync_enter(int signum, void *closure, struct afb_sched_lock *afb_sched_lock)
{
	struct callreq *callreq = closure;
	if (!signum) {
		callreq->afb_sched_lock = afb_sched_lock;
		afb_export_process_xreq(callreq->export, &callreq->xreq);
	} else {
		afb_xreq_reply(&callreq->xreq, NULL, afb_error_text_internal_error, NULL);
	}
}

#endif
/******************************************************************************/

static void callreq_destroy_cb(struct afb_xreq *xreq)
{
	struct callreq *callreq = CONTAINER_OF_XREQ(struct callreq, xreq);

	afb_context_disconnect(&callreq->xreq.context);
	json_object_put(callreq->xreq.json);
	free(callreq);
}

static void callreq_reply_cb(struct afb_xreq *xreq, struct json_object *object, const char *error, const char *info)
{
	struct callreq *callreq = CONTAINER_OF_XREQ(struct callreq, xreq);

#if WITH_AFB_HOOK
	/* centralized hooking */
	if (callreq->mode.hooked) {
		if (callreq->mode.sync) {
			if (callreq->xreq.caller)
				afb_hook_xreq_subcallsync_result(callreq->xreq.caller, errstr2errno(error), object, error, info);
			else
				afb_hook_api_callsync_result(callreq->export, errstr2errno(error), object, error, info);
		} else {
			if (callreq->xreq.caller)
				afb_hook_xreq_subcall_result(callreq->xreq.caller, object, error, info);
			else
				afb_hook_api_call_result(callreq->export, object, error, info);
		}
	}
#endif

	/* true report of the result */
#if WITH_AFB_CALL_SYNC
	if (callreq->mode.sync) {
		callreq->returned = 1;
		callreq->status = store_reply(object, error, info,
				callreq->object, callreq->error, callreq->info);
		sync_leave(callreq);
	} else {
#endif
		callreq->final(callreq->closure, object, error, info, callreq->callback, callreq->export, callreq->xreq.caller);
		json_object_put(object);
#if WITH_AFB_CALL_SYNC
	}
#endif
}

static int callreq_subscribe_cb(struct afb_xreq *xreq, struct afb_event_x2 *event)
{
	int rc = 0, rc2;
	struct callreq *callreq = CONTAINER_OF_XREQ(struct callreq, xreq);

	if (callreq->flags & afb_req_subcall_pass_events)
		rc = afb_xreq_subscribe(callreq->xreq.caller, event);
	if (callreq->flags & afb_req_subcall_catch_events) {
		rc2 = afb_export_subscribe(callreq->export, event);
		if (rc2 < 0)
			rc = rc2;
	}
	return rc;
}

static int callreq_unsubscribe_cb(struct afb_xreq *xreq, struct afb_event_x2 *event)
{
	int rc = 0, rc2;
	struct callreq *callreq = CONTAINER_OF_XREQ(struct callreq, xreq);

	if (callreq->flags & afb_req_subcall_pass_events)
		rc = afb_xreq_unsubscribe(callreq->xreq.caller, event);
	if (callreq->flags & afb_req_subcall_catch_events) {
		rc2 = afb_export_unsubscribe(callreq->export, event);
		if (rc2 < 0)
			rc = rc2;
	}
	return rc;
}

/******************************************************************************/

const struct afb_xreq_query_itf afb_calls_xreq_itf = {
	.unref = callreq_destroy_cb,
	.reply = callreq_reply_cb,
	.subscribe = callreq_subscribe_cb,
	.unsubscribe = callreq_unsubscribe_cb
};

/******************************************************************************/

static struct callreq *callreq_create(
		struct afb_export *export,
		struct afb_xreq *caller,
		const char *api,
		const char *verb,
		struct json_object *args,
		int flags,
		struct modes mode)
{
	struct callreq *callreq;
	size_t lenapi, lenverb;
	char *api2, *verb2;

	lenapi = 1 + strlen(api);
	lenverb = 1 + strlen(verb);
	callreq = malloc(lenapi + lenverb + sizeof *callreq);
	if (!callreq) {
		ERROR("out of memory");
		json_object_put(args);
	} else {
		afb_xreq_init(&callreq->xreq, &afb_calls_xreq_itf);
		api2 = (char*)&callreq[1];
		callreq->xreq.request.called_api = memcpy(api2, api, lenapi);;
		verb2 = &api2[lenapi];
		callreq->xreq.request.called_verb = memcpy(verb2, verb, lenverb);
		callreq->xreq.json = args;
		callreq->mode = mode;
		if (!caller)
			afb_export_context_init(export, &callreq->xreq.context);
		else {
			if (flags & afb_req_subcall_api_session)
				afb_export_context_init(export, &callreq->xreq.context);
			else
				afb_context_subinit(&callreq->xreq.context, &caller->context);
			if (flags & afb_req_subcall_on_behalf)
				afb_context_on_behalf_other_context(&callreq->xreq.context, &caller->context);
			callreq->xreq.caller = caller;
			afb_xreq_unhooked_addref(caller);
			export = afb_export_from_api_x3(caller->request.api);
		}
		callreq->export = export;
		callreq->flags = flags;
	}
	return callreq;
}

/******************************************************************************/
#if WITH_AFB_CALL_SYNC
static int do_sync(
		struct afb_export *export,
		struct afb_xreq *caller,
		const char *api,
		const char *verb,
		struct json_object *args,
		int flags,
		struct json_object **object,
		char **error,
		char **info,
		struct modes mode)
{
	struct callreq *callreq;
	int rc;

	/* allocates the request */
	callreq = callreq_create(export, caller, api, verb, args, flags, mode);
	if (!callreq)
		goto interr;

	/* initializes the request */
	callreq->afb_sched_lock = NULL;
	callreq->returned = 0;
	callreq->status = 0;
	callreq->object = object;
	callreq->error = error;
	callreq->info = info;

	afb_xreq_unhooked_addref(&callreq->xreq); /* avoid early callreq destruction */

	rc = afb_sched_enter(NULL, 0, sync_enter, callreq);
	if (rc >= 0 && callreq->returned) {
		rc = callreq->status;
		afb_xreq_unhooked_unref(&callreq->xreq);
		return rc;
	}

	afb_xreq_unhooked_unref(&callreq->xreq);
interr:
	return store_reply(NULL, afb_error_text_internal_error, NULL, object, error, info);
}
#endif

/******************************************************************************/

static void do_async(
		struct afb_export *export,
		struct afb_xreq *caller,
		const char *api,
		const char *verb,
		struct json_object *args,
		int flags,
		void *callback,
		void *closure,
		void (*final)(void*, struct json_object*, const char*, const char*, union callback, struct afb_export*,struct afb_xreq*),
		struct modes mode)
{
	struct callreq *callreq;

	callreq = callreq_create(export, caller, api, verb, args, flags, mode);

	if (!callreq)
		final(closure, NULL, afb_error_text_internal_error, NULL, (union callback){ .any = callback }, export, caller);
	else {
		callreq->callback.any = callback;
		callreq->closure = closure;
		callreq->final = final;

		afb_export_process_xreq(callreq->export, &callreq->xreq);
	}
}

/******************************************************************************/

static void final_call(
	void *closure,
	struct json_object *object,
	const char *error,
	const char *info,
	union callback callback,
	struct afb_export *export,
	struct afb_xreq *caller)
{
	if (callback.call.x3)
		callback.call.x3(closure, object, error, info, afb_export_to_api_x3(export));
}

static void final_subcall(
	void *closure,
	struct json_object *object,
	const char *error,
	const char *info,
	union callback callback,
	struct afb_export *export,
	struct afb_xreq *caller)
{
	if (callback.subcall.x3)
		callback.subcall.x3(closure, object, error, info, xreq_to_req_x2(caller));
}

/******************************************************************************/

void afb_calls_call(
		struct afb_export *export,
		const char *api,
		const char *verb,
		struct json_object *args,
		void (*callback)(void*, struct json_object*, const char *error, const char *info, struct afb_api_x3*),
		void *closure)
{
	do_async(export, NULL, api, verb, args, CALLFLAGS, callback, closure, final_call, mode_async);
}

#if WITH_AFB_CALL_SYNC
int afb_calls_call_sync(
		struct afb_export *export,
		const char *api,
		const char *verb,
		struct json_object *args,
		struct json_object **object,
		char **error,
		char **info)
{
	return do_sync(export, NULL, api, verb, args, CALLFLAGS, object, error, info, mode_sync);
}
#endif

void afb_calls_subcall(
			struct afb_xreq *xreq,
			const char *api,
			const char *verb,
			struct json_object *args,
			int flags,
			void (*callback)(void *closure, struct json_object *object, const char *error, const char * info, struct afb_req_x2 *req),
			void *closure)
{
	do_async(NULL, xreq, api, verb, args, flags, callback, closure, final_subcall, mode_async);
}

#if WITH_AFB_CALL_SYNC
int afb_calls_subcall_sync(
			struct afb_xreq *xreq,
			const char *api,
			const char *verb,
			struct json_object *args,
			int flags,
			struct json_object **object,
			char **error,
			char **info)
{
	return do_sync(NULL, xreq, api, verb, args, flags, object, error, info, mode_sync);
}
#endif

#if WITH_AFB_HOOK
void afb_calls_hooked_call(
		struct afb_export *export,
		const char *api,
		const char *verb,
		struct json_object *args,
		void (*callback)(void*, struct json_object*, const char *error, const char *info, struct afb_api_x3*),
		void *closure)
{
	afb_hook_api_call(export, api, verb, args);
	do_async(export, NULL, api, verb, args, CALLFLAGS, callback, closure, final_call, mode_hooked_async);
}

#if WITH_AFB_CALL_SYNC
int afb_calls_hooked_call_sync(
		struct afb_export *export,
		const char *api,
		const char *verb,
		struct json_object *args,
		struct json_object **object,
		char **error,
		char **info)
{
	afb_hook_api_callsync(export, api, verb, args);
	return do_sync(export, NULL, api, verb, args, CALLFLAGS, object, error, info, mode_hooked_sync);
}
#endif

void afb_calls_hooked_subcall(
			struct afb_xreq *xreq,
			const char *api,
			const char *verb,
			struct json_object *args,
			int flags,
			void (*callback)(void *closure, struct json_object *object, const char *error, const char * info, struct afb_req_x2 *req),
			void *closure)
{
	afb_hook_xreq_subcall(xreq, api, verb, args, flags);
	do_async(NULL, xreq, api, verb, args, flags, callback, closure, final_subcall, mode_hooked_async);
}

#if WITH_AFB_CALL_SYNC
int afb_calls_hooked_subcall_sync(
			struct afb_xreq *xreq,
			const char *api,
			const char *verb,
			struct json_object *args,
			int flags,
			struct json_object **object,
			char **error,
			char **info)
{
	afb_hook_xreq_subcallsync(xreq, api, verb, args, flags);
	return do_sync(NULL, xreq, api, verb, args, flags, object, error, info, mode_hooked_sync);
}
#endif
#endif

