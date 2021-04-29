/*
 * Copyright (C) 2015-2021 IoT.bzh Company
 * Author: José Bollo <jose.bollo@iot.bzh>
 *
 * SPDX-License-Identifier: LGPL-3.0-only
 */
#define AFB_BINDING_VERSION 4
#include <afb/afb-binding.h>

static const char _hello_[] = "hello world";

void hello(afb_req_t req, unsigned nparams, afb_data_t const *params)
{
	afb_data_t reply;
	afb_type_t stringz;

	AFB_REQ_DEBUG(req, "hello world");
	afb_create_data_raw(&reply, AFB_PREDEFINED_TYPE_STRINGZ, _hello_, sizeof _hello_, NULL, NULL);
	afb_req_reply(req, 0, 1, &reply);
}

const afb_verb_t verbs[] = {
	{ .verb="hello", .callback=hello },
	{ .verb=NULL }
};

const afb_binding_t afbBindingExport = {
	.api = "tuto-1",
	.verbs = verbs
};

