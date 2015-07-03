/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/
/**
 * \file 	APP_DIAG_C1.c
 * \brief	this module is the interface between standard diagnostic module SDS
 *          and the applications modules. Detailed description found below.
 * \brief	Detailed description of the file.
*/
/*-----------------------------------------------------------------------------
 * $Date: 2011-08-19 12:04:24 +0200 (Fr, 19 Aug 2011) $
 * $Rev: 21279 $
 * $Author: maennel $
 * $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_DIAG/trunk/src/APP_DIAG_C1.c $
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
#include "APP_DIAG.h"
#include "APP_DIAG_PL.h"
#include "ERM.h"
#include "APP_COMMON_Utils.h"
#include "OSEK_AppTask_H1.h"
#include "deh.h"
#include "sds_ext.h"
#include "DEH_DTC_CFG.h"
#include "APP_DTC.h"
#include "LOGI_BUT_MAN.h"
#include "EEPROM_LAYOUT_CFG.h"
#include "MONITOR_ADC.h"
#include "APP_PHONE.h"
#include "APP_CAN.h"
#include "ERM_Utils.h"
#include "APP_MDI.h"
#include "APP_SD.h"
#include "VCR.h"
#include "APP_PHONEBOOK_BAP.h"
#include "sds_cfg.h"
#include "APP_DEVICE.h"
#include "CONTEXT_MAN.h"
#include "APP_DEVICE_AudioManager.h"
#include "APP_DIAG_C1.id"
#include "TRACE.h"
#include "TEMPERATURE.h"
#include "APP_PHONE_AT_PNCC.h" /* APP_PHONE_AT_PNCC_pGetPlccContext */
#include "stdio.h"    //  "snprintf"

/*----------------------------------------------------------------------------*/
/* pragma Directives                                                          */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Definitions (avoid as much as possible!)                  */
/*----------------------------------------------------------------------------*/
/**
 \brief	example
 */
extern Uint16Type Pwrmgr_GetVoltageValue(void);
extern ERMContextType        _ERM_gContext;
/*----------------------------------------------------------------------------*/
/* Private Manifest Constant Definitions                                      */
/*----------------------------------------------------------------------------*/
static const Uint8Type _APP_DIAG_u8MmbSeperator[APP_DIAG_BT_NAME_SEPARATOR_LEN] = 
{
    APP_DIAG_BT_NAME_SEPARATOR, 
    APP_DIAG_BT_NAME_SEPARATOR
};
/*----------------------------------------------------------------------------*/
/* Private Types                                                              */
/*----------------------------------------------------------------------------*/
/**
 \brief	description of the type
 */
typedef enum
{
	enAppDiagStartStellglied,
	enAppDiagConfigureMuteBit,
	enAppDiagTriggerAudioTones,
	enAppDiagWaitForCallback,
	enAppDiagRequestToAppDeviceIsBusy
}APP_DIAG_ENUM_STELLGLIED_STATUS;

typedef enum
{
	enAppDiagStartBlock,
	enAppDiagParrotSWVersion,
	enAppDiagParrotDerivative,
	enAppDiagParrotBluetoothChipVersion,
	enAppDiagParrot_SoftwareFeaturesField,
	enAppDiagParrotVersionCompleted
} APP_DIAG_ENUM_PARROT_DETAILS;

/*----------------------------------------------------------------------------*/
/* Prototypes of file local Methods                                           */
/*----------------------------------------------------------------------------*/
static void _APP_DIAG_u8ConvertToInteger(Uint8Type *ptru8BluetoothAddress);

static void _APP_DIAG_DisconnectMediaplayerInCaseOf_IOControl(void);

void _APP_DIAG_vMediaPlayerIsDisconnected_Cb(APP_COMMON_enResponseCode result);

static APP_DIAG_ENUM_STELLGLIED_STATUS _APP_DIAG_enStellgliedStatus = enAppDiagStartStellglied;

static APP_DIAG_ENUM_PARROT_DETAILS _APP_DIAG_enParrotDetails = enAppDiagStartBlock;

static Uint8Type _APP_DIAG_u8ParrotVersionLength = 0;

static APP_CAN_enSourceIdType enSource_InCaseOfIOControl = APP_CAN_enTelephoneCall;
/*----------------------------------------------------------------------------*/
/* File local Code Macros                                                     */
/*----------------------------------------------------------------------------*/
/**
 \brief	description of the macros
 */
#define DEFAULT_LANG_SUPP                             0xFFFF
#define DEFAULT_SMS_WRITING                           BT_TRUE      // on
#define LEN_FOR_FORMAT_SOFTWAREFEATURESFIELD          7

// The below are the default set for VW. The values are in corr to the application functions.
#define DEFAULT_FALLBACK_LANGUAGE                     0xA           // English Fallback
#define DEFAULT_VOICE_CONTROL                         0       // on
#define DEFAULT_AUDIOPARAMETER_SET                    0           // Set 1
#define DEFAULT_MUTE_TYPE                  			  enMuteManagerMuteCooperativeMute // = 0, Mute über CAN ist möglich, deactivate Mute by Wire
#define DEFAULT_MULTIMEDIA_FUNCTIONALITY      		  BT_TRUE      // switch on
#define DEFAULT_AERIAL_DIAGNOSIS                      BT_FALSE     // switch off
#define DEFAULT_BASEPLATE_DIAGNOSIS                   BT_FALSE     // switch off
#define DEFAULT_ROOFLINE_MODULE_TYPE                  0x0           // $00: NAR

/*----------------------------------------------------------------------------*/
/* File local Variables                                                       */
/*----------------------------------------------------------------------------*/
/**
 \brief	description of the private variable
 */
static Uint8Type    _APP_DIAG_u8CounterCodierung = 0x0;
#ifdef UHV_ACTIVATE_FEATURE_SMS
static BooleanType  _APP_DIAG_bSMS_Writing = DEFAULT_SMS_WRITING;
#endif


static BooleanType _APP_DIAG_bWaitForCallBack = BT_FALSE;
static APP_DIAG_ST_DIAG_RESPONSE _APP_DIAG_stResponsePacketToVas;
static APP_DIAG_ST_DIAG_REQUEST  _APP_DIAG_stRequestPacketFromVas;
static Uint8Type _APP_DIAG_u8Service2F_ChannelLeft                       = 0;   // 1 = channel left is active, 0 = channel right is active

static Uint16Type APP_DIAG_ubCounterBusyResponseFromAppTask = 0;
static Uint16Type APP_DIAG_ubCounterWaitForCallBack = 0;
/*----------------------------------------------------------------------------*/
/* File local const Definitions                                               */
/*----------------------------------------------------------------------------*/

// defaults for Record Data Identifier Variant Coding. Soruce is VW Codierung PDF chart.

// defaults for Record Data Identifier Calibration Data
#define DEFAULT_BLUETOOTH_ACKNOWLEDGEMENT_SIGNAL       1           // off
#define DEFAULT_DEVELOPER_TESTMODE                    BT_FALSE     // deactivated

#define MAX_BUSY_RESPONSE_FROM_AP_TASK              4000

/*----------------------------------------------------------------------------*/
/* Function    : APP_DIAG_sfInit                              */
/**
 \brief       To initialise the variables

 */
/*----------------------------------------------------------------------------*/
SuccessFailType APP_DIAG_sfInit()
{
	return SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_DIAG_enConvertAPP_PHONE_ResponseCodes                                                    */
/**
 \brief      to convert the aplication response codes into Diagnose response codes

 \brief

 \param      res
 			APP_COMMON_enResponseCode

 \return     APP_DIAG_ENUM_RESPONSE_CODES
 */
/*----------------------------------------------------------------------------*/
static APP_DIAG_ENUM_RESPONSE_CODES _APP_DIAG_enConvertAPP_PHONE_ResponseCodes(APP_COMMON_enResponseCode res)
{
	APP_DIAG_ENUM_RESPONSE_CODES enResponseImmediate;
	if (res == APP_COMMON_RSP_OK) 
    {
		enResponseImmediate = enAppDiagResponseOkImm;
	} 
    else if (res == APP_COMMON_RSP_ERROR) 
    {
		enResponseImmediate = enAppDiagResponseErrorImm;
	} 
    else if (res == APP_COMMON_RSP_ERROR_BUSY) 
    {
		enResponseImmediate = enAppDiagResponseErrorBusyImm;
	}
	else
	{
		enResponseImmediate = enAppDiagResponseInvalid;
	}
	return enResponseImmediate;
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_DIAG_enConvertAPP_PHONE_ResponseCodes                                                    */
/**
 \brief      to convert the aplication response codes into Diagnose response codes

 \brief

 \param     sF
 			SuccessFailType

 \return     APP_DIAG_ENUM_RESPONSE_CODES
 */
/*----------------------------------------------------------------------------*/
static APP_DIAG_ENUM_RESPONSE_CODES _APP_DIAG_enConvertSuccessFail_ResponseCodes(SuccessFailType sF)
{
	APP_DIAG_ENUM_RESPONSE_CODES enResponseImmediate;
	if (sF == SUCCESS)
    {
		enResponseImmediate = enAppDiagResponseOkImm;
	} 
    else 
    {
		enResponseImmediate = enAppDiagResponseErrorImm;
	}
	return enResponseImmediate;
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_DIAG_cbResponseCode                                                    */
/**
 \brief      to convert the application response codes into Diagnose response codes

 \brief

 \param      rsp
 			APP_COMMON_enResponseCode

 */
/*----------------------------------------------------------------------------*/
static void _APP_DIAG_cbResponseCode(APP_COMMON_enResponseCode rsp)
{
	// Ruckgabe bewerten:
	APP_DIAG_ENUM_RESPONSE_CODES enDiagResponse;
	(void)APP_COMMON_UTILS_stCancelOsekRelAlarm(CTA_APP_DIAG_WAIT_FOR_APP_CB);
	_APP_DIAG_bWaitForCallBack = BT_FALSE;
	_APP_DIAG_enStellgliedStatus = enAppDiagStartStellglied;
	enDiagResponse = _APP_DIAG_enConvertAPP_PHONE_ResponseCodes(rsp);
	_APP_DIAG_stResponsePacketToVas.Identifier = _APP_DIAG_stRequestPacketFromVas.Identifier;
	_APP_DIAG_stResponsePacketToVas.result = enDiagResponse;

	DEBUG_VALUE2(APP_DIAG_SWI_TRC, MOD_APP_DIAG, "Callback Response from Application rsp=%d,id = %04X", rsp,
			_APP_DIAG_stResponsePacketToVas.Identifier);
	APP_DIAG_ubCounterWaitForCallBack = 0;
    // The below code is to take care of error over callback and active stellglied tests.	
if ( (enDiagResponse == enAppDiagResponseErrorImm) &&  ( SDS_UDS_u8GetStatusActuatorTest() != ACTUATOR_TEST_NOT_RUNNING))
	{
		_APP_DIAG_enStellgliedStatus = enAppDiagStartStellglied;
		DEBUG_VALUE1(APP_DIAG_SWI_ERR, MOD_APP_DIAG, "change status = %d", _APP_DIAG_enStellgliedStatus);
		(void)SetRelAlarm(CTA_APP_DIAG_RETRY_BUSY,MSEC(100),MSEC(0));
	}
	(void)SendMessage(msgAppDiagDiagnoseResponse, &_APP_DIAG_stResponsePacketToVas);

}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_DIAG_cbResponseCode                                                    */
/**
 \brief      to convert the application response codes into Diagnose response codes

 \brief

 \param      rsp
 			APP_COMMON_enResponseCode

 */
/*----------------------------------------------------------------------------*/
static void _APP_DIAG_cbResponseMuteManager(APP_COMMON_enResponseCode rsp)
{
	// Ruckgabe bewerten:
	APP_DIAG_ENUM_RESPONSE_CODES enDiagResponse;
	(void)APP_COMMON_UTILS_stCancelOsekRelAlarm(CTA_APP_DIAG_WAIT_FOR_APP_CB);
	_APP_DIAG_bWaitForCallBack = BT_FALSE;
	enDiagResponse = _APP_DIAG_enConvertAPP_PHONE_ResponseCodes(rsp);
	_APP_DIAG_stResponsePacketToVas.Identifier = _APP_DIAG_stRequestPacketFromVas.Identifier;
	_APP_DIAG_stResponsePacketToVas.result = enDiagResponse;

	DEBUG_VALUE2(APP_DIAG_SWI_TRC, MOD_APP_DIAG, "Callback Response Audio Manager=%d,id = %04X", rsp,
			_APP_DIAG_stResponsePacketToVas.Identifier);
	APP_DIAG_ubCounterWaitForCallBack = 0;
	if ( enDiagResponse == enAppDiagResponseOkImm )
	{
		_APP_DIAG_enStellgliedStatus = enAppDiagTriggerAudioTones;
		DEBUG_VALUE1(APP_DIAG_SWI_TRC, MOD_APP_DIAG, "change status = %d", _APP_DIAG_enStellgliedStatus);
		(void)SetRelAlarm(CTA_APP_DIAG_RETRY_BUSY,MSEC(10),MSEC(0));
	}
	else if ( enDiagResponse == enAppDiagResponseErrorImm )
	{
		_APP_DIAG_enStellgliedStatus = enAppDiagStartStellglied;
		DEBUG_VALUE1(APP_DIAG_SWI_TRC, MOD_APP_DIAG, "change status = %d", _APP_DIAG_enStellgliedStatus);
		(void)SendMessage(msgAppDiagDiagnoseResponse, &_APP_DIAG_stResponsePacketToVas);
	}
	else
	{

	}
}

/*-----------------------------------------------------------------------------*/
/* Function    : APP_DIAG_vHandleEvents                          		       */
/**
 \brief      	 Handle Events from Diagnose.
				 For All IDs following method is used.
				 FIRST : Check for validity of the data
				 SECOND: Call Application Function.
				 THIRD: Convert the response in Diagnose specific.
				 FOURTH: Send the response back to Diagnose.

				 This consists of
				 1. CODIERUNG / CODING
				 2. ANPASSUNG / CALIBRATION
				 3. MESSWERTE / MEASUREMENT BLOCK
				 4. STELLGLIED TEST / IO CONTROL ACTUATOR TEST


 \brief			 CODIERUNG:
				 FALLBACK_LANGUAGE
				 $00: no fallbacklanguage
				 $01: German as fallbacklanguage
				 $02: English as fallbacklanguage
				 $03: French as fallbacklanguage
				 $04: Italian as fallbacklanguage
				 $05: Spanish as fallbacklanguage
				 $06: Portuguese as fallbacklanguage
				 $07: Czech as fallbacklanguage
         $08: Dutch as fallbacklanguage
         $0A: US-English as fallbacklanguage
				 $0B: Russian as fallbacklanguage
         $0C: French-Canadian as fallbacklanguage
         $0D: US-Spanish as fallbacklanguage
				 VOICE_CONTROL
				 $00: on
				 $01: off
				 $02: limited operation
				 AUDIOPARAMETER_SET
				 $00: Set 1
				 $01: Set 2
				 $02: Set 3
				 $03: Set 4
				 COOPERATIVEMUTE_TYPE
				 $00: on - CAN is not available
				 $01: off   - CAN is available
				 MULTIMEDIA_FUNCTIONALITY
				 $00: on
				 $01: off
				 AERIAL_DIAGNOSIS
				 $00: on
				 $01: off
				 BASEPLATE_DIAGNOSIS
				 $00: on
				 $01: off
				 ROOFLINE_MODULE_TYPE
				 $00: NAR
				 $01: RdW
				 $10: not available

				 ANPASSUNGEN:
				 1_RESET_TO_FACTORY_SETTING
				 2_SET_NUMBER_ROAMING_FOR_INFO_CALL
				 3_SET_NUMBER_ROAMING_FOR_SERVICE_CALL
				 4_SET_NUMBER_FOR_INFO_CALL
				 5_SET_NUMBER_FOR_SERVICE_CALL
				 6_SMS_WRITING
				 7_BLUETOOTH_ACKNOWLEDGEMENT_SIGNAL
				 8_DEVELOPER_TEST_MODE
				 9_MASKING_OF_LANGUAGES
				 10_UNIVERSAL_PREPARATION_FOR_MOBILETELEPHONE
				 11_TELEPHONE_BASIC_VOLUME
				 12_VOICE_OUTPUT_BASIC_VOLUME
				 13_MICROPHONE_SENSITIVITY
				 14_BLUETOOTH_SET_PIN_CODE
				 15_USER_PROFILES
				 16_MUTE_DELAY
				 17_REFERENCE_CHANNEL_DELAY
				 18_MEDIA_DEVICE_BASIC_VOLUME
				 19_EMERGENCY_NUMBER

 \param		event
 			EventMaskType

 \return     None
 If the function was successful or failed
 SUCCESS, FAIL
 */
/*----------------------------------------------------------------------------*/
void APP_DIAG_vHandleEvents(EventMaskType event)
{
	APP_COMMON_enResponseCode res = APP_COMMON_RSP_OK;
	APP_DIAG_ENUM_RESPONSE_CODES enResponseImmediate = enAppDiagResponseOkImm;
	SuccessFailType sF = SUCCESS;
	Uint16Type pwrvalue = 0;
	static ENUM_APP_DIAG_STATE_CODIERUNG _APP_DIAG_enCodierungStates = enStartCodierung;
	ATReqAlternatingSignalType SignalControl;
	Uint8Type i;
	Uint8Type u8Offset = 0;
	Uint8Type u8BtDeviceCounter = 0;
	AT_BT_Address au8BluetoothAddress;
#if 0
	AT_BT_Address au8BluetoothPtestMacAddress;
#endif
	BooleanType bValue = BT_TRUE;
	BooleanType PlayerEnabled = BT_TRUE;


	if (event & evAppTaskDiagnoseRequest)
	{
		(void)ClearEvent(evAppTaskDiagnoseRequest);
		// collect new message from Diagnose into static variable
		(void)ReceiveMessage(msgAppDiagDiagnoseRequest,&_APP_DIAG_stRequestPacketFromVas);
		DEBUG_VALUE1(APP_DIAG_SWI_TRC, MOD_APP_DIAG, "Received Diagnose id=%04X",
		_APP_DIAG_stRequestPacketFromVas.Identifier);
		_APP_DIAG_stResponsePacketToVas.length = _APP_DIAG_stRequestPacketFromVas.length;
		_APP_DIAG_bWaitForCallBack = BT_FALSE;

		if ( SDS_UDS_u8GetStatusActuatorTest() == ACTUATOR_TEST_ABORTED )
		{
			_APP_DIAG_stRequestPacketFromVas.data[0] = SERVICE_2F_FKT_START_AUS;
		}

		switch (_APP_DIAG_stRequestPacketFromVas.Identifier)
		{
			/*----------------------------------------------------------------------------*/
		    /* CODIERUNG                                                     			  */
		    /*----------------------------------------------------------------------------*/
			{
				case ID_2E_0600_CODIERUNG:
				{
					_APP_DIAG_bWaitForCallBack = BT_FALSE;

					switch (_APP_DIAG_enCodierungStates)
					{
						case enStartCodierung:
							_APP_DIAG_u8CounterCodierung = 0;
							// Before Coding is possible Parrot have to run and is ready for diagnostic requests
							if (ERM_bIsStartupDone() == BT_TRUE )
							{
								EEPROM_sfRamWriteOnly(_APP_DIAG_stRequestPacketFromVas.data, EEPROM_LAYOUT_ENUM_VWCodingValue);
								_APP_DIAG_enCodierungStates = enFallbackLanguageSet;
								enResponseImmediate = enAppDiagResponseFurtherRequestsPending;
								DEBUG_VALUE1(APP_DIAG_SWI_TRC, MOD_APP_DIAG,"Coding Left: %d",(ID_2E_0600_CODIERUNG_MAX_LEN - _APP_DIAG_u8CounterCodierung));
								DEBUG_VALUE1(APP_DIAG_SWI_TRC, MOD_APP_DIAG,"Voice control: %d", (_APP_DIAG_stRequestPacketFromVas.data[1] & 0x0F));
								DEBUG_VALUE1(APP_DIAG_SWI_TRC, MOD_APP_DIAG,"Phonebook sort order: %d", ((_APP_DIAG_stRequestPacketFromVas.data[1] & 0xF0)>> 4));
							}
							else
							{
								DEBUG_TEXT(APP_DIAG_SWI_TRC, MOD_APP_DIAG,"Wait for Parrot awake");
								_APP_DIAG_enCodierungStates = enStartCodierung;
								enResponseImmediate = enAppDiagResponseErrorBusyImm;
							}
							break;
						case enFallbackLanguageSet:
						/*-----------1. FALLBACK LANGUAGE-------------------------------------------*/
							_APP_DIAG_enCodierungStates = enVoiceControlSet;
							_APP_DIAG_u8CounterCodierung++;
							DEBUG_VALUE1(APP_DIAG_SWI_TRC, MOD_APP_DIAG,"Coding Left: %d",(ID_2E_0600_CODIERUNG_MAX_LEN - _APP_DIAG_u8CounterCodierung));
							enResponseImmediate = enAppDiagResponseFurtherRequestsPending;
							break;
						case enVoiceControlSet:
						/*-----------2. VOICE CONTROL------------------------------------------------*/
							// Low nibble is Voice control, High nibble is phonebook sort order --> only LOw nibble is need here
							sF = VCR_sFSetVoiceControlSupport(_APP_DIAG_stRequestPacketFromVas.data[1] & 0x0F);
							_APP_DIAG_enCodierungStates = enAudioParameterSet;
							_APP_DIAG_u8CounterCodierung++;
							DEBUG_VALUE1(APP_DIAG_SWI_TRC, MOD_APP_DIAG,"Coding Left: %d",(ID_2E_0600_CODIERUNG_MAX_LEN - _APP_DIAG_u8CounterCodierung));
							enResponseImmediate = enAppDiagResponseFurtherRequestsPending;
							break;
						case enAudioParameterSet:
						/*-----------3. AUDIO PARAMETER SET------------------------------------------*/
							ERM_vReInitAudioParameterSet(_APP_DIAG_stRequestPacketFromVas.data[2]);
							res = ERM_sFSetAudioParameterSet(_APP_DIAG_stRequestPacketFromVas.data[2], NULL);
							enResponseImmediate = _APP_DIAG_enConvertAPP_PHONE_ResponseCodes(res);
							if (enResponseImmediate == enAppDiagResponseOkImm)
							{
								_APP_DIAG_u8CounterCodierung++;
								DEBUG_VALUE1(APP_DIAG_SWI_TRC, MOD_APP_DIAG,"Coding Left: %d",(ID_2E_0600_CODIERUNG_MAX_LEN - _APP_DIAG_u8CounterCodierung));
								_APP_DIAG_enCodierungStates = enMuteTypeSet;
								enResponseImmediate = enAppDiagResponseFurtherRequestsPending;
							}
							break;

						case enMuteTypeSet:
						/*-----------4. MUTE TYPE----------------------------------------------------*/
							_APP_DIAG_enCodierungStates = enMultiMediaSet;
							_APP_DIAG_u8CounterCodierung++;
							APP_DEVICE_AM_vSetCooperativeMuteDiag(_APP_DIAG_stRequestPacketFromVas.data[3]);
							DEBUG_VALUE1(APP_DIAG_SWI_TRC, MOD_APP_DIAG,"Coding Left: %d",(ID_2E_0600_CODIERUNG_MAX_LEN - _APP_DIAG_u8CounterCodierung));
							enResponseImmediate = enAppDiagResponseFurtherRequestsPending;
							break;
						case enMultiMediaSet:
						/*-----------5. MULTIMEDIA FUNCTIONALITY--------------------------------------*/
							if ( _APP_DIAG_stRequestPacketFromVas.data[4] == 1 )
							{
								const ERMBTDeviceTableType *pDevice = NULL;
								AT_DeviceId deviceId = ERM_u8GetActivePlayer();
								pDevice = ERM_GetBTDevice(deviceId);
								/* check if input is valid */
								if( pDevice != NULL )
								{
									res = APP_DEVICE_rcDeactivateMdiDevice(NULL);
								}
							}
							enResponseImmediate = _APP_DIAG_enConvertAPP_PHONE_ResponseCodes(res);
							if (enResponseImmediate == enAppDiagResponseOkImm)
							{
								BooleanType bMediaIsvisible  = (_APP_DIAG_stRequestPacketFromVas.data[4] == 1) ?
														BT_FALSE : BT_TRUE;
								(void)APP_MDI_sfSetPlayerVisible(bMediaIsvisible);
								_APP_DIAG_u8CounterCodierung++;
								DEBUG_VALUE1(APP_DIAG_SWI_TRC, MOD_APP_DIAG,"Coding Left: %d",(ID_2E_0600_CODIERUNG_MAX_LEN - _APP_DIAG_u8CounterCodierung));
								_APP_DIAG_enCodierungStates = enAerialDiagnosisSet;
								enResponseImmediate = enAppDiagResponseFurtherRequestsPending;
							}
							break;
						case enAerialDiagnosisSet:
						/*-----------6. AERIAL DIAGNOSIS-------------------------------------------*/
							_APP_DIAG_enCodierungStates = enBaseplateDiagnosisSet;
							_APP_DIAG_u8CounterCodierung++;
							DEBUG_VALUE1(APP_DIAG_SWI_TRC, MOD_APP_DIAG,"Coding Left: %d",(ID_2E_0600_CODIERUNG_MAX_LEN - _APP_DIAG_u8CounterCodierung));
							enResponseImmediate = enAppDiagResponseFurtherRequestsPending;
							break;
						case enBaseplateDiagnosisSet:
						/*-----------7. BASEPLATE DIAGNOSIS-----------------------------------*/
							_APP_DIAG_enCodierungStates = enRooflineModuleTypeSet;
							_APP_DIAG_u8CounterCodierung++;
							DEBUG_VALUE1(APP_DIAG_SWI_TRC, MOD_APP_DIAG,"Coding Left: %d",(ID_2E_0600_CODIERUNG_MAX_LEN - _APP_DIAG_u8CounterCodierung));
							enResponseImmediate = enAppDiagResponseFurtherRequestsPending;
							break;
						case enRooflineModuleTypeSet:
						/*-----------8. ROOFLINE MODULE TYPE-------------------------------------------*/
							_APP_DIAG_enCodierungStates = enEndCodierung;
							_APP_DIAG_u8CounterCodierung++;
							DEBUG_VALUE1(APP_DIAG_SWI_TRC, MOD_APP_DIAG,"Coding Left: %d",(ID_2E_0600_CODIERUNG_MAX_LEN - _APP_DIAG_u8CounterCodierung));
							enResponseImmediate = enAppDiagResponseFurtherRequestsPending;
							break;
						case enEndCodierung:
						/*-----------ALL Functions successfull-----------------------------------*/
							enResponseImmediate = enAppDiagResponseOkImm;
							//enResponseImmediate = enAppDiagResponseWaitForCallBack; Use this if callback is needed.
							_APP_DIAG_enCodierungStates = enStartCodierung;
							break;
					}
					break;
				}
			}
			/*----------------------------------------------------------------------------*/
			/* ANPASSUNG		                                                          */
			/*----------------------------------------------------------------------------*/
		    {
		    	case ID_31_0317_RESET_TO_FACTORY_SETTING:
				{
					if (_APP_DIAG_stRequestPacketFromVas.data[0] == 0)
					{
						_APP_DIAG_bWaitForCallBack = BT_TRUE;
						res = APP_DEVICE_rcResetToFactorySetting(_APP_DIAG_cbResponseCode);
						enResponseImmediate = _APP_DIAG_enConvertAPP_PHONE_ResponseCodes(res);
					}
					else if (_APP_DIAG_stRequestPacketFromVas.data[0] == 1)
					{
						_APP_DIAG_bWaitForCallBack = BT_FALSE;
						enResponseImmediate = enAppDiagResponseOkImm;
					}
					else
					{
						_APP_DIAG_bWaitForCallBack = BT_FALSE;
						enResponseImmediate = enAppDiagResponseErrorImm;
					}
					break;
				}
				case ID_2E_0909_SET_NUMBER_ROAMING_FOR_INFO_CALL:
				{
					_APP_DIAG_bWaitForCallBack = BT_FALSE;
					_APP_DIAG_stRequestPacketFromVas.data[_APP_DIAG_stRequestPacketFromVas.length] = 0xFF;
					(void)EEPROM_sfRamWriteOnly(_APP_DIAG_stRequestPacketFromVas.data, EEPROM_LAYOUT_ENUM_SetNumberRoamingForInfoCall);
					CONTEXT_MAN_sfSetInfoCallNumberRoaming(_APP_DIAG_stRequestPacketFromVas.data, _APP_DIAG_stRequestPacketFromVas.length);
					enResponseImmediate = _APP_DIAG_enConvertSuccessFail_ResponseCodes(SUCCESS);
					break;
				}
				case ID_2E_090A_SET_NUMBER_ROAMING_FOR_SERVICE_CALL:
				{
					_APP_DIAG_bWaitForCallBack = BT_FALSE;
					_APP_DIAG_stRequestPacketFromVas.data[_APP_DIAG_stRequestPacketFromVas.length] = 0xFF;
					(void)EEPROM_sfRamWriteOnly(_APP_DIAG_stRequestPacketFromVas.data, EEPROM_LAYOUT_ENUM_SetNumberRoamingForServiceCall);
					CONTEXT_MAN_sfSetServiceCallNumberRoaming(_APP_DIAG_stRequestPacketFromVas.data, _APP_DIAG_stRequestPacketFromVas.length);
					enResponseImmediate = _APP_DIAG_enConvertSuccessFail_ResponseCodes(SUCCESS);
					break;
				}
				case ID_2E_090B_SET_NUMBER_FOR_INFO_CALL:
				{
					_APP_DIAG_bWaitForCallBack = BT_FALSE;
					_APP_DIAG_stRequestPacketFromVas.data[_APP_DIAG_stRequestPacketFromVas.length] = 0xFF;
					(void)EEPROM_sfRamWriteOnly(_APP_DIAG_stRequestPacketFromVas.data, EEPROM_LAYOUT_ENUM_SetNumberForInfoCall);
					CONTEXT_MAN_sfSetInfoCallNumber(_APP_DIAG_stRequestPacketFromVas.data, _APP_DIAG_stRequestPacketFromVas.length);
					enResponseImmediate = _APP_DIAG_enConvertSuccessFail_ResponseCodes(SUCCESS);
					break;
				}
				case ID_2E_090C_SET_NUMBER_FOR_SERVICE_CALL:
				{
					_APP_DIAG_bWaitForCallBack = BT_FALSE;
					_APP_DIAG_stRequestPacketFromVas.data[_APP_DIAG_stRequestPacketFromVas.length] = 0xFF;
					(void)EEPROM_sfRamWriteOnly(_APP_DIAG_stRequestPacketFromVas.data, EEPROM_LAYOUT_ENUM_SetNumberForServiceCall);
					CONTEXT_MAN_sfSetServiceCallNumber(_APP_DIAG_stRequestPacketFromVas.data, _APP_DIAG_stRequestPacketFromVas.length);
					enResponseImmediate = _APP_DIAG_enConvertSuccessFail_ResponseCodes(SUCCESS);
					break;
				}
				case ID_2E_2448_EMERGENCY_NUMBER:
				{
					_APP_DIAG_bWaitForCallBack = BT_FALSE;
					_APP_DIAG_stRequestPacketFromVas.data[_APP_DIAG_stRequestPacketFromVas.length] = 0xFF;
					(void)EEPROM_sfRamWriteOnly(_APP_DIAG_stRequestPacketFromVas.data, EEPROM_LAYOUT_ENUM_EmergencyNumber);
					CONTEXT_MAN_sfSetSOSCallNumber(_APP_DIAG_stRequestPacketFromVas.data, _APP_DIAG_stRequestPacketFromVas.length);
					enResponseImmediate = _APP_DIAG_enConvertSuccessFail_ResponseCodes(SUCCESS);
					break;
				}
	#ifdef UHV_ACTIVATE_FEATURE_SMS
				case ID_2E_0911_SMS_FUNCTIONALITY_OPERATING_UNIT:
				{
					_APP_DIAG_bWaitForCallBack = BT_FALSE;
					if (_APP_DIAG_stRequestPacketFromVas.data[0] == 1)
					{
						_APP_DIAG_bSMS_Writing = BT_TRUE;
					}
					else if (_APP_DIAG_stRequestPacketFromVas.data[0] == 0)
					{
						_APP_DIAG_bSMS_Writing = BT_FALSE;
					}
					else
					{
						_APP_DIAG_bSMS_Writing = DEFAULT_SMS_WRITING;
					}
					APP_PHONEBOOK_BAP_vDiagSmsSupportChanged(_APP_DIAG_bSMS_Writing);
					(void)EEPROM_sfRamWriteOnly(_APP_DIAG_stRequestPacketFromVas.data, EEPROM_LAYOUT_ENUM_SMSWriting);
					enResponseImmediate = _APP_DIAG_enConvertSuccessFail_ResponseCodes(SUCCESS);
					break;
				}
	#endif
				case ID_2E_0914_BLUETOOTH_ACKNOWLEDGEMENT_SIGNAL:
				{
					_APP_DIAG_bWaitForCallBack = BT_TRUE;
					if ((_APP_DIAG_stRequestPacketFromVas.data[0] |= 0) &&
						(_APP_DIAG_stRequestPacketFromVas.data[0] |= 1))
					{
						_APP_DIAG_stRequestPacketFromVas.data[0] = DEFAULT_BLUETOOTH_ACKNOWLEDGEMENT_SIGNAL;
					}
					if (0x00 == _APP_DIAG_stRequestPacketFromVas.data[0])
					{
					  bValue = BT_TRUE;
					}
					else
					{
					  bValue = BT_FALSE;
					}
					res = ERM_sFSetBluetoothAcknowledgementSignal( _APP_DIAG_cbResponseCode, bValue);
					(void)EEPROM_sfRamWriteOnly(_APP_DIAG_stRequestPacketFromVas.data, EEPROM_LAYOUT_ENUM_BluetoothAcknowledgementSignal);
					enResponseImmediate = _APP_DIAG_enConvertAPP_PHONE_ResponseCodes(res);
					break;
				}
				case ID_2E_1001_DEVELOPER_TESTMODE:
				{
					_APP_DIAG_bWaitForCallBack = BT_FALSE;
					if (_APP_DIAG_stRequestPacketFromVas.data[0] == 0)
					{
						sF = VCR_sfEnablePerformanceMeasures(BT_FALSE);
					}
					else if (_APP_DIAG_stRequestPacketFromVas.data[0] == 1)
					{
						sF = VCR_sfEnablePerformanceMeasures(BT_TRUE);
					}
					else
					{
						sF = VCR_sfEnablePerformanceMeasures(DEFAULT_DEVELOPER_TESTMODE);
					}
					enResponseImmediate = _APP_DIAG_enConvertSuccessFail_ResponseCodes(sF);
					(void)EEPROM_sfRamWriteOnly(_APP_DIAG_stRequestPacketFromVas.data, EEPROM_LAYOUT_ENUM_DeveloperTestmode);
					break;
				}
				case ID_2E_2233_MASKING_OF_LANGUAGES:
				{
					_APP_DIAG_bWaitForCallBack = BT_FALSE;
					APP_CAN_vResetLanguageDatasetdownload();
					(void)EEPROM_sfRamWriteOnly(_APP_DIAG_stRequestPacketFromVas.data, EEPROM_LAYOUT_ENUM_MaskingOfLanguages);
					enResponseImmediate = enAppDiagResponseOkImm;
					break;
				}
				case ID_2E_2416_UNIVERSAL_PREPARATION_FOR_MOBILE_TELEPHONE:
				{
					_APP_DIAG_bWaitForCallBack = BT_TRUE;
					res = ERM_sFSetUniversalPreparationForMobileTelephone( _APP_DIAG_cbResponseCode,
																		   _APP_DIAG_stRequestPacketFromVas.data,
																		   _APP_DIAG_stRequestPacketFromVas.length);
					enResponseImmediate = _APP_DIAG_enConvertAPP_PHONE_ResponseCodes(res);
					(void)EEPROM_sfRamWriteOnly(_APP_DIAG_stRequestPacketFromVas.data, EEPROM_LAYOUT_ENUM_UniversalPreparationForMobileTelephone);
					break;
				}
				case ID_2E_241C_TELEPHONE_BASIC_VOLUME:
				{
					_APP_DIAG_bWaitForCallBack = BT_TRUE;
					res = ERM_eSetTelephoneBasicVolumeAlc(_APP_DIAG_cbResponseCode, _APP_DIAG_stRequestPacketFromVas.data[0]);
					(void)EEPROM_sfRamWriteOnly(_APP_DIAG_stRequestPacketFromVas.data, EEPROM_LAYOUT_ENUM_TelephoneBasicVolume);
					enResponseImmediate = _APP_DIAG_enConvertAPP_PHONE_ResponseCodes(res);
					break;
				}
				case ID_2E_241D_VOICE_OUTPUT_BASIC_VOLUME:
				{
					_APP_DIAG_bWaitForCallBack = BT_TRUE;
					res = ERM_eSetVoiceOutputBasicVolume(_APP_DIAG_cbResponseCode, _APP_DIAG_stRequestPacketFromVas.data[0]);
					(void)EEPROM_sfRamWriteOnly(_APP_DIAG_stRequestPacketFromVas.data, EEPROM_LAYOUT_ENUM_VoiceOutputBasicVolume);
					enResponseImmediate	= _APP_DIAG_enConvertAPP_PHONE_ResponseCodes(res);
					break;
				}
				case ID_2E_241E_MICROPHONE_SENSITIVITY:
				{
					_APP_DIAG_bWaitForCallBack = BT_TRUE;
					res = ERM_eSetMicrophoneSensitivity(_APP_DIAG_cbResponseCode,
					_APP_DIAG_stRequestPacketFromVas.data[0]);
					(void)EEPROM_sfRamWriteOnly(_APP_DIAG_stRequestPacketFromVas.data, EEPROM_LAYOUT_ENUM_MicrophoneSensitivity);
					enResponseImmediate = _APP_DIAG_enConvertAPP_PHONE_ResponseCodes(res);
					break;
				}
				case ID_2E_2420_BLUETOOTH_SET_PIN_CODE:
				{
					Uint8Type u8TempPinCodeLen = 0;
					Uint8Type au8BTPinValue[AT_MAX_PIN_CODE_LEN+1];
					Uint8Type i=0;
					_APP_DIAG_bWaitForCallBack = BT_TRUE;
					while ( i < 2 )
					{
						au8BTPinValue[u8TempPinCodeLen] = (((_APP_DIAG_stRequestPacketFromVas.data[i]) & 0xF0) >> 4);
						if ( au8BTPinValue[u8TempPinCodeLen]  >  0x9 )
						{
							enResponseImmediate = enAppDiagResponseErrorImm;
							DEBUG_TEXT(APP_DIAG_SWI_WNG, MOD_APP_DIAG, "Invalid data in EEPROM");
							break;
						}
						u8TempPinCodeLen++;
						au8BTPinValue[u8TempPinCodeLen] = ((_APP_DIAG_stRequestPacketFromVas.data[i])  & 0x0F);
						if ( au8BTPinValue[u8TempPinCodeLen]  >  0x9 )
						{
							enResponseImmediate = enAppDiagResponseErrorImm;
							DEBUG_TEXT(APP_DIAG_SWI_WNG, MOD_APP_DIAG, "Invalid data in EEPROM");
							break;
						}
						u8TempPinCodeLen++;
						i++;
					}
					if ( enResponseImmediate == enAppDiagResponseErrorImm )
					{
						break;
					}
					res = ERM_sFSetBluetoothSetPINCodeDiagnose(_APP_DIAG_cbResponseCode,au8BTPinValue);
					(void)EEPROM_sfRamWriteOnly(_APP_DIAG_stRequestPacketFromVas.data, EEPROM_LAYOUT_ENUM_BluetoothSetPINCode);
					enResponseImmediate = _APP_DIAG_enConvertAPP_PHONE_ResponseCodes(res);
					break;
				}
				case ID_2E_2422_USER_PROFILES:
				{
					if (_APP_DIAG_stRequestPacketFromVas.data[0] == 1)
					{
						_APP_DIAG_bWaitForCallBack = BT_TRUE;
						res = APP_DEVICE_rcDeleteAllProfiles(_APP_DIAG_cbResponseCode);
						enResponseImmediate = _APP_DIAG_enConvertAPP_PHONE_ResponseCodes(res);
					}
					else if (_APP_DIAG_stRequestPacketFromVas.data[0] == 0)
					{
						_APP_DIAG_bWaitForCallBack = BT_FALSE;
						enResponseImmediate = enAppDiagResponseOkImm;
					}
					else
					{
						_APP_DIAG_bWaitForCallBack = BT_FALSE;
						enResponseImmediate = enAppDiagResponseErrorImm;
					}
					break;
				}
				case ID_2E_2423_MUTE_DELAY:
				{
					_APP_DIAG_bWaitForCallBack = BT_FALSE;
					APP_DEVICE_AM_vSetMuteDelayDiag(_APP_DIAG_stRequestPacketFromVas.data[0]*100); // this converts the value in ms
					(void)EEPROM_sfRamWriteOnly(_APP_DIAG_stRequestPacketFromVas.data, EEPROM_LAYOUT_ENUM_MuteDelay);
					enResponseImmediate	= _APP_DIAG_enConvertSuccessFail_ResponseCodes(SUCCESS);
					break;
				}
				case ID_2E_2424_REFERENCE_CHANNEL_DELAY:
				{
					_APP_DIAG_bWaitForCallBack = BT_TRUE;
					res = ERM_sFSetReferenceChannelDelay(_APP_DIAG_cbResponseCode, _APP_DIAG_stRequestPacketFromVas.data[0]);
					(void)EEPROM_sfRamWriteOnly(_APP_DIAG_stRequestPacketFromVas.data, EEPROM_LAYOUT_ENUM_Reference_Channel_Delay);
					enResponseImmediate = _APP_DIAG_enConvertAPP_PHONE_ResponseCodes(res);
					break;
				}
				case ID_2E_2429_MEDIA_DEVICE_BASIC_VOLUME:
				{
					_APP_DIAG_bWaitForCallBack = BT_TRUE;
					if (_APP_DIAG_stRequestPacketFromVas.data[0] > AT_VOLUME_STREAM_MAX)
					{
						_APP_DIAG_stRequestPacketFromVas.data[0] = AT_VOLUME_STREAM_MAX;
					}
					res = ERM_eSetMediaDeviceBasicVolume(_APP_DIAG_cbResponseCode, _APP_DIAG_stRequestPacketFromVas.data[0]);
					(void)EEPROM_sfRamWriteOnly(_APP_DIAG_stRequestPacketFromVas.data, EEPROM_LAYOUT_ENUM_MediaDeviceBasicVolume);
					enResponseImmediate = _APP_DIAG_enConvertAPP_PHONE_ResponseCodes(res);
					break;
				}
		    }
			/*----------------------------------------------------------------------------*/
			/* STELLGLIED TEST                                                            */
			/*----------------------------------------------------------------------------*/
            {
            	case ID_2F_016B_TESTING_SIGNAL_AUDIO:
				case ID_2F_0172_TESTING_SIGNAL_AUDIO_ALTERNATING:
				case ID_2F_02E1_AUDIO_LOOP_BACK:
				{
					switch ( _APP_DIAG_enStellgliedStatus )
					{
						case enAppDiagStartStellglied:
						{
						    _APP_DIAG_enStellgliedStatus = enAppDiagConfigureMuteBit;
						    enResponseImmediate = enAppDiagResponseFurtherRequestsPending;
						    break;
						}
						case enAppDiagConfigureMuteBit:
						{
							if ( _APP_DIAG_stRequestPacketFromVas.Identifier == ID_2F_016B_TESTING_SIGNAL_AUDIO )
							{
								enSource_InCaseOfIOControl = APP_CAN_enTelephoneCall;
							}
							else if ( _APP_DIAG_stRequestPacketFromVas.Identifier == ID_2F_0172_TESTING_SIGNAL_AUDIO_ALTERNATING )
							{
								// not work correct with APP_CAN_enAudioStream so use APP_CAN_enTelephoneCall
							    // enSource_InCaseOfIOControl = APP_CAN_enAudioStream;
							    enSource_InCaseOfIOControl = APP_CAN_enTelephoneCall;
							}
							else if ( _APP_DIAG_stRequestPacketFromVas.Identifier == ID_2F_02E1_AUDIO_LOOP_BACK )
							{
								enSource_InCaseOfIOControl = APP_CAN_enTelephoneCall;
							}
							switch ( _APP_DIAG_stRequestPacketFromVas.data[0])
							{
								case SERVICE_2F_FKT_START_EIN:
								{
									if ((   ( SDS_UDS_u8GetStatusActuatorTest() == ACTUATOR_TEST_RUNNING ) ) &&
											(( BT_FALSE == APP_PHONE_AT_PNCC_bAllCallStatesIdle()) ||
											( BT_TRUE == VCR_bCheckSIVRIsActive()) ||
											( BT_TRUE == APP_MDI_bPlayerActive() ) )
										)
									{
										enResponseImmediate = enAppDiagResponseAbortVasRequest;
									}
									else
									{
										_APP_DIAG_bWaitForCallBack = BT_TRUE;
										sF = APP_DEVICE_AM_sfGetMute(_APP_DIAG_cbResponseMuteManager, enSource_InCaseOfIOControl, __LINE__);									
										DEBUG_TEXT(APP_DIAG_SWI_TRC, MOD_APP_DIAG, "Send Mute to APP");
										if ( sF == FAIL )
										{
											_APP_DIAG_enStellgliedStatus = enAppDiagStartStellglied;
											enResponseImmediate = enAppDiagResponseErrorImm;
										}
										else
										{
											enResponseImmediate = enAppDiagResponseOkImm;
											_APP_DIAG_DisconnectMediaplayerInCaseOf_IOControl();
										}
									}
									break;
								}
								case SERVICE_2F_FKT_START_AUS:
								case SERVICE_2F_FKT_STOP_LAST_STATE:
								{
									_APP_DIAG_bWaitForCallBack = BT_FALSE;
									if ( (_APP_DIAG_stRequestPacketFromVas.Identifier == ID_2F_0172_TESTING_SIGNAL_AUDIO_ALTERNATING)  && ( BT_TRUE == APP_MDI_bPlayerActive() ))
									{
										// Do not send Unmute here because otherwise the Audio manager unmutes the Media Player
										;
									}
									else
									{
										DEBUG_TEXT(APP_DIAG_SWI_TRC, MOD_APP_DIAG, "Send Unmute to APP");
										APP_DEVICE_AM_vReleaseMute(NULL, enSource_InCaseOfIOControl,__LINE__);
									}		
									enResponseImmediate = enAppDiagResponseFurtherRequestsPending;
									_APP_DIAG_cbResponseMuteManager(APP_COMMON_RSP_OK);
									break;
								}
								default:
								{
									sF = FAIL;
									break;
								}

							}
							break;
						}
						case enAppDiagWaitForCallback:
                        {
                            enResponseImmediate = enAppDiagResponseFurtherRequestsPending;
                            DEBUG_TEXT(APP_DIAG_SWI_TRC, MOD_APP_DIAG,"Wait for callback from AppDevice in case of IO-control");
                            break;
                        }
                        case enAppDiagRequestToAppDeviceIsBusy:
                        {
                            enResponseImmediate = enAppDiagResponseFurtherRequestsPending;
                            DEBUG_TEXT(APP_DIAG_SWI_TRC, MOD_APP_DIAG,"AppDevice is busy in case of IO-control");
                            break;
                        }
						case enAppDiagTriggerAudioTones:
						{
							BooleanType value = BT_FALSE;
							switch ( _APP_DIAG_stRequestPacketFromVas.data[0])
							{
								case SERVICE_2F_FKT_START_EIN:
								{
									if ( _APP_DIAG_stRequestPacketFromVas.Identifier == ID_2F_02E1_AUDIO_LOOP_BACK )
									{
										value = BT_TRUE;
										DEBUG_TEXT(APP_DIAG_SWI_TRC, MOD_APP_DIAG, "$02E1: Start Loopback");
									}
									else
									{
										if ( _APP_DIAG_u8Service2F_ChannelLeft == 0)
										{
											SignalControl.frequencyLeft  = SDS_UDS_SIGNAL_FREQUENCY_1KHZ;
											SignalControl.frequencyRight = SDS_UDS_SIGNAL_FREQUENCY_1KHZ;
											SignalControl.chanLeftOn       = SDS_UDS_SIGNAL_CHANNEL_ON;
											SignalControl.chanRightOn      = SDS_UDS_SIGNAL_CHANNEL_OFF;
											SignalControl.outputLevelLeft  = SDS_UDS_SIGNAL_OUTPUT_LEVEL_1VEFF;
											SignalControl.outputLevelRight = SDS_UDS_SIGNAL_OUTPUT_LEVEL_1VEFF;
											_APP_DIAG_u8Service2F_ChannelLeft = 1;
										}
										else if ( _APP_DIAG_u8Service2F_ChannelLeft == 1)
										{
											if ( _APP_DIAG_stRequestPacketFromVas.Identifier == ID_2F_016B_TESTING_SIGNAL_AUDIO )
											{
												SignalControl.frequencyLeft  = SDS_UDS_SIGNAL_FREQUENCY_1KHZ;
												SignalControl.frequencyRight = SDS_UDS_SIGNAL_FREQUENCY_1KHZ;
											}
											else if ( _APP_DIAG_stRequestPacketFromVas.Identifier == ID_2F_0172_TESTING_SIGNAL_AUDIO_ALTERNATING )
											{
												SignalControl.frequencyLeft  = SDS_UDS_SIGNAL_FREQUENCY_333HZ;
												SignalControl.frequencyRight = SDS_UDS_SIGNAL_FREQUENCY_333HZ;
												SignalControl.chanLeftOn       = SDS_UDS_SIGNAL_CHANNEL_ON;
												SignalControl.chanRightOn      = SDS_UDS_SIGNAL_CHANNEL_ON;
											}
											else
											{
											    SignalControl.chanLeftOn       = SDS_UDS_SIGNAL_CHANNEL_OFF;
											    SignalControl.chanRightOn      = SDS_UDS_SIGNAL_CHANNEL_ON;
											}
											SignalControl.outputLevelLeft  = SDS_UDS_SIGNAL_OUTPUT_LEVEL_1VEFF;
											SignalControl.outputLevelRight = SDS_UDS_SIGNAL_OUTPUT_LEVEL_1VEFF;
											_APP_DIAG_u8Service2F_ChannelLeft = 0;
										}
										DEBUG_TEXT(APP_DIAG_SWI_TRC, MOD_APP_DIAG, "Send ON to APP");										
									}
									break;
								}
								case SERVICE_2F_FKT_START_AUS:
								case SERVICE_2F_FKT_STOP_LAST_STATE:
								{
									if ( _APP_DIAG_stRequestPacketFromVas.Identifier == ID_2F_02E1_AUDIO_LOOP_BACK )
									{
										value = BT_FALSE;
										DEBUG_TEXT(APP_DIAG_SWI_TRC, MOD_APP_DIAG, "$02E1: Stop Loopback");
									}
									else
									{
										SignalControl.frequencyLeft  = SDS_UDS_SIGNAL_FREQUENCY_1KHZ;
										SignalControl.frequencyRight = SDS_UDS_SIGNAL_FREQUENCY_1KHZ;
										SignalControl.chanLeftOn       = SDS_UDS_SIGNAL_CHANNEL_OFF;
										SignalControl.chanRightOn      = SDS_UDS_SIGNAL_CHANNEL_OFF;
										SignalControl.outputLevelLeft  = SDS_UDS_SIGNAL_OUTPUT_LEVEL_1VEFF;
										SignalControl.outputLevelRight = SDS_UDS_SIGNAL_OUTPUT_LEVEL_1VEFF;
										enSource_InCaseOfIOControl = APP_CAN_enTelephoneCall;
										DEBUG_TEXT(APP_DIAG_SWI_TRC, MOD_APP_DIAG, "Send OFF to APP");															
									}
									break;
								}
								default:
								{
									res = APP_COMMON_RSP_ERROR;
									break;
								}
							}
							_APP_DIAG_bWaitForCallBack = BT_TRUE;
							if ( _APP_DIAG_stRequestPacketFromVas.Identifier == ID_2F_02E1_AUDIO_LOOP_BACK )
							{
								res = ERM_eTestSetLoopback(_APP_DIAG_cbResponseCode,value);
							}
							else
							{
								res = ERM_eSetAudioChannel(_APP_DIAG_cbResponseCode, SignalControl);
							}
							enResponseImmediate	= _APP_DIAG_enConvertAPP_PHONE_ResponseCodes(res);
							DEBUG_VALUE1(APP_DIAG_SWI_TRC, MOD_APP_DIAG, "ERM status = %d", enResponseImmediate);
							if ( enResponseImmediate == enAppDiagResponseErrorImm )
							{
								enResponseImmediate = enAppDiagResponseAbortVasRequest ;
							}
							break;
						}
						default:
						{
							break;
						}
					}
					break;
				}
				case ID_2F_016C_AUDIO_MUTE_BY_WIRE:
				{
					sF = SUCCESS;
					_APP_DIAG_bWaitForCallBack = BT_FALSE;
					switch(_APP_DIAG_stRequestPacketFromVas.data[0])
					{
						case SERVICE_2F_FKT_START_EIN:
							APP_DEVICE_AM_vSetAudioMuteByWireDiag(APP_CAN_enAudioMute);
							break;
						case SERVICE_2F_FKT_START_AUS:
						case SERVICE_2F_FKT_STOP_LAST_STATE:
							APP_DEVICE_AM_vSetAudioMuteByWireDiag(APP_CAN_enAudioUnmute);
							break;
						default:
							sF = FAIL;
							break;
					}
					enResponseImmediate = _APP_DIAG_enConvertSuccessFail_ResponseCodes(sF);
					break;
				}
				case ID_2F_016D_AUDIO_MUTE_BY_CAN:
				{
					sF = SUCCESS;
					_APP_DIAG_bWaitForCallBack = BT_FALSE;
					switch ( _APP_DIAG_stRequestPacketFromVas.data[0])
					{
						case SERVICE_2F_FKT_START_EIN:
							//Stellglied test can only be started when there is no active
							//source active or rather lower source e.g. audio streaming.
							//We don't need any callback function because it will always
							//be a success.
							if ((   ( SDS_UDS_u8GetStatusActuatorTest() == ACTUATOR_TEST_RUNNING ) ) &&
									(( BT_FALSE == APP_PHONE_AT_PNCC_bAllCallStatesIdle()) ||
									( BT_TRUE == VCR_bCheckSIVRIsActive()) ||
									( BT_TRUE == APP_MDI_bPlayerActive() ) )
								)
							{
								enResponseImmediate = enAppDiagResponseErrorImm;
							}
							else
							{
								_APP_DIAG_bWaitForCallBack = BT_FALSE;
								(void)APP_DEVICE_AM_sfGetMute(NULL, APP_CAN_enNavigation, __LINE__);
								DEBUG_TEXT(APP_DIAG_SWI_TRC, MOD_APP_DIAG, "Send Mute to APP");
							}
							break;
						case SERVICE_2F_FKT_START_AUS:
						case SERVICE_2F_FKT_STOP_LAST_STATE:
							APP_DEVICE_AM_vReleaseMute(NULL, APP_CAN_enNavigation, __LINE__);
							break;
						default:
							sF = FAIL;
							break;
					}
					enResponseImmediate = _APP_DIAG_enConvertSuccessFail_ResponseCodes(sF);
					break;
				}
				case ID_2F_0170_TEST_VOICE_PROMPT:
				{
					_APP_DIAG_bWaitForCallBack = BT_FALSE;
					switch ( _APP_DIAG_stRequestPacketFromVas.data[0])
					{
						case SERVICE_2F_FKT_START_EIN:
							if (( BT_FALSE == APP_PHONE_AT_PNCC_bAllCallStatesIdle()) || (BT_TRUE == VCR_bCheckSIVRIsActive()) || ( BT_TRUE == APP_MDI_bPlayerActive() ) )
							{
								enResponseImmediate = enAppDiagResponseErrorImm;
							}
							else
							{
								res = VCR_rcPlayTestPrompt(_APP_DIAG_cbResponseCode);
								enResponseImmediate = _APP_DIAG_enConvertAPP_PHONE_ResponseCodes(res);
							}
							break;
						case SERVICE_2F_FKT_START_AUS:
						case SERVICE_2F_FKT_STOP_LAST_STATE:
							enResponseImmediate = enAppDiagResponseOkImm;
							break;
						default:
							enResponseImmediate = enAppDiagResponseErrorImm;
							break;
					}
					break;
				}
				case ID_2F_0410_POWER_SUPPLY_CRADLE_ON_OFF_TEST:
				{
					_APP_DIAG_bWaitForCallBack = BT_FALSE;
					switch (_APP_DIAG_stRequestPacketFromVas.data[0])
					{
						case SERVICE_2F_FKT_START_EIN:
						{
							sF = PHY_BUT_DRV_sfSwitchBasePlateVoltageDiagnostics(SERVICE_2F_FKT_START_EIN);
							break;
						}
						case SERVICE_2F_FKT_START_AUS:
						{
							sF = PHY_BUT_DRV_sfSwitchBasePlateVoltageDiagnostics(SERVICE_2F_FKT_START_AUS);
							break;
						}
						case SERVICE_2F_FKT_STOP_LAST_STATE:
						{
							sF = PHY_BUT_DRV_sfSwitchBasePlateVoltageDiagnostics(SERVICE_2F_FKT_STOP_LAST_STATE);
							break;
						}
						default:
						{
							sF = FAIL;
							break;
						}
					}
					enResponseImmediate	= _APP_DIAG_enConvertSuccessFail_ResponseCodes(sF);
					break;
				}
            }
		   /*----------------------------------------------------------------------------*/
		   /* MESSWERT                                                                   */
		   /*----------------------------------------------------------------------------*/
            {
				case ID_22_0286_SPANNUNG_KLEMME_30:
				{
					pwrvalue = MONITOR_ADC_u16GetMovingAverageADCValueUBatt();
					_APP_DIAG_stResponsePacketToVas.data[1] = pwrvalue & 0x00FF;
					_APP_DIAG_stResponsePacketToVas.data[0] = (pwrvalue & 0xFF00) >> 8;
					_APP_DIAG_stResponsePacketToVas.length  = 2;
					enResponseImmediate = enAppDiagResponseOkImm;
					break;
				}
				case ID_22_028D_STEUERGERAETETEMPERATUR:
				{
					_APP_DIAG_stResponsePacketToVas.data[0] =  TEMPERATURE_u8GetTemperature();
					enResponseImmediate = enAppDiagResponseOkImm;
					break;
				}
				case ID_22_2400_MICROPHONE_STATUS:
				{
					_APP_DIAG_stResponsePacketToVas.data[0] = APP_DTC_u8GetStatusMikroTelefon();
					enResponseImmediate = enAppDiagResponseOkImm;
					break;
				}
				case ID_22_2401_MICROPHONE_CURRENT_DRAIN:
				{
					_APP_DIAG_stResponsePacketToVas.data[0] = MONITOR_ADC_u8GetMikrophoneCurrentDrain();
					enResponseImmediate = enAppDiagResponseOkImm;
					break;
				}
				case ID_22_2402_TELEPHONE_SIGNAL_STRENGTH:
				{
					if (APP_PHONE_GetSignalStrength() == APP_PHONE_PIEV_INVALID_VALUE)
					{
						_APP_DIAG_stResponsePacketToVas.data[0] = 0;
					}
					else
					{
						_APP_DIAG_stResponsePacketToVas.data[0] = APP_PHONE_GetSignalStrength() * 20;       // Wertebereich 0 .. 5 => also 20% Steps
					}
					if (_APP_DIAG_stResponsePacketToVas.data[0] > 100)
					{
						_APP_DIAG_stResponsePacketToVas.data[0] = 100;
					}
					enResponseImmediate = enAppDiagResponseOkImm;
					break;
				}
				case ID_22_2403_AUDIO_OUTPUT_LEFT:
				{
					_APP_DIAG_stResponsePacketToVas.data[0] = APP_DTC_u8GetStatusAudioLeft();
					enResponseImmediate = enAppDiagResponseOkImm;
					break;
				}
				case ID_22_2404_AUDIO_OUTPUT_RIGHT:
				{
					_APP_DIAG_stResponsePacketToVas.data[0] = APP_DTC_u8GetStatusAudioRight();
					enResponseImmediate = enAppDiagResponseOkImm;
					break;
				}
				case ID_22_2466_SERVICE_CALL_BUTTON_EMERGENCY_CALL_BUTTON_CRADLE:
				{
					_APP_DIAG_stResponsePacketToVas.data[0] = LOGI_BUT_MAN_u8GetButtonStatus(LOGI_BUT_MAN_CRADLE_SOS_SERVICE_BUTTON);
					enResponseImmediate = enAppDiagResponseOkImm;
					break;
				}
				case ID_22_2467_INFO_CALL_BUTTON_VOICE_CONTROL_BUTTON_CRADLE:
				{
					_APP_DIAG_stResponsePacketToVas.data[0] = LOGI_BUT_MAN_u8GetButtonStatus(LOGI_BUT_MAN_CRADLE_INFO_BUTTON);
					enResponseImmediate = enAppDiagResponseOkImm;
					break;
				}
				case ID_22_2408_BLUETOOTH_CONNECTED_DEVICE_VIA_HFP:
				{
					AT_DeviceId  deviceIndex = ERM_u8GetActivePhone();
					/* Init response structure with 0 */
					(void)memset(&_APP_DIAG_stResponsePacketToVas, 0, sizeof(_APP_DIAG_stResponsePacketToVas));
					_APP_DIAG_stResponsePacketToVas.length = 1;
					if ((deviceIndex != AT_INVALID_DEVICE_ID) && (deviceIndex < ERM_MAX_NUM_PHYSICAL_DEVICES))
					{
						const ERMPhysicalBTDeviceType *p_phy = ERM_pGetPhyDevice(deviceIndex);
						/* jetzt hat man mit p_phy->name und p_phy->name_len zugriff auf devicenamen und dessen länge und Diagnose kann kopieren */
						if ((p_phy->name_len <= (AT_DEVICE_NAME_MAX_LEN+1)) &&
							(p_phy != NULL))
						{
							(void)memcpy(&_APP_DIAG_stResponsePacketToVas.data[0], &p_phy->name[0], p_phy->name_len);
							_APP_DIAG_stResponsePacketToVas.data[p_phy->name_len] = APP_DIAG_BT_NAME_SEPARATOR;  // separator "0x00"
						}
						else
						{
							DEBUG_VALUE1(APP_DIAG_SWI_ERR, MOD_APP_DIAG, "MWB ID 2408 HFP: invalid length %d",
										 p_phy->name_len);
						}
					}
					_APP_DIAG_stResponsePacketToVas.length = ID_22_2408_BLUETOOTH_CONNECTED_DEVICE_VIA_HFP_MAX_LEN;
					enResponseImmediate = enAppDiagResponseOkImm;
					break;
				}
				case ID_22_240A_BLUETOOTH_CONNECTED_HEADSET:
				{
					AT_DeviceId  deviceIndex = ERM_u8GetActiveHeadSet();
					/* Init response structure with 0 */
					(void)memset(&_APP_DIAG_stResponsePacketToVas, 0, sizeof(_APP_DIAG_stResponsePacketToVas));
					_APP_DIAG_stResponsePacketToVas.length = 1;
					if ((deviceIndex != AT_INVALID_DEVICE_ID) &&
						(deviceIndex < ERM_MAX_NUM_PHYSICAL_DEVICES))
					{
						const ERMPhysicalBTDeviceType     *p_phy = ERM_pGetPhyDevice(deviceIndex);
						/* jetzt hat man mit p_phy->name und p_phy->name_len zugriff auf devicenamen und dessen länge und Diagnose kann kopieren */
						if ((p_phy->name_len <= (AT_DEVICE_NAME_MAX_LEN+1)) &&
							(p_phy != NULL))
						{
							(void)memcpy(&_APP_DIAG_stResponsePacketToVas.data[0], &p_phy->name[0], p_phy->name_len);
							_APP_DIAG_stResponsePacketToVas.data[p_phy->name_len] = APP_DIAG_BT_NAME_SEPARATOR;  // separator "0x00"
						}
						else
						{
							DEBUG_VALUE1(APP_DIAG_SWI_ERR, MOD_APP_DIAG, "MWB ID 240A Headset: invalid length %d",
									   p_phy->name_len);
						}
					}
					_APP_DIAG_stResponsePacketToVas.length = ID_22_240A_BLUETOOTH_CONNECTED_HEADSET_MAX_LEN;
					enResponseImmediate = enAppDiagResponseOkImm;
					break;
				}
				case ID_22_240B_BLUETOOTH_PAIRED_DEVICES_VIA_HFP:
				{
					/* Init response structure with 0 */
					(void)memset(&_APP_DIAG_stResponsePacketToVas, 0, sizeof(_APP_DIAG_stResponsePacketToVas));
					u8Offset = 1;
					for (i=0;i<ERM_MAX_NUM_USER_PROFILES;i++)
					{/* wir holen ptr für z.B. User profile */
						const ERMUserProfileType *  p_user = ERM_pGetUserProfile(i);
						/* check ob dieses profile einen korrekten verweis in device tabelle enthalt*/
						if ((p_user->deviceIndex != AT_INVALID_DEVICE_ID) &&
							(p_user->deviceIndex < ERM_MAX_NUM_PHYSICAL_DEVICES))
						{   /* gültiger link – pointer device tabelle*/
							const ERMPhysicalBTDeviceType     * p_phy = ERM_pGetPhyDevice(p_user->deviceIndex);
							/* jetzt hat man mit p_phy->name und p_phy->name_len zugriff auf devicenamen und dessen länge und Diagnose kann kopieren */
							if ((p_phy->name_len <= (AT_DEVICE_NAME_MAX_LEN+1)) &&
								(p_phy != NULL))
							{
								if ((u8Offset + p_phy->name_len + APP_DIAG_BT_NAME_SEPARATOR_LEN) < APP_DIAG_MAX_DATA_LENGTH)
								{
									(void)memcpy(&_APP_DIAG_stResponsePacketToVas.data[u8Offset], &p_phy->name[0], p_phy->name_len);
									u8Offset += p_phy->name_len;
									/* add two "line end" informations */
									(void)memcpy(&_APP_DIAG_stResponsePacketToVas.data[u8Offset], &_APP_DIAG_u8MmbSeperator[0], APP_DIAG_BT_NAME_SEPARATOR_LEN);
									u8Offset += APP_DIAG_BT_NAME_SEPARATOR_LEN;
									u8BtDeviceCounter++;
								}
							}
						}
					}
					/* first byte is the device counter */
					_APP_DIAG_stResponsePacketToVas.data[0] = u8BtDeviceCounter;
					_APP_DIAG_stResponsePacketToVas.length = ID_22_240B_BLUETOOTH_PAIRED_DEVICES_VIA_HFP_MAX_LEN;
					enResponseImmediate = enAppDiagResponseOkImm;
					break;
				}
				case ID_22_240D_BLUETOOTH_PAIRED_MEDIA_DEVICES:
				{
					/* Init response structure with 0 */
					(void)memset(&_APP_DIAG_stResponsePacketToVas, 0, sizeof(_APP_DIAG_stResponsePacketToVas));
					EEPROM_LAYOUT_vGetPlayerEnabled(&PlayerEnabled);
					if (BT_TRUE == PlayerEnabled)
					{
                        u8Offset = 1;
                        for (i=0;i<ERM_MAX_NUM_PLAYER_PROFILE;i++)
                        {/* wir holen ptr für z.B. User profile */
                            const ERMPlayerHeadsetProfileType *p_player_hs = ERM_pGetPlayerHeadsetProfile(i);
                            /* check ob dieses profile einen korrekten verweis in device tabelle enthalt*/
                            if ((p_player_hs->deviceIndex != AT_INVALID_DEVICE_ID) &&
                                (p_player_hs->deviceIndex < ERM_MAX_NUM_PHYSICAL_DEVICES) &&
                                (p_player_hs->deviceType == APP_DEVICE_enTYPE_PLAYER))
                            {   /* gültiger link – pointer device tabelle*/
                                const ERMPhysicalBTDeviceType *p_phy = ERM_pGetPhyDevice(p_player_hs->deviceIndex);
                                /* jetzt hat man mit p_phy->name und p_phy->name_len zugriff auf devicenamen und dessen länge und Diagnose kann kopieren */
                                if ((p_phy->name_len <= (AT_DEVICE_NAME_MAX_LEN+1)) &&
                                    (p_phy != NULL))
                                {
                                    if ((u8Offset + p_phy->name_len + APP_DIAG_BT_NAME_SEPARATOR_LEN) < APP_DIAG_MAX_DATA_LENGTH)
                                    {
                                        (void)memcpy(&_APP_DIAG_stResponsePacketToVas.data[u8Offset], &p_phy->name[0], p_phy->name_len);
                                        u8Offset += p_phy->name_len;
                                        /* add two "line end" informations */
                                        (void)memcpy(&_APP_DIAG_stResponsePacketToVas.data[u8Offset], &_APP_DIAG_u8MmbSeperator[0], APP_DIAG_BT_NAME_SEPARATOR_LEN);
                                        u8Offset += APP_DIAG_BT_NAME_SEPARATOR_LEN;
                                        u8BtDeviceCounter++;
                                    }
                                }
                            }
                        }
					}
					/* first byte is the device counter */
					_APP_DIAG_stResponsePacketToVas.data[0] = u8BtDeviceCounter;
					_APP_DIAG_stResponsePacketToVas.length = ID_22_240D_BLUETOOTH_PAIRED_MEDIA_DEVICES_MAX_LEN;
					enResponseImmediate = enAppDiagResponseOkImm;
					break;
				}
				case ID_22_240F_BLUETOOTH_CONNECTED_MEDIA_DEVICE:
				{
					AT_DeviceId  deviceIndex = ERM_u8GetActivePlayer();
					/* Init response structure with 0 */
					(void)memset(&_APP_DIAG_stResponsePacketToVas, 0, sizeof(_APP_DIAG_stResponsePacketToVas));
					EEPROM_LAYOUT_vGetPlayerEnabled(&PlayerEnabled);
					if (BT_TRUE == PlayerEnabled)
					{
					    if ((deviceIndex != AT_INVALID_DEVICE_ID) && (deviceIndex < ERM_MAX_NUM_PHYSICAL_DEVICES))
                        {
                            const ERMPhysicalBTDeviceType     * p_phy = ERM_pGetPhyDevice(deviceIndex);
                            /* jetzt hat man mit p_phy->name und p_phy->name_len zugriff auf devicenamen und dessen länge und Diagnose kann kopieren */
                            if ((p_phy->name_len <= (AT_DEVICE_NAME_MAX_LEN+1)) &&
                                (p_phy != NULL))
                            {
                                (void)memcpy(&_APP_DIAG_stResponsePacketToVas.data[0], &p_phy->name[0], p_phy->name_len);
                                _APP_DIAG_stResponsePacketToVas.data[p_phy->name_len] = APP_DIAG_BT_NAME_SEPARATOR;  // separator "0x00"
                            }
                            else
                            {
                                DEBUG_VALUE1(APP_DIAG_SWI_ERR, MOD_APP_DIAG, "MWB ID 240F Media Player: invalid length %d",
                                             p_phy->name_len);
                            }
                        }
					}
					_APP_DIAG_stResponsePacketToVas.length = ID_22_240F_BLUETOOTH_CONNECTED_MEDIA_DEVICE_MAX_LEN;
					enResponseImmediate = enAppDiagResponseOkImm;
					break;
				}
				case ID_22_2411_BLUETOOTH_PAIRED_HEADSETS:
				{
					/* Init response structure with 0 */
					(void)memset(&_APP_DIAG_stResponsePacketToVas, 0, sizeof(_APP_DIAG_stResponsePacketToVas));
					u8Offset = 1;
					for (i=0;i<ERM_MAX_NUM_HEADSET_PROFILE;i++)
					{/* wir holen ptr für z.B. User profile */
						const ERMPlayerHeadsetProfileType *  p_player_hs = ERM_pGetPlayerHeadsetProfile(i);
						/* check ob dieses profile einen korrekten verweis in device tabelle enthalt*/
						if ((p_player_hs->deviceIndex != AT_INVALID_DEVICE_ID) &&
							(p_player_hs->deviceIndex < ERM_MAX_NUM_PHYSICAL_DEVICES) &&
							(p_player_hs->deviceType == APP_DEVICE_enTYPE_HEADSET))
						{   /* gültiger link – pointer device tabelle*/
							const ERMPhysicalBTDeviceType     * p_phy = ERM_pGetPhyDevice(p_player_hs->deviceIndex);
							/* jetzt hat man mit p_phy->name und p_phy->name_len zugriff auf devicenamen und dessen länge und Diagnose kann kopieren */
							if ((p_phy->name_len <= (AT_DEVICE_NAME_MAX_LEN+1)) &&
								(p_phy != NULL))
							{
								if ((u8Offset + p_phy->name_len + APP_DIAG_BT_NAME_SEPARATOR_LEN) < APP_DIAG_MAX_DATA_LENGTH)
								{
									(void)memcpy(&_APP_DIAG_stResponsePacketToVas.data[u8Offset], &p_phy->name[0], p_phy->name_len);
									u8Offset += p_phy->name_len;
									/* add two "line end" informations */
									(void)memcpy(&_APP_DIAG_stResponsePacketToVas.data[u8Offset], &_APP_DIAG_u8MmbSeperator[0], APP_DIAG_BT_NAME_SEPARATOR_LEN);
									u8Offset += APP_DIAG_BT_NAME_SEPARATOR_LEN;
									u8BtDeviceCounter++;
								}
							}
						}
					}
					/* first byte is the device counter */
					_APP_DIAG_stResponsePacketToVas.data[0] = u8BtDeviceCounter;
					_APP_DIAG_stResponsePacketToVas.length = ID_22_2411_BLUETOOTH_PAIRED_HEADSETS_MAX_LEN;
					enResponseImmediate = enAppDiagResponseOkImm;
					break;
				}
				case ID_22_2417_AERIAL_CONNECTION_STATUS:
				{
					if ( APP_DTC_u8GetStatusAntenna() != APP_DIAG_DTC_OPEN_CIRCUIT )
					{
						_APP_DIAG_stResponsePacketToVas.data[0] = 0;
					}
					else
					{
						_APP_DIAG_stResponsePacketToVas.data[0] = 1;
					}

					enResponseImmediate = enAppDiagResponseOkImm;
					break;
				}
				case ID_22_2418_MEDIA_DEVICE_CONNECTION_STATUS:
				{
					if( NULL != ERM_pGetActivePlayerProfile())
					{
					  _APP_DIAG_stResponsePacketToVas.data[0] = MEDIA_DEVICE_CONNECTED;
					}
					else
					{
					  _APP_DIAG_stResponsePacketToVas.data[0] = MEDIA_DEVICE_NOT_CONNECTED;
					}
					enResponseImmediate = enAppDiagResponseOkImm;
					break;
				}
				case ID_22_2428_BLUETOOTH_VISIBILITY:
				{
					if (ERM_bIsBluetoothVisible() == BT_TRUE)
					{
					  _APP_DIAG_stResponsePacketToVas.data[0] = 0;
					}
					else
					{
					  _APP_DIAG_stResponsePacketToVas.data[0] = 1;
					}
					enResponseImmediate = enAppDiagResponseOkImm;
					break;
				}
				case ID_22_2435_TELEPHONE_CONNECTIVITY:
				{
					_APP_DIAG_stResponsePacketToVas.data[0] = (Uint8Type)PHY_BUT_DRV_bGetTelephoneConnectivity();
					if ( PHY_BUT_DRV_bGetCradleConnectivity() == TS_UNKNOWN )
					{
						_APP_DIAG_stResponsePacketToVas.data[0] |= (Uint8Type)TS_FALSE;
					}
					else
					{
						_APP_DIAG_stResponsePacketToVas.data[0] |= (Uint8Type)(PHY_BUT_DRV_bGetCradleConnectivity() << 1);
					}
					_APP_DIAG_stResponsePacketToVas.data[0] |= (Uint8Type)(PHY_BUT_DRV_bGetBasePlateConnectivity() << 2);
					enResponseImmediate = enAppDiagResponseOkImm;
					break;
				}
				case ID_22_245E_MICROPHONE_MUTE_BUTTON_SERVICE_CALL_BUTTON:
				{
					_APP_DIAG_stResponsePacketToVas.data[0] = LOGI_BUT_MAN_u8GetButtonStatus(LOGI_BUT_MAN_3BM_MICMUTE_BUTTON);
					enResponseImmediate = enAppDiagResponseOkImm;
					break;
				}
				case ID_22_245F_BLUETOOTH_BUTTON_INFO_CALL_BUTTON:
				{
					_APP_DIAG_stResponsePacketToVas.data[0] = LOGI_BUT_MAN_u8GetButtonStatus(LOGI_BUT_MAN_3BM_BTVISIBLE_BUTTON);
					enResponseImmediate = enAppDiagResponseOkImm;
					break;
				}
				case ID_22_2460_ANSWER_TELEPHONE_BUTTON_VOICE_CONTROL_BUTTON:
				{
					_APP_DIAG_stResponsePacketToVas.data[0] = LOGI_BUT_MAN_u8GetButtonStatus(LOGI_BUT_MAN_3BM_PTT_BUTTON);
					enResponseImmediate = enAppDiagResponseOkImm;
					break;
				}
				case ID_22_0300_SOFTWAREVERSION:
				{
					_APP_DIAG_bWaitForCallBack = BT_FALSE;
					switch ( _APP_DIAG_enParrotDetails )
					{
						case enAppDiagStartBlock:
						{
							enResponseImmediate = enAppDiagResponseFurtherRequestsPending;
							_APP_DIAG_enParrotDetails = enAppDiagParrotSWVersion;
							break;
						}

						case enAppDiagParrotSWVersion:
						{
						    ERMSWVersionStringType ParrotVersion;
							_APP_DIAG_u8ParrotVersionLength = 0;
							res = ERM_rcGetCKSWVersionString(&ParrotVersion);
							enResponseImmediate = _APP_DIAG_enConvertAPP_PHONE_ResponseCodes(res);
							if ( enResponseImmediate == enAppDiagResponseOkImm )
							{
								(void)memcpy(&_APP_DIAG_stResponsePacketToVas.data[_APP_DIAG_u8ParrotVersionLength], &ParrotVersion.version[0], ParrotVersion.version_len);
								_APP_DIAG_u8ParrotVersionLength += ParrotVersion.version_len;
								/* add a blank */
								_APP_DIAG_stResponsePacketToVas.data[_APP_DIAG_u8ParrotVersionLength] = ' ';
								_APP_DIAG_u8ParrotVersionLength++;
								/* add a dollar before the derivat information */
								_APP_DIAG_stResponsePacketToVas.data[_APP_DIAG_u8ParrotVersionLength] = '$';
								_APP_DIAG_u8ParrotVersionLength++;
								enResponseImmediate = enAppDiagResponseFurtherRequestsPending;
								_APP_DIAG_enParrotDetails = enAppDiagParrotDerivative;
							}
							break;
						}
						case enAppDiagParrotDerivative:
						{
							ERMPF_CodeStringType ParrotDerivat;
							res = ERM_rcGetPF_CodeString(&ParrotDerivat);
							enResponseImmediate = _APP_DIAG_enConvertAPP_PHONE_ResponseCodes(res);
							if ( enResponseImmediate == enAppDiagResponseOkImm )
							{
								(void)memcpy(&_APP_DIAG_stResponsePacketToVas.data[_APP_DIAG_u8ParrotVersionLength], &ParrotDerivat.pfcode[0], ParrotDerivat.pfcode_len);
								_APP_DIAG_u8ParrotVersionLength += ParrotDerivat.pfcode_len;
								/* add a blank */
								_APP_DIAG_stResponsePacketToVas.data[_APP_DIAG_u8ParrotVersionLength] = ' ';
								_APP_DIAG_u8ParrotVersionLength++;
								/* add a dollar before the derivat information */
								_APP_DIAG_stResponsePacketToVas.data[_APP_DIAG_u8ParrotVersionLength] = '$';
								_APP_DIAG_u8ParrotVersionLength++;
								enResponseImmediate = enAppDiagResponseFurtherRequestsPending;
								_APP_DIAG_enParrotDetails = enAppDiagParrotBluetoothChipVersion;
							}
							break;
						}
						case enAppDiagParrotBluetoothChipVersion:
						{
							ERMBtChipVersionType p_btChipVers;
							res = ERM_rcGetBtChipVersion(&p_btChipVers);
							enResponseImmediate = _APP_DIAG_enConvertAPP_PHONE_ResponseCodes(res);
							if ( enResponseImmediate == enAppDiagResponseOkImm )
							{
								(void)memcpy(&_APP_DIAG_stResponsePacketToVas.data[_APP_DIAG_u8ParrotVersionLength], &p_btChipVers.hw_version[0], sizeof(ATPbcvHwVersion));
								_APP_DIAG_u8ParrotVersionLength += (sizeof(ATPbcvHwVersion)-1);
								/* add a blank */
								_APP_DIAG_stResponsePacketToVas.data[_APP_DIAG_u8ParrotVersionLength] = ' ';
								_APP_DIAG_u8ParrotVersionLength++;
								(void)memcpy(&_APP_DIAG_stResponsePacketToVas.data[_APP_DIAG_u8ParrotVersionLength], &p_btChipVers.sw_version[0], sizeof(ATPbcvSwVersion));
								_APP_DIAG_u8ParrotVersionLength += (sizeof(ATPbcvSwVersion)-1);
								/* add a blank */
								_APP_DIAG_stResponsePacketToVas.data[_APP_DIAG_u8ParrotVersionLength] = ' ';
								_APP_DIAG_u8ParrotVersionLength++;
								memcpy(&_APP_DIAG_stResponsePacketToVas.data[_APP_DIAG_u8ParrotVersionLength], &p_btChipVers.manufacturer[0], sizeof(ATPbcvManufacturer));
								_APP_DIAG_u8ParrotVersionLength += (sizeof(ATPbcvManufacturer)-1);
								enResponseImmediate = enAppDiagResponseFurtherRequestsPending;
								_APP_DIAG_enParrotDetails = enAppDiagParrot_SoftwareFeaturesField;
							}
							break;
						}
						case enAppDiagParrot_SoftwareFeaturesField:
                        {
                            ERMSWVersionStringType ParrotVersion;
                            res = ERM_rcGetCKSWVersionString(&ParrotVersion);
                            enResponseImmediate = _APP_DIAG_enConvertAPP_PHONE_ResponseCodes(res);
                            if ( enResponseImmediate == enAppDiagResponseOkImm )
                            {
                                DEBUG_VALUE1(APP_DIAG_SWI_TRC, MOD_APP_DIAG, "Parrot SoftwareFeaturesField = %d ", ParrotVersion.u16Description);
                                snprintf((char *)&_APP_DIAG_stResponsePacketToVas.data[_APP_DIAG_u8ParrotVersionLength]\
                                                 ,LEN_FOR_FORMAT_SOFTWAREFEATURESFIELD," $%d ",ParrotVersion.u16Description);
                                _APP_DIAG_u8ParrotVersionLength += LEN_FOR_FORMAT_SOFTWAREFEATURESFIELD;
                                enResponseImmediate = enAppDiagResponseFurtherRequestsPending;
                                _APP_DIAG_enParrotDetails = enAppDiagParrotVersionCompleted;
                            }
                            break;
                        }
						case enAppDiagParrotVersionCompleted:
						{
							/* ensure string termination */
							_APP_DIAG_stResponsePacketToVas.data[_APP_DIAG_u8ParrotVersionLength] = '\0';
							_APP_DIAG_stResponsePacketToVas.length =  _APP_DIAG_u8ParrotVersionLength + 1;
							DEBUG_VALUE1(APP_DIAG_SWI_TRC, MOD_APP_DIAG, " Read Completed with length = %d", _APP_DIAG_stResponsePacketToVas.length);
							enResponseImmediate = enAppDiagResponseOkImm;
							_APP_DIAG_enParrotDetails = enAppDiagStartBlock;
							break;
						}
					}
					break;
				}
				case ID_22_1100_BLUETOOTH_DEVICE_MAC_ADDRESS:
				{
					res = ERM_enGetBTAddress (&au8BluetoothAddress);
					if (res == APP_COMMON_RSP_ERROR_NOT_INITIALIZED)
					{
						enResponseImmediate = enAppDiagResponseErrorImm;
					}
					else if (res == APP_COMMON_RSP_OK)
					{
						for (i=0; i < AT_BT_ADDRESS_LEN; i++)
						{
							_APP_DIAG_stResponsePacketToVas.data[i] = 0;
							_APP_DIAG_u8ConvertToInteger(&au8BluetoothAddress[i]);
						}
						for (i=0; i < AT_BT_ADDRESS_LEN; i++)
						{
							_APP_DIAG_stResponsePacketToVas.data[i/2] = ((_APP_DIAG_stResponsePacketToVas.data[i/2])<<4) + au8BluetoothAddress[i];
							DEBUG_VALUE1(APP_DIAG_SWI_TRC, MOD_APP_DIAG,"%02X", _APP_DIAG_stResponsePacketToVas.data[i/2]);
						}
						enResponseImmediate = enAppDiagResponseOkImm;
					}
					break;
				}
				default:
				{
					break;
				}
			}
		}
	    if  (enResponseImmediate == enAppDiagResponseErrorImm )
        {
	    	_APP_DIAG_enCodierungStates = enStartCodierung;
	    	_APP_DIAG_stResponsePacketToVas.Identifier = _APP_DIAG_stRequestPacketFromVas.Identifier;
	    	_APP_DIAG_stResponsePacketToVas.result = enResponseImmediate;

	    	DEBUG_VALUE2(APP_DIAG_SWI_ERR, MOD_APP_DIAG,"Immediate Response from Application=%d,Diagnose id=%04X",
	    			     enResponseImmediate,_APP_DIAG_stResponsePacketToVas.Identifier);
	    	_APP_DIAG_bWaitForCallBack = BT_FALSE;
	    	_APP_DIAG_enParrotDetails = enAppDiagStartBlock;
	    	(void)SendMessage(msgAppDiagDiagnoseResponse,&_APP_DIAG_stResponsePacketToVas);
	    }
	    else if ( enResponseImmediate == enAppDiagResponseAbortVasRequest)
	    {
	    	_APP_DIAG_enCodierungStates = enStartCodierung;
	    	_APP_DIAG_stResponsePacketToVas.Identifier = _APP_DIAG_stRequestPacketFromVas.Identifier;
	    	_APP_DIAG_stResponsePacketToVas.result = enResponseImmediate;
	    	DEBUG_VALUE2(APP_DIAG_SWI_ERR, MOD_APP_DIAG,"Immediate Response from Application=%d,Diagnose id=%04X",
	    			     enResponseImmediate,_APP_DIAG_stResponsePacketToVas.Identifier);
	    	_APP_DIAG_bWaitForCallBack = BT_FALSE;
	    	_APP_DIAG_enParrotDetails = enAppDiagStartBlock;

	    	// Now simulate a Stellglied Stop to the application, to bring the system in a correct state.
	    	(void)SendMessage(msgAppDiagDiagnoseResponse,&_APP_DIAG_stResponsePacketToVas);
	    	if ( ( _APP_DIAG_stRequestPacketFromVas.Identifier == ID_2F_016B_TESTING_SIGNAL_AUDIO ) ||
    			 ( _APP_DIAG_stRequestPacketFromVas.Identifier == ID_2F_0172_TESTING_SIGNAL_AUDIO_ALTERNATING ) ||
    			 ( _APP_DIAG_stRequestPacketFromVas.Identifier == ID_2F_02E1_AUDIO_LOOP_BACK ) ||
    			 ( _APP_DIAG_stRequestPacketFromVas.Identifier == ID_2F_016D_AUDIO_MUTE_BY_CAN ) )
    		{
    			(void)SetRelAlarm(CTA_APP_DIAG_RETRY_BUSY,MSEC(100),MSEC(0));
    		}
	    }
	    else if ( (enResponseImmediate == enAppDiagResponseErrorBusyImm))
	    {
	    	if (APP_DIAG_ubCounterBusyResponseFromAppTask > MAX_BUSY_RESPONSE_FROM_AP_TASK)
            {
	    		_APP_DIAG_enCodierungStates = enStartCodierung;
	    		APP_DIAG_ubCounterBusyResponseFromAppTask = 0;
	    		_APP_DIAG_stResponsePacketToVas.Identifier = _APP_DIAG_stRequestPacketFromVas.Identifier;
	    		_APP_DIAG_stResponsePacketToVas.result = enResponseImmediate;
	    		_APP_DIAG_enStellgliedStatus = enAppDiagStartStellglied;
	    		_APP_DIAG_enParrotDetails = enAppDiagStartBlock;
	    		(void)SendMessage(msgAppDiagDiagnoseResponse,&_APP_DIAG_stResponsePacketToVas);
	    		DEBUG_VALUE2(APP_DIAG_SWI_ERR, MOD_APP_DIAG,"GIVE UP RETRY Immediate Response from Application=%d,Diagnose id=%04X",
	    				         enResponseImmediate,_APP_DIAG_stRequestPacketFromVas.Identifier);
	    		(void)SetRelAlarm(CTA_APP_DIAG_RETRY_BUSY,MSEC(100),MSEC(0));
	    	}
            else
	    	{
	    		DEBUG_VALUE2(APP_DIAG_SWI_WNG, MOD_APP_DIAG,"Immediate Response from Application=%d,Diagnose id=%04X",
	    				         enResponseImmediate,_APP_DIAG_stRequestPacketFromVas.Identifier);
	    		APP_DIAG_ubCounterBusyResponseFromAppTask++;
    			(void)SetRelAlarm(CTA_APP_DIAG_RETRY_BUSY,MSEC(10),MSEC(0));
	    	}
	    }
	    else if ( (enResponseImmediate == enAppDiagResponseFurtherRequestsPending))
	    {
	    	(void)SetRelAlarm(CTA_APP_DIAG_RETRY_BUSY,MSEC(100),MSEC(0));
	    }
	    else if (enResponseImmediate == enAppDiagResponseOkImm)
        {
	    	DEBUG_VALUE2(APP_DIAG_SWI_TRC, MOD_APP_DIAG,"Immediate Response from Application=%d,Diagnose id=%04X",
	    			     enResponseImmediate,_APP_DIAG_stRequestPacketFromVas.Identifier);
	    	DEBUG_VALUE1(APP_DIAG_SWI_TRC, MOD_APP_DIAG,"First byte in the payload=%d",
	    			     _APP_DIAG_stResponsePacketToVas.data[0]);
	    	_APP_DIAG_enParrotDetails = enAppDiagStartBlock;
	    	(void)SetRelAlarm(CTA_APP_DIAG_WAIT_FOR_APP_CB, MSEC(0), MSEC(10));
	    }
        else if ( (enResponseImmediate == enAppDiagResponseWaitForCallBack))
        {
        	;
        }
    }
}

ALARMCALLBACK(APP_DIAG_vHandlePendingResponseFromApplication)
{
	if ( (_APP_DIAG_stResponsePacketToVas.Identifier == ID_2E_0600_CODIERUNG))
    {
		if (_APP_DIAG_u8CounterCodierung == ID_2E_0600_CODIERUNG_MAX_LEN) // when all codierung fns are called successfully.
		{
			(void)APP_COMMON_UTILS_stCancelOsekRelAlarm(CTA_APP_DIAG_WAIT_FOR_APP_CB);
			_APP_DIAG_stResponsePacketToVas.Identifier = _APP_DIAG_stRequestPacketFromVas.Identifier;
			_APP_DIAG_stResponsePacketToVas.result = enAppDiagResponseOkImm;
		    (void)SendMessage(msgAppDiagDiagnoseResponse,&_APP_DIAG_stResponsePacketToVas);
		}
		else
		{
			if (APP_DIAG_ubCounterWaitForCallBack > MAX_BUSY_RESPONSE_FROM_AP_TASK)
            {
			    APP_DIAG_ubCounterWaitForCallBack = 0;
				_APP_DIAG_stResponsePacketToVas.Identifier = _APP_DIAG_stRequestPacketFromVas.Identifier;
				_APP_DIAG_stResponsePacketToVas.result = enAppDiagResponseWaitForCallBack;
				(void)SendMessage(msgAppDiagDiagnoseResponse,&_APP_DIAG_stResponsePacketToVas);
			}
            else
			{
				APP_DIAG_ubCounterWaitForCallBack++;
			}
        }
    }
	else  // STELLGLIED und ANPASSUNG
	{
		if (_APP_DIAG_bWaitForCallBack == BT_FALSE)
		{
			(void)APP_COMMON_UTILS_stCancelOsekRelAlarm(CTA_APP_DIAG_WAIT_FOR_APP_CB);
			_APP_DIAG_stResponsePacketToVas.Identifier = _APP_DIAG_stRequestPacketFromVas.Identifier;
			_APP_DIAG_stResponsePacketToVas.result = enAppDiagResponseOkImm;
			(void)SendMessage(msgAppDiagDiagnoseResponse,&_APP_DIAG_stResponsePacketToVas);
		}
		else if ( _APP_DIAG_bWaitForCallBack == BT_TRUE )
		{
			if (APP_DIAG_ubCounterWaitForCallBack > MAX_BUSY_RESPONSE_FROM_AP_TASK)
			{
				APP_DIAG_ubCounterWaitForCallBack = 0;
				_APP_DIAG_enStellgliedStatus = enAppDiagStartStellglied;
				_APP_DIAG_stResponsePacketToVas.Identifier = _APP_DIAG_stRequestPacketFromVas.Identifier;
				_APP_DIAG_stResponsePacketToVas.result = enAppDiagResponseWaitForCallBack;
				(void)SetRelAlarm(CTA_APP_DIAG_RETRY_BUSY,MSEC(100),MSEC(0));
				(void)SendMessage(msgAppDiagDiagnoseResponse,&_APP_DIAG_stResponsePacketToVas);
			}
			else
			{
				APP_DIAG_ubCounterWaitForCallBack++;
			}
		}
	}
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_DIAG_u8ConvertToInteger                              */
/**
 \brief       to convert ASCII into HEX.
 */
/*----------------------------------------------------------------------------*/
static void _APP_DIAG_u8ConvertToInteger(Uint8Type *ptru8BluetoothAddress)
{
	if ( (*ptru8BluetoothAddress >= 0x30) && (*ptru8BluetoothAddress <= 0x39))
	{
		*ptru8BluetoothAddress = *ptru8BluetoothAddress - 0x30;
	}
	else if ( (*ptru8BluetoothAddress >= 0x41) && (*ptru8BluetoothAddress <= 0x46))
	{
		*ptru8BluetoothAddress = *ptru8BluetoothAddress - 0x37;
	}
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_DIAG_GetDiag_PhonebookSortOrder                          */
/**
    \brief      Get diagnosis Phone Book Sort Order

    \brief      Description:
                function deliver the current Phone Book Sort Order coding info
                done in diagnosis

    \return     APP_DIAG_ENUM_PHONEBOOK_SORTORDER_CODE Diag_PhonebookSortOrder
*/
/*----------------------------------------------------------------------------*/
APP_DIAG_ENUM_PHONEBOOK_SORTORDER_CODE APP_DIAG_GetDiag_PhonebookSortOrder(void)
{
	Uint8Type Diag_PhonebookSortOrder;

	EEPROM_LAYOUT_vGetPhonebookSortOrder(&Diag_PhonebookSortOrder);
	DEBUG_STATE1(APP_DIAG_SWI_TRC, MOD_APP_DIAG,"Phonebook sort order->$APP_DIAG_ENUM_PHONEBOOK_SORTORDER_CODE$=%d", (APP_DIAG_ENUM_PHONEBOOK_SORTORDER_CODE) Diag_PhonebookSortOrder);

	return ((APP_DIAG_ENUM_PHONEBOOK_SORTORDER_CODE)Diag_PhonebookSortOrder);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_DIAG_DisconnectMediaplayerInCaseOf_IOControl            */
/**
    \brief      Disconnect media player in case of IO Control

    \brief      Description:
                Disconnect media player in case of IO Control and set APP_DIAG_enStellgliedStatus

    \return    void
*/
/*----------------------------------------------------------------------------*/
static void _APP_DIAG_DisconnectMediaplayerInCaseOf_IOControl(void)
{
    // Response code default setting for media player disconnect handling
    APP_COMMON_enResponseCode res = APP_COMMON_RSP_OK;
    APP_DIAG_ENUM_RESPONSE_CODES enResponseImmediate = enAppDiagResponseOkImm;

    // default handle for next step in state machine for IO-control diagnose
     _APP_DIAG_enStellgliedStatus = enAppDiagTriggerAudioTones;

    if((_APP_DIAG_stRequestPacketFromVas.Identifier == ID_2F_0172_TESTING_SIGNAL_AUDIO_ALTERNATING) &&
       (enSource_InCaseOfIOControl == APP_CAN_enAudioStream))
    {
        // Start disconnect media player and wait until media player is disconnect
        // This is handle over call back function _APP_DIAG_vMediaPlayerIsDisconnected_Cb
        res = APP_DEVICE_rcDeactivateMdiDevice(_APP_DIAG_vMediaPlayerIsDisconnected_Cb);
        enResponseImmediate = _APP_DIAG_enConvertAPP_PHONE_ResponseCodes(res);
        if (enAppDiagResponseOkImm == enResponseImmediate)
        {
            _APP_DIAG_enStellgliedStatus = enAppDiagWaitForCallback;
            DEBUG_TEXT(APP_DIAG_SWI_TRC, MOD_APP_DIAG,"Start disconnect media player in case of IO-control");
        }
        else if (enAppDiagResponseErrorBusyImm == enResponseImmediate)
        {   /* App is busy do a retry */
            _APP_DIAG_enStellgliedStatus = enAppDiagRequestToAppDeviceIsBusy;
            DEBUG_TEXT(APP_DIAG_SWI_TRC, MOD_APP_DIAG,"Application is busy. Do a retry for disconnect media player");
        }
        else
        {   /* enResponseImmediate is enAppDiagResponseErrorImm this means no media player is connected
               next step is trigger audio tones ==> _APP_DIAG_enStellgliedStatus = enAppDiagTriggerAudioTones */
            DEBUG_TEXT(APP_DIAG_SWI_TRC, MOD_APP_DIAG,"No connected media player during IO-control");
        }
    }
}

/*----------------------------------------------------------------------------*/
/* Function    :  _APP_DIAG_vMediaPlayerIsDisconnected_Cb                     */
/**
    \brief      Call Back which is call case is media player is disconnected

    \brief      Description:
                Call Back function which is call in case media player is successful disconnected

    \return    void
*/
/*----------------------------------------------------------------------------*/

void _APP_DIAG_vMediaPlayerIsDisconnected_Cb(APP_COMMON_enResponseCode result)
{
    if(APP_COMMON_RSP_OK == result)
    {
        /* Media player correct disconnected go to next step to start IO-Control */
        _APP_DIAG_enStellgliedStatus = enAppDiagTriggerAudioTones;
        DEBUG_VALUE1(APP_DIAG_SWI_TRC, MOD_APP_DIAG, "Media player successful disconnected with ID: %d",ERM_u8GetActivePlayer());
    }
    else
    {
        DEBUG_VALUE1(APP_DIAG_SWI_TRC, MOD_APP_DIAG, "Problem handle disconnect media player %d", result);
        // Something went wrong. Anyhow try to do the next step
        _APP_DIAG_enStellgliedStatus = enAppDiagTriggerAudioTones;
    }
}

/* End Of File APP_DIAG_C1.c */
