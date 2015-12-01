/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/
/** 
 * \file	ADC_PL.h	
 * \brief	This file contains prototypes of all the functions that are used
 *  within the module. Detailed description found below.
 * \brief	It also contains enum, structure, constant definition etc. that are
 * used within the module.  
*/
/*-----------------------------------------------------------------------------
* $Date: 2009-06-03 16:49:48 +0200 (Mi, 03 Jun 2009) $
* $Rev: 6457 $
* $Author: agrawal $
* $URL: http://subversion-01/svn/SW_HW_System/Plattform/TMS470/MOD/MOD_ADC/trunk/inc/ADC_PL.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _ADC_PL_H
#define _ADC_PL_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "ADC_CI.h"
/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Types                                                               */
/*----------------------------------------------------------------------------*/
typedef Uint32Type MBADCCHNSEL;
/*----------------------------------------------------------------------------*/
/* Global Variables Declarations (avoid as much as possible!)                 */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global const Declarations                                                  */
/*----------------------------------------------------------------------------*/

typedef enum
{
	Adc_enSWGp1,
	Adc_enSWGp2,
	Adc_enEvent,
    Adc_enError
} ADC_EN_FUNCTIONS;

/** 
    \brief	td(SH) = tc(ADCLK) * ACQ.1:0. This macro determines the prescaler value for the acquisition
    ( sampling ) time.
    Acquisition Prescale Value Encoding
	Note: ADCLK Determined by PS.2:0 * ICLK
	The acquisition time is also affected by the clock prescaler value (PS).
	Acquisition Prescale Bits Acquisition Prescale Value (ACQ).
	ACQ1 ACQ0 ADCLK Cycles
	0 0 2
	0 1 8
	1 0 32
	1 1 128
*/
typedef enum 
{
	Adc_enAcquistionPreScaleMul_2,
	Adc_enAcquistionPreScaleMul_8,
	Adc_enAcquistionPreScaleMul_32,
	Adc_enAcquistionPreScaleMul_128
} ADC_EN_ACQPRESCALE;

/** 
    \brief	ADCLK Determined by PS.2:0 * ICLK.  This macro determines the prescaler value for the ADC clock.
	Clock Prescale Bits
	ADCLK Clock Prescale Value (PS),
	PS2 PS1 PS0 ICLK Cycles
	0 0 0 1
	0 0 1 2
	0 1 0 3
	0 1 1 4
	1 0 0 5
	1 0 1 6
	1 1 0 7
	1 1 1 8
*/
typedef enum 
{
	Adc_enClockPreScaleMul_1,  
	Adc_enClockPreScaleMul_2,  
	Adc_enClockPreScaleMul_3, 
	Adc_enClockPreScaleMul_4,
	Adc_enClockPreScaleMul_5,
	Adc_enClockPreScaleMul_6,
	Adc_enClockPreScaleMul_7,
	Adc_enClockPreScaleMul_8
} ADC_EN_CLOCKPRESCALE;

typedef enum 
{
	AdcWro,
	AdcInt,
	AdcPoll
} MBADCMODESEL;

/*----------------------------------------------------------------------------*/
/* Public Methods                                                             */
/*----------------------------------------------------------------------------*/
#ifdef IADC_PL

SuccessFailType ADC_sfConfigureInitRegister(ADC_ST *pstAdc);

SuccessFailType ADC_sfConvStart_PL(ADC_PST pstAdc, MBADCCHNSEL  SelChn_N,  ADC_EN_FUNCTIONS GrpId_N, MBADCMODESEL SelMode_N);

Uint16Type ADC_u16ReadChannel(const ADC_PST pstAdc, Uint32Type  SelChn_N);

void ADC_SWResetEnter_V(ADC_ST *pstAdc);

void ADC_vConfigureInterrupt(ADC_ST *pstAdc, ADC_EN_FUNCTIONS u8Location, Uint8Type u8Value);

void ADC_SWResetExit_V(ADC_ST *pstAdc);

#endif
/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/

#endif /* _ADC_PL_H */

/* End Of File ADC_PL.h */



