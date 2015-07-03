/**************************************************************************
 *  COPYRIGHT (C) In2Soft GmbH, 2005-2007                                 *
 *  COPYRIGHT (C) Volkswagen AG, 2006-2007                                *
 *  ALLE RECHTE VORBEHALTEN. ALL RIGHTS RESERVED.                         *
 *                                                                        *
 **************************************************************************
 *  FILE: UDS_APPL.C
 **************************************************************************
 *  PURPOSE:
 *  Example application for UDS communications
 *
 *************************************************************************
 *  CHANGE HISTORY: In2Soft GmbH
 *
 *  Version    Name     Date   Description
 **************************************************************************
 *  V_00_01_00 Ki 12.05.2005 - VW/Audi conformance, first version 
 *  V_00_01_01 Ki 30.05.2005 - Adjustments due to the comments by Audi 
 *  V_00_01_02 Ki 03.06.2005 - Further adjustments 
 *  V_00_01_03 Ki 07.06.2005 - C++ comments eliminated (MISRA conformance)  
 *  V_00_01_04 Ki 22.07.2005 - Configuration matching demo application  
 *  V_00_01_05 Ki 25.07.2005 - Bug-Fix: Service $22  
 *  V_00_01_06 Ki 09.08.2005 - no change
 *  V_00_01_07 Ki 19.09.2005 - string.h, uds_cfg.h, UDS_TASK_CYCLE
 *                           - Mode, UDScub_CANCELED
 *  V_00_01_08 Ki 05.12.2005 - no change
 *  V_01_00_00 Ki 18.01.2006 - typedefs.h --> i2s_types.h
 *  V_01_01_00 Ki 09.03.2006 - no change
 *  V_01_02_00 Ki 02.06.2006 - Multi client capability
 *  V_01_02_01 Ki 04.09.2006 - General revision based on review (by Audi) 
 *  V_01_02_02 Ki 10.10.2006 - Buffer configuration: UDS_CLIENTS == 1
 *  V_01_02_03 Ki 02.11.2006 - string.h deleted
 *                           - SDS compliance
 *                           - UDS_ubDiagService27 for 78h test
 *  V_01_02_04 Ki 19.12.2006 - use of i2s type instead of C types
 *                           - i2s_type.h removed
 *  V_01_02_05 Ki 12.01.2007 - sds_cfg.h include configurable
 *  V_01_02_06 Ki 22.01.2007 - no change
 *  V_01_02_07 Ki 02.03.2007 - general revision, review by Audi
 *  V_01_02_08 Ki 30.03.2007 - sds.h included
 *  V_01_02_09 Ki 02.07.2007 - no change 
 *  V_01_02_10 Ki 11.07.2007 - no change 
 *  V_01_02_11 Ki 07.08.2007 - no change 
 *  V_01_02_12 Ki 02.11.2007 - no change
 *  V_01_02_13 Ki 18.06.2008 - no change 
 *  V_01_02_14 Ki 23.07.2008 - no change
 *************************************************************************/

#include "uds_appl.h"
#include "uds.h"
#if( UDS_STANDARD_SERVICES_USED > 0 ) 
#include "sds.h"
#include "sds_cfg.h"
#include "SDS_UDS_CI.h"
#endif
#include "uds_appl.id"
#include "TRACE.h"

/*************************************************************************
 * VARIABLES
 *************************************************************************/
static word ruw_TestTimer;
byte MsgBuf[BUF_SIZE];   
#if( UDS_CLIENTS == 2 )
static word ruw_TestTimer_ECM;
byte MsgBuf_ECM[BUF_SIZE];
#endif

#if( UDS_STANDARD_SERVICES_USED == 0 ) 
#define SDS_DISABLED 0
#define SDS_A_START_DIAGNOSTIC_SESSION  SDS_DISABLED
#define SDS_A_RESET_OR_DISCONNECT       SDS_DISABLED
#define SDS_A_CLEAR_DIAGNOSTIC_INF      SDS_DISABLED
#define SDS_A_READ_DATA_BY_IDENTIFIER   SDS_DISABLED
#define SDS_A_READ_DATA_BY_IDENTIFIER   SDS_DISABLED
#define SDS_A_COMMUNICATION_CONTROL     SDS_DISABLED
#define SDS_A_TESTER_PRESENT            SDS_DISABLED
#define SDS_A_CTRL_DTC_SETTINGS         SDS_DISABLED
#endif

#define i2s_ch 0

/***************************************************************************
 *  PURPOSE:        This function is called by the diagnostic module in case
 *                  of a diagnostic timeout P3_MAX.
 **************************************************************************
 *  PARAMETERS:     -
 **************************************************************************
 *  RETURN VALUE:   -
 *************************************************************************/
void UDS_vDiagTimeout( void )
{
  ruw_TestTimer = 0;
  /* do your action here in case of an timeout */
}


/***************************************************************************
 *  PURPOSE:        Diagnostic Service Handler
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
#if( SDS_A_START_DIAGNOSTIC_SESSION == SDS_DISABLED )
byte UDS_ubDiagService10( byte* RxTxBuf, word* LenOfMsg, byte Mode)
{
  if( *LenOfMsg != 1)
  {
    return UDScub_REQUEST_OUT_OF_RANGE;
  }
  if( UDS_ubSetSession(UDS_CHANNEL_INDEX RxTxBuf[0]) == I2S_FALSE )
  {
    return UDScub_CONDITIONS_NOT_CORRECT;
  }
  return UDScub_FINISHED;
}
#endif


/*************************************************************************
 *  PURPOSE:        Diagnostic Service Handler
 *************************************************************************
 *                  
 *************************************************************************/
#if( SDS_A_RESET_OR_DISCONNECT == SDS_DISABLED )
byte UDS_ubDiagService11( byte* RxTxBuf, word* LenOfMsg, byte Mode )
{
  if( LenOfMsg[0] != 1 )                   /* check length of the request */
  {
    return UDScub_REQUEST_OUT_OF_RANGE;
  }
  return UDScub_FINISHED;                  /* send response */
}
#endif


/*************************************************************************
 *  PURPOSE:        UDS_ubDiagService14
 *************************************************************************
 *                  send response pending messages
 *************************************************************************/
#if( SDS_A_CLEAR_DIAGNOSTIC_INF == SDS_DISABLED )
byte UDS_ubDiagService14( byte* RxTxBuf, word* LenOfMsg, byte Mode )
{
  LenOfMsg[0] = 0;
  return UDScub_FINISHED;    
}
#endif


/*************************************************************************
 *  PURPOSE:        UDS_ubDiagService19
 *************************************************************************
 *                  send response pending messages
 *************************************************************************/
#if( SDS_A_READ_DATA_BY_IDENTIFIER == SDS_DISABLED )
byte UDS_ubDiagService19( byte* RxTxBuf, word* LenOfMsg, byte Mode )
{
  return UDScub_CONDITIONS_NOT_CORRECT;    
}
#endif


/***************************************************************************
 *  PURPOSE:        UDS_ubDiagService22
 *************************************************************************/
#if( SDS_A_READ_DATA_BY_IDENTIFIER == SDS_DISABLED )
byte UDS_ubDiagService22( byte* RxTxBuf, word* LenOfMsg, byte Mode )
{
  if( LenOfMsg[0] < 2 )
  {
    return UDScub_REQUEST_OUT_OF_RANGE;
  }
  /* check 2 bytes data identifier here */
  if( (RxTxBuf[0] == 0xF1) && (RxTxBuf[1] == 0x90) )
  {
    memcpy( &RxTxBuf[2], "VIN: Audi B8 - Test Applikation", sizeof("VIN: Audi B8 - Test Applikation"));
    LenOfMsg[0] = sizeof("VIN: Audi B8 - Test Applikation")+2;  /* 2 bytes data identifier */
    return UDScub_FINISHED;    
  }
  return UDScub_SUBFUNCTION_NOT_SUPPORTED;   /* send response  */
}
#endif


/***************************************************************************
 *  PURPOSE:        UDS_ubDiagService23
 *************************************************************************/
#ifndef SDS_SW_TEST
#ifdef SDSEXT_SERVICE23_ALLOWED
#define Service23AddressAndLengthFormatID                RxTxBuf[0]
#define Service23MemoryAddress                           ((RxTxBuf[1] << 8) + RxTxBuf[2])
#define Service23MemorySize                              RxTxBuf[3]
#define Service23SupportedAddressAndLengthFormatID       0x12  // es wird nur ein Uint16Type für die Adresse und ein Uint8Type (d.h. max. 255 Zeichen) unterstützt !!!
#endif

Uint8Type SDS_UDS_ubDiagService23(Uint8Type* RxTxBuf, Uint16Type* LenOfMsg, Uint8Type Mode)
{
	tSDSextRead_Return retVal = SDSext_IO_USER_READ_SUCCESSFUL;
	static Uint16Type _UDS_16ReceivedReadE2PLength = 0;

#ifdef SDSEXT_SERVICE23_ALLOWED
	_UDS_16ReceivedReadE2PLength = Service23MemorySize;
	if( *LenOfMsg != (Uint16Type)4 )
	{
		Mode = Mode; /* prevent compiler warnings */
		return UDScub_INCORRECT_MSG_LEN;
	}
	else
	{
		if (Service23AddressAndLengthFormatID != Service23SupportedAddressAndLengthFormatID)
		{
			return UDScub_INCORRECT_MSG_LEN;
		}
		else
		{
			retVal = SDSext_EEPReadUniversalData (Service23MemoryAddress, Service23MemorySize, &RxTxBuf[0]);
			DEBUG_VALUE1(UDS_SWI_TRC, MOD_UDS, "EEprom Read Return Code %d", retVal );
			if( retVal == SDSext_IO_USER_READ_SUCCESSFUL )
			{
				/* give back length of data for the response */
				*LenOfMsg = _UDS_16ReceivedReadE2PLength;
				return UDScub_FINISHED;
			}
			else if( retVal == SDSext_IO_USER_READ_IN_PROGRESS )
			{
				return UDScub_IN_PROCESS;
			}
			else if( retVal == SDSext_IO_USER_READ_FAILED)
			{
				DEBUG_TEXT(UDS_SWI_TRC, MOD_UDS,"EEprom Read Error");
				return UDScub_BUSY_REPEAT_REQUEST;
			}
			else if( retVal == SDSext_IO_USER_READ_NOT_ALLOWED)
			{
				DEBUG_TEXT(UDS_SWI_TRC, MOD_UDS,"Security Access needs");
				return UDScub_ACCESS_DENIED;
			}
			else
			{
			    /* All other */
				DEBUG_TEXT(UDS_SWI_TRC, MOD_UDS,"Not defined. Give Diag. error back");
				return UDScub_BUSY_REPEAT_REQUEST;
			}
		}
	}

#else
	return UDScub_SUBFUNCTION_NOT_SUPPORTED;
#endif

}
#endif

/***************************************************************************
 *  PURPOSE:        UDS_ubDiagService27
 *************************************************************************/
byte UDS_ubDiagService27( byte* RxTxBuf, word* LenOfMsg, byte Mode )
{
  if( RxTxBuf[0] == 0x01 )
  {
    LenOfMsg[0] = 5;
    return UDScub_FINISHED;    
  }
  else if( RxTxBuf[0] == 0x02 )
  {
    if( ruw_TestTimer )
    { 
      ruw_TestTimer -= cuw_UDS_TASK_CYCLE;
      if( ruw_TestTimer )
      { 
        return UDScub_IN_PROCESS;
      }
      LenOfMsg[0] = 2;
      RxTxBuf[1] = 0x34;
      return UDScub_FINISHED;    
    }
    ruw_TestTimer = 1000;  /* delay 1000ms */
    return UDScub_IN_PROCESS;
  }
  return UDScub_SUBFUNCTION_NOT_SUPPORTED;   /* send response  */
}


/*************************************************************************
 *  PURPOSE:        UDS_ubDiagService28
 *************************************************************************
 *                  send response pending messages
 *************************************************************************/
#if( SDS_A_COMMUNICATION_CONTROL == SDS_DISABLED )
byte UDS_ubDiagService28( byte* RxTxBuf, word* LenOfMsg, byte Mode )
{
  return UDScub_CONDITIONS_NOT_CORRECT;    
}
#endif


/***************************************************************************
 *  PURPOSE:        UDS_ubDiagService3E
 *************************************************************************/
#if( SDS_A_TESTER_PRESENT == SDS_DISABLED )
byte UDS_ubDiagService3E( byte* RxTxBuf, word* LenOfMsg, byte Mode )
{
  /* send just an echo */
  return UDScub_FINISHED;    
}
#endif

/*************************************************************************
 *  PURPOSE:        UDS_ubDiagService85
 *************************************************************************
 *                  send response pending messages
 *************************************************************************/
#if( SDS_A_CTRL_DTC_SETTINGS == SDS_DISABLED )
byte UDS_ubDiagService85( byte* RxTxBuf, word* LenOfMsg, byte Mode )
{
  return UDScub_CONDITIONS_NOT_CORRECT;    
}
#endif


#if( UDS_CLIENTS > 1 )
/*************************************************************************
 *  PURPOSE:        Diagnostic Timeout Handler
 *************************************************************************/
void UDS_vDiagTimeout_ECM( void )
{
  ruw_TestTimer_ECM = 0;
}

/*************************************************************************
 *  PURPOSE:        Diagnostic Service Handler
 *************************************************************************/
byte UDS_ubDiagService10_ECM( byte* RxTxBuf, word* LenOfMsg, byte Mode)
{
  if( *LenOfMsg != 1)
  {
    return UDScub_REQUEST_OUT_OF_RANGE;
  }
  if( UDS_ubSetSession(1, RxTxBuf[0]) == 0 )
  {
    return UDScub_CONDITIONS_NOT_CORRECT;
  }
  return UDScub_FINISHED;
}


/*************************************************************************
 *  PURPOSE:        Diagnostic Service Handler
 *************************************************************************/
byte UDS_ubDiagService11_ECM( byte* RxTxBuf, word* LenOfMsg, byte Mode )
{
  if( LenOfMsg[0] != 1 )                   /* check length of the request */
  {
    return UDScub_REQUEST_OUT_OF_RANGE;
  }
  return UDScub_FINISHED;                  /* send response */
}


/*************************************************************************
 *  PURPOSE:        UDS_ubDiagService14
 *************************************************************************/
byte UDS_ubDiagService14_ECM( byte* RxTxBuf, word* LenOfMsg, byte Mode )
{
  if( ruw_TestTimer_ECM )
  { 
    ruw_TestTimer_ECM -= cuw_UDS_TASK_CYCLE;
    if( ruw_TestTimer_ECM )
    { 
      return UDScub_IN_PROCESS;
    }
    LenOfMsg[0] = 0;
    return UDScub_FINISHED;    
  }
  ruw_TestTimer_ECM = 1000;  /* delay 1000ms */
  return UDScub_IN_PROCESS;
}

/***************************************************************************
 *  PURPOSE:        UDS_ubDiagService22
 *************************************************************************/
byte UDS_ubDiagService22_ECM( byte* RxTxBuf, word* LenOfMsg, byte Mode )
{
  if( LenOfMsg[0] < 2 )
  {
    return UDScub_REQUEST_OUT_OF_RANGE;
  }
  /* check 2 bytes data identifier here */
  if( (RxTxBuf[0] == 0xF1) && (RxTxBuf[1] == 0x90) )
  {
    memcpy( &RxTxBuf[2], "VIN: Audi B8 - Test Applikation", sizeof("VIN: Audi B8 - Test Applikation"));
    LenOfMsg[0] = sizeof("VIN: Audi B8 - Test Applikation")+2;  /* 2 bytes data identifier */
    return UDScub_FINISHED;    
  }
  return UDScub_SUBFUNCTION_NOT_SUPPORTED;   /* send response  */
}


/***************************************************************************
 *  PURPOSE:        UDS_ubDiagService3E
 *************************************************************************/
byte UDS_ubDiagService3E_ECM( byte* RxTxBuf, word* LenOfMsg, byte Mode )
{
  /* send just an echo */
  return UDScub_FINISHED;    
}

#endif
