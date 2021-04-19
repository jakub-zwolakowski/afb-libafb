/*
 * Copyright (C) 2015-2021 IoT.bzh Company
 * Author: José Bollo <jose.bollo@iot.bzh>
 *
 * SPDX-License-Identifier: LGPL-3.0-only
 */

#pragma once

/** @defgroup AFB_CTLID
 *  @{
 *
 * Before version 3, the bindings had to declare at most
 * 3 separate functions to handle preinitialisation,
 * initialisation and unhandled events.
 *
 * Since version 4, bindings only need to implement one
 * main callback function to handle all these events.
 *
 * The new callback allows to define new events that were
 * previously missing:
 *  - root init (initialisation without declared API)
 *  - class readyness (when the required class(es) is(are) ready)
 *  - exiting (called before exiting)
 */

/**
 * Enumeration of ids of control message
 *
 * @see mainctl
 */
enum afb_ctlid
{
	/** called on root entries, the entries not linked to an API */
	afb_ctlid_Root_Entry = 0,

	/** called for the preinit of an API, has a companion argument */
	afb_ctlid_Pre_Init = 1,

	/** called for init */
	afb_ctlid_Init = 2,

	/** called when required classes are ready */
	afb_ctlid_Class_Ready = 3,

	/** called when an event is not handled */
	afb_ctlid_Orphan_Event = 4,

	/** called when shuting down */
	afb_ctlid_Exiting = 5
};

/**
 * Argument of the control message is a pointer to one of
 * the given structure of the following enumeration, depending
 * on the ctlid of the control message.
 */
union afb_ctlarg
{
	/**
	 * structure for message afb_ctlid_Root_Entry
	 */
	struct {
		/** path of the binding */
		const char *path;

		/** uid attached to the binding if any (can be NULL) */
		const char *uid;

		/** configuration attached to the binding if any (can be NULL) */
		struct json_object *config;
	} root_entry;

	/**
	 * structure for message afb_ctlid_Pre_Init
	 */
	struct {
		/** path of the binding */
		const char *path;

		/** uid attached to the binding if any (can be NULL) */
		const char *uid;

		/** configuration attached to the binding if any (can be NULL) */
		struct json_object *config;
	} pre_init;

	/**
	 * structure for message afb_ctlid_Orphan_Event
	 */
	struct {
		/** name of the of the orphan event */
		const char *name;
	} orphan_event;

	/**
	 * structure for message afb_ctlid_Exiting
	 */
	struct {
		/** exiting code */
		int code;
	} exiting;
};

/* typedefs */
typedef enum afb_ctlid afb_ctlid_t;
typedef const union afb_ctlarg *afb_ctlarg_t;

/** @} */
