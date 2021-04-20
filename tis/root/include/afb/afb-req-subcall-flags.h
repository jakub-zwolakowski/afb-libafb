/*
 * Copyright (C) 2015-2021 IoT.bzh Company
 * Author: José Bollo <jose.bollo@iot.bzh>
 *
 * SPDX-License-Identifier: LGPL-3.0-only
 */

#pragma once

/** @addtogroup AFB_REQ
 *  @{ */

/**
 * subcall flags
 *
 * When making subcalls, it is now possible to explicitely set the subcall
 * mode to a combination of the following flags using binary OR.
 *
 * In particular, the following combination of flags are to be known:
 *
 *  - for **subcall** having a similar behaviour to the subcalls of bindings
 *    version 1 and 2: afb_req_x2_subcall_pass_events|afb_req_x2_subcall_on_behalf
 *
 *  - for **subcall** having the behaviour of the **call**:
 *    afb_req_x2_subcall_catch_events|afb_req_x2_subcall_api_session
 *
 * Be aware that if none of mode  afb_req_x2_subcall_catch_events or
 * afb_req_x2_subcall_pass_events is set, subscrption to events will be ignored.
 */
enum afb_req_subcall_flags
{
	/**
	 * the calling API wants to receive the events from subscription
	 */
	afb_req_subcall_catch_events = 1,

	/**
	 * the original request will receive the events from subscription
	 */
	afb_req_subcall_pass_events = 2,

	/**
	 * the calling API wants to use the credentials of the original request
	 */
	afb_req_subcall_on_behalf = 4,

	/**
	 * the calling API wants to use its session instead of the one of the
	 * original request
	 */
	afb_req_subcall_api_session = 8,
};

typedef enum afb_req_subcall_flags afb_req_subcall_flags_t;

/** @} */
