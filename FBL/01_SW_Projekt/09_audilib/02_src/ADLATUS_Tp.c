/*-----------------------------------------------------------------------------+
|            ____    ___            ____    _____           ____               |
|           |    |  |   \   |      |    |     |    |    |  |                   |
|           |____|  |    |  |      |____|     |    |    |  |____               |
|           |    |  |    |  |      |    |     |    |    |       |              |
|           |    |  |___/   |____  |    |     |    |____|   ____|              |
|                                                                              |
|           T H E   S M A R T   E M B E D D E D   S O L U T I O N              |
|                                                                              |
+------------------------------------------------------------------------------+
| F I L E   I N F O R M A T I O N                                              |
+------------------------------------------------------------------------------+
|                                                                              |
|  Filename:  ADLATUS_TP.c                                                     |
|                                                                              |
|  Comment:   This file includes the Standard ISO Transport Protocol via CAN   |
|             ISO 15765 .                                                      |
|                                                                              |
+------------------------------------------------------------------------------+
| R E V I S I O N   H I S T O R Y                                              |
+------------------------------------------------------------------------------+
|
| Subversion Info:
| $Id: ADLATUS_Tp.c 4396 2008-11-04 09:26:32Z maennel $
| $HeadURL: http://smartsvn/svn/adlatusrepository/auslieferungen/S1NN/086350-S1NN-ADLATUS-TMS470R-MucSuc/20081201_S1NN_TMS470R_BlfIso-Source_V1.00/01_SW_Projekt/09_audilib/02_src/ADLATUS_Tp.c $
|
+------------------------------------------------------------------------------+
|      (c) SMART ELECTRONIC DEVELOPMENT GMBH - all rights reserved             |
+-----------------------------------------------------------------------------*/

/* ========================================================================== */
/* (1)  I N C L U D E S                                                       */
/* -------------------------------------------------------------------------- */
/*      C O N S T A N T   D E F I N I T I O N   F I L E S  (CDF)              */
/* ========================================================================== */
#include "ADLATUS_CompilerSwitches_cdf.h"    /* must be the first CDF-include */
#include "ADLATUS_Global_cdf.h"
#include "ADLATUS_ConfigSystem_cdf.h"

/* ========================================================================== */
/* (2)  I N C L U D E S                                                       */
/* -------------------------------------------------------------------------- */
/*      T Y P E   D E F I N I T I O N   F I L E S  (TDF)                      */
/* ========================================================================== */
#include "ADLATUS_Types_tdf.h"                 /* must be the first include   */
#include "ADLATUS_Global_tdf.h"
#include "ADLATUS_TP_tdf.h"

#include "ADLATUS_EventHandlerAudi_tdf.h"

/* ========================================================================== */
/* (3)  C O M P I L E R   S W I T C H   S E T T I N G S                       */
/* -------------------------------------------------------------------------- */
/*      O N L Y   F O R   T H I S   M O D U L                                 */
/* ========================================================================== */
/* Extended Identifier                                                        */
/*#define CD_CCtp_ExtendedID */

/* length of telegram depends on message length */
/*#define CD_CCtp_VARIABLE_TG_LENGTH           */

/* define for stmin 0 */
/* #define STMIN_ZERO */

/* ========================================================================== */
/* (4)  D A T A   S E C T I O N   D I R E C T I V E S                         */
/* -------------------------------------------------------------------------- */
/*      O N L Y   F O R   T H I S   M O D U L                                 */
/* ========================================================================== */
/* NO ENTRY                                                                   */

/* ========================================================================== */
/* (5)  L O C A L   D E F I N E S                                             */
/* -------------------------------------------------------------------------- */
/*      O N L Y   F O R   T H I S   M O D U L                                 */
/* ========================================================================== */

/*---------------------------------------------------------------------*/
/*                                                                     */
/*   E S C I  I I I I                                                  */
/*   | | | |  | | | |                                                  */
/*   | | | +--+-+-+-+---    INDEX                                      */
/*   | | +--------------    ERROR                                      */
/*   | +----------------    BUSY                                       */
/*   +------------------    READY                                      */
/*                                                                     */
/*---------------------------------------------------------------------*/
#define  D_FCtp_FLAG_NoReturnValue_UB  (UBYTE) 0x00u

#define  D_FCtp_FLAG_Idle_UB            d_CCglo_InitToZero
#define  D_FCtp_FLAG_Error_UB           d_CCglo_Error
#define  D_FCtp_FLAG_Busy_UB            d_CCglo_Busy
#define  D_FCtp_FLAG_Ready_UB           d_CCglo_Ready

#define  D_CCtp_BufferValid_UB          d_CCglo_BufferValid

/* tp specific flag */

#define  c_FCpara_FLAG_TPRXTimeout_UB   (UBYTE) 0x01u
#define  c_FCpara_FLAG_TPTXTimeout_UB   (UBYTE) 0x02u

#define  c_FCpara_FLAG_TPFlowControl_UB (UBYTE) 0x08u




#define D_CCtp_KWP_PRIORITY             (UBYTE) 0x00u
#define D_CCtp_FLOWCONTROL_PRIORITY     (UBYTE) 0x01u
#define D_CCtp_TESTERPRESENT_PRIORITY   (UBYTE) 0x02u


/* byte zero is statusinfo */
#define D_CCtp_BUFFERSTATUS  d_CCglo_BufferStatus

/* byte one is the channel number */
#define D_CCtp_BUFFERCHANNEL d_CCglo_ChannelNbr

/* byte two includes legal buffer length */
#define D_CCtp_BUFFERLENGTH d_CCglo_MaxBufferLength

/* byte three includes received/sent data length */
#define D_CCtp_BUFFERDATA   d_CCglo_DLC

/* from byte four to end is data section */
#define D_CCtp_BUFFEROFFSET d_CCglo_StartOfData



/*--------------------------------------------------------------------------*/
/* Initialisation                                                           */
/*--------------------------------------------------------------------------*/
#define D_CCtp_INIT_TO_ZERO             0x00 

/****************************************************************************
* Transport protocol TPCI codes and protocol lengths                        *
****************************************************************************/
#define D_CCtp_CFRAMES   (UBYTE) 0x00u

#ifdef CD_CCtp_ExtendedID

#define D_CCtp_ExendedID_POSITON (UBYTE) 0x00u
#define D_CCtp_SF_LENGTH         (UBYTE) 0x06u
#define D_CCtp_FF_LENGTH         (UBYTE) 0x05u
#define D_CCtp_CF_LENGTH         (UBYTE) 0x06u


#define D_CCtp_PCI_POS_SF   (UBYTE) (D_CCtp_ExendedID_POSITON + (UBYTE) 1u)
#define D_CCtp_SID_POS_SF   (UBYTE) (D_CCtp_PCI_POS_SF        + (UBYTE) 1u)
#define D_CCtp_LID_POS_SF   (UBYTE) (D_CCtp_SID_POS_SF        + (UBYTE) 1u)

#define D_CCtp_CNT_POS_FF   (UBYTE) (D_CCtp_ExendedID_POSITON + (UBYTE) 1u)
#define D_CCtp_PCI_POS_FF   (UBYTE) (D_CCtp_CNT_POS_FF        + (UBYTE) 1u)
#define D_CCtp_SID_POS_FF   (UBYTE) (D_CCtp_PCI_POS_FF        + (UBYTE) 1u)
#define D_CCtp_LID_POS_FF   (UBYTE) (D_CCtp_SID_POS_FF        + (UBYTE) 1u) 

#define D_CCtp_CNT_POS_CF   (UBYTE) (D_CCtp_ExendedID_POSITON + (UBYTE) 1u)
#define D_CCtp_DATA_POS_CF  (UBYTE) (D_CCtp_CNT_POS_CF        + (UBYTE) 1u)

#define D_CCtp_PCI_POS_FC   (UBYTE) (D_CCtp_ExendedID_POSITON + (UBYTE) 1u)
#define D_CCtp_BLOCK_POS_FC (UBYTE) (D_CCtp_PCI_POS_FC        + (UBYTE) 1u)
#define D_CCtp_TIME_POS_FC  (UBYTE) (D_CCtp_BLOCK_POS_FC      + (UBYTE) 1u)


#define D_CCtp_TG_LENGTH    (UBYTE) 0x08u

#ifdef CD_CCtp_VARIABLE_TG_LENGTH             
  #define D_CCtp_FC_TG_LENGTH (UBYTE) 0x03u
#else
  #define D_CCtp_FC_TG_LENGTH (UBYTE) 0x08u
#endif

#else

#define D_CCtp_SF_LENGTH (UBYTE) 0x07u
#define D_CCtp_FF_LENGTH (UBYTE) 0x06u
#define D_CCtp_CF_LENGTH (UBYTE) 0x07u

#define D_CCtp_PCI_POS_SF   (UBYTE) 0x00u
#define D_CCtp_SID_POS_SF   (UBYTE) (D_CCtp_PCI_POS_SF + (UBYTE) 1u)
#define D_CCtp_LID_POS_SF   (UBYTE) (D_CCtp_SID_POS_SF + (UBYTE) 1u)

#define D_CCtp_CNT_POS_FF   (UBYTE) 0x00u
#define D_CCtp_PCI_POS_FF   (UBYTE) (D_CCtp_CNT_POS_FF + (UBYTE) 1u)
#define D_CCtp_SID_POS_FF   (UBYTE) (D_CCtp_PCI_POS_FF + (UBYTE) 1u)
#define D_CCtp_LID_POS_FF   (UBYTE) (D_CCtp_SID_POS_FF + (UBYTE) 1u) 

#define D_CCtp_CNT_POS_CF   (UBYTE) 0x00u
#define D_CCtp_DATA_POS_CF  (UBYTE) (D_CCtp_CNT_POS_CF + (UBYTE) 1u)

#define D_CCtp_PCI_POS_FC   (UBYTE) 0x00u
#define D_CCtp_BLOCK_POS_FC (UBYTE) (D_CCtp_PCI_POS_FC + (UBYTE) 1u)
#define D_CCtp_TIME_POS_FC  (UBYTE) (D_CCtp_BLOCK_POS_FC + (UBYTE) 1u)


#define D_CCtp_TG_LENGTH (UBYTE) 0x08u

#ifdef CD_CCtp_VARIABLE_TG_LENGTH             
  #define D_CCtp_FC_TG_LENGTH (UBYTE) 0x03u
#else
  #define D_CCtp_FC_TG_LENGTH (UBYTE) 0x08u
#endif

#endif

#define D_CCtp_SINGLEFRAME        (UBYTE) 0x00u
#define D_CCtp_FIRSTFRAME         (UBYTE) 0x10u
#define D_CCtp_CONSECUTIVEFRAME   (UBYTE) 0x20u
#define D_CCtp_FLOWCONTROLFRAME   (UBYTE) 0x30u

/****************************************************************************
* Description : Flags for controlling the TP-Layer.                         *
*               The higher byte is used for "global" states.                *
*               The lower byte is for the control of sending and receiving. *
*                                                                           *
*  7654 3210 7654 3210                                                      *
*  |||| |||| |||| |||+- 0001                                                *
*  |||| |||| |||| ||+-- 0002                                                *
*  |||| |||| |||| |+--- 0004                                                *
*  |||| |||| |||| +---- 0008 D_CCtp_TX_SEGMENTEDMODEAKTIVE                  *
*  |||| |||| ||||                                                           *
*  |||| |||| |||+------ 0010 D_CCtp_FC_HASTO_COME_STATE                     *
*  |||| |||| ||+------- 0020 D_CCtp_FC_RECEIVED_STATE                       *
*  |||| |||| |+-------- 0040 D_CCtp_FF_RECEIVED_STATE                       *
*  |||| |||| +--------- 0080 D_CCtp_FF_TO_SEND_STATE                        *
*  |||| ||||                                                                *
*  |||| |||+----------- 0100 D_CCtp_TP_TO_SEND_STATE                        *
*  |||| ||++----------- 0200 D_CCtp_CONF_TO_SEND_STATE                      *
*  |||| |+------------- 0400 D_CCtp_CF_HASTO_COME_STATE                     *
*  |||| +-------------- 0800 D_CCtp_FF_TOMUCHDATA                           *
*  ||||                                                                     *
*  |||+---------------- 1000 D_CCtp_FC_TO_SEND_STATE                        *
*  ||+----------------- 2000 D_CCtp_FF_SENT_STATE                           *
*  |+------------------ 4000                                                *
*  +------------------- 8000 D_CCtp_TP_BLOCKSENDING                         *
*                                                                           *
****************************************************************************/  
#define D_CCtp_ERROR                        0x0001u /* OPL326 */
#define D_CCtp_RESET_ERROR                  0xFFFEu

#define D_CCtp_FORMER_MSG_RCV               0x0002u /* ID549 */
#define D_CCtp_RESET_FORMER_MSG_RCV         0xFFFDu

#define D_CCtp_RCV_IN_PROGRESS              0x0004u
#define D_CCtp_RESET_RCV_IN_PROGRESS        0xFFFBu

#define D_CCtp_RX_SEG_MODEAKTIVE            0x0008u
#define D_CCtp_RESET_RX_SEG_MODEAKTIVE      0xFFF7u

#define D_CCtp_FC_HASTO_COME_STATE          0x0010u
#define D_CCtp_RESET_FC_HASTO_COME_STATE    0xFFEFu

#define D_CCtp_FC_RECEIVED_STATE            0x0020u
#define D_CCtp_RESET_FC_RECEIVED_STATE      0xFFDFu

#define D_CCtp_FF_RECEIVED_STATE            0x0040u
#define D_CCtp_RESET_FF_RECEIVED_STATE      0xFFBFu

#define D_CCtp_FF_TO_SEND_STATE             0x0080u
#define D_CCtp_RESET_FF_TO_SEND_STATE       0xFF7Fu

#define D_CCtp_CF_HASTO_COME_STATE          0x0100u
#define D_CCtp_RESET_CF_HASTO_COME_STATE    0xFEFFu

#define D_CCtp_FF_TOMUCHDATA                0x0200u
#define D_CCtp_RESET_FF_TOMUCHDATA          0xFDFFu

#define D_CCtp_FC_TO_SEND_STATE             0x0400u
#define D_CCtp_RESET_FC_TO_SEND_STATE       0xFBFFu

#define D_CCtp_FF_SENT_STATE                0x0800u
#define D_CCtp_RESET_FF_SENT_STATE          0xF7FFu

#define D_CCtp_TP_BLOCKSENDING              0x1000u
#define D_CCtp_RESET_TP_BLOCKSENDING        0xEFFFu

#define D_CCtp_TP_UNEXPECTED_SF             0x2000u
#define D_CCtp_RESET_TP_TP_UNEXPECTED_SF    0xDFFFu

/****************************************************************************/

#define D_CCtp_TIMERELAPSED              0x00u

/*----------------------------------------------------------------------------*/
/* fill bytes for the CAN messages (client 0x55; server 0xAA)                 */
/*----------------------------------------------------------------------------*/
#define D_CCtp_ServerFillByte    (UBYTE) 0xAAu

/* ========================================================================== */
/* (6)  L O C A L  T Y P E   D E F I N E S                                    */
/* -------------------------------------------------------------------------- */
/*      O N L Y   F O R   T H I S   M O D U L                                 */
/* ========================================================================== */

typedef struct
{
  /*--------------------------------------------------------------------------*/
  /* Timing                                                                   */
  /*--------------------------------------------------------------------------*/
  UWORD  STmin_UW;                       /* 2 Byte -                          */
  UWORD  STmax_UW;                       /* 2 Byte -                          */
  UWORD  P2min_UW;                       /* 2 Byte -                          */
  UWORD  P2max_UW;                       /* 2 Byte -                          */
  UWORD  P3max_UW;                       /* 2 Byte -                          */
  UWORD  FCmin_UW;                       /* 2 Byte -                          */

   /* Receive Timer for timeout detection FC */
   UWORD   RXTimer_UW; 
   UWORD   RXTimerSave_UW; 

   /* Send timer for timeslicing tgs */
   UWORD    TXTimer_UW;
   UWORD    TXTimerSave_UW;
   /* */
   UBYTE    RXDataLength_UB;
   UBYTE    TXDataLength_UB;

   UBYTE    RXDataIndex_UB;
   UBYTE    TXDataIndex_UB;
   /* */
   UBYTE    RXBlockSize_UB;
   UBYTE    RXBlockSizeSave_UB; 

   UBYTE    TXBlockSize_UB;
   UBYTE    TXBlockSizeSave_UB;
   /* */
   UBYTE    TXSeg_UB; 
   UBYTE    RXSeg_UB;

   UBYTE    TXCounter_UB; 
   UBYTE    RXCounter_UB;
   /* */

   UBYTE   TPTimerControl_UB;
   UBYTE   TPBsMax_UB;
    
   UWORD   TPstate_UW;
   
   UBYTE   TPSubChannel_UB;
   UBYTE   TPChannel_UB;

   UWORD   TPTesterPresentTimer_UW;
   UWORD   TPFlowControlTimer_UW;

   UBYTE   TPTempBuffer_UB[4+8];

  CANMSGOBJINFO_ST*   CanChannelTable_PST;    /*  4 Byte - pointer to config table */

} TP_CTRL;


/* ========================================================================== */
/* (7)  L O C A L   V A R I A B L E S  / C O N S T A N T S                    */
/* -------------------------------------------------------------------------- */
/*      O N L Y   F O R   T H I S   M O D U L                                 */
/* ========================================================================== */
static TP_CTRL sTP;

#ifdef D_CCtp_ExendedID_POSITON
const UBYTE C_CCTP_FlowControlTG_UB[] = { 0x00, 0x30,D_CCtp_CFRAMES,0x01,0xAA,0xAA,0xAA,0xAA};
const UBYTE C_CCTP_TestPresentTG_UB[] = { 0x00, 0x01,0x7E,0xAA,0xAA,0xAA,0xAA,0xAA};
#else
const UBYTE C_CCTP_FlowControlTGCas_UB[] = { 0x30,D_CCtp_CFRAMES,0x00,0xAA,0xAA,0xAA,0xAA,0xAA};

const UBYTE C_CCTP_FlowControlTG_UB[] = { 0x30,D_CCtp_CFRAMES,0x01,0xAA,0xAA,0xAA,0xAA,0xAA};
const UBYTE C_CCTP_TestPresentTG_UB[] = { 0x01,0x7E,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA};
#endif

/* ========================================================================== */
/* (8)  L O C A L   F U N C T I O N   P R O T O T Y P E S                     */
/* -------------------------------------------------------------------------- */
/*      O N L Y   F O R   T H I S   M O D U L                                 */
/* ========================================================================== */
/* NO ENTRY                                                                   */

/* ========================================================================== */
/* (9)  G L O B A L   V A R I A B L E S                                       */
/* -------------------------------------------------------------------------- */
/*      D E F I N E D   W I T H I N   T H I S   M O D U L                     */
/* ========================================================================== */
/* NO ENTRY                                                                   */

/* ========================================================================== */
/* (10) F U N C T I O N   D E F I N I T I O N S                               */
/* -------------------------------------------------------------------------- */
/*      F O R   T H I S   M O D U L                                           */
/* ========================================================================== */


/*mf ************************************************************************
*                                                                           *
*  Function Name: FUN_BBtp_Init_V                                           *
*                                                                           *
*  Prototype at:   ADLATUS_TP_TDF.h                                         *
*                                                                           *
*  Description:   Initialisation of all TP-layer specific data              *
*                                                                           *
*  Input:         void                                                      *
*                                                                           *
*  Output:        void                                                      *
*                                                                           *
*  Called by:     ----------                                                *
*                 --------------                                            *
*                                                                           *
*  Subroutines:   -                                                         *
*                                                                           *
*  History:       -                                                         *
*                                                                           *
****************************************************************************/
void FUN_CCtp_Init_V(const TPINTERFACE_ST *t_TpInterface_PST)
{

  /*------------------------------------------------------------------------*/
  /* Init timings                                                           */
  /*------------------------------------------------------------------------*/
  sTP.P2min_UW  = t_TpInterface_PST->P2min_UW + 0x0001u;
  sTP.P2max_UW  = t_TpInterface_PST->P2max_UW + 0x0001u;
  sTP.P3max_UW  = t_TpInterface_PST->P3max_UW + 0x0001u;
  sTP.STmin_UW  = t_TpInterface_PST->STmin_UW;
  sTP.STmax_UW  = t_TpInterface_PST->STmax_UW + 0x0001u;
  sTP.TPBsMax_UB = t_TpInterface_PST->BSmax_UB;

#ifdef cs_CCUDS_Audi
  sTP.FCmin_UW  = t_TpInterface_PST->P2min_UW;
#else
  sTP.FCmin_UW  = t_TpInterface_PST->P2min_UW + 0x0001u;
#endif
/*--*/

  FUN_CCtp_InitInternal_V ();
}                                                          

/*mf ************************************************************************
*                                                                           *
*  Function Name: FUN_BBtp_InitIntern_V                                     *
*                                                                           *
*  Prototype at:   ADLATUS_TP_TDF.h                                         *
*                                                                           *
*  Description:   Initialisation of all TP-layer specific data              *
*                                                                           *
*  Input:         void                                                      *
*                                                                           *
*  Output:        void                                                      *
*                                                                           *
*  Called by:     ----------                                                *
*                 --------------                                            *
*                                                                           *
*  Subroutines:   -                                                         *
*                                                                           *
*  History:       -                                                         *
*                                                                           *
****************************************************************************/
void FUN_CCtp_InitInternal_V( void )
{
/*--*/
   sTP.RXBlockSizeSave_UB = sTP.TPBsMax_UB;
   sTP.RXBlockSize_UB     = sTP.TPBsMax_UB; 
/*--*/

   sTP.TXTimerSave_UW = sTP.P2min_UW;

   /* on startup we don't know the size */
   sTP.TXBlockSizeSave_UB   = D_CCtp_INIT_TO_ZERO;
   sTP.TXBlockSize_UB       = D_CCtp_INIT_TO_ZERO;

   sTP.TPstate_UW           = D_CCtp_INIT_TO_ZERO;
   sTP.TPChannel_UB         = D_CCtp_INIT_TO_ZERO;
   sTP.TPSubChannel_UB      = D_CCtp_INIT_TO_ZERO;

   sTP.TXTimer_UW           = D_CCtp_INIT_TO_ZERO;

   sTP.RXTimer_UW           = D_CCtp_INIT_TO_ZERO;
   sTP.RXTimerSave_UW       = D_CCtp_INIT_TO_ZERO;

   sTP.TXSeg_UB             = D_CCtp_INIT_TO_ZERO;

   sTP.TXDataIndex_UB       = D_CCtp_INIT_TO_ZERO;
   sTP.RXDataIndex_UB       = D_CCtp_INIT_TO_ZERO;
   
   sTP.TPFlowControlTimer_UW = D_CCtp_INIT_TO_ZERO;
   
   sTP.TPTimerControl_UB = D_CCtp_INIT_TO_ZERO;

}                                                          

/*---------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                 |
+----------------------------------------------------------------------------+
|                                                                            |
|  Function Name:  FUN_CCTp_Timer_V                                          |
|                                                                            |
|  Prototype at:   ADLATUS_Tp_TDF.h                                          |
|                                                                            |
|  Description:    Timermodul for the transport layer                        | 
|                                                                            |
|  Parameter:      -                                                         |
|                                                                            |
|  Return Value:   -                                                         |
|                                                                            |
+---------------------------------------------------------------------------*/
void FUN_CCTp_Timer_V( UWORD t_TpTimerTicks_UW )
{
  /* Currently Not used. This is to prevent a compiler warning and for      */
  /* updates in future                                                      */
  t_TpTimerTicks_UW |= 0x0000u;

  /*========================================================================*/
  /* -1- Rx Timer is still running                                          */
  /*========================================================================*/
  if( sTP.RXTimer_UW != D_CCtp_TIMERELAPSED) 
  {
    /* Decrement timer value                                                */
    sTP.RXTimer_UW--;

    /*========================================================================*/
    /* -2- timeout occured -> reinit TP-layer                                 */
    /*========================================================================*/
    if(sTP.RXTimer_UW == D_CCtp_TIMERELAPSED) 
    {
      /* Re-init tp --> Set ErrorFlag                                         */
        sTP.TPTimerControl_UB = (D_FCtp_FLAG_Error_UB | c_FCpara_FLAG_TPRXTimeout_UB);
    } /* -2- END OF if (..) --------------------------------------------------*/
  } /* ---1- END OF if (..) --------------------------------------------------*/

  /*========================================================================*/
  /* -1- Tx Timer is still running                                          */
  /*========================================================================*/
  if(sTP.TXTimer_UW != D_CCtp_TIMERELAPSED) 
  {
    /* Decrement timer value                                                */
    sTP.TXTimer_UW--;

    /*======================================================================*/
    /* -2- Timer elapsed -> Sending a message is now possible               */
    /*======================================================================*/
    if(sTP.TXTimer_UW == D_CCtp_TIMERELAPSED) 
    {
      /* send ready if timer is elapsed                                     */
      sTP.TPTimerControl_UB |= (D_FCtp_FLAG_Ready_UB | c_FCpara_FLAG_TPTXTimeout_UB);
      /* reset busy flag                                                    */
      sTP.TPTimerControl_UB &= (UBYTE) ~D_FCtp_FLAG_Busy_UB;

    } /* -2- END OF if(..) -------------------------------------------------*/

    /*======================================================================*/
    /* -2- Timer not elapsed -> Sending a message is currently not possible */
    /*======================================================================*/
    else
    {
      /* set busy because timer is not elapsed                              */
      sTP.TPTimerControl_UB |= D_FCtp_FLAG_Busy_UB;
    } /* -2- END OF else (if(..)) ------------------------------------------*/
  } /* ---1- END OF if (..) ------------------------------------------------*/

  /*------------------------------------------------------------------------*/
  /* Changed from former versions: always look after the flow control       */
  /* timer, otherwise the sending of the flowcontrol takes long time.       */
  /*------------------------------------------------------------------------*/
  /*========================================================================*/
  /* -1- Flow control timer is still running                                */
  /*========================================================================*/
  if(sTP.TPFlowControlTimer_UW != D_CCtp_TIMERELAPSED) 
  {
    /* Decrement timer value                                                */
    sTP.TPFlowControlTimer_UW--;
    /*======================================================================*/
    /* -2- Timer elapsed - Flow Control sending is necessary                */
    /*======================================================================*/
    if(sTP.TPFlowControlTimer_UW == D_CCtp_TIMERELAPSED) 
    {
      /* Set ready flag for flow control                                    */
      sTP.TPTimerControl_UB |= (D_FCtp_FLAG_Ready_UB | c_FCpara_FLAG_TPFlowControl_UB);
      /* reset busy flag                                                    */
      sTP.TPTimerControl_UB &= (UBYTE) ~D_FCtp_FLAG_Busy_UB;

    } /* -2- END OF if(..) -------------------------------------------------*/
    /*======================================================================*/
    /* -2- Timer not elapsed                                                */
    /*======================================================================*/
    else
    {
      sTP.TPTimerControl_UB |= D_FCtp_FLAG_Busy_UB;
    } /* -2- END OF else (if(..)) ------------------------------------------*/ 
  } /* ---1- END OF if(..) -------------------------------------------------*/ 
  
  return;
   
} /* -+- END OF FUN_CCTp_Timer_UB -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/


/*---------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                 |
+----------------------------------------------------------------------------+
|                                                                            |
|  Function Name:  FUN_CCtp_RxTp_UB                                          |
|                                                                            |
|  Prototype at:   ADLATUS_Tp_TDF.h                                          |
|                                                                            |
|  Description:    The function  analyses an incoming message.               | 
|                                                                            |
|  Parameter:      CanChipDataPtr data     ... Data                          |
|                  UBYTE          TGlength ... Data length                   |
|                                                                            |
|  Return Value:   -                                                         |
|                                                                            |
+---------------------------------------------------------------------------*/
UBYTE FUN_CCtp_RxTp_UB (BUFFER_UB* InputBuffer_UB, BUFFER_UB* OutputBuffer_UB)
{
   UWORD l_scratch_UW;
   UWORD l_length_UW;

   UBYTE l_return_UB = D_FCtp_FLAG_NoReturnValue_UB;

   if((D_CCtp_RCV_IN_PROGRESS != (sTP.TPstate_UW & D_CCtp_RCV_IN_PROGRESS)) && \
      (D_CCtp_TP_UNEXPECTED_SF == (sTP.TPstate_UW & D_CCtp_TP_UNEXPECTED_SF)))     
   {                    
     sTP.TPstate_UW &= ~D_CCtp_TP_UNEXPECTED_SF;

     for (l_scratch_UW = 0u; l_scratch_UW < (UWORD)D_CCtp_SF_LENGTH + 0x04 ;l_scratch_UW++)
     {
        (*(&(*InputBuffer_UB) + l_scratch_UW)) = sTP.TPTempBuffer_UB[l_scratch_UW];

     } /* -6- END OPF for(..) ---------------------------------------*/
   }

   if((D_CCtp_TP_BLOCKSENDING != (sTP.TPstate_UW & D_CCtp_TP_BLOCKSENDING)) && \
      (D_CCtp_FORMER_MSG_RCV == (sTP.TPstate_UW & D_CCtp_FORMER_MSG_RCV)))     
   {                    
     sTP.TPstate_UW &= ~D_CCtp_FORMER_MSG_RCV;

     /* buffer is filled */
     (*(&(*OutputBuffer_UB))) |= D_CCtp_BufferValid_UB;
     
     /* analyse tg immediately*/
     l_return_UB |= D_FCtp_FLAG_Ready_UB;

     sTP.TPChannel_UB = ((*(&(*InputBuffer_UB) + D_CCtp_BUFFERCHANNEL)) & d_CCglo_SubSetMask);     
     /* Set Responce Time */
     sTP.TXTimer_UW = sTP.P2min_UW; 
     /* reset tx timeout flag                                      */
     sTP.TPTimerControl_UB &= (UBYTE) ~c_FCpara_FLAG_TPTXTimeout_UB;
     sTP.TPTimerControl_UB &= (UBYTE) ~D_FCtp_FLAG_Ready_UB;
     /* set busy because timer is not elapsed                      */
     sTP.TPTimerControl_UB |= D_FCtp_FLAG_Busy_UB;
   }
   else
   {
   /*==========================================================================*/
   /* -1- check if the inputbuffer contents valid data                         */
   /*==========================================================================*/
   if (((*(&(*InputBuffer_UB))) & D_CCtp_BufferValid_UB) == D_CCtp_BufferValid_UB)
   {
     /*======================================================================*/
     /* -3- analyse telegramm                                                */
     /*======================================================================*/
     switch( (*(&(*InputBuffer_UB) + D_CCtp_BUFFEROFFSET + D_CCtp_PCI_POS_SF)) & 0xF0)
     {
        /*--------------------------------------------------------------------*/
        /*                                                                    */
        /* 0x00 - Single Frame received                                       */
        /*                                                                    */
        /*--------------------------------------------------------------------*/
        case D_CCtp_SINGLEFRAME:
          if(D_CCtp_RCV_IN_PROGRESS != (sTP.TPstate_UW & D_CCtp_RCV_IN_PROGRESS))
          {   
            /*==================================================================*/
            /* -4- If the protocol sequence is correct accept the message       */
            /*     otherwise reset to receiving SF - State                      */
            /* this means a pending message in a block will not be transferred  */
            /* and a FF-FC sequence will be interrupted.                        */
            /*==================================================================*/

            /* ALWAYS free buffer because if a sequence error occurs it will cause a hangup */
            (*(&(*InputBuffer_UB))) &= ~D_CCtp_BufferValid_UB;

            /* reset timeout timer because we have got a message */
            sTP.RXTimer_UW = D_CCtp_INIT_TO_ZERO;
    
            /* set P3 Timeout -- tg is for this ecu */
            FUN_CCevent_SetProjectTimer_V (sTP.P3max_UW);
  
            /*==============================================================*/
            /* -5- Outbuffer is invalid --> outbuffer is free               */
            /*==============================================================*/
            if (((*(&(*OutputBuffer_UB))) & D_CCtp_BufferValid_UB) != D_CCtp_BufferValid_UB)
            {
              /*==============================================================*/
              /* -6- Single frame with less than 7 bytes but not zero         */
              /*==============================================================*/
              if (((*(&(*InputBuffer_UB) + D_CCtp_BUFFEROFFSET + D_CCtp_PCI_POS_SF)) <= D_CCtp_SF_LENGTH) && \
                  ((*(&(*InputBuffer_UB) + D_CCtp_BUFFEROFFSET + D_CCtp_PCI_POS_SF)) >= 0x01)) 
              {
                 /* store length for app usage */
                 sTP.RXDataLength_UB = (*(&(*InputBuffer_UB) + D_CCtp_BUFFEROFFSET + D_CCtp_PCI_POS_SF)) & 0x0F;
                
                 if(0x08 == InputBuffer_UB[D_CCtp_BUFFERDATA]) 
                 {
                   /* Store Data length */
                   (*(&(*OutputBuffer_UB) + D_CCtp_BUFFERDATA))= sTP.RXDataLength_UB;

                   /* store channel number */
                   (*(&(*OutputBuffer_UB) + D_CCtp_BUFFERCHANNEL))= 
                   (*(&(*InputBuffer_UB) + D_CCtp_BUFFERCHANNEL));    
                   /*==============================================================*/
                   /* -6- Copy data with offset SF (Single Frame)                  */
                   /*==============================================================*/
                   for (l_scratch_UW = 0u; l_scratch_UW < (UWORD)D_CCtp_SF_LENGTH;l_scratch_UW++)
                   {
                     (*(&(*OutputBuffer_UB) + D_CCtp_BUFFEROFFSET + l_scratch_UW))= 
                     (*(&(*InputBuffer_UB) + D_CCtp_BUFFEROFFSET + l_scratch_UW + D_CCtp_SID_POS_SF));
                   } /* -6- END OPF for(..) ---------------------------------------*/

                   if(D_CCtp_TP_BLOCKSENDING != (sTP.TPstate_UW & D_CCtp_TP_BLOCKSENDING))
                   {  
                      /* buffer is filled */
                      (*(&(*OutputBuffer_UB))) |= D_CCtp_BufferValid_UB;
                      /* analyse tg immediately*/
                      l_return_UB |= D_FCtp_FLAG_Ready_UB;
                      /* if we receive already a block of data -- terminate the receive */
                      /* state and start all over with new data                         */
                      sTP.TPstate_UW = D_CCtp_INIT_TO_ZERO;
                    
                      /* Set Responce Time */
                      sTP.TXTimer_UW = sTP.P2min_UW; 
                      /* reset tx timeout flag                                      */
                      sTP.TPTimerControl_UB &= (UBYTE) ~c_FCpara_FLAG_TPTXTimeout_UB;
                      sTP.TPTimerControl_UB &= (UBYTE) ~D_FCtp_FLAG_Ready_UB;
                      /* set busy because timer is not elapsed                      */
                      sTP.TPTimerControl_UB |= D_FCtp_FLAG_Busy_UB;

                      sTP.TPChannel_UB = ((*(&(*InputBuffer_UB) + D_CCtp_BUFFERCHANNEL)) & d_CCglo_SubSetMask);
                   }
              	   else
		   {
                     sTP.TPstate_UW |= D_CCtp_FORMER_MSG_RCV;
                   }  
	         }
                }  
              }
            } /* ---5- END OF if(..) ---------------------------------------*/
            else
            {
              for (l_scratch_UW = 0u; l_scratch_UW < (UWORD)D_CCtp_SF_LENGTH + 0x04 ;l_scratch_UW++)
              {
                sTP.TPTempBuffer_UB[l_scratch_UW]= (*(&(*InputBuffer_UB) + l_scratch_UW));

                /* ALWAYS free buffer because if a sequence error occurs it will cause a hangup */
                (*(&(*InputBuffer_UB))) &= ~D_CCtp_BufferValid_UB;

                /* set P3 Timeout -- tg is for this ecu */
                FUN_CCevent_SetProjectTimer_V (sTP.P3max_UW);

                sTP.TPstate_UW |= D_CCtp_TP_UNEXPECTED_SF;

              } /* -6- END OPF for(..) ---------------------------------------*/
            }
        break;

        /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

        /*--------------------------------------------------------------------*/
        /*                                                                    */
        /* 0x10 - First Frame received                                        */
        /*                                                                    */
        /*--------------------------------------------------------------------*/
        case D_CCtp_FIRSTFRAME:
            /* reset timeout timer because we have got a message */
            sTP.RXTimer_UW = D_CCtp_INIT_TO_ZERO;
    
            /* set P3 Timeout -- tg is for this ecu */
            FUN_CCevent_SetProjectTimer_V (sTP.P3max_UW);
                 
            /* check if buffer is free */
            if (((*(&(*OutputBuffer_UB))) & D_CCtp_BufferValid_UB) != D_CCtp_BufferValid_UB)
            {
              /* ALWAYS free buffer because if a sequence error occurs it will cause a hangup */
              (*(&(*InputBuffer_UB))) &= ~D_CCtp_BufferValid_UB;

              /* get the length and store it in a temp variable for further examinations */
              l_length_UW =  ((UWORD) ((*(&(*InputBuffer_UB) + D_CCtp_BUFFEROFFSET + D_CCtp_PCI_POS_FF))) & 0x00FFu);
              l_length_UW |= ((UWORD)(((*(&(*InputBuffer_UB) + D_CCtp_BUFFEROFFSET + D_CCtp_CNT_POS_FF)) << 0x08)) & 0x0F00u);

              /* Don't accept frames longer than our buffer */
              if(l_length_UW > (UWORD) (*(&(*OutputBuffer_UB) + D_CCtp_BUFFERLENGTH)))
              {
                /* to much data causes a break (FC 0x32) */
                sTP.TPstate_UW |= D_CCtp_FF_TOMUCHDATA;
              }
              /* dont use else because we need the other configs */
              /* the break in the FC will cause the init of the TP */ /* OPL325 */
              if(( l_length_UW > (UWORD) D_CCtp_SF_LENGTH) && (InputBuffer_UB[D_CCtp_BUFFERDATA] == 0x08)) 
              {
                 /* to much data causes a break (FC 0x32) */
                if (D_CCtp_FF_TOMUCHDATA != (sTP.TPstate_UW & D_CCtp_FF_TOMUCHDATA))
                {
                  /* if we receive already a block of data -- terminate the receive */
                  /* state and start all over with new data                         */
                  if (D_CCtp_RX_SEG_MODEAKTIVE == (sTP.TPstate_UW & D_CCtp_RX_SEG_MODEAKTIVE))
                  {
                    FUN_CCtp_InitInternal_V();
                  }
                   
                  /* Store Data length */
                  /* we only accept a stream-length of less than 255 bytes */
                  sTP.RXDataLength_UB = (UBYTE) l_length_UW;
                  
                  sTP.RXDataIndex_UB = D_CCtp_INIT_TO_ZERO;

                  /* copy data with offset FF (First Frame) */
                  for (l_scratch_UW = 0u; l_scratch_UW < (UWORD)D_CCtp_FF_LENGTH;l_scratch_UW++)
                  {
                     (*(&(*OutputBuffer_UB) + D_CCtp_BUFFEROFFSET + l_scratch_UW)) =
                     (*(&(*InputBuffer_UB) + D_CCtp_BUFFEROFFSET + l_scratch_UW + D_CCtp_SID_POS_FF));

                     /* Set rx index to next free data element*/
                     sTP.RXDataIndex_UB++;
                   }
                   /* set data length to received info in tg */
                   (*(&(*OutputBuffer_UB) + D_CCtp_BUFFERDATA)) = sTP.RXDataLength_UB;

                   /* Set tg counter to initial value */
                   sTP.RXCounter_UB = 0x01u;

                   /* Assemble FC and reload BS counter */
                   sTP.RXBlockSize_UB = sTP.RXBlockSizeSave_UB;
                } /* endof tomuchdata */

                /* Set flag FC will be send, set flag first frame received */
                sTP.TPstate_UW |= (D_CCtp_FC_TO_SEND_STATE | \
                                    D_CCtp_FF_RECEIVED_STATE | \
                                    D_CCtp_RX_SEG_MODEAKTIVE | \
                                    D_CCtp_RCV_IN_PROGRESS );

                /* Set Responce Time */
                sTP.TPFlowControlTimer_UW = sTP.FCmin_UW; 

                /* tp has a fc to send we are still busy */
                l_return_UB =  D_FCtp_FLAG_Busy_UB;

                sTP.TPChannel_UB = ((*(&(*InputBuffer_UB) + D_CCtp_BUFFERCHANNEL)) & d_CCglo_SubSetMask);                

                /* store channel number */
                (*(&(*OutputBuffer_UB) + D_CCtp_BUFFERCHANNEL)) =
                (*(&(*InputBuffer_UB) + D_CCtp_BUFFERCHANNEL));
              } 
            }  
          break;
         /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

        /*--------------------------------------------------------------------*/
        /*                                                                    */
        /* 0x20 - Consecutive Frame received                                  */
        /*                                                                    */
        /*--------------------------------------------------------------------*/
        case D_CCtp_CONSECUTIVEFRAME:
            
         /* ALWAYS free buffer because if a sequence error occurs it will cause a hangup */
         (*(&(*InputBuffer_UB))) &= ~D_CCtp_BufferValid_UB;

         if(sTP.TPChannel_UB == (((*(&(*InputBuffer_UB) + D_CCtp_BUFFERCHANNEL)) & d_CCglo_SubSetMask)))
         {    
          if(D_CCtp_TP_BLOCKSENDING != (sTP.TPstate_UW &  D_CCtp_TP_BLOCKSENDING))
          {
             /* REMARK on receiving a cf we do not check the outputbuffer for      */
             /* validation because we have checked it on receiving the first frame */

             /* set P3 Timeout -- tg is for this ecu */
             FUN_CCevent_SetProjectTimer_V (sTP.P3max_UW);
             
             if(0x08 == InputBuffer_UB[D_CCtp_BUFFERDATA]) 
             {   
               /* First Frame received and FC gone */            
               if(((sTP.TPstate_UW & D_CCtp_FF_RECEIVED_STATE) != 0u) && 
                  ((sTP.TPstate_UW & D_CCtp_FC_TO_SEND_STATE) == 0u))
               {  
     
                 /* look for a wrong sequence number */
                 if(((*(&(*InputBuffer_UB) + D_CCtp_BUFFEROFFSET + D_CCtp_CNT_POS_CF)) & 0x0F) != (sTP.RXCounter_UB & 0x0F) ) 
                 {  
                   /* Wrong SN received? */
                   FUN_CCtp_InitInternal_V();
 
                   /* reset buffer info */
                   (*(&(*OutputBuffer_UB))) &= ~D_CCtp_BufferValid_UB;
                 }
                 else
                 {
                   /* A correct SN is receiced, calculate next expected SN in case of 
                    * more CF are expected. (Calculation is only neccessary if more 
                    * messages are expected!)*/

                   /* Normaly we are able to copy all data to RX buffer, but if the
                    * last CAN message can't be copied as one block, the following 
                    * check will be done to prevent buffer overrun. Then bytes are 
                    * copied until the RX buffer is full */

                   if (((*(&(*OutputBuffer_UB) + D_CCtp_BUFFERLENGTH)) - sTP.RXDataIndex_UB) < 7) 
                   {  
                     /* Rx buffer with free elements?*/
                     l_scratch_UW = (UWORD)D_CCtp_DATA_POS_CF;                                     

                     while( sTP.RXDataIndex_UB < (*(&(*OutputBuffer_UB) + D_CCtp_BUFFERLENGTH))  ) 
                     { 
                        /* Copy data with start at (TPCI+1) byte*/
                        (*(&(*OutputBuffer_UB) + D_CCtp_BUFFEROFFSET + sTP.RXDataIndex_UB)) =
                          (*(&(*InputBuffer_UB) + D_CCtp_BUFFEROFFSET + l_scratch_UW));

                        sTP.RXDataIndex_UB++;
                        l_scratch_UW++;
                     }
        
                     sTP.RXDataIndex_UB = (*(&(*OutputBuffer_UB) + D_CCtp_BUFFERLENGTH)) /* 21 */ ;      
                     /* No more free data element! */
                   }
                   else 
                   {
                     /* copy data with offset CF (Consecutive Frame) */
                     for (l_scratch_UW = 0u; l_scratch_UW < (UWORD)D_CCtp_CF_LENGTH;l_scratch_UW++)
                     {
                       (*(&(*OutputBuffer_UB) + D_CCtp_BUFFEROFFSET + sTP.RXDataIndex_UB)) =
                       (*(&(*InputBuffer_UB) + D_CCtp_BUFFEROFFSET + l_scratch_UW + D_CCtp_DATA_POS_CF));
                       /* increment index pointer to data */
                       sTP.RXDataIndex_UB++;
                     }
                   }

                   if(sTP.RXDataIndex_UB >= sTP.RXDataLength_UB) 
                   { 
                     /* Last message? (Also if buffer is full!)*/
                     sTP.TPstate_UW &= (D_CCtp_RESET_FF_RECEIVED_STATE & \
                                        D_CCtp_RESET_FC_HASTO_COME_STATE & \
                                        D_CCtp_RESET_FC_RECEIVED_STATE & \
                                        D_CCtp_RESET_CF_HASTO_COME_STATE & \
                                        D_CCtp_RESET_RCV_IN_PROGRESS);

          
                     /* buffer is filled */
                     (*(&(*OutputBuffer_UB))) |= D_CCtp_BufferValid_UB;

                     /* store channel number */
                     (*(&(*OutputBuffer_UB) + D_CCtp_BUFFERCHANNEL)) =
                     (*(&(*InputBuffer_UB) + D_CCtp_BUFFERCHANNEL));  

                     sTP.TPChannel_UB = ((*(&(*InputBuffer_UB) + D_CCtp_BUFFERCHANNEL)) & d_CCglo_SubSetMask);
                      
                     /* analyse tg */
                     l_return_UB = D_FCtp_FLAG_Ready_UB;
  
                     /* set value of send timer */
                     /* Set Responce Time to 10ms (default) */
                     sTP.TXTimer_UW = sTP.P2min_UW;
                     /* reset tx timeout flag                                    */
                     sTP.TPTimerControl_UB &= (UBYTE) ~c_FCpara_FLAG_TPTXTimeout_UB;
                     sTP.TPTimerControl_UB &= (UBYTE) ~D_FCtp_FLAG_Ready_UB;

                     /* set busy because timer is not elapsed                     */
                     sTP.TPTimerControl_UB |= D_FCtp_FLAG_Busy_UB;

                   } 
                   else 
                   {
                     sTP.TPChannel_UB = ((*(&(*InputBuffer_UB) + D_CCtp_BUFFERCHANNEL)) & d_CCglo_SubSetMask);
                     
                     if(sTP.RXBlockSize_UB != (UBYTE)D_CCtp_INIT_TO_ZERO) 
                     {   
                       /* Check if a FC is requested (BS==0?) */
                       sTP.RXBlockSize_UB --;          
                       /* Yes, FC is requested! */
                       if(sTP.RXBlockSize_UB == (UBYTE) D_CCtp_INIT_TO_ZERO)
                       {
                         sTP.TPstate_UW |= D_CCtp_FC_TO_SEND_STATE;
                         /* Check if a FC must be send (counter is zero) */                  
                         /* Set Responce Time  */
                         sTP.TPFlowControlTimer_UW = sTP.FCmin_UW;
 
                         /* Assemble FC and reload BS counter */
                         sTP.RXBlockSize_UB = sTP.RXBlockSizeSave_UB;
                       }
                       else
                       {
                         /* Set Timeout CFtg -> CFtg */
                         sTP.RXTimer_UW = sTP.STmax_UW;
                       }
                     }
                     /* Calculate next expected SN now 
                     (not neccessary if last CF was received) */
                     sTP.RXCounter_UB = ((sTP.RXCounter_UB + 1) & 0x0F);  
                   }  
                 }
               }
             }
           }
         }
         break;
         /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


        /*--------------------------------------------------------------------*/
        /*                                                                    */
        /* 0x30 - FlowControl Frame received                                  */
        /*                                                                    */
        /*--------------------------------------------------------------------*/
        case D_CCtp_FLOWCONTROLFRAME:
          /* ALWAYS free buffer because if a sequence error occurs it will cause a hangup */
          (*(&(*InputBuffer_UB))) &= ~D_CCtp_BufferValid_UB;
          
          if (((*(&(*InputBuffer_UB) + D_CCtp_BUFFEROFFSET + D_CCtp_PCI_POS_FC)) & 0x0F) >= 0x02) /* OPL327 */
          {
            /* reset layer because of a break FC */
            FUN_CCtp_InitInternal_V();
            sTP.TPstate_UW |= D_CCtp_ERROR;
          }
          else
          {
            if(InputBuffer_UB[D_CCtp_BUFFERDATA] == 0x08) /* OPL326 */
            {
              if(D_CCtp_TP_BLOCKSENDING == (sTP.TPstate_UW &  D_CCtp_TP_BLOCKSENDING))
              {
                /* reset timeout timer because we have got a message */
                sTP.RXTimer_UW = D_CCtp_INIT_TO_ZERO;
     
                /* set P3 Timeout -- tg is for this ecu */
                FUN_CCevent_SetProjectTimer_V (sTP.P3max_UW);

                sTP.TPChannel_UB = ((*(&(*InputBuffer_UB) + D_CCtp_BUFFERCHANNEL)) & d_CCglo_SubSetMask);
                      
                /* Attention: No FC after last data message allowed */
                if((sTP.TPstate_UW & (D_CCtp_FF_SENT_STATE | D_CCtp_FC_HASTO_COME_STATE)) == 
                                 (D_CCtp_FF_SENT_STATE | D_CCtp_FC_HASTO_COME_STATE)) 
                {
                  /* get blocksize for sending */
                  if ((*(&(*InputBuffer_UB) + D_CCtp_BUFFEROFFSET + D_CCtp_BLOCK_POS_FC)) != (UBYTE) 0x00u)
                  {
                    sTP.TXBlockSizeSave_UB = (*(&(*InputBuffer_UB) + D_CCtp_BUFFEROFFSET + D_CCtp_BLOCK_POS_FC)); 
                    sTP.TXBlockSize_UB = sTP.TXBlockSizeSave_UB;
                  }
                  else
                  { 
                    sTP.TXBlockSize_UB = (d_CCconsys_TxWorkBufferLength00 / D_CCtp_CF_LENGTH);
                    sTP.TXBlockSizeSave_UB = (d_CCconsys_TxWorkBufferLength00 / D_CCtp_CF_LENGTH);
                  }
                  /* Get Timer value */
                  if ((((*(&(*InputBuffer_UB) + D_CCtp_BUFFEROFFSET + D_CCtp_TIME_POS_FC)) >= (UBYTE) 0xF1u) &&
                      ((*(&(*InputBuffer_UB) + D_CCtp_BUFFEROFFSET + D_CCtp_TIME_POS_FC)) <= (UBYTE) 0xF9u) ) || 
                      ((*(&(*InputBuffer_UB) + D_CCtp_BUFFEROFFSET + D_CCtp_TIME_POS_FC)) == (UBYTE) 0x00u))
                  {
                    sTP.TXTimerSave_UW = 0x0001u + 0x0001u;
                    sTP.TXTimer_UW = sTP.TXTimerSave_UW;
                  }
                  else
                  {
                    if ((*(&(*InputBuffer_UB) + D_CCtp_BUFFEROFFSET + D_CCtp_TIME_POS_FC)) <= (UBYTE) 0x7Fu)
                    {
                      /* if we got a valid time - take it */
                      sTP.TXTimerSave_UW = (*(&(*InputBuffer_UB) + D_CCtp_BUFFEROFFSET + D_CCtp_TIME_POS_FC)) + 0x0001;
                      sTP.TXTimer_UW = sTP.TXTimerSave_UW;
                    }
                    else
                    {
                      /* or use OUR!!! default (127ms) */
                      sTP.TXTimer_UW = 0x0081u;
                      sTP.TXTimerSave_UW = 0x0081u;
                    }
                  }
                  /* look for PCI Byte */
                  if (((*(&(*InputBuffer_UB) + D_CCtp_BUFFEROFFSET + D_CCtp_PCI_POS_FC)) & (UBYTE) 0x0Fu) == (UBYTE) 0x00u)
                  {
                    sTP.TPstate_UW &= D_CCtp_RESET_FC_HASTO_COME_STATE;
                    /* set flag FC received */
                    sTP.TPstate_UW |= D_CCtp_FC_RECEIVED_STATE;
                  }
                  else 
                  {
                    /* if wait flag not set */
                    if (((*(&(*InputBuffer_UB) + D_CCtp_BUFFEROFFSET + D_CCtp_PCI_POS_FC)) & 0x0F) == 0x01)   
                    {
                      /* Set flag FC will be send */
                      sTP.RXTimer_UW = sTP.STmax_UW; 
                    }
                  }
                }
              }
            }
            else
            {
              sTP.TPstate_UW |= D_CCtp_ERROR;
            }          
          }  /* else ignore */

          break;
         /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

        /*--------------------------------------------------------------------*/
        /*                                                                    */
        /* default                                                            */
        /*                                                                    */
        /*--------------------------------------------------------------------*/
        default:
          /* ALWAYS free buffer because if a sequence error occurs it will cause a hangup */
         (*(&(*InputBuffer_UB))) &= ~D_CCtp_BufferValid_UB;

         break;
         /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

       } /* -3- END OF switch (..) -------------------------------------------*/
    } /* ---2- END OF else (if(..)) ------------------------------------------*/
   }
   return (l_return_UB);

} /* -+- END OF FUN_CCtp_RxTp_UB -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/


/*---------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                 |
+----------------------------------------------------------------------------+
|                                                                            |
|  Function Name:  FUN_CCtp_TxTp_UB                                          |
|                                                                            |
|  Prototype at:   ADLATUS_Tp_TDF.h                                          |
|                                                                            |
|  Description:    Cyclic called function (e.g. every 1ms)                   |
|                  In this function we periodically check for things to do.  |
|                                                                            |
|  Parameter:      -                                                         |
|                                                                            |
|  Return Value:   -                                                         |
|                                                                            |
+---------------------------------------------------------------------------*/
UBYTE FUN_CCtp_TxTp_UB(BUFFER_UB* InputBuffer_UB, BUFFER_UB* OutputBuffer_UB)  
{

  UBYTE l_counter_ub;
  UBYTE l_return_UB = D_FCtp_FLAG_NoReturnValue_UB;

    /*========================================================================*/      
    /* -1- timeout on rx path                                                 */
    /*     if we have got an error we had a timeout (P3 max)                  */
    /*========================================================================*/      
    if ((sTP.TPTimerControl_UB & c_FCpara_FLAG_TPRXTimeout_UB) == c_FCpara_FLAG_TPRXTimeout_UB)
    {
      /* Set return value                                                     */
      FUN_CCtp_InitInternal_V();

      /* ALWAYS free buffer because if a sequence error occurs it will cause a hangup */
      (*(&(*InputBuffer_UB))) &= ~D_CCtp_BufferValid_UB;

    } /* -1- END OF if(..) ---------------------------------------------------*/

    /*========================================================================*/      
    /* -1- No error occured                                                   */
    /*========================================================================*/      
    else
    {
      /*======================================================================*/      
      /* -2- fresh buffer from session and an empty  buffer to fill ?         */
      /*======================================================================*/      
      if( (( (*(&(*InputBuffer_UB ))) & D_CCtp_BufferValid_UB) == D_CCtp_BufferValid_UB) &&
          (( (*(&(*OutputBuffer_UB))) & D_CCtp_BufferValid_UB) != D_CCtp_BufferValid_UB) )
      {
        /*====================================================================*/      
        /* -3- error on receiving data occured ?                              */
        /*====================================================================*/      
        if ((sTP.TPstate_UW & D_CCtp_ERROR) == D_CCtp_ERROR) /* OPL326 */
        {
          sTP.TPstate_UW &= ~D_CCtp_ERROR;
          (*(&(*OutputBuffer_UB))) &= (UBYTE) ~D_CCtp_BufferValid_UB;

          /* reset Ready flag -- inputbuffer is free                          */
          (*(&(*InputBuffer_UB))) &= (UBYTE) ~D_CCtp_BufferValid_UB;           
        }
        else
        {
          /*==================================================================*/
          /* -4- Timer is elapsed                                             */
          /*==================================================================*/
          if ((sTP.TPTimerControl_UB & D_FCtp_FLAG_Ready_UB ) == D_FCtp_FLAG_Ready_UB)
          {
            /*================================================================*/
            /* -5- We have data from session layer                            */
            /*================================================================*/
            if((sTP.TPTimerControl_UB & c_FCpara_FLAG_TPTXTimeout_UB ) == c_FCpara_FLAG_TPTXTimeout_UB)
            {
              sTP.TPTimerControl_UB &= (UBYTE) ~D_FCtp_FLAG_Ready_UB;
              /* reset tx timeout flag                                        */
              sTP.TPTimerControl_UB &= (UBYTE) ~c_FCpara_FLAG_TPTXTimeout_UB;

              /* Set Responce Time to P2min                                   */
              sTP.TXTimer_UW = sTP.P2min_UW; /*P_TplRxFCSTmin_f;*/

              /*==============================================================*/      
              /* -6- In a block transfer do not re-set all to initial state   */
              /*==============================================================*/      
              if((sTP.TPstate_UW & D_CCtp_TP_BLOCKSENDING ) != D_CCtp_TP_BLOCKSENDING)
              {
                /*  new diag message to send (complete frame or first frame)  */
                /* load length info to tp data */
                sTP.TXDataLength_UB = (*(&(*InputBuffer_UB) + D_CCtp_BUFFERDATA));
 
                /*============================================================*/      
                /* -7- (0)1 to 6/7 bytes will be a SingleFrame                */
                /*============================================================*/      
                if(sTP.TXDataLength_UB <= D_CCtp_SF_LENGTH) 
                {       
                  /* kL4_SingleFrame TPCI is 0x00                             */
                  (*(&(*OutputBuffer_UB) + D_CCtp_BUFFEROFFSET + D_CCtp_PCI_POS_SF)) = sTP.TXDataLength_UB;

                  /*==========================================================*/
                  /* -8- Copy Data to Canbuffer (incl 0x00)                   */
                  /*==========================================================*/      
                  for(l_counter_ub = 0; l_counter_ub < D_CCtp_SF_LENGTH;l_counter_ub++)
                  {
                    /*========================================================*/      
                    /* -9- Fill the message with zero                         */
                    /*========================================================*/      
                    if((l_counter_ub + 1) > sTP.TXDataLength_UB)
                    {
                      /* make it nice                                         */
                      (*(&(*OutputBuffer_UB) + D_CCtp_BUFFEROFFSET + l_counter_ub + D_CCtp_SID_POS_SF)) = D_CCtp_ServerFillByte;

                    } /* -8- END OF if (..) ----------------------------------*/
                    /*========================================================*/      
                    /* -9- Fill in the data                                   */
                    /*========================================================*/      
                    else
                    {
                      (*(&(*OutputBuffer_UB) + D_CCtp_BUFFEROFFSET + l_counter_ub + D_CCtp_SID_POS_SF)) = (*(&(*InputBuffer_UB) + D_CCtp_BUFFEROFFSET + l_counter_ub));
                    } /* -9- END OF else (if(..)) ----------------------------*/
                  } /* ---8- END OG for(..) ----------------------------------*/

                  /*----------------------------------------------------------*/
                  /* Set Timeout P3                                           */
                  /*----------------------------------------------------------*/
                  sTP.RXTimer_UW = sTP.P3max_UW;


#ifdef CD_CCtp_VARIABLE_TG_LENGTH             
                  (*(&(*OutputBuffer_UB) + D_CCtp_BUFFERDATA)) = (*(&(*InputBuffer_UB) + D_CCtp_BUFFERDATA)) + D_CCtp_SID_POS_SF;  
#else
                  (*(&(*OutputBuffer_UB) + D_CCtp_BUFFERDATA)) = D_CCtp_TG_LENGTH;
#endif          

                  /* reset Ready flag -- inputbuffer is free                  */
                  (*(&(*InputBuffer_UB))) &= (UBYTE) ~D_CCtp_BufferValid_UB;

                  /* set Ready flag -- outputbuffer filled                    */
                  (*(&(*OutputBuffer_UB))) |= D_CCtp_BufferValid_UB;

                  /* store channel number                                     */
                  (*(&(*OutputBuffer_UB) + D_CCtp_BUFFERCHANNEL)) =   
                  (*(&(*InputBuffer_UB) + D_CCtp_BUFFERCHANNEL));     
                
                  /* get rid of the message                                   */
                  l_return_UB = D_FCtp_FLAG_Ready_UB;

                  sTP.TPstate_UW &= D_CCtp_RESET_RX_SEG_MODEAKTIVE;

                } /* -7- END OF if (..) --------------------------------------*/

                /*============================================================*/
                /* -7- we have multiple frames -- send first frame            */
                /*============================================================*/
                else 
                {
                  /* push datalength                                          */
                  l_counter_ub = sTP.TXDataLength_UB - D_CCtp_FF_LENGTH;

                  /* Init segment counter                                     */
                  sTP.TXSeg_UB = 0x01;

                  /*==========================================================*/      
                  /* -8- count segments (telegrams)                           */
                  /*==========================================================*/      
                  while (l_counter_ub >= D_CCtp_CF_LENGTH)
                  {
                    l_counter_ub -= D_CCtp_CF_LENGTH;
                    sTP.TXSeg_UB++;
                  } /* -8- END OF while (..) ---------------------------------*/
             
                  /* DL in TPCI byte                                          */
                  (*(&(*OutputBuffer_UB) + D_CCtp_BUFFEROFFSET + D_CCtp_CNT_POS_FF)) = D_CCtp_FIRSTFRAME;

                  /* means length <= 255                                      */  
                  (*(&(*OutputBuffer_UB) + D_CCtp_BUFFEROFFSET + D_CCtp_PCI_POS_FF)) = sTP.TXDataLength_UB;

                  /*==========================================================*/      
                  /* -8- prepare tg for sending (and fill it up)              */
                  /*==========================================================*/      
                  for(l_counter_ub = 0; l_counter_ub < D_CCtp_FF_LENGTH;l_counter_ub++)
                  {
                    (*(&(*OutputBuffer_UB) + D_CCtp_BUFFEROFFSET + l_counter_ub + D_CCtp_SID_POS_FF)) = (*(&(*InputBuffer_UB) + D_CCtp_BUFFEROFFSET + l_counter_ub));
                  } /* -8- END OF for (..) -----------------------------------*/

                  /* First bytes are transmitted (in queue) - set new index   */         
                  sTP.TXDataIndex_UB = D_CCtp_FF_LENGTH; 

                  /* substract sent data                                      */
                  sTP.TXDataLength_UB -= D_CCtp_FF_LENGTH;

                  /* reset Blocksize because in this state its unknown        */
                  sTP.TXBlockSize_UB = D_CCtp_INIT_TO_ZERO;   
 
                  /* set counter for first CF                                 */
                  sTP.TXCounter_UB = 0x01u;

                  /* set Timeout FF -> FC                                     */
                  sTP.RXTimer_UW = sTP.STmax_UW; /* P_TTplTOBs_f; */

                  /* set flag: FF sent and wait for FC                        */
                  sTP.TPstate_UW |= (D_CCtp_FC_HASTO_COME_STATE | D_CCtp_FF_SENT_STATE | D_CCtp_TP_BLOCKSENDING);

                  sTP.TPstate_UW &= D_CCtp_RESET_RX_SEG_MODEAKTIVE;

                  /* set tg-length -- used for sending (a first frame always has 8 bytes) */
                  (*(&(*OutputBuffer_UB) + D_CCtp_BUFFERDATA)) = D_CCtp_TG_LENGTH;

                  /* send first (and maybe only) telegram                     */
                  l_return_UB = D_FCtp_FLAG_Ready_UB;

                  /* lock diagtask                                            */
                  l_return_UB |= D_FCtp_FLAG_Busy_UB;

                  /* set Ready flag -- outputbuffer filled                    */
                  (*(&(*OutputBuffer_UB))) |= D_CCtp_BufferValid_UB;

                  /* store channel number                                     */
                  (*(&(*OutputBuffer_UB) + D_CCtp_BUFFERCHANNEL)) =   
                  (*(&(*InputBuffer_UB) + D_CCtp_BUFFERCHANNEL));     
              
                } /* -7- END OF else (if(..)) --------------------------------*/
              } /* ---6- END OF if(..) ---------------------------------------*/
              /*==============================================================*/      
              /* -6- if not call the cyclical task                            */
              /*==============================================================*/      
              else
              {
                /*============================================================*/      
                /* -7- FirstFrame sent and FlowControl already received       */
                /*============================================================*/      
                if((sTP.TPstate_UW & (D_CCtp_FC_RECEIVED_STATE | D_CCtp_FF_SENT_STATE)) == (D_CCtp_FC_RECEIVED_STATE | D_CCtp_FF_SENT_STATE)) 
                {
                  /*==========================================================*/      
                  /* -8- check if buffer is free                              */
                  /*==========================================================*/      
                  if (((*(&(*OutputBuffer_UB))) & D_CCtp_BufferValid_UB) != D_CCtp_BufferValid_UB)
                  {
                    l_return_UB = D_FCtp_FLAG_Busy_UB;
                    /*========================================================*/      
                    /* -9- if there is still something to send                */
                    /*========================================================*/      
                    if ((sTP.TXSeg_UB != D_CCtp_INIT_TO_ZERO) && (sTP.TXDataLength_UB != (UBYTE)0x00u))
                    {
                      sTP.TXSeg_UB--;

                      /* Assemble CF now -- first get counter                 */
                      (*(&(*OutputBuffer_UB) + D_CCtp_BUFFEROFFSET + D_CCtp_CNT_POS_CF)) = (D_CCtp_CONSECUTIVEFRAME | sTP.TXCounter_UB);

                      /* next increase counter for next message               */
                      sTP.TXCounter_UB = (UBYTE) ((sTP.TXCounter_UB + (UBYTE) 0x01u) & (UBYTE) 0x0Fu);  

                      /*======================================================*/      
                      /* -10- prepare tg for sending (and fill it up)         */
                      /*======================================================*/      
                      for(l_counter_ub = 0; l_counter_ub < D_CCtp_CF_LENGTH;l_counter_ub++)
                      {
                        /*====================================================*/      
                        /* -11-                                               */
                        /*====================================================*/      
                        if((l_counter_ub + 1) > sTP.TXDataLength_UB)
                        {
                          /* make it nice                                     */
                          (*(&(*OutputBuffer_UB) + D_CCtp_BUFFEROFFSET + l_counter_ub + D_CCtp_DATA_POS_CF)) = D_CCtp_ServerFillByte;
                        } /* -11- END OF if(..) ------------------------------*/

                        /*====================================================*/      
                        /* -11-                                               */                                                  
                        /*====================================================*/      
                        else
                        {
                          (*(&(*OutputBuffer_UB) + D_CCtp_BUFFEROFFSET + l_counter_ub + D_CCtp_DATA_POS_CF)) =
                          (*(&(*InputBuffer_UB) + D_CCtp_BUFFEROFFSET + sTP.TXDataIndex_UB + l_counter_ub));

                        } /* -11- END OF else (if(..)) -----------------------*/              
                      } /* ---10- END OF for (..) ----------------------------*/

                      /* set Ready flag -- outputbuffer filled                */
                      (*(&(*OutputBuffer_UB))) |= D_CCtp_BufferValid_UB;
                  
                      /* annouce the telegram                                 */
                      l_return_UB |= D_FCtp_FLAG_Ready_UB;
                
#ifdef CD_CCtp_VARIABLE_TG_LENGTH             

                      /*======================================================*/      
                      /* -10- get rid of the message                          */
                      /*======================================================*/      
                      if(sTP.TXDataLength_UB >= D_CCtp_CF_LENGTH)
                      {
                        /* if consecutive frames send eight bytes             */
                        (*(&(*OutputBuffer_UB) + D_CCtp_BUFFERDATA)) = D_CCtp_TG_LENGTH;

                      } /* -10- END OF if (..) -------------------------------*/
                      /*======================================================*/      
                      /* -10-                                                 */                                                  
                      /*======================================================*/      
                      else
                      {    
                        /* if last frame send only remaining bytes            */
                        (*(&(*OutputBuffer_UB) + D_CCtp_BUFFERDATA)) = (sTP.TXDataLength_UB + D_CCtp_DATA_POS_CF);
                      } /* -9- END OF else (if (..)) -------------------------*/
#else
                      /* get rid of the message                               */
                      (*(&(*OutputBuffer_UB) + D_CCtp_BUFFERDATA)) = D_CCtp_TG_LENGTH;
#endif

                      /*======================================================*/      
                      /* -10- if  there is still something to transmit        */
                      /*======================================================*/      
                      if((sTP.TXSeg_UB != D_CCtp_INIT_TO_ZERO) && (sTP.TXDataLength_UB != 0x00))
                      {
                        l_return_UB |= D_FCtp_FLAG_Busy_UB;

                        /* reload timer                                       */
                        sTP.TXTimer_UW = sTP.TXTimerSave_UW;
                        /* set busy because timer is not elapsed              */
                        sTP.TPTimerControl_UB |= D_FCtp_FLAG_Busy_UB;

                        /* set data pointer to next elements                  */
                        sTP.TXDataIndex_UB += D_CCtp_CF_LENGTH;
                        sTP.TXDataLength_UB -= D_CCtp_CF_LENGTH;
            
                        /*====================================================*/      
                        /* -11- look for Blockcounter of tester               */
                        /*====================================================*/      
                        if(sTP.TXBlockSize_UB != D_CCtp_INIT_TO_ZERO)
                        {
                          /*  Say next tg was sent                            */
                          sTP.TXBlockSize_UB--;

                          /*==================================================*/
                          /* -12-                                             */
                          /*==================================================*/      
                          if(sTP.TXBlockSize_UB == D_CCtp_INIT_TO_ZERO) /* && TXSeg_UB) */
                          {
                            /* from now on - wait for FC of counterpart       */
                            sTP.TPstate_UW &= D_CCtp_RESET_FC_RECEIVED_STATE;
                            sTP.TPstate_UW |= D_CCtp_FC_HASTO_COME_STATE;
                            /* set Timeout CF -> FC                           */

                            sTP.RXTimer_UW = sTP.STmax_UW;

                            l_return_UB |= D_FCtp_FLAG_Busy_UB;

                          } /* -12- END OF if (..) ---------------------------*/
                        } /* ---11- END OF if (..) ---------------------------*/
                      } /* -----10- END OF if (..) ---------------------------*/

                      /*======================================================*/      
                      /* -10- nothing more to send                            */
                      /*======================================================*/      
                      else
                      {
                        sTP.TPstate_UW &= (D_CCtp_RESET_FC_RECEIVED_STATE & 
                                           D_CCtp_RESET_FF_SENT_STATE & 
                                           D_CCtp_RESET_TP_BLOCKSENDING);

                        /* reset Ready flag -- inputbuffer is free */
                        (*(&(*InputBuffer_UB))) &= ~D_CCtp_BufferValid_UB;

                        /* reset  "diag-lock" flag */
                        l_return_UB &= (UBYTE) ~D_FCtp_FLAG_Busy_UB;
            
                        /* set Timeout FF -> FC */
                        sTP.RXTimer_UW = sTP.P3max_UW; /* P_TTplTOBs_f; */

                      } /* -10- END OF else (if(..)) -------------------------*/
                    } /* ---9- END OF if (..) --------------------------------*/

                    /*========================================================*/      
                    /* -9-                                                    */
                    /*========================================================*/      
                    else  
                    {
                      /* all done!!!                                          */
                      /* last CF is gone, we're ready! No FC after last CF    */
                      sTP.TPstate_UW &= (D_CCtp_RESET_FC_RECEIVED_STATE & 
                                         D_CCtp_RESET_FF_SENT_STATE &
                                         D_CCtp_RESET_TP_BLOCKSENDING);
  
                      /* reset Ready flag -- inputbuffer is free              */
                      (*(&(*InputBuffer_UB))) &= ~D_CCtp_BufferValid_UB;
                   
                      /* reset "diag-lock" flag                               */
                      l_return_UB &= (UBYTE) ~D_FCtp_FLAG_Busy_UB;

                      /* set Timeout to p3 max                                */
                      sTP.RXTimer_UW = sTP.P3max_UW; /* P_TTplTOBs_f;         */

                    } /* -9- END OF else (if(..)) ----------------------------*/
                  } /* ---8- END OF                                           */
                } /*------7- End ... if (sTP.TXSeg_UB)                        */
              } /* -------6- End ... if((.....TPstate_UW & 0x0030) == 0x0030) */
            } /* ---------5- END OF if (..) ------------------------------------------*/

            /*================================================================*/      
            /* -5-  No data from session layer                                */
            /*================================================================*/      
            else
            {
              /*==============================================================*/      
              /* -6- Flow Control timer elapsed                               */
              /*==============================================================*/      
              if((sTP.TPTimerControl_UB & c_FCpara_FLAG_TPFlowControl_UB) == c_FCpara_FLAG_TPFlowControl_UB) 
              {
                sTP.TPTimerControl_UB &= (UBYTE) ~D_FCtp_FLAG_Ready_UB;
 
                sTP.TPTimerControl_UB &= (UBYTE) ~c_FCpara_FLAG_TPFlowControl_UB;

                /*============================================================*/
                /* -7- Flow Control must be send                              */
                /*============================================================*/
                if((sTP.TPstate_UW & D_CCtp_FC_TO_SEND_STATE) == D_CCtp_FC_TO_SEND_STATE)
                {
                  /*==========================================================*/
                  /* -8- Buffer is free                                       */
                  /*==========================================================*/
                  if (((*(&(*OutputBuffer_UB))) & D_CCtp_BufferValid_UB) != D_CCtp_BufferValid_UB)
                  {
                    sTP.TPstate_UW &= D_CCtp_RESET_FC_TO_SEND_STATE;
                    /* next tg must be a consecutive frame                    */
                    sTP.TPstate_UW |= (D_CCtp_CF_HASTO_COME_STATE);
#ifdef STMIN_ZERO

                    if((((*(&(*OutputBuffer_UB))) & d_CCglo_ChannelNbr)& d_CCglo_ChannelMask) != d_CCglo_Channel00)
                    {
#endif
                      /* Just for a fine look of the flow control message...  */
                      FUN_CCglo_AMemCopy_V( 
                        (ULONG)&(*OutputBuffer_UB) + (ULONG) D_CCtp_BUFFEROFFSET,
                        (ULONG)&C_CCTP_FlowControlTG_UB[0u],
                        (ULONG)D_CCtp_TG_LENGTH);
   
#ifdef STMIN_ZERO
                    }
                    else
                    {
                      /* Just for a fine look of the flow control message...  */
                      FUN_CCglo_AMemCopy_V( 
                        (ULONG)&(*OutputBuffer_UB) + (ULONG) D_CCtp_BUFFEROFFSET,
                        (ULONG)&C_CCTP_FlowControlTGCas_UB[0u],
                        (ULONG)D_CCtp_TG_LENGTH);
                    }
#endif 

                    /* patch the STmin of config project...                   */
                    (*(&(*OutputBuffer_UB) + D_CCtp_BUFFEROFFSET + D_CCtp_TIME_POS_FC)) = (UBYTE) sTP.STmin_UW;

                    /*========================================================*/      
                    /* -9-                                                    */
                    /*========================================================*/      
                    if (sTP.RXDataLength_UB < (*(&(*InputBuffer_UB) + D_CCtp_BUFFERLENGTH)))
                    {
                      (*(&(*OutputBuffer_UB) + D_CCtp_BUFFEROFFSET + D_CCtp_SID_POS_SF)) = sTP.TPBsMax_UB;
                    } /* -9- END OF if(..) -----------------------------------*/

                    /*========================================================*/      
                    /* -9-                                                    */                                                      
                    /*========================================================*/      
                    else
                    { 
                      (*(&(*OutputBuffer_UB) + D_CCtp_BUFFEROFFSET + D_CCtp_SID_POS_SF)) = sTP.TPBsMax_UB;

                    } /* -9- END OF else (if(..)) ----------------------------*/

                    (*(&(*OutputBuffer_UB) + D_CCtp_BUFFERDATA)) = D_CCtp_FC_TG_LENGTH;

                    /* set Ready flag -- outputbuffer filled                  */
                    (*(&(*OutputBuffer_UB))) |= D_CCtp_BufferValid_UB;
            
                    l_return_UB = D_FCtp_FLAG_Ready_UB;
         
                    /* Set Timeout P2 */
                    sTP.RXTimer_UW = sTP.STmax_UW;

                  } /* -8- END OF if (..) ------------------------------------*/
                } /* ---7- */
              } /* -----6- */
            } /* -------5- */
          } /* ---------4- END OF if (..) ------------------------------------*/
          /*==================================================================*/
          /* -4- Timer not elapsed                                            */
          /*==================================================================*/
          else
          {
            /*================================================================*/
            /* -5- Timer is still running - TP is busy                        */
            /*================================================================*/
            if((sTP.TPTimerControl_UB & D_FCtp_FLAG_Busy_UB) == D_FCtp_FLAG_Busy_UB)
            {
              l_return_UB = D_FCtp_FLAG_Busy_UB;

            } /* -5- END OF if(..) -------------------------------------------*/

            /*================================================================*/
            /* -5- Timer is not running                                       */
            /*================================================================*/
            else
            { 
              l_return_UB = D_FCtp_FLAG_NoReturnValue_UB;

            } /* -5- END OF else (if (..)) -----------------------------------*/
          } /* ---4- END OF else (if (..)) -----------------------------------*/
        } /* -----3- END OF if (..) ------------------------------------------*/
      } /* -------2- */
      /*======================================================================*/      
      /* -2- Buffer state not correct                                         */
      /*======================================================================*/      
      else
      {
        /*--------------------------------------------------------------------*/
        /* Currently no data to send                                          */
        /*--------------------------------------------------------------------*/
        l_return_UB = D_FCtp_FLAG_Idle_UB;

        /*====================================================================*/      
        /* -3- Timer is elapsed                                               */
        /*====================================================================*/      
        if ((sTP.TPTimerControl_UB & D_FCtp_FLAG_Ready_UB ) == D_FCtp_FLAG_Ready_UB)
        {

          /*==================================================================*/      
          /* -4- Flow Control timer elapsed                                   */
          /*==================================================================*/      
          if((sTP.TPTimerControl_UB & c_FCpara_FLAG_TPFlowControl_UB) == c_FCpara_FLAG_TPFlowControl_UB) 
          {
            sTP.TPTimerControl_UB &= (UBYTE) ~D_FCtp_FLAG_Ready_UB;
            sTP.TPTimerControl_UB &= (UBYTE) ~c_FCpara_FLAG_TPFlowControl_UB;

            /*================================================================*/      
            /* -5- Flow Control must be send                                  */
            /*================================================================*/      
            if((sTP.TPstate_UW & D_CCtp_FC_TO_SEND_STATE) == D_CCtp_FC_TO_SEND_STATE)
            {
              /*==============================================================*/
              /* -6- Buffer is free                                           */
              /*==============================================================*/      
              if (((*(&(*OutputBuffer_UB))) & D_CCtp_BufferValid_UB) != D_CCtp_BufferValid_UB)
              {
                sTP.TPstate_UW &= D_CCtp_RESET_FC_TO_SEND_STATE;
                /* next tg must be a consecutive frame                        */
                sTP.TPstate_UW |= (D_CCtp_CF_HASTO_COME_STATE);
#ifdef STMIN_ZERO
                if((((*(&(*OutputBuffer_UB))) & d_CCglo_ChannelNbr)& d_CCglo_ChannelMask) != d_CCglo_Channel00)
                {
#endif
                  /* Just for a fine look of the flow control message...      */
                  FUN_CCglo_AMemCopy_V( 
                    (ULONG)&(*OutputBuffer_UB) + (ULONG) D_CCtp_BUFFEROFFSET,
                    (ULONG)&C_CCTP_FlowControlTG_UB[0u],
                    (ULONG)D_CCtp_TG_LENGTH);
#ifdef STMIN_ZERO
                }
                else
                {
                  /* Just for a fine look of the flow control message...      */
                  FUN_CCglo_AMemCopy_V( 
                    (ULONG)&(*OutputBuffer_UB) + (ULONG) D_CCtp_BUFFEROFFSET,
                    (ULONG)&C_CCTP_FlowControlTGCas_UB[0u],
                    (ULONG)D_CCtp_TG_LENGTH);
                }
#endif

                /* patch the STmin of config project...  */
                (*(&(*OutputBuffer_UB) + D_CCtp_BUFFEROFFSET + D_CCtp_TIME_POS_FC)) = (UBYTE) sTP.STmin_UW;

                /*============================================================*/      
                /* -7-                                                        */
                /*============================================================*/      
                if (sTP.RXDataLength_UB < (*(&(*InputBuffer_UB) + D_CCtp_BUFFERLENGTH)))
                {
                   (*(&(*OutputBuffer_UB) + D_CCtp_BUFFEROFFSET + D_CCtp_SID_POS_SF)) = sTP.TPBsMax_UB;
                } /* -7- END OF if(..) ---------------------------------------*/

                /*============================================================*/      
                /* -7-                                                        */                                                      
                /*============================================================*/      
                else                                                 
                { 
                  (*(&(*OutputBuffer_UB) + D_CCtp_BUFFEROFFSET + D_CCtp_SID_POS_SF)) = sTP.TPBsMax_UB;
                } /* -7- END OF else (if(..)) --------------------------------*/

                (*(&(*OutputBuffer_UB) + D_CCtp_BUFFERDATA)) = D_CCtp_FC_TG_LENGTH;

                /* set Ready flag -- outputbuffer filled                      */
                (*(&(*OutputBuffer_UB))) |= D_CCtp_BufferValid_UB;

                l_return_UB = D_FCtp_FLAG_Ready_UB;

                /* Set Timeout P2                                             */
                sTP.RXTimer_UW = sTP.STmax_UW;

                /* we will answer with 32 -> break because of too much data   */
                if (D_CCtp_FF_TOMUCHDATA == (sTP.TPstate_UW & D_CCtp_FF_TOMUCHDATA))
                {
                  (*(&(*OutputBuffer_UB) + D_CCtp_BUFFEROFFSET )) |= 0x02;
                  sTP.TPstate_UW &= ~D_CCtp_RESET_FF_TOMUCHDATA;
     
                  FUN_CCtp_InitInternal_V();

                } /*-7- END OF if (..) ---------------------------------------*/
              } /* --6- END OF if (..) ---------------------------------------*/
            } /* ----5- */
          } /* ------4- */
        } /* --------3- END OF if (..) ---------------------------------------*/
      } /* ----------2- END OF else (if(..)) ---------------------------------*/
    } /* ------------1- END OF else (if(..)) ---------------------------------*/

  return (l_return_UB);

} /* -+- END OF FUN_CCtp_TxTp_UB -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

/* ========================================================================== */
/*      E N D   O F   F I L E                                                 */
/* ========================================================================== */
