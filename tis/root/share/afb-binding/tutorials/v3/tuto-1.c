/*
 * Copyright (C) 2015-2021 IoT.bzh Company
 * Author: José Bollo <jose.bollo@iot.bzh>
 *
 * SPDX-License-Identifier: LGPL-3.0-only
 */
#define AFB_BINDING_VERSION 3
#include <afb/afb-binding.h>

void hello(afb_req_t req)
{
	AFB_REQ_DEBUG(req, "hello world");
	afb_req_reply(req, NULL, NULL, "hello world");
}

const afb_verb_t verbs[] = {
	{ .verb="hello", .callback=hello },
	{ .verb=NULL }
};

const afb_binding_t afbBindingExport = {
	.api = "tuto-1",
	.verbs = verbs
};

