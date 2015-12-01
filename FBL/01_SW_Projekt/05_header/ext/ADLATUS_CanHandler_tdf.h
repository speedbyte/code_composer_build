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
|  Filename:  ADLATUS_CanHandler_tdf.h                                         |
|                                                                              |
|  Comment:   This file includes all defines of the                            |
|             CAN handler for the SMART ADLATUS                                |
|                                                                              |
+------------------------------------------------------------------------------+
| R E V I S I O N   H I S T O R Y                                              |
+------------------------------------------------------------------------------+
|
| Subversion Info:
| $Id: ADLATUS_CanHandler_tdf.h 4396 2008-11-04 09:26:32Z maennel $
| $HeadURL: http://smartsvn/svn/adlatusrepository/auslieferungen/S1NN/086350-S1NN-ADLATUS-TMS470R-MucSuc/20081201_S1NN_TMS470R_BlfIso-Source_V1.00/01_SW_Projekt/05_header/ext/ADLATUS_CanHandler_tdf.h $
|
+------------------------------------------------------------------------------+
|      (c) SMART ELECTRONIC DEVELOPMENT GMBH - all rights reserved             |
+-----------------------------------------------------------------------------*/

/* ========================================================================== */
/*      O P E N   M U L T I P L E   I N C L U D E   P R O T E C T I O N       */
/* ========================================================================== */
#ifndef __TDF_CANHANDLER_
#define __TDF_CANHANDLER_
/* ========================================================================== */

/* ========================================================================== */
/*                                                                            */
/*      T Y P E D E F                                                         */
/*                                                                            */
/* ========================================================================== */
#ifdef cs_CCTP20
  /*----------------------------------------------------------------------------*/
  /* CAN Control                                                                */
  /*----------------------------------------------------------------------------*/
  typedef struct _CANCONTROL
  {
    CANMSGOBJCONFIG_ST* CanMsgObjConfig_PST;    /*  4 Byte - Pointer to structure    */
    const CANMSGOBJINFO_ST*   CanChannelTable_PST;    /*  4 Byte - pointer to config table */

    ULONG               QuarzClockFrequency_UL; /*  2 Byte - Pointer to constant ULONG */

    UWORD               CanNode_UW;             /*  2 Byte - Pointer to const UWORD */
    UBYTE               DefaultBaudrate_UB;     /*  1 Byte - Pointer to const UBYTE */
    UBYTE               SelectedChannel_UB;     /*  1 Byte - Selected Channel for communication */

    UBYTE               MbUsedinRAM_UB;

    UBYTE               RepeatCounter_UB;

    UBYTE               WriteIndex_AUB;
    UBYTE               ReadIndex_AUB;
  
  
    /* vvvvvv TP20 specific stuff vvvvvv */
    UNION2              SendID_UW;                /* TP20 Send ID */
    UNION2              ReceiveID_UW;             /* TP20 Rcv ID  */
    UWORD               CanTimer_UW;              /* Timer value for TP20 */
    UBYTE               CCTP20_Subset_UB; 
    UBYTE               CCTP20_Status_UB;
    UBYTE               CanTimerControl_UB;
    UBYTE               CanErrorCounter_UB;

                                            /*  x * 2 Byte - read & write  Index */
                                              /* ----------------------------- */
  } CANCONTROL_ST;                              /* 4 + (2 * x) Byte              */

#elif defined(cs_CCISO)
typedef struct _CANCONTROL
{
  CANMSGOBJCONFIG_ST* CanMsgObjConfig_PST;    /*  4 Byte - Pointer to structure    */
      const CANMSGOBJINFO_ST*   CanChannelTable_PST;    /*  4 Byte - pointer to config table */

  ULONG               QuarzClockFrequency_UL; /*  2 Byte - Pointer to constant ULONG */

  UWORD               CanNode_UW;             /*  2 Byte - Pointer to const UWORD */
  UBYTE               DefaultBaudrate_UB;     /*  1 Byte - Pointer to const UBYTE */
  UBYTE               SelectedChannel_UB;     /*  1 Byte - Selected Channel for communication */

  UBYTE               MbUsedinRAM_UB;
  UBYTE               RepeatCounter_UB;

  UBYTE*              SendData_PUB;
  
    UBYTE               MbUsedforStartup_UB;

  #if defined (cs_CCUDS_Audi)  
    DIAGRESPONSETG_ST*  DiagResponseTG_PST; 
  #endif
    
  UBYTE               WriteIndex_AUB;
  UBYTE               ReadIndex_AUB;
                                              
} CANCONTROL_ST; 
#else
  #error (undefined transport layer) 
#endif

/* ========================================================================== */
/*                                                                            */
/*      E X T E R N   D E C L A R A T I O N S                                 */
/*                                                                            */
/* ========================================================================== */
/* No ENTRY                                                                   */

extern CANCONTROL_ST CanCtrl_ST;
/* ========================================================================== */
/*                                                                            */
/*      G L O B A L   F U N C T I O N   P R O T O T Y P E S                   */
/*                                                                            */
/* ========================================================================== */
UBYTE FUN_CCcan_CheckMsgObjBuffer_UB   ( void                             );

void  FUN_CCcan_InitCan_V              (const CANINTERFACE_ST* t_Interface_PST  );
void  FUN_CCcan_InitCanBaudrate_V      ( UBYTE t_BaudrateIndex_UB         );
void  FUN_CCcan_InitCanController_V    ( void                             );
void  FUN_CCcan_InitCanMsgObj_V        ( CANMSGOBJCONFIG_ST *CanMsgObjConfig_PST, \
                                         UWORD              t_CanNode_UW          );

UBYTE FUN_CCcan_CheckCanReceiver_UB    ( void                             );
UBYTE FUN_CCcan_RxData_UB              ( PTR_BUFFER_UB t_InBuffer_PBUF,   \
                                         PTR_BUFFER_UB t_OutBuffer_PBUF   );
UBYTE FUN_CCcan_TxData_UB              ( PTR_BUFFER_UB t_InBuffer_PBUF,   \
                                         PTR_BUFFER_UB t_OutBuffer_PBUF   );

void SetMsgObjectList                  ( UBYTE MsgObjNbr_UB);  

void FUN_CCCan_Timer_V                 ( UWORD t_TimerTicks_UW );

void FUN_CCcan_TxDataFromRam_V(void);

/* UDS specific stuff */
void FUN_CCcan_TxDiagResponse_V(UBYTE t_Handle_UB);

/* TP20 specific stuff */
void FUN_CCcanInitTP20Data_V (void);

void  FUN_CCcan_DeactivateCanMsgObj_V(UBYTE t_MsgObjNbr_UB);
/* ========================================================================== */
/*      C L O S E   M U L T I P L E   I N C L U D E   P R O T E C T I O N     */
/* ========================================================================== */
#endif
/* ========================================================================== */

/* ========================================================================== */
/*      E N D   O F   F I L E                                                 */
/* ========================================================================== */
