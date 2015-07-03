/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/
/** 
 * \file 	SDS_UDS_Service22_C1.c
 * \brief	This file contains the program to xxxx
 * \brief	This file fulfills the following requirements:
 * 			1. Requirement 1
 * 			2. Requirement 2
 * 			3. Requirement x
*/
/*-----------------------------------------------------------------------------
* $Date: 2010-06-08 10:30:12 +0200 (Di, 08 Jun 2010) $
* $Rev: 13479 $
* $Author: weber $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_SDS_UDS/trunk/src/SDS_UDS_Grundeinstellung_C1.c $
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
#include "DEBUG.h"
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
#include "EEPROM_LAYOUT_CFG.h"
#include "TRACE.h"
/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "SDS_ext.h"
#include "SDS_UDS_CI.h"
#include "SDS_UDS_ServiceGrundeinstellung_C1.id"

/*----------------------------------------------------------------------------*/
/* pragma Directives                                                          */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Definitions (avoid as much as possible!)                  */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private Types                                                              */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private Manifest Constant Definitions                                      */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Prototypes of file local Methods                                           */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* File local Code Macros                                                        */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the macros
*/

/*----------------------------------------------------------------------------*/
/* File local Variables                                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* File local const Definitions                                               */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private Address Calculations                                               */
/*----------------------------------------------------------------------------*/






/*----------------------------------------------------------------------------*/
/* Function    : SDSext_DIAG_vHandleEvents                                         		  */
/**
 \brief      Handle Events from App Diagnose.

 \brief      Description
 Handle events send by the Diagnose

 \return     SuccessFailType
 If the function was successful or failed
 SUCCESS, FAIL
 */
/*----------------------------------------------------------------------------*/
SuccessFailType SDSext_DIAG_vHandleEvents()
{
	static Uint16Type _SDS_UDS_u16BusyRetryCounter = 0;
	if (event & evHostTaskDiagnoseResponse)
	{
		(void)ClearEvent(evHostTaskDiagnoseResponse);
		// check for new message from Diagnose.
		(void)ReceiveMessage(msgAppDiagDiagnoseResponse,
				&_SDS_UDS_stResponsePacketToVas);


		if ( _SDS_UDS_stResponsePacketToVas.Identifier == ID_31_0317_RESET_TO_FACTORY_SETTING)
		{
			switch ( _SDS_UDS_stResponsePacketToVas.result )
			{
				case enAppDiagResponseWaitForCallBack:
				{
					_SDS_UDS_u8Service31_BasicSettingsStatus = BASIC_SETTINGS_RUNNING;
					break;
				}
				case enAppDiagResponseErrorBusyImm:
				{
					if ( _SDS_UDS_u16BusyRetryCounter == 2000 ) // 20 seconds
					{
						DEBUG_Printf(UHV_TRACE, UHV_SDS_UDS, "Retrying Failed R2FS");
						_SDS_UDS_u8Service31_BasicSettingsStatus = BASIC_SETTINGS_ABORTED;
						_SDS_UDS_u16TimerBasicSettings = 500;
						_SDS_UDS_u16BusyRetryCounter = 0;
						(void)CancelAlarm(CTA_HOST_R2FS_DIAG_RETRY);
					}
					else  // retry again
					{
						DEBUG_Printf(UHV_TRACE, UHV_SDS_UDS,
								"R2FS Retry");
						_SDS_UDS_u16BusyRetryCounter++;
						_SDS_UDS_u8Service31_BasicSettingsStatus = BASIC_SETTINGS_RUNNING;
						(void)SetRelAlarm(CTA_HOST_R2FS_DIAG_RETRY, MSEC(10), MSEC(0));
					}
					break;
				}
				case enAppDiagResponseErrorImm:
				{
					DEBUG_Printf(UHV_TRACE, UHV_SDS_UDS, "Aborting Reset To Factory");
					_SDS_UDS_u8Service31_BasicSettingsStatus = BASIC_SETTINGS_ABORTED;
					_SDS_UDS_u16TimerBasicSettings = 500;
					_SDS_UDS_u16BusyRetryCounter = 0;
					(void)CancelAlarm(CTA_HOST_R2FS_DIAG_RETRY);
					break;
				}
				case enAppDiagResponseOkImm:
				{
					_SDS_UDS_u8Service31_BasicSettingsStatus = BASIC_SETTINGS_FINISHED;
					_SDS_UDS_u16TimerBasicSettings = 500;
					_SDS_UDS_u16BusyRetryCounter = 0;
					(void)CancelAlarm(CTA_HOST_R2FS_DIAG_RETRY);
					break;
				}
				default:
				{
					break;
				}
			}
		}
		ubResponseFromAppTask = 1;
		DEBUG_Printf(UHV_DEBUG_1, UHV_SDS_UDS, "Msg frm ADiag = %d", _SDS_UDS_stResponsePacketToVas.result);
	}
	return SUCCESS;
}


ALARMCALLBACK(SDS_UDS_RetryMechanismR2FS)
{
	_SDS_UDS_stRequestPacketFromVas.Identifier= ID_31_0317_RESET_TO_FACTORY_SETTING;
	_SDS_UDS_stRequestPacketFromVas.length = 1;
	_SDS_UDS_stRequestPacketFromVas.data[0] = RESET_TO_FACTORY_SETTING_ON;
	(void)SendMessage(msgAppDiagDiagnoseRequest,
			&_SDS_UDS_stRequestPacketFromVas);
}

/*----------------------------------------------------------------------------*/
/* Function    : _SDS_UDS_bResetOfAdaptionValuesStatus                                                 */
/**
 \brief      internal function to return the state of Basic settings"

 \param      None

 \return     _SDS_UDS_bResetOfAdaptionValuesState
 state of basic settings

 */
/*----------------------------------------------------------------------------*/
BooleanType _SDS_UDS_bResetOfAdaptionValuesStatus(void)
{
	return _SDS_UDS_bResetOfAdaptionValuesState;
}

/*----------------------------------------------------------------------------*/
/* Function    : _SDS_UDS_vResetOfAdaptionValues                                                 */
/**
 \brief      internal function to control the functionality "reser to factory"

 \param      start
 start or stop function

 \param      BasicSetting
 BasicSetting (Werkseinstellung) or only $0905 (Anpassung)

 \return     None

 */
/*----------------------------------------------------------------------------*/
void _SDS_UDS_vResetOfAdaptionValues(BooleanType start,	BooleanType BasicSetting)
{
	if (start == BT_TRUE)
	{
		if (BasicSetting == BT_TRUE)
		{
			_SDS_UDS_bResetOfAdaptionValuesState = BT_TRUE;
			_SDS_UDS_u8Service31_BasicSettingsStatus = BASIC_SETTINGS_RUNNING;

		}
		DEBUG_Printf(UHV_TRACE, UHV_SDS_UDS,
				"Werkseinstellung: Step: reset to factory");
		_SDS_UDS_stRequestPacketFromVas.Identifier
		= ID_31_0317_RESET_TO_FACTORY_SETTING;
		_SDS_UDS_stRequestPacketFromVas.length = 1;
		_SDS_UDS_stRequestPacketFromVas.data[0] = RESET_TO_FACTORY_SETTING_ON;
		(void)SendMessage(msgAppDiagDiagnoseRequest,
				&_SDS_UDS_stRequestPacketFromVas);
	}
	else
	{
		_SDS_UDS_u8Service31_BasicSettingsStatus = BASIC_SETTINGS_NOT_RUNNING;
		_SDS_UDS_u16TimerBasicSettings = 0;
	}
}

/*----------------------------------------------------------------------------*/
/* Function    : _SDS_UDS_u8GetStatusBasicSettings                                                 */
/**
 \brief      Get the status of Basic Settings in Routine Control

 \brief      Description:
 Get the status of Basic Settings in Routine Control

 \param      void

 \return     Uint8Type

 */
/*----------------------------------------------------------------------------*/

Uint8Type _SDS_UDS_u8GetStatusBasicSettings(void)
{
	return _SDS_UDS_u8Service31_BasicSettingsStatus;
}



/*----------------------------------------------------------------------------*/
/* Function    : SDS_UDS_vInitialiseGrundeinstellung                                       */
/**
 \brief      To reset the Static variables

 \brief      Description:
 this function resets the static variables.


 \return     void
 */
/*----------------------------------------------------------------------------*/
void SDS_UDS_vInitialiseGrundeinstellung(void)
{
	eReadInProgress = SDSext_IO_USER_READ_SUCCESSFUL;
}




/* End Of File SDS_UDS_Grundeinstellung_C1.c */
