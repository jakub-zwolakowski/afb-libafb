/*
 * Copyright (C) 2015-2021 IoT.bzh Company
 * Author: José Bollo <jose.bollo@iot.bzh>
 *
 * SPDX-License-Identifier: LGPL-3.0-only
 */

#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <time.h>

#include "afb-ctlid.h"

/******************************************************************************/
/* declaration of types */

struct afb_verb_v4;

struct afb_api_x4;
struct afb_req_x4;
struct afb_event_x4;
struct afb_type_x4;
struct afb_data_x4;
struct afb_evfd_x4;
struct afb_timer_x4;

typedef struct afb_api_x4        *afb_api_x4_t;
typedef struct afb_req_x4        *afb_req_x4_t;
typedef struct afb_event_x4      *afb_event_x4_t;
typedef struct afb_data_x4       *afb_data_x4_t;
typedef struct afb_type_x4       *afb_type_x4_t;
typedef struct afb_evfd_x4       *afb_evfd_x4_t;
typedef struct afb_timer_x4      *afb_timer_x4_t;

/******************************************************************************/

typedef
	enum afb_type_flags_x4
{
	/** Data of that type can be shared through memory */
	Afb_Type_Flags_x4_Shareable = 1,

	/** Data of that type can be streamed */
	Afb_Type_Flags_x4_Streamable = 2,

	/** Data of that type can be opacified. */
	Afb_Type_Flags_x4_Opaque = 4
}
	afb_type_flags_x4_t;

/******************************************************************************/

/**
 * Main api callback function.
 *
 * @param api       the api that receive the callback
 * @param ctlid     identifier of the reason of the call (@see afb_ctlid)
 * @param ctlarg    data associated to the call
 * @param userdata  the userdata of the api (@see afb_api_get_userdata)
 *
 * @return a value that must be negative if an error occured. The
 *         return value is important when ctlid relates to an initialisation
 *         step, i.e. when it has one of the following predefined values:
 *         afb_ctlid_Root_Entry, afb_ctlid_Pre_Init, afb_ctlid_Init.
 */
typedef int (*afb_api_callback_x4_t)(
		afb_api_x4_t api,
		afb_ctlid_t ctlid,
		afb_ctlarg_t ctlarg,
		void *userdata);

/**
 * Request handling callback used for verbs.
 *
 * @param req      the request to process
 * @param nparams  the count of parameter data of the request
 * @param params   the array of parameter data of the request
 */
typedef void (*afb_req_callback_x4_t)(
		afb_req_x4_t req,
		unsigned nparams,
		afb_data_x4_t const params[]);

/**
 * Callback for receiving asynchronous results of a call.
 *
 * @param closure  the closure value given at call
 * @param status   the status of the call, negative on error
 * @param nreplies the count of replied data
 * @param replies  the array of replied data
 * @param api      the api that made the call
 */
typedef void (*afb_call_callback_x4_t)(
		void *closure,
		int status,
		unsigned nreplies,
		afb_data_x4_t const replies[],
		afb_api_x4_t api);

/**
 * Callback for receiving asynchronous results of a subcall.
 *
 * @param closure  the closure value given at subcall
 * @param status   the status of the subcall, negative on error
 * @param nreplies the count of replied data
 * @param replies  the array of replied data
 * @param req      the req that made the subcall
 */
typedef void (*afb_subcall_callback_x4_t)(
		void *closure,
		int status,
		unsigned nreplies,
		afb_data_x4_t const replies[],
		afb_req_x4_t req);

/**
 * Callback for receiving result of checking of permissions.
 *
 * @param closure  the closure value given at check
 * @param status   the status of the check, negative when rejected
 * @param req      the req that made the check
 */
typedef void (*afb_check_callback_x4_t)(
		void *closure,
		int status,
		afb_req_x4_t req);

/**
 * Callback for handling events.
 *
 * @param closure  the closure value given at creation of the handler
 * @param event_name name of the event
 * @param nparams  the count of parameter data of the event
 * @param params   the array of parameter data of the event
 * @param api      the api that holds the handler
 */
typedef void (*afb_event_handler_x4_t)(
		void *closure,
		const char *event_name,
		unsigned nparams,
		afb_data_x4_t const params[],
		afb_api_x4_t api);

/**
 * The type afb_type_converter_x4_t denote a conversion callback.
 *
 * A conversion callback receives 4 parameters:
 *
 * @param closure   A closure defined when converter is declared
 * @param from      The data to convert
 * @param type      The type to convert to
 * @param to        Where to store the result of the conversion
 *
 * @return It should return an integer status of 0 in case of success
 *         or a negative value indicating the error.
 */
typedef int (*afb_type_converter_x4_t)(
		void *closure,
		afb_data_x4_t from,
		afb_type_x4_t type,
		afb_data_x4_t *to);

/**
 * The type afb_type_updater_x4_t denote a conversion callback that is able
 * to update the target instead of creating it.
 *
 * A conversion callback receives 4 parameters:
 *
 * @param  closure   A closure defined when converter is declared
 * @param  from      The data of reference
 * @param  type      The type of the data to update
 * @param  to        the existing data to update from the given reference
 *
 * @return It should return an integer status of 0 in case of success
 *         or a negative value indicating the error.
 */
typedef int (*afb_type_updater_x4_t)(
		void *closure,
		afb_data_x4_t from,
		afb_type_x4_t type,
		afb_data_x4_t to);

/**
 * Callbacks of file event handlers
 *
 * These callbacks are called when an event occurs on the handled
 * file descriptor.
 *
 * @param efd the file event handler object
 * @param fd the file descriptor index
 * @param revents the received events
 * @param closure the closure given at creation
 */
typedef void (*afb_evfd_handler_x4_t)(afb_evfd_x4_t efd, int fd, uint32_t revents, void *closure);

/**
 * Callbacks of timers
 *
 * These callbacks are called when a programmed time even occurs.
 *
 * @param timer the timer object
 * @param closure the closure given at creation
 * @param decount reverse index of the event: zero for infinite timer
 *                or a decreasing value finishing with 1
 */
typedef void (*afb_timer_handler_x4_t)(afb_timer_x4_t timer, void *closure, int decount);

/******************************************************************************/

/**
 * Definition of the function's interface for the APIv3
 */
struct afb_binding_x4r1_itf
{
/*-- DATA ------------------------------------------*/

	/** create a new data from raw values */
	int (*create_data_raw)(
		afb_data_x4_t *data,
		afb_type_x4_t type,
		const void *buffer,
		size_t size,
		void (*dispose)(void*),
		void *closure);

	/** create a new data allocated in shareable memory */
	int (*create_data_alloc)(
		afb_data_x4_t *data,
		afb_type_x4_t type,
		void **pointer,
		size_t size);

	/** create a new data by copying in shareable memory */
	int (*create_data_copy)(
		afb_data_x4_t *data,
		afb_type_x4_t type,
		const void *pointer,
		size_t size);

	/** increment of reference count of data */
	afb_data_x4_t(*data_addref)(
		afb_data_x4_t data);

	/** decrement of reference count of data */
	void (*data_unref)(
		afb_data_x4_t data);

	/** get mutable pointer and size optionally */
	int (*data_get_mutable)(
		afb_data_x4_t data,
		void **pointer,
		size_t *size);

	/** get constant pointer and size optionally */
	int (*data_get_constant)(
		afb_data_x4_t data,
		void **pointer,
		size_t *size);

	/** updfate the data from the given value */
	int (*data_update)(
		afb_data_x4_t data,
		afb_data_x4_t value);

	/** get the data converted/formated to the given type (return a new reference) */
	int (*data_convert)(
		afb_data_x4_t data,
		afb_type_x4_t type,
		afb_data_x4_t *result);

	/** get the type */
	afb_type_x4_t (*data_type)(
		afb_data_x4_t data);

	/** notify the data changed */
	void (*data_notify_changed)(
		afb_data_x4_t data);

	/** is data volatile? */
	int (*data_is_volatile)(
		afb_data_x4_t data);

	/** set data volatile */
	void (*data_set_volatile)(
		afb_data_x4_t data);

	/** set data not volatile */
	void (*data_set_not_volatile)(
		afb_data_x4_t data);

	/** is data constant? */
	int (*data_is_constant)(
		afb_data_x4_t data);

	/** set data constant */
	void (*data_set_constant)(
		afb_data_x4_t data);

	/** set data not constant */
	void (*data_set_not_constant)(
		afb_data_x4_t data);

	/** lock data for reading */
	void (*data_lock_read)(
		afb_data_x4_t data);

	/** try lock data for reading */
	int (*data_try_lock_read)(
		afb_data_x4_t data);

	/** lock data for writing */
	void (*data_lock_write)(
		afb_data_x4_t data);

	/** try lock data for writing */
	int (*data_try_lock_write)(
		afb_data_x4_t data);

	/** unlock the data */
	void (*data_unlock)(
		afb_data_x4_t data);

/*-- REQ ------------------------------------------*/

	/** logmask of req */
	int (*req_logmask)(
		afb_req_x4_t req);

	/** increase reference count of the request */
	afb_req_x4_t (*req_addref)(
		afb_req_x4_t req);

	/** decrease reference count of the request */
	void (*req_unref)(
		afb_req_x4_t req);

	/** get api */
	afb_api_x4_t (*req_api)(
		afb_req_x4_t req);

	/** get vcbdata */
	void *(*req_vcbdata)(
		afb_req_x4_t req);

	/** get called api */
	const char *(*req_called_api)(
		afb_req_x4_t req);

	/** get called verb */
	const char *(*req_called_verb)(
		afb_req_x4_t req);

	/** log a message for the request */
	void (*req_vverbose)(
		afb_req_x4_t req,
		int level,
		const char *file,
		int line,
		const char * func,
		const char *fmt,
		va_list args);

	/** close the client session related to the api of the request */
	void (*req_session_close)(
		afb_req_x4_t req);

	/** set the levele of assurancy related to the api of the request */
	int (*req_session_set_LOA)(
		afb_req_x4_t req,
		unsigned level);

	/** [LEGACY] handle client cookie of the api getting the request */
	void *(*LEGACY_req_cookie)(
		afb_req_x4_t req,
		int replace,
		void *(*createcb)(void *closure),
		void (*freecb)(void *value),
		void *closure);

	/** make subscription of the client of the request to the event */
	int (*req_subscribe)(
		afb_req_x4_t req,
		afb_event_x4_t event);

	/** remove subscription of the client of the request to the event */
	int (*req_unsubscribe)(
		afb_req_x4_t req,
		afb_event_x4_t event);

	/** get description of the client of the request */
	struct json_object *(*req_get_client_info)(
		afb_req_x4_t req);

	/** check the permission */
	void (*req_check_permission)(
		afb_req_x4_t req,
		const char *permission,
		afb_check_callback_x4_t callback,
		void *closure);

	/** get the parameters of the request */
	unsigned (*req_parameters)(
		afb_req_x4_t req,
		afb_data_x4_t const **params);

	/** reply to the request */
	void (*req_reply)(
		afb_req_x4_t req,
		int status,
		unsigned nreplies,
		afb_data_x4_t const *replies);

	/** asynchronous subcall */
	void (*req_subcall)(
		afb_req_x4_t req,
		const char *apiname,
		const char *verbname,
		unsigned nparams,
		afb_data_x4_t const params[],
		int flags,
		afb_subcall_callback_x4_t callback,
		void *closure);

	/** synchronous subcall */
	int (*req_subcall_sync)(
		afb_req_x4_t req,
		const char *apiname,
		const char *verbname,
		unsigned nparams,
		afb_data_x4_t const params[],
		int flags,
		int *status,
		unsigned *nreplies,
		afb_data_x4_t replies[]);

/*-- EVENT ------------------------------------------*/

	/** rereference the event */
	afb_event_x4_t (*event_addref)(
		afb_event_x4_t event);

	/** unreference the event */
	void (*event_unref)(
		afb_event_x4_t event);

	/** get the event name */
	const char *(*event_name)(
		afb_event_x4_t event);

	/** push the event to its subscribers */
	int (*event_push)(
		afb_event_x4_t event,
		unsigned nparams,
		afb_data_x4_t const params[]);

	/** broadcast the event */
	int (*event_broadcast)(
		afb_event_x4_t event,
		unsigned nparams,
		afb_data_x4_t const params[]);

/*-- TYPE ------------------------------------------*/

	/** lookup for a type */
	int (*type_lookup)(
		afb_type_x4_t *type,
		const char *name);

	/** register a type */
	int (*type_register)(
		afb_type_x4_t *type,
		const char *name,
		afb_type_flags_x4_t flags);

	/** name of the type */
	const char *(*type_name)(
		afb_type_x4_t type
		);

	/** set the family of a type */
	int (*type_set_family)(
		afb_type_x4_t type,
		afb_type_x4_t family
		);

	/** add converter */
	int (*type_add_converter)(
		afb_type_x4_t fromtype,
		afb_type_x4_t totype,
		afb_type_converter_x4_t converter,
		void *closure
		);

	/** add updater */
	int (*type_add_updater)(
		afb_type_x4_t fromtype,
		afb_type_x4_t totype,
		afb_type_updater_x4_t updater,
		void *closure
		);

/*-- API ------------------------------------------*/

	/** logmask of api */
	int (*api_logmask)(
		afb_api_x4_t api);

	/** sending log messages */
	void (*api_vverbose)(
		afb_api_x4_t api,
		int level,
		const char *file,
		int line,
		const char * func,
		const char *fmt,
		va_list args);

	/** Get name of the api */
	const char *(*api_name)(
		afb_api_x4_t api);

	/** Getting user data */
	void *(*api_get_userdata)(
		afb_api_x4_t api);

	/** Setting user data */
	void *(*api_set_userdata)(
		afb_api_x4_t api,
		void *userdata);

	/** settings of the api */
	struct json_object *(*api_settings)(
		afb_api_x4_t api);

	/** broadcasts event 'name' with 'object' */
	int (*api_event_broadcast)(
		afb_api_x4_t api,
		const char *name,
		unsigned nparams,
		afb_data_x4_t const params[]);

	/** creates an event of 'name' */
	int (*api_new_event)(
		afb_api_x4_t api,
		const char *name,
		afb_event_x4_t *event);

	/** add an event handler for the api */
	int (*api_event_handler_add)(
		afb_api_x4_t api,
		const char *pattern,
		afb_event_handler_x4_t callback,
		void *closure);

	/** delete an event handler of the api */
	int (*api_event_handler_del)(
		afb_api_x4_t api,
		const char *pattern,
		void **closure);

	/** asynchronous call for the api */
	void (*api_call)(
		afb_api_x4_t api,
		const char *apiname,
		const char *verbname,
		unsigned nparams,
		afb_data_x4_t const params[],
		afb_call_callback_x4_t callback,
		void *closure);

	/** synchronous call for the api */
	int (*api_call_sync)(
		afb_api_x4_t api,
		const char *apiname,
		const char *verbname,
		unsigned nparams,
		afb_data_x4_t const params[],
		int *status,
		unsigned *nreplies,
		afb_data_x4_t replies[]);

	/** add one verb to the api */
	int (*api_add_verb)(
		afb_api_x4_t api,
		const char *verb,
		const char *info,
		afb_req_callback_x4_t callback,
		void *vcbdata,
		const struct afb_auth *auth,
		uint32_t session,
		int glob);

	/** delete one verb of the api */
	int (*api_del_verb)(
		afb_api_x4_t api,
		const char *verb,
		void **vcbdata);

	/** seal the api */
	void (*api_seal)(
		afb_api_x4_t api);

	/** set verbs of the api using v3 description */
	int (*api_set_verbs)(
		afb_api_x4_t api,
		const struct afb_verb_v4 *verbs);

	/** requires an api initialized or not */
	int (*api_require_api)(
		afb_api_x4_t api,
		const char *name,
		int initialized);

	/** indicate provided classes of the api */
	int (*api_class_provide)(
		afb_api_x4_t api,
		const char *name);

	/** indicate required classes of the api */
	int (*api_class_require)(
		afb_api_x4_t api,
		const char *name);

	/** delete the api */
	int (*api_delete)(
		afb_api_x4_t api);

/*-- MISC ------------------------------------------*/

	/** creation of a new api*/
	int (*create_api)(
		afb_api_x4_t root,
		afb_api_x4_t *newapi,
		const char *apiname,
		const char *info,
		int noconcurrency,
		afb_api_callback_x4_t mainctl,
		void *userdata);

	/** queue a job */
	int (*job_post)(
		afb_api_x4_t root,
		long delayms,
		int timeout,
		void (*callback)(int signum, void *arg),
		void *argument,
		void *group);

	/** add an alias */
	int (*alias_api)(
		afb_api_x4_t root,
		const char *name,
		const char *as_name);

	/** setup shared object */
	int (*setup_shared_object)(
		afb_api_x4_t root,
		void *handle);

/*-- PREDEFINED TYPES -----------------------------------*/

	/** predefined type opaque */
	afb_type_x4_t type_opaque;

	/** predefined type stringz */
	afb_type_x4_t type_stringz;

	/** predefined type json */
	afb_type_x4_t type_json;

	/** predefined type json-c */
	afb_type_x4_t type_json_c;

	/** predefined type char (0 or 1) */
	afb_type_x4_t type_bool;

	/** predefined type int32_t */
	afb_type_x4_t type_i32;

	/** predefined type uint32_t */
	afb_type_x4_t type_u32;

	/** predefined type int64_t */
	afb_type_x4_t type_i64;

	/** predefined type uint64_t */
	afb_type_x4_t type_u64;

	/** predefined type double */
	afb_type_x4_t type_double;

/*-- FD's EVENT HANDLING -----------------------------------*/

	/** create file event handler */
	int (*evfd_create)(
		afb_evfd_x4_t *efd,
		int fd,
		uint32_t events,
		afb_evfd_handler_x4_t handler,
		void *closure,
		int autounref,
		int autoclose);

	/** addref a file event handler */
	afb_evfd_x4_t (*evfd_addref)(
		afb_evfd_x4_t efd);

	/** unref a file event handler */
	void (*evfd_unref)(
		afb_evfd_x4_t efd);

	/** get file of a file event handler */
	int (*evfd_get_fd)(
		afb_evfd_x4_t efd);

	/** get events of a file event handler */
	uint32_t (*evfd_get_events)(
		afb_evfd_x4_t efd);

	/** set events of a file event handler */
	void (*evfd_set_events)(
		afb_evfd_x4_t efd,
		uint32_t events);

/*-- TIMER HANDLING -----------------------------------*/

	/** create a timer */
	int (*timer_create)(
		afb_timer_x4_t *timer,
		int absolute,
		time_t start_sec,
		unsigned start_ms,
		unsigned count,
		unsigned period_ms,
		unsigned accuracy_ms,
		afb_timer_handler_x4_t handler,
		void *closure,
		int autounref);

	/** addref the timer */
	afb_timer_x4_t (*timer_addref)(
		afb_timer_x4_t timer);

	/** unref the timer */
	void (*timer_unref)(
		afb_timer_x4_t timer);

/*-- EXTRA FUNCTIONS -----------------------------------*/

	/** get the current LOA for the request */
	unsigned (*req_session_get_LOA)(
		afb_req_x4_t req);

	/** add dependency from a data to an other data */
	int (*data_dependency_add)(
		afb_data_x4_t from_data,
		afb_data_x4_t to_data);

	/** remove dependency from a data to an other data */
	int (*data_dependency_sub)(
		afb_data_x4_t from_data,
		afb_data_x4_t to_data);

	/** drop all dependency from a data to all other data */
	void (*data_dependency_drop_all)(
		afb_data_x4_t data);

	/** set the cookie of the api getting the request */
	int (*req_cookie_set)(
		afb_req_x4_t req,
		void *value,
		void (*freecb)(void*),
		void *freeclo);

	/** get the cookie of the api getting the request */
	int (*req_cookie_get)(
		afb_req_x4_t req,
		void **value);

	/** get the cookie of the api getting the request */
	int (*req_cookie_getinit)(
		afb_req_x4_t req,
		void **value,
		int (*initcb)(void *closure, void **value, void (**freecb)(void*), void **freeclo),
		void *closure);

	/** set the cookie of the api getting the request */
	int (*req_cookie_drop)(
		afb_req_x4_t req);

/*-- END OF VERSION 4r1 -----------------------------------*/
};
