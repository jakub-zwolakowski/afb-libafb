/*
 * Copyright (C) 2015-2021 IoT.bzh Company
 * Author: José Bollo <jose.bollo@iot.bzh>
 *
 * SPDX-License-Identifier: LGPL-3.0-only
 */

#pragma once

/******************************************************************************/
/* predefines */
#if AFB_BINDING_VERSION == 3

#define AFB_SESSION_RENEW		0
#define AFB_SESSION_REFRESH		0

#endif
/******************************************************************************/

/**
 * Enum for Session/Token/Assurance middleware of bindings version 2 and 3.
 */
enum afb_session_flags
{
       AFB_SESSION_LOA_MASK = 3,	/**< mask for LOA */

       AFB_SESSION_LOA_0 = 0,	/**< value for LOA of 0 */
       AFB_SESSION_LOA_1 = 1,	/**< value for LOA of 1 */
       AFB_SESSION_LOA_2 = 2,	/**< value for LOA of 2 */
       AFB_SESSION_LOA_3 = 3,	/**< value for LOA of 3 */

       AFB_SESSION_CHECK = 4,	/**< Requires token authentification */
       /* ___spare___ = 8, */
       AFB_SESSION_CLOSE = 16,	/**< After token authentification, closes the session at end */

       AFB_SESSION_NONE = 0		/**< nothing required */
};

