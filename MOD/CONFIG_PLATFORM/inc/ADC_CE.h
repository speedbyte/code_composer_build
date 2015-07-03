/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/
/** 
 * \file	ADC_CE.h
 * \brief	CE - Configuration External File. Detailed description found below.  
 * \brief	This file comprises of constants to switch on or off the interfaces that
 * can be used by other modules. It also contains various constants that can be 
 * configured by external people.
*/
/*-----------------------------------------------------------------------------
* $Date: 2009-03-25 19:15:59 +0100 (Mi, 25 Mrz 2009) $
* $Rev: 5531 $
* $Author: agrawal $
* $URL: http://subversion-01/svn/SW_HW_System/Plattform/TMS470/MOD/MOD_ADC/trunk/inc/ADC_CE.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _ADC_CE_H
#define _ADC_CE_H

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
 * \brief	interface switch to initialise ADC.
*/
#define IADC_INIT
/** 
 * \brief	interface switch to convert ADC Channels.
*/
#define IADC_CONVERT
/** 
 * \brief	interface switch to read the ADC Channels.
*/
#define IADC_RECEIVE
/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/
/** 
 * \brief	macro to get ADC value corresponding to a particular channel.
*/
#define ADC_U16GETADCVALUE(x) ADC_u16GetAdcValue(ADC1, x)
/*----------------------------------------------------------------------------*/
/* Public Constants                                                           */
/*----------------------------------------------------------------------------*/
/** 
    \brief	configuring ADC_channel 0 as Audio Left  Plus Diagnosis.
*/
#define ADC_DIAG_AUDIOLEFT_PLUS AdcChn0
/** 
    \brief	configuring ADC_channel 1 as Audio Left  Minus Diagnosis.
*/
#define ADC_DIAG_AUDIOLEFT_MINUS AdcChn1
/** 
    \brief	configuring ADC_channel 2 as Audio Right Plus Diagnosis.
*/
#define ADC_DIAG_AUDIORIGHT_PLUS AdcChn2
/** 
    \brief	configuring ADC_channel 3 as Audio Right Minus Diagnosis.
*/
#define ADC_DIAG_AUDIORIGHT_MINUS AdcChn3
/** 
    \brief	configuring ADC_channel 4 as Microphone Minus Diagnosis.
*/
#define ADC_DIAG_MIKROFON_MINUS AdcChn4
/** 
    \brief	configuring ADC_channel 5 as Antenna
*/
#define ADC_ANTENNA AdcChn5
/** 
    \brief	configuring ADC_channel 6 as Radio Mute Diagnosis
*/
#define ADC_DIAG_RADIO_MUTE AdcChn6
/** 
    \brief	configuring ADC_channel 7 as 3 Button Module detection
*/
#define ADC_3BM AdcChn7
/** 
    \brief	configuring ADC_channel 8 as LED green
*/
#define ADC_LED_GREEN AdcChn8
/** 
    \brief	configuring ADC_channel 9 as LED red
*/
#define ADC_LED_RED AdcChn9
/** 
    \brief	configuring ADC_channel 10 as Baseplate
*/
#define ADC_BASEPLATE AdcChn10
/** 
    \brief	configuring ADC_channel 12 as UBatt detection
*/
#define ADC_UBATT AdcChn12
/** 
    \brief	configuring ADC_channel 13 as U7V5 detection
*/
#define ADC_U7V5 AdcChn13
/** 
    \brief	configuring ADC_channel 14 as Microphone Plus Diagnosis.
*/
#define ADC_DIAG_MIKROFON_PLUS AdcChn14
/** 
    \brief	configuring ADC_channel 15 as Temperature diagnosis
*/
#define ADC_TEMPERATURE AdcChn15
/** 
    \brief	Configuring all channels to be converted.
*/
#define ADC_CONFIGURED_CHANNELS (AdcChn0 | AdcChn1 | AdcChn2 | \
    		AdcChn3 | \
    		AdcChn4 | \
    		AdcChn5 | \
    		AdcChn6 | \
    		AdcChn7 | \
    		AdcChn8 | \
    		AdcChn9 | \
    		AdcChn10 | \
    		AdcChn11  | AdcChn12  |AdcChn13 | AdcChn14 |  AdcChn15)

/** 
\brief	Acquistion time is a configurable parameter.
		Acquisition prescale bits.
		These bits define the prescaler value for the acquisition
		(sampling) time.The acquisition time of the voltage at the ADC input is
		defined by:td(SH) = tc(ADCLK) * ACQ.1:0
		(see section 2.2.9, Compatibility Mode Prescaler and
		ADC Timing). The acquisition prescaler bits allow the
		ADC module to achieve better performance by increasing
		the acquisition time. Increased acquisition time is
		required for sources that have greater than a 10-ohm
		source impedance. Table 9 lists the encoding for the
		prescale values allowed for ACQ.1:0.
*/
#define ADC_ACQUISITION_PRESCALER Adc_enAcquistionPreScaleMul_2
/**
\brief
 * 	Clock prescale bits.
	This bit defines the prescaler value for the ADC clock
	(ADCLK) as tc(ADCLK) = tc(ICLK) * PS.2:0.
*/
#define ADC_CLOCK_PRESCALER Adc_enClockPreScaleMul_1

#endif 
/* _ADC_CE_H */

/* End Of File ADC_CE.h */

