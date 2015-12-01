/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/
/** 
 * \file 	MONITOR_ADC.h
 * \brief	Module Header File. 
 * 			This file must be included by the application/system in order to use
 * 			the interfaces provided by this module. Detailed description found below. 
 * \brief	This file mainly contains:
 * 			1. prototypes of all the functions that can be used by other modules.
 * 			2. enum, structure, constant definition etc. that can be used by other 
 * 			modules.  
*/
/*-----------------------------------------------------------------------------
* $Date: 2010-09-02 17:58:14 +0200 (Do, 02 Sep 2010) $
* $Rev: 14875 $
* $Author: agrawal $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_MONITOR_ADC/trunk/inc/MONITOR_ADC.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _MONITOR_ADC_H
#define _MONITOR_ADC_H

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
#include "MONITOR_ADC_CE.h"

/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/
#define ADC_AUDIO_DIAG_START           0
#define ADC_AUDIO_DIAG_START_READY     2  // 2 x 50ms = 100ms
#define ADC_AUDIO_MEASUREMENT_RUNNING  6  
#define ADC_AUDIO_DIAG_STOP            7

#define MONITOR_ADC_MASK_LEFT_PLUS   0x08
#define MONITOR_ADC_MASK_LEFT_MINUS  0x04
#define MONITOR_ADC_MASK_RIGHT_PLUS  0x02
#define MONITOR_ADC_MASK_RIGHT_MINUS 0x01


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
#ifdef IMONITOR_ADC_READ /* BEGIN DECLARE INTERFACE */

Uint16Type MONITOR_ADC_u16GetMovingAverageADCValueRadioMute();

Uint16Type MONITOR_ADC_u16GetMovingAverageADCValueMikrophonePlus();

Uint16Type MONITOR_ADC_u16GetMovingAverageADCValueMikrophoneMinus();

Uint16Type MONITOR_ADC_u16GetMovingAverageADCValueAudioLeftPlus();

Uint16Type MONITOR_ADC_u16GetMovingAverageADCValueAudioLeftMinus();

Uint16Type MONITOR_ADC_u16GetMovingAverageADCValueAudioRightPlus();

Uint16Type MONITOR_ADC_u16GetMovingAverageADCValueAudioRightMinus();

Uint16Type MONITOR_ADC_u16GetMovingAverageADCValueU7V5();

Uint8Type MONITOR_ADC_u8GetMikrophoneCurrentDrain();

Uint16Type MONITOR_ADC_u16GetMovingAverageADCValue3BM(void);

Uint16Type MONITOR_ADC_u16GetMovingAverageADCValueBaseplate();

Uint16Type MONITOR_ADC_u16GetMovingAverageADCValueAntenna();

Uint16Type MONITOR_ADC_u16GetMovingAverageADCValueTemperature();

Uint16Type MONITOR_ADC_u16GetMovingAverageADCValueUBatt();

Uint8Type  MONITOR_ADC_u8GetValidSignalForErrorDetection(void);

void MONITOR_ADC_vRetriggerADC_AudioStatemachine();

#endif /* END DECLARE INTERFACE */

#ifdef IMONITOR_ADC_INIT /* BEGIN DECLARE INTERFACE */

SuccessFailType MONITOR_ADC_sfInit();

#endif /* END DECLARE INTERFACE */

#ifdef IMONITOR_ADC_TASK

SuccessFailType MONITOR_ADC_sfReadADCValueForDTCs();

#endif
/*----------------------------------------------------------------------------*/


#endif /* _MONITOR_ADC_H */

/* End Of File MONITOR_ADC.h */


