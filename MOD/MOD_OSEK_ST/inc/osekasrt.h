/* Kernbauer Version: 1.14 Konfiguration: TMS470 Erzeugungsgangnummer: 372 */

/*****************************************************************************
| Project Name: OSEK 2.2
|    File Name: osekasrt.h
|
|  General code: @Version: 3.54b@
|  Module version: $vv$=1.17
|
|  Description: different assertions for error detection
|
|-----------------------------------------------------------------------------
|               C O P Y R I G H T
|-----------------------------------------------------------------------------
| Copyright (c) 2000-2008 Vector Informatik GmbH               All rights reserved.
|****************************************************************************/

/* double include preventer */
#ifndef _OSEKASRT_H
#define _OSEKASRT_H

/*lint -save Messages inhibited in this file, will be re-enabled at the end of file */
/*lint -e539 Did not expect positive indentation */

/* Vector release management */
#if defined USE_QUOTE_INCLUDES
 #include "vrm.h"
#else
 #include <vrm.h>
#endif
/* KB begin vrmReleaseNumber */
/* Source release number */
#define osdVrmMajRelNum 3
#define osdVrmMinRelNum 10
/* KB end vrmReleaseNumber */
#if defined USE_QUOTE_INCLUDES
 #include "vrm.h"
#else
 #include <vrm.h>
#endif

/* KB begin osekHw_FILE_Cast */
#define osd_FILE_Cast
/* KB end osekHw_FILE_Cast */


#if osdErrorHook
 #define osAssertFailed_(x) osLastError = (uint16) (x); osAssertFailed()
#else
 #define osAssertFailed_(x) osAssertFailed()
#endif


#if ((STATUS_LEVEL == EXTENDED_STATUS) && (osdEnableAssertions))
 #ifndef osdSuppressFilenames
    #define osSysErrAssert(p, x) if ((p) == 0)\
                                 {\
                                    osDisableGlobal();\
                                    osActiveTaskModule = osd_FILE_Cast __FILE__;\
                                    osActiveTaskLineNumber = __LINE__;\
                                    osAssertFailed_(x);\
                                 }
 #else          
    #define osSysErrAssert(p, x) if ((p) == 0)\
                                 {\
                                    osDisableGlobal();\
                                    osAssertFailed_(x);\
                                 }
 #endif  /* osdSuppressFilenames */
#else /* if STATUS_LEVEL == STANDARD_STATUS */
   #define osSysErrAssert(p, x)
#endif /* STATUS_LEVEL == STANDARD_STATUS */

#if osdErrorHook
   #define osAPIError(x, y)      osLastError = (uint16) (y); osErrorHook((StatusType) (x))
#else
   #define osAPIError(x, y)
#endif

/*lint -restore re-enable messages*/

#endif /* double include preventer */

/* END OF HEADER osekasrt.h */
