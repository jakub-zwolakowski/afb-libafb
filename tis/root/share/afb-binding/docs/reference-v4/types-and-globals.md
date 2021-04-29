# Types and globals

## The global afbBindingRoot

The global **afbBindingRoot** of type **afb\_api\_t** is always implicitly
defined for bindings of version 3 or upper. It records the root api of
the binding.

When the binding has a defined **afbBindingExport**,  the root api
**afbBindingRoot** is the **afb\_pi\_t** relative to the api created for
this static description.

When the binding has no defined **afbBindingExport**, the root api is
a virtual api representing the shared object of the binding. In that case
the name of the api is the path of the shared object. Its use is restricted
but allows log messages.

## The global afbBindingExport

The global **afbBindingExport** is not mandatory.

If **afbBindingExport** is defined and exported, it must be of the type
**const afb\_binding\_t** and must describe the *root* api of the binding.

## The type afb\_api\_t

Bindings now can declare more than one api. The counter part is that
a new handle is needed to manage apis. These handles are of the type
**afb\_api\_t**.

## The type afb\_binding\_t

The main structure, of type **afb\_binding\_t**, for describing the binding
must be exported under the name **afbBindingExport**.

This structure is defined as below.

```C
typedef struct afb_binding_v4 afb_binding_t;
```

Where:

```C
/**
 * Description of the bindings of type version 4
 */
struct afb_binding_v4
{
	/** api name for the binding, can't be NULL */
	const char *api;

	/** textual specification of the binding, can be NULL */
	const char *specification;

	/** some info about the api, can be NULL */
	const char *info;

	/** array of descriptions of verbs terminated by a NULL name, can be NULL */
	const struct afb_verb_v4 *verbs;

	/** callback at load of the binding */
	afb_api_callback_x4_t mainctl;

	/** userdata for afb_api_ */
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
```

## The type afb\_verb\_t

Each verb is described with a structure of type **afb\_verb\_t**
defined below:

```C
typedef struct afb_verb_v4 afb_verb_t;
```

```C
/**
 * Description of one verb as provided for binding API version 4
 */
struct afb_verb_v4
{
	/** name of the verb, NULL only at end of the array */
	const char *verb;

	/** callback function implementing the verb */
	afb_req_callback_x4_t callback;

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
```

The **session** flags is one of the constant defined below:

| Name                   | Description
|:----------------------:|------------------------------------------------------
| AFB\_SESSION\_NONE       | no flag, synonym to 0
| AFB\_SESSION\_LOA\_0      | Requires the LOA to be 0 or more, synonym to 0 or AFB\_SESSION\_NONE
| AFB\_SESSION\_LOA\_1      | Requires the LOA to be 1 or more
| AFB\_SESSION\_LOA\_2      | Requires the LOA to be 2 or more
| AFB\_SESSION\_LOA\_3      | Requires the LOA to be 3 or more
| AFB\_SESSION\_CHECK      | Requires the token to be set and valid
| AFB\_SESSION\_CLOSE      | Implies closing the session after request processed

The LOA (Level Of Assurance) is set, by binding api,
using the function **afb\_req\_session\_set\_LOA**.
It can be check using the function **afb\_req\_session\_get\_LOA**.

The session can be closed, by binding api, using the function
 **afb\_req\_session\_close**.

## The types afb\_auth\_t and afb\_auth\_type\_t

The structure **afb\_auth\_t** is used within verb description to
set security requirements.
The interpretation of the structure depends on the value of the field **type**.

```C
typedef struct afb_auth afb_auth_t;

/**
 * Definition of an authorization entry
 */
struct afb_auth
{
	/** type of entry @see afb_auth_type */
	enum afb_auth_type type;

	union {
		/** text when @ref type == @ref afb_auth_Permission */
		const char *text;

		/** level of assurancy when @ref type ==  @ref afb_auth_LOA */
		unsigned loa;

		/** first child when @ref type in { @ref afb_auth_Or, @ref afb_auth_And, @ref afb_auth_Not } */
		const struct afb_auth *first;
	};

	/** second child when @ref type in { @ref afb_auth_Or, @ref afb_auth_And } */
	const struct afb_auth *next;
};

```

The possible values for **type** is defined here:

```C
typedef enum afb_auth_type afb_auth_type_t;

/**
 * Enumeration  for authority (Session/Token/Assurance) definitions.
 *
 * @see afb_auth
 */
enum afb_auth_type
{
	/** never authorized, no data */
	afb_auth_No = 0,

	/** authorized if token valid, no data */
	afb_auth_Token,

	/** authorized if LOA greater than or equal to data 'loa' */
	afb_auth_LOA,

	/** authorized if permission 'text' is granted */
	afb_auth_Permission,

	/** authorized if 'first' or 'next' is authorized */
	afb_auth_Or,

	/** authorized if 'first' and 'next' are authorized */
	afb_auth_And,

	/** authorized if 'first' is not authorized */
	afb_auth_Not,

	/** always authorized, no data */
	afb_auth_Yes
};
```

Example:

```C
static const afb_auth_t myauth[] = {
    { .type = afb_auth_Permission, .text = "urn:AGL:permission:me:public:set" },
    { .type = afb_auth_Permission, .text = "urn:AGL:permission:me:public:get" },
    { .type = afb_auth_Or, .first = &myauth[1], .next = &myauth[0] }
};
```

## The type afb\_req\_subcall\_flags\_t

This is an enumeration that defines bit's positions for setting behaviour
of subcalls.

| flag                       | value | description
|----------------------------|-------|--------------
| afb\_req\_subcall\_catch\_events | 1 | the calling API wants to receive the events from subscription
| afb\_req\_subcall\_pass\_events  | 2 | the original request will receive the events from subscription
| afb\_req\_subcall\_on\_behalf    | 4 | the calling API wants to use the credentials of the original request
| afb\_req\_subcall\_api\_session  | 8 | the calling API wants to use its session instead of the one of the original request

## The type afb\_ctlid\_t

The enumeration **afb\_ctlid\_t** is used in control functions of
APIs to identify control message received.

It's defined as:

```C
typedef enum afb_ctlid afb_ctlid_t;

enum afb_ctlid
{
	/** called on root entries, the entries not linked to an API */
	afb_ctlid_Root_Entry = 0,

	/** called for the preinit of an API, has a companion argument */
	afb_ctlid_Pre_Init = 1,

	/** called for init */
	afb_ctlid_Init = 2,

	/** called when required classes are ready */
	afb_ctlid_Class_Ready = 3,

	/** called when an event is not handled */
	afb_ctlid_Orphan_Event = 4,

	/** called when shuting down */
	afb_ctlid_Exiting = 5
};
```

## The type afb\_ctlarg\_t

The union **afb\_ctlarg\_t** wrap the argument of the control message.
Its real value depends on a value of type afb\_ctlid\_t. That type is
used in API's control functions.

```C
typedef const union afb_ctlarg *afb_ctlarg_t;

union afb_ctlarg
{
	/**
	 * structure for message afb_ctlid_Root_Entry
	 */
	struct {
		/** path of the binding */
		const char *path;
	} root_entry;

	/**
	 * structure for message afb_ctlid_Pre_Init
	 */
	struct {
		/** path of the binding */
		const char *path;
	} pre_init;

	/**
	 * structure for message afb_ctlid_Orphan_Event
	 */
	struct {
		/** name of the of the orphan event */
		const char *name;
	} orphan_event;

	/**
	 * structure for message afb_ctlid_Exiting
	 */
	struct {
		/** exiting code */
		int code;
	} exiting;
};
```

## The type afb\_api\_callback\_t

```C
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
typedef int (*afb_api_callback_t)(
		afb_api_t api,
		afb_ctlid_t ctlid,
		afb_ctlarg_t ctlarg,
		void *userdata);
```

## The type afb\_req\_callback\_t

```C
/**
 * Request handling callback used for verbs.
 *
 * @param req      the request to process
 * @param nparams  the count of parameter data of the request
 * @param params   the array of parameter data of the request
 */
typedef void (*afb_req_callback_t)(
		afb_req_t req,
		unsigned nparams,
		afb_data_t const params[]);
```

## The type afb\_call\_callback\_t

```C
/**
 * Callback for receiving asynchronous results of a call.
 *
 * @param closure  the closure value given at call
 * @param status   the status of the call, negative on error
 * @param nreplies the count of replied data
 * @param replies  the array of replied data
 * @param api      the api that made the call
 */
typedef void (*afb_call_callback_t)(
		void *closure,
		int status,
		unsigned nreplies,
		afb_data_t const replies[],
		afb_api_t api);
```

## The type afb\_subcall\_callback\_t

```C
/**
 * Callback for receiving asynchronous results of a subcall.
 *
 * @param closure  the closure value given at subcall
 * @param status   the status of the subcall, negative on error
 * @param nreplies the count of replied data
 * @param replies  the array of replied data
 * @param req      the req that made the subcall
 */
typedef void (*afb_subcall_callback_t)(
		void *closure,
		int status,
		unsigned nreplies,
		afb_data_t const replies[],
		afb_req_t req);
```

## The type afb\_check\_callback\_t

```C
/**
 * Callback for receiving result of checking of permissions.
 *
 * @param closure  the closure value given at check
 * @param status   the status of the check, negative when rejected
 * @param req      the req that made the check
 */
typedef void (*afb_check_callback_t)(
		void *closure,
		int status,
		afb_req_t req);
```

## The type afb\_event\_handler\_t

```C
/**
 * Callback for handling events.
 *
 * @param closure  the closure value given at creation of the handler
 * @param event_name name of the event
 * @param nparams  the count of parameter data of the event
 * @param params   the array of parameter data of the event
 * @param api      the api that holds the handler
 */
typedef void (*afb_event_handler_t)(
		void *closure,
		const char *event_name,
		unsigned nparams,
		afb_data_t const params[],
		afb_api_t api);
```

## The type afb\_type\_converter\_t

```C
/**
 * The type afb_type_converter_t denote a conversion callback.
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
typedef int (*afb_type_converter_t)(
		void *closure,
		afb_data_t from,
		afb_type_t type,
		afb_data_t *to);
```

## The type afb\_type\_updater\_t

```C
/**
 * The type afb_type_updater_t denote a conversion callback that is able
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
typedef int (*afb_type_updater_t)(
		void *closure,
		afb_data_t from,
		afb_type_t type,
		afb_data_t to);
```

## The type afb\_evfd\_handler\_t

```C
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
typedef void (*afb_evfd_handler_t)(
		afb_evfd_t efd,
		int fd,
		uint32_t revents,
		void *closure);
```

## The type afb\_timer\_handler\_t

```C
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
typedef void (*afb_timer_handler_t)(
		afb_timer_t timer,
		void *closure,
		int decount);
```
