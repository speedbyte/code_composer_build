/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:   APP_PHONE_BAP.h
*-----------------------------------------------------------------------------
* Module Name:  APP_PHONE_BAP    
*-----------------------------------------------------------------------------
* Description:  Global Interfaces, Enum, Structs for APP_PHONE_BAP
*-----------------------------------------------------------------------------
* $Date: 2011-06-06 17:55:18 +0200 (Mo, 06 Jun 2011) $
* $Rev: 20038 $
* $Author: nikolov $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_PHONE_BAP/trunk/inc/APP_PHONE_BAP.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _APP_PHONE_BAP_H
#define _APP_PHONE_BAP_H

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
#include "APP_PHONE_BAP_CE.h"
#include "APP_PHONE_BAP_CI.h"
#include "ERM.h"
#include "APP_PHONE.h"
#include "APP_DEVICE.h"
#include "BAP_Dispatcher.h"

/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/
#ifndef IAPP_PHONE_BAP_INIT
#define IAPP_PHONE_BAP_INIT
#endif
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
\brief	description of the interface IAPP_PHONE_INIT
*/
#ifdef IAPP_PHONE_BAP_INIT /* BEGIN DECLARE INTERFACE(IAPP_PHONE_INIT) */

/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONE_sfInitApp(void)                   */   
/** 
    \brief      Inits the phone application 
  
    \brief      Description:\n
                Inits the Phone Application \n
    
    \return     SuccessFailType
                success or error
        
   */
/*----------------------------------------------------------------------------*/
SuccessFailType APP_PHONE_BAP_sfInitApp(void);

#endif /* END DECLARE INTERFACE(IAPP_PHONE_INIT) */


/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONE_vHandleEvents()                                    */
/** 
    \brief      Handles incoming events 
  
    \brief      Description:\n
                Waits for incoming events from AT_Cmd_Parser and BAP-Dispatcher                
 
    \return     SuccessFailType
                success or error
        
   */
/*----------------------------------------------------------------------------*/
void APP_PHONE_BAP_vHandleEvents(EventMaskType events);
void APP_PHONE_BAP_vOnErmNotification(const ERMNotificationType * pNotification);
void APP_PHONE_BAP_vOnAppDeviceNotification(APP_DEVICE_enNotificationId notification);
void APP_PHONE_BAP_vOnAppPhoneNotification(APP_PHONE_enNotificationId notification);
void APP_PHONE_BAP_vOnAppCshdlNotification(APP_CSHDL_NotificationType notification);
void APP_PHONE_BAP_vOnBAPDispatcherNotification(APP_BAPDispatcher_NotificationType notification);


/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_BAP_vHandleHfpVersionChanged()                        */
/** 
    \brief      update mobile service support list due to change of HFP version  
 
    \brief      Description:\n
                update mobile service support list due to change of HFP version  \n
 
    \return     SuccessFailType
                SUCCESS or FAIL 
*/
/*----------------------------------------------------------------------------*/
void _APP_PHONE_BAP_vHandleHfpVersionChanged();


void _APP_PHONE_BAP_vSetBasicMobileServiceSupport(PhoneMobileServiceSupport *pPhoneMss);

BooleanType APP_PHONE_BAP_bIsUserActivated(void);
#endif /* _APP_PHONE_BAP_H */

/* End Of File APP_PHONE_BAP.h */

