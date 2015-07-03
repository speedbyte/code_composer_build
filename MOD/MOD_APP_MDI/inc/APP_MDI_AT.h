/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:   APP_MDI_AT.h
*-----------------------------------------------------------------------------
* Module Name:  APP_MDI
*-----------------------------------------------------------------------------
* Description:  Module Global Interfaces, Enum, Structs for APP_MDI
*-----------------------------------------------------------------------------
* $Date: 2011-05-27 10:47:46 +0200 (Fr, 27 Mai 2011) $
* $Rev: 19889 $
* $Author: nikolov $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_MDI/trunk/inc/APP_MDI_AT.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _APP_MDI_AT_H
#define _APP_MDI_AT_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "UART.h"
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
/* Public Methods                                                             */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/

//ATAudioSourceActStateEnum APP_MDI_GetPASAEvent(void);
//void APP_MDI_InitPASAEvent(void);


//SuccessFailType _APP_MDI_AT_sfHandleInitServices(const SCI_ST* pUart);
SuccessFailType _APP_MDI_sfSendKeyboardEventATRequest( const SCI_ST *pUart, ATReqKeyboardKeysEnum key, ATReqKeyboardEventEnum event);
SuccessFailType _APP_MDI_sfSendRandomModeATRequest( const SCI_ST *pUart, ATRandomModeStatusEnum random_mode);
SuccessFailType _APP_MDI_sfSendRepeatModeATRequest( const SCI_ST *pUart, ATPlayerRepeatModeEnum repeat_mode);
SuccessFailType _APP_MDI_sfSendAutomaticPlayerNotificationATRequest( const SCI_ST *pUart, ATReqConfigPositionNotiEnum notification_mode);
SuccessFailType _APP_MDI_sfSendPlayerStateATRequest( const SCI_ST *pUart, ATDMDBActionEnum disco_mode);
SuccessFailType _APP_MDI_sfSendALCKATRequest( const SCI_ST *pUart, ATAlckLockType  lockState);
SuccessFailType _APP_MDI_sfSendGetMetadataTypeATRequest( const SCI_ST *pUart, AT_DeviceId deviceId);
SuccessFailType _APP_MDI_sfStartServiceATRequest( const SCI_ST *pUart, AT_DeviceId deviceId, ATBTServiceEnum service);
SuccessFailType _APP_MDI_sfStopServiceATRequest( const SCI_ST *pUart, AT_DeviceId deviceId, ATBTServiceEnum service);
SuccessFailType _APP_MDI_sfSendDgpsATRequest( const SCI_ST *pUart, AT_DeviceId deviceId);
SuccessFailType _APP_MDI_sfSendDgedATRequest( const SCI_ST *pUart, AT_DeviceId deviceId, Uint8Type fieldmask);
SuccessFailType _APP_MDI_sfConnectDeviceATRequest( const SCI_ST *pUart, AT_DeviceId deviceId);
SuccessFailType _APP_MDI_sfSendPbsvATRequest( const SCI_ST *pUart, ATBTServiceEnum service, ATPbsvActionType action);





#endif /* _APP_MDI_AT_H */

/* End Of File APP_MDI_AT.h */

