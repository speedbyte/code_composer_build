/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/
/** 
 * \file 	APP_DTC.h
 * \brief	This file contains prototypes of all the functions that can be used
 * by other modules. Detailed description found below. 
 * \brief	It also contains enum, structure, constant definition etc. that can 
 * be used by other modules. The application/system must include this file 
 * in order to use the interfaces provided by this module.
*/
/*-----------------------------------------------------------------------------
* $Date: 2010-10-18 14:21:54 +0200 (Mo, 18 Okt 2010) $
* $Rev: 15771 $
* $Author: agrawal $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_DTC/trunk/inc/APP_DTC.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _APP_DTC_H
#define _APP_DTC_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include "Common.h"
#include "tms470r1.h"

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
/* include the export configuration file */
#include "APP_DTC_CE.h"

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
#ifdef IAPP_DTC_INIT

SuccessFailType APP_DTC_sfInit(void);
void APP_DTC_vStartupSequence(void);

#endif

#ifdef IAPP_DTC_TASK_APP

void APP_DTC_vSoftwareDiagnostics(void);
void APP_DTC_vFastRampUpPinsDiagnostics(void);
void APP_DTC_vSlowRampUpPinsDiagnostics(void);

#endif

#ifdef IAPP_DTC_DIAG_AUDIO

Uint8Type APP_DTC_u8GetStatusMikroTelefon(void);
Uint8Type APP_DTC_u8GetStatusAudioLeft(void);
Uint8Type APP_DTC_u8GetStatusAudioRight(void);
BooleanType APP_DTC_bGetAudioAllowed(void);
void APP_DTC_vSetLeftChannelAudioMonitoring(BooleanType bLeftMonitoringValue);

#endif

#ifdef IAPP_DTC_DIAG_RADIO

Uint8Type APP_DTC_u8GetStatusRadioMute(void);

#endif

#ifdef IAPP_DTC_DIAG_3BM

Uint8Type APP_DTC_u8GetStatus3ButtonModule(void);

#endif

#ifdef IAPP_DTC_DIAG_BASEPLATE

Uint8Type APP_DTC_u8GetStatusBaseplate(void);
Uint8Type APP_DTC_u8GetStatusAntenna(void);

#endif

#ifdef IAPP_DTC_CODING

Uint8Type APP_DTC_u8GetStatusCoding(void);

#endif

#ifdef IAPP_DTC_VARIANT_TYPE

Uint8Type APP_DTC_u8GetVariantType(void);

#endif

/*----------------------------------------------------------------------------*/


#endif /* _APP_DTC_H */

/* End Of File APP_DTC.h */


