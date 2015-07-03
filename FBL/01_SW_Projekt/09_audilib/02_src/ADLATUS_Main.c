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
|  Filename:  ADLATUS_Main.c                                                   |
|                                                                              |
|  Comment:   This file includes the ADLATUS entry routine and the system      |
|             startup test handling of the SMART ADLATUS.                      |
|                                                                              |
+------------------------------------------------------------------------------+
| R E V I S I O N   H I S T O R Y                                              |
+------------------------------------------------------------------------------+
|
| Subversion Info:
| $Id: ADLATUS_Main.c 4396 2008-11-04 09:26:32Z maennel $
| $HeadURL: http://smartsvn/svn/adlatusrepository/auslieferungen/S1NN/086350-S1NN-ADLATUS-TMS470R-MucSuc/20081201_S1NN_TMS470R_BlfIso-Source_V1.00/01_SW_Projekt/09_audilib/02_src/ADLATUS_Main.c $
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
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#include "ADLATUS_SequenceAudi_cdf.h"
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


/* ========================================================================== */
/* (2)  I N C L U D E S                                                       */
/* -------------------------------------------------------------------------- */
/*      T Y P E   D E F I N I T I O N   F I L E S  (TDF)                      */
/* ========================================================================== */
#include "ADLATUS_Types_tdf.h"          /* 1st include: Type definitions      */
#include "ADLATUS_ConfigSystem_tdf.h"   /* 2nd include: System configuration  */
#include "ADLATUS_ConfigProject_tdf.h"  /* 3rd include: Project configuration */
#include "ADLATUS_ConfigAdlatus_tdf.h"  /* 4th include: System configuration  */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#include "ADLATUS_Main_tdf.h"             /* System configuration             */
#include "ADLATUS_Setup_tdf.h"            /* ADLATUS Configuration            */
#include "ADLATUS_System_tdf.h"           /* Operating System entry           */
#include "ADLATUS_Hal_tdf.h"              /* HW Layer Routines                */
#include "ADLATUS_ValHandler_tdf.h"       /* Validation Routines              */
#include "ADLATUS_Timer_tdf.h"            /* Timer Function                   */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* Customer Specific Files:                                                   */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#include "ADLATUS_Periphery_tdf.h"            /* Init I/O                     */
#include "ADLATUS_ApplicationInterface_tdf.h" /* Check Flash Request          */
#include "ADLATUS_SSIHandler_tdf.h"
#include "ADLATUS_NvmHandler_tdf.h"
#include "ADLATUS_ExternalWatchdog_tdf.h"
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* Customer Specific Files:  VW / AUDI                                        */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#include "ADLATUS_EventHandlerAudi_cdf.h"
#include "ADLATUS_EventHandlerAudi_tdf.h"

#include "ADLATUS_NvmHandlerAudi_cdf.h"

/* ========================================================================== */
/* (3)  C O M P I L E R   S W I T C H   S E T T I N G S                       */
/* -------------------------------------------------------------------------- */
/*      O N L Y   F O R   T H I S   M O D U L                                 */
/* ========================================================================== */
/* Global Compiler switch settings are done at ADLATUS_CompilerSwitches_CDF.h */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* Local compiler switches: No Entry                                          */
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
/* NO ENTRY                                                                   */

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
UBYTE           t_AdlatusInitState_UB;

/* ========================================================================== */
/* (8)  L O C A L   F U N C T I O N   P R O T O T Y P E S                     */
/* -------------------------------------------------------------------------- */
/*      O N L Y   F O R   T H I S   M O D U L                                 */
/* ========================================================================== */
void main                             (void);
void FUN_CCmain_Main_V                (void);
void FUN_CCmain_SysStateEvaluation_V  (void);


/* ========================================================================== */
/* (9)  G L O B A L   V A R I A B L E S                                       */
/* -------------------------------------------------------------------------- */
/*      D E F I N E D   W I T H I N   T H I S   M O D U L                     */
/* ========================================================================== */
INITCONTROL_ST  t_InitCtrl_ST;

/* ========================================================================== */
/* (10) F U N C T I O N   D E F I N I T I O N S                               */
/* -------------------------------------------------------------------------- */
/*      F O R   T H I S   M O D U L                                           */
/* ========================================================================== */

/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  main                                                        |
|                                                                              |
|  Prototype at:   ADLATUS_Main.c                                              |
|                                                                              |
|  Description:    Main-Routine of C-language.                                 |
|                                                                              |
|  Parameter:       --                                                         |
|                                                                              |
|  Return Value:    --                                                         |
|                                                                              |
+-----------------------------------------------------------------------------*/
void main (void)
{  
  FUN_CCmain_Main_V ();

} /* -+- END OF FUNCTION 'main' -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/




/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CCmain_Main_V                                           |
|                                                                              |
|  Prototype at:   ADLATUS_Main.c                                              |
|                                                                              |
|  Description:    Main-Routine of the project.                                |
|                                                                              |
|  Parameter:       --                                                         |
|                                                                              |
|  Return Value:    --                                                         |
|                                                                              |
+-----------------------------------------------------------------------------*/
void FUN_CCmain_Main_V (void)
{  
  /*--------------------------------------------------------------------------*/
  /* Init global variables                                                    */
  /*--------------------------------------------------------------------------*/

  /*--------------------------------------------------------------------------*/
  /* Init local variables                                                     */
  /*--------------------------------------------------------------------------*/
  t_InitCtrl_ST.SystemState_UB = d_CCglo_NoError;

  /* ------------------------------------------------------------------------ */
  /* I N I T   A D L A T U S                                                  */
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Disable all Interrupts                                                   */
  /*                                                                          */
  /*--------------------------------------------------------------------------*/
  FUN_CChal_DisGlobalInterrupt_V ();

  /* ------------------------------------------------------------------------ */
  /* I N I T   A D L A T U S                                                  */
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Init HAL                                                                 */
  /*                                                                          */
  /*--------------------------------------------------------------------------*/
  FUN_CChal_InitHAL_V ( &c_CCconsys_HalInterface_ST);

  /* ------------------------------------------------------------------------ */
  /* I N I T   A D L A T U S                                                  */
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Init Hardware-Timer                                                      */
  /*                                                                          */
  /*--------------------------------------------------------------------------*/
  FUN_CChal_InitHardwareTimer_V ();

  /* ------------------------------------------------------------------------ */
  /* Init Software Timer Handler                                              */
  /*--------------------------------------------------------------------------*/
  FUN_CCtim_InitSWTimerHandler_V (&c_CCconsys_SWTimerHandlerInterface_ST);

  /* ------------------------------------------------------------------------ */
  /* I N I T   A D L A T U S   F O R   P R O J E C T                          */
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Internal Watchdog                                                        */
  /*                                                                          */
  /*--------------------------------------------------------------------------*/
  /*--------------------------------------------------------------------------*/
  /* Disable internal watchdog                                                */
  /*--------------------------------------------------------------------------*/
  FUN_CChal_DisableIntWatchdog_V  ();

  /*--------------------------------------------------------------------------*/
  /* Init internal watchdog                                                   */
  /*--------------------------------------------------------------------------*/
  FUN_CChal_InitIntWatchdog_V (); 

  /*--------------------------------------------------------------------------*/
  /* Enable internal watchdog                                                 */
  /*--------------------------------------------------------------------------*/
  FUN_CChal_EnableIntWatchdog_V ();

  /* ------------------------------------------------------------------------ */
  /* I N I T   A D L A T U S   F O R   P R O J E C T                          */
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Synchron Serial Interface (SSI)                                          */
  /*                                                                          */
  /* Comment: If we have a SPI (or similar extWD), we have to init the        */
  /*          interface BEFORE we init the WD                                 */
  /*--------------------------------------------------------------------------*/
  FUN_CCssi_InitSSI_V ();

  /* ------------------------------------------------------------------------ */
  /* I N I T   A D L A T U S   F O R   P R O J E C T                          */
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* External Watchdog                                                        */
  /*                                                                          */
  /*--------------------------------------------------------------------------*/
  FUN_CCwtdog_InitExtWatchdog_V ();

  /*--------------------------------------------------------------------------*/
  /* Enable external watchdog                                                 */
  /*--------------------------------------------------------------------------*/
  FUN_CCwtdog_EnableExtWatchdog_V ();

  /*--------------------------------------------------------------------------*/
  /* Disable external watchdog                                                */
  /*--------------------------------------------------------------------------*/
  FUN_CCwtdog_DisableExtWatchdog_V ();

  /* ------------------------------------------------------------------------ */
  /* I N I T   A D L A T U S                                                  */
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Set I/O - Configuration                                                  */
  /*                                                                          */
  /*--------------------------------------------------------------------------*/
  FUN_CCper_InitIOStartup_V    ();

  /*--------------------------------------------------------------------------*/
  /* Init the system check                                                    */
  /*--------------------------------------------------------------------------*/
  FUN_CCVhnd_InitValidation_V (&c_CCconprj_ValidationMap_AST[0] );

  /* ------------------------------------------------------------------------ */
  /* I N I T   A D L A T U S   F O R   P R O J E C T                          */
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* NOTE: This routine inits the Event Handler                               */
  /*--------------------------------------------------------------------------*/
  FUN_CCeventaudi_EventInit_V ();
  /*--------------------------------------------------------------------------*/
  /* S Y S T E M   C H E C K                                                  */
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Flash Request                                                            */
  /*--------------------------------------------------------------------------*/
  t_InitCtrl_ST.SystemState_UB    = FUN_CCapplint_CheckFlashRequest_UB ();
  /* Store the channel Info                                                   */
  t_InitCtrl_ST.ActiveChannel_UB  = (t_InitCtrl_ST.SystemState_UB & d_CCglo_ChannelMask);
  /* Clear the channel Info for the following checks                          */
  t_InitCtrl_ST.SystemState_UB   &= (~d_CCglo_ChannelMask);

  /*==========================================================================*/
  /* -1- No FlashRequest from FSW                                             */
  /*==========================================================================*/
  if ( d_CCglo_NoFSWFlashRequest == (d_CCglo_FSWFlashRequest & t_InitCtrl_ST.SystemState_UB))
  {
    /* ---------------------------------------------------------------------- */
    /* S Y S T E M   C H E C K                                                */
    /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
    /* RAM Test                                                               */
    /*------------------------------------------------------------------------*/
    t_InitCtrl_ST.SystemState_UB = FUN_CCVhnd_CheckValidationMap_UB ( \
      (UWORD)(d_CCglo_AswArea | d_CCglo_RamMemory | d_CCglo_CheckEnabled), \
      (UWORD)(d_CCglo_AdlatusAreaMask | d_CCglo_MemoryTypeMask | d_CCglo_MemoryCheckMask) );

    /*========================================================================*/
    /* -2- No error occured during RAM test                                   */
    /*========================================================================*/
    if (d_CCglo_NoRamCheckError == t_InitCtrl_ST.SystemState_UB)
    {
      /* -------------------------------------------------------------------- */
      /* S Y S T E M   C H E C K                                              */
      /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
      /* Calculate ADLATUS Checksum                                           */
      /*----------------------------------------------------------------------*/
      t_InitCtrl_ST.SystemState_UB = FUN_CCVhnd_CheckValidationMap_UB ( \
        (UWORD)(d_CCglo_AdlatusArea | d_CCglo_FlashMemory | d_CCglo_CheckEnabled), \
        (UWORD)(d_CCglo_AdlatusAreaMask | d_CCglo_MemoryTypeMask | d_CCglo_MemoryCheckMask) );

      /*======================================================================*/
      /* -3- No error occured during ADLATUS Checksum test                    */
      /*======================================================================*/
      if ( d_CCglo_NoAdlatusChecksumError == t_InitCtrl_ST.SystemState_UB)
      {
        /* ------------------------------------------------------------------ */
        /* S Y S T E M   C H E C K                                            */
        /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
        /* FSW Quickcheck                                                       */
        /*--------------------------------------------------------------------*/
        t_InitCtrl_ST.SystemState_UB = FUN_CCVhnd_CheckValidationMap_UB ( \
          (UWORD)(d_CCglo_AswArea | d_CCglo_FlashMemory | d_CCglo_DoQuickCheck), \
          (UWORD)(d_CCglo_AdlatusAreaMask | d_CCglo_MemoryTypeMask | d_CCglo_QuickCheckMask) );

        /*====================================================================*/
        /* -4- Application Software is programmed                             */
        /*====================================================================*/
        if ( d_CCglo_FswInSystem == t_InitCtrl_ST.SystemState_UB)
        {
          /* -----------------------------------------------------------------*/
          /* S Y S T E M   C H E C K                                          */
          /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
          /* FSW Checksum calculation                                         */
          /*------------------------------------------------------------------*/
          t_InitCtrl_ST.SystemState_UB = FUN_CCVhnd_CheckValidationMap_UB ( \
            (UWORD)(d_CCglo_AswArea | d_CCglo_FlashMemory | d_CCglo_CheckEnabled), \
            (UWORD)(d_CCglo_AdlatusAreaMask | d_CCglo_MemoryTypeMask | d_CCglo_MemoryCheckMask) );

        } /* ---4- END OF if(..) ---------------------------------------------*/
      } /* -----3- END OF if(..) ---------------------------------------------*/
    } /* -------2- END OF if(..) ---------------------------------------------*/
  } /* ---------1- END OF if(..) ---------------------------------------------*/

  /*==========================================================================*/
  /* -1- FlashRequest from FSW                                                */
  /*==========================================================================*/
  else
  {
    /* Set system check flag                                                  */
    t_InitCtrl_ST.SystemState_UB = d_CCglo_FSWFlashRequest;
  } /* -1- END OF else (if(..)) ----------------------------------------------*/

  /* ------------------------------------------------------------------------ */
  /* I N I T   A D L A T U S   F O R   P R O J E C T                          */
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* NonVolatileMemory (NVM) Handler                                          */
  /*                                                                          */
  /*--------------------------------------------------------------------------*/
  FUN_CCnvm_InitNvmHandler_V (&c_CCconsys_NvmInterface_ST);

  /*--------------------------------------------------------------------------*/
  /*                                                                          */
  /* C U S T O M E R   S P E C I F I C   I N I T I A L I S A T I O N          */
  /*                                                                          */
  /*--------------------------------------------------------------------------*/
  FUN_CCmain_SysStateEvaluation_V ();

  /* ------------------------------------------------------------------------ */
  /* I N I T   A D L A T U S                                                  */
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Init ADLATUS                                                             */
  /*                                                                          */
  /*--------------------------------------------------------------------------*/
  FUN_CCsetup_InitAdlatus_V ();

  /* ------------------------------------------------------------------------ */
  /* I N I T   A D L A T U S                                                  */
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Set I/O - Configuration                                                  */
  /*                                                                          */
  /*--------------------------------------------------------------------------*/
  FUN_CCper_InitIOForAdlatus_V ();

  /* ------------------------------------------------------------------------ */
  /* I N I T   A D L A T U S                                                  */
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Start ADLATUS                                                            */
  /*                                                                          */
  /*--------------------------------------------------------------------------*/
  FUN_CCsys_OperatingSystem_V ();
 
} /*-+- END OF FUNCTION 'FUN_CCmain_Main_V' -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/




/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CCmain_SysStateEvaluation_V                             |
|                                                                              |
|  Prototype at:   FLASHCORE_Main.c                                            |
|                                                                              |
|  Description:    Evaluation of the current system state after startup checks |
|                                                                              |
|  Parameter:      --                                                          |
|                                                                              |
|  Return Value:    --                                                         |
|                                                                              |
+-----------------------------------------------------------------------------*/
void  FUN_CCmain_SysStateEvaluation_V ( void )
{
  /*==========================================================================*/
  /* -1- System State Evaluation                                              */
  /*==========================================================================*/
  switch ( t_InitCtrl_ST.SystemState_UB )
  {
    /*------------------------------------------------------------------------*/
    /*                                                                        */
    /* FSW IN SYSTEM                                                          */
    /* NO SYSTEM CHECK ERROR                                                  */
    /* NO FLASH REQUEST                                                       */
    /*                                                                        */
    /*------------------------------------------------------------------------*/
    case d_CCglo_NoError:

      /*----------------------------------------------------------------------*/
      /* Active Channel: All channels                                         */
      /*----------------------------------------------------------------------*/
      t_InitCtrl_ST.ActiveChannel_UB = d_CCglo_AllChannels;

      /*======================================================================*/
      /* -2- The FSW is set valid in the nvm                                  */
      /*======================================================================*/
      if(FUN_CCeventaudi_EventHandler_B (d_CCeventaudi_LOOKFOR_FSWInvalid_UB) == FALSE)
      {
        /*--------------------------------------------------------------------*/
        /* jump to application software                                       */
        /*--------------------------------------------------------------------*/
        FUN_CCapplint_JumpToFSW_V ();

      } /* ---2- END OF if (..) ----------------------------------------------*/

      /*======================================================================*/
      /* -2- The FSW is set invalid in the nvm                                */
      /*======================================================================*/
      else
      {
        /*--------------------------------------------------------------------*/
        /* Active Channel: All channels                                       */
        /*--------------------------------------------------------------------*/
        t_InitCtrl_ST.ActiveChannel_UB = d_CCglo_AllChannels;

        /*--------------------------------------------------------------------*/
        /* Init Channel 00: VW / AUDI - Activate Standard Mode after RESET    */
        /*--------------------------------------------------------------------*/
        t_InitCtrl_ST.ActiveSession_AUB[d_CCglo_Channel00] = d_CCseqaudi_DiagSessionProgMode;
        t_InitCtrl_ST.InitCondition_AUB[d_CCglo_Channel00] = d_CCglo_ResetChannel;

        /*--------------------------------------------------------------------*/
        /* look for ECU reset request                                         */
        /*--------------------------------------------------------------------*/
        t_InitCtrl_ST.SystemState_UB = FUN_CCapplint_CheckFlashRequest_UB ();

        /*====================================================================*/
        /* -3- an ECU reset request is read out                               */
        /*====================================================================*/
        if ( d_CCglo_FSWEcuResetRequest == (d_CCglo_FSWEcuResetRequest & t_InitCtrl_ST.SystemState_UB))
        {
          t_InitCtrl_ST.InitCondition_AUB[d_CCglo_Channel00] = d_CCglo_ResetRequest;

          /*------------------------------------------------------------------*/
          /* set flag                                                         */
          /*------------------------------------------------------------------*/
          FUN_CCeventaudi_EventHandler_B (d_CCeventaudi_SET_____ECUResetRequest_UB);

        } /* ---3- END OF if (..) --------------------------------------------*/
        /*====================================================================*/
        /* -3- no ECU reset request is read out                               */  /* [OPL286] */
        /*====================================================================*/
        else
        {
          /*==================================================================*/
          /* -4- an Default Session Request is read out                       */
          /*==================================================================*/
          if ( d_CCglo_FSWDefSessionRequest == (d_CCglo_FSWDefSessionRequest & t_InitCtrl_ST.SystemState_UB))
          {
            t_InitCtrl_ST.InitCondition_AUB[d_CCglo_Channel00] = d_CCglo_ResetRequest;

            /*----------------------------------------------------------------*/
            /* set flag                                                       */
            /*----------------------------------------------------------------*/
            FUN_CCeventaudi_EventHandler_B (d_CCeventaudi_SET_____DefSessionRequ_UB);

          } /* -4- END OF if (..) --------------------------------------------*/
        } /* ---3- END OF else (if (..)) -------------------------------------*/
      } /* -----2- END OF else (if (..)) -------------------------------------*/
      
      break;
      /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

    /*------------------------------------------------------------------------*/
    /*                                                                        */
    /* FLASH REQUEST                                                          */
    /*                                                                        */
    /*------------------------------------------------------------------------*/
    case d_CCglo_FSWFlashRequest:

      /*--------------------------------------------------------------------*/
      /* Init Channel 00: VW / AUDI - Activate Standard Mode after FLASH    */
      /*                              REQUEST                               */
      /*--------------------------------------------------------------------*/

      t_InitCtrl_ST.ActiveSession_AUB[d_CCglo_Channel00] = d_CCseqaudi_DiagSessionProgMode;
      t_InitCtrl_ST.InitCondition_AUB[d_CCglo_Channel00] = d_CCglo_FlashRequest;

      FUN_CCeventaudi_EventHandler_B (d_CCeventaudi_SET_____FlashRequest_UB );

      /*------------------------------------------------------------------*/
      /* communication is active any communication break leads to a       */
      /* timeout                                                          */
      /*------------------------------------------------------------------*/
      FUN_CCeventaudi_EventHandler_B (d_CCeventaudi_SET_____OpenChannel_UB);

      break;
      /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

    /*------------------------------------------------------------------------*/
    /*                                                                        */
    /* E R R O R                                                              */
    /*                                                                        */
    /*------------------------------------------------------------------------*/
    default:
      /*----------------------------------------------------------------------*/
      /* Active Channel: All channels                                         */
      /*----------------------------------------------------------------------*/
      t_InitCtrl_ST.ActiveChannel_UB = d_CCglo_AllChannels;

      /*----------------------------------------------------------------------*/
      /* Init Channel 00: VW / AUDI - Activate Standard Mode after RESET      */
      /*----------------------------------------------------------------------*/
      t_InitCtrl_ST.ActiveSession_AUB[d_CCglo_Channel00] = d_CCseqaudi_DiagSessionProgMode;
      t_InitCtrl_ST.InitCondition_AUB[d_CCglo_Channel00] = d_CCglo_ResetChannel;

      
      if ((t_InitCtrl_ST.SystemState_UB != d_CCglo_RamCheckError) &&
          (t_InitCtrl_ST.SystemState_UB != d_CCglo_AdlatusChecksumError))
      {
        /* from now on the ASW isn't valid */
        FUN_CCeventaudi_EventHandler_B (d_CCeventaudi_SET_____FSWInvalid_UB);
      }

      /*----------------------------------------------------------------------*/
      /* look for ECU reset request                                           */
      /*----------------------------------------------------------------------*/
      t_InitCtrl_ST.SystemState_UB = FUN_CCapplint_CheckFlashRequest_UB ();

      /*======================================================================*/
      /* -2- an ECU reset request is read out                                 */
      /*======================================================================*/
      if ( d_CCglo_FSWEcuResetRequest == (d_CCglo_FSWEcuResetRequest & t_InitCtrl_ST.SystemState_UB))
      {
         /* After a Reset -- (programming session) we are in an extendend mode */ 
        t_InitCtrl_ST.InitCondition_AUB[d_CCglo_Channel00] = d_CCglo_ResetRequest;

        /*--------------------------------------------------------------------*/
        /* set flag                                                           */
        /*--------------------------------------------------------------------*/
        FUN_CCeventaudi_EventHandler_B (d_CCeventaudi_SET_____ECUResetRequest_UB);

      } /* -2- END OF if (..) ------------------------------------------------*/
      
      /*======================================================================*/
      /* -2- no ECU reset request is read out                                 */  /* [OPL286] */
      /*======================================================================*/
      else
      {
        /*====================================================================*/
        /* -3- an Default Session Request is read out                         */
        /*====================================================================*/
        if ( d_CCglo_FSWDefSessionRequest == (d_CCglo_FSWDefSessionRequest & t_InitCtrl_ST.SystemState_UB))
        {
          t_InitCtrl_ST.InitCondition_AUB[d_CCglo_Channel00] = d_CCglo_ResetRequest;

          /*------------------------------------------------------------------*/
          /* set flag                                                         */
          /*------------------------------------------------------------------*/
          FUN_CCeventaudi_EventHandler_B (d_CCeventaudi_SET_____DefSessionRequ_UB);

        } /* ---3- END OF if (..) --------------------------------------------*/
      } /* -----2- END OF else (if (..)) -------------------------------------*/
      
      break;
      /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  } /* -1- END OF switch (..) ------------------------------------------------*/

  return;

} /*-+- END OF FUNCTION 'FUN_CCmain_SysStateEvaluation_V' -+-+-+-+-+-+-+-+-+-+*/


/* ========================================================================== */
/*      E N D   O F   F I L E                                                 */
/* ========================================================================== */
