/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/
/** 
 * \file 	APP_DTC_C1.c
 * \brief	this module monitors the audio channel and intimates the Diagnostic
 * \brief	This file fulfills the following requirements:
 * 			1. Diagnose Antenna if Antenna diagnose is set in Codierung.
 * 			2. Diagnose Baseplate if base plate Diagnose is set in Codierung.
 * 			3. Diagnose 3BM if 3 button module diagnose availablity is set to 1 by Codeirung.
 *          4. if the part numbers do not support baseplate, then set the DTCs to OPEN CKT.
 *             ( This is a workaround for the hardware because the pins on these HW Variants
 *             for baseplate and antenna are left open.
*/
/*-----------------------------------------------------------------------------
* $Date: 2011-11-09 15:53:14 +0100 (Mi, 09 Nov 2011) $
* $Rev: 23190 $
* $Author: butz $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_DTC/trunk/src/APP_DTC_C1.c $
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
#include "Common.h"
#include "APP_DTC.h"
#include "APP_DTC_CI.h"
#include "deh.h"
#include "DEH_DTC_CFG.h"
#include "ADC.h"
#include "APP_CAN.h"
#include "MONITOR_ADC.h"
#include "APP_COMMON_Utils.h"
#include "APP_DIAG.h"
#include "PHY_BUT_DRV.h"
#include "uds.h"
#include "OSEK_MonitorTask_H1.h"
#include "SPI.h"
#include "APP_DEVICE_AudioManager.h"
#include "APP_DTC_C1.id"
#include "TRACE.h"
#include "EEPROM_LAYOUT_CFG.h"
#include "Pwrmgr.h"
#include "TEMPERATURE.h"
/*----------------------------------------------------------------------------*/
/* pragma Directives                                                          */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Definitions (avoid as much as possible!)                  */
/*----------------------------------------------------------------------------*/
/** 
    \brief	example
*/
static BooleanType _APP_DTC_bAudioDtcAllowed = BT_FALSE;

static BooleanType _APP_DTC_bStartUpFinished = BT_FALSE;

static APP_DTC_ENUM_START_SEQUENCE _APP_DTC_enAppDtcStartSequence = enAppDtcReadValuesOnStartUp;

static Uint8Type _APP_DTC_u8CurrentVariantType = 0xFF;

static Uint16Type _APP_DTC_u8StartAudioDiagnoseStartDelay = 0;

/*----------------------------------------------------------------------------*/
/* Private Manifest Constant Definitions                                      */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private Types                                                              */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the type
*/
/*----------------------------------------------------------------------------*/
/* Prototypes of file local Methods                                           */
/*----------------------------------------------------------------------------*/
static void _APP_DTC_vMonitorMikroTelefon(void);
static void _APP_DTC_vMonitorAudioLeftChannel(void);
static void _APP_DTC_vMonitorAudioRightChannel(void);
//static void _APP_DTC_vMonitorRadioMute(void);
static void _APP_DTC_vMonitor3ButtonModule(void);
static void _APP_DTC_vMonitorBasePlate_VW(void);
static void _APP_DTC_vMonitorAntenna_VW(void);
static void _APP_DTC_vMonitorBasePlate_Skoda(void);
static void _APP_DTC_vMonitorAntenna_Skoda(void);
static void _APP_DTC_vMonitorTemperature(void);
static void _APP_DTC_vMonitorUBatt(void);
static void _APP_DTC_vCheckCodingValues(void);
static void _APP_DTC_vCheckEcuCoded(void);

/*----------------------------------------------------------------------------*/
/* File local Code Macros                                                     */
/*----------------------------------------------------------------------------*/
/** 
\brief	 description of the macros
*/
#undef AUDIO_DIAG_ALWAYS_ON
/*----------------------------------------------------------------------------*/
/* File local Variables                                                       */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the private variable
*/
typedef struct  
{
	Uint8Type u8RadioMuteStatus;
	Uint8Type u8MikrophoneStatus;
	Uint8Type u8AudioLeftStatus;
	Uint8Type u8AudioRightStatus;
	Uint8Type u83ButtonModuleStatus;
	Uint8Type u8BaseplateStatus;
	Uint8Type u8AntennaStatus;
	Uint8Type u8CodingStatus;
	Uint8Type u8TemperatureStatus;
	Uint8Type u8UBattStatus;
} APP_DTC_ST_ALL_DTCS;

static APP_DTC_ST_ALL_DTCS  APP_DTC_stAppDTC_DeviceDtcStatus;
/*----------------------------------------------------------------------------*/
/* File local const Definitions                                               */
/*----------------------------------------------------------------------------*/
//Mask for ECUDataProgrammingInfo
#define MASK_UNGULTIGE_EEPROM_DATEN				0xC0

/*----------------------------------------------------------------------------*/
/* Private Address Calculations                                               */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Function    : APP_DTC_sfInit                                                    */
/**
    \brief      initialise app dtc init values to default

    \brief      Description:
                this function initialise all app dtc variables back to
                the specific default settings.

    \return     SuccessFailType
   */
/*----------------------------------------------------------------------------*/
SuccessFailType APP_DTC_sfInit(void)
{
  SuccessFailType          sF      = SUCCESS;


  _APP_DTC_bAudioDtcAllowed = BT_FALSE;
  APP_DTC_stAppDTC_DeviceDtcStatus.u8RadioMuteStatus = APP_DIAG_DTC_OK;
  APP_DTC_stAppDTC_DeviceDtcStatus.u8MikrophoneStatus = APP_DIAG_DTC_OK;
  APP_DTC_stAppDTC_DeviceDtcStatus.u8AudioLeftStatus = APP_DIAG_DTC_INVALID;
  APP_DTC_stAppDTC_DeviceDtcStatus.u8AudioRightStatus = APP_DIAG_DTC_OK;
  APP_DTC_stAppDTC_DeviceDtcStatus.u83ButtonModuleStatus = APP_DIAG_DTC_INVALID;
  APP_DTC_stAppDTC_DeviceDtcStatus.u8BaseplateStatus = APP_DIAG_DTC_INVALID;
  APP_DTC_stAppDTC_DeviceDtcStatus.u8AntennaStatus = APP_DIAG_DTC_INVALID;
  APP_DTC_stAppDTC_DeviceDtcStatus.u8CodingStatus = APP_DIAG_DTC_OK;
  APP_DTC_stAppDTC_DeviceDtcStatus.u8TemperatureStatus = APP_DIAG_DTC_OK;
  APP_DTC_stAppDTC_DeviceDtcStatus.u8UBattStatus = APP_DIAG_DTC_OK;
  
  _APP_DTC_enAppDtcStartSequence = enAppDtcReadValuesOnStartUp;
  _APP_DTC_u8CurrentVariantType = 0xFF;

  return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_DTC_vSoftwareDiagnostics                                                    */
/**
    \brief      Startup Sequence and Software Diagnostics.

    \brief      Description:
				In this function, the start up sequence will be carried out and
				software diagnostics done for exampe ( nicht codiert, ungültige ds,
				falsch codiert etc. )

    \return     void
   */
/*----------------------------------------------------------------------------*/
void APP_DTC_vSoftwareDiagnostics(void)
{
    if( TRUE == APP_CAN_u8DTCStore() )
    {
    	_APP_DTC_vCheckCodingValues();
    	_APP_DTC_vCheckEcuCoded();
    }
    else
    {
        DEBUG_TEXT(APP_DTC_SWI_WNG, MOD_APP_DTC, "KL15Off/Under-OverVolt/StrtStpBit isSet");
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_DTC_vSlowRampUpDiagnostics                                                    */
/**
    \brief      Diagnostics for slow ramp up pins.

    \brief      Description:
                Some pins such as Power pin and Temperature pins do not require fast measurements
                as they slowly ramp up to their final state. Hence the diagnostics for these pins are
                done in 2 second cycle.

    \return     void
   */
/*----------------------------------------------------------------------------*/
void APP_DTC_vSlowRampUpPinsDiagnostics(void)
{
    _APP_DTC_vMonitorUBatt();
    if( TRUE == APP_CAN_u8DTCStore() )
    {
    	_APP_DTC_vMonitorTemperature();
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_DTC_vFastRampUpPinsDiagnostics                                                    */
/**
    \brief      Diagnostics for fast ramp up pins.

    \brief      Description:
                pins such as Audio pin and Baseplate pins etc require fast measurements
                as they qucikly ramp up to their final state. Hence the diagnostics for these pins are
                done in 50ms cycle.

    \return     void
   */
/*----------------------------------------------------------------------------*/
void APP_DTC_vFastRampUpPinsDiagnostics(void)
{
        //RT8881 Check for(APP_CAN_u8DTCStore() == TRUE) removed to allow,
        //monitoring of Button, Cradle even if CL15 = OFF.
        if((FALSE == Pwrmgr_GetCurrentScope()) || (FALSE == APP_CAN_u8GetNWDF()))
        {
            //do nothing
        }
        else
        {
        DEBUG_TEXT(APP_DTC_SWI_DBG, MOD_APP_DTC, "MONITORING START");
        if ( (_APP_DTC_u8StartAudioDiagnoseStartDelay > 10 )  && ( _APP_DTC_bAudioDtcAllowed == BT_FALSE) )
        {
			DEBUG_VALUE1(APP_DTC_SWI_TRC, MOD_APP_DTC, "Audio Diag active after count %d", _APP_DTC_u8StartAudioDiagnoseStartDelay);
			_APP_DTC_bAudioDtcAllowed = BT_TRUE;
        }
        //if ( enMuteByWire == APP_DEVICE_AM_enGetMuteType())
        //{
      	//    _APP_DTC_vMonitorRadioMute();
        //}
        _APP_DTC_vMonitorMikroTelefon();
        _APP_DTC_vMonitor3ButtonModule();
        if ( (APP_CAN_enGetMarke() != APP_CAN_SK_Skoda) )
    	{
    	    _APP_DTC_vMonitorBasePlate_VW();
    	}
    	else if ( ( APP_CAN_enGetMarke() == APP_CAN_SK_Skoda ) )
    	{
    	    _APP_DTC_vMonitorBasePlate_Skoda();
    	}
        if ( ( APP_CAN_enGetMarke() != APP_CAN_SK_Skoda ) )
        {
    		  _APP_DTC_vMonitorAntenna_VW();
        }
        else if ( (APP_CAN_enGetMarke() == APP_CAN_SK_Skoda ) )
        {
    		  _APP_DTC_vMonitorAntenna_Skoda();
        }
        if (_APP_DTC_bAudioDtcAllowed == BT_TRUE)
        {
      	    _APP_DTC_vMonitorAudioLeftChannel();
            _APP_DTC_vMonitorAudioRightChannel();
        }

        DEBUG_TEXT(APP_DTC_SWI_DBG, MOD_APP_DTC, "MONITORING END");
        }

}
#if 0
/*----------------------------------------------------------------------------*/
/* Function    : _APP_DTC_vMonitorRadioMute                                   */
/** 
    \brief      Montitors for short circuit on Radio Mute line.
  
    \brief      Description:
                corresponding DTCs are set when a error is detected. 

    \return     void

*/
/*----------------------------------------------------------------------------*/
static void _APP_DTC_vMonitorRadioMute(void)
{
	tDEH_Return retVal_u8 = DEH_OK;
	Uint16Type u16RadioMute;

	u16RadioMute = MONITOR_ADC_u16GetMovingAverageADCValueRadioMute();
	if (APP_CAN_enAudioUnmute == APP_CAN_enGetMuteStatus() )
	{  // MUTE_OUT = HIGH prüfen ,d.h. Unmute
		if (u16RadioMute > APP_DTC_RADIOMUTE_NOPROBLEM_HIGH)
		{
			semP(DEH_LockResource);
			retVal_u8 = DEH_AdditionalReportEvent(DTC_904911_STUMMSCHALTUNGRADIO_KURZSCHLUSSNACHMASSE, FALSE);
			retVal_u8 = DEH_AdditionalReportEvent(DTC_904912_STUMMSCHALTUNGRADIO_KURZSCHLUSSNACHPLUS, FALSE);
			retVal_u8 = DEH_AdditionalReportEvent(DTC_904913_STUMMSCHALTUNGRADIO_UNTERBRECHUNG, FALSE);
			semV(DEH_LockResource);

			DEBUG_TEXT(APP_DTC_SWI_DBG, MOD_APP_DTC, "No radio mute Error");
			APP_DTC_stAppDTC_DeviceDtcStatus.u8RadioMuteStatus = APP_DIAG_DTC_OK;
		}
		else if (u16RadioMute < APP_DTC_RADIOMUTE_KURZSCHLUSSNACHMASSE)
		{
			semP(DEH_LockResource);
			retVal_u8 = DEH_AdditionalReportEvent(DTC_904911_STUMMSCHALTUNGRADIO_KURZSCHLUSSNACHMASSE, TRUE);
			retVal_u8 = DEH_AdditionalReportEvent(DTC_904912_STUMMSCHALTUNGRADIO_KURZSCHLUSSNACHPLUS, FALSE);
			retVal_u8 = DEH_AdditionalReportEvent(DTC_904913_STUMMSCHALTUNGRADIO_UNTERBRECHUNG, FALSE);
			semV(DEH_LockResource);

			DEBUG_TEXT(APP_DTC_SWI_ERR, MOD_APP_DTC, "SC to Gnd radio mute");
			APP_DTC_stAppDTC_DeviceDtcStatus.u8RadioMuteStatus = APP_DIAG_DTC_SHORT_CIRCUIT_TO_GROUND;
		}
		else
		{
			semP(DEH_LockResource);
			retVal_u8 = DEH_AdditionalReportEvent(DTC_904911_STUMMSCHALTUNGRADIO_KURZSCHLUSSNACHMASSE, FALSE);
			retVal_u8 = DEH_AdditionalReportEvent(DTC_904912_STUMMSCHALTUNGRADIO_KURZSCHLUSSNACHPLUS, FALSE);
			retVal_u8 = DEH_AdditionalReportEvent(DTC_904913_STUMMSCHALTUNGRADIO_UNTERBRECHUNG, TRUE);
			semV(DEH_LockResource);

			DEBUG_TEXT(APP_DTC_SWI_ERR, MOD_APP_DTC, "open ckt radio mute error");
			APP_DTC_stAppDTC_DeviceDtcStatus.u8RadioMuteStatus = APP_DIAG_DTC_OPEN_CIRCUIT;
		}
	}
	else
	{  // MUTE_OUT = LOW prüfen ,d.h. Mute
		if (u16RadioMute < APP_DTC_RADIOMUTE_NOPROBLEM_LOW)
		{
			semP(DEH_LockResource);
			retVal_u8 = DEH_AdditionalReportEvent(DTC_904911_STUMMSCHALTUNGRADIO_KURZSCHLUSSNACHMASSE, FALSE);
			retVal_u8 = DEH_AdditionalReportEvent(DTC_904912_STUMMSCHALTUNGRADIO_KURZSCHLUSSNACHPLUS, FALSE);
			retVal_u8 = DEH_AdditionalReportEvent(DTC_904913_STUMMSCHALTUNGRADIO_UNTERBRECHUNG, FALSE);
			semV(DEH_LockResource);

			DEBUG_TEXT(APP_DTC_SWI_DBG, MOD_APP_DTC, "No radio mute Error");
			APP_DTC_stAppDTC_DeviceDtcStatus.u8RadioMuteStatus = APP_DIAG_DTC_OK;
		}
		else
		{
			semP(DEH_LockResource);
			retVal_u8 = DEH_AdditionalReportEvent(DTC_904911_STUMMSCHALTUNGRADIO_KURZSCHLUSSNACHMASSE, FALSE);
			retVal_u8 = DEH_AdditionalReportEvent(DTC_904912_STUMMSCHALTUNGRADIO_KURZSCHLUSSNACHPLUS, TRUE);
			retVal_u8 = DEH_AdditionalReportEvent(DTC_904913_STUMMSCHALTUNGRADIO_UNTERBRECHUNG, FALSE);
			semV(DEH_LockResource);

			DEBUG_TEXT(APP_DTC_SWI_ERR, MOD_APP_DTC, "SC to Plus radio mute error");
			APP_DTC_stAppDTC_DeviceDtcStatus.u8RadioMuteStatus = APP_DIAG_DTC_SHORT_CIRCUIT_TO_BATTERY;
		}
	}
	if ( retVal_u8 != DEH_OK )
	{

		DEBUG_TEXT(APP_DTC_SWI_ERR, MOD_APP_DTC, "RadioMute: deh error");
	}

	DEBUG_VALUE2(APP_DTC_SWI_DBG, MOD_APP_DTC, "status radio mute = %d, adcRadioMute = %d", 
                 APP_DTC_stAppDTC_DeviceDtcStatus.u8RadioMuteStatus, u16RadioMute);
}
#endif
/*----------------------------------------------------------------------------*/
/* Function    : _APP_DTC_vMonitorMikroTelefon                                         		  */
/** 
    \brief      Montitors for short circuit on Mikrophone line.
  
    \brief      Description:
                corresponding DTCs are set when a error is detected. 

    \return     void

*/
/*----------------------------------------------------------------------------*/
static void _APP_DTC_vMonitorMikroTelefon()
{
	
	tDEH_Return retVal_u8 = DEH_OK;
	Uint16Type u16MicrophonePlus;
	Uint16Type u16MicrophoneMinus;		
	
	u16MicrophonePlus  = MONITOR_ADC_u16GetMovingAverageADCValueMikrophonePlus(); 
	u16MicrophoneMinus = MONITOR_ADC_u16GetMovingAverageADCValueMikrophoneMinus();

    if ( ((u16MicrophonePlus > APP_DTC_MICROPHONE_NOPROBLEM_PLUS_MIN) && (u16MicrophonePlus < APP_DTC_MICROPHONE_NOPROBLEM_PLUS_MAX)) ) 
    {
			semP(DEH_LockResource);
			retVal_u8 = DEH_AdditionalReportEvent(DTC_904811_MIKROTELEFON_KURZSCHLUSSNACHMASSE, FALSE);
			retVal_u8 = DEH_AdditionalReportEvent(DTC_904812_MIKROTELEFON_KURZSCHLUSSNACHPLUS, FALSE);
			retVal_u8 = DEH_AdditionalReportEvent(DTC_904813_MIKROTELEFON_UNTERBRECHUNG, FALSE);
			semV(DEH_LockResource);

			DEBUG_TEXT(APP_DTC_SWI_DBG, MOD_APP_DTC, "No microphone Error");
			APP_DTC_stAppDTC_DeviceDtcStatus.u8MikrophoneStatus = APP_DIAG_DTC_OK;
    }
    else if ( (u16MicrophonePlus > APP_DTC_MICROPHONE_UNTERBRECHNUNG_PLUS_MIN) && (u16MicrophonePlus < APP_DTC_MICROPHONE_UNTERBRECHNUNG_PLUS_MAX)  )
    {
			semP(DEH_LockResource);
			retVal_u8 = DEH_AdditionalReportEvent(DTC_904811_MIKROTELEFON_KURZSCHLUSSNACHMASSE, FALSE);
			retVal_u8 = DEH_AdditionalReportEvent(DTC_904812_MIKROTELEFON_KURZSCHLUSSNACHPLUS, FALSE);
			retVal_u8 = DEH_AdditionalReportEvent(DTC_904813_MIKROTELEFON_UNTERBRECHUNG, TRUE);
            semV(DEH_LockResource);

            DEBUG_TEXT(APP_DTC_SWI_ERR, MOD_APP_DTC, "open ckt microphone error");
			APP_DTC_stAppDTC_DeviceDtcStatus.u8MikrophoneStatus = APP_DIAG_DTC_OPEN_CIRCUIT;
    }
    else if ( (u16MicrophonePlus > APP_DTC_MICROPHONE_KURZSCHLUSSNACHPLUS_PLUS) )
    {
			semP(DEH_LockResource);
			retVal_u8 = DEH_AdditionalReportEvent(DTC_904811_MIKROTELEFON_KURZSCHLUSSNACHMASSE, FALSE);
			retVal_u8 = DEH_AdditionalReportEvent(DTC_904812_MIKROTELEFON_KURZSCHLUSSNACHPLUS, TRUE);
			retVal_u8 = DEH_AdditionalReportEvent(DTC_904813_MIKROTELEFON_UNTERBRECHUNG, FALSE);
			semV(DEH_LockResource);

			DEBUG_TEXT(APP_DTC_SWI_ERR, MOD_APP_DTC, "SC to Plus microphone error");
			APP_DTC_stAppDTC_DeviceDtcStatus.u8MikrophoneStatus = APP_DIAG_DTC_SHORT_CIRCUIT_TO_BATTERY;
    }
    else if ( (u16MicrophonePlus < APP_DTC_MICROPHONE_KURZSCHLUSSNACHMASSE_PLUS)  )
    {
			semP(DEH_LockResource);
			retVal_u8 = DEH_AdditionalReportEvent(DTC_904811_MIKROTELEFON_KURZSCHLUSSNACHMASSE, TRUE);
			retVal_u8 = DEH_AdditionalReportEvent(DTC_904812_MIKROTELEFON_KURZSCHLUSSNACHPLUS, FALSE);
			retVal_u8 = DEH_AdditionalReportEvent(DTC_904813_MIKROTELEFON_UNTERBRECHUNG, FALSE);
			semV(DEH_LockResource);

			DEBUG_TEXT(APP_DTC_SWI_ERR, MOD_APP_DTC, "SC to Gnd microphone");
			APP_DTC_stAppDTC_DeviceDtcStatus.u8MikrophoneStatus = APP_DIAG_DTC_SHORT_CIRCUIT_TO_GROUND;
    }
	if ( retVal_u8 != DEH_OK )
	{
		DEBUG_TEXT(APP_DTC_SWI_ERR, MOD_APP_DTC, "Microphone: deh error");
	}

    DEBUG_VALUE1(APP_DTC_SWI_DBG, MOD_APP_DTC, "status microphone = %d ", APP_DTC_stAppDTC_DeviceDtcStatus.u8MikrophoneStatus);
	DEBUG_VALUE1(APP_DTC_SWI_DBG, MOD_APP_DTC, "adcMicrophonePlus = %d", u16MicrophonePlus);
	DEBUG_VALUE1(APP_DTC_SWI_DBG, MOD_APP_DTC, "adcMicrophoneMinus = %d", u16MicrophoneMinus);
	
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_DTC_vMonitorAudioLeftChannel                                         		  */
/** 
    \brief      Montitors for short circuit on Audio Left Channel.
  
    \brief      Description:
                corresponding DTCs are set when a error is detected. 

    \return     void

*/
/*----------------------------------------------------------------------------*/
static void _APP_DTC_vMonitorAudioLeftChannel(void)
{
	tDEH_Return retVal_u8 = DEH_OK;
	Uint16Type u16AudioLeftPlus;
	Uint16Type u16AudioLeftMinus;
	Uint16Type u16U7V5;
	BooleanType _APP_DTC_bAudioLeftDtcAllowed;

	u16AudioLeftPlus  = MONITOR_ADC_u16GetMovingAverageADCValueAudioLeftPlus();
	u16AudioLeftMinus = MONITOR_ADC_u16GetMovingAverageADCValueAudioLeftMinus(); 	

	u16U7V5           = MONITOR_ADC_u16GetMovingAverageADCValueU7V5();
    EEPROM_LAYOUT_vGetPlayerEnabled(&_APP_DTC_bAudioLeftDtcAllowed);
    if (u16U7V5 != 0)
    {
      u16AudioLeftPlus  = (u16AudioLeftPlus  * (APP_DTC_U7V5_CORR_VALUE / u16U7V5)) >> 4;
      u16AudioLeftMinus = (u16AudioLeftMinus * (APP_DTC_U7V5_CORR_VALUE / u16U7V5)) >> 4;
    }

    if ( (MONITOR_ADC_u8GetValidSignalForErrorDetection() & MONITOR_ADC_MASK_LEFT_PLUS ) && (MONITOR_ADC_u8GetValidSignalForErrorDetection() & MONITOR_ADC_MASK_LEFT_MINUS ) )
    {
		if ( ((u16AudioLeftPlus > APP_DTC_AUDIO_LEFT_NOPROBLEM_PLUS_MIN) && (u16AudioLeftPlus < APP_DTC_AUDIO_LEFT_NOPROBLEM_PLUS_MAX))
			 && ((u16AudioLeftMinus > APP_DTC_AUDIO_LEFT_NOPROBLEM_MINUS_MIN) && (u16AudioLeftMinus < APP_DTC_AUDIO_LEFT_NOPROBLEM_MINUS_MAX)))
		{
		  if ( (u16AudioLeftPlus - u16AudioLeftMinus) > APP_DTC_AUDIO_LEFT_NOPROBLEM_DELTA_PLUS_MINUS )
		  {
				if ( _APP_DTC_bAudioLeftDtcAllowed == BT_TRUE)
				{
					semP(DEH_LockResource);
					retVal_u8 = DEH_AdditionalReportEvent(DTC_904F11_AUDIOKANALLINKS_KURZSCHLUSSNACHMASSE, FALSE);
					retVal_u8 = DEH_AdditionalReportEvent(DTC_904F12_AUDIOKANALLINKS_KURZSCHLUSSNACHPLUS, FALSE);
					retVal_u8 = DEH_AdditionalReportEvent(DTC_904FF0_AUDIOKANALLINKS_KURZSCHLUSSUNTEREINANDER, FALSE);
					retVal_u8 = DEH_AdditionalReportEvent(DTC_904F13_AUDIOKANALLINKS_UNTERBRECHUNG, FALSE);
					semV(DEH_LockResource);
				}

				DEBUG_TEXT(APP_DTC_SWI_DBG, MOD_APP_DTC, "No audio left Error");
				DEBUG_VALUE1(APP_DTC_SWI_DBG, MOD_APP_DTC, "adcAudioLeftPlus = %d", u16AudioLeftPlus);
				DEBUG_VALUE1(APP_DTC_SWI_DBG, MOD_APP_DTC, "adcAudioLeftMinus = %d", u16AudioLeftMinus);
				APP_DTC_stAppDTC_DeviceDtcStatus.u8AudioLeftStatus = APP_DIAG_DTC_OK;
		  }
		  else
		  {
				if ( _APP_DTC_bAudioLeftDtcAllowed == BT_TRUE)
				{
					semP(DEH_LockResource);
					retVal_u8 = DEH_AdditionalReportEvent(DTC_904F11_AUDIOKANALLINKS_KURZSCHLUSSNACHMASSE, FALSE);
					retVal_u8 = DEH_AdditionalReportEvent(DTC_904F12_AUDIOKANALLINKS_KURZSCHLUSSNACHPLUS, FALSE);
					retVal_u8 = DEH_AdditionalReportEvent(DTC_904FF0_AUDIOKANALLINKS_KURZSCHLUSSUNTEREINANDER, TRUE);
					retVal_u8 = DEH_AdditionalReportEvent(DTC_904F13_AUDIOKANALLINKS_UNTERBRECHUNG, FALSE);
					semV(DEH_LockResource);
				}

				DEBUG_TEXT(APP_DTC_SWI_ERR, MOD_APP_DTC, "SC among themselves audio left Error");
				DEBUG_VALUE1(APP_DTC_SWI_ERR, MOD_APP_DTC, "adcAudioLeftPlus = %d", u16AudioLeftPlus);
				DEBUG_VALUE1(APP_DTC_SWI_ERR, MOD_APP_DTC, "adcAudioLeftMinus = %d", u16AudioLeftMinus);
				APP_DTC_stAppDTC_DeviceDtcStatus.u8AudioLeftStatus = APP_DIAG_DTC_SHORT_CIRCUIT_AMONG_THEMSELVES;
		  }
		}
		else if ( (u16AudioLeftPlus > APP_DTC_AUDIO_LEFT_UNTERBRECHNUNG_PLUS_MIN) && (u16AudioLeftPlus < APP_DTC_AUDIO_LEFT_UNTERBRECHNUNG_PLUS_MAX) && (u16AudioLeftMinus < APP_DTC_AUDIO_LEFT_UNTERBRECHNUNG_MINUS_MAX) )
		{
			if ( _APP_DTC_bAudioLeftDtcAllowed == BT_TRUE)
			{
				semP(DEH_LockResource);
				retVal_u8 = DEH_AdditionalReportEvent(DTC_904F11_AUDIOKANALLINKS_KURZSCHLUSSNACHMASSE, FALSE);
				retVal_u8 = DEH_AdditionalReportEvent(DTC_904F12_AUDIOKANALLINKS_KURZSCHLUSSNACHPLUS, FALSE);
				retVal_u8 = DEH_AdditionalReportEvent(DTC_904FF0_AUDIOKANALLINKS_KURZSCHLUSSUNTEREINANDER, FALSE);
				retVal_u8 = DEH_AdditionalReportEvent(DTC_904F13_AUDIOKANALLINKS_UNTERBRECHUNG, TRUE);
				semV(DEH_LockResource);
			}

			DEBUG_TEXT(APP_DTC_SWI_ERR, MOD_APP_DTC, "open ckt audio left error");
			DEBUG_VALUE1(APP_DTC_SWI_ERR, MOD_APP_DTC, "adcAudioLeftPlus = %d", u16AudioLeftPlus);
			DEBUG_VALUE1(APP_DTC_SWI_ERR, MOD_APP_DTC, "adcAudioLeftMinus = %d", u16AudioLeftMinus);
			APP_DTC_stAppDTC_DeviceDtcStatus.u8AudioLeftStatus = APP_DIAG_DTC_OPEN_CIRCUIT;
		}
		else if ( (u16AudioLeftPlus > APP_DTC_AUDIO_LEFT_KURZSCHLUSSNACHPLUS_PLUS_MIN) )
		{
			if ( _APP_DTC_bAudioLeftDtcAllowed == BT_TRUE)
			{
				semP(DEH_LockResource);
				retVal_u8 = DEH_AdditionalReportEvent(DTC_904F11_AUDIOKANALLINKS_KURZSCHLUSSNACHMASSE, FALSE);
				retVal_u8 = DEH_AdditionalReportEvent(DTC_904F12_AUDIOKANALLINKS_KURZSCHLUSSNACHPLUS, TRUE);
				retVal_u8 = DEH_AdditionalReportEvent(DTC_904FF0_AUDIOKANALLINKS_KURZSCHLUSSUNTEREINANDER, FALSE);
				retVal_u8 = DEH_AdditionalReportEvent(DTC_904F13_AUDIOKANALLINKS_UNTERBRECHUNG, FALSE);
				semV(DEH_LockResource);
			}

			DEBUG_TEXT(APP_DTC_SWI_ERR, MOD_APP_DTC, "SC to Plus audio left error");
			DEBUG_VALUE1(APP_DTC_SWI_ERR, MOD_APP_DTC, "adcAudioLeftPlus = %d", u16AudioLeftPlus);
			DEBUG_VALUE1(APP_DTC_SWI_ERR, MOD_APP_DTC, "adcAudioLeftMinus = %d", u16AudioLeftMinus);
			APP_DTC_stAppDTC_DeviceDtcStatus.u8AudioLeftStatus = APP_DIAG_DTC_SHORT_CIRCUIT_TO_BATTERY;
		}
		else if ( (u16AudioLeftPlus <= APP_DTC_AUDIO_LEFT_KURZSCHLUSSNACHMASSE_PLUS_MAX) && (u16AudioLeftMinus < APP_DTC_AUDIO_LEFT_KURZSCHLUSSNACHMASSE_MINUS_MAX) )
		{
			if ( _APP_DTC_bAudioLeftDtcAllowed == BT_TRUE)
			{
				semP(DEH_LockResource);
				retVal_u8 = DEH_AdditionalReportEvent(DTC_904F11_AUDIOKANALLINKS_KURZSCHLUSSNACHMASSE, TRUE);
				retVal_u8 = DEH_AdditionalReportEvent(DTC_904F12_AUDIOKANALLINKS_KURZSCHLUSSNACHPLUS, FALSE);
				retVal_u8 = DEH_AdditionalReportEvent(DTC_904FF0_AUDIOKANALLINKS_KURZSCHLUSSUNTEREINANDER, FALSE);
				retVal_u8 = DEH_AdditionalReportEvent(DTC_904F13_AUDIOKANALLINKS_UNTERBRECHUNG, FALSE);
				semV(DEH_LockResource);
			}

			DEBUG_TEXT(APP_DTC_SWI_ERR, MOD_APP_DTC, "SC to Gnd audio left");
			DEBUG_VALUE1(APP_DTC_SWI_ERR, MOD_APP_DTC, "adcAudioLeftPlus = %d", u16AudioLeftPlus);
			DEBUG_VALUE1(APP_DTC_SWI_ERR, MOD_APP_DTC, "adcAudioLeftMinus = %d", u16AudioLeftMinus);
			APP_DTC_stAppDTC_DeviceDtcStatus.u8AudioLeftStatus = APP_DIAG_DTC_SHORT_CIRCUIT_TO_GROUND;
		}
		else if (u16AudioLeftMinus > APP_DTC_AUDIO_LEFT_KURZSCHLUSSNACHPLUS_MINUS_MIN)
		{
			if ( _APP_DTC_bAudioLeftDtcAllowed == BT_TRUE)
			{
				semP(DEH_LockResource);
				retVal_u8 = DEH_AdditionalReportEvent(DTC_904F11_AUDIOKANALLINKS_KURZSCHLUSSNACHMASSE, FALSE);
				retVal_u8 = DEH_AdditionalReportEvent(DTC_904F12_AUDIOKANALLINKS_KURZSCHLUSSNACHPLUS, TRUE);
				retVal_u8 = DEH_AdditionalReportEvent(DTC_904FF0_AUDIOKANALLINKS_KURZSCHLUSSUNTEREINANDER, FALSE);
				retVal_u8 = DEH_AdditionalReportEvent(DTC_904F13_AUDIOKANALLINKS_UNTERBRECHUNG, FALSE);
				semV(DEH_LockResource);
			}

			DEBUG_TEXT(APP_DTC_SWI_ERR, MOD_APP_DTC, "SC to Plus audio left error");
			DEBUG_VALUE1(APP_DTC_SWI_ERR, MOD_APP_DTC, "adcAudioLeftPlus = %d", u16AudioLeftPlus);
			DEBUG_VALUE1(APP_DTC_SWI_ERR, MOD_APP_DTC, "adcAudioLeftMinus = %d", u16AudioLeftMinus);
			APP_DTC_stAppDTC_DeviceDtcStatus.u8AudioLeftStatus = APP_DIAG_DTC_SHORT_CIRCUIT_TO_BATTERY;
		}
		else if (u16AudioLeftPlus < APP_DTC_AUDIO_RIGHT_NOPROBLEM_PLUS_MIN)
		{
			if ( _APP_DTC_bAudioLeftDtcAllowed == BT_TRUE)
			{
				semP(DEH_LockResource);
				retVal_u8 = DEH_AdditionalReportEvent(DTC_904F11_AUDIOKANALLINKS_KURZSCHLUSSNACHMASSE, TRUE);
				retVal_u8 = DEH_AdditionalReportEvent(DTC_904F12_AUDIOKANALLINKS_KURZSCHLUSSNACHPLUS, FALSE);
				retVal_u8 = DEH_AdditionalReportEvent(DTC_904FF0_AUDIOKANALLINKS_KURZSCHLUSSUNTEREINANDER, FALSE);
				retVal_u8 = DEH_AdditionalReportEvent(DTC_904F13_AUDIOKANALLINKS_UNTERBRECHUNG, FALSE);
				semV(DEH_LockResource);
			}

			DEBUG_TEXT(APP_DTC_SWI_ERR, MOD_APP_DTC, "SC to Gnd audio left");
			DEBUG_VALUE1(APP_DTC_SWI_ERR, MOD_APP_DTC, "adcAudioLeftPlus = %d", u16AudioLeftPlus);
			DEBUG_VALUE1(APP_DTC_SWI_ERR, MOD_APP_DTC, "adcAudioLeftMinus = %d", u16AudioLeftMinus);
			APP_DTC_stAppDTC_DeviceDtcStatus.u8AudioLeftStatus = APP_DIAG_DTC_SHORT_CIRCUIT_TO_GROUND;
		}
		if ( _APP_DTC_bAudioLeftDtcAllowed == BT_FALSE)
		{
			semP(DEH_LockResource);
			retVal_u8 = DEH_AdditionalReportEvent(DTC_904F11_AUDIOKANALLINKS_KURZSCHLUSSNACHMASSE, FALSE);
			retVal_u8 = DEH_AdditionalReportEvent(DTC_904F12_AUDIOKANALLINKS_KURZSCHLUSSNACHPLUS, FALSE);
			retVal_u8 = DEH_AdditionalReportEvent(DTC_904FF0_AUDIOKANALLINKS_KURZSCHLUSSUNTEREINANDER, FALSE);
			retVal_u8 = DEH_AdditionalReportEvent(DTC_904F13_AUDIOKANALLINKS_UNTERBRECHUNG, FALSE);
			semV(DEH_LockResource);
		}
		if ( retVal_u8 != DEH_OK )
		{

			DEBUG_TEXT(APP_DTC_SWI_ERR, MOD_APP_DTC, "Audio Links: deh error");
		}
		DEBUG_VALUE1(APP_DTC_SWI_DBG, MOD_APP_DTC, "status audio left = %d", APP_DTC_stAppDTC_DeviceDtcStatus.u8AudioLeftStatus);
    }
	else
	{
		DEBUG_VALUE2(APP_DTC_SWI_ERR, MOD_APP_DTC, "Throw invalid Values Left Plus %d, Left Minus %d", u16AudioLeftPlus, u16AudioLeftMinus );
	}
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_DTC_vMonitorAudioRightChannel                                         		  */
/** 
    \brief      Montitors for short circuit on Audio Right Channel.
  
    \brief      Description:
                corresponding DTCs are set when a error is detected. 

    \return     void

*/
/*----------------------------------------------------------------------------*/
static void _APP_DTC_vMonitorAudioRightChannel(void)
{
	tDEH_Return retVal_u8 = DEH_OK;
	Uint16Type u16AudioRightPlus;
	Uint16Type u16AudioRightMinus;
	Uint16Type u16U7V5;

	u16AudioRightPlus  = MONITOR_ADC_u16GetMovingAverageADCValueAudioRightPlus();
	u16AudioRightMinus = MONITOR_ADC_u16GetMovingAverageADCValueAudioRightMinus(); 	
    u16U7V5            = MONITOR_ADC_u16GetMovingAverageADCValueU7V5();

    if (u16U7V5 != 0)
    {
      u16AudioRightPlus  = (u16AudioRightPlus  * (APP_DTC_U7V5_CORR_VALUE / u16U7V5)) >> 4;
      u16AudioRightMinus = (u16AudioRightMinus * (APP_DTC_U7V5_CORR_VALUE / u16U7V5)) >> 4;
    }
    if ( (MONITOR_ADC_u8GetValidSignalForErrorDetection() & MONITOR_ADC_MASK_RIGHT_PLUS ) && (MONITOR_ADC_u8GetValidSignalForErrorDetection() & MONITOR_ADC_MASK_RIGHT_MINUS ) )
    {
        if ( ((u16AudioRightPlus > APP_DTC_AUDIO_RIGHT_NOPROBLEM_PLUS_MIN) && (u16AudioRightPlus < APP_DTC_AUDIO_RIGHT_NOPROBLEM_PLUS_MAX))
             && ((u16AudioRightMinus > APP_DTC_AUDIO_RIGHT_NOPROBLEM_MINUS_MIN) && (u16AudioRightMinus < APP_DTC_AUDIO_RIGHT_NOPROBLEM_MINUS_MAX)) )
        {
            if ( (u16AudioRightPlus - u16AudioRightMinus) > APP_DTC_AUDIO_RIGHT_NOPROBLEM_DELTA_PLUS_MINUS )
            {
                semP(DEH_LockResource);
                retVal_u8 = DEH_AdditionalReportEvent(DTC_904E11_AUDIOKANALRECHTS_KURZSCHLUSSNACHMASSE, FALSE);
                retVal_u8 = DEH_AdditionalReportEvent(DTC_904E12_AUDIOKANALRECHTS_KURZSCHLUSSNACHPLUS, FALSE);
                retVal_u8 = DEH_AdditionalReportEvent(DTC_904EF0_AUDIOKANALRECHTS_KURZSCHLUSSUNTEREINANDER, FALSE);
                retVal_u8 = DEH_AdditionalReportEvent(DTC_904E13_AUDIOKANALRECHTS_UNTERBRECHUNG, FALSE);
                semV(DEH_LockResource);

                DEBUG_TEXT(APP_DTC_SWI_DBG, MOD_APP_DTC, "No audio right Error");
        		DEBUG_VALUE1(APP_DTC_SWI_DBG, MOD_APP_DTC, "adcAudioRightPlus = %d", u16AudioRightPlus);
        		DEBUG_VALUE1(APP_DTC_SWI_DBG, MOD_APP_DTC, "adcAudioRightMinus = %d", u16AudioRightMinus);
                APP_DTC_stAppDTC_DeviceDtcStatus.u8AudioRightStatus = APP_DIAG_DTC_OK;
            }
            else
            {
                semP(DEH_LockResource);
                retVal_u8 = DEH_AdditionalReportEvent(DTC_904E11_AUDIOKANALRECHTS_KURZSCHLUSSNACHMASSE, FALSE);
                retVal_u8 = DEH_AdditionalReportEvent(DTC_904E12_AUDIOKANALRECHTS_KURZSCHLUSSNACHPLUS, FALSE);
                retVal_u8 = DEH_AdditionalReportEvent(DTC_904EF0_AUDIOKANALRECHTS_KURZSCHLUSSUNTEREINANDER, TRUE);
                retVal_u8 = DEH_AdditionalReportEvent(DTC_904E13_AUDIOKANALRECHTS_UNTERBRECHUNG, FALSE);
                semV(DEH_LockResource);

                DEBUG_TEXT(APP_DTC_SWI_ERR, MOD_APP_DTC, "SC among themselves audio right Error");
        		DEBUG_VALUE1(APP_DTC_SWI_ERR, MOD_APP_DTC, "adcAudioRightPlus = %d", u16AudioRightPlus);
        		DEBUG_VALUE1(APP_DTC_SWI_ERR, MOD_APP_DTC, "adcAudioRightMinus = %d", u16AudioRightMinus);
                APP_DTC_stAppDTC_DeviceDtcStatus.u8AudioRightStatus = APP_DIAG_DTC_SHORT_CIRCUIT_AMONG_THEMSELVES;
          }
        }
        else if ( (u16AudioRightPlus > APP_DTC_AUDIO_RIGHT_UNTERBRECHNUNG_PLUS_MIN) && (u16AudioRightPlus < APP_DTC_AUDIO_RIGHT_UNTERBRECHNUNG_PLUS_MAX) && (u16AudioRightMinus < APP_DTC_AUDIO_RIGHT_UNTERBRECHNUNG_MINUS_MAX) )
        {
            semP(DEH_LockResource);
            retVal_u8 = DEH_AdditionalReportEvent(DTC_904E11_AUDIOKANALRECHTS_KURZSCHLUSSNACHMASSE, FALSE);
            retVal_u8 = DEH_AdditionalReportEvent(DTC_904E12_AUDIOKANALRECHTS_KURZSCHLUSSNACHPLUS, FALSE);
            retVal_u8 = DEH_AdditionalReportEvent(DTC_904EF0_AUDIOKANALRECHTS_KURZSCHLUSSUNTEREINANDER, FALSE);
            retVal_u8 = DEH_AdditionalReportEvent(DTC_904E13_AUDIOKANALRECHTS_UNTERBRECHUNG, TRUE);
            semV(DEH_LockResource);

            DEBUG_TEXT(APP_DTC_SWI_ERR, MOD_APP_DTC, "open ckt audio right error");
    		DEBUG_VALUE1(APP_DTC_SWI_ERR, MOD_APP_DTC, "adcAudioRightPlus = %d", u16AudioRightPlus);
    		DEBUG_VALUE1(APP_DTC_SWI_ERR, MOD_APP_DTC, "adcAudioRightMinus = %d", u16AudioRightMinus);
            APP_DTC_stAppDTC_DeviceDtcStatus.u8AudioRightStatus = APP_DIAG_DTC_OPEN_CIRCUIT;
        }
        else if ( (u16AudioRightPlus > APP_DTC_AUDIO_RIGHT_KURZSCHLUSSNACHPLUS_PLUS_MIN) )
        {
            semP(DEH_LockResource);
            retVal_u8 = DEH_AdditionalReportEvent(DTC_904E11_AUDIOKANALRECHTS_KURZSCHLUSSNACHMASSE, FALSE);
            retVal_u8 = DEH_AdditionalReportEvent(DTC_904E12_AUDIOKANALRECHTS_KURZSCHLUSSNACHPLUS, TRUE);
            retVal_u8 = DEH_AdditionalReportEvent(DTC_904EF0_AUDIOKANALRECHTS_KURZSCHLUSSUNTEREINANDER, FALSE);
            retVal_u8 = DEH_AdditionalReportEvent(DTC_904E13_AUDIOKANALRECHTS_UNTERBRECHUNG, FALSE);
            semV(DEH_LockResource);

            DEBUG_TEXT(APP_DTC_SWI_ERR, MOD_APP_DTC, "SC to Plus audio right error");
    		DEBUG_VALUE1(APP_DTC_SWI_ERR, MOD_APP_DTC, "adcAudioRightPlus = %d", u16AudioRightPlus);
    		DEBUG_VALUE1(APP_DTC_SWI_ERR, MOD_APP_DTC, "adcAudioRightMinus = %d", u16AudioRightMinus);
            APP_DTC_stAppDTC_DeviceDtcStatus.u8AudioRightStatus = APP_DIAG_DTC_SHORT_CIRCUIT_TO_BATTERY;
        }
        else if ( (u16AudioRightPlus <= APP_DTC_AUDIO_RIGHT_KURZSCHLUSSNACHMASSE_PLUS_MAX) && (u16AudioRightMinus < APP_DTC_AUDIO_RIGHT_KURZSCHLUSSNACHMASSE_MINUS_MAX) )
        {
    			semP(DEH_LockResource);
    		  retVal_u8 = DEH_AdditionalReportEvent(DTC_904E11_AUDIOKANALRECHTS_KURZSCHLUSSNACHMASSE, TRUE);
    		  retVal_u8 = DEH_AdditionalReportEvent(DTC_904E12_AUDIOKANALRECHTS_KURZSCHLUSSNACHPLUS, FALSE);
    		  retVal_u8 = DEH_AdditionalReportEvent(DTC_904EF0_AUDIOKANALRECHTS_KURZSCHLUSSUNTEREINANDER, FALSE);
    		  retVal_u8 = DEH_AdditionalReportEvent(DTC_904E13_AUDIOKANALRECHTS_UNTERBRECHUNG, FALSE);
    			semV(DEH_LockResource);

    			DEBUG_TEXT(APP_DTC_SWI_ERR, MOD_APP_DTC, "SC to Gnd audio right");
    			DEBUG_VALUE1(APP_DTC_SWI_ERR, MOD_APP_DTC, "adcAudioRightPlus = %d", u16AudioRightPlus);
    			DEBUG_VALUE1(APP_DTC_SWI_ERR, MOD_APP_DTC, "adcAudioRightMinus = %d", u16AudioRightMinus);
    			APP_DTC_stAppDTC_DeviceDtcStatus.u8AudioRightStatus = APP_DIAG_DTC_SHORT_CIRCUIT_TO_GROUND;
        }
        else if (u16AudioRightMinus > APP_DTC_AUDIO_RIGHT_KURZSCHLUSSNACHPLUS_MINUS_MIN)
        {
    			semP(DEH_LockResource);
    		  retVal_u8 = DEH_AdditionalReportEvent(DTC_904E11_AUDIOKANALRECHTS_KURZSCHLUSSNACHMASSE, FALSE);
    		  retVal_u8 = DEH_AdditionalReportEvent(DTC_904E12_AUDIOKANALRECHTS_KURZSCHLUSSNACHPLUS, TRUE);
    		  retVal_u8 = DEH_AdditionalReportEvent(DTC_904EF0_AUDIOKANALRECHTS_KURZSCHLUSSUNTEREINANDER, FALSE);
    		  retVal_u8 = DEH_AdditionalReportEvent(DTC_904E13_AUDIOKANALRECHTS_UNTERBRECHUNG, FALSE);
    			semV(DEH_LockResource);

    			DEBUG_TEXT(APP_DTC_SWI_ERR, MOD_APP_DTC, "SC to Plus audio right error");
    			DEBUG_VALUE1(APP_DTC_SWI_ERR, MOD_APP_DTC, "adcAudioRightPlus = %d", u16AudioRightPlus);
    			DEBUG_VALUE1(APP_DTC_SWI_ERR, MOD_APP_DTC, "adcAudioRightMinus = %d", u16AudioRightMinus);
    			APP_DTC_stAppDTC_DeviceDtcStatus.u8AudioRightStatus = APP_DIAG_DTC_SHORT_CIRCUIT_TO_BATTERY;
        }
        else if (u16AudioRightPlus < APP_DTC_AUDIO_RIGHT_NOPROBLEM_PLUS_MIN)
        {
    			semP(DEH_LockResource);
    		  retVal_u8 = DEH_AdditionalReportEvent(DTC_904E11_AUDIOKANALRECHTS_KURZSCHLUSSNACHMASSE, TRUE);
    		  retVal_u8 = DEH_AdditionalReportEvent(DTC_904E12_AUDIOKANALRECHTS_KURZSCHLUSSNACHPLUS, FALSE);
    		  retVal_u8 = DEH_AdditionalReportEvent(DTC_904EF0_AUDIOKANALRECHTS_KURZSCHLUSSUNTEREINANDER, FALSE);
    		  retVal_u8 = DEH_AdditionalReportEvent(DTC_904E13_AUDIOKANALRECHTS_UNTERBRECHUNG, FALSE);
    			semV(DEH_LockResource);

    			DEBUG_TEXT(APP_DTC_SWI_ERR, MOD_APP_DTC, "SC to Gnd audio right");
    			DEBUG_VALUE1(APP_DTC_SWI_ERR, MOD_APP_DTC, "adcAudioRightPlus = %d", u16AudioRightPlus);
    			DEBUG_VALUE1(APP_DTC_SWI_ERR, MOD_APP_DTC, "adcAudioRightMinus = %d", u16AudioRightMinus);
    			APP_DTC_stAppDTC_DeviceDtcStatus.u8AudioRightStatus = APP_DIAG_DTC_SHORT_CIRCUIT_TO_GROUND;
        }
    	if ( retVal_u8 != DEH_OK )
    	{

    		DEBUG_TEXT(APP_DTC_SWI_ERR, MOD_APP_DTC, "Audio Recht: deh error");
    	}
    	DEBUG_VALUE1(APP_DTC_SWI_DBG, MOD_APP_DTC, "status audio right = %d", APP_DTC_stAppDTC_DeviceDtcStatus.u8AudioRightStatus);
    }
    else
    {
    	DEBUG_VALUE2(APP_DTC_SWI_ERR, MOD_APP_DTC, "Throw invalid Values Right Plus %d, Right Minus %d", u16AudioRightPlus, u16AudioRightMinus );
    }

}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_DTC_vMonitor3ButtonModule                                         		  */
/** 
    \brief      Montitors for short circuit on Audio Right Channel.
  
    \brief      Description:
                corresponding DTCs are set when a error is detected. 

    \return     void
*/
/*----------------------------------------------------------------------------*/
static void _APP_DTC_vMonitor3ButtonModule(void)
{
	tDEH_Return retVal_u8 = DEH_OK;
	SuccessFailType sF = SUCCESS;
	Uint16Type u163BMDiag;
	BooleanType b3BMCoding = LOGI_BUT_MAN_bGetRooflineModuleState();
	sF = LOGI_BUT_MAN_sfGetFailureValue(enLogiButManDevice3BM);
	u163BMDiag  = MONITOR_ADC_u16GetMovingAverageADCValue3BM();
	if ( u163BMDiag < APP_DTC_3BM_MAXUNDERVOLTAGE)
    {
        if ( b3BMCoding == BT_TRUE )
        {
        	semP(DEH_LockResource);
			retVal_u8 = DEH_AdditionalReportEvent(DTC_905211_DREITASTENMODUL_KURZSCHLUSSNACHMASSE, TRUE);
			retVal_u8 = DEH_AdditionalReportEvent(DTC_905212_DREITASTENMODUL_KURZSCHLUSSNACHPLUS, FALSE);
			retVal_u8 = DEH_AdditionalReportEvent(DTC_905213_DREITASTENMODUL_UNTERBRECHUNG, FALSE);
			retVal_u8 = DEH_AdditionalReportEvent(DTC_905207_DREITASTENMODUL_MECHANISCHER_FEHLER, FALSE);
			semV(DEH_LockResource);
			DEBUG_TEXT(APP_DTC_SWI_ERR, MOD_APP_DTC, "SC to Gnd 3BM error");
        }

		APP_DTC_stAppDTC_DeviceDtcStatus.u83ButtonModuleStatus = APP_DIAG_DTC_SHORT_CIRCUIT_TO_GROUND;
    }
	else if ( (u163BMDiag >= APP_DTC_3BM_MAXUNDERVOLTAGE) && ( u163BMDiag < APP_DTC_3BM_MINOPENCIRCUIT))
    {
        if ( b3BMCoding == BT_TRUE )
        {
        	if ( sF == FAIL )
        	{
            	semP(DEH_LockResource);
        		retVal_u8 = DEH_AdditionalReportEvent(DTC_905211_DREITASTENMODUL_KURZSCHLUSSNACHMASSE, FALSE);
        		retVal_u8 = DEH_AdditionalReportEvent(DTC_905212_DREITASTENMODUL_KURZSCHLUSSNACHPLUS, FALSE);
        		retVal_u8 = DEH_AdditionalReportEvent(DTC_905213_DREITASTENMODUL_UNTERBRECHUNG, FALSE);
        		retVal_u8 = DEH_AdditionalReportEvent(DTC_905207_DREITASTENMODUL_MECHANISCHER_FEHLER, TRUE);
        		semV(DEH_LockResource);
        	}
        	else
        	{
            	semP(DEH_LockResource);
    			retVal_u8 = DEH_AdditionalReportEvent(DTC_905211_DREITASTENMODUL_KURZSCHLUSSNACHMASSE, FALSE);
    			retVal_u8 = DEH_AdditionalReportEvent(DTC_905212_DREITASTENMODUL_KURZSCHLUSSNACHPLUS, FALSE);
    			retVal_u8 = DEH_AdditionalReportEvent(DTC_905213_DREITASTENMODUL_UNTERBRECHUNG, FALSE);
    			retVal_u8 = DEH_AdditionalReportEvent(DTC_905207_DREITASTENMODUL_MECHANISCHER_FEHLER, FALSE);
    			semV(DEH_LockResource);
        	}
        }
		if ( sF == FAIL )
		{

			DEBUG_TEXT(APP_DTC_SWI_ERR, MOD_APP_DTC, "Mech failure 3bm error");
			APP_DTC_stAppDTC_DeviceDtcStatus.u83ButtonModuleStatus = APP_DIAG_DTC_MECH_FAILURE;
		}
		else
		{

			DEBUG_TEXT(APP_DTC_SWI_DBG, MOD_APP_DTC, "No 3BM Error");
			APP_DTC_stAppDTC_DeviceDtcStatus.u83ButtonModuleStatus = APP_DIAG_DTC_OK;
		}
    }
	else if ( (u163BMDiag >= APP_DTC_3BM_MINOPENCIRCUIT) && ( u163BMDiag < APP_DTC_3BM_MINHIGHVOLTAGE))
    {
        if ( b3BMCoding == BT_TRUE )
        {
        	semP(DEH_LockResource);
			retVal_u8 = DEH_AdditionalReportEvent(DTC_905211_DREITASTENMODUL_KURZSCHLUSSNACHMASSE, FALSE);
			retVal_u8 = DEH_AdditionalReportEvent(DTC_905212_DREITASTENMODUL_KURZSCHLUSSNACHPLUS, FALSE);
			retVal_u8 = DEH_AdditionalReportEvent(DTC_905213_DREITASTENMODUL_UNTERBRECHUNG, TRUE);
			retVal_u8 = DEH_AdditionalReportEvent(DTC_905207_DREITASTENMODUL_MECHANISCHER_FEHLER, FALSE);
			semV(DEH_LockResource);
			DEBUG_TEXT(APP_DTC_SWI_DBG, MOD_APP_DTC, "open ckt 3BM error");
        }
		APP_DTC_stAppDTC_DeviceDtcStatus.u83ButtonModuleStatus = APP_DIAG_DTC_OPEN_CIRCUIT;
    }
	else if ( u163BMDiag > APP_DTC_3BM_MINHIGHVOLTAGE)
    {
        if ( b3BMCoding == BT_TRUE )
        {
        	semP(DEH_LockResource);
			retVal_u8 = DEH_AdditionalReportEvent(DTC_905211_DREITASTENMODUL_KURZSCHLUSSNACHMASSE, FALSE);
			retVal_u8 = DEH_AdditionalReportEvent(DTC_905212_DREITASTENMODUL_KURZSCHLUSSNACHPLUS, TRUE);
			retVal_u8 = DEH_AdditionalReportEvent(DTC_905213_DREITASTENMODUL_UNTERBRECHUNG, FALSE);
			retVal_u8 = DEH_AdditionalReportEvent(DTC_905207_DREITASTENMODUL_MECHANISCHER_FEHLER, FALSE);
			semV(DEH_LockResource);
			DEBUG_TEXT(APP_DTC_SWI_ERR, MOD_APP_DTC, "SC to Plus 3BM error");
        }

		APP_DTC_stAppDTC_DeviceDtcStatus.u83ButtonModuleStatus = APP_DIAG_DTC_SHORT_CIRCUIT_TO_BATTERY;
    }
    if ( b3BMCoding == BT_FALSE )
    {
		semP(DEH_LockResource);
		retVal_u8 = DEH_AdditionalReportEvent(DTC_905211_DREITASTENMODUL_KURZSCHLUSSNACHMASSE, FALSE);
		retVal_u8 = DEH_AdditionalReportEvent(DTC_905212_DREITASTENMODUL_KURZSCHLUSSNACHPLUS, FALSE);
		retVal_u8 = DEH_AdditionalReportEvent(DTC_905213_DREITASTENMODUL_UNTERBRECHUNG, FALSE);
		retVal_u8 = DEH_AdditionalReportEvent(DTC_905207_DREITASTENMODUL_MECHANISCHER_FEHLER, FALSE);
		semV(DEH_LockResource);
		DEBUG_TEXT(APP_DTC_SWI_DBG, MOD_APP_DTC, "3BM DTC Off");
	}

	DEBUG_VALUE1(APP_DTC_SWI_DBG, MOD_APP_DTC, "status value 3BM = %d", APP_DTC_stAppDTC_DeviceDtcStatus.u83ButtonModuleStatus);
	DEBUG_VALUE1(APP_DTC_SWI_DBG, MOD_APP_DTC, "adc3BMDiag = %d", u163BMDiag);

    /* suppress warning of not referenced variable */ 
    (void)retVal_u8;
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_DTC_vMonitorBasePlate_VW                                         		  */
/** 
    \brief      Montitors for short circuit on Baseplate line.
  
    \brief      Description:
                corresponding DTCs are set when a error is detected. 

    \return     void
*/
/*----------------------------------------------------------------------------*/
static void _APP_DTC_vMonitorBasePlate_VW(void)
{
	tDEH_Return retVal_u8 = DEH_OK;
	Uint16Type u16BaseplateDiag;
	SuccessFailType sF = SUCCESS;
	BooleanType bBaseplateCoding = LOGI_BUT_MAN_bGetBasePlateConnectivity();
	sF = LOGI_BUT_MAN_sfGetFailureValue(enLogiButManDeviceCradle);
	u16BaseplateDiag  = MONITOR_ADC_u16GetMovingAverageADCValueBaseplate();
	if ( _APP_DTC_u8CurrentVariantType == 0x01 )
	{
		if ( u16BaseplateDiag < APP_DTC_BASEPLATE_MAXUNDERVOLTAGE_VW)
		{
			if ( bBaseplateCoding == BT_TRUE )
			{
				semP(DEH_LockResource);
				retVal_u8 = DEH_AdditionalReportEvent(DTC_905111_BASEPLATE_KURZSCHLUSSNACHMASSE, TRUE);
				retVal_u8 = DEH_AdditionalReportEvent(DTC_905112_BASEPLATE_KURZSCHLUSSNACHPLUS, FALSE);
				retVal_u8 = DEH_AdditionalReportEvent(DTC_905113_BASEPLATE_UNTERBRECHNUNG, FALSE);
				retVal_u8 = DEH_AdditionalReportEvent(DTC_905707_HANDYHALTER_MECHANISCHER_FEHLER, FALSE);
				semV(DEH_LockResource);
				DEBUG_TEXT(APP_DTC_SWI_ERR, MOD_APP_DTC, "SC to Gnd Baseplate error");
			}
			APP_DTC_stAppDTC_DeviceDtcStatus.u8BaseplateStatus = APP_DIAG_DTC_SHORT_CIRCUIT_TO_GROUND;
		}
		else if ( (u16BaseplateDiag >= APP_DTC_BASEPLATE_MAXUNDERVOLTAGE_VW) && ( u16BaseplateDiag < APP_DTC_BASEPLATE_MINOPENCIRCUIT_VW))
		{
			if ( bBaseplateCoding == BT_TRUE )
			{
				if ( sF == FAIL )
				{
					semP(DEH_LockResource);
					retVal_u8 = DEH_AdditionalReportEvent(DTC_905111_BASEPLATE_KURZSCHLUSSNACHMASSE, FALSE);
					retVal_u8 = DEH_AdditionalReportEvent(DTC_905112_BASEPLATE_KURZSCHLUSSNACHPLUS, FALSE);
					retVal_u8 = DEH_AdditionalReportEvent(DTC_905113_BASEPLATE_UNTERBRECHNUNG, FALSE);
					retVal_u8 = DEH_AdditionalReportEvent(DTC_905707_HANDYHALTER_MECHANISCHER_FEHLER, TRUE);
					semV(DEH_LockResource);
				}
				else
				{
					semP(DEH_LockResource);
					retVal_u8 = DEH_AdditionalReportEvent(DTC_905111_BASEPLATE_KURZSCHLUSSNACHMASSE, FALSE);
					retVal_u8 = DEH_AdditionalReportEvent(DTC_905112_BASEPLATE_KURZSCHLUSSNACHPLUS, FALSE);
					retVal_u8 = DEH_AdditionalReportEvent(DTC_905113_BASEPLATE_UNTERBRECHNUNG, FALSE);
					retVal_u8 = DEH_AdditionalReportEvent(DTC_905707_HANDYHALTER_MECHANISCHER_FEHLER, FALSE);
					semV(DEH_LockResource);
				}
			}
			if ( sF == FAIL )
			{

				DEBUG_TEXT(APP_DTC_SWI_ERR, MOD_APP_DTC, "Mech failure Baseplate error");
				APP_DTC_stAppDTC_DeviceDtcStatus.u8BaseplateStatus = APP_DIAG_DTC_MECH_FAILURE;
			}
			else
			{
				DEBUG_TEXT(APP_DTC_SWI_DBG, MOD_APP_DTC, "No Baseplate Error");
				APP_DTC_stAppDTC_DeviceDtcStatus.u8BaseplateStatus = APP_DIAG_DTC_OK;
			}
		}
		else if ( (u16BaseplateDiag >= APP_DTC_BASEPLATE_MINOPENCIRCUIT_VW) && ( u16BaseplateDiag < APP_DTC_BASEPLATE_MINHIGHVOLTAGE_VW))
		{
			if ( bBaseplateCoding == BT_TRUE )
			{
				semP(DEH_LockResource);
				retVal_u8 = DEH_AdditionalReportEvent(DTC_905111_BASEPLATE_KURZSCHLUSSNACHMASSE, FALSE);
				retVal_u8 = DEH_AdditionalReportEvent(DTC_905112_BASEPLATE_KURZSCHLUSSNACHPLUS, FALSE);
				retVal_u8 = DEH_AdditionalReportEvent(DTC_905113_BASEPLATE_UNTERBRECHNUNG, TRUE);
				retVal_u8 = DEH_AdditionalReportEvent(DTC_905707_HANDYHALTER_MECHANISCHER_FEHLER, FALSE);
				semV(DEH_LockResource);
				DEBUG_TEXT(APP_DTC_SWI_DBG, MOD_APP_DTC, "open ckt Baseplate error");
			}
			APP_DTC_stAppDTC_DeviceDtcStatus.u8BaseplateStatus = APP_DIAG_DTC_OPEN_CIRCUIT;
		}
		else if ( u16BaseplateDiag > APP_DTC_BASEPLATE_MINHIGHVOLTAGE_VW)
		{
			if ( bBaseplateCoding == BT_TRUE )
			{
				semP(DEH_LockResource);
				retVal_u8 = DEH_AdditionalReportEvent(DTC_905111_BASEPLATE_KURZSCHLUSSNACHMASSE, FALSE);
				retVal_u8 = DEH_AdditionalReportEvent(DTC_905112_BASEPLATE_KURZSCHLUSSNACHPLUS, TRUE);
				retVal_u8 = DEH_AdditionalReportEvent(DTC_905113_BASEPLATE_UNTERBRECHNUNG, FALSE);
				retVal_u8 = DEH_AdditionalReportEvent(DTC_905707_HANDYHALTER_MECHANISCHER_FEHLER, FALSE);
				semV(DEH_LockResource);
				DEBUG_TEXT(APP_DTC_SWI_ERR, MOD_APP_DTC, "SC to Plus Baseplate error");
			}
			APP_DTC_stAppDTC_DeviceDtcStatus.u8BaseplateStatus = APP_DIAG_DTC_SHORT_CIRCUIT_TO_BATTERY;
		}
		if ( bBaseplateCoding == BT_FALSE )
		{
			semP(DEH_LockResource);
			retVal_u8 = DEH_AdditionalReportEvent(DTC_905111_BASEPLATE_KURZSCHLUSSNACHMASSE, FALSE);
			retVal_u8 = DEH_AdditionalReportEvent(DTC_905112_BASEPLATE_KURZSCHLUSSNACHPLUS, FALSE);
			retVal_u8 = DEH_AdditionalReportEvent(DTC_905113_BASEPLATE_UNTERBRECHNUNG, FALSE);
			retVal_u8 = DEH_AdditionalReportEvent(DTC_905707_HANDYHALTER_MECHANISCHER_FEHLER, FALSE);
			semV(DEH_LockResource);
			DEBUG_TEXT(APP_DTC_SWI_DBG, MOD_APP_DTC, "BPlate DTC Off");
		}
	}
	else
	{
		semP(DEH_LockResource);
		retVal_u8 = DEH_AdditionalReportEvent(DTC_905111_BASEPLATE_KURZSCHLUSSNACHMASSE, FALSE);
		retVal_u8 = DEH_AdditionalReportEvent(DTC_905112_BASEPLATE_KURZSCHLUSSNACHPLUS, FALSE);
		retVal_u8 = DEH_AdditionalReportEvent(DTC_905113_BASEPLATE_UNTERBRECHNUNG, FALSE);
		retVal_u8 = DEH_AdditionalReportEvent(DTC_905707_HANDYHALTER_MECHANISCHER_FEHLER, FALSE);
		semV(DEH_LockResource);
		DEBUG_TEXT(APP_DTC_SWI_DBG, MOD_APP_DTC, "BPlate DTC Off");
		APP_DTC_stAppDTC_DeviceDtcStatus.u8BaseplateStatus = APP_DIAG_DTC_OPEN_CIRCUIT;
	}

	DEBUG_VALUE1(APP_DTC_SWI_DBG, MOD_APP_DTC, "status value Baseplate = %d", APP_DTC_stAppDTC_DeviceDtcStatus.u8BaseplateStatus);
	DEBUG_VALUE1(APP_DTC_SWI_DBG, MOD_APP_DTC, "adcBaseplateDiag = %d", u16BaseplateDiag);
    
    /* suppress warning of not referenced variable */ 
    (void)retVal_u8;
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_DTC_vMonitorBasePlate_Skoda                                         		  */
/** 
    \brief      Montitors for short circuit on Baseplate line.
  
    \brief      Description:
                corresponding DTCs are set when a error is detected. 

    \return     void
*/
/*----------------------------------------------------------------------------*/
static void _APP_DTC_vMonitorBasePlate_Skoda(void)
{
	tDEH_Return retVal_u8 = DEH_OK;
	Uint16Type u16BaseplateDiag;
	SuccessFailType sF = SUCCESS;
	BooleanType bBaseplateCoding = LOGI_BUT_MAN_bGetBasePlateConnectivity();
	sF = LOGI_BUT_MAN_sfGetFailureValue(enLogiButManDeviceCradle);
	u16BaseplateDiag  = MONITOR_ADC_u16GetMovingAverageADCValueBaseplate();
	if ( _APP_DTC_u8CurrentVariantType == 0x01 )
	{
		if ( u16BaseplateDiag < APP_DTC_BASEPLATE_MAXUNDERVOLTAGE_VW)
		{
			if ( bBaseplateCoding == BT_TRUE )
			{
				semP(DEH_LockResource);
				retVal_u8 = DEH_AdditionalReportEvent(DTC_905111_BASEPLATE_KURZSCHLUSSNACHMASSE, TRUE);
				retVal_u8 = DEH_AdditionalReportEvent(DTC_905112_BASEPLATE_KURZSCHLUSSNACHPLUS, FALSE);
				retVal_u8 = DEH_AdditionalReportEvent(DTC_905113_BASEPLATE_UNTERBRECHNUNG, FALSE);
				retVal_u8 = DEH_AdditionalReportEvent(DTC_905707_HANDYHALTER_MECHANISCHER_FEHLER, FALSE);
				semV(DEH_LockResource);
				DEBUG_TEXT(APP_DTC_SWI_ERR, MOD_APP_DTC, "SC to Gnd Baseplate error");
			}
			APP_DTC_stAppDTC_DeviceDtcStatus.u8BaseplateStatus = APP_DIAG_DTC_SHORT_CIRCUIT_TO_GROUND;
		}
		else if ( (u16BaseplateDiag >= APP_DTC_BASEPLATE_MAXUNDERVOLTAGE_VW) && ( u16BaseplateDiag < APP_DTC_BASEPLATE_MINOPENCIRCUIT_VW))
		{
			if ( bBaseplateCoding == BT_TRUE )
			{
				if ( sF == FAIL )
				{
					semP(DEH_LockResource);
					retVal_u8 = DEH_AdditionalReportEvent(DTC_905111_BASEPLATE_KURZSCHLUSSNACHMASSE, FALSE);
					retVal_u8 = DEH_AdditionalReportEvent(DTC_905112_BASEPLATE_KURZSCHLUSSNACHPLUS, FALSE);
					retVal_u8 = DEH_AdditionalReportEvent(DTC_905113_BASEPLATE_UNTERBRECHNUNG, FALSE);
					retVal_u8 = DEH_AdditionalReportEvent(DTC_905707_HANDYHALTER_MECHANISCHER_FEHLER, TRUE);
					semV(DEH_LockResource);
				}
				else
				{
					semP(DEH_LockResource);
					retVal_u8 = DEH_AdditionalReportEvent(DTC_905111_BASEPLATE_KURZSCHLUSSNACHMASSE, FALSE);
					retVal_u8 = DEH_AdditionalReportEvent(DTC_905112_BASEPLATE_KURZSCHLUSSNACHPLUS, FALSE);
					retVal_u8 = DEH_AdditionalReportEvent(DTC_905113_BASEPLATE_UNTERBRECHNUNG, FALSE);
					retVal_u8 = DEH_AdditionalReportEvent(DTC_905707_HANDYHALTER_MECHANISCHER_FEHLER, FALSE);
					semV(DEH_LockResource);
				}
			}
			if ( sF == FAIL )
			{

				DEBUG_TEXT(APP_DTC_SWI_ERR, MOD_APP_DTC, "Mech failure Baseplate error");
				APP_DTC_stAppDTC_DeviceDtcStatus.u8BaseplateStatus = APP_DIAG_DTC_MECH_FAILURE;
			}
			else
			{

				DEBUG_TEXT(APP_DTC_SWI_DBG, MOD_APP_DTC, "No Baseplate Error");
				APP_DTC_stAppDTC_DeviceDtcStatus.u8BaseplateStatus = APP_DIAG_DTC_OK;
			}
		}
		else if ( (u16BaseplateDiag >= APP_DTC_BASEPLATE_MINOPENCIRCUIT_VW) && ( u16BaseplateDiag < APP_DTC_BASEPLATE_MINHIGHVOLTAGE_VW))
		{
			if ( bBaseplateCoding == BT_TRUE )
			{
				semP(DEH_LockResource);
				retVal_u8 = DEH_AdditionalReportEvent(DTC_905111_BASEPLATE_KURZSCHLUSSNACHMASSE, FALSE);
				retVal_u8 = DEH_AdditionalReportEvent(DTC_905112_BASEPLATE_KURZSCHLUSSNACHPLUS, FALSE);
				retVal_u8 = DEH_AdditionalReportEvent(DTC_905113_BASEPLATE_UNTERBRECHNUNG, TRUE);
				retVal_u8 = DEH_AdditionalReportEvent(DTC_905707_HANDYHALTER_MECHANISCHER_FEHLER, FALSE);
				semV(DEH_LockResource);
				DEBUG_TEXT(APP_DTC_SWI_DBG, MOD_APP_DTC, "open ckt Baseplate error");
			}
			APP_DTC_stAppDTC_DeviceDtcStatus.u8BaseplateStatus = APP_DIAG_DTC_OPEN_CIRCUIT;
		}
		else if ( u16BaseplateDiag > APP_DTC_BASEPLATE_MINHIGHVOLTAGE_VW)
		{
			if ( bBaseplateCoding == BT_TRUE )
			{
				semP(DEH_LockResource);
				retVal_u8 = DEH_AdditionalReportEvent(DTC_905111_BASEPLATE_KURZSCHLUSSNACHMASSE, FALSE);
				retVal_u8 = DEH_AdditionalReportEvent(DTC_905112_BASEPLATE_KURZSCHLUSSNACHPLUS, TRUE);
				retVal_u8 = DEH_AdditionalReportEvent(DTC_905113_BASEPLATE_UNTERBRECHNUNG, FALSE);
				retVal_u8 = DEH_AdditionalReportEvent(DTC_905707_HANDYHALTER_MECHANISCHER_FEHLER, FALSE);
				semV(DEH_LockResource);
				DEBUG_TEXT(APP_DTC_SWI_ERR, MOD_APP_DTC, "SC to Plus Baseplate error");
			}
			APP_DTC_stAppDTC_DeviceDtcStatus.u8BaseplateStatus = APP_DIAG_DTC_SHORT_CIRCUIT_TO_BATTERY;
		}
		if ( bBaseplateCoding == BT_FALSE )
		{
			semP(DEH_LockResource);
			retVal_u8 = DEH_AdditionalReportEvent(DTC_905111_BASEPLATE_KURZSCHLUSSNACHMASSE, FALSE);
			retVal_u8 = DEH_AdditionalReportEvent(DTC_905112_BASEPLATE_KURZSCHLUSSNACHPLUS, FALSE);
			retVal_u8 = DEH_AdditionalReportEvent(DTC_905113_BASEPLATE_UNTERBRECHNUNG, FALSE);
			retVal_u8 = DEH_AdditionalReportEvent(DTC_905707_HANDYHALTER_MECHANISCHER_FEHLER, FALSE);
			semV(DEH_LockResource);
			DEBUG_TEXT(APP_DTC_SWI_DBG, MOD_APP_DTC, "BPlate DTC Off");
		}
	}
	else
	{
		semP(DEH_LockResource);
		retVal_u8 = DEH_AdditionalReportEvent(DTC_905111_BASEPLATE_KURZSCHLUSSNACHMASSE, FALSE);
		retVal_u8 = DEH_AdditionalReportEvent(DTC_905112_BASEPLATE_KURZSCHLUSSNACHPLUS, FALSE);
		retVal_u8 = DEH_AdditionalReportEvent(DTC_905113_BASEPLATE_UNTERBRECHNUNG, FALSE);
		retVal_u8 = DEH_AdditionalReportEvent(DTC_905707_HANDYHALTER_MECHANISCHER_FEHLER, FALSE);
		semV(DEH_LockResource);
		DEBUG_TEXT(APP_DTC_SWI_DBG, MOD_APP_DTC, "BPlate DTC Off");
		APP_DTC_stAppDTC_DeviceDtcStatus.u8BaseplateStatus = APP_DIAG_DTC_OPEN_CIRCUIT;
	}

	DEBUG_VALUE1(APP_DTC_SWI_DBG, MOD_APP_DTC, "status value Baseplate = %d", APP_DTC_stAppDTC_DeviceDtcStatus.u8BaseplateStatus);
	DEBUG_VALUE1(APP_DTC_SWI_DBG, MOD_APP_DTC, "adcBaseplateDiag = %d", u16BaseplateDiag);

    /* suppress warning of not referenced variable */ 
    (void)retVal_u8;
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_DTC_vMonitorAntenna_VW                                         		  */
/** 
    \brief      Montitors for short circuit on Baseplate line.
  
    \brief      Description:
                corresponding DTCs are set when a error is detected. 

    \return     void
*/
/*----------------------------------------------------------------------------*/
static void _APP_DTC_vMonitorAntenna_VW(void)
{
	tDEH_Return retVal_u8 = DEH_OK;
	BooleanType bAntennaCoding = LOGI_BUT_MAN_bGetAntennaDiagnose();	
	Uint16Type u16AntennaDiag;
	u16AntennaDiag  = MONITOR_ADC_u16GetMovingAverageADCValueAntenna();
	if ( _APP_DTC_u8CurrentVariantType == 0x01 )
	{
		if ( u16AntennaDiag < APP_DTC_ANTENNA_MAXUNDERVOLTAGE_VW)
		{
			if ( bAntennaCoding == BT_TRUE )
			{
				semP(DEH_LockResource);
				retVal_u8 = DEH_AdditionalReportEvent(DTC_905311_GSMANTENNA_KURZSCHLUSSNACHMASSE, TRUE);
				retVal_u8 = DEH_AdditionalReportEvent(DTC_905315_GSMANTENNA_KURZSCHLUSSNACHPLUS_UNTERBRECHNUNG, FALSE);
				semV(DEH_LockResource);
				DEBUG_TEXT(APP_DTC_SWI_ERR, MOD_APP_DTC, "SC to Gnd Antenna error");
			}
			APP_DTC_stAppDTC_DeviceDtcStatus.u8AntennaStatus = APP_DIAG_DTC_SHORT_CIRCUIT_TO_GROUND;
		}
		else if ( (u16AntennaDiag >= APP_DTC_ANTENNA_MAXUNDERVOLTAGE_VW) && ( u16AntennaDiag < APP_DTC_ANTENNA_MINHIGHVOLTAGE_VW))
		{
			if ( bAntennaCoding == BT_TRUE )
			{
				semP(DEH_LockResource);
				retVal_u8 = DEH_AdditionalReportEvent(DTC_905311_GSMANTENNA_KURZSCHLUSSNACHMASSE, FALSE);
				retVal_u8 = DEH_AdditionalReportEvent(DTC_905315_GSMANTENNA_KURZSCHLUSSNACHPLUS_UNTERBRECHNUNG, FALSE);
				semV(DEH_LockResource);
			}
			DEBUG_TEXT(APP_DTC_SWI_DBG, MOD_APP_DTC, "No Antenna Error");
			APP_DTC_stAppDTC_DeviceDtcStatus.u8AntennaStatus = APP_DIAG_DTC_OK;
		}
		else if ( u16AntennaDiag > APP_DTC_ANTENNA_MINHIGHVOLTAGE_VW)
	   {
			if ( bAntennaCoding == BT_TRUE )
			{
				semP(DEH_LockResource);
				retVal_u8 = DEH_AdditionalReportEvent(DTC_905311_GSMANTENNA_KURZSCHLUSSNACHMASSE, FALSE);
				retVal_u8 = DEH_AdditionalReportEvent(DTC_905315_GSMANTENNA_KURZSCHLUSSNACHPLUS_UNTERBRECHNUNG, TRUE);
				semV(DEH_LockResource);
				DEBUG_TEXT(APP_DTC_SWI_DBG, MOD_APP_DTC, "Antenna-SCPlus/Open");
			}
			APP_DTC_stAppDTC_DeviceDtcStatus.u8AntennaStatus = APP_DIAG_DTC_OPEN_CIRCUIT;
		}
		if ( bAntennaCoding == BT_FALSE )
		{
			semP(DEH_LockResource);
			retVal_u8 = DEH_AdditionalReportEvent(DTC_905311_GSMANTENNA_KURZSCHLUSSNACHMASSE, FALSE);
			retVal_u8 = DEH_AdditionalReportEvent(DTC_905315_GSMANTENNA_KURZSCHLUSSNACHPLUS_UNTERBRECHNUNG, FALSE);
			semV(DEH_LockResource);
			DEBUG_TEXT(APP_DTC_SWI_DBG, MOD_APP_DTC, "Antenna Dtc off");
		}
	}
	else
	{
		semP(DEH_LockResource);
		retVal_u8 = DEH_AdditionalReportEvent(DTC_905311_GSMANTENNA_KURZSCHLUSSNACHMASSE, FALSE);
		retVal_u8 = DEH_AdditionalReportEvent(DTC_905315_GSMANTENNA_KURZSCHLUSSNACHPLUS_UNTERBRECHNUNG, FALSE);
		semV(DEH_LockResource);
		DEBUG_TEXT(APP_DTC_SWI_DBG, MOD_APP_DTC, "Antenna DTC off");
		APP_DTC_stAppDTC_DeviceDtcStatus.u8AntennaStatus = APP_DIAG_DTC_OPEN_CIRCUIT;
	}

	DEBUG_VALUE1(APP_DTC_SWI_DBG, MOD_APP_DTC, "status value Antenna = %d", APP_DTC_stAppDTC_DeviceDtcStatus.u8AntennaStatus);
	DEBUG_VALUE1(APP_DTC_SWI_DBG, MOD_APP_DTC, "adcAntennaDiag = %d", u16AntennaDiag);

    /* suppress warning of not referenced variable */ 
    (void)retVal_u8;
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_DTC_vMonitorAntenna_Skoda                                         		  */
/** 
    \brief      Montitors for short circuit on Baseplate line.
  
    \brief      Description:
                corresponding DTCs are set when a error is detected. 

    \return     void
*/
/*----------------------------------------------------------------------------*/
static void _APP_DTC_vMonitorAntenna_Skoda(void)
{
	tDEH_Return retVal_u8 = DEH_OK;
	BooleanType bAntennaCoding = LOGI_BUT_MAN_bGetAntennaDiagnose();
	Uint16Type u16AntennaDiag;
	u16AntennaDiag  = MONITOR_ADC_u16GetMovingAverageADCValueAntenna();
	if ( _APP_DTC_u8CurrentVariantType == 0x01 )
	{
		if ( u16AntennaDiag < APP_DTC_ANTENNA_MAXUNDERVOLTAGE_SKODA)
	    {
	        if ( bAntennaCoding == BT_TRUE )
			{
				semP(DEH_LockResource);
				retVal_u8 = DEH_AdditionalReportEvent(DTC_905311_GSMANTENNA_KURZSCHLUSSNACHMASSE, TRUE);
				retVal_u8 = DEH_AdditionalReportEvent(DTC_905315_GSMANTENNA_KURZSCHLUSSNACHPLUS_UNTERBRECHNUNG, FALSE);
				semV(DEH_LockResource);
				DEBUG_TEXT(APP_DTC_SWI_ERR, MOD_APP_DTC, "SC to Gnd Antenna error");
			}
			APP_DTC_stAppDTC_DeviceDtcStatus.u8AntennaStatus = APP_DIAG_DTC_SHORT_CIRCUIT_TO_GROUND;
	    }
		else if ( (u16AntennaDiag >= APP_DTC_ANTENNA_MAXUNDERVOLTAGE_SKODA) && ( u16AntennaDiag < APP_DTC_ANTENNA_MINHIGHVOLTAGE_SKODA))
	    {
	        if ( bAntennaCoding == BT_TRUE )
			{
				semP(DEH_LockResource);
				retVal_u8 = DEH_AdditionalReportEvent(DTC_905311_GSMANTENNA_KURZSCHLUSSNACHMASSE, FALSE);
				retVal_u8 = DEH_AdditionalReportEvent(DTC_905315_GSMANTENNA_KURZSCHLUSSNACHPLUS_UNTERBRECHNUNG, FALSE);
				semV(DEH_LockResource);
			}
	        DEBUG_TEXT(APP_DTC_SWI_DBG, MOD_APP_DTC, "No Antenna Error");
			APP_DTC_stAppDTC_DeviceDtcStatus.u8AntennaStatus = APP_DIAG_DTC_OK;
	    }
		else if ( u16AntennaDiag > APP_DTC_ANTENNA_MINHIGHVOLTAGE_SKODA)
	    {
	        if ( bAntennaCoding == BT_TRUE )
			{
				semP(DEH_LockResource);
				retVal_u8 = DEH_AdditionalReportEvent(DTC_905311_GSMANTENNA_KURZSCHLUSSNACHMASSE, FALSE);
				retVal_u8 = DEH_AdditionalReportEvent(DTC_905315_GSMANTENNA_KURZSCHLUSSNACHPLUS_UNTERBRECHNUNG, TRUE);
				semV(DEH_LockResource);
				DEBUG_TEXT(APP_DTC_SWI_DBG, MOD_APP_DTC, "Antenna-SCPlus/Open");
			}
			APP_DTC_stAppDTC_DeviceDtcStatus.u8AntennaStatus = APP_DIAG_DTC_OPEN_CIRCUIT;
	    }
	    if ( bAntennaCoding == BT_FALSE )
		{
			semP(DEH_LockResource);
			retVal_u8 = DEH_AdditionalReportEvent(DTC_905311_GSMANTENNA_KURZSCHLUSSNACHMASSE, FALSE);
			retVal_u8 = DEH_AdditionalReportEvent(DTC_905315_GSMANTENNA_KURZSCHLUSSNACHPLUS_UNTERBRECHNUNG, FALSE);
			semV(DEH_LockResource);
			DEBUG_TEXT(APP_DTC_SWI_DBG, MOD_APP_DTC, "Antenna DTC off");
		}
	}
	else
	{
		semP(DEH_LockResource);
		retVal_u8 = DEH_AdditionalReportEvent(DTC_905311_GSMANTENNA_KURZSCHLUSSNACHMASSE, FALSE);
		retVal_u8 = DEH_AdditionalReportEvent(DTC_905315_GSMANTENNA_KURZSCHLUSSNACHPLUS_UNTERBRECHNUNG, FALSE);
		semV(DEH_LockResource);
		DEBUG_TEXT(APP_DTC_SWI_DBG, MOD_APP_DTC, "Antenna DTC off");
		APP_DTC_stAppDTC_DeviceDtcStatus.u8AntennaStatus = APP_DIAG_DTC_OPEN_CIRCUIT;
	}

	DEBUG_VALUE1(APP_DTC_SWI_DBG, MOD_APP_DTC, "status value Antenna = %d", APP_DTC_stAppDTC_DeviceDtcStatus.u8AntennaStatus);
	DEBUG_VALUE1(APP_DTC_SWI_DBG, MOD_APP_DTC, "adcAntennaDiag = %d", u16AntennaDiag);

    /* suppress warning of not referenced variable */ 
    (void)retVal_u8;
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_DTC_vMonitorUBatt                                      */
/**
    \brief      function to handle the DTC storage for Voltage monitoring

    \brief      Description:
                function to manage the DTC storage for the different Voltage states


    \param      void
    \return     void
   */
/*----------------------------------------------------------------------------*/
static void _APP_DTC_vMonitorUBatt(void)
{
    tDEH_Return retVal_u8 = DEH_OK;

    PWMGR_enVoltageStates cshdlVoltageState = Pwrmgr_GetVoltageState();

	if (ON == APP_CAN_u8Klemme15Status())
	{
		DEBUG_TEXT(APP_DTC_SWI_DBG, MOD_APP_DTC, "KL15 is ON");
		switch(cshdlVoltageState)
		{
			case PWMGR_NormalLowVoltage:
				semP(DEH_LockResource);
				retVal_u8 = DEH_AdditionalReportEvent(DTC_D01100_VERSORGUNGSSPANNUNG_ZUNIEDRIG, TRUE);
				retVal_u8 = DEH_AdditionalReportEvent(DTC_D40000_FUNKTIONSEINSCHRAENKUNG_UNTERSPANNUNG, TRUE);
				retVal_u8 = DEH_AdditionalReportEvent(DTC_D01200_VERSORGUNGSSPANNUNG_ZUHOCH, FALSE);
				retVal_u8 = DEH_AdditionalReportEvent(DTC_D40100_FUNKTIONSEINSCHRAENKUNG_UEBERSPANNUNG, FALSE);
				semV(DEH_LockResource);
				DEBUG_TEXT(APP_DTC_SWI_WNG, MOD_APP_DTC, "Normal Low Voltage");
			break;

			case PWMGR_NormalLowRecoverVoltage:
				semP(DEH_LockResource);
				retVal_u8 = DEH_AdditionalReportEvent(DTC_D01100_VERSORGUNGSSPANNUNG_ZUNIEDRIG, FALSE);
				retVal_u8 = DEH_AdditionalReportEvent(DTC_D40000_FUNKTIONSEINSCHRAENKUNG_UNTERSPANNUNG, TRUE);
				retVal_u8 = DEH_AdditionalReportEvent(DTC_D01200_VERSORGUNGSSPANNUNG_ZUHOCH, FALSE);
				retVal_u8 = DEH_AdditionalReportEvent(DTC_D40100_FUNKTIONSEINSCHRAENKUNG_UEBERSPANNUNG, FALSE);
				semV(DEH_LockResource);
				DEBUG_TEXT(APP_DTC_SWI_WNG, MOD_APP_DTC, "Normal Low Recover Voltage");
			break;

			case PWMGR_NormalHighVoltage:
				semP(DEH_LockResource);
				retVal_u8 = DEH_AdditionalReportEvent(DTC_D01100_VERSORGUNGSSPANNUNG_ZUNIEDRIG, FALSE);
				retVal_u8 = DEH_AdditionalReportEvent(DTC_D40000_FUNKTIONSEINSCHRAENKUNG_UNTERSPANNUNG, FALSE);
				retVal_u8 = DEH_AdditionalReportEvent(DTC_D01200_VERSORGUNGSSPANNUNG_ZUHOCH, TRUE);
				retVal_u8 = DEH_AdditionalReportEvent(DTC_D40100_FUNKTIONSEINSCHRAENKUNG_UEBERSPANNUNG, TRUE);
				semV(DEH_LockResource);
				DEBUG_TEXT(APP_DTC_SWI_WNG, MOD_APP_DTC, "Normal High Voltage");
			break;

			case PWMGR_NormalHighRecoverVoltage:
				semP(DEH_LockResource);
				retVal_u8 = DEH_AdditionalReportEvent(DTC_D01100_VERSORGUNGSSPANNUNG_ZUNIEDRIG, FALSE);
				retVal_u8 = DEH_AdditionalReportEvent(DTC_D40000_FUNKTIONSEINSCHRAENKUNG_UNTERSPANNUNG, FALSE);
				retVal_u8 = DEH_AdditionalReportEvent(DTC_D01200_VERSORGUNGSSPANNUNG_ZUHOCH, FALSE);
				retVal_u8 = DEH_AdditionalReportEvent(DTC_D40100_FUNKTIONSEINSCHRAENKUNG_UEBERSPANNUNG, TRUE);
				semV(DEH_LockResource);
				DEBUG_TEXT(APP_DTC_SWI_WNG, MOD_APP_DTC, "Normal Recover High Voltage");
			break;

			case PWMGR_NormalVoltage:
				semP(DEH_LockResource);
				retVal_u8 = DEH_AdditionalReportEvent(DTC_D01100_VERSORGUNGSSPANNUNG_ZUNIEDRIG, FALSE);
				retVal_u8 = DEH_AdditionalReportEvent(DTC_D40000_FUNKTIONSEINSCHRAENKUNG_UNTERSPANNUNG, FALSE);
				retVal_u8 = DEH_AdditionalReportEvent(DTC_D01200_VERSORGUNGSSPANNUNG_ZUHOCH, FALSE);
				retVal_u8 = DEH_AdditionalReportEvent(DTC_D40100_FUNKTIONSEINSCHRAENKUNG_UEBERSPANNUNG, FALSE);
				semV(DEH_LockResource);
				DEBUG_TEXT(APP_DTC_SWI_DBG, MOD_APP_DTC, "Normal Voltage");
			break;

			default:
			break;
		}
	}
    retVal_u8 = retVal_u8;
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_DTC_vMonitorTemperature                                         		  */
/**
    \brief      Montitors for short circuit on Temperature Channel.

    \brief      Description:
                corresponding DTCs are set when a error is detected.

    \return     void

*/
/*----------------------------------------------------------------------------*/
static void _APP_DTC_vMonitorTemperature(void)
{
	tDEH_Return retVal_u8 = DEH_OK;
	Uint8Type u8Temperature;

	u8Temperature  = TEMPERATURE_u8GetTemperature();

	if (u8Temperature > (TEMPERATURE_HIGH + 100) )   // + 100 weil des Wertebereiches vom VAG Tester
    {
	  /* Attention - Measurement are only all 2 second - steer over host task */
	  // after 60s temperature > 70 degree DTC have to register - Config over deh_cfg.c --> DEH_EVENT_11.DTC
	  semP(DEH_LockResource);
      retVal_u8 = DEH_AdditionalReportEvent(DTC_905000_FUNKTIONEINSCHRAENKUNGDURCHUEBERTEMPERATUR, TRUE);
      semV(DEH_LockResource);
      DEBUG_TEXT(APP_DTC_SWI_ERR, MOD_APP_DTC, "Temperature too high");
    }
    else
    {
      semP(DEH_LockResource);
      retVal_u8 = DEH_AdditionalReportEvent(DTC_905000_FUNKTIONEINSCHRAENKUNGDURCHUEBERTEMPERATUR, FALSE);
      semV(DEH_LockResource);
      DEBUG_TEXT(APP_DTC_SWI_DBG, MOD_APP_DTC, "No temperature Error");
    }

	if ( retVal_u8 != DEH_OK )
	{

		DEBUG_TEXT(APP_DTC_SWI_ERR, MOD_APP_DTC, "Temperature: deh error");
	}

	DEBUG_VALUE1(APP_DTC_SWI_DBG, MOD_APP_DTC, "status temperature = %d", APP_DTC_stAppDTC_DeviceDtcStatus.u8TemperatureStatus);
	DEBUG_VALUE1(APP_DTC_SWI_DBG, MOD_APP_DTC, "Temperature = %d", u8Temperature);


}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_DTC_vCheckCodingValues                                         		  */
/** 
    \brief      diagnosis coding connect check
  
    \brief      Description:
                this function check whether coding items are not coded but connected and reversed 

    \return     void
*/
/*----------------------------------------------------------------------------*/
static void _APP_DTC_vCheckCodingValues(void)
{
	tDEH_Return retVal_u8 = DEH_OK;
	SuccessFailType sF = SUCCESS;
  Uint8Type u8EepromCodingParameters[ID_2E_0600_CODIERUNG_MAX_LEN];

	(void)EEPROM_enGetByteSequenceFromRamMirror(&u8EepromCodingParameters[0], ID_2E_0600_CODIERUNG_MAX_LEN, EEPROM_LAYOUT_ENUM_VWCodingValue);
	if ( _APP_DTC_u8CurrentVariantType == 0xFF)
	{
		DEBUG_VALUE1(APP_DTC_SWI_ERR, MOD_APP_DTC, "VAriant still not read from E2P %d", _APP_DTC_u8CurrentVariantType);
	}
	else
	{
		if (( _APP_DTC_u8CurrentVariantType == 0x00 ) || ( _APP_DTC_u8CurrentVariantType == 0x02 )) // HW without cradle support
		{
			if ((u8EepromCodingParameters[5] == 0))
				 sF = FAIL;    // AerialDiagnosis ein codiert, aber hardwaremäßig nicht verbunden.

			if ((u8EepromCodingParameters[6] == 0) || (u8EepromCodingParameters[6] == 2))
				 sF = FAIL;    // BaseplateDiagnosis ein codiert, aber hardwaremäßig nicht verbunden.
		}
		else if( _APP_DTC_u8CurrentVariantType == 0x01 ) // HW with cradle support
		{
			if ((u8EepromCodingParameters[5] == 1) && (APP_DTC_stAppDTC_DeviceDtcStatus.u8AntennaStatus == APP_DIAG_DTC_OK)
					&& (APP_DTC_stAppDTC_DeviceDtcStatus.u8AntennaStatus != APP_DIAG_DTC_INVALID))
			{
				 sF = FAIL;    // AerialDiagnosis aus codiert, aber hardwaremäßig verbunden => also falsch kodiert
			}

			if ((u8EepromCodingParameters[6] == 1) && (APP_DTC_stAppDTC_DeviceDtcStatus.u8BaseplateStatus == APP_DIAG_DTC_OK)
				&& (APP_DTC_stAppDTC_DeviceDtcStatus.u8BaseplateStatus != APP_DIAG_DTC_INVALID))
			{
				sF = FAIL;    // BaseplateDiagnosis aus codiert, aber hardwaremäßig verbunden => also falsch kodiert
			}
		}
		if ((u8EepromCodingParameters[7] == 0x10) && (APP_DTC_stAppDTC_DeviceDtcStatus.u83ButtonModuleStatus == APP_DIAG_DTC_OK)
			&& (APP_DTC_stAppDTC_DeviceDtcStatus.u83ButtonModuleStatus != APP_DIAG_DTC_INVALID))
		{
			 sF = FAIL;    // RooflineDiagnosis aus codiert, aber hardwaremäßig verbunden => also falsch kodiert
		}

		if (sF == FAIL)
		{
			DEBUG_TEXT(APP_DTC_SWI_ERR, MOD_APP_DTC, "SG is coded wrongly");
			semP(DEH_LockResource);
			retVal_u8 = DEH_AdditionalReportEvent(DTC_D01400_SG_FALSCHCODIERT, TRUE);
			semV(DEH_LockResource);
		}
		else
		{
			APP_DTC_stAppDTC_DeviceDtcStatus.u8CodingStatus = APP_DIAG_DTC_OK;
			semP(DEH_LockResource);
			retVal_u8 = DEH_ClearEvent(DTC_D01400_SG_FALSCHCODIERT);
			semV(DEH_LockResource);
		}
	}
}


/*----------------------------------------------------------------------------*/
/* Function    : APP_DTC_vStartupSequence                                         		  */
/**
    \brief      diagnosis coding connect check

    \brief      Description:
                this function check whether coding items are not coded but connected and reversed

    \return     void
*/
/*----------------------------------------------------------------------------*/
void APP_DTC_vStartupSequence(void)
{
	tDEH_Return retVal_u8 = DEH_OK;
  	BooleanType bAudioLeftDtcAllowed;
  	//data download handling
	Uint8Type u8TmpByte = 0xFF;

	if ( (_APP_DTC_bStartUpFinished == BT_TRUE))
	{
		;
	}
	else
	{
		switch(_APP_DTC_enAppDtcStartSequence)
		{
			case enAppDtcReadValuesOnStartUp:
			{
				EEPROM_LAYOUT_vGetPlayerEnabled(&bAudioLeftDtcAllowed);
				_APP_DTC_u8CurrentVariantType = PHY_BUT_DRV_u8GetVariantType();
				DEBUG_VALUE1(APP_DTC_SWI_TRC, MOD_APP_DTC, "Variant:%d", _APP_DTC_u8CurrentVariantType);
				DEBUG_VALUE1(APP_DTC_SWI_TRC, MOD_APP_DTC, "LeftAllowed:%d", bAudioLeftDtcAllowed);

				//TODO: Handling needed only why DTC's are cleared by DataDownload (SHOULD BE REMOVED AFTER!!!)
				// read saved data download status
				(void)EEPROM_enGetSingleByteFromRamMirror(&u8TmpByte, EEPROM_LAYOUT_ENUM_ECUDataProgrammingInformation);
				DEBUG_VALUE1(APP_DTC_SWI_TRC, MOD_APP_DTC, "ECUDataProgrammingInformation:%d", u8TmpByte);
				if( (u8TmpByte & MASK_UNGULTIGE_EEPROM_DATEN) )
				{
					//data download isn't successful, store a DTC
					semP(DEH_LockResource);
					(void)DEH_AdditionalReportEvent(DTC_A00500_UNGUELTIGER_DATENSATZ, TRUE);
					semV(DEH_LockResource);
					DEBUG_TEXT(APP_DTC_SWI_TRC, MOD_APP_DTC, "DataDownload: Set DTC TRUE");
				}
				_APP_DTC_enAppDtcStartSequence = enAppDtcStartMonitoringCodierung;
				break;
			}
			case enAppDtcStartMonitoringCodierung:
			{
				if ( (osGetSystemCounter() > 6000) )
				{
					_APP_DTC_bStartUpFinished = BT_TRUE;
					DEBUG_TEXT(APP_DTC_SWI_TRC, MOD_APP_DTC, "APP DTC Startup Finished");
				}
				break;
			}
		}
	}

    /* suppress warning of not referenced variable */
    (void)retVal_u8;
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_DTC_vCheckEcuCoded                                         		  */
/**
    \brief      diagnosis to check if the ECU is coded.

    \brief      Description:
                This function checks if the ECU has already been coded. The check is done every 2 seconds
                and after successful coding the FSP should be completely deleted.

    \return     void
*/
/*----------------------------------------------------------------------------*/
static void _APP_DTC_vCheckEcuCoded(void)
{
	tDEH_Return retVal_u8 = DEH_OK;
	Uint8Type u8CodedDoneAnyTime;
	BooleanType bSteuerGeraetNichtCodiert = BT_FALSE;

	(void)EEPROM_enGetSingleByteFromRamMirror(&u8CodedDoneAnyTime, EEPROM_LAYOUT_ENUM_VWTesterCodingInformation);
	// Low nibble is length of coding
	// High nibble is for coding write successful done. High nibble value 0x1 is define as successful coding
	if (((u8CodedDoneAnyTime & 0xF0)>> 4)!= 0x01)
	{
		bSteuerGeraetNichtCodiert = BT_TRUE;
	}

	if (bSteuerGeraetNichtCodiert == BT_TRUE)
	{
		semP(DEH_LockResource);
		(void)DEH_AdditionalReportEvent(DTC_D01300_SG_NICHTCODIERT, TRUE);
		semV(DEH_LockResource);
		DEBUG_TEXT(APP_DTC_SWI_DBG, MOD_APP_DTC, "SU,00-in-date");
	}
	else
	{
		semP(DEH_LockResource);
		retVal_u8 = DEH_ClearEvent(DTC_D01300_SG_NICHTCODIERT);
		semV(DEH_LockResource);
		DEBUG_TEXT(APP_DTC_SWI_DBG,MOD_APP_DTC,"SU,CodingDateReady");
	}

}


/*----------------------------------------------------------------------------*/
/* Function    : APP_DTC_u8GetStatusMikroTelefon                                   */
/**
    \brief      returns the current status of the microphone lines.

    \brief      

    \return     Uint8Type
                
   */
/*----------------------------------------------------------------------------*/
Uint8Type APP_DTC_u8GetStatusMikroTelefon(void)
{   
    return  (APP_DTC_stAppDTC_DeviceDtcStatus.u8MikrophoneStatus );
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_DTC_u8GetStatusAudioLeft                                   */
/**
    \brief      returns the current status of the Audio left lines.

    \brief      

    \return     Uint8Type
                
   */
/*----------------------------------------------------------------------------*/
Uint8Type APP_DTC_u8GetStatusAudioLeft(void)
{   
    return  (APP_DTC_stAppDTC_DeviceDtcStatus.u8AudioLeftStatus );
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_DTC_u8GetStatusAudioRight                                   */
/**
    \brief      returns the current status of the Audio Right lines.

    \brief      

    \return     Uint8Type
              value of Audio Right status
                
   */
/*----------------------------------------------------------------------------*/
Uint8Type APP_DTC_u8GetStatusAudioRight(void)
{   
    return  (APP_DTC_stAppDTC_DeviceDtcStatus.u8AudioRightStatus );
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_DTC_u8GetStatus3ButtonModule                                   */
/**
    \brief      returns the current status of the 3 Button Module lines

    \brief      

    \return     Uint8Type

   */
/*----------------------------------------------------------------------------*/
Uint8Type APP_DTC_u8GetStatus3ButtonModule(void)
{   
    return  (APP_DTC_stAppDTC_DeviceDtcStatus.u83ButtonModuleStatus );
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_DTC_u8GetStatusRadioMute                                   */
/**
    \brief      returns the current status of the Radio Mute lines

    \brief      

    \return     Uint8Type
                
   */
/*----------------------------------------------------------------------------*/
Uint8Type APP_DTC_u8GetStatusRadioMute(void)
{   
    return  (APP_DTC_stAppDTC_DeviceDtcStatus.u8RadioMuteStatus );
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_DTC_u8GetStatusBaseplate                                   */
/**
    \brief      returns the current status of the Baseplate line

    \brief      

    \return     Uint8Type
                
   */
/*----------------------------------------------------------------------------*/
Uint8Type APP_DTC_u8GetStatusBaseplate(void)
{   
    return  (APP_DTC_stAppDTC_DeviceDtcStatus.u8BaseplateStatus );
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_DTC_u8GetStatusAntenna                                   */
/**
    \brief      returns the current status of the Antenna line

    \brief      

    \return     Uint8Type

   */
/*----------------------------------------------------------------------------*/
Uint8Type APP_DTC_u8GetStatusAntenna(void)
{   
    return  (APP_DTC_stAppDTC_DeviceDtcStatus.u8AntennaStatus);
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_DTC_u8GetStatusTemperature                                   */
/**
    \brief      returns the current status of the Temperature lines.

    \brief

    \return     Uint8Type
              value of Temperature status

   */
/*----------------------------------------------------------------------------*/
Uint8Type APP_DTC_u8GetStatusTemperature(void)
{
    return  (APP_DTC_stAppDTC_DeviceDtcStatus.u8TemperatureStatus );
}
/*----------------------------------------------------------------------------*/
/* Function    : APP_DTC_u8GetStatusCoding                                   */
/**
    \brief      returns the current status of the Coding Values

    \brief      

    \return     Uint8Type
                
   */
/*----------------------------------------------------------------------------*/
Uint8Type APP_DTC_u8GetStatusCoding(void)
{   
    return  (APP_DTC_stAppDTC_DeviceDtcStatus.u8CodingStatus);
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_DTC_u8GetStatusUBatt                                   */
/**
    \brief      returns the current status of the UBatt

    \brief

    \return     Uint8Type

   */
/*----------------------------------------------------------------------------*/
Uint8Type APP_DTC_u8GetStatusUBatt(void)
{
    return  (APP_DTC_stAppDTC_DeviceDtcStatus.u8UBattStatus);
}


/*----------------------------------------------------------------------------*/
/* Function    : APP_DTC_u8GetVariantType                                   */
/**
    \brief      returns the UHV Variant

    \brief      

    \return     Uint8Type
              
*/
/*----------------------------------------------------------------------------*/
Uint8Type APP_DTC_u8GetVariantType(void)
{   
    return  (_APP_DTC_u8CurrentVariantType);
}


/*----------------------------------------------------------------------------*/
/* Function    : APP_DTC_bAudioAllowed                                   */
/**
    \brief      returns the current status of audio adc monitoring is allowed or not

    \brief      Audio Diagnose will be done in the following situation:
				1. When the elapsed time from the time of ECU Start is lesser than 4 seconds 
				and the no Audio Mute has been requested from any application ( Telefon, Voice, MDI )
				within this time
				2. When a Diagnose session is active.

    \return     BooleanType
               status of audio adc monitoring
                
   */
/*----------------------------------------------------------------------------*/
BooleanType APP_DTC_bGetAudioAllowed(void)
{   
	if (
		(
			(APP_CAN_enAudioUnmute == APP_CAN_enGetMuteStatus()) 
			&& !(_APP_DTC_bStartUpFinished)
		)|| 
		(
			(APP_CAN_enAudioUnmute == APP_CAN_enGetMuteStatus())
			&& (UDScub_DEFAULT_SESSION != (Uint8Type)UDS_ubGetSession())
		)
	   )
	{
		if ( _APP_DTC_bAudioDtcAllowed == BT_FALSE ) 
		{
			_APP_DTC_u8StartAudioDiagnoseStartDelay++;
			if ( _APP_DTC_u8StartAudioDiagnoseStartDelay == 0x01 )
			{
				DEBUG_TEXT(APP_DTC_SWI_TRC, MOD_APP_DTC, "Hardware Audio Diag triggered");
				AUDIO_DIAG_START
			}
		}
	}
#ifndef AUDIO_DIAG_ALWAYS_ON
	else
    {
		if ( _APP_DTC_u8StartAudioDiagnoseStartDelay > 0   )
		{
			_APP_DTC_u8StartAudioDiagnoseStartDelay = 0;
			DEBUG_TEXT(APP_DTC_SWI_TRC, MOD_APP_DTC, "Audio Diag not active");
			AUDIO_DIAG_END
			_APP_DTC_bAudioDtcAllowed = BT_FALSE;
		}
    }
#endif
    return  (_APP_DTC_bAudioDtcAllowed );

}



/* End Of File APP_DTC_C1.c */
