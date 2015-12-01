/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/
/** 
 * \file 	CONTEXT_MAN.h
 * \brief	This file contains prototypes of all the functions that can be used
 * by other modules. Detailed description found below. 
 * \brief	It also contains enum, structure, constant definition etc. that can 
 * be used by other modules. The application/system must include this file 
 * in order to use the interfaces provided by this module.
*/
/*-----------------------------------------------------------------------------
* $Date: 2010-10-26 09:44:18 +0200 (Di, 26 Okt 2010) $
* $Rev: 15913 $
* $Author: weber $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_CONTEXT_MAN/trunk/inc/CONTEXT_MAN.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _CONTEXT_MAN_H
#define _CONTEXT_MAN_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include "common.h"
#include "CONTEXT_MAN_CE.h"
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
/* Global const Declarations                                                  */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Methods                                                             */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/
#ifdef ICONTEXT_MAN_INIT /* BEGIN DECLARE INTERFACE */

extern SuccessFailType CONTEXT_MAN_sfInit(void);

#endif /* END DECLARE INTERFACE*/

#ifdef ICONTEXT_MAN_BUTTON_PRESS  /* BEGIN DECLARE INTERFACE */

SuccessFailType CONTEXT_MANAGER_sfResolveFunctionCall(LOGI_BUT_MAN_ENUM_BUTTON_NAMES enButton, LOGI_BUT_MAN_ENUM_PRESSTYPE enPressType);

#endif /* END DECLARE INTERFACE*/

#ifdef ICONTEXT_MAN_FAHRZEUG_MARKE  /* BEGIN DECLARE INTERFACE */

SuccessFailType CONTEXT_MAN_sfSetFahrzeugTyp(APP_CAN_ENUM_MARKE enMarke);
APP_CAN_ENUM_MARKE CONTEXT_MAN_sfGetFahrzeugTyp(void);

#endif /* END DECLARE INTERFACE*/

#ifdef ICONTEXT_MAN_APP_NOTIFICATIONS

void CONTEXT_MAN_vOnKpbNotification(KPB_enNotificationId notification);

#endif

#ifdef ICONTEXT_MAN_DIAG

SuccessFailType CONTEXT_MAN_sfGetInfoCallNumber(ATPhoneNumberType* number, Uint16Type* number_len);
SuccessFailType CONTEXT_MAN_sfGetServiceCallNumber(ATPhoneNumberType* number, Uint16Type* number_len);
SuccessFailType CONTEXT_MAN_sfGetSOSCallNumber(ATPhoneNumberType* number, Uint16Type* number_len);
SuccessFailType CONTEXT_MAN_sfGetInfoCallNumberRoaming(ATPhoneNumberType* number, Uint16Type* number_len);
SuccessFailType CONTEXT_MAN_sfGetServiceCallNumberRoaming(ATPhoneNumberType* number, Uint16Type* number_len);

SuccessFailType CONTEXT_MAN_sfSetInfoCallNumber(Uint8Type* number, Uint8Type number_len);
SuccessFailType CONTEXT_MAN_sfSetServiceCallNumber(Uint8Type* number, Uint8Type number_len);
SuccessFailType CONTEXT_MAN_sfSetSOSCallNumber(Uint8Type* number, Uint8Type number_len);
SuccessFailType CONTEXT_MAN_sfSetInfoCallNumberRoaming(Uint8Type* number, Uint8Type number_len);
SuccessFailType CONTEXT_MAN_sfSetServiceCallNumberRoaming(Uint8Type* number, Uint8Type number_len);

#endif

void CONTEXT_MAN_vHandleEvents(EventMaskType event);

void CONTEXT_MAN_vOnContextManNotification(const ERMNotificationType * pNotification);

#endif /* _CONTEXT_MAN_H */

/* End Of File CONTEXT_MAN.h */
