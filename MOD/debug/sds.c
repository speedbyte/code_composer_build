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
#if( SDS_A_READ_DATA_BY_IDENTIFIER == SDS_ENABLED )
static tSDS_Return CopyData(const t_DataIdCfgItem* DataIdItem, byte* data );
static tSDS_Return eInProgress;
#endif 
 
/*************************************************************************
 * VARIABLES
 *************************************************************************/
#if( SDS_A_RESET_OR_DISCONNECT == SDS_ENABLED )
static byte ubResetControl;     /* process hard, soft reset */
#endif
static word uwBufSize;

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
void SDS_vGetVersionInfo( I2S_VersionInfoType *versioninfo )
{
  versioninfo->vendorID = IN2SOFT_VENDOR_ID;
  versioninfo->moduleID = MODULE_ID;
  versioninfo->sw_major_version = MODULE_MAJOR_VERSION;
  versioninfo->sw_minor_version = MODULE_MINOR_VERSION;
  versioninfo->sw_patch_version = MODULE_PATCH_VERSION;
} 


/***************************************************************************
 *  PURPOSE:        Initialization of the module
 **************************************************************************
 *  PARAMETERS:     bufsize: buffer size
 **************************************************************************
 *  RETURN VALUE:   -
 *************************************************************************/
void SDS_vInit( word bufsize )
{
  uwBufSize = bufsize-1; /* one less for service id */
  #if( SDS_A_RESET_OR_DISCONNECT == SDS_ENABLED )
  ubResetControl = I2S_FALSE;
  #endif
  #if( SDS_A_READ_DATA_BY_IDENTIFIER == SDS_ENABLED )
  eInProgress = SDS_IO_USER_READ_SUCCESSFUL;
  #endif 
}


/***************************************************************************
 *  PURPOSE:        SDS task to execute backstage operations; e.g. RESET
 **************************************************************************
 *  PARAMETERS:     -
 **************************************************************************
 *  RETURN VALUE:   -
 *************************************************************************/
void SDS_vTask( void )
{
  #if ( SDS_A_RESET_OR_DISCONNECT == SDS_ENABLED )
  byte ubIdle = UDS_ubIsIdle(UDS_CHANNEL_INDEX_2);
  if( (ubResetControl != I2S_FALSE) && (ubIdle == I2S_TRUE) )
  {
    SDSext_vDoReset(ubResetControl);
    ubResetControl = I2S_FALSE;
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
#if ( SDS_A_START_DIAGNOSTIC_SESSION == SDS_ENABLED )
#define SessionType RxTxBuf[0]
byte UDS_ubDiagService10( byte* RxTxBuf, word* LenOfMsg, byte Mode)
{
tDEH_Return DEH_retVal;
byte retVal = UDScub_FINISHED;
byte current = UDS_ubGetSession(UDS_CHANNEL_INDEX_2);

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
    if( (SessionType == UDScub_PROGRAMMING_SESSION) || (SessionType == UDScub_DEVELOPMENT_SESSION) )
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
#if( SDS_A_RESET_OR_DISCONNECT == SDS_ENABLED )
#define ResetType RxTxBuf[0]
byte UDS_ubDiagService11( byte* RxTxBuf, word* LenOfMsg, byte Mode )
{
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
      ubResetControl = ResetType;
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
#if( SDS_A_CLEAR_DIAGNOSTIC_INF == SDS_ENABLED )
byte UDS_ubDiagService14( byte* RxTxBuf, word* LenOfMsg, byte Mode)
{
dword dtc;
tDEH_Return retVal;

  if( *LenOfMsg != (word)3 )
  {
    Mode = Mode; /* prevent compiler warnings */
    return UDScub_INCORRECT_MSG_LEN;
  }  
  dtc  = (dword)RxTxBuf[0] << 16;
  dtc += (word)RxTxBuf[1] << 8; 
  dtc += RxTxBuf[2]; 

  retVal  = DEH_ClearDiagnosticInformation( (tDEH_DTCGroup)dtc );
  
  if( (retVal == DEH_OK) || (retVal == DEH_E_CLEAR) )
  {
    /* len = 0 --> do not return dtc to the tester */
    /* only service id will be returned to tester */ 
    LenOfMsg[0] = 0;
    return UDScub_FINISHED;
  }
  else if( retVal == DEH_E_UNKNOWN_DTCGROUP )
  {
    return UDScub_REQUEST_OUT_OF_RANGE;
  }
  else if( (retVal == DEH_E_BUSY) || (retVal == DEH_E_INIT) )
  {
    return UDScub_BUSY_REPEAT_REQUEST;
  }
  else
  {
    return UDScub_CONDITIONS_NOT_CORRECT;
  }
}
#endif

/***************************************************************************
 *  PURPOSE:        UDS_ubDiagService19
 **************************************************************************
 *  PARAMETERS:     see UDS_ubDiagService10
 **************************************************************************
 *  RETURN VALUE:   see UDS_ubDiagService10
 *************************************************************************/
#if( SDS_A_READ_DTC_BY_STATUS == SDS_ENABLED )
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
  
  dtc =  (dword)RxTxBuf[1] << 16;
  dtc += (word)RxTxBuf[2] << 8;
  dtc += RxTxBuf[3];
  
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
        retVal = DEH_ReadDTCByStatus( (tDEH_DTCInfoType)DTCInformationType, (tDEH_DTCStatusMask)DTCStatusMask, &das );
        if( (retVal == DEH_OK) || (retVal == DEH_E_DATA_STILL_AVAILABLE) )
        {
          RxTxBuf[index+0] = (byte)(das.DTC>>16);
          RxTxBuf[index+1] = (byte)(das.DTC>>8);
          RxTxBuf[index+2] = (byte)(das.DTC);
          RxTxBuf[index+3] = das.StatusOfDTC;
          index += 4; /* sizeof(tDEH_DTCAndStatus); */
          if( (index+4) > uwBufSize ) /* check if buffer space available for the next DTC+status */
          {
            retVal = DEH_OK; /* stop collecting */
          }           
        }  
      }
      while( retVal == DEH_E_DATA_STILL_AVAILABLE );
      RxTxBuf[1] = SDS_A_DTCStatusAvailabilityMask; 
      *LenOfMsg = index;
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
          retVal = DEH_ReadDTCExtDataRecord( (tDEH_DTC)dtc, (uint8_t*)&RxTxBuf[4], (tDEH_RecordNumber)DTCExtendedDataRecordNumber, (uint16_t)uwBufSize-5, (uint16_t*)&index, (uint8_t*)&RxTxBuf[5]); 
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
#if( SDS_A_READ_DATA_BY_IDENTIFIER == SDS_ENABLED )
byte UDS_ubDiagService22( byte* RxTxBuf, word* LenOfMsg, byte Mode )
{
byte i;
static byte n, NumberOfDids, CurrentDid, DidTempNotAvailable;
static word uwIdx, awDid[8];

  if( eInProgress != SDS_IO_USER_READ_IN_PROGRESS )
  {
    if( LenOfMsg[0] < (word)2 )                /* at least one DID expected */
    {
      return UDScub_INCORRECT_MSG_LEN;
    }
    else if( (LenOfMsg[0] & 0x1) > (word)0 )   /* check if lenght is even */
    {
      return UDScub_INCORRECT_MSG_LEN;
    }
    else if( Mode == UDScub_PHYSICAL )
    {
      if( LenOfMsg[0] > (word)16 )             /* max number of identifier = 8 */
      {
        return UDScub_REQUEST_OUT_OF_RANGE;
      }
    }
    else
    {
      if( LenOfMsg[0] > (word)6 )              /* max number of identifier = 3 */
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
    upper=(sizeof(cs_DataIdItem)/sizeof(t_DataIdCfgItem))-1;
    /* lower,i = first DID; upper = last DID */
    
    while( i < upper ) /* stop searching if border reached */
    {
      /* get next interval */
      if( awDid[CurrentDid] < cs_DataIdItem[i].DataId )
      {
        upper = i;     /* update upper border */
      }
      else
      {
        lower = i;     /* update lower border */
      }
      i = (upper-lower);     /* calculate the middle */
      i = lower + i/2 + i%2; /* of lower and upper border */
      if( cs_DataIdItem[i].DataId == awDid[CurrentDid] )
      {
        found = 1;     /* data identifier found */
        break;         /* stop searching */
      }      
      if( i == lower ) /* check for termination condition */
      {
        if( awDid[CurrentDid] == cs_DataIdItem[i+1].DataId )
        {
          found = 1;   /* data identifier found */
          i++;
        }
        break;         /* stop searching */
      }                 
      if( i == upper ) /* check for termination condition */
      {
        if( awDid[CurrentDid] == cs_DataIdItem[i-1].DataId )
        {
          found = 1;   /* data identifier found */
          i--;
        }
        break;         /* stop searching */
      }
    }
    if( found != 0 )   /* insert data into the buffer if DID found */
    {
      if( (uwIdx+cs_DataIdItem[i].Len+2) <= uwBufSize )
      {
        eInProgress = CopyData(&cs_DataIdItem[i], &RxTxBuf[uwIdx+2]);
        if( eInProgress == SDS_IO_USER_READ_SUCCESSFUL )
        {
          RxTxBuf[uwIdx]   = (byte)(awDid[CurrentDid] >> 8);
          RxTxBuf[uwIdx+1] = (byte)awDid[CurrentDid];
          uwIdx += cs_DataIdItem[i].Len+2;
        }
        else if( eInProgress == SDS_IO_USER_READ_IN_PROGRESS )
        {
          return UDScub_IN_PROCESS;    
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
#if( SDS_A_COMMUNICATION_CONTROL == SDS_ENABLED )
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
#if( SDS_A_TESTER_PRESENT == SDS_ENABLED )
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
#if( SDS_A_CTRL_DTC_SETTINGS == SDS_ENABLED )
#define DTCSettingType RxTxBuf[0]
byte UDS_ubDiagService85( byte* RxTxBuf, word* LenOfMsg, byte Mode )
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


#if( SDS_A_READ_DATA_BY_IDENTIFIER == SDS_ENABLED )
/*************************************************************************
 *  PURPOSE:        Get data for service 22
 *************************************************************************/
static tSDS_Return CopyData(const t_DataIdCfgItem* DataIdItem, byte* data )
{
word i=0;
    
  switch( DataIdItem->Mem )
  {
    case SDS_BINARY:
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
      
    case SDS_ROM:
    case SDS_ASCII:
      while( i<DataIdItem->Len )
      {
        data[i] = ((const byte*)DataIdItem->Address)[i];
        i++;
      }
      break;
    
    case SDS_RAM:
      while( i<DataIdItem->Len )
      {
        data[i] = ((byte*)DataIdItem->Address)[i];
        i++;
      }
      break;
    
    case SDS_EEPROM:
    case SDS_USER:
      return SDSext_eReadUserAndIOData( data, DataIdItem );
    
    default:
      return SDS_IO_USER_READ_FAILED;
  }
  return SDS_IO_USER_READ_SUCCESSFUL;
}
#endif

