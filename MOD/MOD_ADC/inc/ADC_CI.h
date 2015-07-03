/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/
/** 
 * \file 	ADC_CI.h
 * \brief	CI - Configuration Internal File.  Detailed description found below.
 * \brief	This file comprises of constants to switch on or off the interfaces that
 * are used internally within the module. It also contains various constants that can be 
 * configured by the module owner.
*/
/*-----------------------------------------------------------------------------
* $Date: 2009-03-25 19:15:59 +0100 (Mi, 25 Mrz 2009) $
* $Rev: 5531 $
* $Author: agrawal $
* $URL: http://subversion-01/svn/SW_HW_System/Plattform/TMS470/MOD/MOD_ADC/trunk/inc/ADC_CI.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _ADC_CI_H
#define _ADC_CI_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Import configuration                                                       */
/*----------------------------------------------------------------------------*/
/** 
 * \brief	interface switch to use functions implemented in Physical layer.
*/
#define IADC_PL
/*----------------------------------------------------------------------------*/
/* Synonym adaptions                                    					  */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Prototypes of module global methods                                        */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the method
*/
/*----------------------------------------------------------------------------*/
/* Module global code macros                                                  */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Module global variables                                                    */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the module global variable
*/

/*----------------------------------------------------------------------------*/
/* Module global const definitions                                            */
/*----------------------------------------------------------------------------*/
/** 
    \brief	To choose the mode of operation in ADC
*/
#define ADC_COMPATIBILITY_MODE
/**
    \brief	Total number of channels in ADC Peripheral.
*/
#define ADC_NUMBER_OF_CHANNELS 16
/** 
    \brief	MAX time out in milliseconds.
*/
#define ADC_MAXTIMEOUT 10

#endif /* _ADC_CI_H */

/* End Of File ADC_CI.h */






