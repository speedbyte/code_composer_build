/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     VCR.h
*-----------------------------------------------------------------------------
* Module Name:    Voice Control Recognition
*-----------------------------------------------------------------------------
* Description:    Voice Control Recognition
*-----------------------------------------------------------------------------
* $Date: 2011-08-31 08:56:12 +0200 (Mi, 31 Aug 2011) $
* $Rev: 21451 $
* $Author: brust $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_VCR/trunk/inc/VCR.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _VCR_H
#define _VCR_H


/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include "APP_COMMON_Utils.h"
#include "common.h"
#include "APP_PHONEBOOK.h"
#include "ERM.h"
/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "VCR_CE.h"
#include "VCR_CI.h"

/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Types                                                               */
/*----------------------------------------------------------------------------*/

/* RT 2429, moved to MOD_ERM */
//typedef enum {
//    VCR_SIVR_NOT_SUPPORTED   = 0,
//    VCR_SIVR_FULL_SUPPORTED  = 1,
//    VCR_SIVR_LIMITED_SUPPORT = 2
//}VCRSIVRSupportEnum;


typedef enum {
    VCR_HAPTIC_NONE           = 0,
    VCR_HAPTIC_PLAY_NAME      = 1,
    VCR_HAPTIC_STORE_NAME     = 2,
    VCR_HAPTIC_DELETE_NAME    = 3,
    VCR_HAPTIC_DELETE_ALL     = 4,
    VCR_HAPTIC_READ_NEW_SMS   = 5,
    VCR_HAPTIC_READ_SMS       = 6,
    VCR_HAPTIC_PLAY_TEST      = 7,
    VCR_HAPTIC_PLAY_VMEMO     = 8  /* unused so far */
} VCRHapticTriggerEnum;



typedef enum {
    VCR_RESULT_TYPE_NONE          = 0, /* used as init value */
    VCR_RESULT_TYPE_NUMBER        = 1,
    VCR_RESULT_TYPE_CONTACT_UNI   = 2,
    VCR_RESULT_TYPE_CONTACT_AMB   = 3,
    VCR_RESULT_TYPE_LANGUAGE      = 4,
    VCR_RESULT_TYPE_READSMS       = 5,
    VCR_RESULT_TYPE_BT_PIN        = 6,
    VCR_RESULT_TYPE_RINGTONE      = 7,
    VCR_RESULT_TYPE_PLAYPB        = 8,
    VCR_RESULT_TYPE_CALLSTACKS    = 9,
    VCR_RESULT_TYPE_TRAINING      = 10
}VCRResultTypeEnum;


typedef struct {
    VCRResultTypeEnum  type; /* = VCR_RESULT_TYPE_CONTACT_AMB */
    /* store only number of items available and the item id   */
    ATSIVRItemTypeEnum item_type; /* same for all item_ids    */
    BooleanType        location_avail; /* we received location with result */
    Uint16Type         location_id;
    Uint16Type         item_id[VCR_MAXNUM_RES_STORE];
    Uint8Type          num_ids;   /* number of available ids  */
    Uint8Type          current_num; /* current run counter for resolve of ambi names */
}VCRResultContactAmbType;

typedef struct {
    VCRResultTypeEnum  type; /* = VCR_RESULT_TYPE_CONTACT_UNI */
    ATSIVRItemTypeEnum item_type;
    /* store location if received in RRES */
    BooleanType        is_dial_index;
    BooleanType        location_avail;
    Uint16Type         location_id;
    Uint16Type         item_id;
    Uint16Type         n_location[VCR_MAXNUM_RES_STORE];
    ATPhoneNumberType  phone_number[VCR_MAXNUM_RES_STORE];
    Uint16Type         phone_number_len[VCR_MAXNUM_RES_STORE];
    Uint16Type         avail_loc;
    Uint16Type         confidence_rate;
    Uint8Type          n; /* amount of phone numbers for given item */
    Uint8Type          c; /* amount of categories for given item only indicate 1 or more than one */
    Uint8Type          n_cat; /* number of items where n_location match location_id */
    Uint8Type          current_num; /* current run counter for resolve of multiple numbers */
    /* location for the available numbers 0..n-1 */
}VCRResultContactUniType;

/*----------------------------------------------------------------------------*/
/* Global Variables Declarations (avoid as much as possible!)                 */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global const Declarations                                                  */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Methods                                                             */
/*----------------------------------------------------------------------------*/
extern void            VCR_vCb3BM_PTT(void);
extern void            VCR_vHandleEvents(EventMaskType events);
extern SuccessFailType VCR_sfInitApp(void);
extern void            VCR_sfErmIndicationCk5050(BooleanType ck5050_active);
extern SuccessFailType VCR_sFChangeLanguage(ATSIVRLanguagesEnum newLangId);
extern SuccessFailType VCR_sFChangeLanguageExecute(APP_CAN_enLang newCANLangID);

extern SuccessFailType VCR_sFChangeLanguageViaCan(APP_CAN_enLang newLangId);
extern void            VCR_vCBSendMsgViaAPP(APP_COMMON_enResponseCode response);

extern SuccessFailType VCR_sFGetCurrentPhoneNumberforDial(
        ATPhoneNumberType * p_number,
        Uint16Type        * p_number_len);

extern BooleanType     APP_SD_bNameIdsFromVCRToSd(VCRResultContactAmbType  amb);
extern BooleanType     APP_SD_bNameMultNumFromVCRToSd(VCRResultContactUniType  uni);
extern BooleanType     APP_SD_bCatMultNumFromVCRToSd(VCRResultContactUniType  uni);
extern SuccessFailType VCR_sfEnablePerformanceMeasures(BooleanType enable);
extern void            VCR_vInstrumentClusterAvailable(BooleanType available);

extern void VCR_vCBGetPBInfoSingleEntryUserPB(APP_COMMON_enResponseCode response);
extern void VCR_vCBGetPBInfoSingleEntryCallStack(APP_COMMON_enResponseCode response);
extern void VCR_vCBGetPBIndexEntryCallStack(APP_COMMON_enResponseCode response);


extern void VCR_vCBRestoreFactory(APP_COMMON_enResponseCode response);
extern void VCR_vCBDeleteSMS(APP_COMMON_enResponseCode response);
extern void VCR_vCBReadSMS(APP_COMMON_enResponseCode response);
extern void VCR_vCBMarkSMSRead(APP_COMMON_enResponseCode response);
extern void VCR_vCBSetBTPin(APP_COMMON_enResponseCode response);

extern void VCR_vCBIncNumVoiceTagForActiveUser(APP_COMMON_enResponseCode response);
extern void VCR_vCBDecNumVoiceTagForActiveUser(APP_COMMON_enResponseCode response);
extern void VCR_vCBResetStartVoiceTagForActiveUser(APP_COMMON_enResponseCode response);
extern void VCR_vCBResetStopVoiceTagForActiveUser(APP_COMMON_enResponseCode response);
/* function to check if SIVR is active */
extern BooleanType     VCR_bCheckSIVRIsActive(void);

extern BooleanType     VCR_bCheckIsPTTPressed(void);
extern BooleanType     VCR_bCheckIsCALLPressed(void);
extern BooleanType     VCR_bGetInstrumentClusterAvailable(void);
/* functions for MFLs */
extern void            VCR_vEventCall(void);
extern ATSIVREngStatusEnum  VCR_eGetVoiceRecognitionStatus(void);
extern VCRHapticTriggerEnum VCR_eGetHapticTrigTypeStatus(void);

extern SuccessFailType VCR_sFSetFallbackLanguage(Uint8Type new_value);
extern SuccessFailType VCR_sFSetVoiceControlSupport(Uint8Type new_value);

extern SuccessFailType VCR_sFHapticTrigger(VCRHapticTriggerEnum haptic_type,
                                           Uint16Type           index);

extern APP_COMMON_enResponseCode VCR_rcPlayTestPrompt(
        void (*cb)(APP_COMMON_enResponseCode));

extern void VCR_vStopSIVRForCall(void);
extern void VCR_vStopSIVRForCK5050PowerOff(void);

extern void VCR_vCBPBSyncOK(APP_COMMON_enResponseCode response);


extern void VCR_vCBAddContact(APP_COMMON_enResponseCode response);


extern void VCR_vOnAppDeviceNotification(APP_DEVICE_enNotificationId notification);


extern const VCRResultContactAmbType *VCR_stGetContactAmb();
extern const VCRResultContactUniType *VCR_stGetContactUni();

extern void VCR_vNotifyPBEntryAdded(Uint16Type index);
extern void VCR_vOnAppCshdlNotification(APP_CSHDL_NotificationType notification);
extern void VCR_cbMuteGetAck(APP_COMMON_enResponseCode rsp);
extern void VCR_vRequestStopEngine(void);

/* function added to fix ticket #8968 */
extern void VCR_vStatusResetForRestoreFactorySettings(void);



/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Private Enumerations                                                       */
/*----------------------------------------------------------------------------*/


#endif /* _VCR_H */

/* End Of File VCR.h */
