/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     ERM_DeviceHandler.h
*-----------------------------------------------------------------------------
* Module Name:    External Resource Manager Device part
*-----------------------------------------------------------------------------
* Description:    External resource manager global functions.
*                 the ERM is responsible for control of the UART resource
*                 towards the different other application modules
*-----------------------------------------------------------------------------
* $Date: 2011-11-23 08:25:57 +0100 (Mi, 23 Nov 2011) $
* $Rev: 24103 $
* $Author: maennel $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_ERM/trunk/inc/ERM_DeviceHandler.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _ERM_DEVICE_HANDLER_H
#define _ERM_DEVICE_HANDLER_H


/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include "ERM_CI.h"
#include "ERM_CE.h"
#include "tms470r1.h"
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
/* Global const Declarations                                                  */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Methods                                                             */
/*----------------------------------------------------------------------------*/

extern void            _ERM_vInitBTDeviceTable(void);
extern void            _ERM_vInitPhysicalDeviceTable(void);
extern void            _ERM_vInitUserProfileTable(void);
extern void            _ERM_vInitPlayerHeadsetProfileTable(void);
extern void            _ERM_vInitActiveProfileList(void);
extern void            _ERM_vPrintTableSize(void);

extern SuccessFailType _ERM_sFInitBTDeviceValues(AT_DeviceId id);
extern SuccessFailType _ERM_CheckActiveBTDevice(void);
extern SuccessFailType _ERM_sFBTDeviceConnectionUpdate(AT_DeviceId id,
                                                       ATBooleanEnum  connected);
extern SuccessFailType _ERM_sFBTFillDeviceInfo(ATRspPlpdInfoType *p_info);
extern SuccessFailType _ERM_sFBTDeviceServiceUpdate(ATResBTServiceNotType *p_info);
extern void            _ERM_vCheckDeviceTable(void);
extern SuccessFailType _ERM_sfUpdateActiveUserList(Uint8Type activeUser);
extern SuccessFailType _ERM_sfDeleteDevice(AT_DeviceId deviceId);
extern AT_DeviceId     _ERM_u8GetReleasedDevice();
extern SuccessFailType _ERM_sfReadPersistentUserProfile(Uint8Type index);
extern SuccessFailType _ERM_sfReadPersistentPlayerHsProfile(Uint8Type index);
extern SuccessFailType _ERM_sfReadPersistentPhysicalDevice(Uint8Type index);
extern SuccessFailType _ERM_sfReadPersistentActiveProfiles(void);
extern SuccessFailType _ERM_sfReadPersistentUserList(void);
extern SuccessFailType _ERM_sfWritePersistentUserList(void);
extern BooleanType     _ERM_bUserListWasInitialized(void);
extern void            _ERM_vNotifyApplication(const ERMNotificationType *pNotification);
extern SuccessFailType _ERM_sFBTSupportedServiceUpdate(const ATResBTServiceConnType *pPbsc);
extern SuccessFailType _ERM_sFBTSdpUpdate(const ATRspSdpNotType *pPsdp);
//extern BooleanType       _ERM_sfCheckUserListConsistency(void);
extern SuccessFailType _ERM_sfConsistencyCheck();
extern BooleanType       _ERM_eGetActiveDeviceRingtoneAlternate(void);
SuccessFailType   _ERM_eIntSetActiveDeviceRingtoneAlternate(BooleanType value);
extern ATRintoneModeEnum _ERM_eGetActiveDeviceRingtone(void);
extern SuccessFailType   _ERM_eIntSetActiveDeviceRingtone(ATRintoneModeEnum value);

extern ERMUserProfileType * _ERM_pGetUserProfileForEdit(Uint8Type index);
extern Uint8Type _ERM_u8GetFreeUserProfileSlot(void);
extern void _ERM_vInitTmpEpromData(void);
extern SuccessFailType _ERM_sfSetUserProfileEntryToDefault(ERMUserProfileType * p_entry);

extern SuccessFailType _ERM_sfSetPhysicalDeviceEntryToDefault(ERMPhysicalBTDeviceType * p_entry, 
                                       AT_DeviceId deviceId, 
                                       const Uint8Type *pName, 
                                       Uint16Type nameLen) ;


APP_COMMON_enResponseCode _ERM_rcWritePersistentUserProfile(void);
APP_COMMON_enResponseCode _ERM_rcWritePersistentPlayerHsProfile(void);
APP_COMMON_enResponseCode _ERM_rcWritePersistentPhysicalDevice(void);
APP_COMMON_enResponseCode _ERM_rcWritePersistentActiveProfileList(void);

SuccessFailType _ERM_sfSetPlayerHeadsetProfileEntryToDefault(ERMPlayerHeadsetProfileType * p_entry,
				                                             APP_DEVICE_DeviceType deviceType);

void _ERM_OnConsistencyCheckCb(APP_COMMON_enResponseCode rsp);
AT_DeviceId ERM_u8GetActiveHeadSet(void);
AT_DeviceId ERM_u8GetActivePlayer(void);
AT_DeviceId ERM_u8GetActiveDeviceAVRCP(void);
AT_DeviceId ERM_u8GetActivePhone(void);
void ERM_vSetDisconnectWillFollow(BooleanType bDisconnectWillFollow);
BooleanType ERM_bGetDisconnectWillFollow(void);
AT_DeviceId ERM_u8GetLastActiveHeadSet(void);
AT_DeviceId ERM_u8GetLastActivePlayer(void);
AT_DeviceId ERM_u8GetLastActivePhone(void);
AT_DeviceId ERM_GetLastPairedDeviceId(void);
AT_DeviceId ERM_GetLastServiceDeviceId(void);
const Uint8Type *ERM_GetBTDeviceName(void);
Uint16Type ERM_u16GetNumberOfPairedHeadSets(void);
Uint16Type ERM_u16GetNumberOfHeadsetProfiles(void);
Uint16Type ERM_u16GetNumberOfPairedPlayer(void);
Uint16Type ERM_u16GetNumberOfPlayerProfiles(void);
Uint16Type ERM_u16GetNumberOfPairedPhones(void);
Uint16Type ERM_u16GetNumberOfUserProfiles(void);
const ERMPlayerHeadsetProfileType *ERM_pGetActivePlayerProfile(void);
const ERMPlayerHeadsetProfileType *ERM_pGetActiveHeadsetProfile(void);
const ERMUserProfileType *ERM_pGetActiveUserProfile(void);
BooleanType ERM_bMaxNumberOfUserProfilesReached(void);
BooleanType ERM_bMaxNumberOfPlayerHeadsetProfilesReached(void);
BooleanType ERM_bMaxNumberOfPlayerProfilesReached(void);
BooleanType ERM_bMaxNumberOfHeadsetProfilesReached(void);
Uint8Type _ERM_u8GetFreePlayerHeadsetProfileSlot(void);
Uint8Type ERM_u8GetOldestNoActiveUserProfileId(void);
Uint8Type ERM_u8GetOldestUserProfileId(void);
Uint8Type ERM_u8GetOldestPlayerProfileId(void);
Uint8Type ERM_u8GetOldestHeadsetProfileId(void);
BooleanType ERM_vCheckforprofileId(AT_DeviceId id);//RT13844

/* functions to prepare eeprom write of different eeprom tables */
APP_COMMON_enResponseCode _ERM_rcPrepare_EepromUpdate(
                                    void (*cb)(APP_COMMON_enResponseCode), 
                                    ERMIntMessageEnum ePendingEepromMsg );

APP_COMMON_enResponseCode _ERM_rcPrepare_DeleteUserProfile(Uint8Type index);

APP_COMMON_enResponseCode _ERM_rcPrepare_AddUserProfile(AT_DeviceId deviceId, Uint8Type * freeIndex,
                                                        Uint8Type playerIdx, Uint8Type headsetIdx);

APP_COMMON_enResponseCode _ERM_rcPrepare_ChangeUserProfile_Mailboxnumber(Uint8Type index, 
                                                                const Uint8Type *pNumber, 
                                                                Uint16Type len);
APP_COMMON_enResponseCode _ERM_rcPrepare_ChangeUserProfile_AudioMode( Uint8Type index, 
                                                                ERMUserAudioModeType audioMode); 
APP_COMMON_enResponseCode _ERM_rcPrepare_ChangeUserProfile_RingtoneAlternate(
                                                           Uint8Type index, 
                                                           BooleanType bAlternate);
APP_COMMON_enResponseCode _ERM_rcPrepare_ChangeUserProfile_Ringtone( 
                                                           Uint8Type index, 
                                                           ATRintoneModeEnum ringtone);
APP_COMMON_enResponseCode _ERM_rcPrepare_ChangeUserProfile_AssociatedHeadset( 
                                                           Uint8Type userProfileIndex, 
                                                           Uint8Type hsProfileIndex);
APP_COMMON_enResponseCode _ERM_rcPrepare_ChangeUserProfile_AssociatedPlayer( 
                                                           Uint8Type userProfileIndex, 
                                                           Uint8Type playerProfileIndex,
                                                           BooleanType bConsiderInvalidPlayer);

APP_COMMON_enResponseCode
_ERM_rcPrepare_ChangePhyTable_Name( AT_DeviceId deviceId, AT_DeviceNameString name, Uint16Type nameLen);

APP_COMMON_enResponseCode
_ERM_rcPrepare_ChangeUserProfile_PbCompletelyLoaded( Uint8Type index);

APP_COMMON_enResponseCode _ERM_rcPrepare_DeletePlayerHeadsetProfile(Uint8Type index);
APP_COMMON_enResponseCode _ERM_rcPrepare_AddPlayerHeadsetProfile(AT_DeviceId deviceId, 
                                                                 APP_DEVICE_DeviceType deviceType, 
                                                                 Uint8Type * freeIndex);
APP_COMMON_enResponseCode
_ERM_rcPrepare_ChangePlayerHeadsetProfile_AuthMode( Uint8Type index, BooleanType bAuthNeeded);

APP_COMMON_enResponseCode _ERM_rcPrepare_DeletePhysicalDevice(AT_DeviceId deviceId);
APP_COMMON_enResponseCode _ERM_rcPrepare_AddPhysicalDevice(AT_DeviceId deviceId, 
                                                           const Uint8Type *pName, 
                                                           Uint16Type nameLen);

APP_COMMON_enResponseCode _ERM_rcPrepare_ChangePhyTable_Voicetags(Uint8Type index, 
                                                            Uint8Type numVoiceTags, 
                                                        BooleanType bDeleteOngoing);

APP_COMMON_enResponseCode _ERM_rcWritePersistentActiveProfileList(void);

/* add and delete a profile from active profile list */

APP_COMMON_enResponseCode _ERM_rcPrepare_DeleteActiveProfile(AT_DeviceId deviceId, APP_DEVICE_DeviceType profileType);
APP_COMMON_enResponseCode _ERM_rcPrepare_AddActiveProfile(AT_DeviceId deviceId, APP_DEVICE_DeviceType profileType);
APP_COMMON_enResponseCode _ERM_rcPrepare_AddActivePhoneProfileWhenNotConnectedAsPhone(Uint8Type profileIdx);

void _ERM_vUpdateCANBTConnectionState(BooleanType);



extern void _ERM_vCopyTmpUserProfileToRam(void);
extern void _ERM_vCopyTmpPlayerHsProfileToRam(void);
extern void _ERM_vCopyTmpPhyDeviceToRam(void);
extern void _ERM_vCopyTmpActProfileListToRam(void);


/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Private Enumerations                                                       */
/*----------------------------------------------------------------------------*/


#endif /* _ERM_DEVICE_HANDLER_H */

/* End Of File ERM_DeviceHandler.h */
