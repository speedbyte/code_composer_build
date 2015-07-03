/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/
/**
 * \file 	sds_ext.c
 * \brief	This file contains the program to xxxx
 * \brief	This file fulfills the following requirements:
 * 			1. Requirement 1
 * 			2. Requirement 2
 * 			3. Requirement x
 */
/*-----------------------------------------------------------------------------
 * $Date: 2011-09-15 17:19:23 +0200 (Do, 15 Sep 2011) $
 * $Rev: 21758 $
 * $Author: butz $
 * $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_SDS_UDS/trunk/src/sds_ext.c $
 *-----------------------------------------------------------------------------
 */
/**************************************************************************
 *  COPYRIGHT (C) Volkswagen AG, 2006-2007                                 *
 *  ALLE RECHTE VORBEHALTEN. ALL RIGHTS RESERVED.                         *
 *                                                                        *
 **************************************************************************
 *  FILE: SDS.C
 **************************************************************************
 *  PURPOSE: Standard implementation of UDS services
 *  $10  DiagnosticSessionControl
 *  $11  EcuReset (hardware independent part only)
 *  $14  ClearDiagnosticInformation
 *  $19  ReadDTCInformation
 *  $22  ReadDataByIdentifier
 *  $28  CommunicationControl
 *  $3E  TesterPresent
 *  $85  ControlDTCSetting
 *
 *************************************************************************
 *  CHANGE HISTORY: In2Soft GmbH
 *
 *  Version    Name     Date   Description
 **************************************************************************
 *  V_00_01_00 Ki 12.10.2006 - first version
 *  V_00_01_01 Ki 02.11.2006 - new function: SDS_GetVersionInfo
 *  V_00_01_02 Ki 12.12.2006 - compatibility to uds 01.01.02
 *                           - DEH_ControlDTCSettings --> DEH_ControlDTCSetting
 *                           - i2s_type.h removed
 *                           - Process return value DEH_E_NO_DATA
 *                           - bug-fix: 3 byte instead of 4 byte DTC
 *                           - new: DTCStatusAvailabilityMask
 *  V_00_01_03 Ki 19.12.2006 - TRUE --> I2S_TRUE, FALSE --> I2S_FALSE
 *                           - set define _I2S_SDS_C_ for memory allocation´
 *                             in sds_cfg.h
 *  V_00_01_04 Ki 10.01.2007 - Service $10: timing parameter in response added
 *                           - Service $14: no DTC in response
 *                           - Service $22: up to 8 data IDs within on request
 *                           - Service $3E: suppress bit handling added
 *                           - uds_cfg.h included (because of P2 timing)
 *                           - buffer size init: one less for SID
 *  V_00_01_05 Ki 12.01.2007 - Service $22: number of DIDs 8 for physical
 *                             and 3 for functional addressed requests
 *  V_00_01_06 Ki 02.03.2007 - general revision, review by VW
 *  V_01_00_00 Ki 30.03.2007 - CommunicationAndNetworkType_1 and _2 deleted
 *                           - SDS_28_DisableRxAndEnableTx and SDS_28_DisableRxAndTx
 *                             deleted, SDS_28_GatewayVehicleWakeUp supported
 *                           - Negative response codes revised
 *  V_01_00_01 Ki 11.04.2007 - Calls of SDSext_ubZeroVehicleOrEngineSpeed() after check
 *                             for valid request parameters
 *                           - SDS_USAGE deleted
 *                           - Check for valid values of service $28 before DEH call
 *                           - $7F,10,7E for $10,02 and $10,4F while in default session
 *                           - no byte cast for tDEH_Return anymore
 *                           - UDScub_SUBFUNCTION_NOT_SUPPORTED for unknown $10,xx
 *                           - Call of SDSext_vSetProgRequest() at $10,02
 *                           - UDScub_SUBFUNCTION_NOT_SUPPORTED for 3E,XX != 3E,00
 *  V_01_00_02 Ki 24.04.2007 - SDS_11_SoftReset returns UDScub_SUBFUNCTION_NOT_SUPPORTED
 *                           - Service $14 responds positive on DEH_E_CLEAR
 *                           - new: SDSext_vResetSecurityAccessEffected
 *  V_01_00_03 Ki 02.07.2007 - Service $22: UDScub_REQUEST_OUT_OF_RANGE if no DID not found
 *                           - Service $22: linear --> binary search
 *                           - At all DEH calls retVal=DEH_E_INIT_MEMORY applied
 *  V_01_00_04 Ki 11.07.2007 - Service $22: bug fix for index i at interval end
 *  V_01_00_05 Ki 23.07.2007 - All DEH function calls with type-casts
 *  V_01_00_06 Ki 02.11.2007 - 10,02 optional --> #define SDS_ECU_PROGRAMABLE
 *  V_01_00_07 Ki 04.02.2008 - Buffer size check for UDS_ubDiagService19 (19,02)
 *  V_01_00_08 Ki 18.06.2008 - ReturnValue handling for DEH_SetDiagnosticSession fixed
 *                           - DEH_E_INIT_MEMORY --> DEH_E_INIT in service $10, $14, $19, $85
 *                           - Process ReportEmissionRelatedOBDDTCByStatusMask and
 *                             AllOBDExtendedDataRecordNumbers if DTCStatusAvailabilityMask=OBD
 *                           - Module ID = 0x15
 *                           - Service $22: REQUEST_OUT_OF_RANGE if no of DIDs >8 resp. >3
 *                           - byte SDSext_ubComCtrl() --> void SDSext_vComCtrl()
 *                           - call of SDSext_vComCtrl() on each session change
 *  V_01_00_09 Ki 24.06.2008 - return values for $22 modified: DID not found, len > x
 *  V_01_00_10 Ki 23.07.2008 - return values for SDSext_ubZeroVehicleOrEngineSpeed modified
 *                           - SDS_85_ZeroVehicleOrEngineSpeed used also in $10, $28
 *  V_01_01_00 Ki 08.08.2008 - SDS_10_ZeroVehicleOrEngineSpeed used in $10
 *                           - SDS_28_ZeroVehicleOrEngineSpeed used in $28
 *                           - SDS_85_ZeroVehicleOrEngineSpeed used in $85
 *                           - session check in $10 separated by SDS_ECU_PROGRAMABLE
 *                           - void SDSext_vComCtrl() --> byte SDSext_ubComCtrl()
 *                           - parameter of SDSext_ubComCtrl() in $10 extended
 *                           - send NRC=$31 if DID not supported at all
 *************************************************************************/

#define  _I2S_SDS_C_

#include <string.h>
#include "sds.h"
#include "uds.h"
#include "sds_cfg.h"
#include "deh.h"

#include "SDS_UDS_CI.h"

#define MODULE_ID                          (byte)0x15  /* Std-SW module ID */
#define MODULE_MAJOR_VERSION               (byte)0x01  /* module version */
#define MODULE_MINOR_VERSION               (byte)0x01  /* module version */
#define MODULE_PATCH_VERSION               (byte)0x00  /* module version */

/* Configuration checks: valid number of clients? */
#if( SDS_UDS_CLIENT_ID > 0 )
#if( SDS_UDS_CLIENT_ID >= UDS_CLIENTS )
#error "Client ID for SDS must be lower than number of UDS clients!"
#endif
#endif

#if( UDS_CLIENTS > 1 )
#define i2s_ch SDS_UDS_CLIENT_ID
#endif

/*************************************************************************
 * LOCALS
 *************************************************************************/
#if( SDS_A_READ_DATA_BY_IDENTIFIER == SDSext_ENABLED )
static tSDSextRead_Return CopyData(const t_ReadDataIdCfgItem* DataIdItem, byte* data);
tSDSextRead_Return eReadInProgress;
#endif

/*************************************************************************
 * VARIABLES
 *************************************************************************/
#if( SDS_A_RESET_OR_DISCONNECT == SDS_ENABLED )
static byte ubResetControl;     /* process hard, soft reset */
#endif
word uwBufSize;
static TriStateType_DEH_EE_Waiting_Time WaitingTimeState  = DEH_EE_WAIT_TIME_IDLE;

/* ----------------------------------------------------------------------- */
/*  - F u n c t i o n s of the module ------------------------------------ */
/* ----------------------------------------------------------------------- */


/*  FUNCTION SDS_vGetVersionInfo( I2S_VersionInfoType *versioninfo )
 **************************************************************************
 *  PURPOSE:        Output the module verson and vendor information
 **************************************************************************
 *  PARAMETERS:     address of the structure to be written
 **************************************************************************
 *  RETURN VALUE:   vendor id, module id, module version
 *************************************************************************/
#if( SDS_ECU_PROGRAMABLE == 0 )

void SDS_vGetVersionInfo( I2S_VersionInfoType *versioninfo )
{
  versioninfo->vendorID = IN2SOFT_VENDOR_ID;
  versioninfo->moduleID = MODULE_ID;
  versioninfo->sw_major_version = MODULE_MAJOR_VERSION;
  versioninfo->sw_minor_version = MODULE_MINOR_VERSION;
  versioninfo->sw_patch_version = MODULE_PATCH_VERSION;
}
#endif


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
#include "tp.h"
#include "VCR.h"
#include "sds_ext.h"
#include "ISRMask_CB_cfg.h"
#include "APP_DIAG.h"
#include "can_def.h"
#include "can_cfg.h"
#include "crcmgr.h"
#include "deh_dtc_cfg.h"
#include "OSEK_HostTask_H1.h"
#include "CSHDL.h"
#include "EEPROM_LAYOUT_CFG.h"
#include "TRACE.h"
#include "sds_ext.id"
/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* pragma Directives                                                          */
/*----------------------------------------------------------------------------*/

#pragma DATA_SECTION (Set_FlashRequest_Paddern, ".REQUEST")

/*----------------------------------------------------------------------------*/
/* Global Variables Definitions (avoid as much as possible!)                  */
/*----------------------------------------------------------------------------*/
static const Uint8Type au8ResetPosResponse[2] = {0x51, 0x01};
static const Uint8Type au8DefSessionPosResponse[6] = 
{
    0x50, 
    0x01, 
    (Uint8Type)(cuw_P2_MAX_DEF >> 8), 
    (Uint8Type)(cuw_P2_MAX_DEF >> 0), 
    (Uint8Type)((cuw_P2_MAX/10) >> 8), 
    (Uint8Type)((cuw_P2_MAX/10) >> 0)
};
Uint8Type ubSecState = 0; // muss unbedingt nach Systemstart aus dem EEPROM geladen werden.


Uint8Type ubResponseFromAppTask = 0;


#ifdef SDSEXT_SERVICE86_ALLOWED
Uint8Type u8Service86DataValuesInitStatus;
Uint8Type u8Service86DataValues [VWDB_EE_ADDDRESS_Service86ResponseOnEvent_LEN];
Uint8Type u8Service86DataValuesDTC[DEH_MAX_NUMBER_OF_EVENTPATHS*3];
#endif


Uint8Type _SDS_UDS_u8UserProfile;

APP_DIAG_ST_DIAG_RESPONSE _SDS_UDS_stResponsePacketToVas;
APP_DIAG_ST_DIAG_REQUEST _SDS_UDS_stRequestPacketFromVas;

/*----------------------------------------------------------------------------*/
/* Private Manifest Constant Definitions                                      */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private Types                                                              */
/*----------------------------------------------------------------------------*/
/**
 \brief	description of the type
 */
/**
\brief	description of the type
*/


/*----------------------------------------------------------------------------*/
/* Prototypes of file local Methods                                           */
/*----------------------------------------------------------------------------*/
/**
 \brief	description of the method
 */
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
BooleanType _SDS_UDS_bResetOfAdaptionValuesState = BT_FALSE;

#ifdef SDSEXT_SERVICE86_ALLOWED
static Uint8Type u8Service86DataValuesDTClength;
#endif

static Uint8Type ubResetControl; /* process hard, soft reset */
static Uint8Type ubReadInitValue; // zum Auslesen aller Initwerte aus dem EEPROM

// Status for IO Basic Settings Status   0 = OFF, 1 = ON
Uint8Type
		_SDS_UDS_u8Service31_BasicSettingsStatus= BASIC_SETTINGS_NOT_RUNNING;
// Timer for Basic Setting
Uint16Type _SDS_UDS_u16TimerBasicSettings = 0;

Uint32Type Set_FlashRequest_Paddern;


/**
\brief	description of the private variable
*/
Uint8Type u8PhoneNumberLength = 0;

extFingerprint logBlockState = SDS_UDS_PROG_DATE;
// $F181: Identifikation der Standardsoftware: VW Standard Software Modul
const Uint8Type
		VERSION_VALUES[ID_22_F181_IDENTIFIKATION_DER_STANDARDSOFTWARE_MAX_LEN] = {
				0x01, 0x01, 0x04, 0x01, // BAP
				0x02, 0xFF, 0xFF, 0xFF, // UDS-Protocol (Appl. and Impl.)
				0x03, 0xFF, 0xFF, 0xFF, // Diagnostic-Event-Handler
				0x04, 0x04, 0x02, 0x03, // CAN-Driver
				0x05, 0x01, 0x02, 0x0E, // VW-TP2.0
				0x07, 0xFF, 0xFF, 0xFF, // ISO15765-2  (UDS)
				0x09, 0x02, 0x02, 0x00, // OSEK-Operating System
				0x0A, 0x03, 0x07, 0x03, // OSEK-Networkmanagement
				0x0C, 0x02, 0x02, 0x00 // OSEK-Communication
		};
Uint8Type ub_RequestPending22 = 0;


/*----------------------------------------------------------------------------*/
/* File local const Definitions                                               */
/*----------------------------------------------------------------------------*/
/**
 \brief	description of the private constant
 */
#ifdef SDSEXT_SERVICE86_ALLOWED
#define Service86DTCInformationType 0x02
#define Service86DTCStatusMask u8Service86DataValues[Service86EventTypeRecord]
#endif

/**
\brief	description of the private constant
*/

// #define DTC_HACK_MASK                 0x09             // damit nur der DTC Status 0x08 und 0x09 gesendet werden !!!

#define FLASH_REQUEST_PADDERN_INIT 0x00000000UL

#undef  UpLoadFuerEntwicklung       // damit ein Upload für die Entwicklung durchgeführt werden kann
#define EEPromReadWriteViaCAN       // damit ist es möglich beliebige Adressemn im EEPROM zu lesen und zu schreiben OHNE PRÜFUNG !!! => also Vorsicht
// Handhabung: Read: 76B 8 06 3D 01 01 xx xx        xx = Adresse
//            Write: 76B 8 06 3D 00 01 xx xx yy     yy = Data
//                                |  |------- Länge der zu lesenden Bytes
//                                |---------- 1 = Read, 0 = Write

#define csVersionInfo      cs_VersionInfo[i]





// end of Service 2F area



#define MAX_CHANGE_FUNCTIONS     10

#define DTC_Vorhalt              0xFF

/**
\brief	description of the private constant
*/




// Die nachfolgende Struktur wird dazu verwendet, alle Anpassungen (Calibrierung) durchzuführen. Dazu werden der Reihe nach die entsprechenden Applikationsfunktionen aufgerufen
// hier sollten immer alle ID's augeführt werden, die im automaitsch erzeugtem File APP_DIAG_ID_CHART.h unter dem Bereich /*CalibrationData*/ zu finden sind.
// nicht benötigte ID's können ggf. auskommentiert werden.
// Momentan sind das die ID's 0905 und 2422. Dies ist so von VW gefordert. Der ID 0500 wird nur intern verwendet, d.h. es müssen keine Applikationsfunktinen aufgerufen werden.

/*----------------------------------------------------------------------------*/
/* Private Address Calculations                                               */
/*----------------------------------------------------------------------------*/

/***************************************************************************
 *  PURPOSE:        Initialization of the module
 **************************************************************************
 *  PARAMETERS:     bufsize: buffer size
 **************************************************************************
 *  RETURN VALUE:   -
 *************************************************************************/
void SDSext_vInit(Uint16Type bufsize )
{
	uwBufSize = bufsize-1; /* one less for service id */
#if( SDS_A_RESET_OR_DISCONNECT == SDS_ENABLED )
ubResetControl = I2S_FALSE;
#endif
#if( SDS_A_READ_DATA_BY_IDENTIFIER == SDS_ENABLED )
eInProgress = SDS_IO_USER_READ_SUCCESSFUL;
#endif

	ubResetControl = I2S_FALSE;
	ubReadInitValue = 0;
	_SDS_UDS_u8UserProfile = DEFAULT_USER_PROFILES;
	_SDS_UDS_u8ResetToFactory = DEFAULT_RESET_TO_FACTORY_SETTING;
#ifdef SDSEXT_SERVICE86_ALLOWED
	u8Service86DataValuesInitStatus = Service86inactive;
#endif
	SDS_UDS_vInitialiseService22();
	SDS_UDS_vInitialiseService2E();
	SDS_UDS_vInitialiseService2F();
	SDS_UDS_vInitialiseServiceDataDownload();
	SDS_UDS_vInitialiseServiceDTC();
	SDS_UDS_vInitialiseServiceSecurityCheck();
	SDS_UDS_vInitialiseService31();
#ifndef USE_DEBUGGING_WITH_TRACE_ID
	(void)DEBUG_Printf(UHV_TRACE, UHV_SDS_UDS, "Init SDSExt");
#endif
}

/***************************************************************************
 *  PURPOSE:        SDS task to execute backstage operations; e.g. RESET
 **************************************************************************
 *  PARAMETERS:     -
 **************************************************************************
 *  RETURN VALUE:   -
 *************************************************************************/
void SDSext_vTask(void )
{
	int i;
	Uint8Type ubIdle = UDS_ubIsIdle(UDS_CHANNEL_INDEX_2);

#ifdef SDSEXT_SERVICE86_ALLOWED
	//  Uint8Type u8Service86DataValuesDTCnew[DEH_MAX_NUMBER_OF_EVENTPATHS*4];
	tDEH_Return retVal;
	tDEH_DTCAndStatus das;
	Uint16Type index = 0;
	Uint8Type u8actService86DataValuesDTC[DEH_MAX_NUMBER_OF_EVENTPATHS*3+3];
	Uint8Type ubSendCanDTC_Message;
#endif
#if ( SDS_A_RESET_OR_DISCONNECT == SDS_ENABLED )
byte ubIdle = UDS_ubIsIdle(UDS_CHANNEL_INDEX_2);
if( (ubResetControl != I2S_FALSE) && (ubIdle == I2S_TRUE) )
{
  SDSext_vDoReset(ubResetControl);
  ubResetControl = I2S_FALSE;
}
#endif
	if ( ((ubResetControl != I2S_FALSE) && (ubIdle == I2S_TRUE)) 
        || (FLASH_REQUEST_PADDERN_BOOT == Set_FlashRequest_Paddern)
        || (FLASH_REQUEST_PADDERN_DEF_SESSION == Set_FlashRequest_Paddern))
	{
		DEBUG_VALUE1(SDS_UDS_SWI_TRC, MOD_SDS_UDS, "SDSext_vTask: FBL Paddern = %08X", 
                     Set_FlashRequest_Paddern);
		if (FLASH_REQUEST_PADDERN_BOOT == Set_FlashRequest_Paddern)
		{
			Set_FlashRequest_Paddern = FLASH_REQUEST_PADDERN_INIT;
			UDS_ubSendMsg((Uint8Type *)&au8ResetPosResponse[0], 2);
			ubResetControl = SDS_11_HardReset;  // Simulate hard reset
		}
		else if (FLASH_REQUEST_PADDERN_DEF_SESSION == Set_FlashRequest_Paddern)
        {
			Set_FlashRequest_Paddern = FLASH_REQUEST_PADDERN_INIT;
			UDS_ubSendMsg((Uint8Type *)&au8DefSessionPosResponse[0], 6);
			ubResetControl = SDS_11_HardReset;  // Simulate hard reset
        }
		SDSext_vDoReset(ubResetControl);
		ubResetControl = I2S_FALSE;
	}

	if ((ubReadInitValue & MASK_READ_INIT_READY) != MASK_READ_INIT_READY)
	{
#ifndef USE_DEBUGGING_WITH_TRACE_ID
		(void)DEBUG_Printf(UHV_DEBUG_2, UHV_SDS_UDS, "START_UP_SEQ");
#endif
		ubReadInitValue = SDS_UDS_u8ProcessorStartUpDiagnoseSequence(ubReadInitValue);

	}

	// Zufallszahl für SEED
	RandomCounter[0]++;
	RandomCounter[1]--;
	RandomCounter[2]++;
	RandomCounter[3]--;

	// Timer Handling Security access
	if (TimerSecurity > 0)
	{
		TimerSecurity--;
		if (TimerSecurity == 0)
		{ // Timer läuft gerade ab
			ubSecState &= MASK_SEC_STATUS_UNLOCKED; // die Key Sperre wieder aufheben und im EEPROM sichern
			// ubSecState ins EEPROM speichern
			(void)EEPROM_sfRamWriteOnly(&ubSecState, EEPROM_LAYOUT_ENUM_SecurityAccessState);
			/* Bootloader security also has to be updated to 0 */
			SDS_UDS_vSetBootloaderTimeLockState(0);
			DEBUG_TEXT(SDS_UDS_SWI_TRC, MOD_SDS_UDS, "Security Timer finished. So unlock");
		}
	}

	SDS_UDS_vTimerHandlingStellgliedTests();

	// Timer Handling for Basic Setting (Werkseinstellung)

	if ((_SDS_UDS_u16TimerBasicSettings > 0) && (( _SDS_UDS_u8Service31_BasicSettingsStatus == BASIC_SETTINGS_ABORTED)
			|| ( _SDS_UDS_u8Service31_BasicSettingsStatus == BASIC_SETTINGS_FINISHED)))
	{
		_SDS_UDS_u16TimerBasicSettings--;
		if (_SDS_UDS_u16TimerBasicSettings == 0)
		{   // Timer läuft gerade ab
			_SDS_UDS_u8Service31_BasicSettingsStatus = BASIC_SETTINGS_NOT_RUNNING;
#ifndef USE_DEBUGGING_WITH_TRACE_ID
			(void)DEBUG_Printf(UHV_TRACE, UHV_SDS_UDS,
					"Activate Stop");
#endif
		}
	}

#ifdef SDSEXT_SERVICE86_ALLOWED
	if (u8Service86DataValuesInitStatus == Service86init)
	{
		u8Service86DataValuesDTClength = 0;
		do // immer einlesen, damit bei der Aktivierung vom Service $86 vom IST-Zustand aus begonnen werden kann
		{
			retVal = DEH_ReadDTCByStatus( (tDEH_DTCInfoType)Service86DTCInformationType, (tDEH_DTCStatusMask)Service86DTCStatusMask, &das );
			if( (retVal == DEH_OK) || (retVal == DEH_E_DATA_STILL_AVAILABLE) )
			{
				u8Service86DataValuesDTC[u8Service86DataValuesDTClength+0] = (Uint8Type)(das.DTC>>16);
				u8Service86DataValuesDTC[u8Service86DataValuesDTClength+1] = (Uint8Type)(das.DTC>>8);
				u8Service86DataValuesDTC[u8Service86DataValuesDTClength+2] = (Uint8Type)(das.DTC);
				u8Service86DataValuesDTClength += 3; /* sizeof(tDEH_DTCAndStatus); */
				if( (index+3)> (DEH_MAX_NUMBER_OF_EVENTPATHS*3) ) /* check if buffer space available for the next DTC+status */
				{
					retVal = DEH_OK; /* stop collecting */
				}
			}
		}
		while( retVal == DEH_E_DATA_STILL_AVAILABLE );
		u8Service86DataValuesInitStatus = Service86running;
	}
	else if (u8Service86DataValuesInitStatus == Service86running)
	{
		index = 3;
		do
		{
			retVal = DEH_ReadDTCByStatus( (tDEH_DTCInfoType)Service86DTCInformationType, (tDEH_DTCStatusMask)Service86DTCStatusMask, &das );
			if( (retVal == DEH_OK) || (retVal == DEH_E_DATA_STILL_AVAILABLE) )
			{
				u8actService86DataValuesDTC[index+0] = (Uint8Type)(das.DTC>>16);
				u8actService86DataValuesDTC[index+1] = (Uint8Type)(das.DTC>>8);
				u8actService86DataValuesDTC[index+2] = (Uint8Type)(das.DTC);
				index += 3; /* sizeof(tDEH_DTCAndStatus); */
				if( (index+3)> (DEH_MAX_NUMBER_OF_EVENTPATHS*3) ) /* check if buffer space available for the next DTC+status */
				{
					retVal = DEH_OK; /* stop collecting */
				}
			}
		}
		while( retVal == DEH_E_DATA_STILL_AVAILABLE );
	}
	if (index> 3)
	{ // DTC sind vorhanden
		if ((index-3)> u8Service86DataValuesDTClength) // d.h. wenn (index-3) größer ist, dann ist auf jeden Fall mindestens ein DTC zusätzlich aufgetreten.
		{ // und dann kann das aufwendige Überprüfen entfallen
			ubSendCanDTC_Message = TRUE;
		}
		else
		{
			ubSendCanDTC_Message = FALSE;
#if 0
			for (i=3; i<index; i+=3)
			{
				for (j=0; j<u8Service86DataValuesDTClength; j+=3)
				{
					ubSendCanDTC_Message = TRUE; // auf ein Neues...
					if ((u8actService86DataValuesDTC[i+0] == u8Service86DataValuesDTC[j+0]) &&
							(u8actService86DataValuesDTC[i+1] == u8Service86DataValuesDTC[j+1]) &&
							(u8actService86DataValuesDTC[i+2] == u8Service86DataValuesDTC[j+2]))
						ubSendCanDTC_Message = FALSE;
				}
				if (ubSendCanDTC_Message == TRUE)
				{
					j = u8Service86DataValuesDTClength;
					i = index;
					break; // mindestens ein DTC ist neu => also Überprüfung beenden und ab damit
				}
				else
				{
					// und weiter geht's...
				}
			}
#endif
		}
		if (ubSendCanDTC_Message == TRUE)
		{
			for (i=0; i<(index-3); i++)
				u8Service86DataValuesDTC[i] = u8actService86DataValuesDTC[i+3]; // aktuellen DTC Stand merken

			u8Service86DataValuesDTClength = index-3;

			if (u8Service86DataValues[Service86EventType] == Service86EventType_StartResponseOnEvent)
			{
				u8actService86DataValuesDTC[0] = 0x59;
				u8actService86DataValuesDTC[1] = 0x02;
				u8actService86DataValuesDTC[2] = 0x09;

				UDS_ubSendMsg((Uint8Type*) u8actService86DataValuesDTC[0],index);
			}
		}
	}
#endif
}


/***************************************************************************
 *  PURPOSE:        UDS_ubDiagService10
 **************************************************************************
 *  PARAMETERS:     Pointer to the message data
 *                  Pointer to the message length
 *                  RequestType: 1=physical, 2=functional
 **************************************************************************
 *  RETURN VALUE:   UDScub_IN_PROCESS (0x00)
 *                  UDScub_CANCELED   (0xFE)
 *                  UDScub_FINISHED   (0xFF)
 *                  Negative response code (0x01 to 0xFD) (see uds.h)
 *                                          0x78 reserved!!! don´t return this value
 *************************************************************************/
#if ( SDS_A_START_DIAGNOSTIC_SESSION == SDSext_ENABLED )
#define SessionType RxTxBuf[0]
byte UDS_ubDiagService10( byte* RxTxBuf, word* LenOfMsg, byte Mode)
{
tDEH_Return DEH_retVal;
byte retVal = UDScub_FINISHED;
byte current = UDS_ubGetSession(UDS_CHANNEL_INDEX_2);

#ifndef USE_DEBUGGING_WITH_TRACE_ID
(void)DEBUG_Printf(UHV_TRACE, UHV_SDS_UDS, "Service $10: new Session = %02X  len = %d", RxTxBuf[0],LenOfMsg[0]);
#endif

#ifdef SUPPORT_BT_UPDATE
	if (BT_TRUE == CSHDL_bGetBuetoothUpdateActive())
		return UDScub_BUSY_REPEAT_REQUEST;
#endif

  if( *LenOfMsg != (word)1 )
  {
    Mode = Mode; /* prevent compiler warnings */
    retVal = UDScub_INCORRECT_MSG_LEN;
  }
  /********* session check in case of the ******************/
  /********* ECU does not supports programming session *****/
  #if( SDS_ECU_PROGRAMABLE == 0 )
  else if( SessionType == UDScub_PROGRAMMING_SESSION )
  {
    retVal = UDScub_SUBFUNCTION_NOT_SUPPORTED;
  }
  else if( current == UDScub_DEFAULT_SESSION )
  {
    if( SessionType == UDScub_DEVELOPMENT_SESSION )
    {
      retVal = UDScub_SFNS_IN_ACTIVE_DIAGNOSTIC_SESSION;
    }
  }
  else
  {
  }
  /********* session check in case of the ******************/
  /********* ECU supports programming session **************/
  #else
  else if( current == UDScub_DEFAULT_SESSION )
  {
    if( (SessionType == UDScub_PROGRAMMING_SESSION) )
    {
      retVal = UDScub_SFNS_IN_ACTIVE_DIAGNOSTIC_SESSION;
    }
  }
  else if( current != UDScub_PROGRAMMING_SESSION )
  {
    if( SessionType == UDScub_PROGRAMMING_SESSION )
    {
      #if( SDS_10_ZeroVehicleOrEngineSpeed == SDS_ENABLED )
      retVal = SDSext_ubZeroVehicleOrEngineSpeed((byte)0x10);
      if( retVal == 0 )
      #endif
      {
          retVal = UDScub_REQUEST_RECEIVED_RESPONSE_PENDING;
      }
    }
  }
  else
  {
    /* do not sent pos. response in programming session after 78h */
    SDSext_vSetProgRequest();
    retVal = UDScub_CANCELED;
  }
  #endif

  if( (retVal == UDScub_FINISHED) || (retVal == UDScub_REQUEST_RECEIVED_RESPONSE_PENDING))
  {
    if( UDS_ubSetSession(UDS_CHANNEL_INDEX SessionType) == I2S_TRUE )
    {
      DEH_retVal = DEH_SetDiagnosticSession((tDEH_DiagnosticType)SessionType);
      if( DEH_retVal == DEH_OK )
      {
        /* return value ignored because DEH settings can´t be restored */
        SDSext_ubComCtrl( SDS_28_EnableRxAndTx, SDS_28_AllNetworks|SDS_28_NormalAndNetworkManagmentCommunicationMessages );
        SDSext_vResetSecurityAccessEffected();
        if( retVal == UDScub_FINISHED )
        {
          LenOfMsg[0] = 5;
          RxTxBuf[1] = (byte)(cuw_P2_MAX_DEF >> 8);  /* values from uds module */
          RxTxBuf[2] = (byte)(cuw_P2_MAX_DEF >> 0);  /* 1ms resolution */
          RxTxBuf[3] = (byte)((cuw_P2_MAX/10) >> 8); /* 10ms resolution */
          RxTxBuf[4] = (byte)((cuw_P2_MAX/10) >> 0);
        }
     	  SDS_UDS_vDiagTimeout(); // zurücksetzen der Werte für UserProfile und ResetToFactory
		  // laut E-Mmail vom 17.06.2009, Fr. Zheng => soll auch in die EV mit aufgenommen werden
      }
      else if( DEH_retVal == DEH_E_INIT )
      {
        UDS_ubSetSession(UDS_CHANNEL_INDEX current);
        retVal = UDScub_BUSY_REPEAT_REQUEST;
      }
      else
      {
        UDS_ubSetSession(UDS_CHANNEL_INDEX current);
        retVal = UDScub_CONDITIONS_NOT_CORRECT;
      }
    }
    else
    {
      retVal = UDScub_SUBFUNCTION_NOT_SUPPORTED;
    }
  }
  return retVal;
}
#endif


/***************************************************************************
 *  PURPOSE:        UDS_ubDiagService11
 **************************************************************************
 *  PARAMETERS:     see UDS_ubDiagService10
 **************************************************************************
 *  RETURN VALUE:   see UDS_ubDiagService10
 *************************************************************************/
#if( SDS_A_RESET_OR_DISCONNECT == SDSext_ENABLED )
#define ResetType RxTxBuf[0]
byte UDS_ubDiagService11( byte* RxTxBuf, word* LenOfMsg, byte Mode )
{
    EEDeviceControlBlockType *pEEHost = (EEDeviceControlBlockType *)getEEHostControl();
    FlagType flag = EEPROM_flagIsEEFree(pEEHost);

  if( *LenOfMsg != (word)1 )
  {
    Mode = Mode; /* prevent compiler warnings */
    return UDScub_INCORRECT_MSG_LEN;
  }
  switch( ResetType )
  {
    #if( SDS_11_POWER_DOWN == SDS_ENABLED )
    case SDS_11_EnableRapidPowerShutDownSequence:
    case SDS_11_DisableRapidPowerShutDownSequence:
      if( SDSext_ubPowerDown(ResetType) == I2S_FALSE )
      {
        return UDScub_CONDITIONS_NOT_CORRECT;
      }
      if( ResetType == SDS_11_EnableRapidPowerShutDownSequence )
      {
        *LenOfMsg = 2;
        RxTxBuf[1] = SDS_11_TimeToPowerDown;   /* time to reset */
      }
      break;
    #endif

    case SDS_11_SoftReset:
      return UDScub_SUBFUNCTION_NOT_SUPPORTED;

    case SDS_11_KeyOffOnReset:
        flag = EEPROM_flagIsEEFree(pEEHost);
        if( flag == TRUE )
        {
            DEBUG_TEXT(SDS_UDS_SWI_TRC,MOD_SDS_UDS,"EEP is free - Reset ECU");
            ubResetControl = ResetType;
        }
        else
        {
            DEBUG_TEXT(SDS_UDS_SWI_TRC,MOD_SDS_UDS,"EEP blocks - No ECU Reset - Retry");
            return UDScub_REQUEST_RECEIVED_RESPONSE_PENDING;
        }
        break;

    default:
      return UDScub_SUBFUNCTION_NOT_SUPPORTED;
  }
  return UDScub_FINISHED;
}
#endif

/***************************************************************************
 *  PURPOSE:        UDS_ubDiagService14
 **************************************************************************
 *  PARAMETERS:     see UDS_ubDiagService10
 **************************************************************************
 *  RETURN VALUE:   see UDS_ubDiagService10
 *************************************************************************/
#if( SDS_A_CLEAR_DIAGNOSTIC_INF == SDSext_ENABLED )
byte UDS_ubDiagService14( byte* RxTxBuf, word* LenOfMsg, byte Mode)
{
	dword dtc;
	tDEH_Return retVal;

	if( *LenOfMsg != (word)3 )
	{
		Mode = Mode; /* prevent compiler warnings */
		return UDScub_INCORRECT_MSG_LEN;
	}
	dtc = (dword)RxTxBuf[0] << 16;
	dtc += (word)RxTxBuf[1] << 8;
	dtc += RxTxBuf[2];

    if(WaitingTimeState == DEH_EE_WAIT_TIME_IDLE)
    {
        retVal = DEH_ClearDiagnosticInformation( (tDEH_DTCGroup)dtc );

        if( (retVal == DEH_OK) || (retVal == DEH_E_CLEAR) )
        {
            WaitingTimeState = DEH_EE_WAIT_TIME_START;
            (void)SetRelAlarm(CTA_DEH_EE_WRITE_TIME, MSEC(250), MSEC(0));
            return UDScub_REQUEST_RECEIVED_RESPONSE_PENDING;
        }
        else if ( retVal == DEH_E_UNKNOWN_DTCGROUP )
        {
            return UDScub_REQUEST_OUT_OF_RANGE;
        }
        else if ( (retVal == DEH_E_INIT))
        {
            return UDScub_BUSY_REPEAT_REQUEST;
        }
        else if ( (retVal == DEH_E_BUSY) )
        {
            return UDScub_REQUEST_RECEIVED_RESPONSE_PENDING;
        }
        else
        {
            return UDScub_CONDITIONS_NOT_CORRECT;
        }
    }
    else if(WaitingTimeState == DEH_EE_WAIT_TIME_STOP)
    {
        /*  wait so long to be sure that all eeprom data for DEH was written - 130Bytes */
        /* len = 0 --> do not return dtc to the tester */
        /* only service id will be returned to tester */
        LenOfMsg[0] = 0;
        WaitingTimeState = DEH_EE_WAIT_TIME_IDLE;
        return UDScub_FINISHED;
    }
    else
    {
        return UDScub_REQUEST_RECEIVED_RESPONSE_PENDING;
    }
}

ALARMCALLBACK(SDS_UDS_Wait_DEH_Write_Finish)
{
    WaitingTimeState = DEH_EE_WAIT_TIME_STOP;
}

#endif
/***************************************************************************
 *  PURPOSE:        UDS_ubDiagService19
 **************************************************************************
 *  PARAMETERS:     see UDS_ubDiagService10
 **************************************************************************
 *  RETURN VALUE:   see UDS_ubDiagService10
 *************************************************************************/
#if( SDS_A_READ_DTC_BY_STATUS == SDSext_ENABLED )
#define DTCInformationType RxTxBuf[0]
#define DTCStatusMask RxTxBuf[1]
#define DTCExtendedDataRecordNumber RxTxBuf[4]
byte UDS_ubDiagService19( byte* RxTxBuf, word* LenOfMsg, byte Mode)
{
	tDEH_Return retVal;
	tDEH_DTCAndStatus das;
	dword dtc;
	word index=0;

	if( *LenOfMsg == (word)0 )
	{
		Mode = Mode; /* prevent compiler warnings */
		return UDScub_INCORRECT_MSG_LEN;
	}

	dtc = (dword)RxTxBuf[1] << 16;
	dtc += (word)RxTxBuf[2] << 8;
	dtc += RxTxBuf[3];

	DEBUG_VALUE1(SDS_UDS_SWI_TRC, MOD_SDS_UDS, ">> Get DTCInformationType=0x%04X request from Tester", DTCInformationType);
	switch( DTCInformationType )
	{
	    /* Read DTC and status */
	    case SDS_19_ReportDTCByStatusMask:
#if( SDS_A_DTCStatusAvailabilityMask == 0xDF )
		case SDS_19_ReportEmissionRelatedOBDDTCByStatusMask:
#endif
		if( *LenOfMsg != (word)2 )
		{
			return UDScub_INCORRECT_MSG_LEN;
		}
		index = 2;

		do
		{
			retVal = DEH_ReadDTCByStatus( (tDEH_DTCInfoType)DTCInformationType,(tDEH_DTCStatusMask)DTCStatusMask, &das);
			DEBUG_VALUE1(SDS_UDS_SWI_TRC, MOD_SDS_UDS, "das.DTC=0x%04X", das.DTC);
			DEBUG_VALUE1(SDS_UDS_SWI_TRC, MOD_SDS_UDS, "Get DTCStatusMask request from Tester=0x%04X", DTCStatusMask);
			DEBUG_VALUE1(SDS_UDS_SWI_TRC, MOD_SDS_UDS, "Internal das.StatusOfDTC =0x%04X get from DEH", das.StatusOfDTC);
			DEBUG_VALUE1(SDS_UDS_SWI_TRC, MOD_SDS_UDS, "(DTCStatusMask & (das.StatusOfDTC & SDS_A_DTCStatusAvailabilityMask)=0x%04X", DTCStatusMask & (das.StatusOfDTC & SDS_A_DTCStatusAvailabilityMask));
			/* (das.StatusOfDTC & SDS_A_DTCStatusAvailabilityMask) & DTCStatusMask) */
			/* Looking in VW80124 for UHV only DTC support for kind of type with the Status 0x19 (DTCStatusAvailabilityMask
			  - For systems without warning lamp ) is needed. First read the internal state DTCs (give all internal DTCs States back)
			  and mask it out with DTCStatusAvailabilityMask. The result have now to compare with the DTCStatusMask request from the
			  tester ((DTCStatusMask & result). Is this new result now not zero DTC Information (DTC list) have to give back to tester.
			  Otherwise a empty list. The internal Status of DTC when give back to tester have mask out again with DTCStatusAvailabilityMask
			  because UHV have only support DTcs with type 0x19 (DTCStatusAvailabilityMask) */
			if (((das.StatusOfDTC & SDS_A_DTCStatusAvailabilityMask) & DTCStatusMask)== 0)
			{
				DEBUG_TEXT(SDS_UDS_SWI_TRC, MOD_SDS_UDS, "DTCStatusMask & (das.StatusOfDTC & SDS_A_DTCStatusAvailabilityMask) not match.");
			}
			else
			{
				if( (retVal == DEH_OK) || (retVal == DEH_E_DATA_STILL_AVAILABLE) )
				{
					RxTxBuf[index+0] = (byte)(das.DTC>>16);
					RxTxBuf[index+1] = (byte)(das.DTC>>8);
					RxTxBuf[index+2] = (byte)(das.DTC);
					/*  das.StatusOfDTC is the internal StatusOfDTC get from DEH
						Because UHV have support only that Stati of DTC that defined in SDS_A_DTCStatusAvailabilityMask
						das.StatusOfDTC is mask out with SDS_A_DTCStatusAvailabilityMask */
					RxTxBuf[index+3] = das.StatusOfDTC & SDS_A_DTCStatusAvailabilityMask;
					DEBUG_VALUE1(SDS_UDS_SWI_TRC, MOD_SDS_UDS, "StatusOfDTC=0x%04X goes to Tester", RxTxBuf[index+3]);
					index += 4; /* sizeof(tDEH_DTCAndStatus); */
					if( (index+4) > uwBufSize ) /* check if buffer space available for the next DTC+status */
					{
						retVal = DEH_OK; /* stop collecting */
					}
				}
			}
		}
		while( retVal == DEH_E_DATA_STILL_AVAILABLE );
		RxTxBuf[1] = SDS_A_DTCStatusAvailabilityMask;
		*LenOfMsg = index;

		/* The below function overwrites the RxTxBuf and hides the DTCs which are masked out. */
		*LenOfMsg = SDS_UDS_u16CalculateDtcMaskierung(index, &RxTxBuf[0] );
		break;

#ifdef DEH_SUP_SNAP_SHOT_DATA
		case SDS_19_ReportDTCSnapshotRecordByDTCNumber:
		if( *LenOfMsg != (word)4 )
		{
			return UDScub_INCORRECT_MSG_LEN;
		}
		retVal = DEH_ReadDTCSnapshotRecord( (tDEH_DTC)dtc, (uint8_t*)&RxTxBuf[4], (uint16_t)uwBufSize-5, (uint16_t*)&index, (uint8_t*)&RxTxBuf[5] );
		*LenOfMsg = index+5;
		break;
#endif

		/* Read freeze frames */
	case SDS_19_ReportDTCExtendedDataRecordByDTCNumber:
		if( *LenOfMsg != (word)5 )
		{
			return UDScub_INCORRECT_MSG_LEN;
		}
		switch( DTCExtendedDataRecordNumber )
		{
		case SDS_19_StandardDTCInformation:
		case SDS_19_StandardFreezeFrame_1:
		case SDS_19_AllStandardFreezeFrameAndStandardDTCInformation:
#if( SDS_A_DTCStatusAvailabilityMask == 0xDF )
			case SDS_19_AllOBDExtendedDataRecordNumbers:
#endif
		case SDS_19_AllDTCExtendedDataRecordNumbers:
			/* DEH_ReadDTCExtDataRecord write info RxTxBuf[x] data. This information goes back to Vas Tester */
			retVal = DEH_ReadDTCExtDataRecord( (tDEH_DTC)dtc, (uint8_t*)&RxTxBuf[4], (tDEH_RecordNumber)DTCExtendedDataRecordNumber, (uint16_t)uwBufSize-5, (uint16_t*)&index, (uint8_t*)&RxTxBuf[5]);
            DEBUG_VALUE1(SDS_UDS_SWI_TRC, MOD_SDS_UDS, "Internal das.StatusOfDTC =0x%04X get from DEH",RxTxBuf[4]);
			/*  In RxTxBuf[4] is now the internal StatusOfDTC get from DEH
                Because UHV have support only that Stati of DTC that defined in SDS_A_DTCStatusAvailabilityMask
                RxTxBuf[4] is mask out with SDS_A_DTCStatusAvailabilityMask */
			RxTxBuf[4] = RxTxBuf[4] & SDS_A_DTCStatusAvailabilityMask;
			DEBUG_VALUE1(SDS_UDS_SWI_TRC, MOD_SDS_UDS, "StatusOfDTC=0x%04X goes to Tester", RxTxBuf[4]);

			*LenOfMsg = index+5;
			break;
		default:
			return UDScub_REQUEST_OUT_OF_RANGE;
		}
		break;

		/* unsupported subfunction */
	default:
		return UDScub_SUBFUNCTION_NOT_SUPPORTED;
	}
	if( (retVal == DEH_OK) || (retVal == DEH_E_NO_DATA) )
	{
		return UDScub_FINISHED;
	}
	/*else if( (retVal == DEH_E_MODE) || (retVal == DEH_E_UNKNOWN_DTC) )
	 {
	 return UDScub_REQUEST_OUT_OF_RANGE;
	 } default case */
	else if( (retVal == DEH_E_BUSY) || (retVal == DEH_E_INIT) )
	{
		return UDScub_BUSY_REPEAT_REQUEST;
	}
	else if( retVal == DEH_E_WRONG_DIAG_SESSION )
	{
		return UDScub_SFNS_IN_ACTIVE_DIAGNOSTIC_SESSION;
	}
	else if( (retVal == DEH_E_NO_BUFFER) || (retVal == DEH_E_READ_MEMORY) )
	{
		return UDScub_CONDITIONS_NOT_CORRECT;
	}
	else
	{
		return UDScub_REQUEST_OUT_OF_RANGE;
	}
}

#endif

/***************************************************************************
 *  PURPOSE:        UDS_ubDiagService22
 **************************************************************************
 *  PARAMETERS:     see UDS_ubDiagService10
 **************************************************************************
 *  RETURN VALUE:   see UDS_ubDiagService10
 *************************************************************************/
#if( SDS_A_READ_DATA_BY_IDENTIFIER == SDSext_ENABLED )
byte UDS_ubDiagService22( byte* RxTxBuf, word* LenOfMsg, byte Mode )
{
	byte i;
	static byte n, NumberOfDids, CurrentDid, DidTempNotAvailable;
	static word uwIdx, awDid[8];

	if( eReadInProgress != SDSext_IO_USER_READ_IN_PROGRESS )
	{
		if( LenOfMsg[0] < (word)2 ) /* at least one DID expected */
		{
			return UDScub_INCORRECT_MSG_LEN;
		}
		else if( (LenOfMsg[0] & 0x1) > (word)0 ) /* check if lenght is even */
		{
			return UDScub_INCORRECT_MSG_LEN;
		}
		else if( Mode == UDScub_PHYSICAL )
		{
			if( LenOfMsg[0] > (word)16 ) /* max number of identifier = 8 */
			{
				return UDScub_REQUEST_OUT_OF_RANGE;
			}
		}
		else
		{
			if( LenOfMsg[0] > (word)6 ) /* max number of identifier = 3 */
			{
				return UDScub_REQUEST_OUT_OF_RANGE;
			}
		}
		for( n=0; n<LenOfMsg[0]; n+=2 )
		{
			awDid[n/2] = ((word)RxTxBuf[n] << 8) + RxTxBuf[n+1]; /* save DIDs temporary */
		}
		NumberOfDids = n/2;

		CurrentDid = 0;
		uwIdx = 0;
		DidTempNotAvailable = I2S_FALSE;
	}
	while( CurrentDid < NumberOfDids )
	{
		byte lower, upper, found;
		lower=0; found = 0; i = 0;
		upper=(sizeof(cs_ReadDataIdItem)/sizeof(t_ReadDataIdCfgItem))-1;
		/* lower,i = first DID; upper = last DID */

		while( i < upper ) /* stop searching if border reached */
		{
			/* get next interval */
			if( awDid[CurrentDid] < cs_ReadDataIdItem[i].DataId )
			{
				upper = i; /* update upper border */
			}
			else
			{
				lower = i; /* update lower border */
			}
			i = (upper-lower); /* calculate the middle */
			i = lower + i/2 + i%2; /* of lower and upper border */
			if( cs_ReadDataIdItem[i].DataId == awDid[CurrentDid] )
			{
				found = 1; /* data identifier found */
				break; /* stop searching */
			}
			if( i == lower ) /* check for termination condition */
			{
				if( awDid[CurrentDid] == cs_ReadDataIdItem[i+1].DataId )
				{
					found = 1; /* data identifier found */
					i++;
				}
				break; /* stop searching */
			}
			if( i == upper ) /* check for termination condition */
			{
				if( awDid[CurrentDid] == cs_ReadDataIdItem[i-1].DataId)
				{
					found = 1; /* data identifier found */
					i--;
				}
				break; /* stop searching */
			}
		}
		if( found != 0 ) /* insert data into the buffer if DID found */
		{
			if( (uwIdx+cs_ReadDataIdItem[i].Len+2) <= uwBufSize )
			{
				eReadInProgress = CopyData(&cs_ReadDataIdItem[i],&RxTxBuf[uwIdx+2]);
				if( eReadInProgress == SDSext_IO_USER_READ_SUCCESSFUL )
				{
					RxTxBuf[uwIdx] = (byte)(awDid[CurrentDid] >> 8);
					RxTxBuf[uwIdx+1] = (byte)awDid[CurrentDid];
					if( u8PhoneNumberLength > 0 )
					{
						uwIdx += u8PhoneNumberLength+2;
					}
					else
					{
						uwIdx += cs_ReadDataIdItem[i].Len+2;
					}
				}
				else if( eReadInProgress == SDSext_IO_USER_READ_IN_PROGRESS )
				{
					return UDScub_IN_PROCESS;
				}
				else if( eReadInProgress == SDSext_IO_USER_READ_NOT_ALLOWED )
				{
					return UDScub_ACCESS_DENIED;
				}
				else /* failed */
				{
					/* continue collecting remaining DIDs */
					DidTempNotAvailable = I2S_TRUE;
				}
			}
		}
		CurrentDid ++;
	}
	if( uwIdx > 0 )
	{
		LenOfMsg[0] = uwIdx;
		u8PhoneNumberLength = 0;
		return UDScub_FINISHED;
	}
	else
	{
		if( DidTempNotAvailable == I2S_TRUE )
		{
			/* DID not supported temporarily */
			return UDScub_CONDITIONS_NOT_CORRECT;
		}
		else
		{
			/* DID not supported at all */
			return UDScub_REQUEST_OUT_OF_RANGE;
		}
	}
}
#endif

/***************************************************************************
 *  PURPOSE:        UDS_ubDiagService28
 **************************************************************************
 *  PARAMETERS:     see UDS_ubDiagService10
 **************************************************************************
 *  RETURN VALUE:   see UDS_ubDiagService10
 *************************************************************************/
#if( SDS_A_COMMUNICATION_CONTROL == SDSext_ENABLED )
#define ControlType RxTxBuf[0]
byte UDS_ubDiagService28( byte* RxTxBuf, word* LenOfMsg, byte Mode )
{
byte retVal;

  if( LenOfMsg[0] != (word)2 )
  {
    Mode = Mode; /* prevent compiler warnings */
    retVal = UDScub_INCORRECT_MSG_LEN;
  }
  else switch( ControlType )
  {
    case SDS_28_EnableRxAndTx:
    case SDS_28_EnableRxAndDisableTx:
    case SDS_28_GatewayVehicleWakeUp:
      #if( SDS_28_ZeroVehicleOrEngineSpeed == SDS_ENABLED )
      retVal = SDSext_ubZeroVehicleOrEngineSpeed((byte)0x28);
      if( retVal == 0 )
      #endif
      {
        retVal = SDSext_ubComCtrl( ControlType, RxTxBuf[1] );
        if( retVal == UDScub_FINISHED )
        {
          LenOfMsg[0] = 1;
        }
      }
      break;
    default:       /* unknown ControlType */
      retVal = UDScub_SUBFUNCTION_NOT_SUPPORTED;
      break;
  }
  return retVal;
}
#endif


/***************************************************************************
 *  PURPOSE:        UDS_ubDiagService3E
 **************************************************************************
 *  PARAMETERS:     see UDS_ubDiagService10
 **************************************************************************
 *  RETURN VALUE:   see UDS_ubDiagService10
 *************************************************************************/
#if( SDS_A_TESTER_PRESENT == SDSext_ENABLED )
byte UDS_ubDiagService3E( byte* RxTxBuf, word* LenOfMsg, byte Mode )
{
  if( *LenOfMsg != (word)1 )
  {
    Mode = Mode;        /* prevent compiler warnings */
    return UDScub_INCORRECT_MSG_LEN;
  }
  if( RxTxBuf[0] != (byte)0x00 )  /* only ZeroSubFunction allowed */
  {
    return UDScub_SUBFUNCTION_NOT_SUPPORTED;
  }
  return UDScub_FINISHED;
}
#endif


/***************************************************************************
 *  PURPOSE:        UDS_ubDiagService85
 **************************************************************************
 *  PARAMETERS:     see UDS_ubDiagService10
 **************************************************************************
 *  RETURN VALUE:   see UDS_ubDiagService10
 *************************************************************************/
#if( SDS_A_CTRL_DTC_SETTINGS == SDSext_ENABLED )
#define DTCSettingType RxTxBuf[0]
byte SDS_UDS_ubDiagService85( byte* RxTxBuf, word* LenOfMsg, byte Mode )
{
dword dtcType;
tDEH_Return retVal;

  if( *LenOfMsg != (word)4 )
  {
    Mode = Mode; /* prevent compiler warnings */
    return UDScub_INCORRECT_MSG_LEN;
  }
  if( (DTCSettingType != (byte)SDS_85_Off) && (DTCSettingType != (byte)SDS_85_On) )
  {
    return UDScub_SUBFUNCTION_NOT_SUPPORTED;
  }

  dtcType  = (dword)RxTxBuf[1] << 16;
  dtcType += (word)RxTxBuf[2] << 8;
  dtcType += RxTxBuf[3];

  #if( SDS_85_ZeroVehicleOrEngineSpeed == SDS_ENABLED )
  {
    byte returnValue = SDSext_ubZeroVehicleOrEngineSpeed((byte)0x85);
    if( returnValue > 0 )
    {
      return returnValue;
    }
  }
  #endif

  retVal = DEH_ControlDTCSetting( (tDEH_DTCSetType)RxTxBuf[0], (tDEH_TypeOfDTC)dtcType );

  if( retVal == DEH_OK )
  {
    *LenOfMsg = 1;
    return UDScub_FINISHED;
  }

  if( retVal == DEH_E_UNKNOWN_DTC )
  {
    return UDScub_REQUEST_OUT_OF_RANGE;
  }
  else if( retVal == DEH_E_INIT )
  {
    return UDScub_BUSY_REPEAT_REQUEST;
  }
  else
  {
    return UDScub_SUBFUNCTION_NOT_SUPPORTED;
  }
}
#endif


#if( SDS_A_READ_DATA_BY_IDENTIFIER == SDSext_ENABLED )
/*************************************************************************
 *  PURPOSE:        Get data for service 22
 *************************************************************************/
static tSDSextRead_Return CopyData(const t_ReadDataIdCfgItem* DataIdItem, byte* data )
{
word i=0;

	switch( DataIdItem->extMem )
	{
	case SDSext_BINARY:
		while( i<DataIdItem->Len )
		{
			char ch_h = ((const byte*)DataIdItem->Address)[(2*i)+0];
			char ch_l = ((const byte*)DataIdItem->Address)[(2*i)+1];
			if( ch_h >= 'A')
			{
				ch_h += 9;
			}
			if( ch_l >= 'A')
			{
				ch_l += 9;
			}
			data[i] = (ch_h << 4) + (ch_l & 0x0F);
			i++;
		}
		break;

	case SDSext_ROM:
	case SDSext_ASCII:
		while( i<DataIdItem->Len )
		{
			data[i] = ((const byte*)DataIdItem->Address)[i];
			i++;
		}
		if ( DataIdItem->DataId == ID_22_F182_IDENTIFIKATION_DER_PARAMETRIERUNG )
		{
			data[0] = data[0] - '0'; // A spl case because this id is a mixture of binary and ascii values.
			// The first byte is a binary and rest are ascii.
		}
		break;

	case SDSext_RAM:
		while( i<DataIdItem->Len )
		{
			data[i] = ((byte*)DataIdItem->Address)[i];
			i++;
		}
		break;

	case SDSext_EEPROM:
	case SDSext_SPECIAL_EEPROM:
	case SDSext_USER:
		return SDSext_eReadUserAndIOData( data, DataIdItem );

	default:
		return SDSext_IO_USER_READ_FAILED;
	}
	return SDSext_IO_USER_READ_SUCCESSFUL;
}
#endif




