/*
 * Copyright (C) 2015-2021 IoT.bzh Company
 * Author: José Bollo <jose.bollo@iot.bzh>
 *
 * SPDX-License-Identifier: LGPL-3.0-only
 */

#pragma once

/********************************************************************************/

/** test if an code is for an error. It's yes when the value negative */
#define AFB_IS_ERRNO(code)            ((code) < 0)

/** Test if the code is a user error */
#define AFB_USER_ERROR_BASE           (-1000)

/** Test if the code is a user error */
#define AFB_IS_USER_ERROR(code)       ((code) <= AFB_USER_ERROR_BASE)

/** Test if the code is a binder error */
#define AFB_IS_BINDER_ERROR(code)     (AFB_IS_ERRNO(code) && !AFB_IS_USER_ERROR(code))

/********************************************************************************/

/** Any unclassified internal error */
#define AFB_ERRNO_INTERNAL_ERROR      -1    /* 500 HTTP_INTERNAL_SERVER_ERROR */

/** Specific to memory depletion */
#define AFB_ERRNO_OUT_OF_MEMORY       -2    /* 500 HTTP_INTERNAL_SERVER_ERROR */

/** Invoked API does not exist */
#define AFB_ERRNO_UNKNOWN_API         -3    /* 404 HTTP_NOT_FOUND */

/** Invoked verb does not exit */
#define AFB_ERRNO_UNKNOWN_VERB        -4    /* 404 HTTP_NOT_FOUND */

/** The service is not available */
#define AFB_ERRNO_NOT_AVAILABLE       -5    /* 501 HTTP_NOT_IMPLEMENTED */

/** The client miss authorization */
#define AFB_ERRNO_UNAUTHORIZED        -6    /* 401 HTTP_UNAUTHORIZED */

/** The client has an invalid token */
#define AFB_ERRNO_INVALID_TOKEN       -7    /* 401 HTTP_UNAUTHORIZED */

/** The client has not the permission to perform the action */
#define AFB_ERRNO_FORBIDDEN           -8    /* 402 HTTP_FORBIDDEN */

/** The scope associated to the client's token is insuficient */
#define AFB_ERRNO_INSUFFICIENT_SCOPE  -9    /* 402 HTTP_FORBIDDEN */

/** The called API is not ready */
#define AFB_ERRNO_BAD_API_STATE      -10    /* 402 HTTP_FORBIDDEN */

/** The service did not reply */
#define AFB_ERRNO_NO_REPLY           -11

/** Some argument of the request are invalid */
#define AFB_ERRNO_INVALID_REQUEST    -12

/** An item is missing, depend on the context */
#define AFB_ERRNO_NO_ITEM            -13

/** The current state can accept the request */
#define AFB_ERRNO_BAD_STATE          -14

/** Connection to the service is broken */
#define AFB_ERRNO_DISCONNECTED       -15

