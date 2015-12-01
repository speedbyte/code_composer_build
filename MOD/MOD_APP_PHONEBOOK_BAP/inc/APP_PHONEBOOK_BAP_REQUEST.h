/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:   APP_PHONEBOOK_BAP_REQUEST.h
*-----------------------------------------------------------------------------
* Module Name:  APP_PHONEBOOK_BAP    
*-----------------------------------------------------------------------------
* Description:  BAP specific Interfaces, Enum, Structs for APP_PHONEBOOK
*-----------------------------------------------------------------------------
* $Date: 2010-04-29 14:58:08 +0200 (Do, 29 Apr 2010) $
* $Rev: 12694 $
* $Author: weber $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_PHONEBOOK_BAP/trunk/inc/APP_PHONEBOOK_BAP_REQUEST.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _APP_PHONEBOOK_BAP_REQUEST_H
#define _APP_PHONEBOOK_BAP_REQUEST_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "UART.h"
#include "APP_PHONEBOOK_BAP_CI.h"
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
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Public Methods                                                             */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_BAP_REQUEST_sfSendMissedCallsIndication       */
/** 
    \brief      Send BAP Request with number of missed calls \n
  
    \brief      Description:\n
                Send BAP Request with number of missed calls 

    \param      missedCalls 
                Uint16Type 
    
    \return     SuccessFailType 
*/
/*----------------------------------------------------------------------------*/
SuccessFailType 
_APP_PHONEBOOK_BAP_REQUEST_sfSendMissedCallsIndication(Uint16Type missedCalls, Uint16Type missedNumbers);

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_BAP_REQUEST_sfSendPbState                     */
/** 
    \brief      update BAP PbState  
 
    \brief      Description:\n
                update BAP PbState \n 

    \param      pbDwnlState 
                BapPbDwldState
    
    \param      pbEntries
                Uint16Type   
 
    \return     SuccessFailType
                SUCCESS or FAIL 
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_PHONEBOOK_BAP_REQUEST_sfSendPbState(BapPbDwldState pbDwnlState,
                                                         Uint16Type pbEntries);

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_BAP_REQUEST_sfSendPbChangedArray             */
/** 
    \brief      Send BAP Changed Array message with start and elems \n
  
    \brief      Description:\n
                Send BAP Changed Array message with start and elems \n

    \param      start
                Uint16Type
                startIndex of changed part

    \param      reqElements
                Uint16Type
                number of changed elements

    \return     SuccessFailType 
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType 
_APP_PHONEBOOK_BAP_REQUEST_sfSendPbChangedArray( Uint16Type start, Uint16Type reqElements );


/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_BAP_REQUEST_sfSendEmptyCallListStatusArray()  */
/** 
    \brief      send Calllist BapRequest indicating that there are no entries 
                at this position  \n
  
    \brief      Description:\n
                send Calllist BapRequest indicating that there are no entries 
                at this position   \n

    \return     SuccessFailType 
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType 
_APP_PHONEBOOK_BAP_REQUEST_sfSendEmptyCallListStatusArray(BapArrayHeaderType *pRecvHeader,
                                                          fctId_t fctId );

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_BAP_REQUEST_sfSendSmsReceivedNoSmsForIndex()  */
/** 
    \brief      send SMSReceived BapRequest indicating that there is no sms at 
                this position  \n
  
    \brief      Description:\n
                send SMSReceived BapRequest indicating that there is no sms at 
                this position  \n

    \return     SuccessFailType 
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType 
_APP_PHONEBOOK_BAP_REQUEST_sfSendSmsReceivedNoSmsForIndex(const BapArrayHeaderType *pRecvHeader);


/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_BAP_REQUEST_sfSendVoiceMailBox                */
/** 
    \brief      update BAP VoiceMailboxNumber  
 
    \brief      Description:\n
                update BAP VoiceMailBoxNumber \n 
 
    \return     SuccessFailType
                SUCCESS or FAIL 
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_PHONEBOOK_BAP_REQUEST_sfSendVoiceMailBox();

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_BAP_REQUEST_sfSendSmsAttribOnly               */
/** 
    \brief      send SMSReceived BapRequest with Sms Attrib only \n
  
    \brief      Description:\n
                send SMSReceived BapRequest with Sms Attrib only \n

    \return     SuccessFailType 
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_PHONEBOOK_BAP_REQUEST_sfSendSmsAttribOnly(const BapArrayHeaderType *recvHeader,const ConvArrayHeaderType *convHeader);

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_BAP_REQUEST_sfSendSmsAttribOnly               */
/**
    \brief      send SMSReceived BapRequest with Sms Pos only \n
  
    \brief      Description:\n
                send SMSReceived BapRequest with Sms Pos only \n

    \return     SuccessFailType 
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_PHONEBOOK_BAP_REQUEST_sfSendSmsPosOnly(const BapArrayHeaderType *recvHeader,const ConvArrayHeaderType *convHeader);

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_BAP_REQUEST_sfSendSmsState                    */
/** 
    \brief      update sms state \n
  
    \brief      Description:\n
                update sms state \n
    
    \param      simReady
                Uint8Type
        
    \param      storageState
                Uint8Type

    \param      newSms
                Uint16Type    

    \return     SuccessFailType 
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_PHONEBOOK_BAP_REQUEST_sfSendSmsState(Uint8Type simReady,
                                                          Uint8Type storageState,
                                                          Uint16Type newSms);

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_BAP_REQUEST_sfHandleSmsRequestFinished                */
/** 
    \brief      handles sms request finished state   
  
    \brief      Description:\n
                handles sms request finished state \n   

    \return     SuccessFailType
                FAIL or SUCCESS 
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_PHONEBOOK_BAP_REQUEST_sfHandleSmsRequestFinished();


/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_BAP_REQUEST_sfSendSmsChangedArray      */
/** 
    \brief      Send BAP Changed Array message for received sms\n
  
    \brief      Description:\n
                Send BAP Changed Array message for received sms\n

    \return     SuccessFailType 
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_PHONEBOOK_BAP_REQUEST_sfSendSmsChangedArray( void );

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_BAP_sfSendPhonebookPosOnlyStatusArray                 */
/** 
    \brief      build up Phonebook array for BAP\n
  
    \brief      Description:\n
                build up Phonebook array for BAP \n

    \return     SuccessFailType 
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType
_APP_PHONEBOOK_BAP_REQUEST_sfSendPhonebookPosOnlyStatusArray(const BapArrayHeaderType *pRecvHeader,const ConvArrayHeaderType *pConvHeader);

SuccessFailType _APP_PHONEBOOK_BAP_REQUEST_sfSendCallListStatusArray(fctId_t callFctId);
SuccessFailType _APP_PHONEBOOK_BAP_REQUEST_sfSendMobServSupp(const PbMobileServiceSupport *pPbMss);
SuccessFailType _APP_PHONEBOOK_BAP_REQUEST_sfSendSmsDeletedChangedArray();
SuccessFailType _APP_PHONEBOOK_BAP_REQUEST_sfSendNewSmsChangedArray();
SuccessFailType _APP_PHONEBOOK_BAP_REQUEST_sfSendPhonebookStatusArray();
SuccessFailType _APP_PHONEBOOK_BAP_REQUEST_sfSendNewPbEntryChangedArray( Uint16Type newPbIndex );
SuccessFailType _APP_PHONEBOOK_BAP_REQUEST_sfHandleSmsMarkedAsRead(Uint16Type pos, BapSmsType type);
SuccessFailType _APP_PHONEBOOK_BAP_REQUEST_sfHandleSmsDeleted(void);
SuccessFailType _APP_PHONEBOOK_BAP_REQUEST_sfSendPbSpellerResult(Uint16Type index, Uint16Type number);


SuccessFailType _APP_PHONEBOOK_BAP_REQUEST_sfSendReceivedCallListChangedArray();
SuccessFailType _APP_PHONEBOOK_BAP_REQUEST_sfSendDialledCallListChangedArray();
SuccessFailType _APP_PHONEBOOK_BAP_REQUEST_sfSendMissedCallListChangedArray();

void _APP_PHONEBOOK_BAP_vSetBasicPbMobileServiceSupport(PbMobileServiceSupport *pPbMss);
#endif /* _APP_PHONEBOOK_BAP_REQUEST_H */

/* End Of File APP_PHONEBOOK_BAP_REQUEST.h */

