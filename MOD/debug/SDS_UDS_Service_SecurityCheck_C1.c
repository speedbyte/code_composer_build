/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/
/** 
 * \file 	SDS_UDS_Service_SecurityCheck_C1.c
 * \brief	This file contains the program to xxxx
 * \brief	This file fulfills the following requirements:
 * 			1. Requirement 1
 * 			2. Requirement 2
 * 			3. Requirement x
*/
/*-----------------------------------------------------------------------------
* $Date: 2009-08-20 19:55:57 +0200 (Do, 20 Aug 2009) $
* $Rev: 7960 $
* $Author: agrawal $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_SDS_UDS/trunk/src/SDS_UDS_Service_SecurityCheck_C1.c $
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
#include "EEPROM_LAYOUT_CFG.h"
#include "TRACE.h"
#include "SDS_UDS_Service_SecurityCheck_C1.id"
/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "SDS_ext.h"
#include "SDS_UDS_CI.h"

/*----------------------------------------------------------------------------*/
/* pragma Directives                                                          */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Definitions (avoid as much as possible!)                  */
/*----------------------------------------------------------------------------*/
Uint16Type TimerSecurity = 0;
Uint8Type RandomCounter[4] = { 0xFF, 0xAA, 0x80, 0x55 }; // wird "pseudo komplex" in SDSext_vTask manipuliert;
/*----------------------------------------------------------------------------*/
/* Private Manifest Constant Definitions                                      */
/*----------------------------------------------------------------------------*/
/* return values of SDSext functions  */


/*----------------------------------------------------------------------------*/
/* Private Types                                                              */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the type
*/

/*----------------------------------------------------------------------------*/
/* Prototypes of file local Methods                                           */
/*----------------------------------------------------------------------------*/
static void _SDSext_vCreateSeed(Uint8Type* SeedBuf);
static void _SDSext_vSetSeedToNull(Uint8Type* SeedBuf);

/*----------------------------------------------------------------------------*/
/* File local Code Macros                                                        */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the macros
*/

/*----------------------------------------------------------------------------*/
/* File local Variables                                                       */
/*----------------------------------------------------------------------------*/
static Uint8Type u8BootloaderTimeLockState = 0;

/** 
\brief	description of the private variable
*/
/*
 xxxxxxxx
 ||||||||-----   MASK_SEED_COUNTER       
 |||||||------   MASK_SEED_COUNTER      
 ||||||-------   MASK_SEED_ALREADY_SEND   
 |||||--------   MASK_SEC_STATUS_LOCKED   
 ||||---------      
 |||----------      
 ||-----------   MASK_SEC_ACTIVE      
 |------------   MASK_SEC_DD_ACTIVE    
 */
static Uint8Type ubSeed[4];

/*----------------------------------------------------------------------------*/
/* File local const Definitions                                               */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the private constant
*/
#define FIX_DD_LOGIN_OFFSET     20103              // Security code for datensatz download
#define KEY_NOT_VALID            0                 // no key valid
#define KEY_VALID                1                 // normal key for user login valid
#define KEY_DD_VALID             2                 // datensatz download key valid
#define MAX_KEY_LEN              4                 // andere Länge brauchen vorerst nicht unterstützt werden
/*----------------------------------------------------------------------------*/
/* Private Address Calculations                                               */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Function    : SDS_UDS_vInitialiseService_SecurityCheck                              */
/** 
 \brief      To reset the Static variables
 
 \brief      Description:
 this function resets the static variables.

  
 \return     void 
 */
/*----------------------------------------------------------------------------*/
void SDS_UDS_vInitialiseServiceSecurityCheck(void) 
{
	;
}
/*  FUNCTION void _SDSext_vCreateSeed
 **************************************************************************
 *  PURPOSE:        Erzeugt eine Zufalls Seed.
 **************************************************************************
 *  PARAMETERS:     SeedBuf
 **************************************************************************
 *  RETURN VALUE:   Seed
 *************************************************************************/
static void _SDSext_vCreateSeed(Uint8Type* SeedBuf) {
	SeedBuf[0] = RandomCounter[0];
	SeedBuf[1] = RandomCounter[1];
	SeedBuf[2] = RandomCounter[2];
	SeedBuf[3] = RandomCounter[3];
}

/*  FUNCTION void _SDSext_vSetSeedToNull
 **************************************************************************
 *  PURPOSE:        setzt den Seed auf 0x00000000.
 **************************************************************************
 *  PARAMETERS:     SeedBuf
 **************************************************************************
 *  RETURN VALUE:   Seed
 *************************************************************************/
static void _SDSext_vSetSeedToNull(Uint8Type* SeedBuf) {
	SeedBuf[0] = 0;
	SeedBuf[1] = 0;
	SeedBuf[2] = 0;
	SeedBuf[3] = 0;
}

/*  FUNCTION Uint8Type ubCheckKeyValid
 **************************************************************************
 *  PURPOSE:        vergleich den vom Tester gesendeten Key mit dem selbst berechneten Key
 **************************************************************************
 *  PARAMETERS:     KeyInput:    Key vom Tester
 *                  SeedBuf:     Seed zum Berechnen vom Key
 *                  LenOfKey:    Länge vom Key in Bytes (Default = 4)
 **************************************************************************
 *  RETURN VALUE:   Key valid or not valid
 *************************************************************************/
Uint8Type ubCheckKeyValid(const Uint8Type* KeyInput, Uint8Type* SeedBuf,
		Uint8Type LenOfKey) {
	Uint8Type ubTestKey= KEY_NOT_VALID;

	Sint32Type Key = (KeyInput[0] << 24) + (KeyInput[1] << 16) + (KeyInput[2]
			<< 8) + KeyInput[3];
	Sint32Type Seed = (SeedBuf[0] << 24) + (SeedBuf[1] << 16) + (SeedBuf[2]
			<< 8) + SeedBuf[3];
#ifndef USE_DEBUGGING_WITH_TRACE_ID
	(void)DEBUG_Printf(UHV_DEBUG, UHV_SDS_UDS, "Seed = %d, Key = %d", Seed, Key);
#endif

	if (Key == (Seed + FIX_LOGIN_OFFSET)) {
		ubTestKey = KEY_VALID;
	} else if (Key == (Seed + FIX_DD_LOGIN_OFFSET)) {
		ubTestKey = KEY_DD_VALID;
	} else {
		ubTestKey = KEY_NOT_VALID;
	}
	return ubTestKey;
}


Uint8Type SDS_UDS_u8GetBootloaderTimeLockState(void)
/*----------------------------------------------------------------------------*/
/* Function    : SDS_UDS_u8GetBootloaderTimeLockState                         */
/**
 \brief      Get u8BootloaderTimeLockState

 \param      none

 \return     u8BootloaderTimeLockState
 */
/*----------------------------------------------------------------------------*/
{
	(void)EEPROM_enGetSingleByteFromRamMirror(&u8BootloaderTimeLockState, EEPROM_LAYOUT_ENUM_TIMELOCK);
	return u8BootloaderTimeLockState;
}

void SDS_UDS_vSetBootloaderTimeLockState( Uint8Type u8SetBootloaderTimeLockState)
/*----------------------------------------------------------------------------*/
/* Function    : SDS_UDS_vSetBootloaderTimeLockState                         */
/**
 \brief      Set u8BootloaderTimeLockState

 \param      SetBootloaderTimeLockState

 \return     none
 */
/*----------------------------------------------------------------------------*/
{
	u8BootloaderTimeLockState = u8SetBootloaderTimeLockState;
	(void)EEPROM_sfRamWriteOnly(&u8BootloaderTimeLockState, EEPROM_LAYOUT_ENUM_TIMELOCK);
}

/***************************************************************************
 *  PURPOSE:        SDSext_ubDiagService27
 **************************************************************************
 *  PARAMETERS:     see UDS_ubDiagService10
 **************************************************************************
 *  RETURN VALUE:   see UDS_ubDiagService10
 *************************************************************************/
#define SecurityAccessType RxTxBuf[0]

/*----------------------------------------------------------------------------*/
/* Function    : SDSext_ubDiagService27                                                 */
/** 
 \brief      Security Access
 
 \brief      Description:
 Session-Nummer..Bezeichnung
 $01 OBD-Diagnosemodus
 $02 Programmiermodus
 $03 VW - Diagnosemodus
 $40 Bandendemodus (EOL)
 $4F Entwicklungsmodus
 SID(hex)..Service.. LID(hex)..Subfunktion/Data-Identifier..ErlaubteSessions..Sec.Lev.
 $27 Security Access Security Access Type X X X
 $03 Request Seed Login X X X 0
 $04 Send Key Login X X X 0
 $09 Request Seed System Specific X X X 0
 $0A Send Key System Specific X X X 0
 $11 Request Seed Bootloader X 0
 $12 Send Key Bootloader X 0

 \param      InputParameterName1
 InputParameterType1
 value limits
 \param      InputParameterName2
 InputParameterType2
 value limits
 \return     OutputParameterType
 OutputParameterType description
 value limits
 */
/*----------------------------------------------------------------------------*/
Uint8Type SDSext_ubDiagService27(Uint8Type* RxTxBuf, Uint16Type* LenOfMsg,
		Uint8Type Mode) {
	Uint8Type DiagSes = UDS_ubGetSession(UDS_CHANNEL_INDEX_2);
	Uint16Type i = 0;
	/* return variable for this function with a negative default value   */
	Uint8Type retVal = UDScub_SUBFUNCTION_NOT_SUPPORTED;

	switch (SecurityAccessType) {
	case UDScub_SECURITY_ACCESS_SEED:
	case UDScub_SECURITY_ACCESS_SEED_SYSTEMSPEZIFISCH_VAS5163:
		if ( *LenOfMsg != (Uint16Type)1 ) {
			Mode = Mode; /* prevent compiler warnings */
			return UDScub_INCORRECT_MSG_LEN;
		} else {
			if ((ubSecState & MASK_SEC_STATUS_LOCKED) == MASK_SEC_STATUS_LOCKED) {
				/* Start Timer not again */
				/* TI: 9928 -> Wichtig - Nur bei einem Reset oder Kl.15-Wechsel soll die Sperrzeit neu starten. */
				/* TimerSecurity = SEC_TIMER_DELAY; */
				DEBUG_VALUE1(SDS_UDS_SWI_TRC, MOD_SDS_UDS,"UDScub_SECURITY_ACCESS_SEED: Security timer is running!!!  ubSecState = %02X",ubSecState);
				DEBUG_VALUE1(SDS_UDS_SWI_TRC, MOD_SDS_UDS,"Security timer is running for %d seconds",(TimerSecurity/100));

				u8BootloaderTimeLockState = u8BootloaderTimeLockState + 1;
				DEBUG_VALUE1(SDS_UDS_SWI_TRC, MOD_SDS_UDS, "Read: TimeLockState 0x%02X", u8BootloaderTimeLockState);
				(void)EEPROM_sfRamWriteOnly(&u8BootloaderTimeLockState, EEPROM_LAYOUT_ENUM_TIMELOCK);
				retVal = UDScub_TIME_DELAY_NOT_EXPIRED;
			} else {
				if ((ubSecState & MASK_SEC_ACTIVE) == MASK_SEC_ACTIVE) { // RT #1056: bug fix 26.03.2009
					_SDSext_vSetSeedToNull(&ubSeed[0]);
					DEBUG_VALUE1(SDS_UDS_SWI_TRC, MOD_SDS_UDS,"set Seed to 0x00000000  ubSecState = %02X",ubSecState);
				} else if ((ubSecState & MASK_SEED_ALREADY_SEND)
						== MASK_SEED_ALREADY_SEND) {
					DEBUG_VALUE1(SDS_UDS_SWI_TRC, MOD_SDS_UDS,"UDScub_SECURITY_ACCESS_SEED: Seed is sendig already, => send again the same Seed  ubSecState = %02X",ubSecState);
					/* es wurde bereits ein Seed gesendet, d.h. einfach noch einmal den SELBEN" Seed senden */
				} else {
					_SDSext_vCreateSeed(&ubSeed[0]);
					ubSecState |= MASK_SEED_ALREADY_SEND; // dieser Zustand wird nicht ins EEPROM gespeichert, bzw. beim Zurücklesen nach Systemstart ausmaskiert
				}
				*LenOfMsg = 5;
				for (i=1; i < *LenOfMsg; i++) {
					RxTxBuf[i] = ubSeed[i-1];
				}

				retVal = UDScub_FINISHED;
			}
		}
		break;

	case UDScub_SECURITY_ACCESS_KEY:
		if ( *LenOfMsg != (Uint16Type)5 )
		{
			Mode = Mode; /* prevent compiler warnings */
			return UDScub_INCORRECT_MSG_LEN;
		}
		else if ((ubSecState & MASK_SEC_STATUS_LOCKED)
				== MASK_SEC_STATUS_LOCKED)
		{
			DEBUG_VALUE1(SDS_UDS_SWI_TRC, MOD_SDS_UDS,"UDScub_SECURITY_ACCESS_KEY: Security timer is running!!!  ubSecState = %02X",ubSecState);
			retVal = SDS_UDS_REQUEST_SEQUENCE_ERROR;
		}
		else if ((ubSecState & MASK_SEED_ALREADY_SEND)
				== MASK_SEED_ALREADY_SEND)
		{
			if (ubCheckKeyValid(&RxTxBuf[1], &ubSeed[0], MAX_KEY_LEN)
					> KEY_NOT_VALID)
			{
				if (ubCheckKeyValid(&RxTxBuf[1], &ubSeed[0], MAX_KEY_LEN)
						== KEY_VALID)
				{
					ubSecState |= MASK_SEC_ACTIVE;
				}

				else
				{
					ubSecState |= MASK_SEC_DD_ACTIVE;
				}


				ubSecState &= MASK_SEC_STATUS_UNLOCKED; // die Key Sperre wieder aufheben und im EEPROM sichern
				TimerSecurity = 0;
				/* Bootloader security have also to updated to 0 */
				SDS_UDS_vSetBootloaderTimeLockState(0);
				*LenOfMsg = 1;
				RxTxBuf[0] = UDScub_SECURITY_ACCESS_KEY; // eigentlich steht das schon in RxTxBuf[0]
				retVal = UDScub_FINISHED;
				DEBUG_VALUE1(SDS_UDS_SWI_TRC, MOD_SDS_UDS,"UDScub_SECURITY_ACCESS_KEY: Key valid  ubSecState = %02X",ubSecState);
			} else {
				if (((++ubSecState) & MASK_SEED_COUNTER) == MASK_SEED_COUNTER) // Hinweis: ++ubSecState kann kein Überlauf erzeugen, weil dies durch MASK_SEC_STATUS_LOCKED gesichert wird
				{
					DEBUG_TEXT(SDS_UDS_SWI_TRC, MOD_SDS_UDS,"UDScub_SECURITY_ACCESS_KEY: three times wrong key !!!");
					ubSecState |= MASK_SEC_STATUS_LOCKED; // dreimal falscher Key => für 10min sperren !!!
					TimerSecurity = SEC_TIMER_DELAY; // tja, und warten...
				}
				else
				{
					DEBUG_VALUE1(SDS_UDS_SWI_TRC, MOD_SDS_UDS,"UDScub_SECURITY_ACCESS_KEY: key error, attempt = %d",(ubSecState & MASK_SEED_COUNTER));
				}
				// ubSecState ins EEPROM speichern
				(void)EEPROM_sfRamWriteOnly(&ubSecState, EEPROM_LAYOUT_ENUM_SecurityAccessState);
				/* Bootloader security have also to updated */
				u8BootloaderTimeLockState = u8BootloaderTimeLockState + 1;
				(void)EEPROM_sfRamWriteOnly(&u8BootloaderTimeLockState, EEPROM_LAYOUT_ENUM_TIMELOCK);
				DEBUG_VALUE1(SDS_UDS_SWI_TRC, MOD_SDS_UDS, "ubSecState 0x%02X",  ubSecState);
				DEBUG_VALUE1(SDS_UDS_SWI_TRC, MOD_SDS_UDS, "Read: TimeLockState 0x%02X", u8BootloaderTimeLockState);
				retVal = UDScub_INVALID_KEY;
			}
		} else {
			retVal = SDS_UDS_REQUEST_SEQUENCE_ERROR;
		}
		break;

	default:
		return UDScub_SUBFUNCTION_NOT_SUPPORTED;

	}/*end of switch*/
	return retVal;
}



/* End Of File SDS_UDS_Service_SecurityCheck_C1.c */
