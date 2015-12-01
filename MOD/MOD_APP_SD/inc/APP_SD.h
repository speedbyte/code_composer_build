/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:   APP_SD.h
*-----------------------------------------------------------------------------
* Module Name:  APP_SD    
*-----------------------------------------------------------------------------
* Description:  Global Interfaces, Enum, Structs for APP_SD
*-----------------------------------------------------------------------------
* $Date: 2011-11-08 12:01:36 +0100 (Di, 08 Nov 2011) $
* $Rev: 23159 $
* $Author: maennel $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_SD/trunk/inc/APP_SD.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _APP_SD_H
#define _APP_SD_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include "common.h"
#include "KPB.h"
#include "APP_PHONE.h"
#include "APP_PHONEBOOK.h"
#include "APP_SMS.h"
#include "ERM.h"
#include "VCR.h"
#include "APP_SD_CI.h"
/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "APP_SD_CE.h"

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

/** 
\brief	description of the interface IAPP_SD_INIT
*/
//#ifdef IAPP_SD_INIT /* BEGIN DECLARE INTERFACE(IAPP_PHONE_INIT) */

SuccessFailType APP_SD_sfInitApp();
SuccessFailType _APP_SD_sfGetNextEvent(AppSdEventCtxType *pEvent);

//#endif /* END DECLARE INTERFACE(IAPP_SD_INIT) */

/** 
\brief	description of the interface IAPP_SD_EVENTS
*/
//#ifdef IAPP_SD_EVENTS /* BEGIN DECLARE INTERFACE(IAPP_PHONE_EVENTS) */

typedef enum
{
    APP_SD_NUMBER_TYPE_INFO = 0,
    APP_SD_NUMBER_TYPE_SOS,
    APP_SD_NUMBER_TYPE_SERVICE,
    APP_SD_NUMBER_TYPE_MAILBOX,
    APP_SD_NUMBER_TYPE_UNKNOWN_CALLER_ID
}APP_SD_enNumberType;


void APP_SD_vHandleEvents(EventMaskType events);
void APP_SD_vOnKpbIndication(const KpbIndType *pKpbInd);
KPB_enVersionNumber KPB_enGetKpbVersionNumber();


#include "APP_CSHDL.h"
//KPB_enFrameIdType APP_SD_GetActiveFrameId();
extern void APP_SD_vOnAppPhoneNotification(APP_PHONE_enNotificationId notification);
extern void APP_SD_vOnErmNotification(const ERMNotificationType * pNotification);
extern void APP_SD_vOnAppCshdlNotification(APP_CSHDL_NotificationType notification);
extern void APP_SD_vOnMflButton(LOGI_BUT_MAN_ENUM_BUTTON_NAMES key, LOGI_BUT_MAN_ENUM_PRESSTYPE pressType);
extern void APP_SD_vOnAppCanLangNotification(APP_CAN_LanguageType sdLangEvent);
extern void APP_SD_vOnAppCanSpeedNotification(SPEED_enSpeed sdSpeedEvent);
extern void APP_SD_vOnAppPhoneBookNotification(APP_PB_enNotificationId notification);
extern void APP_SD_vOnAppSmsNotification(APP_SMS_enNotificationId notification);
extern void APP_SD_vOnAppDeviceNotification(APP_DEVICE_enNotificationId notification);
extern void APP_SD_vOnVcrNotification(VCRNotificationIdEnum notification);
extern void APP_SD_vOnKpbNotification(KPB_enNotificationId notification);
//extern void APP_SD_ERM_vHandleErmStateChange(ERMStateType newState);
extern Uint8Type APP_SD_u8GetContextId (void);
extern void APP_SD_vSetVcrIsActive (void);

extern BooleanType APP_SD_bHapticalInterfaceAvailable(void);
void APP_SD_vFrameDataTransfered(BooleanType bSuccess);
void APP_SD_vListDataRequest(Uint16Type firstIdx, Uint16Type number);


SuccessFailType APP_SD_sfGetTextForNumberType(APP_SD_enNumberType numberType, const Uint8Type ** pText, Uint16Type * pTextLen  );
void APP_SD_vDiagLanguagesChanged();
void APP_SD_vFrameDataTransfered(BooleanType bSuccess);
APP_SD_enContextId APP_SD_getCurrentCtx();

   /* ListDataRequests Callbacks */
void _APP_SD_vListDataRequest_S104_Cb(APP_COMMON_enResponseCode result);
void _APP_SD_vListDataRequest_S105_Cb(APP_COMMON_enResponseCode result);
void _APP_SD_vListDataRequest_S211_Cb(APP_COMMON_enResponseCode result);
void _APP_SD_vListDataRequest_S404_Cb(APP_COMMON_enResponseCode result);
void _APP_SD_vListDataRequest_S405_Cb(APP_COMMON_enResponseCode result);
void _APP_SD_vListDataRequest_S515_Cb(APP_COMMON_enResponseCode result);
void _APP_SD_vListDataRequest_SC01_Cb(APP_COMMON_enResponseCode result);

   /* InitPending Callbacks */
void _APP_SD_vInitPendingS001_Cb(APP_COMMON_enResponseCode result);
void _APP_SD_vInitPendingS105_Cb(APP_COMMON_enResponseCode result);
void _APP_SD_vInitPendingS111_Cb(APP_COMMON_enResponseCode result);
void _APP_SD_vInitPendingS117_Cb(APP_COMMON_enResponseCode result);
void _APP_SD_vInitPendingS212_Cb(APP_COMMON_enResponseCode result);
void _APP_SD_vInitPendingS213_Cb(APP_COMMON_enResponseCode result);
void _APP_SD_vInitPendingS405_Cb(APP_COMMON_enResponseCode result);
void _APP_SD_vInitPendingS306_Cb(APP_COMMON_enResponseCode result);
void _APP_SD_vInitPendingS406_Msg_Cb(APP_COMMON_enResponseCode result);
void _APP_SD_vInitPendingS406_MarkAsRead_Cb(APP_COMMON_enResponseCode result);
void _APP_SD_vInitPendingS407_Cb(APP_COMMON_enResponseCode result);
void _APP_SD_vInitPendingS517_Cb(APP_COMMON_enResponseCode result);
void _APP_SD_vInitPendingS520_Cb(APP_COMMON_enResponseCode result);
void _APP_SD_vInitPendingS581_Cb(APP_COMMON_enResponseCode result);
void _APP_SD_vInitPendingS597_Cb(APP_COMMON_enResponseCode result);
//void _APP_SD_vInitPendingS602_Cb(APP_COMMON_enResponseCode result);
void _APP_SD_vInitPendingS669_Cb(APP_COMMON_enResponseCode result);
void _APP_SD_vInitPendingS801_ReqNumber_Cb(APP_COMMON_enResponseCode result);
void _APP_SD_vInitPendingS801_Dial_Cb(APP_COMMON_enResponseCode result);
void _APP_SD_vInitPendingS945_Dial_Cb(APP_COMMON_enResponseCode result);
void _APP_SD_vInitPendingSC12_Cb(APP_COMMON_enResponseCode result);
void _APP_SD_vInitPendingSC13_Cb(APP_COMMON_enResponseCode result);
void _APP_SD_vInitPendingSC16_Cb(APP_COMMON_enResponseCode result);
void _APP_SD_vInitPendingS408_Cb(APP_COMMON_enResponseCode result);

   /* ButtonPending Callbacks */
void _APP_SD_vButtonPendingS104_Cb(APP_COMMON_enResponseCode result);
void _APP_SD_vButtonPendingS104_Cb2(APP_COMMON_enResponseCode result);
void _APP_SD_vButtonPendingS306_Cb(APP_COMMON_enResponseCode result);
void _APP_SD_vButtonPendingS404_Cb(APP_COMMON_enResponseCode result);
void _APP_SD_vButtonPendingS413_Cb(APP_COMMON_enResponseCode result);
void _APP_SD_vButtonPendingS512_Cb(APP_COMMON_enResponseCode result);
void _APP_SD_vButtonPendingS515_Cb(APP_COMMON_enResponseCode result);
void _APP_SD_vButtonPendingS51D_Cb(APP_COMMON_enResponseCode result);
void _APP_SD_vButtonPendingS520_Cb(APP_COMMON_enResponseCode result);
void _APP_SD_vButtonPendingS522_Cb(APP_COMMON_enResponseCode result); /*RT-13181-Phone*/
void _APP_SD_vButtonPendingS52C_Cb(APP_COMMON_enResponseCode result);
void _APP_SD_vButtonPendingS535_Cb(APP_COMMON_enResponseCode result);
void _APP_SD_vButtonPendingS536_Cb(APP_COMMON_enResponseCode result);
void _APP_SD_vButtonPendingS556_Cb(APP_COMMON_enResponseCode result);
void _APP_SD_vButtonPendingS563_Cb(APP_COMMON_enResponseCode result);
void _APP_SD_vButtonPendingS572_Cb(APP_COMMON_enResponseCode result);
void _APP_SD_vButtonPendingS575_Cb(APP_COMMON_enResponseCode result);
void _APP_SD_vButtonPendingS580_Cb(APP_COMMON_enResponseCode result);
void _APP_SD_vButtonPendingS581_Cb(APP_COMMON_enResponseCode result);
void _APP_SD_vButtonPendingS642_Cb(APP_COMMON_enResponseCode result);
void _APP_SD_vButtonPendingS661_Cb(APP_COMMON_enResponseCode result);
void _APP_SD_vButtonPendingS669_Cb(APP_COMMON_enResponseCode result);
void _APP_SD_vButtonPendingS812_Cb(APP_COMMON_enResponseCode result);
void _APP_SD_vButtonPendingS933_Cb(APP_COMMON_enResponseCode result);
void _APP_SD_vButtonPendingSC13_Cb(APP_COMMON_enResponseCode result);
void _APP_SD_vButtonPendingSC24_Cb(APP_COMMON_enResponseCode result);

void _APP_SD_AddUserProfile_Cb(APP_COMMON_enResponseCode result);
void _APP_SD_AddPlayerProfile_Cb(APP_COMMON_enResponseCode result);
void _APP_SD_AddHeadsetProfile_Cb(APP_COMMON_enResponseCode result);
void _APP_SD_HandlePendingButtonS542_Cb(APP_COMMON_enResponseCode result);

BooleanType APP_SD_bIsIdleScreenActive(APP_SD_enIdleScreenState idleState);
BooleanType APP_SD_bIsCallScreenActive(void);
void _APP_SD_setPlayerVisibleIfEncoded(BooleanType bEnable);
BooleanType _APP_SD_blastRingtonestate(void);//RT14853
void _APP_SD_vnotifiedRingtonestate();//RT14853

   /* Timeout Callbacks */
void APP_SD_cbFrameTimeout();



//#endif /* END DECLARE INTERFACE(IAPP_SD_EVENTS) */

#endif /* _APP_SD_H */

/* End Of File APP_SD.h */

