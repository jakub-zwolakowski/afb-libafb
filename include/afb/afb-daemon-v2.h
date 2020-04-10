/*
 * Copyright (C) 2015-2020 IoT.bzh Company
 * Author: José Bollo <jose.bollo@iot.bzh>
 *
 * SPDX-License-Identifier: LGPL-3.0-only
 */

#pragma once

#include "afb-daemon-itf-x1.h"

/** @addtogroup AFB_DAEMON
 *  @{ */

/**
 * @deprecated use bindings version 3
 *
 * Retrieves the common systemd's event loop of AFB
 */
static inline struct sd_event *afb_daemon_get_event_loop_v2()
{
	return afb_get_daemon_v2().itf->get_event_loop(afb_get_daemon_v2().closure);
}

/**
 * @deprecated use bindings version 3
 *
 * Retrieves the common systemd's user/session d-bus of AFB
 */
static inline struct sd_bus *afb_daemon_get_user_bus_v2()
{
	return afb_get_daemon_v2().itf->get_user_bus(afb_get_daemon_v2().closure);
}

/**
 * @deprecated use bindings version 3
 *
 * Retrieves the common systemd's system d-bus of AFB
 */
static inline struct sd_bus *afb_daemon_get_system_bus_v2()
{
	return afb_get_daemon_v2().itf->get_system_bus(afb_get_daemon_v2().closure);
}

/**
 * @deprecated use bindings version 3
 *
 * Broadcasts widely the event of 'name' with the data 'object'.
 * 'object' can be NULL.
 *
 * For convenience, the function calls 'json_object_put' for 'object'.
 * Thus, in the case where 'object' should remain available after
 * the function returns, the function 'json_object_get' shall be used.
 *
 * Calling this function is only forbidden during preinit.
 *
 * Returns 0 in case of success or -1 in case of error
 */
static inline int afb_daemon_broadcast_event_v2(const char *name, struct json_object *object)
{
	return afb_get_daemon_v2().itf->event_broadcast(afb_get_daemon_v2().closure, name, object);
}

/**
 * @deprecated use bindings version 3
 *
 * Creates an event of 'name' and returns it.
 *
 * Calling this function is only forbidden during preinit.
 *
 * See afb_event_is_valid to check if there is an error.
 */
static inline struct afb_event_x1 afb_daemon_make_event_v2(const char *name)
{
	return afb_get_daemon_v2().itf->event_make(afb_get_daemon_v2().closure, name);
}

/**
 * @deprecated use bindings version 3
 *
 * Send a message described by 'fmt' and following parameters
 * to the journal for the verbosity 'level'.
 *
 * 'file', 'line' and 'func' are indicators of position of the code in source files
 * (see macros __FILE__, __LINE__ and __func__).
 *
 * 'level' is defined by syslog standard:
 *      EMERGENCY         0        System is unusable
 *      ALERT             1        Action must be taken immediately
 *      CRITICAL          2        Critical conditions
 *      ERROR             3        Error conditions
 *      WARNING           4        Warning conditions
 *      NOTICE            5        Normal but significant condition
 *      INFO              6        Informational
 *      DEBUG             7        Debug-level messages
 */
static inline void afb_daemon_verbose_v2(int level, const char *file, int line, const char * func, const char *fmt, ...) __attribute__((format(printf, 5, 6)));
static inline void afb_daemon_verbose_v2(int level, const char *file, int line, const char * func, const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	afb_get_daemon_v2().itf->vverbose_v2(afb_get_daemon_v2().closure, level, file, line, func, fmt, args);
	va_end(args);
}

/**
 * @deprecated use bindings version 3
 *
 * Get the root directory file descriptor. This file descriptor can
 * be used with functions 'openat', 'fstatat', ...
 *
 * Returns the file descriptor or -1 in case of error.
 */
static inline int afb_daemon_rootdir_get_fd_v2()
{
	return afb_get_daemon_v2().itf->rootdir_get_fd(afb_get_daemon_v2().closure);
}

/**
 * @deprecated use bindings version 3
 *
 * Opens 'filename' within the root directory with 'flags' (see function openat)
 * using the 'locale' definition (example: "jp,en-US") that can be NULL.
 *
 * Returns the file descriptor or -1 in case of error.
 */
static inline int afb_daemon_rootdir_open_locale_v2(const char *filename, int flags, const char *locale)
{
	return afb_get_daemon_v2().itf->rootdir_open_locale(afb_get_daemon_v2().closure, filename, flags, locale);
}

/**
 * @deprecated use bindings version 3
 *
 * Queue the job defined by 'callback' and 'argument' for being executed asynchronously
 * in this thread (later) or in an other thread.
 * If 'group' is not NUL, the jobs queued with a same value (as the pointer value 'group')
 * are executed in sequence in the order of there submission.
 * If 'timeout' is not 0, it represent the maximum execution time for the job in seconds.
 * At first, the job is called with 0 as signum and the given argument.
 * The job is executed with the monitoring of its time and some signals like SIGSEGV and
 * SIGFPE. When a such signal is catched, the job is terminated and reexecuted but with
 * signum being the signal number (SIGALRM when timeout expired).
 *
 * Returns 0 in case of success or -1 in case of error.
 */
static inline int afb_daemon_queue_job_v2(void (*callback)(int signum, void *arg), void *argument, void *group, int timeout)
{
	return afb_get_daemon_v2().itf->queue_job(afb_get_daemon_v2().closure, callback, argument, group, timeout);
}

/**
 * @deprecated use bindings version 3
 *
 * Retrieves the afb_req stored at 'sreq'.
 * Returns the stored request.
 * The count of reference is UNCHANGED, thus, the
 * function 'afb_req_unref' should be called on the result
 * after that the asynchronous reply if sent.
 */
static inline struct afb_req_x1 afb_daemon_unstore_req_v2(struct afb_stored_req *sreq)
{
	return afb_get_daemon_v2().itf->unstore_req(afb_get_daemon_v2().closure, sreq);
}

/**
 * @deprecated use bindings version 3
 *
 * Tells that it requires the API of "name" to exist
 * and if 'initialized' is not null to be initialized.
 * Calling this function is only allowed within init.
 *
 * Returns 0 in case of success or -1 in case of error.
 */
static inline int afb_daemon_require_api_v2(const char *name, int initialized)
{
	return afb_get_daemon_v2().itf->require_api(afb_get_daemon_v2().closure, name, initialized);
}

/**
 * @deprecated use bindings version 3
 *
 * Create an aliased name 'as_name' for the api 'name'.
 * Calling this function is only allowed within preinit.
 *
 * Returns 0 in case of success or -1 in case of error.
 */
static inline int afb_daemon_add_alias_v2(const char *name, const char *as_name)
{
	return afb_get_daemon_v2().itf->add_alias(afb_get_daemon_v2().closure, name, as_name);
}

/**
 * @deprecated use bindings version 3
 *
 * Creates a new api of name 'api' with brief 'info'.
 *
 * Returns 0 in case of success or -1 in case of error.
 */
static inline int afb_daemon_new_api_v2(
	const char *api,
	const char *info,
	int noconcurrency,
	int (*preinit)(void*, struct afb_api_x3 *),
	void *closure)
{
	return -!(afb_get_daemon_v2().itf->new_api(afb_get_daemon_v2().closure, api, info, noconcurrency, preinit, closure));
}

/** @} */

