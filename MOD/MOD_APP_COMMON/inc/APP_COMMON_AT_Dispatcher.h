/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:   APP_COMMON_AT_Dispatcher.h
*-----------------------------------------------------------------------------
* Module Name:  APP_COMMON    
*-----------------------------------------------------------------------------
* Description:  global helper function common for all applications 
*-----------------------------------------------------------------------------
* $Date: 2010-03-01 11:05:29 +0100 (Mo, 01 Mrz 2010) $
* $Rev: 11280 $
* $Author: goldammer $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_COMMON/trunk/inc/APP_COMMON_Utils.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _APP_COMMON_AT_DISPATCHER_H
#define _APP_COMMON_AT_DISPATCHER_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include "osek.h"
/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Types                                                               */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Declarations (avoid as much as possible!)                 */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Methods                                                             */
/*----------------------------------------------------------------------------*/

extern void APP_COMMON_vHandleEvents( EventMaskType event );


#endif /* _APP_COMMON_AT_DISPATCHER_H */

/* End Of File APP_COMMON_AT_Dispatcher.h */
