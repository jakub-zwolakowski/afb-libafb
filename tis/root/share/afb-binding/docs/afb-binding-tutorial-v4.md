# Tutorial of bindings version 4

## Sample binding: tuto-1

This is the code of the binding **tutorials/v4/tuto-1.c**:

```C
  1 #define AFB_BINDING_VERSION 4
  2 #include <afb/afb-binding.h>
  3
  4 static const char _hello_[] = "hello world";
  5
  6 void hello(afb_req_t req, unsigned nparams, afb_data_t const *params)
  7 {
  8         afb_data_t reply;
  9         afb_type_t stringz;
 10
 11         AFB_REQ_DEBUG(req, "hello world");
 12         afb_create_data_raw(&reply, AFB_PREDEFINED_TYPE_STRINGZ, _hello_, sizeof _hello_, NULL, NULL);
 13         afb_req_reply(req, 0, 1, &reply);
 14 }
 15
 16 const afb_verb_t verbs[] = {
 17         { .verb="hello", .callback=hello },
 18         { .verb=NULL }
 19 };
 20
 21 const afb_binding_t afbBindingExport = {
 22         .api = "tuto-1",
 23         .verbs = verbs
 24 };
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
Content-Length: 77
Set-Cookie: x-afb-uuid-3333=0f6345d9-9d5b-4ff5-8bb2-cbd147ed45da; Path=/api; Max-Age=32000000; HttpOnly
Date: Tue, 24 Nov 2020 15:28:49 GMT

{"jtype":"afb-reply","request":{"status":"success"},"response":"hello world"}
```

Testing using **afb-client** (with option -H for
getting a human readable output):

```bash
$ afb-client -H ws://localhost:3333/api tuto-1 hello
ON-REPLY 1:tuto-1/hello: OK
{
  "jtype":"afb-reply",
  "request":{
    "status":"success"
  },
  "response":"hello world"
}
```

This shows basic things:

- The include to get for creating a binding
- How to declare the API offered by the binding
- How to handle requests made to the binding

### Getting declarations for the binding

The lines 1 and 2 show how to get the include file **afb-binding.h**.

```C
  1 #define AFB_BINDING_VERSION 4
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
> The ***binder*** currently expose a **DRAFT** version of **C++** api.
> To get it include the file <**afb/afb-binding**> (without **.h**).


### Declaring the API of the binding

Lines 16 to 24 show the declaration of the ***binding***.

The ***binder*** knows that this is a ***binding*** because
it finds the exported symbol **afbBindingExport** that is expected to be
a structure of type **afb_binding_t**.

```C
 16 const afb_verb_t verbs[] = {
 17         { .verb="hello", .callback=hello },
 18         { .verb=NULL }
 19 };
 20
 21 const afb_binding_t afbBindingExport = {
 22         .api = "tuto-1",
 23         .verbs = verbs
 24 };
```

The structure **afbBindingExport** actually tells that:

- the exported **API** name is **tuto-1** (line 22)
- the array of verbs is the above defined one

The exported list of verb is specified by an array of structures of
type **afb_verb_t**, each describing a verb, ended with a verb NULL (line 18).

The only defined verb here (line 11) is named **hello** (field **.verb**)
and the function that handle the related request is **hello**
(field **.callback**).

### Handling binder's requests

When the ***binder*** receives a request for the verb **hello** of
of the api **tuto-1**, it invoke the callback **hello** of the **binding**
with the argument **req** that handles the client request.

```C
  4 static const char _hello_[] = "hello world";
  5
  6 void hello(afb_req_t req, unsigned nparams, afb_data_t const *params)
  7 {
  8         afb_data_t reply;
  9         afb_type_t stringz;
 10
 11         AFB_REQ_DEBUG(req, "hello world");
 12         afb_create_data_raw(&reply, AFB_PREDEFINED_TYPE_STRINGZ, _hello_, sizeof _hello_, NULL, NULL);
 13         afb_req_reply(req, 0, 1, &reply);
 14 }
```

The callback method for the verb receives the request and must
reply to it, either synchronously or asynchronously.

At the line 13, the callback for **tuto-1/hello** replies to the request **req**.
Parameters of the reply are:

 1. The first parameter is the request that is replied
 2. The second parameter is a status code (here 0)
 3. The third parameter is the count of replied data (here 1)
 4. The fourth parameter is an array of the replied data (here a single value)
