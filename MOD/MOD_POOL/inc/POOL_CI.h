
/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     POOL_CI.h
*-----------------------------------------------------------------------------
* Module Name:    POOL Handling
*-----------------------------------------------------------------------------
* Description: This file contains internal interface definitions (prototype).
*-----------------------------------------------------------------------------
* $Date: 2009-03-16 10:34:10 +0100 (Mo, 16 Mrz 2009) $
* $Rev: 5237 $
* $Author: schmidt $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_POOL/trunk/inc/POOL_CI.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _POOL_CI_H
#define _POOL_CI_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include "POOL_CE.h"

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Import configuration                                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Constants                                                           */
/*----------------------------------------------------------------------------*/

/* Markers defined for guarding the pool */
#ifdef HOSTTEST /* BEGIN DECLARE HOSTTEST */
// on  intel-processor i386
#define POOL_START_MARKER     0x30414548  /* "0AEH" */
#define POOL_END_BUSY         0x42494154  /* "BIAT" */
#define POOL_END_FREE         0x46494154  /* "FIAT" */

#else  /* ELSE DECLARE HOSTTEST */
#define POOL_START_MARKER     0x48454130  /* "HEA0" */
#define POOL_END_BUSY         0x54414942  /* "TAIB" */
#define POOL_END_FREE         0x54414946  /* "TAIF" */
#endif /* END DECLARE HOSTTEST */

#define POOL_INVALID_POOLID      0xFFFF

//#ifdef POOL_START_PATTERN
#define POOL_START_MARKER_OFFSET (sizeof(Uint32Type)) /* start Marker */
//#else 
//#define POOL_START_MARKER_OFFSET 0
//#endif

#ifdef POOL_END_PATTERN
#define POOL_END_OFFSET          (sizeof(Uint32Type)) /* end Marker */
#else
#define POOL_END_OFFSET 0
#endif

#define POOL_OFFSET              (POOL_START_MARKER_OFFSET + POOL_END_OFFSET)
#define POOL_MINIMUM_SIZE        (sizeof(Uint8Type *)) /* Minimum pool size */


/*----------------------------------------------------------------------------*/
/* Public Types                                                               */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Declarations (avoid as much as possible!)                 */
/*----------------------------------------------------------------------------*/

#endif /* _POOL_CI_H */

/* End Of File POOL_CI.h */
