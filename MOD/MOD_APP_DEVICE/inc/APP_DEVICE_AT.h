/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:   APP_DEVICE_AT.h
*-----------------------------------------------------------------------------
* Module Name:  APP_DEVICE    
*-----------------------------------------------------------------------------
* Description:  Global Interfaces, Enum, Structs for APP_DEVICE
*-----------------------------------------------------------------------------
* $Date: 2009-12-22 08:15:54 +0100 (Di, 22 Dez 2009) $
* $Rev: 10353 $
* $Author: weber $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_DEVICE/trunk/inc/APP_DEVICE.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _APP_DEVICE_AT_H
#define _APP_DEVICE_AT_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include "common.h"
#include "APP_COMMON_Utils.h"
/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "APP_DEVICE_CE.h"
/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Declarations (avoid as much as possible!)                 */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Methods                                                             */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------*/
/* Prototypes of module internal Methods                                           */
/*----------------------------------------------------------------------------*/

SuccessFailType _APP_DEVICE_sfStartServiceATRequest(   const SCI_ST    *pUart,
                                                    AT_DeviceId     deviceId,
                                                    ATBTServiceEnum service);

SuccessFailType _APP_DEVICE_sfStopServiceATRequest( const SCI_ST    *pUart,  
                                                 AT_DeviceId     deviceId,
                                                 ATBTServiceEnum service);

SuccessFailType _APP_DEVICE_sfConnectDeviceATRequest( const SCI_ST    *pUart,
                                                   AT_DeviceId     deviceId);

SuccessFailType _APP_DEVICE_sfDisconnectDeviceATRequest( const SCI_ST    *pUart,
                                                   AT_DeviceId     deviceId);

SuccessFailType _APP_DEVICE_sfSendGetImsiRequest(   const SCI_ST    *pUart);

#endif /* _APP_DEVICE_AT_H */

/* End Of File APP_DEVICE_Connection.h */

