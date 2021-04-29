# Tutorial of bindings version 3

## Sample binding: tuto-1

This is the code of the binding **tutorials/v3/tuto-1.c**:

```C
  1 #define AFB_BINDING_VERSION 3
  2 #include <afb/afb-binding.h>
  3
  4 void hello(afb_req_t req)
  5 {
  6         AFB_REQ_DEBUG(req, "hello world");
  7         afb_req_reply(req, NULL, NULL, "hello world");
  8 }
  9
 10 const afb_verb_t verbs[] = {
 11         { .verb="hello", .callback=hello },
 12         { .verb=NULL }
 13 };
 14
 15 const afb_binding_t afbBindingExport = {
 16         .api = "tuto-1",
 17         .verbs = verbs
 18 };
```

Compiling:

```bash
gcc -fPIC -shared tuto-1.c -o tuto-1.so $(pkg-config --cflags-only-I afb-binding)
```

> Note: the variable environment variable PKG_CONFIG_PATH might be necessary
> tuned to get **pkg-config** working properly

Running it (with the binder afb-binder):

```bash
afb-binder --binding ./tuto-1.so --port 3333
```

At this point, afb-binder has started, it loaded the binding tuto-1.so and now
listen at localhost on the port 3333.

Testing using **curl**:

```bash
$ curl -i http://localhost:3333/api/tuto-1/hello
HTTP/1.1 200 OK
Connection: close
Content-Length: 89
Set-Cookie: x-afb-uuid-3333=273a0eae-7248-48b2-beab-3fc37fc930fd; Path=/api; Max-Age=32000000; HttpOnly
Date: Tue, 24 Nov 2020 13:09:15 GMT

{"jtype":"afb-reply","request":{"status":"success","info":"hello world"},"response":null}
```

Testing using **afb-client** (with option -H for
getting a human readable output):

```bash
$ afb-client -H ws://localhost:3333/api tuto-1 hello
ON-REPLY 1:tuto-1/hello: OK
{
  "jtype":"afb-reply",
  "request":{
    "status":"success",
    "info":"hello world"
  }
}
```

This shows basic things:

- The include to get for creating a binding
- How to declare the API offered by the binding
- How to handle requests made to the binding

### Getting declarations for the binding

The lines 1 and 2 show how to get the include file **afb-binding.h**.

```C
  1 #define AFB_BINDING_VERSION 3
  2 #include <afb/afb-binding.h>
```

You must define the version of ***binding*** that you are using.
This is done line 1 where we define that this is the version 3.

If you don't define it, an error is reported and the compilation aborts.
Note that this define is sometime done outside of the code but using
the option -D of C compilers/preprocessors.

To include **afb-binding.h** successfully, the include search path
should be set correctly if needed (not needed only if installed in
/usr/include/afb directory that is the default).

Setting the include path is easy using **pkg-config**:

```bash
pkg-config --cflags-only-I afb-binding
```

> Note for **C++** developers:
>
> The ***binder*** currently expose a draft version of **C++** api.
> To get it include the file <**afb/afb-binding**> (without **.h**).


### Declaring the API of the binding

Lines 10 to 18 show the declaration of the ***binding***.

The ***binder*** knows that this is a ***binding*** because
it finds the exported symbol **afbBindingExport** that is expected to be
a structure of type **afb_binding_t**.

```C
 10 const afb_verb_t verbs[] = {
 11         { .verb="hello", .callback=hello },
 12         { .verb=NULL }
 13 };
 14
 15 const afb_binding_t afbBindingExport = {
 16         .api = "tuto-1",
 17         .verbs = verbs
 18 };
```

The structure **afbBindingExport** actually tells that:

- the exported **API** name is **tuto-1** (line 16)
- the array of verbs is the above defined one

The exported list of verb is specified by an array of structures of
type **afb_verb_t**, each describing a verb, ended with a verb NULL (line 12).

The only defined verb here (line 11) is named **hello** (field **.verb**)
and the function that handle the related request is **hello**
(field **.callback**).

### Handling binder's requests

When the ***binder*** receives a request for the verb **hello** of
of the api **tuto-1**, it invoke the callback **hello** of the **binding**
with the argument **req** that handles the client request.

```C
  4 void hello(afb_req_t req)
  5 {
  6         AFB_REQ_DEBUG(req, "hello world");
  7         afb_req_reply(req, NULL, NULL, "hello world");
  8 }
```

The callback method for the verb receives the request and must
reply to it, either synchronously or asynchronously.

At the line 7, the callback for **tuto-1/hello** replies to the request **req**.
Parameters of the reply are:

 1. The first parameter is the request that is replied
 2. The second parameter is a json object (here NULL)
 3. The third parameter is the error string indication (here NULL: no error)
 4. The fourth parameter is an informative string (that can be NULL) that can be used to provide meta data.

The 3 last parameters are sent back to the client as the reply content.

<!-- pagebreak -->

## Sample binding: tuto-2

The second tutorial shows many important feature that can
commonly be used when writing a ***binding***:

- initialization, getting arguments, sending replies, pushing events.

This is the code of the binding **tutorials/v3/tuto-2.c**:

```C
      1 #include <string.h>
      2 #include <json-c/json.h>
      3
      4 #define AFB_BINDING_VERSION 3
      5 #include <afb/afb-binding.h>
      6
      7 afb_event_t event_login, event_logout;
      8
      9 void login(afb_req_t req)
     10 {
     11         json_object *args, *user, *passwd;
     12         char *usr;
     13
     14         args = afb_req_json(req);
     15         if (!json_object_object_get_ex(args, "user", &user)
     16          || !json_object_object_get_ex(args, "password", &passwd)) {
     17                 AFB_REQ_ERROR(req, "login, bad request: %s", json_object_get_string(args));
     18                 afb_req_reply(req, NULL, "bad-request", NULL);
     19         } else if (afb_req_context_get(req)) {
     20                 AFB_REQ_ERROR(req, "login, bad state, logout first");
     21                 afb_req_reply(req, NULL, "bad-state", NULL);
     22         } else if (strcmp(json_object_get_string(passwd), "please")) {
     23                 AFB_REQ_ERROR(req, "login, unauthorized: %s", json_object_get_string(args));
     24                 afb_req_reply(req, NULL, "unauthorized", NULL);
     25         } else {
     26                 usr = strdup(json_object_get_string(user));
     27                 AFB_REQ_NOTICE(req, "login user: %s", usr);
     28                 afb_req_session_set_LOA(req, 1);
     29                 afb_req_context_set(req, usr, free);
     30                 afb_req_reply(req, NULL, NULL, NULL);
     31                 afb_event_push(event_login, json_object_new_string(usr));
     32         }
     33 }
     34
     35 void action(afb_req_t req)
     36 {
     37         json_object *args, *val;
     38         char *usr;
     39
     40         args = afb_req_json(req);
     41         usr = afb_req_context_get(req);
     42         AFB_REQ_NOTICE(req, "action for user %s: %s", usr, json_object_get_string(args));
     43         if (json_object_object_get_ex(args, "subscribe", &val)) {
     44                 if (json_object_get_boolean(val)) {
     45                         AFB_REQ_NOTICE(req, "user %s subscribes to events", usr);
     46                         afb_req_subscribe(req, event_login);
     47                         afb_req_subscribe(req, event_logout);
     48                 } else {
     49                         AFB_REQ_NOTICE(req, "user %s unsubscribes to events", usr);
     50                         afb_req_unsubscribe(req, event_login);
     51                         afb_req_unsubscribe(req, event_logout);
     52                 }
     53         }
     54         afb_req_reply(req, json_object_get(args), NULL, NULL);
     55 }
     56
     57 void logout(afb_req_t req)
     58 {
     59         char *usr;
     60
     61         usr = afb_req_context_get(req);
     62         AFB_REQ_NOTICE(req, "login user %s out", usr);
     63         afb_event_push(event_logout, json_object_new_string(usr));
     64         afb_req_session_set_LOA(req, 0);
     65         afb_req_context_clear(req);
     66         afb_req_reply(req, NULL, NULL, NULL);
     67 }
     68
     69 int preinit(afb_api_t api)
     70 {
     71         AFB_API_NOTICE(api, "preinit");
     72         return 0;
     73 }
     74
     75 int init(afb_api_t api)
     76 {
     77         AFB_API_NOTICE(api, "init");
     78         event_login = afb_api_make_event(api, "login");
     79         event_logout = afb_api_make_event(api, "logout");
     80         if (afb_event_is_valid(event_login) && afb_event_is_valid(event_logout))
     81                 return 0;
     82         AFB_API_ERROR(api, "Can't create events");
     83         return -1;
     84 }
     85
     86 const afb_verb_t verbs[] = {
     87         { .verb="login", .callback=login },
     88         { .verb="action", .callback=action, .session=AFB_SESSION_LOA_1 },
     89         { .verb="logout", .callback=logout, .session=AFB_SESSION_LOA_1 },
     90         { .verb=NULL }
     91 };
     92
     93 const afb_binding_t afbBindingExport = {
     94         .api = "tuto-2",
     95         .specification = NULL,
     96         .verbs = verbs,
     97         .preinit = preinit,
     98         .init = init,
     99         .noconcurrency = 0
    100 };
```

Compiling:

```bash
gcc -fPIC -shared tuto-2.c -o tuto-2.so $(pkg-config --cflags --libs afb-binding)
```

Running:

```bash
afb-binder --binding ./tuto-2.so --port 3333
```

Testing:

```bash
$ afb-client -H localhost:3333/api
tuto-2 login {"help":true}
ON-REPLY 1:tuto-2/login: ERROR
{
  "jtype":"afb-reply",
  "request":{
    "status":"bad-request"
  }
}
tuto-2 login {"user":"jose","password":"please"}
ON-REPLY 2:tuto-2/login: OK
{
  "jtype":"afb-reply",
  "request":{
    "status":"success"
  }
}
tuto-2 login {"user":"jobol","password":"please"}
ON-REPLY 3:tuto-2/login: ERROR
{
  "jtype":"afb-reply",
  "request":{
    "status":"bad-state"
  }
}
tuto-2 action {"subscribe":true}
ON-REPLY 4:tuto-2/action: OK
{
  "jtype":"afb-reply",
  "request":{
    "status":"success"
  },
  "response":{
    "subscribe":true
  }
}
```

In another terminal:

```bash
$ afb-client -H localhost:3333/api
tuto-2 login {"user":"jobol","password":"please"}
ON-REPLY 1:tuto-2/login: OK
{
  "jtype":"afb-reply",
  "request":{
    "status":"success"
  }
}
tuto-2 logout true
ON-REPLY 2:tuto-2/logout: OK
{
  "jtype":"afb-reply",
  "request":{
    "status":"success"
  }
}
```

It produced in the first terminal:

```bash
ON-EVENT tuto-2/login:
{
  "jtype":"afb-event",
  "event":"tuto-2/login",
  "data":"jobol"
}
ON-EVENT tuto-2/logout:
{
  "jtype":"afb-event",
  "event":"tuto-2/logout",
  "data":"jobol"
}
```
