/*
 * Copyright (C) 2015-2020 IoT.bzh Company
 * Author: José Bollo <jose.bollo@iot.bzh>
 *
 * $RP_BEGIN_LICENSE$
 * Commercial License Usage
 *  Licensees holding valid commercial IoT.bzh licenses may use this file in
 *  accordance with the commercial license agreement provided with the
 *  Software or, alternatively, in accordance with the terms contained in
 *  a written agreement between you and The IoT.bzh Company. For licensing terms
 *  and conditions see https://www.iot.bzh/terms-conditions. For further
 *  information use the contact form at https://www.iot.bzh/contact.
 *
 * GNU General Public License Usage
 *  Alternatively, this file may be used under the terms of the GNU General
 *  Public license version 3. This license is as published by the Free Software
 *  Foundation and appearing in the file LICENSE.GPLv3 included in the packaging
 *  of this file. Please review the following information to ensure the GNU
 *  General Public License requirements will be met
 *  https://www.gnu.org/licenses/gpl-3.0.html.
 * $RP_END_LICENSE$
 */

/*
 * Predefined types declared in constant initialized memory
 * It can then take placr in some read only memory.
 * For convenience, the symbols are exported as writeable
 * but the program takes care of the flag FLAG_IS_PREDEFINED
 * to avoid modifying such read only data.
 */

#include "libafb-config.h"

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>
#include <stdio.h>

#include "afb-type.h"
#include "afb-type-internal.h"
#include "afb-type-predefined.h"

#include "afb-data.h"

#include "sys/x-errno.h"
#include "utils/jsonstr.h"

#include <json-c/json.h>
#if !defined(JSON_C_TO_STRING_NOSLASHESCAPE)
#define JSON_C_TO_STRING_NOSLASHESCAPE 0
#endif

/*****************************************************************************/
/*****************************************************************************/
/**                                                                         **/
/**                   OPAQUE HELPERS                                        **/
/**                                                                         **/
/*****************************************************************************/
/*****************************************************************************/

#define OPAQUE_KEY      "#@"
#define OPAQUE_FMT_RD   OPAQUE_KEY"%x"
#define OPAQUE_FMT_WR   OPAQUE_KEY"%04x"
#define OPAQUE_BUFSIZE  (sizeof(OPAQUE_KEY) + 4)

static
int
opaque_from_string(
	const char *string,
	char term,
	struct afb_type *type,
	struct afb_data **out
) {
	int rc, p, opaqueid;
	struct afb_data *found;
	struct afb_type *otype;

	rc = sscanf(string, OPAQUE_FMT_RD "%n", &opaqueid, &p);
	if (rc != 1 || string[p] != term) {
		rc = X_EINVAL;
	}
	else {
		rc = afb_data_get_opacified(opaqueid, &found, &otype);
		if (rc >= 0) {
			if (type == otype)
				*out = found;
			else {
				afb_data_unref(found);
				rc = X_ENOENT;
			}
		}
	}
	return rc;
}

static
int
opaque_to_string(
	struct afb_data *in,
	char *buffer,
	size_t size
) {
	int opaqueid, rc;

	opaqueid = afb_data_opacify(in);
	if (opaqueid < 0) {
		rc = opaqueid;
	}
	else {
		rc = snprintf(buffer, size, OPAQUE_FMT_WR, opaqueid);
		if (rc < 0 || rc >= size) {
			rc = X_EFAULT;
		}
	}
	return rc;
}

/*****************************************************************************/
/*****************************************************************************/
/**                                                                         **/
/**         PREDEFINED TYPES  -  ROUTINES                                   **/
/**                                                                         **/
/*****************************************************************************/
/*****************************************************************************/

#define PREDEF(stype) afb_type_predefined_##stype
#define ALIAS(stype)  __attribute__((alias("predefined_" #stype)))

#define CONVERT(ftype,ttype) \
	static int convert_##ftype##_to_##ttype(\
		void *closure,\
		struct afb_data *in,\
		struct afb_type *type,\
		struct afb_data **out \
	)

#define PREDEFINED_OPERATION(stype) \
		static const struct opdesc opcvt_##stype[] =

#define CONVERT_TO(ftype,ttype) \
	{ .kind = Convert_To, .type = &PREDEF(ttype), \
	  .converter = convert_##ftype##_to_##ttype, .closure = 0 }

#define PREDEFINED_TYPE(stype,flag,super,nxt) \
	extern struct afb_type ALIAS(stype) PREDEF(stype);  \
	static const struct afb_type predefined_##stype = \
	{ \
		.name = AFB_PREFIX_PREDEF_TYPE #stype, \
		.next = nxt, \
		.operations = (struct opdesc*)opcvt_##stype,\
		.family = super, \
		.flags = FLAG_IS_PREDEFINED | flag, \
		.op_count = (uint16_t)(sizeof opcvt_##stype / sizeof opcvt_##stype[0]) \
	}

/*****************************************************************************/
/* PREDEFINED OPAQUE */

CONVERT(opaque,stringz)
{
	int rc;
	char buffer[OPAQUE_BUFSIZE], *s;
	unsigned sz;

	rc = opaque_to_string(in, buffer, sizeof buffer);
	if (rc >= 0) {
		sz = (size_t)(rc + 1);
		s = malloc(sz);
		if (s == NULL) {
			rc = X_ENOMEM;
		}
		else {
			rc = afb_data_create_raw(out, type, s, sz, free, s);
			if (rc >= 0)
				memcpy(s, buffer, sz);
		}
	}
	return rc;
}

CONVERT(opaque,json)
{
	int rc;
	char buffer[OPAQUE_BUFSIZE], *s;
	unsigned sz;

	rc = opaque_to_string(in, buffer, sizeof buffer);
	if (rc >= 0) {
		sz = (size_t)(rc + 3);
		s = malloc(sz);
		if (s == NULL) {
			rc = X_ENOMEM;
		}
		else {
			rc = afb_data_create_raw(out, type, s, sz, free, s);
			if (rc >= 0) {
				s[--sz] = 0;
				s[--sz] = s[0] = '"';
				memcpy(s + 1, buffer, --sz);
			}
		}
	}
	return rc;
}

CONVERT(opaque,json_c)
{
	int rc;
	char buffer[OPAQUE_BUFSIZE];
	json_object *json;

	rc = opaque_to_string(in, buffer, sizeof buffer);
	if (rc >= 0) {
		json = json_object_new_string(buffer);
		if (json == NULL)
			rc = X_ENOMEM;
		else
			rc = afb_data_create_raw(out, type, json, 0, (void*)json_object_put, json);
	}
	return rc;
}

PREDEFINED_OPERATION(opaque)
	{
		CONVERT_TO(opaque, stringz),
		CONVERT_TO(opaque, json),
		CONVERT_TO(opaque, json_c)
	};

PREDEFINED_TYPE(opaque, FLAG_IS_OPAQUE, 0, 0);

/*****************************************************************************/
/* PREDEFINED STRINGZ */

CONVERT(stringz,opaque)
{
	int rc;
	const char *s;

	s = afb_data_const_pointer(in);
	rc = opaque_from_string(s, 0, type, out);
	return rc;
}

CONVERT(stringz,json)
{
	const char *istr;
	char *ostr;
	size_t isz, osz;

	/* null is still null */
	istr = afb_data_const_pointer(in);
	if (!istr)
		return afb_data_create_raw(out, type, "null", 5, 0, 0);

	/* empty case is optimized */
	isz = afb_data_size(in);
	if (isz <= 1)
		return afb_data_create_raw(out, type, "\"\"", 3, 0, 0);

	/* compute escaped size and allocate it */
	osz = jsonstr_string_escape_length(istr, isz - 1);
	ostr = malloc(3 + osz);
	if (!ostr)
		return X_ENOMEM;

	/* make the json string */
	ostr[0] = '"';
	jsonstr_string_escape_unsafe(&ostr[1], istr, isz - 1);
	ostr[osz + 1] = '"';
	ostr[osz + 2] = 0;

	/* create the data */
	return afb_data_create_raw(out, type, ostr, 3 + osz, free, ostr);
}

PREDEFINED_OPERATION(stringz)
	{
		CONVERT_TO(stringz, opaque),
		CONVERT_TO(stringz, json)
	};

PREDEFINED_TYPE(stringz, FLAG_IS_STREAMABLE, 0, &PREDEF(opaque));

/*****************************************************************************/
/* PREDEFINED JSON */

CONVERT(json,json_c)
{
	int rc;
	json_object *json;
	enum json_tokener_error jerr;
	const char *str;

	str = afb_data_const_pointer(in);
	json = json_tokener_parse_verbose(str, &jerr);
	if (jerr != json_tokener_success)
		json = json_object_new_string(str);
	rc = afb_data_create_raw(out, type, json, 0, (void*)json_object_put, json);
	return rc;
}

CONVERT(json,opaque)
{
	int rc;
	const char *s;

	s = afb_data_const_pointer(in);
	if (*s++ != '"')
		rc = X_EINVAL;
	else
		rc = opaque_from_string(s, '"', type, out);
	return rc;
}

PREDEFINED_OPERATION(json)
	{
		CONVERT_TO(json, opaque),
		CONVERT_TO(json, json_c)
	};

PREDEFINED_TYPE(json, FLAG_IS_STREAMABLE, &PREDEF(stringz), &PREDEF(stringz));

/*****************************************************************************/
/* PREDEFINED JSON-C */

CONVERT(json_c,json)
{
	struct json_object *object;
	const char *jsonstr;
	size_t sz;
	int rc;

	object = (struct json_object *)afb_data_const_pointer(in);
#if JSON_C_VERSION_NUM >= 0x000D00
	jsonstr = json_object_to_json_string_length(object, JSON_C_TO_STRING_NOSLASHESCAPE, &sz);
#else
	jsonstr = json_object_to_json_string_ext(object, JSON_C_TO_STRING_NOSLASHESCAPE);
	sz = strlen(jsonstr);
#endif
	if (jsonstr == NULL)
		rc = X_ENOMEM;
	else {
		json_object_get(object);
		rc = afb_data_create_raw(out, type, (void*)jsonstr, sz + 1, (void*)json_object_put, object);
		if (rc < 0)
			json_object_put(object);
	}
	return rc;
}

CONVERT(json_c,opaque)
{
	int rc;
	struct json_object *object;
	const char *s;

	object = (struct json_object *)afb_data_const_pointer(in);
	if (!json_object_is_type(object, json_type_string))
		rc = X_EINVAL;
	else {
		s = json_object_get_string(object);
		rc = opaque_from_string(s, 0, type, out);
	}
	return rc;
}

PREDEFINED_OPERATION(json_c)
	{
		CONVERT_TO(json_c, opaque),
		CONVERT_TO(json_c, json)
	};

PREDEFINED_TYPE(json_c, 0, 0, &PREDEF(json));

/*****************************************************************************/

extern struct afb_type ALIAS(json_c) _afb_type_head_of_predefineds_;

