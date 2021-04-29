
# Functions of class **afb\_misc**


## Function afb\_create\_api

```C
/** MISC ***********************************************************/

/**
 * Creates a new api of name 'apiname' briefly described by 'info' (that can
 * be NULL).
 *
 * When the pre-initialization function is given, it is a function that
 * receives 2 parameters:
 *
 *  - the closure as given in the call
 *  - the created api that can be initialised
 *
 * This pre-initialization function must return a negative value to abort
 * the creation of the api. Otherwise, it returns a non-negative value to
 * continue.
 *
 * @param api the api that creates the other one
 * @param apiname the name of the new api
 * @param info the brief description of the new api (can be NULL)
 * @param noconcurrency zero or not zero whether the new api is reentrant or not
 * @param mainctl the main api callback (if any, can be NULL)
 * @param userdata the initial value of userdata
 *
 * @return the created api in case of success or NULL on error
 *
 * @see afb_api_delete_api
 */
int
afb_create_api(
	afb_api_t *newapi,
	const char *apiname,
	const char *info,
	int noconcurrency,
	afb_api_callback_t mainctl,
	void *userdata
);
```

## Function afb\_job\_post

```C
/**
 * Queue the job defined by 'callback' and 'argument' for being executed asynchronously
 * in this thread (later) or in an other thread.
 *
 * If 'delay' is not 0, it represent the minimal delay in miliseconds before the start
 * of the job.
 *
 * If 'timeout' is not 0, it represent the maximum execution time for the job in seconds.
 * At first, the job is called with 0 as signum and the given argument.
 *
 * If 'group' is not NULL, the jobs queued with a same value (as the pointer value 'group')
 * are executed in sequence in the order of there submission.
 *
 * The job is executed with the monitoring of its time and some signals like SIGSEGV and
 * SIGFPE. When a such signal is catched, the job is terminated and reexecuted but with
 * signum being the signal number (SIGALRM when timeout expired).
 *
 * When executed, the callback function receives 2 arguments:
 *
 *  - int signum: the signal catched if any or zero at the beginning
 *  - void *arg: the parameter 'argument'
 *
 * A typical implementation of the job callback is:
 *
 * ```C
 * void my_job_cb(int signum, void *arg)
 * {
 *	struct myarg_t *myarg = arg;
 *	if (signum)
 *		AFB_API_ERROR(myarg->api, "job interrupted with signal %s", strsignal(signum));
 *	else
 *		really_do_my_job(myarg);
 * }
 * ```
 *
 * @param delayms the minimal delay (in milliseconds) before starting the job
 * @param timeout the timeout (in seconds) of execution of the job
 * @param callback the job as a callback function
 * @param argument the argument to pass to the queued job
 * @param group the group of the job, NULL if no group
 *
 * @return 0 in case of success or -1 in case of error with errno set appropriately.
 */
int
afb_job_post(
	long delayms,
	int timeout,
	void (*callback)(int signum, void *arg),
	void *argument,
	void *group
);
```

## Function afb\_alias\_api

```C
/**
 * Create an aliased name 'as_name' for the api 'name'.
 * Calling this function is only allowed within preinit.
 *
 * @param name the api to alias
 * @param as_name the aliased name
 *
 * @return 0 in case of success or -1 in case of error with errno set appropriately.
 */
int
afb_alias_api(
	const char *name,
	const char *as_name
);
```

## Function afb\_setup\_shared\_object

```C
/**
 * Setup the given shared object.
 *
 * This function allows bindings to load shared object that use the binder
 * functions.
 *
 * @param api the api that requires to setup the shared object can be null
 * @param handle the handle of the shared object (as returned by dlopen)
 *
 * @return 0 in case of success or -1 in case of error with errno set appropriately.
 */
int
afb_setup_shared_object(
	afb_api_t api,
	void *handle
);
```
