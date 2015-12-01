/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:   APP_PHONEBOOK_SMS.h
*-----------------------------------------------------------------------------
* Module Name:  APP_PHONEBOOK    
*-----------------------------------------------------------------------------
* Description: sms stuff 
*-----------------------------------------------------------------------------
* $Date: 2010-03-15 12:13:46 +0100 (Mo, 15 Mrz 2010) $
* $Rev: 11558 $
* $Author: weber $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_SMS/trunk/inc/APP_SMS_Requests.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _APP_PHONEBOOK_SMS_H
#define _APP_PHONEBOOK_SMS_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "APP_SMS_CI.h"

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



//#ifdef IAPP_SMS_EXTERNAL /* BEGIN DECLARE INTERFACE(IAPP_SMS_EXTERNAL) */



//#ifdef IAPP_SMS_INTERNAL /* BEGIN DECLARE INTERFACE(IAPP_SMS_INTERNAL) */
APP_SMS_enInitState _APP_SMS_enGetNextInitState(APP_SMS_enInitState state);
SuccessFailType     _APP_SMS_sfSendSetCharsetATRequest(const SCI_ST *puart);
SuccessFailType     _APP_SMS_sfSendStartSynchroATRequest(const SCI_ST *puart);
SuccessFailType     _APP_SMS_sfSendSmsFolderSizeATRequest(const SCI_ST *puart);


/*----------------------------------------------------------------------------*/
/* Function    : _APP_SMS_sfRequestSms                              */
/** 
    \brief      request sms for given folder type  
  
    \brief      Description:\n
                request sms for given folder type \n

    \param      puart
                const SCI_ST*

    \param      type
                ATSmsTypeEnum 

    \param      index
                Uint16Type 

    \param      markAsRead
                BooleanType 

    \return     SuccessFailType
                SUCCESS or FAIL 
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SMS_sfRequestSms( const SCI_ST* puart,
                                       ATSmsTypeEnum type, 
                                       Uint16Type index,
                                       BooleanType markAsRead );

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SMS_sfRequestSmsPreview                              */
/** 
    \brief      request sms preview for given folder type  
  
    \brief      Description:\n
                request sms preview for given folder type \n

    \param      puart
                const SCI_ST*

    \param      type
                ATSmsTypeEnum 

    \param      index
                Uint16Type 

    \return     SuccessFailType
                SUCCESS or FAIL 
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SMS_sfRequestSmsPreview(const SCI_ST* puart,
                                      ATSmsTypeEnum type, 
                                      Uint16Type index );

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SMS_sfSendSmsDeleteATRequest                            */
/** 
    \brief      returns type and index of next sms to be requested   
  
    \brief      Description:\n
                returns type and index of next sms to be requested \n

    \param      puart
                const SCI_ST*

    \param      type
                ATSmsTypeEnum *

    \param      index
                Uint16Type

    \return     void 
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SMS_sfSendSmsDeleteATRequest(  const SCI_ST* puart, 
                                                    ATSmsTypeEnum type,
                                                    Uint16Type index );
//#endif /* END DECLARE INTERFACE(IAPP_SMS_INTERNAL) */


#endif /* _APP_PHONEBOOK_SMS_H */

/* End Of File APP_PHONEBOOK_SMS.h */

