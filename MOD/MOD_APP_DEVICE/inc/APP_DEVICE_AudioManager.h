/******************************************************************************/
/*****************  Copyright (c) 2010 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:   APP_DEVICE_AudioManager.h
*-----------------------------------------------------------------------------
* Module Name:  APP_DEVICE
*-----------------------------------------------------------------------------
* Description:  Global Interfaces, Enum, Structs for APP_DEVICE
*-----------------------------------------------------------------------------
* $Date: 2010-03-15 12:13:46 +0100 (Mo, 15 Mrz 2010) $
* $Rev: 11558 $
* $Author: weber $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_DEVICE/trunk/inc/APP_DEVICE.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _APP_DEVICE_AUDIOMANAGER_H
#define _APP_DEVICE_AUDIOMANAGER_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "APP_CAN.h"
/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Types                                                               */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Declarations (avoid as much as possible!)                 */
/*----------------------------------------------------------------------------*/
typedef enum
{
    APP_DEVICE_AM_enAudioUnmute = 0,
    APP_DEVICE_AM_enAudioMute
} APP_DEVICE_AM_AudioStatusType;
/*----------------------------------------------------------------------------*/
/* Public Methods                                                             */
/*----------------------------------------------------------------------------*/
extern void APP_DEVICE_AudioManager_sfInitApp(void);

extern void APP_DEVICE_AM_vOnAppCshdlNotification(APP_CSHDL_NotificationType notification);

extern SuccessFailType APP_DEVICE_AM_sfGetMute(
                                        void (*cb)(APP_COMMON_enResponseCode),
                                        APP_CAN_enSourceIdType source,
                                        Uint32Type line);

extern void APP_DEVICE_AM_vReleaseMute(void (*cb)(APP_COMMON_enResponseCode),
                                       APP_CAN_enSourceIdType source,
                                       Uint32Type line);
extern void APP_DEVICE_AM_vSetCooperativeMuteDiag(Uint8Type u8CooperativeMuteValue);
extern void APP_DEVICE_AM_vSetMuteDelayDiag(Uint16Type u16MuteDelayValue);
extern void APP_DEVICE_AM_vSetAudioMuteByWireDiag(APP_CAN_enAudioStatusType enMuteValue);

extern void APP_DEVICE_AM_vRadioInfoReceived(APP_CAN_RadioType radioInfo);
extern void APP_DEVICE_AM_vHandlePASA(ATAudioSourceActStateEnum pasa);

extern void APP_DEVICE_AM_vOnAppCshdlNotification(APP_CSHDL_NotificationType notification);


void APP_DEVICE_AM_cbWaitForCANAck();
void APP_DEVICE_AM_cbReleaseCanMute();
APP_CAN_enSourceIdType APP_DEVICE_AMGetActiveSource(void);
/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/

#endif /* _APP_DEVICE_AUDIOMANAGER_H */

/* End Of File APP_DEVICE_AudioManager.h */

