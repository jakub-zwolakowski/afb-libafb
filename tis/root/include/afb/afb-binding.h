/*
 * Copyright (C) 2015-2021 IoT.bzh Company
 * Author: José Bollo <jose.bollo@iot.bzh>
 *
 * SPDX-License-Identifier: LGPL-3.0-only
 */

#pragma once

/**
 * @mainpage
 *
 * @section brief Brief introduction
 *
 * This is part of the application framework binder micro-service and
 * is provided as the API for writing bindings.
 *
 * The normal usage is to include only one file as below:
 *
 * ```C
 * #define AFB_BINDING_VERSION 3
 * #include <afb/afb-binding.h>
 * ```
 *
 */
/**
 * @file afb/afb-binding.h
 */

#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>

/**
 * @def AFB_BINDING_INTERFACE_VERSION

 *  * Version of the binding interface.
 *
 * This is intended to be test for tuning condition code.
 * It is of the form MAJOR * 1000 + REVISION.
 *
 * @see AFB_BINDING_UPPER_VERSION that should match MAJOR
 */
#define AFB_BINDING_INTERFACE_VERSION 4000

/**
 * @def AFB_BINDING_LOWER_VERSION
 *
 * Lowest binding API version supported.
 *
 * @see AFB_BINDING_VERSION
 * @see AFB_BINDING_UPPER_VERSION
 */
#define AFB_BINDING_LOWER_VERSION     3

/**
 * @def AFB_BINDING_UPPER_VERSION
 *
 * Upper binding API version supported.
 *
 * @see AFB_BINDING_VERSION
 * @see AFB_BINDING_LOWER_VERSION
 */
#define AFB_BINDING_UPPER_VERSION     4

/**
 * @def AFB_BINDING_VERSION
 *
 * This macro must be defined before including <afb/afb-binding.h> to set
 * the required binding API.
 *
 * The value of 0 is valid. It removes any version specific defines.
 */
#if !defined(AFB_BINDING_VERSION)
/* undefined version */
#error "\
\n\
\n\
  AFB_BINDING_VERSION should be defined before including <afb/afb-binding.h>\n\
  AFB_BINDING_VERSION defines the version of binding that you use.\n\
  Currently the version to use is 4 (older versions: 1 and 2 are obsolete, 3 is legacy).\n\
  Consider to add one of the following define before including <afb/afb-binding.h>:\n\
\n\
    #define AFB_BINDING_VERSION  4\n\
\n\
"

#elif AFB_BINDING_VERSION == 0
/*
 * Use of AFB_BINDING_VERSION==0 is valid.
 * It avoids to define unversionned symbols.
 * If that case, removes definition of the root.
 */
#  define AFB_BINDING_NO_ROOT 1

#elif AFB_BINDING_VERSION < AFB_BINDING_LOWER_VERSION
/* removed versions */
#  error "\
\n\
\n\
  Support of binding version this version is removed!\n\
  Please switch to binding version 4:\n\
\n\
    #define AFB_BINDING_VERSION  4\n\
\n\
"
#endif

/***************************************************************************************************/
#include "afb-binding-v3.h"
#include "afb-binding-v4.h"
