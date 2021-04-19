/*
 * Copyright (C) 2015-2021 IoT.bzh Company
 * Author: José Bollo <jose.bollo@iot.bzh>
 *
 * SPDX-License-Identifier: LGPL-3.0-only
 */

/**< @file afb/afb-binding-v4.h */

#pragma once

/******************************************************************************/

#include "afb-errno.h"
#include "afb-verbosity.h"
#include "afb-auth.h"
#include "afb-ctlid.h"
#include "afb-session.h"
#include "afb-req-subcall-flags.h"

#include "afb-binding-x4-itf.h"
#include "afb-binding-x4.h"

/******************************************************************************/

/**
 * @page validity-v4 Validity of a binding v4
 *
 * A binding V4 MUST have at least two exported symbols of name:
 *
 *   - @ref afbBindingV4root
 *   - @ref afbBindingV4  and/or  @ref afbBindingV4entry
 *
 * @ref afbBindingV4root is automatically created when **AFB_BINDING_VERSION == 4**
 * without programmer action, as a hidden variable linked as *weak*.
 *
 * The symbols @ref afbBindingV4  and  **afbBindingV4entry** are under control
 * of the programmer.
 *
 * The symbol @ref afbBindingV4 if defined is used, to describe
 * an API that will be declared during pre-initialization of bindings.
 *
 * The symbol @ref afbBindingV4entry if defined will be called during
 * pre-initialization.
 *
 * If @ref afbBindingV4entry and @ref afbBindingV4 are both defined, it is an
 * error to fill the field @ref preinit of @ref afbBindingV4.
 *
 * @see afb_binding_v4
 * @see afbBindingV4root
 * @see afbBindingV4entry
 * @see afbBindingV4
 */

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

/******************************************************************************/
#if AFB_BINDING_VERSION == 4

typedef struct afb_verb_v4			afb_verb_t;
typedef struct afb_binding_v4			afb_binding_t;

#if !defined(AFB_BINDING_NO_ROOT) /* use with caution, see @ref validity-v4 */
/**
 * Pre-initialization function.
 *
 * If this function is defined and exported in the produced binding
 * (shared object), it will be called during pre-initialization with the
 * rootapi defined by \ref afbBindingV4root
 *
 * @param rootapi the root api (equals to afbBindingV4root)
 *
 * @return the function must return a negative integer on error to abort the
 * initialization of the binding. Any positive or zero returned value is a
 * interpreted as a success.

 * @see afbBindingV4root
 * @see afbBindingV4
 * @see @ref validity-v4
 */
extern int afbBindingV4entry(afb_api_x4_t rootapi, afb_ctlid_t ctlid, afb_ctlarg_t ctlarg, void *userdata);

/**
 * Static definition of the root api of the binding.
 *
 * This symbol if defined describes the API of the binding.
 *
 * If it is not defined then the function @ref afbBindingV4entry must be defined
 *
 * @see afbBindingV4root
 * @see afbBindingV4entry
 * @see @ref validity-v4
 */
extern const struct afb_binding_v4 afbBindingV4;

/**
 * Get the default log mask
 */
static inline int afb_get_logmask()
{
	return afb_api_logmask(afbBindingV4root);
}

/******************************************************************************/

#define afbBindingExport	afbBindingV4
#define afbBindingRoot		afbBindingV4root
#define afbBindingEntry		afbBindingV4entry

/******************************************************************************/
#endif

/******************************************************************************/
#endif
