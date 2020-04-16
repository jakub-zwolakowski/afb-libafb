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

#include "libafb-config.h"

#if WITH_DYNAMIC_BINDING

#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

#include "sys/x-dynlib.h"
#include "apis/afb-api-so.h"
#include "apis/afb-api-so-v3.h"
#include "sys/verbose.h"
#include "core/afb-sig-monitor.h"

struct safe_dlopen
{
	const char *path;
	x_dynlib_t *dynlib;
	int global;
	int lazy;
	int status;
};

static void safe_dlopen_cb(int sig, void *closure)
{
	struct safe_dlopen *sd = closure;
	if (!sig)
		sd->status = x_dynlib_open(sd->path, sd->dynlib, sd->global, sd->lazy);
	else {
		ERROR("dlopen of %s raised signal %s", sd->path, strsignal(sig));
		sd->status = X_EINTR;
	}
}

static int safe_dlopen(const char *filename, x_dynlib_t *dynlib, int global, int lazy)
{
	struct safe_dlopen sd;
	sd.path = filename;
	sd.global = global;
	sd.lazy = lazy;
	sd.dynlib = dynlib;
	afb_sig_monitor_run(0, safe_dlopen_cb, &sd);
	return sd.status;
}

static int load_binding(const char *path, int force, struct afb_apiset *declare_set, struct afb_apiset * call_set)
{
	int rc;
	x_dynlib_t dynlib;

	// This is a loadable library let's check if it's a binding
	rc = safe_dlopen(path, &dynlib, 0, 0);
	if (rc) {
		_VERBOSE_(
			force ? Log_Level_Error : Log_Level_Notice,
			"binding [%s] not loadable: %s",
				path,
				rc == X_EINTR ? "signal raised" : x_dynlib_error(&dynlib)
		);
		goto error;
	}
	/*
	 * This is a loadable library let's check if it's a binding ...
	 */

	/* try the version 3 */
	rc = afb_api_so_v3_add(path, &dynlib, declare_set, call_set);
	if (rc < 0)
		/* error when loading a valid v3 binding */
		goto error2;

	if (rc)
		return 0; /* yes version 3 */

	/* not a valid binding */
	_VERBOSE_(force ? Log_Level_Error : Log_Level_Info, "binding [%s] %s",
					path, "isn't a supported AFB binding");

error2:
	x_dynlib_close(&dynlib);
error:
	return force ? rc : 0;
}


int afb_api_so_add_binding(const char *path, struct afb_apiset *declare_set, struct afb_apiset * call_set)
{
	return load_binding(path, 1, declare_set, call_set);
}

#if WITH_DIRENT
static int adddirs(char path[PATH_MAX], size_t end, struct afb_apiset *declare_set, struct afb_apiset * call_set, int failstops)
{
	DIR *dir;
	struct dirent *dent;
	struct stat st;
	size_t len;
	int rc = 0;

	/* open the DIR now */
	dir = opendir(path);
	if (dir == NULL) {
		ERROR("can't scan binding directory %s, %m", path);
		return -errno;
	}
	INFO("Scanning dir=[%s] for bindings", path);

	/* scan each entry */
	if (end)
		path[end++] = '/';
	for (;;) {
		errno = 0;
		dent = readdir(dir);
		if (dent == NULL) {
			if (errno != 0)
				ERROR("read error while scanning directory %.*s: %m", (int)(end - 1), path);
			break;
		}

		/* get the name and inspect dereferenced link instead of the directory entry */
		len = strlen(dent->d_name);
		if (len + end >= PATH_MAX) {
			ERROR("path too long while scanning bindings for %.*s%s", (int)end, path, dent->d_name);
			continue;
		}
		memcpy(&path[end], dent->d_name, len+1);
		rc = stat(path, &st);
		if (rc < 0) {
			ERROR("getting status of %s failed: %m", path);
			continue;
		}
		else if (S_ISDIR(st.st_mode)) {
			/* case of directories */
			if (dent->d_name[0] == '.') {
/*
Exclude from the search of bindings any
directory starting with a dot (.) by default.

It is possible to reactivate the previous behaviour
by defining the following preprocessor variables

 - AFB_API_SO_ACCEPT_DOT_PREFIXED_DIRS

   When this variable is defined, the directories
   starting with a dot are searched except
   if their name is "." or ".." or ".debug"

 - AFB_API_SO_ACCEPT_DOT_DEBUG_DIRS

   When this variable is defined and the variable
   AFB_API_SO_ACCEPT_DOT_PREFIXED_DIRS is also defined
   scans any directory not being "." or "..".

The previous behaviour was like difining the 2 variables,
meaning that only . and .. were excluded from the search.

This change is intended to definitely solve the issue
SPEC-662. Yocto installed the debugging symbols in the
subdirectory .debug. For example the binding.so also
had a .debug/binding.so file attached. Opening that
debug file made dlopen crashing.
See https://sourceware.org/bugzilla/show_bug.cgi?id=22101
 */
#if !defined(AFB_API_SO_ACCEPT_DOT_PREFIXED_DIRS) /* not defined by default */
				continue; /* ignore any directory beginning with a dot */
#else
				if (len == 1)
					continue; /* . */
				if (dent->d_name[1] == '.' && len == 2)
					continue; /* .. */
#if !defined(AFB_API_SO_ACCEPT_DOT_DEBUG_DIRS) /* not defined by default */
				if (len == 6
				 && dent->d_name[1] == 'd'
				 && dent->d_name[2] == 'e'
				 && dent->d_name[3] == 'b'
				 && dent->d_name[4] == 'u'
				 && dent->d_name[5] == 'g')
					continue; /* .debug */
#endif
#endif
			}
			rc = adddirs(path, end+len, declare_set, call_set, failstops);
		} else if (S_ISREG(st.st_mode)) {
			/* case of files */
			if (memcmp(&dent->d_name[len - 3], ".so", 4))
				continue;
			rc = load_binding(path, 0, declare_set, call_set);
		}
		if (rc < 0 && failstops) {
			closedir(dir);
			return rc;
		}
	}
	closedir(dir);
	return 0;
}

int afb_api_so_add_directory(const char *path, struct afb_apiset *declare_set, struct afb_apiset * call_set, int failstops)
{
	size_t length;
	char buffer[PATH_MAX];

	length = strlen(path);
	if (length >= sizeof(buffer)) {
		ERROR("path too long %lu [%.99s...]", (unsigned long)length, path);
		return X_ENAMETOOLONG;
	}

	memcpy(buffer, path, length + 1);
	return adddirs(buffer, length, declare_set, call_set, failstops);
}

int afb_api_so_add_path(const char *path, struct afb_apiset *declare_set, struct afb_apiset * call_set, int failstops)
{
	struct stat st;
	int rc;

	rc = stat(path, &st);
	if (rc < 0) {
		ERROR("Invalid binding path [%s]: %m", path);
		return -errno;
	}
	if (S_ISDIR(st.st_mode))
		rc = afb_api_so_add_directory(path, declare_set, call_set, failstops);
	else if (strstr(path, ".so"))
		rc = load_binding(path, 0, declare_set, call_set);
	else
		INFO("not a binding [%s], skipped", path);
	return rc;
}

int afb_api_so_add_pathset(const char *pathset, struct afb_apiset *declare_set, struct afb_apiset * call_set, int failstops)
{
	static char sep[] = ":";
	char *ps, *p;
	int rc;

	ps = strdupa(pathset);
	for (;;) {
		p = strsep(&ps, sep);
		if (!p)
			return 0;
		rc = afb_api_so_add_path(p, declare_set, call_set, failstops);
		if (rc < 0)
			return rc;
	}
}

int afb_api_so_add_pathset_fails(const char *pathset, struct afb_apiset *declare_set, struct afb_apiset * call_set)
{
	return afb_api_so_add_pathset(pathset, declare_set, call_set, 1);
}

int afb_api_so_add_pathset_nofails(const char *pathset, struct afb_apiset *declare_set, struct afb_apiset * call_set)
{
	return afb_api_so_add_pathset(pathset, declare_set, call_set, 0);
}

#endif
#endif
