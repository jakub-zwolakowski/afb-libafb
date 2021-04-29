// Copyright (C) 2015-2021 IoT.bzh Company
// Author: José Bollo <jose.bollo@iot.bzh>
//
// SPDX-License-Identifier: LGPL-3.0-only

#include <string>
#include <cstring>
#include <utility>

#include <json-c/json.h>

#define AFB_BINDING_VERSION 4
#include <afb/afb-binding>

afb::event event_login, event_logout;

json_object *json_of_data(const afb::data &d)
{
	json_object *r = nullptr;
	afb::data j(afb::type_json_c(), d);
	if (j) {
		r = reinterpret_cast<json_object*>(const_cast<void*>(*j));
		r = json_object_get(r);
	}
	return r;
}

json_object *json_of_data(std::pair<unsigned, afb_data_t const *> dataset, unsigned index)
{
	return index < dataset.first ? json_of_data(dataset.second[index]) : nullptr;
}


afb::data json_to_req_data(afb::req req, json_object *obj)
{
	afb::data r(afb::type_json_c(), obj, 0,
		reinterpret_cast<void(*)(void*)>(json_object_put),
		reinterpret_cast<void*>(obj));
	return r;
}

void reply_error(afb::req req, const char *text)
{
	afb::dataset<1> a;
	a[0] = json_to_req_data(req, json_object_new_string(text));
	req.reply(-1, a);
}

class session
{
private:
	std::string user_;
public:
	session(const char *user) : user_(user) {}
	~session() {}
	operator const char *() const { return user_.c_str(); }
};

void login(afb::req req, afb::received_data params)
{
	json_object *args, *user, *passwd;
	const char *usr;
	afb::dataset<1> a;

	args = json_of_data(params, 0);
	if (!json_object_object_get_ex(args, "user", &user)
	 || !json_object_object_get_ex(args, "password", &passwd)) {
		AFB_REQ_ERROR(req, "login, bad request: %s", json_object_get_string(args));
		reply_error(req, "bad-request");
	} else if (afb_req_context_get(req, NULL)) {
		AFB_REQ_ERROR(req, "login, bad state, logout first");
		reply_error(req, "bad-state");
	} else if (strcmp(json_object_get_string(passwd), "please")) {
		AFB_REQ_ERROR(req, "login, unauthorized: %s", json_object_get_string(args));
		reply_error(req, "unauthorized");
	} else {
		usr = json_object_get_string(user);
		AFB_REQ_NOTICE(req, "login user: %s", usr);
		req.context<session>().set(new session(usr));
		req.session_set_LOA(1);
		req.reply(0, 0, nullptr);
		a[0] = json_to_req_data(req, json_object_new_string((const char*)usr));
		event_login.push(a);
	}
}

void action(afb::req req, afb::received_data params)
{
	json_object *args, *val;
	session &usr = req.context<session>();

	args = json_of_data(params, 0);
	AFB_REQ_NOTICE(req, "action for user %s: %s", (const char*)usr, json_object_get_string(args));
	if (json_object_object_get_ex(args, "subscribe", &val)) {
		if (json_object_get_boolean(val)) {
			AFB_REQ_NOTICE(req, "user %s subscribes to events", (const char*)usr);
			req.subscribe(event_login);
			req.subscribe(event_logout);
		} else {
			AFB_REQ_NOTICE(req, "user %s unsubscribes to events", (const char*)usr);
			req.unsubscribe(event_login);
			req.unsubscribe(event_logout);
		}
	}
	req.reply(0, params);
}

void logout(afb::req req, afb::received_data params)
{
	session &usr = req.context<session>();
	afb::dataset<1> a;

	AFB_REQ_NOTICE(req, "login user %s out", (const char*)usr);
	a[0] = json_to_req_data(req, json_object_new_string((const char*)usr));
	event_logout.push(a);
	req.session_set_LOA(0);
	req.context<session>().clear();
	req.reply();
}

int mainctl(afb_api_t api, afb_ctlid_t ctlid, afb_ctlarg_t ctlarg, void *userdata)
{
	afb::api a(api);

	if (ctlid == afb_ctlid_Init) {
		AFB_NOTICE("init");
		event_login = a.new_event("login");
		event_logout = a.new_event("logout");
		if (!event_login || !event_login) {
			AFB_ERROR("Can't create events");
			return -1;
		}
	}
	return 0;
}

const afb_verb_t verbs[] = {
	afb::verb<login>("login", "log in the system"),
	afb::verb<action>("action", "perform an action", AFB_SESSION_LOA_1),
	afb::verb<logout>("logout", "log out the system", AFB_SESSION_LOA_1),
	afb::verbend()
};

const afb_binding_t afbBindingExport = afb::binding("tuto-3", verbs, "third tutorial: C++", mainctl);

