/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:   APP_PHONEBOOK_BAP_ERM.h
*-----------------------------------------------------------------------------
* Module Name:  APP_PHONEBOOK_BAP    
*-----------------------------------------------------------------------------
* Description:  Module Global Interfaces, Enum, Structs for APP_PHONEBOOK_BAP
*-----------------------------------------------------------------------------
* $Date: 2010-05-26 18:08:33 +0200 (Mi, 26 Mai 2010) $
* $Rev: 13311 $
* $Author: brust $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_PHONEBOOK_BAP/trunk/inc/APP_PHONEBOOK_BAP_ERM.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _APP_PHONEBOOK_BAP_ERM_H
#define _APP_PHONEBOOK_BAP_ERM_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "ERM.h"
#include "APP_COMMON_Utils.h"

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
#ifndef IAPP_PHONEBOOK_BAP_EXTERNAL
#define IAPP_PHONEBOOK_BAP_EXTERNAL
#endif
#ifdef IAPP_PHONEBOOK_BAP_EXTERNAL /* BEGIN DECLARE
INTERFACE(IAPP_PHONEBOOK_BAP_EXTERNAL) */

/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONEBOOK_BAP_ERM_vHandleErmStateChange                             */
/** 
    \brief      handle State change of ERM 
  
    \brief      Description:\n
                handle State change of ERM\n

    \return     void 
*/
/*----------------------------------------------------------------------------*/
void APP_PHONEBOOK_BAP_ERM_vHandleErmStateChange(ERMStateType newState);


#endif /* END DECLARE INTERFACE(IAPP_PHONEBOOK_BAP_INTERNAL) */


#ifndef IAPP_PHONEBOOK_BAP_INTERNAL
#define IAPP_PHONEBOOK_BAP_INTERNAL
#endif

#ifdef IAPP_PHONEBOOK_BAP_INTERNAL /* BEGIN DECLARE
INTERFACE(IAPP_PHONEBOOK_BAP_INTERNAL) */
/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_BAP_vOnSmsSendCb                              */
/** 
    \brief      handle sms send result\n
  
    \brief      Description:\n
                handle sms send result \n

    \param      result
                APP_COMMON_enResponseCode
    
    \return     void 
*/
/*----------------------------------------------------------------------------*/
void _APP_PHONEBOOK_BAP_vOnSendSmsCb(APP_COMMON_enResponseCode result);
/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_BAP_vOnReadSmsCb                              */
/** 
    \brief      handle sms read result\n
  
    \brief      Description:\n
                handle sms read result \n

    \param      result
                APP_COMMON_enResponseCode
    
    \return     void 
*/
/*----------------------------------------------------------------------------*/
void _APP_PHONEBOOK_BAP_vOnReadSmsCb(APP_COMMON_enResponseCode result);
/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_BAP_vOnReadMissedCallsCb                      */
/** 
    \brief      handle read missed calls result\n
  
    \brief      Description:\n
                handle read missed calls result \n

    \param      result
                APP_COMMON_enResponseCode
    
    \return     void 
*/
/*----------------------------------------------------------------------------*/
void _APP_PHONEBOOK_BAP_vOnReadMissedCallsCb(APP_COMMON_enResponseCode result);
/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_BAP_vOnReadDialedListCb                      */
/** 
    \brief      handle read dialed list result\n
  
    \brief      Description:\n
                handle read dialed list result \n

    \param      result
                APP_COMMON_enResponseCode
    
    \return     void 
*/
/*----------------------------------------------------------------------------*/
void _APP_PHONEBOOK_BAP_vOnReadDialedListCb(APP_COMMON_enResponseCode result);
/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_BAP_vOnReadRecvCallsListCb                      */
/** 
    \brief      handle read received calls list result\n
  
    \brief      Description:\n
                handle read received calls list result \n

    \param      result
                APP_COMMON_enResponseCode
    
    \return     void 
*/
/*----------------------------------------------------------------------------*/
void _APP_PHONEBOOK_BAP_vOnReadRecvCallsListCb(APP_COMMON_enResponseCode result);
/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_BAP_vOnReadUsersPbCb                      */
/** 
    \brief      handle read users pb result\n
  
    \brief      Description:\n
                handle read users pb result \n

    \param      result
                APP_COMMON_enResponseCode
    
    \return     void 
*/
/*----------------------------------------------------------------------------*/
void _APP_PHONEBOOK_BAP_vOnReadUsersPbCb(APP_COMMON_enResponseCode result);

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_BAP_vOnMarkSmsAsReadCb                        */
/** 
    \brief      handle mark sms as read result\n
  
    \brief      Description:\n
                handle mark sms as read result \n

    \param      result
                APP_COMMON_enResponseCode
    
    \return     void 
*/
/*----------------------------------------------------------------------------*/
void _APP_PHONEBOOK_BAP_vOnMarkSmsAsReadCb(APP_COMMON_enResponseCode result);

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_BAP_vOnDeleteSmsCb                        */
/** 
    \brief      handle delete sms result\n
  
    \brief      Description:\n
                handle delete sms result \n

    \param      result
                APP_COMMON_enResponseCode
    
    \return     void 
*/
/*----------------------------------------------------------------------------*/
void _APP_PHONEBOOK_BAP_vOnHandleDeleteSmsCb(APP_COMMON_enResponseCode result);

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_BAP_sfUpdatePBDwldState                       */
/** 
    \brief      update bap pb download state 
  
    \brief      Description:\n
                update bap pb download state\n
 
    \return     SuccessFailType 
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_PHONEBOOK_sfUpdatePBDwldState();

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_BAP_vOnSearchPbCb                      */
/** 
    \brief      handle search pb result\n
  
    \brief      Description:\n
                handle search pb result \n

    \param      result
                APP_COMMON_enResponseCode
    
    \return     void 
*/
/*----------------------------------------------------------------------------*/
void _APP_PHONEBOOK_BAP_vOnSearchPbCb(APP_COMMON_enResponseCode result);

void _APP_PHONEBOOK_BAP_vOnPbStartDownloadCb(APP_COMMON_enResponseCode result);

#endif /* END DECLARE INTERFACE(IAPP_PHONEBOOK_BAP_INTERNAL) */



#endif /* _APP_PHONEBOOK_BAP_ERM_H */

/* End Of File APP_PHONEBOOK_BAP_ERM.h */

