Macro for logging
=================

By default, the logging macros emit the given messages
with its formatted parameters and add the file path, the line
number within the file and the function name locating
from where the macro comes.

However, the final behaviour of macros can be tuned
using 2 preprocessor defines that can be defined
before including **<afb/afb-binding.h>**:

- **AFB_BINDING_PRAGMA_NO_VERBOSE_DATA**:
  if defined, only filename and line are shown
- **AFB_BINDING_PRAGMA_NO_VERBOSE_DETAILS**:
  if defined, only the message and its formatted arguments are shown


## Logging for an api

The following macros must be used for logging for an **api** of type
**afb_api_t**.

```C
AFB_API_ERROR(api,fmt,...)
AFB_API_WARNING(api,fmt,...)
AFB_API_NOTICE(api,fmt,...)
AFB_API_INFO(api,fmt,...)
AFB_API_DEBUG(api,fmt,...)
```

## Logging for a request


The following macros can be used for logging in the context
of a request **req** of type **afb_req_t**:

```C
AFB_REQ_ERROR(req,fmt,...)
AFB_REQ_WARNING(req,fmt,...)
AFB_REQ_NOTICE(req,fmt,...)
AFB_REQ_INFO(req,fmt,...)
AFB_REQ_DEBUG(req,fmt,...)
```

## Logging globally

The following macros are provided for logging globally.

```C
AFB_ERROR(fmt,...)
AFB_WARNING(fmt,...)
AFB_NOTICE(fmt,...)
AFB_INFO(fmt,...)
AFB_DEBUG(fmt,...)
```

