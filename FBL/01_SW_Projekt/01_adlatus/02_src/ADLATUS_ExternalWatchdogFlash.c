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
| $Id: ADLATUS_ExternalWatchdogFlash.c 4408 2008-11-06 17:15:01Z maennel $
| $HeadURL: http://smartsvn/svn/adlatusrepository/auslieferungen/S1NN/086350-S1NN-ADLATUS-TMS470R-MucSuc/20081201_S1NN_TMS470R_BlfIso-Source_V1.00/01_SW_Projekt/01_adlatus/02_src/ADLATUS_ExternalWatchdogFlash.c $
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
#define  d_CCwtdog_InitExtWdogTime      1/*(200)*/ /* 1 = 1ms */

/******************************************************************************/
/******************************************************************************/
/**                                                                          **/
/**     APPLICATION AREA: RESP.01                                            **/
/**     ----------------------------------------------------------------     **/
/**     S Y S T E M   C O N F I G U R A T I O N                              **/
/**     ----------------------------------------------------------------     **/
/**     response pending time     NOT USED IN THIE PROJECT!                  **/
/**                                                                          **/
/******************************************************************************/
/******************************************************************************/

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
WATCHDOGCONTROL_ST   t_CCper_WatchdogCtrl_ST;

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
|  Function Name:  FUN_CCwtdog_InitExtWatchdog_V                               |
|                                                                              |
|  Prototype at:   ADLATUS_ExternalWatchdog_tdf.h                              |
|                                                                              |
|  Description:    CUSTOMER INTERFACE:                                         |
|                  Init the external watchdog.                                 |
|                                                                              |
|  Parameter:       --                                                         |
|                                                                              |
|  Return Value:    --                                                         |
|                                                                              |
+-----------------------------------------------------------------------------*/
void FUN_CCwtdog_InitExtWatchdog_V (void)
{
  UWORD  t_Counter_UW;
  UWORD  t_WatchdogSize_UW;
  UBYTE* t_WatchdogRAM_PUB;
  UBYTE* t_WatchdogFlash_PUB;

  /*--------------------------------------------------------------------------*/
  /* Init chip select pin for output                                          */
  /*--------------------------------------------------------------------------*/

  /*--------------------------------------------------------------------------*/
  /* Init the Watchdog state register                                         */
  /*--------------------------------------------------------------------------*/
  t_CCper_WatchdogCtrl_ST.WatchdogState_UB  = d_CCwtdog_InitWatchdogState;
  t_CCper_WatchdogCtrl_ST.WatchdogTimer_T16 = d_CCwtdog_InitExtWdogTime;
  
  t_CCper_WatchdogCtrl_ST.TXDelayCounter_UL = 0x00;

#ifdef cs_CCwtdog_NormalADLATUS
  t_WatchdogFlash_PUB = (UBYTE*) d_CCadrinfo_WatchdogFlashStartAdr;
#else
  t_WatchdogFlash_PUB = (UBYTE*) d_CCadrinfo_ReprogWatchdogFlashStartAdr;
#endif
  t_WatchdogRAM_PUB   = (UBYTE*) d_CCadrinfo_WatchdogRamStartAdr;

  t_WatchdogSize_UW   = (UWORD)  d_CCadrinfo_WatchdogSize;  

  /*==========================================================================*/
  /* -1- Copy watchdog from Flash to RAM                                      */
  /*==========================================================================*/
  for ( t_Counter_UW = 0x0000u; 
        t_Counter_UW < t_WatchdogSize_UW; 
        t_Counter_UW ++ ) 
  {
    *t_WatchdogRAM_PUB = *t_WatchdogFlash_PUB;
    t_WatchdogRAM_PUB++;
    t_WatchdogFlash_PUB++;

 } /* -1- END OF for (..) ---------------------------------------------------*/

  return;

} /*-+- END OF FUNCTION 'FUN_CCwtdog_InitExtWatchdog_V' -+-+-+-+-+-+-+-+-+-+-+*/
/*lint +e40 */

/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CCwtdog_InitExtWatchdogIntern_V                         |
|                                                                              |
|  Prototype at:   ADLATUS_ExternalWatchdog_tdf.h                              |
|                                                                              |
|  Description:    Inits the TXDelayCounter                                    |
|                                                                              |
|  Parameter:       --                                                         |
|                                                                              |
|  Return Value:    --                                                         |
|                                                                              |
+-----------------------------------------------------------------------------*/
void FUN_CCwtdog_InitExtWatchdogIntern_V (void)
{

  /*--------------------------------------------------------------------------*/
  /* Reset the TXdelayCounter                                                 */
  /*--------------------------------------------------------------------------*/
  t_CCper_WatchdogCtrl_ST.TXDelayCounter_UL = 0x00u;

  return;

} /*-+- END OF FUNCTION 'FUN_CCwtdog_InitExtWatchdogIntern_V' -+-+-+-+-+-+-+-+*/

/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CCwtdog_GetExtWatchdogState_UB                          |
|                                                                              |
|  Prototype at:   ADLATUS_ExternalWatchdog_tdf.h                              |
|                                                                              |
|  Description:    CUSTOMER INTERFACE:                                         |
|                  Inform about the watchdog is enabled or not.                |
|                                                                              |
|  Parameter:       --                                                         |
|                                                                              |
|  Return Value:    UBYTE WatchdogState  0x00 .. watchdog disabled             |
|                                        0x80 .. watchdog enabled              |
|                                                                              |
+-----------------------------------------------------------------------------*/
UBYTE FUN_CCwtdog_GetExtWatchdogState_UB (void)
{
  return (t_CCper_WatchdogCtrl_ST.WatchdogState_UB & d_CCglo_StateExternalWatchdogEnabled);

} /*-+- END OF FUNCTION 'FUN_CCwtdog_GetExtWatchdogState_UB' -+-+-+-+-+-+-+-+-*/

/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CCwtdog_EnableExtWatchdog_V                             |
|                                                                              |
|  Prototype at:   ADLATUS_ExternalWatchdog_tdf.h                              |
|                                                                              |
|  Description:    CUSTOMER INTERFACE:                                         |
|                  This function enables the external watchdog.                |
|                                                                              |
|  Parameter:       --                                                         |
|                                                                              |
|  Return Value:    --                                                         |
|                                                                              |
+-----------------------------------------------------------------------------*/
void FUN_CCwtdog_EnableExtWatchdog_V (void)
{
  /*--------------------------------------------------------------------------*/
  /* Enable the system watchdog                                               */
  /*--------------------------------------------------------------------------*/
  t_CCper_WatchdogCtrl_ST.WatchdogState_UB |= d_CCglo_FlagEnableExternalWatchdog;
  
  t_CCper_WatchdogCtrl_ST.WatchdogState_UB |= d_CCglo_FlagTriggerExternalWatchdog;
  
  t_CCper_WatchdogCtrl_ST.WatchdogState_UB &= ~d_CCglo_FlagDisableExternalWatchdog;

  return;

} /*-+- END OF FUNCTION 'FUN_CCwtdog_EnableExtWatchdog_V' -+-+-+-+-+-+-+-+-+-+*/

/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CCwtdog_DisableExtWatchdog_V                            |
|                                                                              |
|  Prototype at:   ADLATUS_ExternalWatchdog_tdf.h                              |
|                                                                              |
|  Description:    CUSTOMER INTERFACE:                                         |
|                  This function disables the external watchdog.               |
|                                                                              |
|  Parameter:       --                                                         |
|                                                                              |
|  Return Value:    --                                                         |
|                                                                              |
+-----------------------------------------------------------------------------*/
void FUN_CCwtdog_DisableExtWatchdog_V (void)
{
  /*--------------------------------------------------------------------------*/
  /* Disable the system watchdog                                              */
  /*--------------------------------------------------------------------------*/
  
  /* t_CCper_WatchdogCtrl_ST.WatchdogState_UB |=          d_CCglo_FlagDisableExternalWatchdog; */
  /* t_CCper_WatchdogCtrl_ST.WatchdogState_UB &= (UBYTE) ~d_CCglo_FlagEnableExternalWatchdog; */
  /* t_CCper_WatchdogCtrl_ST.WatchdogState_UB &= (UBYTE) ~d_CCglo_FlagTriggerExternalWatchdog; */

  return;

} /*-+- END OF FUNCTION 'FUN_CCwtdog_DisableExtWatchdog_V' -+-+-+-+-+-+-+-+-+-*/


/* ========================================================================== */
/*      E N D   O F   F I L E                                                 */
/* ========================================================================== */
