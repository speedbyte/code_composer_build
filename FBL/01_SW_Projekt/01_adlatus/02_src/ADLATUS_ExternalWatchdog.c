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
|  Filename:  ADLATUS_ExternalWatchdog.c                                       |
|                                                                              |
|  Comment:   This file includes the all routines for the external watchdog:   |
|                                                                              |
|                                                                              |
|                                                                              |
+------------------------------------------------------------------------------+
| R E V I S I O N   H I S T O R Y                                              |
+------------------------------------------------------------------------------+
|
| Subversion Info:
| $Id: ADLATUS_ExternalWatchdog.c 5801 2010-07-09 13:21:35Z werner $
| $HeadURL: http://smartsvn/svn/adlatusrepository/auslieferungen/S1NN/S1NN-ADLATUS-TMS470R1SF55B-MucSuc/20100706_S1NN_TMS470S_BlfIso-Source_V1.00/01_SW_Projekt/01_adlatus/02_src/ADLATUS_ExternalWatchdog.c $
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
#include "ADLATUS_CanHandler_cdf.h"

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* Includes for the address information                                       */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#include "ADLATUS_Adr_Info.h"


/* ========================================================================== */
/* (2)  I N C L U D E S                                                       */
/* -------------------------------------------------------------------------- */
/*      T Y P E   D E F I N I T I O N   F I L E S  (TDF)                      */
/* ========================================================================== */
#include "ADLATUS_Types_tdf.h"                 /* must be the first include   */
#include "ADLATUS_ExternalWatchdog_tdf.h"
#include "ADLATUS_SSIHandler_tdf.h"
                            
#include "ADLATUS_CanHandler_tdf.h"

#include "ADLATUS_HAL_tdf.h"

/* -------------------------------------------------------------------------- */
/*      Processor specific header files                                       */
/* -------------------------------------------------------------------------- */
#include "tms470regs.h"

/* ========================================================================== */
/* (3)  C O M P I L E R   S W I T C H   S E T T I N G S                       */
/* -------------------------------------------------------------------------- */
/*      O N L Y   F O R   T H I S   M O D U L                                 */
/* ========================================================================== */
/* Global Compiler switch settings are done at ADLATUS_CompilerSwitches_CDF.h */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* Local compiler switches:                                                   */
/*                                                                            */
/*    -------------------------------------------------------------------     */
/*    ADLATUS Model                                                           */
/*                                                                            */
/*    This switch is necessary for the different ADLATUS models which could   */
/*    be generated.                                                           */
/*                                                                            */
/*    (A) Normal ADLATUS .. the standard ADLATUS for reprogramming ASW        */
/*                          Watchdog is disabled at startup.                  */
/*    (B) reprog ADLATUS .. the sw which seems like an ASW and reprogramms    */
/*                          the normal ADLATUS.                               */
/*                          Watchdog is enabled at startup.                   */
/*                                                                            */
/* -------------------------------------------------------------------------- */

/* Compile Model (A) Normal ADLATUS                                           */
#define cs_CCwtdog_NormalADLATUS   

/* Compile Model (B) ReprogADLATUS                                            */
/*#define cs_CCwtdog_ReprogADLATUS*/

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
UBYTE wdStatus = 0x00;
/******************************************************************************/
/******************************************************************************/
/**                                                                          **/
/**     APPLICATION AREA: WDT.01                                             **/
/**     ----------------------------------------------------------------     **/
/**     S Y S T E M   C O N F I G U R A T I O N                              **/
/**     ----------------------------------------------------------------     **/
/**     watchdog trigger time                                                **/
/**                                                                          **/
/******************************************************************************/
/******************************************************************************/
/* 0 = watchdog disabled, other values = trigger time in ms */
#define  d_CCwtdog_InitExtWdogTime      (20) /* 1 = 1ms */

/******************************************************************************/
/******************************************************************************/
/**                                                                          **/
/**     APPLICATION AREA: RESP.01                                            **/
/**     ----------------------------------------------------------------     **/
/**     S Y S T E M   C O N F I G U R A T I O N                              **/
/**     ----------------------------------------------------------------     **/
/**     response pending time      ( only for UDS )                        **/
/**                                                                          **/
/******************************************************************************/
/******************************************************************************/
#define  d_CCwtdog_TXDelayCounter            0x0907ul   /* ca 2500 ms */

/* -------------------------------------------------------------------------- */
/*      W A T C H D O G   S T A T E   R E G I S T E R   D E F I N E S         */
/*      -------------------------------------------------------------------   */
/*                                                                            */
/*      Watchdog State_UB  = 7 6 5 4  3 2 1 0                                 */
/*                           | | | |  | | | |                                 */
/*                           | | | |  | | | +---  enable watchdog             */
/*                           | | | |  | | +-----  disable watchdog            */
/*                           | | | |  | +-------  trigger watchdog            */
/*                           | | | |  +---------  not used                    */
/*                           | | | |                                          */
/*                           | | | +------------  not used                    */
/*                           | | +--------------  not used                    */
/*                           | +----------------  not used                    */
/*                           +------------------  watchdog enabled            */
/*                                                                            */
/* -------------------------------------------------------------------------- */
/* --> look at ADLATUS_Global_CDF.h                                           */
/* -------------------------------------------------------------------------- */
/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
#ifdef cs_CCwtdog_NormalADLATUS
/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
#define d_CCwtdog_InitWatchdogState     d_CCglo_InitExternalWatchdogState
/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
#else
/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
  #define d_CCwtdog_InitWatchdogState   ( d_CCglo_InitExternalWatchdogState    \
                                        | d_CCglo_StateExternalWatchdogEnabled )
/*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
#endif
/*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

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
extern WATCHDOGCONTROL_ST   t_CCper_WatchdogCtrl_ST;

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

/*lint -e40 */
/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CCwtdog_TriggerExtWatchdog_V                            |
|                                                                              |
|  Prototype at:   ADLATUS_ExternalWatchdog_tdf.h                              |
|                                                                              |
|  Description:    CUSTOMER INTERFACE:                                         |
|                  Trigger for the external watchdog.                          |
|                                                                              |
|  Parameter:       --                                                         |
|                                                                              |
|  Return Value:    --                                                         |
|                                                                              |
+-----------------------------------------------------------------------------*/
void FUN_CCwtdog_TriggerExtWatchdog_V (UWORD t_WtdogTimerTicks_UW )
{

  /*==========================================================================*/
  /* -1- Watchdogtimer not elapsed                                            */
  /*==========================================================================*/
  if (t_CCper_WatchdogCtrl_ST.WatchdogTimer_T16 != 0x0000u)
  {
    /*========================================================================*/
    /* -2- not enough timerTicks for an elapsed watchdog                      */
    /*========================================================================*/
    if (t_CCper_WatchdogCtrl_ST.WatchdogTimer_T16 > t_WtdogTimerTicks_UW)
    {
      /*----------------------------------------------------------------------*/
      /* Decrement Watchdog Timer                                             */
      /*----------------------------------------------------------------------*/
      t_CCper_WatchdogCtrl_ST.WatchdogTimer_T16 -= t_WtdogTimerTicks_UW;

    } /* -----2- END OF if(..) -----------------------------------------------*/
    /*========================================================================*/
    /* -2- watchdog will be elapsed                                           */
    /*========================================================================*/
    else
    {
      t_CCper_WatchdogCtrl_ST.WatchdogTimer_T16 = 0x0000u;

    } /* -----2- END OF else(..) ---------------------------------------------*/

    /*========================================================================*/
    /* -2- Watchdogtimer elapsed                                              */
    /*========================================================================*/
    if (t_CCper_WatchdogCtrl_ST.WatchdogTimer_T16 == 0x0000u)
    {
       t_CCper_WatchdogCtrl_ST.WatchdogTimer_T16 = d_CCwtdog_InitExtWdogTime;
     
      /*======================================================================*/
      /* Select activity                                                      */
      /*======================================================================*/
      /*======================================================================*/
      /* -3- ENABLE WATCHDOG                                                  */
      /*======================================================================*/
      if (d_CCglo_FlagEnableExternalWatchdog == 
         (t_CCper_WatchdogCtrl_ST.WatchdogState_UB & d_CCglo_ExternalWatchdogFlagMask))
      {

          /* Watchdog is enabled - now we could trigger it                      */
          t_CCper_WatchdogCtrl_ST.WatchdogState_UB |= d_CCglo_FlagTriggerExternalWatchdog;
          t_CCper_WatchdogCtrl_ST.WatchdogState_UB |= d_CCglo_StateExternalWatchdogEnabled;
        
      } /* -----3- END OF if(..) ---------------------------------------------*/
      /*======================================================================*/
      /* -3- other activity                                                   */
      /*======================================================================*/
      else
      {
        /*====================================================================*/
        /* -4- TRIGGER WATCHDOG                                               */
        /*====================================================================*/
        if( (d_CCglo_FlagEnableExternalWatchdog | d_CCglo_FlagTriggerExternalWatchdog) == (t_CCper_WatchdogCtrl_ST.WatchdogState_UB & d_CCglo_ExternalWatchdogFlagMask))
        {

          /*==================================================================*/
          /* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =*/
        /*                                                                    */
        /*  A T T E N T I O N :                                               */
        /*                                                                    */
        /*  Place your watchdog trigger here!                                 */
        /*                                                                    */
        /*                                                                    */
          /* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =*/
          /*==================================================================*/

          /*------------------------------------------------------------------*/
        /* START DEBUG        Dummy watchdog for test         DEBUG START     */
          /*------------------------------------------------------------------*/
        if (0x01 == wdStatus)
        {
            e_GIO_ST.Port_ST[3].Dclr_UL  = 0x01ul;
            wdStatus = 0x00;

        }
        else
        {
            e_GIO_ST.Port_ST[3].Dset_UL  = 0x01ul;
            wdStatus = 0x01;
        }



#ifdef TOGGLEPIN
              
          e_GIO_ST.Port_ST[0].Dset_UL = 0x10ul; 
          e_GIO_ST.Port_ST[0].Dclr_UL = 0x10ul;

#endif           

          /*------------------------------------------------------------------*/
        /*  END  DEBUG        Dummy watchdog for test         DEBUG  END      */
          /*------------------------------------------------------------------*/

        } /* ---4- END OF if(..) ---------------------------------------------*/
        /*====================================================================*/
        /* -4- other activity                                                 */
        /*====================================================================*/
        else
        {
          /*==================================================================*/
          /* -5- DISABLE WATCHDOG                                             */
          /*==================================================================*/
          if(d_CCglo_FlagDisableExternalWatchdog== (t_CCper_WatchdogCtrl_ST.WatchdogState_UB & d_CCglo_ExternalWatchdogFlagMask))
          {
          /* Reset watchdog state information                                   */
          t_CCper_WatchdogCtrl_ST.WatchdogState_UB &= (UBYTE) ~d_CCglo_StateExternalWatchdogEnabled;

          } /* -5- END OF if(..) ---------------------------------------------*/
          /*==================================================================*/
          /* -5- other activity                                               */
          /*==================================================================*/
          else
          {
            /*----------------------------------------------------------------*/
            /* nothing to do                                                  */
            /*----------------------------------------------------------------*/

          } /* -5- END OF else(..) -------------------------------------------*/
        } /* ---4- END OF else(..) -------------------------------------------*/
      } /* -----3- END OF else(..) -------------------------------------------*/
    } /* -------2- END OF if(..) ---------------------------------------------*/
  } /* ---------1- END OF if(..) ---------------------------------------------*/

  return;

} /*-+- END OF FUNCTION 'FUN_CCwtdog_TriggerExtWatchdog_V' -+-+-+-+-+-+-+-+-+-*/
/*lint +e40 */


/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CCwtdog_TriggerWatchdog_V                               |
|                                                                              |
|  Prototype at:   ADLATUS_ExternalWatchdog_tdf.h                              |
|                                                                              |
|  Description:    HIS Driver INTERFACE:                                       |
|                  Trigger for the external watchdog.                          |
|                                                                              |
|  Parameter:       --                                                         |
|                                                                              |
|  Return Value:    --                                                         |
|                                                                              |
+-----------------------------------------------------------------------------*/
void FUN_CCwtdog_TriggerWatchdogEraseMem_V (void )
{
  UWORD t_Elapsedmsec_UW;

  /*--------------------------------------------------------------------------*/
  /* read out the elapsed 1ms hardware timer ticks                            */
  /*--------------------------------------------------------------------------*/
  t_Elapsedmsec_UW = FUN_CChal_HardwareTimerCheck_UW();

  /*==========================================================================*/
  /* -1- 1ms is elapsed                                                       */
  /*==========================================================================*/
  if ( 0x0000u != t_Elapsedmsec_UW )
  {

    /*------------------------------------------------------------------------*/
    /* trigger the watchdog                                                   */
    /*------------------------------------------------------------------------*/
    FUN_CCwtdog_TriggerExtWatchdog_V ( t_Elapsedmsec_UW );

    /*------------------------------------------------------------------------*/
    /* trigger the internal watchdog                                          */
    /*------------------------------------------------------------------------*/
    FUN_CChal_TriggerIntWatchdog_V( t_Elapsedmsec_UW );

#ifdef cs_CCUDS_Audi
  #ifdef cs_CCwtdog_NormalADLATUS

    /*------------------------------------------------------------------------*/
    /* send the response pending if necessary                                 */
    /*------------------------------------------------------------------------*/
    t_CCper_WatchdogCtrl_ST.TXDelayCounter_UL += (ULONG) t_Elapsedmsec_UW;

    if (d_CCwtdog_TXDelayCounter <= t_CCper_WatchdogCtrl_ST.TXDelayCounter_UL)
    {
      t_CCper_WatchdogCtrl_ST.TXDelayCounter_UL = 0;
      FUN_CCcan_TxDataFromRam_V();
    }
  #endif
#endif

  } 
  return;
} /*-+- END OF FUNCTION 'FUN_CCwtdog_TriggerWatchdogEraseMem_V' +-+-+-+-+-+-+-*/

/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CCwtdog_TriggerWatchdogProgMem_V                        |
|                                                                              |
|  Prototype at:   ADLATUS_ExternalWatchdog_tdf.h                              |
|                                                                              |
|  Description:    HIS Driver INTERFACE:                                       |
|                  Trigger for the external watchdog, while programming        |
|                  memory.                                                     |
|                                                                              |
|  Parameter:       --                                                         |
|                                                                              |
|  Return Value:    --                                                         |
|                                                                              |
+-----------------------------------------------------------------------------*/
void FUN_CCwtdog_TriggerWatchdogProgMem_V (void )
{
  UWORD t_Elapsedmsec_UW;

  /*--------------------------------------------------------------------------*/
  /* read out the elapsed 1ms hardware timer ticks                            */
  /*--------------------------------------------------------------------------*/
  t_Elapsedmsec_UW = FUN_CChal_HardwareTimerCheck_UW();

  /*==========================================================================*/
  /* -1- 1ms is elapsed                                                       */
  /*==========================================================================*/
  if ( 0x0000u != t_Elapsedmsec_UW )
  {                        

    /*------------------------------------------------------------------------*/
    /* trigger the watchdog                                                   */
    /*------------------------------------------------------------------------*/
    FUN_CCwtdog_TriggerExtWatchdog_V ( t_Elapsedmsec_UW );

    /*------------------------------------------------------------------------*/
    /* trigger the internal watchdog                                          */
    /*------------------------------------------------------------------------*/
    FUN_CChal_TriggerIntWatchdog_V( t_Elapsedmsec_UW );

  }
  return;

} /*-+- END OF FUNCTION 'FUN_CCwtdog_TriggerWatchdogProgMem_V' -+-+-+-+-+-+-+-*/




/* ========================================================================== */
/*      E N D   O F   F I L E                                                 */
/* ========================================================================== */
