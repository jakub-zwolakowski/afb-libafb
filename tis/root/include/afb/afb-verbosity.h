/*
 * Copyright (C) 2015-2021 IoT.bzh Company
 * Author: José Bollo <jose.bollo@iot.bzh>
 *
 * SPDX-License-Identifier: LGPL-3.0-only
 */

#pragma once

/** @defgroup AFB_LOGGING
 *  @{ */

/**
 * Definition of verbosity levels.
 * This definition is directly inspired by the unix function syslog.
 */
enum afb_syslog_levels {
	AFB_SYSLOG_LEVEL_EMERGENCY	= 0,    /* system is unusable */
	AFB_SYSLOG_LEVEL_ALERT		= 1,    /* action must be taken immediately */
	AFB_SYSLOG_LEVEL_CRITICAL	= 2,    /* critical conditions */
	AFB_SYSLOG_LEVEL_ERROR		= 3,    /* error conditions */
	AFB_SYSLOG_LEVEL_WARNING	= 4,    /* warning conditions */
	AFB_SYSLOG_LEVEL_NOTICE		= 5,    /* normal but significant condition */
	AFB_SYSLOG_LEVEL_INFO		= 6,    /* informational */
	AFB_SYSLOG_LEVEL_DEBUG		= 7     /* debug-level messages */
};

/**
 * Expected levels are managed using a bit mask value.
 * This macro checks whether the given level is required to log.
 *
 * This is useful for avoiding the call to the logging function
 * and the evaluation of its parameters.
 * However, prefer to use the predefined dedicated macros that already include
 * the necessary logic to avoid unnecessarily calls.
 *
 *  AFB_API_ERROR, AFB_API_WARNING, AFB_API_NOTICE, AFB_API_INFO and AFB_API_DEBUG
 *  AFB_REQ_ERROR, AFB_REQ_WARNING, AFB_REQ_NOTICE, AFB_REQ_INFO and AFB_REQ_DEBUG
 *  AFB_ERROR, AFB_WARNING, AFB_NOTICE, AFB_INFO and AFB_DEBUG
 */
#define AFB_SYSLOG_MASK_WANT(verbomask,level)	((verbomask) & (1 << (level)))

#define AFB_SYSLOG_MASK_WANT_EMERGENCY(x)	AFB_SYSLOG_MASK_WANT(x,AFB_SYSLOG_LEVEL_EMERGENCY)
#define AFB_SYSLOG_MASK_WANT_ALERT(x)		AFB_SYSLOG_MASK_WANT(x,AFB_SYSLOG_LEVEL_ALERT)
#define AFB_SYSLOG_MASK_WANT_CRITICAL(x)	AFB_SYSLOG_MASK_WANT(x,AFB_SYSLOG_LEVEL_CRITICAL)
#define AFB_SYSLOG_MASK_WANT_ERROR(x)		AFB_SYSLOG_MASK_WANT(x,AFB_SYSLOG_LEVEL_ERROR)
#define AFB_SYSLOG_MASK_WANT_WARNING(x)		AFB_SYSLOG_MASK_WANT(x,AFB_SYSLOG_LEVEL_WARNING)
#define AFB_SYSLOG_MASK_WANT_NOTICE(x)		AFB_SYSLOG_MASK_WANT(x,AFB_SYSLOG_LEVEL_NOTICE)
#define AFB_SYSLOG_MASK_WANT_INFO(x)		AFB_SYSLOG_MASK_WANT(x,AFB_SYSLOG_LEVEL_INFO)
#define AFB_SYSLOG_MASK_WANT_DEBUG(x)		AFB_SYSLOG_MASK_WANT(x,AFB_SYSLOG_LEVEL_DEBUG)

/**
 * The output of the filenames by logging subsystem can be controlled by the
 * the macro AFB_BINDING_PRAGMA_SHORT_FILENAME. If that macro is defined,
 * the filenames will be shortened to its base name. Otherwise, the full
 * path of the file is used accordingly to option -fdebug-prefix-map.
 */
#if defined(AFB_BINDING_PRAGMA_SHORT_FILENAME)
#  include <string.h>
#  define __file__   (1 + (strrchr(__FILE__,'/') ?: " "__FILE__))
#else
#  define __file__   __FILE__
#endif

/*
 * Effective verbosity of compiled binding depends on the compilation variables below:
 *  - AFB_BINDING_PRAGMA_NO_VERBOSE_DATA
 *  - AFB_BINDING_PRAGMA_NO_VERBOSE_DETAILS
 *
 * If AFB_BINDING_PRAGMA_NO_VERBOSE_DATA is defined then only the filename and the line
 * of the message is reported for any message that is not an error. This mode is intended
 * to lower footprint of bindings by removing function names, text messages and companion data.
 *
 * If AFB_BINDING_PRAGMA_NO_VERBOSE_DETAILS is defined but not AFB_BINDING_PRAGMA_NO_VERBOSE_DATA
 * then no detail is given, no file name, no function name, no line of occurence. But
 * the message and its companion data is prompted.
 */
#if defined(AFB_BINDING_PRAGMA_NO_VERBOSE_DATA)

# define AFB_API_VERBOSE(api,level,...) \
		do { if(level <= AFB_SYSLOG_LEVEL_ERROR) \
			afb_api_verbose(api,level,__file__,__LINE__,NULL,__VA_ARGS__); \
		else afb_api_verbose(api,level,__file__,__LINE__,NULL,NULL); } while(0)

# define AFB_REQ_VERBOSE(req,level,...) \
		do { if(level <= AFB_SYSLOG_LEVEL_ERROR) \
			afb_req_verbose(req,level,__file__,__LINE__,NULL,__VA_ARGS__); \
		else afb_req_verbose(req,level,__file__,__LINE__,NULL,NULL); } while(0)

#elif defined(AFB_BINDING_PRAGMA_NO_VERBOSE_DETAILS)

# define AFB_API_VERBOSE(api,level,...) \
	afb_api_verbose(api,level,NULL,0,NULL,__VA_ARGS__)

# define AFB_REQ_VERBOSE(req,level,...) \
	afb_req_verbose(req,level,NULL,0,NULL,__VA_ARGS__)

#else

# define AFB_API_VERBOSE(api,level,...) \
	afb_api_verbose(api,level,__file__,__LINE__,__func__,__VA_ARGS__)

# define AFB_REQ_VERBOSE(req,level,...) \
	afb_req_verbose(req,level,__file__,__LINE__,__func__,__VA_ARGS__)

#endif

/**
 * These macro test if loging is required for the given syslog level.
 * If the level of log is required then calls the macro that invoke
 * the logging function.
 */
#define AFB_API_SYSLOG(api,llevel,...) \
        do{ if(afb_api_wants_log_level((api),(llevel))) AFB_API_VERBOSE((api),(llevel),__VA_ARGS__); }while(0)
#define AFB_REQ_SYSLOG(req,llevel,...) \
        do{ if(afb_req_wants_log_level((req),(llevel))) AFB_REQ_VERBOSE((req),(llevel),__VA_ARGS__); }while(0)

/**
 * predefined logging facility for API
 */
#define AFB_API_ERROR(api,...)		AFB_API_SYSLOG(api,AFB_SYSLOG_LEVEL_ERROR,__VA_ARGS__)
#define AFB_API_WARNING(api,...)	AFB_API_SYSLOG(api,AFB_SYSLOG_LEVEL_WARNING,__VA_ARGS__)
#define AFB_API_NOTICE(api,...)		AFB_API_SYSLOG(api,AFB_SYSLOG_LEVEL_NOTICE,__VA_ARGS__)
#define AFB_API_INFO(api,...)		AFB_API_SYSLOG(api,AFB_SYSLOG_LEVEL_INFO,__VA_ARGS__)
#define AFB_API_DEBUG(api,...)		AFB_API_SYSLOG(api,AFB_SYSLOG_LEVEL_DEBUG,__VA_ARGS__)

/**
 * predefined logging facility for REQ
 */
#define AFB_REQ_ERROR(req,...)		AFB_REQ_SYSLOG(req,AFB_SYSLOG_LEVEL_ERROR,__VA_ARGS__)
#define AFB_REQ_WARNING(req,...)	AFB_REQ_SYSLOG(req,AFB_SYSLOG_LEVEL_WARNING,__VA_ARGS__)
#define AFB_REQ_NOTICE(req,...)		AFB_REQ_SYSLOG(req,AFB_SYSLOG_LEVEL_NOTICE,__VA_ARGS__)
#define AFB_REQ_INFO(req,...)		AFB_REQ_SYSLOG(req,AFB_SYSLOG_LEVEL_INFO,__VA_ARGS__)
#define AFB_REQ_DEBUG(req,...)		AFB_REQ_SYSLOG(req,AFB_SYSLOG_LEVEL_DEBUG,__VA_ARGS__)

/**
 * predefined logging facility for implicit root API
 */
#if !defined(AFB_BINDING_NO_ROOT)
#define AFB_ERROR(...)			AFB_API_ERROR(afbBindingRoot,__VA_ARGS__)
#define AFB_WARNING(...)		AFB_API_WARNING(afbBindingRoot,__VA_ARGS__)
#define AFB_NOTICE(...)			AFB_API_NOTICE(afbBindingRoot,__VA_ARGS__)
#define AFB_INFO(...)			AFB_API_INFO(afbBindingRoot,__VA_ARGS__)
#define AFB_DEBUG(...)			AFB_API_DEBUG(afbBindingRoot,__VA_ARGS__)
#endif

/** @} */
