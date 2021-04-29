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

#include <string>
#include <mutex>
#include <map>
#include <set>

#include <string.h>
#include <json-c/json.h>

#define AFB_BINDING_VERSION 4
#include <afb/afb-binding>

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

/*
#define AFB_BINDING_VERSION 4
#include <afb/afb-binding.h>
*/

#if !defined(APINAME)
#define APINAME "hello"
#endif

afb::type type_jsonc, type_stringz;

afb::data cstrzl_to_req_data(afb::req request, const char *string, size_t sz)
{
	return afb::data(type_stringz, string, sz, NULL, NULL);
}

afb::data cstrz_to_req_data(afb::req request, const char *string)
{
	return cstrzl_to_req_data(request, string, string ? (1 + strlen(string)) : 0);
}

afb::data fstrzl_to_req_data(afb::req request, char *string, size_t sz)
{
	return afb::data(type_stringz, string, sz, free, string);
}

afb::data fstrz_to_req_data(afb::req request, char *string)
{
	return fstrzl_to_req_data(request, string, string ? (1 + strlen(string)) : 0);
}

afb::data cpstrz_to_req_data(afb::req request, const char *string)
{
	size_t sz;
	char *copy;

	if (string == NULL) {
		copy = NULL;
		sz = 0;
	}
	else {
		sz = strlen(string) + 1;
		copy = reinterpret_cast<char*>(malloc(sz));
		if (copy != NULL)
			memcpy(copy, string, sz);
		else
			sz = 0;
	}
	return fstrzl_to_req_data(request, copy, sz);
}

int make_json_object(afb::req request, afb_data_t *data, json_object *object)
{
	return afb_create_data_raw(data, type_jsonc, object, 0,
		reinterpret_cast<void(*)(void*)>(json_object_put), object);
}

json_object *json_of_data(const afb::data &d)
{
	json_object *r = nullptr;
	afb::data j(type_jsonc, d);
	if (j) {
		r = reinterpret_cast<json_object*>(const_cast<void*>(*j));
		r = json_object_get(r);
	}
	return r;
}

afb::data json_to_req_data(afb::req req, json_object *obj)
{
	afb::data r(type_jsonc, obj, 0,
		reinterpret_cast<void(*)(void*)>(json_object_put),
		reinterpret_cast<void*>(obj));
	return r;
}

int reply_oEI(afb::req request, json_object *object, const char *error, const char *info)
{
	afb::dataset<3> data;

	data[0] = json_to_req_data(request, object);
	data[1] = cstrz_to_req_data(request, error);
	data[2] = cstrz_to_req_data(request, info);
	request.reply(error ? -1 : 0, data);
	return 0;
}

int reply_oei(afb::req request, json_object *object, const char *error, const char *info)
{
	afb::dataset<3> data;

	data[0] = json_to_req_data(request, object);
	data[1] = cpstrz_to_req_data(request, error);
	data[2] = cpstrz_to_req_data(request, info);
	request.reply(error ? -1 : 0, data);
	return 0;
}

int reply_oEi_v(afb::req request, json_object *object, const char *error, const char *info, va_list args)
{
	size_t sz;
	int rc;
	char *str;
	afb::dataset<3> data;

	rc = vasprintf(&str, info, args);
	if (rc < 0) {
		str = NULL;
		sz = 0;
	}
	else {
		sz = rc;
	}

	data[0] = json_to_req_data(request, object);
	data[1] = cstrz_to_req_data(request, error);
	data[2] = fstrzl_to_req_data(request, str, sz + 1);
	request.reply(error ? -1 : 0, data);
	return 0;
}

int reply_oEi_f(afb::req request, json_object *object, const char *error, const char *info, ...)
{
	int rc;
	va_list args;
	va_start(args, info);
	rc = reply_oEi_v(request, object, error, info, args);
	va_end(args);
	return rc;
}

/**************************************************************************/

json_object *args_to_json(afb::received_data params, json_object **result)
{
	int rc;
	struct json_object *r;
	unsigned i;

	switch (params.size()) {
	case 0:
		r = NULL;
		break;
	case 1:
		r = json_of_data(params[0]);
		break;
	default:
		r = json_object_new_array();
		for (i = 0 ; rc >= 0 && i < params.size() ; i++)
			json_object_array_add(r, json_of_data(params[i]));
	}
	return r;
}

/**************************************************************************/

static std::mutex mutex;

/**************************************************************************/

static std::map<std::string, afb::event> events;

/* deletes the event of tag */
static int event_del(const char *tag)
{
	const std::lock_guard<std::mutex> lock(mutex);

	/* check exists */
	auto e = events.find(tag);
	if (e == events.end())  {
		return -ENOENT;
	}

	/* exist, erase it */
	events.erase(e);
	return 0;
}

/* creates the event of tag */
static int event_add(const char *tag, const char *name)
{
	const std::lock_guard<std::mutex> lock(mutex);

	/* check valid tag */
	auto e = events.find(tag);
	if (e != events.end())  {
		return -EEXIST;
	}

	events.insert({ std::string(tag), afb::root().new_event(name) });
	return 0;
}

static int event_subscribe(afb::req request, const char *tag)
{
	const std::lock_guard<std::mutex> lock(mutex);

	/* get event of tag */
	auto e = events.find(tag);
	if (e == events.end())  {
		return -ENOENT;
	}
	request.subscribe(e->second);
	return 0;
}

static int event_unsubscribe(afb::req request, const char *tag)
{
	const std::lock_guard<std::mutex> lock(mutex);

	/* get event of tag */
	auto e = events.find(tag);
	if (e == events.end())  {
		return -ENOENT;
	}
	request.unsubscribe(e->second);
	return 0;
}

static int event_push(afb_data_t data, const char *tag)
{
	const std::lock_guard<std::mutex> lock(mutex);

	/* get event of tag */
	auto e = events.find(tag);
	if (e == events.end())  {
		return -ENOENT;
	}
	e->second.push(1, &data);
	return 0;
}

static int event_broadcast(afb_data_t data, const char *tag)
{
	const std::lock_guard<std::mutex> lock(mutex);

	/* get event of tag */
	auto e = events.find(tag);
	if (e == events.end())  {
		return -ENOENT;
	}
	e->second.broadcast(1, &data);
	return 0;
}

/**************************************************************************/

/** structure for recording created API */
class api
{
private:
	/** attached closure of event handlers */
	std::multiset<json_object*> clos_;

public:
	/** the api */
	afb_api_t handler;

public:
	~api() {
		for(auto i : clos_)
			json_object_put(i);
	}
	void add(json_object *clo) {
		clos_.insert(json_object_get(clo)); }
	void del(json_object *clo) {
		auto i = clos_.find(clo);
		if (i != clos_.end()) {
			clos_.erase(i);
			json_object_put(clo); } }
};

static std::map<std::string, api*> apis;

/* search the api of name */
static api *searchapi(const char *name)
{
	const std::lock_guard<std::mutex> lock(mutex);
	auto i = apis.find(name);
	return i == apis.end() ? nullptr : i->second;
}

static api *api_create(const char *name)
{
	const std::lock_guard<std::mutex> lock(mutex);
	api *a = new api;
	apis.insert({name, a});
	return a;
}

static void api_destroy(const char *name)
{
	const std::lock_guard<std::mutex> lock(mutex);
	auto i = apis.find(name);
	if (i != apis.end()) {
		delete i->second;
		apis.erase(i);
	}
}

/**************************************************************************/

static void success(afb::req request, afb::received_data params)
{
	request.reply(0, 0, 0);
}

static void fail(afb::req request, afb::received_data params)
{
	request.reply(-1, 0, 0);
}

/* simple ping */
static void ping(afb::req request, afb::received_data params)
{
	static int pingcount = 0;

	int pgc = ++pingcount;
	json_object *args;

	args_to_json(params, &args);

	reply_oEi_f(request, args, NULL, "ping count=%d", pgc);
}

static void bug (afb::req request, afb::received_data params)
{
	request.reply(0, 5, 0);
}

/**************************************************************************/

class callargs
{
protected:
	bool valid_;
	std::string api_;
	std::string verb_;
	afb::dataset<1> args_;
public:
	callargs(afb::req request, afb::received_data params);
	operator bool() const { return valid_; }
	void subcall(
		afb::req request,
		int flags,
		void (*callback)(void *closure, int status, unsigned nreplies, afb_data_t const replies[], afb_req_t req),
		void *closure) const;
	int subcallsync(
		afb::req request,
		int flags,
		int &status,
		unsigned &nreplies,
		afb_data_t replies[]) const;
	void call(
		afb::api api,
		void (*callback)(void *closure, int status, unsigned nreplies, afb_data_t const replies[], afb_api_t api),
		void *closure) const;
	int callsync(
		afb::api api,
		int &status,
		unsigned &nreplies,
		afb_data_t replies[]) const;
};

callargs::callargs(afb::req request, afb::received_data params)
{
	const char *err = nullptr;
	json_object *o, *json, *args;

	args_to_json(params, &json);
	if (!json_object_object_get_ex(json, "api", &o) || !json_object_is_type(o, json_type_string)) {
		err = "wrong-api";
	}
	else {
		api_ = json_object_get_string(o);

		if (!json_object_object_get_ex(json, "verb", &o) || !json_object_is_type(o, json_type_string)) {
			err = "wrong-verb";
		}
		else {
			verb_ = json_object_get_string(o);

			if (json_object_object_get_ex(json, "args", &args))
				args_[0] = json_to_req_data(request, json_object_get(args));
		}
	}
	json_object_put(json);
	valid_ = !err;
	if (err)
		reply_oEI(request, 0, "invalid", err);
}

void callargs::subcall(
	afb::req request,
	int flags,
	void (*callback)(void *closure, int status, unsigned nreplies, afb_data_t const replies[], afb_req_t req),
	void *closure
) const
{
	request.subcall(api_.c_str(), verb_.c_str(), args_.count(), args_.data(), flags, callback, closure);
}

int callargs::subcallsync(
	afb::req request,
	int flags,
	int &status,
	unsigned &nreplies,
	afb_data_t replies[]
) const
{
	return request.subcallsync(api_.c_str(), verb_.c_str(), args_.count(), args_.data(), flags, status, nreplies, replies);
}

void callargs::call(
	afb::api api,
	void (*callback)(void *closure, int status, unsigned nreplies, afb_data_t const replies[], afb_api_t api),
	void *closure
) const
{
	api.call(api_.c_str(), verb_.c_str(), args_.count(), args_.data(), callback, closure);
}

int callargs::callsync(
	afb::api api,
	int &status,
	unsigned &nreplies,
	afb_data_t replies[]
) const
{
	return api.callsync(api_.c_str(), verb_.c_str(), args_.count(), args_.data(), status, nreplies, replies);
}

static void subcallcb (void *closure, int status, afb::received_data replies, afb::req request)
{
	request.reply(status, replies);
}

static void subcall (afb::req request, afb::received_data params)
{
	callargs args(request, params);
	if (args)
		args.subcall(request, 0, afb::subcallcb<subcallcb>, NULL);
}

static void subcallsync (afb::req request, afb::received_data params)
{
	afb_data_t replies[3];
	unsigned nreplies = 3;
	int status;

	callargs args(request, params);
	if (args) {
		args.subcallsync(request, 0, status, nreplies, replies);
		request.reply(status, nreplies, replies);
	}
}

static void callcb (void *closure, int status, unsigned nreplies, afb_data_t const *replies, afb_api_t api)
{
	afb::req request(reinterpret_cast<afb_req_t>(closure));

	request.reply(status, nreplies, replies);
	request.unref();
}

static void call (afb::req request, afb::received_data params)
{
	callargs args(request, params);
	if (args) {
		request.addref();
		args.call(request.api(), callcb,
			const_cast<void*>(reinterpret_cast<const void*>(afb_req_t(request))));
	}

}

static void callsync (afb::req request, afb::received_data params)
{
	afb_data_t replies[3];
	unsigned nreplies = 3;
	int status;

	callargs args(request, params);
	if (args) {
		args.callsync(request.api(), status, nreplies, replies);
		request.reply(status, nreplies, replies);
	}
}

/**************************************************************************/

static int parse_event(afb::req request, afb::received_data params, const char **tag, const char **name, afb_data_t *data)
{
	json_object *args;
	json_object *x;

	args_to_json(params, &args);
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

static void eventadd (afb::req request, afb::received_data params)
{
	const char *tag, *name;

	if (!parse_event(request, params, &tag, &name, NULL))
		reply_oEI(request, NULL, "failed", "bad arguments");
	else if (0 != event_add(tag, name))
		reply_oEI(request, NULL, "failed", "creation error");
	else
		request.reply(0, 0, 0);
}

static void eventdel (afb::req request, afb::received_data params)
{
	const char *tag;

	if (!parse_event(request, params, &tag, NULL, NULL))
		reply_oEI(request, NULL, "failed", "bad arguments");
	else if (0 != event_del(tag))
		reply_oEI(request, NULL, "failed", "deletion error");
	else
		request.reply(0, 0, 0);
}

static void eventsub (afb::req request, afb::received_data params)
{
	const char *tag;

	if (!parse_event(request, params, &tag, NULL, NULL))
		reply_oEI(request, NULL, "failed", "bad arguments");
	else if (0 != event_subscribe(request, tag))
		reply_oEI(request, NULL, "failed", "subscription error");
	else
		request.reply(0, 0, 0);
}

static void eventunsub (afb::req request, afb::received_data params)
{
	const char *tag;

	if (!parse_event(request, params, &tag, NULL, NULL))
		reply_oEI(request, NULL, "failed", "bad arguments");
	else if (0 != event_unsubscribe(request, tag))
		reply_oEI(request, NULL, "failed", "unsubscription error");
	else
		request.reply(0, 0, 0);
}

static void eventpush (afb::req request, afb::received_data params)
{
	const char *tag;
	afb_data_t data;

	if (!parse_event(request, params, &tag, NULL, &data))
		reply_oEI(request, NULL, "failed", "bad arguments");
	else if (0 > event_push(data, tag))
		reply_oEI(request, NULL, "failed", "push error");
	else
		request.reply(0, 0, 0);
}

static void broadcast(afb::req request, afb::received_data params)
{
	const char *name;
	const char *tag;
	afb_data_t data;

	if (parse_event(request, params, &tag, NULL, &data)) {
		if (0 > event_broadcast(data, tag))
			reply_oEI(request, NULL, "failed", "broadcast error");
		else
			request.reply(0, 0, 0);
	} else if (parse_event(request, params, NULL, &name, &data)) {
		if (0 > afb_api_broadcast_event(afbBindingRoot, name, 1, &data))
			reply_oEI(request, NULL, "failed", "broadcast error");
		else
			request.reply(0, 0, 0);
	} else {
		reply_oEI(request, NULL, "failed", "bad arguments");
		afb_data_unref(data);
	}
}

/**************************************************************************/

static void verbose (afb::req request, afb::received_data params)
{
	int level = 5;
	json_object *query, *l;

	args_to_json(params, &query);

	if (json_object_is_type(query,json_type_int))
		level = json_object_get_int(query);
	else if (json_object_object_get_ex(query, "level", &l) && json_object_is_type(l, json_type_int))
		level = json_object_get_int(l);

	if (!json_object_object_get_ex(query,"message",&l))
		l = query;

	AFB_REQ_VERBOSE(request, level, "verbose called for %s", json_object_get_string(l));
	request.reply(0, 0, 0);

	json_object_put(query);
}

/**************************************************************************/

static void exitnow (afb::req request, afb::received_data params)
{
	int code = 0;
	json_object *query, *l;

	args_to_json(params, &query);

	if (json_object_is_type(query,json_type_int))
		code = json_object_get_int(query);
	else if (json_object_object_get_ex(query, "code", &l) && json_object_is_type(l, json_type_int))
		code = json_object_get_int(l);

	if (!json_object_object_get_ex(query,"reason",&l))
		l = NULL;

	AFB_REQ_NOTICE(request, "in phase of exiting with code %d, reason: %s", code, l ? json_object_get_string(l) : "unknown");
	request.reply(0, 0, 0);
	exit(code);
}

static void haspermcb(void *closure, int status, afb_req_t request)
{
	const char *perm = (const char *)closure;
	if (status)
		reply_oEi_f(request, NULL, NULL, "permission %s granted", perm?:"(null)");
	else
		reply_oEi_f(request, NULL, "not-granted", "permission %s NOT granted", perm?:"(null)");
	free(closure);
}

static void hasperm (afb::req request, afb::received_data params)
{
	const char *perm;
	json_object *json, *o;

	args_to_json(params, &json);
	if (!json_object_object_get_ex(json, "perm", &o) || !json_object_is_type(o, json_type_string))
		reply_oEI(request, NULL, "bad-perm", NULL);
	else {
		perm = strdup(json_object_get_string(o));
		afb_req_check_permission(request, perm, haspermcb, (void*)perm);
	}
	json_object_put(json);
}

static void closess (afb::req request, afb::received_data params)
{
	afb_req_session_close(request);
	reply_oEI(request, NULL, NULL, "session closed");
}

static void setloa (afb::req request, afb::received_data params)
{
	int loa;
	json_object *json;

	args_to_json(params, &json);
	loa = json_object_get_int(json);
	afb_req_session_set_LOA(request, loa);
	reply_oEi_f(request, NULL, NULL, "LOA set to %d", loa);
	json_object_put(json);
}

static void ok (afb::req request, afb::received_data params)
{
	request.reply(0, 0, 0);
}

static void setctx (afb::req request, afb::received_data params)
{
	json_object *json;

	args_to_json(params, &json);
	afb_req_context_set(request, json, (void(*)(void*))json_object_put, json);
	reply_oEI(request, json_object_get(json), NULL, "context set");
}

static int initctxcb(void *closure, void **value, void (**freecb)(void*), void **freeclo)
{
//	json_object *json = closure;
	*value = *freeclo = closure;
	*freecb = (void(*)(void*))json_object_put;
	return 0;
}

static void setctxif (afb::req request, afb::received_data params)
{
	json_object *json;

	args_to_json(params, &json);
	afb_req_context(request, NULL, initctxcb, json);
	reply_oEI(request, json_object_get(json), NULL, "context set");
}

static void getctx (afb::req request, afb::received_data params)
{
	struct json_object *x;
	afb_req_context_get(request, (void**)&x);
	reply_oEI(request, json_object_get(x), NULL, "returning the context");
}

static void info (afb::req request, afb::received_data params)
{
	reply_oEI(request, afb_req_get_client_info(request), NULL, NULL);
}

static void replycount (afb::req request, afb::received_data params)
{
	json_object *json;

	args_to_json(params, &json);
	int nreply = json_object_get_int(json);
	while (nreply-- > 0)
		request.reply(0, 0, 0);;
	json_object_put(json);
}

static void ref(afb::req request, afb::received_data params)
{
	afb_req_addref(request);
	request.reply(0, 0, 0);;
	afb_req_unref(request);
}

static void mute(afb::req request, afb::received_data params)
{
}

static void mutebug(afb::req request, afb::received_data params)
{
	afb_req_addref(request);
}

void queue_cb(int signum, void *arg)
{
	afb::req request((afb_req_t)arg);
	request.reply(0, 0, 0);;
	afb_req_unref(request);
}

static void queue(afb::req request, afb::received_data params)
{
	afb_req_addref(request);
	afb_job_post(0, 0, queue_cb, (void*)(afb_req_t)request, NULL);
}

static void settings(afb::req request, afb::received_data params)
{
	afb_api_t api = afb_req_get_api(request);
	struct json_object *object = afb_api_settings(api);
	reply_oEI(request, json_object_get(object), NULL, NULL);
}

static void in_after (afb::req request, afb::received_data params)
{
	int rc;
	const char *ts, *ty;
	char *te;
	double td;
	struct timespec t;
	json_object *json, *x;


	args_to_json(params, &json);

	/* get the type */
	ty = json_object_object_get_ex(json, "type", &x) ? json_object_get_string(x) : "nop";
	if (strcmp(ty, "call") && strcmp(ty, "callsync")
	 && strcmp(ty, "subcall") && strcmp(ty, "subcallsync") && strcmp(ty, "nop")) {
		reply_oEI(request, NULL, "invalid", "bad type");
		goto end;
	}

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

	/* wait for that time */
	if (td > 0) {
		t.tv_nsec = (long)(1e6 * modf(td, &td));
		t.tv_sec = (time_t)td;
		do {
			rc = nanosleep(&t, &t);
		} while (rc != 0 && errno == EINTR);

		if (rc) {
			reply_oEI(request, NULL, "error", "sleep failed");
			goto end;
		}
	}

	/* do the call */
	if (!strcmp(ty, "subcallsync"))
		subcall(request, params);
	else if (!strcmp(ty, "subcall"))
		subcallsync(request, params);
	else if (!strcmp(ty, "callsync"))
		callsync(request, params);
	else if (!strcmp(ty, "call"))
		call(request, params);
	else
		request.reply(0, 0, 0);

end:
	json_object_put(json);
}

static void *thread_after (void *closure)
{
	afb_data_t data;
	afb::req request((afb_req_t)closure);
	unsigned nparams;
	afb_data_t const *params;

	nparams = afb_req_parameters(request, &params);
	in_after (request, afb::received_data(nparams, params));
	afb_req_unref(request);
	return NULL;
}

static void after (afb::req request, afb::received_data params)
{
	int rc;
	pthread_t tid;
	pthread_attr_t attr;

	afb_req_addref(request);

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	rc = pthread_create(&tid, &attr, thread_after, (void*)(afb_req_t)request);
	pthread_attr_destroy(&attr);

	if (rc != 0) {
		afb_req_unref(request);
		reply_oEI(request, NULL, "cant-start", NULL);
	}
}

static void manageapi (afb::req request, afb::received_data params);

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
static const afb_verb_t verbs[] = {
	afb::verb("ping", afb::verbcb<ping>, nullptr),
	afb::verb("success", afb::verbcb<success>, nullptr),
	afb::verb("fail", afb::verbcb<fail>, nullptr),
	afb::verb("bug", afb::verbcb<bug>, nullptr),
	afb::verb("subcall", afb::verbcb<subcall>, nullptr),
	afb::verb("subcallsync", afb::verbcb<subcallsync>, nullptr),
	afb::verb("call", afb::verbcb<call>, nullptr),
	afb::verb("callsync", afb::verbcb<callsync>, nullptr),
	afb::verb("eventadd", afb::verbcb<eventadd>, nullptr),
	afb::verb("eventdel", afb::verbcb<eventdel>, nullptr),
	afb::verb("eventsub", afb::verbcb<eventsub>, nullptr),
	afb::verb("eventunsub", afb::verbcb<eventunsub>, nullptr),
	afb::verb("eventpush", afb::verbcb<eventpush>, nullptr),
	afb::verb("verbose", afb::verbcb<verbose>, nullptr),
	afb::verb("broadcast", afb::verbcb<broadcast>, nullptr),
	afb::verb("hasperm", afb::verbcb<hasperm>, nullptr),
	afb::verb("exit", afb::verbcb<exitnow>, nullptr),
	afb::verb("close", afb::verbcb<closess>, nullptr, AFB_SESSION_CLOSE),
	afb::verb("set-loa", afb::verbcb<setloa>, nullptr, 0, &auths[0]),
	afb::verb("has-loa-1", afb::verbcb<ok>, nullptr, AFB_SESSION_LOA_1),
	afb::verb("has-loa-2", afb::verbcb<ok>, nullptr, AFB_SESSION_LOA_2),
	afb::verb("has-loa-3", afb::verbcb<ok>, nullptr, AFB_SESSION_LOA_3),
	afb::verb("setctx", afb::verbcb<setctx>, nullptr),
	afb::verb("setctxif", afb::verbcb<setctxif>, nullptr),
	afb::verb("getctx", afb::verbcb<getctx>, nullptr),
	afb::verb("checktok", afb::verbcb<ok>, nullptr, AFB_SESSION_CHECK),
	afb::verb("info", afb::verbcb<info>, nullptr),
	afb::verb("reply-count", afb::verbcb<replycount>, nullptr),
	afb::verb("ref", afb::verbcb<ref>, nullptr),
	afb::verb("api", afb::verbcb<manageapi>, nullptr),
	afb::verb("mute", afb::verbcb<mute>, nullptr),
	afb::verb("mutebug", afb::verbcb<mutebug>, nullptr),
	afb::verb("queue", afb::verbcb<queue>, nullptr),
	afb::verb("settings", afb::verbcb<settings>, nullptr),
	afb::verb("after", afb::verbcb<after>, nullptr),
	afb::verbend()
};


static int apimainctl(afb_api_t api, afb_ctlid_t ctlid, afb_ctlarg_t ctlarg, void *userdata)
{
	if (ctlid == afb_ctlid_Pre_Init)
		afb::api(api).set_verbs(verbs);
	return 0;
}

static void apiverb (afb::req request, afb::received_data params)
{
	json_object *json;

	args_to_json(params, &json);
	reply_oEi_f(request, json, NULL, "api: %s, verb: %s",
		afb_req_get_called_api(request), afb_req_get_called_verb(request));
}

static void apievhndl(void *closure, const char *event, unsigned n, afb_data_t const a[], afb_api_t api)
{
	struct json_object *obj = (struct json_object *)closure;
	struct json_object *args;
	afb::received_data params(n,a);

	args_to_json(params, &args);
	afb_api_verbose(api, 0, NULL, 0, NULL, "the handler of closure(%s) received the event %s(%s)",
		json_object_get_string(obj), event, json_object_get_string(args));
	json_object_put(args);
}

static void manageapi (afb::req request, afb::received_data params)
{
	int rc;
	api *sapi;
	const char *action, *apiname, *verbname, *pattern;
	json_object *json, *x, *closure;
	afb_api_t oapi;

	/* get as json */
	args_to_json(params, &json);

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
		oapi = sapi ? sapi->handler : nullptr;
	} else {
		oapi = request.api();
		apiname = afb_api_name(oapi);
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
		rc = afb_create_api(&sapi->handler, apiname, NULL, 1, apimainctl, NULL);
		if (rc < 0) {
			api_destroy(apiname);
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
			api_destroy(apiname);
		} else if (!strcasecmp(action, "addverb")) {
			if (!verbname){
				reply_oEI(request, NULL, "invalid", "no verb");
				goto end;
			}
			afb_api_add_verb(oapi, verbname, NULL, afb::verbcb<apiverb>, NULL, NULL, 0, !!strchr(verbname, '*'));
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
			sapi->add(closure);
			afb_api_event_handler_add(oapi, pattern, apievhndl, closure);
		} else if (!strcasecmp(action, "delhandler")) {
			if (!pattern){
				reply_oEI(request, NULL, "invalid", "no pattern");
				goto end;
			}
			closure = NULL;
			afb_api_event_handler_del(oapi, pattern, (void**)&closure);
			sapi->del(closure);
		} else if (!strcasecmp(action, "seal")) {
			afb_api_seal(oapi);
		} else {
			reply_oEi_f(request, NULL, "invalid", "unknown action %s", action ?: "NULL");
			goto end;
		}
	}
	request.reply(0, 0, 0);;
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
		type_jsonc = "afb:JSON-C";
		type_stringz = "afb:STRINGZ";
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

const afb_binding_t afbBindingExport = afb::binding(APINAME, verbs, "hello 4 C++", mainctl);
