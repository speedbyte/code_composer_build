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
|  Filename:  ADLATUS_SessionHandler.c                                         |
|                                                                              |
|  Comment:   This file includes all session-layer services for the            |
|             SMART ADLATUS communication protocol stack.                      |
|                                                                              |
+------------------------------------------------------------------------------+
| R E V I S I O N   H I S T O R Y                                              |
+------------------------------------------------------------------------------+
|
| Subversion Info:
| $Id: ADLATUS_SessionHandler.c 4396 2008-11-04 09:26:32Z maennel $
| $HeadURL: http://smartsvn/svn/adlatusrepository/auslieferungen/S1NN/086350-S1NN-ADLATUS-TMS470R-MucSuc/20081201_S1NN_TMS470R_BlfIso-Source_V1.00/01_SW_Projekt/09_audilib/02_src/ADLATUS_SessionHandler.c $
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
#include "ADLATUS_ConfigSystem_cdf.h"  /* ADLATUS System configuration        */
#include "ADLATUS_Global_cdf.h"

/* ========================================================================== */
/* (2)  I N C L U D E S                                                       */
/* -------------------------------------------------------------------------- */
/*      T Y P E   D E F I N I T I O N   F I L E S  (TDF)                      */
/* ========================================================================== */
#include "ADLATUS_Types_tdf.h"                 /* must be the first include   */
#include "ADLATUS_SessionHandler_tdf.h"        /* Session Handler             */
#include "ADLATUS_Kwp2000Audi_tdf.h"
#include "ADLATUS_Timer_tdf.h"

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
#ifndef cs_CCtypes_32BitSequence
  #ifndef cs_CCtypes_16BitSequence 
       #error (undefined sequence width)
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
/*      D E F I N E S   F O R   C O M M A N D   T A B L E   E N T R I E S     */
/*      -----------------------------------------------------------------     */
/*      (on FUN_CCses_TxSession_UB)                                           */
/*                                                                            */
/* -------------------------------------------------------------------------- */
#define d_CCses_RequestLengthErrorOffset    0x00
#define d_CCses_SequenceErrorOffset         (d_CCses_RequestLengthErrorOffset + 0x01)
#define d_CCses_SessionErrorOffset          (d_CCses_SequenceErrorOffset + 0x01)
#define d_CCses_AddressingErrorOffset       (d_CCses_SessionErrorOffset + 0x01)
#define d_CCses_WrongAddrNoRespErrorOffset  (d_CCses_AddressingErrorOffset + 0x01)

/* ([OPL364] - new define for new errorfunction in sequence table) */
#define d_CCses_SecurityAccessErrorOffset  (d_CCses_WrongAddrNoRespErrorOffset + 0x01)

#define d_CCses_FirstServiceRequestOffset   (d_CCses_SecurityAccessErrorOffset + 0x01)

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

/* -------------------------------------------------------------------------- */
/*                                                                            */
/*      E V A L U A T I O N   S T A T E    D E F I N E S                      */
/*      -----------------------------------------------------------------     */
/*      (on FUN_CCses_TxSession_UB)                                           */
/*                                                                            */
/*      EvaluationState_UB = 7 6 5 4  3 2 1 0                                 */
/*                           | | | |  | | | |                                 */
/*                           | | | |  | | | +---   Request Length Error       */
/*                           | | | |  | | +-----   Request Sequence Error     */
/*                           | | | |  | +-------   Request Session Error      */
/*                           | | | |  +---------   Request Addressing Error   */
/*                           | | | +------------   Request WrongAddr no Resp  */
/*                           +-+-+--------------   NOT USED                   */
/*                                                                            */
/* -------------------------------------------------------------------------- */
#define  d_CCses_NoError                         0x00
#define  d_CCses_FlagRequestLengthError          0x01
#define  d_CCses_FlagRequestSequenceError        0x02
#define  d_CCses_FlagRequestSessionError         0x04
#define  d_CCses_FlagRequestAddressingError      0x08
#define  d_CCses_FlagRequestWrongAddrNoRespError 0x10
#define  d_CCses_FlagRequestSecurityAccessError  0x20


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

/* -------------------------------------------------------------------------- */
/*                                                                            */
/*      S C A N   S T A T E    D E F I N E S                                  */
/*      -----------------------------------------------------------------     */
/*      (on FUN_CCses_LookForCommand_PST)                                     */
/*                                                                            */
/* -------------------------------------------------------------------------- */
#define  d_CCses_StopScan                   0x00
#define  d_CCses_ScanRunning                0x01
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

/* ========================================================================== */
/* (6)  L O C A L  T Y P E   D E F I N E S                                    */
/* -------------------------------------------------------------------------- */
/*      O N L Y   F O R   T H I S   M O D U L                                 */
/* ========================================================================== */

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*      Session Control Structure                                             */
/*      -----------------------------------------------------------------     */
/*                                                                            */
/*----------------------------------------------------------------------------*/
typedef struct _SESSIONCONTROL
{
  /* ------------------------------------------------------------------------ */
  /*  4 Byte - pointer to command evaluation table                            */
  /*                                                                          */
  /*  4 Byte - pointer to current command table                               */
  /*                                                                          */
  /*  4 Byte - pointer to current command                                     */
  /*                                                                          */
  /*  4 Byte - pointer to global Command Init Table base address              */
  /*                                                                          */
  /*  4 Byte - pointer to global RuntimeInfo base address                     */
  /*                                                                          */
  /*  x Byte - 1Byte * x Channels - state register for the conditions         */
  /* ------------------------------------------------------------------------ */
  /* 16 + x Byte                                                              */

  const COMMAND_ST* *         CmdTableEvaluation_PPST;
  const COMMAND_ST*           CurrentCmdTable_PST;
  const COMMAND_ST*           SelectedCmdEntry_PST;

  const COMMANDINIT_ST* *      CmdInitTable_PPST;

  CMDRUNTIMEINFO_ST*    CmdRunTimeInfo_PST;

#ifdef cs_CCUDS_Audi
  const ADDRESSMODECONFIG_ST* AddrModeCfg_PST;    /*  4 Byte - Ptr to Address Mode Config Table */
 #endif

  UBYTE                 ActCondition_AUB[d_CCconsys_NbrOfLogChannel];

} SESSIONCONTROL_ST;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


/* ========================================================================== */
/* (7)  L O C A L   V A R I A B L E S  / C O N S T A N T S                    */
/* -------------------------------------------------------------------------- */
/*      O N L Y   F O R   T H I S   M O D U L                                 */
/* ========================================================================== */
SESSIONCONTROL_ST    SesCtrl_ST;  /* Session Control Structure                */


/* ========================================================================== */
/* (8)  L O C A L   F U N C T I O N   P R O T O T Y P E S                     */
/* -------------------------------------------------------------------------- */
/*      O N L Y   F O R   T H I S   M O D U L                                 */
/* ========================================================================== */
const COMMAND_ST* FUN_CCses_LookForCmd_PST ( PTR_BUFFER_UB Buffer_PUB, \
                                       const COMMAND_ST* CmdStructEntry_PST);

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




/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CCses_InitSessionLayer_V                                |
|                                                                              |
|  Prototype at:   ADLATUS_SessionHandler_tdf.h                                |
|                                                                              |
|  Description:    Initialization of the session layer.                        |
|                                                                              |
|  Parameter:      const SESSIONINTERFACE_ST* t_SessionInterface_PST ..        |
|                                                       Ptr to Interface       |
|                                                                              |
|  Return Value:    --                                                         |
|                                                                              |
+-----------------------------------------------------------------------------*/
void FUN_CCses_InitSessionLayer_V (const SESSIONINTERFACE_ST* t_SessionInterface_PST)
{
  /*--------------------------------------------------------------------------*/
  /* Init ControlStructureEntry 1: Ptr to the session selection table         */
  /*--------------------------------------------------------------------------*/
  SesCtrl_ST.CmdTableEvaluation_PPST = t_SessionInterface_PST->SessionSelectionTable_PPST;
  /*--------------------------------------------------------------------------*/
  /* Init ControlStructureEntry 2: Ptr to global command runtime info         */
  /*--------------------------------------------------------------------------*/
  SesCtrl_ST.CmdRunTimeInfo_PST = t_SessionInterface_PST->CmdRunTimeInfo_PST;
  /*--------------------------------------------------------------------------*/
  /* Init ControlStructureEntry 3: Ptr to selected command entry              */
  /*--------------------------------------------------------------------------*/
  SesCtrl_ST.SelectedCmdEntry_PST = 0;

  SesCtrl_ST.CmdRunTimeInfo_PST->AllowedSessions_UB = 0;  /* (KA) ist diese Zeile notwendig / richtig??? */
  /*--------------------------------------------------------------------------*/
  /* Init ControlStructureEntry 4: Init Command Init Table                    */
  /*--------------------------------------------------------------------------*/
  SesCtrl_ST.CmdInitTable_PPST = t_SessionInterface_PST->CmdInitTable_PPST;
  /*--------------------------------------------------------------------------*/
  /* Init ControlStructureEntry 5: Init CommandRuntimeInfo                    */
  /*--------------------------------------------------------------------------*/
  /* Note: It is not necessary here, because this will be done by the         */
  /*       InitChannel - Routine!                                             */
  /*--------------------------------------------------------------------------*/
  /* Init ControlStructureEntry 6: Init AddressModeConfig                     */
  /*--------------------------------------------------------------------------*/

  #ifdef cs_CCUDS_Audi
  SesCtrl_ST.AddrModeCfg_PST = (const ADDRESSMODECONFIG_ST*) t_SessionInterface_PST->AddrModeCfg_PST;
  #endif


  return;

} /*-+- END OF FUNCTION 'FUN_CCses_InitSessionLayer_V' -+-+-+-+-+-+-+-+-+-+-+-*/




/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CCses_GetActiveSession_UB                               |
|                                                                              |
|  Prototype at:   ADLATUS_SessionHandler_tdf.h                                |
|                                                                              |
|  Description:    Send the current session info for a channel.                |
|                                                                              |
|  Parameter:      UBYTE  t_ChannelNbr_UB    .. Channel for session info       |
|                                                                              |
|  Return Value:   UBYTE  t_ActiveSession_UB .. Currently active session of    |
|                                               channel                        |
|                                                                              |
+-----------------------------------------------------------------------------*/
UBYTE FUN_CCses_GetActiveSession_UB ( UBYTE t_Channel_UB )
{
  /*--------------------------------------------------------------------------*/
  /* Return the currently active session for the requested channel            */
  /*--------------------------------------------------------------------------*/
  return (SesCtrl_ST.CmdRunTimeInfo_PST[t_Channel_UB].ActiveSession_UB);

} /*-+- END OF FUNCTION 'FUN_CCses_GetActiveSession_UB' -+-+-+-+-+-+-+-+-+-+-+*/




/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CCses_LookForCmd_PST                                    |
|                                                                              |
|  Prototype at:   ADLATUS_SessionHandler_tdf.h                                |
|                                                                              |
|  Description:    This function scans the comand structure for a valid        |
|                  service request entry.                                      |
|                                                                              |
|  Parameter:      COMMAND_ST* .. Ptr to cmd structure                         |
|                                                                              |
|  Return Value:   COMMAND_ST* .. Ptr to a cmd structure entry                 |
|                                                                              |
+-----------------------------------------------------------------------------*/
const COMMAND_ST* FUN_CCses_LookForCmd_PST (PTR_BUFFER_UB Buffer_PUB, const COMMAND_ST* CmdStructEntry_PST)
{
  UBYTE ScanState_UB = d_CCses_ScanRunning;
  UBYTE t_AddrMode_UB;
  UBYTE t_Lid_UB;
  const COMMAND_ST* CmdStructEntryError_PST;
  
  CmdStructEntryError_PST = CmdStructEntry_PST;
  CmdStructEntry_PST = &CmdStructEntry_PST[d_CCses_FirstServiceRequestOffset];
  /*==========================================================================*/
  /* -1- scan Structure for valid command                                     */
  /*==========================================================================*/
  while (ScanState_UB != d_CCses_StopScan) 
  {
    /*========================================================================*/
    /* -2- SID not supported                                                  */
    /*========================================================================*/
    if (CmdStructEntry_PST->ServiceID_UB == d_CCglo_NoEntry )
    {
      /*----------------------------------------------------------------------*/
      /* STOP SCANNING  -  SERVICE NOT SUPPORTED                              */
      /*----------------------------------------------------------------------*/
      ScanState_UB = d_CCses_StopScan;
    } /* -2- END OF if (..) --------------------------------------------------*/

    /*========================================================================*/
    /* -2- SID supported                                                      */
    /*========================================================================*/
    else
    {
      /*======================================================================*/
      /* -3- SID position is in DLC                                           */
      /*======================================================================*/
      if (Buffer_PUB[d_CCglo_DLC] > CmdStructEntry_PST->SIDPosition_UB)
      {
        /*====================================================================*/
        /* -4- SID not found or interpretation of byte is not allowed         */
        /*====================================================================*/
        if (  \
          (CmdStructEntry_PST->ServiceID_UB != \
            Buffer_PUB[d_CCglo_StartOfData + CmdStructEntry_PST->SIDPosition_UB]))
        {
          /*------------------------------------------------------------------*/
          /* Scan next entry                                                  */
          /*------------------------------------------------------------------*/
          CmdStructEntry_PST ++;
        }
      
        /*====================================================================*/
        /* -4- SID found                                                      */
        /*====================================================================*/
        else
        {
          /*==================================================================*/
          /* -5- Only SID defined                                             */
          /*==================================================================*/
          if ( CmdStructEntry_PST->SelectPosition_UB == d_CCglo_Byte00 )
          {
            /*----------------------------------------------------------------*/
            /* STOP SCANNING  -  CORRECT ENTRY FOUND                          */
            /*----------------------------------------------------------------*/
            ScanState_UB = d_CCses_StopScan;
          } /* -5- END OF if(..) ---------------------------------------------*/
          
          /*==================================================================*/
          /* -5-  Check the next parameter                                    */
          /*==================================================================*/
          else
          {
            /*================================================================*/
            /* -6- LID not supported                                          */
            /*================================================================*/
            if (CmdStructEntry_PST->LocalID_UB == d_CCglo_NoEntry )
            {
              /*--------------------------------------------------------------*/
              /* STOP SCANNING  -  SUBFUNCTION NOT SUPPORTED                  */
              /*--------------------------------------------------------------*/
              ScanState_UB = d_CCses_StopScan;
            } /* -6- END OF if (..) ------------------------------------------*/

            /*================================================================*/
            /* -6- LID supported                                              */
            /*================================================================*/
            else
            {
              /*==============================================================*/
              /* -7- LID position in range of DLC                             */
              /*==============================================================*/
              if (CmdStructEntry_PST->LIDPosition_UB < Buffer_PUB[d_CCglo_DLC])
              {
                t_Lid_UB = Buffer_PUB[d_CCglo_StartOfData + CmdStructEntry_PST->LIDPosition_UB];
                /*============================================================*/
                /* -8- LID is subFunction (byte 01 of the service)            */
                /*============================================================*/
                if (d_CCglo_Byte01 == CmdStructEntry_PST->LIDPosition_UB)
                {
                  /*----------------------------------------------------------*/
                  /* ignore the suppressPosRspMsgIndicationBit                */
                  /*----------------------------------------------------------*/
                  t_Lid_UB &= d_CCglo_LIDSubFunctionMask;

                } /* -8- END OF if(..) ---------------------------------------*/
                  
                /*============================================================*/
                /* -8- LID not found or interpretation of byte is not allowed */
                /*============================================================*/
                if ( CmdStructEntry_PST->LocalID_UB != t_Lid_UB) 
                {
                  /*----------------------------------------------------------*/
                  /* Scan next entry                                          */
                  /*----------------------------------------------------------*/
                  CmdStructEntry_PST ++;

                } /* -8- END OF if(..) ---------------------------------------*/

                /*============================================================*/
                /* -8- LID found                                              */
                /*============================================================*/
                else
                {
                  /*==========================================================*/
                  /* -9- Only SID and LID defined                             */
                  /*==========================================================*/
                  if ( CmdStructEntry_PST->SelectPosition_UB == d_CCglo_Byte01 )
                  {
                    /*--------------------------------------------------------*/
                    /* STOP SCANNING  -  CORRECT ENTRY FOUND                  */
                    /*--------------------------------------------------------*/
                    ScanState_UB = d_CCses_StopScan;
                  } /* -9- END OF if(..) -------------------------------------*/

                  /*==========================================================*/
                  /* -9-  Check the next parameter                            */
                  /*==========================================================*/
                  else
                  {
                    /*========================================================*/
                    /* -A- CMD-ID not supported                               */
                    /*========================================================*/
                    if (CmdStructEntry_PST->CommandID_UB == d_CCglo_NoEntry )
                    {
                      /*------------------------------------------------------*/
                      /* STOP SCANNING  -  SUBFUNCTION NOT SUPPORTED          */
                      /*------------------------------------------------------*/
                      ScanState_UB = d_CCses_StopScan;
                    } /* -A- END OF if (..) ----------------------------------*/

                    /*========================================================*/
                    /* -A- CMD-ID supported                                   */
                    /*========================================================*/
                    else
                    {
                      /*======================================================*/
                      /* -B- CMD-ID in range of DLC                           */
                      /*======================================================*/
                      if (CmdStructEntry_PST->SelectPosition_UB < Buffer_PUB[d_CCglo_DLC])
                      {
                        /*====================================================*/
                        /* -C- CMD-ID not found or interpretation of byte is  */
                        /*     not allowed                                    */
                        /*====================================================*/
                        if (
                         (CmdStructEntry_PST->CommandID_UB != \
                            Buffer_PUB[d_CCglo_StartOfData + CmdStructEntry_PST->SelectPosition_UB]))
                        {
                          /*--------------------------------------------------*/
                          /* Scan next entry                                  */
                          /*--------------------------------------------------*/
                          CmdStructEntry_PST ++;

                        } /* -C- END OF if(..) -------------------------------*/

                        /*====================================================*/
                        /* -C- CMD-ID found                                   */
                        /*====================================================*/
                        else
                        {
                          /*--------------------------------------------------*/
                          /* STOP SCANNING  -  CORRECT ENTRY FOUND            */
                          /*--------------------------------------------------*/
                          ScanState_UB = d_CCses_StopScan;

                        } /* -C- END OF else (if(..)) ------------------------*/
                      }/* -B- END OF else (if(..)) ---------------------------*/
                      /*======================================================*/
                      /* -B- CMD-ID not in range of DLC                       */
                      /*======================================================*/
                      else
                      {
                        /*----------------------------------------------------*/
                        /* check the addressing mode for functional addressing*/
		                /*----------------------------------------------------*/
		                /* check the addressing mode for functional addressing*/
		                /*----------------------------------------------------*/
		                t_AddrMode_UB = FUN_CCkwpaudi_CheckAddrMode_UB  \
		                                 (Buffer_PUB, d_CCglo_FuncAddressAllowed);
		
		                /*====================================================*/
		                /* -4- service is functionally addressed              */
		                /*====================================================*/
                        if (( d_CCglo_Ready == t_AddrMode_UB ) && \
                           ((d_CCglo_AddressAllowedMask & CmdStructEntry_PST->AllowedSessions_UB) != 0))
		                {
		                  CmdStructEntry_PST = &CmdStructEntryError_PST[d_CCses_WrongAddrNoRespErrorOffset];
		                }
		                else
		                {    
		                  /*==================================================*/
		                  /* -8- Higher Priority: Session before length error */
		                  /*==================================================*/
		                  if (((((d_CCglo_AllowedSessionMask & CmdStructEntry_PST->AllowedSessions_UB) & \
		                       SesCtrl_ST.CmdRunTimeInfo_PST[d_CCglo_Channel00].AllowedSessions_UB) == 0)) && \
		                      ((d_CCglo_AllowedSessionMask & CmdStructEntry_PST->AllowedSessions_UB) != 0))
		                  {
		                    CmdStructEntry_PST = &CmdStructEntryError_PST[d_CCses_SessionErrorOffset];
		                  }
		                  else
		                  {
		                    CmdStructEntry_PST = &CmdStructEntryError_PST[d_CCses_RequestLengthErrorOffset];
		                  }
		                }    
                        /*----------------------------------------------------*/
                        /* STOP SCANNING  -  CMD unreachable                  */
                        /*----------------------------------------------------*/
                        ScanState_UB = d_CCses_StopScan;
                      }/* -B- END OF else (if(..)) ---------------------------*/
                    } /* --A- END OF else (if(..)) ---------------------------*/
                  } /* ----9- END OF else (if(..)) ---------------------------*/
                } /* ------8- END OF else (if(..)) ---------------------------*/
              } /* --------7- END OF if --------------------------------------*/
              /*==============================================================*/
              /* -7- LID position not in range of DLC                         */
              /*==============================================================*/
              else
              {
                /*------------------------------------------------------------*/
                /* check the addressing mode for functional addressing        */
                /*------------------------------------------------------------*/
                t_AddrMode_UB = FUN_CCkwpaudi_CheckAddrMode_UB  \
                                 (Buffer_PUB, d_CCglo_FuncAddressAllowed);

                /*============================================================*/
                /* -4- service is functionally addressed                      */
                /*============================================================*/
                if (( d_CCglo_Ready == t_AddrMode_UB ) && \
                   ((d_CCglo_AddressAllowedMask & CmdStructEntry_PST->AllowedSessions_UB) != 0))
                {
                  CmdStructEntry_PST = &CmdStructEntryError_PST[d_CCses_WrongAddrNoRespErrorOffset];
                }
                else
                {    
                  /*======================================================*/
                  /* -8- Higher Priority: Session before length error     */
                  /*======================================================*/
                  if (((((d_CCglo_AllowedSessionMask & CmdStructEntry_PST->AllowedSessions_UB) & \
                       SesCtrl_ST.CmdRunTimeInfo_PST[d_CCglo_Channel00].AllowedSessions_UB) == 0)) && \
                      ((d_CCglo_AllowedSessionMask & CmdStructEntry_PST->AllowedSessions_UB) != 0))
                  {
                    CmdStructEntry_PST = &CmdStructEntryError_PST[d_CCses_SessionErrorOffset];
                  }
                  else
                  {
                    CmdStructEntry_PST = &CmdStructEntryError_PST[d_CCses_RequestLengthErrorOffset];
                  }
                }               
                /*------------------------------------------------------------*/
                /* STOP SCANNING  -  LID unreachable                          */
                /*------------------------------------------------------------*/
                ScanState_UB = d_CCses_StopScan;
              } /* -------7- END OF else (if(..)) ----------------------------*/
            } /* ---------6- END OF else (if(..)) ----------------------------*/
          } /* -----------5- END OF else (if(..)) ----------------------------*/
        } /* -------------4- END OF else(..) ---------------------------------*/
      }/* ----------------3- END OF if(..) -----------------------------------*/
      /*======================================================================*/
      /* -3- SID position is not in DLC                                       */
      /*======================================================================*/
      else
      {
        CmdStructEntry_PST = &CmdStructEntryError_PST[d_CCses_RequestLengthErrorOffset];
        /*--------------------------------------------------------------------*/
        /* STOP SCANNING  -  SID unreachable                                  */
        /*--------------------------------------------------------------------*/
        ScanState_UB = d_CCses_StopScan;
      } /* -------------3- END OF else (if(..)) ------------------------------*/
    } /* ---------------2- END OF else (if (..)) -----------------------------*/
  } /* -----------------1- END OF while (..) ---------------------------------*/

  /*--------------------------------------------------------------------------*/
  /* Return value: Pointer to found structure entry                           */
  /*--------------------------------------------------------------------------*/
  return (CmdStructEntry_PST);

} /* -+- END OF 'FUN_CCses_LookForCmd_PST' -+-+-+-+-+-+-+-+-+*/ /*l_int !e818 */






/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */
/*                                                                            */
/* S I N G L E   C H A N N E L   S Y S T E M                                  */
/*                                                                            */
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */
#ifdef cs_CCsys_SingleChannelSystem /* -1-                                    */
/*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CCses_InitChannel_V                                     |
|                                                                              |
|  Prototype at:   ADLATUS_SessionHandler_tdf.h                                |
|                                                                              |
|  Description:    Initialization of the session layer.                        |
|                                                                              |
|  Parameter:      UBYTE  t_ChannelNbr_UB    .. Channel which should be init   |
|                  UBYTE  t_ActiveSession_UB .. Currently active session       |
|                  UBYTE  t_InitHandle_UB    .. 0x00 ResetChannel              |
|                                               0x01 FlashRequest              |
|                                                                              |
|  Return Value:    --                                                         |
|                                                                              |
+-----------------------------------------------------------------------------*/
void FUN_CCses_InitChannel_V ( UBYTE t_Channel_UB,       \
                               UBYTE t_ActiveSession_UB, \
                               UBYTE t_InitHandle_UB     )
{

  /*--------------------------------------------------------------------------*/
  /*                                                                          */
  /*  N .. d_CCconsys_MaxSession                                              */
  /*                                                                          */
  /*             SessionInitTable:                                            */
  /*             +----------------+----------------+-----+----------------+   */
  /*             | Session 0      | Session 1      | ... | Session N-1    |   */
  /*             +----------------+----------------+-----+----------------+   */
  /* Channel 0   | PtrToInitTable | PtrToInitTable | ... | PtrToInitTable |   */
  /*             +----------------+----------------+-----+----------------+   */
  /*                                                                          */
  /*               PtrToInitTable >-+                                         */
  /*                                |                                         */
  /*                                +--> +----------------+---------------+   */
  /*                     Reset Channel   | Active Session | InitCondition |   */
  /*                                     +----------------+---------------+   */
  /*                     Flash Request   | Active Session | InitCondition |   */
  /*                                     +----------------+---------------+   */
  /*                                                                          */
  /*--------------------------------------------------------------------------*/
  const COMMANDINIT_ST* t_SessionInitTable_PST;


  /* The next line is only to prevent a compiler warning!!                    */
  t_Channel_UB |= 0x00;

  /*--------------------------------------------------------------------------*/
  /* Get the correct address to Session Init Table entry                      */
  /*--------------------------------------------------------------------------*/
  /* (1) Calculate the offset for the correct channel CommandInitTable is not */
  /*     necessary within a single channel system!                            */
  /*     t_Offset_UB = (t_Channel_UB * d_CCconsys_MaxSession);                */
  /* (2) Offset for correct entry is not necessary within a single channel    */
  /*     system!                                                              */
  /*     t_Offset_UB += t_ActiveSession_UB;                                   */
  /* (3) Copy selected command structure base address                         */
  t_SessionInitTable_PST = SesCtrl_ST.CmdInitTable_PPST[t_ActiveSession_UB];

  /*--------------------------------------------------------------------------*/
  /* Init ControlStructureEntry 5: Command Runtime Info of each channel       */
  /*--------------------------------------------------------------------------*/
  SesCtrl_ST.CmdRunTimeInfo_PST[d_CCglo_Channel00].ActiveSession_UB = \
            t_SessionInitTable_PST[t_InitHandle_UB].ActiveSession_UB;

  SesCtrl_ST.CmdRunTimeInfo_PST[d_CCglo_Channel00].AllowedSessions_UB = \
            t_SessionInitTable_PST[t_InitHandle_UB].AllowedSessions_UB;

  /*--------------------------------------------------------------------------*/
  /* Init ControlStructureEntry 6: Session Control Variable for each channel  */
  /*--------------------------------------------------------------------------*/
  SesCtrl_ST.ActCondition_AUB[d_CCglo_Channel00] = \
                t_SessionInitTable_PST[t_InitHandle_UB].InitCondition_UB;
 
  return;

} /*-+- END OF FUNCTION 'FUN_CCses_InitChannel_V' -+-+-+-+-+-+-+-+-+-+-+-+-+-+*/





/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CCses_RxSession_UB                                      |
|                                                                              |
|  Prototype at:   ADLATUS_SessionHandler_tdf.h                                |
|                                                                              |
|  Description:    This function selects the correct command entry from        |
|                  structure and checks the correct execution conditions.      |
|                                                                              |
|  Parameter:      PTR_BUFFER_UB t_InBuffer_PBUF  .. Ptr to Inputbuffer        |
|                  PTR_BUFFER_UB t_OutBuffer_PBUF .. Ptr to Outputbuffer       |
|                                                                              |
|  Return Value:   UBYTE  t_Result_UB                                          |
|                                                                              |
|                         R B E I  I I I I                                     |
|                         | | | |  | | | |                                     |
|                         | | | +--+-+-+-+---    INDEX                         |
|                         | | +--------------    ERROR                         |
|                         | +----------------    BUSY                          |
|                         +------------------    READY                         |
|                                                                              |
+-----------------------------------------------------------------------------*/
UBYTE FUN_CCses_RxSession_UB ( PTR_BUFFER_UB t_InBuffer_PBUF, \
                               PTR_BUFFER_UB t_OutBuffer_PBUF )
{

  UBYTE t_Result_UB;           /* 1 Byte - Return Value of the routine        */
  UBYTE t_EvaluationState_UB;  /* 1 Byte - EvaluationState of the function    */
  UBYTE t_Index_UB;            /* 1 Byte - Indexvariable for differend scans  */

#ifdef cs_CCUDS_Audi
  UBYTE t_ScanState_UB;        /* 1 Byte - Scan variable                      */
  
  UBYTE t_AddrMode_UB;
 #endif
    
/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
/* --> Start of BUGFIX - V 1.01 - 30.07.2004                                  */
/* ---------------------------------------------------------------------------*/
  UBYTE t_Counter_UB;
/* ---------------------------------------------------------------------------*/
/* <-- End of BUGFIX   - V 1.01 - 30.07.2004                                  */
/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
#ifdef cs_CCtypes_32BitSequence /* -2-                                        */
/*----------------------------------------------------------------------------*/
  ULONG t_PreCondition_UL;     /* 4 Byte - PreCondition entry from PCT        */
  ULONG t_CurrentCondition_UL; /* 4 Byte - Cur. Condition variable for check  */
/*----------------------------------------------------------------------------*/
#endif  /* -2- cs_CCtypes_32BitSequence                                       */
/*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
#ifdef cs_CCtypes_16BitSequence /* -2-                                        */
/*----------------------------------------------------------------------------*/
  UWORD t_PreCondition_UW;     /* 2 Byte - PreCondition entry from PCT        */
  UWORD t_CurrentCondition_UW; /* 2 Byte - Cur. Condition variable for check  */
/*----------------------------------------------------------------------------*/
#endif  /* -2- cs_CCtypes_16BitSequence                                       */
/*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/


  /* ------------------------------------------------------------------------ */
  /* Init (1) - Init Result                                                   */
  /* ------------------------------------------------------------------------ */
  t_Result_UB = d_CCglo_Idle;

#ifdef cs_CCUDS_Audi
  t_AddrMode_UB = d_CCglo_Ready;
 #endif

  /*==========================================================================*/
  /*                                                                          */
  /* -1- InBuffer is valid / OutBuffer is invalid                             */
  /*                                                                          */
  /*==========================================================================*/
  if( ((t_InBuffer_PBUF[d_CCglo_BufferState]  & d_CCglo_BufferValid) == d_CCglo_BufferValid) &&
      ((t_OutBuffer_PBUF[d_CCglo_BufferState] & d_CCglo_BufferValid) == d_CCglo_BufferInvalid) )
  {
    /* ---------------------------------------------------------------------- */
    /*                                                                        */
    /*      C O P Y   D A T A   T O   O U T B U F F E R                       */
    /*                                                                        */
    /* ---------------------------------------------------------------------- */

    /* ---------------------------------------------------------------------- */
    /* Store the channel number                                               */
    /* ---------------------------------------------------------------------- */
    t_OutBuffer_PBUF[d_CCglo_ChannelNbr] = t_InBuffer_PBUF[d_CCglo_ChannelNbr];

    /* ---------------------------------------------------------------------- */
    /* Store the length                                                       */
    /* ---------------------------------------------------------------------- */
    t_OutBuffer_PBUF[d_CCglo_DLC] = t_InBuffer_PBUF[d_CCglo_DLC];


/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
/* --> Start of BUGFIX - V 1.01 - 30.07.2004                                  */
/* ---------------------------------------------------------------------------*/

    /*========================================================================*/
    /* -2- DLC of the received message is less or equal than the buffer size  */
    /* NOTE: This is to prevent a buffer overflow - copy not more than        */
    /*       maxbufferlength of bytes                                         */
    /*========================================================================*/
    if(t_InBuffer_PBUF[d_CCglo_DLC] <= t_InBuffer_PBUF[d_CCglo_MaxBufferLength])
    {
      /* Copy the received data length                                        */
      t_Counter_UB = t_InBuffer_PBUF[d_CCglo_DLC];
    } /* -2- END OF if(..) ---------------------------------------------------*/

    /*========================================================================*/
    /* -2- DLC of the received message is more than the buffer size           */
    /* NOTE: This is to prevent a buffer overflow - copy not more than        */
    /*       maxbufferlength of bytes                                         */
    /*========================================================================*/
    else
    {  
      /* Copy only as much data as the outbuffer could store                  */ 
      t_Counter_UB = t_InBuffer_PBUF[d_CCglo_MaxBufferLength];
    } /* -2- END OF else (if(..)) --------------------------------------------*/


    /*========================================================================*/
    /* -2- Copy x data                                                        */
    /*========================================================================*/
    for (t_Index_UB =  0x00;
         t_Index_UB <  t_Counter_UB;
         t_Index_UB ++ )
    {

/* --------------------------------------------------------------------------*/
/* <-- End of BUGFIX - V 1.01 - 30.07.2004                                   */
/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/

      FUN_CCtim_SystemTimerCheck_V();

      /*----------------------------------------------------------------------*/
      /* Copy the data from TxInBuffer to TxOutBuffer                         */
      /*----------------------------------------------------------------------*/
      t_OutBuffer_PBUF[ (d_CCglo_StartOfData + t_Index_UB) ] = \
      t_InBuffer_PBUF[  (d_CCglo_StartOfData + t_Index_UB) ];
      /* Init the InBuffer                                                    */
      t_InBuffer_PBUF[  (d_CCglo_StartOfData + t_Index_UB) ] = 0x00;

    } /* -2- END OF for (..) -------------------------------------------------*/

    /* ---------------------------------------------------------------------- */
    /* Set Inbuffer invalid                                                   */
    /* ---------------------------------------------------------------------- */
    t_InBuffer_PBUF[d_CCglo_BufferState]  &= d_CCglo_BufferInvalid;
    /* ---------------------------------------------------------------------- */
    /* Set Outbuffer valid                                                    */
    /* ---------------------------------------------------------------------- */
    t_OutBuffer_PBUF[d_CCglo_BufferState] |= d_CCglo_BufferValid;


    /* ---------------------------------------------------------------------- */
    /*                                                                        */
    /*      C M D   S T R U C T U R E   S E L E C T I O N                     */
    /*                                                                        */
    /* ---------------------------------------------------------------------- */
    /* ---------------------------------------------------------------------- */
    /* Init (2) - Init EvaluateionState                                       */
    /* ---------------------------------------------------------------------- */
    t_EvaluationState_UB = d_CCses_NoError;

    /* ---------------------------------------------------------------------- */
    /* Init (3) - Get channel number: [ (Subset) + (ChannelNumber) ]          */
    /* ---------------------------------------------------------------------- */
    /* NOTE: This is not necessary within a single channel system!            */
    /* t_Channel_UB =  t_InBuffer_PBUF[d_CCglo_ChannelNbr];                   */
    /* t_Channel_UB &= d_CCglo_ChannelMask;                                   */

    /* ---------------------------------------------------------------------- */
    /* Init (4) - Get the base address of command structure table             */
    /* ---------------------------------------------------------------------- */
    /* (1) Base address of command structure selection table (line) is not    */
    /*     necessary within a single channel system!                          */
    /* t_Index_UB = (t_Channel_UB * d_CCconsys_MaxSession);                   */
    /* (2) Offset for correct entry (column)                                  */
    t_Index_UB = SesCtrl_ST.CmdRunTimeInfo_PST[d_CCglo_Channel00].ActiveSession_UB;
    /* (3) Copy selected command structure base address                       */  
    SesCtrl_ST.CurrentCmdTable_PST = SesCtrl_ST.CmdTableEvaluation_PPST[t_Index_UB];

    /* ---------------------------------------------------------------------- */
    /*                                                                        */
    /*      L O O K   F O R   A   V A L I D   C M D                           */
    /*                                                                        */
    /*      Note: The regular command structure start at the third element.   */
    /* ---------------------------------------------------------------------- */
    SesCtrl_ST.SelectedCmdEntry_PST = \
          FUN_CCses_LookForCmd_PST ( t_OutBuffer_PBUF, \
             &SesCtrl_ST.CurrentCmdTable_PST[d_CCses_RequestLengthErrorOffset]);

    /* ---------------------------------------------------------------------- */
    /*                                                                        */
    /*      S T O R E   R U N T I M E I N F O                                 */
    /*                                                                        */
    /* ---------------------------------------------------------------------- */
    SesCtrl_ST.CmdRunTimeInfo_PST[d_CCglo_Channel00].CmdEntry_PST = \
                                               SesCtrl_ST.SelectedCmdEntry_PST;

    /*========================================================================*/
    /*                                                                        */
    /* -2- Request length check is necessary                                  */
    /*                                                                        */
    /*========================================================================*/
    if (SesCtrl_ST.SelectedCmdEntry_PST->ServiceRequestLength_UB != d_CCglo_DontCheckLength)
    {
      /*======================================================================*/
      /* -3- Length is not correct                                            */
      /*======================================================================*/
      if ( SesCtrl_ST.SelectedCmdEntry_PST->ServiceRequestLength_UB  != 
           t_InBuffer_PBUF[d_CCglo_DLC] )
      {
        /*--------------------------------------------------------------------*/
        /* Set selected command to the sequence error command.                */
        /*--------------------------------------------------------------------*/
        SesCtrl_ST.CmdRunTimeInfo_PST[d_CCglo_Channel00].CmdEntry_PST = \
            &SesCtrl_ST.CurrentCmdTable_PST[d_CCses_RequestLengthErrorOffset];

        /*--------------------------------------------------------------------*/
        /* Set internal error flag                                            */
        /*--------------------------------------------------------------------*/  
        t_EvaluationState_UB |= d_CCses_FlagRequestLengthError;

      } /* -3- END OF if(..) -------------------------------------------------*/
    } /* ---2- END OF if(..) -------------------------------------------------*/

    /*========================================================================*/
    /*                                                                        */
    /* -2- Command is not allowed in all sessions                             */
    /*                                                                        */
    /*========================================================================*/
    if ((d_CCglo_AllowedSessionMask & SesCtrl_ST.SelectedCmdEntry_PST->AllowedSessions_UB)\
        != d_CCglo_AllowedInAllSession)
    {
      /*======================================================================*/
      /* -3- Command is not allowed in current active session                 */
      /*                                                                      */
      /* NOTE: if a session flag is set and the result is zero we will have a */
      /*       session error                                                  */
      /*======================================================================*/
      if (((d_CCglo_AllowedSessionMask & SesCtrl_ST.SelectedCmdEntry_PST->AllowedSessions_UB) & \
          SesCtrl_ST.CmdRunTimeInfo_PST[d_CCglo_Channel00].AllowedSessions_UB) == 0)
      {
        /*--------------------------------------------------------------------*/
        /* Set selected command to the sequence error command.                */
        /*--------------------------------------------------------------------*/
        SesCtrl_ST.CmdRunTimeInfo_PST[d_CCglo_Channel00].CmdEntry_PST = \
            &SesCtrl_ST.CurrentCmdTable_PST[d_CCses_SessionErrorOffset];

        /*--------------------------------------------------------------------*/
        /* Set internal error flag                                            */
        /*--------------------------------------------------------------------*/
        t_EvaluationState_UB |= d_CCses_FlagRequestSessionError;
      } /* -3- END OF if(..) -------------------------------------------------*/
    } /* ---2- END OF if(..) -------------------------------------------------*/

    /*========================================================================*/
    /*                                                                        */
    /* -2- Command is not allowed with all addressing methods                 */
    /*                                                                        */
    /*========================================================================*/
    if ((d_CCglo_AddressAllowedMask & SesCtrl_ST.SelectedCmdEntry_PST->  \
                             AllowedSessions_UB) != d_CCglo_AllAddressAllowed)
    {

      /*----------------------------------------------------------------------*/
      /* init scan variables                                                  */
      /*----------------------------------------------------------------------*/
      t_Index_UB = 0x00;
      t_ScanState_UB = d_CCses_ScanRunning;

      /*======================================================================*/
      /* -3- scan Structure for the correct channel number                    */
      /*======================================================================*/
      while ( (d_CCses_ScanRunning == t_ScanState_UB )  &&
              ( d_CCglo_NoEntry != SesCtrl_ST.AddrModeCfg_PST  \
                                         [t_Index_UB].AddressMode_UB ) )
      {
        /*====================================================================*/
        /* -4- Channel number found                                           */
        /*====================================================================*/
        if ( t_InBuffer_PBUF[d_CCglo_ChannelNbr] == SesCtrl_ST. \
                              AddrModeCfg_PST[t_Index_UB].ChannelNbr_UB )
        {
          /*------------------------------------------------------------------*/
          /* stop scan                                                        */
          /*------------------------------------------------------------------*/
          t_ScanState_UB   = d_CCses_StopScan;

        } /* -4- END OF if(..) -----------------------------------------------*/

        /*====================================================================*/
        /* -4- Channel number not found                                       */
        /*====================================================================*/
        else
        {
          /*------------------------------------------------------------------*/
          /* Check next entry                                                 */
          /*------------------------------------------------------------------*/
          t_Index_UB += 0x01;

        } /* -4- END OF else (if(..)) ----------------------------------------*/
      } /* ---3- END OF while (..) -------------------------------------------*/
      
      /*======================================================================*/
      /* -3- Command is not allowed with the actual adressing method          */
      /*                                                                      */
      /* NOTE: if a addressing flag is set and the result is zero we will     */
      /*       have an adressing error                                        */
      /*======================================================================*/
      if ( ( (d_CCglo_AddressAllowedMask & SesCtrl_ST.SelectedCmdEntry_PST->AllowedSessions_UB) & \
        SesCtrl_ST.AddrModeCfg_PST[t_Index_UB].AddressMode_UB) == 0) 
      {
        /*--------------------------------------------------------------------*/
        /* Set selected command to the addressing error command.              */
        /*--------------------------------------------------------------------*/
        SesCtrl_ST.CmdRunTimeInfo_PST[d_CCglo_Channel00].CmdEntry_PST = \
            &SesCtrl_ST.CurrentCmdTable_PST[d_CCses_AddressingErrorOffset];

        /*--------------------------------------------------------------------*/
        /* Set internal error flag                                            */
        /*--------------------------------------------------------------------*/
        t_EvaluationState_UB |= d_CCses_FlagRequestAddressingError;

      } /* -3- END OF if(..) -------------------------------------------------*/
    } /* ---2- END OF if(..) -------------------------------------------------*/

    /*======================================================================*/
    /* -2- Command is not allowed with the actual adressing method and a    */
    /*     suppressed response                                              */
    /*                                                                      */
    /*======================================================================*/
    if ((SesCtrl_ST.SelectedCmdEntry_PST->LocalID_UB != d_CCglo_NoEntry ) && \
        (d_CCglo_Byte01 == SesCtrl_ST.SelectedCmdEntry_PST->LIDPosition_UB))
    {
       /*======================================================================*/
       /* -3- positiv response shall be suppressed                             */
       /*======================================================================*/
       if( d_CCglo_LIDSuppressPosRsp == (d_CCglo_LIDSuppressPosRspMask &  \
          t_OutBuffer_PBUF[d_CCglo_StartOfData + (UBYTE) 0x01u]) )
       {
        /*---------------------------------------------------------------------*/
        /* check the addressing mode for functional addressing                 */
        /*---------------------------------------------------------------------*/
        t_AddrMode_UB = FUN_CCkwpaudi_CheckAddrMode_UB  \
                                 (t_OutBuffer_PBUF, d_CCglo_FuncAddressAllowed);

        /*=========================================================================*/
        /* -4- service is functionally addressed                                   */
        /*=========================================================================*/
        if ( d_CCglo_Ready != t_AddrMode_UB )
        {
          /*---------------------------------------------------------------------*/
          /* Wrong addressing AND no response required                           */
          /*---------------------------------------------------------------------*/
          /*--------------------------------------------------------------------*/
          /* Set selected command to the addressing error command.              */
          /*--------------------------------------------------------------------*/
          SesCtrl_ST.CmdRunTimeInfo_PST[d_CCglo_Channel00].CmdEntry_PST = \
            &SesCtrl_ST.CurrentCmdTable_PST[d_CCses_RequestLengthErrorOffset];

          /*--------------------------------------------------------------------*/
          /* Set internal error flag                                            */
          /*--------------------------------------------------------------------*/
          t_EvaluationState_UB |= d_CCses_FlagRequestWrongAddrNoRespError;
        } /* end of -4- */
      } /* end of -3- */
    } /* end of -2- */
    /* ---------------------------------------------------------------------- */
    /*                                                                        */
    /*      S & K   C H E C K                                                 */
    /*                                                                        */
    /* ---------------------------------------------------------------------- */

    /*========================================================================*/
    /* -2- Security Check is possible                                         */
    /*========================================================================*/
    if ( d_CCses_NoError == t_EvaluationState_UB )
    {
      /*======================================================================*/
      /* -3- Security Access check required?                                  */
      /*======================================================================*/
      if (d_CCglo_SecurityAccessRequired == \
         (SesCtrl_ST.SelectedCmdEntry_PST->AllowedSessions_UB & d_CCglo_SecurityAccessRequired))
      {
        /*----------------------------------------------------------------------*/
        /* Get the current Security Access status from KWP2000 modul            */
        /*----------------------------------------------------------------------*/

        /*======================================================================*/
        /* -4- Security access is denied?                                       */
        /*======================================================================*/
        if ( 0x80u == FUN_CCkwpaudi_GetSecurityLevel_UB())
        {
          /*--------------------------------------------------------------------*/
          /* Set selected command to the sequence error command.                */
          /*--------------------------------------------------------------------*/
          SesCtrl_ST.CmdRunTimeInfo_PST[d_CCglo_Channel00].CmdEntry_PST = \
            &SesCtrl_ST.CurrentCmdTable_PST[d_CCses_SecurityAccessErrorOffset];
          t_EvaluationState_UB |= d_CCses_FlagRequestSecurityAccessError;

        } /* -4- END OF if(..) -------------------------------------------------*/
      } /* ---3- END OF if(..) -------------------------------------------------*/
    } /* -----2- END OF if(..) -------------------------------------------------*/
     
/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
#ifdef cs_CCtypes_32BitSequence /* -2-                                        */
/*----------------------------------------------------------------------------*/

    /* ---------------------------------------------------------------------- */
    /*                                                                        */
    /*      S E Q U E N C E   C H E C K                                       */
    /*                                                                        */
    /* ---------------------------------------------------------------------- */

    /*========================================================================*/
    /* -2- Sequence Check is possible                                         */
    /*========================================================================*/
    if ( d_CCses_NoError == t_EvaluationState_UB )
    {
      /*----------------------------------------------------------------------*/
      /* Init (5) - Get the current PreConditionTable (PCT) line              */
      /*----------------------------------------------------------------------*/
      t_PreCondition_UL = (ULONG) *SesCtrl_ST.SelectedCmdEntry_PST->PreCondCheck_PUL;

      /*----------------------------------------------------------------------*/
      /* Init (6) - Init the current condition check variable                 */
      /*----------------------------------------------------------------------*/
      t_CurrentCondition_UL = 0x80000000ul;

      /*======================================================================*/
      /* -3- Shift bit to correct position                                    */
      /*======================================================================*/
      for (t_Index_UB = 0x00;
           t_Index_UB < SesCtrl_ST.ActCondition_AUB[d_CCglo_Channel00];
           t_Index_UB ++ )
      {
        /* Shift the Bit                                                      */
        t_CurrentCondition_UL >>= 0x01;

      } /* -3- END OF for(..) ------------------------------------------------*/

      /*======================================================================*/
      /* -3- Sequence is not correct                                          */
      /*======================================================================*/
      if ( 0x00000000UL == (t_CurrentCondition_UL & t_PreCondition_UL))
      {
        /*--------------------------------------------------------------------*/
        /* Set selected command to the sequence error command.                */
        /*--------------------------------------------------------------------*/
        SesCtrl_ST.CmdRunTimeInfo_PST[d_CCglo_Channel00].CmdEntry_PST = \
          &SesCtrl_ST.CurrentCmdTable_PST[d_CCses_SequenceErrorOffset];

      } /* -3- END OF if(..) -------------------------------------------------*/
    } /* ---2- END OF if(..) -------------------------------------------------*/

/*----------------------------------------------------------------------------*/
#endif  /* -2- cs_CCtypes_32BitSequence                                       */
/*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
#ifdef cs_CCtypes_16BitSequence /* -2-                                        */
/*----------------------------------------------------------------------------*/

    /* ---------------------------------------------------------------------- */
    /*                                                                        */
    /*      S E Q U E N C E   C H E C K                                       */
    /*                                                                        */
    /* ---------------------------------------------------------------------- */

    /*========================================================================*/
    /* -2- Sequence Check is possible                                         */
    /*========================================================================*/
    if ( d_CCses_NoError == t_EvaluationState_UB )
    {
      /*----------------------------------------------------------------------*/
      /* Init (5) - Get the current PreConditionTable (PCT) line              */
      /*----------------------------------------------------------------------*/
      t_PreCondition_UW = (UWORD) *SesCtrl_ST.SelectedCmdEntry_PST->PreCondCheck_PUW;

      /*----------------------------------------------------------------------*/
      /* Init (6) - Init the current condition check variable                 */
      /*----------------------------------------------------------------------*/
      t_CurrentCondition_UW = 0x8000u;

      /*======================================================================*/
      /* -3- Shift bit to correct position                                    */
      /*======================================================================*/
      for (t_Index_UB = 0x00;
           t_Index_UB < SesCtrl_ST.ActCondition_AUB[d_CCglo_Channel00];
           t_Index_UB ++ )
      {
        /* Shift the Bit                                                      */
        t_CurrentCondition_UW >>= 0x01;

      } /* -3- END OF for(..) ------------------------------------------------*/

      /*======================================================================*/
      /* -3- Sequence is not correct                                          */
      /*======================================================================*/
      if ( 0x0000u == (t_CurrentCondition_UW & t_PreCondition_UW))
      {
        /*--------------------------------------------------------------------*/
        /* Set selected command to the sequence error command.                */
        /*--------------------------------------------------------------------*/
        SesCtrl_ST.CmdRunTimeInfo_PST[d_CCglo_Channel00].CmdEntry_PST = \
          &SesCtrl_ST.CurrentCmdTable_PST[d_CCses_SequenceErrorOffset];

      } /* -3- END OF if(..) -------------------------------------------------*/
    } /* ---2- END OF if(..) -------------------------------------------------*/

/*----------------------------------------------------------------------------*/
#endif  /* -2- cs_CCtypes_16BitSequence                                       */
/*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    /* ---------------------------------------------------------------------- */
    /* Set the result                                                         */
    /* ---------------------------------------------------------------------- */
    t_Result_UB |= d_CCglo_Ready;
  
  } /* ---1- END OF if (..) --------------------------------------------------*/

  /*--------------------------------------------------------------------------*/
  /* return                                                                   */
  /*--------------------------------------------------------------------------*/
  return ( t_Result_UB);

} /*-+- END OF FUNCTION 'FUN_CCses_RxSession_UB' -+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/





/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CCses_TxSession_UB                                      |
|                                                                              |
|  Prototype at:   ADLATUS_SessionHandler_tdf.h                                |
|                                                                              |
|  Description:    This function copys the correct response into the TxOut-    |
|                  Buffer and provides the state machine process.              |
|                                                                              |
|  Parameter:      PTR_BUFFER_UB t_InBuffer_PBUF  .. Ptr to Inputbuffer        |
|                  PTR_BUFFER_UB t_OutBuffer_PBUF .. Ptr to Outputbuffer       |
|                                                                              |
|  Return Value:   UBYTE  t_Result_UB                                          |
|                                                                              |
|                         R B E I  I I I I                                     |
|                         | | | |  | | | |                                     |
|                         | | | +--+-+-+-+---    INDEX                         |
|                         | | +--------------    ERROR                         |
|                         | +----------------    BUSY                          |
|                         +------------------    READY                         |
|                                                                              |
+-----------------------------------------------------------------------------*/
UBYTE FUN_CCses_TxSession_UB ( PTR_BUFFER_UB t_InBuffer_PBUF, \
                               PTR_BUFFER_UB t_OutBuffer_PBUF )
{

  UBYTE   t_Result_UB;             /* 1 Byte - Result of function             */
  UBYTE   t_ResponseIndex_UB;      /* 1 Byte - Resp Index from RunTimeInfo    */
  UBYTE   t_TempIndex_UB;          /* 1 Byte - Temp variable                  */
  UBYTE   t_Offset_UB;             /* 1 Byte - Offset for response selection  */
  FAR_PTR_UBYTE  t_ResponseCodeTable_PUB; /* 4 Byte - Ptr to Response Code           */
  /* ------------------------------------------------------------------------ */


  /* ------------------------------------------------------------------------ */
  /* Init (1) - Init Result                                                   */
  /* ------------------------------------------------------------------------ */
  t_Result_UB = d_CCglo_Idle;

  /*==========================================================================*/
  /* -1- Inbuffer is valid and Outbuffer is invalid                           */
  /*==========================================================================*/
  if ( ((t_InBuffer_PBUF[d_CCglo_BufferState]  & d_CCglo_BufferValid ) == d_CCglo_BufferValid ) &&
       ((t_OutBuffer_PBUF[d_CCglo_BufferState] & d_CCglo_BufferValid ) != d_CCglo_BufferValid ) )
  {

    /* ---------------------------------------------------------------------- */
    /*                                                                        */
    /*      S E L E C T   T H E   C O R R E C T   E N T R Y   O F   T H E     */
    /*      R E S P O N S E   C O D E   T A B L E  ( RCT )                    */
    /*                                                                        */
    /* ---------------------------------------------------------------------- */

    /* ---------------------------------------------------------------------- */
    /* Init (2) - Get channel number: [ (Subset) + (ChannelNumber) ]          */
    /* ---------------------------------------------------------------------- */
    /* NOTE: This is not necessary within a single channel system             */
    /* t_Channel_UB =  t_InBuffer_PBUF[d_CCglo_ChannelNbr];                   */
    /* t_Channel_UB &= d_CCglo_ChannelMask;                                   */

    /* The next line is only to prevent a compiler warning!!                  */

    /*------------------------------------------------------------------------*/
    /* Init (3) - Get the response index                                      */
    /*------------------------------------------------------------------------*/
    t_ResponseIndex_UB  = SesCtrl_ST.CmdRunTimeInfo_PST[d_CCglo_Channel00].ResponseIndex_UB;

    /*------------------------------------------------------------------------*/
    /* Init (4) - Init t_TempIndex_UB will be done later                      */
    /*------------------------------------------------------------------------*/

    /*------------------------------------------------------------------------*/
    /* Init (5) - Init offset information                                     */
    /*------------------------------------------------------------------------*/
    t_Offset_UB = 0x00;

    /*------------------------------------------------------------------------*/
    /* Init (6) - Init base address of the current Response Code Table        */
    /*------------------------------------------------------------------------*/
    t_ResponseCodeTable_PUB = \
      (FAR_PTR_UBYTE) SesCtrl_ST.CmdRunTimeInfo_PST[d_CCglo_Channel00].CmdEntry_PST->ResponseCodeTable_PUB;

    /*========================================================================*/
    /* -2- Calculate the offset for the correct response entry from table     */
    /*========================================================================*/
    for (t_TempIndex_UB =  0x00;
         t_TempIndex_UB <  t_ResponseIndex_UB;
         t_TempIndex_UB ++ )
    {
      /*----------------------------------------------------------------------*/
      /*                                                                      */
      /*                            +-------------------------------------+   */
      /*                            |   Response Code Table               |   */
      /*                            +-------------------------------------+   */
      /*  Byte [Offset + 0]         | PostCondition                       |   */
      /*                            +-------------------------------------+   */
      /*  Byte [Offset + 1]         | Response Length   ( N-Byte)         |   */
      /*                            +-------------------------------------+   */
      /*  Byte [Offset + 1 + 1]     | Response Byte 0   (first one)       |   */
      /*                            +-------------------------------------+   */
      /*  ...                       ...                                       */
      /*                            +-------------------------------------+   */
      /*  Byte [Offset + 1 + N]     | Response Byte N-1  (last one)       |   */
      /*                            +-------------------------------------+   */
      /*  Byte [Offset + 1 + N + 1] | Next Entry                          |   */
      /*                            +-------------------------------------+   */
      /*                                                                      */
      /*----------------------------------------------------------------------*/
      /* Set Offset to Response length entry                                  */
      /*----------------------------------------------------------------------*/
      t_Offset_UB += (UBYTE) 0x01;
      /*----------------------------------------------------------------------*/
      /* Add length of this response                                          */
      /*----------------------------------------------------------------------*/
      t_Offset_UB += t_ResponseCodeTable_PUB[t_Offset_UB];
      /*----------------------------------------------------------------------*/
      /* Add one for the next entry of table                                  */
      /*----------------------------------------------------------------------*/
      t_Offset_UB += (UBYTE) 0x01;

    } /* -2- END OF for(..) --------------------------------------------------*/


    /* ---------------------------------------------------------------------- */
    /*                                                                        */
    /*      S W I T C H   S T A T E M A C H I N  E                            */
    /*                                                                        */
    /* ---------------------------------------------------------------------- */

    /*========================================================================*/
    /* -2- Postcondtion is defined                                            */
    /*========================================================================*/
    if ( d_CCglo_NoPostCondDefined != (t_ResponseCodeTable_PUB[t_Offset_UB] & d_CCglo_NoPostCondDefined) )
    {
      /* if we have a negative response -- allow a reset */
      if ( d_CCglo_NegativeResponse != (d_CCglo_NegativeResponse & t_ResponseCodeTable_PUB[t_Offset_UB] ))
      {
        /****************************************************************/
        /* no reset of the sequence -- keep the system in the actual    */
        /* state an ddo not reset the postcondition                     */
        /****************************************************************/ 
        /*----------------------------------------------------------------------*/
        /* Set postcondition                                                    */
        /*----------------------------------------------------------------------*/
        SesCtrl_ST.ActCondition_AUB[d_CCglo_Channel00] = \
                                   t_ResponseCodeTable_PUB[t_Offset_UB];
      }
    } /* -2- END OF if(..) ---------------------------------------------------*/

    /* ---------------------------------------------------------------------- */
    /*                                                                        */
    /*      C O P Y   T H E   R E S P O N S E   T O   O U T B U F F E R       */
    /*                                                                        */
    /* ---------------------------------------------------------------------- */

    /*------------------------------------------------------------------------*/
    /* Store the channel number from  TxInBuffer to TxOutBuffer               */
    /*------------------------------------------------------------------------*/
    t_OutBuffer_PBUF[d_CCglo_ChannelNbr] = t_InBuffer_PBUF[d_CCglo_ChannelNbr];

    /*========================================================================*/
    /* -2- Copy Data from response table                                      */
    /*     Note: The length is the second entry of a response code table set  */
    /*           --> ( 1 + offset )                                           */
    /*========================================================================*/
    if ( 0x00 != t_ResponseCodeTable_PUB[(0x01 + t_Offset_UB)])
    {
      /*----------------------------------------------------------------------*/
      /* Store the data length for TxOutBuffer                                */
      /*----------------------------------------------------------------------*/
      t_OutBuffer_PBUF[d_CCglo_DLC] = t_ResponseCodeTable_PUB[(0x01 + t_Offset_UB)];

      /*======================================================================*/
      /* -3- Copy x data                                                      */
      /*     Note: The data starts on the third entry of a response code      */
      /*           table set.  --> ( 2 + offset )                             */
      /*======================================================================*/
      for (t_TempIndex_UB =  0x00;
           t_TempIndex_UB <  t_OutBuffer_PBUF[d_CCglo_DLC];
           t_TempIndex_UB ++ )
      {
        /*--------------------------------------------------------------------*/
        /* Copy the data from ResponseCodeTable to TxOutBuffer                */
        /*--------------------------------------------------------------------*/
        t_OutBuffer_PBUF[ (d_CCglo_StartOfData + t_TempIndex_UB) ] = \
                         t_ResponseCodeTable_PUB[(0x02 + t_Offset_UB)];
        /*--------------------------------------------------------------------*/
        /* Increment the offset for the next data byte                        */
        /*--------------------------------------------------------------------*/
        t_Offset_UB += 0x01;

      } /* -3- END OF for (..) -----------------------------------------------*/

      /*----------------------------------------------------------------------*/
      /* Set the Buffer state information                                     */
      /*----------------------------------------------------------------------*/
      /* InBuffer is invalid                                                  */
      t_InBuffer_PBUF[d_CCglo_BufferState]  &= (UBYTE) ~d_CCglo_BufferValid;
      /* OutBuffer is valid                                                   */
      t_OutBuffer_PBUF[d_CCglo_BufferState] |= d_CCglo_BufferValid; 

      /* -------------------------------------------------------------------- */
      /*                                                                      */
      /*      S E T   R E T U R N   I N F O R M A T I O N                     */
      /*                                                                      */
      /* -------------------------------------------------------------------- */
      t_Result_UB |= d_CCglo_Ready;

    } /* ---2- END OF if (..)  -----------------------------------------------*/

    /*========================================================================*/
    /* -2- Copy Data direct from TxInputBuffer                                */
    /*========================================================================*/
    else
    {
      /*======================================================================*/
      /* -3- there is data in the TxInputBuffer                               */
      /*======================================================================*/
      if ( 0x00 != t_InBuffer_PBUF[d_CCglo_DLC] )
      {
        /*--------------------------------------------------------------------*/
        /* Store the data length for TxOutBuffer                              */
        /*--------------------------------------------------------------------*/
        t_OutBuffer_PBUF[d_CCglo_DLC] = t_InBuffer_PBUF[d_CCglo_DLC];

        /*====================================================================*/
        /* -4- Copy x data                                                    */
        /*====================================================================*/
        for (t_TempIndex_UB =  0x00;
             t_TempIndex_UB <  t_OutBuffer_PBUF[d_CCglo_DLC];
             t_TempIndex_UB ++ )
        {
          /*------------------------------------------------------------------*/
          /* Copy the data from TxInBuffer to TxOutBuffer                     */
          /*------------------------------------------------------------------*/
          t_OutBuffer_PBUF[ (d_CCglo_StartOfData + t_TempIndex_UB) ] = \
          t_InBuffer_PBUF[  (d_CCglo_StartOfData + t_TempIndex_UB) ];
          /* Init the InBuffer                                                */
          t_InBuffer_PBUF[  (d_CCglo_StartOfData + t_TempIndex_UB) ] = 0x00;

          FUN_CCtim_SystemTimerCheck_V();

        } /* -4- END OF for (..) ---------------------------------------------*/

        /*--------------------------------------------------------------------*/
        /* Set the Buffer state information                                   */
        /*--------------------------------------------------------------------*/
        /* InBuffer is invalid                                                */
        t_InBuffer_PBUF[d_CCglo_BufferState]  &= (UBYTE) ~d_CCglo_BufferValid;
        /* OutBuffer is valid                                                 */
        t_OutBuffer_PBUF[d_CCglo_BufferState] |= d_CCglo_BufferValid;

        /* ------------------------------------------------------------------ */
        /*                                                                    */
        /*      S E T   R E T U R N   I N F O R M A T I O N                   */
        /*                                                                    */
        /* ------------------------------------------------------------------ */
        t_Result_UB |= d_CCglo_Ready;

      } /* ---3- END OF if (..) ----------------------------------------------*/
      /*======================================================================*/
      /* -3- there is no data in the TxInputBuffer, no aswer shall be sent    */
      /*     for this service                                                 */
      /*======================================================================*/
      else
      {
        /*--------------------------------------------------------------------*/
        /* Set the Buffer state information                                   */
        /*--------------------------------------------------------------------*/
        /* InBuffer is invalid                                                */
        t_InBuffer_PBUF[d_CCglo_BufferState]  &= (UBYTE) ~d_CCglo_BufferValid;
        /* OutBuffer is valid                                                 */
        t_OutBuffer_PBUF[d_CCglo_BufferState] &= (UBYTE) ~d_CCglo_BufferValid;

        /* ------------------------------------------------------------------ */
        /*                                                                    */
        /*      S E T   R E T U R N   I N F O R M A T I O N                   */
        /*                                                                    */
        /* ------------------------------------------------------------------ */
        t_Result_UB |= d_CCglo_Idle;

      } /* ---3- END OF else (if (..)) ---------------------------------------*/
    } /* -----2- END OF else if (..) -----------------------------------------*/
  } /* -------1- END OF if(..) -----------------------------------------------*/

  return ( t_Result_UB );

} /*-+- END OF FUNCTION 'FUN_CCses_TxSession_UB' -+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/


/*----------------------------------------------------------------------------*/
#endif /* -1- cs_CCsys_SingleChannelSystem                                    */
/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
/*                                                                            */
/*      E N D   O F   S I N G L E   C H A N N E L   S Y S T E M               */
/*                                                                            */
/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */










/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */
/*                                                                            */
/*      M U L T I   C H A N N E L   S Y S T E M                               */
/*                                                                            */
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */
#ifdef cs_CCsys_MultiChannelSystem /* -1-                                     */
/*----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CCses_InitChannel_V                                     |
|                                                                              |
|  Prototype at:   ADLATUS_SessionHandler_tdf.h                                |
|                                                                              |
|  Description:    Initialization of the session layer.                        |
|                                                                              |
|  Parameter:      UBYTE  t_ChannelNbr_UB    .. Channel which should be init   |
|                  UBYTE  t_ActiveSession_UB .. Currently active session       |
|                  UBYTE  t_InitHandle_UB    .. 0x00 ResetChannel              |
|                                               0x01 FlashRequest              |
|                                                                              |
|  Return Value:    --                                                         |
|                                                                              |
+-----------------------------------------------------------------------------*/
void FUN_CCses_InitChannel_V ( UBYTE t_Channel_UB,       \
                               UBYTE t_ActiveSession_UB, \
                               UBYTE t_InitHandle_UB     )
{

  /*--------------------------------------------------------------------------*/
  /*                                                                          */
  /*  N .. d_CCconsys_MaxSession                                              */
  /*  Y .. d_CCconsys_NbrOfLogChannel                                         */
  /*                                                                          */
  /*             SessionInitTable:                                            */
  /*             +----------------+----------------+-----+----------------+   */
  /*             | Session 0      | Session 1      | ... | Session N-1    |   */
  /*             +----------------+----------------+-----+----------------+   */
  /* Channel 0   | PtrToInitTable | PtrToInitTable | ... | PtrToInitTable |   */
  /*             +----------------+----------------+-----+----------------+   */
  /* Channel 1   | PtrToInitTable | PtrToInitTable | ... | PtrToInitTable |   */
  /*             +----------------+----------------+-----+----------------+   */
  /* ...         | ...            | ...            | ... | ...            |   */
  /*             +----------------+----------------+-----+----------------+   */
  /* Channel Y-1 | PtrToInitTable | PtrToInitTable | ... | PtrToInitTable |   */
  /*             +----------------+----------------+-----+----------------+   */
  /*                                                                          */
  /*               PtrToInitTable >-+                                         */
  /*                                |                                         */
  /*                                +--> +----------------+---------------+   */
  /*                     Reset Channel   | Active Session | InitCondition |   */
  /*                                     +----------------+---------------+   */
  /*                     Flash Request   | Active Session | InitCondition |   */
  /*                                     +----------------+---------------+   */
  /*                                                                          */
  /*--------------------------------------------------------------------------*/
  UBYTE           t_Offset_UB;
  const COMMANDINIT_ST* t_SessionInitTable_PST;

  /*--------------------------------------------------------------------------*/
  /* Get the correct address to Session Init Table entry                      */
  /*--------------------------------------------------------------------------*/
  /* (1) Calculate the offset for the correct channel CommandInitTable        */
  t_Offset_UB = (t_Channel_UB * d_CCconsys_MaxSession);
  /* (2) Offset for correct entry                                             */
  t_Offset_UB += t_ActiveSession_UB;
  /* (3) Copy selected command structure base address                         */
  t_SessionInitTable_PST = (const COMMANDINIT_ST*) SesCtrl_ST.CmdInitTable_PPST[t_Offset_UB];

  /*--------------------------------------------------------------------------*/
  /* Init ControlStructureEntry 5: Command Runtime Info of each channel       */
  /*--------------------------------------------------------------------------*/
  SesCtrl_ST.CmdRunTimeInfo_PST[t_Channel_UB].ActiveSession_UB = \
            t_SessionInitTable_PST[t_InitHandle_UB].ActiveSession_UB;

  SesCtrl_ST.CmdRunTimeInfo_PST[t_Channel_UB].AllowedSessions_UB = \
            t_SessionInitTable_PST[t_InitHandle_UB].AllowedSessions_UB;

  /*--------------------------------------------------------------------------*/
  /* Init ControlStructureEntry 6: Session Control Variable for each channel  */
  /*--------------------------------------------------------------------------*/
  SesCtrl_ST.ActCondition_AUB[t_Channel_UB] = \
              t_SessionInitTable_PST[t_InitHandle_UB].InitCondition_UB;

  return;

} /*-+- END OF FUNCTION 'FUN_CCses_InitChannel_V' -+-+-+-+-+-+-+-+-+-+-+-+-+-+*/




/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CCses_RxSession_UB                                      |
|                                                                              |
|  Prototype at:   ADLATUS_SessionHandler_tdf.h                                |
|                                                                              |
|  Description:    This function selects the correct command entry from        |
|                  structure and checks the correct execution conditions.      |
|                                                                              |
|  Parameter:      PTR_BUFFER_UB t_InBuffer_PBUF  .. Ptr to Inputbuffer        |
|                  PTR_BUFFER_UB t_OutBuffer_PBUF .. Ptr to Outputbuffer       |
|                                                                              |
|  Return Value:   UBYTE  t_Result_UB                                          |
|                                                                              |
|                         R B E I  I I I I                                     |
|                         | | | |  | | | |                                     |
|                         | | | +--+-+-+-+---    INDEX                         |
|                         | | +--------------    ERROR                         |
|                         | +----------------    BUSY                          |
|                         +------------------    READY                         |
|                                                                              |
+-----------------------------------------------------------------------------*/
UBYTE FUN_CCses_RxSession_UB ( PTR_BUFFER_UB t_InBuffer_PBUF, \
                               PTR_BUFFER_UB t_OutBuffer_PBUF )
{

  UBYTE t_Result_UB;           /* 1 Byte - Return Value of the routine        */
  UBYTE t_EvaluationState_UB;  /* 1 Byte - EvaluationState of the function    */
  UBYTE t_Channel_UB;          /* 1 Byte - Current channel number             */
  UBYTE t_Index_UB;            /* 1 Byte - Indexvariable for differend scans  */
  
/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
/* --> Start of BUGFIX - V 1.01 - 30.07.2004                                  */
/* ---------------------------------------------------------------------------*/
  UBYTE t_Counter_UB;
/* ---------------------------------------------------------------------------*/
/* <-- End of BUGFIX - V 1.01 - 30.07.2004                                    */
/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
#ifdef cs_CCtypes_32BitSequence /* -2-                                        */
/*----------------------------------------------------------------------------*/
  ULONG t_PreCondition_UL;     /* 4 Byte - PreCondition entry from PCT        */
  ULONG t_CurrentCondition_UL; /* 4 Byte - Cur. Condition variable for check  */
/*----------------------------------------------------------------------------*/
#endif  /* -2- cs_CCtypes_32BitSequence                                       */
/*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
#ifdef cs_CCtypes_16BitSequence /* -2-                                        */
/*----------------------------------------------------------------------------*/
  UWORD t_PreCondition_UW;     /* 2 Byte - PreCondition entry from PCT        */
  UWORD t_CurrentCondition_UW; /* 2 Byte - Cur. Condition variable for check  */
/*----------------------------------------------------------------------------*/
#endif  /* -2- cs_CCtypes_16BitSequence                                       */
/*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/


  /* ------------------------------------------------------------------------ */
  /* Init (1) - Init Result                                                   */
  /* ------------------------------------------------------------------------ */
  t_Result_UB = d_CCglo_Idle;

  /*==========================================================================*/
  /*                                                                          */
  /* -1- InBuffer is valid / OutBuffer is invalid                             */
  /*                                                                          */
  /*==========================================================================*/
  if( ((t_InBuffer_PBUF[d_CCglo_BufferState]  & d_CCglo_BufferValid) == d_CCglo_BufferValid) &&
      ((t_OutBuffer_PBUF[d_CCglo_BufferState] & d_CCglo_BufferValid) == d_CCglo_BufferInvalid) )
  {
    /* ---------------------------------------------------------------------- */
    /*                                                                        */
    /*      C O P Y   D A T A   T O   O U T B U F F E R                       */
    /*                                                                        */
    /* ---------------------------------------------------------------------- */

    /* ---------------------------------------------------------------------- */
    /* Store the channel number                                               */
    /* ---------------------------------------------------------------------- */
    t_OutBuffer_PBUF[d_CCglo_ChannelNbr] = t_InBuffer_PBUF[d_CCglo_ChannelNbr];

    /* ---------------------------------------------------------------------- */
    /* Store the length                                                       */
    /* ---------------------------------------------------------------------- */
    t_OutBuffer_PBUF[d_CCglo_DLC] = t_InBuffer_PBUF[d_CCglo_DLC];

/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
/* --> Start of BUGFIX - V 1.01 - 30.07.2004                                  */
/* ---------------------------------------------------------------------------*/

    /*========================================================================*/
    /* -2- DLC of the received message is less or equal than the buffer size  */
    /* NOTE: This is to prevent a buffer overflow - copy not more than        */
    /*       maxbufferlength of bytes                                         */
    /*========================================================================*/
    if(t_InBuffer_PBUF[d_CCglo_DLC] <= t_InBuffer_PBUF[d_CCglo_MaxBufferLength])
    {
      /* Copy the received data length                                        */
      t_Counter_UB = t_InBuffer_PBUF[d_CCglo_DLC];
    } /* -2- END OF if(..) ---------------------------------------------------*/

    /*========================================================================*/
    /* -2- DLC of the received message is more than the buffer size           */
    /* NOTE: This is to prevent a buffer overflow - copy not more than        */
    /*       maxbufferlength of bytes                                         */
    /*========================================================================*/
    else
    {  
      /* Copy only as much data as the outbuffer could store                  */ 
      t_Counter_UB = t_InBuffer_PBUF[d_CCglo_MaxBufferLength];
    } /* -2- END OF else (if(..)) --------------------------------------------*/


    /*========================================================================*/
    /* -2- Copy x data                                                        */
    /*========================================================================*/
    for (t_Index_UB =  0x00;
         t_Index_UB <  t_Counter_UB;
         t_Index_UB ++ )
    {

/* ---------------------------------------------------------------------------*/
/* <-- End of BUGFIX - V 1.01 - 30.07.2004                                    */
/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/

      /*----------------------------------------------------------------------*/
      /* Copy the data from TxInBuffer to TxOutBuffer                         */
      /*----------------------------------------------------------------------*/
      t_OutBuffer_PBUF[ (d_CCglo_StartOfData + t_Index_UB) ] = \
      t_InBuffer_PBUF[  (d_CCglo_StartOfData + t_Index_UB) ];
      /* Init the InBuffer                                                    */
      t_InBuffer_PBUF[  (d_CCglo_StartOfData + t_Index_UB) ] = 0x00;

    } /* -2- END OF for (..) -------------------------------------------------*/

    /* ---------------------------------------------------------------------- */
    /* Set Inbuffer invalid                                                   */
    /* ---------------------------------------------------------------------- */
    t_InBuffer_PBUF[d_CCglo_BufferState]  &= d_CCglo_BufferInvalid;
    /* ---------------------------------------------------------------------- */
    /* Set Outbuffer valid                                                    */
    /* ---------------------------------------------------------------------- */
    t_OutBuffer_PBUF[d_CCglo_BufferState] |= d_CCglo_BufferValid;


    /* ---------------------------------------------------------------------- */
    /*                                                                        */
    /*      C M D   S T R U C T U R E   S E L E C T I O N                     */
    /*                                                                        */
    /* ---------------------------------------------------------------------- */
    /* ---------------------------------------------------------------------- */
    /* Init (2) - Init EvaluateionState                                       */
    /* ---------------------------------------------------------------------- */
    t_EvaluationState_UB = d_CCses_NoError;

    /* ---------------------------------------------------------------------- */
    /* Init (3) - Get channel number: [ (Subset) + (ChannelNumber) ]          */
    /* ---------------------------------------------------------------------- */
    t_Channel_UB =  t_InBuffer_PBUF[d_CCglo_ChannelNbr];
    t_Channel_UB &= d_CCglo_ChannelMask;

    /* ---------------------------------------------------------------------- */
    /* Init (4) - Get the base address of command structure table             */
    /* ---------------------------------------------------------------------- */
    /* (1) Base address of command structure selection table (line)           */
    t_Index_UB = (t_Channel_UB * d_CCconsys_MaxSession);
    /* (2) Offset for correct entry (column)                                  */
    t_Index_UB += SesCtrl_ST.CmdRunTimeInfo_PST[t_Channel_UB].ActiveSession_UB;
    /* (3) Copy selected command structure base address                       */  
    SesCtrl_ST.CurrentCmdTable_PST = SesCtrl_ST.CmdTableEvaluation_PPST[t_Index_UB];

    /* ---------------------------------------------------------------------- */
    /*                                                                        */
    /*      L O O K   F O R   A   V A L I D   C M D                           */
    /*                                                                        */
    /*      Note: The regular command structure start at the third element.   */
    /* ---------------------------------------------------------------------- */
    SesCtrl_ST.SelectedCmdEntry_PST = \
          FUN_CCses_LookForCmd_PST ( t_OutBuffer_PBUF, \
                                     &SesCtrl_ST.CurrentCmdTable_PST[d_CCses_FirstServiceRequestOffset]);

    /* ---------------------------------------------------------------------- */
    /*                                                                        */
    /*      S T O R E   R U N T I M E I N F O                                 */
    /*                                                                        */
    /* ---------------------------------------------------------------------- */
    SesCtrl_ST.CmdRunTimeInfo_PST[t_Channel_UB].CmdEntry_PST = \
                                               SesCtrl_ST.SelectedCmdEntry_PST;

    /*========================================================================*/
    /*                                                                        */
    /* -2- Request length check is necessary                                  */
    /*                                                                        */
    /*========================================================================*/
    if (SesCtrl_ST.SelectedCmdEntry_PST->ServiceRequestLength_UB != d_CCglo_DontCheckLength)
    {
      /*======================================================================*/
      /* -3- Length is not correct                                            */
      /*======================================================================*/
      if ( SesCtrl_ST.SelectedCmdEntry_PST->ServiceRequestLength_UB  != 
           t_InBuffer_PBUF[d_CCglo_DLC] )
      {
        /*--------------------------------------------------------------------*/
        /* Set selected command to the sequence error command.                */
        /*--------------------------------------------------------------------*/
        SesCtrl_ST.CmdRunTimeInfo_PST[t_Channel_UB].CmdEntry_PST = \
            &SesCtrl_ST.CurrentCmdTable_PST[d_CCses_RequestLengthErrorOffset];

        /*--------------------------------------------------------------------*/
        /* Set internal error flag                                            */
        /*--------------------------------------------------------------------*/  
        t_EvaluationState_UB |= d_CCses_FlagRequestLengthError;

      } /* -3- END OF if(..) -------------------------------------------------*/
    } /* ---2- END OF if(..) -------------------------------------------------*/

    /*========================================================================*/
    /*                                                                        */
    /* -2- Command is not allowed in all sessions                             */
    /*                                                                        */
    /*========================================================================*/
    if (d_CCglo_AllowedSessionMask & SesCtrl_ST.SelectedCmdEntry_PST->  \
                             AllowedSessions_UB != d_CCglo_AllowedInAllSession)
    {
      /*======================================================================*/
      /* -3- Command is not allowed in current active session                 */
      /*                                                                      */
      /* NOTE: if a session flag is set and the result is zero we will have a */
      /*       session error                                                  */
      /*======================================================================*/
      if (((d_CCglo_AllowedSessionMask & SesCtrl_ST.SelectedCmdEntry_PST->AllowedSessions_UB) & \
          SesCtrl_ST.CmdRunTimeInfo_PST[t_Channel_UB].AllowedSessions_UB) == 0)
      {
        /*--------------------------------------------------------------------*/
        /* Set selected command to the sequence error command.                */
        /*--------------------------------------------------------------------*/
        SesCtrl_ST.CmdRunTimeInfo_PST[t_Channel_UB].CmdEntry_PST = \
            &SesCtrl_ST.CurrentCmdTable_PST[d_CCses_SessionErrorOffset];

        /*--------------------------------------------------------------------*/
        /* Set internal error flag                                            */
        /*--------------------------------------------------------------------*/
        t_EvaluationState_UB |= d_CCses_FlagRequestSessionError;
      } /* -3- END OF if(..) -------------------------------------------------*/
    } /* ---2- END OF if(..) -------------------------------------------------*/

    /*========================================================================*/
    /*                                                                        */
    /* -2- Command is not allowed with all addressing methods                 */
    /*                                                                        */
    /*========================================================================*/
    if (d_CCglo_AddressAllowedMask & SesCtrl_ST.SelectedCmdEntry_PST->  \
                             AllowedSessions_UB != d_CCglo_AllAddressAllowed)
    {

      /*----------------------------------------------------------------------*/
      /* init scan variables                                                  */
      /*----------------------------------------------------------------------*/
      t_Index_UB = 0x00;
      t_ScanState_UB = d_CCses_ScanRunning;

      /*======================================================================*/
      /* -3- scan Structure for valid command                                 */
      /*======================================================================*/
      while ( (d_CCses_ScanRunning == t_ScanState_UB )  &&
              ( d_CCglo_NoEntry != SesCtrl_ST.AddrModeCfg_PST \
                                                [t_Index_UB].AddressMode_UB ) )
      {
        /*====================================================================*/
        /* -4- Channel number found                                           */
        /*====================================================================*/
        if ( t_InBuffer_PBUF[d_CCglo_ChannelNbr] == SesCtrl_ST. \
                                  AddrModeCfg_PST[t_Index_UB].ChannelNbr )
        {
          /*------------------------------------------------------------------*/
          /* stop scan                                                        */
          /*------------------------------------------------------------------*/
          t_ScanState_UB   = d_CCses_StopScan;

        } /* -4- END OF if(..) -----------------------------------------------*/

        /*====================================================================*/
        /* -4- Channel number not found                                       */
        /*====================================================================*/
        else
        {
          /*------------------------------------------------------------------*/
          /* Check next entry                                                 */
          /*------------------------------------------------------------------*/
          t_Index_UB += 0x01;

        } /* -4- END OF else (if(..)) ----------------------------------------*/
      } /* ---3- END OF while (..) -------------------------------------------*/
      
      /*======================================================================*/
      /* -3- Command is not allowed with the actual adressing method          */
      /*                                                                      */
      /* NOTE: if a addressing flag is set and the result is zero we will     */
      /*       have an adressing error                                        */
      /*======================================================================*/
      if ( ( (d_CCglo_AddressAllowedMask & SesCtrl_ST.SelectedCmdEntry_PST->AllowedSessions_UB) & \
        SesCtrl_ST.AddrModeCfg_PST[t_Index_UB].AddressMode_UB) == 0) 
      {
        /*--------------------------------------------------------------------*/
        /* Set selected command to the addressing error command.              */
        /*--------------------------------------------------------------------*/
        SesCtrl_ST.CmdRunTimeInfo_PST[t_Channel_UB].CmdEntry_PST = \
            &SesCtrl_ST.CurrentCmdTable_PST[d_CCses_AddressingErrorOffset];

        /*--------------------------------------------------------------------*/
        /* Set internal error flag                                            */
        /*--------------------------------------------------------------------*/
        t_EvaluationState_UB |= d_CCses_FlagRequestAddressingError;

      } /* -3- END OF if(..) -------------------------------------------------*/
    } /* ---2- END OF if(..) -------------------------------------------------*/

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
#ifdef cs_CCtypes_32BitSequence /* -2-                                        */
/*----------------------------------------------------------------------------*/

    /* ---------------------------------------------------------------------- */
    /*                                                                        */
    /*      S E Q U E N C E   C H E C K                                       */
    /*                                                                        */
    /* ---------------------------------------------------------------------- */

    /*========================================================================*/
    /* -2- Sequence Check is possible                                         */
    /*========================================================================*/
    if ( d_CCses_NoError == t_EvaluationState_UB )
    {
      /*----------------------------------------------------------------------*/
      /* Init (5) - Get the current PreConditionTable (PCT) line              */
      /*----------------------------------------------------------------------*/
      t_PreCondition_UL = (ULONG) *SesCtrl_ST.SelectedCmdEntry_PST->PreCondCheck_PUL;

      /*----------------------------------------------------------------------*/
      /* Init (6) - Init the current condition check variable                 */
      /*----------------------------------------------------------------------*/
      t_CurrentCondition_UL = 0x80000000ul;

      /*======================================================================*/
      /* -3- Shift bit to correct position                                    */
      /*======================================================================*/
      for (t_Index_UB = 0x00;
           t_Index_UB < SesCtrl_ST.ActCondition_AUB[t_Channel_UB];
           t_Index_UB ++ )
      {
        /* Shift the Bit                                                      */
        t_CurrentCondition_UL >>= 0x01;

      } /* -3- END OF for(..) ------------------------------------------------*/

      /*======================================================================*/
      /* -3- Sequence is not correct                                          */
      /*======================================================================*/
      if ( 0x00000000ul == (t_CurrentCondition_UL & t_PreCondition_UL))
      {
        /*--------------------------------------------------------------------*/
        /* Set selected command to the sequence error command.                */
        /*--------------------------------------------------------------------*/
        SesCtrl_ST.CmdRunTimeInfo_PST[t_Channel_UB].CmdEntry_PST = \
          &SesCtrl_ST.CurrentCmdTable_PST[d_CCses_SequenceErrorOffset];

      } /* -3- END OF if(..) -------------------------------------------------*/
    } /* ---2- END OF if(..) -------------------------------------------------*/

/*----------------------------------------------------------------------------*/
#endif  /* -2- cs_CCtypes_32BitSequence                                       */
/*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
#ifdef cs_CCtypes_16BitSequence /* -2-                                        */
/*----------------------------------------------------------------------------*/

    /* ---------------------------------------------------------------------- */
    /*                                                                        */
    /*      S E Q U E N C E   C H E C K                                       */
    /*                                                                        */
    /* ---------------------------------------------------------------------- */

    /*========================================================================*/
    /* -2- Sequence Check is possible                                         */
    /*========================================================================*/
    if ( d_CCses_NoError == t_EvaluationState_UB )
    {
      /*----------------------------------------------------------------------*/
      /* Init (5) - Get the current PreConditionTable (PCT) line              */
      /*----------------------------------------------------------------------*/
      t_PreCondition_UW = (UWORD) *SesCtrl_ST.SelectedCmdEntry_PST->PreCondCheck_PUW;

      /*----------------------------------------------------------------------*/
      /* Init (6) - Init the current condition check variable                 */
      /*----------------------------------------------------------------------*/
      t_CurrentCondition_UW = (UWORD) 0x8000;

      /*======================================================================*/
      /* -3- Shift bit to correct position                                    */
      /*======================================================================*/
      for (t_Index_UB = 0x00;
           t_Index_UB < SesCtrl_ST.ActCondition_AUB[t_Channel_UB];
           t_Index_UB ++ )
      {
        /* Shift the Bit                                                      */
        t_CurrentCondition_UW >>= 0x01;

      } /* -3- END OF for(..) ------------------------------------------------*/

      /*======================================================================*/
      /* -3- Sequence is not correct                                          */
      /*======================================================================*/
      if ( 0x0000 == (t_CurrentCondition_UW & t_PreCondition_UW))
      {
        /*--------------------------------------------------------------------*/
        /* Set selected command to the sequence error command.                */
        /*--------------------------------------------------------------------*/
        SesCtrl_ST.CmdRunTimeInfo_PST[t_Channel_UB].CmdEntry_PST = \
          &SesCtrl_ST.CurrentCmdTable_PST[d_CCses_SequenceErrorOffset];

      } /* -3- END OF if(..) -------------------------------------------------*/
    } /* ---2- END OF if(..) -------------------------------------------------*/

/*----------------------------------------------------------------------------*/
#endif  /* -2- cs_CCtypes_16BitSequence                                       */
/*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    /* ---------------------------------------------------------------------- */
    /* Set the result                                                         */
    /* ---------------------------------------------------------------------- */
    t_Result_UB |= d_CCglo_Ready;
  
  } /* ---1- END OF if (..) --------------------------------------------------*/


  /*--------------------------------------------------------------------------*/
  /* return                                                                   */
  /*--------------------------------------------------------------------------*/
  return ( t_Result_UB);

} /*-+- END OF FUNCTION 'FUN_CCses_RxSession_UB' -+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/





/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CCses_TxSession_UB                                      |
|                                                                              |
|  Prototype at:   ADLATUS_SessionHandler_tdf.h                                |
|                                                                              |
|  Description:    This function copys the correct response into the TxOut-    |
|                  Buffer and provides the state machine process.              |
|                                                                              |
|  Parameter:      PTR_BUFFER_UB t_InBuffer_PBUF  .. Ptr to Inputbuffer        |
|                  PTR_BUFFER_UB t_OutBuffer_PBUF .. Ptr to Outputbuffer       |
|                                                                              |
|  Return Value:   UBYTE  t_Result_UB                                          |
|                                                                              |
|                         R B E I  I I I I                                     |
|                         | | | |  | | | |                                     |
|                         | | | +--+-+-+-+---    INDEX                         |
|                         | | +--------------    ERROR                         |
|                         | +----------------    BUSY                          |
|                         +------------------    READY                         |
|                                                                              |
+-----------------------------------------------------------------------------*/
UBYTE FUN_CCses_TxSession_UB ( PTR_BUFFER_UB t_InBuffer_PBUF, \
                               PTR_BUFFER_UB t_OutBuffer_PBUF )
{

  UBYTE   t_Result_UB;             /* 1 Byte - Result of function             */
  UBYTE   t_Channel_UB;            /* 1 Byte - Channel Information            */
  UBYTE   t_ResponseIndex_UB;      /* 1 Byte - Resp Index from RunTimeInfo    */
  UBYTE   t_TempIndex_UB;          /* 1 Byte - Temp variable                  */
  UBYTE   t_Offset_UB;             /* 1 Byte - Offset for response selection  */
  UBYTE*  t_ResponseCodeTable_PUB; /* 4 Byte - Ptr to Response Code           */
  /* ------------------------------------------------------------------------ */


  /* ------------------------------------------------------------------------ */
  /* Init (1) - Init Result                                                   */
  /* ------------------------------------------------------------------------ */
  t_Result_UB = (UBYTE) d_CCglo_Idle;

  /*==========================================================================*/
  /* -1- Inbuffer is valid and Outbuffer is invalid                           */
  /*==========================================================================*/
  if ( ((t_InBuffer_PBUF[d_CCglo_BufferState]  & d_CCglo_BufferValid ) == d_CCglo_BufferValid ) &&
       ((t_OutBuffer_PBUF[d_CCglo_BufferState] & d_CCglo_BufferValid ) != d_CCglo_BufferValid ) )
  {
    /* ---------------------------------------------------------------------- */
    /*                                                                        */
    /*      S E L E C T   T H E   C O R R E C T   E N T R Y   O F   T H E     */
    /*      R E S P O N S E   C O D E   T A B L E  ( RCT )                    */
    /*                                                                        */
    /* ---------------------------------------------------------------------- */

    /* ---------------------------------------------------------------------- */
    /* Init (2) - Get channel number: [ (Subset) + (ChannelNumber) ]          */
    /* ---------------------------------------------------------------------- */
    t_Channel_UB =  t_InBuffer_PBUF[d_CCglo_ChannelNbr];
    t_Channel_UB &= d_CCglo_ChannelMask;

    /*------------------------------------------------------------------------*/
    /* Init (3) - Get the response index                                      */
    /*------------------------------------------------------------------------*/
    t_ResponseIndex_UB  = (UBYTE) SesCtrl_ST.CmdRunTimeInfo_PST[t_Channel_UB].ResponseIndex_UB;

    /*------------------------------------------------------------------------*/
    /* Init (4) - Init t_TempIndex_UB will be done later                      */
    /*------------------------------------------------------------------------*/

    /*------------------------------------------------------------------------*/
    /* Init (5) - Init offset information                                     */
    /*------------------------------------------------------------------------*/
    t_Offset_UB = 0x00;

    /*------------------------------------------------------------------------*/
    /* Init (6) - Init base address of the current Response Code Table        */
    /*------------------------------------------------------------------------*/
    t_ResponseCodeTable_PUB = \
      (UBYTE*) SesCtrl_ST.CmdRunTimeInfo_PST[t_Channel_UB].CmdEntry_PST->ResponseCodeTable_PUB;

    /*========================================================================*/
    /* -2- Calculate the offset for the correct response entry from table     */
    /*========================================================================*/
    for (t_TempIndex_UB =  0x00;
         t_TempIndex_UB <  t_ResponseIndex_UB;
         t_TempIndex_UB ++ )
    {
      /*----------------------------------------------------------------------*/
      /*                                                                      */
      /*                            +-------------------------------------+   */
      /*                            |   Response Code Table               |   */
      /*                            +-------------------------------------+   */
      /*  Byte [Offset + 0]         | PostCondition                       |   */
      /*                            +-------------------------------------+   */
      /*  Byte [Offset + 1]         | Response Length   ( N-Byte)         |   */
      /*                            +-------------------------------------+   */
      /*  Byte [Offset + 1 + 1]     | Response Byte 0   (first one)       |   */
      /*                            +-------------------------------------+   */
      /*  ...                       ...                                       */
      /*                            +-------------------------------------+   */
      /*  Byte [Offset + 1 + N]     | Response Byte N-1  (last one)       |   */
      /*                            +-------------------------------------+   */
      /*  Byte [Offset + 1 + N + 1] | Next Entry                          |   */
      /*                            +-------------------------------------+   */
      /*                                                                      */
      /*----------------------------------------------------------------------*/
      /* Set Offset to Response length entry                                  */
      /*----------------------------------------------------------------------*/
      t_Offset_UB += (UBYTE) 0x01;
      /*----------------------------------------------------------------------*/
      /* Add length of this response                                          */
      /*----------------------------------------------------------------------*/
      t_Offset_UB += (UBYTE) t_ResponseCodeTable_PUB[t_Offset_UB];
      /*----------------------------------------------------------------------*/
      /* Add one for the next entry of table                                  */
      /*----------------------------------------------------------------------*/
      t_Offset_UB += (UBYTE) 0x01;

    } /* -2- END OF for(..) --------------------------------------------------*/


    /* ---------------------------------------------------------------------- */
    /*                                                                        */
    /*      S W I T C H   S T A T E M A C H I N  E                            */
    /*                                                                        */
    /* ---------------------------------------------------------------------- */

    /*========================================================================*/
    /* -2- Postcondtion is defined                                            */
    /*========================================================================*/
    if ( d_CCglo_NoPostCondDefined != (UBYTE) t_ResponseCodeTable_PUB[t_Offset_UB] )
    {
      /*----------------------------------------------------------------------*/
      /* Set postcondition                                                    */
      /*----------------------------------------------------------------------*/
      SesCtrl_ST.ActCondition_AUB[t_Channel_UB] = (UBYTE) t_ResponseCodeTable_PUB[t_Offset_UB];

    } /* -2- END OF if(..) ---------------------------------------------------*/


    /* ---------------------------------------------------------------------- */
    /*                                                                        */
    /*      C O P Y   T H E   R E S P O N S E   T O   O U T B U F F E R       */
    /*                                                                        */
    /* ---------------------------------------------------------------------- */

    /*------------------------------------------------------------------------*/
    /* Store the channel number from  TxInBuffer to TxOutBuffer               */
    /*------------------------------------------------------------------------*/
    t_OutBuffer_PBUF[d_CCglo_ChannelNbr] = t_InBuffer_PBUF[d_CCglo_ChannelNbr];

    /*========================================================================*/
    /* -2- Copy Data from response table                                      */
    /*     Note: The length is the second entry of a response code table set  */
    /*           --> ( 1 + offset )                                           */
    /*========================================================================*/
    if ( 0x00 != (UBYTE) t_ResponseCodeTable_PUB[(0x01 + t_Offset_UB)])
    {
      /*----------------------------------------------------------------------*/
      /* Store the data length for TxOutBuffer                                */
      /*----------------------------------------------------------------------*/
      t_OutBuffer_PBUF[d_CCglo_DLC] = (UBYTE) t_ResponseCodeTable_PUB[(0x01 + t_Offset_UB)];

      /*======================================================================*/
      /* -3- Copy x data                                                      */
      /*     Note: The data starts on the third entry of a response code      */
      /*           table set.  --> ( 2 + offset )                             */
      /*======================================================================*/
      for (t_TempIndex_UB =  0x00;
           t_TempIndex_UB <  t_OutBuffer_PBUF[d_CCglo_DLC];
           t_TempIndex_UB ++ )
      {
        /*--------------------------------------------------------------------*/
        /* Copy the data from ResponseCodeTable to TxOutBuffer                */
        /*--------------------------------------------------------------------*/
        t_OutBuffer_PBUF[ (d_CCglo_StartOfData + t_TempIndex_UB) ] = \
                         (UBYTE) t_ResponseCodeTable_PUB[(0x02 + t_Offset_UB)];
        /*--------------------------------------------------------------------*/
        /* Increment the offset for the next data byte                        */
        /*--------------------------------------------------------------------*/
        t_Offset_UB += 0x01;

      } /* -3- END OF for (..) -----------------------------------------------*/

      /*----------------------------------------------------------------------*/
      /* Set the Buffer state information                                     */
      /*----------------------------------------------------------------------*/
      /* InBuffer is invalid                                                  */
      t_InBuffer_PBUF[d_CCglo_BufferState]  &= (UBYTE) ~d_CCglo_BufferValid;
      /* OutBuffer is valid                                                   */
      t_OutBuffer_PBUF[d_CCglo_BufferState] |= d_CCglo_BufferValid; 

      /* -------------------------------------------------------------------- */
      /*                                                                      */
      /*      S E T   R E T U R N   I N F O R M A T I O N                     */
      /*                                                                      */
      /* -------------------------------------------------------------------- */
      t_Result_UB |= d_CCglo_Ready;

    } /* ---2- END OF if (..)  -----------------------------------------------*/

    /*========================================================================*/
    /* -2- Copy Data direct from TxInputBuffer                                */
    /*========================================================================*/
    else
    {
      /*======================================================================*/
      /* -3- there is data in the TxInputBuffer                               */
      /*======================================================================*/
      if ( 0x00 != t_InBuffer_PBUF[d_CCglo_DLC] )
      {
        /*--------------------------------------------------------------------*/
        /* Store the data length for TxOutBuffer                              */
        /*--------------------------------------------------------------------*/
        t_OutBuffer_PBUF[d_CCglo_DLC] = t_InBuffer_PBUF[d_CCglo_DLC];

        /*====================================================================*/
        /* -4- Copy x data                                                    */
        /*====================================================================*/
        for (t_TempIndex_UB =  0x00;
             t_TempIndex_UB <  t_OutBuffer_PBUF[d_CCglo_DLC];
             t_TempIndex_UB ++ )
        {
          /*------------------------------------------------------------------*/
          /* Copy the data from TxInBuffer to TxOutBuffer                     */
          /*------------------------------------------------------------------*/
          t_OutBuffer_PBUF[ (d_CCglo_StartOfData + t_TempIndex_UB) ] = \
          t_InBuffer_PBUF[  (d_CCglo_StartOfData + t_TempIndex_UB) ];
          /* init InBuffer                                                    */
          t_InBuffer_PBUF[  (d_CCglo_StartOfData + t_TempIndex_UB) ] = 0x00;

        } /* -4- END OF for (..) ---------------------------------------------*/

        /*--------------------------------------------------------------------*/
        /* Set the Buffer state information                                   */
        /*--------------------------------------------------------------------*/
        /* InBuffer is invalid                                                */
        t_InBuffer_PBUF[d_CCglo_BufferState]  &= (UBYTE) ~d_CCglo_BufferValid;
        /* OutBuffer is valid                                                 */
        t_OutBuffer_PBUF[d_CCglo_BufferState] |= d_CCglo_BufferValid;

        /* ------------------------------------------------------------------ */
        /*                                                                    */
        /*      S E T   R E T U R N   I N F O R M A T I O N                   */
        /*                                                                    */
        /* ------------------------------------------------------------------ */
        t_Result_UB |= d_CCglo_Ready;

      } /* ---3- END OF if (..) ----------------------------------------------*/
      /*======================================================================*/
      /* -3- there is no data in the TxInputBuffer, no aswer shall be sent    */
      /*     for this service                                                 */
      /*======================================================================*/
      else
      {
        /*--------------------------------------------------------------------*/
        /* Set the Buffer state information                                   */
        /*--------------------------------------------------------------------*/
        /* InBuffer is invalid                                                */
        t_InBuffer_PBUF[d_CCglo_BufferState]  &= (UBYTE) ~d_CCglo_BufferValid;
        /* OutBuffer is valid                                                 */
        t_OutBuffer_PBUF[d_CCglo_BufferState] &= (UBYTE) ~d_CCglo_BufferValid;

        /* ------------------------------------------------------------------ */
        /*                                                                    */
        /*      S E T   R E T U R N   I N F O R M A T I O N                   */
        /*                                                                    */
        /* ------------------------------------------------------------------ */
        t_Result_UB |= d_CCglo_Idle;

      } /* ---3- END OF else (if (..)) ---------------------------------------*/
    } /* ---2- END OF else if (..) -------------------------------------------*/
  } /* -1- END OF if(..) -----------------------------------------------------*/

  return ( t_Result_UB );

} /*-+- END OF FUNCTION 'FUN_CCses_TxSession_UB' -+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/


/*----------------------------------------------------------------------------*/
#endif /* -1- cs_CCsys_MultiChannelSystem                                     */
/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
/*                                                                            */
/*      E N D   O F   M U L T I    C H A N N E L   S Y S T E M                */
/*                                                                            */
/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */




/* ========================================================================== */
/*      E N D   O F   F I L E                                                 */
/* ========================================================================== */
