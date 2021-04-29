/*
 * Copyright (C) 2015-2021 IoT.bzh Company
 * Author: José Bollo <jose.bollo@iot.bzh>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>
#include <math.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <json-c/json.h>

#define AFB_BINDING_VERSION 4
#include <afb/afb-binding.h>

#if !defined(APINAME)
#define APINAME "hello"
#endif

int make_stringz_const_length(afb_req_t request, afb_data_t *data, const char *string, size_t sz)
{
	return afb_create_data_raw(data, AFB_PREDEFINED_TYPE_STRINGZ, string, sz, NULL, NULL);
}

int make_stringz_const(afb_req_t request, afb_data_t *data, const char *string)
{
	return make_stringz_const_length(request, data, string, string ? (1 + strlen(string)) : 0);
}

int make_stringz_free_length(afb_req_t request, afb_data_t *data, char *string, size_t sz)
{
	int rc;

	rc = afb_create_data_raw(data, AFB_PREDEFINED_TYPE_STRINGZ, string, sz, free, string);
	if (rc < 0)
		free(string);
	return rc;
}

int make_stringz_free(afb_req_t request, afb_data_t *data, char *string)
{
	return make_stringz_free_length(request, data, string, string ? (1 + strlen(string)) : 0);
}

int make_stringz_copy(afb_req_t request, afb_data_t *data, const char *string)
{
	size_t sz;
	char *copy;

	if (string == NULL) {
		copy = NULL;
		sz = 0;
	}
	else {
		sz = strlen(string) + 1;
		copy = malloc(sz);
		if (copy == NULL)
			return -ENOMEM;
		memcpy(copy, string, sz);
	}
	return make_stringz_free_length(request, data, copy, sz);
}

int make_json_object(afb_req_t request, afb_data_t *data, json_object *object)
{
	return afb_create_data_raw(data, AFB_PREDEFINED_TYPE_JSON_C, object, 0, (void*)json_object_put, object);
}

int reply_oEI(afb_req_t request, json_object *object, const char *error, const char *info)
{
	int rc;
	afb_data_t data[3];

	rc = make_json_object(request, &data[0], object);
	if (rc >= 0) {
		rc = make_stringz_const(request, &data[1], error);
		if (rc >= 0) {
			rc = make_stringz_const(request, &data[2], info);
			if (rc >= 0) {
				afb_req_reply(request, error ? -1 : 0, 3, data);
				return 0;
			}
			afb_data_unref(data[1]);
		}
		afb_data_unref(data[0]);
	}
	afb_req_reply(request, -2, 0, 0);
	return rc;
}

int reply_oei(afb_req_t request, json_object *object, const char *error, const char *info)
{
	int rc;
	afb_data_t data[3];

	rc = make_json_object(request, &data[0], object);
	if (rc >= 0) {
		rc = make_stringz_copy(request, &data[1], error);
		if (rc >= 0) {
			rc = make_stringz_copy(request, &data[2], info);
			if (rc >= 0) {
				afb_req_reply(request, error ? -1 : 0, 3, data);
				return 0;
			}
			afb_data_unref(data[1]);
		}
		afb_data_unref(data[0]);
	}
	afb_req_reply(request, -2, 0, 0);
	return rc;
}

int reply_oEi_v(afb_req_t request, json_object *object, const char *error, const char *info, va_list args)
{
	size_t sz;
	int rc;
	char *str;
	afb_data_t data[3];

	rc = vasprintf(&str, info, args);
	if (rc < 0) {
		str = NULL;
		sz = 0;
	}
	else {
		sz = rc;
	}

	rc = make_json_object(request, &data[0], object);
	if (rc >= 0) {
		rc = make_stringz_const(request, &data[1], error);
		if (rc >= 0) {
			rc = make_stringz_free_length(request, &data[2], str, sz + 1);
			if (rc >= 0) {
				afb_req_reply(request, error ? -1 : 0, 3, data);
				return 0;
			}
			str = NULL; /* already freed int make_stringz_free_length */
			afb_data_unref(data[1]);
		}
		afb_data_unref(data[0]);
	}
	free(str);
	afb_req_reply(request, -2, 0, 0);
	return rc;
}

int reply_oEi_f(afb_req_t request, json_object *object, const char *error, const char *info, ...)
{
	int rc;
	va_list args;
	va_start(args, info);
	rc = reply_oEi_v(request, object, error, info, args);
	va_end(args);
	return rc;
}

/**************************************************************************/

int data_2_json(afb_data_t data, struct json_object **result)
{
	int rc;
	struct json_object *r;
	afb_data_t item;

	rc = afb_data_convert(data, AFB_PREDEFINED_TYPE_JSON_C, &item);
	if (rc < 0)
		r = NULL;
	else {
		r = json_object_get((struct json_object*)afb_data_ro_pointer(item));
		afb_data_unref(item);
	}
	*result = r;
	return rc;
}

int args_to_json(unsigned nparams, afb_data_t const *params, json_object **result)
{
	int rc;
	struct json_object *r, *x;
	unsigned i;

	switch (nparams) {
	case 0:
		*result = NULL;
		rc = 0;
		break;
	case 1:
		rc = data_2_json(params[0], result);
		break;
	default:
		*result = r = json_object_new_array();
		rc = -!r;
		for (i = 0 ; rc >= 0 && i < nparams ; i++) {
			rc = data_2_json(params[i], &x);
			if (rc >= 0)
				json_object_array_add(r, x);
		}
	}
	return rc;
}

/**************************************************************************/

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

/**************************************************************************/

struct event
{
	struct event *next;
	afb_event_t event;
	char tag[1];
};

static struct event *events = 0;

/* searchs the event of tag */
static struct event *event_get(const char *tag)
{
	struct event *e = events;
	while(e && strcmp(e->tag, tag))
		e = e->next;
	return e;
}

/* deletes the event of tag */
static int event_del(const char *tag)
{
	struct event *e, **p;

	pthread_mutex_lock(&mutex);

	/* check exists */
	e = event_get(tag);
	if (!e)  {
		pthread_mutex_unlock(&mutex);
		return -1;
	}

	/* unlink */
	p = &events;
	while(*p != e) p = &(*p)->next;
	*p = e->next;

	/* destroys */
	afb_event_unref(e->event);
	free(e);

	pthread_mutex_unlock(&mutex);
	return 0;
}

/* creates the event of tag */
static int event_add(const char *tag, const char *name)
{
	int rc;
	struct event *e;

	pthread_mutex_lock(&mutex);

	/* check valid tag */
	e = event_get(tag);
	if (e) {
		pthread_mutex_unlock(&mutex);
		return -EEXIST;
	}

	/* creation */
	e = malloc(strlen(tag) + sizeof *e);
	if (!e) {
		pthread_mutex_unlock(&mutex);
		return -ENOMEM;
	}
	strcpy(e->tag, tag);

	/* make the event */
	rc = afb_api_new_event(afbBindingRoot, name, &e->event);
	if (rc < 0) {
		pthread_mutex_unlock(&mutex);
		free(e);
		return rc;
	}

	/* link */
	e->next = events;
	events = e;
	pthread_mutex_unlock(&mutex);
	return 0;
}

static int event_subscribe(afb_req_t request, const char *tag)
{
	int rc;
	struct event *e;

	pthread_mutex_lock(&mutex);
	e = event_get(tag);
	rc = e ? afb_req_subscribe(request, e->event) : -1;
	pthread_mutex_unlock(&mutex);

	return rc;
}

static int event_unsubscribe(afb_req_t request, const char *tag)
{
	int rc;
	struct event *e;

	pthread_mutex_lock(&mutex);
	e = event_get(tag);
	rc = e ? afb_req_unsubscribe(request, e->event) : -1;
	pthread_mutex_unlock(&mutex);

	return rc;
}

static int event_push(afb_data_t data, const char *tag)
{
	int rc;
	struct event *e;

	pthread_mutex_lock(&mutex);
	e = event_get(tag);
	rc = e ? afb_event_push(e->event, 1, &data) : -1;
	pthread_mutex_unlock(&mutex);

	return rc;
}

static int event_broadcast(afb_data_t data, const char *tag)
{
	int rc;
	struct event *e;

	pthread_mutex_lock(&mutex);
	e = event_get(tag);
	rc = e ? afb_event_broadcast(e->event, 1, &data) : -1;
	pthread_mutex_unlock(&mutex);

	return rc;
}

/**************************************************************************/

struct apiclo
{
	struct apiclo *next;
	json_object *clo;
};

/** structure for recording created API */
struct api
{
	/** link to the next */
	struct api *next;

	/** the api handler */
	afb_api_t api;

	/** attached closure of event handlers */
	struct apiclo *clos;

	/** name of the api */
	char name[1];
};

static struct api *apis = 0;

/* search the api of name */
static struct api *searchapi(const char *name)
{
	struct api *a = apis;

	pthread_mutex_lock(&mutex);
	while(a && strcmp(a->name, name))
		a = a->next;
	pthread_mutex_unlock(&mutex);

	return a;
}

static void api_add_clo(struct api *api, json_object *clo)
{
	struct apiclo *p;

	p = malloc(sizeof *p);
	p->clo = json_object_get(clo);
	pthread_mutex_lock(&mutex);
	p->next = api->clos;
	api->clos = p;
	pthread_mutex_unlock(&mutex);
}

static void api_del_clo(struct api *api, json_object *clo)
{
	struct apiclo **pp, *p;

	pthread_mutex_lock(&mutex);
	pp = &api->clos;
	while ((p = *pp) && p->clo != clo)
		pp = &p->next;
	if (p)
		*pp = p->next;
	pthread_mutex_unlock(&mutex);
	if (p) {
		json_object_put(p->clo);
		free(p);
	}
}

static struct api *api_create(const char *name)
{
	struct api *api;

	api = malloc (sizeof * api + strlen(name));
	strcpy(api->name, name);
	api->api = NULL;
	api->clos = NULL;
	pthread_mutex_lock(&mutex);
	api->next = apis;
	apis = api;
	pthread_mutex_unlock(&mutex);
	return api;
}

static void api_destroy(struct api *api)
{
	struct apiclo *p;
	struct api *a, **pa;

	pthread_mutex_lock(&mutex);
	pa = &apis;
	while((a = *pa) && a != api)
		pa = &a->next;
	if (a)
		*pa = a->next;
	pthread_mutex_unlock(&mutex);

	while((p = api->clos)) {
		api->clos = p->next;
		json_object_put(p->clo);
		free(p);
	}
	free(api);
}

/**************************************************************************/

static void success(afb_req_t request, unsigned nparams, afb_data_t const *params)
{
	afb_req_reply(request, 0, 0, 0);
}

static void fail(afb_req_t request, unsigned nparams, afb_data_t const *params)
{
	afb_req_reply(request, -1, 0, 0);
}

/* simple ping */
static void ping(afb_req_t request, unsigned nparams, afb_data_t const *params)
{
	static int pingcount = 0;

	int pgc = ++pingcount;
	json_object *args;

	args_to_json(nparams, params, &args);

	reply_oEi_f(request, args, NULL, "ping count=%d", pgc);
}

static void bug (afb_req_t request, unsigned nparams, afb_data_t const *params)
{
	afb_req_reply(0, 0, 0, 0);
}

/**************************************************************************/

static int get_call_args(afb_req_t request, unsigned nparams, afb_data_t const *params, const char **api, const char **verb, afb_data_t *arg0)
{
	const char *err;
	json_object *o, *json, *args;

	args_to_json(nparams, params, &json);

	if (!json_object_object_get_ex(json, "api", &o) || !json_object_is_type(o, json_type_string)) {
		err = "wrong-api";
		goto error;
	}
	*api = json_object_get_string(o);

	if (!json_object_object_get_ex(json, "verb", &o) || !json_object_is_type(o, json_type_string)) {
		err = "wrong-verb";
		goto error;
	}
	*verb = json_object_get_string(o);

	if (!json_object_object_get_ex(json, "args", &args))
		args = NULL;
	if (make_json_object(request, arg0, json_object_get(args)) < 0) {
		err = "out-of-memory";
		goto error;
	}

	json_object_put(json);
	return 1;

error:
	reply_oEI(request, 0, "invalid", err);
	json_object_put(json);
	return 0;
}

static void subcallcb (void *closure, int status, unsigned nreplies, afb_data_t const *replies, afb_req_t request)
{
	unsigned i;
	for(i = 0 ; i < nreplies ; i++)
		afb_data_addref(replies[i]);
	afb_req_reply(request, status, nreplies, replies);
}

static void subcall (afb_req_t request, unsigned nparams, afb_data_t const *params)
{
	afb_data_t args;
	const char *api, *verb;

	if (get_call_args(request, nparams, params, &api, &verb, &args))
		afb_req_subcall(request, api, verb, 1, &args, 0, subcallcb, NULL);
}

static void subcallsync (afb_req_t request, unsigned nparams, afb_data_t const *params)
{
	afb_data_t args, replies[3];
	unsigned nreplies = 3;
	int status;
	const char *api, *verb;

	if (get_call_args(request, nparams, params, &api, &verb, &args)) {
		afb_req_subcall_sync(request, api, verb, 1, &args, 0, &status, &nreplies, replies);
		afb_req_reply(request, status, nreplies, replies);
	}
}

static void callcb (void *closure, int status, unsigned nreplies, afb_data_t const *replies, afb_api_t api)
{
	afb_req_t request = closure;
	unsigned i;

	for(i = 0 ; i < nreplies ; i++)
		afb_data_addref(replies[i]);
	afb_req_reply(request, status, nreplies, replies);
	afb_req_unref(request);
}

static void call (afb_req_t request, unsigned nparams, afb_data_t const *params)
{
	afb_data_t args;
	const char *api, *verb;

	if (get_call_args(request, nparams, params, &api, &verb, &args))
		afb_api_call(afb_req_get_api(request), api, verb,
			1, &args, callcb, (void*)afb_req_addref(request));
}

static void callsync (afb_req_t request, unsigned nparams, afb_data_t const *params)
{
	afb_data_t args, replies[3];
	unsigned nreplies = 3;
	int status;
	const char *api, *verb;

	if (get_call_args(request, nparams, params, &api, &verb, &args)) {
		afb_api_call_sync(afb_req_get_api(request), api, verb, 1, &args, &status, &nreplies, replies);
		afb_req_reply(request, status, nreplies, replies);
	}
}

/**************************************************************************/

static int parse_event(afb_req_t request, unsigned nparams, afb_data_t const *params, const char **tag, const char **name, afb_data_t *data)
{
	json_object *args;
	json_object *x;

	args_to_json(nparams, params, &args);
	if (tag) {
		if (!json_object_object_get_ex(args, "tag", &x)
		 || !json_object_is_type(x, json_type_string))
			goto error;
		*tag = json_object_get_string(x);
	}
	if (name) {
		if (!json_object_object_get_ex(args, "name", &x)
		 || !json_object_is_type(x, json_type_string))
			goto error;
		*name = json_object_get_string(x);
	}
	if (data) {
		if (!json_object_object_get_ex(args, "data", &x))
			x = NULL;

		if (make_json_object(request, data, json_object_get(x)) < 0)
			goto error;
	}

	json_object_put(args);
	return 1;
error:
	json_object_put(args);
	return 0;
}

static void eventadd (afb_req_t request, unsigned nparams, afb_data_t const *params)
{
	const char *tag, *name;

	if (!parse_event(request, nparams, params, &tag, &name, NULL))
		reply_oEI(request, NULL, "failed", "bad arguments");
	else if (0 != event_add(tag, name))
		reply_oEI(request, NULL, "failed", "creation error");
	else
		afb_req_reply(request, 0, 0, 0);
}

static void eventdel (afb_req_t request, unsigned nparams, afb_data_t const *params)
{
	const char *tag;

	if (!parse_event(request, nparams, params, &tag, NULL, NULL))
		reply_oEI(request, NULL, "failed", "bad arguments");
	else if (0 != event_del(tag))
		reply_oEI(request, NULL, "failed", "deletion error");
	else
		afb_req_reply(request, 0, 0, 0);
}

static void eventsub (afb_req_t request, unsigned nparams, afb_data_t const *params)
{
	const char *tag;

	if (!parse_event(request, nparams, params, &tag, NULL, NULL))
		reply_oEI(request, NULL, "failed", "bad arguments");
	else if (0 != event_subscribe(request, tag))
		reply_oEI(request, NULL, "failed", "subscription error");
	else
		afb_req_reply(request, 0, 0, 0);
}

static void eventunsub (afb_req_t request, unsigned nparams, afb_data_t const *params)
{
	const char *tag;

	if (!parse_event(request, nparams, params, &tag, NULL, NULL))
		reply_oEI(request, NULL, "failed", "bad arguments");
	else if (0 != event_unsubscribe(request, tag))
		reply_oEI(request, NULL, "failed", "unsubscription error");
	else
		afb_req_reply(request, 0, 0, 0);
}

static void eventpush (afb_req_t request, unsigned nparams, afb_data_t const *params)
{
	const char *tag;
	afb_data_t data;

	if (!parse_event(request, nparams, params, &tag, NULL, &data))
		reply_oEI(request, NULL, "failed", "bad arguments");
	else if (0 > event_push(data, tag))
		reply_oEI(request, NULL, "failed", "push error");
	else
		afb_req_reply(request, 0, 0, 0);
}

static void broadcast(afb_req_t request, unsigned nparams, afb_data_t const *params)
{
	const char *name;
	const char *tag;
	afb_data_t data;

	if (parse_event(request, nparams, params, &tag, NULL, &data)) {
		if (0 > event_broadcast(data, tag))
			reply_oEI(request, NULL, "failed", "broadcast error");
		else
			afb_req_reply(request, 0, 0, 0);
	} else if (parse_event(request, nparams, params, NULL, &name, &data)) {
		if (0 > afb_api_broadcast_event(afbBindingRoot, name, 1, &data))
			reply_oEI(request, NULL, "failed", "broadcast error");
		else
			afb_req_reply(request, 0, 0, 0);
	} else {
		reply_oEI(request, NULL, "failed", "bad arguments");
		afb_data_unref(data);
	}
}

/**************************************************************************/

static void verbose (afb_req_t request, unsigned nparams, afb_data_t const *params)
{
	int level = 5;
	json_object *query, *l;

	args_to_json(nparams, params, &query);

	if (json_object_is_type(query,json_type_int))
		level = json_object_get_int(query);
	else if (json_object_object_get_ex(query, "level", &l) && json_object_is_type(l, json_type_int))
		level = json_object_get_int(l);

	if (!json_object_object_get_ex(query,"message",&l))
		l = query;

	AFB_REQ_VERBOSE(request, level, "verbose called for %s", json_object_get_string(l));
	afb_req_reply(request, 0, 0, 0);

	json_object_put(query);
}

/**************************************************************************/

static void exitnow (afb_req_t request, unsigned nparams, afb_data_t const *params)
{
	int code = 0;
	json_object *query, *l;

	args_to_json(nparams, params, &query);

	if (json_object_is_type(query,json_type_int))
		code = json_object_get_int(query);
	else if (json_object_object_get_ex(query, "code", &l) && json_object_is_type(l, json_type_int))
		code = json_object_get_int(l);

	if (!json_object_object_get_ex(query,"reason",&l))
		l = NULL;

	AFB_REQ_NOTICE(request, "in phase of exiting with code %d, reason: %s", code, l ? json_object_get_string(l) : "unknown");
	afb_req_reply(request, 0, 0, 0);
	exit(code);
}

static void haspermcb(void *closure, int status, afb_req_t request)
{
	const char *perm = closure;
	if (status)
		reply_oEi_f(request, NULL, NULL, "permission %s granted", perm?:"(null)");
	else
		reply_oEi_f(request, NULL, "not-granted", "permission %s NOT granted", perm?:"(null)");
	free(closure);
}

static void hasperm (afb_req_t request, unsigned nparams, afb_data_t const *params)
{
	const char *perm;
	json_object *json, *o;

	args_to_json(nparams, params, &json);
	if (!json_object_object_get_ex(json, "perm", &o) || !json_object_is_type(o, json_type_string))
		reply_oEI(request, NULL, "bad-perm", NULL);
	else {
		perm = strdup(json_object_get_string(o));
		afb_req_check_permission(request, perm, haspermcb, (void*)perm);
	}
	json_object_put(json);
}

static void closess (afb_req_t request, unsigned nparams, afb_data_t const *params)
{
	afb_req_session_close(request);
	reply_oEI(request, NULL, NULL, "session closed");
}

static void setloa (afb_req_t request, unsigned nparams, afb_data_t const *params)
{
	int loa;
	json_object *json;

	args_to_json(nparams, params, &json);
	loa = json_object_get_int(json);
	afb_req_session_set_LOA(request, loa);
	reply_oEi_f(request, NULL, NULL, "LOA set to %d", loa);
	json_object_put(json);
}

static void getloa (afb_req_t request, unsigned nparams, afb_data_t const *params)
{
	unsigned loa = afb_req_session_get_LOA(request);
	reply_oEi_f(request, json_object_new_int((int)loa), NULL, "LOA = %u", loa);
}

static void ok (afb_req_t request, unsigned nparams, afb_data_t const *params)
{
	afb_req_reply(request, 0, 0, 0);
}

static void setctx (afb_req_t request, unsigned nparams, afb_data_t const *params)
{
	json_object *json;

	args_to_json(nparams, params, &json);
	afb_req_context_set(request, json, (void*)json_object_put, json);
	reply_oEI(request, json_object_get(json), NULL, "context set");
}

static int initctxcb(void *closure, void **value, void (**freecb)(void*), void **freeclo)
{
	json_object *json = closure;
	*value = *freeclo = closure;
	*freecb = (void(*)(void*))json_object_put;
	return 0;
}

static void setctxif(afb_req_t request, unsigned nparams, afb_data_t const *params)
{
	json_object *json;

	args_to_json(nparams, params, &json);
	afb_req_context(request, NULL, initctxcb, json);
	reply_oEI(request, json_object_get(json), NULL, "context setif");
}

static void getctx(afb_req_t request, unsigned nparams, afb_data_t const *params)
{
	struct json_object *x;

	afb_req_context_get(request, (void**)&x);
	reply_oEI(request, json_object_get(x), NULL, "returning the context");
}

static void getfile(afb_req_t request, unsigned nparams, afb_data_t const *params)
{
	json_object *json, *path;
	FILE *f;
	char *buffer;
	long sz;

	args_to_json(nparams, params, &json);
	if (json_object_is_type(json, json_type_string))
		path = json;
	else if (!json_object_object_get_ex(json ,"path", &path)) {
		reply_oEI(request, NULL, "invalid-arguments", NULL);
		json_object_put(json);
		return;
	}
	f = fopen(json_object_get_string(path), "r");
	json_object_put(json);
	if (f == NULL) {
		reply_oEI(request, NULL, "bad-file", NULL);
		return;
	}

	fseek(f, 0, SEEK_END);
	sz = ftell(f);
	buffer = malloc((size_t)sz + 1);
	if (buffer == NULL) {
		reply_oEI(request, NULL, "out-of-memory", NULL);
		return;
	}

	fseek(f, 0, SEEK_SET);
	fread(buffer, (size_t)sz, 1, f);
	fclose(f);
	buffer[sz] = 0;
	json = json_object_new_string_len(buffer, (int)sz);
	reply_oEI(request, json, NULL, NULL);
}

static void info (afb_req_t request, unsigned nparams, afb_data_t const *params)
{
	reply_oEI(request, afb_req_get_client_info(request), NULL, NULL);
}

static void replycount (afb_req_t request, unsigned nparams, afb_data_t const *params)
{
	json_object *json;

	args_to_json(nparams, params, &json);
	int nreply = json_object_get_int(json);
	while (nreply-- > 0)
		afb_req_reply(request, 0, 0, 0);;
	json_object_put(json);
}

static void ref(afb_req_t request, unsigned nparams, afb_data_t const *params)
{
	afb_req_addref(request);
	afb_req_reply(request, 0, 0, 0);;
	afb_req_unref(request);
}

static void mute(afb_req_t request, unsigned nparams, afb_data_t const *params)
{
}

static void mutebug(afb_req_t request, unsigned nparams, afb_data_t const *params)
{
	afb_req_addref(request);
}

void queue_cb(int signum, void *arg)
{
	afb_req_t request = arg;
	afb_req_reply(request, 0, 0, 0);;
	afb_req_unref(request);
}

static void queue(afb_req_t request, unsigned nparams, afb_data_t const *params)
{
	afb_req_addref(request);
	afb_job_post(0, 0, queue_cb, (void*)request, NULL);
}

static void settings(afb_req_t request, unsigned nparams, afb_data_t const *params)
{
	afb_api_t api = afb_req_get_api(request);
	struct json_object *object = afb_api_settings(api);
	reply_oEI(request, json_object_get(object), NULL, NULL);
}

static void opaque(afb_req_t request, unsigned nparams, afb_data_t const *params)
{
	json_object *query, *obj, *rep = NULL;
	afb_data_t data, key;

	args_to_json(nparams, params, &query);
	if (json_object_object_get_ex(query, "set", &obj)) {
		afb_create_data_raw(&data, AFB_PREDEFINED_TYPE_OPAQUE, json_object_get(obj), 0, (void*)json_object_put, obj);
		afb_data_convert(data, AFB_PREDEFINED_TYPE_JSON_C, &key);
		rep = json_object_get((json_object*)afb_data_ro_pointer(key));
		afb_data_unref(key);
	}
	else if (json_object_object_get_ex(query, "get", &obj)) {
		afb_create_data_raw(&key, AFB_PREDEFINED_TYPE_JSON_C, json_object_get(obj), 0, (void*)json_object_put, obj);
		afb_data_convert(key, AFB_PREDEFINED_TYPE_OPAQUE, &data);
		afb_data_unref(key);
		rep = json_object_get((json_object*)afb_data_ro_pointer(data));
		afb_data_unref(data);
	}
	else if (json_object_object_get_ex(query, "drop", &obj)) {
		afb_create_data_raw(&key, AFB_PREDEFINED_TYPE_JSON_C, json_object_get(obj), 0, (void*)json_object_put, obj);
		afb_data_convert(key, AFB_PREDEFINED_TYPE_OPAQUE, &data);
		afb_data_unref(key);
		afb_data_unref(data);
		afb_data_unref(data);
	}
	reply_oEI(request, rep, NULL, NULL);
}

static void hndafter(int sig, void *closure)
{
	afb_req_t request = closure;
	unsigned nparams;
	afb_data_t const *params;
	int rc;
	const char *ty;
	json_object *json, *x;

	nparams = afb_req_parameters(request, &params);
	args_to_json(nparams, params, &json);

	/* get the type */
	ty = json_object_object_get_ex(json, "type", &x) ? json_object_get_string(x) : "nop";
	if (strcmp(ty, "call") && strcmp(ty, "callsync")
	 && strcmp(ty, "subcall") && strcmp(ty, "subcallsync") && strcmp(ty, "nop")) {
		reply_oEI(request, NULL, "invalid", "bad type");
		goto end;
	}

	/* do the call */
	if (!strcmp(ty, "subcallsync"))
		subcallsync(request, nparams, params);
	else if (!strcmp(ty, "subcall"))
		subcall(request, nparams, params);
	else if (!strcmp(ty, "callsync"))
		callsync(request, nparams, params);
	else if (!strcmp(ty, "call"))
		call(request, nparams, params);
	else
		afb_req_reply(request, 0, 0, 0);

end:
	json_object_put(json);
	afb_req_unref(request);
}

static void after (afb_req_t request, unsigned nparams, afb_data_t const *params)
{
	int rc;
	afb_timer_t timer;
	json_object *json, *x;
	const char *ts, *ty;
	char *te;
	double td;
	time_t tsec;
	unsigned tms;
	long delayms;

	args_to_json(nparams, params, &json);

	/* get the delay */
	ts = json_object_object_get_ex(json, "delay", &x) ? json_object_get_string(x) : NULL;
	if (!ts) {
		reply_oEI(request, NULL, "invalid", "no delay");
		goto end;
	}
	td = strtod(ts, &te);
	if (*te || td < 0 || td > 3e6) { /* a month is the biggest accepted */
		reply_oEI(request, NULL, "invalid", "bad delay");
		goto end;
	}
	delayms = (long)(td * 1000.0);

	afb_req_addref(request);
	rc = afb_job_post(delayms, 0, hndafter, (void*)request, 0);
	if (rc < 0) {
		afb_req_unref(request);
		reply_oEI(request, NULL, "cant-defer", NULL);
	}
end:
	json_object_put(json);
}

static void api (afb_req_t request, unsigned nparams, afb_data_t const *params);

/**
 * Definition of an authorization entry
 */
static struct afb_auth auths[] = {
	{	/* 0 (TOKEN or (YES and LOA 0 and (NO or not YES)) */
		.type = afb_auth_Or,
		.first = &auths[1],
		.next = &auths[9],
	},
	{	/* 1 (YES and LOA 0 and (NO or not YES) */
		.type = afb_auth_And,
		.first = &auths[2],
		.next = &auths[3],
	},
	{	/* 2 (YES) */
		.type = afb_auth_Yes
	},
	{	/* 3 (LOA 0 and (NO or not YES) */
		.type = afb_auth_And,
		.first = &auths[4],
		.next = &auths[5],
	},
	{	/* 4 (LOA 0) */
		.type = afb_auth_LOA,
		.loa = 0
	},
	{	/* 5 (NO or not YES) */
		.type = afb_auth_Or,
		.first = &auths[6],
		.next = &auths[7],
	},
	{	/* 6 (NO) */
		.type = afb_auth_No
	},
	{	/* 7 (not YES) */
		.type = afb_auth_Not,
		.first = &auths[8]
	},
	{	/* 8 (YES) */
		.type = afb_auth_Yes
	},
	{	/* 9 (TOKEN ...) */
		.type = afb_auth_And,
		.first = &auths[10],
		.next = &auths[13],
	},
	{	/* 10 (TOKEN or not ((PERM permission or not PERM permission) and YES)) */
		.type = afb_auth_Or,
		.first = &auths[12],
		.next = &auths[11],
	},
	{	/* 11 (not ((PERM permission or not PERM permission) and YES))*/
		.type = afb_auth_Not,
		.first = &auths[13]
	},
	{	/* 12 (TOKEN) */
		.type = afb_auth_Token
	},
	{	/* 13 ((PERM permission or not PERM permission) and YES) */
		.type = afb_auth_And,
		.first = &auths[14],
		.next = &auths[17],
	},
	{	/* 14 (PERM permission or not PERM permission) */
		.type = afb_auth_Or,
		.first = &auths[16],
		.next = &auths[15],
	},
	{	/* 15 (not PERM permission) */
		.type = afb_auth_Not,
		.first = &auths[16]
	},
	{	/* 16 (PERM permission) */
		.type = afb_auth_Permission,
		.text = "permission"
	},
	{	/* 17 (YES) */
		.type = afb_auth_Yes
	}
};


// NOTE: this sample does not use session to keep test a basic as possible
//       in real application most APIs should be protected with AFB_SESSION_CHECK
static const struct afb_verb_v4 verbs[]= {
  { .verb="ping",        .callback=ping },
  { .verb="success",     .callback=success },
  { .verb="fail",        .callback=fail },
  { .verb="bug",         .callback=bug },
  { .verb="subcall",     .callback=subcall },
  { .verb="subcallsync", .callback=subcallsync },
  { .verb="call",        .callback=call },
  { .verb="callsync",    .callback=callsync },
  { .verb="eventadd",    .callback=eventadd },
  { .verb="eventdel",    .callback=eventdel },
  { .verb="eventsub",    .callback=eventsub },
  { .verb="eventunsub",  .callback=eventunsub },
  { .verb="eventpush",   .callback=eventpush },
  { .verb="verbose",     .callback=verbose },
  { .verb="broadcast",   .callback=broadcast },
  { .verb="hasperm",     .callback=hasperm },
  { .verb="exit",        .callback=exitnow },
  { .verb="close",       .callback=closess, .session=AFB_SESSION_CLOSE },
  { .verb="set-loa",     .callback=setloa, .auth = &auths[0] },
  { .verb="get-loa",     .callback=getloa },
  { .verb="has-loa-1",   .callback=ok, .session=AFB_SESSION_LOA_1 },
  { .verb="has-loa-2",   .callback=ok, .session=AFB_SESSION_LOA_2 },
  { .verb="has-loa-3",   .callback=ok, .session=AFB_SESSION_LOA_3 },
  { .verb="setctx",      .callback=setctx },
  { .verb="setctxif",    .callback=setctxif },
  { .verb="getctx",      .callback=getctx },
  { .verb="checktok",    .callback=ok, .session=AFB_SESSION_CHECK },
  { .verb="info",        .callback=info },
  { .verb="reply-count", .callback=replycount },
  { .verb="ref",         .callback=ref},
  { .verb="api",         .callback=api},
  { .verb="mute",        .callback=mute},
  { .verb="mutebug",     .callback=mutebug},
  { .verb="queue",       .callback=queue},
  { .verb="settings",    .callback=settings},
  { .verb="after",       .callback=after},
  { .verb="opaque",      .callback=opaque},
  { .verb="getfile",     .callback=getfile},
  { .verb=NULL}
};

static int apimainctl(afb_api_t api, afb_ctlid_t ctlid, afb_ctlarg_t ctlarg, void *userdata)
{
	if (ctlid == afb_ctlid_Pre_Init)
		afb_api_set_verbs(api, verbs);
	return 0;
}

static void apiverb (afb_req_t request, unsigned nparams, afb_data_t const *params)
{
	json_object *json;

	args_to_json(nparams, params, &json);
	reply_oEi_f(request, json, NULL, "api: %s, verb: %s",
		afb_req_get_called_api(request), afb_req_get_called_verb(request));
}

static void apievhndl(void *closure, const char *event, unsigned nparams, afb_data_t const *params, afb_api_t api)
{
	struct json_object *obj = closure;
	struct json_object *args;

	args_to_json(nparams, params, &args);
	afb_api_verbose(api, 0, NULL, 0, NULL, "the handler of closure(%s) received the event %s(%s)",
		json_object_get_string(obj), event, json_object_get_string(args));
	json_object_put(args);
}

static void api (afb_req_t request, unsigned nparams, afb_data_t const *params)
{
	int rc;
	struct api *sapi;
	const char *action, *apiname, *verbname, *pattern;
	json_object *json, *x, *closure;
	afb_api_t api = afb_req_get_api(request), oapi;

	/* get as json */
	args_to_json(nparams, params, &json);

	/* get the action */
	if (!json_object_object_get_ex(json, "action", &x)) {
		reply_oEI(request, NULL, "invalid", "no action");
		goto end;
	}
	action = json_object_get_string(x);

	/* get the verb */
	verbname = json_object_object_get_ex(json, "verb", &x) ?
		json_object_get_string(x) : NULL;

	/* get the pattern */
	pattern = json_object_object_get_ex(json, "pattern", &x) ?
		json_object_get_string(x) : NULL;

	/* get the closure */
	closure = NULL;
	json_object_object_get_ex(json, "closure", &closure);

	/* get the api */
	if (json_object_object_get_ex(json, "api", &x)) {
		apiname = json_object_get_string(x);
		sapi = searchapi(apiname);
		oapi = sapi ? sapi->api : NULL;
	} else {
		oapi = api;
		apiname = afb_api_name(api);
		sapi = searchapi(apiname);
	}

	/* search the sapi */
	if (!strcasecmp(action, "create")) {
		if (!apiname) {
			reply_oEI(request, NULL, "invalid", "no api");
			goto end;
		}
		if (sapi) {
			reply_oEI(request, NULL, "already-exist", NULL);
			goto end;
		}
		sapi = api_create(apiname);
		if (!sapi) {
			reply_oEI(request, NULL, "out-of-memory", NULL);
			goto end;
		}
		rc = afb_create_api(&sapi->api, apiname, NULL, 1, apimainctl, NULL);
		if (rc < 0) {
			api_destroy(sapi);
			reply_oEi_f(request, NULL, "cant-create", "%m");
			goto end;
		}
	} else {
		if (!oapi) {
			reply_oEI(request, NULL, "cant-find-api", NULL);
			goto end;
		}
		if (!strcasecmp(action, "destroy")) {
			if (!sapi) {
				reply_oEI(request, NULL, "cant-destroy", NULL);
				goto end;
			}
			afb_api_delete(oapi);
			api_destroy(sapi);
		} else if (!strcasecmp(action, "addverb")) {
			if (!verbname){
				reply_oEI(request, NULL, "invalid", "no verb");
				goto end;
			}
			afb_api_add_verb(oapi, verbname, NULL, apiverb, NULL, NULL, 0, !!strchr(verbname, '*'));
		} else if (!strcasecmp(action, "delverb")) {
			if (!verbname){
				reply_oEI(request, NULL, "invalid", "no verb");
				goto end;
			}
			afb_api_del_verb(oapi, verbname, NULL);
		} else if (!strcasecmp(action, "addhandler")) {
			if (!pattern){
				reply_oEI(request, NULL, "invalid", "no pattern");
				goto end;
			}
			api_add_clo(sapi, closure);
			afb_api_event_handler_add(oapi, pattern, apievhndl, closure);
		} else if (!strcasecmp(action, "delhandler")) {
			if (!pattern){
				reply_oEI(request, NULL, "invalid", "no pattern");
				goto end;
			}
			closure = NULL;
			afb_api_event_handler_del(oapi, pattern, (void**)&closure);
			api_del_clo(sapi, closure);
		} else if (!strcasecmp(action, "seal")) {
			afb_api_seal(oapi);
		} else {
			reply_oEi_f(request, NULL, "invalid", "unknown action %s", action ?: "NULL");
			goto end;
		}
	}
	afb_req_reply(request, 0, 0, 0);;
end:
	json_object_put(json);

}

/*************************************************************/

static int mainctl(afb_api_t api, afb_ctlid_t ctlid, afb_ctlarg_t ctlarg, void *userdata)
{
	switch (ctlid) {
	case afb_ctlid_Root_Entry:
		AFB_NOTICE("unexpected root entry");
		break;

	case afb_ctlid_Pre_Init:
		AFB_NOTICE("hello binding comes to live");
		AFB_NOTICE("hello binding is %s", ctlarg->pre_init.path ?: "<null>");
		AFB_NOTICE("hello binding's uid is %s", ctlarg->pre_init.uid);
		AFB_NOTICE("hello binding's config is %s", json_object_get_string(ctlarg->pre_init.config));

#if defined(PREINIT_PROVIDE_CLASS)
		afb_api_provide_class(api, PREINIT_PROVIDE_CLASS);
#endif
#if defined(PREINIT_REQUIRE_CLASS)
		afb_api_require_class(api, PREINIT_REQUIRE_CLASS);
#endif
		break;

	case afb_ctlid_Init:
		AFB_NOTICE("hello binding starting");
#if defined(INIT_REQUIRE_API)
		afb_api_require_api(api, INIT_REQUIRE_API, 1);
#endif
		afb_alias_api(afb_api_name(api), "fakename");
		break;

	case afb_ctlid_Class_Ready:
		AFB_NOTICE("hello binding has classes ready");
		break;

	case afb_ctlid_Orphan_Event:
		AFB_NOTICE("received orphan event %s", ctlarg->orphan_event.name);
		break;

	case afb_ctlid_Exiting:
		AFB_NOTICE("exiting code %d", ctlarg->exiting.code);
		break;

	default:
		break;
	}
	return 0;
}

const struct afb_binding_v4 afbBindingV4 = {
	.api = APINAME,
	.specification = NULL,
	.verbs = verbs,
	.mainctl = mainctl
};

