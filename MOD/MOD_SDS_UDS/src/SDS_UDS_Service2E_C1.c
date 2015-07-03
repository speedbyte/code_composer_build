/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/
/**
 * \file 	SDS_UDS_Service2E_C1.c
 * \brief	This file contains the program to xxxx
 * \brief	This file fulfills the following requirements:
 * 			1. Requirement 1
 * 			2. Requirement 2
 * 			3. Requirement x
*/
/*-----------------------------------------------------------------------------
* $Date: 2011-09-29 08:40:38 +0200 (Do, 29 Sep 2011) $
* $Rev: 21974 $
* $Author: weber $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_SDS_UDS/trunk/src/SDS_UDS_Service2E_C1.c $
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
/* Libraries from external Vendors                                             */
/*----------------------------------------------------------------------------*/
#include "Common.h"
#include "sds.h"
#include "uds.h"
#include "sds_ext.h"
#include "ISRMask_CB_cfg.h"
#include "sds_cfg.h"
#include "OSEK_HostTask_H1.h"
#include "OSEK_MonitorTask_H1.h"
#include "EEPROM_LAYOUT_CFG.h"
#include "TRACE.h"
#include "ERM.h"
#include "deh.h"
#include "DEH_DTC_CFG.h"

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "SDS_ext.h"
#include "SDS_UDS_CI.h"
#include "SDS_UDS_Service2E_C1.id"

/*----------------------------------------------------------------------------*/
/* pragma Directives                                                          */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Definitions (avoid as much as possible!)                  */
/*----------------------------------------------------------------------------*/
Uint8Type
		_SDS_UDS_au8FingerprintValue[ID_2E_F198_WERKSTATTCODE_DAS_ANGESCHLOSSENEN_TESTERS_MAX_LEN] = {0,0,0,0,0,0};
Uint8Type
		_SDS_UDS_au8ProgrammingDate[ID_2E_F199_PROGRAMMING_DATE_MAX_LEN] = {0,0,0};
static BooleanType _SDS_UDS_bProgrammingDateIsCorrect = BT_TRUE;
/*----------------------------------------------------------------------------*/
/* Private Types                                                              */
/*----------------------------------------------------------------------------*/
/**
\brief	description of the type
*/
typedef struct
{
  word DataId;                   /* data id */
  word MinLen;                   /* minimum length of data */
  word MaxLen;                   /* maximum length of data */
  Uint16Type Address;                 /* data address or index in data table */
} t_WriteDataIdCfgItem;
/*----------------------------------------------------------------------------*/
/* Private Manifest Constant Definitions                                      */
/*----------------------------------------------------------------------------*/
#define E2P_WRITE_OVER_RAM_MIRROR 0xFFFF

/************** A T T E N T I O N *****************************
   because of speed optimization binary search algorithm is used
   which requires a sorted (ascending !!!) data identifier table */

static const t_WriteDataIdCfgItem cs_WriteDataIdItem[] = {
{ ID_2E_0500_ANALYSE_1, ID_2E_0500_ANALYSE_1_MIN_LEN, ID_2E_0500_ANALYSE_1_MAX_LEN,  E2P_WRITE_OVER_RAM_MIRROR },
{ ID_2E_0600_CODIERUNG,  ID_2E_0600_CODIERUNG_MIN_LEN, ID_2E_0600_CODIERUNG_MAX_LEN,  E2P_WRITE_OVER_RAM_MIRROR },
{ ID_2E_0909_SET_NUMBER_ROAMING_FOR_INFO_CALL,  ID_2E_0909_SET_NUMBER_ROAMING_FOR_INFO_CALL_MIN_LEN, ID_2E_0909_SET_NUMBER_ROAMING_FOR_INFO_CALL_MAX_LEN,  E2P_WRITE_OVER_RAM_MIRROR },
{ ID_2E_090A_SET_NUMBER_ROAMING_FOR_SERVICE_CALL, ID_2E_090A_SET_NUMBER_ROAMING_FOR_SERVICE_CALL_MIN_LEN, ID_2E_090A_SET_NUMBER_ROAMING_FOR_SERVICE_CALL_MAX_LEN,  E2P_WRITE_OVER_RAM_MIRROR },
{ ID_2E_090B_SET_NUMBER_FOR_INFO_CALL,  ID_2E_090B_SET_NUMBER_FOR_INFO_CALL_MIN_LEN, ID_2E_090B_SET_NUMBER_FOR_INFO_CALL_MAX_LEN,  E2P_WRITE_OVER_RAM_MIRROR },
{ ID_2E_090C_SET_NUMBER_FOR_SERVICE_CALL,  ID_2E_090C_SET_NUMBER_FOR_SERVICE_CALL_MIN_LEN, ID_2E_090C_SET_NUMBER_FOR_SERVICE_CALL_MAX_LEN,  E2P_WRITE_OVER_RAM_MIRROR },
#ifdef UHV_ACTIVATE_FEATURE_SMS
{ ID_2E_0911_SMS_FUNCTIONALITY_OPERATING_UNIT,  ID_2E_0911_SMS_FUNCTIONALITY_OPERATING_UNIT_MIN_LEN, ID_2E_0911_SMS_FUNCTIONALITY_OPERATING_UNIT_MAX_LEN,  E2P_WRITE_OVER_RAM_MIRROR },
#endif
{ ID_2E_0914_BLUETOOTH_ACKNOWLEDGEMENT_SIGNAL,  ID_2E_0914_BLUETOOTH_ACKNOWLEDGEMENT_SIGNAL_MIN_LEN, ID_2E_0914_BLUETOOTH_ACKNOWLEDGEMENT_SIGNAL_MAX_LEN,  E2P_WRITE_OVER_RAM_MIRROR },
{ ID_2E_0A07_CDMA_IMSI_ON_OFF,  ID_2E_0A07_CDMA_IMSI_ON_OFF_MIN_LEN, ID_2E_0A07_CDMA_IMSI_ON_OFF_MAX_LEN,  E2P_WRITE_OVER_RAM_MIRROR },
{ ID_2E_1001_DEVELOPER_TESTMODE,  ID_2E_1001_DEVELOPER_TESTMODE_MIN_LEN, ID_2E_1001_DEVELOPER_TESTMODE_MAX_LEN,  E2P_WRITE_OVER_RAM_MIRROR },
{ ID_2E_2233_MASKING_OF_LANGUAGES,  ID_2E_2233_MASKING_OF_LANGUAGES_MIN_LEN, ID_2E_2233_MASKING_OF_LANGUAGES_MAX_LEN,  E2P_WRITE_OVER_RAM_MIRROR },
{ ID_2E_2416_UNIVERSAL_PREPARATION_FOR_MOBILE_TELEPHONE,  ID_2E_2416_UNIVERSAL_PREPARATION_FOR_MOBILE_TELEPHONE_MIN_LEN, ID_2E_2416_UNIVERSAL_PREPARATION_FOR_MOBILE_TELEPHONE_MAX_LEN,  E2P_WRITE_OVER_RAM_MIRROR },
{ ID_2E_241C_TELEPHONE_BASIC_VOLUME,  ID_2E_241C_TELEPHONE_BASIC_VOLUME_MIN_LEN, ID_2E_241C_TELEPHONE_BASIC_VOLUME_MAX_LEN,  E2P_WRITE_OVER_RAM_MIRROR },
{ ID_2E_241D_VOICE_OUTPUT_BASIC_VOLUME,  ID_2E_241D_VOICE_OUTPUT_BASIC_VOLUME_MIN_LEN, ID_2E_241D_VOICE_OUTPUT_BASIC_VOLUME_MAX_LEN,  E2P_WRITE_OVER_RAM_MIRROR },
{ ID_2E_241E_MICROPHONE_SENSITIVITY,  ID_2E_241E_MICROPHONE_SENSITIVITY_MIN_LEN, ID_2E_241E_MICROPHONE_SENSITIVITY_MAX_LEN,  E2P_WRITE_OVER_RAM_MIRROR },
{ ID_2E_2420_BLUETOOTH_SET_PIN_CODE,  ID_2E_2420_BLUETOOTH_SET_PIN_CODE_MIN_LEN, ID_2E_2420_BLUETOOTH_SET_PIN_CODE_MAX_LEN,  E2P_WRITE_OVER_RAM_MIRROR },
{ ID_2E_2422_USER_PROFILES,  ID_2E_2422_USER_PROFILES_MIN_LEN, ID_2E_2422_USER_PROFILES_MAX_LEN,  0 },
{ ID_2E_2423_MUTE_DELAY,  ID_2E_2423_MUTE_DELAY_MIN_LEN, ID_2E_2423_MUTE_DELAY_MAX_LEN,  E2P_WRITE_OVER_RAM_MIRROR},
{ ID_2E_2424_REFERENCE_CHANNEL_DELAY,  ID_2E_2424_REFERENCE_CHANNEL_DELAY_MIN_LEN, ID_2E_2424_REFERENCE_CHANNEL_DELAY_MAX_LEN,  E2P_WRITE_OVER_RAM_MIRROR},
{ ID_2E_2429_MEDIA_DEVICE_BASIC_VOLUME,  ID_2E_2429_MEDIA_DEVICE_BASIC_VOLUME_MIN_LEN, ID_2E_2429_MEDIA_DEVICE_BASIC_VOLUME_MAX_LEN,  E2P_WRITE_OVER_RAM_MIRROR },
{ ID_2E_2448_EMERGENCY_NUMBER,  ID_2E_2448_EMERGENCY_NUMBER_MIN_LEN, ID_2E_2448_EMERGENCY_NUMBER_MAX_LEN,  E2P_WRITE_OVER_RAM_MIRROR},
{ ID_2E_F15A_FINGERPRINT,  ID_2E_F15A_FINGERPRINT_MIN_LEN, ID_2E_F15A_FINGERPRINT_MAX_LEN,  VWDB_EE_ADDDRESS_RESERVED_01 },
{ ID_2E_F197_SYSTEMBEZEICHNUNG, ID_2E_F197_SYSTEMBEZEICHNUNG_MIN_LEN, ID_2E_F197_SYSTEMBEZEICHNUNG_MAX_LEN,  VWDB_EE_ADDDRESS_SysNameEngineType },
{ ID_2E_F198_WERKSTATTCODE_DAS_ANGESCHLOSSENEN_TESTERS,  ID_2E_F198_WERKSTATTCODE_DAS_ANGESCHLOSSENEN_TESTERS_MIN_LEN, ID_2E_F198_WERKSTATTCODE_DAS_ANGESCHLOSSENEN_TESTERS_MAX_LEN,  VWDB_EE_ADDDRESS_RepairShopCodeTSNbr },
{ ID_2E_F199_PROGRAMMING_DATE,  ID_2E_F199_PROGRAMMING_DATE_MIN_LEN, ID_2E_F199_PROGRAMMING_DATE_MAX_LEN,  VWDB_EE_ADDDRESS_ProgDate },
{ ID_2E_F19E_ASAM_ODX_DATEI_KENNZEICHNUNG, ID_2E_F19E_ASAM_ODX_DATEI_KENNZEICHNUNG_MIN_LEN, ID_2E_F19E_ASAM_ODX_DATEI_KENNZEICHNUNG_MAX_LEN,  VWDB_EE_ADDDRESS_AsamOdxFileID },
{ ID_2E_F1A0_VARIANTE_DES_PARAMETERSATZES, ID_2E_F1A0_VARIANTE_DES_PARAMETERSATZES_MIN_LEN, ID_2E_F1A0_VARIANTE_DES_PARAMETERSATZES_MAX_LEN,  VWDB_EE_ADDDRESS_DataSetNbr },
{ ID_2E_F1A1_VERSION_DES_PARAMETERSATZES,  ID_2E_F1A1_VERSION_DES_PARAMETERSATZES_MIN_LEN, ID_2E_F1A1_VERSION_DES_PARAMETERSATZES_MAX_LEN, VWDB_EE_ADDDRESS_DataVersionNbr },
{ ID_2E_F1A2_ASAM_ODX_DATEIVERSION,  ID_2E_F1A2_ASAM_ODX_DATEIVERSION_MIN_LEN, ID_2E_F1A2_ASAM_ODX_DATEIVERSION_MAX_LEN,  E2P_WRITE_OVER_RAM_MIRROR },
{ ID_2E_F1A3_HARDWAREVERSION,  ID_2E_F1A3_HARDWAREVERSION_MIN_LEN, ID_2E_F1A3_HARDWAREVERSION_MAX_LEN,  VWDB_EE_ADDDRESS_ECUHardwareVersionNbr },
{ ID_2E_F1A4_FAHRZEUGAUSSTATTUNGSCODE,  ID_2E_F1A4_FAHRZEUGAUSSTATTUNGSCODE_MIN_LEN, ID_2E_F1A4_FAHRZEUGAUSSTATTUNGSCODE_MAX_LEN,  VWDB_EE_ADDDRESS_VehEquipCodePRNbr },
{ ID_2E_F1AC_BAUZUSTANDSDOKUMENTATION, ID_2E_F1AC_BAUZUSTANDSDOKUMENTATION_MIN_LEN, ID_2E_F1AC_BAUZUSTANDSDOKUMENTATION_MAX_LEN,  VWDB_EE_ADDDRESS_VWEOLConfiguration },
};

/*----------------------------------------------------------------------------*/
/* Prototypes of file local Methods                                           */
/*----------------------------------------------------------------------------*/
static BooleanType _SDS_UDS_bWriteDiagnosisLengthCheck(
		Uint16Type u16ReceivedLen, const t_WriteDataIdCfgItem* DataIdItem);

static tSDSextWrite_Return SDSext_eWriteUserAndIOData(Uint8Type* data,
		const t_WriteDataIdCfgItem* DataIdItem, Uint16Type u16ReceivedLen);
static  BooleanType _SDS_UDS_bWriteDiagnosisRangeCheck(const Uint8Type* data,
		Uint16Type DataId, Uint16Type Len);
static BooleanType
_SDS_UDS_bIsValidPhoneNumber( const Uint8Type *pNumber, Uint16Type numberLen);
/*----------------------------------------------------------------------------*/
/* File local Code Macros                                                        */
/*----------------------------------------------------------------------------*/
/**
\brief	description of the macros
*/
/*----------------------------------------------------------------------------*/
/* File local const Definitions                                               */
/*----------------------------------------------------------------------------*/
/**
\brief	description of the private constant
*/
#define FALLBACK_LANGUAGE_MAX                         0x10        // $10: Turkish as fallbacklanguage
#define VOICE_CONTROL_MAX                             0x01        // $01: off
#define PHONEBOOK_SORT_ORDER                          0x01        // $01: Firstname
#define AUDIOPARAMETER_SET_MAX                        0x09        // $09: Set 10
#define MUTE_TYPE_MAX                                 0x01        // $01: off
#define MULTIMEDIA_FUNCTIONALITY_MAX                  0x01        // $01: off
#define AERIAL_DIAGNOSIS_MAX                          0x01        // $01: off
#define BASEPLATE_DIAGNOSIS_MAX                       0x02        // $01: off

#define MAX_EEPROM_WRITE_RETRIES_SERVICE_2E           1000 //used for Diagnose 2E Write E2P direct, numbers of retries to write in EEPROM
/*----------------------------------------------------------------------------*/
/* File local Variables                                                       */
/*----------------------------------------------------------------------------*/
/**
\brief	description of the private variable
*/
static BooleanType _SDS_UDS_bIsFingerprintValueValid = BT_FALSE;
static tSDSextWrite_Return eWriteInProgress;
static Uint16Type _SDS_UDS_u16EEpromWriting_2E_Control = 0;
Uint8Type ub_RequestPending = 0;
Uint16Type ubCounterResponseFromAppTask = 0;

/*
DEBUG_STATE_ENUM_NAME='tSDSextWrite_Return'  DEBUG_STATE_NAME='SDS_IO_USER_WRITE_SUCCESSFUL                     '  DEBUG_STATE_NUMBER='0'
DEBUG_STATE_ENUM_NAME='tSDSextWrite_Return'  DEBUG_STATE_NAME='SDS_IO_USER_WRITE_FAILED                         '  DEBUG_STATE_NUMBER='1'
DEBUG_STATE_ENUM_NAME='tSDSextWrite_Return'  DEBUG_STATE_NAME='SDS_IO_USER_WRITE_NOT_ALLOWED                    '  DEBUG_STATE_NUMBER='2'
DEBUG_STATE_ENUM_NAME='tSDSextWrite_Return'  DEBUG_STATE_NAME='SDS_IO_USER_WRITE_OUT_OF_RANGE                   '  DEBUG_STATE_NUMBER='3'
DEBUG_STATE_ENUM_NAME='tSDSextWrite_Return'  DEBUG_STATE_NAME='SDS_IO_USER_WRITE_REQUEST_SEQUENCE_ERROR         '  DEBUG_STATE_NUMBER='4'
DEBUG_STATE_ENUM_NAME='tSDSextWrite_Return'  DEBUG_STATE_NAME='SDS_IO_USER_WRITE_NO_RESPONSE_FROM_SUB_COMPONENT '  DEBUG_STATE_NUMBER='5'
DEBUG_STATE_ENUM_NAME='tSDSextWrite_Return'  DEBUG_STATE_NAME='SDS_IO_USER_WRITE_TRY_AGAIN                      '  DEBUG_STATE_NUMBER='6'
DEBUG_STATE_ENUM_NAME='tSDSextWrite_Return'  DEBUG_STATE_NAME='SDS_IO_USER_REPEAT_FOR_COMPLETE_E2P_WRITE        '  DEBUG_STATE_NUMBER='7'
*/

/*----------------------------------------------------------------------------*/
/* Private Address Calculations                                               */
/*----------------------------------------------------------------------------*/
/* Function    : SDS_UDS_vInitialiseService2E                                       */
/**
 \brief      To reset the Static variables

 \brief      Description:
 this function resets the static variables.


 \return     void
 */
/*----------------------------------------------------------------------------*/
void SDS_UDS_vInitialiseService2E(void)
{
	_SDS_UDS_bIsFingerprintValueValid = BT_FALSE; // Fingerprint ist ungültig, muss erneut gesendet werden.
	eWriteInProgress = SDS_IO_USER_WRITE_SUCCESSFUL;
}


/*----------------------------------------------------------------------------*/
/* Function    : _SDS_UDS_bIsValidPhoneNumber                                  */
/**
    \brief      checks if the given number is valid in length and contained
                characters

    \brief      checks if the given number is valid in length and contained
                characters

    \param      pNumber
                const Uint8Type *
                contains the number to be checked

    \param      numberLen
                Uint16Type
                number of bytes valid in pNumber

    \return     BooleanType
                BT_TRUE means number is valid, else BT_FALSE
*/
/*----------------------------------------------------------------------------*/
static BooleanType _SDS_UDS_bIsValidPhoneNumber( const Uint8Type *pNumber, Uint16Type numberLen)
{
    /* check len */
    // RT #1472: die Endekennung "F" wird nur bei ungerade Anzahl von Ziffern vom VAS-Tester gesendet
    // bei einer geraden Anzahl von Ziffern braucht der User kein "FF" eingeben, sondern dieses "FF" muss
    // automatisch von der UHV-SW erzeugt werden. Sollte als letztes Byte mit einem "F" in oberen Nibble beginnen,
    // dann wird dies bereits im vorgeschaltetem RangeCheck (s.o.) abgefangen
    Uint16Type i            = 0U;

    if((numberLen == 0U) || 
       (numberLen > ID_2E_090A_SET_NUMBER_ROAMING_FOR_SERVICE_CALL_MAX_LEN))
    {
        return BT_FALSE;
    }
    /* Check if first byte is 0xC0 or not in range of 0x0 and 0x09 */
    if (((pNumber[0] & 0xF0) != 0xC0) && (((pNumber[0] & 0xf0) > 0x90) ||  ((pNumber[0] & 0x0f) > 0x09)))
    {
        DEBUG_VALUE1(SDS_UDS_SWI_TRC, MOD_SDS_UDS, "invalid value in first byte 0x%02X",
                 pNumber[0]);
        return BT_FALSE;
    }
    for( i=1U; i<numberLen; i++)
    {
        if (((pNumber[i] & 0xf0) > 0x90) ||
            ((pNumber[i] & 0x0f) > 0x09))
        {
            /* Are we outside the allowed area ( BCD Code 00 to 99 ) */
        	if (
                (pNumber[i] == 0xFF )||
                ((pNumber[i] & 0x0f) == 0x0F)
                )
            {
        		// found the end of string character. Break the loop
        		break;
            }
        	else
        	{
                DEBUG_VALUE1(SDS_UDS_SWI_TRC, MOD_SDS_UDS, "invalid number sent from Tester 0x%02X",
                         pNumber[0]);
                return BT_FALSE;
        	}
        }
    }
    return BT_TRUE;
}

/*----------------------------------------------------------------------------*/
/* Function    : UDS_ubDiagService2E                                                 */
/**
 \brief      UDS Service Write Data By Identifier

 \brief      Description:
 this function writes data from VAS tester into eeprom or only in RAM for further processing

 \param      RxTxBuf
 data include ID from VAS tester

 \param      LenOfMsg
 length from VAS tester

 \param      Mode
 Type des funktionalen Requests: 2=UDS, 3=OBD  =>  NOT USED !!!

 \return     UDScub_FINISHED: Sendedaten liegen vor
 UDScub_REQUEST_OUT_OF_RANGE Fehlercode: wenn ein Parameter ungültig ist.
 UDScub_REQUEST_RECEIVED_RESPONSE_PENDING: Steuergerät benötigt noch etwas Zeit
 UDScub_ACCESS_DENIED: Login erforderlich
 SDS_UDS_GENERAL_PROGRAMMING_FAILURE: wird gesendet, wenn EEPROM nicht (momentan) beschreibbar ist
 SDS_UDS_REQUEST_SEQUENCE_ERROR: Fingerprint nicht gültig, bzw. noch nicht empfangen
 */
/*----------------------------------------------------------------------------*/
Uint8Type UDS_ubDiagService2E(Uint8Type* RxTxBuf, Uint16Type* LenOfMsg,Uint8Type Mode)
{
	Uint8Type i,j;
	Uint8Type retval = UDScub_FINISHED;
	Uint16Type u16ReceivedID = ((Uint16Type)RxTxBuf[0] << 8) + RxTxBuf[1];
	Uint8Type lower, upper;
	BooleanType found = BT_FALSE;
	Uint16Type u16ReceivedLen = LenOfMsg[0]-2; // "-2" weil die CAN-Nutzdaten auch den Identifier enthalten

	lower=0;
	i = 0;

	upper=(sizeof(cs_WriteDataIdItem)/sizeof(t_WriteDataIdCfgItem))-1;
	/* lower,i = first DID; upper = last DID */
	// die Überprüfung auf '-1' soll mögliche Fehlerquellen beseitigen,z.B. cs_WriteDataIdItem = 0.
	if (upper == 0xFF) // 0xFF = -1
		upper = 0;

	while (i < upper) /* stop searching if border reached */
	{
		/* get next interval */
		if (u16ReceivedID < cs_WriteDataIdItem[i].DataId)
		{
			upper = i; /* update upper border */
		}
		else
		{
			lower = i; /* update lower border */
		}
		i = (upper-lower); /* calculate the middle */
		i = lower + i/2 + i%2; /* of lower and upper border */
		if (cs_WriteDataIdItem[i].DataId == u16ReceivedID)
		{
			found = BT_TRUE; /* data identifier found */
			break; /* stop searching */
		}
		if (i == lower) /* check for termination condition */
		{
			if (u16ReceivedID == cs_WriteDataIdItem[i+1].DataId)
			{
				found = BT_TRUE; /* data identifier found */
				i++;
			}
			break; /* stop searching */
		}
		if (i == upper) /* check for termination condition */
		{
			if (u16ReceivedID == cs_WriteDataIdItem[i-1].DataId)
			{
				found = BT_TRUE; /* data identifier found */
				i--;
			}
			break; /* stop searching */
		}
	}

	if (found == BT_TRUE) /* insert data into the buffer if Data ID found */
	{
		if ( (u16ReceivedLen+2) <= uwBufSize) // "+2" weil hier der Identifier wieder mit dazu zählt
		{
			// hier erfolgt jetzt eine Überprüfung der Länge:
			if (_SDS_UDS_bWriteDiagnosisLengthCheck(u16ReceivedLen,
					&cs_WriteDataIdItem[i]) == BT_FALSE)
			{
				return UDScub_INCORRECT_MSG_LEN;
			}

			// zusätzlich muss hier noch der Spezial-Fall : dynamische Länge geprüft und entsprechend korrigiert werden.
			// Beispiel: wenn mit dem VAS-Tester der BT-Name geändert werden soll, wird folgendes Handling durchlaufen:
			// 1. Service $22 lies alle 40 Bytes aus dem EEPROM, d.h. diese Daten befinden sind in RxTxBuf
			// 2. In VAS-Tester wird ein neuer BT-Name (kürzer als der vorherige) eingegeben
			// 3. Service $2E schreibt immer die fixe Länge, d.h. 40 Bytes für den BT-Namen ins EEPROM.
			//    Also auch die restlichen Bytes aus RxTxBuf vom Service $22 (Read BT-Name) => RT#1444 ist dadurch entstanden.
			// Lösungsansatz: Der VAS-Tester überträgt keine Endekennung, also muss die UHV-SW einfach selbst die Endekennung '\0'  setzen,
			if ((cs_WriteDataIdItem[i].DataId
					== ID_2E_2416_UNIVERSAL_PREPARATION_FOR_MOBILE_TELEPHONE)
					&& (u16ReceivedLen
							< ID_2E_2416_UNIVERSAL_PREPARATION_FOR_MOBILE_TELEPHONE_MAX_LEN+2))
			{
				for (j=LenOfMsg[0]; j < ID_2E_2416_UNIVERSAL_PREPARATION_FOR_MOBILE_TELEPHONE_MAX_LEN+2; j++)
				{
					RxTxBuf[j] = '\0';
					u16ReceivedLen++;
				}
			}

			eWriteInProgress = SDSext_eWriteUserAndIOData(&RxTxBuf[2], &cs_WriteDataIdItem[i], u16ReceivedLen);
			if (ub_RequestPending > 0)
			{
				return UDScub_IN_PROCESS; // erst einmal auf Rückmeldung von den App-Task Routinen warten, bzw. Fingerprint und ProgDate schreiben
			}

			/* Handle response states to SDS-UDS layer */
			switch (eWriteInProgress)
			{
				case SDS_IO_USER_WRITE_SUCCESSFUL:
				{	RxTxBuf[0] = (Uint8Type)(u16ReceivedID >> 8);
					RxTxBuf[1] = (Uint8Type)u16ReceivedID;
					*LenOfMsg = 2;
					_SDS_UDS_u16EEpromWriting_2E_Control = 0;
					retval = UDScub_FINISHED;
					break;
				}
				case SDS_IO_USER_WRITE_FAILED:
				{
					_SDS_UDS_u16EEpromWriting_2E_Control = 0;
					retval = SDS_UDS_GENERAL_PROGRAMMING_FAILURE;
					break;
				}
				case SDS_IO_USER_WRITE_REQUEST_SEQUENCE_ERROR:
				{
					_SDS_UDS_u16EEpromWriting_2E_Control = 0;
					retval = SDS_UDS_REQUEST_SEQUENCE_ERROR;
					break;
				}
				case SDS_IO_USER_WRITE_OUT_OF_RANGE:
				{
					_SDS_UDS_u16EEpromWriting_2E_Control = 0;
					retval = UDScub_REQUEST_OUT_OF_RANGE;
					break;
				}
				case SDS_IO_USER_WRITE_NOT_ALLOWED:
				{
					_SDS_UDS_u16EEpromWriting_2E_Control = 0;
					retval = UDScub_ACCESS_DENIED;
					break;
				}
				case SDS_IO_USER_WRITE_NO_RESPONSE_FROM_SUB_COMPONENT:
				{
					_SDS_UDS_u16EEpromWriting_2E_Control = 0;
					retval = SDS_UDS_NORESPONSEFROMSUBNETCOMPONENT;
					break;
				}
				case SDS_IO_USER_WRITE_TRY_AGAIN:
				{
					retval = UDScub_REQUEST_RECEIVED_RESPONSE_PENDING;
					break;
				}

				default:
				{
					_SDS_UDS_u16EEpromWriting_2E_Control = 0;
					retval = UDScub_CONDITIONS_NOT_CORRECT;
					break;
				}
			}
		}
		else
		{
			_SDS_UDS_u16EEpromWriting_2E_Control = 0;
			retval = UDScub_REQUEST_OUT_OF_RANGE;
		}
	}
	else
	{
		_SDS_UDS_u16EEpromWriting_2E_Control = 0;
		retval = UDScub_REQUEST_OUT_OF_RANGE;
		DEBUG_VALUE2(SDS_UDS_SWI_TRC, MOD_SDS_UDS, "Service2E: value not found!!! %04X instead of %04X",
				cs_WriteDataIdItem[i].DataId, u16ReceivedID);
	}
	return (retval);
}


/*----------------------------------------------------------------------------*/
/* Function    : SDSext_eWriteUserAndIOData                                    */
/**
 \brief      Callback function for set interrupt bits

 \brief      Description
 function to set bits for the diferent interrupts

 \param      data
 address of buffer

 \param      DataIdItem
 pointer to the structure with configuration data of Data-Identifier

 \return     SDS_IO_USER_WRITE_SUCCESSFUL=0, \
                SDS_IO_USER_WRITE_FAILED,
 */
/*----------------------------------------------------------------------------*/
tSDSextWrite_Return SDSext_eWriteUserAndIOData(Uint8Type* data,
		const t_WriteDataIdCfgItem* DataIdItem, Uint16Type u16ReceivedLen)
{
	int i,j;
	static Uint8Type u8IODataE2PValue = 0;
	Uint8Type u8TmpDTC;
	tSDSextWrite_Return retVal = SDS_IO_USER_WRITE_SUCCESSFUL;
	EEDeviceControlBlockType *pEEHost = (EEDeviceControlBlockType *)getEEHostControl();

	// zunächst wird der Security Status überprüft
	switch (DataIdItem->DataId)
	{
		CASE_ALL_ID_SECURITY
		{
			if ((ubSecState & MASK_SEC_ACTIVE) != MASK_SEC_ACTIVE)
			{
				DEBUG_TEXT(SDS_UDS_SWI_TRC, MOD_SDS_UDS,"Denied access, No function activated");
				return SDS_IO_USER_WRITE_NOT_ALLOWED;
			}
			break;
		}
		case ID_2E_0500_ANALYSE_1:
		{
			if ( ((UDScub_EXTENTED_DIAG_SESSION == UDS_ubGetSession(UDS_CHANNEL_INDEX_2)) &&
					((ubSecState & MASK_SEC_ACTIVE) == MASK_SEC_ACTIVE) ) ||
					(UDScub_DEVELOPMENT_SESSION == UDS_ubGetSession(UDS_CHANNEL_INDEX_2)) ||
					(UDScub_EOL_SESSION == UDS_ubGetSession(UDS_CHANNEL_INDEX_2)))
			{
				//  Zugriff auf DTC Maskierung zulassen
				DEBUG_VALUE1(SDS_UDS_SWI_TRC, MOD_SDS_UDS,"Security access allowed %04X",DataIdItem->DataId);
			}
			else
			{
				DEBUG_VALUE1(SDS_UDS_SWI_TRC, MOD_SDS_UDS,"Security access NOT allowed %04X",DataIdItem->DataId);
				return SDS_IO_USER_WRITE_NOT_ALLOWED;
			}
			break;
		}
		default:
		{
			break;
		}
	}

	if (ub_RequestPending == 1)
	{
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
					return SDS_IO_USER_WRITE_NO_RESPONSE_FROM_SUB_COMPONENT;
			}
			else if (_SDS_UDS_stResponsePacketToVas.result
					== (Uint8Type)enAppDiagResponseWaitForCallBack)
			{
					ub_RequestPending = 0;
					ubCounterResponseFromAppTask = 0;
					DEBUG_VALUE1(SDS_UDS_SWI_ERR, MOD_SDS_UDS, "Give up WAit Retry %04X",
							_SDS_UDS_stRequestPacketFromVas.Identifier);
					return SDS_IO_USER_WRITE_NO_RESPONSE_FROM_SUB_COMPONENT;
			}
			else
			{
				ub_RequestPending = 0;
				ubCounterResponseFromAppTask = 0;
				if (_SDS_UDS_stResponsePacketToVas.result
						== (Uint8Type)enAppDiagResponseErrorImm)
				{
                    DEBUG_VALUE1(SDS_UDS_SWI_ERR, MOD_SDS_UDS, "Error %04X",
							_SDS_UDS_stRequestPacketFromVas.Identifier);

					return SDS_IO_USER_WRITE_NO_RESPONSE_FROM_SUB_COMPONENT; // das war wohl nix, sonst ab ins EEPROM...
				}
				// wenn wir hier angekommen sind, dann ist von seiten der Applikation alles okay
				ub_RequestPending = 2;
                DEBUG_VALUE1(SDS_UDS_SWI_TRC, MOD_SDS_UDS, "SUCCESS %04X",
						_SDS_UDS_stRequestPacketFromVas.Identifier);
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

				return SDS_IO_USER_WRITE_NO_RESPONSE_FROM_SUB_COMPONENT;
			}
			else
			{
				ubCounterResponseFromAppTask++;
			}
		}
	}
	else if ( ub_RequestPending < 10)
	{
		switch (DataIdItem->DataId)
		{
			CASE_ALL_ID_CODIERUNG
			{
				if (_SDS_UDS_bIsFingerprintValueValid == BT_FALSE)
				{
					/* The condition for correct coding is that the fingerprint should be successfully
					 * written to the SW. If it is not fullfilled then it is a Request Sequence Error. */
					DEBUG_TEXT(SDS_UDS_SWI_TRC, MOD_SDS_UDS, "Coding:FalschReihenfolge");
					return SDS_IO_USER_WRITE_REQUEST_SEQUENCE_ERROR;
				}

				if (ub_RequestPending == 2)
				{
					(void) EEPROM_sfRamWriteOnly(&_SDS_UDS_au8FingerprintValue[0], EEPROM_LAYOUT_ENUM_VWCodingRepairShopCodeOrSerialNumber );
					if ( _SDS_UDS_bProgrammingDateIsCorrect ==  BT_TRUE)
					{
						(void)EEPROM_sfRamWriteOnly(&_SDS_UDS_au8ProgrammingDate[0], EEPROM_LAYOUT_ENUM_VWCodingDate);
					}
					u8IODataE2PValue = ERM_GetMicGain();
					(void)EEPROM_sfRamWriteOnly(&u8IODataE2PValue, EEPROM_LAYOUT_ENUM_MicrophoneSensitivity);
					u8IODataE2PValue = ERM_GetAlcVolume();
					(void)EEPROM_sfRamWriteOnly(&u8IODataE2PValue, EEPROM_LAYOUT_ENUM_TelephoneBasicVolume);
					u8IODataE2PValue = ERM_GetPromptVolume();
					(void)EEPROM_sfRamWriteOnly(&u8IODataE2PValue, EEPROM_LAYOUT_ENUM_VoiceOutputBasicVolume);
					u8IODataE2PValue = ERM_GetStreamVolume();
					(void)EEPROM_sfRamWriteOnly(&u8IODataE2PValue, EEPROM_LAYOUT_ENUM_MediaDeviceBasicVolume);

					ub_RequestPending = 3;
					break;
				}
				else if (ub_RequestPending == 3)
				{

					ub_RequestPending = 4;
					break;
				}
				else if (ub_RequestPending == 4)
				{
					ub_RequestPending = 5;
					APP_CAN_vResetLanguageDatasetdownload();
					break;
				}
				else if (ub_RequestPending == 5)
				{
					// Prepare data coding is successful done
					(void)EEPROM_enGetSingleByteFromRamMirror(&u8IODataE2PValue, EEPROM_LAYOUT_ENUM_VWTesterCodingInformation);
					// Low nibble is length of coding
					// High nibble is for coding write successful done
					// Now add coding info write successful done with value 0x10 - High nibble value 0x1 is define as successful coding
					if (((u8IODataE2PValue & 0xF0)>> 4)!= 0x01)
					{
						u8IODataE2PValue = (u8IODataE2PValue | 0x10);
						(void)EEPROM_sfRamWriteOnly(&u8IODataE2PValue, EEPROM_LAYOUT_ENUM_VWTesterCodingInformation);
					}
					//Delete DTC not coded.
					ub_RequestPending = 10;
					semP(DEH_LockResource);
					(void)DEH_ClearEvent(DTC_D01300_SG_NICHTCODIERT);
					semV(DEH_LockResource);
					break;
				}

			}	// direkter Übergang wenn bei Switch-Eintritt ub_RequestPending == 0 ist.
			CASE_ALL_ID_ANPASSUNG
			{
				if ( DataIdItem->DataId == ID_2E_0500_ANALYSE_1 )
				{
					for (i=0; i < MAX_NUMBER_OF_DTC_MASKIERUNG; i++)
					{
						u8DTC_Maskierung[i] = data[i];
					}
					DEBUG_TEXT(SDS_UDS_SWI_TRC, MOD_SDS_UDS,"Set DTC Masking");
					SDS_UDS_u8DTC_Mask_Counter = 0;
					for (i=0; i<MAX_NUMBER_OF_DTC_MASKIERUNG; i++)
					{
						u8TmpDTC = u8DTC_Maskierung[i];
						for (j=0; j<8;j++)
						{
							if ((u8TmpDTC & DTC_MASK_BIT) == DTC_CLEAR_MASK_BIT)
							{  // hier den ausmaskierten DTC speichern
								DTC_Maskiert[SDS_UDS_u8DTC_Mask_Counter][0] = DTC_Mask[i*8+j][0];
								DTC_Maskiert[SDS_UDS_u8DTC_Mask_Counter][1] = DTC_Mask[i*8+j][1];
								DTC_Maskiert[SDS_UDS_u8DTC_Mask_Counter][2] = DTC_Mask[i*8+j][2];
								SDS_UDS_u8DTC_Mask_Counter++;
							}
							u8TmpDTC = u8TmpDTC << 1;
						}
					}
					(void)EEPROM_sfRamWriteOnly(&data[0], EEPROM_LAYOUT_ENUM_DTC_Maskierung);
					ub_RequestPending = 10;
					break;
				}

				else
				{
					if (_SDS_UDS_bIsFingerprintValueValid == BT_FALSE)
					{
						return SDS_IO_USER_WRITE_REQUEST_SEQUENCE_ERROR;
					}
					if (ub_RequestPending == 2)
					{
						(void)EEPROM_sfRamWriteOnly(&_SDS_UDS_au8FingerprintValue[0],  EEPROM_LAYOUT_ENUM_VWCalibrationRepairShopCodeOrSerialNumber );
						ub_RequestPending = 3;
						break;
					}
					else if (ub_RequestPending == 3)
					{
						if ( _SDS_UDS_bProgrammingDateIsCorrect ==  BT_TRUE)
						{
							(void)EEPROM_sfRamWriteOnly(&_SDS_UDS_au8ProgrammingDate[0],  EEPROM_LAYOUT_ENUM_VWCalibrationDate);
						}
						ub_RequestPending = 10;
						break;
					}
					// direkter Übergang wenn bei Switch-Eintritt ub_RequestPending == 0 ist.

					if (_SDS_UDS_bWriteDiagnosisRangeCheck(&data[0],DataIdItem->DataId,u16ReceivedLen)== BT_FALSE)
					{
						return SDS_IO_USER_WRITE_OUT_OF_RANGE;
					}
                    // CDMA handle over Anpassung an save over Ram to EEprom
					if (DataIdItem->DataId == ID_2E_0A07_CDMA_IMSI_ON_OFF)
					{
						DEBUG_TEXT(SDS_UDS_SWI_TRC, MOD_SDS_UDS,"Write: CDMA write");
						(void)EEPROM_sfRamWriteOnly(&data[0],EEPROM_LAYOUT_ENUM_CDMA_Detection);
						ub_RequestPending = 2;
					}
					else
					{
						ub_RequestPending = 1; // den App-Task Routinen Zeit zum Antworten geben.
						_SDS_UDS_stRequestPacketFromVas.Identifier = DataIdItem->DataId;
						_SDS_UDS_stRequestPacketFromVas.length = u16ReceivedLen;
						(void)memcpy(&_SDS_UDS_stRequestPacketFromVas.data[0], &data[0], _SDS_UDS_stRequestPacketFromVas.length);

						(void)SendMessage(msgAppDiagDiagnoseRequest, &_SDS_UDS_stRequestPacketFromVas);
					}
					break;
				}
			}
			case ID_2E_F198_WERKSTATTCODE_DAS_ANGESCHLOSSENEN_TESTERS:
			{
				for (i=0; i< ID_2E_F198_WERKSTATTCODE_DAS_ANGESCHLOSSENEN_TESTERS_MAX_LEN; i++)
					_SDS_UDS_au8FingerprintValue[i] = data[i];
				_SDS_UDS_bIsFingerprintValueValid = BT_TRUE;
				return (retVal);
			}
			case ID_2E_F199_PROGRAMMING_DATE:
			{
				for (i=0; i< ID_2E_F199_PROGRAMMING_DATE_MAX_LEN; i++)
					_SDS_UDS_au8ProgrammingDate[i] = data[i];
				_SDS_UDS_bProgrammingDateIsCorrect = BT_TRUE;
				if (_SDS_UDS_bWriteDiagnosisRangeCheck(&data[0],DataIdItem->DataId,u16ReceivedLen)== BT_FALSE)
				{
					_SDS_UDS_bProgrammingDateIsCorrect = BT_FALSE;
					return SDS_IO_USER_WRITE_OUT_OF_RANGE;
				}
				return (retVal);
			}
			case ID_2E_F1A2_ASAM_ODX_DATEIVERSION:
			{
				(void)EEPROM_sfRamWriteOnly(&data[0], EEPROM_LAYOUT_ENUM_AsamOdxFileVersion);
				ub_RequestPending = 10;
				break;
			}
			case ID_2E_F197_SYSTEMBEZEICHNUNG:
			case ID_2E_F19E_ASAM_ODX_DATEI_KENNZEICHNUNG:
			case ID_2E_F1A0_VARIANTE_DES_PARAMETERSATZES:
			case ID_2E_F1A1_VERSION_DES_PARAMETERSATZES:
			case ID_2E_F1A3_HARDWAREVERSION:
			case ID_2E_F1A4_FAHRZEUGAUSSTATTUNGSCODE:
			case ID_2E_F1AC_BAUZUSTANDSDOKUMENTATION:
			{
				ub_RequestPending = 10;
			}
			default:
			{
				break;
			}
		}
	}

	else if ( ub_RequestPending == 10 )
	{
#if 0
		Uint8Type i;
		for ( i = 0; i< u16ReceivedLen;i++)
		{
			DEBUG_VALUE1(SDS_UDS_SWI_TRC, MOD_SDS_UDS,"%02X" , data[i]);
		}
#endif
		if (DataIdItem->DataId == ID_2E_2422_USER_PROFILES)
		{
			_SDS_UDS_u8UserProfile = data[0];
			retVal = SDS_IO_USER_WRITE_SUCCESSFUL;
		}
		else
		{
			if ( DataIdItem->Address != E2P_WRITE_OVER_RAM_MIRROR )
			{
				if (_SDS_UDS_u16EEpromWriting_2E_Control == 0)
				{
					DEBUG_TEXT(SDS_UDS_SWI_TRC, MOD_SDS_UDS, "Start Service 2E write E2P data");
					_SDS_UDS_u16EEpromWriting_2E_Control = 1;
				}
				retVal = SDSext_EEPWriteUniversalData( (EEAddressType) DataIdItem->Address, u16ReceivedLen, &data[0]);
				DEBUG_STATE1(SDS_UDS_SWI_TRC, MOD_SDS_UDS, "EEprom Write ReturnCode -> $tSDSextWrite_Return$=%d", retVal );
			}
			else
			{
				retVal = SDS_IO_USER_WRITE_TRY_AGAIN;
				ub_RequestPending = 11;
			}
		}
		if ( retVal == SDS_IO_USER_WRITE_SUCCESSFUL)
		{
				ub_RequestPending = 12;
		}
		else if ( retVal == SDS_IO_USER_WRITE_FAILED )
		{
			/* Also in this cases we do a retry - this return codes describe a reason that E2P could not read a the moment but not why */
			/* Because of mixed return codes from lower level SW layers for E2P it necessary to do all things here                */
			DEBUG_VALUE1(SDS_UDS_SWI_TRC, MOD_SDS_UDS, "Writing EE-Data. Try: %d", _SDS_UDS_u16EEpromWriting_2E_Control);
			if (MAX_EEPROM_WRITE_RETRIES_SERVICE_2E < _SDS_UDS_u16EEpromWriting_2E_Control)
			{
				_SDS_UDS_u16EEpromWriting_2E_Control = 0;
				DEBUG_TEXT(SDS_UDS_SWI_TRC, MOD_SDS_UDS, "MAX EEPROM WRITE RETRIES reached - Stop. Phone->EEprom error");
				ub_RequestPending = 0;
				retVal=SDS_IO_USER_WRITE_FAILED; // MAX_EEPROM_WRITE_RETRIES hat das Schreiben nicht geklappt => jetzt ist Schluss...
			}
			else
			{
			   _SDS_UDS_u16EEpromWriting_2E_Control++;
			   retVal=SDS_IO_USER_WRITE_TRY_AGAIN;
			}
		}
		else if ( retVal == SDS_IO_USER_WRITE_TRY_AGAIN )
		{
			;// try again
		}
	}
	else if ( ub_RequestPending == 12 )
	{
		 /* we have to guarantee that data have write complete to eeprom      */
		/* otherwise it is possible that next SDS request can overwrite data */
		if ( EEPROM_flagIsEEFree(pEEHost) )
		{
			ub_RequestPending = 0;
			_SDS_UDS_u16EEpromWriting_2E_Control = 0;
			DEBUG_TEXT(SDS_UDS_SWI_TRC, MOD_SDS_UDS, "ServiceComplete write 2E data to E2P");
		}
		else
		{
			/* Waiting that data is complete write to E2P */
			DEBUG_VALUE1(SDS_UDS_SWI_TRC, MOD_SDS_UDS, "Still writing EE-Data. Try: %d", _SDS_UDS_u16EEpromWriting_2E_Control );
			if (MAX_EEPROM_WRITE_RETRIES_SERVICE_2E < _SDS_UDS_u16EEpromWriting_2E_Control)
			{
				_SDS_UDS_u16EEpromWriting_2E_Control = 0;
				ub_RequestPending = 0;
				DEBUG_TEXT(SDS_UDS_SWI_TRC, MOD_SDS_UDS, "MAX EEPROM WRITE RETRIES reached. Stop.");
				return SDS_IO_USER_WRITE_FAILED; // MAX_EEPROM_WRITE_RETRIES hat das Schreiben nicht geklappt => jetzt ist Schluss...
			}
			_SDS_UDS_u16EEpromWriting_2E_Control++;
		}
	}
	else if ( ub_RequestPending == 11 )
	{
		if ( EEPROM_LAYOUT_bAllBlocksEmpty == BT_TRUE )
		{
			DEBUG_TEXT(SDS_UDS_SWI_TRC, MOD_SDS_UDS, "RamServiceComplete");
			ub_RequestPending = 0;
		}
		else
		{
			; // Try again
		}
	}
#ifndef USE_DEBUGGING_WITH_TRACE_ID
	DEBUG_Printf(UHV_DEBUG_3, UHV_SDS_UDS, "Ret val Fn1_2E:%d", retVal);
#endif
	return (retVal);
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
static BooleanType _SDS_UDS_bWriteDiagnosisLengthCheck(
		Uint16Type u16ReceivedLen, const t_WriteDataIdCfgItem* DataIdItem)
{
	BooleanType bRetVal = BT_TRUE;

	if ((u16ReceivedLen >= DataIdItem->MinLen) && (u16ReceivedLen
			<= DataIdItem->MaxLen))
	{
		bRetVal = BT_TRUE;
		DEBUG_VALUE2(SDS_UDS_SWI_TRC, MOD_SDS_UDS,"2E WriteServ.: LengthCheckforID=%04X with length %d OK",DataIdItem->DataId,u16ReceivedLen);
		DEBUG_VALUE2(SDS_UDS_SWI_DBG, MOD_SDS_UDS,"min len=%d,max len=%d",DataIdItem->MinLen,DataIdItem->MaxLen);
	}
	else
	{
		bRetVal = BT_FALSE;
		DEBUG_VALUE2(SDS_UDS_SWI_ERR, MOD_SDS_UDS,"2E-WriteServ.: LengthCheckforID=%04X with length %d not okay",DataIdItem->DataId,u16ReceivedLen);
		DEBUG_VALUE2(SDS_UDS_SWI_ERR, MOD_SDS_UDS,"min len=%d,max len=%d",DataIdItem->MinLen,DataIdItem->MaxLen);
	}
	return (bRetVal);
}



/*----------------------------------------------------------------------------*/
/* Function    : _SDS_UDS_bWriteDiagnosisRangeCheck                                                 */
/**
 \brief      Write diagnosis range check

 \brief      Description:
 this function check the range from the values sends by the VAS tester

 \param      data
 values from VAS tester

 \param      DataId
 data ID

 \param      Len
 Length

 \return     BT_TRUE: range is okay
 BT_FALSE: range is not okay
 */
/*----------------------------------------------------------------------------*/
static BooleanType _SDS_UDS_bWriteDiagnosisRangeCheck(const Uint8Type* data,
		Uint16Type DataId, Uint16Type Len)
{
	BooleanType bRetVal = BT_TRUE;
	Uint8Type i = 0;
	Uint8Type ErrDataPrint = 0xFF;

	switch (DataId)
	{
		case ID_2E_0600_CODIERUNG: // ein falscher Wertebereich reicht, und falscher als falsch geht nicht...
		{
			/* If all the data is 0xFF or the length is lesser than 8 then it is a
			 * Request out of Range Error.*/
			for (i=0; i < ID_2E_0600_CODIERUNG_MAX_LEN; i++)
			{
				if (data[i] == 0xFF)
				{
					bRetVal = BT_FALSE;
					ErrDataPrint=data[i];
					break;
				}
			}
			if (data[0] > FALLBACK_LANGUAGE_MAX)
			{
				bRetVal = BT_FALSE;
				ErrDataPrint=data[0];
				i = 0;
			}

			// Low nibble is Voice control
			// High nibble is phonebook sort order
			else if (((data[1] & 0x0F) > VOICE_CONTROL_MAX) || (((data[1] & 0xF0) >> 4) > PHONEBOOK_SORT_ORDER))
			{
				bRetVal = BT_FALSE;
				ErrDataPrint=data[1];
				i = 1;
			}

			else if (data[2] > AUDIOPARAMETER_SET_MAX)
			{
				bRetVal = BT_FALSE;
				ErrDataPrint=data[2];
				i = 2;
			}

			else if (data[3] > MUTE_TYPE_MAX)
			{
				bRetVal = BT_FALSE;
				ErrDataPrint=data[3];
				i = 3;
			}

			else if (data[4] > MULTIMEDIA_FUNCTIONALITY_MAX)
			{
				bRetVal = BT_FALSE;
				ErrDataPrint=data[4];
				i = 4;
			}

			else if (data[5] > AERIAL_DIAGNOSIS_MAX)
			{
				bRetVal = BT_FALSE;
				ErrDataPrint=data[5];
				i = 5;
			}

			else if (data[6] > BASEPLATE_DIAGNOSIS_MAX)
			{
				bRetVal = BT_FALSE;
				ErrDataPrint=data[6];
				i = 6;
			}

			else if ((data[7] == APP_DIAG_ROOFLINE_MODULE_TYPE_NAR) ||
					(data[7] == APP_DIAG_ROOFLINE_MODULE_TYPE_RDW) ||
					(data[7] == APP_DIAG_ROOFLINE_MODULE_TYPE_NA))
			{
				; // is okay
			}
			else
			{
				bRetVal = BT_FALSE;
				i = 7;  /*  !!! because last else if  */
				ErrDataPrint=data[7];  /*  !!! because last else if  */
			}

			if (bRetVal == BT_FALSE)
			{
				DEBUG_VALUE1(SDS_UDS_SWI_ERR, MOD_SDS_UDS,"Write: Range Check for SID=%04X",DataId);
				DEBUG_VALUE2(SDS_UDS_SWI_ERR, MOD_SDS_UDS,"Byte=%d, Value=%d",i,ErrDataPrint);
			}
			break;
		}
		case ID_2E_241C_TELEPHONE_BASIC_VOLUME:
		{
			if (data[0] > AT_VOLUME_GEN_MAX)
			{
				bRetVal = BT_FALSE;
				DEBUG_VALUE1(SDS_UDS_SWI_ERR, MOD_SDS_UDS, "Write: Range Check for ID = %04X not okay",
						DataId);
				DEBUG_VALUE2(SDS_UDS_SWI_ERR, MOD_SDS_UDS, "received value = %02X, max value = %02X",
						data[0], AT_VOLUME_GEN_MAX);

			}
			break;
		}
		case ID_2E_241D_VOICE_OUTPUT_BASIC_VOLUME:
		{
			if (data[0] > AT_VOLUME_PROMPT_MAX)
			{
				bRetVal = BT_FALSE;
				DEBUG_VALUE1(SDS_UDS_SWI_ERR, MOD_SDS_UDS, "Write: Range Check for ID = %04X not okay",
						DataId);
				DEBUG_VALUE2(SDS_UDS_SWI_ERR, MOD_SDS_UDS, "received value = %02X, max value = %02X",
						data[0], AT_VOLUME_PROMPT_MAX);
			}
			if (data[0] < AT_VOLUME_PROMPT_MIN)
            {
                bRetVal = BT_FALSE;
                DEBUG_VALUE1(SDS_UDS_SWI_ERR, MOD_SDS_UDS, "Write: Range Check for ID = %04X not okay",
                        DataId);
                DEBUG_VALUE2(SDS_UDS_SWI_ERR, MOD_SDS_UDS, "received value = %02X, min value = %02X",
                        data[0], AT_VOLUME_PROMPT_MIN );
            }
			break;
		}
		case ID_2E_241E_MICROPHONE_SENSITIVITY:
			if (data[0] > AT_MIC_GAIN_MAX) {
				bRetVal = BT_FALSE;
				DEBUG_VALUE1(SDS_UDS_SWI_ERR, MOD_SDS_UDS, "Write: Range Check for ID = %04X not okay",
						DataId);
				DEBUG_VALUE2(SDS_UDS_SWI_ERR, MOD_SDS_UDS, "received value = %02X, max value = %02X",
						data[0], AT_MIC_GAIN_MAX);

			}
			break;

        case ID_2E_2423_MUTE_DELAY:
        {
            if (data[0] > 0x19 /* 0x19 == 2.5 Seconds */)
            {
                bRetVal = BT_FALSE;
            }
            break;
        }

		case ID_2E_2424_REFERENCE_CHANNEL_DELAY:
		{
			if (data[0] > AT_AEC_DELAY_MAX)
			{
				bRetVal = BT_FALSE;
			}
			break;
		}
		case ID_2E_2429_MEDIA_DEVICE_BASIC_VOLUME:
		{
			if (data[0] > AT_VOLUME_STREAM_MAX)
			{
				bRetVal = BT_FALSE;
				DEBUG_VALUE1(SDS_UDS_SWI_ERR, MOD_SDS_UDS, "Write: Range Check for ID = %04X not okay",
						DataId);
				DEBUG_VALUE2(SDS_UDS_SWI_ERR, MOD_SDS_UDS, "received value = %02X, max value = %02X",
						data[0], AT_VOLUME_STREAM_MAX);

			}
			break;
		}
		case ID_2E_1001_DEVELOPER_TESTMODE:
		case ID_2E_2422_USER_PROFILES:
#ifdef UHV_ACTIVATE_FEATURE_SMS
		case ID_2E_0911_SMS_FUNCTIONALITY_OPERATING_UNIT:
#endif
		case ID_2E_0A07_CDMA_IMSI_ON_OFF:
		case ID_2E_0914_BLUETOOTH_ACKNOWLEDGEMENT_SIGNAL:
		{
			if (data[0] > 1)
			{
				bRetVal = BT_FALSE;
				DEBUG_VALUE2(SDS_UDS_SWI_ERR, MOD_SDS_UDS, "Write: Range Check for ID = %04X not okay, received value = %02X, max value = 0x1",
						DataId, data[0]);
			}
			break;
		}
		case ID_2E_F199_PROGRAMMING_DATE:
		{
			/* No Check allowed because of VW 80125 V.2.3 */
			/* If for UHV a newer version of VW 80125 is true check of date have to uncommented */
			/*
			if ((data[0] == 0 ) ||  (data[1] == 0 ) || (data[2] == 0 ))
			{
				bRetVal = BT_FALSE;

				DEBUG_TEXT(SDS_UDS_SWI_ERR, MOD_SDS_UDS,
						"Write: Range Check for ID = %04X not okay, all recvd= 0"
						);
			}
			*/
			break;
		}
        case ID_2E_0909_SET_NUMBER_ROAMING_FOR_INFO_CALL:
		case ID_2E_090A_SET_NUMBER_ROAMING_FOR_SERVICE_CALL:
        case ID_2E_090B_SET_NUMBER_FOR_INFO_CALL:
        case ID_2E_090C_SET_NUMBER_FOR_SERVICE_CALL:
        case ID_2E_2448_EMERGENCY_NUMBER:
		{
            if (BT_FALSE == _SDS_UDS_bIsValidPhoneNumber(&data[0], Len))
            {
                DEBUG_VALUE2(SDS_UDS_SWI_TRC, MOD_SDS_UDS, "Write: Range Check for ID %04X not okay for Len = %d", DataId, Len);
                bRetVal = BT_FALSE;
            }
            break;
        }
		// TODO: next step: range check for all other ID's.
		default:
		{
			break;
		}
	}
	return (bRetVal);
}

/* End Of File SDS_UDS_Service2E_C1.c */
