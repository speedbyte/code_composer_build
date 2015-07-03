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
|  Filename:  ADLATUS_CanHandler.c                                             |
|                                                                              |
|  Processor:  TMS470                                                          |
|  Compiler:   TMS470 C/C++ Compiler             Version 2.24                  |
|                                                                              |
+------------------------------------------------------------------------------+
| A U T H O R   I D E N T I T Y                                                |
+------------------------------------------------------------------------------+
|                                                                              |
| Initials     Name                      Company                               |
| --------     ---------------------     ----------------------------------    |
|   RS         Ralf Schmidgall           SMART Electronic Development GmbH     |
|   MB         Michael Becker            SMART Electronic Development GmbH     |
|                                                                              |
+------------------------------------------------------------------------------+
| R E V I S I O N   H I S T O R Y                                              |
+------------------------------------------------------------------------------+
|
| Subversion Info:
| $Id: ADLATUS_CanHandler.c 4508 2008-11-28 12:28:20Z maennel $
| $HeadURL: http://smartsvn/svn/adlatusrepository/auslieferungen/S1NN/086350-S1NN-ADLATUS-TMS470R-MucSuc/20081201_S1NN_TMS470R_BlfIso-Source_V1.00/01_SW_Projekt/06_hallib/02_src/ADLATUS_CanHandler.c $
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
#include "ADLATUS_Canhandler_cdf.h"
#include "ADLATUS_ConfigSystem_cdf.h"
#ifdef cs_CCTP20
  #include "ADLATUS_EventHandlerAudi_cdf.h"
#endif
#include "ADLATUS_ValHandler_cdf.h"


/* ========================================================================== */
/* (2)  I N C L U D E S                                                       */
/* -------------------------------------------------------------------------- */
/*      T Y P E   D E F I N I T I O N   F I L E S  (TDF)                      */
/* ========================================================================== */
#include "ADLATUS_Types_tdf.h"                 /* must be the first include   */
#include "ADLATUS_Canhandler_tdf.h"
#include "ADLATUS_HAL_tdf.h"
#include "tms470regs.h"
#ifdef cs_CCTP20
  #include "ADLATUS_EventHandlerAudi_tdf.h"
#endif

#include "ADLATUS_ConfigSystem_tdf.h"
#include "ADLATUS_timer_tdf.h"

/* ========================================================================== */
/* (3)  C O M P I L E R   S W I T C H   S E T T I N G S                       */
/* -------------------------------------------------------------------------- */
/*      O N L Y   F O R   T H I S   M O D U L                                 */
/* ========================================================================== */
/* Global Compiler switch settings are done at ADLATUS_CompilerSwitches_CDF.h */
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

/* ************************************************************************** */
/* ************************************************************************** */
/* **                                                                      ** */
/* **  C A N   C O N T R O L L E R    D E F I N E S                        ** */
/* **                                                                      ** */
/* ************************************************************************** */
/* ************************************************************************** */

/* -------------------------------------------------------------------------- */
/*      Max CAN Nodes                                                         */
/* -------------------------------------------------------------------------- */
#define  d_CCcan_MaxCanNodes (UBYTE) 0x01u /* TMS470 338/348 has one CAN      */

/* -------------------------------------------------------------------------- */
/*      Max Nbr of Message Objects                                            */
/* -------------------------------------------------------------------------- */
#define  d_CCcan_NbrOfMsgObjs (UBYTE) 0x0Fu      /* 16                        */

/* -------------------------------------------------------------------------- */
/* Identifier Mask                                                            */
/* -------------------------------------------------------------------------- */
#define  d_CCcan_StandardIdMask             0x000007FFul

/* -------------------------------------------------------------------------- */
/* Additional Identifier Byte Possition                                       */
/* -------------------------------------------------------------------------- */
#define  d_AdditionalIdPossition   (UBYTE) 0x00u /* Byte 0                    */
/* -------------------------------------------------------------------------- */
/*      Timer init                                                            */
/* -------------------------------------------------------------------------- */
#define  d_CCCan_TIMERELAPSED                     0x0000u



/* -------------------------------------------------------------------------- */
/* TP 2.0 defines                                                             */
/* -------------------------------------------------------------------------- */
#define d_CCcan_TP20ChnlOpngRequestReceived    (UBYTE) 0x01u
#define d_CCcan_TP20ChnlOpngResponseHasToSend  (UBYTE) 0x02u
#define d_CCcan_TP20ChnlOpngResponseWasSent    (UBYTE) 0x04u
#define d_CCcan_TP20ChnlEstablished            (UBYTE) 0x08u
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define d_CCcan_TP20ChannelSetupMask           (UBYTE) 0x0Fu
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#define d_CCcan_TimerDisabled                  (UBYTE) 0x00u
#define d_CCcan_TimerEnabled                   (UBYTE) 0x80u
#define d_CCcan_TimerNotElapsed                (UBYTE) 0x00u
#define d_CCcan_TimerElapsed                   (UBYTE) 0x01u


/* ========================================================================== */
/* (6)  L O C A L  T Y P E   D E F I N E S                                    */
/* -------------------------------------------------------------------------- */
/*      O N L Y   F O R   T H I S   M O D U L                                 */
/* ========================================================================== */


/* ========================================================================== */
/* (7)  L O C A L   V A R I A B L E S  / C O N S T A N T S                    */
/* -------------------------------------------------------------------------- */
/*      O N L Y   F O R   T H I S   M O D U L                                 */
/* ========================================================================== */

/*----------------------------------------------------------------------------*/
/*      C A N                                                                 */
/*      --------------------------------------------------------------------- */
/*      Test Pattern for Message Object buffer test                           */
/*----------------------------------------------------------------------------*/
const UBYTE c_CCcan_TestPattern_AUB[0x03u] = \
{
  0x5Au,
  0xA5u,
  0x00u
}; /*--- END OF TABLE --------------------------------------------------------*/

/* -------------------------------------------------------------------------- */
/*      C A N                                                                 */
/*      --------------------------------------------------------------------- */
/*      CAN Baudrate                                                          */
/*                                                                            */
/* -------------------------------------------------------------------------- */


/* eCAN Bit -timing configuration register (CANBTC) bit definitions */
                     
const ULONG c_CCcan_BaudrateTable_AUL[] = 
{
/* 1mpbs */    0x0000027eUL,
/* 500kpbs */  0x00020253UL, 
/* nd(500) */  0x00000000UL, 
/* 250kpbs */  0x00050253UL, 
/* 200kpbs */  0x00050273UL, 
/* 200kpbs */  0x00000000UL, 
/* 200kpbs */  0x00000000UL,
/* 200kpbs */  0x00000000UL,
/* 125kpbs */  0x000b0243UL,
/* 100kpbs */  0x000e0253UL,
/* 83,3kpbs */ 0x00110253UL
}; /*--- END OF Table --------------------------------------------------------*/

/* -------------------------------------------------------------------------- */
/*      C A N                                                                 */
/*      --------------------------------------------------------------------- */
/*      CAN TempBuffer                                                        */
/*                                                                            */
/* -------------------------------------------------------------------------- */
UBYTE t_CCcan_CanFifoBuffer_AUB[d_CCcan_NbrOfInternalCanBuffer][4 + 8];

/* -------------------------------------------------------------------------- */
/*      C A N                                                                 */
/*      --------------------------------------------------------------------- */
/*      CAN Control structure                                                 */
/*                                                                            */
/* -------------------------------------------------------------------------- */
CANCONTROL_ST CanCtrl_ST;

/* ========================================================================== */
/* (8)  L O C A L   F U N C T I O N   P R O T O T Y P E S                     */
/* -------------------------------------------------------------------------- */
/*      O N L Y   F O R   T H I S   M O D U L                                 */
/* ========================================================================== */
void  FUN_CCcan_StopCanController_V  (void);
void  FUN_CCcan_StartCanController_V (void);
void  FUN_CCcan_ActivateCanMsgObj_V  (UBYTE t_MsgObjNbr_UB);
void  FUN_CCcan_DeactivateCanMsgObj_V(UBYTE t_MsgObjNbr_UB);

void FUN_CCcan_Waitconf_V(void);
void FUN_CCcan_Waitconfx_V(void);
void FUN_CCcan_Wait_V(void);

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#ifdef cs_CCTP20
  UBYTE FUN_CCcan_RxDataTP20_UB (UBYTE t_channel_UB);
  UBYTE FUN_CCcan_TxDataTP20_UB (PTR_BUFFER_UB t_InBuffer_PBUF );
#endif
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

/*lint -e40 */

void FUN_CCcan_Waitconf_V(void)
{
   while(!(e_SCC_ST.CANES_UN.CANES_ST.CCE_B1))
   {
     ;
   }
    /* wait for confirm of config-req.  */
}
/****************************************************************************/
void FUN_CCcan_Waitconfx_V(void)
{
    /* wait for confirm of config-req.  */
   while((e_SCC_ST.CANES_UN.CANES_ST.CCE_B1 != 0u)) 
   {
     ;
   }

}
/****************************************************************************/
void FUN_CCcan_Wait_V(void)
{
   while (e_SCC_ST.CANTRS_UL != 0ul) 
   {
     ;
   }
}
/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CCcan_CheckMsgObjBuffer_UB                              |
|                                                                              |
|  Prototype at:   ADLATUS_CanHandler_tdf.h                                    |
|                                                                              |
|  Description:    This function checks each MsgObj buffer.                    |
|                                                                              |
|  Parameter:      -                                                           |
|                                                                              |
|  Return Value:   d_CCcan_MsgObjBufferCheckOK                                 |
|                  d_CCcan_MsgObjBufferCheckNOK                                |
|                                                                              |
+-----------------------------------------------------------------------------*/
UBYTE  FUN_CCcan_CheckMsgObjBuffer_UB (void)
{
  UBYTE            t_CheckResult_UB;
  UBYTE            t_CanNode_UB;
  UBYTE            t_CanMsgObj_UB;
  UBYTE            t_Index_UB;
  UBYTE            t_PatternIndex_UB;

  /*--------------------------------------------------------------------------*/
  /* Init local variables                                                     */
  /*--------------------------------------------------------------------------*/
  t_CheckResult_UB  = (d_CCVhnd_CheckOK | d_CCVhnd_Ready);

  /*--------------------------------------------------------------------------*/
  /* disable all mailboxes (otherwise they are not accessable)                */
  /*--------------------------------------------------------------------------*/
  e_SCC_ST.CANME_UL = 0x00000000UL;
    
  /*==========================================================================*/
  /* -1- Check each CAN Node                                                  */
  /*==========================================================================*/
  for( t_CanNode_UB = 0x00; t_CanNode_UB < d_CCcan_MaxCanNodes; t_CanNode_UB++ )
  {
    e_CAN_MB[t_CanNode_UB].MID_ST.l.CanMID_UL = 0x00000000ul;
    e_CAN_MB[t_CanNode_UB].MCF_ST.l.CanMCF_UL = 0x00000000ul; 

    /*========================================================================*/
    /* -2- Check each CAN Message Object of each node                         */
    /*========================================================================*/
    for( t_CanMsgObj_UB = 0x00u; t_CanMsgObj_UB < d_CCcan_NbrOfMsgObjs; t_CanMsgObj_UB++ )
    {  
      /*======================================================================*/
      /* -3- Check each Buffer                                                */
      /*======================================================================*/
      for (t_Index_UB = 0x00u; t_Index_UB < 0x08; t_Index_UB++ )
      {  
        /*====================================================================*/
        /* -4- Check each Buffer with a pattern                               */
        /*====================================================================*/
        for ( t_PatternIndex_UB = (UBYTE) 0x00u; \
              t_PatternIndex_UB <= (UBYTE) 0x02u; \
              t_PatternIndex_UB++ )
        {
          /*------------------------------------------------------------------*/
          /* Write pattern into Buffer cell                                   */
          /*------------------------------------------------------------------*/
          e_CAN_MB[t_CanMsgObj_UB].u.CanData_UB[t_Index_UB] = 
          c_CCcan_TestPattern_AUB[t_PatternIndex_UB];

          /*==================================================================*/
          /* -5- RAM cell and test pattern are not equal!!                    */
          /*==================================================================*/
          if ( e_CAN_MB[t_CanMsgObj_UB].u.CanData_UB[t_Index_UB] != 
               c_CCcan_TestPattern_AUB[t_PatternIndex_UB])

          {
            /*----------------------------------------------------------------*/
            /* Set Result value                                               */
            /*----------------------------------------------------------------*/
            t_CheckResult_UB = d_CCglo_RamCheckError;

          } /* -5- END OF if(..) ---------------------------------------------*/
        } /* ---4- END OF for (..) -------------------------------------------*/
      } /* -----3- END OF for (..) -------------------------------------------*/
    } /* -------2- END OF for (..) -------------------------------------------*/
  } /* ---------1- END OF for (..) -------------------------------------------*/

  return (t_CheckResult_UB);

} /* -+- END OF FUN_CCcan_CheckMsgObjBuffer_UL -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CCcan_InitCan_V                                         |
|                                                                              |
|  Prototype at:   ADLATUS_CanHandler_tdf.h                                    |
|                                                                              |
|  Description:    Init function for can message objects.                      |
|                                                                              |
|  Parameter:      CANMSGOBJINIT_ST* CanInit_PST .. Ptr to init structure      |
|                                                                              |
|  Return Value:   -                                                           |
|                                                                              |
+-----------------------------------------------------------------------------*/
void  FUN_CCcan_InitCan_V (const CANINTERFACE_ST* t_Interface_PST)
{
  /*--------------------------------------------------------------------------*/
  /* Init local CanControlStructure                                           */
  /*--------------------------------------------------------------------------*/
  CanCtrl_ST.CanMsgObjConfig_PST    = (CANMSGOBJCONFIG_ST*) t_Interface_PST->CanMsgObjConfig_PST;
  CanCtrl_ST.CanChannelTable_PST    = (CANMSGOBJINFO_ST*)   t_Interface_PST->CanChannelTable_PST;
  CanCtrl_ST.QuarzClockFrequency_UL =                      *t_Interface_PST->QuarzClockFrequency_PUL;
  CanCtrl_ST.CanNode_UW             =                      *t_Interface_PST->CanNode_PUW;
  CanCtrl_ST.DefaultBaudrate_UB     =  (UBYTE)             *t_Interface_PST->DefaultBaudrate_PUW;
  CanCtrl_ST.SelectedChannel_UB     =                       d_CCglo_AllChannels;

  CanCtrl_ST.RepeatCounter_UB       = d_CCcan_InitState;
  CanCtrl_ST.MbUsedinRAM_UB         = d_CCcan_NoMbforRamDefined;

#if defined (cs_CCTP20)


  CanCtrl_ST.CCTP20_Subset_UB = 0u; 
  CanCtrl_ST.CCTP20_Status_UB = 0u;

  FUN_CCcanInitTP20Data_V ();

#elif defined (cs_CCISO)
  #if defined (cs_CCUDS_Audi)  
  CanCtrl_ST.DiagResponseTG_PST     = (DIAGRESPONSETG_ST*)  t_Interface_PST->DiagResponseTG_PST;
  #endif
#else
  #error (undefined transport layer)
#endif 

  /*--------------------------------------------------------------------------*/
  /* Init CAN message object                                                  */
  /* NOTE: First Init the Can Controller and then init the baudrate           */
  /*--------------------------------------------------------------------------*/
  FUN_CCcan_InitCanController_V ();

  return;

} /* -+- END OF FUN_CCcan_InitCan_V -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/


/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CCcan_InitCanController_V                               |
|                                                                              |
|  Prototype at:   ADLATUS_CanHandler_tdf.h                                    |
|                                                                              |
|  Description:    Init function for can message objects.                      |
|                                                                              |
|  Parameter:      -                                                           |
|                                                                              |
|  Return Value:   -                                                           |
|                                                                              |
+-----------------------------------------------------------------------------*/
void  FUN_CCcan_InitCanController_V(void)
{
  UBYTE t_MsgObjCounter_UB;
  UBYTE t_BufferNbr_UB;

   /* wait for transmit ready (maybe there is something in the queue) */
   FUN_CCcan_Wait_V(); 
   
   /* Def. Tx-Pin of CAN  (maybe obsolete) */
   e_SCC_ST.CANTIOC_UN.CANTIOC_ST.TXFUNC_B1 = 1u;
   /* Def. Rx-Pin of CAN  (maybe obsolete) */
   e_SCC_ST.CANRIOC_UN.CANRIOC_ST.RXFUNC_B1 = 1u;

   /* set Change configuration request */ 
   e_SCC_ST.CANMC_UN.CANMC_ST.CCR_B1 = 1u;

   /* wait for confirm of config-req.  */
   FUN_CCcan_Waitconf_V(); 

   /* RESET CAN Register  */
   e_SCC_ST.CANMC_UN.CANMC_ST.SRES_B1 = 1u;

   /* Init BTC register */   
   e_SCC_ST.CANBTC_UL = c_CCcan_BaudrateTable_AUL[CanCtrl_ST.DefaultBaudrate_UB];

   /* write register direct --> saves space!  */
   e_SCC_ST.CANMC_UN.CANMC_UW  = 0x1080u; 
     
   /* disable to have access        */
   e_SCC_ST.CANMIM_UL = 0;

   /* Setup Acceptance Masks * All must observed (FullCan area) */

   e_SCC_ST.CANGAM_UL = 0x00000000ul;
   /* in this phase all mailboxes are disabled */
   e_SCC_ST.CANME_UL = 0x00000000u;
    
   /* direction for all mb are input */
   e_SCC_ST.CANMD_UL = 0xFFFFFFFFul; 

  /*--------------------------------------------------------------------------*/
  /*                                                                          */
  /* Init the activated Message Objects                                       */
  /*                                                                          */
  /*--------------------------------------------------------------------------*/
  t_MsgObjCounter_UB =  0x00;

  /*==========================================================================*/
  /* -1- Scan the whole configuration table                                   */
  /*==========================================================================*/
  while ( d_CCglo_NoEntry != CanCtrl_ST.CanMsgObjConfig_PST[t_MsgObjCounter_UB].MsgObjectNbr_UB  )
  {
    /* Init the specified message object                                      */
    FUN_CCcan_InitCanMsgObj_V( &CanCtrl_ST.CanMsgObjConfig_PST[t_MsgObjCounter_UB], \
                               (UWORD) 0u );
    t_MsgObjCounter_UB++;
  } /* -1- END OF while(..) --------------------------------------------------*/

  /*========================================================================*/
  /* -2- Init all Buffers/Channel                                           */
  /*========================================================================*/
  for (t_BufferNbr_UB = 0x00u; t_BufferNbr_UB < d_CCcan_NbrOfInternalCanBuffer; t_BufferNbr_UB++ )
  {
    /* Init Buffer State                                                    */
    t_CCcan_CanFifoBuffer_AUB[t_BufferNbr_UB][d_CCglo_BufferState] = 0x00u;
  } /* -2- END OF for(..) --------------------------------------------------*/

  /*------------------------------------------------------------------------*/
  /* Init Can Handler variables                                             */
  /*------------------------------------------------------------------------*/
  CanCtrl_ST.ReadIndex_AUB  = 0x00u;
  CanCtrl_ST.WriteIndex_AUB = 0x00u;

  /*------------------------------------------------------------------*/
  /* Reset the Channel Selection on CAN                               */
  /*------------------------------------------------------------------*/
  CanCtrl_ST.SelectedChannel_UB =  d_CCglo_AllChannels;

   /* Ready with configuration */
   e_SCC_ST.CANMC_UN.CANMC_ST.CCR_B1 = 0u;
   
   /* wait for confirm of config-req.  */
   FUN_CCcan_Waitconfx_V(); 
   
  return;
}  

/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CCcan_StopCanController_V                               |
|                                                                              |
|  Prototype at:   ADLATUS_CanHandler.c                                        |
|                                                                              |
|  Description:    Init function for can message objects.                      |
|                                                                              |
|  Parameter:      -                                                           |
|                                                                              |
|  Return Value:   -                                                           |
|                                                                              |
+-----------------------------------------------------------------------------*/
void  FUN_CCcan_StopCanController_V ( void )
{

  return;

} /*-+- END OF FUNCTION FUN_CCcan_StopCanController_V  +-+-+-+-+-+-+-+-+-+-+-+*/


/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CCcan_StartCanController_V                              |
|                                                                              |
|  Prototype at:   ADLATUS_CanHandler.c                                        |
|                                                                              |
|  Description:    Init function for can message objects.                      |
|                                                                              |
|  Parameter:      -                                                           |
|                                                                              |
|  Return Value:   -                                                           |
|                                                                              |
+-----------------------------------------------------------------------------*/
void  FUN_CCcan_StartCanController_V (void)
{
  return;

} /*-+- END OF FUNCTION FUN_CCcan_StartCanController_V  +-+-+-+-+-+-+-+-+-+-+-*/


/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CCcan_ActivateCanMsgObj_V                               |
|                                                                              |
|  Prototype at:   ADLATUS_CanHandler.c                                        |
|                                                                              |
|  Description:    This function activates a selected message object.          |
|                                                                              |
|  Parameter:      UWORD t_CanNode_UW   ... Selected CAN Node                  |
|                                                                              |
|  Return Value:   -                                                           |
|                                                                              |
+-----------------------------------------------------------------------------*/
void  FUN_CCcan_ActivateCanMsgObj_V( UBYTE t_MsgObjNbr_UB )
{
  
  CanCtrl_ST.CanMsgObjConfig_PST[t_MsgObjNbr_UB].MsgObjState_UB = d_CCcan_Activate;

  FUN_CCcan_InitCanMsgObj_V(&CanCtrl_ST.CanMsgObjConfig_PST[t_MsgObjNbr_UB], \
                               (UWORD) 0u );
  
 return;

} /*-+- END OF FUNCTION 'FUN_CCcan_ActivateCanMsgObj_V'  +-+-+-+-+-+-+-+-+-+-+*/





/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CCcan_DeactivateCanMsgObj_V                             |
|                                                                              |
|  Prototype at:   ADLATUS_CanHandler.c                                        |
|                                                                              |
|  Description:    This function deactivates a selected message object.        |
|                                                                              |
|  Parameter:      UWORD t_CanNode_UW   ... Selected CAN Node                  |
|                                                                              |
|  Return Value:   -                                                           |
|                                                                              |
+-----------------------------------------------------------------------------*/
void  FUN_CCcan_DeactivateCanMsgObj_V(UBYTE t_MsgObjNbr_UB )
{  
  CanCtrl_ST.CanMsgObjConfig_PST[t_MsgObjNbr_UB].MsgObjState_UB = d_CCcan_Deactivate;

  FUN_CCcan_InitCanMsgObj_V(&CanCtrl_ST.CanMsgObjConfig_PST[t_MsgObjNbr_UB], \
                               (UWORD) 0u );
                               
  return;

} /*-+- END OF FUNCTION 'FUN_CCcan_DeactivateCanMsgObj_V'  +-+-+-+-+-+-+-+-+-+*/




/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CCcan_InitCanMsgObj_V                                   |
|                                                                              |
|  Prototype at:   ADLATUS_CanHandler_tdf.h                                    |
|                                                                              |
|  Description:    Init function for can message objects.                      |
|                                                                              |
|  Parameter:      -                                                           |
|                                                                              |
|  Return Value:   -                                                           |
|                                                                              |
+-----------------------------------------------------------------------------*/
void  FUN_CCcan_InitCanMsgObj_V( CANMSGOBJCONFIG_ST *CanMsgObjConfig_PST, \
                                 UWORD t_CanNode_UW )
{

  UBYTE t_MsgObjNbr_UB;
  UBYTE t_MsgObjFlag_UB;

  /*--------------------------------------------------------------------------*/
  /* Get the Message Object number                                            */
  /*--------------------------------------------------------------------------*/
  t_MsgObjNbr_UB = CanMsgObjConfig_PST->MsgObjectNbr_UB;

  /* get flag position in 32 bit configuration register */
  t_MsgObjFlag_UB = 0x00000001UL << t_MsgObjNbr_UB;

  /* disable mailbox for accessing */
  e_SCC_ST.CANME_UL &= ((ULONG)(~t_MsgObjFlag_UB));  
 
   /* clear Message control register */
  e_CAN_MB[t_MsgObjNbr_UB].MCF_ST.l.CanMCF_UL = 0x00000008ul;
  e_CAN_MB[t_MsgObjNbr_UB].MID_ST.l.CanMID_UL = 0x00000000ul;
    
  if (d_CCcan_Transmit == CanMsgObjConfig_PST->MsgObjDirection_UB )
  {
      /* a zero flag means transmit mailbox */
      e_SCC_ST.CANMD_UL &= ((ULONG)(~t_MsgObjFlag_UB)); 
  }
  else
  {    
      /* configure mailbox for receiving */
      e_SCC_ST.CANMD_UL |= ((ULONG)(t_MsgObjFlag_UB)); 
  }
  
  /*========================================================================*/
  /* -1- CAN Identifier: 11 Bit                                             */
  /*========================================================================*/
  if (d_CCcan_11BitID == CanMsgObjConfig_PST->IDLength_UB )
  {    											  
    e_CAN_MB[t_MsgObjNbr_UB].MID_ST.l.CanMID_UL = ((CanMsgObjConfig_PST->CanIdentifier_UL << 18) & 0x1FFC0000UL);

  } /* -1- END OF if(..) ---------------------------------------------------*/

  /*========================================================================*/
  /* -1- CAN Identifier: 29 Bit                                             */
  /*========================================================================*/
  else
  {
    /* set identifier and IDE bit */
    e_CAN_MB[t_MsgObjNbr_UB].MID_ST.l.CanMID_UL = ((CanMsgObjConfig_PST->CanIdentifier_UL & 0x9FFFFFFFul) | 0x80000000ul);

  } /* -1- END OF else (if(..)) --------------------------------------------*/


  /*==========================================================================*/
  /* -1- Message Object is active                                             */
  /*= =========================================================================*/
  if ( d_CCcan_Activate == CanMsgObjConfig_PST->MsgObjState_UB )
  {
    /* activate mailbox                                             */
    e_SCC_ST.CANME_UL |= ((ULONG)(t_MsgObjFlag_UB));


  } /* ---1- END OF if(..) ---------------------------------------------------*/

  /*==========================================================================*/
  /* -1- Message Object is not active                                         */
  /*==========================================================================*/
  else
  {
    /* deactivate mailbox                                          */
      e_SCC_ST.CANME_UL &= ((ULONG)(~t_MsgObjFlag_UB));
  } /* -1- END OF else (if(..)) ----------------------------------------------*/

  return;

} /*-+- END OF FUNCTION 'FUN_CCcan_InitCanMsgObj_V'  +-+-+-+-+*/ /*l_int !e818 */

/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CCcan_CheckCanReceiver_UB                               |
|                                                                              |
|  Prototype at:   ADLATUS_CanHandler_tdh.h                                    |
|                                                                              |
|  Description:    HAL-Function CAN Receiver - Searching function for the      |
|                  correct message object which has data received.             |
|                                                                              |
|  Parameter:      -                                                           |
|                                                                              |
|  Return Value:   UBYTE  t_Result_UB                                          |
|                                                                              |
|                         R B E x  x x x x                                     |
|                         | | | |  | | | |                                     |
|                         | | | +--+-+-+-+---    Message Object Number         |
|                         | | +--------------    ERROR                         |
|                         | +----------------    BUSY                          |
|                         +------------------    READY                         |
|                                                                              |
+-----------------------------------------------------------------------------*/
UBYTE FUN_CCcan_CheckCanReceiver_UB (void)
{

  UBYTE t_Result_UB;              /* Result value of the function                */
  UBYTE t_State_UB;               /* temporary state of CheckCanReceiverFunction */

  UBYTE t_MsgObjNbr_UB;           /* temp MsgObjNbr for MsgObject evaluation     */
  UBYTE t_MsgObjFlag_UB;
  UBYTE t_TableLine_UB;           /* temp table line for MsgObject evaluation    */
  UBYTE t_Index_UB;               /* Index for copy routine                      */
  UBYTE t_WriteIndex_UB;          /* Index for the Fifo evaluation               */

  UBYTE t_Channel_UB;             /* temp channel number for MsgObject evaluation */
  UBYTE t_SubSet_UB;              /* temp Subset number for MsgObject evaluation */

  UBYTE t_ReceivedDataLength_UB;  /* temp data lengt from rx data msg object      */

  UBYTE t_TempBuffer[8];          /* Temp buffer vor data storrage during         */
                                  /* channel evaluation                           */

  UBYTE t_MsgObjCounter_UB;
  /*--------------------------------------------------------------------------*/
  /* Init                                                                     */
  /*--------------------------------------------------------------------------*/
  t_Result_UB             =  d_CCglo_Idle;
  t_MsgObjNbr_UB          = d_CCglo_InitToZero;
  t_State_UB              = d_CCglo_InitToZero;
  t_TableLine_UB          = d_CCglo_InitToZero;
  t_ReceivedDataLength_UB = d_CCglo_InitToZero;

  t_TempBuffer[0]         = d_CCglo_InitToZero;
  /*==========================================================================*/
  /* -1- New Data received in MsgObj x                                        */
  /*==========================================================================*/
  if(e_SCC_ST.CANRMP_UL != 0ul)
  {
    t_MsgObjCounter_UB =  d_CCglo_InitToZero;
    /*========================================================================*/
    /*                                                                        */
    /* -1- Scan all enabled Rx-MsgObjs for received Data                      */
    /*                                                                        */
    /*========================================================================*/
    while ( d_CCcan_StopScan != (t_State_UB & d_CCcan_StopScan) )
    {
      /*========================================================================*/
      /* -2- End of Can config table found                                      */
      /*========================================================================*/
      if ( d_CCglo_NoEntry == CanCtrl_ST.CanMsgObjConfig_PST[t_TableLine_UB].MsgObjDirection_UB ) 
      {                                  
        /* Stop Scanning                                                        */
        t_State_UB |= d_CCcan_StopScan;
        
      } /* -2- END OF if(..) ---------------------------------------------------*/
      
      /*========================================================================*/
      /* -2- Normal entry found                                                 */
      /*========================================================================*/
      else
      {
        /* set flag mask */
        t_MsgObjFlag_UB = 
          (0x00000001UL << CanCtrl_ST.CanMsgObjConfig_PST[t_MsgObjCounter_UB].MsgObjectNbr_UB );
        /*=================================================================*/
        /* valid entry found - check for activation an receiving           */        
        /* -3- check if mb is enabled and receiver                         */
        /*=================================================================*/
        if ((d_CCcan_Activate == CanCtrl_ST.CanMsgObjConfig_PST[t_MsgObjCounter_UB].MsgObjState_UB ) &&
          (d_CCcan_Receive ==   CanCtrl_ST.CanMsgObjConfig_PST[t_MsgObjCounter_UB].MsgObjDirection_UB ) &&
          ((e_SCC_ST.CANME_UL & (ULONG)t_MsgObjFlag_UB) == (ULONG)t_MsgObjFlag_UB) &&                 
          ((e_SCC_ST.CANRMP_UL & (ULONG)t_MsgObjFlag_UB) == (ULONG)t_MsgObjFlag_UB))
        {
          /*=================================================================*/
          /* -4- Message lost                                                */
          /*=================================================================*/
          if ((e_SCC_ST.CANRML_UL & (ULONG)t_MsgObjFlag_UB) == (ULONG)t_MsgObjFlag_UB) 
          {
            /* reset message lost flag */
            e_SCC_ST.CANRML_UL &= (ULONG)t_MsgObjFlag_UB; 
            
          } /* -4- END OF if(..) -----------------------------------------------*/
          
          /*------------------------------------------------------------------*/
          /* Get the received data length                                     */
          /*------------------------------------------------------------------*/
          t_ReceivedDataLength_UB = e_CAN_MB[t_MsgObjCounter_UB].MCF_ST.bw.CanMCF;
          
          /*==============================================================*/
          /* -7- Store the data only if the length is valid for CAN       */
          /*==============================================================*/
          if ((t_ReceivedDataLength_UB >  (UBYTE) 0x00u) && \
              (t_ReceivedDataLength_UB <= (UBYTE) 0x08u))
          {
            /* if the msg is valid store info */
            t_State_UB = d_CCcan_MsgReceived;
                    
            /*============================================================*/
            /* -8- Store the data temporary for evaluation                */
            /*============================================================*/
            for (t_Index_UB =  0x00u;
                 t_Index_UB <  t_ReceivedDataLength_UB;
                 t_Index_UB ++ )
            {
              /* Copy Data from CanControllerBuffer to TempBuffer         */
              t_TempBuffer[t_Index_UB] = \
                e_CAN_MB[t_MsgObjCounter_UB].u.CanData_UB[t_Index_UB];
              
              
            } /* -8- END OF FOR (..) -------------------------------------*/
          } /* ---7- END OF FOR (..) -------------------------------------*/

          /* regardless to a valid message stop scanning */
          t_State_UB |= d_CCcan_StopScan;
            
          /* store receiver mailbox number */
          t_MsgObjNbr_UB = t_MsgObjCounter_UB;

          /*------------------------------------------------------------------*/
          /* Reset flag for rx data identification                            */
          /*------------------------------------------------------------------*/
          e_SCC_ST.CANRMP_UL |= (ULONG)t_MsgObjFlag_UB;
        }  
      } 
      
      t_MsgObjCounter_UB++;
      
    } /* ---------2- END OF while (..) ---------------------------------------*/
    
    /*========================================================================*/
    /*                                                                        */
    /* -1- Data received                                                      */
    /*                                                                        */
    /*========================================================================*/
    if ((t_State_UB & d_CCcan_MsgReceived) == d_CCcan_MsgReceived)
    {
      /*------------------------------------------------------------------------*/
      /* Look for the Message Object / Channel assignment                       */
      /* start scanning                                                         */
      /*------------------------------------------------------------------------*/
      t_State_UB     &= (UBYTE) ~d_CCcan_StopScan;
      t_TableLine_UB  =  d_CCglo_InitToZero;
      
      /*========================================================================*/
      /* -2- Scan for the correct channel information                           */
      /*========================================================================*/
      while (d_CCcan_StopScan != (t_State_UB & d_CCcan_StopScan))
      {
        /*======================================================================*/
        /* -3- End Of table found                                               */
        /*======================================================================*/
        if ( d_CCglo_NoEntry == CanCtrl_ST.CanChannelTable_PST[t_TableLine_UB].MsgObjectNbr_UB ) 
        {
          /* Stop scanning                                                      */
          t_State_UB |= d_CCcan_StopScan;
        } /* -3- END OF if(..) -------------------------------------------------*/
        
        /*======================================================================*/
        /* -3- Normal entry found                                               */
        /*======================================================================*/
        else
        {
          /*====================================================================*/
          /* -4- Entry is not correct for received message object               */
          /*====================================================================*/
          if ( t_MsgObjNbr_UB != CanCtrl_ST.CanChannelTable_PST[t_TableLine_UB].MsgObjectNbr_UB ) 
          {
            /* increment Table Line                                             */
            t_TableLine_UB ++;
          } /* -4- END OF if(..) -----------------------------------------------*/
          
          /*====================================================================*/
          /* -4- Entry is correct for received message object                   */
          /*====================================================================*/
          else
          {
            /*==================================================================*/
            /* -5- No additional Identifier defined --> correct entry found     */
            /*                                      --> stop scanning           */
            /*==================================================================*/
            if ( d_CCcan_AddIdentifier != (CanCtrl_ST.CanChannelTable_PST[t_TableLine_UB].AddIdByteInfo_UB & d_CCcan_AddIdentifier)) 
            {
              /* stop scanning                                                  */
              t_State_UB |= (d_CCcan_StopScan | d_CCcan_CorrectMsgEntryFound);
            } /* -5- END OF if(..) ---------------------------------------------*/
            
            /*==================================================================*/
            /* -5- Additional Identifier defined                                */
            /*==================================================================*/
            else
            {
              /* Set the flag for additional ID at the state register           */
              t_State_UB |= d_CCcan_AddIdentifier;
              
              /*================================================================*/
              /* -6- Additional Identifier not correct                          */
              /*     --> scan next entry                                        */
              /*================================================================*/
              if ( t_TempBuffer[d_AdditionalIdPossition] != \
                CanCtrl_ST.CanChannelTable_PST[t_TableLine_UB].AddIdentifier_UB) 
              {
                /* increment Table Line                                         */
                t_TableLine_UB ++;
              }
              /*================================================================*/
              /* -6- Additional Identifier is correct --> correct entry found   */
              /*                                      --> stop scanning         */
              /*================================================================*/
              else
              {
                /* stop scanning                                                */
                t_State_UB |= ( d_CCcan_StopScan | \
                  d_CCcan_CorrectMsgEntryFound | \
                  d_CCcan_AddIdentifier       );
                
              } /* ---6- END OF else (if(..)) ----------------------------------*/
            } /* -----5- END OF else (if(..)) ----------------------------------*/
          } /* -------4- END OF else (if(..)) ----------------------------------*/
        } /* ---------3- END OF else (if(..)) ----------------------------------*/
      } /* -----------2- END OF while (..) -------------------------------------*/
      
      
      /*========================================================================*/
      /*                                                                        */
      /* -2- Correct table entry found                                          */
      /*                                                                        */
      /*========================================================================*/
      if (d_CCcan_CorrectMsgEntryFound == (t_State_UB & d_CCcan_CorrectMsgEntryFound))
      {
        /*--------------------------------------------------------------------*/
        /* Get the necessary data                                             */
        /*--------------------------------------------------------------------*/
        t_Channel_UB    = (CanCtrl_ST.CanChannelTable_PST[t_TableLine_UB].ChannelNbr_UB & d_CCglo_ChannelMask);
        t_SubSet_UB     = (CanCtrl_ST.CanChannelTable_PST[t_TableLine_UB].ChannelNbr_UB & d_CCglo_SubSetMask );
        t_WriteIndex_UB =  CanCtrl_ST.WriteIndex_AUB;
        
        /*----------------------------------------------------------------------*/
        /* Increment the WriteIndex                                             */
        /*----------------------------------------------------------------------*/
        CanCtrl_ST.WriteIndex_AUB++;
        CanCtrl_ST.WriteIndex_AUB &= (UBYTE) 0x03u;
        
        /*----------------------------------------------------------------------*/
        /* Store the received data into the correct Fifobuffer                  */
        /* (1) Set buffer valid                                                 */
        /*----------------------------------------------------------------------*/
        t_CCcan_CanFifoBuffer_AUB[t_WriteIndex_UB][d_CCglo_BufferState] |= \
          d_CCglo_BufferValid;
        /*----------------------------------------------------------------------*/
        /* (2) Store the channel Info                                           */
        /*----------------------------------------------------------------------*/
        t_CCcan_CanFifoBuffer_AUB[t_WriteIndex_UB][d_CCglo_ChannelNbr] = \
          CanCtrl_ST.CanChannelTable_PST[t_TableLine_UB].ChannelNbr_UB;
        /*----------------------------------------------------------------------*/
        /* (3) Store received data length                                       */
        /*----------------------------------------------------------------------*/
        t_CCcan_CanFifoBuffer_AUB[t_WriteIndex_UB][d_CCglo_DLC] = \
          t_ReceivedDataLength_UB;
        
        /*======================================================================*/
        /* -3- (4) Copy the received data from TempBuffer to Fifo Buffer        */
        /*======================================================================*/
        for (t_Index_UB =  0x00u;
        t_Index_UB <  t_ReceivedDataLength_UB;
        t_Index_UB ++ )
        {
          /* Copy Data from CanControllerBuffer to RxMsgBuffer                  */
          t_CCcan_CanFifoBuffer_AUB[t_WriteIndex_UB][d_CCglo_StartOfData + t_Index_UB] = \
            t_TempBuffer[t_Index_UB];
          
        } /* -3- END OF FOR (..) -----------------------------------------------*/
        
      t_Result_UB = (d_CCglo_Ready | t_Channel_UB);
        
        /*======================================================================*/
        /* -3- Currently no special channel selected for communication          */
        /*     --> Default value!!                                              */
        /*======================================================================*/
        if ( d_CCglo_AllChannels == CanCtrl_ST.SelectedChannel_UB )
        {
          /*--------------------------------------------------------------------*/
          /* Set this channel which has received the message as selected Channel*/
          /*--------------------------------------------------------------------*/
          CanCtrl_ST.SelectedChannel_UB = t_Channel_UB;
          
          /*--------------------------------------------------------------------*/
          /* Init all variable for MsgObject deactivation                       */
          /*--------------------------------------------------------------------*/
          t_State_UB     &= (UBYTE) ~d_CCcan_StopScan;
          t_TableLine_UB  = d_CCglo_InitToZero;
          
          /*====================================================================*/
          /* -4- Scan for the correct channel information                       */
          /*====================================================================*/
          while (d_CCcan_StopScan != (t_State_UB & d_CCcan_StopScan))
          {
            /*==================================================================*/
            /* -5- End Of table found                                           */
            /*==================================================================*/
            if ( d_CCglo_NoEntry == CanCtrl_ST.CanChannelTable_PST[t_TableLine_UB].MsgObjectNbr_UB ) 
            {
              /* Stop scanning                                                  */
              t_State_UB |= d_CCcan_StopScan;
              
            } /* -3- END OF if(..) ---------------------------------------------*/
            
            /*==================================================================*/
            /* -5- Normal entry found                                           */
            /*==================================================================*/
            else
            {
              /*================================================================*/
              /* -6- Entry is for the selected channel number                   */
              /*================================================================*/
              if (  CanCtrl_ST.SelectedChannel_UB == \
                (CanCtrl_ST.CanChannelTable_PST[t_TableLine_UB].ChannelNbr_UB & d_CCglo_ChannelMask) ) 
              {
                /* Check the next entry                                         */
                t_TableLine_UB ++;
                
              } /* -6- END OF if(..) -------------------------------------------*/
              
              /*================================================================*/
              /* -6- Entry is not for the selected channel number               */
              /*================================================================*/
              else
              {
                /*--------------------------------------------------------------*/
                /* Get the Message Object Number                                */
                /*--------------------------------------------------------------*/
                t_MsgObjNbr_UB = CanCtrl_ST.CanChannelTable_PST[t_TableLine_UB].MsgObjectNbr_UB;
                
                /*--------------------------------------------------------------*/
                /* Deactivate this Message Object                               */
                /*--------------------------------------------------------------*/
                /* FUN_CCcan_DeactivateCanMsgObj_V((UBYTE) t_MsgObjNbr_UB); */
                
                
                /* increment Table Line                                         */
                t_TableLine_UB ++;
                
              } /* -6- END OF else (if(..)) ------------------------------------*/
            } /* ---5- END OF else (if(..)) ------------------------------------*/
          } /* -----4- END OF while (..) ---------------------------------------*/
        } /* -------3- END OF If (..) ------------------------------------------*/
        

#if defined (cs_CCTP20)
        /************************************************************************/
        /*                                                                      */
        /* TP20 INTERFACE                                                       */
        /*                                                                      */
        /************************************************************************/
        
        t_Result_UB = FUN_CCcan_RxDataTP20_UB ( (UBYTE) (t_SubSet_UB | t_Channel_UB) );
        
        if (d_CCcan_MsgObjInvalid == t_Result_UB)
        {
          /* make buffer invalid */
          t_CCcan_CanFifoBuffer_AUB[t_WriteIndex_UB][d_CCglo_BufferState] &= \
            ~d_CCglo_BufferValid;
          /* no valid data received */
          t_Result_UB = d_CCglo_Idle;
        }
#endif
      } /* -2- END OF if (..) --------------------------------------------------*/
    } /* ---1- END OF if (..) --------------------------------------------------*/
  }

  return (t_Result_UB);

} /*-+- END OF FUNCTION 'FUN_CCcan_CheckCanReceiver_UB' -+-+-+-+-+-+-+-+-+-+-+*/ 


/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CCcan_RxData_UB                                         |
|                                                                              |
|  Prototype at:   ADLATUS_CanHandler_tdh.h                                    |
|                                                                              |
|  Description:    HAL-Function CAN Receiver                                   |
|                                                                              |
|  Parameter:      UBYTE         t_MsgObjNbr_UB   .. Rx Message Object         |
|                  PTR_BUFFER_UB t_OutBuffer_PBUF .. Rx Buffer                 |
|                                                                              |
|  Return Value:   UBYTE  t_Result_UB                                          |
|                                                                              |
|                         R B E x  x x x x                                     |
|                         | | | |  | | | |                                     |
|                         | | | +--+-+-+-+---    not used                      |
|                         | | +--------------    ERROR                         |
|                         | +----------------    BUSY                          |
|                         +------------------    READY                         |
|                                                                              |
+-----------------------------------------------------------------------------*/
UBYTE FUN_CCcan_RxData_UB (PTR_BUFFER_UB t_InBuffer_PBUF, PTR_BUFFER_UB t_OutBuffer_PBUF )
{

  UBYTE t_Result_UB;
  UBYTE t_Channel_UB;
  UBYTE t_ReadIndex_UB;
  UBYTE t_Index_UB;

  /* This construct is only in order to prevent a compiler warning            */
  t_InBuffer_PBUF += 0x00u; 

  /*--------------------------------------------------------------------------*/
  /* Init result                                                              */
  /*--------------------------------------------------------------------------*/
  t_Result_UB  = d_CCglo_Idle;
  
  /*==========================================================================*/
  /* -1- If OutBuffer is valid (means free                                    */
  /*==========================================================================*/
  if ( (t_OutBuffer_PBUF[d_CCglo_BufferState] & d_CCglo_BufferValid) != d_CCglo_BufferValid)
  {
    /* Init the temp information variables                                    */
    t_Channel_UB   = ( t_OutBuffer_PBUF[d_CCglo_ChannelNbr] & d_CCglo_ChannelMask);
    t_ReadIndex_UB = CanCtrl_ST.ReadIndex_AUB;

    /*========================================================================*/
    /* -2- FifoBuffer is valid --> data received                              */
    /*========================================================================*/
    if ( d_CCglo_BufferValid ==                            \
         (  t_CCcan_CanFifoBuffer_AUB[t_ReadIndex_UB     ] \
                                     [d_CCglo_BufferState] & d_CCglo_BufferValid ))
    {
      /*----------------------------------------------------------------------*/
      /* Copy the data from Can FifoBuffer to System RxBuffer                 */
      /* (1) OutBuffer is valid                                               */
      /*----------------------------------------------------------------------*/
      t_OutBuffer_PBUF[d_CCglo_BufferState] |= d_CCglo_BufferValid;
      /*----------------------------------------------------------------------*/
      /* (2) copy the cannel and prio info                                    */
      /*----------------------------------------------------------------------*/
      t_OutBuffer_PBUF[d_CCglo_ChannelNbr] = \
      t_CCcan_CanFifoBuffer_AUB[t_ReadIndex_UB][d_CCglo_ChannelNbr];
      /*----------------------------------------------------------------------*/
      /* (3) copy DLC                                                         */
      /*----------------------------------------------------------------------*/
      t_OutBuffer_PBUF[d_CCglo_DLC] = \
      t_CCcan_CanFifoBuffer_AUB[t_ReadIndex_UB][d_CCglo_DLC];
      /*======================================================================*/
      /* -3- (4) Copy the received data                                       */
      /*======================================================================*/
      for (t_Index_UB   =  0x00u;
           t_Index_UB <  t_OutBuffer_PBUF[d_CCglo_DLC];
           t_Index_UB   ++ )
      {
        /* Copy Data from CanControllerBuffer to RxMsgBuffer                  */
        t_OutBuffer_PBUF[d_CCglo_StartOfData + t_Index_UB] = \
        t_CCcan_CanFifoBuffer_AUB[t_ReadIndex_UB][d_CCglo_StartOfData + t_Index_UB];

      } /* -3- END OF FOR (..) -----------------------------------------------*/

      /*----------------------------------------------------------------------*/
      /* Set the fifobuffer invalid                                           */
      /*----------------------------------------------------------------------*/
      t_CCcan_CanFifoBuffer_AUB[t_ReadIndex_UB][d_CCglo_BufferState] &= \
                                                 (UBYTE) ~d_CCglo_BufferValid;
      /*----------------------------------------------------------------------*/
      /* Increment the read index                                             */
      /*----------------------------------------------------------------------*/
      CanCtrl_ST.ReadIndex_AUB++;
      CanCtrl_ST.ReadIndex_AUB &= (UBYTE) 0x03u;

      /*----------------------------------------------------------------------*/
      /* Set Result                                                           */
      /*----------------------------------------------------------------------*/
      t_Result_UB |= d_CCglo_Ready;

    } /* -2- END OF else (if(..)) --------------------------------------------*/
  } /* ---1- END OF else (if(..)) --------------------------------------------*/

  return (t_Result_UB);

} /*-+- END OF FUNCTION 'FUN_CCcan_RxData_UB' -+-+-+-+-+-+-+-*/  /*l_int !e818 */





/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CCcan_TxData_UB                                         |
|                                                                              |
|  Prototype at:   ADLATUS_CanHandler_tdh.h                                    |
|                                                                              |
|  Description:    HAL-Function CAN Transmitter                                |
|                                                                              |
|  Parameter:      UBYTE         t_MsgObjNbr_UB  .. Tx Message Object          |
|                  PTR_BUFFER_UB t_InBuffer_PBUF .. Tx Buffer                  |
|                                                                              |
|  Return Value:   UBYTE  t_Result_UB                                          |
|                                                                              |
|                         R B E x  x x x x                                     |
|                         | | | |  | | | |                                     |
|                         | | | +--+-+-+-+---    not used                      |
|                         | | +--------------    ERROR                         |
|                         | +----------------    BUSY                          |
|                         +------------------    READY                         |
|                                                                              |
+-----------------------------------------------------------------------------*/
UBYTE FUN_CCcan_TxData_UB (PTR_BUFFER_UB t_InBuffer_PBUF, PTR_BUFFER_UB t_OutBuffer_PBUF )
{

  UBYTE t_Result_UB;
  UBYTE t_TableLine_UB;
  UBYTE t_Index_UB;
  static UBYTE t_MsgObjNbr_UB;
  UBYTE t_State_UB;
  ULONG t_EndlessLoopProtection_UL;

  UBYTE t_Mailbox_UB;

#ifdef cs_CCTP20
  UBYTE t_Channel_UB;
#endif

  /* This construct is only in order to prevent a compiler warning            */
  t_OutBuffer_PBUF += 0x00; 

#ifdef cs_CCTP20
  /************************************************************************/
  /*                                                                      */
  /* TP20 INTERFACE                                                       */
  /*                                                                      */
  /************************************************************************/

  t_Channel_UB = t_InBuffer_PBUF[d_CCglo_ChannelNbr];

   FUN_CCcan_TxDataTP20_UB ((PTR_BUFFER_UB) &t_InBuffer_PBUF[0u] );
#endif

  /*--------------------------------------------------------------------------*/
  /* Init Result                                                              */
  /*--------------------------------------------------------------------------*/
  t_Result_UB = d_CCglo_Idle;

  /*==========================================================================*/
  /*                                                                          */
  /* -1- The inputbuffer is valid                                             */
  /*                                                                          */
  /*==========================================================================*/
  if ( d_CCglo_BufferValid == (t_InBuffer_PBUF[d_CCglo_BufferState] & d_CCglo_BufferValid))
  {
    /* init scanning                                                            */
    t_State_UB     = 0x00u;
    t_TableLine_UB = 0x00u;

    /*==========================================================================*/
    /*                                                                          */
    /* -2- Scan for the correct channel / MsgObj information                    */
    /*                                                                          */
    /*==========================================================================*/
    while ( d_CCcan_StopScan != (t_State_UB & d_CCcan_StopScan) )
    {
      /*======================================================================*/
      /* -3- End of table found  --> ERROR                                    */
      /*======================================================================*/
      if ( d_CCglo_NoEntry == CanCtrl_ST.CanChannelTable_PST[t_TableLine_UB].MsgObjectNbr_UB ) 
        {
        /* Stop Scan                                                          */
        t_State_UB = d_CCcan_StopScan;
      } /* -3- END OF if(..) -------------------------------------------------*/

      /*======================================================================*/
      /* -3- Entry is vaild                                                   */
      /*======================================================================*/
      else
      {
        /*========================================================================*/
        /* -4- ChannelInfo is not correct                                         */
        /*     InBuffer[ChannelInfo] != ConfigTable[ChannelInfo]                  */
        /*========================================================================*/
        if ( t_InBuffer_PBUF[d_CCglo_ChannelNbr] != \
             CanCtrl_ST.CanChannelTable_PST[t_TableLine_UB].ChannelNbr_UB )
        {
          /* Check next entry                                                     */
          t_TableLine_UB ++;
       
        } /* -4- END OF if (..) --------------------------------------------------*/

        /*========================================================================*/
        /* -4- ChannelInfo is correct                                             */
        /*========================================================================*/
        else
        {
          /* Stop Scan                                                          */
          t_State_UB = (d_CCcan_StopScan | d_CCcan_CorrectMsgEntryFound);

        } /* -4- END OF else (if(..)) ----------------------------------------*/
      } /* ---3- END OF else (if(..)) ----------------------------------------*/
    } /* -----2- END OF while (..) -------------------------------------------*/


    /*========================================================================*/
    /*                                                                        */
    /* -2- SEND DATA VIA CAN                                                  */
    /*                                                                        */
    /*========================================================================*/
    if ( d_CCcan_CorrectMsgEntryFound ==  (t_State_UB & d_CCcan_CorrectMsgEntryFound))
    {
      /*----------------------------------------------------------------------*/
      /* Get the correct tx Message Object number                             */
      /*----------------------------------------------------------------------*/
      t_MsgObjNbr_UB = CanCtrl_ST.CanChannelTable_PST[t_TableLine_UB].MsgObjectNbr_UB;
                                                                                       
      t_State_UB |= (CanCtrl_ST.CanChannelTable_PST[t_TableLine_UB].AddIdByteInfo_UB & d_CCglo_FixedMsgLgnth); 

      /* get Info which mailbox will be used for ram function */
      if(d_CCglo_MbSendFromRam == (CanCtrl_ST.CanChannelTable_PST[t_TableLine_UB].AddIdByteInfo_UB & d_CCglo_MbSendFromRam))
      {
        CanCtrl_ST.MbUsedinRAM_UB = t_MsgObjNbr_UB;
      }
      
      /* get mailbox number and build mask */
      t_Mailbox_UB = 0x00000001UL << t_MsgObjNbr_UB;

      /* due to a bug in the TMS470 SCC, an incoming message may be */
      /* discarded if a transmit is started at the same time */
      /* wait for mailbox free (transmitter ready) */
      while ((e_SCC_ST.CANTRS_UL & (ULONG)t_Mailbox_UB) != 0ul)
      { 
        ; 
      }
      
      /*----------------------------------------------------------------------*/
      /* Set new data length information                                      */
      /*----------------------------------------------------------------------*/
      if (d_CCglo_FixedMsgLgnth != (t_State_UB & d_CCglo_FixedMsgLgnth)) 
      {
         e_CAN_MB[t_MsgObjNbr_UB].MCF_ST.bw.CanMCF = ((t_InBuffer_PBUF[d_CCglo_DLC]) & 0x0F);      
      }
      else
      {
         e_CAN_MB[t_MsgObjNbr_UB].MCF_ST.bw.CanMCF = 0x08u;
      }
      /*======================================================================*/
      /* -3- Store the transmit data into RxBuffer                            */
      /*======================================================================*/      
      
#ifdef cs_CCFixed_Msg_LENGTH      
      for (t_Index_UB = (UBYTE)  0x00u;
           t_Index_UB < (UBYTE)  0x08u;
           t_Index_UB ++ )
#else
       for (t_Index_UB = (UBYTE) 0x00u;
           t_Index_UB <  t_InBuffer_PBUF[d_CCglo_DLC];
           t_Index_UB ++ )
#endif
      {
        /* Copy Data from CanControllerBuffer to RxMsgBuffer                    */
        e_CAN_MB[t_MsgObjNbr_UB].u.CanData_UB[t_Index_UB] = 
                             t_InBuffer_PBUF[d_CCglo_StartOfData + t_Index_UB];
        /* fill unused buffer with zeros */
        if (t_Index_UB > t_InBuffer_PBUF[d_CCglo_DLC])
        {
            e_CAN_MB[t_MsgObjNbr_UB].u.CanData_UB[t_Index_UB] = 0x00u;         
        }

      } /* -3- END OF FOR (..) -----------------------------------------------*/

      /* due to a bug in the TMS470 SCC, an incoming message may be */
      /* discarded if a transmit is started at the same time */
      while (e_SCC_ST.CANES_UN.CANES_ST.RM_B1)
      {  
         ; 
      }

      /* Get out here, the world is waiting for you */
      e_SCC_ST.CANTA_UL  |= (ULONG)t_Mailbox_UB;
      e_SCC_ST.CANTRS_UL |= (ULONG)t_Mailbox_UB;

      /* Set timer for endless loop protection                                */
      t_EndlessLoopProtection_UL = 0xFFFFFFFFul;
      /*======================================================================*/
      /* -3- Wait during message is sending                                   */
      /*======================================================================*/
      while (((e_SCC_ST.CANTA_UL & (ULONG)t_Mailbox_UB) == 0ul) && \
                        (0ul != t_EndlessLoopProtection_UL))
      {
          /* Decrement protection counter                                     */
          t_EndlessLoopProtection_UL--;
    
          /*------------------------------------------------------------------*/ /* [OPL299] */
          /* check the system timer                                           */
          /*------------------------------------------------------------------*/
          FUN_CCtim_SystemTimerCheck_V (); 

      }

      /*======================================================================*/
      /* -3- Tx Timeout occured                                               */
      /*======================================================================*/
      if (0x0000ul == t_EndlessLoopProtection_UL)
      {
        /*--------------------------------------------------------------------*/
        /* Set result flag                                                    */
        /* NOTE: Can is busy during transmission.                             */
        /*--------------------------------------------------------------------*/
        t_Result_UB |= d_CCglo_Busy;

      } /* -3- END OF if(..) -------------------------------------------------*/

      /*======================================================================*/
      /* -3- Msg was send                                                     */
      /*======================================================================*/
      else
      {
        /*--------------------------------------------------------------------*/
        /* Set Input buffer invalid                                           */
        /*--------------------------------------------------------------------*/
        t_InBuffer_PBUF[d_CCglo_BufferState] &= (UBYTE) ~d_CCglo_BufferValid;

        /*--------------------------------------------------------------------*/
        /* Set result flag                                                    */
        /*--------------------------------------------------------------------*/
        t_Result_UB |= d_CCglo_Ready;

      } /* -3- END OF else if(..) --------------------------------------------*/
    } /* ---2- END OF if(..) -------------------------------------------------*/
  } /* -----1- END OF if(..) -------------------------------------------------*/

  return (t_Result_UB);

} /* END OF FUNCTION 'FUN_CCcan_TxData_UB' -+-+-+-+-+-+-+-+-+-*/ /*l_int !e818 */

#ifdef cs_CCUDS_Audi
/******************************************************************************/
void FUN_CCcan_TxDiagResponse_V(UBYTE t_Handle_UB) 
{
   /* Set timer for endless loop protection                                */
   ULONG t_EndlessLoopProtection_UL = 0xFFFFFFFFul;
      
   UBYTE t_ByteCounter_UB = 0u;
   UBYTE t_Mailbox_UB;
   UBYTE t_MailboxNbr_UB; 

   /* get formerly stored mailbox number */
   /* get formerly stored mailbox number */
   t_MailboxNbr_UB = CanCtrl_ST.DiagResponseTG_PST[t_Handle_UB].SendMailBox_UB;   
   t_Mailbox_UB = 0x00000001ul << t_MailboxNbr_UB;

   /* wait for mailbox free (transmitter ready) */
   while ((e_SCC_ST.CANTRS_UL & (ULONG)t_Mailbox_UB) != 0ul) 
   {
     ;
   }
      
   for (t_ByteCounter_UB = 0x00u; \
        t_ByteCounter_UB < (UBYTE) 0x08u; \
        t_ByteCounter_UB ++ )
   {
        /* Copy Data from CanControllerBuffer to RxMsgBuffer                    */
        e_CAN_MB[t_MailboxNbr_UB].u.CanData_UB[t_ByteCounter_UB] = 
        CanCtrl_ST.DiagResponseTG_PST[t_Handle_UB].Data_AUB[t_ByteCounter_UB];

   } /* -5- END OF FOR (..) --------------------------------------------------*/

   /* due to a bug in the TMS470 SCC, an incoming message may be */
   /* discarded if a transmit is started at the same time */
   while (e_SCC_ST.CANES_UN.CANES_ST.RM_B1) 
   {
     ;
   }

   /* Get out here, the world is waiting for you */
   e_SCC_ST.CANTA_UL  |= (ULONG)t_Mailbox_UB;   
   e_SCC_ST.CANTRS_UL |= (ULONG)t_Mailbox_UB;

   /*======================================================================*/
   /* -3- Wait during message is sending                                   */
   /*======================================================================*/
   while (((e_SCC_ST.CANTA_UL & (ULONG)t_Mailbox_UB) == 0ul) && 
               (0x0000ul != t_EndlessLoopProtection_UL))
   {
          /* Decrement protection counter                                  */
          t_EndlessLoopProtection_UL--;

          /*---------------------------------------------------------------*/ /* [OPL299] */
          /* check the system timer                                        */
          /*---------------------------------------------------------------*/
          FUN_CCtim_SystemTimerCheck_V (); 
   }

   return;
    
} /* END OF FUNCTION 'FUN_CCcan_TxDataFromRam_V' +-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/

#endif
/******************************************************************************/
#ifdef cs_CCTP20
/******************************************************************************/
/*                                                                            */
/* TP2.0 Part                                                                 */
/*                                                                            */
/******************************************************************************/

void FUN_CCcanInitTP20Data_V (void)
{
  /* TP 20 specific stuff */
  /*------------------------------------------------------------------*/
  /* Reset the connection setup variables                             */
  /*------------------------------------------------------------------*/

  CanCtrl_ST.SendID_UW.UWord_ST.UWord0_UW = 0u;       /* TP20 Send ID */
  CanCtrl_ST.ReceiveID_UW.UWord_ST.UWord0_UW = 0u;    /* TP20 Rcv ID  */

  CanCtrl_ST.CanTimer_UW = d_CCCan_TIMERELAPSED;

  CanCtrl_ST. CanTimerControl_UB = 0u;
  CanCtrl_ST. CanErrorCounter_UB = 0u;

  /*------------------------------------------------------------------*/
  /* Reset the Channel Selection on CAN                               */
  /*------------------------------------------------------------------*/
  CanCtrl_ST.SelectedChannel_UB =  d_CCglo_AllChannels;

}



/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CCCan_Timer_V                                           |
|                                                                              |
|  Prototype at:   ADLATUS_CanHandler_tdh.h                                    |
|                                                                              |
|  Description:    Timer for channel open sequence and retry error handling    |
|                                                                              |
|  Parameter:      UWORD t_TimerTicks_UW                                       |
|                                                                              |
|  Return Value:   UBYTE  t_Result_UB                                          |
|                                                                              |
|                         R B E x  x x x x                                     |
|                         | | | |  | | | |                                     |
|                         | | | +--+-+-+-+---    Message Object Number         |
|                         | | +--------------    ERROR                         |
|                         | +----------------    BUSY                          |
|                         +------------------    READY                         |
|                                                                              |
+-----------------------------------------------------------------------------*/
void FUN_CCCan_Timer_V(UWORD t_TimerTicks_UW )
{

  t_TimerTicks_UW |= 0u;

  /*========================================================================*/
  /* -1- Tx Timer is still running                                          */
  /*========================================================================*/
  if(CanCtrl_ST.CanTimer_UW != d_CCCan_TIMERELAPSED) 
  {
    if (CanCtrl_ST.CanTimer_UW > t_TimerTicks_UW)
    {
      CanCtrl_ST.CanTimer_UW -= t_TimerTicks_UW;
    }
    else
    {
    /* Decrement timer value                                                */
      CanCtrl_ST.CanTimer_UW = 0u;
    }
    if(CanCtrl_ST.CanTimer_UW == d_CCCan_TIMERELAPSED) 
    {
      CanCtrl_ST.CanTimerControl_UB |= d_CCcan_TimerElapsed;
    }
  }
  return;
   
} /* -+- END OF FUN_CCCan_Timer_UB -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/



/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CCcan_RxDataTP20_UB                                     |
|                                                                              |
|  Prototype at:   ADLATUS_CanHandler.c                                        |
|                                                                              |
|  Description:    HAL-Function CAN Receiver specia for TP20 opening sequence  |
|                                                                              |
|  Parameter:      UBYTE t_channel - Channel number                            |
|                                                                              |
|  Return Value:   void                                                        |
|                                                                              |
+-----------------------------------------------------------------------------*/
UBYTE FUN_CCcan_RxDataTP20_UB (UBYTE t_channel_UB)
{
  UBYTE t_ReadIndex_UB;
  UBYTE t_Result_UB;
  UBYTE t_TableLine_UB;
  /*--------------------------------------------------------------------------*/
  /* Init                                                                     */
  /*--------------------------------------------------------------------------*/
  t_Result_UB = 0u;

  /*==========================================================================*/
  /*                                                                          */
  /* -1- Message received on 'Opening Identifier' (-> Subset 00)              */
  /*                                                                          */
  /*==========================================================================*/
  if ( d_CCglo_SubSet00 == (d_CCglo_SubSetMask & t_channel_UB ))
  {
    /* Extract the channel number and get the read index for FIFO             */
    t_channel_UB   &= d_CCglo_ChannelMask;
    t_ReadIndex_UB  = CanCtrl_ST.ReadIndex_AUB[t_channel_UB];
    
    /*------------------------------------------------------------------------*/
    /* Increment the read index                                               */
    /*------------------------------------------------------------------------*/
    CanCtrl_ST.ReadIndex_AUB[t_channel_UB] += (UBYTE) 0x01u;
    CanCtrl_ST.ReadIndex_AUB[t_channel_UB] &= (UBYTE) 0x03u;

    /*=========================================================================*/
    /* -2- Parameter of Opening message are correct                            */
    /*=========================================================================*/
    if ((t_CCcan_CanFifoBuffer_AUB[t_channel_UB][t_ReadIndex_UB][d_CCglo_DLC               ] == 0x07) && \
        (t_CCcan_CanFifoBuffer_AUB[t_channel_UB][t_ReadIndex_UB][d_CCglo_StartOfData + 0x01] == 0xC0) && \
        (t_CCcan_CanFifoBuffer_AUB[t_channel_UB][t_ReadIndex_UB][d_CCglo_StartOfData + 0x06] == 0x01) && \
        (t_CCcan_CanFifoBuffer_AUB[t_channel_UB][t_ReadIndex_UB][d_CCglo_StartOfData] == \
        (UBYTE)CanCtrl_ST.CanMsgObjConfig_PST[1].CanIdentifier_UL))
    {

      FUN_CCeventaudi_EventHandler_B (d_CCeventaudi_RESET___TP20CloseChannel_UB); 
 
      /*----------------------------------------------------------------------*/
      /* Set the TP20 Status                                                  */
      /*----------------------------------------------------------------------*/
      /* Channel Opening Request received - Response must be send             */
      /* set 0x01 u. 0x02 */
      CanCtrl_ST.CCTP20_Status_UB  |= (d_CCcan_TP20ChnlOpngRequestReceived   | 	  
                                       d_CCcan_TP20ChnlOpngResponseHasToSend  );
      /* Response must be send - No channel is established                    */
       /* reset 0x04 u. 0x08 */
      CanCtrl_ST.CCTP20_Status_UB  &= ~(d_CCcan_TP20ChnlOpngResponseWasSent | 	 
                                        d_CCcan_TP20ChnlEstablished             );
      /*----------------------------------------------------------------------*/
      /* get working IDs                                                      */
      /*----------------------------------------------------------------------*/
      CanCtrl_ST.SendID_UW.UByte_ST.UByte1_UB = t_CCcan_CanFifoBuffer_AUB[t_channel_UB][t_ReadIndex_UB][d_CCglo_StartOfData + 0x04];
      CanCtrl_ST.SendID_UW.UByte_ST.UByte0_UB = t_CCcan_CanFifoBuffer_AUB[t_channel_UB][t_ReadIndex_UB][d_CCglo_StartOfData + 0x05];
      /*----------------------------------------------------------------------*/
      /* Message is only for CAN - Set the buffer invalid                     */
      /*----------------------------------------------------------------------*/
      t_CCcan_CanFifoBuffer_AUB[t_channel_UB][t_ReadIndex_UB][d_CCglo_BufferState] &= ~d_CCglo_BufferValid;
      
      /* stop CAN because of reconfiguring */
      FUN_CCcan_StopCanController_V  ();

      /* ========================================================================== */
      /* -3- No Express ID                                                        */
      /* ========================================================================== */
      if((CanCtrl_ST.SendID_UW.UWord_ST.UWord0_UW & 0x300u) == 0x300u)
      {
        CanCtrl_ST.ReceiveID_UW.UWord_ST.UWord0_UW = \
          (UWORD)CanCtrl_ST.CanMsgObjConfig_PST[2u].CanIdentifier_UL;
        CanCtrl_ST.CCTP20_Subset_UB = d_CCglo_SubSet02; 
       /*----------------------------------------------------------------------*/
        /* Activate Rx Message Object for "working communication"               */
        /*----------------------------------------------------------------------*/
        FUN_CCcan_ActivateCanMsgObj_V((UBYTE)0x02u);
        FUN_CCcan_DeactivateCanMsgObj_V((UBYTE)0x04u);

        t_TableLine_UB = 0x02;
      }

      /*==========================================================================*/
      /* -3- Express ID                                                           */
      /*==========================================================================*/
      else
      {
        CanCtrl_ST.ReceiveID_UW.UWord_ST.UWord0_UW = \
          (UWORD)CanCtrl_ST.CanMsgObjConfig_PST[4].CanIdentifier_UL;
        CanCtrl_ST.CCTP20_Subset_UB = d_CCglo_SubSet04; 

       /*----------------------------------------------------------------------*/
        /* Activate Rx Message Object for "working communication"               */
        /*----------------------------------------------------------------------*/
    
        FUN_CCcan_ActivateCanMsgObj_V((UBYTE)0x04u);
        FUN_CCcan_DeactivateCanMsgObj_V((UBYTE)0x02u);

        t_TableLine_UB = 0x04u;
      }

      /*--------------------------------------------------------------*/
      /* Get the Message Object Number                                */
      /*--------------------------------------------------------------*/
      CanCtrl_ST.CanMsgObjConfig_PST[t_TableLine_UB].MsgObjState_UB     = d_CCcan_Activate;
      CanCtrl_ST.CanMsgObjConfig_PST[t_TableLine_UB].CanIdentifier_UL   = (ULONG) CanCtrl_ST.ReceiveID_UW.UWord_ST.UWord0_UW;

      /* reconfigure CAN */
      FUN_CCcan_ActivateCanMsgObj_V((UBYTE) t_TableLine_UB);
      
      t_TableLine_UB++;
      /*--------------------------------------------------------------*/
      /* Get the Message Object Number                                */
      /*--------------------------------------------------------------*/
      CanCtrl_ST.CanMsgObjConfig_PST[t_TableLine_UB].CanIdentifier_UL  = (ULONG) CanCtrl_ST.SendID_UW.UWord_ST.UWord0_UW;

      /* reconfigure CAN */
      FUN_CCcan_ActivateCanMsgObj_V((UBYTE) t_TableLine_UB);
      /* reconfigure done --- start CAN */
       FUN_CCcan_StartCanController_V ();

      /*-----------------------------------------------------------------------*/
      /* Set the Tx options                                                    */
      /*-----------------------------------------------------------------------*/
      /* set timer to 10ms plus a little delay */          
      CanCtrl_ST.CanTimer_UW = 11u;
      /* set retry counter plus first message */
      CanCtrl_ST.CanErrorCounter_UB = 11u;
      /* init timer control */
      CanCtrl_ST.CanTimerControl_UB = d_CCcan_TimerEnabled;


    } /* -2- END OF if(..) -----------------------------------------------------*/
    else
    {
       t_Result_UB = d_CCcan_MsgObjInvalid;
    }
  } /* ---1- END OF if(..) -----------------------------------------------------*/

  /*==========================================================================*/
  /*                                                                          */
  /* -1- Message received on 'Working Identifier' (-> Subset 02/04)           */
  /*                                                                          */
  /*==========================================================================*/
  else
  {
    /*========================================================================*/
    /* -2- Communication Setup State evaluation                               */
    /*========================================================================*/
    switch (d_CCcan_TP20ChannelSetupMask & CanCtrl_ST.CCTP20_Status_UB) 
    {
      /*------------------------------------------------------------------------*/
      /* (a) Channel Opening Request Message Received and                       */
      /* (b) Channel Opening Response Message sent and                          */
      /* (c) Communication is established                                       */
      /*------------------------------------------------------------------------*/
      case (d_CCcan_TP20ChnlOpngRequestReceived   | \
            d_CCcan_TP20ChnlOpngResponseWasSent   | \
            d_CCcan_TP20ChnlEstablished                ):
        /*--------------------------------------------------------------------*/
        /* Normal message received                                            */
        /*--------------------------------------------------------------------*/
        t_Result_UB = d_CCglo_Ready;
        
        break;
        /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      /*------------------------------------------------------------------------*/
      /* (a) Channel Opening Message Received and                               */
      /* (b) Response was sent                                                  */
      /*------------------------------------------------------------------------*/
      case (d_CCcan_TP20ChnlOpngRequestReceived   | \
            d_CCcan_TP20ChnlOpngResponseWasSent  ):

        /*--------------------------------------------------------------------*/
        /* Message on working ID received - Communication established         */
        /*--------------------------------------------------------------------*/
        CanCtrl_ST.CCTP20_Status_UB |=  d_CCcan_TP20ChnlEstablished;	
        	   /* set 0x08 */

        /*--------------------------------------------------------------------*/
        /* Disable the communication setup timeout timer                      */
        /*--------------------------------------------------------------------*/
        CanCtrl_ST.CanTimer_UW       = d_CCCan_TIMERELAPSED;
        CanCtrl_ST.CanErrorCounter_UB = 0;
        CanCtrl_ST.CanTimerControl_UB = d_CCcan_TimerDisabled;
        /*--------------------------------------------------------------------*/
        /* Set the result                                                     */
        /*--------------------------------------------------------------------*/
        t_Result_UB = d_CCglo_Ready;
        
        break;
        /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

      /*------------------------------------------------------------------------*/
      /* default                                                                */
      /*------------------------------------------------------------------------*/
      default:
        break;
        /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    } /* -2- END OF switch (..) ----------------------------------------------*/
  } /* ---1- END OF else (if(..)) ----------------------------------------------*/

  return t_Result_UB;

} /* -+- END OF 'FUN_CCcan_RxDataTP20_UB' -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/




/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CCcan_TxDataTP20_UB                                     |
|                                                                              |
|  Prototype at:   ADLATUS_CanHandler_tdh.h                                    |
|                                                                              |
|  Description:    HAL-Function CAN Transmitter                                |
|                                                                              |
|  Parameter:      UBYTE         t_MsgObjNbr_UB  .. Tx Message Object          |
|                  PTR_BUFFER_UB t_InBuffer_PBUF .. Tx Buffer                  |
|                                                                              |
|  Return Value:   UBYTE  t_Result_UB                                          |
|                                                                              |
|                         R B E x  x x x x                                     |
|                         | | | |  | | | |                                     |
|                         | | | +--+-+-+-+---    not used                      |
|                         | | +--------------    ERROR                         |
|                         | +----------------    BUSY                          |
|                         +------------------    READY                         |
|                                                                              |
+-----------------------------------------------------------------------------*/
UBYTE FUN_CCcan_TxDataTP20_UB (PTR_BUFFER_UB t_InBuffer_PBUF )
{

  UBYTE t_Result_UB;
  UBYTE t_Channel_UB;

  /*--------------------------------------------------------------------------*/
  /* Init Result                                                              */
  /*--------------------------------------------------------------------------*/
  t_Result_UB  = d_CCglo_Idle;
  t_Channel_UB = t_InBuffer_PBUF[d_CCglo_ChannelNbr];

  /*==========================================================================*/
  /*                                                                          */
  /* -1- Evaluate the current communication state                             */
  /*                                                                          */
  /* NOTE: The sequence of the checks is runtime optimized!!                  */
  /*==========================================================================*/
  switch (d_CCcan_TP20ChannelSetupMask & CanCtrl_ST.CCTP20_Status_UB) 
  {
    /*------------------------------------------------------------------------*/
    /*                                                                        */
    /* (a) Channel Opening Message Received and                               */
    /* (b) Response was sent                                                  */
    /* (c) Communication was established                                      */
    /*                                                                        */
    /*------------------------------------------------------------------------*/
    case (d_CCcan_TP20ChnlOpngRequestReceived | \
          d_CCcan_TP20ChnlOpngResponseWasSent | \
          d_CCcan_TP20ChnlEstablished               ):

        /*----------------------------------------------------------------------*/
        /* Init the channel Info for sending the response message               */
        /*----------------------------------------------------------------------*/
        /* reset old Subset                                                     */
        t_InBuffer_PBUF[d_CCglo_ChannelNbr]  &= d_CCglo_ChannelMask;
        /* Set new subset                                                       */
        t_InBuffer_PBUF[d_CCglo_ChannelNbr]  |= (CanCtrl_ST.CCTP20_Subset_UB | 0x10);

      /* check for something to do only if data is valid */
      /*======================================================================*/
      /* -2- Close Channel after Disconnect                                   */
      /*======================================================================*/
      if( FUN_CCeventaudi_EventHandler_B (d_CCeventaudi_LOOKFOR_TP20CloseChannel_UB) != FALSE) 
      {
        /* reset flag */
        FUN_CCeventaudi_EventHandler_B (d_CCeventaudi_RESET___TP20ApplDisCntRqst_UB);
        FUN_CCeventaudi_EventHandler_B (d_CCeventaudi_RESET___TP20CloseChannel_UB); 

        FUN_CCcan_StopCanController_V  ();

        FUN_CCcan_DeactivateCanMsgObj_V((UBYTE)0x04u);
        FUN_CCcan_DeactivateCanMsgObj_V((UBYTE)0x02u);


        FUN_CCcan_StartCanController_V ();

        FUN_CCcanInitTP20Data_V ();

        CanCtrl_ST.CCTP20_Status_UB = 0x00u;
      }

      break;
      /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    /*------------------------------------------------------------------------*/
    /*                                                                        */
    /* (a) Channel Opening Message Received and                               */
    /* (b) Response must be send                                              */
    /*                                                                        */
    /*------------------------------------------------------------------------*/
    case (d_CCcan_TP20ChnlOpngRequestReceived   | \
          d_CCcan_TP20ChnlOpngResponseHasToSend  ):

      /*======================================================================*/
      /* -2- TxTimer is elapsed                                               */
      /*======================================================================*/
      if (   (d_CCcan_TimerEnabled | d_CCcan_TimerElapsed)  ==  \
           ( (d_CCcan_TimerEnabled | d_CCcan_TimerElapsed) & CanCtrl_ST.CanTimerControl_UB))
      {
        /*--------------------------------------------------------------------*/
        /* Disable Timer                                                      */
        /*--------------------------------------------------------------------*/
        CanCtrl_ST.CanTimerControl_UB &= (UBYTE) ~(d_CCcan_TimerEnabled | d_CCcan_TimerElapsed);
        /*----------------------------------------------------------------------*/
        /* Init the Buffer state for sending the response message               */
        /*----------------------------------------------------------------------*/
        t_InBuffer_PBUF[d_CCglo_BufferState] |=  d_CCglo_BufferValid;
        t_InBuffer_PBUF[d_CCglo_ChannelNbr]  &=  d_CCglo_ChannelMask;
        t_InBuffer_PBUF[d_CCglo_ChannelNbr]  |=  d_CCglo_SubSet01;
        t_InBuffer_PBUF[d_CCglo_DLC]          = 0x07u;
        /*----------------------------------------------------------------------*/
        /* fill buffer with setup data (TX and RX id)                           */
        /*----------------------------------------------------------------------*/
        t_InBuffer_PBUF[d_CCglo_StartOfData+0] = 0x00u;
        t_InBuffer_PBUF[d_CCglo_StartOfData+1] = 0xD0u;
        t_InBuffer_PBUF[d_CCglo_StartOfData+2] = CanCtrl_ST.SendID_UW.UByte_ST.UByte1_UB;
        t_InBuffer_PBUF[d_CCglo_StartOfData+3] = CanCtrl_ST.SendID_UW.UByte_ST.UByte0_UB;
        t_InBuffer_PBUF[d_CCglo_StartOfData+4] = CanCtrl_ST.ReceiveID_UW.UByte_ST.UByte1_UB;
        t_InBuffer_PBUF[d_CCglo_StartOfData+5] = CanCtrl_ST.ReceiveID_UW.UByte_ST.UByte0_UB;
        t_InBuffer_PBUF[d_CCglo_StartOfData+6] = 0x01u;
        /*----------------------------------------------------------------------*/
        /* set timerout-timer, timerstatus and increment counter                */
        /*----------------------------------------------------------------------*/
        CanCtrl_ST.CanTimer_UW         = 100;
        CanCtrl_ST.CanTimerControl_UB |= d_CCcan_TimerEnabled;
        CanCtrl_ST.CanErrorCounter_UB --;
        /*----------------------------------------------------------------------*/
        /* Response message will be send now - set/reset the flags              */
        /*----------------------------------------------------------------------*/
        CanCtrl_ST.CCTP20_Status_UB  &= ~d_CCcan_TP20ChnlOpngResponseHasToSend;  /* reset 0x02 */
        CanCtrl_ST.CCTP20_Status_UB  |=  d_CCcan_TP20ChnlOpngResponseWasSent;    /* set 0x04   */

        FUN_CCeventaudi_EventHandler_B (d_CCeventaudi_SET_____TP20OpenChannel_UB);

      }/* -2- END OF if(..) --------------------------------------------------*/

      break;
      /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


    /*------------------------------------------------------------------------*/
    /*                                                                        */
    /* (a) Channel Opening Message Received and                               */
    /* (b) Response was sent and                                              */
    /* (c) No answere on working Identifier                                   */
    /*                                                                        */
    /*------------------------------------------------------------------------*/
    case (d_CCcan_TP20ChnlOpngRequestReceived   | \
          d_CCcan_TP20ChnlOpngResponseWasSent    ):

      /*======================================================================*/
      /* -2- TxTimer is elapsed                                               */
      /*======================================================================*/
      if (   (d_CCcan_TimerEnabled | d_CCcan_TimerElapsed)  ==  \
           ( (d_CCcan_TimerEnabled | d_CCcan_TimerElapsed) & CanCtrl_ST.CanTimerControl_UB))
      {
        /*--------------------------------------------------------------------*/
        /* Disable Timer                                                      */
        /*--------------------------------------------------------------------*/
        CanCtrl_ST.CanTimerControl_UB &= (UBYTE) ~(d_CCcan_TimerEnabled | d_CCcan_TimerElapsed);

        /*======================================================================*/
        /* -3- Retry sending the response message                               */
        /*======================================================================*/
        if (CanCtrl_ST.CanErrorCounter_UB != (UBYTE)0u)
        {
          /*--------------------------------------------------------------------*/
          /* Init the Buffer state for sending the response message               */
          /*--------------------------------------------------------------------*/
          t_InBuffer_PBUF[d_CCglo_BufferState] |= d_CCglo_BufferValid;
          t_InBuffer_PBUF[d_CCglo_ChannelNbr]  &= d_CCglo_ChannelMask;
          t_InBuffer_PBUF[d_CCglo_ChannelNbr]  |= d_CCglo_SubSet01;
          t_InBuffer_PBUF[d_CCglo_DLC]          = 0x07u;
          /*--------------------------------------------------------------------*/
          /* fill buffer with setup data (TX and RX id)                           */
          /*--------------------------------------------------------------------*/
          t_InBuffer_PBUF[d_CCglo_StartOfData+0] = 0x00u;
          t_InBuffer_PBUF[d_CCglo_StartOfData+1] = 0xD0u;
          t_InBuffer_PBUF[d_CCglo_StartOfData+2] = CanCtrl_ST.SendID_UW.UByte_ST.UByte1_UB;
          t_InBuffer_PBUF[d_CCglo_StartOfData+3] = CanCtrl_ST.SendID_UW.UByte_ST.UByte0_UB;
          t_InBuffer_PBUF[d_CCglo_StartOfData+4] = CanCtrl_ST.ReceiveID_UW.UByte_ST.UByte1_UB;
          t_InBuffer_PBUF[d_CCglo_StartOfData+5] = CanCtrl_ST.ReceiveID_UW.UByte_ST.UByte0_UB;
          t_InBuffer_PBUF[d_CCglo_StartOfData+6] = 0x01u;
          /*--------------------------------------------------------------------*/
          /* set timerout-timer, timerstatus and increment counter                */
          /*--------------------------------------------------------------------*/
          CanCtrl_ST.CanTimer_UW         = 100u;
          CanCtrl_ST.CanTimerControl_UB |= d_CCcan_TimerEnabled;
          CanCtrl_ST.CanErrorCounter_UB --;

        } /* -------3- END OF if(..) -----------------------------------------*/

        /*====================================================================*/
        /* -3- Number of attemps elapsed - No communication established         */
        /*====================================================================*/
        else
        {
          /*------------------------------------------------------------------*/
          /* Send a disconnect                                                    */
          /*------------------------------------------------------------------*/
          t_InBuffer_PBUF[d_CCglo_BufferState] |= d_CCglo_BufferValid;
          t_InBuffer_PBUF[d_CCglo_ChannelNbr]  &= d_CCglo_ChannelMask;
          t_InBuffer_PBUF[d_CCglo_ChannelNbr]  |= (CanCtrl_ST.CCTP20_Subset_UB + 0x10);
          t_InBuffer_PBUF[d_CCglo_DLC]          = 0x01u;

          /*------------------------------------------------------------------*/
          /* fill buffer with the disconnect message                          */
          /*------------------------------------------------------------------*/
          t_InBuffer_PBUF[d_CCglo_StartOfData+0] = 0xA8u;

          /*------------------------------------------------------------------*/
          /* Inform system about a failed channel setup                       */
          /*------------------------------------------------------------------*/
          FUN_CCeventaudi_EventHandler_B (d_CCeventaudi_RESET___TP20OpenChannel_UB);
          /* reset flag */
          FUN_CCeventaudi_EventHandler_B (d_CCeventaudi_RESET___TP20CloseChannel_UB); 
          FUN_CCeventaudi_EventHandler_B (d_CCeventaudi_RESET___TP20ApplDisCntRqst_UB);

          /*------------------------------------------------------------------*/
          /* Deactivate all Message Object for "working communication"        */
          /* Activate default values for message object configuration         */
          /*------------------------------------------------------------------*/
          FUN_CCcan_StopCanController_V  ();

          FUN_CCcan_DeactivateCanMsgObj_V((UBYTE)0x02u);
          FUN_CCcan_DeactivateCanMsgObj_V((UBYTE)0x04u);
              
          FUN_CCcan_StartCanController_V ();

          FUN_CCcanInitTP20Data_V ();

          CanCtrl_ST.CCTP20_Status_UB = 0x00u;

        } /* -------3- END OF if(..) -----------------------------------------*/
      } /* ---------2- END OF if (..) ----------------------------------------*/

      break;
      /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    /*------------------------------------------------------------------------*/
    /* IDLE                                                                   */
    /*------------------------------------------------------------------------*/
    default:
      break; /* END OF 'default:' --------------------------------------------*/
      /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  } /* -1- END OF switch(..) -------------------------------------------------*/
 
  return (t_Result_UB);

} /* END OF FUNCTION 'FUN_CCcan_TxDataTP20_UB' -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
#endif

/* #define debugmessages */
#ifdef debugmessages
void FUN_CCcan_TxDataTest_V(UBYTE data, UBYTE Mbox)
{
    /* Set timer for endless loop protection                                */
    ULONG t_EndlessLoopProtection_UL = 0xFFFFFFFFUL;

    CCcan_HwMsgObj_PST[Mbox].Data_UB[0] = data;

    CCcan_HwMsgObj_PST[Mbox].MOFCR |= (0x01ul << 24ul);      

    CCcan_HwMsgObj_PST[Mbox].MOCTR = 0x07000000ul;  /* Set TXRQ */

    /*======================================================================*/
    /* -3- Wait during message is sending                                   */
    /*======================================================================*/
    while ( ((ULONG) d_CCcan_TxOK != ( CCcan_HwSysReg_PST->NSR & (ULONG)d_CCcan_TxOK)) && 
            ( 0x0000ul != t_EndlessLoopProtection_UL                     )    )
    {
      /* Decrement protection counter                                       */
      t_EndlessLoopProtection_UL -= 0x0001ul;

      /*------------------------------------------------------------------*/ /* [OPL299] */
      /* check the system timer                                           */
      /*------------------------------------------------------------------*/
      FUN_CCtim_SystemTimerCheck_V (); 
    }

    CCcan_HwSysReg_PST->NSR &= (ULONG) ~d_CCcan_TxOK;

    return;
} /* END OF FUNCTION 'FUN_CCcan_TxDataFromRam_V' +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
#endif

/*lint +e40 */

/* ========================================================================== */
/*      E N D   O F   F I L E                                                 */
/* ========================================================================== */
