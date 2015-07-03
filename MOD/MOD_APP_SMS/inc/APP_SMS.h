/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:   APP_SMS.h
*-----------------------------------------------------------------------------
* Module Name:  APP_SMS    
*-----------------------------------------------------------------------------
* Description:  Global Interfaces, Enum, Structs for APP_SMS
*-----------------------------------------------------------------------------
* $Date: 2010-04-29 14:58:08 +0200 (Do, 29 Apr 2010) $
* $Rev: 12694 $
* $Author: weber $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_SMS/trunk/inc/APP_SMS.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _APP_SMS_H
#define _APP_SMS_H

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
#include "APP_SMS_CE.h"

/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Types                                                               */
/*----------------------------------------------------------------------------*/
typedef struct
{
    BooleanType                     isValid;
    ATSmsTypeEnum                   type;     
    ATRspSmsReceiveMsgType          data;     
}AppSmsReadSmsType;

typedef enum
{
    APP_SMS_SUPPORTED,
    APP_SMS_UNSUPPORTED,
    APP_SMS_NEW_SMS,
    APP_SMS_NUMBER_OF_NEW_SMS_CHANGED,
    APP_SMS_DELETED_SMS
} APP_SMS_enNotificationId;


typedef enum
{
    APP_SMS_NOT_INITIALIZED = 0,
    APP_SMS_NOT_INITIALIZED_AND_SUPPORTED,  /* already received PMST:1 but initialization was not yet finsihed */    
    APP_SMS_INITIALIZED_AND_SUPPORTED,      /* received PMST:1 and init has been finsihed */    
    APP_SMS_INITIALIZED_AND_UNSUPPORTED     /* sms not supported, indicated e.g. through PMST:0 */
} APP_SMS_enSupportState;


typedef enum
{
    APP_SMS_enRspSd = 0,
    APP_SMS_enRspVcr,
    APP_SMS_enRspBap
}APP_SMS_enCallId;

/*----------------------------------------------------------------------------*/
/* Global Variables Declarations (avoid as much as possible!)                 */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Methods                                                             */
/*----------------------------------------------------------------------------*/

/********************************************/
// Interface for AppTask to init / run App  //
/********************************************/
SuccessFailType APP_SMS_sfInitApp(void);

void APP_SMS_vHandleEvents(EventMaskType event);

/********************************************/
// Interface for Apps to use App Sms        //
/********************************************/

/*----------------------------------------------------------------------------*/
/* Function    : APP_SMS_rcSendSms                           */
/** 
    \brief      sends sms  
  
    \brief      Description:\n
                sends sms\n

    \param      cb
                void *

    \param      telNr
                const Uint8Type *
                
    \param      telNrLen
                Uint16Type

    \param      msg
                const Uint8Type *
    
    \param      msgLen
                Uint16Type
    
    \return     APP_COMMON_enResponseCode
                APP_COMMON_RSP_OK or APP_COMMON_RSP_ERROR 
*/
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode APP_SMS_rcSendSms(void (*cb)(APP_COMMON_enResponseCode),   
                                 const Uint8Type * telNr,
                                        Uint16Type telNrLen,
                                        const Uint8Type * msg,
                                        Uint16Type msgLen);
/*----------------------------------------------------------------------------*/
/* Function    : APP_SMS_rcSendConcSms                           */
/** 
    \brief      sends sms  
  
    \brief      Description:\n
                sends sms\n

    \param      cb
                void *

    \param      telNr
                const Uint8Type *
                
    \param      telNrLen
                Uint16Type

    \param      msg
                const Uint8Type *
    
    \param      msgLen
                Uint16Type
    
    \return     APP_COMMON_enResponseCode
                APP_COMMON_RSP_OK or APP_COMMON_RSP_ERROR 
*/
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode APP_SMS_rcSendConcSms(void (*cb)(APP_COMMON_enResponseCode),   
                                                const Uint8Type * telNr,
                                                Uint16Type telNrLen,
                                                const Uint8Type * msg,
                                                Uint16Type msgLen);
/*----------------------------------------------------------------------------*/
/* Function    : APP_SMS_rcReadSms                                  */
/** 
    \brief      interface method for applications to read sms from ck5050 
  
    \brief      Description:\n
                interface method for applications to read sms from ck5050\n

    \return     APP_COMMON_enResponseCode 
                OK or ERROR ... 
*/
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode APP_SMS_rcReadSms( void (*cb)(APP_COMMON_enResponseCode),
                                             Uint16Type start, 
                                             Uint16Type elements);

/*----------------------------------------------------------------------------*/
/* Function    : APP_SMS_rcReadSms                                  */
/** 
    \brief      interface method for applications to read sms preview from ck5050 
  
    \brief      Description:\n
                interface method for applications to read sms preview from ck5050\n

    \return     APP_COMMON_enResponseCode 
                OK or ERROR ... 
*/
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode APP_SMS_rcReadSmsPreview( void (*cb)(APP_COMMON_enResponseCode),
                                                    Uint16Type start, 
                                                    Uint16Type elements);

/*----------------------------------------------------------------------------*/
/* Function    : APP_SMS_rcDeleteSms                                          */
/** 
    \brief      interface method for applications to delete sms from ck5050 
  
    \brief      Description:\n
                interface method for applications to delete sms from ck5050\n

    \return     APP_COMMON_enResponseCode 
                OK or ERROR ... 
*/
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode APP_SMS_rcDeleteSms( void (*cb)(APP_COMMON_enResponseCode),
                                               Uint16Type start, 
                                               Uint16Type elements,
                                               APP_SMS_enCallId caller);
/*----------------------------------------------------------------------------*/
/* Function    : APP_SMS_rcMarkAsRead                                         */
/** 
    \brief      interface method for applications to delete sms from ck5050 
  
    \brief      Description:\n
                interface method for applications to delete sms from ck5050\n

    \return     APP_COMMON_enResponseCode 
                OK or ERROR ... 
*/
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode APP_SMS_rcMarkAsRead( void (*cb)(APP_COMMON_enResponseCode),
                                                Uint16Type index);


Uint16Type      APP_SMS_u16GetNumReadSms();
Uint16Type      APP_SMS_u16GetNumUnreadSms();
ATSmsTypeEnum   APP_SMS_enGetSmsType(Uint16Type index); 
Uint16Type      APP_SMS_u16GetSmsTypeIndex(Uint16Type index); 
/*----------------------------------------------------------------------------*/
/* Function    : APP_SMS_enGetSmsSupportState                                 */
/** 
    \brief      returns current state of sms support
  
    \brief      Description: returns surrent state of sms support\n

    \return     APP_SMS_enSupportState 
*/
/*----------------------------------------------------------------------------*/
APP_SMS_enSupportState APP_SMS_enGetSmsSupportState();

/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONEBOOK_SMS_pGetPmrdContext                          */
/** 
    \brief      returns pointer to current pmrd context 
  
    \brief      Description:\n
                returns pointer to current pmrd context
    
    \return     const AppSmsReadSmsType *  
*/
/*----------------------------------------------------------------------------*/
const AppSmsReadSmsType *APP_SMS_pGetPmrdContext(Uint16Type index);


/*----------------------------------------------------------------------------*/
/* Function    : APP_SMS_bIsSmsInitialized                                    */
/** 
    \brief      returns whether SMS is supported and initialization has been
                finished 
  
    \brief      Description: returns whether SMS is supported and initialization has been
                finished \n

    \return     BooleanType 
*/
/*----------------------------------------------------------------------------*/
BooleanType APP_SMS_bIsSmsInitialized();


/*----------------------------------------------------------------------------*/
/* Function    : APP_SMS_vHandleATResponse                          */
/** 
    \brief      handle response msgs from AT Cmd Parser 
  
    \brief      Description:\n
                handles responses from AT Cmd Parser \n

    \param      atRsp 
                const ATRspAllType *
    
    \return     void 
*/
/*----------------------------------------------------------------------------*/
void APP_SMS_vHandleATResponse(const ATRspMessageType *atRsp);



#include "APP_PHONE.h"
void APP_SMS_vOnPhoneNotification(APP_PHONE_enNotificationId notId);


/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/


#endif /* _APP_SMS_H */

/* End Of File APP_SMS.h */

