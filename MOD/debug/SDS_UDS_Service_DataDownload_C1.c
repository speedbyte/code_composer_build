/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/
/** 
 * \file 	SDS_UDS_DataDownload_C1.c
 * \brief	This file contains the program to xxxx
 * \brief	This file fulfills the following requirements:
 * 			1. Requirement 1
 * 			2. Requirement 2
 * 			3. Requirement x
*/
/*-----------------------------------------------------------------------------
* $Date: 2011-02-17 16:31:23 +0100 (Do, 17 Feb 2011) $
* $Rev: 17980 $
* $Author: butz $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_SDS_UDS/trunk/src/SDS_UDS_Service_DataDownload_C1.c $
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
#include "EEPROM.h"
#include "EEPROM_LAYOUT_CFG.h"
#include "TRACE.h"
/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "SDS_ext.h"
#include "SDS_UDS_CI.h"
#include "SDS_UDS_Service_DataDownload_C1.id"

/*----------------------------------------------------------------------------*/
/* pragma Directives                                                          */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Definitions (avoid as much as possible!)                  */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Private Manifest Constant Definitions                                      */
/*----------------------------------------------------------------------------*/
/* return values of SDSext functions  */
typedef enum
{
SDS_SERVICE_FINISH_AN_AVAILABLE=0,     /* action e.q. function call okay */
SDS_SUB_FUNCTION_NOT_SUPPORTED,        /* subfunction not supported */
SDS_IMPROPER_UPLOAD_TYPE,              /* improper upload type */
SDS_WRONG_START_ADDRESS,
SDS_WRONG_NUMBER_OF_BYTES
} tSDSextStatus;

/*----------------------------------------------------------------------------*/
/* Private Types                                                              */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the type
*/

/*----------------------------------------------------------------------------*/
/* Prototypes of file local Methods                                           */
/*----------------------------------------------------------------------------*/
static tSDSextStatus _SDS_UDS_enCheckUpDownloadStartAddress(Uint8Type* pBuffer,	Uint8Type* DataFormatID);
static tSDSextStatus _SDS_UDS_enCheckUpDownloadEndAddress(Uint8Type* pBuffer);

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
static Uint8Type ubUpDownLoad = 0; // 0: kein Up-Down-Load aktiv, 1: Download ist aktiv, 2: Upload ist aktiv
/** 
\brief	description of the private variable
*/
static Uint8Type ubAnzBSN = 0;
/** 
\brief	description of the private variable
*/
static Uint8Type ubPrevBSN = 0;
/**
\brief	description of the private variable
*/
static Uint8Type ubRequestPendingMemory [Service34MaxNumberOfBlockLength + Service34IDplusBSN];
/** 
\brief	description of the private variable
*/
static Union32Type uploadMemorySize = { 0 };
/** 
\brief	description of the private variable
*/
static Union32Type uploadStartAddress = { 0 };
/** 
\brief	description of the private variable
*/
static Uint8Type u8ECUDataProgInfo[1] = { 0 };
/** 
\brief	description of the private variable
*/
static Uint8Type ubVWDataCounter[2] = { 0, 0 };
/** 
\brief	description of the private variable
*/
static Uint32Type _SDS_UDS_u32CalculatedCRC = 0;
/** 
\brief	description of the private variable
*/
static Uint32Type _SDS_UDS_u32ReceivedCRC = 47119876;
/** 
\brief	description of the private variable
*/
static Uint8Type ubChecksumState = FALSE;
/**
\brief	description of the private variable
*/

/*----------------------------------------------------------------------------*/
/* File local const Definitions                                               */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the private constant
*/
#define SDS_UDS_MAX_CHECKSUM_BYTES            4

//data download status handling
#define RESET_DATADOWNLOADSTATUS		0xFE
#define SET_DATADOWNLOADSTATUS			0x01

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

/*----------------------------------------------------------------------------*/
/* Function    : SDS_UDS_vInitialiseServiceDataDownload                       */
/** 
 \brief      To reset the Static variables
 
 \brief      Description:
 this function resets the static variables.

  
 \return     void 
 */
/*----------------------------------------------------------------------------*/
void SDS_UDS_vInitialiseServiceDataDownload(void) 
{
	ubUpDownLoad = 0;
}

/*  FUNCTION Uint8Type _SDS_UDS_vChecksumStart
 **************************************************************************
 *  PURPOSE:        startet die Überprüft der Checksumme
 **************************************************************************
 *  PARAMETERS:     
 *                  
 *                  
 **************************************************************************
 *  RETURN VALUE:   speichert das Ergebnis in ubChecksumState
 *************************************************************************/
void SDS_UDS_vChecksumStart(void)
{
	if (_SDS_UDS_u32CalculatedCRC == _SDS_UDS_u32ReceivedCRC)
	{
		ubChecksumState = TRUE;
		DEBUG_VALUE1(SDS_UDS_SWI_TRC, MOD_SDS_UDS,"Checksumme okay: _SDS_UDS_u32CalculatedCRC = %08X",_SDS_UDS_u32CalculatedCRC);
		DEBUG_VALUE1(SDS_UDS_SWI_TRC, MOD_SDS_UDS,", _SDS_UDS_u32ReceivedCRC = %08X",_SDS_UDS_u32ReceivedCRC);
	}
	else
	{
		ubChecksumState = FALSE;
		DEBUG_VALUE1(SDS_UDS_SWI_ERR, MOD_SDS_UDS,
					"Checksumme ERROR: _SDS_UDS_u32CalculatedCRC = %08X",_SDS_UDS_u32CalculatedCRC);
		DEBUG_VALUE1(SDS_UDS_SWI_ERR, MOD_SDS_UDS,", _SDS_UDS_u32ReceivedCRC = %08X",_SDS_UDS_u32ReceivedCRC);
	}
	_SDS_UDS_u32CalculatedCRC = 0;
}

/*  FUNCTION Uint8Type _SDS_UDS_ubChecksumStatus
 **************************************************************************
 *  PURPOSE:        gibt den Status der Checksummen Überprüfung zurück
 **************************************************************************
 *  PARAMETERS:     
 *                  
 *                  
 **************************************************************************
 *  RETURN VALUE:   ubChecksumState: valid or not valid
 *************************************************************************/
Uint8Type _SDS_UDS_ubChecksumStatus(void)
{
	if(ubChecksumState)
	{
		// 1. Reset Bit Ungultige EEPROM Datensatz
		(void)EEPROM_enGetSingleByteFromRamMirror(&u8ECUDataProgInfo[0], EEPROM_LAYOUT_ENUM_ECUDataProgrammingInformation);
		u8ECUDataProgInfo[0] &= ~SET_UNGULTIGE_EEPROM_DATEN;
		(void)EEPROM_sfRamWriteOnly(&u8ECUDataProgInfo[0], EEPROM_LAYOUT_ENUM_ECUDataProgrammingInformation);

		// 2.Erfolgreicherprogrammierversuche in 040A für den DS erhöhen
		(void)EEPROM_enGetByteSequenceFromRamMirror(&ubVWDataCounter[0], VWDB_EE_ADDDRESS_VWDataCounterOfSuccessfulProgrammingAttempts_LEN, EEPROM_LAYOUT_ENUM_VWDataCounterOfSuccessfulProgrammingAttempts);
		if(0xFF == ubVWDataCounter[1])
		{
			ubVWDataCounter[0]++;
		}
		ubVWDataCounter[1]++;
		(void)EEPROM_sfRamWriteOnly( &ubVWDataCounter[0], EEPROM_LAYOUT_ENUM_VWDataCounterOfSuccessfulProgrammingAttempts);

		// 3. Delete DTC Unglutige Datensatz
		semP(DEH_LockResource);
		(void)DEH_ClearEvent(DTC_A00500_UNGUELTIGER_DATENSATZ);
		semV(DEH_LockResource);
	}

	return ubChecksumState;
}


/***************************************************************************
 *  PURPOSE:        UDS_ubDiagService35
 **************************************************************************
 *  PARAMETERS:     see UDS_ubDiagService10
 **************************************************************************
 *  RETURN VALUE:   see UDS_ubDiagService10
 *************************************************************************/
#define Service35DataFormatIdentifier          RxTxBuf[0]
#define Service35UnCompressed                  0

#define Service35AddressAndLengthFormatID      RxTxBuf[1]

#define Service35MemoryAddress                 RxTxBuf[2]
#define Service35UnCompressedMemorySize        RxTxBuf[6]

#define Service35IDplusBSN                     2             // ServiceIdentifier und BlockSequenceNumber (BSN)
#define Service35MaxNumberOfBlockLength        8             // die maximale Anzahl der in jeder TransferData-Message enthaltenen Nutz-Datenmenge
Uint8Type UDS_ubDiagService35(Uint8Type* RxTxBuf, Uint16Type* LenOfMsg, Uint8Type Mode)
{
	tSDSextStatus retSDS = SDS_SUB_FUNCTION_NOT_SUPPORTED;
	Mode = Mode;

	if ((ubSecState & MASK_SEC_DD_ACTIVE) != MASK_SEC_DD_ACTIVE)
		return UDScub_ACCESS_DENIED;

	retSDS = _SDS_UDS_enCheckUpDownloadStartAddress(&Service35MemoryAddress,&Service35DataFormatIdentifier);
	if (retSDS == SDS_SERVICE_FINISH_AN_AVAILABLE)
	{
#ifdef UpLoadFuerEntwicklung
		RxTxBuf[6] = 0;
		RxTxBuf[7] = 0;
		RxTxBuf[8] = 0x01;
		RxTxBuf[9] = 0x90;
#endif
		retSDS = _SDS_UDS_enCheckUpDownloadEndAddress(&Service35UnCompressedMemorySize);
		if (retSDS == SDS_SERVICE_FINISH_AN_AVAILABLE)
		{
			if (uploadMemorySize.u32 <= SDS_UDS_DEVELOPMENT_FUNCTION_END_ADDRESS)
			{
				ubUpDownLoad = 2; // 0: kein Up-Down-Load aktiv, 1: Download ist aktiv, 2: Upload ist aktiv
				*LenOfMsg = 2;
				RxTxBuf[0] = 0x10;
				RxTxBuf[1] = Service35IDplusBSN + Service35MaxNumberOfBlockLength; // damit werden 8 NutzUint8Types pro Service $36 gesendet, schöner wäre eine komplette Page (32 Bytes) zuzulassen
			} // aber momentan schein ist dann ein Problem mit dem CAN zu geben Stand 22.10.2008 => noch klären
		}
	}
	return UDScub_FINISHED;
}

/***************************************************************************
 *  PURPOSE:        UDS_ubDiagService36
 **************************************************************************
 *  PARAMETERS:     see UDS_ubDiagService10
 **************************************************************************
 *  RETURN VALUE:   see UDS_ubDiagService10
 *************************************************************************/

#define Service36BlockSequenceNumber          RxTxBuf[0]

Uint8Type UDS_ubDiagService36(Uint8Type* RxTxBuf, Uint16Type* LenOfMsg, Uint8Type Mode)
{
	int i;
	Uint16Type u16UploadAddress = 0;

	tSDSextWrite_Return retVal = SDS_IO_USER_WRITE_FAILED;

	if ((ubSecState & MASK_SEC_DD_ACTIVE) != MASK_SEC_DD_ACTIVE)
		return UDScub_ACCESS_DENIED;

	if (ubUpDownLoad == 1)
	{
		if (ub_RequestPending == 0)
		{
			ubRequestPendingMemory[0] = (Uint8Type) *LenOfMsg -1; // die max. Länge wird nienals mehr als ein Byte, d.h. 255 sein
			ubRequestPendingMemory[1] = Service36BlockSequenceNumber;

			if (ubRequestPendingMemory[1] == ubPrevBSN)
			{
				//same BSN with prev transfer, return SUCCESS
				ub_RequestPending = 1;
				return UDScub_IN_PROCESS;
			}
			else if ( (ubRequestPendingMemory[1] != (ubPrevBSN+1)) ||
					  (ubRequestPendingMemory[1] > ubAnzBSN) )
			{
				return SDS_UDS_WRONG_BLOCK_SEQUENCE_NUMBER;
			}
			else
			{
				for (i=2; i < ubRequestPendingMemory[0]+2; i++)
				{
					ubRequestPendingMemory[i] = RxTxBuf[i-1];
					if(ubRequestPendingMemory[1] != ubAnzBSN)
					{
						_SDS_UDS_u32CalculatedCRC += RxTxBuf[i-1];
					}
				}
				if(ubRequestPendingMemory[1] != ubAnzBSN)
				{
					_SDS_UDS_u32CalculatedCRC ^= FIX_LOGIN_OFFSET;
				}

				ub_RequestPending = 2; // jetzt das erste mal versuchen zu schreiben
				DEBUG_TEXT(SDS_UDS_SWI_TRC, MOD_SDS_UDS, "Start DSD write E2P data next block");
			}
		}

		if (ub_RequestPending == 1)
		{
			*LenOfMsg = 1;
			RxTxBuf[0] = ubRequestPendingMemory[1];
			ub_RequestPending = 0;
			return UDScub_FINISHED; // damit wurden 10ms nach dem Schreiben gewartet
		}

		if (ub_RequestPending > 1)
		{
			u16UploadAddress = uploadStartAddress.u16[1] +(ubRequestPendingMemory[1] * Service34MaxNumberOfBlockLength) - Service34MaxNumberOfBlockLength;

			if (u16UploadAddress < EEPROM_ADDR_MAX /*- Service34MaxNumberOfBlockLength */)
			{
				/* There is some DEH data in the pipe to write to eeprom. This have to do first  */
				/* DEH write EEprom data have to be register before we overwrite all eeprom data */
				if ( DEH_EE_NOTHING_TODO == APL_DEH_IsDEHData_SuccessfulRegisterToEEprom())
				{
					retVal=SDSext_EEPWriteUniversalData_WithRetryMechanism((EEAddressType)u16UploadAddress,
																		   ubRequestPendingMemory[0],(Uint8Type*) &ubRequestPendingMemory[2]);
					DEBUG_STATE1(SDS_UDS_SWI_TRC, MOD_SDS_UDS, "EEprom Write ReturnCode -> $tSDSextWrite_Return$=%d", retVal );
				}
				else
				{
					DEBUG_TEXT(SDS_UDS_SWI_TRC, MOD_SDS_UDS, "DEH data is in the pipe. This have to do first");
					retVal = SDS_IO_USER_WRITE_TRY_AGAIN;
				}
			}
			else
			{
				if (u16UploadAddress == EEPROM_ADDR_MAX)
				{
					/* CRC Info received */
					 retVal = SDS_IO_USER_WRITE_SUCCESSFUL;
				}
				else
				{
					/* we are out of scope of the E2P addresses */
					ub_RequestPending = 0;
					return UDScub_REQUEST_OUT_OF_RANGE;
				}
			}

			if (SDS_IO_USER_WRITE_SUCCESSFUL == retVal)
			{
				//Block successful downloaded
				DEBUG_VALUE1(SDS_UDS_SWI_TRC, MOD_SDS_UDS, "DSD write E2P data with address %02X is finished",u16UploadAddress);
				ubPrevBSN++;
				ub_RequestPending = 1; // merke: nur einmal schreiben
				return UDScub_IN_PROCESS;
			}
			else if(SDS_IO_USER_WRITE_FAILED == retVal)
			{
				ub_RequestPending = 0;
				return SDS_UDS_GENERAL_PROGRAMMING_FAILURE; /* MAX EEPROM WRITE RETRIES - STOP */

			}
			else
			{
				ub_RequestPending++; // used for internal retries
				return UDScub_IN_PROCESS;
			}
		}
		else
		{
		   return SDS_UDS_GENERAL_PROGRAMMING_FAILURE;
		}
	}
	else if (ubUpDownLoad == 2)
	{
		(void)SDSext_EEPReadUniversalData(uploadStartAddress.u16[1]+((Service36BlockSequenceNumber-1)* Service35MaxNumberOfBlockLength),
	                                      Service35MaxNumberOfBlockLength, &RxTxBuf[1]);

#ifdef UpLoadFuerEntwicklung
		DEBUG_VALUE1(SDS_UDS_SWI_ERR, MOD_SDS_UDS, "Address: %d= ",
							uploadStartAddress.u16[1]+((Service36BlockSequenceNumber-1) * Service35MaxNumberOfBlockLength));
		DEBUG_VALUE4(SDS_UDS_SWI_ERR, MOD_SDS_UDS, "%d %d %d %d ",
							RxTxBuf[1],
							RxTxBuf[2],
							RxTxBuf[3],
							RxTxBuf[4]);
		DEBUG_VALUE4(SDS_UDS_SWI_ERR, MOD_SDS_UDS, "%d %d %d %d",
							RxTxBuf[5],
							RxTxBuf[6],
							RxTxBuf[7],
							RxTxBuf[8]);
#endif
		*LenOfMsg = Service35IDplusBSN + Service35MaxNumberOfBlockLength;
		return UDScub_FINISHED;
	}
	else
	{
		return SDS_UDS_REQUEST_SEQUENCE_ERROR;//RequestUp/Download not received
	}
}

/***************************************************************************
 *  PURPOSE:        UDS_ubDiagService34
 **************************************************************************
 *  PARAMETERS:     see UDS_ubDiagService10
 **************************************************************************
 *  RETURN VALUE:   see UDS_ubDiagService10
 *************************************************************************/
#define Service34DataFormatIdentifier          RxTxBuf[0]
#define Service34UnCompressed                  0

#define Service34AddressAndLengthFormatID      RxTxBuf[1]

#define Service34MemoryAddress                 RxTxBuf[2]
#define Service34UnCompressedMemorySize        RxTxBuf[6]

Uint8Type UDS_ubDiagService34(Uint8Type* RxTxBuf, Uint16Type* LenOfMsg, Uint8Type Mode)
{
	tSDSextStatus retSDS = SDS_SUB_FUNCTION_NOT_SUPPORTED;
	Mode = Mode;

	if ((ubSecState & MASK_SEC_DD_ACTIVE) != MASK_SEC_DD_ACTIVE)
		return UDScub_ACCESS_DENIED;

	if (ub_RequestPending == 0)
	{
		retSDS = _SDS_UDS_enCheckUpDownloadStartAddress( &Service34MemoryAddress, &Service34DataFormatIdentifier);
		if (retSDS == SDS_SERVICE_FINISH_AN_AVAILABLE)
		{
			retSDS = _SDS_UDS_enCheckUpDownloadEndAddress(&Service34UnCompressedMemorySize);
			if (retSDS == SDS_SERVICE_FINISH_AN_AVAILABLE)
			{
				if (uploadMemorySize.u32 <= SDS_UDS_DEVELOPMENT_FUNCTION_END_ADDRESS)
				{
					ub_RequestPending = 1;
					return UDScub_IN_PROCESS;//send response pending only if necessary
				}
				else
				{
					DEBUG_VALUE1(SDS_UDS_SWI_ERR, MOD_SDS_UDS, "Download: uploadMemorySize exceeded = %d",
										uploadMemorySize.u32);
					//size is not acceptable for us, out of our range
					return SDS_UDS_UPLOAD_DOWNLOAD_NOT_ACCEPTED;
				}
			}
			else
			{
				DEBUG_TEXT(SDS_UDS_SWI_ERR, MOD_SDS_UDS, "Download: End Address failed");
				//end address is wrong
				return SDS_UDS_UPLOAD_DOWNLOAD_NOT_ACCEPTED;
			}
		}
		else
		{
			DEBUG_TEXT(SDS_UDS_SWI_ERR, MOD_SDS_UDS, "Download: Start Address failed");
			//start address or compression type is wrong
			return SDS_UDS_UPLOAD_DOWNLOAD_NOT_ACCEPTED;
		}
	}
	else if(ub_RequestPending == 1)
	{
		// 1.Store a DTC "Datensatz ungültig"
		semP(DEH_LockResource);
		(void)DEH_AdditionalReportEvent(DTC_A00500_UNGUELTIGER_DATENSATZ, TRUE);
		semV(DEH_LockResource);

		// 2.Fingerprint schreiben
		(void)EEPROM_sfRamWriteOnly(&_SDS_UDS_au8FingerprintValue[0],EEPROM_LAYOUT_ENUM_VWDataSetRepairShopCodeOrSerialNumber);

		// 3.Datum schreiben
		(void)EEPROM_sfRamWriteOnly(&_SDS_UDS_au8ProgrammingDate[0],EEPROM_LAYOUT_ENUM_VWDataSetProgrammingDate);

		// 4.Programmierversuche in 0409 für den DS erhöhen
		(void)EEPROM_enGetByteSequenceFromRamMirror(&ubVWDataCounter[0], VWDB_EE_ADDDRESS_VWDataCounterOfProgrammingAttempts_LEN, EEPROM_LAYOUT_ENUM_VWDataCounterOfProgrammingAttempts);
		if(0xFF == ubVWDataCounter[1])
		{
			ubVWDataCounter[0]++;
		}
		ubVWDataCounter[1]++;
		DEBUG_VALUE2(SDS_UDS_SWI_TRC, MOD_SDS_UDS, "Download: Progr.versuche Cnt1=%d, Cnt0=%d",
				ubVWDataCounter[1], ubVWDataCounter[0]);
		(void)EEPROM_sfRamWriteOnly( &ubVWDataCounter[0], EEPROM_LAYOUT_ENUM_VWDataCounterOfProgrammingAttempts);

		// 5.+6. Set Bit Ungultige EEPROM Daten and Bit Fehler
		(void)EEPROM_enGetSingleByteFromRamMirror(&u8ECUDataProgInfo[0], EEPROM_LAYOUT_ENUM_ECUDataProgrammingInformation);
		u8ECUDataProgInfo[0] |= SET_UNGULTIGE_EEPROM_DATEN;
		u8ECUDataProgInfo[0] |= SET_FEHLER_EEPROM_DATEN;
		(void)EEPROM_sfRamWriteOnly(&u8ECUDataProgInfo[0], EEPROM_LAYOUT_ENUM_ECUDataProgrammingInformation);

		//initialization, variable used for Service36 BSN handling
		ubPrevBSN = 0;

		ubUpDownLoad = 1; // 0: kein Up-Down-Load aktiv, 1: Download ist aktiv, 2: Upload ist aktiv

		ub_RequestPending = 0;

		// jetzt die OKAY Bestätigungen an den Client absenden
		*LenOfMsg = 2;
		RxTxBuf[0] = 0x10;
		RxTxBuf[1] = Service34IDplusBSN + Service34MaxNumberOfBlockLength; // damit werden 8 NutzUint8Types pro Service $36 gesendet, schöner wäre eine komplette Page (32 Bytes) zuzulassen

	}

	return UDScub_FINISHED;

}


/***************************************************************************
 *  PURPOSE:        UDS_ubDiagService37
 **************************************************************************
 *  PARAMETERS:     see UDS_ubDiagService10
 **************************************************************************
 *  RETURN VALUE:   see UDS_ubDiagService10
 *************************************************************************/

Uint8Type UDS_ubDiagService37(Uint8Type* RxTxBuf, Uint16Type* LenOfMsg,
		Uint8Type Mode)
{
	Mode = Mode;

	ubUpDownLoad = 0;

	if ((ubSecState & MASK_SEC_DD_ACTIVE) != MASK_SEC_DD_ACTIVE)
		return UDScub_ACCESS_DENIED;

	_SDS_UDS_u32ReceivedCRC = (ubRequestPendingMemory[2] << 24) + (ubRequestPendingMemory[3] << 16) +
							  (ubRequestPendingMemory[4] << 8) + ubRequestPendingMemory[5];

	DEBUG_VALUE1(SDS_UDS_SWI_TRC, MOD_SDS_UDS, "UDS_ubDiagService37: Received CheckSum = %08X",
			     _SDS_UDS_u32ReceivedCRC);
	DEBUG_VALUE1(SDS_UDS_SWI_TRC, MOD_SDS_UDS, "UDS_ubDiagService37: Calculated CheckSum = %08X",
				     _SDS_UDS_u32CalculatedCRC);

	// 1. Reset Bit Fehler
	(void)EEPROM_enGetSingleByteFromRamMirror(&u8ECUDataProgInfo[0], EEPROM_LAYOUT_ENUM_ECUDataProgrammingInformation);
	u8ECUDataProgInfo[0] &= ~SET_FEHLER_EEPROM_DATEN;
	(void)EEPROM_sfRamWriteOnly(&u8ECUDataProgInfo[0], EEPROM_LAYOUT_ENUM_ECUDataProgrammingInformation);

	//check if received checksum equal calculated checksum
	if(_SDS_UDS_u32ReceivedCRC != _SDS_UDS_u32CalculatedCRC)
	{
		//checksum is wrong
		DEBUG_VALUE1(SDS_UDS_SWI_TRC, MOD_SDS_UDS, "UDS_ubDiagService37: received CRC = %08X wrong", _SDS_UDS_u32ReceivedCRC);
	}
	ubSecState &= ~MASK_SEC_DD_ACTIVE;
	ubSecState |= MASK_SEC_ACTIVE;
	return UDScub_FINISHED;

}

/*-----------------------------------------------------------------------*
 * function:     tSDS_Status _SDS_UDS_enCheckUpDownloadEndAddress( Uint8Type* pBuffer )
 * description:  RequestTransferExit
 * in:           -
 * out:          -
 * inout:        -
 * global_in:    -
 * global_out:   -
 * global_inout: -
 * return:       - SDS_SUB_FUNCTION_NOT_SUPPORTED
 *-----------------------------------------------------------------------*/
static tSDSextStatus _SDS_UDS_enCheckUpDownloadEndAddress(Uint8Type* pBuffer) {
	/* return variable for this function with a negative default value   */
	tSDSextStatus retSDS = (tSDSextStatus) SDS_SUB_FUNCTION_NOT_SUPPORTED;
	uploadMemorySize.u8[U32U8LSB] = pBuffer[0];
	uploadMemorySize.u8[U32U8LLSB] = pBuffer[1];
	uploadMemorySize.u8[U32U8MMSB] = pBuffer[2];
	uploadMemorySize.u8[U32U8MSB] = pBuffer[3];

	DEBUG_VALUE1(SDS_UDS_SWI_ERR, MOD_SDS_UDS, "received DownloadEndAddress = %d",
				uploadMemorySize.u32);
	DEBUG_VALUE1(SDS_UDS_SWI_ERR, MOD_SDS_UDS, "start address =%d",
				uploadStartAddress.u32);

	if ((uploadStartAddress.u32 + uploadMemorySize.u32) > SDS_UDS_DEVELOPMENT_FUNCTION_END_ADDRESS)
	{
		uploadStartAddress.u32 = 0;
		uploadMemorySize.u32 = 0;
		retSDS = SDS_WRONG_NUMBER_OF_BYTES;
	}
	else
	{
		retSDS = SDS_SERVICE_FINISH_AN_AVAILABLE;
		ubAnzBSN = (uploadMemorySize.u32 / Service34MaxNumberOfBlockLength) + 1;
	}

	return retSDS;
}

/*-----------------------------------------------------------------------*
 * function:     tSDS_Status _SDS_UDS_enCheckUpDownloadStartAddress( Uint8Type* pBuffer, Uint8Type* DataFormatID )
 * description:  RequestTransferExit
 * in:           -
 * out:          -
 * inout:        -
 * global_in:    -
 * global_out:   -
 * global_inout: -
 * return:       - SDS_SUB_FUNCTION_NOT_SUPPORTED
 *-----------------------------------------------------------------------*/
static tSDSextStatus _SDS_UDS_enCheckUpDownloadStartAddress(Uint8Type* pBuffer, Uint8Type* DataFormatID)
{
	/* return variable for this function with a "negative" default value   */
	tSDSextStatus retSDS = SDS_SUB_FUNCTION_NOT_SUPPORTED;

	uploadStartAddress.u8[U32U8LSB] = pBuffer[0];
	uploadStartAddress.u8[U32U8LLSB] = pBuffer[1];
	uploadStartAddress.u8[U32U8MMSB] = pBuffer[2];
	uploadStartAddress.u8[U32U8MSB] = pBuffer[3];

	if	(uploadStartAddress.u32 <= SDS_UDS_DEVELOPMENT_FUNCTION_END_ADDRESS)
	{
		if (DataFormatID[0] == Service34UnCompressed)
		{
			retSDS = SDS_SERVICE_FINISH_AN_AVAILABLE;
		}
		else
		{
			uploadStartAddress.u32 = 0;
			retSDS = SDS_IMPROPER_UPLOAD_TYPE;
		}
	}
	else
	{
		uploadStartAddress.u32 = 0;
		retSDS = SDS_WRONG_START_ADDRESS;
	}

	return retSDS;
}

/*-----------------------------------------------------------------------*
 * function:     BooleanType SDS_UDS_GetDataDownloadStatus( void )
 * description:  DataDownload status information for EEPROM write protect functionality
 * in:           -
 * out:          -
 * inout:        -
 * global_in:    -
 * global_out:   -
 * global_inout: -
 * return:       - DataDownloadStatus information
 *-----------------------------------------------------------------------*/
BooleanType SDS_UDS_GetAdditionalDataDownloadStatus( void )
{
	if( (ubSecState & MASK_SEC_DD_ACTIVE) == MASK_SEC_DD_ACTIVE )
	{
		return BT_TRUE;
	}
	else
	{
		return BT_FALSE;
	}
}
/* End Of File SDS_UDS_DataDownload_C1.c */
