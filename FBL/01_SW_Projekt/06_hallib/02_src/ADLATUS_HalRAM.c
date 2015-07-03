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
|  Filename:  ADLATUS_HalRAM.c                                                 |
|                                                                              |
|  Processor:  TMS470                                                          |
|  Compiler:   TMS470 C/C++ Compiler             Version 2.24                  |
|                                                                              |
|  Comment:   Hardware access layer; functions that have to run in RAM.        |
|                                                                              |
+------------------------------------------------------------------------------+
| A U T H O R   I D E N T I T Y                                                |
+------------------------------------------------------------------------------+
|                                                                              |
| Initials     Name                      Company                               |
| --------     ---------------------     ----------------------------------    |
|   KA         Markus Karas              SMART Electronic Development GmbH     |
|                                                                              |
+------------------------------------------------------------------------------+
| R E V I S I O N   H I S T O R Y                                              |
+------------------------------------------------------------------------------+
|
| Subversion Info:
| $Id: ADLATUS_HalRAM.c 4481 2008-11-27 13:28:39Z maennel $
| $HeadURL: http://smartsvn/svn/adlatusrepository/auslieferungen/S1NN/086350-S1NN-ADLATUS-TMS470R-MucSuc/20081201_S1NN_TMS470R_BlfIso-Source_V1.00/01_SW_Projekt/06_hallib/02_src/ADLATUS_HalRAM.c $
|
+------------------------------------------------------------------------------+
|      (c) SMART ELECTRONIC DEVELOPMENT GMBH - all rights reserved             |
+-----------------------------------------------------------------------------*/

/* ========================================================================== */
/* (1)  I N C L U D E S                                                       */
/* -------------------------------------------------------------------------- */
/*      C O N S T A N T   D E F I N I T I O N   F I L E S  (CDF)              */
/* ========================================================================== */
#include "ADLATUS_Global_cdf.h"

/* ========================================================================== */
/* (2)  I N C L U D E S                                                       */
/* -------------------------------------------------------------------------- */
/*      T Y P E   D E F I N I T I O N   F I L E S  (TDF)                      */
/* ========================================================================== */
#include "ADLATUS_Types_tdf.h"                   /* must be the first include */
#include "ADLATUS_HAL_tdf.h"

#include "tms470regs.h"

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
#define TOGGLEPIN

/* ========================================================================== */
/* (6)  E X T E R N A L   D E F I N E S                                       */
/* -------------------------------------------------------------------------- */
/*                                                                            */
/* ========================================================================== */
/* NO ENTRY                                                                   */

/* ========================================================================== */
/* (6)  L O C A L  T Y P E   D E F I N E S                                    */
/* -------------------------------------------------------------------------- */
/*      O N L Y   F O R   T H I S   M O D U L                                 */
/* ========================================================================== */
/* NO ENTRY                                                                   */

/* ========================================================================== */
/* (7)  L O C A L   V A R I A B L E S  / C O N S T A N T S                    */
/* -------------------------------------------------------------------------- */
/*      O N L Y   F O R   T H I S   M O D U L                                 */
/* ========================================================================== */
/* NO ENTRY                                                                   */

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
/* NO ENTRY                                                                   */

/*lint -e40 */    
/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CChal_HardwareTimerCheck_UW                             |
|                                                                              |
|  Prototype at:   ADLATUS_Hal_TDF.H                                           |
|                                                                              |
|  Description:    Global system timer for the ComCore / FlashCore.            |
|                                                                              |
|  Parameter:      -                                                           |
|                                                                              |
|  Return Value:   UWORD  t_CalculatedTimerTicks_UW .. Milli seconds since     |
|                                                      last time timer was     |
|                                                      elapsed.                |
|                                                                              |
+-----------------------------------------------------------------------------*/
UWORD FUN_CChal_HardwareTimerCheck_UW (void)
{
  UWORD t_CalculatedTimerTicks_UW;
  ULONG t_NewTimerValue_UL;
  ULONG t_TimeDelta_UL;

  t_CalculatedTimerTicks_UW        = 0x0000u;

  /*--------------------------------------------------------------------------*/
  /* Get the timer value                                                      */
  /*                                                                          */
  /* NOTE: We use the CNTR.20:0                                               */
  /*                                                                          */
  /*        +-------------+------------+                                      */
  /*        |  CNTR.20:0  |  MOD.10:0  |                                      */
  /*        +-------------+------------+                                      */
  /*       MSB                        LSB                                     */
  /*                                                                          */
  /* The CNTR.20:0 contains the timer value we need. MOD.10:0 is the actual   */
  /* value of the prescale counter (we don't need it).                        */
  /*--------------------------------------------------------------------------*/
  t_NewTimerValue_UL = e_SARRTI_ST.DRTICNTR_UN.DRTICNTR_ST.DRTICNTR_B21;

  /*--------------------------------------------------------------------------*/
  /* Calculate the difference (take notice of the overflow).                  */
  /* the timer has 21 valid bits.                                             */
  /*--------------------------------------------------------------------------*/
  t_TimeDelta_UL = (t_NewTimerValue_UL - t_CChal_HalCtrl_ST.OldTimerValue_UL) & 0x001FFFFFul;

  /*==========================================================================*/
  /* -1- Calculate the 1ms time cycles                                        */
  /*     Algorithm: (Ticks < Difference)                                      */
  /*==========================================================================*/
  while ( t_TimeDelta_UL >= t_CChal_HalCtrl_ST.TimerTicksForOneMilliSecond_UL)
  {
    t_TimeDelta_UL -= t_CChal_HalCtrl_ST.TimerTicksForOneMilliSecond_UL;
    t_CalculatedTimerTicks_UW ++;
  } /* -1- END OF while(..) --------------------------------------------------*/

  /*==========================================================================*/
  /* -1- the time since last calculation is equal or more than 1ms            */
  /*==========================================================================*/
  if(0x0000u != t_CalculatedTimerTicks_UW)
  {
    /*------------------------------------------------------------------------*/
    /* Store new value for the next cycle but subtract the remaining delta    */
    /* note: the timer has only 21 valid bits.                                */
    /*------------------------------------------------------------------------*/
     t_CChal_HalCtrl_ST.OldTimerValue_UL = (t_NewTimerValue_UL - t_TimeDelta_UL) & 0x001FFFFFul;

    /*------------------------------------------------------------------------*/
    /* NOTE: If no full ms is elapsed since last calculation, don't update    */
    /*       the OldTimerValue. So the calculation for the next cycle is      */
    /*       more exact.                                                      */
    /*------------------------------------------------------------------------*/

  } /* -1- END OF if(..) -----------------------------------------------------*/

  return (t_CalculatedTimerTicks_UW);

} /*-+- END OF FUNCTION 'FUN_CChal_HardwareTimerCheck_UW' +-+-+-+-+-+-+-+-+-+-*/


/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CChal_TriggerIntWatchdog_V                              |
|                                                                              |
|  Prototype at:   ADLATUS_Hal_TDF.H                                           |
|                                                                              |
|  Description:    This function triggers the internal watchdog                |
|                                                                              |
|  Parameter:      UWORD t_SysTimeTimerTicks_UW .. ticks since last trigger    |
|                                                                              |
|  Return Value:   -                                                           |
|                                                                              |
+-----------------------------------------------------------------------------*/
void FUN_CChal_TriggerIntWatchdog_V (UWORD t_SysTimeTimerTicks_UW )
{
  /* NOTE: This is only to prevent a compiler warning. The parameter is       */
  /*       necessary because of the function compatibility to other time      */
  /*       triggert routines!!                                                */
  t_SysTimeTimerTicks_UW |= 0x0000u;

#ifdef TOGGLEPIN
              
          e_GIO_ST.Port_ST[0].Dset_UL = 0x10ul; 
          e_GIO_ST.Port_ST[0].Dclr_UL = 0x10ul;

#endif   
  /* Reload the internal watchdog value register                              */
  /* no watchdog has to be initialized, but we trigger the wd */
  e_SARRTI_ST.WKEY_UL = (UBYTE) 0xE5u;
  e_SARRTI_ST.WKEY_UL = (UBYTE) 0xA3u;

  return;

} /*-+- END OF FUNCTION 'FUN_CChal_TriggerIntWatchdog_V' -+-+-+-+-+-+-+-+-+-+-*/
/*lint +e40 */ 



/* ========================================================================== */
/*      E N D   O F   F I L E                                                 */
/* ========================================================================== */
