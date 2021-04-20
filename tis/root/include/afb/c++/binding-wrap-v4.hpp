/*
 * Copyright (C) 2015-2021 IoT.bzh Company
 * Author: José Bollo <jose.bollo@iot.bzh>
 *
 * SPDX-License-Identifier: LGPL-3.0-only
 */

#pragma once

#include <cstddef>
#include <cstdlib>
#include <cstdarg>
#include <functional>
#include <utility>
#include <string>
#include <stdexcept>
#include <memory>

/* check the version */
#if AFB_BINDING_VERSION != 4
# error "AFB_BINDING_VERSION must be 4"
#endif

/* get C definitions of bindings */
extern "C" {
#include <afb/afb-binding.h>
}

namespace afb {
/*************************************************************************/
/* pre-declaration of classes                                            */
/*************************************************************************/

class api;
class arg;
class data;
class event;
class req;
class type;

/*************************************************************************/
/* declaration of functions                                              */
/*************************************************************************/
#if 0
int broadcast_event(const char *name, struct json_object *object = nullptr);

event new_event(const char *name);

void verbose(int level, const char *file, int line, const char * func, const char *fmt, va_list args);

void verbose(int level, const char *file, int line, const char * func, const char *fmt, ...);

int queue_job(void (*callback)(int signum, void *arg), void *argument, void *group, int timeout);

int require_api(const char *apiname, bool initialized = true);

int add_alias(const char *apiname, const char *aliasname);

int verbosity();

bool wants_errors();
bool wants_warnings();
bool wants_notices();
bool wants_infos();
bool wants_debugs();

void call(const char *api, const char *verb, struct json_object *args, void (*callback)(void*closure, int iserror, struct json_object *result, afb_api_t api), void *closure);

template <class T> void call(const char *api, const char *verb, struct json_object *args, void (*callback)(T*closure, int iserror, struct json_object *result, afb_api_t api), T *closure);

bool callsync(const char *api, const char *verb, struct json_object *args, int &status, unsigned &nreplies, afb_data_t replies[]);


	int queue_job(queue_cb callback, void *argument, void *group, int timeout) const;
inline int api::queue_job(queue_cb callback, void *argument, void *group, int timeout) const { return afb_api_queue_job(api_, callback, argument, group, timeout); }


	api create_api(const char *apiname, const char *info, int noconcurrency, mainctl_cb mainctl, void *closure) const;
	api create_api(const std::string &apiname, const std::string &info, int noconcurrency, mainctl_cb mainctl, void *closure) const;

inline api api::new_api(const char *apiname, const char *info, int noconcurrency, api::mainctl_cb mainctl, void *closure) const { afb_api_t a; afb_api_new_api(api_, &a, apiname, info, noconcurrency, mainctl, closure); return api(a); }
inline api api::new_api(const std::string &apiname, const std::string &info, int noconcurrency, api::mainctl_cb mainctl, void *closure) const { return new_api(apiname.c_str(), info.c_str(), noconcurrency, mainctl, closure); }

inline int api::add_alias(const char *name, const char *as_name) const { return afb_api_add_alias(api_, name, as_name); }
inline int api::add_alias(const std::string &name, const std::string &as_name) const { return afb_api_add_alias(api_, name.c_str(), as_name.c_str()); }
	int add_alias(const char *name, const char *as_name) const;
	int add_alias(const std::string &name, const std::string &as_name) const;

inline int queue_job(void (*callback)(int signum, void *arg), void *argument, void *group, int timeout)
	{ return root().queue_job(callback, argument, group, timeout); }

inline int add_alias(const char *apiname, const char *aliasname)
	{ return root().add_alias(apiname, aliasname); }

#endif

/*************************************************************************/
/* effective declaration of classes                                      */
/*************************************************************************/

class type;
class data;

/**
 * class wrapping afb_type_t
 */
class type
{
protected:
	/** wrapped type */
	afb_type_t type_;
public:
	/** default constructor */
	type() noexcept
		: type_{nullptr} {}

	/** initialisation constructor */
	type(afb_type_t type) noexcept
		: type_{type} {}

	/** copy constructor */
	type(const type &other) noexcept
		: type_{other.type_} {}

	/** initialisation constructor */
	type(const char *name)
		{ afb_type_lookup(&type_, name); }

	/** initialisation constructor */
	type(const std::string &name)
		{ afb_type_lookup(&type_, name.c_str()); }

	/** assign */
	type &operator=(afb_type_t type) noexcept
		{ type_ = type; return *this; }
	type &operator=(const type &other) noexcept
		{ return *this = other.type_; }
	type &operator=(const char *name)
		{ afb_type_lookup(&type_, name); return *this; }
	type &operator=(const std::string &name)
		{ afb_type_lookup(&type_, name.c_str()); return *this; }

	/** validity */
	bool is_valid() const noexcept
		{ return bool(type_); }
	operator bool() const noexcept
		{ return is_valid(); }

	/** unwrap */
	operator afb_type_t() const noexcept
		{ return type_; }

	/** creation of data */
	afb_data_t create_raw(const void *pointer, size_t size, void (*dispose)(void*), void *closure) const {
		afb_data_t r;
		int rc = afb_create_data_raw(&r, type_, pointer, size, dispose, closure);
		if (rc < 0)
			throw std::bad_alloc();
		return r;
	}

	afb_data_t create_alloc(void **pointer, size_t size) const {
		afb_data_t r;
		int rc = afb_create_data_alloc(&r, type_, pointer, size);
		if (rc < 0)
			throw std::bad_alloc();
		return r;
	}

	afb_data_t create_copy(const void *pointer, size_t size) const {
		afb_data_t r;
		int rc = afb_create_data_copy(&r, type_, pointer, size);
		if (rc < 0)
			throw std::bad_alloc();
		return r;
	}

	/** conversion of data */
	afb_data_t convert(afb_data_t data) const {
		afb_data_t r;
		int rc = afb_data_convert(data, type_, &r);
		if (rc < 0)
			throw std::bad_alloc();
		return r;
	}
};

/**
 * class wrapping afb_data_t
 */
class data
{
protected:
	/** wrapped data */
	afb_data_t data_;
public:
	data() noexcept;
	data(afb_data_t d) noexcept;
	data(const data &other) noexcept;
	data(data &&other) noexcept;
	data(afb::type type, const data &other);
	data(afb::type type, const void *pointer, size_t size, void (*dispose)(void*), void *closure);
	data(afb::type type, const void *pointer, size_t size);
	data(afb::type type, void *&pointer, size_t size);
	~data();

	data &operator=(afb_data_t d);
	data &operator=(const data &other);
	data &operator=(data &&other);

	void unref();
	void addref();

	operator afb_data_t() const;

	operator bool() const;
	bool is_valid() const;

	afb::type type() const;
	const void *pointer() const;
	const void *operator *() const;
	size_t size() const;

	data convert(afb::type t) const;
};

/**
 * class intended to manage received set of data: params,
 * and asynchronous replies
 */
template <unsigned count_>
class dataset
{
protected:
	afb_data_t array_[count_];
public:
	dataset() {}

	/* copy method */
	dataset(const dataset &other) = delete;

	class ref {
	private:
		afb_data_t *item_;
		friend class dataset;
		ref(afb_data_t *item) : item_{item} {}
	public:
		operator data() const { return data(*item_); }
		data operator=(const data &data) const { *item_ = data; return data; }
	};

	/* access */
	ref operator[](int idx) { return ref(&array_[idx]); }
	ref operator[](unsigned idx) { return ref(&array_[idx]); }
	unsigned count() const { return count_; }
	afb_data_t const *data() const { return array_; }
};

/**
 * class intended to manage received set of data: params,
 * and asynchronous replies
 */
class received_data
{
protected:
	/** recorded count of data */
	unsigned size_;

	/** recorded array of data */
	afb_data_t const *array_;

public:
	/** common constructor */
	received_data(unsigned size, afb_data_t const array[]) noexcept
		: size_{size}, array_{array} {}

	/** copy constructor */
	received_data(const received_data &other) noexcept
		: size_{other.size_}, array_{other.array_} {}

	/** as pair */
	operator std::pair<unsigned, afb_data_t const *>() const noexcept
		{ return std::pair<unsigned, afb_data_t const *>(size_, array_); }

	/** count of data */
	unsigned size() const noexcept
		{ return size_; }

	operator unsigned() const noexcept
		{ return size_; }

	/** get the array of data */
	afb_data_t const *array() const noexcept
		{ return array_; }

	operator afb_data_t const *() const noexcept
		{ return array_; }

	/** add a reference on each data */
	void addref() const noexcept
		{ afb_data_array_addref(size_, array_); }

	/** add a reference on each data */
	void unref() const noexcept
		{ afb_data_array_unref(size_, array_); }

	/** access the data */
	afb::data operator[](unsigned idx) const noexcept
		{ return afb::data(array_[idx]); }

	afb::data operator[](int idx) const noexcept
		{ return afb::data(array_[idx]); }

	afb::data at(unsigned idx) const {
		if (size_ <= idx)
			throw std::out_of_range("out of range");
		return afb::data(array_[idx]);
	}

	afb::data at(int idx) const {
		if (idx < 0 || size_ <= idx)
			throw std::out_of_range("out of range");
		return afb::data(array_[idx]);
	}
};

/* apis */
class api
{
protected:
	afb_api_t api_;
public:
	using call_cb = afb_call_callback_t;
	using event_cb = afb_event_handler_t;
	using verb_cb = afb_req_callback_t;
	using mainctl_cb = afb_api_callback_t;
	using queue_cb = void (*)(int signum, void *arg);

	api();
	api(afb_api_t a);
	api(const api &other) = delete;
	api(api &&other);
	~api();
	api &operator=(const api &other) = delete;
	api &operator=(api &&other);

	operator afb_api_t() const;
	afb_api_t operator->() const;

	/* General functions */
	const char *name() const;
	void *get_userdata() const;
	void set_userdata(void *value) const;
	int require_api(const char *name, int initialized) const;
	int require_api(const std::string &name, int initialized) const;

	/* Verbosity functions */
	int wants_log_level(int level) const;
	void vverbose(int level, const char *file, int line, const char *func, const char *fmt, va_list args) const;
	void verbose(int level, const char *file, int line, const char *func, const char *fmt, ...) const;

	/* Data retrieval functions */
	struct json_object *settings() const;

	/* Calls and job functions */
	void call(const char *apiname, const char *verb, unsigned nparams, afb_data_t const params[], call_cb callback, void *closure) const;
	void call(const std::string &apiname, const std::string &verb, unsigned nparams, afb_data_t const params[], call_cb callback, void *closure) const;
	int callsync(const char *apiname, const char *verb, unsigned nparams, afb_data_t const params[], int &status, unsigned &nreplies, afb_data_t replies[]) const;
	int callsync(const std::string &apiname, const std::string &verb, unsigned nparams, afb_data_t const params[], int &status, unsigned &nreplies, afb_data_t replies[]) const;

	/* Event functions */
	int broadcast_event(const char *name, unsigned nparams, afb_data_t const params[]) const;
	int broadcast_event(const std::string &name, unsigned nparams, afb_data_t const params[]) const;
	event new_event(const char *name) const;
	event new_event(const std::string &name) const;
	int event_handler_add(const char *pattern, event_cb callback, void *closure) const;
	int event_handler_add(const std::string &pattern, event_cb callback, void *closure) const;
	int event_handler_del(const char *pattern, void **closure) const;
	int event_handler_del(const std::string &pattern, void **closure) const;

	/* Dynamic api functions */
	int set_verbs(const afb_verb_t *verbs) const;
	int add_verb(const char *verb, const char *info, verb_cb callback, void *vcbdata, const struct afb_auth *auth, uint32_t session, int glob) const;
	int add_verb(const std::string &verb, const std::string &info, verb_cb callback, void *vcbdata, const struct afb_auth *auth, uint32_t session, int glob) const;
	int del_verb(const char *verb, void **vcbdata) const;
	int del_verb(const std::string &verb, void **vcbdata) const;
	int provide_class(const char *name) const;
	int provide_class(const std::string &name) const;
	int require_class(const char *name) const;
	int require_class(const std::string &name) const;
	void seal() const;
	int delete_api() const;
};

/* events */
class event
{
	afb_event_t event_;
public:
	event();
	event(afb_event_t e);
	event(const event &other);
	event(event &&other);
	~event();

	event &operator=(afb_event_t e);
	event &operator=(const event &other);
	event &operator=(event &&other);

	operator afb_event_t() const;
	afb_event_t extract();
	afb_event_t twin() const;

	operator bool() const;
	bool is_valid() const;

	int broadcast(unsigned nparams, afb_data_t const params[]) const;
	template <unsigned n> int broadcast(const dataset<n> &params) const;
	int push(unsigned nparams, afb_data_t const params[]) const;
	template <unsigned n> int push(const dataset<n> &params) const;

	void unref();
	void addref();
	const char *name() const;
};

/* req(uest) */
class req
{
	afb_req_t req_;

public:
	req() = delete;
	req(afb_req_t r);
	req(const req &other);
	req &operator=(const req &other);

	operator afb_req_t() const;
	afb_req_t operator->() const;

	operator bool() const;
	bool is_valid() const;

	afb::api api() const;

	void reply(int status = 0) const;
	void reply(int status, unsigned nreplies, afb_data_t const replies[]) const;
	template <unsigned n> void reply(int status, const dataset<n> &replies) const;
	void reply(int status, received_data replies) const;

	void addref() const;

	void unref() const;

	void session_close() const;

	bool session_set_LOA(unsigned level) const;

	bool subscribe(const event &event) const;

	bool unsubscribe(const event &event) const;

	void subcall(const char *api, const char *verb, unsigned nparams, afb_data_t const params[], int flags, void (*callback)(void *closure, int status, unsigned nreplies, afb_data_t const replies[], afb_req_t req), void *closure) const;
	template <class T> void subcall(const char *api, const char *verb, unsigned nparams, afb_data_t const params[], int flags, void (*callback)(T *closure, int status, unsigned nreplies, afb_data_t const replies[], afb_req_t req), T *closure) const;

	int subcallsync(const char *api, const char *verb, unsigned nparams, afb_data_t const params[], int flags, int &status, unsigned &nreplies, afb_data_t replies[]) const;

	void verbose(int level, const char *file, int line, const char * func, const char *fmt, va_list args) const;

	void verbose(int level, const char *file, int line, const char * func, const char *fmt, ...) const;

	struct json_object *get_client_info() const;

	template < class T = void >
	class contextclass {

		friend class req;
		afb_req_t req_;
		contextclass(afb_req_t r) : req_(r) {}

	public:
		inline operator T *() const { return get(); }
		inline operator T &() const { return *get(); }
		inline T* get() const {
			T* val = nullptr;
			afb_req_context_get(req_, reinterpret_cast<void**>(&val));
			return val;
		}

		inline void set(T *value, void (*destroyer)(T*) = [](T*t){delete t;}) const {
			afb_req_context_set(req_,
				reinterpret_cast<void*>(value),
				reinterpret_cast<void(*)(void*)>(destroyer),
				reinterpret_cast<void*>(value));
		}

		inline void unset() { afb_req_context_drop(req_); }
		inline void clear() { afb_req_context_drop(req_); }
/*
		inline T *lazy(T *(*allocator)() = []()->T*{return new T();}, void (*destroyer)(T*) = [](T*t){delete t;}) const {
			return reinterpret_cast<T*>(
				afb_req_context(req_, 0,
					[allocator](void*)->T*{return allocator();},
					reinterpret_cast<void(*)(void*)>(destroyer),
					nullptr));
		}

		template <class I>
		inline T *lazy(I *i, T *(*allocator)(I*) = [](I*i)->T*{return new T(i);}, void (*destroyer)(T*) = [](T*t){delete t;}) const {
			return reinterpret_cast<T*>(
				afb_req_context(req_, 0,
					[allocator](void*i)->T*{return allocator(reinterpret_cast<I*>(i));},
					reinterpret_cast<void(*)(void*)>(destroyer),
					reinterpret_cast<void*>(i)));
		}
*/
	};

	template < class T > contextclass<T> context() const { return contextclass<T>(req_); }
};

/*************************************************************************/
/* effective declaration of classes                                      */
/*************************************************************************/
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

template < class _T_ >
struct client_context {

	inline _T_ *get(afb_req_t req) const {
		_T_ *val = nullptr;
		afb_req_context(req, reinterpret_cast<void**>(&val), _init_, nullptr);
		return val;
	}

private:

	static int _init_(void*, void **value, void (**freecb)(void*), void **freeclo) {
		_T_ *t = new _T_();
		*value = *freeclo = t;
		*freecb = _destroy_;
		return 0;
	}
	static void _destroy_(void *ptr) { delete reinterpret_cast<_T_*>(ptr); }
};

template < class _T_ >
struct api_context {

	inline _T_ *get(afb_req_t req) const {
		afb_api_t api = afb_req_get_api(req);
		void *ptr = afb_api_get_userdata(api);
		return reinterpret_cast<_T_*>(ptr);
	}
};

template < class _T_ >
struct vcb_context {

	inline _T_ *get(afb_req_t req) const {
		void *ptr = afb_req_get_vcbdata(req);
		return reinterpret_cast<_T_*>(ptr);
	}
};

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

template <void (*_F_)(afb::req,afb::received_data)>
void verbcb(afb_req_t req, unsigned nparams, afb_data_t const params[])
{
	_F_(req, afb::received_data(nparams, params));
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

template <class _C_, void (_C_::*_F_)(afb::req,afb::received_data)>
void verbcb_client(afb_req_t req, unsigned nparams, afb_data_t const params[])
{
	client_context<_C_>::get(req)->_F_(req, afb::received_data(nparams, params));
}

template <class _C_, void (_C_::*_F_)(afb::req,afb::received_data) const>
void verbcb_client(afb_req_t req, unsigned nparams, afb_data_t const params[])
{
	client_context<_C_>::get(req)->_F_(req, afb::received_data(nparams, params));
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

template <class _C_, void (_C_::*_F_)(afb::req,afb::received_data)>
void verbcb_api(afb_req_t req, unsigned nparams, afb_data_t const params[])
{
	api_context<_C_>::get(req)->_F_(req, afb::received_data(nparams, params));
}

template <class _C_, void (_C_::*_F_)(afb::req,afb::received_data) const>
void verbcb_api(afb_req_t req, unsigned nparams, afb_data_t const params[])
{
	api_context<_C_>::get(req)->_F_(req, afb::received_data(nparams, params));
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

template <class _C_, void (_C_::*_F_)(afb::req,afb::received_data)>
void verbcb_vcb(afb_req_t req, unsigned nparams, afb_data_t const params[])
{
	vcb_context<_C_>::get(req)->_F_(req, afb::received_data(nparams, params));
}

template <class _C_, void (_C_::*_F_)(afb::req,afb::received_data) const>
void verbcb_vcb(afb_req_t req, unsigned nparams, afb_data_t const params[])
{
	vcb_context<_C_>::get(req)->_F_(req, afb::received_data(nparams, params));
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

template <void (*_F_)(void*,int,afb::received_data,afb::req)>
void subcallcb(void *closure, int status, unsigned nreplies, afb_data_t const replies[], afb_req_t req)
{
	_F_(closure, status, afb::received_data(nreplies, replies), req);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

template <class _C_, void (_C_::*_F_)(void*,int,afb::received_data,afb::req)>
void subcallcb(void *closure, int status, unsigned nreplies, afb_data_t const replies[], afb_req_t req)
{
	reinterpret_cast<_C_*>(closure)->_F_(status, afb::received_data(nreplies, replies), req);
}

template <class _C_, void (_C_::*_F_)(void*,int,afb::received_data,afb::req) const>
void subcallcb(void *closure, int status, unsigned nreplies, afb_data_t const replies[], afb_req_t req)
{
	reinterpret_cast<_C_*>(closure)->_F_(status, afb::received_data(nreplies, replies), req);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

template <void (*_F_)(void*,int,afb::received_data,afb::api)>
void callcb(void *closure, int status, unsigned nreplies, afb_data_t const replies[], afb_api_t api)
{
	_F_(closure, status, afb::received_data(nreplies, replies), api);
}

template <void (*_F_)(void*,int,afb::received_data)>
void callcb(void *closure, int status, unsigned nreplies, afb_data_t const replies[], afb_api_t api)
{
	_F_(closure, status, afb::received_data(nreplies, replies));
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

template <class _C_, void (_C_::*_F_)(int,afb::received_data,afb::api)>
void callcb(void *closure, int status, unsigned nreplies, afb_data_t const replies[], afb_api_t api)
{
	reinterpret_cast<_C_*>(closure)->_F_(status, afb::received_data(nreplies, replies), api);
}

template <class _C_, void (_C_::*_F_)(int,afb::received_data,afb::api) const>
void callcb(void *closure, int status, unsigned nreplies, afb_data_t const replies[], afb_api_t api)
{
	reinterpret_cast<_C_*>(closure)->_F_(status, afb::received_data(nreplies, replies), api);
}

template <class _C_, void (_C_::*_F_)(int,afb::received_data)>
void callcb(void *closure, int status, unsigned nreplies, afb_data_t const replies[], afb_api_t api)
{
	reinterpret_cast<_C_*>(closure)->_F_(status, afb::received_data(nreplies, replies));
}

template <class _C_, void (_C_::*_F_)(int,afb::received_data) const>
void callcb(void *closure, int status, unsigned nreplies, afb_data_t const replies[], afb_api_t api)
{
	reinterpret_cast<_C_*>(closure)->_F_(status, afb::received_data(nreplies, replies));
}

/*************************************************************************/
/* effective members of class afb::type                                  */
/*************************************************************************/

inline type type_opaque() { return type(AFB_PREDEFINED_TYPE_OPAQUE); }
inline type type_stringz() { return type(AFB_PREDEFINED_TYPE_STRINGZ); }
inline type type_json() { return type(AFB_PREDEFINED_TYPE_JSON); }
inline type type_json_c() { return type(AFB_PREDEFINED_TYPE_JSON_C); }

/*************************************************************************/
/* effective members of class afb::data                                  */
/*************************************************************************/

inline data::data()  noexcept : data_{nullptr} { }
inline data::data(afb_data_t d) noexcept : data_{d} { }
inline data::data(data &&other) noexcept : data_{std::exchange(other.data_, nullptr)} {}
inline data::data(const data &other) noexcept : data_{other.data_} {}
inline data::data(afb::type type, const data &other) { afb_data_convert(other, type, &data_); }
inline data::data(afb::type type, const void *pointer, size_t size, void (*dispose)(void*), void *closure)
	{ afb_create_data_raw(&data_, type, pointer, size, dispose, closure); }
inline data::data(afb::type type, const void *pointer, size_t size)
	{ afb_create_data_copy(&data_, type, pointer, size); }
inline data::data(afb::type type, void *&pointer, size_t size)
	{ afb_create_data_alloc(&data_, type, &pointer, size); }
inline data::~data() {  }

inline data &data::operator=(afb_data_t d) { data_ = d; return *this; }
inline data &data::operator=(const data &other) { return *this = other.data_; }
inline data &data::operator=(data &&other) { return *this = std::exchange(other.data_, nullptr); }

inline void data::unref() { afb_data_unref(std::exchange(data_, nullptr)); }
inline void data::addref() { afb_data_addref(data_); }

inline data::operator afb_data_t() const { return data_; }

inline data::operator bool() const { return is_valid(); }
inline bool data::is_valid() const { return afb_data_is_valid(data_); }

inline type data::type() const { return afb_data_type(data_); }
inline const void *data::pointer() const { return afb_data_ro_pointer(data_); }
inline const void *data::operator *() const { return afb_data_ro_pointer(data_); }
inline size_t data::size() const { return afb_data_size(data_); }

inline data data::convert(afb::type t) const { data r(t, *this); return r; }

/*************************************************************************/
/* effective members of class afb::api                                   */
/*************************************************************************/

inline api::api() : api_{nullptr} { }
inline api::api(afb_api_t a) : api_{a} { }
inline api::api(api &&other) : api_{other.api_} { other.api_ = nullptr; }
inline api::~api() { api_ = nullptr; }
inline api &api::operator=(api &&other) { api_ = other.api_; other.api_ = nullptr; return *this;}
inline api::operator afb_api_t() const { return api_; }
inline afb_api_t api::operator->() const { return api_; }
inline const char *api::name() const { return afb_api_name(api_); }
inline void *api::get_userdata() const { return afb_api_get_userdata(api_); }
inline void api::set_userdata(void *value) const { afb_api_set_userdata(api_, value); }
inline int api::require_api(const char *name, int initialized) const { return afb_api_require_api(api_, name, initialized); }
inline int api::require_api(const std::string& name, int initialized) const { return afb_api_require_api(api_, name.c_str(), initialized); }
inline int api::wants_log_level(int level) const { return afb_api_wants_log_level(api_, level); }
inline void api::vverbose(int level, const char *file, int line, const char *func, const char *fmt, va_list args) const { afb_api_vverbose(api_, level, file, line, func, fmt, args); }
inline void api::verbose(int level, const char *file, int line, const char *func, const char *fmt, ...) const
{
	va_list args;
	va_start(args, fmt);
	vverbose(level, file, line, func, fmt, args);
	va_end(args);
}
inline struct json_object *api::settings() const { return afb_api_settings(api_); }
inline void api::call(const char *apiname, const char *verb, unsigned nparams, afb_data_t const params[], api::call_cb callback, void *closure) const { afb_api_call(api_, apiname, verb, nparams, params, callback, closure); }
inline void api::call(const std::string &apiname, const std::string &verb, unsigned nparams, afb_data_t const params[], api::call_cb callback, void *closure) const { afb_api_call(api_, apiname.c_str(), verb.c_str(), nparams, params, callback, closure); }
inline int api::callsync(const char *apiname, const char *verb, unsigned nparams, afb_data_t const params[], int &status, unsigned &nreplies, afb_data_t replies[]) const { return afb_api_call_sync(api_, apiname, verb, nparams, params, &status, &nreplies, replies); }
inline int api::callsync(const std::string &apiname, const std::string &verb, unsigned nparams, afb_data_t const params[], int &status, unsigned &nreplies, afb_data_t replies[]) const { return afb_api_call_sync(api_, apiname.c_str(), verb.c_str(), nparams, params, &status, &nreplies, replies); }

inline int api::broadcast_event(const char *name, unsigned nparams, afb_data_t const params[]) const { return afb_api_broadcast_event(api_, name, nparams, params); }
inline int api::broadcast_event(const std::string &name, unsigned nparams, afb_data_t const params[]) const { return afb_api_broadcast_event(api_, name.c_str(), nparams, params); }
inline event api::new_event(const char *name) const { afb_event_t e; afb_api_new_event(api_, name, &e); return event(e); }
inline event api::new_event(const std::string &name) const { return new_event(name.c_str()); }
inline int api::event_handler_add(const char *pattern, api::event_cb callback, void *closure) const { return afb_api_event_handler_add(api_, pattern, callback, closure); }
inline int api::event_handler_add(const std::string &pattern, api::event_cb callback, void *closure) const { return afb_api_event_handler_add(api_, pattern.c_str(), callback, closure); }
inline int api::event_handler_del(const char *pattern, void **closure) const { return afb_api_event_handler_del(api_, pattern, closure); }
inline int api::event_handler_del(const std::string &pattern, void **closure) const { return afb_api_event_handler_del(api_, pattern.c_str(), closure); }
inline int api::set_verbs(const afb_verb_t *verbs) const { return afb_api_set_verbs(api_, verbs); }
inline int api::add_verb(const char *verb, const char *info, api::verb_cb callback, void *vcbdata, const struct afb_auth *auth, uint32_t session, int glob) const { return afb_api_add_verb(api_, verb, info, callback, vcbdata, auth, session, glob); }
inline int api::add_verb(const std::string &verb, const std::string &info, api::verb_cb callback, void *vcbdata, const struct afb_auth *auth, uint32_t session, int glob) const { return afb_api_add_verb(api_, verb.c_str(), info.c_str(), callback, vcbdata, auth, session, glob); }
inline int api::del_verb(const char *verb, void **vcbdata) const { return afb_api_del_verb(api_, verb, vcbdata); }
inline int api::del_verb(const std::string &verb, void **vcbdata) const { return afb_api_del_verb(api_, verb.c_str(), vcbdata); }
inline int api::provide_class(const char *name) const { return afb_api_provide_class(api_, name); }
inline int api::provide_class(const std::string &name) const { return afb_api_provide_class(api_, name.c_str()); }
inline int api::require_class(const char *name) const { return afb_api_require_class(api_, name); }
inline int api::require_class(const std::string &name) const { return afb_api_require_class(api_, name.c_str()); }
inline void api::seal() const { afb_api_seal(api_); }
inline int api::delete_api() const { return afb_api_delete(api_); }

/*************************************************************************/
/* effective members of class afb::event                                 */
/*************************************************************************/

inline event::event() : event_{nullptr} { }
inline event::event(afb_event_t e) : event_{e} { }
inline event::event(event &&other) : event_{other.extract()} { }
inline event::event(const event &other) : event_{other.twin()} { }
inline event::~event() { }
inline event &event::operator=(afb_event_t e)
{
	afb_event_t x = std::exchange(event_, e);;
	if (x)
		afb_event_unref(x);
	return *this;
}
inline event &event::operator=(const event &other) { return *this = other.twin(); }
inline event &event::operator=(event &&other) { return *this = other.extract(); }

inline afb_event_t event::extract() { return std::exchange(event_, nullptr); }
inline afb_event_t event::twin() const { return afb_event_addref(event_); }
inline event::operator afb_event_t() const { return event_; }

inline event::operator bool() const { return is_valid(); }
inline bool event::is_valid() const { return afb_event_is_valid(event_); }

inline int event::broadcast(unsigned nparams, afb_data_t const params[]) const { return afb_event_broadcast(event_, nparams, params); }
inline int event::push(unsigned nparams, afb_data_t const params[]) const { return afb_event_push(event_, nparams, params); }
template <unsigned n> int event::broadcast(const dataset<n> &params) const { return broadcast(params.count(), params.data()); }
template <unsigned n> int event::push(const dataset<n> &params) const { return push(params.count(), params.data()); }

inline void event::unref() { if (event_) afb_event_unref(event_); event_ = nullptr; }
inline void event::addref() { if (event_) afb_event_addref(event_); }
inline const char *event::name() const { return afb_event_name(event_); }

/*************************************************************************/
/* effective members of class afb::req                                   */
/*************************************************************************/

inline req::req(afb_req_t r) : req_(r) {}
inline req::req(const req &other) : req_(other.req_) {}
inline req &req::operator=(const req &other) { req_ = other.req_; return *this; }

inline req::operator afb_req_t() const { return req_; }
inline afb_req_t req::operator->() const { return req_; }

inline req::operator bool() const { return is_valid(); }
inline bool req::is_valid() const { return afb_req_is_valid(req_); }

inline api req::api() const { return afb_req_get_api(req_); }

inline void req::reply(int status) const
	{ afb_req_reply(req_, status, 0, nullptr); }
inline void req::reply(int status, unsigned nreplies, afb_data_t const replies[]) const
	{ afb_req_reply(req_, status, nreplies, replies); }
template <unsigned n> void req::reply(int status, const dataset<n> &replies) const
	{ afb_req_reply(req_, status, replies.count(), replies.data()); }
inline void req::reply(int status, received_data replies) const
	{
		replies.addref();
		reply(status, replies.size(), replies.array());
	}

inline void req::addref() const { afb_req_addref(req_); }

inline void req::unref() const { afb_req_unref(req_); }

inline void req::session_close() const { afb_req_session_close(req_); }

inline bool req::session_set_LOA(unsigned level) const { return !afb_req_session_set_LOA(req_, level); }

inline bool req::subscribe(const event &event) const { return !afb_req_subscribe(req_, event); }

inline bool req::unsubscribe(const event &event) const { return !afb_req_unsubscribe(req_, event); }

inline void req::subcall(const char *api, const char *verb, unsigned nparams, afb_data_t const params[], int flags, void (*callback)(void *closure, int status, unsigned nreplies, afb_data_t const replies[], afb_req_t req), void *closure) const
{
	afb_req_subcall(req_, api, verb, nparams, params, flags, callback, closure);
}

template <class T>
inline void req::subcall(const char *api, const char *verb, unsigned nparams, afb_data_t const params[], int flags, void (*callback)(T *closure, int status, unsigned nreplies, afb_data_t const replies[], afb_req_t req), T *closure) const
{
	subcall(api, verb, nparams, params, flags, reinterpret_cast<afb_subcall_callback_t>(callback), reinterpret_cast<void*>(closure));
}

inline int req::subcallsync(const char *api, const char *verb, unsigned nparams, afb_data_t const params[], int flags, int &status, unsigned &nreplies, afb_data_t replies[]) const
{
	return afb_req_subcall_sync(req_, api, verb, nparams, params, flags, &status, &nreplies, replies);
}

inline void req::verbose(int level, const char *file, int line, const char * func, const char *fmt, va_list args) const
{
	afb_req_verbose(req_, level, file, line, func, fmt, args);
}

inline void req::verbose(int level, const char *file, int line, const char * func, const char *fmt, ...) const
{
	va_list args;
	va_start(args, fmt);
	afb_req_verbose(req_, level, file, line, func, fmt, args);
	va_end(args);
}

inline struct json_object *req::get_client_info() const
{
	return afb_req_get_client_info(req_);
}

/* commons */
inline api root() { api a(afbBindingRoot); return a; }
inline int broadcast_event(const char *name, unsigned nparams, afb_data_t const params[])
	{ return root().broadcast_event(name, nparams, params); }

inline event new_event(const char *name)
	{ return root().new_event(name); }

inline void verbose(int level, const char *file, int line, const char * func, const char *fmt, va_list args)
	{ root().verbose(level, file, line, func, fmt, args); }

inline void verbose(int level, const char *file, int line, const char * func, const char *fmt, ...)
	{ va_list args; va_start(args, fmt); root().verbose(level, file, line, func, fmt, args); va_end(args); }

inline int require_api(const char *apiname, bool initialized)
	{ return root().require_api(apiname, int(initialized)); }

inline int logmask()
	{ return afb_get_logmask(); }

inline bool wants_errors()
	{ return AFB_SYSLOG_MASK_WANT_ERROR(logmask()); }

inline bool wants_warnings()
	{ return AFB_SYSLOG_MASK_WANT_WARNING(logmask()); }

inline bool wants_notices()
	{ return AFB_SYSLOG_MASK_WANT_NOTICE(logmask()); }

inline bool wants_infos()
	{ return AFB_SYSLOG_MASK_WANT_INFO(logmask()); }

inline bool wants_debugs()
	{ return AFB_SYSLOG_MASK_WANT_DEBUG(logmask()); }

inline void call(const char *api, const char *verb, unsigned nparams, afb_data_t const params[], void (*callback)(void*closure, int status, unsigned nreplies, afb_data_t const replies[], afb_api_t api), void *closure)
{
	root().call(api, verb, nparams, params, callback, closure);
}

template <class T>
inline void call(const char *api, const char *verb, unsigned nparams, afb_data_t const params[], void (*callback)(T*closure, int status, unsigned nreplies, afb_data_t const replies[], afb_api_t api), T *closure)
{
	root().call(api, verb, nparams, params, reinterpret_cast<afb_call_callback_t>(callback), reinterpret_cast<void*>(closure));
}

inline int callsync(const char *api, const char *verb, unsigned nparams, afb_data_t const params[], int &status, unsigned &nreplies, afb_data_t replies[])
{
	return root().callsync(api, verb, nparams, params, status, nreplies, replies);
}

/*************************************************************************/
/* declaration of the binding's authorization s                          */
/*************************************************************************/

constexpr afb_auth auth_no()
{
	afb_auth r = { afb_auth_No, {0}, nullptr};
	r.type = afb_auth_No;
	return r;
}

constexpr afb_auth auth_yes()
{
	afb_auth r = { afb_auth_No, {0}, nullptr};
	r.type = afb_auth_Yes;
	return r;
}

constexpr afb_auth auth_token()
{
	afb_auth r = { afb_auth_No, {0}, nullptr};
	r.type = afb_auth_Token;
	return r;
}

constexpr afb_auth auth_LOA(unsigned loa)
{
	afb_auth r = { afb_auth_No, {0}, nullptr};
	r.type = afb_auth_LOA;
	r.loa = loa;
	return r;
}

constexpr afb_auth auth_permission(const char *permission)
{
	afb_auth r = { afb_auth_No, {0}, nullptr};
	r.type = afb_auth_Permission;
	r.text = permission;
	return r;
}

constexpr afb_auth auth_not(const afb_auth *other)
{
	afb_auth r = { afb_auth_No, {0}, nullptr};
	r.type = afb_auth_Not;
	r.first = other;
	return r;
}

constexpr afb_auth auth_not(const afb_auth &other)
{
	return auth_not(&other);
}

constexpr afb_auth auth_or(const afb_auth *first, const afb_auth *next)
{
	afb_auth r = { afb_auth_No, {0}, nullptr};
	r.type = afb_auth_Or;
	r.first = first;
	r.next = next;
	return r;
}

constexpr afb_auth auth_or(const afb_auth &first, const afb_auth &next)
{
	return auth_or(&first, &next);
}

constexpr afb_auth auth_and(const afb_auth *first, const afb_auth *next)
{
	afb_auth r = { afb_auth_No, {0}, nullptr};
	r.type = afb_auth_And;
	r.first = first;
	r.next = next;
	return r;
}

constexpr afb_auth auth_and(const afb_auth &first, const afb_auth &next)
{
	return auth_and(&first, &next);
}

constexpr afb_verb_t verb(
	const char *name,
	afb_req_callback_t callback,
	const char *info = nullptr,
	uint16_t session = 0,
	const afb_auth *auth = nullptr,
	bool glob = false,
	void *vcbdata = nullptr
)
{
	return { name, callback, auth, info, vcbdata, session, glob };
}

template <void (*_F_)(afb::req,afb::received_data)>
constexpr afb_verb_t verb(
	const char *name,
	const char *info = nullptr,
	uint16_t session = 0,
	const afb_auth *auth = nullptr,
	bool glob = false,
	void *vcbdata = nullptr
)
{
	return { name, verbcb<_F_>, auth, info, vcbdata, session, glob };
}

template <class _C_, void (_C_::*_F_)(afb::req,afb::received_data)>
constexpr afb_verb_t verb_client(
	const char *name,
	const char *info = nullptr,
	uint16_t session = 0,
	const afb_auth *auth = nullptr,
	bool glob = false,
	void *vcbdata = nullptr
)
{
	return { name, verbcb_client<_C_, _F_>, auth, info, vcbdata, session, glob };
}

template <class _C_, void (_C_::*_F_)(afb::req,afb::received_data) const>
constexpr afb_verb_t verb_client(
	const char *name,
	const char *info = nullptr,
	uint16_t session = 0,
	const afb_auth *auth = nullptr,
	bool glob = false,
	void *vcbdata = nullptr
)
{
	return { name, verbcb_client<_C_, _F_>, auth, info, vcbdata, session, glob };
}

template <class _C_, void (_C_::*_F_)(afb::req,afb::received_data)>
constexpr afb_verb_t verb_api(
	const char *name,
	const char *info = nullptr,
	uint16_t session = 0,
	const afb_auth *auth = nullptr,
	bool glob = false,
	void *vcbdata = nullptr
)
{
	return { name, verbcb_api<_C_, _F_>, auth, info, vcbdata, session, glob };
}

template <class _C_, void (_C_::*_F_)(afb::req,afb::received_data) const>
constexpr afb_verb_t verb_api(
	const char *name,
	const char *info = nullptr,
	uint16_t session = 0,
	const afb_auth *auth = nullptr,
	bool glob = false,
	void *vcbdata = nullptr
)
{
	return { name, verbcb_api<_C_, _F_>, auth, info, vcbdata, session, glob };
}

constexpr afb_verb_t verbend()
{
	return { 0, 0, 0, 0, 0, 0, 0 };
}

constexpr afb_binding_t binding(
	const char *name,
	const afb_verb_t *verbs,
	const char *info = nullptr,
	afb_api_callback_t mainctl = nullptr,
	const char *specification = nullptr,
	bool noconcurrency = false,
	void *userdata = nullptr
)
{
	return {
		name, specification, info, verbs, mainctl, userdata,
		nullptr, nullptr, nullptr, static_cast<unsigned>(noconcurrency) };
};

/*************************************************************************/
/***                         E N D                                     ***/
/*************************************************************************/
}
