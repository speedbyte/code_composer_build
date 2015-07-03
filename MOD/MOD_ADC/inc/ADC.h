/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/
/** 
 * \file 	ADC.h
 * \brief	This file contains prototypes of all the functions that can be used
 * by other modules. Detailed description found below. 
 * \brief	It also contains enum, structure, constant definition etc. that can 
 * be used by other modules. The application/system must include this file 
 * in order to use the interfaces provided by this module.
*/
/*-----------------------------------------------------------------------------
* $Date: 2009-06-03 16:49:48 +0200 (Mi, 03 Jun 2009) $
* $Rev: 6457 $
* $Author: agrawal $
* $URL: http://subversion-01/svn/SW_HW_System/Plattform/TMS470/MOD/MOD_ADC/trunk/inc/ADC.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _ADC_H
#define _ADC_H

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
#include "ADC_CE.h"

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
/**
 \brief  To select  ADC1 Interface
*/
#define ADC1 						(ADC_PST)TMS470R1_BASEADDRESS_ADC
/**
 \brief  constant for pointing to the 1st bit in ADSR1 or ADSR2 Register.
*/
#define AdcChn0		0x000001
/**
 \brief  constforel 0 pointing to the 2nd bit in ADSR1 or ADSR2 Register.
*/
#define AdcChn1		0x000002
/**
 \brief  constant for pointing to the 3rd bit in ADSR1 or ADSR2 Register.
*/
#define AdcChn2		0x000004
/**
 \brief  constant for pointing to the 4th bit in ADSR1 or ADSR2 Register.
*/
#define AdcChn3		0x000008
/**
 \brief  constant for pointing to the 5th bit in ADSR1 or ADSR2 Register.
*/
#define AdcChn4		0x000010
/**
 \brief  constant for pointing to the 6th bit in ADSR1 or ADSR2 Register.
*/
#define AdcChn5		0x000020
/**
 \brief  constant for pointing to the 7th bit in ADSR1 or ADSR2 Register.
*/
#define AdcChn6		0x000040
/**
 \brief  constant for pointing to the 8th bit in ADSR1 or ADSR2 Register.
*/
#define AdcChn7		0x000080
/**
 \brief  constant for pointing to the 9th bit in ADSR1 or ADSR2 Register.
*/
#define AdcChn8		0x000100
/**
 \brief  constant for pointing to the 10th bit in ADSR1 or ADSR2 Register.
*/
#define AdcChn9		0x000200
/**
 \brief  constant for pointing to the 11th bit in ADSR1 or ADSR2 Register.
*/
#define AdcChn10	0x000400
/**
 \brief  constant for pointing to the 12th bit in ADSR1 or ADSR2 Register.
*/
#define AdcChn11	0x000800
/**
 \brief  constant for pointing to the 13th bit in ADSR1 or ADSR2 Register.
*/
#define AdcChn12	0x001000
/**
 \brief  constant for pointing to the 14th bit in ADSR1 or ADSR2 Register.
*/
#define AdcChn13	0x002000
/**
 \brief  constant for pointing to the 15th bit in ADSR1 or ADSR2 Register.
*/
#define AdcChn14	0x004000
/**
 \brief  constant for pointing to the 16th bit in ADSR1 or ADSR2 Register.
*/
#define AdcChn15	0x008000

/*----------------------------------------------------------------------------*/
/* Public Methods                                                             */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/
#ifdef IADC_INIT /* BEGIN DECLARE INTERFACE */

SuccessFailType ADC_sfInit(ADC_PST pstAdc);

#endif /* END DECLARE INTERFACE */
/*----------------------------------------------------------------------------*/
#ifdef IADC_CONVERT  /* BEGIN DECLARE INTERFACE */

SuccessFailType ADC_sfConvStart(ADC_PST pstAdc);

#endif /* END DECLARE INTERFACE */
/*----------------------------------------------------------------------------*/
#ifdef IADC_RECEIVE  /* BEGIN DECLARE INTERFACE */

Uint16Type ADC_u16GetAdcValue(const ADC_PST pstAdc, Uint32Type SelChn_N); // called over ADC_U16GETADCVALUE(channel name)

#endif /* END DECLARE INTERFACE */
/*----------------------------------------------------------------------------*/

#endif /* _ADC_H */

/* End Of File ADC.h */


