/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:   APP_PHONEBOOK_BAP.h
*-----------------------------------------------------------------------------
* Module Name:  APP_PHONEBOOK    
*-----------------------------------------------------------------------------
* Description:  BAP specific Interfaces, Enum, Structs for APP_PHONEBOOK
*-----------------------------------------------------------------------------
* $Date: 2011-10-18 10:30:40 +0200 (Di, 18 Okt 2011) $
* $Rev: 22438 $
* $Author: maennel $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_PHONEBOOK_BAP/trunk/inc/APP_PHONEBOOK_BAP.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _APP_PHONEBOOK_BAP_H
#define _APP_PHONEBOOK_BAP_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include "APP_DEVICE.h"
#include "APP_SMS.h"
#include "APP_PHONEBOOK.h"
/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
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


SuccessFailType APP_PHONEBOOK_BAP_sfInitApp(void);
void APP_PHONEBOOK_BAP_vHandleEvents(EventMaskType event);

extern void APP_PHONEBOOK_BAP_vOnAppDeviceNotification(APP_DEVICE_enNotificationId new_notification);
extern void APP_PHONEBOOK_BAP_vOnAppSmsNotification(APP_SMS_enNotificationId notification);
extern void APP_PHONEBOOK_BAP_vOnAppPBNotification(APP_PB_enNotificationId new_notification);
extern void APP_PHONEBOOK_BAP_vOnAppCshdlNotification(APP_CSHDL_NotificationType notification);
extern void APP_PHONEBOOK_BAP_vOnBAPDispatcherNotification(APP_BAPDispatcher_NotificationType notification, fctId_t aFctId); /*RT13652*/


void APP_PHONEBOOK_BAP_vDiagSmsSupportChanged(BooleanType bIsSmsAllowed);
void APP_PHONEBOOK_BAP_vOnErmNotification(const ERMNotificationType * pNotification);
void APP_PHONEBOOK_BAP_vDeleteSms(Uint16Type start);
#endif /* _APP_PHONEBOOK_BAP_H */

/* End Of File APP_PHONEBOOK_BAP.h */

