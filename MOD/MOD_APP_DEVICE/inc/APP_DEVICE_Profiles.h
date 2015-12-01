/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:   APP_DEVICE_Profiles.h
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

#ifndef _APP_DEVICE_PROFILES_H
#define _APP_DEVICE_PROFILES_H

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



/*----------------------------------------------------------------------------*/
/* Public Types                                                               */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Global Variables Declarations (avoid as much as possible!)                 */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Prototypes of module internal Methods                                           */
/*----------------------------------------------------------------------------*/
SuccessFailType
_APP_DEVICE_sfHandlePendingAddProfile(const AppDeviceAddProfileType *pAdd);
SuccessFailType
_APP_DEVICE_sfHandlePendingDeleteProfile(const AppDeviceDeleteProfileType *pDel);
 SuccessFailType
_APP_DEVICE_sfHandlePendingSetAutoConnMode(const AppDeviceSetAutoConnModeType *pAct);
 SuccessFailType
_APP_DEVICE_sfHandlePendingChangeMailbox(const AppDeviceChangeMailboxType * pChange);
 SuccessFailType
_APP_DEVICE_sfHandlePendingChangeRingtone(const AppDeviceChangeRingtoneType * pChange);
 SuccessFailType
_APP_DEVICE_sfHandlePendingChangeRingtoneMode(const AppDeviceChangeRingtoneAlternateType * pChange);
 SuccessFailType
_APP_DEVICE_sfHandlePendingUpdateActiveProfileList(const AppDeviceUpdateActiveProfileListType * pUp);
 SuccessFailType
_APP_DEVICE_sfHandlePendingAddMissingUserProfile(AppDeviceAddMissingProfileType *pMiss);
 SuccessFailType
_APP_DEVICE_sfHandlePendingAddMissingPlayerProfile(AppDeviceAddMissingProfileType *pMiss);
 SuccessFailType
_APP_DEVICE_sfHandlePendingInitCK5050ForUser(const AppDeviceInitCK5050ForUserType * pInit);
 SuccessFailType
_APP_DEVICE_sfHandlePendingAssociatePlayerToUser(const AppDeviceChangeAssociatedPlayerType * pChange );
 SuccessFailType
_APP_DEVICE_sfHandlePendingAssociateHeadsetToUser(const AppDeviceChangeAssociatedHeadsetType * pChange );
 SuccessFailType
_APP_DEVICE_sfHandlePendingChangeAuthMode(const AppDeviceChangeAuthModeType * pChange);
 SuccessFailType
_APP_DEVICE_sfHandlePendingSetPbCompletelyLoaded(const AppDeviceSetPbCompletelyLoadedType * pSet);
 SuccessFailType
_APP_DEVICE_sfHandlePendingChangeDeviceName(AppDeviceChangeDeviceNameType * pChange );
 SuccessFailType
_APP_DEVICE_sfHandlePendingSaveBtName(const AppDeviceSaveBtNameType *pBtName);
 SuccessFailType
_APP_DEVICE_sfHandlePendingDeleteAllProfiles(const AppDeviceDeleteAllProfilesType *pDelAll);
 SuccessFailType 
_APP_DEVICE_sfHandlePendingStopRingtoneNonBlock();
SuccessFailType 
_APP_DEVICE_sfHandlePendingStartRingtoneNonBlock(AppDeviceStartPlayRingtone* pStartRingtonePlay);
SuccessFailType
_APP_DEVICE_sfHandlePendingUpdateDeletedPlayerInProfiles();

SuccessFailType
_APP_DEVICE_sfHandlePendingChangeSortingOrderForNewUser(const AppDeviceChangePBOrder * pChange);

 APP_COMMON_enResponseCode _APP_DEVICE_sfUpdateErmActiveProfileTable(void(*cb)(APP_COMMON_enResponseCode));
 APP_COMMON_enResponseCode _APP_DEVICE_sfAddMissingUserProfile(void(*cb)(APP_COMMON_enResponseCode));
 APP_COMMON_enResponseCode _APP_DEVICE_sfAddMissingPlayerProfile(void(*cb)(APP_COMMON_enResponseCode));
 APP_COMMON_enResponseCode _APP_DEVICE_sfInitCK5050ForUserProfile(void(*cb)(APP_COMMON_enResponseCode));
 APP_COMMON_enResponseCode _APP_DEVICE_rcAssociatePlayerToUser(void(*cb)(APP_COMMON_enResponseCode));
 APP_COMMON_enResponseCode _APP_DEVICE_rcAssociateHeadsetToUser(void(*cb)(APP_COMMON_enResponseCode));
 APP_COMMON_enResponseCode _APP_DEVICE_vChangePBSortingOrder(void(*cb)(APP_COMMON_enResponseCode));
 APP_COMMON_enResponseCode _APP_DEVICE_rcUpdatedeletedPlayerInUserProfiles(Uint8Type deviceId);

 void _APP_DEVICE_vHandleNoUserProfileAvailable(void);
 void _APP_DEVICE_vHandleNoPlayerProfileAvailable(void);
 void _APP_DEVICE_vHandleNoHeadsetProfileAvailable(void);

void _APP_DEVICE_Profiles_vHandlePhoneStateChanged(const ERMNotificationType * pNotification);
void _APP_DEVICE_Profiles_vHandlePlayerStateChanged(const ERMNotificationType * pNotification);
void _APP_DEVICE_Profiles_vOnErmNotification(const ERMNotificationType * pNotification);

void _APP_DEVICE_vHandlePlayerProfileAdded();
/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/


#endif /* _APP_DEVICE_PROFILES_H */

/* End Of File APP_DEVICE_Profiles.h */

