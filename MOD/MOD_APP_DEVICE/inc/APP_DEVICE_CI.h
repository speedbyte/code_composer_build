/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/*****************************************************************************/
/* File Name:   APP_DEVICE_CI.h
*-----------------------------------------------------------------------------
* Module Name:  APP_DEVICE
*-----------------------------------------------------------------------------
* Description:  modul global constants, declarations
*-----------------------------------------------------------------------------
* $Date: 2011-06-10 17:24:52 +0200 (Fr, 10 Jun 2011) $
* $Rev: 20109 $
* $Author: nikolov $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_DEVICE/trunk/inc/APP_DEVICE_CI.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _APP_DEVICE_CI_H
#define _APP_DEVICE_CI_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
//#include "common.h"
//#include "ATCommonData.h"
/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "APP_COMMON_Utils.h" 
#include "ATParser.h"
/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Import configuration                                                       */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of configured interface
*/

/*----------------------------------------------------------------------------*/
/* Synonym adaptions                                    */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the synonym adaption
*/

/*----------------------------------------------------------------------------*/
/* module global enums                                                        */
/*----------------------------------------------------------------------------*/
typedef enum
{
    APP_DEVICE_enResultOk      = 0x00,
    APP_DEVICE_enResultError   = 0x01
}APP_DEVICE_enResult;

typedef enum
{
/* 0  */    APP_DEVICE_INVALID_FCT_ID = 0,
/* 1  */    APP_DEVICE_PHONE_SERVICE_ACTIVATION,
/* 2  */    APP_DEVICE_PHONE_SET_AUTO_CONN_MODE,
/* 3  */    APP_DEVICE_ERM_ADD_PROFILE,
/* 4  */    APP_DEVICE_ERM_DELETE_PROFILE,
///* 5  */    APP_DEVICE_ERM_ADD_PLAYERHS_PROFILE,
/* 5  */    APP_DEVICE_ERM_DELETE_PLAYERHS_PROFILE,
/* 6  */    APP_DEVICE_ERM_CHANGE_MAILBOX_NUMBER,
/* 7  */    APP_DEVICE_ERM_UPDATE_ACTIVE_PROFILE_LIST,
/* 8  */    APP_DEVICE_ERM_ADD_MISSING_USER_PROFILE,
/* 9  */    APP_DEVICE_ERM_ADD_MISSING_PLAYER_PROFILE,
/* 10 */    APP_DEVICE_ERM_CHANGE_RINGTONE,
/* 11 */    APP_DEVICE_ERM_CHANGE_RINGTONE_MODE,
/* 12 */    APP_DEVICE_ERM_INIT_CK5050_FOR_USER,
/* 13 */    APP_DEVICE_ERM_ASSOCIATE_HEADSET_TO_USER,
/* 14 */    APP_DEVICE_ERM_ASSOCIATE_PLAYER_TO_USER,
/* 15 */    APP_DEVICE_ERM_CHANGE_AUTH_MODE,
/* 16 */    APP_DEVICE_ERM_SET_PB_COMPLETELY_LOADED,    
/* 17 */    APP_DEVICE_ERM_CHANGE_DEVICE_NAME,
/* 18 */    APP_DEVICE_ERM_RESET_TO_FACTORY_SETTING,
/* 19 */    APP_DEVICE_ERM_SAVE_BT_NAME,
/* 20 */    APP_DEVICE_ERM_DELETE_ALL_PROFILES,
/* 21  */   APP_DEVICE_PHONE_PLAYER_SERVICE_ACTIVATION,
/* 22  */   APP_DEVICE_STOP_RINGTONE_NON_BLOCK,
/* 23  */   APP_DEVICE_START_RINGTONE_NON_BLOCK,
/* 24  */   APP_DEVICE_ERM_UPDATE_BACKUP_EEPROM,
/* 25  */   APP_DEVICE_ACTIVATE_PLAYER,
/* 26  */   APP_DEVICE_DEACTIVATE_PLAYER,
/* 27 */    APP_DEVICE_DISCONNECT_DEVICE,
/* 28 */    APP_DEVICE_ERM_UPDATE_DELETED_PLAYER_IN_PROFILES,
/* 29 */    APP_DEVICE_PHONE_SERVICE_DEACTIVATION,
/* 30 */    APP_DEVICE_CHANGE_PB_ORDER_FOR_NEW_USER
} APP_DEVICE_enFunctionIdType;

typedef enum
{
    APP_DEVICE_PHONE_CONNECT_NOT_SENT,
    APP_DEVICE_PHONE_CONNECT_SENT,
    APP_DEVICE_PHONE_STOP_SERVICE_NOT_SENT,
    APP_DEVICE_PHONE_WAIT_FOR_SERVICE_STOPPED,
    APP_DEVICE_PHONE_DISCONNECT_MP_DEVICE,
    APP_DEVICE_PHONE_WAIT_FOR_MP_DEVICE_DISCONNECTED,
//    APP_DEVICE_PHONE_START_SERVICE_NOT_SENT,
    APP_DEVICE_PHONE_START_SERVICE_SENT
}APP_DEVICE_enPhoneServiceState;

typedef enum
{
/* 0  */   APP_DEVICE_MDI_CONNECT_NOT_SENT,
/* 1  */   APP_DEVICE_MDI_CONNECT_SENT,
/* 2  */   APP_DEVICE_MDI_SERVICE_AVRCP_STOP_SENT,
/* 3  */   APP_DEVICE_MDI_SERVICE_A2DP_STOP_SENT,
/* 4  */   APP_DEVICE_MDI_SERVICE_NOT_SENT,
/* 5  */   APP_DEVICE_MDI_SERVICE_AVRCP_START_SENT,
/* 6  */   APP_DEVICE_MDI_SERVICE_A2DP_START_SENT,
/* 7  */   APP_DEVICE_MDI_WAIT_FOR_MP_DEVICE_DISCONNECTED
}APP_DEVICE_enServiceActivationState;

typedef enum
{
    APP_DEVICE_SET_MODE_NOT_SENT,
    APP_DEVICE_SET_MODE_SENT
}APP_DEVICE_enAutoConnState;

typedef enum
{
    APP_DEVICE_ADD_PROFILE_IN_EEPROM_NOT_SENT,
    APP_DEVICE_ADD_PROFILE_IN_EEPROM_SENT
}APP_DEVICE_enAddUserProfileState;

typedef enum
{
    APP_DEVICE_DELETE_PROFILE_IN_EEPROM_NOT_SENT,
    APP_DEVICE_DELETE_DEVICE_PAIRING_NOT_SENT,
    APP_DEVICE_DELETE_DEVICE_PAIRING_SENT
}APP_DEVICE_enDeleteUserProfileState;

typedef enum
{
    APP_DEVICE_DELETE_ALL_PROFILES_IN_EEPROM_NOT_SENT,
    APP_DEVICE_DELETE_ALL_DEVICE_PAIRING_NOT_SENT,
    APP_DEVICE_DELETE_ALL_DEVICE_PAIRING_SENT
}APP_DEVICE_enDeleteAllUserProfilesState;

typedef enum
{
    APP_DEVICE_CHANGE_MAILBOX_IN_EEPROM_NOT_SENT,
    APP_DEVICE_CHANGE_MAILBOX_IN_EEPROM_SENT
}APP_DEVICE_enChangeMailboxState;

typedef enum
{
    APP_DEVICE_CHANGE_RINGTONE_IN_EEPROM_NOT_SENT,
    APP_DEVICE_CHANGE_RINGTONE_IN_CK5050_NOT_SENT,
    APP_DEVICE_UPDATE_RINGTONE_MODE_IN_CK5050_NOT_SENT
}APP_DEVICE_enChangeRingtoneState;

typedef enum
{
    APP_DEVICE_CHANGE_RINGTONE_MODE_IN_EEPROM_NOT_SENT,
    APP_DEVICE_CHANGE_RINGTONE_MODE_IN_CK5050_NOT_SENT
}APP_DEVICE_enChangeRingtoneAlternateState;

typedef enum
{
    APP_DEVICE_SET_PB_COMPLETELY_LOADED_IN_EEPROM_NOT_SENT
}APP_DEVICE_enSetPbCompletelyLoadedState;



typedef enum
{
    APP_DEVICE_CHANGE_ASSOC_HEADSET_IN_EEPROM_NOT_SENT
}APP_DEVICE_enChangeAssociatedHeadsetState;


typedef enum
{
    APP_DEVICE_CHANGE_ASSOC_PLAYER_IN_EEPROM_NOT_SENT
}APP_DEVICE_enChangeAssociatedPlayerState;

typedef enum
{
    APP_DEVICE_CHANGE_PB_ORDER_FOR_NEW_USER_NOT_SENT
} APP_DEVICE_enAppDeviceChangePBOrder;

typedef enum
{
    APP_DEVICE_UPDATE_PROFILE_LIST_IN_EEPROM_NOT_SENT,
    APP_DEVICE_UPDATE_PROFILE_LIST_IN_EEPROM_SENT
}APP_DEVICE_enUpdateActProfileListState;


typedef enum
{
    APP_DEVICE_INIT_CK5050_RINGTONE_NOT_SENT,
    APP_DEVICE_INIT_CK5050_ALTERNATE_RINGTONE_NOT_SENT
}APP_DEVICE_enInitCK5050ForUserState;



typedef enum
{
    APP_DEVICE_DELETE_OLDEST_PROFILE_IN_EEPROM_NOT_SENT,
    APP_DEVICE_DELETE_OLDEST_PROFILE_PAIRING_NOT_SENT,
    APP_DEVICE_ADD_NEW_PROFILE_NOT_SENT    
}APP_DEVICE_enAddMissingProfileState;


typedef enum
{
    APP_DEVICE_CHANGE_AUTH_MODE_IN_EEPROM_NOT_SENT
}APP_DEVICE_enChangeAuthModeState;


typedef enum
{
    APP_DEVICE_CHANGE_DEVICE_NAME_IN_EEPROM_NOT_SENT = 0,
    APP_DEVICE_CHANGE_DEVICE_NAME_IN_EEPROM_SENT
}APP_DEVICE_enChangeDeviceNameState;

typedef enum
{
    APP_DEVICE_START_RINGTONE_STOP_PLAYBACK,
    APP_DEVICE_START_RINGTONE_MUTE_SENT,    
    APP_DEVICE_START_RINGTONE_PLAYBACK_SENT
}APP_DEVICE_enChangeStartRingtoneState;

typedef enum
{
/* 0 */    APP_DEVICE_NOTIFY_FACTORY_SETTING = 0,
/* 1 */    APP_DEVICE_DISCONNECT_ACTIVE_PHONE,
/* 2 */    APP_DEVICE_DISCONNECT_ACTIVE_PLAYER_A2DP,
/* 3 */    APP_DEVICE_DISCONNECT_ACTIVE_PLAYER_AVRCP,
/* 4 */    APP_DEVICE_DISCONNECT_ACTIVE_HEADSET,
/* 5 */    APP_DEVICE_DELETE_ALL_USER_PROFILES_RAM,
/* 6 */    APP_DEVICE_DELETE_ALL_SETTINGS_CK5050P,
/* 7 */    APP_DEVICE_WRITE_BACK_BT_PIN_NAME,
/* 8 */    APP_DEVICE_DELETE_ALL_PROFILES_EEPROM,
/* 9 */    APP_DEVICE_DELETE_ALL_PROFILES_EEPROM_WAIT_FOR_EEPROM_FINISHED,
/* 10 */   APP_DEVICE_DELETE_ALL_PROFILES_EEPROM_SUCCESS,
/* 11 */   APP_DEVICE_DELETE_ALL_PROFILES_EEPROM_FAILED
}APP_DEVICE_enChangeFactorySettingState;

typedef enum
{
/* 0 */   APP_DEVICE_WRITE_BACK_BACKUP_BT_PIN,
/* 1 */   APP_DEVICE_WRITE_BACK_BACKUP_BT_NAME,
/* 2 */   APP_DEVICE_WRITE_BACK_BACKUP_BRAND
} APP_DEVICE_enBackupAreaEeprom;

typedef enum
{
    APP_DEVICE_WRITE_BT_NAME_TO_EEPROM,
    APP_DEVICE_WRITE_BT_NAME_TO_CK5050P
}APP_DEVICE_enSaveBtNameState;

typedef enum
{
    APP_DEVICE_DISCONNECT_DEVICE_NOT_SENT        = 0x00,
    APP_DEVICE_DISCONNECT_DEVICE_SENT
}APP_DEVICE_enDisconnectDeviceStateType;
/*----------------------------------------------------------------------------*/
/* Prototypes of module global methods                                        */
/*----------------------------------------------------------------------------*/
//void _APP_DEVICE_vNotifyApps(APP_DEVICE_enNotificationId event);
/*----------------------------------------------------------------------------*/
/* Module global code macros                                                  */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the macros
*/
//#define sqrt(x) ((x)*(x))

/*----------------------------------------------------------------------------*/
/* Module global variables                                                    */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the module global variable
*/


/*----------------------------------------------------------------------------*/
/* Module global const definitions                                                  */
/*----------------------------------------------------------------------------*/
#define APP_DEVICE_WAIT_TIMEOUT     10
/*----------------------------------------------------------------------------*/
/* Module internal struct                                                     */
/*----------------------------------------------------------------------------*/

typedef struct
{
    APP_DEVICE_enFunctionIdType fctType;
    void(*callBack)(APP_COMMON_enResponseCode);
}AppDeviceCommonType;

typedef struct
{
    AppDeviceCommonType             common;
    APP_DEVICE_enPhoneServiceState  state;
    AT_DeviceId                     deviceId;
    ATBTServiceEnum                 serviceId;
    Uint16Type                      waitCounter;
    Uint8Type                       maxAllowedSteps;
}AppDeviceServiceActivateType;


typedef struct
{
    AppDeviceCommonType                 common;
    APP_DEVICE_enServiceActivationState state;
    AT_DeviceId                         deviceId;
    Uint8Type                           maxAllowedSteps;
    BooleanType                         bIsA2DPOfWrongDeviceStarted;
    BooleanType                         bIsAVRCPOfWrongDeviceStarted;
}AppDeviceActPlayerType;

typedef struct
{
    AppDeviceCommonType                     common;
    APP_DEVICE_enServiceActivationState     state;
    AT_DeviceId                             deviceId;
    Uint8Type                               maxAllowedSteps;
}AppDeviceDeactPlayerType;

typedef struct
{
    AppDeviceCommonType         common;
    APP_DEVICE_enAutoConnState  state;
    BooleanType                 enable;
}AppDeviceSetAutoConnModeType;


typedef struct
{
    AppDeviceCommonType                 common;
    APP_DEVICE_enAddUserProfileState    state;
    APP_DEVICE_DeviceType               deviceType;
    AT_DeviceId                         deviceId;
    AT_DeviceNameString                 name;
    Uint16Type                          nameLen;
}AppDeviceAddProfileType; 

typedef struct
{
    AppDeviceCommonType                 common;
    APP_DEVICE_enDeleteUserProfileState state;
    Uint16Type                          profileIndex;
    AT_DeviceId                         deviceId;
    APP_DEVICE_DeviceType               deviceType;
}AppDeviceDeleteProfileType; 

typedef struct
{
    AppDeviceCommonType                 common;
    APP_DEVICE_enDeleteAllUserProfilesState state;
    Uint16Type                          profileIndex;
    AT_DeviceId                         deviceId;
    APP_DEVICE_DeviceType               deviceType;
}AppDeviceDeleteAllProfilesType; 

typedef struct
{
    AppDeviceCommonType                    common;
    APP_DEVICE_enChangeFactorySettingState state;
    AT_DeviceNameString                    name; 
    Uint8Type                              pin[2];
}AppDeviceFactorySettingType; 

typedef struct
{
    AppDeviceCommonType                    common;
    APP_DEVICE_enBackupAreaEeprom 		   state;
    APP_CAN_ENUM_MARKE                     marke;
    AT_DeviceNameString                    name; 
    Uint8Type                              pin[2];
}AppDeviceBackupAreaEepromType;

typedef struct
{
    AppDeviceCommonType                 common;
    APP_DEVICE_enSaveBtNameState        state;
    Uint8Type                           btName[AT_DEVICE_NAME_MAX_LEN + 1];
    Uint8Type                           length;
}AppDeviceSaveBtNameType;

typedef struct
{
    AppDeviceCommonType                 common;
    APP_DEVICE_enChangeMailboxState     state;
    Uint16Type                          profileIndex;
    ATPhoneNumberType                   number;
    Uint16Type                          length;    
}AppDeviceChangeMailboxType; 

typedef struct
{
    AppDeviceCommonType                 common;
    APP_DEVICE_enChangeRingtoneState    state;
    Uint16Type                          profileIndex;
    ATRintoneModeEnum                   ringtone;
}AppDeviceChangeRingtoneType; 

typedef struct
{
    AppDeviceCommonType                         common;
    APP_DEVICE_enChangeAssociatedHeadsetState   state;
}AppDeviceChangeAssociatedHeadsetType;

typedef struct
{
    AppDeviceCommonType                         common;
    APP_DEVICE_enChangeAssociatedPlayerState    state;
}AppDeviceChangeAssociatedPlayerType;

typedef struct
{
    AppDeviceCommonType                         common;
    APP_DEVICE_enAppDeviceChangePBOrder         state;
}AppDeviceChangePBOrder;

typedef struct
{
    AppDeviceCommonType                         common;
    APP_DEVICE_enChangeRingtoneAlternateState   state;
    Uint16Type                                  profileIndex;
    BooleanType                                 bAlternate;
}AppDeviceChangeRingtoneAlternateType; 

typedef struct
{
    AppDeviceCommonType                         common;
    APP_DEVICE_enSetPbCompletelyLoadedState     state;
    Uint16Type                                  profileIndex;
} AppDeviceSetPbCompletelyLoadedType;

typedef struct
{
    AppDeviceCommonType                     common;
    APP_DEVICE_enInitCK5050ForUserState     state;
}AppDeviceInitCK5050ForUserType; 



typedef struct
{
    AppDeviceCommonType                     common;
    APP_DEVICE_enUpdateActProfileListState  state;
}AppDeviceUpdateActiveProfileListType; 


typedef struct
{
    AppDeviceCommonType                 common;
    APP_DEVICE_enAddMissingProfileState state;
    AT_DeviceId                         deviceId;
    AT_DeviceNameString                 name;
    Uint16Type                          nameLen;
    AT_DeviceId                         oldestProfileDeviceId;
}AppDeviceAddMissingProfileType; 

typedef struct
{
    AppDeviceCommonType                 common;
    APP_DEVICE_enChangeAuthModeState    state;
    Uint8Type                           profileIdx;
    APP_DEVICE_DeviceType               profileType;
    BooleanType                         bAuthNeeded;
}AppDeviceChangeAuthModeType;

typedef struct
{
    AppDeviceCommonType                 common;
    APP_DEVICE_enChangeDeviceNameState  state;
    AT_DeviceId                         deviceId;
    AT_DeviceNameString                 name;
    Uint16Type                          nameLen;                     
} AppDeviceChangeDeviceNameType;

typedef struct
{
    AppDeviceCommonType                     common;
    ATRintoneModeEnum                       ringtone;    
    APP_DEVICE_enChangeStartRingtoneState   state;
    Uint8Type                               retry;
}AppDeviceStartPlayRingtone;

typedef struct
{
    AppDeviceCommonType                     common;
    Uint8Type                               retry;
}AppDeviceStopPlayRingtone;

typedef struct
{
    AppDeviceCommonType                     common;
    APP_DEVICE_enDisconnectDeviceStateType  state;   
}AppDeviceDisconnectDevice;

typedef struct
{    
    BooleanType             disableAutoConnPending;
    BooleanType             enableAutoConnPending;
    BooleanType             autoConnMode;

    ATAudioSourceActStateEnum AudioSource;

    ATRintoneModeEnum       ringtone;    
    /* set when a new devices connects, DeviceId is saved depending on
     * deviceType and reset to AT_INVALID_DEVICE_ID as soon as ActiveProfileList has
     * been updated */
    BooleanType             updateActiveProfilePending;
    Uint8Type               newActPlayerProfileIdx;
    Uint8Type               newActPhoneProfileIdx;
    Uint8Type               newActHeadsetProfileIdx;

    BooleanType             addMissingUserProfilePending;
    BooleanType             addMissingPlayerProfilePending;

    BooleanType             initCK5050ForUserProfilePending;
    BooleanType             initCK5050ForUserProfilePendingDueToCallTakeOver;
    BooleanType             associatePlayerToUserPending;
    BooleanType             associateHeadsetToUserPending;
    BooleanType             stopRingtoneNonBlock;
    BooleanType             disconnectDevice;
    BooleanType             updateDeletedPlayerInProfiles;
    BooleanType             changeSortingOrderForProfile;
    BooleanType             startRingtoneNonBlock;   
    BooleanType             ck5050pIsReady; 
    struct{
        AT_DeviceId deviceId;
        void(*cb)(APP_COMMON_enResponseCode);   
    }                       actPlayerPending;
    AT_DeviceId             deactPlayerPending;
    AT_DeviceId             actPlayerWhenWrongPlayerWasStarted;
    Uint16Type              disconnectDeviceId;
    Uint8Type               updatedeletedPlayerinProfilesDeviceId;
    Uint8Type               numberOfProfilesToBeUpdatedBecauseOfDeletedPlayer;
    Uint8Type               PBSortingOrderRetryCounter;
    AT_DeviceId             deativatePhoneServiceDeviceId;
    union
    {
        AppDeviceCommonType                     common;
        AppDeviceServiceActivateType            serviceAct;  
        AppDeviceActPlayerType                  actPlayer;    
        AppDeviceDeactPlayerType                deactPlayer;    
        AppDeviceSetAutoConnModeType            autoConn; 
        AppDeviceAddProfileType                 addProfile; 
        AppDeviceDeleteProfileType              delProfile;
        AppDeviceDeleteAllProfilesType          delAllProfiles;
        AppDeviceChangeMailboxType              changeMailbox;
        AppDeviceChangeRingtoneType             changeRingtone;
        AppDeviceChangeRingtoneAlternateType    changeRingtoneMode;
        AppDeviceUpdateActiveProfileListType    actProfile;
        AppDeviceAddMissingProfileType          addMissProfile;
        AppDeviceInitCK5050ForUserType          initCk5050ForUser;
        AppDeviceChangeAssociatedHeadsetType    changeAssocHeadset;
        AppDeviceChangeAssociatedPlayerType     changeAssocPlayer;  
        AppDeviceChangeAuthModeType             changeAuthMode;
        AppDeviceSetPbCompletelyLoadedType      setPbCompletelyLoaded;
        AppDeviceChangeDeviceNameType           changeDeviceName;
        AppDeviceFactorySettingType             factorySetting;
        AppDeviceBackupAreaEepromType           backupAreaEeprom;
        AppDeviceSaveBtNameType                 saveBtName;
        AppDeviceStartPlayRingtone              startPlayRingtone;
        AppDeviceStopPlayRingtone               stopPlayRingtone;
        AppDeviceDisconnectDevice               disconnectDevice;
        AppDeviceChangeAssociatedPlayerType     updateDeletedPlayer;
        AppDeviceChangePBOrder                  changePBOrder;
        // add struct for each fct which can be pending and having
        // more than one state
    }fct;
}AppDeviceContext;



void _APP_DEVICE_vOnDeviceConnectCb(APP_COMMON_enResponseCode response);
void _APP_DEVICE_vOnServiceStopCb(APP_COMMON_enResponseCode response);
void _APP_DEVICE_vOnServiceStartCb(APP_COMMON_enResponseCode response);
void _APP_DEVICE_vOnAddProfileToEeprom_Cb(APP_COMMON_enResponseCode response);
void _APP_DEVICE_vOnDeleteProfileInEeprom_Cb(APP_COMMON_enResponseCode response);
void _APP_DEVICE_vOnDeleteDevice_Cb(APP_COMMON_enResponseCode response);
void _APP_DEVICE_vOnChangeMailboxInEeprom_Cb(APP_COMMON_enResponseCode rsp);
void _APP_DEVICE_vOnAddActiveProfileToEeprom_Cb(APP_COMMON_enResponseCode response );
void _APP_DEVICE_vOnAddMissingProfile_Cb(APP_COMMON_enResponseCode rsp);
void _APP_DEVICE_vOnChangeRingtone_Cb(APP_COMMON_enResponseCode rsp);
void _APP_DEVICE_vOnChangeRingtoneMode_Cb(APP_COMMON_enResponseCode rsp);
void _APP_DEVICE_vOnInitCK5050ForUser_Cb(APP_COMMON_enResponseCode rsp);
void _APP_DEVICE_vOnChangeAssociatedHeadset_Cb(APP_COMMON_enResponseCode rsp);
void _APP_DEVICE_vOnChangeAssociatedPlayer_Cb(APP_COMMON_enResponseCode rsp);
void _APP_DEVICE_vOnChangePBSortingOrder_Cb(APP_COMMON_enResponseCode rsp);
void _APP_DEVICE_vOnChangeAssociatedPlayerAfterDeletedPlayer_Cb(APP_COMMON_enResponseCode rsp);
void _APP_DEVICE_vOnChangeAuthMode_Cb(APP_COMMON_enResponseCode rsp);
void _APP_DEVICE_vOnSetPbCompletelyLoaded_Cb(APP_COMMON_enResponseCode rsp);
void _APP_DEVICE_vOnChangeDeviceName_Cb(APP_COMMON_enResponseCode rsp);
void _APP_DEVICE_vOnDeleteAllUserProfileCK5050p_Cb(APP_COMMON_enResponseCode rsp);
void _APP_DEVICE_vOnWriteDefaultLanguageInEeprom_Cb(CommonResponseCodeType rC);
void _APP_DEVICE_vOnWriteDefaultInEeprom_Cb(CommonResponseCodeType rC);
void _APP_DEVICE_vDisconnectProfile_Cb(APP_COMMON_enResponseCode rsp);
void _APP_DEVICE_vOnSaveBtName_Cb(APP_COMMON_enResponseCode response );
void _APP_DEVICE_vOnDeleteAllProfilesInEeprom_Cb(APP_COMMON_enResponseCode rsp);
void _APP_DEVICE_vOnDeleteAllDevice_Cb(APP_COMMON_enResponseCode rsp);
void _APP_DEVICE_GetMuteRingtone_Cb(APP_COMMON_enResponseCode rsp);
void _APP_DEVICE_PlayRingtone_Cb(APP_COMMON_enResponseCode rsp);
void _APP_DEVICE_StopRingtone_Cb(APP_COMMON_enResponseCode rsp);
void _APP_DEVICE_StopBeforePlayRingtone_Cb(APP_COMMON_enResponseCode rsp);
void _APP_DEVICE_DisconnectDevice_Cb(APP_COMMON_enResponseCode rsp);
void _APP_DEVICE_vDeactivateMdiFromAvrcpStarted_Cb(APP_COMMON_enResponseCode rsp);
void _APP_DEVICE_vNotifyApps(APP_DEVICE_enNotificationId event);

void _APP_DEVICE_vSendResultReleaseContext(APP_COMMON_enResponseCode result, Uint32Type error_line);
#endif /* _APP_DEVICE_CI_H */

/* End Of File APP_DEVICE_CI.h */
