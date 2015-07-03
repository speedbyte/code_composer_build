/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:   APP_DEVICE_Connection.h
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

#ifndef _APP_DEVICE_CONNECTION_H
#define _APP_DEVICE_CONNECTION_H

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

 SuccessFailType
_APP_DEVICE_sfHandlePendingActivate(const SCI_ST * puart, AppDeviceServiceActivateType *pAct);
SuccessFailType
_APP_DEVICE_sfHandlePendingDeactivate(const SCI_ST * puart);
SuccessFailType 
_APP_DEVICE_sfHandlePendingMdActivate(const SCI_ST* pUart, AppDeviceActPlayerType* pActSvc);
SuccessFailType 
_APP_DEVICE_sfHandlePendingMdDeactivate(const SCI_ST* pUart, AppDeviceDeactPlayerType* pDeactSvc);


void _APP_DEVICE_vOnPhoneServiceActivationResult(APP_COMMON_enResponseCode response);
void _APP_DEVICE_vOnServiceActivationResult(APP_COMMON_enResponseCode result, ERMExtendedErrorCode errorcode);
void _APP_DEVICE_vOnServiceDeactivationResult(APP_COMMON_enResponseCode result);
void _APP_DEVICE_vOnPhoneServiceDeactivationResult(APP_COMMON_enResponseCode result);
void _APP_DEVICE_vHandlePlayerDisconnected();

 SuccessFailType 
_APP_DEVICE_sfHandlePendingStopRingtoneNonBlock();

SuccessFailType _APP_DEVICE_sfHandlePendingDisconnectDevice();



//void _APP_DEVICE_vHandleHeadsetStateChanged(void);
void _APP_DEVICE_Connection_vHandlePhoneNotification(const ERMNotificationPbsnType * pPbsn);
void _APP_DEVICE_Connection_vHandleA2dpNotification(const ERMNotificationPbsnType * pPbsn);
void _APP_DEVICE_Connection_vHandleAvrcpNotification(const ERMNotificationPbsnType * pPbsn);
void _APP_DEVICE_Connection_vHandleBtConnectionNotification(const ERMNotificationPsbdType * pPsbd);
void _APP_DEVICE_Connection_vHandlePhoneStateChanged(const ERMNotificationType * pNotification);
void _APP_DEVICE_Connection_vHandlePairingNotification(const ERMNotificationPprsType * pPprs);
void _APP_DEVICE_Connection_vHandleServiceSupportNotification(const ERMNotificationPbscType * pPbsc);
void _APP_DEVICE_Connection_vOnErmNotification(const ERMNotificationType * pNotification);


/* handler to check if all requirements to fulfill fctId are met */
BooleanType _APP_DEVICE_Connection_bMdActivateFinished(AT_DeviceId devId);
BooleanType _APP_DEVICE_Connection_bMdDeactivateFinished(AT_DeviceId devId);
BooleanType _APP_DEVICE_Connection_bPhoneActivateFinished(AT_DeviceId devId);
#endif /* _APP_DEVICE_CONNECTION_H */

/* End Of File APP_DEVICE_Connection.h */

