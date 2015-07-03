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
|  Filename:  ADLATUS_System.c                                                 |
|                                                                              |
|  Comment:   This file includes the operating system of the SMART ADLATUS.    |
|                                                                              |
+------------------------------------------------------------------------------+
| R E V I S I O N   H I S T O R Y                                              |
+------------------------------------------------------------------------------+
|
| Subversion Info:
| $Id: ADLATUS_System.c 4396 2008-11-04 09:26:32Z maennel $
| $HeadURL: http://smartsvn/svn/adlatusrepository/auslieferungen/S1NN/086350-S1NN-ADLATUS-TMS470R-MucSuc/20081201_S1NN_TMS470R_BlfIso-Source_V1.00/01_SW_Projekt/03_fsw/02_src/ADLATUS_System.c $
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
#include "ADLATUS_ConfigSystem_cdf.h" /* .. System configuration              */
#include "ADLATUS_Global_cdf.h"


/* ========================================================================== */
/* (2)  I N C L U D E S                                                       */
/* -------------------------------------------------------------------------- */
/*      T Y P E   D E F I N I T I O N   F I L E S  (TDF)                      */
/* ========================================================================== */
#include "ADLATUS_Types_tdf.h"           /* must be the first include         */
#include "ADLATUS_System_tdf.h"          /* .. System Modul type defines      */
#include "ADLATUS_Timer_tdf.h"           /* .. SW Timer handling              */
#include "ADLATUS_Hal_tdf.h"             /* .. HW Access                      */
#include "ADLATUS_NvmHandler_tdf.h"      /* .. NVM Access (Trigger)           */


/* ========================================================================== */
/* (3)  C O M P I L E R   S W I T C H   S E T T I N G S                       */
/* -------------------------------------------------------------------------- */
/*      O N L Y   F O R   T H I S   M O D U L                                 */
/* ========================================================================== */
/* Global Compiler switch settings are done at ADLATUS_CompilerSwitches_CDF.h */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* Local compiler switches: No Entry                                          */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#ifndef cs_CCsys_SingleChannelSystem
  #ifndef cs_CCsys_MultiChannelSystem 
       #error (undefined channel configuration)
  #endif
#endif
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


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

/* -------------------------------------------------------------------------- */
/*                                                                            */
/*      C O M M U N I C A T I O N   L A Y E R                                 */
/*      -----------------------------------------------------------------     */
/*      defines for layer assignment                                          */
/*                                                                            */
/*      +--------------------+--------------------+                           */
/*      | 06 - TxApplication |                    |                           */
/*      +--------------------+--------------------+                           */
/*      | 04 - RxSession     | 05 - TxSession     |                           */
/*      +--------------------+--------------------+                           */
/*      | 02 - RxTP          | 03 - TxTP          |                           */
/*      +--------------------+--------------------+                           */
/*      | 00 - RxPhysical    | 01 - TxPhysical    |                           */
/*      +--------------------+--------------------+                           */
/*                                                                            */
/*              Rx Path             Tx Path                                   */
/*                                                                            */
/* -------------------------------------------------------------------------- */
#define  d_CCsys_RxPath              (UBYTE) 0x00u  /* 0000 0000               */
#define  d_CCsys_TxPath              (UBYTE) 0x01u  /* 0000 0001               */
#define  d_CCsys_DirectionMask       (UBYTE) 0x01u  /* 0000 0001               */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define  d_CCsys_RxPhysicalLayer     (UBYTE) 0x00u  /* 0000 0000               */
#define  d_CCsys_TxPhysicalLayer     (UBYTE) 0x01u  /* 0000 0001               */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define  d_CCsys_RxTpLayer           (UBYTE) 0x02u  /* 0000 0010               */
#define  d_CCsys_TxTpLayer           (UBYTE) 0x03u  /* 0000 0011               */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define  d_CCsys_RxSessionLayer      (UBYTE) 0x04u  /* 0000 0100               */
#define  d_CCsys_TxSessionLayer      (UBYTE) 0x05u  /* 0000 0101               */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define  d_CCsys_RxApplicationLayer  (UBYTE) 0x06u  /* 0000 0110               */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


/* -------------------------------------------------------------------------- */
/*                                                                            */
/*  C O M M U N I C A T I O N   F L A G   R E G I S T E R   D E F I N E S     */
/*      -----------------------------------------------------------------     */
/*                                                                            */
/* -------------------------------------------------------------------------- */
#define  d_CCsys_InitSysComFlags_UW                        0x0000u
/*----------------------------------------------------------------------------*/
/*  RX READY FLAG - PHYSICAL LAYER                                            */
/*----------------------------------------------------------------------------*/
#define  d_CCsys_SetFlagPhysicalRxReady                    0x0001u
#define  d_CCsys_ResetFlagPhysicalRxReady                  0xFFFEu
/*----------------------------------------------------------------------------*/
/*  RX BUSY FLAG  - PHYSICAL LAYER                                            */
/*----------------------------------------------------------------------------*/
#define  d_CCsys_SetFlagPhysicalRxBusy                     0x0002u
#define  d_CCsys_ResetFlagPhysicalRxBusy                   0xFFFDu
/*----------------------------------------------------------------------------*/
/*  TX READY FLAG - PHYSICAL LAYER                                            */
/*----------------------------------------------------------------------------*/
#define  d_CCsys_SetFlagPhysicalTxReady                    0x0004u
#define  d_CCsys_ResetFlagPhysicalTxReady                  0xFFFBu
/*----------------------------------------------------------------------------*/
/*  TX BUSY FLAG  - PHYSICAL LAYER                                            */
/*----------------------------------------------------------------------------*/
#define  d_CCsys_SetFlagPhysicalTxBusy                     0x0008u
#define  d_CCsys_ResetFlagPhysicalTxBusy                   0xFFF7u
/*----------------------------------------------------------------------------*/
/*  RX READY FLAG - TP LAYER                                                  */
/*----------------------------------------------------------------------------*/
#define  d_CCsys_SetFlagTpRxReady                          0x0010u
#define  d_CCsys_ResetFlagTpRxReady                        0xFFEFu
/*----------------------------------------------------------------------------*/
/*  RX BUSY FLAG  - TP LAYER                                                  */
/*----------------------------------------------------------------------------*/
#define  d_CCsys_SetFlagTpRxBusy                           0x0020u
#define  d_CCsys_ResetFlagTpRxBusy                         0xFFDFu
/*----------------------------------------------------------------------------*/
/*  TX READY FLAG - TP LAYER                                                  */
/*----------------------------------------------------------------------------*/
#define  d_CCsys_SetFlagTpTxReady                          0x0040u
#define  d_CCsys_ResetFlagTpTxReady                        0xFFBFu
/*----------------------------------------------------------------------------*/
/*  TX BUSY FLAG  - TP LAYER                                                  */
/*----------------------------------------------------------------------------*/
#define  d_CCsys_SetFlagTpTxBusy                           0x0080u
#define  d_CCsys_ResetFlagTpTxBusy                         0xFF7Fu
/*----------------------------------------------------------------------------*/
/*  RX READY FLAG - SESSION LAYER                                             */
/*----------------------------------------------------------------------------*/
#define  d_CCsys_SetFlagSessionRxReady                     0x0100u
#define  d_CCsys_ResetFlagSessionRxReady                   0xFEFFu
/*----------------------------------------------------------------------------*/
/*  RX BUSY FLAG  - SESSION LAYER                                             */
/*----------------------------------------------------------------------------*/
#define  d_CCsys_SetFlagSessionRxBusy                      0x0200u
#define  d_CCsys_ResetFlagSessionRxBusy                    0xFDFFu
/*----------------------------------------------------------------------------*/
/*  TX READY FLAG - SESSION LAYER                                             */
/*----------------------------------------------------------------------------*/
#define  d_CCsys_SetFlagSessionTxReady                     0x0400u
#define  d_CCsys_ResetFlagSessionTxReady                   0xFBFFu
/*----------------------------------------------------------------------------*/
/*  TX BUSY FLAG  - SESSION LAYER                                             */
/*----------------------------------------------------------------------------*/
#define  d_CCsys_SetFlagSessionTxBusy                      0x0800u
#define  d_CCsys_ResetFlagSessionTxBusy                    0xF7FFu
/*----------------------------------------------------------------------------*/
/*  RX READY FLAG - APPLICATION LAYER                                         */
/*----------------------------------------------------------------------------*/
#define  d_CCsys_SetFlagApplicationRxReady                 0x1000u
#define  d_CCsys_ResetFlagApplicationRxReady               0xEFFFu
/*----------------------------------------------------------------------------*/
/*  RX BUSY FLAG  - APPLICATION LAYER                                         */
/*----------------------------------------------------------------------------*/
#define  d_CCsys_SetFlagApplicationRxBusy                  0x2000u
#define  d_CCsys_ResetFlagApplicationRxBusy                0xDFFFu


/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
/*                                                                            */
/*  Specially flag constellation for polling mode                             */
/*                                                                            */
/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
/*----------------------------------------------------------------------------*/
/*  TX PATH AT WORK                                                           */
/*----------------------------------------------------------------------------*/
#define d_CCsys_SetFlagTxPathBusy   ( d_CCsys_SetFlagPhysicalTxReady    | \
                                      d_CCsys_SetFlagPhysicalTxBusy     | \
                                      d_CCsys_SetFlagTpTxReady          | \
                                      d_CCsys_SetFlagTpTxBusy           | \
                                      d_CCsys_SetFlagSessionTxReady     | \
                                      d_CCsys_SetFlagSessionTxBusy      | \
                                      d_CCsys_SetFlagApplicationRxReady   )

/*----------------------------------------------------------------------------*/
/*  Communication Busy                                                        */
/*----------------------------------------------------------------------------*/
#define d_CCsys_SetFlagCommunicationBusy ( d_CCsys_SetFlagPhysicalRxReady    | \
                                           d_CCsys_SetFlagPhysicalRxBusy     | \
                                           d_CCsys_SetFlagPhysicalTxReady    | \
                                           d_CCsys_SetFlagPhysicalTxBusy     | \
                                           d_CCsys_SetFlagTpRxReady          | \
                                           d_CCsys_SetFlagTpRxBusy           | \
                                           d_CCsys_SetFlagTpTxReady          | \
                                           d_CCsys_SetFlagTpTxBusy           | \
                                           d_CCsys_SetFlagSessionRxReady     | \
                                           d_CCsys_SetFlagSessionRxBusy      | \
                                           d_CCsys_SetFlagSessionTxReady     | \
                                           d_CCsys_SetFlagSessionTxBusy      | \
                                           d_CCsys_SetFlagApplicationRxReady | \
                                           d_CCsys_SetFlagApplicationRxBusy    )
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/*----------------------------------------------------------------------------*/
/*  RX PATH AT WORK                                                           */
/*----------------------------------------------------------------------------*/
#define d_CCsys_SetFlagRxPathBusy   ( d_CCsys_SetFlagPhysicalRxReady    | \
                                      d_CCsys_SetFlagPhysicalRxBusy     | \
                                      d_CCsys_SetFlagTpRxReady          | \
                                      d_CCsys_SetFlagTpRxBusy           | \
                                      d_CCsys_SetFlagSessionRxReady     | \
                                      d_CCsys_SetFlagSessionRxBusy      | \
                                      d_CCsys_SetFlagApplicationRxReady | \
                                      d_CCsys_SetFlagApplicationRxBusy    )



/* -------------------------------------------------------------------------- */
/*                                                                            */
/*      C O M L A Y E R   S C A N   S T A T E   D E F I N E S                 */
/*      -----------------------------------------------------------------     */
/*                                                                            */
/*      t_ScanState_UB = x x x x  x x C S                                     */
/*                       | | | |  | | | |                                     */
/*                       | | | |  | | | +---   Stop Scan                      */
/*                       | | | |  | | +-----   Channel selected               */
/*                       +-+-+-+--+-+-------   Not used                       */
/*                                                                            */
/* -------------------------------------------------------------------------- */
#define  d_CCsys_InitScanState        (UBYTE) 0x00u
#define  d_CCsys_StopScan             (UBYTE) 0x01u
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

/* -------------------------------------------------------------------------- */
/*                                                                            */
/*      B U F F E R   R E F E R E N C E  -  T A B L E                         */
/*      -----------------------------------------------------------------     */
/*      defines for buffer assignment                                         */
/*                                                                            */
/*      NOTE: This defines are corresponding with the buffer config table at  */
/*            modul ADLATUS_ConfigSystem.c                                    */
/* -------------------------------------------------------------------------- */
/*                                                                            */
/*                                      +------------------+------------------*/
/*                                      | Phys. Layer Rx   | Phys. Layer Tx   */
/*                                      +------------------+------------------*/
#define d_CCsys_RxPhysOutputBuffer   (UBYTE)    0x00u
#define d_CCsys_TxPhysInputBuffer    (UBYTE)                       0x01u
#define d_CCsys_RxTPInputBuffer      (UBYTE)    0x00u
#define d_CCsys_TxTPOutputBuffer     (UBYTE)                       0x01u
/*                                      +------------------+------------------*/
/*                                      |    Tp Layer Rx   |    Tp Layer Tx   */
/*                                      +------------------+------------------*/
#define d_CCsys_RxTPOutputBuffer     (UBYTE)    0x02u
#define d_CCsys_TxTPInputBuffer      (UBYTE)                       0x03u
#define d_CCsys_RxSessionInputBuffer (UBYTE)    0x02u
#define d_CCsys_TxSessionOutputBuffer (UBYTE)                      0x03u
/*                                      +------------------+------------------*/
/*                                      | Session Layer Rx | Session Layer Tx */
/*                                      +------------------+------------------*/
#define d_CCsys_RxSessionOutputBuffer (UBYTE)    0x04u
#define d_CCsys_TxSessionInputBuffer (UBYTE)                       0x05u
#define d_CCsys_RxApplInputBuffer    (UBYTE)    0x04u
#define d_CCsys_RxApplOutBuffer      (UBYTE)                       0x05u
/*                                      +------------------+------------------*/
/*                                      |        Application Layer            */
/*                                      +------------------+------------------*/
/*                                                                            */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define  d_CCsys_InputBufferLine     (UBYTE)  0x00u
#define  d_CCsys_OutputBufferLine    (UBYTE)  0x01u
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */



/* ========================================================================== */
/* (6)  L O C A L  T Y P E   D E F I N E S                                    */
/* -------------------------------------------------------------------------- */
/*      O N L Y   F O R   T H I S   M O D U L                                 */
/* ========================================================================== */


/*----------------------------------------------------------------------------*/
/* FlagRegister for System Control                                            */
/*----------------------------------------------------------------------------*/
typedef struct _FLAGREGISTER
{
  UWORD SysComRegister_UW;  /*  2 Byte - System Communication Flag Register   */
  UBYTE ErrorRegister_UB;   /*  1 Byte - Error Flag Register                  */
  UBYTE Reserved01_UB;      /*  1 Byte - Reserved for ULONG alingment         */
                            /* -----------------------------------------------*/
} FLAGREGISTER_ST;          /*  4 Byte                                        */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

/*----------------------------------------------------------------------------*/
/* System control register                                                    */
/*----------------------------------------------------------------------------*/
typedef struct _SYSTEMCONTROL
{
  PTR_BUFFER_UB              InBuffer_PBUF;                         /*  4 Byte - Ptr to Inbuffer  */
  PTR_BUFFER_UB              OutBuffer_PBUF;                        /*  4 Byte - Ptr to Outbuffer */
  const BUFFERCONFIGURATION_ST* SysBuffer_PST;                      /*  4 Byte                    */
  const COMSTACKFUNCTIONS_ST* ComStackFunctions_PST;                /*  4 Byte                    */
  const COMHALFUNCTIONS_ST* ComHalFkt_PST;                          /*  4 Byte                    */
  const PTR_TO_EVENTPROCESSING_FKT* EventProcessingFkt_PFUN;        /*  4 Byte                    */
  FLAGREGISTER_ST            FlagReg_AST[d_CCconsys_NbrOfLogChannel]; /*  4 Byte * x channel      */
                       /*---------------------------------------------------------------------*/
} SYSTEMCONTROL_ST;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


/* ========================================================================== */
/* (7)  L O C A L   V A R I A B L E S  / C O N S T A N T S                    */
/* -------------------------------------------------------------------------- */
/*      O N L Y   F O R   T H I S   M O D U L                                 */
/* ========================================================================== */

/* -------------------------------------------------------------------------- */
/*                                                                            */
/*      B U F F E R   R E F E R E N C E  -  T A B L E                         */
/*      -----------------------------------------------------------------     */
/*      BufferAssignment                                                      */
/*                                                                            */
/* -------------------------------------------------------------------------- */
/* Note:                                                                      */
/* The accessability of this constant is compiler or/and device dependent     */
const UBYTE c_CCsys_BufferAssignmentTable_AUB[7][2] =
{
/*-------------------------+-----------------------------+-------------------------------*/
/* Layer Info              | 00 - InputBuffer Line       | 01 - OutputBufferLine         */
/*-------------------------+-----------------------------+-------------------------------*/
/* 00 Rx Physical Layer */ { d_CCsys_RxPhysOutputBuffer  , d_CCsys_RxPhysOutputBuffer    },
/* 01 Tx Physical Layer */ { d_CCsys_TxPhysInputBuffer   , d_CCsys_TxPhysInputBuffer     },
/* 02 Rx Tp Layer       */ { d_CCsys_RxTPInputBuffer     , d_CCsys_RxTPOutputBuffer      },
/* 03 Tx Tp Layer       */ { d_CCsys_TxTPInputBuffer     , d_CCsys_TxTPOutputBuffer      },
/* 04 Rx Session Layer  */ { d_CCsys_RxSessionInputBuffer, d_CCsys_RxSessionOutputBuffer },
/* 05 Tx Session Layer  */ { d_CCsys_TxSessionInputBuffer, d_CCsys_TxSessionOutputBuffer },
/* 06 Rx Applic. Layer  */ { d_CCsys_RxApplInputBuffer   , d_CCsys_RxApplOutBuffer       },

}; /* -+- END OF ARRAY -------------------------------------------------------*/
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


/* -------------------------------------------------------------------------- */
/*                                                                            */
/*      S Y S T E M    C O N T R O L   R E G I S T E R                        */
/*      -----------------------------------------------------------------     */
/*                                                                            */
/*                                                                            */
/* -------------------------------------------------------------------------- */
SYSTEMCONTROL_ST  CCsys_SysCtrl_ST;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


/* ========================================================================== */
/* (8)  L O C A L   F U N C T I O N   P R O T O T Y P E S                     */
/* -------------------------------------------------------------------------- */
/*      O N L Y   F O R   T H I S   M O D U L                                 */
/* ========================================================================== */

/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */
#ifdef cs_CCsys_SingleChannelSystem /* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */
/*----------------------------------------------------------------------------*/

void   FUN_CCsys_BufferRouter_V      ( UBYTE  t_LayerInfo_UB  );
UBYTE  FUN_CCsys_ResultEvaluation_UB ( UBYTE  t_LayerInfo_UB, \
                                       UBYTE  t_Result_UB     );
/*----------------------------------------------------------------------------*/
/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
#else /* cs_CCsys_MultiChannelSystem  --------------------------------------- */
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */
/*----------------------------------------------------------------------------*/

void   FUN_CCsys_BufferRouter_V      ( UBYTE  t_Channel_UB,    \
                                       UBYTE  t_LayerInfo_UB  );
UBYTE  FUN_CCsys_ResultEvaluation_UB ( UBYTE  t_Channel_UB,    \
                                       UBYTE  t_LayerInfo_UB,  \
                                       UBYTE  t_Result_UB     );

/*----------------------------------------------------------------------------*/
/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
#endif /* cs_CCsys_MultiChannelSystem  -------------------------------------- */
/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */



/* ========================================================================== */
/* (9)  G L O B A L   V A R I A B L E S                                       */
/* -------------------------------------------------------------------------- */
/*      D E F I N E D   W I T H I N   T H I S   M O D U L                     */
/* ========================================================================== */
/* No entry                                                                   */

/* ========================================================================== */
/* (10) F U N C T I O N   D E F I N I T I O N S                               */
/* -------------------------------------------------------------------------- */
/*      F O R   T H I S   M O D U L                                           */
/* ========================================================================== */




/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */
/*                                                                            */
/* S I N G L E   C H A N N E L   S Y S T E M                                  */
/*                                                                            */
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */
#ifdef cs_CCsys_SingleChannelSystem /* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
  /* ------------------------------------------------------------------------ */
  /*      +--------------------+--------------------+                         */
  /*      | 06 - TxApplication |                    |                         */
  /*      +--------------------+--------------------+                         */
  /*      | 04 - RxSession     | 05 - TxSession     |                         */
  /*      +--------------------+--------------------+                         */
  /*      | 02 - RxTP          | 03 - TxTP          |                         */
  /*      +--------------------+--------------------+                         */
  /*      | 00 - RxPhysical    | 01 - TxPhysical    |                         */
  /*      +--------------------+--------------------+                         */
  /*              Rx Path             Tx Path                                 */
  /* -------------------------------------------------------------------------*/
  /*      SysComRegister_UW:                                                  */
  /*                                                                          */
  /*      F E D C  B A 9 8  7 6 5 4  3 2 1 0                                  */
  /*      | | | |  | | | |  | | | |  | | | |                                  */
  /*      | | | |  | | | |  | | | |  | | | +-- Rx Physical    Ready  0x02     */
  /*      | | | |  | | | |  | | | |  | | +---- Rx Physical    Busy            */
  /*      | | | |  | | | |  | | | |  | +------ Tx Physical    Ready           */
  /*      | | | |  | | | |  | | | |  +-------- Tx Physical    Busy            */
  /*      | | | |  | | | |  | | | +----------- Rx Transport   Ready  0x04     */
  /*      | | | |  | | | |  | | +------------- Rx Transport   Busy            */
  /*      | | | |  | | | |  | +--------------- Tx Transport   Ready  0x01     */
  /*      | | | |  | | | |  +----------------- Tx Transport   Busy            */
  /*      | | | |  | | | +-------------------- Rx Session     Ready  0x06     */
  /*      | | | |  | | +---------------------- Rx Session     Busy            */
  /*      | | | |  | +------------------------ Tx Session     Ready  0x03     */
  /*      | | | |  +-------------------------- Tx Session     Busy            */
  /*      | | | +----------------------------- Rx Application Ready  0x05     */
  /*      | | +------------------------------- Rx Application Busy            */
  /*      +-+--------------------------------- not used                       */
  /*                                                                          */
  /* ------------------------------------------------------------------------ */

const UWORD c_CCsys_ReadyFlags_AUW[1][8] =
{
  {
    0xFFFFu,
    d_CCsys_ResetFlagTpTxReady,
    d_CCsys_ResetFlagPhysicalRxReady,
    d_CCsys_ResetFlagSessionTxReady,
    d_CCsys_ResetFlagTpRxReady,
    d_CCsys_ResetFlagApplicationRxReady,
    d_CCsys_ResetFlagSessionRxReady,
    0xFFFFu
  }
};
/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  CCsys_InitOperatingSystem_V                                 |
|                                                                              |
|  Prototype at:   ADLATUS_System_tdf.h                                        |
|                                                                              |
|  Description:    Initialisation of the operating system for the SMART        |
|                  ADLATUS.                                                    |
|                                                                              |
|  Parameter:      const SYSTEMINTERFACE_ST* t_SysInterface_PST .. Ptr to interface  |
|                                                                              |
|  Return Value:    --                                                         |
|                                                                              |
+-----------------------------------------------------------------------------*/
void FUN_CCsys_InitOperatingSystem_V (const SYSTEMINTERFACE_ST* t_SysInterface_PST)
{

  /*--------------------------------------------------------------------------*/
  /* Init system variables                                                    */
  /* (1) Pointer to system buffer structure                                   */
  /* (2) Pointer to communication function stack structure                    */
  /* (3) Pointer to communication HAL function                                */
  /* (4) Pointer to communication error function                              */
  /* (5) Pointer to event processing functions                                */
  /*--------------------------------------------------------------------------*/
  CCsys_SysCtrl_ST.SysBuffer_PST           = t_SysInterface_PST->SystemBufferConfig_PST;
  CCsys_SysCtrl_ST.ComStackFunctions_PST   = t_SysInterface_PST->ComStackFunctions_PST;
  CCsys_SysCtrl_ST.ComHalFkt_PST           = t_SysInterface_PST->ComHalFunctions_PST;
  CCsys_SysCtrl_ST.EventProcessingFkt_PFUN = t_SysInterface_PST->EventProcessingFkt_PAFUN;

  /*--------------------------------------------------------------------------*/
  /* Init system variables                                                    */
  /* (6) Init system communication state register                             */
  /* (7) Init system communication error register                             */
  /*--------------------------------------------------------------------------*/
  CCsys_SysCtrl_ST.FlagReg_AST[d_CCglo_Channel00].SysComRegister_UW = 0x0000u;
  CCsys_SysCtrl_ST.FlagReg_AST[d_CCglo_Channel00].ErrorRegister_UB  = 0x00;
  
  return;

} /* -+- END OF FUNCTION 'FUN_CCsys_InitOperatingSystem_V' -+-+-+-+-+-+-+-+-+-*/






/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  CCsys_OperatingSystem_V                                     |
|                                                                              |
|  Prototype at:   ADLATUS_System_tdf.h                                        |
|                                                                              |
|  Description:    Operating System of the SMART ADLATUS.                      |
|                                                                              |
|  Parameter:       --                                                         |
|                                                                              |
|  Return Value:    --                                                         |
|                                                                              |
+-----------------------------------------------------------------------------*/
void FUN_CCsys_OperatingSystem_V (void)
{

  UBYTE t_Result_UB;         /* Temp variable for current function call result*/
  UBYTE t_Direction_UB;      /* Temp variable for current direction (Rx/Tx)   */
  UBYTE t_ActScanLayer_UB;   /* Temp variable for current layer in process    */
  UBYTE t_ScanStartLayer_UB; /* Temp variable for scan start layer info       */
  UBYTE t_ScanStopLayer_UB;  /* Temp variable for scan stop layer info        */
  UBYTE t_ScanState_UB;      /* Temp variable for the return value of the     */
                             /* ResultEvaluation-routine                      */

  volatile UBYTE t_InBuffer_PBUF[12];

  ULONG counter;

  counter   = 0xCE80;

  /*--------------------------------------------------------------------------*/
  /* Init local system variables                                              */
  /*--------------------------------------------------------------------------*/
  t_Direction_UB = 0x01;

  /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
  /*                                                                          */
  /* -1- Starting endless loop                                                */
  /*                                                                          */
  /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
  while (1)
  {
    counter--;
    
    if (counter == 0)
    {
      counter = 0x33A0;
     

      t_InBuffer_PBUF[d_CCglo_BufferState] = d_CCglo_BufferValid;

      t_InBuffer_PBUF[d_CCglo_ChannelNbr] = (d_CCglo_SubSet04 | d_CCglo_Channel00);

      t_InBuffer_PBUF[d_CCglo_DLC] = 0x06;

      /* Copy Data from CanControllerBuffer to RxMsgBuffer                    */
      t_InBuffer_PBUF[d_CCglo_StartOfData + 0] = 0x22;
      t_InBuffer_PBUF[d_CCglo_StartOfData + 1] = 0x00;
      t_InBuffer_PBUF[d_CCglo_StartOfData + 2] = 0xCA;
      t_InBuffer_PBUF[d_CCglo_StartOfData + 3] = 0xFE;
      t_InBuffer_PBUF[d_CCglo_StartOfData + 4] = 0xBA;
      t_InBuffer_PBUF[d_CCglo_StartOfData + 5] = 0xBE;

      FUN_CCcan_TxData_UB (&t_InBuffer_PBUF[0], &t_InBuffer_PBUF[0] );
    }
    /*========================================================================*/
    /* -2- Check each direction of a channel                                  */
    /*========================================================================*/
    for (t_Direction_UB =  d_CCsys_RxPath;
         t_Direction_UB <= d_CCsys_TxPath;
         t_Direction_UB ++ )
    {
      /*======================================================================*/
      /* -3- Direction: RxPath                                                */
      /*======================================================================*/
      if ( d_CCsys_RxPath == t_Direction_UB )
      {
        /*--------------------------------------------------------------------*/
        /* Set StartLayer info                                                */
        /*--------------------------------------------------------------------*/
        t_ScanStartLayer_UB = d_CCsys_RxPhysicalLayer;

        /*--------------------------------------------------------------------*/
        /* Set StopLayer info                                                 */
        /* NOTE: The application call is only possible if the TxPath is idle  */
        /*--------------------------------------------------------------------*/

        /*====================================================================*/
        /* -4- TxPath is busy                                                 */
        /*====================================================================*/
        if ( (CCsys_SysCtrl_ST.FlagReg_AST[d_CCglo_Channel00].SysComRegister_UW & \
              d_CCsys_SetFlagTxPathBusy) != 0x0000u)
        {
          /* Scan only from physical to session layer without application     */
          t_ScanStopLayer_UB  = d_CCsys_RxSessionLayer;

        } /* -4- END OF if(..) -----------------------------------------------*/

        /*====================================================================*/
        /* -4- TxPath is idle                                                 */
        /*====================================================================*/
        else
        {
          /* Scan from physical to application layer                          */
          t_ScanStopLayer_UB  = d_CCsys_RxApplicationLayer;
        } /* -4- END OF else (if(..)) ----------------------------------------*/
      } /* ---3- END OF if(..) -----------------------------------------------*/
   
      /*======================================================================*/
      /* -3- Direction: TxPath                                                */
      /*======================================================================*/
      else
      {
        /* Scan from physical to session layer                                */
        t_ScanStartLayer_UB = d_CCsys_TxPhysicalLayer;
        t_ScanStopLayer_UB  = d_CCsys_TxSessionLayer;

      } /* -3- END OF else (if(..)) ------------------------------------------*/

      /*======================================================================*/
      /* -3- Scan each layer of the current direction                         */
      /*======================================================================*/
      for ( t_ActScanLayer_UB =  t_ScanStartLayer_UB;
            t_ActScanLayer_UB <= t_ScanStopLayer_UB;
            t_ActScanLayer_UB += 0x02 )
      {
        /*---------------------------------------------------------------------+
        |                                                                      |
        | S Y S T E M   T I M E R                                              |
        |                                                                      |
        +----------------------------------------------------------------------+
        | Check the HW-Timer                                                   |
        +---------------------------------------------------------------------*/
        FUN_CCtim_SystemTimerCheck_V ();

        /*--------------------------------------------------------------------*/
        /* Check the communication interface HW access layer function         */
        /*--------------------------------------------------------------------*/
        t_Result_UB = CCsys_SysCtrl_ST.ComHalFkt_PST[0x00].ComHALFkt_PFUN ();

        /*====================================================================*/
        /* -4- Data from physical layer received                              */
        /*====================================================================*/
        if ( d_CCglo_Ready == t_Result_UB )
        {
          /* Here is a good place to get a timer value for the random value   */
          /* creation                                                         */
          FUN_CChal_StoreHwTimerValue_V ();
        } /* -4- END OF if(..) -----------------------------------------------*/

        /*--------------------------------------------------------------------*/
        /* Buffer router                                                      */
        /*--------------------------------------------------------------------*/
        FUN_CCsys_BufferRouter_V ( t_ActScanLayer_UB );

        /*--------------------------------------------------------------------*/
        /* Reset the Ready-Flag from function before                          */
        /*------------------------------------------------------------------*/
        CCsys_SysCtrl_ST.FlagReg_AST[d_CCglo_Channel00].SysComRegister_UW &= c_CCsys_ReadyFlags_AUW[0][t_ActScanLayer_UB];

        /*--------------------------------------------------------------------*/
        /* Call the communication function                                    */
        /*--------------------------------------------------------------------*/
        t_Result_UB = CCsys_SysCtrl_ST.ComStackFunctions_PST[d_CCglo_Channel00].Layer_PFUN[t_ActScanLayer_UB] \
                             ( CCsys_SysCtrl_ST.InBuffer_PBUF, \
                               CCsys_SysCtrl_ST.OutBuffer_PBUF );

        /*--------------------------------------------------------------------*/
        /* Result evaluation                                                  */
        /*--------------------------------------------------------------------*/
        t_ScanState_UB =  FUN_CCsys_ResultEvaluation_UB ( t_ActScanLayer_UB, \
                                                          t_Result_UB        );

        /*====================================================================*/
        /* -4- Stop criterion occured                                         */
        /*====================================================================*/
        if ( d_CCsys_StopScan == (t_ScanState_UB & d_CCsys_StopScan))
        {
          t_ActScanLayer_UB = (t_ScanStopLayer_UB + 1);

        } /* -4- END OF if (..) ----------------------------------------------*/
      } /* ---3- END OF FOR (..) ---------------------------------------------*/

      /*======================================================================*/
      /* -3- Direction: TxPath                                                */
      /*     ATTENTION: Don't change this sequence!!                          */
      /*======================================================================*/
      if ( d_CCsys_TxPath == t_Direction_UB)
      {
        /*====================================================================*/
        /* -4- Communication is idle                                          */
        /*====================================================================*/
        if ( 0x0000u == (CCsys_SysCtrl_ST.FlagReg_AST[d_CCglo_Channel00].SysComRegister_UW & \
                        d_CCsys_SetFlagCommunicationBusy) )
        {

          /*----------------------------------------------------------------*/
          /* Call the channel specific event handler                        */
          /*----------------------------------------------------------------*/
          CCsys_SysCtrl_ST.EventProcessingFkt_PFUN[d_CCglo_Channel00] ();

        } /* -4- END OF if(..) -----------------------------------------------*/
      } /* ---3- END OF if(..) -----------------------------------------------*/
    } /* -----2- END OF for (..) ---------------------------------------------*/
  } /* -------1- END OF while (1) --------------------------------------------*/

} /*-+- END OF FUNCTION 'CCsys_OperatingSystem_V' -+-+-+-+-+-+-+-+-+-+-+-+-+-+*/


/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CCsys_BufferRouter_V                                    |
|                                                                              |
|  Prototype at:   ADLATUS_System.c                                            |
|                                                                              |
|  Description:    This function configurate the buffer structure before a     |
|                  function is called. It depends on the layer information.    |
|                                                                              |
|  Parameter:      UBYTE t_ChannelNbr_UB                                       |
|                  UBYTE t_LayerInfo_UB                                        |
|                                                                              |
|  Return Value:   -                                                           |
|                                                                              |
+-----------------------------------------------------------------------------*/
void FUN_CCsys_BufferRouter_V ( UBYTE t_LayerInfo_UB )
{
  UBYTE t_ConfigTableLine_UB;

  /*------------------------------------------------------------------------*/
  /* Init Config-Table Line for inputbuffer selection                       */
  /*------------------------------------------------------------------------*/
  t_ConfigTableLine_UB = \
    c_CCsys_BufferAssignmentTable_AUB[t_LayerInfo_UB][d_CCsys_InputBufferLine]; 

  /*------------------------------------------------------------------------*/
  /* Set the Input buffer                                                   */
  /*------------------------------------------------------------------------*/
  CCsys_SysCtrl_ST.InBuffer_PBUF = \
    CCsys_SysCtrl_ST.SysBuffer_PST[d_CCglo_Channel00].Line_PBUF[t_ConfigTableLine_UB];

  /*------------------------------------------------------------------------*/
  /* Init Config-Table Line for outputbuffer selection                      */
  /*------------------------------------------------------------------------*/
  t_ConfigTableLine_UB = \
    c_CCsys_BufferAssignmentTable_AUB[t_LayerInfo_UB][d_CCsys_OutputBufferLine]; 

  /*------------------------------------------------------------------------*/
  /* Set the Output buffer                                                  */
  /*------------------------------------------------------------------------*/
  CCsys_SysCtrl_ST.OutBuffer_PBUF = \
    CCsys_SysCtrl_ST.SysBuffer_PST[d_CCglo_Channel00].Line_PBUF[t_ConfigTableLine_UB];

  return;

} /* END OF FUNCTION 'FUN_CCsys_BufferRouter_V' -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/

/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CCsys_ResultEvaluation_V                                |
|                                                                              |
|  Prototype at:   ADLATUS_System.c                                            |
|                                                                              |
|  Description:    This function evaluates the result of a function call       |
|                  of a communication stack routine. The correspondig flags    |
|                  will be set within the communication flag register via the  |
|                  SysComHandler.                                              |
|                                                                              |
|  Parameter:      UBYTE t_Result_UB    .. Result which should be evaluated    |
|                  UWORD t_LayerInfo_UW .. Info about the layer which has      |
|                                          produced the result.                |
|                  UBYTE t_Channel_UB   .. Channel number                      |
|                                                                              |
|  Return Value:   UBYTE t_ScanState_UB .. 0x00 Don't StopScan                 |
|                                       .. 0x01 StopScan                       |
|                                                                              |
+-----------------------------------------------------------------------------*/
UBYTE  FUN_CCsys_ResultEvaluation_UB ( UBYTE t_LayerInfo_UB, \
                                       UBYTE t_Result_UB     )
{
  UBYTE  t_ScanState_UB;   /* return value of the routine                     */
  UBYTE  t_Counter_UB;     /* runtime variable for channel check              */
  UWORD  t_ReadyFlag_UW;   /* variable for READY flag shifting                */
  UWORD  t_BusyFlag_UW;    /* variable for BUSY flag shifting                 */

  /*--------------------------------------------------------------------------*/
  /* Init Scanstate for the SysComHandler function                            */
  /*--------------------------------------------------------------------------*/
  t_ScanState_UB = d_CCsys_InitScanState;

  /*==========================================================================*/
  /* -1- Execute only if something is to do                                   */
  /*==========================================================================*/
  if ( (d_CCglo_Idle != t_Result_UB) ||
       (0x0000u != CCsys_SysCtrl_ST.FlagReg_AST[d_CCglo_Channel00].SysComRegister_UW))
  {

    /*------------------------------------------------------------------------*/
    /* Init variables for the SysComHandler function                          */
    /*------------------------------------------------------------------------*/
    t_ReadyFlag_UW = 0x0001u;
    t_BusyFlag_UW  = 0x0002u;

    /* -----------------------------------------------------------------------*/
    /*      Layer Information:                                                */
    /*                                                                        */
    /*      +--------------------+--------------------+                       */
    /*      | 06 - TxApplication |                    |                       */
    /*      +--------------------+--------------------+                       */
    /*      | 04 - RxSession     | 05 - TxSession     |                       */
    /*      +--------------------+--------------------+                       */
    /*      | 02 - RxTP          | 03 - TxTP          |                       */
    /*      +--------------------+--------------------+                       */
    /*      | 00 - RxPhysical    | 01 - TxPhysical    |                       */
    /*      +--------------------+--------------------+                       */
    /*                                                                        */
    /*              Rx Path             Tx Path                               */
    /*                                                                        */
    /* -----------------------------------------------------------------------*/
    /*      SysComRegister_UW:                                                */
    /*                                                                        */
    /*      F E D C  B A 9 8  7 6 5 4  3 2 1 0                                */
    /*      | | | |  | | | |  | | | |  | | | |                                */
    /*      | | | |  | | | |  | | | |  | | | +-- Rx Physical    Ready         */
    /*      | | | |  | | | |  | | | |  | | +---- Rx Physical    Busy          */
    /*      | | | |  | | | |  | | | |  | +------ Tx Physical    Ready         */
    /*      | | | |  | | | |  | | | |  +-------- Tx Physical    Busy          */
    /*      | | | |  | | | |  | | | +----------- Rx Transport   Ready         */
    /*      | | | |  | | | |  | | +------------- Rx Transport   Busy          */
    /*      | | | |  | | | |  | +--------------- Tx Transport   Ready         */
    /*      | | | |  | | | |  +----------------- Tx Transport   Busy          */
    /*      | | | |  | | | +-------------------- Rx Session     Ready         */
    /*      | | | |  | | +---------------------- Rx Session     Busy          */
    /*      | | | |  | +------------------------ Tx Session     Ready         */
    /*      | | | |  +-------------------------- Tx Session     Busy          */
    /*      | | | +----------------------------- Rx Application Ready         */
    /*      | | +------------------------------- Rx Application Busy          */
    /*      +-+--------------------------------- not used                     */
    /*========================================================================*/
    /* -2- Shift the flags in order to the layer info                         */
    /*========================================================================*/
    for ( t_Counter_UB = 0x00;
          t_Counter_UB < t_LayerInfo_UB;
          t_Counter_UB ++)
    {
      t_ReadyFlag_UW <<= 0x02;
      t_BusyFlag_UW  <<= 0x02;

    } /* -2- END OF for(..) --------------------------------------------------*/


    /*------------------------------------------------------------------------*/
    /*                                                                        */
    /*  S E T   T H E   F L A G S                                             */
    /*                                                                        */
    /*------------------------------------------------------------------------*/

    /*========================================================================*/
    /* -2- READY Flag is set                                                  */
    /*========================================================================*/
    if ( d_CCglo_Ready == (t_Result_UB & d_CCglo_Ready))  
    {
      /* Set corresponding READY Flag of the layer                            */
      CCsys_SysCtrl_ST.FlagReg_AST[d_CCglo_Channel00].SysComRegister_UW |= t_ReadyFlag_UW;
    } /* -2- END OF if(..) ---------------------------------------------------*/

    /*========================================================================*/
    /* -2- READY Flag is not set                                              */
    /*========================================================================*/
    else
    {
      /* Reset corresponding READY Flag of the layer                          */
      CCsys_SysCtrl_ST.FlagReg_AST[d_CCglo_Channel00].SysComRegister_UW &= (UWORD) ~t_ReadyFlag_UW;
    } /* -2- END OF else (if(..)) --------------------------------------------*/

    /*========================================================================*/
    /* -2- BUSY Flag is set                                                   */
    /*========================================================================*/
    if ( d_CCglo_Busy == (t_Result_UB & d_CCglo_Busy))  
    {
      /* Set corresponding BUSY Flag of the layer                             */
      CCsys_SysCtrl_ST.FlagReg_AST[d_CCglo_Channel00].SysComRegister_UW |= t_BusyFlag_UW;
    } /* -2- END OF if(..) ---------------------------------------------------*/

    /*========================================================================*/
    /* -2- BUSY Flag is not set                                               */
    /*========================================================================*/
    else
    {
      /* Reset corresponding BUSY Flag of the layer                           */
      CCsys_SysCtrl_ST.FlagReg_AST[d_CCglo_Channel00].SysComRegister_UW &= (UWORD) ~t_BusyFlag_UW;
    } /* -2- END OF else (if(..)) --------------------------------------------*/

  } /* -2- END OF if(..) -----------------------------------------------------*/

  /*--------------------------------------------------------------------------*/
  /*                                                                          */
  /*  C H E C K   T H E   S T O P   C R I T E R I O N                         */
  /*                                                                          */
  /*--------------------------------------------------------------------------*/

  /*==========================================================================*/
  /* -1- RxPath                                                               */
  /*==========================================================================*/
  if ((t_LayerInfo_UB & d_CCsys_DirectionMask) == d_CCsys_RxPath)
  {
    /*========================================================================*/
    /* -2- Stop Scan                                                          */
    /*========================================================================*/
    if ( (t_Result_UB & d_CCglo_Busy) || (t_Result_UB & d_CCglo_Error) ) 
    {
      /* Set Flag                                                             */
      t_ScanState_UB |= d_CCsys_StopScan;

    } /* -2- END OF if (..) --------------------------------------------------*/
  } /* ---1- END OF if (..) --------------------------------------------------*/

  /*==========================================================================*/
  /* -1- TxPath                                                               */
  /*==========================================================================*/
  else
  {

    /*========================================================================*/
    /* -2- Stop Scan                                                          */
    /*========================================================================*/
    if ( d_CCglo_Idle != (t_Result_UB & d_CCglo_ResultInfoMask)) 
    {
      /* Set Flag                                                             */
      t_ScanState_UB |= d_CCsys_StopScan;

    } /* -2- END OF if (..) --------------------------------------------------*/
  } /* ---1- END OF else (if (..)) -------------------------------------------*/

  return ( t_ScanState_UB );

} /* END OF FUNCTION 'FUN_CCsys_ResultEvaluation_UB' -+-+-+-+-+-+-+-+-+-+-+-+-*/


/*----------------------------------------------------------------------------*/
/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
/*                                                                            */
/*      E N D   O F   S I N G L E   C H A N N E L   S Y S T E M               */
/*                                                                            */
/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
#else /* cs_CCsys_SingleChannelSystem  >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */
/*                                                                            */
/*      M U L T I   C H A N N E L   S Y S T E M                               */
/*                                                                            */
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */
/*----------------------------------------------------------------------------*/


  /* ------------------------------------------------------------------------ */
  /*      +--------------------+--------------------+                         */
  /*      | 06 - TxApplication |                    |                         */
  /*      +--------------------+--------------------+                         */
  /*      | 04 - RxSession     | 05 - TxSession     |                         */
  /*      +--------------------+--------------------+                         */
  /*      | 02 - RxTP          | 03 - TxTP          |                         */
  /*      +--------------------+--------------------+                         */
  /*      | 00 - RxPhysical    | 01 - TxPhysical    |                         */
  /*      +--------------------+--------------------+                         */
  /*              Rx Path             Tx Path                                 */
  /* -------------------------------------------------------------------------*/
  /*      SysComRegister_UW:                                                  */
  /*                                                                          */
  /*      F E D C  B A 9 8  7 6 5 4  3 2 1 0                                  */
  /*      | | | |  | | | |  | | | |  | | | |                                  */
  /*      | | | |  | | | |  | | | |  | | | +-- Rx Physical    Ready  0x02     */
  /*      | | | |  | | | |  | | | |  | | +---- Rx Physical    Busy            */
  /*      | | | |  | | | |  | | | |  | +------ Tx Physical    Ready           */
  /*      | | | |  | | | |  | | | |  +-------- Tx Physical    Busy            */
  /*      | | | |  | | | |  | | | +----------- Rx Transport   Ready  0x04     */
  /*      | | | |  | | | |  | | +------------- Rx Transport   Busy            */
  /*      | | | |  | | | |  | +--------------- Tx Transport   Ready  0x01     */
  /*      | | | |  | | | |  +----------------- Tx Transport   Busy            */
  /*      | | | |  | | | +-------------------- Rx Session     Ready  0x06     */
  /*      | | | |  | | +---------------------- Rx Session     Busy            */
  /*      | | | |  | +------------------------ Tx Session     Ready  0x03     */
  /*      | | | |  +-------------------------- Tx Session     Busy            */
  /*      | | | +----------------------------- Rx Application Ready  0x05     */
  /*      | | +------------------------------- Rx Application Busy            */
  /*      +-+--------------------------------- not used                       */
  /*                                                                          */
  /* ------------------------------------------------------------------------ */

const UWORD c_CCsys_ReadyFlags_AUW[2][8] =
{
  {
    0xFFFFu,
    d_CCsys_ResetFlagTpTxReady,
    d_CCsys_ResetFlagPhysicalRxReady,
    d_CCsys_ResetFlagSessionTxReady,
    d_CCsys_ResetFlagTpRxReady,
    d_CCsys_ResetFlagApplicationRxReady,
    d_CCsys_ResetFlagSessionRxReady,
    0xFFFFu
  },{
    0xFFFFu,
    d_CCsys_ResetFlagTpTxReady,
    d_CCsys_ResetFlagPhysicalRxReady,
    d_CCsys_ResetFlagSessionTxReady,
    d_CCsys_ResetFlagTpRxReady,
    d_CCsys_ResetFlagApplicationRxReady,
    d_CCsys_ResetFlagSessionRxReady,
    0xFFFFu
  }
};
/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  CCsys_InitOperatingSystem_V                                 |
|                                                                              |
|  Prototype at:   ADLATUS_System_tdf.h                                        |
|                                                                              |
|  Description:    Initialisation of the operating system for the SMART        |
|                  ADLATUS.                                                    |
|                                                                              |
|  Parameter:      const SYSTEMINTERFACE_ST* t_SysInterface_PST .. Ptr to interface  |
|                                                                              |
|  Return Value:    --                                                         |
|                                                                              |
+-----------------------------------------------------------------------------*/
void FUN_CCsys_InitOperatingSystem_V (const SYSTEMINTERFACE_ST* t_SysInterface_PST)
{

  UBYTE t_Channel_UB;  /* runtime variable for initialisation                 */

  /*--------------------------------------------------------------------------*/
  /* Init system variables                                                    */
  /* (1) Pointer to system buffer structure                                   */
  /* (2) Pointer to communication function stack structure                    */
  /* (3) Pointer to communication HAL function                                */
  /* (4) Pointer to communication error function                              */
  /* (5) Pointer to event processing functions                                */
  /*--------------------------------------------------------------------------*/
  CCsys_SysCtrl_ST.SysBuffer_PST           = t_SysInterface_PST->SystemBufferConfig_PST;
  CCsys_SysCtrl_ST.ComStackFunctions_PST   = t_SysInterface_PST->ComStackFunctions_PST;
  CCsys_SysCtrl_ST.ComHalFkt_PST           = t_SysInterface_PST->ComHalFunctions_PST;
  CCsys_SysCtrl_ST.EventProcessingFkt_PFUN = t_SysInterface_PST->EventProcessingFkt_PAFUN;

  /*==========================================================================*/
  /* -1- Init system flag registers                                           */
  /*==========================================================================*/
  for (t_Channel_UB = 0x00;
       t_Channel_UB < d_CCconsys_NbrOfLogChannel;
       t_Channel_UB ++ )
  {
    /*------------------------------------------------------------------------*/
    /* Init system variables                                                  */
    /* (6) Init system communication state register                           */
    /* (7) Init system communication error register                           */
    /*------------------------------------------------------------------------*/
    CCsys_SysCtrl_ST.FlagReg_AST[t_Channel_UB].SysComRegister_UW = 0x0000;
    CCsys_SysCtrl_ST.FlagReg_AST[t_Channel_UB].ErrorRegister_UB  = 0x00;
  } /* -1- END OF for(..) ----------------------------------------------------*/

  return;

} /* -+- END OF FUNCTION 'FUN_CCsys_InitOperatingSystem_V' -+-+-+-+-+-+-+-+-+-*/




/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  CCsys_OperatingSystem_V                                     |
|                                                                              |
|  Prototype at:   ADLATUS_System_tdf.h                                        |
|                                                                              |
|  Description:    Operating System of the SMART ADLATUS.                      |
|                                                                              |
|  Parameter:       --                                                         |
|                                                                              |
|  Return Value:    --                                                         |
|                                                                              |
+-----------------------------------------------------------------------------*/
void FUN_CCsys_OperatingSystem_V (void)
{

  UBYTE t_Result_UB;         /* Temp variable for current function call result*/
  UBYTE t_Direction_UB;      /* Temp variable for current direction (Rx/Tx)   */
  UBYTE t_ActScanLayer_UB;   /* Temp variable for current layer in process    */
  UBYTE t_ScanStartLayer_UB; /* Temp variable for scan start layer info       */
  UBYTE t_ScanStopLayer_UB;  /* Temp variable for scan stop layer info        */
  UBYTE t_ScanState_UB;      /* Temp variable for the return value of the     */
                             /* ResultEvaluation-routine                      */
  UBYTE t_Channel_UB;        /* Temp variable for channel scanning            */


  /*--------------------------------------------------------------------------*/
  /* Init local system variables                                              */
  /*--------------------------------------------------------------------------*/
  t_Direction_UB              = (UBYTE) 0x01;

  /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
  /*                                                                          */
  /* -1- Starting endless loop                                                */
  /*                                                                          */
  /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
  while (1)
  {
    /*========================================================================*/
    /* -2- Check one channel each time                                        */
    /*                                                                        */
    /*     NOTE: At the start of the system are equal from priority's point   */
    /*           of view. If one channel will be selected, the other channel  */
    /*           must be ignored until a RESET is done.                       */
    /*                                                                        */
    /*========================================================================*/
    for (t_Channel_UB = 0x00;                         \
         t_Channel_UB < d_CCconsys_NbrOfLogChannel;   \
         t_Channel_UB ++ )                            \
    {

      /*======================================================================*/
      /* -3- Check each direction of a channel                                */
      /*======================================================================*/
      for (t_Direction_UB =  d_CCsys_RxPath;
           t_Direction_UB <= d_CCsys_TxPath;
           t_Direction_UB ++ )
      {
        /*====================================================================*/
        /* -4- Direction: RxPath                                              */
        /*====================================================================*/
        if ( d_CCsys_RxPath == t_Direction_UB )
        {
          /*------------------------------------------------------------------*/
          /* Set StartLayer info                                              */
          /*------------------------------------------------------------------*/
          t_ScanStartLayer_UB = d_CCsys_RxPhysicalLayer;

          /*------------------------------------------------------------------*/
          /* Set StopLayer info                                               */
          /* NOTE: The application call is only possible if the TxPath is idle*/
          /*------------------------------------------------------------------*/

          /*==================================================================*/
          /* -5- TxPath is busy                                               */
          /*==================================================================*/
          if ( (CCsys_SysCtrl_ST.FlagReg_AST[t_Channel_UB].SysComRegister_UW & \
                d_CCsys_SetFlagTxPathBusy) != 0x0000u)
          {
            /* Scan only from physical to session layer without application   */
            t_ScanStopLayer_UB  = d_CCsys_RxSessionLayer;

          } /* -5- END OF if(..) ---------------------------------------------*/

          /*==================================================================*/
          /* -5- TxPath is idle                                               */
          /*==================================================================*/
          else
          {
            /* Scan from physical to application layer                        */
            t_ScanStopLayer_UB  = d_CCsys_RxApplicationLayer;
          } /* -5- END OF else (if(..)) --------------------------------------*/
        } /* ---4- END OF if(..) ---------------------------------------------*/
   
        /*====================================================================*/
        /* -4- Direction: TxPath                                              */
        /*====================================================================*/
        else
        {
          /* Scan from physical to session layer                              */
          t_ScanStartLayer_UB = d_CCsys_TxPhysicalLayer;
          t_ScanStopLayer_UB  = d_CCsys_TxSessionLayer;

        } /* -4- END OF else (if(..)) ----------------------------------------*/


        /*====================================================================*/
        /* -4- Scan each layer of the current direction                       */
        /*====================================================================*/
        for ( t_ActScanLayer_UB =  t_ScanStartLayer_UB;
              t_ActScanLayer_UB <= t_ScanStopLayer_UB;
              t_ActScanLayer_UB += 0x02 )
        {

          /*-------------------------------------------------------------------+
          |                                                                    |
          | S Y S T E M   T I M E R                                            |
          |                                                                    |
          +--------------------------------------------------------------------+
          | Check the HW-Timer                                                 |
          +-------------------------------------------------------------------*/
          FUN_CCtim_SystemTimerCheck_V ();

          /*------------------------------------------------------------------*/
          /* Check the communication interface HW access layer function       */
          /*------------------------------------------------------------------*/
          t_Result_UB = CCsys_SysCtrl_ST.ComHalFkt_PST[t_Channel_UB].ComHALFkt_PFUN ();

          /*==================================================================*/
          /* -5- Data from physical layer received                            */
          /*==================================================================*/
          if ( d_CCglo_Ready == t_Result_UB )
          {
            /*----------------------------------------------------------------*/
            /* Here is a good place to get a timer value for the random value */
            /* creation                                                       */
            /*----------------------------------------------------------------*/
            FUN_CChal_StoreHwTimerValue_V ();

          } /* -5- END OF if(..) ---------------------------------------------*/

          /*------------------------------------------------------------------*/
          /* Buffer router                                                    */
          /*------------------------------------------------------------------*/
          FUN_CCsys_BufferRouter_V ( t_Channel_UB, t_ActScanLayer_UB );

          /*------------------------------------------------------------------*/
          /* Reset the Ready-Flag from function before                        */
          /*------------------------------------------------------------------*/
          CCsys_SysCtrl_ST.FlagReg_AST[t_Channel_UB].SysComRegister_UW &= c_CCsys_ReadyFlags_AUW[t_Channel_UB][t_ActScanLayer_UB];

          /*------------------------------------------------------------------*/
          /* Call the communication function                                  */
          /*------------------------------------------------------------------*/
          t_Result_UB = CCsys_SysCtrl_ST.ComStackFunctions_PST[t_Channel_UB].Layer_PFUN[t_ActScanLayer_UB] \
                             ( CCsys_SysCtrl_ST.InBuffer_PBUF, \
                               CCsys_SysCtrl_ST.OutBuffer_PBUF );

          /*------------------------------------------------------------------*/
          /* Result evaluation                                                */
          /*------------------------------------------------------------------*/
          t_ScanState_UB =  FUN_CCsys_ResultEvaluation_UB ( t_Channel_UB,      \
                                                            t_ActScanLayer_UB, \
                                                            t_Result_UB        );

          /*==================================================================*/
          /* -5- Stop criterion occured                                       */
          /*==================================================================*/
          if ( d_CCsys_StopScan == (t_ScanState_UB & d_CCsys_StopScan))
          {
            t_ActScanLayer_UB = (t_ScanStopLayer_UB + 1);

          } /* -5- END OF if (..) --------------------------------------------*/
        } /* ---4- END OF FOR (..) -------------------------------------------*/

        /*====================================================================*/
        /* -4- Direction: TxPath                                              */
        /*     ATTENTION: Don't change this sequence!!                        */
        /*====================================================================*/
        if ( d_CCsys_TxPath == t_Direction_UB)
        {
          /*==================================================================*/
          /* -5- Communication is idle                                        */
          /*==================================================================*/
          if ( 0x0000 == (CCsys_SysCtrl_ST.FlagReg_AST[t_Channel_UB].SysComRegister_UW & \
                d_CCsys_SetFlagCommunicationBusy) )
          {

            /*----------------------------------------------------------------*/
            /* Call the channel specific event handler                        */
            /*----------------------------------------------------------------*/
            CCsys_SysCtrl_ST.EventProcessingFkt_PFUN[t_Channel_UB] ();

          } /* -5- END OF if(..) ---------------------------------------------*/
        } /* ---4- END OF if(..) ---------------------------------------------*/
      } /* -----3- END OF for (..) -------------------------------------------*/
    } /* -------2- END OF for (..) -------------------------------------------*/
  } /* ---------1- END OF while (1) ------------------------------------------*/

} /*-+- END OF FUNCTION 'CCsys_OperatingSystem_V' -+-+-+-+-+-+-+-+-+-+-+-+-+-+*/






/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CCsys_BufferRouter_V                                    |
|                                                                              |
|  Prototype at:   ADLATUS_System.c                                            |
|                                                                              |
|  Description:    This function configurate the buffer structure before a     |
|                  function is called. It depends on the layer information.    |
|                                                                              |
|  Parameter:      UBYTE t_ChannelNbr_UB                                       |
|                  UBYTE t_LayerInfo_UB                                        |
|                                                                              |
|  Return Value:   -                                                           |
|                                                                              |
+-----------------------------------------------------------------------------*/
void FUN_CCsys_BufferRouter_V ( UBYTE t_Channel_UB, UBYTE t_LayerInfo_UB )
{
  UBYTE t_ConfigTableLine_UB;
   
  /*------------------------------------------------------------------------*/
  /* Init Config-Table Line for inputbuffer selection                       */
  /*------------------------------------------------------------------------*/
  t_ConfigTableLine_UB = \
    c_CCsys_BufferAssignmentTable_AUB[t_LayerInfo_UB][d_CCsys_InputBufferLine]; 

  /*------------------------------------------------------------------------*/
  /* Set the Input buffer                                                   */
  /*------------------------------------------------------------------------*/
  CCsys_SysCtrl_ST.InBuffer_PBUF = \
    CCsys_SysCtrl_ST.SysBuffer_PST[t_Channel_UB].Line_PBUF[t_ConfigTableLine_UB];

  /*------------------------------------------------------------------------*/
  /* Init Config-Table Line for outputbuffer selection                      */
  /*------------------------------------------------------------------------*/
  t_ConfigTableLine_UB = \
    c_CCsys_BufferAssignmentTable_AUB[t_LayerInfo_UB][d_CCsys_OutputBufferLine]; 

  /*------------------------------------------------------------------------*/
  /* Set the Output buffer                                                  */
  /*------------------------------------------------------------------------*/
  CCsys_SysCtrl_ST.OutBuffer_PBUF = \
    CCsys_SysCtrl_ST.SysBuffer_PST[t_Channel_UB].Line_PBUF[t_ConfigTableLine_UB];

  return;

} /* END OF FUNCTION 'FUN_CCsys_BufferRouter_V' -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/


/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CCsys_ResultEvaluation_V                                |
|                                                                              |
|  Prototype at:   ADLATUS_System.c                                            |
|                                                                              |
|  Description:    This function evaluates the result of a function call       |
|                  of a communication stack routine. The correspondig flags    |
|                  will be set within the communication flag register via the  |
|                  SysComHandler.                                              |
|                                                                              |
|  Parameter:      UBYTE t_Result_UB    .. Result which should be evaluated    |
|                  UWORD t_LayerInfo_UW .. Info about the layer which has      |
|                                          produced the result.                |
|                  UBYTE t_Channel_UB   .. Channel number                      |
|                                                                              |
|  Return Value:   UBYTE t_ScanState_UB .. 0x00 Don't StopScan                 |
|                                       .. 0x01 StopScan                       |
|                                                                              |
+-----------------------------------------------------------------------------*/
UBYTE  FUN_CCsys_ResultEvaluation_UB ( UBYTE t_Channel_UB,   \
                                       UBYTE t_LayerInfo_UB, \
                                       UBYTE t_Result_UB     )
{
  UBYTE  t_ScanState_UB;   /* return value of the routine                     */
  UBYTE  t_Counter_UB;     /* runtime variable for channel check              */
  UWORD  t_ReadyFlag_UW;   /* variable for READY flag shifting                */
  UWORD  t_BusyFlag_UW;    /* variable for BUSY flag shifting                 */

  /*--------------------------------------------------------------------------*/
  /* Init Handle for the SysComHandler function                               */
  /*--------------------------------------------------------------------------*/
  t_ScanState_UB = d_CCsys_InitScanState;

  /*==========================================================================*/
  /* -1- Execute only if something is to do                                   */
  /*==========================================================================*/
  if ( (d_CCglo_Idle != t_Result_UB) ||
       (d_CCglo_Idle != CCsys_SysCtrl_ST.FlagReg_AST[t_Channel_UB].SysComRegister_UW))
  {
    t_ReadyFlag_UW = 0x0001;
    t_BusyFlag_UW  = 0x0002;

    /* -----------------------------------------------------------------------*/
    /*      Layer Information:                                                */
    /*                                                                        */
    /*      +--------------------+--------------------+                       */
    /*      | 06 - TxApplication |                    |                       */
    /*      +--------------------+--------------------+                       */
    /*      | 04 - RxSession     | 05 - TxSession     |                       */
    /*      +--------------------+--------------------+                       */
    /*      | 02 - RxTP          | 03 - TxTP          |                       */
    /*      +--------------------+--------------------+                       */
    /*      | 00 - RxPhysical    | 01 - TxPhysical    |                       */
    /*      +--------------------+--------------------+                       */
    /*                                                                        */
    /*              Rx Path             Tx Path                               */
    /*                                                                        */
    /* -----------------------------------------------------------------------*/
    /*      SysComRegister_UW:                                                */
    /*                                                                        */
    /*      F E D C  B A 9 8  7 6 5 4  3 2 1 0                                */
    /*      | | | |  | | | |  | | | |  | | | |                                */
    /*      | | | |  | | | |  | | | |  | | | +-- Rx Physical    Ready         */
    /*      | | | |  | | | |  | | | |  | | +---- Rx Physical    Busy          */
    /*      | | | |  | | | |  | | | |  | +------ Tx Physical    Ready         */
    /*      | | | |  | | | |  | | | |  +-------- Tx Physical    Busy          */
    /*      | | | |  | | | |  | | | +----------- Rx Transport   Ready         */
    /*      | | | |  | | | |  | | +------------- Rx Transport   Busy          */
    /*      | | | |  | | | |  | +--------------- Tx Transport   Ready         */
    /*      | | | |  | | | |  +----------------- Tx Transport   Busy          */
    /*      | | | |  | | | +-------------------- Rx Session     Ready         */
    /*      | | | |  | | +---------------------- Rx Session     Busy          */
    /*      | | | |  | +------------------------ Tx Session     Ready         */
    /*      | | | |  +-------------------------- Tx Session     Busy          */
    /*      | | | +----------------------------- Rx Application Ready         */
    /*      | | +------------------------------- Rx Application Busy          */
    /*      +-+--------------------------------- not used                     */
    /*========================================================================*/
    /* -2- Shift theflags in order to the layer info                          */
    /*========================================================================*/
    for ( t_Counter_UB = 0x00;
          t_Counter_UB < t_LayerInfo_UB;
          t_Counter_UB ++)
    {
      t_ReadyFlag_UW <<= 0x02u;
      t_BusyFlag_UW  <<= 0x02u;

    } /* -2- END OF for(..) --------------------------------------------------*/


    /*------------------------------------------------------------------------*/
    /*                                                                        */
    /*  S E T   T H E   F L A G S                                             */
    /*                                                                        */
    /*------------------------------------------------------------------------*/

    /*========================================================================*/
    /* -2- READY Flag is set                                                  */
    /*========================================================================*/
    if ( d_CCglo_Ready == (t_Result_UB & d_CCglo_Ready))  
    {
      /* Set corresponding READY Flag of the layer                            */
      CCsys_SysCtrl_ST.FlagReg_AST[t_Channel_UB].SysComRegister_UW |= t_ReadyFlag_UW;
    } /* -2- END OF if(..) ---------------------------------------------------*/

    /*========================================================================*/
    /* -1- READY Flag is not set                                              */
    /*========================================================================*/
    else
    {
      /* Reset corresponding READY Flag of the layer                            */
      CCsys_SysCtrl_ST.FlagReg_AST[t_Channel_UB].SysComRegister_UW &= (UWORD) ~t_ReadyFlag_UW;
    } /* -1- END OF else (if(..)) --------------------------------------------*/

    /*========================================================================*/
    /* -1- BUSY Flag is set                                                   */
    /*========================================================================*/
    if ( d_CCglo_Busy == (t_Result_UB & d_CCglo_Busy))  
    {
      /* Set corresponding BUSY Flag of the layer                             */
      CCsys_SysCtrl_ST.FlagReg_AST[t_Channel_UB].SysComRegister_UW |= t_BusyFlag_UW;
    } /* -1- END OF if(..) ---------------------------------------------------*/

    /*========================================================================*/
    /* -1- BUSY Flag is not set                                               */
    /*========================================================================*/
    else
    {
      /* Reset corresponding BUSY Flag of the layer                             */
      CCsys_SysCtrl_ST.FlagReg_AST[t_Channel_UB].SysComRegister_UW &= (UWORD) ~t_BusyFlag_UW;
    } /* -1- END OF else (if(..)) --------------------------------------------*/

  } /* ---1- END OF if(..) ---------------------------------------------------*/

  /*--------------------------------------------------------------------------*/
  /*                                                                          */
  /*  C H E C K   T H E   S T O P   C R I T E R I O N                         */
  /*                                                                          */
  /*--------------------------------------------------------------------------*/

  /*==========================================================================*/
  /* -1- RxPath                                                               */
  /*==========================================================================*/
  if ((t_LayerInfo_UB & d_CCsys_DirectionMask) == d_CCsys_RxPath)
  {
    /*========================================================================*/
    /* -2- Stop Scan                                                          */
    /*========================================================================*/
    if ( (t_Result_UB & d_CCglo_Busy) || (t_Result_UB & d_CCglo_Error) ) 
    {
      /* Set Flag                                                             */
      t_ScanState_UB |= d_CCsys_StopScan;

    } /* -2- END OF if (..) --------------------------------------------------*/
  } /* ---1- END OF if (..) --------------------------------------------------*/

  /*==========================================================================*/
  /* -1- TxPath                                                               */
  /*==========================================================================*/
  else
  {
    /*========================================================================*/
    /* -2- Stop Scan                                                          */
    /*========================================================================*/
    if ( d_CCglo_Idle != (t_Result_UB & d_CCglo_ResultInfoMask)) 
    {
      /* Set Flag                                                             */
      t_ScanState_UB |= d_CCsys_StopScan;

    } /* -2- END OF if (..) --------------------------------------------------*/
  } /* ---1- END OF else (if (..)) -------------------------------------------*/

  return ( t_ScanState_UB );

} /* END OF FUNCTION 'FUN_CCsys_ResultEvaluation_UB' -+-+-+-+-+-+-+-+-+-+-+-+-*/


/* ---------------------------------------------------------------------------*/
#endif /*                                                                     */
/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
/*                                                                            */
/*      E N D   O F   M  U L T I   C H A N N E L   S Y S T E M                */
/*                                                                            */
/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */


/* ========================================================================== */
/*      E N D   O F   F I L E                                                 */
/* ========================================================================== */
