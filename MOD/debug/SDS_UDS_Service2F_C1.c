/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/
/**
 * \file 	SDS_UDS_Service2F_C1.c
 * \brief	This file contains the program to xxxx
 * \brief	This file fulfills the following requirements:
 * 			1. Requirement 1
 * 			2. Requirement 2
 * 			3. Requirement x
*/
/*-----------------------------------------------------------------------------
* $Date: 2010-11-02 09:21:36 +0100 (Di, 02 Nov 2010) $
* $Rev: 16010 $
* $Author: weber $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_SDS_UDS/trunk/src/SDS_UDS_Service2F_C1.c $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include "tms470r1.h"
#include <string.h>
#include <osek.h>           // vector
#include <osekext.h>        // vector
/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/
#include "Common.h"
#include "sds.h"
#include "uds.h"
#include "tp.h"
#include "deh.h"
#include "VCR.h"
#include "sds_ext.h"
#include "ISRMask_CB_cfg.h"
#include "APP_DIAG.h"
#include "can_def.h"
#include "can_cfg.h"
#include "crcmgr.h"
#include "deh_dtc_cfg.h"
#include "sds_cfg.h"
#include "OSEK_HostTask_H1.h"
#include "CSHDL.h"
#include "TRACE.h"
/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "SDS_ext.h"
#include "SDS_UDS_CI.h"
#include "SDS_UDS_Service2F_C1.id"

/*----------------------------------------------------------------------------*/
/* pragma Directives                                                          */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Definitions (avoid as much as possible!)                  */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Private Types                                                              */
/*----------------------------------------------------------------------------*/
/**
\brief	description of the type
*/
typedef struct
{
	word DataId;
	  word MinLen;                   /* minimum length of data */
	  word MaxLen;                   /* maximum length of data */
} t_StellgliedDataIdCfgItem;
/*----------------------------------------------------------------------------*/
/* Private Manifest Constant Definitions                                      */
/*----------------------------------------------------------------------------*/
/************** A T T E N T I O N *****************************
   because of speed optimization binary search algorithm is used
   which requires a sorted (ascending !!!) data identifier table */

static const t_StellgliedDataIdCfgItem cs_StellgliedDataIdItem[] = {
{ID_2F_016B_TESTING_SIGNAL_AUDIO, ID_2F_COMMON_STELLGLIED_MIN_LEN, ID_2F_COMMON_STELLGLIED_MAX_LEN},
{ID_2F_016C_AUDIO_MUTE_BY_WIRE, ID_2F_COMMON_STELLGLIED_MIN_LEN, ID_2F_COMMON_STELLGLIED_MAX_LEN},
{ID_2F_016D_AUDIO_MUTE_BY_CAN, ID_2F_COMMON_STELLGLIED_MIN_LEN, ID_2F_COMMON_STELLGLIED_MAX_LEN},
{ID_2F_0170_TEST_VOICE_PROMPT, ID_2F_COMMON_STELLGLIED_MIN_LEN, ID_2F_COMMON_STELLGLIED_MAX_LEN},
{ID_2F_0171_ILLUMINATION_TELEPHONE_OPERATING_UNIT, ID_2F_COMMON_STELLGLIED_MIN_LEN, ID_2F_COMMON_STELLGLIED_MAX_LEN},
{ID_2F_0172_TESTING_SIGNAL_AUDIO_ALTERNATING, ID_2F_COMMON_STELLGLIED_MIN_LEN, ID_2F_COMMON_STELLGLIED_MAX_LEN},
{ID_2F_02E1_AUDIO_LOOP_BACK, ID_2F_COMMON_STELLGLIED_MIN_LEN, ID_2F_COMMON_STELLGLIED_MAX_LEN},
{ID_2F_0410_POWER_SUPPLY_CRADLE_ON_OFF_TEST, ID_2F_COMMON_STELLGLIED_MIN_LEN, ID_2F_COMMON_STELLGLIED_MAX_LEN},
{ID_2F_0411_ILLUMINATION_TELEPHONE_OPERATION_UNIT_CONTROL_TEST, ID_2F_COMMON_STELLGLIED_MIN_LEN, ID_2F_COMMON_STELLGLIED_MAX_LEN},
{ID_2F_0412_BLUETOOTH_AUDIO_PATH_LOOPBACK_CONTROL_TEST, ID_2F_COMMON_STELLGLIED_MIN_LEN, ID_2F_COMMON_STELLGLIED_MAX_LEN}
};
/*----------------------------------------------------------------------------*/
/* Prototypes of file local Methods                                           */
/*----------------------------------------------------------------------------*/
static tSDSextWrite_Return _SDS_UDS_u8HandleService2F(
		Uint16Type u16Service2FInputOutputID, Uint8Type u8InputOutputParameter,
		Uint8Type u8Service2FControlState1, Uint8Type u8Service2FControlState2) ;
static SuccessFailType _SDS_UDS_sfStoreTimerIOControlActuatorTest(
		Uint8Type Timer);
static BooleanType _SDS_UDS_bWriteDiagnosisLengthCheck_2F(
		Uint16Type *ptru16ReceivedLen, const t_StellgliedDataIdCfgItem* DataIdItem, Uint8Type u8ControlParameter);
/*----------------------------------------------------------------------------*/
/* File local Code Macros                                                        */
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
// Timer for IO Control Actuator Test
static Uint16Type _SDS_UDS_u16TimerIOControlActuatorTest = 0;
// Timer for IO Control Actuator Test: Channel timer
static Uint16Type _SDS_UDS_u16TimerChannelIOControlActuatorTest = 0;
static Uint16Type _SDS_UDS_u16Service2FactivInputOutputID = 0;
// Status for IO Control Actuator Test   0 = OFF, 1 = ON
static Uint8Type _SDS_UDS_u8Service2F_ActuatorTestStatus= 0x0;
static Uint8Type _SDS_UDS_u8u8Service2FControlState2 = 0x0;

static Uint16Type _SDS_UDS_u16TimerChannelStellgliedFinised = 0;
/*----------------------------------------------------------------------------*/
/* File local const Definitions                                               */
/*----------------------------------------------------------------------------*/
/**
\brief	description of the private constant
*/

#define SDS_UDS_SERVICE_2F_RETURN_CONTROL_TO_ECU                     0
#define SDS_UDS_SERVICE_2F_FREEZE_CURRENT_STATE                      2
#define SDS_UDS_SERVICE_2F_SHORT_TERM_ADJUSTMENT                     3

#define SDS_UDS_SERVICE_2F_TIMER                                     0xFA
#define SDS_UDS_SERVICE_2F_INFINITE_TIMING_NO_TIMING_DEFINED         0xFF

#define SDS_UDS_CHANNEL_2_SEKUNDEN                          200  // the task is called every 10 milliseconds. Hence 200*10 = 2 seconds.
#define SDS_UDS_CHANNEL_5_SEKUNDEN                          399
#define SDS_UDS_CHANNEL_10_SEKUNDEN                          1000
/*----------------------------------------------------------------------------*/
/* Private Address Calculations                                               */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Function    : SDS_UDS_vInitialiseService2F                                       */
/**
 \brief      To reset the Static variables

 \brief      Description:
 this function resets the static variables.


 \return     void
 */
/*----------------------------------------------------------------------------*/
void SDS_UDS_vInitialiseService2F(void)
{
	if ( (UDScub_DEFAULT_SESSION == (Uint8Type)UDS_ubGetSession()) && (_SDS_UDS_u8Service2F_ActuatorTestStatus == ACTUATOR_TEST_RUNNING))
	{
		_SDS_UDS_u16TimerChannelIOControlActuatorTest = 1; // for all other stellglied tests which have no channel timer
	}
	else
	{
		_SDS_UDS_u8Service2F_ActuatorTestStatus = ACTUATOR_TEST_NOT_RUNNING;
	}

}


/*----------------------------------------------------------------------------*/
/* Function    : _SDS_UDS_vService2F_CallApp_Diag                                                 */
/**
 \brief      internal function to call the corresponding application function

 \param      ID
 service 2F identifier

 \param      data
 state value

 \return     None

 */
/*----------------------------------------------------------------------------*/
tSDSextWrite_Return _SDS_UDS_vService2F_CallApp_Diag(Uint16Type ID, Uint8Type data) {
	int i;
	tSDSextWrite_Return retVal = SDS_IO_USER_WRITE_SUCCESSFUL;

	DEBUG_TEXT(SDS_UDS_SWI_DBG, MOD_SDS_UDS, "Called _SDS_UDS_vService2F_CallApp_Diag");
	_SDS_UDS_u16Service2FactivInputOutputID = ID; // den Identifier merken für das spätere gezielte Deaktivieren


#ifndef USE_DEBUGGING_WITH_TRACE_ID
	DEBUG_Printf( UHV_DEBUG_1, UHV_SDS_UDS, "ReqPend=%d and Resp from APP = %d", ub_RequestPending, _SDS_UDS_stResponsePacketToVas.result);
#endif
	if (ub_RequestPending == 1)
	{
		DEBUG_VALUE1(SDS_UDS_SWI_DBG, MOD_SDS_UDS, "ResponseAppTask=%d", ubResponseFromAppTask);
		if (ubResponseFromAppTask == 1)
		{
			ubResponseFromAppTask = 0;
			if (_SDS_UDS_stResponsePacketToVas.result
					== (Uint8Type)enAppDiagResponseErrorBusyImm)
			{
					ub_RequestPending = 0;
					ubCounterResponseFromAppTask = 0;
	                    DEBUG_VALUE1(SDS_UDS_SWI_ERR, MOD_SDS_UDS, "Give up Busy Response%04X",
								_SDS_UDS_stRequestPacketFromVas.Identifier);

						_SDS_UDS_u8Service2F_ActuatorTestStatus = ACTUATOR_TEST_ABORTED;
					retVal = SDS_IO_USER_WRITE_FAILED;
			}
			else if (_SDS_UDS_stResponsePacketToVas.result
					== (Uint8Type)enAppDiagResponseWaitForCallBack)
			{
					ub_RequestPending = 0;
					ubCounterResponseFromAppTask = 0;

					DEBUG_VALUE1(SDS_UDS_SWI_ERR, MOD_SDS_UDS, "Give up WAit Retry %04X",
							_SDS_UDS_stRequestPacketFromVas.Identifier);

					_SDS_UDS_u8Service2F_ActuatorTestStatus = ACTUATOR_TEST_ABORTED;
					retVal =  SDS_IO_USER_WRITE_FAILED;
			}
			else
			{

				ub_RequestPending = 0;
				ubCounterResponseFromAppTask = 0;
				if ( _SDS_UDS_stResponsePacketToVas.result
						== (Uint8Type)enAppDiagResponseErrorImm )
				{
                    DEBUG_VALUE1(SDS_UDS_SWI_ERR, MOD_SDS_UDS, "Error %04X",
							_SDS_UDS_stRequestPacketFromVas.Identifier);

					_SDS_UDS_u8Service2F_ActuatorTestStatus = ACTUATOR_TEST_ABORTED;
					retVal =  SDS_IO_USER_WRITE_FAILED; // das war wohl nix, sonst ab ins EEPROM...
				}

				else if ( _SDS_UDS_stResponsePacketToVas.result
						== (Uint8Type)enAppDiagResponseAbortVasRequest )
				{
                    DEBUG_VALUE1(SDS_UDS_SWI_ERR, MOD_SDS_UDS, "Abort Current Request %04X",
							_SDS_UDS_stRequestPacketFromVas.Identifier);

					_SDS_UDS_u8Service2F_ActuatorTestStatus = ACTUATOR_TEST_ABORTED;
					retVal =  SDS_IO_USER_WRITE_FAILED; // das war wohl nix, sonst ab ins EEPROM...
				}

				else if (_SDS_UDS_stResponsePacketToVas.result
						== (Uint8Type)enAppDiagResponseOkImm)
				{
					// wenn wir hier angekommen sind, dann ist von seiten der Applikation alles okay
                    DEBUG_VALUE1(SDS_UDS_SWI_TRC, MOD_SDS_UDS, "SUCCESS %04X",
							_SDS_UDS_stRequestPacketFromVas.Identifier);
					if (data == SERVICE_2F_FKT_STOP_LAST_STATE)
					{
						_SDS_UDS_u16TimerChannelIOControlActuatorTest
								= 0;
					}
					else
					{
						switch (_SDS_UDS_u16Service2FactivInputOutputID)
						{
							case ID_2F_016B_TESTING_SIGNAL_AUDIO:
								_SDS_UDS_u16TimerChannelIOControlActuatorTest
										= SDS_UDS_CHANNEL_5_SEKUNDEN;
								break;

							case ID_2F_0170_TEST_VOICE_PROMPT:
								_SDS_UDS_u16TimerChannelIOControlActuatorTest
										= SDS_UDS_CHANNEL_5_SEKUNDEN;
								break;

							case ID_2F_0172_TESTING_SIGNAL_AUDIO_ALTERNATING:
								_SDS_UDS_u16TimerChannelIOControlActuatorTest
										= SDS_UDS_CHANNEL_5_SEKUNDEN;
								break;

							default:
								break;
						}
					}
					retVal = SDS_IO_USER_WRITE_SUCCESSFUL;
				}
			}
			if ( ubResponseFromAppTask == 0x0 )
			{
				_SDS_UDS_stResponsePacketToVas.result = 0xFF;
			}
		}
		else
		{
			if (ubCounterResponseFromAppTask > MAX_RESPONSE_FROM_AP_TASK)
			{
				ub_RequestPending = 0;
				ubCounterResponseFromAppTask = 0;
                DEBUG_VALUE1(SDS_UDS_SWI_ERR, MOD_SDS_UDS, "Give up No Response %04X",
						_SDS_UDS_stRequestPacketFromVas.Identifier);

				retVal = SDS_IO_USER_WRITE_FAILED;
			}
			else
			{
				ubCounterResponseFromAppTask++;
			}
		}
	}
	else
	{
		_SDS_UDS_stRequestPacketFromVas.Identifier = ID;
		_SDS_UDS_stRequestPacketFromVas.length = 1;   // 0x00 => OFF, 0xFF => ON
		for (i=0; i<_SDS_UDS_stRequestPacketFromVas.length; i++)
		_SDS_UDS_stRequestPacketFromVas.data[i] = data;
		if (data == SERVICE_2F_FKT_STOP_LAST_STATE)
		{
			_SDS_UDS_u16TimerChannelIOControlActuatorTest = 0;
		}
		else
		{
			switch (_SDS_UDS_u16Service2FactivInputOutputID)
			{
				case ID_2F_016B_TESTING_SIGNAL_AUDIO:
					_SDS_UDS_u16TimerChannelIOControlActuatorTest
							= SDS_UDS_CHANNEL_5_SEKUNDEN;
					break;

				case ID_2F_0170_TEST_VOICE_PROMPT:
					_SDS_UDS_u16TimerChannelIOControlActuatorTest
							= SDS_UDS_CHANNEL_5_SEKUNDEN;
					break;

				case ID_2F_0172_TESTING_SIGNAL_AUDIO_ALTERNATING:
					_SDS_UDS_u16TimerChannelIOControlActuatorTest
							= SDS_UDS_CHANNEL_5_SEKUNDEN;
					break;

				default:
					break;
			}
		}
		DEBUG_VALUE1(SDS_UDS_SWI_TRC, MOD_SDS_UDS, "Request to APP Task not yet sent, hence response is %d", ubResponseFromAppTask);
		ub_RequestPending = 1; // den App-Task Routinen Zeit zum Antworten geben.
		// ... und ab ...
		if ( ubResponseFromAppTask == 0x0 )
		{
			_SDS_UDS_stResponsePacketToVas.result = 0xFF;
		}
		DEBUG_TEXT(SDS_UDS_SWI_TRC, MOD_SDS_UDS, "Sending first time Request APP Diag");
		(void)SendMessage(msgAppDiagDiagnoseRequest, &_SDS_UDS_stRequestPacketFromVas);
	}
#ifndef USE_DEBUGGING_WITH_TRACE_ID
	DEBUG_Printf(UHV_DEBUG_3, UHV_SDS_UDS, "Ret val Fn1_2F:%d", retVal);
#endif
	return (retVal);
}

/*----------------------------------------------------------------------------*/
/* Function    : _SDS_UDS_u8HandleService2F                                                 */
/**
 \brief      handle all identifier for service 2F in the same way

 \param      u16Service2FInputOutputID
 service 2F identifier

 \param      u8InputOutputParameter
 input output control parameter

 \param      u8Service2FControlState1
 control timer

 \param      u8Service2FControlState2
 state value

 \param      u16Service2F_Mask
 mask for active running identifier

 \param      LenOfMsg
 length from VAS tester

 \return     UDScub_FINISHED: Sendedaten liegen vor
 UDScub_REQUEST_OUT_OF_RANGE Fehlercode: wenn ein Parameter ungültig ist.
 UDScub_INCORRECT_MSG_LEN: falsche Länge
 */
/*----------------------------------------------------------------------------*/
static tSDSextWrite_Return _SDS_UDS_u8HandleService2F(
		Uint16Type u16Service2FInputOutputID, Uint8Type u8InputOutputParameter,
		Uint8Type u8Service2FControlState1, Uint8Type u8Service2FControlState2)
	{
	tSDSextWrite_Return retVal = SDS_IO_USER_WRITE_OUT_OF_RANGE;
	_SDS_UDS_u8u8Service2FControlState2 = u8Service2FControlState2;

	if ( ubResponseFromAppTask == 0x0 )
	{
		_SDS_UDS_stResponsePacketToVas.result = 0xFF;
	}
	switch (u8InputOutputParameter)
	{
	case SDS_UDS_SERVICE_2F_RETURN_CONTROL_TO_ECU:
	{
		if (  ub_RequestPending == 0)
		{
			if ( _SDS_UDS_u8Service2F_ActuatorTestStatus == ACTUATOR_TEST_FINISHED )
			{
				retVal = SDS_IO_USER_WRITE_REQUEST_SEQUENCE_ERROR;
			}
			else
			{
				DEBUG_TEXT(SDS_UDS_SWI_TRC, MOD_SDS_UDS, "Stop Stellglied Test");
				_SDS_UDS_u16TimerChannelIOControlActuatorTest = 0;
				_SDS_UDS_sfStoreTimerIOControlActuatorTest((Uint8Type)0x0);
				_SDS_UDS_u8Service2F_ActuatorTestStatus = ACTUATOR_TEST_FINISHED;
		        // Stellgliedtest Ausschalten via Event an APP-DIAG senden
		        retVal = _SDS_UDS_vService2F_CallApp_Diag(u16Service2FInputOutputID,SERVICE_2F_FKT_STOP_LAST_STATE);
		        _SDS_UDS_u16TimerChannelStellgliedFinised = 100;
			}
		}
		else
		{
			if ( _SDS_UDS_u8Service2F_ActuatorTestStatus == ACTUATOR_TEST_RUNNING )
			{
				; // keep sending Request pending.
			}
			else
			{
				// Wiederholen weil kein gültige Antwort vom APP_DIAG.
				retVal = _SDS_UDS_vService2F_CallApp_Diag(u16Service2FInputOutputID,SERVICE_2F_FKT_STOP_LAST_STATE);
			}
		}
	}
		break;

	case SDS_UDS_SERVICE_2F_FREEZE_CURRENT_STATE:
	{
		if ( APP_CAN_enAudioMute == APP_CAN_enGetMuteStatus())
		{
			retVal = (tSDSextWrite_Return)UDScub_CONDITIONS_NOT_CORRECT;
		}
		else
		{
			retVal = SDS_IO_USER_WRITE_SUCCESSFUL;
		}
		break;
	}
	case SDS_UDS_SERVICE_2F_SHORT_TERM_ADJUSTMENT:
	{
		if (  ub_RequestPending == 0)
		{
			if ( _SDS_UDS_u8Service2F_ActuatorTestStatus == ACTUATOR_TEST_RUNNING)
			{
				retVal = SDS_IO_USER_WRITE_REQUEST_SEQUENCE_ERROR;
			}
			else
			{
				_SDS_UDS_sfStoreTimerIOControlActuatorTest(u8Service2FControlState1);
		        _SDS_UDS_u8Service2F_ActuatorTestStatus = ACTUATOR_TEST_RUNNING;
		        // Stellgliedtest Einschalten via Event an APP-DIAG senden
		        if ( u8Service2FControlState2 == 0xFF )
		        {
		        	DEBUG_TEXT(SDS_UDS_SWI_TRC, MOD_SDS_UDS, "Stellglied ON");
		        	retVal = _SDS_UDS_vService2F_CallApp_Diag(u16Service2FInputOutputID,SERVICE_2F_FKT_START_EIN);
		        }
		        else if ( u8Service2FControlState2 == 0x00 )
		        {
		        	DEBUG_TEXT(SDS_UDS_SWI_TRC, MOD_SDS_UDS, "Stellglied OFF");
		        	retVal = _SDS_UDS_vService2F_CallApp_Diag(u16Service2FInputOutputID,SERVICE_2F_FKT_START_AUS);
		        }
			}
		}
		else
		{
			// Wiederholen weil kein gültige Antwort vom APP_DIAG bzw kein Callback aufgerufen von Applikation.
	        if ( u8Service2FControlState2 == 0xFF )
	        {
	        	retVal = _SDS_UDS_vService2F_CallApp_Diag(u16Service2FInputOutputID,SERVICE_2F_FKT_START_EIN);
	        }
	        else if ( u8Service2FControlState2 == 0x00 )
	        {
	        	retVal = _SDS_UDS_vService2F_CallApp_Diag(u16Service2FInputOutputID,SERVICE_2F_FKT_START_AUS);
	        }
		}
		break;
	}
	default:

		DEBUG_VALUE1(SDS_UDS_SWI_TRC, MOD_SDS_UDS, "Service2F: not valid InputOutputParameter: %d",
				u8InputOutputParameter);

		break;
	}
	return retVal;
}


/*----------------------------------------------------------------------------*/
/* Function    : UDS_ubDiagService2F                                                 */
/**
 \brief      UDS Service input outout control by Identifier

 \brief      Description:
 Dieser UDS-Service wird dazu verwendet, um einen bestimmten Sollwert an einen Systemeingang
 oder an eine interne System-Funktion zu übergeben (Übergabe von Prozessdaten). Auch das
 gezielte (selektive) Ansteuern von externen/internen Stellgliedern ist mit diesem UDS-Service möglich.

 \param      RxTxBuf
 data include ID from VAS tester

 \param      LenOfMsg
 length from VAS tester

 \param      Mode
 Typ des funktionalen Requests: 2=UDS, 3=OBD  =>  NOT USED !!!

 \return     UDScub_FINISHED: Sendedaten liegen vor
 UDScub_REQUEST_OUT_OF_RANGE Fehlercode: wenn ein Parameter ungültig ist.
 UDScub_INCORRECT_MSG_LEN: falsche Länge
 */
/*----------------------------------------------------------------------------*/
Uint8Type UDS_ubDiagService2F(Uint8Type* RxTxBuf, Uint16Type* LenOfMsg,
		Uint8Type Mode)
{
	Uint8Type i;
	Uint8Type retval = UDScub_FINISHED;
	Uint16Type u16ReceivedID = ((Uint16Type)RxTxBuf[0] << 8) + RxTxBuf[1];
	Uint8Type lower, upper;
	BooleanType found = BT_FALSE;
	Uint16Type u16ReceivedLen = LenOfMsg[0]; // "-2" weil die CAN-Nutzdaten auch den Identifier enthalten
	tSDSextWrite_Return eStellgliedInProgress;
	lower=0;
	i = 0;

#ifndef USE_DEBUGGING_WITH_TRACE_ID
	DEBUG_Printf(UHV_DEBUG_3, UHV_SDS_UDS,
			"Service2F: ID = %04X called, len = %d", u16ReceivedID, LenOfMsg[0]);
#endif

	upper=(sizeof(cs_StellgliedDataIdItem)/sizeof(t_StellgliedDataIdCfgItem))-1;
	/* lower,i = first DID; upper = last DID */
	// die Überprüfung auf '-1' soll mögliche Fehlerquellen beseitigen,z.B. cs_StellgliedDataIdItem = 0.
	if (upper == 0xFF) // 0xFF = -1
		upper = 0;

	while (i < upper) /* stop searching if border reached */
	{
		/* get next interval */
		if (u16ReceivedID < cs_StellgliedDataIdItem[i].DataId)
		{
			upper = i; /* update upper border */
		}
		else
		{
			lower = i; /* update lower border */
		}
		i = (upper-lower); /* calculate the middle */
		i = lower + i/2 + i%2; /* of lower and upper border */
		if (cs_StellgliedDataIdItem[i].DataId == u16ReceivedID)
		{
			found = BT_TRUE; /* data identifier found */
			break; /* stop searching */
		}
		if (i == lower) /* check for termination condition */
		{
			if (u16ReceivedID == cs_StellgliedDataIdItem[i+1].DataId)
			{
				found = BT_TRUE; /* data identifier found */
				i++;
			}
			break; /* stop searching */
		}
		if (i == upper) /* check for termination condition */
		{
			if (u16ReceivedID == cs_StellgliedDataIdItem[i-1].DataId)
			{
				found = BT_TRUE; /* data identifier found */
				i--;
			}
			break; /* stop searching */
		}
	}

	if (found == BT_TRUE) /* insert data into the buffer if Data ID found */
	{
#ifndef USE_DEBUGGING_WITH_TRACE_ID
		(void)DEBUG_Printf(UHV_DEBUG_2, UHV_SDS_UDS,
				"write: found value = %d for %04X = %04X", i,
				cs_StellgliedDataIdItem[i].DataId, u16ReceivedID);
#endif


		if ( (u16ReceivedLen) <= uwBufSize) // "+2" weil hier der Identifier wieder mit dazu zählt
		{
			// hier erfolgt jetzt eine Überprüfung der Länge:
			if (_SDS_UDS_bWriteDiagnosisLengthCheck_2F(&u16ReceivedLen,
					&cs_StellgliedDataIdItem[i], RxTxBuf[2]) == BT_FALSE)
			{
				return UDScub_INCORRECT_MSG_LEN;
			}

			eStellgliedInProgress
			= _SDS_UDS_u8HandleService2F(u16ReceivedID, RxTxBuf[2], RxTxBuf[3], RxTxBuf[4]);
			if (ub_RequestPending > 0)
			{
				return UDScub_IN_PROCESS;
			}
			switch (eStellgliedInProgress)
			{
				case SDS_IO_USER_WRITE_SUCCESSFUL:
				{
					*LenOfMsg = u16ReceivedLen;
					retval = UDScub_FINISHED;
					break;
				}
				case SDS_IO_USER_WRITE_FAILED:
				{
					retval = SDS_UDS_GENERAL_PROGRAMMING_FAILURE;
					break;
				}
				case SDS_IO_USER_WRITE_REQUEST_SEQUENCE_ERROR:
				{
					retval = SDS_UDS_REQUEST_SEQUENCE_ERROR;
					break;
				}
				case SDS_IO_USER_WRITE_OUT_OF_RANGE:
				{
					retval = UDScub_REQUEST_OUT_OF_RANGE;
					break;
				}
				case SDS_IO_USER_WRITE_NOT_ALLOWED:
				{
					retval = UDScub_ACCESS_DENIED;
					break;
				}
				case UDScub_CONDITIONS_NOT_CORRECT:
				{
					retval = UDScub_CONDITIONS_NOT_CORRECT;
					break;
				}
				default:
				{
					retval = UDScub_CONDITIONS_NOT_CORRECT;
					break;
				}
			}
		}
		else
		{
			retval = UDScub_REQUEST_OUT_OF_RANGE;
		}
	}
	else
	{
		retval = UDScub_REQUEST_OUT_OF_RANGE;

		DEBUG_VALUE2(SDS_UDS_SWI_TRC, MOD_SDS_UDS, "Service2F: value not found!!! %04X instead of %04X",
				cs_StellgliedDataIdItem[i].DataId, u16ReceivedID);

	}
	return (retval);
}

/*----------------------------------------------------------------------------*/
/* Function    : _SDS_UDS_sfStoreTimerIOControlActuatorTest                                                 */
/**
 \brief      internal function to handle the timer value for service 2F

 \param      Timer
 Timer value

 \return     SUCCESS : timer value okay
 FAIL    : timer value out of range

 */
/*----------------------------------------------------------------------------*/
static SuccessFailType _SDS_UDS_sfStoreTimerIOControlActuatorTest(Uint8Type Timer)
{
	if (Timer <= SDS_UDS_SERVICE_2F_TIMER)
	{
		_SDS_UDS_u16TimerIOControlActuatorTest = Timer * 100; // pro 1 Sekunden werden 100 x 10ms Ticker benötigt
	}
	else if (Timer == SDS_UDS_SERVICE_2F_INFINITE_TIMING_NO_TIMING_DEFINED)
	{
		// Timer löschen, falls er noch läuft, damit er nicht ablaufen kann, d.h. läuft ewig...
		_SDS_UDS_u16TimerIOControlActuatorTest = 0;
	}
	else
	{
		// Timer löschen, falls er noch läuft
		_SDS_UDS_u16TimerIOControlActuatorTest = 0;
		return FAIL;
	}
#ifndef USE_DEBUGGING_WITH_TRACE_ID
	(void)DEBUG_Printf(UHV_DEBUG_2, UHV_SDS_UDS,
			"_SDS_UDS_sfStoreTimerIOControlActuatorTest = %X", Timer);
#endif
	return SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* Function    : _SDS_UDS_bWriteDiagnosisLengthCheck                                                 */
/**
 \brief      Write diagnosis length check

 \brief      Description:
 this function check the length send by the VAS tester

 \param      u16ReceivedLen
 length from VAS tester

 \param      DataIdItem
 ID struct

 \return     BT_TRUE: length is okay
 BT_FALSE: length is not okay
 */
/*----------------------------------------------------------------------------*/
static BooleanType _SDS_UDS_bWriteDiagnosisLengthCheck_2F(
		Uint16Type *ptru16ReceivedLen, const t_StellgliedDataIdCfgItem* DataIdItem, Uint8Type u8ControlParameter)
{
	BooleanType bRetVal = BT_TRUE;
	if (u8ControlParameter == SDS_UDS_SERVICE_2F_RETURN_CONTROL_TO_ECU)
	{
		if (*ptru16ReceivedLen == DataIdItem->MinLen)
		{
			bRetVal = BT_TRUE;
#ifndef USE_DEBUGGING_WITH_TRACE_ID
			DEBUG_Printf(
					UHV_DEBUG_2,
					UHV_SDS_UDS,
					"Write: Length Check for ID = %04X okay, received length = %d, corr length = %d",
					DataIdItem->DataId, *ptru16ReceivedLen, DataIdItem->MinLen);
#endif
			*ptru16ReceivedLen = DataIdItem->MinLen;
		}
		else
		{
			bRetVal = BT_FALSE;

			/*DEBUG_VALUE4(SDS_UDS_SWI_ERR, MOD_SDS_UDS, "Write: Length Check for ID = %04X not okay, received length = %d,
					DataIdItem->DataId, *ptru16ReceivedLen);*/
			DEBUG_VALUE1(SDS_UDS_SWI_ERR, MOD_SDS_UDS, "corr length = %d",
					DataIdItem->MinLen);


		}
	}
	else if (u8ControlParameter == SDS_UDS_SERVICE_2F_SHORT_TERM_ADJUSTMENT)
	{
		if (*ptru16ReceivedLen == DataIdItem->MaxLen)
		{
			bRetVal = BT_TRUE;
#ifndef USE_DEBUGGING_WITH_TRACE_ID
			DEBUG_Printf(
					UHV_DEBUG_2,
					UHV_SDS_UDS,
					"Write: Length Check for ID = %04X okay, received length = %d, corr length = %d",
					DataIdItem->DataId, *ptru16ReceivedLen, DataIdItem->MaxLen);
#endif
			*ptru16ReceivedLen = DataIdItem->MaxLen;
		}
		else
		{
			bRetVal = BT_FALSE;
#ifndef USE_DEBUGGING_WITH_TRACE_ID
			DEBUG_Printf(
				UHV_ERROR,
				UHV_SDS_UDS,
				"Write: Length Check for ID = %04X not okay, received length = %d, corr length = %d",
				DataIdItem->DataId, *ptru16ReceivedLen, DataIdItem->MaxLen);
#endif
		}
	}
	else
	{
		bRetVal = BT_TRUE;
	}
	return (bRetVal);
}

/*----------------------------------------------------------------------------*/
/* Function    : _SDS_UDS_bWriteDiagnosisLengthCheck                                                 */
/**
 \brief      Write diagnosis length check

 \brief      Description:
 this function check the length send by the VAS tester

 \param      u16ReceivedLen
 length from VAS tester

 \param      DataIdItem
 ID struct

 \return     BT_TRUE: length is okay
 BT_FALSE: length is not okay
 */
/*----------------------------------------------------------------------------*/

void SDS_UDS_vTimerHandlingStellgliedTests(void)
{
	tSDSextWrite_Return retVal = SDS_IO_USER_WRITE_SUCCESSFUL;
	// Timer Handling for IO Control Actuator Test  (Stellgliedtest)
	if (_SDS_UDS_u16TimerIOControlActuatorTest > 0)
	{
		_SDS_UDS_u16TimerIOControlActuatorTest--;
		if (_SDS_UDS_u16TimerIOControlActuatorTest == 0)
		{ // Timer läuft gerade ab
			if (_SDS_UDS_u8Service2F_ActuatorTestStatus == ACTUATOR_TEST_RUNNING)
			{
				DEBUG_TEXT(SDS_UDS_SWI_TRC, MOD_SDS_UDS, "Service2F: Timer finished");
				// ein Stellgliedtest läuft tatsächlich ;-) => also stoppen, bzw. ausschalten
				_SDS_UDS_u8Service2F_ActuatorTestStatus
						= ACTUATOR_TEST_FINISHED;
				_SDS_UDS_vService2F_CallApp_Diag(
						_SDS_UDS_u16Service2FactivInputOutputID,
						SERVICE_2F_FKT_STOP_LAST_STATE);
				_SDS_UDS_u16TimerChannelStellgliedFinised = 100;
			}
		}
	}


	// Timer Channel Handling for IO Control Actuator Test
	if (_SDS_UDS_u16TimerChannelIOControlActuatorTest > 0)
	{
		_SDS_UDS_u16TimerChannelIOControlActuatorTest--;
		if (_SDS_UDS_u16TimerChannelIOControlActuatorTest == 0)
		{ // Timer läuft gerade ab
			// Channel toggeln oder Frequenz toggeln oder Voice Prompt erneut ausgeben
			DEBUG_TEXT(SDS_UDS_SWI_DBG, MOD_SDS_UDS, "Service2F: Channel Timer finished");
			if ( (UDScub_DEFAULT_SESSION == (Uint8Type)UDS_ubGetSession()))
			{
				DEBUG_TEXT(SDS_UDS_SWI_TRC, MOD_SDS_UDS, "Stop Stellglied Test");
				_SDS_UDS_u8Service2F_ActuatorTestStatus = ACTUATOR_TEST_FINISHED;
		        // Stellgliedtest Ausschalten via Event an APP-DIAG senden
		        retVal = _SDS_UDS_vService2F_CallApp_Diag(_SDS_UDS_u16Service2FactivInputOutputID,SERVICE_2F_FKT_STOP_LAST_STATE);
				if ( ub_RequestPending > 0 )
				{
					_SDS_UDS_u16TimerChannelIOControlActuatorTest = 1; //Try again after 10 ms.
				}
		        _SDS_UDS_u16TimerChannelStellgliedFinised = 100; // so that the MWB will be reset to ACTUATOR_TEST_NOT_RUNNING
			}
			else
			{
				if ( _SDS_UDS_u8u8Service2FControlState2 == 0xFF )
				{
					retVal = _SDS_UDS_vService2F_CallApp_Diag(_SDS_UDS_u16Service2FactivInputOutputID,SERVICE_2F_FKT_START_EIN);
				}
				else if ( _SDS_UDS_u8u8Service2FControlState2 == 0x00 )
				{
					retVal = _SDS_UDS_vService2F_CallApp_Diag(_SDS_UDS_u16Service2FactivInputOutputID,SERVICE_2F_FKT_START_AUS);
				}
				if ( ub_RequestPending > 0 )
				{
					_SDS_UDS_u16TimerChannelIOControlActuatorTest = 1; //Try again after 10 ms.
				}
				else if ( retVal == SDS_IO_USER_WRITE_FAILED )
				{
					_SDS_UDS_u16TimerChannelIOControlActuatorTest = 0;
					_SDS_UDS_u8Service2F_ActuatorTestStatus = ACTUATOR_TEST_ABORTED;
				}
			}
		}
	}

	if ((_SDS_UDS_u16TimerChannelStellgliedFinised > 0) && ( _SDS_UDS_u8Service2F_ActuatorTestStatus == ACTUATOR_TEST_FINISHED))
	{
		_SDS_UDS_u16TimerChannelStellgliedFinised--;
		if ( _SDS_UDS_u16TimerChannelStellgliedFinised == 0 )
		{
			DEBUG_TEXT(SDS_UDS_SWI_TRC, MOD_SDS_UDS, "Reset Actuator status to 0x0");
			_SDS_UDS_u8Service2F_ActuatorTestStatus = ACTUATOR_TEST_NOT_RUNNING;
		}
	}

}

/*----------------------------------------------------------------------------*/
/* Function    : SDS_UDS_u8GetStatusActuatorTest                                                 */
/**
 \brief      Get the status of Actuator test

 \brief      Description:
 to return the status of Acutator test for Service 0x22

 \param      void

 \return     Uint8Type

 */
/*----------------------------------------------------------------------------*/

Uint8Type SDS_UDS_u8GetStatusActuatorTest(void)
{
	return _SDS_UDS_u8Service2F_ActuatorTestStatus;
}


/* End Of File SDS_UDS_Service2F_C1.c */
