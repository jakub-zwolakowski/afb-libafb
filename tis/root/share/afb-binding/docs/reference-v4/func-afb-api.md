
# Functions of class **afb\_api**


## Function afb\_api\_logmask

```C
/**
 * Get the logmask of the 'api'.
 *
 * @param api the api whose logmask is to be returned
 *
 * @return logmask of the api.
 */
int
afb_api_logmask(
	afb_api_t api
);
```

## Function afb\_api\_name

```C
/**
 * Get the name of the 'api'.
 *
 * @param api the api whose name is to be returned
 *
 * @return the name of the api.
 *
 * The returned value must not be changed nor freed.
 */
const char *
afb_api_name(
	afb_api_t api
);
```

## Function afb\_api\_get\_userdata

```C
/**
 * Get the "userdata" pointer of the 'api'
 *
 * @param api the api whose user's data is to be returned
 *
 * @return the user's data  pointer of the api.
 *
 * @see afb_api_set_userdata
 */
void *
afb_api_get_userdata(
	afb_api_t api
);
```

## Function afb\_api\_set\_userdata

```C
/**
 * Set the "userdata" pointer of the 'api' to 'value'
 *
 * @param api   the api whose user's data is to be set
 * @param value the data to set
 *
 * @return the previous value
 *
 * @see afb_api_get_userdata
 */
void *
afb_api_set_userdata(
	afb_api_t api,
	void *value
);
```

## Function afb\_api\_wants\_log\_level

```C
/**
 * Is the log message of 'level (as defined for syslog) required for the api?
 *
 * @param api   the api to check
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
afb_api_wants_log_level(
	afb_api_t api,
	int level
);
```

## Function afb\_api\_vverbose

```C
/**
 * Send to the journal with the logging 'level' a message described
 * by 'fmt' applied to the va-list 'args'.
 *
 * 'file', 'line' and 'func' are indicators of code position in source files
 * (see macros __FILE__, __LINE__ and __func__).
 *
 * 'level' is defined by syslog standard:
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
 * @param api the api that collects the logging message
 * @param level the level of the message
 * @param file the source file that logs the messages or NULL
 * @param line the line in the source file that logs the message
 * @param func the name of the function in the source file that logs (or NULL)
 * @param fmt the format of the message as in printf
 * @param args the arguments to the format string of the message as a va_list
 *
 * @see syslog
 * @see printf
 */
void
afb_api_vverbose(
	afb_api_t api,
	int level,
	const char *file,
	int line,
	const char *func,
	const char *fmt,
	va_list args
);
```

## Function afb\_api\_verbose

```C
/**
 * Send to the journal with the log 'level' a message described
 * by 'fmt' and following parameters.
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
 * @param api the api that collects the logging message
 * @param level the level of the message
 * @param file the source file that logs the messages or NULL
 * @param line the line in the source file that logs the message
 * @param func the name of the function in the source file that logs (or NULL)
 * @param fmt the format of the message as in printf
 * @param ... the arguments to the format string of the message
 *
 * @see syslog
 * @see printf
 */
void
afb_api_verbose(
	afb_api_t api,
	int level,
	const char *file,
	int line,
	const char *func,
	const char *fmt,
	...
);
```

## Function afb\_api\_broadcast\_event

```C
/**
 * Broadcasts widely the event of 'name' with the data 'object'.
 * 'object' can be NULL.
 *
 * NOTE: For convenience, the function calls 'afb_data_unref' for 'params'.
 * Thus, in the case where 'params' should remain available after
 * the function returns, the function 'afb_data_addref' shall be used.
 *
 * Calling this function is forbidden during preinit.
 *
 * The event sent as the name API/name where API is the name of the
 * api.
 *
 * @param api      the api that broadcast the event
 * @param name     the event name suffix
 * @param nparams  count of parameter data
 * @param params   array of parameter data (can be NULL when nparams is zero)
 *
 * @return the count of clients that received the event.
 */
int
afb_api_broadcast_event(
	afb_api_t api,
	const char *name,
	unsigned nparams,
	afb_data_t const params[]
);
```

## Function afb\_api\_require\_api

```C
/**
 * Check that it requires the API of 'name'.
 * If 'initialized' is not zero it request the API to be
 * initialized, implying its initialization if needed.
 *
 * Calling this function is only allowed within init.
 *
 * A single request allows to require multiple apis.
 *
 * @param api the api that requires the other api by its name
 * @param name a space separated list of required api names
 * @param initialized if zero, the api is just required to exist. If not zero,
 * the api is required to exist and to be initialized at return of the call
 * (initializing it if needed and possible as a side effect of the call).
 *
 * @return 0 in case of success or -1 in case of error with errno set appropriately.
 */
int
afb_api_require_api(
	afb_api_t api,
	const char *name,
	int initialized
);
```

## Function afb\_api\_new\_event

```C
/**
 * Creates an event of 'name' and returns it.
 *
 * Calling this function is only forbidden during preinit.
 *
 * See afb_event_is_valid to check if there is an error.
 *
 * The event created as the name API/name where API is the name of the
 * api.
 *
 * @param api the api that creates the event
 * @param name the event name suffix
 *
 * @return the created event. Use the function afb_event_is_valid to check
 * whether the event is valid (created) or not (error as reported by errno).
 *
 * @see afb_event_is_valid
 */
int
afb_api_new_event(
	afb_api_t api,
	const char *name,
	afb_event_t *event
);
```

## Function afb\_api\_add\_verb

```C
/**
 * Add one verb to the dynamic set of the api
 *
 * @param api the api to change
 * @param verb name of the verb
 * @param info brief description of the verb, can be NULL
 * @param callback callback function implementing the verb
 * @param vcbdata data for the verb callback, available through req
 * @param auth required authorization, can be NULL
 * @param session authorization and session requirements of the verb
 * @param glob is the verb glob name
 *
 * @return 0 in case of success or -1 on failure with errno set
 *
 * @see afb_verb_v3
 * @see afb_api_del_verb
 * @see afb_api_set_verbs_v3
 * @see fnmatch for matching names using glob
 */
int
afb_api_add_verb(
	afb_api_t api,
	const char *verb,
	const char *info,
	afb_req_callback_t callback,
	void *vcbdata,
	const struct afb_auth *auth,
	uint32_t session,
	int glob
);
```

## Function afb\_api\_del\_verb

```C
/**
 * Delete one verb from the dynamic set of the api
 *
 * @param api the api to change
 * @param verb name of the verb to delete
 * @param vcbdata if not NULL will receive the vcbdata of the deleted verb
 *
 * @return 0 in case of success or -1 on failure with errno set
 *
 * @see afb_api_add_verb
 */
int
afb_api_del_verb(
	afb_api_t api,
	const char *verb,
	void **vcbdata
);
```

## Function afb\_api\_seal

```C
/**
 * Seal the api. After a call to this function the api can not be modified
 * anymore.
 *
 * @param api the api to be sealed
 */
void
afb_api_seal(
	afb_api_t api
);
```

## Function afb\_api\_set\_verbs

```C
/**
 * Set the verbs of the 'api' using description of verbs of the api v2
 *
 * @param api the api that will get the verbs
 * @param verbs the array of verbs to add terminated with an item with name=NULL
 *
 * @return 0 in case of success or -1 on failure with errno set
 *
 * @see afb_verb_v4
 * @see afb_api_add_verb
 * @see afb_api_del_verb
 */
int
afb_api_set_verbs(
	afb_api_t api,
	const struct afb_verb_v4 *verbs
);
```

## Function afb\_api\_event\_handler\_add

```C
/**
 * Add a specific event handler for the api
 *
 * The handler callback is called when an event matching the given pattern
 * is received (it is received if broadcasted or after subscription through
 * a call or a subcall).
 *
 * The handler callback receive 4 arguments:
 *
 *  - the closure given here
 *  - the event full name
 *  - the companion JSON object of the event
 *  - the api that subscribed the event
 *
 * @param api the api that creates the handler
 * @param pattern the global pattern of the event to handle
 * @param callback the handler callback function
 * @param closure the closure of the handler
 *
 * @return 0 in case of success or -1 on failure with errno set
 *
 * @see afb_api_on_event
 * @see afb_api_event_handler_del
 */
int
afb_api_event_handler_add(
	afb_api_t api,
	const char *pattern,
	afb_event_handler_t callback,
	void *closure
);
```

## Function afb\_api\_event\_handler\_del

```C
/**
 * Delete a specific event handler for the api
 *
 * @param api the api that the handler belongs to
 * @param pattern the global pattern of the handler to remove
 * @param closure if not NULL it will receive the closure set to the handler
 *
 * @return 0 in case of success or -1 on failure with errno set
 *
 * @see afb_api_on_event
 * @see afb_api_event_handler_add
 */
int
afb_api_event_handler_del(
	afb_api_t api,
	const char *pattern,
	void **closure
);
```

## Function afb\_api\_call

```C
/**
 * Calls the 'verb' of the 'api' with the arguments 'args' and 'verb' in the name of the binding.
 * The result of the call is delivered to the 'callback' function with the 'callback_closure'.
 *
 * NOTE: For convenience, the function calls 'afb_data_unref' for items of
 * 'params'. Thus, in the case where some data of 'params' should remain
 * available after the function returns, the function 'afb_data_addref'
 * shall be used on such items before the call.
 *
 * The 'callback' receives 5 arguments:
 *  1. 'closure' the user defined closure pointer 'closure',
 *  2. 'status'   the replied status
 *  3. 'nreplies' the count of replied data
 *  4. 'replies'  the array of replied data
 *  5. 'api'      the api
 *
 * NOTE: For convenience, the data received by the callback function in the
 * array 'replies' are released automatically after the return of the
 * callback. Thus, in the case where some data of 'replies' should remain
 * available after the end of the callback, the function 'afb_data_addref'
 * shall be used on such items to keep them alive after end of callback.
 *
 * @param api      The api that makes the call
 * @param apiname  The api name of the method to call
 * @param verbname The verb name of the method to call
 * @param nparams  count of parameter data
 * @param params   array of parameter data (can be NULL when nparams is zero)
 * @param callback The to call on completion
 * @param closure  The closure to pass to the callback
 *
 * @see afb_req_subcall
 * @see afb_req_subcall_sync
 * @see afb_api_call_sync
 */
void
afb_api_call(
	afb_api_t api,
	const char *apiname,
	const char *verbname,
	unsigned nparams,
	afb_data_t const params[],
	afb_call_callback_t callback,
	void *closure
);
```

## Function afb\_api\_call\_sync

```C
/**
 * Calls the 'verb' of the 'api' with the arguments 'args' and 'verb' in the name of the binding.
 * 'result' will receive the response.
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
 * NOTE: The data returned by the call in replies MUST be released by the caller
 * using 'afb_data_unref'.
 *
 * If the array 'replies' is too small to receive all the replied data, the
 * reply is truncated to the count of 'replies' (indicated by 'nreplies'),
 * the remaining data replied are ignored.
 *
 * @param api      The api that makes the call
 * @param apiname  The api name of the method to call
 * @param verbname The verb name of the method to call
 * @param nparams  count of parameter data
 * @param params   array of parameter data (can be NULL when nparams is zero)
 * @param status   the replied status of the call
 * @param nreplies at call, size of params, at return, count of data in replies
 * @param replies  where to store the replied data
 *
 * @returns 0 in case of success or a negative value in case of error.
 *
 * @see afb_req_subcall
 * @see afb_req_subcall_sync
 * @see afb_api_call
 */
int
afb_api_call_sync(
	afb_api_t api,
	const char *apiname,
	const char *verbname,
	unsigned nparams,
	afb_data_t const params[],
	int *status,
	unsigned *nreplies,
	afb_data_t replies[]
);
```

## Function afb\_api\_provide\_class

```C
/**
 * Tells that the api provides a class of features. Classes are intended to
 * allow ordering of initializations: apis that provides a given class are
 * initialized before apis requiring it.
 *
 * This function is only valid during the pre-initialization stage.
 *
 * @param api  the api that provides the classes
 * @param name a space separated list of the names of the provided classes
 *
 * @returns 0 in case of success or a negative value in case of error.
 *
 * @see afb_api_require_class
 */
int
afb_api_provide_class(
	afb_api_t api,
	const char *name
);
```

## Function afb\_api\_require\_class

```C
/**
 * Tells that the api requires a set of class features. Classes are intended to
 * allow ordering of initializations: apis that provides a given class are
 * initialized before apis requiring it.
 *
 * This function is only valid during the pre-initialization stage.
 *
 * @param api  the api that requires the classes
 * @param name a space separated list of the names of the required classes
 *
 * @returns 0 in case of success or a negative value in case of error.
 *
 * @see afb_api_provide_class
 */
int
afb_api_require_class(
	afb_api_t api,
	const char *name
);
```

## Function afb\_api\_delete

```C
/**
 * Delete the given api.
 *
 * It is of the responsibility of the caller to don't used the deleted api
 * anymore after this function returned.
 *
 * @param api the api to delete
 *
 * @returns 0 in case of success or a negative value in case of error.
 *
 * @see afb_api_new_api
 */
int
afb_api_delete(
	afb_api_t api
);
```

## Function afb\_api\_settings

```C
/**
 * Settings of the api.
 *
 * Get the settings of the API. The settings are recorded
 * as a JSON object. The returned object should not be modified.
 * It MUST NOT be released using json_object_put.
 *
 * @param api the api whose settings are required
 *
 * @returns The setting object.
 */
struct json_object *
afb_api_settings(
	afb_api_t api
);
```
