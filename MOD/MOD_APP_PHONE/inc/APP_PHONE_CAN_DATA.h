/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:   APP_PHONE_AT.h
*-----------------------------------------------------------------------------
* Module Name:  APP_PHONE    
*-----------------------------------------------------------------------------
* Description:  Module Global Interfaces, Enum, Structs for APP_PHONE
*-----------------------------------------------------------------------------
* $Date: 2009-06-22 15:00:21 +0200 (Mo, 22 Jun 2009) $
* $Rev: 6796 $
* $Author: rammelt $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_PHONE/trunk/inc/APP_PHONE_CAN_DATA.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _APP_PHONE_CAN_DATA_H
#define _APP_PHONE_CAN_DATA_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/

#include "ATCommonData.h"
/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "APP_PHONE_CAN.h"
#include "APP_PHONE.h"
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

void APP_PHONE_CAN_DATA_vOnAppPhoneNotification(APP_PHONE_enNotificationId notification);

/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/


/** 
\brief	description of the interface IAPP_PHONE_CAN_DATA_INTERNAL
*/
//#ifdef IAPP_PHONE_INTERNAL
extern void _APP_PHONE_CAN_DATA_vSendPhoneOn(BooleanType isPhoneOn);
extern void _APP_PHONE_CAN_DATA_vSendPhoneActiveCall(BooleanType isActive);
extern void _APP_PHONE_CAN_DATA_vSendPhoneRing(BooleanType isRinging);
extern void _APP_PHONE_CAN_DATA_vSendProvider(const Uint8Type *pProvName, Uint16Type length);
extern void _APP_PHONE_CAN_DATA_vSendFieldStrength(AT_IndicatorValue fieldStrenght);
extern void _APP_PHONE_CAN_DATA_vSendRegisterState(APP_PHONE_enCanRegisterState regState);
extern void _APP_PHONE_CAN_DATA_vMapAndSendRegisterState(AT_IndicatorValue atServiceValue, AT_IndicatorValue atRoamingValue);
//#endif


#endif /* _APP_PHONE_CAN_DATA_H */

/* End Of File APP_PHONE_CAN_DATA.h */

