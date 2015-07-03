/* Kernbauer Version: 1.14 Konfiguration: TMS470 Erzeugungsgangnummer: 372 */

/*****************************************************************************
| Project Name: Vector Release Management (VRM)
|    File Name: vrm.h
|
|  General code: @Version: 3.54b@
|  Module version: $vv$=1.04
|
|  Description: header file for release management
|               (The actual number of the newest relrease is stored in this
|               file)
|
|-----------------------------------------------------------------------------
|               C O P Y R I G H T
|-----------------------------------------------------------------------------
| Copyright (c) 2000-2008 Vector Informatik GmbH          All rights reserved.
|****************************************************************************/

/* PRQA S 0883 EOF */ /* this file is intentionally included multiple times */

/* double include preventer */
#ifndef __VRM__1__
#define __VRM__1__
 

/* KB begin vrmReleaseNumberReference */
/* source VRM check               */
#define osdVrmMajRelNumRef 3
#define osdVrmMinRelNumRef 10

/* generator VRM check            */
#define osdVrmGenMajRelNumRef 3
#define osdVrmGenMinRelNumRef 10
/* KB end vrmReleaseNumberReference */


#else /* __VRM__1__ */

#ifndef __VRM__2__
#define __VRM__2__


/* source VRM checks */
#if defined osdVrmMajRelNum && defined osdVrmMinRelNum

#if osdVrmMajRelNum!=osdVrmMajRelNumRef
#error "VRM: major release number mismatch (Source)"
#endif

#if osdVrmMinRelNum!=osdVrmMinRelNumRef
#error "VRM: minor release number mismatch (Source)"
#endif

#endif /* osdVrmMajRelNum && osdVrmMinRelNum */


/* generator VRM checks */
#if defined osdVrmGenMajRelNum && defined osdVrmGenMinRelNum

#if osdVrmGenMajRelNum!=osdVrmGenMajRelNumRef
#error "VRM: major release number mismatch (Generator)"
#endif

#if osdVrmGenMinRelNum!=osdVrmGenMinRelNumRef
#error "VRM: minor release number mismatch (Generator)"
#endif

#endif /* osdVrmGenMajRelNum && osdVrmGenMinRelNum */

#ifdef osdVrmMajRelNum
#undef osdVrmMajRelNum
#endif

#ifdef osdVrmMinRelNum
#undef osdVrmMinRelNum
#endif

#ifdef osdVrmGenMajRelNum
#undef osdVrmGenMajRelNum
#endif

#ifdef osdVrmGenMinRelNum
#undef osdVrmGenMinRelNum
#endif

#endif /* __VRM__2__ */
#undef  __VRM__2__

#endif /* __VRM__1__ */

