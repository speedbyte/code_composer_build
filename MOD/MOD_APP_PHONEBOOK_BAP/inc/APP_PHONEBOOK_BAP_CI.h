/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/*****************************************************************************/
/* File Name:   APP_PHONEBOOK_BAP_CI.h
*-----------------------------------------------------------------------------
* Module Name:  APP_PHONEBOOK_BAP
*-----------------------------------------------------------------------------
* Description:  modul global constants, declarations
*-----------------------------------------------------------------------------
* $Date: 2011-10-18 10:30:40 +0200 (Di, 18 Okt 2011) $
* $Rev: 22438 $
* $Author: maennel $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_PHONEBOOK_BAP/trunk/inc/APP_PHONEBOOK_BAP_CI.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _APP_PHONEBOOK_BAP_CI_H
#define _APP_PHONEBOOK_BAP_CI_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include "common.h"

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "APP_COMMON_Utils.h" 
#include "BAP_Dispatcher.h"

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
    /* TODO: be aware of using this enums. They have to match the fctid, but the
     * highest id has to be the last defined inside the enum to preserve overlayed
     * definition with enums that do not correspond to functions ids!!!
     */
    PHONEBOOK_BAP_INVALID_FCT_ID    = 0,
    PHONEBOOK_BAP_MISSED_CALL_IND   = BapFct_Phonebook_MissedCallIndication,
    PHONEBOOK_BAP_MISSED_CALLS      = BapFct_Phonebook_MissedCalls,
    PHONEBOOK_BAP_RECEIVED_CALLS    = BapFct_Phonebook_ReceivedCalls,
    PHONEBOOK_BAP_DIALED_NUMBERS    = BapFct_Phonebook_DialedNumbers,
    PHONEBOOK_BAP_PHONEBOOK         = BapFct_Phonebook_Phonebook,
    PHONEBOOK_BAP_PB_STATE          = BapFct_Phonebook_PbState,
    PHONEBOOK_BAP_SMS_STATE         = BapFct_Phonebook_SMSState,
    PHONEBOOK_BAP_SMS_RECEIVED      = BapFct_Phonebook_SMSReceived,
    PHONEBOOK_BAP_SMS_SEND          = BapFct_Phonebook_SMSSend,
    PHONEBOOK_BAP_ACTIVATE_PB,
    PHONEBOOK_BAP_ACTIVATE_INC_PB,
    PHONEBOOK_BAP_ACTIVATE_OLD_PB_AFTER_SORT_TIMEOUT

} APP_PHONEBOOK_BAP_enFunctionIdType;



typedef enum
{
    APP_PB_BAP_ACT_PB__PB_STATE_LOADING = 0,
    APP_PB_BAP_ACT_PB__PB_STATE_COMPLETE,
    APP_PB_BAP_ACT_PB__CHANGED_ARRAY,
    APP_PB_BAP_ACT_PB__INVALID
}APP_PHONEBOOK_BAP_enActPBState;

typedef enum
{
    APP_PB_BAP_ACT_OLD_PB__PB_STATE_NOT_AVAILABLE = 0,
    APP_PB_BAP_ACT_OLD_PB__PB_STATE_COMPLETE,
    APP_PB_BAP_ACT_OLD_PB__CHANGED_ARRAY,
    APP_PB_BAP_ACT_OLD_PB__INVALID
}APP_PHONEBOOK_BAP_enActOldPBAfterSortTimeoutState;

typedef enum
{
    APP_PB_ACT_TYPE__CHANGED             = 0,
    APP_PB_ACT_TYPE__CHANGED_INCOMPLETE,
    APP_PB_ACT_TYPE__NOT_CHANGED,
    APP_PB_ACT_TYPE__NOT_CHANGED_INCOMPLETE,
    APP_PB_ACT_TYPE__NOT_AVAILABLE,
    APP_PB_ACT_TYPE__OLD_AFTER_SORTING_TIMOUT
}APP_PHONEBOOK_BAP_enPbActType;

/*RT13652*/
typedef enum
{
    APP_PB_BAP_CHANGED_ARRAY_INVALID = 0,
    APP_PB_BAP_CHANGED_ARRAY_PHONEBOOK,
    APP_PB_BAP_CHANGED_ARRAY_DIALEDCALLS,
    APP_PB_BAP_CHANGED_ARRAY_RECEIVEDCALLS,
    APP_PB_BAP_CHANGED_ARRAY_MISSEDCALLS,
}APP_PHONEBOOK_BAP_enChangedArrayType;

/*----------------------------------------------------------------------------*/
/* Prototypes of module global methods                                        */
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_PHONEBOOK_BAP_sfUpdatePbDwldState();
void _APP_PHONEBOOK_BAP_vReleaseContext(const BapIndication *pBapInd);
void _APP_PHONEBOOK_BAP_vRetryContext(const BapIndication *pBapInd);


/*----------------------------------------------------------------------------*/
/* Module global code macros                                                         */
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

/*----------------------------------------------------------------------------*/
/* Module internal struct                                                     */
/*----------------------------------------------------------------------------*/
typedef struct
{
                /* Byte 1 */
/* Bit 7 */     Uint8Type :1;
/* Bit 6 */     Uint8Type :1;
/* Bit 5 */     Uint8Type :1;
/* Bit 4 */     Uint8Type Heartbeat:1;
/* Bit 3 */     Uint8Type FunctionList:1;
/* Bit 2 */     Uint8Type BapConfig:1;
/* Bit 1 */     Uint8Type GetAll:1;
/* Bit 0 */     Uint8Type :1;
                /* Byte 2 */
 /* Bit 15 */   Uint8Type FsgOpState:1;
 /* Bit 14 */   Uint8Type FsgSetup:1;
 /* Bit 13 */   Uint8Type :1;
 /* Bit 12 */   Uint8Type :1;
 /* Bit 11 */   Uint8Type :1;
 /* Bit 10 */   Uint8Type :1;
 /* Bit 9  */   Uint8Type :1;
 /* Bit 8  */   Uint8Type :1;
                /* Byte 3 */
 /* Bit 23 */   Uint8Type Phonebook:1;
 /* Bit 22 */   Uint8Type PbState:1;
 /* Bit 21 */   Uint8Type CallStackDeleteAll:1;
 /* Bit 20 */   Uint8Type DialedNumbers:1;
 /* Bit 19 */   Uint8Type ReceivedCalls:1;
 /* Bit 18 */   Uint8Type MissedCalls:1;
 /* Bit 17 */   Uint8Type MissedCallIndication:1;
 /* Bit 16 */   Uint8Type MobileServiceSupport:1;
                /* Byte 4 */
 /* Bit 31 */   Uint8Type SMSStoreTemplate:1;
 /* Bit 30 */   Uint8Type SMSTemplate:1;
 /* Bit 29 */   Uint8Type SMSReceived:1;
 /* Bit 28 */   Uint8Type SMSFlash:1;
 /* Bit 27 */   Uint8Type SMSState:1;
 /* Bit 26 */   Uint8Type PbStartDownload:1;
 /* Bit 25 */   Uint8Type VoiceMailboxNumber:1;
 /* Bit 24 */   Uint8Type PbSpeller:1;
                /* Byte 5 */
 /* Bit 39 */   Uint8Type vCalendar:1;
 /* Bit 38 */   Uint8Type vCardStore:1;
 /* Bit 37 */   Uint8Type vCardCopyToMobile:1;
 /* Bit 36 */   Uint8Type vCardSend:1;
 /* Bit 35 */   Uint8Type vCard:1;
 /* Bit 34 */   Uint8Type SMSDeleteAll:1;
 /* Bit 33 */   Uint8Type SMSCopyToMobile:1;
 /* Bit 32 */   Uint8Type SMSSend:1;
                /* Byte 6 */
 /* Bit 47 */   Uint8Type :1;
 /* Bit 46 */   Uint8Type :1;
 /* Bit 45 */   Uint8Type :1;
 /* Bit 44 */   Uint8Type :1;
 /* Bit 43 */   Uint8Type :1;
 /* Bit 42 */   Uint8Type :1;
 /* Bit 41 */   Uint8Type vCalendarCopyToMobile:1;
 /* Bit 40 */   Uint8Type vCalendarSend:1;
                /* Byte 7 */
 /* Bit 55 */   Uint8Type :1;
 /* Bit 54 */   Uint8Type :1;
 /* Bit 53 */   Uint8Type :1;
 /* Bit 52 */   Uint8Type :1;
 /* Bit 51 */   Uint8Type :1;
 /* Bit 50 */   Uint8Type :1;
 /* Bit 49 */   Uint8Type :1;
 /* Bit 48 */   Uint8Type :1;
                /* Byte 8 */
 /* Bit 63 */   Uint8Type :1;
 /* Bit 62 */   Uint8Type :1;
 /* Bit 61 */   Uint8Type :1;
 /* Bit 60 */   Uint8Type :1;
 /* Bit 59 */   Uint8Type :1;
 /* Bit 58 */   Uint8Type :1;
 /* Bit 57 */   Uint8Type :1;
 /* Bit 56 */   Uint8Type :1;
}PbMobileServiceSupport;


typedef struct
{
    APP_PHONEBOOK_BAP_enFunctionIdType  fctId;
    BapIndication                       bapInd;
}AppPbCommon;


typedef struct
{
    BapArrayHeaderType              arrayHeader;
    ConvArrayHeaderType             convHeader;
}AppSmsBapContext;

typedef struct
{
    BapArrayHeaderType              arrayHeader;
    ConvArrayHeaderType             convHeader;
    BapSpellerMode                  spellerMode;
}AppPbContext;

typedef struct
{
    AppPbCommon                                         common;
    AppPbContext                                        appUserPb;
    AppPbContext                                        appMissedPb;
    AppPbContext                                        appDialedPb;
    AppPbContext                                        appReceivedPb;
    AppSmsBapContext                                    appSms;  
    PbMobileServiceSupport                              mobServSupp;
    BooleanType                                         bUsersPbAlreadySyncedAfterConnect;

    BapPbDwldState                                      curPbState;

    /* used to handle activation of a new or saved phonebook when syncro is finsihed */
    BooleanType                                         bActPbPending;
    BooleanType                                         bActPbIncompletePending;
    APP_PHONEBOOK_BAP_enActPBState                      pbActState;
    
    /* used to activate the old pb after sort timeout */
    BooleanType                                         bActOldPbPending;
    APP_PHONEBOOK_BAP_enActOldPBAfterSortTimeoutState   pbActOldPbState;
    //BooleanType                                         StatusArrayPending;  /*13652*/
    APP_PHONEBOOK_BAP_enChangedArrayType                enStatusArrayPending;
}AppPhoneBookBapContext;


#endif /* _APP_PHONEBOOK_BAP_CI_H */

/* End Of File APP_PHONEBOOK_BAP_CI.h */
