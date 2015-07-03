/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/
/** 
 * \file 	SDS_UDS_REadWriteEEProm_C1.c
 * \brief	This file contains the program to xxxx
 * \brief	This file fulfills the following requirements:
 * 			1. Requirement 1
 * 			2. Requirement 2
 * 			3. Requirement x
*/
/*-----------------------------------------------------------------------------
* $Date: 2009-10-23 16:24:52 +0200 (Fr, 23 Okt 2009) $
* $Rev: 9340 $
* $Author: agrawal $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_SDS_UDS/trunk/src/SDS_UDS_REadWriteEEProm_C1.c $
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
#include "OSEK_HostTask_H1.h"
#include "sds.h"
#include "VCR.h"
#include "EEPROM_LAYOUT_CFG.h"
#include "sds_ext.h"
#include "APP_DIAG.h"
#include "sds_cfg.h"
#include "DEH.h"
#include "TRACE.h"

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "SDS_ext.h"
#include "SDS_UDS_CI.h"
#include "SDS_UDS_ReadWriteEEProm_C1.id"

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

/*----------------------------------------------------------------------------*/
/* Private Manifest Constant Definitions                                      */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Prototypes of file local Methods                                           */
/*----------------------------------------------------------------------------*/
static tSDSextWrite_Return SDSext_EEPWriteHandleRetries(EEDeviceControlBlockType *pEEHost,CommonResponseCodeType *rC_E2P);

/*----------------------------------------------------------------------------*/
/* File local Code Macros                                                        */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the macros
*/
/*----------------------------------------------------------------------------*/
/* File local const Definitions                                               */
/*----------------------------------------------------------------------------*/
#define MAX_EEPROM_WRITE_RETRIES_SERVICE_W           1000 //used for Diagnose 2E Write E2P direct, numbers of retries to write in EEPROM

/*----------------------------------------------------------------------------*/
/* File local Variables                                                       */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the private variable
*/

/*----------------------------------------------------------------------------*/
/* Private Address Calculations                                               */
/*----------------------------------------------------------------------------*/
tSDSextRead_Return SDSext_EEPReadUniversalData(uint16_t EEP_Position, uint16_t BlockSize_u16, uint8_t *Buffer_u8)
{
	tSDSextRead_Return retVal = SDSext_IO_USER_READ_SUCCESSFUL;
	CommonResponseCodeType retVal_E2P = COMMON_RSP_OK;
	SuccessFailType sF = SUCCESS;
	EEDeviceControlBlockType *pEEHost = (EEDeviceControlBlockType *)getEEHostControl();
	FlagType flag    = EEPROM_flagIsEEFree(pEEHost);

	if( flag == TRUE)
	{
		DEBUG_VALUE2(SDS_UDS_SWI_DBG, MOD_SDS_UDS,"EEP Read Address = %02X, Len = %d", EEP_Position, BlockSize_u16);

		retVal_E2P = EEPROM_sfRead(pEEHost, EEP_Position, &Buffer_u8[0], BlockSize_u16);
		if ( retVal_E2P == COMMON_RSP_OK )
		{
			sF = SUCCESS;
		}
		else
		{
			sF = FAIL;
		}
		if (SUCCESS == sF)
		{
			retVal = SDSext_IO_USER_READ_SUCCESSFUL;
		}
		else
		{
			retVal = SDSext_IO_USER_READ_FAILED;
			DEBUG_TEXT(SDS_UDS_SWI_TRC, MOD_SDS_UDS,"E2P Read FAIL");
		}
	}
	else
	{
		retVal = SDSext_IO_USER_READ_IN_PROGRESS;
	}
	return (retVal);
}

/*============================================================================
 function name    :  SDSext_EEPWriteUniversalData()
 input parameter  :  EEP_Position  : write position in Byte-Array
 BlockSize     : size of Uint8Types to read from memory
 Buffer        : pointer where the source Uint8Types are standing.

 output parameter :   -
 globals          :
 return value     :  tSDSextRead_Return
 description      :  Function to write universal data into the EEPROM
 ------------------------------------------------------------------------------
 date        short     change description
 ------------------------------------------------------------------------------
 08.11.03    GIGE    built base version

 ============================================================================*/
tSDSextWrite_Return SDSext_EEPWriteUniversalData(uint16_t EEP_Position,	uint16_t BlockSize_u16, uint8_t *Buffer_u8)
{
	tSDSextWrite_Return retVal = SDS_IO_USER_WRITE_SUCCESSFUL;
	CommonResponseCodeType rC = COMMON_RSP_OK;
	EEDeviceControlBlockType *pEEHost = (EEDeviceControlBlockType *)getEEHostControl();
	FlagType flag = EEPROM_flagIsEEFree(pEEHost);

	if( flag == TRUE )
	{
		DEBUG_VALUE2(SDS_UDS_SWI_DBG, MOD_SDS_UDS,"EEP Write Address = %02X, Len = %d", EEP_Position, BlockSize_u16);

		rC = EEPROM_LAYOUT_rcWrite(pEEHost, EEP_Position, &Buffer_u8[0], BlockSize_u16,	NULL);
		if (rC == COMMON_RSP_OK)
		{
			retVal = SDS_IO_USER_WRITE_SUCCESSFUL;
		}
		else
		{
			retVal = SDS_IO_USER_WRITE_FAILED;
		}
	}
	else
	{
		retVal = SDS_IO_USER_WRITE_TRY_AGAIN;
	}
	return (retVal);
}

/*----------------------------------------------------------------------------*/
/* Function    : SDSext_EEPWriteUniversalData_WithRetryMechanism              */
/**
    \brief      Write data to E2P with retry mechanism

    \param      EEP_Position
                Uint8Type
                Write start position of E2P

    \param      BlockSize_u16
                uint16_t
                number of bytes to write to E2P

	\param      *Buffer_u8
                uint8_t
                Data to write to E2P

	\return     tSDSextWrite_Return
                Info about write status of E2P
*/
/*----------------------------------------------------------------------------*/
tSDSextWrite_Return SDSext_EEPWriteUniversalData_WithRetryMechanism(uint16_t EEP_Position,	uint16_t BlockSize_u16, uint8_t *Buffer_u8)
{
	EEDeviceControlBlockType *pEEHost = (EEDeviceControlBlockType *)getEEHostControl();
	static CommonResponseCodeType rC_E2P = COMMON_RSP_ERROR_NOT_INITIALIZED;

	/* Is E2P busy go back direct - (EEPROM_flagIsEEFree(pEEHost) != TRUE)                            */
	/* (rC_E2P == COMMON_RSP_OK) - successful register to write E2P but write is not complete done !! */
	if((EEPROM_flagIsEEFree(pEEHost) == TRUE) && (rC_E2P != COMMON_RSP_OK) )
	{
		rC_E2P = EEPROM_LAYOUT_rcWrite(pEEHost, EEP_Position, &Buffer_u8[0], BlockSize_u16,	NULL);
	}

	return (SDSext_EEPWriteHandleRetries(pEEHost,&rC_E2P));
}

/*----------------------------------------------------------------------------*/
/* Function    : SDSext_EEPWriteHandleRetries                                 */
/**
    \brief      E2P handle retry mechanism

    \param      *pEEHost
                EEDeviceControlBlockType
                Access to  E2p Control block

    \param      rC_E2P
                return code from E2P write routine

	\return     tSDSextWrite_Return
                Info about write status of E2P
*/
/*----------------------------------------------------------------------------*/
tSDSextWrite_Return SDSext_EEPWriteHandleRetries(EEDeviceControlBlockType *pEEHost,CommonResponseCodeType *rC_E2P)
{
	tSDSextWrite_Return retVal = SDS_IO_USER_WRITE_FAILED;
	static uint16_t _SDS_UDS_u16EEpromWriting_W_Control= 0;

	/* E2P sucessful complete write done */
	if((EEPROM_flagIsEEFree(pEEHost) == TRUE) && (*rC_E2P == COMMON_RSP_OK) )
	{
		_SDS_UDS_u16EEpromWriting_W_Control = 0;
		*rC_E2P = COMMON_RSP_ERROR_NOT_INITIALIZED;
		return(SDS_IO_USER_WRITE_SUCCESSFUL);
	}

    if (MAX_EEPROM_WRITE_RETRIES_SERVICE_W < _SDS_UDS_u16EEpromWriting_W_Control)
	{
		_SDS_UDS_u16EEpromWriting_W_Control = 0;
		*rC_E2P = COMMON_RSP_ERROR_NOT_INITIALIZED;
		DEBUG_TEXT(SDS_UDS_SWI_TRC, MOD_SDS_UDS, "MAX EEPROM WRITE RETRIES reached - Stop. Phone->EEprom error");
		retVal=SDS_IO_USER_WRITE_FAILED; // MAX_EEPROM_WRITE_RETRIES - write not work -> Stop
	}
	else
	{
	   /* Retry read E2P again - do this with response pending */
	    _SDS_UDS_u16EEpromWriting_W_Control++;
	    if (*rC_E2P == COMMON_RSP_OK)
	    {
	    	retVal=SDS_IO_USER_REPEAT_FOR_COMPLETE_E2P_WRITE;
	    }
	    else
	    {
	    	retVal=SDS_IO_USER_WRITE_TRY_AGAIN;
	    }
	}

    return (retVal);
}

/* End Of File SDS_UDS_REadWriteEEProm_C1.c */
