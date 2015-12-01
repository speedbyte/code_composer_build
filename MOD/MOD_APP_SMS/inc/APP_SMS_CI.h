/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/*****************************************************************************/
/* File Name:   APP_SMS_CI.h
*-----------------------------------------------------------------------------
* Module Name:  APP_SMS
*-----------------------------------------------------------------------------
* Description:  modul global constants, declarations
*-----------------------------------------------------------------------------
* $Date: 2009-10-04 12:04:14 +0200 (So, 04 Okt 2009) $
* $Rev: 8789 $
* $Author: schmidt $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_SMS/trunk/inc/APP_SMS_CI.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _APP_SMS_CI_H
#define _APP_SMS_CI_H

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
    APP_SMS_enStateInvalid,
    APP_SMS_enRestartInit,
    APP_SMS_enSetCharset,
    APP_SMS_enStartSynchro,
    APP_SMS_enWaitSynchro,
    APP_SMS_enFinishedSynchro,
    APP_SMS_enGetFolderSizes,
    APP_SMS_enInitFinished
}APP_SMS_enInitState;

typedef enum
{
    APP_SMS_enSmsRequestStateInvalid,
    APP_SMS_enSmsRequestEntries,
    APP_SMS_enSmsRequestFinished
}APP_SMS_enRequestState;


typedef enum
{
    APP_SMS_enConcSmsNumberSend,
    APP_SMS_enConcSmsMessagesSend,
    APP_SMS_enConcSmsFinalParamSend
}APPSmsConcMode;

typedef enum
{
    APP_SMS_DELETE_NOT_SENT,
    APP_SMS_DELETE_SENT
}APP_SMS_enDeleteState;

typedef enum
{
    APP_SMS_MARK_AS_READ_NOT_SENT,
    APP_SMS_MARK_AS_READ_SENT
}APP_SMS_enMarkAsReadState;

typedef enum
{
    APP_SMS_enInvalidFct = 0,
    APP_SMS_enSmsInit,
    APP_SMS_enReadSms,
    APP_SMS_enReadSmsPreview,
    APP_SMS_enSendSms,
    APP_SMS_enSendConcSms,
    APP_SMS_enDeleteSms,
    APP_SMS_enMarkAsRead
}APP_SMS_enFctId;


typedef enum
{
    APP_SMS_enResultOk      = 0x00,
    APP_SMS_enResultError   = 0x01
}APP_SMS_enResult;

/*----------------------------------------------------------------------------*/
/* Prototypes of module global methods                                        */
/*----------------------------------------------------------------------------*/

void _APP_SMS_vReleaseFctContext();
void _APP_SMS_vNotifyApps(APP_SMS_enNotificationId event);
void _APP_SMS_vOnReadSmsResult(APP_SMS_enResult result);
void _APP_SMS_vOnSendSmsResult(APP_SMS_enResult result);
void _APP_SMS_vOnDeleteSmsResult(APP_SMS_enResult result);
void _APP_SMS_vOnMarkAsReadResult(APP_SMS_enResult result);
void _APP_SMS_vReInitApp(void);

/*----------------------------------------------------------------------------*/
/* Module global code macros                                                  */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the macros
*/
/*----------------------------------------------------------------------------*/
/* Module global variables                                                    */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the module global variable
*/




/*----------------------------------------------------------------------------*/
/* Module global const definitions                                            */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the define
*/

#define APP_SMS_UNREAD_UTF8_MAX_LEN     319

/*----------------------------------------------------------------------------*/
/* Module internal struct                                                     */
/*----------------------------------------------------------------------------*/

typedef struct
{
    APP_SMS_enFctId fctId;
    void (*callback)(APP_COMMON_enResponseCode);
}AppSmsCommonType;


typedef struct
{
    AppSmsCommonType    common;
    APP_SMS_enInitState state;
}AppSmsInitType;


typedef struct
{
    AppSmsCommonType            common;
    APPSmsConcMode              mode;
    ATPhoneNumberType           telNr;
    Uint16Type                  telNrLen;
    const Uint8Type *           msg;
    Uint16Type                  msgLen;
    Uint16Type                  currentMsgLen;
    Uint16Type                  offset;
}AppSmsConcSendType;


typedef struct
{
    AppSmsCommonType            common;
    APP_SMS_enRequestState      state; 
    ATSmsTypeEnum               lastRequestedSmsType; 
    Uint16Type                  startIndex;
    Uint16Type                  nextIndex;
    Uint16Type                  endIndex;
    AppSmsReadSmsType           sms[MAX_REQUESTED_SMS_ENTRIES];   
}AppSmsRecvdType;


typedef struct
{
    AppSmsCommonType            common;
    APP_SMS_enDeleteState       state; 
    ATSmsTypeEnum               lastSmsType; 
    Uint16Type                  index;
    Uint16Type                  elements;
}AppSmsDeleteType;

typedef struct
{
    AppSmsCommonType            common;
    APP_SMS_enMarkAsReadState   state; 
    ATSmsTypeEnum               type;
    Uint16Type                  index;
}AppSmsMarkAsReadType;

typedef struct
{
    //APP_SMS_enLastATRequestType   enLastRequestType;

    // sms stuff
    // sms specific globals
    APP_SMS_enSupportState     supportState;     
    BooleanType                bSmsSendAllowed;
    BooleanType                bInitSmsFinished;  
    Uint16Type                 unreadFolderSize;
    Uint16Type                 readFolderSize;
    // add struct for each fct which can be pending and having
    // more than one state
    union
    {
        AppSmsCommonType        common;
        AppSmsInitType          smsInit;
        AppSmsRecvdType         smsRecvd;
        AppSmsRecvdType         smsPreview;
        AppSmsCommonType        smsSend;
        AppSmsConcSendType      smsConcSend;
        AppSmsDeleteType        smsDelete;
        AppSmsMarkAsReadType    markAsRead;
    }fct;
}AppSmsContext;


#endif /* _APP_SMS_CI_H */

/* End Of File APP_SMS_CI.h */
