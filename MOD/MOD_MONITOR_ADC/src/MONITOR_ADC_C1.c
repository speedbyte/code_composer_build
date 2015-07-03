/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/
/** 
 * \file 	MONITOR_ADC_C1.c
 * \brief	This module monitors the audio channel and intimates the Diagnostic for errors.
 * \brief	This file fulfills the following requirements:
 * 			1. Requirement 1
 * 			2. Requirement 2
 * 			3. Requirement x
*/
/*-----------------------------------------------------------------------------
 * $Date: 2011-01-26 12:10:27 +0100 (Mi, 26 Jan 2011) $
 * $Rev: 17496 $
 * $Author: butz $
 * $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_MONITOR_ADC/trunk/src/MONITOR_ADC_C1.c $
 *-----------------------------------------------------------------------------
 */
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include <osek.h>           // vector
#include <osekext.h>        // vector
/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "MONITOR_ADC.h"
#include "ADC.h"
#include "SPI.h"
#include "GIO.h"
#include "PORT.h"
#include "APP_DTC.h"
#include "TRACE.h"
#include "MONITOR_ADC_C1.id"
/*----------------------------------------------------------------------------*/
/* pragma Directives                                                          */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Definitions (avoid as much as possible!)                  */
/*----------------------------------------------------------------------------*/
/** 
 \brief	example
 */
static Uint16Type _MONITOR_ADC_u16MovingAverageADCValueRadioMute=0U;
static Uint16Type _MONITOR_ADC_u16MovingAverageADCValueMikrophonePlus=0U;
static Uint16Type _MONITOR_ADC_u16MovingAverageADCValueMikrophoneMinus=0U;
static Uint16Type _MONITOR_ADC_u16MovingAverageADCValueAudioLeftPlus=0U;
static Uint16Type _MONITOR_ADC_u16MovingAverageADCValueAudioLeftMinus=0U;
static Uint16Type _MONITOR_ADC_u16MovingAverageADCValueAudioRightPlus=0U;
static Uint16Type _MONITOR_ADC_u16MovingAverageADCValueAudioRightMinus=0U;
static Uint16Type _MONITOR_ADC_u16MovingAverageADCValueU7V5=0U;
static Uint16Type _MONITOR_ADC_u16MovingAverageADCValue3BM=0U;
static Uint16Type _MONITOR_ADC_u16MovingAverageADCValueBaseplate=0U;
static Uint16Type _MONITOR_ADC_u16MovingAverageADCValueAntenna=0U;
static Uint16Type _MONITOR_ADC_u16MovingAverageADCValueTemperature=0U;
static Uint16Type _MONITOR_ADC_u16MovingAverageADCValueUBatt=0U;
static Uint8Type _MONITOR_ADC_u8ValidSignalForErrorDetection = 0x00;

/*----------------------------------------------------------------------------*/
/* Private Manifest Constant Definitions                                      */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private Types                                                              */
/*----------------------------------------------------------------------------*/
/** 
 \brief	description of the type
 */
typedef enum
{
	enRadioMute,
	enMicrophonePlus,
	enMicrophoneMinus,
	enAudioLeftPlus,
	enAudioLeftMinus,
	enAudioRightPlus,
	enAudioRightMinus,
    enU7V5,
	en3BMDiag,
	enCradleDiag,
	enAntennaDiag,
	enTemperature,
	enUBatt
} ENUM_AUDIO_LINE;
/*----------------------------------------------------------------------------*/
/* Prototypes of file local Methods                                           */
/*----------------------------------------------------------------------------*/
static SuccessFailType _MONITOR_ADC_u16MovingAverageADCValue(Uint16Type u16MovingAverageADCValueforDTC,
		ENUM_AUDIO_LINE u8AudioLineType);
/*----------------------------------------------------------------------------*/
/* File local Code Macros                                                     */
/*----------------------------------------------------------------------------*/
/** 
 \brief	description of the macros
 */

/*----------------------------------------------------------------------------*/
/* File local Variables                                                       */
/*----------------------------------------------------------------------------*/
/** 
 \brief	description of the private variable
 */
/*----------------------------------------------------------------------------*/
/* File local const Definitions                                               */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private Address Calculations                                               */
/*----------------------------------------------------------------------------*/
/* Function    : _MONITOR_ADC_u16MovingAverageADCValue                                         		  */
/** 
 \brief      Read ADC values and store it in a variable.
 
 \brief      Description:
 Read ADC Values, calculate the moving average and store it.

 \return     SuccessFailType
 If the function was successful or failed
 SUCCESS, FAIL
 */
/*----------------------------------------------------------------------------*/
static SuccessFailType _MONITOR_ADC_u16MovingAverageADCValue(Uint16Type u16MovingAverageADCValueforDTC,
		ENUM_AUDIO_LINE u8AudioLineType)
{
	switch (u8AudioLineType)
	{
		case enRadioMute:
			_MONITOR_ADC_u16MovingAverageADCValueRadioMute = (_MONITOR_ADC_u16MovingAverageADCValueRadioMute
					+ u16MovingAverageADCValueforDTC ) / 2U;
			break;
		case enMicrophonePlus:
			_MONITOR_ADC_u16MovingAverageADCValueMikrophonePlus = (_MONITOR_ADC_u16MovingAverageADCValueMikrophonePlus
					+ u16MovingAverageADCValueforDTC ) / 2U;
			break;
		case enMicrophoneMinus:
			_MONITOR_ADC_u16MovingAverageADCValueMikrophoneMinus = (_MONITOR_ADC_u16MovingAverageADCValueMikrophoneMinus
					+ u16MovingAverageADCValueforDTC ) / 2U;
			break;
		case enAudioLeftPlus:
			_MONITOR_ADC_u16MovingAverageADCValueAudioLeftPlus
					= (_MONITOR_ADC_u16MovingAverageADCValueAudioLeftPlus + u16MovingAverageADCValueforDTC )
							/ 2U;
			if ( abs(_MONITOR_ADC_u16MovingAverageADCValueAudioLeftPlus - u16MovingAverageADCValueforDTC) <= 1)
			{
				_MONITOR_ADC_u8ValidSignalForErrorDetection |= MONITOR_ADC_MASK_LEFT_PLUS;
			}
			else
			{
				DEBUG_VALUE1(MONITOR_ADC_SWI_WNG, MOD_MONITOR_ADC, "Last Left Plus Value not in range 2, Detection Invalid %d", u16MovingAverageADCValueforDTC);
				_MONITOR_ADC_u8ValidSignalForErrorDetection &= MONITOR_ADC_MASK_LEFT_PLUS;
			}
			break;
		case enAudioLeftMinus:
			_MONITOR_ADC_u16MovingAverageADCValueAudioLeftMinus
					= (_MONITOR_ADC_u16MovingAverageADCValueAudioLeftMinus + u16MovingAverageADCValueforDTC )
							/ 2U;
			if ( abs(_MONITOR_ADC_u16MovingAverageADCValueAudioLeftMinus - u16MovingAverageADCValueforDTC) <= 1)
			{
				_MONITOR_ADC_u8ValidSignalForErrorDetection |= MONITOR_ADC_MASK_LEFT_MINUS;
			}
			else
			{
				DEBUG_VALUE1(MONITOR_ADC_SWI_WNG, MOD_MONITOR_ADC, "Last Left Minus Value not in range 2, Detection Invalid %d", u16MovingAverageADCValueforDTC);
				_MONITOR_ADC_u8ValidSignalForErrorDetection &= MONITOR_ADC_MASK_LEFT_MINUS;
			}
			break;
		case enAudioRightPlus:
			_MONITOR_ADC_u16MovingAverageADCValueAudioRightPlus
					= (_MONITOR_ADC_u16MovingAverageADCValueAudioRightPlus + u16MovingAverageADCValueforDTC )
							/ 2U;
			if ( abs(_MONITOR_ADC_u16MovingAverageADCValueAudioRightPlus - u16MovingAverageADCValueforDTC) <= 1)
			{
				_MONITOR_ADC_u8ValidSignalForErrorDetection |= MONITOR_ADC_MASK_RIGHT_PLUS;
			}
			else
			{
				DEBUG_VALUE1(MONITOR_ADC_SWI_WNG, MOD_MONITOR_ADC, "Last Right Plus Value not in range 2, Detection Invalid %d", u16MovingAverageADCValueforDTC);
				_MONITOR_ADC_u8ValidSignalForErrorDetection &= MONITOR_ADC_MASK_RIGHT_PLUS;
			}
			break;
		case enAudioRightMinus:
			_MONITOR_ADC_u16MovingAverageADCValueAudioRightMinus
					= (_MONITOR_ADC_u16MovingAverageADCValueAudioRightMinus + u16MovingAverageADCValueforDTC )
							/ 2U;
			if ( abs(_MONITOR_ADC_u16MovingAverageADCValueAudioRightMinus - u16MovingAverageADCValueforDTC) <= 1)
			{
				_MONITOR_ADC_u8ValidSignalForErrorDetection |= MONITOR_ADC_MASK_RIGHT_MINUS;
			}
			else
			{
				DEBUG_VALUE1(MONITOR_ADC_SWI_WNG, MOD_MONITOR_ADC, "Last Right Minus Value not in range 2, Detection Invalid %d", u16MovingAverageADCValueforDTC);
				_MONITOR_ADC_u8ValidSignalForErrorDetection &= MONITOR_ADC_MASK_RIGHT_MINUS;
			}
			break;
		case enU7V5:
			_MONITOR_ADC_u16MovingAverageADCValueU7V5
					= (_MONITOR_ADC_u16MovingAverageADCValueU7V5 + u16MovingAverageADCValueforDTC )
							/ 2U;
			break;
		case en3BMDiag:
			_MONITOR_ADC_u16MovingAverageADCValue3BM
					= (_MONITOR_ADC_u16MovingAverageADCValue3BM + u16MovingAverageADCValueforDTC )
							/ 2U;
			break;
		case enCradleDiag:
			_MONITOR_ADC_u16MovingAverageADCValueBaseplate
					= (_MONITOR_ADC_u16MovingAverageADCValueBaseplate + u16MovingAverageADCValueforDTC )
							/ 2U;
			break;
		case enAntennaDiag:
			_MONITOR_ADC_u16MovingAverageADCValueAntenna
					= (_MONITOR_ADC_u16MovingAverageADCValueAntenna + u16MovingAverageADCValueforDTC )
							/ 2U;
			break;
		case enTemperature:
			_MONITOR_ADC_u16MovingAverageADCValueTemperature
					= (_MONITOR_ADC_u16MovingAverageADCValueTemperature + u16MovingAverageADCValueforDTC )
							/ 2U;
			break;
		case enUBatt:
			_MONITOR_ADC_u16MovingAverageADCValueUBatt
					= (_MONITOR_ADC_u16MovingAverageADCValueUBatt + u16MovingAverageADCValueforDTC )
							/ 2U;
			break;
	}
	return SUCCESS;
}
/*----------------------------------------------------------------------------*/
/* Function    : MONITOR_ADC_sfReadADCValueForAudioDTCs                                         		  */
/** 
 \brief      Read ADC values and store it in a variable.
 
 \brief      Description:
 Read ADC Values, calculate the moving average and store it.

 \return     SuccessFailType
 If the function was successful or failed
 SUCCESS, FAIL
 */
/*----------------------------------------------------------------------------*/
SuccessFailType MONITOR_ADC_sfReadADCValueForDTCs() {
	Uint16Type tempADCValueRadioMute;
	Uint16Type tempADCValueMikrophonePlus;
	Uint16Type tempADCValueMikrophoneMinus;
	Uint16Type tempADCValueAudioLeftPlus;
	Uint16Type tempADCValueAudioLeftMinus;
	Uint16Type tempADCValueAudioRightPlus;
	Uint16Type tempADCValueAudioRightMinus;
	Uint16Type tempADCValueU7V5;
	Uint16Type tempADCValue3BM;
	Uint16Type tempADCValueCradle;
	Uint16Type tempADCValueAntenna;
	Uint16Type tempADCValueTemperature;
	Uint16Type tempADCValueUBatt;
	
	tempADCValueRadioMute = ADC_U16GETADCVALUE(ADC_DIAG_RADIO_MUTE);
	tempADCValue3BM = ADC_U16GETADCVALUE(ADC_3BM);
	tempADCValueCradle = ADC_U16GETADCVALUE(ADC_BASEPLATE);
	tempADCValueMikrophonePlus = ADC_U16GETADCVALUE(ADC_DIAG_MIKROFON_PLUS);
	tempADCValueMikrophoneMinus = ADC_U16GETADCVALUE(ADC_DIAG_MIKROFON_MINUS);
	tempADCValueU7V5 = ADC_U16GETADCVALUE(ADC_U7V5);
	tempADCValueAntenna = ADC_U16GETADCVALUE(ADC_ANTENNA);
	
	tempADCValueAudioLeftPlus = ADC_U16GETADCVALUE(ADC_DIAG_AUDIOLEFT_PLUS);
	tempADCValueAudioLeftMinus = ADC_U16GETADCVALUE(ADC_DIAG_AUDIOLEFT_MINUS);
	tempADCValueAudioRightPlus = ADC_U16GETADCVALUE(ADC_DIAG_AUDIORIGHT_PLUS);
	tempADCValueAudioRightMinus = ADC_U16GETADCVALUE(ADC_DIAG_AUDIORIGHT_MINUS);

	tempADCValueTemperature = ADC_U16GETADCVALUE(ADC_TEMPERATURE);
	tempADCValueUBatt = ADC_U16GETADCVALUE(ADC_UBATT);


	
	/* Correction factor for Cradle and Antenna because their references are the Battery voltage */
#if 0  // For B2 muster ( cradle was anyways not available here. Still for some self modified boards, it is needed.)
	tempADCValueCradle = (tempADCValueCradle * 666)/(ADC_U16GETADCVALUE(ADC_UBATT));
#endif
	tempADCValueAntenna = (tempADCValueAntenna * 666)/(ADC_U16GETADCVALUE(ADC_UBATT));

	DEBUG_VALUE1(MONITOR_ADC_SWI_DBG, MOD_MONITOR_ADC, "ADCValueRadioMute = %d", tempADCValueRadioMute);
	DEBUG_VALUE1(MONITOR_ADC_SWI_DBG, MOD_MONITOR_ADC, "ADCValue3BM = %d", tempADCValue3BM);
	DEBUG_VALUE1(MONITOR_ADC_SWI_DBG, MOD_MONITOR_ADC, "ADCValueCradle = %d", tempADCValueCradle);
	DEBUG_VALUE1(MONITOR_ADC_SWI_DBG, MOD_MONITOR_ADC, "ADCValueMikrophonePlus = %d",tempADCValueMikrophonePlus);
	DEBUG_VALUE1(MONITOR_ADC_SWI_DBG, MOD_MONITOR_ADC, "ADCValueMikrophoneMinus = %d", tempADCValueMikrophoneMinus);
	DEBUG_VALUE1(MONITOR_ADC_SWI_DBG, MOD_MONITOR_ADC, "ADCValueU7V5 = %d", tempADCValueU7V5);
	DEBUG_VALUE1(MONITOR_ADC_SWI_DBG, MOD_MONITOR_ADC, "ADCValueAntenna = %d", tempADCValueAntenna);
	DEBUG_VALUE1(MONITOR_ADC_SWI_DBG, MOD_MONITOR_ADC,"ADCValueTemperature = %d", tempADCValueTemperature);
	DEBUG_VALUE1(MONITOR_ADC_SWI_DBG, MOD_MONITOR_ADC, "ADCValueUBatt = %d", tempADCValueUBatt);

    (void)_MONITOR_ADC_u16MovingAverageADCValue(tempADCValueU7V5, enU7V5);
	(void)_MONITOR_ADC_u16MovingAverageADCValue(tempADCValueRadioMute, enRadioMute);
	(void)_MONITOR_ADC_u16MovingAverageADCValue(tempADCValueMikrophonePlus, enMicrophonePlus);
	(void)_MONITOR_ADC_u16MovingAverageADCValue(tempADCValueMikrophoneMinus, enMicrophoneMinus);
	(void)_MONITOR_ADC_u16MovingAverageADCValue(tempADCValue3BM, en3BMDiag);
	(void)_MONITOR_ADC_u16MovingAverageADCValue(tempADCValueCradle, enCradleDiag);
	(void)_MONITOR_ADC_u16MovingAverageADCValue(tempADCValueAntenna, enAntennaDiag);

	if ( BT_TRUE == APP_DTC_bGetAudioAllowed())
	{
		(void)_MONITOR_ADC_u16MovingAverageADCValue(tempADCValueAudioLeftPlus, enAudioLeftPlus);
		(void)_MONITOR_ADC_u16MovingAverageADCValue(tempADCValueAudioLeftMinus, enAudioLeftMinus);
		(void)_MONITOR_ADC_u16MovingAverageADCValue(tempADCValueAudioRightPlus, enAudioRightPlus);
		(void)_MONITOR_ADC_u16MovingAverageADCValue(tempADCValueAudioRightMinus, enAudioRightMinus);
	}
	else
	{
		_MONITOR_ADC_u16MovingAverageADCValueAudioLeftPlus = tempADCValueAudioLeftPlus;
		_MONITOR_ADC_u16MovingAverageADCValueAudioLeftMinus = tempADCValueAudioLeftMinus;
		_MONITOR_ADC_u16MovingAverageADCValueAudioRightPlus = tempADCValueAudioRightPlus;
		_MONITOR_ADC_u16MovingAverageADCValueAudioRightMinus = tempADCValueAudioRightMinus;
	}

	(void)_MONITOR_ADC_u16MovingAverageADCValue(tempADCValueTemperature, enTemperature);
	(void)_MONITOR_ADC_u16MovingAverageADCValue(tempADCValueUBatt, enUBatt);

	return SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* Function    : MONITOR_ADC_sfInit     		  */
/** 
 \brief      Return Radio Mute ADC value to the application
 
 \brief      Description:
 Read ADC Values, and returns to the Application..

 \return     SuccessFailType
 */
/*----------------------------------------------------------------------------*/
SuccessFailType MONITOR_ADC_sfInit() {
	_MONITOR_ADC_u16MovingAverageADCValueRadioMute=0U;
	_MONITOR_ADC_u16MovingAverageADCValueMikrophonePlus=0U;
	_MONITOR_ADC_u16MovingAverageADCValueMikrophoneMinus=0U;
	_MONITOR_ADC_u16MovingAverageADCValueAudioLeftPlus=0U;
	_MONITOR_ADC_u16MovingAverageADCValueAudioLeftMinus=0U;
	_MONITOR_ADC_u16MovingAverageADCValueAudioRightPlus=0U;
	_MONITOR_ADC_u16MovingAverageADCValueAudioRightMinus=0U;
    _MONITOR_ADC_u16MovingAverageADCValueU7V5=0U;
	_MONITOR_ADC_u16MovingAverageADCValue3BM=0U;
	_MONITOR_ADC_u16MovingAverageADCValueBaseplate=0U;
	_MONITOR_ADC_u16MovingAverageADCValueAntenna=0U;

	_MONITOR_ADC_u16MovingAverageADCValueTemperature=0U;
	_MONITOR_ADC_u16MovingAverageADCValueUBatt=0U;
	return SUCCESS;
}


/*----------------------------------------------------------------------------*/
/* Function    : MONITOR_ADC_u16GetMovingAverageADCValueRadioMute     		  */
/** 
 \brief      Return Radio Mute ADC value to the application
 
 \brief      Description:
 Read ADC Values, and returns to the Application..

 \return     Uint16Type
 */
/*----------------------------------------------------------------------------*/
Uint16Type MONITOR_ADC_u16GetMovingAverageADCValueRadioMute() {
	return _MONITOR_ADC_u16MovingAverageADCValueRadioMute;
}

/*----------------------------------------------------------------------------*/
/* Function    : MONITOR_ADC_u16GetMovingAverageADCValueU7V5 		  */
/** 
 \brief      Return voltage 7V5 ADC value to the application
 
 \brief      Description:
 Read ADC Values, and returns to the Application..

 \return     Uint16Type
 */
/*----------------------------------------------------------------------------*/
Uint16Type MONITOR_ADC_u16GetMovingAverageADCValueU7V5() {
	return _MONITOR_ADC_u16MovingAverageADCValueU7V5;
}

/*----------------------------------------------------------------------------*/
/* Function    : MONITOR_ADC_u16GetMovingAverageADCValueMikrophonePlus 		  */
/** 
 \brief      Return Mikrophone Plus ADC value to the application
 
 \brief      Description:
 Read ADC Values, and returns to the Application..

 \return     Uint16Type
 */
/*----------------------------------------------------------------------------*/
Uint16Type MONITOR_ADC_u16GetMovingAverageADCValueMikrophonePlus() {
	return _MONITOR_ADC_u16MovingAverageADCValueMikrophonePlus;
}

/*----------------------------------------------------------------------------*/
/* Function    : MONITOR_ADC_u16GetMovingAverageADCValueMikrophoneMinus                                         		  */
/** 
 \brief      Return Mikrophone Minus ADC value to the application
 
 \brief      Description:
 Read ADC Values, and returns to the Application..

 \return     Uint16Type
 */
/*----------------------------------------------------------------------------*/
Uint16Type MONITOR_ADC_u16GetMovingAverageADCValueMikrophoneMinus() {
	return _MONITOR_ADC_u16MovingAverageADCValueMikrophoneMinus;
}

/*----------------------------------------------------------------------------*/
/* Function    : MONITOR_ADC_u8GetMikrophoneCurrentDrain                                         		  */
/** 
 \brief      Return Mikrophone current drain to APP Diag
 
 \brief      Description:
 Read ADC Values, and returns to the Application..

 \return     Uint8Type
 */
/*----------------------------------------------------------------------------*/
Uint8Type MONITOR_ADC_u8GetMikrophoneCurrentDrain() {
	
  Uint16Type u8MicrophoneCurrentValue = ((_MONITOR_ADC_u16MovingAverageADCValueU7V5 * 339 ) - (620 * _MONITOR_ADC_u16MovingAverageADCValueMikrophonePlus)) >> 10; 
  if ( u8MicrophoneCurrentValue >  255 )
  {
	  u8MicrophoneCurrentValue = 0;
  }
  return ((Uint8Type)u8MicrophoneCurrentValue);
}

/*----------------------------------------------------------------------------*/
/* Function    : MONITOR_ADC_u16GetMovingAverageADCValueAudioLeftPlus                                         		  */
/** 
 \brief      Return Audio Left Plus ADC value to the application
 
 \brief      Description:
 Read ADC Values, and returns to the Application..

 \return     Uint16Type
 */
/*----------------------------------------------------------------------------*/
Uint16Type MONITOR_ADC_u16GetMovingAverageADCValueAudioLeftPlus() {
	return _MONITOR_ADC_u16MovingAverageADCValueAudioLeftPlus;
}

/*----------------------------------------------------------------------------*/
/* Function    : MONITOR_ADC_u16GetMovingAverageADCValueAudioLeftMinus                                         		  */
/** 
 \brief      Return Audio Left Minus ADC value to the application
 
 \brief      Description:
 Read ADC Values, and returns to the Application..

 \return     Uint16Type
 */
/*----------------------------------------------------------------------------*/
Uint16Type MONITOR_ADC_u16GetMovingAverageADCValueAudioLeftMinus() {
	return _MONITOR_ADC_u16MovingAverageADCValueAudioLeftMinus;
}

/*----------------------------------------------------------------------------*/
/* Function    : MONITOR_ADC_u16GetMovingAverageADCValueAudioRightPlus                                         		  */
/** 
 \brief      Return Audio Right Plus ADC value to the application
 
 \brief      Description:
 Read ADC Values, and returns to the Application..

 \return     Uint16Type
 */
/*----------------------------------------------------------------------------*/
Uint16Type MONITOR_ADC_u16GetMovingAverageADCValueAudioRightPlus() {
	return _MONITOR_ADC_u16MovingAverageADCValueAudioRightPlus;
}

/*----------------------------------------------------------------------------*/
/* Function    : MONITOR_ADC_u16GetMovingAverageADCValueAudioRightMinus                                         		  */
/** 
 \brief      Return Audio Right Minus ADC value to the application
 
 \brief      Description:
 Read ADC Values, and returns to the Application..

 \return     Uint16Type
 */
/*----------------------------------------------------------------------------*/
Uint16Type MONITOR_ADC_u16GetMovingAverageADCValueAudioRightMinus() {
	return _MONITOR_ADC_u16MovingAverageADCValueAudioRightMinus;
}

/*----------------------------------------------------------------------------*/
/* Function    : MONITOR_ADC_u16GetMovingAverageADCValue3BM*/
/** 
 \brief      Return 3BM ADC value to the application
 
 \brief      Description:
 Read ADC Values, and returns to the Application..

 \return     Uint16Type
 */
/*----------------------------------------------------------------------------*/
Uint16Type MONITOR_ADC_u16GetMovingAverageADCValue3BM() {
	return _MONITOR_ADC_u16MovingAverageADCValue3BM;
}

/*----------------------------------------------------------------------------*/
/* Function    : MONITOR_ADC_u16GetMovingAverageADCValueBaseplate*/
/** 
 \brief      Return Cradle ADC value to the application
 
 \brief      Description:
 Read ADC Values, and returns to the Application..

 \return     Uint16Type
 */
/*----------------------------------------------------------------------------*/
Uint16Type MONITOR_ADC_u16GetMovingAverageADCValueBaseplate() {
	return _MONITOR_ADC_u16MovingAverageADCValueBaseplate;
}

/*----------------------------------------------------------------------------*/
/* Function    : MONITOR_ADC_u16GetMovingAverageADCValueAntenna*/
/** 
 \brief      Return Cradle ADC value to the application
 
 \brief      Description:
 Read ADC Values, and returns to the Application..

 \return     Uint16Type
 */
/*----------------------------------------------------------------------------*/
Uint16Type MONITOR_ADC_u16GetMovingAverageADCValueAntenna() {
	return _MONITOR_ADC_u16MovingAverageADCValueAntenna;
}

/*----------------------------------------------------------------------------*/
/* Function    : MONITOR_ADC_u16GetMovingAverageADCValueTemperature*/
/**
 \brief      Return Temperature value to the application

 \brief      Description:
 Read ADC Values, and returns to the Application..

 \return     Uint16Type
 */
/*----------------------------------------------------------------------------*/
Uint16Type MONITOR_ADC_u16GetMovingAverageADCValueTemperature() {
	return _MONITOR_ADC_u16MovingAverageADCValueTemperature;
}

/*----------------------------------------------------------------------------*/
/* Function    : MONITOR_ADC_u16GetMovingAverageADCValueUBatt*/
/**
 \brief      Return UBatt value to the application

 \brief      Description:
 Read ADC Values, and returns to the Application..

 \return     Uint16Type
 */
/*----------------------------------------------------------------------------*/
Uint16Type MONITOR_ADC_u16GetMovingAverageADCValueUBatt() {
	return _MONITOR_ADC_u16MovingAverageADCValueUBatt;
}

/*----------------------------------------------------------------------------*/
/* Function    : MONITOR_ADC_u8GetValidSignalForErrorDetection*/
/**
 \brief      Returns if the signal is valid or just a spike from the music on the audio lines.

 \brief      Description:
 It contains the bit operattion for X X X X AudioLeftPlus AudioLeftMinus AudioRightPlus AudioRightMinus

 \return     Uint8Type
 */
/*----------------------------------------------------------------------------*/
Uint8Type  MONITOR_ADC_u8GetValidSignalForErrorDetection(void)
{
	return _MONITOR_ADC_u8ValidSignalForErrorDetection;
}
/* End Of File MONITOR_ADC_C1.c */
