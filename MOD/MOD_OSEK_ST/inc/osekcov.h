/* Kernbauer Version: 1.14 Konfiguration: TMS470 Erzeugungsgangnummer: 372 */

/*****************************************************************************
| Project Name: OSEK 2.2
|    File Name: osekcov.h
|
|  General code: @Version: 3.54b@
|  Module version: $vv$=1.01
|
|  Description: contains the implementation of the coverage macro
|
|-----------------------------------------------------------------------------
|               C O P Y R I G H T
|-----------------------------------------------------------------------------
| Copyright (c) 2007       Vector Informatik GmbH          All rights reserved.
|****************************************************************************/

/* double include preventer */
#ifndef _OSEKCOV_H
#define _OSEKCOV_H

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

/* KB begin osekCoverageMacro */
#if defined osdEnableCoverage
   /* size of coverage array */
   #define osdCoverageArraySize ((1038 / 8) + 1)

   /* external of coverage array */
   extern osqRAM1 osqRAM2 uint8 osqRAM3 osCoverage[osdCoverageArraySize];

   /* coverage enabled -> the macros will set the selected bit in the array */
   #define COVER(x)                     osCoverage[(x) >> 3U] |= (0x1U << ((x) & 0x7U));
   #define ELSEWITHCOVER(x)             else { COVER(x) }
#else
   /* no coverage -> macros are empty */
   #define COVER(x)
   #define ELSEWITHCOVER(x)
#endif
/* KB end osekCoverageMacro */

#endif /* double include preventer */
