
# Functions of class **afb\_req**


## Function afb\_req\_logmask

```C
/**
 * Get the logmask of the 'req'.
 *
 * @param req the req whose logmask is to be returned
 *
 * @return logmask of the req.
 */
int
afb_req_logmask(
	afb_req_t req
);
```

## Function afb\_req\_wants\_log\_level

```C
/**
 * Is the log message of 'level (as defined for syslog) required for the
 * request 'req'?
 *
 * @param req the request
 * @param level the level to check as defined for syslog:
 *
 *      EMERGENCY         0        System is unusable
 *      ALERT             1        Action must be taken immediately
 *      CRITICAL          2        Critical conditions
 *      ERROR             3        Error conditions
 *      WARNING           4        Warning conditions
 *      NOTICE            5        Normal but significant condition
 *      INFO              6        Informational
 *      DEBUG             7        Debug-level messages
 *
 * @return 0 if not required or a value not null if required
 *
 * @see syslog
 */
int
afb_req_wants_log_level(
	afb_req_t req,
	int level
);
```

## Function afb\_req\_is\_valid

```C
/**
 * Checks whether the request 'req' is valid or not.
 *
 * @param req the request to check
 *
 * @return 0 if not valid or 1 if valid.
 */
int
afb_req_is_valid(
	afb_req_t req
);
```

## Function afb\_req\_get\_api

```C
/**
 * Retrieves the api that serves the request
 *
 * @param req the request whose serving api is queried
 *
 * @return the api serving the request
 */
afb_api_t
afb_req_get_api(
	afb_req_t req
);
```

## Function afb\_req\_get\_vcbdata

```C
/**
 * Retrieves the callback data of the verb. This callback data is set
 * when the verb is created.
 *
 * @param req whose verb vcbdata is queried
 *
 * @return the callback data attached to the verb description
 */
void *
afb_req_get_vcbdata(
	afb_req_t req
);
```

## Function afb\_req\_get\_called\_api

```C
/**
 * Retrieve the name of the called api.
 *
 * @param req the request
 *
 * @return the name of the called api
 *
 * @see afb_alias_api
 */
const char *
afb_req_get_called_api(
	afb_req_t req
);
```

## Function afb\_req\_get\_called\_verb

```C
/**
 * Retrieve the name of the called verb
 *
 * @param req the request
 *
 * @return the name of the called verb
 */
const char *
afb_req_get_called_verb(
	afb_req_t req
);
```

## Function afb\_req\_addref

```C
/**
 * Increments the count of references of 'req'.
 *
 * @param req the request
 *
 * @return returns the request req
 */
afb_req_t
afb_req_addref(
	afb_req_t req
);
```

## Function afb\_req\_unref

```C
/**
 * Decrement the count of references of 'req'.
 *
 * @param req the request
 */
void
afb_req_unref(
	afb_req_t req
);
```

## Function afb\_req\_vverbose

```C
/**
 * Send associated to 'req' a message described by 'fmt' and its 'args'
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
 *
 * @param req the request
 * @param level the level of the message
 * @param file the source filename that emits the message or NULL
 * @param line the line number in the source filename that emits the message
 * @param func the name of the function that emits the message or NULL
 * @param fmt the message format as for printf
 * @param args the arguments to the format 'fmt'
 *
 * @see printf
 * @see afb_req_verbose
 */
void
afb_req_vverbose(
	afb_req_t req,
	int level, const char *file,
	int line,
	const char * func,
	const char *fmt,
	va_list args
);
```

## Function afb\_req\_verbose

```C
/**
 * Send associated to 'req' a message described by 'fmt' and following parameters
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
 *
 * @param req the request
 * @param level the level of the message
 * @param file the source filename that emits the message or NULL
 * @param line the line number in the source filename that emits the message
 * @param func the name of the function that emits the message or NULL
 * @param fmt the message format as for printf
 * @param ... the arguments of the format 'fmt'
 *
 * @see printf
 * @see afb_req_vverbose
 */
void
afb_req_verbose(
	afb_req_t req,
	int level, const char *file,
	int line,
	const char * func,
	const char *fmt,
	...
);
```

## Function afb\_req\_context

```C
/**
 * Manage the pointer stored by the binding for the client session of 'req'.
 *
 * If some value was set as context by a previous call to 'afb_req_context' or
 * 'afb_req_context_set', return that value where 'ptrval' points.
 *
 * Otherwise, call the initialization callback 'initcb' with the given 'closure'
 * to initialize the value. The callback 'initcb' must initialiaze the 3 data
 * whose pointers are given: value, freecb and freeclo, as for the function
 * 'afb_req_context_set'.
 *
 * It is allowed to pass NULL for 'initcb'. In that case, the value of the
 * context is initialized to 'closure'.
 *
 * This function is atomic: it ensures that 2 threads will not race together.
 *
 * @param req the request
 * @param ptrval if not zero the value of the context is returned where pointed
 * @param initcb the initialisation function or NULL
 * @param closure the closure to the creation function
 *
 * @return 0 if context was set, 1 if context has been created, a negative value on error
 */
int
afb_req_context(
	afb_req_t req,
	void **ptrval,
	int (*initcb)(void *closure, void **value, void (**freecb)(void*), void **freeclo),
	void *closure
);
```

## Function afb\_req\_context\_get

```C
/**
 * Gets in '*ptrval' the pointer stored by the binding for the session of 'req'.
 * When the binding has not yet recorded a pointer, NULL is returned.
 *
 * @param req the request
 * @param ptrval if not zero the value of the context is returned where pointed
 *
 * @return 0 on success, a negative value on error
 */
int
afb_req_context_get(
	afb_req_t req,
	void **ptrval
);
```

## Function afb\_req\_context\_set

```C
/**
 * Stores for the binding the pointer 'value' to the session of 'req'.
 * The function 'freecb' will be called with the given 'freeclo' when the
 * session is closed or if binding stores an other value.
 *
 * @param req the request
 * @param value the context value to store
 * @param freecb the cleaning function for the stored context (can be NULL)
 * @param freeclo the closure for the cleaning function 'freecb' (can be 'context')
 *
 * @return 0 in case of success or a negative error code
 */
int
afb_req_context_set(
	afb_req_t req,
	void *value,
	void (*freecb)(void*),
	void *freeclo
);
```

## Function afb\_req\_context\_drop

```C
/**
 * Frees the pointer stored by the binding for the session of 'req'
 * and sets it to NULL.
 *
 * @param req the request
 *
 * @return 0 in case of success or a negative error code
 */
int
afb_req_context_drop(
	afb_req_t req
);
```

## Function afb\_req\_session\_close

```C
/**
 * Closes the session associated with 'req'
 * and delete all associated contexts.
 *
 * @param req the request
 */
void
afb_req_session_close(
	afb_req_t req
);
```

## Function afb\_req\_session\_set\_LOA

```C
/**
 * Sets the level of assurance of the session of 'req'
 * to 'level'. The effect of this function is subject of
 * security policies.
 *
 * @param req the request
 * @param level of assurance from 0 to 7
 *
 * @return 0 on success or -1 if failed.
 */
int
afb_req_session_set_LOA(
	afb_req_t req,
	unsigned level
);
```

## Function afb\_req\_session\_get\_LOA

```C
/**
 * Gets the current level of assurance of the session of 'req'.
 *
 * @param req the request
 *
 * @return the current LOA, a value of 0 to 7
 */
unsigned
afb_req_session_get_LOA(
	afb_req_t req
);
```

## Function afb\_req\_subscribe

```C
/**
 * Establishes for the client link identified by 'req' a subscription
 * to the 'event'.
 *
 * Establishing subscription MUST be called BEFORE replying to the request.
 *
 * @param req the request
 * @param event the event to subscribe
 *
 * @return 0 in case of successful subscription or -1 in case of error.
 */
int
afb_req_subscribe(
	afb_req_t req,
	afb_event_t event
);
```

## Function afb\_req\_unsubscribe

```C
/**
 * Revokes the subscription established to the 'event' for the client
 * link identified by 'req'.
 * Returns 0 in case of successful subscription or -1 in case of error.
 *
 * Revoking subscription MUST be called BEFORE replying to the request.
 *
 * @param req the request
 * @param event the event to revoke
 *
 * @return 0 in case of successful subscription or -1 in case of error.
 */
int
afb_req_unsubscribe(
	afb_req_t req,
	afb_event_t event
);
```

## Function afb\_req\_check\_permission

```C
/**
 * Check whether the 'permission' is granted or not to the client
 * identified by 'req'.
 *
 * @param req the request
 * @param permission string to check
 * @param callback that receive the result of the check
 * @param closure closure for the callback
 */
void
afb_req_check_permission(
	afb_req_t req,
	const char *permission,
	afb_check_callback_t callback,
	void *closure
);
```

## Function afb\_req\_get\_client\_info

```C
/**
 * Get informations about the client of the
 * request 'req'.
 *
 * Returns an object with client informations:
 *  {
 *    "pid": int, "uid": int, "gid": int,
 *    "label": string, "id": string, "user": string,
 *    "uuid": string, "LOA": int
 *  }
 *
 * If some of this information can't be computed, the field of the return
 * object will not be set at all.
 *
 * @param req the request
 *
 * @return a JSON object that must be freed using @ref json_object_put
 */
struct json_object *
afb_req_get_client_info(
	afb_req_t req
);
```

## Function afb\_req\_parameters

```C
/**
 * Get the parameters of the request
 *
 * @param req      the request
 * @param params   pointer where to store the address to the array of data parameters
 *                 (can be NULL)
 *
 * @return the count of parameters
 */
unsigned
afb_req_parameters(
	afb_req_t req,
	afb_data_t const **params
);
```

## Function afb\_req\_reply

```C
/**
 * Reply to the request
 *
 * NOTE: For convenience, the function calls 'afb_data_unref' for items of
 * 'replies'. Thus, in the case where some data of 'replies' should remain
 * available after the function returns, the function 'afb_data_addref'
 * shall be used on such items before the call.
 *
 * @param req      the request
 * @param status   an integer status
 * @param nreplies count of replied data
 * @param replies  array of the replied data if any (can be NULL when nparams is 0)
 */
void
afb_req_reply(
	afb_req_t req,
	int status,
	unsigned nreplies,
	afb_data_t const *replies
);
```

## Function afb\_req\_subcall

```C
/**
 * Calls the 'verb' of the 'api' with the parameters 'params' and 'verb' in the name of the binding.
 * The result of the call is delivered to the 'callback' function with the 'callback_closure'.
 *
 * NOTE: For convenience, the function calls 'afb_data_unref' for items of
 * 'params'. Thus, in the case where some data of 'params' should remain
 * available after the function returns, the function 'afb_data_addref'
 * shall be used on such items before the call.
 *
 * The 'callback' receives 5 arguments:
 *  1. 'closure'  the user defined closure pointer 'closure',
 *  2. 'status'   the replied status
 *  3. 'nreplies' the count of replied data
 *  4. 'replies'  the array of replied data
 *  5. 'req'      the req
 *
 * NOTE: For convenience, the data received by the callback function in the
 * array 'replies' are released automatically after the return of the
 * callback. Thus, in the case where some data of 'replies' should remain
 * available after the end of the callback, the function 'afb_data_addref'
 * shall be used on such items to keep them alive after end of callback.
 *
 * @param req      The request
 * @param apiname  The api name of the method to call
 * @param verbname The verb name of the method to call
 * @param nparams  count of parameter data
 * @param params   array of parameter data (can be NULL when nparams is zero)
 * @param flags    The bit field of flags for the subcall as defined by @ref afb_req_subcall_flags
 * @param callback The to call on completion
 * @param closure  The closure to pass to the callback
 *
 * @see also 'afb_req_subcall_sync'
 */
void
afb_req_subcall(
	afb_req_t req,
	const char *apiname,
	const char *verbname,
	unsigned nparams,
	afb_data_t const params[],
	int flags,
	afb_subcall_callback_t callback,
	void *closure
);
```

## Function afb\_req\_subcall\_sync

```C
/**
 * Makes a call to the method of name 'api' / 'verb' with the parameters 'params'.
 * This call is made in the context of the request 'req'.
 * This call is synchronous, it waits untill completion of the request.
 * It returns 0 on success or a negative value on error answer.
 *
 * At call, the value pointed by nreplies must be the count of data
 * of the array replies. The values in replies are not used at call and
 * can be anything of anyvalue, the caller does not care or use it.
 *
 * NOTE: For convenience, the function calls 'afb_data_unref' for items of
 * 'params'. Thus, in the case where some data of 'params' should remain
 * available after the function returns, the function 'afb_data_addref'
 * shall be used on such items before the call.
 *
 * At return, status receives the reply status, nreplies receives the count
 * of value replied and stored in the array replies.
 *
 * NOTE: The data returned by the call in the array replies MUST be released
 * by the caller using 'afb_data_unref' or 'afb_data_array_unref'.
 *
 * If the array 'replies' is too small to receive all the replied data, the
 * reply is truncated to the count of 'replies' (indicated by 'nreplies'),
 * the remaining data replied are ignored.
 *
 * @param req      The request
 * @param apiname  The api name of the method to call
 * @param verbname The verb name of the method to call
 * @param nparams  count of parameter data
 * @param params   array of parameter data (can be NULL when nparams is zero)
 * @param flags    The bit field of flags for the subcall as defined by @ref afb_req_subcall_flags
 * @param status   the replied status of the call
 * @param nreplies at call, size of params, at return, count of data in replies
 * @param replies  where to store the replied data
 *
 * @return 0 in case of success or -1 in case of error
 */
int
afb_req_subcall_sync(
	afb_req_t req,
	const char *apiname,
	const char *verbname,
	unsigned nparams,
	afb_data_t const params[],
	int flags,
	int *status,
	unsigned *nreplies,
	afb_data_t replies[]
);
```
