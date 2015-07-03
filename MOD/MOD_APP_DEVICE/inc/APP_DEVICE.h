/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:   APP_DEVICE.h
*-----------------------------------------------------------------------------
* Module Name:  APP_DEVICE
*-----------------------------------------------------------------------------
* Description:  Global Interfaces, Enum, Structs for APP_DEVICE
*-----------------------------------------------------------------------------
* $Date: 2011-09-27 13:22:01 +0200 (Di, 27 Sep 2011) $
* $Rev: 21939 $
* $Author: maennel $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_DEVICE/trunk/inc/APP_DEVICE.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _APP_DEVICE_H
#define _APP_DEVICE_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include "common.h"
#include "APP_COMMON_Utils.h"
#include "APP_CSHDL.h"
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
#define APP_DEVICE_COD_SERVICE_MASK                 0x00FFE000
#define APP_DEVICE_COD_MAJOR_MASK                   0x00001F00
#define APP_DEVICE_COD_MINOR_MASK                   0x000000FC

#define APP_DEVICE_MAX_NUMBER_RETRIES_PB_SORTING    0x01



/*----------------------------------------------------------------------------*/
/* Public Types                                                               */
/*----------------------------------------------------------------------------*/

typedef enum
{
    APP_DEVICE_NOTIFY_MAILBOX_NUMBER_CHANGED,
    APP_DEVICE_NOTIFY_USER_PROFILE_ADDED,
    APP_DEVICE_NOTIFY_USER_PROFILE_DELETED,
    APP_DEVICE_NOTIFY_ALL_USER_PROFILE_DELETED,
    APP_DEVICE_NOTIFY_PLAYER_PROFILE_ADDED,
    APP_DEVICE_NOTIFY_PLAYER_PROFILE_DELETED,
    APP_DEVICE_NOTIFY_HEADSET_PROFILE_ADDED,
    APP_DEVICE_NOTIFY_HEADSET_PROFILE_DELETED,
    APP_DEVICE_NOTIFY_FACTORY_SETTING_RESET,
    APP_DEVICE_NOTIFY_USER_PROFILE_NAME_SAVED,
    APP_DEVICE_NOTIFY_A2DP_STREAMING_ACTIVE,
    APP_DEVICE_NOTIFY_OUTGOING_CALL_VIA_VCR
}APP_DEVICE_enNotificationId;

typedef enum
{
    APP_DEVICE_enTYPE_UNKNOWN,
    APP_DEVICE_enTYPE_PHONE,
    APP_DEVICE_enTYPE_HEADSET,
    APP_DEVICE_enTYPE_PLAYER
}APP_DEVICE_DeviceType;


/* Class of device: ServiceClass field */
typedef enum
{
    APP_DEVICE_COD_SERVICE_AUDIO              =  0x00200000,
    APP_DEVICE_COD_SERVICE_TELEPHONY          =  0x00400000
}APP_DEVICE_CodServiceClass;

/* Class of device: MajorDeviceClass field */
typedef enum
{
    APP_DEVICE_COD_MAJOR_MISC                 =  0x00000000,
    APP_DEVICE_COD_MAJOR_COMPUTER             =  0x00000100,
    APP_DEVICE_COD_MAJOR_PHONE                =  0x00000200,
    APP_DEVICE_COD_MAJOR_LAN_ACCESS_POINT     =  0x00000300,
    APP_DEVICE_COD_MAJOR_AUDIO                =  0x00000400,
    APP_DEVICE_COD_MAJOR_PERIPHERAL           =  0x00000500,
    APP_DEVICE_COD_MAJOR_UNCLASSIFIED         =  0x00001F00
}APP_DEVICE_CodMajorDeviceClass;

/* Class of device: MinorDeviceClass field */
typedef enum
{
    APP_DEVICE_COD_MINOR_PHONE_UNCLASSIFIED   =  0x00000200,
    APP_DEVICE_COD_MINOR_PHONE_CELLULAR       =  0x00000004,
    APP_DEVICE_COD_MINOR_PHONE_CORDLESS       =  0x00000008,
    APP_DEVICE_COD_MINOR_PHONE_SMART          =  0x0000000C,
    APP_DEVICE_COD_MINOR_PHONE_MODEM          =  0x00000010,

    APP_DEVICE_COD_MINOR_AUDIO_UNCLASSIFIED   =  0x00000000,
    APP_DEVICE_COD_MINOR_AUDIO_HEADSET        =  0x00000004,
    APP_DEVICE_COD_MINOR_AUDIO_HANDSFREE      =  0x00000008,

    APP_DEVICE_COD_MINOR_COMP_UNCLASSIFIED    =  0x00000000,
    APP_DEVICE_COD_MINOR_COMP_DESKTOP         =  0x00000004,
    APP_DEVICE_COD_MINOR_COMP_SERVER          =  0x00000008,
    APP_DEVICE_COD_MINOR_COMP_LAPTOP          =  0x0000000C,
    APP_DEVICE_COD_MINOR_COMP_HANDHELD        =  0x00000010,
    APP_DEVICE_COD_MINOR_COMP_PALM            =  0x00000014
}APP_DEVICE_CodMinorDeviceClass;

/*----------------------------------------------------------------------------*/
/* Global Variables Declarations (avoid as much as possible!)                 */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Methods                                                             */
/*----------------------------------------------------------------------------*/

SuccessFailType APP_DEVICE_sfInitApp(void);

void APP_DEVICE_vHandleATResponse(const ATRspMessageType *atRsp);

void APP_DEVICE_vHandleEvents(EventMaskType event);




APP_COMMON_enResponseCode APP_DEVICE_sfActivatePhoneDevice( AT_DeviceId deviceId,
                                                            void(*cb)(APP_COMMON_enResponseCode));
APP_COMMON_enResponseCode APP_DEVICE_sfDeactivatePhoneService( AT_DeviceId deviceId,
                                                            void(*cb)(APP_COMMON_enResponseCode));
APP_COMMON_enResponseCode APP_DEVICE_rcActivateMdiDevice( void(*cb)(APP_COMMON_enResponseCode),
                                                          AT_DeviceId deviceId);
APP_COMMON_enResponseCode APP_DEVICE_rcDeactivateMdiDevice( void(*cb)(APP_COMMON_enResponseCode));
APP_COMMON_enResponseCode APP_DEVICE_sfSetAutoConnectionMode( BooleanType enable,
                                                              void(*cb)(APP_COMMON_enResponseCode));
#include "ERM.h"
void APP_DEVICE_vHandleErmATResultEvent(APP_COMMON_enResponseCode response, ERMExtendedErrorCode errorcode);

void APP_DEVICE_vHandleErmStateChange(ERMStateType state);
void APP_DEVICE_vOnErmNotification(const ERMNotificationType * pNotification);
void APP_DEVICE_vOnAppCshdlNotification(APP_CSHDL_NotificationType notification);
#include "APP_PHONE.h"
void APP_DEVICE_vOnAppPhoneNotification(APP_PHONE_enNotificationId notification);

#include "APP_PHONEBOOK.h"
void APP_DEVICE_vOnAppPhoneBookNotification(APP_PB_enNotificationId notification);

BooleanType APP_DEVICE_bIsDeviceType(AT_BTClassOfDevice cod, APP_DEVICE_DeviceType deviceType);
BooleanType APP_DEVICE_bIsServiceClass(AT_BTClassOfDevice cod, APP_DEVICE_CodServiceClass serviceClass);
BooleanType APP_DEVICE_bIsMajorDevClass(AT_BTClassOfDevice cod, APP_DEVICE_CodMajorDeviceClass majorDevClass);
BooleanType APP_DEVICE_bIsMinorDevClass(AT_BTClassOfDevice cod, APP_DEVICE_CodMinorDeviceClass minorDevClass);
BooleanType APP_DEVICE_bIsFactorySettingActive(void);

APP_COMMON_enResponseCode APP_DEVICE_rcBackupEeprom(
        void (*cb)(APP_COMMON_enResponseCode),
        APP_CAN_ENUM_MARKE brand);

APP_COMMON_enResponseCode APP_DEVICE_rcAddUserProfile(
        void (*cb)(APP_COMMON_enResponseCode),
        AT_DeviceId deviceId,
        const Uint8Type *pName, Uint16Type length );

APP_COMMON_enResponseCode APP_DEVICE_rcDeleteUserProfile(
        void (*cb)(APP_COMMON_enResponseCode),
        Uint16Type indexOfProfile);

APP_COMMON_enResponseCode APP_DEVICE_rcAddPlayerProfile(
        void (*cb)(APP_COMMON_enResponseCode),
        AT_DeviceId deviceId,
        const Uint8Type *pName, Uint16Type length );

APP_COMMON_enResponseCode APP_DEVICE_rcAddHeadsetProfile(
        void (*cb)(APP_COMMON_enResponseCode),
        AT_DeviceId deviceId,
        const Uint8Type *pName, Uint16Type length );

APP_COMMON_enResponseCode APP_DEVICE_rcDeletePlayerProfile(
        void (*cb)(APP_COMMON_enResponseCode),
        Uint16Type indexOfProfile);

APP_COMMON_enResponseCode APP_DEVICE_rcDeleteHeadsetProfile(
        void (*cb)(APP_COMMON_enResponseCode),
        Uint16Type indexOfProfile);

APP_COMMON_enResponseCode APP_DEVICE_rcSetVoiceMailboxNumber(
        void (*cb)(APP_COMMON_enResponseCode),
        Uint8Type profileIndex,
        const Uint8Type *pNumber, Uint16Type length );

APP_COMMON_enResponseCode APP_DEVICE_rcSetRingtone(
        void (*cb)(APP_COMMON_enResponseCode),
        Uint8Type profileIndex,
        ATRintoneModeEnum ringtone );

APP_COMMON_enResponseCode APP_DEVICE_rcSetRingtoneAlternate(
        void (*cb)(APP_COMMON_enResponseCode),
        Uint8Type profileIndex,
        BooleanType bAlternate);


APP_COMMON_enResponseCode APP_DEVICE_rcSetHeadsetAuthMode(
        void (*cb)(APP_COMMON_enResponseCode),
        Uint8Type profileIndex,
        BooleanType bAuthNeeded );

APP_COMMON_enResponseCode APP_DEVICE_rcSetPlayerAuthMode(
        void (*cb)(APP_COMMON_enResponseCode),
        Uint8Type profileIndex,
        BooleanType bAuthNeeded );

APP_COMMON_enResponseCode APP_DEVICE_rcSetPbCompletelyLoaded(
        void (*cb)(APP_COMMON_enResponseCode),
        Uint8Type profileIndex);

APP_COMMON_enResponseCode APP_DEVICE_rcSetPlayerHsName(
        void (*cb)(APP_COMMON_enResponseCode),
        Uint8Type profileIndex,
        const Uint8Type *pName, Uint16Type length  );

APP_COMMON_enResponseCode APP_DEVICE_rcSetUserName(
        void (*cb)(APP_COMMON_enResponseCode),
        Uint8Type profileIndex,
        const Uint8Type *pName, Uint16Type length  );

APP_COMMON_enResponseCode APP_DEVICE_rcResetToFactorySetting(
        void (*cb)(APP_COMMON_enResponseCode));

APP_COMMON_enResponseCode APP_DEVICE_rcFactorySetDiagTriggerReset(
                                    void (*cb)(APP_COMMON_enResponseCode));

APP_COMMON_enResponseCode APP_DEVICE_rcSaveBtName(
        void (*cb)(APP_COMMON_enResponseCode),
        const Uint8Type* pData,
        Uint16Type dataLen );

APP_COMMON_enResponseCode APP_DEVICE_rcDeleteAllProfiles(
        void (*cb)(APP_COMMON_enResponseCode));

APP_COMMON_enResponseCode APP_DEVICE_rcStopRingtoneNonBlock();
APP_COMMON_enResponseCode APP_DEVICE_rcDisconnectDevice(Uint16Type deviceId);
APP_COMMON_enResponseCode APP_DEVICE_rcStartRingtoneNonBlock(ATRintoneModeEnum ringtone);

Uint8Type APP_DEVICE_u8GetAddedProfileId(APP_DEVICE_DeviceType deviceType);
AT_DeviceId APP_DEVICE_GetActiveServiceDeviceId( ATBTServiceEnum service);
AT_DeviceId APP_DEVICE_u8GetPhPlActivateDevId();
void APP_DEVICE_vAddMissingUserProfileImplicitlyBySD();
void APP_DEVICE_vAddMissingPlayerProfileImplicitlyBySD();
ATAudioSourceActStateEnum APP_DEVICE_GetPASAEvent(void);
void APP_DEVICE_InitPASAEvent(void);

void APP_DEVICE_vEmergencyReleaseContextFromAppSd(void);
void APP_DEVICE_vSetDeactivatePlayerPendingFlag(AT_DeviceId deviceId);
void APP_DEVICE_vSetActivatePlayerPendingFlag(void(*cb)(APP_COMMON_enResponseCode), AT_DeviceId deviceId);

/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/


#endif /* _APP_DEVICE_H */

/* End Of File APP_DEVICE.h */

