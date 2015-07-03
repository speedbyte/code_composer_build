/**************************************************************************
 *  COPYRIGHT (C) In2Soft GmbH, 2005-2007                                 *
 *  COPYRIGHT (C) Volkswagen AG, 2006-2007                                *
 *  ALLE RECHTE VORBEHALTEN. ALL RIGHTS RESERVED.                         *
 *                                                                        *
 **************************************************************************
 *  FILE: TP.C
 **************************************************************************
 *  PURPOSE:
 *  C-File including transport protocol functionality ISO 15765 
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
 *  V_00_01_04 Ki 22.07.2005 - Workaround for CANdrv confirmation  
 *  V_00_01_05 Ki 04.08.2005 - Bug-Fix: length calculation CF transmit
 *                           - Modification: Conf. within Req-Function   
 *  V_00_01_06 Ki 09.08.2005 - New: wait repetition, invalid STmin values
 *                             overflow handling while receiving
 *  V_00_01_07 Ki 19.09.2005 - TpCanTransmit, string.h, TP_ubApplDataInd(mode)
 *                           - TP_ubApplDataCon() without parameter
 *                           - CheckAddresses() for 11bit mixed, return-value
 *  V_00_01_08 Ki 08.12.2005 - Revision and changes based on VW MISRA test report
 *  V_01_00_00 Ki 18.01.2006 - Pointer operation modified by using an offset
 *                           - include of i2s_types.h
 *                           - OBD identifiers support (normal/fixed addressing only)
 *  V_01_02_00 Ki 02.06.2006 - Multi channel capability
 *  V_01_02_01 Ki 04.09.2006 - General revision based on review (by Audi)
 *  V_01_02_02 Ki 10.10.2006 - Comment for TP_vInitChannel 
 *                           - API function with/without ch parameter 
 *  V_01_02_03 Ki 02.11.2006 - TP_ubApplDataCon with new parameter
 *                           - Version number moved into C file
 *                           - New function: TP_GetVersionInfo
 *  V_01_02_04 Ki 19.12.2006 - i2s_type.h removed
 *                           - I2V_vCanDataInd --> I2V_vCanDataPrecopy
 *                           - I2V_vCanDataCon independet from XT_ID
 *                           - TpCanTransmit --> CanTransmit
 *                           - bug-fix: copy OBD msg (single channel TP)
 *                             only if cub_ISO15765_4_ADDRESSING set
 *                           - ignore frames with dlc < cub_CAN_SIZE
 *                           - TRUE --> I2S_TRUE, FALSE --> I2S_FALSE
 *                           - ENABLED_TP_WITH_VECTOR_CAN deleted
 *                           - set define _I2S_TP_C_ for memory allocation´
 *                             in sds_cfg.h
 *  V_01_02_05 Ki 12.01.2007 - byte cast at function call vCanDataInd()
 *                           - variable ch decleration in vCanDataInd() moved
 *                           - default value for TP_CONF_BY_CAN = 0
 *                           - include can_inc.h removed   
 *                           - byte cast at function call CheckAddresses()
 *                           - return value of TP_ubApplDataCon modified
 *  V_01_02_06 Ki 02.03.2007 - general revision, review by Audi
 *  V_01_02_07 Ki 30.03.2007 - timeout detection while wait for FC indication
 *  V_01_02_08 Ki 11.04.2007 - ignore unexpected FC
 *  V_01_02_09 Ki 02.07.2007 - default value for rub_SepTime = 127ms
 *                           - wait handling within FC deleted
 *                           - workaround for CAN driver racing problem: inverted
 *                             processsing of the indication and confirmation msg
 *                             #define TP_CONF_BY_CAN deleted
 *                             ubCanDataReq and vCanDataCon without data param
 *  V_01_02_10 Ki 11.07.2007 - vCanDataCon within I2V_vCanDataCon activated
 *  V_01_02_11 Ki 23.07.2007 - TP_STATE_IND splitted in _IND_FC and _IND_CF
 *                           - Ignore all funtional TP frames except SF
 *  V_01_02_12 Ki 02.11.2007 - Length calculation on Rx path corrected: byte-->word
 *                           - ubCheckAddresses() extended for cub_SECOND_FUNC_ADDRESS
 *                             and cul_REQ_CAN_ID_11BIT_FUNC_2
 *  V_01_02_13 Ki 18.06.2008 - Send $21 if req. received while previous req. in process
 *                           - Call UDS_ubIgnoreFuncReq on each SF indication
 *                           - Check CAN length for 8 bytes if cub_USE_PADDING set
 *  V_01_02_14 Ki 23.07.2008 - Invalid TPCI will be ignored
 *                           - Funct. SF will stop the segm. receive anyway if
 *                             return FALSE by UDS_ubIgnoreFuncReq
 *                           - Unexpected phys. SN will stop segm. receiption
 *  V_01_02_15 Ki 08.12.2008 - Ignore funct. SF in any case (also if APP=IDLE)
 ************************************************************************/
#define _I2S_TP_C_

#include <string.h> /* memcpy */
#include "tp.h"

#define MODULE_ID                          (byte)0x07  /* Std-SW module ID */
#define MODULE_MAJOR_VERSION               (byte)0x01  /* module version */
#define MODULE_MINOR_VERSION               (byte)0x02  /* module version */
#define MODULE_PATCH_VERSION               (byte)0x15  /* module version */

/* Configuration checks: valid number of services? */
#if ( TP_CHANNELS == 0 )
#error "Number of TP clients = 0 not allowed!"
#endif

extern byte UDS_ubIgnoreFuncReq( byte* RxBuf, byte Len, byte mode );

/* ----------------------------------------------------------------------- */
/*  - C o n s t a n t s -------------------------------------------------- */
/* ----------------------------------------------------------------------- */
                               
#define cub_CAN_SIZE                          (byte)8  /* buffer len for 1 can frame */
#define cuw_STOP_TIMER                        (word)0  /* tp timer values in 10ms */

#define cub_INVALID_ID                        (byte)0  /* invalid can identifier */
#define cub_PHYSICAL                          (byte)1  /* physical can identifier */
#define cub_FUNCTIONAL                        (byte)2  /* functional can identifier */
#define cub_OBD                               (byte)3  /* OBD can identifier */

/* OBD identifiers and addresses */
#define cul_REQ_CAN_ID_11BIT_OBD         (dword)0x7DF  /* functional rx can id for OBD */
#define cub_ECU_ADR_OBD                   (byte)0x33   /* OBD target for extended/normal fixed adressing */

/* values for busy repeat response handling */
#define cub_BUSY_CONF                     (byte)0xFF   /* functional rx can id for OBD */
#define cub_BUSY_IDLE                     (byte)0x00   /* idle, init value */

/* ----------------------------------------------------------------------- */
/*  - T y p e d e f s ---------------------------------------------------- */
/* ----------------------------------------------------------------------- */

/* states between the transport layer and can driver */ 
typedef enum  {
  TP_STATE_IDLE=0,           /* transport layer idle */
  TP_STATE_SEND_SF,          /* send single frame */
  TP_STATE_SEND_FF,          /* send first frame */
  TP_STATE_SEND_CF,          /* send consecutive frame */
  TP_STATE_SEND_FC,          /* send flow control */
  TP_STATE_CON,              /* waiting for confirmation by can driver */
  TP_STATE_IND_FC,           /* waiting for FC from tester */
  TP_STATE_IND_CF            /* waiting for CF from tester */
} t_TP_STATE;

/* ----------------------------------------------------------------------- */
/*  - V a r i a b l e s -------------------------------------------------- */
/* ----------------------------------------------------------------------- */

typedef struct {
t_APP_STATE re_AppState;         /* states of the appl. uses tp */
t_TP_STATE  re_TpState;          /* states of the module */
byte rub_Sequence;               /* sequence number for frag. tx/rx */
byte rub_SepTime;                /* separation time (consecutive frames) */
byte rub_BlockSize;              /* block size of current xmission (rx/tx) */
byte *rpub_RxBuf;                /* pointer to rx buf */
byte *rpub_TxBuf;                /* pointer to tx buf */
byte ruba_CanData[cub_CAN_SIZE]; /* CAN tx/rx buffer */
word ruw_BufSize;                /* size of the rx/tx buf in bytes */
word ruw_TpLen;                  /* Length of a tp frame (rx/tx) */
word ruw_Index;                  /* current position of rx/tx */
word ruw_TpTimer;                /* module timer (e.g. STmin) */
byte rub_IdentType;              /* identifier type: funct/phys */
byte rub_TesterAdr;              /* current tester address */
byte rub_Release;                /* buffer lock/release handling */
byte rub_BRR;                    /* flag and SID for busy repeat request handling */
} t_TP_VARIABLES;


#if( TP_CHANNELS == 1 )
t_TP_VARIABLES s_TpVars;
#define re_AppState     s_TpVars.re_AppState
#define re_TpState      s_TpVars.re_TpState
#define rub_Sequence    s_TpVars.rub_Sequence
#define rub_SepTime     s_TpVars.rub_SepTime
#define rub_BlockSize   s_TpVars.rub_BlockSize
#define rpub_RxBuf      s_TpVars.rpub_RxBuf
#define rpub_TxBuf      s_TpVars.rpub_TxBuf
#define ruba_CanData    s_TpVars.ruba_CanData
#define ruw_BufSize     s_TpVars.ruw_BufSize
#define ruw_TpLen       s_TpVars.ruw_TpLen
#define ruw_Index       s_TpVars.ruw_Index
#define ruw_TpTimer     s_TpVars.ruw_TpTimer
#define rub_IdentType   s_TpVars.rub_IdentType
#define rub_TesterAdr   s_TpVars.rub_TesterAdr
#define rub_Release     s_TpVars.rub_Release 
#define rub_BRR         s_TpVars.rub_BRR
#else
t_TP_VARIABLES s_TpVars[TP_CHANNELS];
#define re_AppState     s_TpVars[i2s_ch].re_AppState
#define re_TpState      s_TpVars[i2s_ch].re_TpState
#define rub_Sequence    s_TpVars[i2s_ch].rub_Sequence
#define rub_SepTime     s_TpVars[i2s_ch].rub_SepTime
#define rub_BlockSize   s_TpVars[i2s_ch].rub_BlockSize
#define rpub_RxBuf      s_TpVars[i2s_ch].rpub_RxBuf
#define rpub_TxBuf      s_TpVars[i2s_ch].rpub_TxBuf
#define ruba_CanData    s_TpVars[i2s_ch].ruba_CanData
#define ruw_BufSize     s_TpVars[i2s_ch].ruw_BufSize
#define ruw_TpLen       s_TpVars[i2s_ch].ruw_TpLen
#define ruw_Index       s_TpVars[i2s_ch].ruw_Index
#define ruw_TpTimer     s_TpVars[i2s_ch].ruw_TpTimer
#define rub_IdentType   s_TpVars[i2s_ch].rub_IdentType
#define rub_TesterAdr   s_TpVars[i2s_ch].rub_TesterAdr
#define rub_Release     s_TpVars[i2s_ch].rub_Release 
#define rub_BRR         s_TpVars[i2s_ch].rub_BRR 
#define cub_UDS_CLIENT             acub_UDS_CLIENT[i2s_ch]
#define cub_ADDRESSING             acub_ADDRESSING[i2s_ch]
#define cub_SECOND_FUNC_ADDRESS    acub_SECOND_FUNC_ADDRESS[i2s_ch]
#define cub_ISO15765_4_ADDRESSING  acub_ISO15765_4_ADDRESSING[i2s_ch]
#define cub_ECU_ADR_PHYS           acub_ECU_ADR_PHYS[i2s_ch]      
#define cub_ECU_ADR_FUNC           acub_ECU_ADR_FUNC[i2s_ch]
#define cul_REQ_CAN_ID_11BIT_PHYS  acul_REQ_CAN_ID_11BIT_PHYS[i2s_ch]
#define cul_RES_CAN_ID_11BIT       acul_RES_CAN_ID_11BIT[i2s_ch]
#define cub_BS_DEF                 acub_BS_DEF[i2s_ch]
#define cub_ST_DEF                 acub_ST_DEF[i2s_ch]
#define cuw_MIN_TIME_CONF          acuw_MIN_TIME_CONF[i2s_ch]
#define cuw_MAX_TIME_SEND          acuw_MAX_TIME_SEND[i2s_ch]
#define cuw_MAX_TIME_CONF          acuw_MAX_TIME_CONF[i2s_ch]      
#define CAN_TX_HANDLE              acub_CAN_TX_HANDLE[i2s_ch]
#endif

/* ----------------------------------------------------------------------- */
/*  - M a c r o s / P r o t o t y p e s ---------------------------------- */
/* ----------------------------------------------------------------------- */
#if (TP_CHANNELS > 1)
static byte ubCheckAddresses( byte* i2s_ch, dword canID, byte *data );
#else
static byte ubCheckAddresses( dword canID, byte *data );
#endif

static void  vSetTpNew(TP_CHANNEL_INDEX_API word T, t_TP_STATE S);
static byte  ubFillPaddingBytes( TP_CHANNEL_INDEX_API byte len );
static byte  ubCanDataReq( TP_CHANNEL_INDEX_API byte dlc );
static void  vCanDataCon ( TP_CHANNEL_INDEX_API_2 );
static void  vCanDataInd( TP_CHANNEL_INDEX_API byte IdType, byte* RxD );

/* ----------------------------------------------------------------------- */
/*  - F l a g s ---------------------------------------------------------- */
/* ----------------------------------------------------------------------- */

/* bit encoding of the TPCI (transport protocol control information) */
#define cub_TPCI_OC_SingleFrame        (byte)0x00
#define cub_TPCI_OC_FirstFrame         (byte)0x10
#define cub_TPCI_OC_ConsecutiveFrame   (byte)0x20
#define cub_TPCI_OC_FlowControl        (byte)0x30

#define cub_WAIT_BIT                   (byte)0x01  /* wait bit within a flow control message */
#define cub_OVERFLOW_BIT               (byte)0x02  /* overflow bit within a flow control message */

/* ----------------------------------------------------------------------- */
/*  - F u n c t i o n s -------------------------------------------------- */
/* ----------------------------------------------------------------------- */

/*  FUNCTION TP_vGetVersionInfo( I2S_VersionInfoType *psVersionInfo )
 **************************************************************************
 *  PURPOSE:        Output the module verson and vendor information
 **************************************************************************
 *  PARAMETERS:     psVersionInfo: address of the structure to be written
 **************************************************************************
 *  RETURN VALUE:   vendor id, module id, module version
 *************************************************************************/
void TP_vGetVersionInfo( I2S_VersionInfoType *psVersionInfo  )
{
  psVersionInfo ->vendorID = IN2SOFT_VENDOR_ID;
  psVersionInfo ->moduleID = MODULE_ID;
  psVersionInfo ->sw_major_version = MODULE_MAJOR_VERSION;
  psVersionInfo ->sw_minor_version = MODULE_MINOR_VERSION;
  psVersionInfo ->sw_patch_version = MODULE_PATCH_VERSION;
} 


/*  FUNCTION void TP_vInit( void )
 **************************************************************************
 *  PURPOSE:        Init of the transport protocol module
 **************************************************************************
 *  PARAMETERS:     none
 **************************************************************************
 *  RETURN VALUE:   none
 *************************************************************************/
void TP_vInit( void )
{
  #if( TP_CHANNELS > 1 )
  byte i2s_ch=0;
  for( ; i2s_ch<TP_CHANNELS; i2s_ch++ )
  #endif
  {
    vSetTpNew(TP_CHANNEL_INDEX cuw_STOP_TIMER,TP_STATE_IDLE);
    rub_TesterAdr = cub_TESTER_ADR;
    re_AppState = APP_STATE_IDLE;
    rub_SepTime = cub_ST_DEF;
    rub_BlockSize = cub_BS_DEF;
    rpub_RxBuf = 0;
    ruw_BufSize = 0;
    rub_IdentType = cub_INVALID_ID;
    rub_Release = I2S_TRUE;
    rub_BRR = cub_BUSY_IDLE;
  }
}

/*  FUNCTION void TP_vInitChannel( TP_CHANNEL_INDEX_API byte *RecBuf, word bufSize )
 **************************************************************************
 *  PURPOSE:        Init of the transport protocol module
 **************************************************************************
 *  PARAMETERS:     i2s_ch  : index of the tp channel
 *                  RecBuf : addresses of the rx buffers
 *                  bufSize: size of the rx buffers in bytes 
 **************************************************************************
 *  RETURN VALUE:   none
 *************************************************************************/
void TP_vInitChannel( TP_CHANNEL_INDEX_API byte* RecBuf, word bufSize )
{
  #if( TP_CHANNELS > 1 )
  if( i2s_ch >= TP_CHANNELS )
  {
    return;   /* invalid channel index */
  }
  #endif
  rpub_RxBuf = RecBuf;
  ruw_BufSize = bufSize;
}

/*  FUNCTION void TP_vTask (void)
 **************************************************************************
 *  PURPOSE:        Main function of the transport protocol module. This
 *                  function must be called cyclic each cuw_TASK_CYCLE.
 **************************************************************************
 *  PARAMETERS:     none 
 **************************************************************************
 *  RETURN VALUE:   none
 *************************************************************************/
void TP_vTask( void )
{
  byte Tmp;
  byte offset=0;
  
  #if( TP_CHANNELS > 1 )
  byte i2s_ch;
  for( i2s_ch=0; i2s_ch<TP_CHANNELS; i2s_ch++ )
  #endif
  {
    /* no action while tp timer is running */
    if( ruw_TpTimer >= cuw_TASK_CYCLE )
    {
        ruw_TpTimer -= cuw_TASK_CYCLE;
    }
    else
    {
      ruw_TpTimer = 0;
    }
    if ( cub_ADDRESSING >= TP_EXTENDED )
  	{
      offset = 1;    /* 1 byte offset because of address information */
      if( cub_ADDRESSING == TP_EXTENDED )
      {
        ruba_CanData[0] = cub_TESTER_ADR;      /* target address */
      }
      else
      {
        ruba_CanData[0] = cub_ADR_EXTENSION;   /* address extension */
      }
    }  
    #if( cub_BUSY_REPEAT_REQUEST != 0 )
    if( re_TpState != TP_STATE_CON )
    {
        if( rub_BRR > cub_BUSY_IDLE )
        {
          if( rub_BRR == cub_BUSY_CONF )       /* waiting for confirmation */
          {
            rub_BRR = cub_BUSY_IDLE;           /* leave wait for conf. state */ 
          }
          else
          {
            ruba_CanData[0+offset] = (byte)3 | cub_TPCI_OC_SingleFrame;    /* TPCI and length */
            ruba_CanData[1+offset] = 0x7F;                      /* neg. Responose SID */
            ruba_CanData[2+offset] = rub_BRR;                   /* copy request service id */
            ruba_CanData[3+offset] = 0x21;                      /* busy repeat response */
            Tmp = (byte)3+offset+1;
            Tmp = ubFillPaddingBytes(TP_CHANNEL_INDEX Tmp); 
            /*ruw_TpTimer = cuw_MAX_TIME_SEND;*/                  /* time for conf-wait = cuw_TASK_CYCLE */
            rub_BRR = cub_BUSY_CONF;
            ubCanDataReq( TP_CHANNEL_INDEX Tmp);
            return;
          }
        }
    }
    #endif
    if( ruw_TpTimer == 0 )
    {      
      switch( re_TpState )  /* check tp states */
      {
        /* ------------------------------------ */
        /* | TPCI/Len | Data (max. 6/7 bytes) | */  
        /* ------------------------------------ */
        case TP_STATE_SEND_SF:        
          ruba_CanData[0+offset] = (byte)ruw_TpLen | cub_TPCI_OC_SingleFrame;    /* TPCI and length */
          memcpy(&ruba_CanData[1+offset], &rpub_TxBuf[0], ruw_TpLen);            /* copy data bytes */
          Tmp = (byte)ruw_TpLen+offset+1;
          Tmp = ubFillPaddingBytes(TP_CHANNEL_INDEX Tmp); 
          vSetTpNew(TP_CHANNEL_INDEX cuw_MAX_TIME_SEND,TP_STATE_CON);            /* tp wait for confirmation */      
          ubCanDataReq( TP_CHANNEL_INDEX Tmp); 
          break;
    
        /* ------------------------------------------ */
        /* | TPCI/Len | Len | Data (max. 5/6 bytes) | */  
        /* ------------------------------------------ */
        case TP_STATE_SEND_FF:
          rub_Sequence = 0;                                                       /* init SN */
          ruba_CanData[0+offset] = (byte)(ruw_TpLen>>8) | cub_TPCI_OC_FirstFrame; /* TPCI and length */
          ruba_CanData[1+offset] = (byte)ruw_TpLen;                               /* length */
          memcpy(&ruba_CanData[2+offset], &rpub_TxBuf[0], (byte)6-offset);        /* copy data bytes */
          ruw_Index = (byte)6-offset;                                             /* init number of sent bytes */
          vSetTpNew(TP_CHANNEL_INDEX cuw_MAX_TIME_SEND,TP_STATE_CON);             /* tp wait for confirmation */
          ubCanDataReq( TP_CHANNEL_INDEX cub_CAN_SIZE); 
          break;
    
        /* ----------------------------------- */
        /* | TPCI/SN | Data (max. 6/7 bytes) | */  
        /* ----------------------------------- */
        case TP_STATE_SEND_CF:
          rub_Sequence++;                                                              /* update SN */
          ruba_CanData[0+offset] = (rub_Sequence&0x0F) | cub_TPCI_OC_ConsecutiveFrame; /* store TPCI and SN */ 
          if( (ruw_TpLen-ruw_Index) > ((word)7-offset) )
          {                                
            Tmp = (byte)7-offset;
          }
          else
          {
            Tmp = (byte)(ruw_TpLen-ruw_Index);
          } 
          memcpy(&ruba_CanData[1+offset], &rpub_TxBuf[ruw_Index], Tmp);    /* copy data bytes */
          ruw_Index += Tmp;                                                /* update number of sent bytes */ 
          Tmp = ubFillPaddingBytes(TP_CHANNEL_INDEX Tmp+offset+1); 
          vSetTpNew(TP_CHANNEL_INDEX cuw_MAX_TIME_SEND,TP_STATE_CON);      /* tp wait for confirmation */
          ubCanDataReq(TP_CHANNEL_INDEX Tmp); 
          break;
    
        /* ------------------------ */
        /* | TPCI/FS | BS | STmin | */  
        /* ------------------------ */
        case TP_STATE_SEND_FC:
          ruba_CanData[0+offset] = cub_TPCI_OC_FlowControl;    /* store TPCI, FS = 0 */ 
          ruba_CanData[1+offset] = cub_BS_DEF;                 /* store Block size default */ 
          ruba_CanData[2+offset] = cub_ST_DEF;                 /* store STmin default */ 
          Tmp = ubFillPaddingBytes(TP_CHANNEL_INDEX offset+3); 
          if( ruw_TpLen > ruw_BufSize )                        /* receive buffer sufficient ? */
          {
            ruba_CanData[0+offset] |= cub_OVERFLOW_BIT;        /* set overflow flag */
          }
          vSetTpNew(TP_CHANNEL_INDEX cuw_MAX_TIME_SEND,TP_STATE_CON);        /* tp wait for confirmation */
          ubCanDataReq(TP_CHANNEL_INDEX Tmp); 
          break;
    
        /* waiting for FC or CF of the tester */ 
        case TP_STATE_IND_FC:
        case TP_STATE_IND_CF:
        /* tp timeout or error while waiting for confirmation by can driver */
        case TP_STATE_CON:
          if( re_AppState != APP_STATE_IDLE )                 /* tp inform application */
          {
            if( rub_Release == I2S_FALSE )
            {
              re_AppState = APP_STATE_ERROR;                  /* tp inform application */
            }
            else
            {
              re_AppState = APP_STATE_IDLE;                   /* tp inform application */
            }
          }
          vSetTpNew(TP_CHANNEL_INDEX cuw_STOP_TIMER,TP_STATE_IDLE); /* tp go IDLE */
          break;
    
        /* tp is idle --> do nothing */
        case TP_STATE_IDLE:
          break;
        
        /* undefined state (normally never reached) */
        default:
          re_AppState = APP_STATE_IDLE;                             /* appliction go IDLE */
          vSetTpNew(TP_CHANNEL_INDEX cuw_STOP_TIMER,TP_STATE_IDLE); /* tp go IDLE */
          break;                                               
      }
    }
  }
}


/*  FUNCTION byte TP_ubApplDataReq( TP_CHANNEL_INDEX_API byte *buf, word len, byte rel )
 **************************************************************************
 *  PURPOSE:        This function is called by the diagnostic application
 *                  to respond to the testers request.
 **************************************************************************
 *  PARAMETERS:     i2s_ch: TP channel index
 *                  buf:    address of the transmit buffer
 *                  len:    number of bytes to send
 *                  rel:    I2S_TRUE = release Rx buffer for new request
 *                          in case of error or success
 *                          I2S_FALSE = don´t release
 **************************************************************************
 *  RETURN VALUE:   I2S_TRUE:    request accepted
 *                  I2S_FALSE:   request denied/canceled
 *************************************************************************/
byte TP_ubApplDataReq( TP_CHANNEL_INDEX_API byte *buf, word len, byte rel )
{
byte LenFF;

  #if( TP_CHANNELS > 1 )
  if( i2s_ch >= TP_CHANNELS )
  {
    return I2S_FALSE;                              /* invalid channel index */
  }
  #endif
  if( re_TpState != TP_STATE_IDLE )
  {
    return I2S_FALSE;                              /* TP can not accept data while processing */
  }
  if( len == 0 )                                   /* check len */
  {
    return I2S_FALSE;                              /* requet cancelled */
  }
  ruw_TpLen = len;                                 /* store number of bytes to send */
  ruw_Index = 0;                                   /* init index */
  rpub_TxBuf = buf;                                /* store transmit buffer address */
  
  if ( cub_ADDRESSING >= TP_EXTENDED )             /* skip TA, AE */
  {
    LenFF = 6;
  }
  else
  {
    LenFF = 7;
  }
  if( ruw_TpLen <= LenFF )                         /* check single/first frame to need */
  {
    vSetTpNew(TP_CHANNEL_INDEX cuw_STOP_TIMER,TP_STATE_SEND_SF); /* tp send single frame */
  }
  else
  {
    vSetTpNew(TP_CHANNEL_INDEX cuw_STOP_TIMER,TP_STATE_SEND_FF); /* tp send first frame */
  }
  re_AppState = APP_STATE_PROCESS;                 /* application continue/tp wait for data */
  rub_Release = rel;                               /* store desired buffer handling */
  return I2S_TRUE;                                 /* request accepted */
}


/*  FUNCTION t_APP_STATE TP_eGetApplState( TP_CHANNEL_INDEX_API_2 )
 **************************************************************************
 *  PURPOSE:        This function is polled by the diagnostic application
 *                  to recognize the end of the transmission or an error state.
 **************************************************************************
 *  PARAMETERS:     i2s_ch: TP channel index
 **************************************************************************
 *  RETURN VALUE:   APP_STATE_CONFIRMATION:  Tx successful
 *                  APP_STATE_ERROR:         Tx failed
 *                  APP_STATE_PROCESS:       TP waiting for appl. data / Tx in process
 *                  APP_STATE_IDLE:          TP waiting for tester/application data
 *                  APP_STATE_INDICATION:    TP received tester data
 *************************************************************************/
t_APP_STATE TP_eGetApplState( TP_CHANNEL_INDEX_API_2 )
{
t_APP_STATE retVal;
  
  #if( TP_CHANNELS > 1 )
  if( i2s_ch >= TP_CHANNELS )
  {
    retVal = APP_STATE_IDLE;
  }
  else
  #endif
  {
    retVal = re_AppState;
  }
  return retVal;                      /* return last state */
}

/*  FUNCTION word TP_uwApplDataInd( TP_CHANNEL_INDEX_API byte *mode )
 **************************************************************************
 *  PURPOSE:        This function is polled by the diagnostic application
 *                  to recognize the receiption of a diagnotic request.
 **************************************************************************
 *  PARAMETERS:     ch:      TP channel index
 *                  mode:    target address for the addr. type to be written 
 **************************************************************************
 *  RETURN VALUE:   No of received bytes
 *************************************************************************/
word TP_uwApplDataInd( TP_CHANNEL_INDEX_API byte *mode )
{  
word retVal = 0;                               /* no request available */
  
  #if( TP_CHANNELS > 1 )
  if( i2s_ch < TP_CHANNELS )
  #endif
  {
    if( re_AppState == APP_STATE_INDICATION )  /* request available for application ? */
    {
      re_AppState = APP_STATE_PROCESS;         /* application continue/tp wait for data */
      *mode = rub_IdentType;                   /* inform about id type */
      retVal = ruw_TpLen;                      /* frame len */
    }
  }
  return retVal;
}

/*  FUNCTION void TP_vCancelRequest( TP_CHANNEL_INDEX_API_2 )
 **************************************************************************
 *  PURPOSE:        Use this function to cancel a request, no response will
 *                  be generated.
 **************************************************************************
 *  PARAMETERS:     ch:  TP channel
 **************************************************************************
 *  RETURN VALUE:   none
 *************************************************************************/
void TP_vCancelRequest( TP_CHANNEL_INDEX_API_2 )
{
  #if( TP_CHANNELS > 1 )
  if( i2s_ch < TP_CHANNELS )
  #endif
  {
    re_AppState = APP_STATE_IDLE;
    /*re_TpState = TP_STATE_IDLE;*/
  }
}

/*  FUNCTION static void vCanDataCon ( TP_CHANNEL_INDEX_API_2 )
 **************************************************************************
 *  PURPOSE:        This function is called by the can driver if a can  
 *                  message was sent completely.
 **************************************************************************
 *  PARAMETERS:     i2s_ch   : index of the tp channel
 **************************************************************************
 *  RETURN VALUE:   none
 *************************************************************************/
static void vCanDataCon ( TP_CHANNEL_INDEX_API_2 )
{
byte offset=0;

  if( re_TpState != TP_STATE_CON )  /* do nothing if tp is not waiting for confirmation */
  {
    #if( cub_BUSY_REPEAT_REQUEST != 0 )
    if( rub_BRR == cub_BUSY_CONF )
    {
      /*ruw_TpTimer = cuw_STOP_TIMER;*/
      rub_BRR = cub_BUSY_IDLE;
    } 
    #endif 
    return;
  }  
  if ( cub_ADDRESSING >= TP_EXTENDED )
  {
    offset = 1;                     /* 1 byte offset because of address information */
  }
  /* check TPCI */
  switch( ruba_CanData[0+offset] & 0xF0 )
  {
    case cub_TPCI_OC_SingleFrame:
      if( re_AppState == APP_STATE_PROCESS )         /* application waiting for confirmation ? */
      {
        if( rub_Release == I2S_FALSE )
        {
          re_AppState = APP_STATE_CONFIRMATION;      /* tp confirm application */
        }
        else
        {
          re_AppState = APP_STATE_IDLE;              /* tp confirm application */
        }
      }
      vSetTpNew(TP_CHANNEL_INDEX cuw_STOP_TIMER,TP_STATE_IDLE);      /* tp go IDLE state */
      break;

    case cub_TPCI_OC_FirstFrame:
      vSetTpNew(TP_CHANNEL_INDEX cuw_MAX_TIME_CONF,TP_STATE_IND_FC); /* tp wait for flow control */
      break;

    case cub_TPCI_OC_ConsecutiveFrame:           
      if( ruw_Index == ruw_TpLen )                   /* all data sent ? */
      {
        if( re_AppState == APP_STATE_PROCESS )       /* application waiting for confirmation ? */
        {
          if( rub_Release == I2S_FALSE )
          {
            re_AppState = APP_STATE_CONFIRMATION;    /* tp confirm application */
          }
          else
          {
            re_AppState = APP_STATE_IDLE;                            /* tp confirm application */
          }
        }
        vSetTpNew(TP_CHANNEL_INDEX cuw_STOP_TIMER,TP_STATE_IDLE);    /* tp go IDLE state */
      }
      else if( rub_BlockSize == (byte)0 )                            /* bs=0 --> send all without FC */
      {
        offset = rub_SepTime+1;
        vSetTpNew(TP_CHANNEL_INDEX offset,TP_STATE_SEND_CF);         /* tp send consecutive frame */
      }
      else 
      {
        rub_BlockSize --;
        if( rub_BlockSize == (byte)0 )                                  /* number of allowed blocks sent ? */
        {
          vSetTpNew(TP_CHANNEL_INDEX cuw_MAX_TIME_CONF,TP_STATE_IND_FC);/* tp wait for flow control */
        }
        else
        {
          offset = rub_SepTime+1;
          vSetTpNew(TP_CHANNEL_INDEX offset,TP_STATE_SEND_CF);       /* tp send consecutive frame */
        }
      }
      break;

    case cub_TPCI_OC_FlowControl:
      rub_BlockSize = ruba_CanData[1+offset];                        /* store own BS */
      rub_SepTime = ruba_CanData[2+offset];                          /* store own ST */
      if( (ruba_CanData[0+offset] & cub_OVERFLOW_BIT) != 0 )         /* overflow bit set ? */
      {
        vSetTpNew(TP_CHANNEL_INDEX cuw_STOP_TIMER,TP_STATE_IDLE);       /* tp wait for consecutive frame */
      }
      else
      { 
        vSetTpNew(TP_CHANNEL_INDEX cuw_MAX_TIME_CONF,TP_STATE_IND_CF);  /* tp wait for consecutive frame */
      }
      break;

    /* unknown state (normally never reached) */
    default:
      /* ignore invalid TPCIs */
      break;
  }
}

/*  FUNCTION static void vCanDataInd( TP_CHANNEL_INDEX_API byte IdType, byte* RxD )
 **************************************************************************
 *  PURPOSE:        This function is called by the can driver if a can  
 *                  message was received completely.
 **************************************************************************
 *  PARAMETERS:     i2s_ch : index of the tp channel
 *                  IdType: rx can identifier
 *                  RxD   : pointer to received can data
 **************************************************************************
 *  RETURN VALUE:   none
 *************************************************************************/
static void vCanDataInd( TP_CHANNEL_INDEX_API byte IdType, byte* RxD )
{
byte Tmp,MaxLen;
byte offset=0, retVal=I2S_FALSE;
  
  if ( cub_ADDRESSING >= TP_EXTENDED ) /* skip TA, AE */
  {
    offset = 1;
    MaxLen = 6;
  }
  else
  {
    MaxLen = 7;
  }
  /* check by TPCI */
  switch( RxD[0+offset] & 0xF0 )
  {
    /* ------------------------------------ */
    /* | TPCI/Len | Data (max. 6/7 bytes) | */  
    /* ------------------------------------ */
    case cub_TPCI_OC_SingleFrame:      
      Tmp = RxD[0+offset] & 0x0F;                              /* extract length from TPCI */
      if( cub_UDS_CLIENT )
      {
        retVal = UDS_ubIgnoreFuncReq( &RxD[1+offset], Tmp, IdType);
        if( retVal == I2S_TRUE )                               /* dont send busy for funct. testerPresent */
        {
          return;
        }
      } 
      if( re_AppState != APP_STATE_IDLE )                      /* check if application is in process */
      {        
          #if( cub_BUSY_REPEAT_REQUEST != 0 )
          if( cub_UDS_CLIENT )                                 /* do BRR handling for UDS clients only */ 
          {
            if( rub_BRR == cub_BUSY_IDLE )
            {
              rub_BRR = RxD[1+offset];                         /* send busy */
            }
          }
          #endif  
          return;                                              /* don´t process SF request */
      }
      if( (Tmp > 0 ) && (Tmp <= MaxLen) )                      /* check len and R-Bit */
      { 
        memcpy(&rpub_RxBuf[0], &RxD[1+offset], Tmp);           /* copy data */      
        ruw_TpLen = Tmp;
        re_AppState = APP_STATE_INDICATION;                    /* tp inform application */
        rub_IdentType = IdType;                                /* save identifier type */
      }
      vSetTpNew(TP_CHANNEL_INDEX cuw_STOP_TIMER,TP_STATE_IDLE);              /* tp go idle */
      break;

    /* ------------------------------------------ */
    /* | TPCI/Len | Len | Data (max. 5/6 bytes) | */  
    /* ------------------------------------------ */
    case cub_TPCI_OC_FirstFrame:
      if( IdType >= cub_FUNCTIONAL )                           /* reject functional FFs */
      {
        return;
      }
      if( re_AppState != APP_STATE_IDLE )                      /* reject if application not IDLE */
      {
        return;
      }
      ruw_TpLen = (((word)RxD[0+offset] << 8) + RxD[1+offset])&0xFFF;  /* extract length from TPCI */
      if( ruw_TpLen < (MaxLen+1) )
      {
        return;                                                /* min len for seg. must be 8 */
      }
      rub_Sequence = 0;                                        /* init sequeces numbering */
      memcpy(&rpub_RxBuf[0], &RxD[2+offset], MaxLen-1);      /* store data */           
      ruw_Index = MaxLen-1;                                    /* store copied data len */
      rub_IdentType = IdType;                                  /* save identifier type */
      vSetTpNew(TP_CHANNEL_INDEX cuw_MIN_TIME_CONF,TP_STATE_SEND_FC);        /* tp send flow control */
      break;

    /* consecutive frame received */
    /* ---------------------------------- */
    /* | TPCI/SN | Data (max. 6/7 bytes | */  
    /* ---------------------------------- */
    case cub_TPCI_OC_ConsecutiveFrame:
      if( IdType >= cub_FUNCTIONAL )                           /* reject functional CFs */
      {
        return;
      }
      if( re_TpState == TP_STATE_IND_CF )                      /* is tp waiting for data ? */
      {
        rub_Sequence++;                                        /* update sequence number */
        if( ((RxD[0+offset] ^ rub_Sequence) & 0xF) != 0 )      /* expected and received SN equal ? */
        {
          rub_Sequence = 0;                                    /* reset SN and TP go IDLE */
          vSetTpNew(TP_CHANNEL_INDEX cuw_STOP_TIMER,TP_STATE_IDLE);
        }
        else
        {
          word LenTmp = ruw_TpLen-ruw_Index; 
          if( LenTmp > MaxLen )
          {
            LenTmp = MaxLen;
          }
          memcpy(&rpub_RxBuf[ruw_Index], &RxD[1+offset], LenTmp); /* store data */
          ruw_Index += LenTmp;                                    /* update no of received bytes */
          if( ruw_Index >= ruw_TpLen )                         /* all bytes received ? */
          {
            re_AppState = APP_STATE_INDICATION;                /* tp inform application */
            rub_IdentType = IdType;                            /* save identifier type */
            vSetTpNew(TP_CHANNEL_INDEX cuw_STOP_TIMER,TP_STATE_IDLE);        /* tp go IDLE */
          }
          else 
          {
            rub_BlockSize --;
            if( rub_BlockSize == (byte)0 )                     /* allowed no of blocks received ? */
            {
              vSetTpNew(TP_CHANNEL_INDEX cuw_MIN_TIME_CONF,TP_STATE_SEND_FC); /* tp send flow control */
            }
            else
            {
              vSetTpNew(TP_CHANNEL_INDEX cuw_MAX_TIME_CONF,TP_STATE_IND_CF);  /* tp wait for consecutive frame */
            }
          }
        }
      }
      break;

    /* ------------------------ */
    /* | TPCI/FS | BS | STmin | */   
    /* ------------------------ */
    case cub_TPCI_OC_FlowControl: 
      if( IdType >= cub_FUNCTIONAL )                           /* reject functional FCs */
      {
        return;
      }
       /* ignore FC if not waiting for */
      if( re_TpState == TP_STATE_IND_FC )                 
      {
        /* overflow? or invalid flow status? */
        if( (RxD[0+offset] & 0x0E) != 0 )
        {  
          vSetTpNew(TP_CHANNEL_INDEX cuw_STOP_TIMER,TP_STATE_IDLE); /* to go IDLE */
          if( rub_Release == I2S_FALSE )
          {
            re_AppState = APP_STATE_ERROR;              /* application go ERROR */
          }
          else
          {
            re_AppState = APP_STATE_IDLE;               /* application go IDLE */
          }
        }
        else
        {
          rub_BlockSize = RxD[1+offset];                /* accept new BS */
          Tmp = RxD[2+offset];
          if( Tmp <= 0x7F )                             /* check for valid STmin */
          {
            rub_SepTime = Tmp;                          /* accept new ST */
          }
          else if( (Tmp >= 0xF1) && (Tmp <= 0xF9) )
          {
            rub_SepTime = 1;                            /* set STmin = 1ms */
          }
          else
          {
            rub_SepTime = 127;                          /* set max STmin = default value */
          }          
          if( (RxD[0+offset] & cub_WAIT_BIT) != 0 )     /* check for wait */
          {
            vSetTpNew(TP_CHANNEL_INDEX cuw_MAX_TIME_CONF,TP_STATE_IND_FC);     /* tp wait for flow control */
          }
          else
          {
            vSetTpNew(TP_CHANNEL_INDEX (word)rub_SepTime+1,TP_STATE_SEND_CF);  /* tp send consecutive frame */
          }
        }
      }
      break;

    /* unknown TPCI (normally never reached) */
    default:
      /* ignore invalid TPCIs */
      break;
  }
}

/*  FUNCTION static byte CheckAddresses( byte* channel, dword canID, byte *data )
 **************************************************************************
 *  PURPOSE:        This function checks the TA, SA and the can identifier
 *                  if they match with the expected values.
 **************************************************************************
 *  PARAMETERS:     canID:   rx can identifier
 *                  data:    pointer to the rx data
 *                  channel: address of the output variable for channel index
 **************************************************************************
 *  RETURN VALUE:   cub_PHYSICAL
 *                  cub_FUNCTIONAL
 *                  cub_OBD
 *                  cub_INVALID_ID
 *************************************************************************/
#if( TP_CHANNELS > 1 )
static byte ubCheckAddresses( byte* channel, dword canID, byte *data )
#else
static byte ubCheckAddresses( dword canID, byte *data )
#endif
{  
  #if( TP_CHANNELS > 1 )
  byte i2s_ch;
  for( i2s_ch=0; i2s_ch<TP_CHANNELS; i2s_ch++ )
  {
    *channel = i2s_ch;
  #else
  {
  #endif
    if ( cub_ADDRESSING == TP_NORMAL )
    {
      if( canID == cul_REQ_CAN_ID_11BIT_PHYS )
      {
      	return cub_PHYSICAL;
      }
      if( canID == cul_REQ_CAN_ID_11BIT_FUNC )
      {
      	return cub_FUNCTIONAL;
      }
      if( cub_ISO15765_4_ADDRESSING == 1 )
      {
        if( canID == cul_REQ_CAN_ID_11BIT_OBD )
        {
        	return cub_OBD;
        }
      }
      if( cub_SECOND_FUNC_ADDRESS == 1 )
      {
        if( canID == cul_REQ_CAN_ID_11BIT_FUNC_2 )
        {
        	return cub_FUNCTIONAL;
        }
      }
    }
    else if( cub_ADDRESSING == TP_EXTENDED )
    {
      if( canID == cul_REQ_CAN_ID_11BIT_PHYS )
      {
        if ( data[0] == cub_ECU_ADR_PHYS )
        {
           return cub_PHYSICAL;
        }
      }
      else if( canID == cul_REQ_CAN_ID_11BIT_FUNC )
      {
        if ( data[0] == cub_ECU_ADR_FUNC )
        {
        	 return cub_FUNCTIONAL;
        }
      }
      else
      {
        if( cub_ISO15765_4_ADDRESSING == 1 )
        {
          if( canID == cul_REQ_CAN_ID_11BIT_OBD )
          {
            if ( data[0] == cub_ECU_ADR_OBD )
            {
             	 return cub_OBD;
            }
          }
        }
        if( cub_SECOND_FUNC_ADDRESS == 1 )
        {
          if( canID == cul_REQ_CAN_ID_11BIT_FUNC_2 )
          {
            if ( data[0] == cub_ECU_ADR_FUNC )
            {
            	return cub_FUNCTIONAL;
            }
          }
        }
      }      
    }
    else if( cub_ADDRESSING == TP_NORMAL_FIXED )
    {
      dword id = canID & 0xFFFF0000;
      byte TA = (canID & 0x0000FF00) >> 8;
      byte SA = canID & 0x000000FF;
      {
         if( id == cul_CAN_ID_FIXED_PHYS )
         {
            if ( TA == cub_ECU_ADR_PHYS )
            {
              rub_TesterAdr = SA; 
              return cub_PHYSICAL;
            }
         }
         else if( id == cul_CAN_ID_FIXED_FUNC )
         {
           if( TA == cub_ECU_ADR_FUNC )
           {
             rub_TesterAdr = SA; 
             return cub_FUNCTIONAL;
           }
           if( cub_ISO15765_4_ADDRESSING == 1 )
           {
             if( TA == cub_ECU_ADR_OBD )
             {
               rub_TesterAdr = SA; 
               return cub_OBD;
             }
           }
         }
         else {}
      }
    }
    else if( cub_ADDRESSING == TP_MIXED )
    {
      dword id = canID & 0xFFFF0000;
      byte TA = (canID & 0x0000FF00) >> 8;
      byte SA = canID & 0x000000FF;
      {
         if( data[0] != cub_ADR_EXTENSION )
         {
           if( id == cul_CAN_ID_MIXED_PHYS )
           {
              if( TA == cub_ECU_ADR_PHYS )
              {
                rub_TesterAdr = SA; 
                return cub_PHYSICAL;
              }
           }
           else if( id == cul_CAN_ID_MIXED_FUNC )
           {
              if( TA == cub_ECU_ADR_FUNC )
              {
                rub_TesterAdr = SA; 
                return cub_FUNCTIONAL;
              }
           }
           else {}
         }
      }
    }
    else if( cub_ADDRESSING == TP_MIXED_11BIT )
    {
       if( data[0] == cub_ADR_EXTENSION )
       {
          if( canID == cul_REQ_CAN_ID_11BIT_PHYS )
          {
          	return cub_PHYSICAL;
          }
          if( canID == cul_REQ_CAN_ID_11BIT_FUNC )
          {
          	return cub_FUNCTIONAL;
          }
       }
    }
    else
    {
    }
  }
  return cub_INVALID_ID;
}

/*  FUNCTION static byte ubFillPaddingBytes( TP_CHANNEL_INDEX_API byte len )
 **************************************************************************
 *  PURPOSE:        This function fills unused bytes with the padding value
 *                  if padding feature activated and returns the new length
 *                  of the can frame.
 **************************************************************************
 *  PARAMETERS:     i2s_ch : index of the tp channel
 *                  len   : length of the can frame before padding (max. 8)
 **************************************************************************
 *  RETURN VALUE:   length of the can frame after padding (max. 8)
 *************************************************************************/
static byte ubFillPaddingBytes( TP_CHANNEL_INDEX_API byte len )
{ 
    #if ( cub_USE_PADDING != 0 )
    while( len < cub_CAN_SIZE )
    {
      ruba_CanData[len] = cub_PADDING_BYTE;
      len ++; 
    }
    #endif
    return len;
}

/*  FUNCTION static void vSetTpNew(TP_CHANNEL_INDEX_API word T, t_TP_STATE S)
 **************************************************************************
 *  PURPOSE:        static function to set the new timer value and new state
 *                  of the module after proceeding an action, e. g. an
 *                  indication or confirmation of a message etc.
 **************************************************************************
 *  PARAMETERS:     timer value in ms, state
 **************************************************************************
 *  RETURN VALUE:   none
 *************************************************************************/
static void vSetTpNew( TP_CHANNEL_INDEX_API word T, t_TP_STATE S)
{ 
  ruw_TpTimer = T;
  re_TpState = S;
}


/*  FUNCTION static byte ubCanDataReq( byte ch, byte dlc )
 **************************************************************************
 *  PURPOSE:        This function requests the can driver to send a can
 *                  message and returns the result of the request.
 **************************************************************************
 *  PARAMETERS:     ch:      TP channel
 *                  tp_data: start address of send data
 *                  dlc:     number of byte to send (max. 8) 
 **************************************************************************
 *  RETURN VALUE:   1: request accepted
 *                  0: request denied
 *************************************************************************/
static byte ubCanDataReq( TP_CHANNEL_INDEX_API byte dlc )
{
  byte ub_Cnt;
  byte* txbuf = CanTxDataPtr[CAN_TX_HANDLE];
  for( ub_Cnt=0; ub_Cnt<dlc; ub_Cnt++)
  {
    txbuf[ub_Cnt] = ruba_CanData[ub_Cnt];
  }
  ub_Cnt = CanTransmit( CAN_TX_HANDLE ); /* send data by handle */ 
  return ub_Cnt;
}

/*  FUNCTION void I2V_vCanDataCon( CanTransmitHandle txObject )
 **************************************************************************
 *  PURPOSE:        User specific function which is called in the scope of
 *                  the CAN transmit interrupt routine after the message has
 *                  been sent on the CAN bus successfully.
 *                  Format of this function is given by Vector: See CANdrv
 *                  documentation chapter "UserConfirmation". 
 **************************************************************************
 *  PARAMETERS:     txObject: Handle of the transmit object
 **************************************************************************
 *  RETURN VALUE:   none
 *************************************************************************/
void I2V_vCanDataCon( CanTransmitHandle txObject )
{
  /* WORKAROUND: the confirmation function is disabled in case of using */
  /* the Vector can driver */ 
  #if ( TP_CHANNELS > 1 )
  byte i2s_ch;
  for( i2s_ch=0; i2s_ch<TP_CHANNELS; i2s_ch++ )  /* look for valid handle */
  {
    if( acub_CAN_TX_HANDLE[i2s_ch] == txObject ) /* handle valid one ? */
    {
      break;
    }
  }
  if( i2s_ch >= TP_CHANNELS )
  {
    return; /* handle not valid/found  --> can not assign to a tp channel */
  }
  #endif
  vCanDataCon( TP_CHANNEL_INDEX_2 );
}         

/*  FUNCTION canuint8 I2V_ubCanDataPrecopy( xx )
 **************************************************************************
 *  PURPOSE:        User specific function which is called in the receive
 *                  interrupt of a CAN message before data has been copied
 *                  and the CAN Controller receive register have been released. 
 *                  Format of this function is given by Vector: See CANdrv
 *                  documentation. The function prototype is HW depended and
 *                  is controlled by can_def.h 
 **************************************************************************
 *  PARAMETERS:     rxObject:    handle of the receive object
 *               or rxDataPtr:   byte pointer to receive data
 *               or rxStruct:    structure pointer to receive data
 *               or CanID, Data: can identifier and pointer to receive data
 **************************************************************************
 *  RETURN VALUE:   kCanNoCopyData
 *************************************************************************/
#if defined ( C_SINGLE_RECEIVE_BUFFER )
#define CanID CanRxActualId
#define CanDLC CanRxActualDLC
#define Data CanRDSPtr
canuint8 I2V_ubCanDataPrecopy(CanReceiveHandle rxObject)

#elif defined ( C_MULTIPLE_RECEIVE_BUFFER )
#define CanID CanRxActualId
#define CanDLC CanRxActualDLC
#define Data rxDataPtr
canuint8 I2V_ubCanDataPrecopy(CanChipDataPtr rxDataPtr)

#elif defined ( C_MULTIPLE_RECEIVE_CHANNEL ) || defined (C_SINGLE_RECEIVE_CHANNEL )
#define CanID CanRxActualId(rxStruct)
#define CanDLC CanRxActualDLC(rxStruct)
#define Data rxStruct->pChipData
canuint8 I2V_ubCanDataPrecopy(CanRxInfoStructPtr rxStruct)
#else
canuint8 I2V_ubCanDataPrecopy(dword CanID, byte CanDLC, byte* Data)
#endif
{
byte IdType;
#if( TP_CHANNELS > 1 )
byte i2s_ch;
#endif
   
  #if ( cub_USE_PADDING != 0 )
  if( CanDLC != cub_CAN_SIZE )
  {
    return kCanNoCopyData; /* ignore can frames with len not equal to 8 bytes */
  }
  #endif
   
  #if( TP_CHANNELS > 1 )
  IdType = ubCheckAddresses( &i2s_ch, CanID, (byte*)Data );
  #else
  IdType = ubCheckAddresses( CanID, (byte*)Data );
  #endif
  
  if( IdType != cub_INVALID_ID )
  {
    if( IdType == cub_PHYSICAL )
    {
      /* indication received before tx confirmation --> simulate confirmation */
      if( re_TpState == TP_STATE_CON )
      {
        vCanDataCon( TP_CHANNEL_INDEX_2 );
      }  
      vCanDataInd( TP_CHANNEL_INDEX  cub_PHYSICAL, (byte*)Data );
    }
    else
    {
      #if( TP_CHANNELS > 1 )
      /* copy functional message to all concerning channels */
      for( i2s_ch=0; i2s_ch<TP_CHANNELS; i2s_ch++ )
      #endif
      {
        if( cub_UDS_CLIENT != 0 )  /* client must be UDS for functional messages ! */
        {
          /* copy data only if not OBD or copy always if client is participates on OBD */ 
          if( (IdType != cub_OBD) || (cub_ISO15765_4_ADDRESSING != 0) )
          {
            /* indication received before tx confirmation --> simulate confirmation */
            if( re_TpState == TP_STATE_CON )
            {
              vCanDataCon( TP_CHANNEL_INDEX_2 );
            }  
            vCanDataInd( TP_CHANNEL_INDEX  IdType, (byte*)Data );
          }
        }
      }
    }
  }
  return kCanNoCopyData;
}

