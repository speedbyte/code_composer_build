/**************************************************************************
 *  COPYRIGHT (C) In2Soft GmbH, 2005-2007                                 *
 *  COPYRIGHT (C) Volkswagen AG, 2006-2007                                *
 *  ALLE RECHTE VORBEHALTEN. ALL RIGHTS RESERVED.                         *
 *                                                                        *
 **************************************************************************
 *  FILE: UDS.C
 **************************************************************************
 *  PURPOSE:
 *  C-File including UDS protocol functionality in combinition with
 *  transport protocol ISO 15765 
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
 *  V_00_01_05 Ki 04.08.2005 - Bug-Fix: suppress response, not supported in
 *                             active mode handling
 *  V_00_01_06 Ki 09.08.2005 - Time trigger and timeout callback modified
 *  V_00_01_07 Ki 19.09.2005 - UDScub_MULTIPLE_FRAME deleted, UDScub_CANCELED new
 *                           - UDS_vTask() new structured
 *  V_00_01_08 Ki 05.12.2005 - Revision and changes based on VW MISRA test report
 *  V_01_00_00 Ki 18.01.2006 - new function: UDS_ubGetSession
 *                           - include of i2s_types.h
 *  V_01_01_00 Ki 09.03.2006 - Mask of the pos. resp. suppress bit
 *                           - No neg. response at func. addressing for 
 *                             codes: 11,12,7E,7F,31
 *  V_01_02_00 Ki 02.06.2006 - Multi client capability
 *  V_01_02_01 Ki 04.09.2006 - General revision based on review (by Audi) 
 *  V_01_02_02 Ki 10.10.2006 - Check for valid clients in UDS_ubGet/SetSession
 *                           - API function with/without ch parameter 
 *  V_01_02_03 Ki 02.11.2006 - TP_CHANNELS check
 *                           - bug-fix: null pointer call
 *                           - set status to IDLE on timeout
 *                           - UDS_ubIsIdle new caused by SDS
 *                           - Version number moved into C file
 *                           - New function: TP_vGetVersionInfo
 *                           - UDS_ubIgnoreFuncReq: param ch deleted
 *                           - tp_ubApplDataCon handling modified
 *                           - UDS_vTask revised
 *  V_01_02_04 Ki 19.12.2006 - i2s_type.h removed
 *                           - TRUE --> I2S_TRUE, FALSE --> I2S_FALSE
 *                           - set define _I2S_UDS_C_ for memory allocation´
 *                             in sds_cfg.h
 *  V_01_02_05 Ki 12.01.2007 - no change
 *  V_01_02_06 Ki 22.01.2007 - trigger timer in UDS_ubIgnoreFuncReq only in
 *                             IDLE state
 *  V_01_02_07 Ki 02.03.2007 - general revision, review by Audi
 *  V_01_02_08 Ki 30.03.2007 - no change
 *  V_01_02_09 Ki 02.07.2007 - UDS_SEND_POS_RESP_AFTER_78H handling
 *  V_01_02_10 Ki 11.07.2007 - no change
 *  V_01_02_11 Ki 07.08.2007 - no change 
 *  V_01_02_12 Ki 02.11.2007 - UDS_ubIsIdle returns TRUE it UDS and TP are IDLE 
 *  V_01_02_13 Ki 18.06.2008 - Check if functional in UDS_ubIgnoreFuncReq 
 *  V_01_02_14 Ki 23.07.2008 - no change
 *************************************************************************/
#define _I2S_UDS_C_

#include "uds.h"
#if( UDS_USAGE == 0 )
#include "tp.h"
#else
#include "aali.h"
#endif

#define MODULE_ID                          (byte)0x02  /* Std-SW module ID */
#define MODULE_MAJOR_VERSION               (byte)0x01  /* module version */
#define MODULE_MINOR_VERSION               (byte)0x02  /* module version */
#define MODULE_PATCH_VERSION               (byte)0x14  /* module version */

/* Configuration checks: valid number of services? */
#if ( UDS_MAX_NO_OF_SERVICES != 28)
#error "Number of Services != 26 not allowed!"
#endif

/* Configuration checks: valid number of services? */
#if ( UDS_CLIENTS == 0 )
#error "Number of UDS clients = 0 not allowed !"
#endif

/* Configuration checks: valid number of TP channels? */
#if ( UDS_CLIENTS > TP_CHANNELS )
#error "number of UDS_CLIENTS must be less or equal to TP_CHANNELS !"
#endif

/* ----------------------------------------------------------------------- */
/*  - L o c a l  V a r i a b l e s --------------------------------------- */
/* ----------------------------------------------------------------------- */

typedef struct t_UDS_Variables {
byte rub_State;             /* States of the module */
byte rub_Session;           /* Current diagnostic session */
byte *rpub_Data;            /* Pointer to the rx/tx buffer */
byte ruba_DataNR[8];        /* Negative response tx buffer */
byte rub_SupPosRspMsgInd;   /* Response suppress indication bit */
word ruw_UdsLen;            /* Length of of tx/rx message */
word ruw_UdsTimer;          /* Module timer (e.g. for P2max) */
byte rub_Mode;              /* addressing mode */
byte rub_Ready;             /* diagnostic readiness of the ECU */
} UDS_Variables;

#if( UDS_CLIENTS == 1 )
static UDS_Variables s_UdsVars;
#define rub_State           s_UdsVars.rub_State
#define rub_Session         s_UdsVars.rub_Session
#define rpub_Data           s_UdsVars.rpub_Data
#define ruba_DataNR         s_UdsVars.ruba_DataNR
#define rub_SupPosRspMsgInd s_UdsVars.rub_SupPosRspMsgInd
#define ruw_UdsLen          s_UdsVars.ruw_UdsLen
#define ruw_UdsTimer        s_UdsVars.ruw_UdsTimer
#define rub_Mode            s_UdsVars.rub_Mode
#define rub_Ready           s_UdsVars.rub_Ready
#define i2s_ch              0
#else
static UDS_Variables s_UdsVars[UDS_CLIENTS];
#define rub_State           s_UdsVars[i2s_ch].rub_State
#define rub_Session         s_UdsVars[i2s_ch].rub_Session
#define rpub_Data           s_UdsVars[i2s_ch].rpub_Data
#define ruba_DataNR         s_UdsVars[i2s_ch].ruba_DataNR
#define rub_SupPosRspMsgInd s_UdsVars[i2s_ch].rub_SupPosRspMsgInd
#define ruw_UdsLen          s_UdsVars[i2s_ch].ruw_UdsLen
#define ruw_UdsTimer        s_UdsVars[i2s_ch].ruw_UdsTimer
#define rub_Mode            s_UdsVars[i2s_ch].rub_Mode
#define rub_Ready           s_UdsVars[i2s_ch].rub_Ready
#endif
#define csTimeoutFkt()      cs_TimeoutFkt[i2s_ch].func()
#define csServicesModes     cs_ServicesModes[i2s_ch][i]

/* ----------------------------------------------------------------------- */
/*  - G l o b a l  V a r i a b l e s ------------------------------------- */
/* ----------------------------------------------------------------------- */


/* ----------------------------------------------------------------------- */
/*  - D e f i n e s ------------------------------------------------------ */
/* ----------------------------------------------------------------------- */
#define cub_TESTER_PRESENT           (byte)0x3E   /* tester present service id */
#define cub_NEGATIVE_RESPONSE        (byte)0x7F   /* negative response service id */
#define cub_IDLE_STATE               (byte)0x7F   /* module is idle */
#define cub_RESPONSE_PENDING        ((byte)0x80)  /* module is sending 78h */

#define cub_SID_WITH_SF_MASK        ((byte)0x80)  /* suppress pos. response bit */

/* ----------------------------------------------------------------------- */
/*  - M a c r o s -------------------------------------------------------- */
/* ----------------------------------------------------------------------- */
#define S168_RESPONSE_CODE(x) ((x==UDScub_SERVICE_NOT_SUPPORTED)    || \
                               (x==UDScub_SUBFUNCTION_NOT_SUPPORTED)|| \
                               (x==UDScub_REQUEST_OUT_OF_RANGE)     || \
                               (x==UDScub_SFNS_IN_ACTIVE_DIAGNOSTIC_SESSION) || \
                               (x==UDScub_SNS_IN_ACTIVE_DIAGNOSTIC_SESSION)  )
/* ----------------------------------------------------------------------- */
/*  - P r o t o t y p e s ------------------------------------------------ */
/* ----------------------------------------------------------------------- */
/* set new timer value and new state */ 
static void vSetUdsNew( UDS_CHANNEL_INDEX_API word T, byte S);
static byte ubGetSessionIndex( UDS_CHANNEL_INDEX_API_2 );


/* ----------------------------------------------------------------------- */
/*  - F u n c t i o n s of the module ------------------------------------ */
/* ----------------------------------------------------------------------- */

/*  FUNCTION byte UDS_ubDiagActive ( UDS_CHANNEL_INDEX_API_2 )
 **************************************************************************
 *  PURPOSE:        Set/Clear diagnosis ready for receive
 **************************************************************************
 *  PARAMETERS:     i2s_ch: client index
 **************************************************************************
 *  RETURN VALUE:   I2S_TRUE   Diagnosis is running
 *                  I2S_FLASE  Diagnosis is not running
 *************************************************************************/
byte UDS_ubIsActive( UDS_CHANNEL_INDEX_API_2 )
{
byte retVal = I2S_TRUE;

  if( ruw_UdsTimer == 0 )
  {
    retVal = I2S_FALSE;
  }
  return retVal;
}

/*  FUNCTION void UDS_vSetReady ( UDS_CHANNEL_INDEX_API byte state )
 **************************************************************************
 *  PURPOSE:        Set/Clear diagnosis ready for receive
 **************************************************************************
 *  PARAMETERS:     i2s_ch: client index
 *                  state:  I2S_TRUE  = accept requests
 *                          I2S_FLASE = denie requests if not runnig already
 **************************************************************************
 *  RETURN VALUE:   none
 *************************************************************************/
void UDS_vSetReady( UDS_CHANNEL_INDEX_API byte state )
{
  rub_Ready = state;
}

/*  FUNCTION UDS_vGetVersionInfo( I2S_VersionInfoType *psVersionInfo )
 **************************************************************************
 *  PURPOSE:        Output the module verson and vendor information
 **************************************************************************
 *  PARAMETERS:     address of the structure to be written
 **************************************************************************
 *  RETURN VALUE:   vendor id, module id, module version
 *************************************************************************/
void UDS_vGetVersionInfo( I2S_VersionInfoType *psVersionInfo )
{
  psVersionInfo->vendorID = IN2SOFT_VENDOR_ID;
  psVersionInfo->moduleID = MODULE_ID;
  psVersionInfo->sw_major_version = MODULE_MAJOR_VERSION;
  psVersionInfo->sw_minor_version = MODULE_MINOR_VERSION;
  psVersionInfo->sw_patch_version = MODULE_PATCH_VERSION;
} 


/*  FUNCTION void UDS_vInit( void )
 **************************************************************************
 *  PURPOSE:        Initialization of the UDS module.
 **************************************************************************
 *  PARAMETERS:     none
 **************************************************************************
 *  RETURN VALUE:   none
 *************************************************************************/
void UDS_vInit( void )
{
  #if( UDS_CLIENTS > 1 )
  byte i2s_ch;
  for( i2s_ch=0; i2s_ch<UDS_CLIENTS; i2s_ch++ )
  #endif
  {
    rub_State  = cub_IDLE_STATE;           /* init state */
    rub_Session = UDScub_DEFAULT_SESSION;  /* init session */
    ruw_UdsTimer = 0;                      /* reset timer */
    ruw_UdsLen = 0;                        /* reset length */
    rpub_Data  = 0;                        /* init rx/tx buffer address */
    rub_Ready = I2S_TRUE;                  /* accept requests */
  }
}


/*  FUNCTION void UDS_vInitClient( UDS_CHANNEL_INDEX_API byte* RxBuf )
 **************************************************************************
 *  PURPOSE:        Initialization of the UDS module.
 **************************************************************************
 *  PARAMETERS:     i2s_ch:  client index
 *                  buf:     pointer to the rx/tx buffer
 **************************************************************************
 *  RETURN VALUE:   none
 *************************************************************************/
void UDS_vInitClient( UDS_CHANNEL_INDEX_API byte* RxBuf )
{
  #if( UDS_CLIENTS > 1 )
  if( i2s_ch < UDS_CLIENTS )
  #endif 
  {
    rpub_Data  = RxBuf;   /* store rx/tx buffer address */
  }
}

/*  FUNCTION UDS_ubIgnoreFuncReq( byte* RxBuf, byte Len, byte mode )
 ***************************************************************************
 *  PURPOSE:        This function is called by the transport protocoll module
 *                  in case of a reception of a functional single frame request.
 *                  The application accept or trash the request by the return
 *                  value (see below). 
 **************************************************************************
 *  PARAMETERS:     RxBuf: Address of the received data buffer
 *                  Len:   Length of the data
 *                  mode:  Type of funct. address, e. i. UDS=2 or OBD=3
 **************************************************************************
 *  RETURN VALUE:   I2S_FALSE accept request for further process
 *                  I2S_TRUE  trash/ignore request (--> be ready for next request)
 *************************************************************************/
byte UDS_ubIgnoreFuncReq( byte* RxBuf, byte Len, byte mode )
{
byte retVal = I2S_FALSE;                             /* process request */

  if( Len == 2 )
  {
    if( mode == UDScub_FUNCTIONAL )                   /* ignore only if functional */
    {
      if( RxBuf[0] == cub_TESTER_PRESENT )            /* testerPresent Message ? */
      {                         
        if( (RxBuf[1] & cub_SID_WITH_SF_MASK) > 0 )   /* suppress pos. response bit set ? */
        {
          #if( UDS_CLIENTS > 1 )
          byte i2s_ch;
          for( i2s_ch=0; i2s_ch<UDS_CLIENTS; i2s_ch++ )
          #endif
          { 
            if( rub_State == cub_IDLE_STATE )
            {
              if( ruw_UdsTimer > 0 )                  /* trigger only if not zero */
              {
                ruw_UdsTimer = cuw_S3;                /* set new timer and state */
              }
            } 
          }
          retVal = I2S_TRUE;                          /* trash request */
        }
      }
    }
  }
  return retVal;                                  
} 

/*  FUNCTION void UDS_vTask ( void )
 **************************************************************************
 *  PURPOSE:        Main function, cyclic called task of the module.
 **************************************************************************
 *  PARAMETERS:     none
 **************************************************************************
 *  RETURN VALUE:   none
 *************************************************************************/
void UDS_vTask( void )
{
byte respCode, i, dm_idx, *buf;
word NoOfRxBytes;
t_APP_STATE ConfState;
    
  #if( UDS_CLIENTS > 1 )
  byte i2s_ch;
  for( i2s_ch=0; i2s_ch<UDS_CLIENTS; i2s_ch++ )
  #endif
  {  
    NoOfRxBytes = 0;
    if( ruw_UdsTimer > cuw_UDS_TASK_CYCLE )   /* no timeout ? */
    {                                            
      ruw_UdsTimer -= cuw_UDS_TASK_CYCLE;     /* --> decrement timer */
    }
    else 
    {
      if( ruw_UdsTimer > 0 )
      {    
        ruw_UdsTimer = 0;                        /* timeout! --> reset timer */
        rub_Session = UDScub_DEFAULT_SESSION;    /* go back to default session */
        rub_State = cub_IDLE_STATE;              /* reset state uds waiting for conf */
        csTimeoutFkt();                          /* notifiy application about timeout */
      }
    }    
    if( rub_State & cub_RESPONSE_PENDING )       /* 78h message ? */
    {
      ConfState = TP_eGetApplState(TP_CHANNEL_INDEX_2);   /* get confirmation state */
      if( ConfState == APP_STATE_CONFIRMATION )  /* confirmation received ? */ 
      {
        rub_State &= ~cub_RESPONSE_PENDING;      /* clear pending --> ready for callback */
      }
      else 
      {
        if( ConfState == APP_STATE_ERROR )       /* transmission failed: what now? */
        {
          TP_vCancelRequest(TP_CHANNEL_INDEX_2); /* cancel request */
          vSetUdsNew( UDS_CHANNEL_INDEX cuw_S3, cub_IDLE_STATE ); /* set new timer and state */
        }
      }       
    }    
    else
    {
      if( rub_State == cub_IDLE_STATE )          /* no request running? */
      {
        NoOfRxBytes = TP_uwApplDataInd(TP_CHANNEL_INDEX &dm_idx);
        rub_Mode = dm_idx;
        if( NoOfRxBytes != 0 )                          /* no request available? */
        {
          ruw_UdsLen = NoOfRxBytes-1;                   /* store no of bytes without SID */
          dm_idx = ubGetSessionIndex(UDS_CHANNEL_INDEX_2);   /* get index in table for the current session */        
          respCode = 0;                                 /* initiate as service supported in active mode */
          /* process diag. only if application has allowed or diag. is running already */ 
          if( (rub_Ready == I2S_FALSE) && (ruw_UdsTimer == 0) )
          {
            respCode = UDScub_CONDITIONS_NOT_CORRECT;   /* generate negative response */
          }
          else                                          /* look for SID in table */ 
          {
            for( i=0; i<UDS_MAX_NO_OF_SERVICES; i++ )   /* go through SID table */
            {
              if( csServicesModes.Sid == rpub_Data[0] ) /* SID matches ? */
              {
                if( csServicesModes.func != 0 )         /* SID supported ? */  
                {
                  /* get state service support in current mode */
                  dm_idx = (csServicesModes.ModeAvailability >> dm_idx) & (byte)0x1;
                  if( dm_idx == 0 )
                  {
                    respCode = UDScub_SNS_IN_ACTIVE_DIAGNOSTIC_SESSION;
                  }
                }        
                else
                {
                  respCode = UDScub_SERVICE_NOT_SUPPORTED;
                }        
                break;                                  /* stop searching */
              }
            }
            if( i == UDS_MAX_NO_OF_SERVICES )           /* service not found at all */
            {
               respCode = UDScub_SERVICE_NOT_SUPPORTED;
            }
          }    
          /* SID not found || SID not valid in current mode || diagnosis locked at all ?*/
          if( respCode > 0 )
          {
            if( rub_Mode == UDScub_PHYSICAL )           /* send neg. resp. if phys. addressed */
            {
              ruba_DataNR[1] = rpub_Data[0];            /* SID */
              ruba_DataNR[0] = cub_NEGATIVE_RESPONSE;   /* send neg. response */
              ruba_DataNR[2] = respCode;                /* negative response code */
              buf = ruba_DataNR;
              TP_ubApplDataReq( TP_CHANNEL_INDEX buf, 3, I2S_TRUE );
            }                                         
            else                                        /* don´t send neg. resp. if func. addressed */
            {                                           /* func. UDS or OBD can identifier !!! */
              TP_vCancelRequest(TP_CHANNEL_INDEX_2);    /* cancel request */
            }
            vSetUdsNew( UDS_CHANNEL_INDEX cuw_S3, cub_IDLE_STATE );    /* set new timer and state */
          }
          else
          {
            /* save table index (i) of the service here as UDS state and set new timer */
            vSetUdsNew( UDS_CHANNEL_INDEX cuw_P2_MAX_DEF-cuw_UDS_TASK_CYCLE, i );
            rub_SupPosRspMsgInd = (rpub_Data[1] & cub_SID_WITH_SF_MASK) && csServicesModes.suppressRespAllowed;
            if( rub_SupPosRspMsgInd )
            {
              rpub_Data[1] &= ~cub_SID_WITH_SF_MASK;  /* mask pos. response suppress bit */
            }
          }
        }
      }  
    }  
    /* process here new request, request after 78h */
    if( rub_State < cub_IDLE_STATE )
    {    
      i = rub_State;  /* get index for service specific callback */
      respCode = csServicesModes.func( &rpub_Data[1], &ruw_UdsLen, rub_Mode);
      
      /* service in process ? */
      if( (respCode == UDScub_IN_PROCESS) || (respCode == UDScub_REQUEST_RECEIVED_RESPONSE_PENDING) )
      {
        /* check 78h time elapsed */
        if( (ruw_UdsTimer <= cuw_UDS_TASK_CYCLE) || (respCode == UDScub_REQUEST_RECEIVED_RESPONSE_PENDING) )
        {
          ruba_DataNR[0] = cub_NEGATIVE_RESPONSE;
          ruba_DataNR[1] = rpub_Data[0];            /* SID */
          ruba_DataNR[2] = UDScub_REQUEST_RECEIVED_RESPONSE_PENDING;
          buf = ruba_DataNR;
          TP_ubApplDataReq( TP_CHANNEL_INDEX buf, 3, I2S_FALSE );      /* send neg. response */
          rub_State |= cub_RESPONSE_PENDING;        /* set pending flag */
          ruw_UdsTimer = cuw_P2_MAX-cuw_UDS_TASK_CYCLE;
          #if( UDS_SEND_POS_RESP_AFTER_78H == 1 )
          rub_SupPosRspMsgInd = 0;                  /* send pos. response, if 78h was sent */
          #endif
        }
      }  
      else if( respCode == UDScub_CANCELED )        /* trash request ? */
      {   
        TP_vCancelRequest(TP_CHANNEL_INDEX_2);      /* cancel request */
        vSetUdsNew( UDS_CHANNEL_INDEX cuw_S3, cub_IDLE_STATE );      /* set new timer and state */
      }
      else if( respCode == UDScub_FINISHED )        /* return = pos. response ? */
      {
        if( rub_SupPosRspMsgInd )                   /* response to be suppressed ? */
        {
          TP_vCancelRequest(TP_CHANNEL_INDEX_2);    /* cancel request */
        }
        else
        {
          rpub_Data[0] += 0x40;                     /* response SID */
          buf = rpub_Data;
          NoOfRxBytes = ruw_UdsLen+1; 
          TP_ubApplDataReq( TP_CHANNEL_INDEX buf, NoOfRxBytes, I2S_TRUE ); /* send pos. response */
        }
        vSetUdsNew( UDS_CHANNEL_INDEX cuw_S3, cub_IDLE_STATE );      /* set new timer and state */
      }
      else                                          /* neg. response code ! */
      {   
        /* functional UDS or OBD can identifier !!! */
        if( (rub_Mode >= UDScub_FUNCTIONAL) && S168_RESPONSE_CODE(respCode) )
        {
          TP_vCancelRequest(TP_CHANNEL_INDEX_2);    /* cancel request */
        }
        else
        {
          ruba_DataNR[0] = cub_NEGATIVE_RESPONSE;
          ruba_DataNR[1] = rpub_Data[0];            /* SID */
          ruba_DataNR[2] = respCode;                /* neg. response code */
          buf = ruba_DataNR; 
          TP_ubApplDataReq( TP_CHANNEL_INDEX buf, 3, I2S_TRUE );      /* send neg. response */
        }
        vSetUdsNew( UDS_CHANNEL_INDEX cuw_S3, cub_IDLE_STATE );      /* set new timer and state */
      }
    }
  } /* FOR loop */
}  

/*  FUNCTION byte UDS_ubSetSession( UDS_CHANNEL_INDEX_API byte bSession )
 **************************************************************************
 *  PURPOSE:        Sets the requested session and returns result of the 
 *                  set operation.
 **************************************************************************
 *  PARAMETERS:     i2s_ch:      client index
 *                  Session: Session to be set (one from the config file)
 **************************************************************************
 *  RETURN VALUE:   I2S_FALSE: set failed
 *                  I2S_TRUE:  set successful
 *************************************************************************/
byte UDS_ubSetSession( UDS_CHANNEL_INDEX_API byte bSession )
{
byte i;
byte retVal = I2S_FALSE;

  #if( UDS_CLIENTS > 1 )
  if( i2s_ch < UDS_CLIENTS )
  #endif 
  {
    if( bSession != UDScub_INVALID_SESSION )
    {
      for( i=0; i<UDS_MAX_NO_OF_SESSIONS; i++ )
      {
        if( bSession == ubDiagSession[i2s_ch][i] )
        {
          /* session to set is supported */
          rub_Session = bSession;
          retVal = I2S_TRUE;
          break;
        }
      } 
    }
  }
  return retVal;
}


/*  FUNCTION byte UDS_ubGetSession( UDS_CHANNEL_INDEX_API_2 )
 **************************************************************************
 *  PURPOSE:        Returns the current session.
 **************************************************************************
 *  PARAMETERS:     i2s_ch:    client index
 **************************************************************************
 *  RETURN VALUE:   current session or UDScub_INVALID_SESSION
 *************************************************************************/
byte UDS_ubGetSession( UDS_CHANNEL_INDEX_API_2 )
{
byte retVal = rub_Session;

  #if( UDS_CLIENTS > 1 )
  if( i2s_ch >= UDS_CLIENTS )
  {
    retVal = UDScub_INVALID_SESSION;
  }
  #endif 
  return retVal;
}


/*  FUNCTION static byte ubGetSessionIndex( UDS_CHANNEL_INDEX_API_2 )
 **************************************************************************
 *  PURPOSE:        Returns the array index of the current session.
 **************************************************************************
 *  PARAMETERS:     i2s_ch:   client index
 **************************************************************************
 *  RETURN VALUE:   current session (one from the config file)
 *************************************************************************/
static byte ubGetSessionIndex( UDS_CHANNEL_INDEX_API_2 )
{
byte i;

  /* down-count will return 0 (default session) in case of no match */ 
  for( i=UDS_MAX_NO_OF_SESSIONS-1; i>0; i-- )
  {
    if( rub_Session == ubDiagSession[i2s_ch][i] )
    {
      break;
    }
  } 
  return i;
}

/*  FUNCTION static void vSetUdsNew(UDS_CHANNEL_INDEX_API word T, byte S)
 **************************************************************************
 *  PURPOSE:        static function to set the new timer value and new state
 *                  of the module after proceeding an action.
 **************************************************************************
 *  PARAMETERS:     timer value in ms, state
 **************************************************************************
 *  RETURN VALUE:   none 
 *************************************************************************/
static void vSetUdsNew( UDS_CHANNEL_INDEX_API word T, byte S)
{ 
  ruw_UdsTimer = T;
  rub_State = S;
}

/*  FUNCTION byte UDS_ubIsIdle( UDS_CHANNEL_INDEX_API_2 )
 **************************************************************************
 *  PURPOSE:        Returns TRUE if TP and UDS layer are in IDLE state
 **************************************************************************
 *  PARAMETERS:     i2s_ch:   client index
 **************************************************************************
 *  RETURN VALUE:   I2S_TRUE  if state is IDLE
 *                  I2S_FALSE otherwise 
 *************************************************************************/
byte UDS_ubIsIdle( UDS_CHANNEL_INDEX_API_2 )
{
byte retVal = I2S_FALSE;
t_APP_STATE tpState;

  tpState = TP_eGetApplState( TP_CHANNEL_INDEX_2 );
  if( (tpState == APP_STATE_IDLE) && (rub_State == cub_IDLE_STATE) )
  {
    retVal = I2S_TRUE;
  }
  return retVal;
}

/*  FUNCTION byte UDS_ubSendMsg( UDS_CHANNEL_INDEX_API byte* buf, word len )
 **************************************************************************
 *  PURPOSE:        Sends a generic UDS message
 **************************************************************************
 *  PARAMETERS:     i2s_ch:   client index
 **************************************************************************
 *  RETURN VALUE:   I2S_TRUE  message to send accepted
 *                  I2S_FALSE message to send denied
 *************************************************************************/
byte UDS_ubSendMsg( UDS_CHANNEL_INDEX_API byte* buf, word len )
{
byte retVal;

  retVal = TP_ubApplDataReq( TP_CHANNEL_INDEX buf, len, I2S_TRUE );
  return retVal;
}
