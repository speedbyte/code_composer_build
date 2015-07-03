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
|  Filename:  ADLATUS_HAL.c                                                    |
|                                                                              |
|  Processor:  TMS470                                                          |
|  Compiler:   TMS470 C/C++ Compiler             Version 2.24                  |
|                                                                              |
|  Comment:   Hardware access layer for the target processor / controller.     |
|                                                                              |
+------------------------------------------------------------------------------+
| A U T H O R   I D E N T I T Y                                                |
+------------------------------------------------------------------------------+
|                                                                              |
| Initials     Name                      Company                               |
| --------     ---------------------     ----------------------------------    |
|   RS         Ralf Schmidgall           SMART Electronic Development GmbH     |
|                                                                              |
+------------------------------------------------------------------------------+
| R E V I S I O N   H I S T O R Y                                              |
+------------------------------------------------------------------------------+
|
| Subversion Info:
| $Id: ADLATUS_ParrotHal.c 4541 2008-12-02 10:47:09Z maennel $
| $HeadURL: http://smartsvn/svn/adlatusrepository/auslieferungen/S1NN/086350-S1NN-ADLATUS-TMS470R-MucSuc/20090305_S1NN_TMS470R_ParrotDriver_Hotfix/01_SW_Projekt/11_parrot/02_src/ADLATUS_ParrotHal.c $
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
#include "ADLATUS_PARHal_cdf.h"

/* ========================================================================== */
/* (2)  I N C L U D E S                                                       */
/* -------------------------------------------------------------------------- */
/*      T Y P E   D E F I N I T I O N   F I L E S  (TDF)                      */
/* ========================================================================== */
#include "ADLATUS_Types_tdf.h"                   /* must be the first include */
#include "ADLATUS_PARHAL_tdf.h"

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

/* ========================================================================== */
/* (6)  E X T E R N A L   D E F I N E S                                       */
/* -------------------------------------------------------------------------- */
/*                                                                            */
/* ========================================================================== */


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
HALCONTROL_ST   t_PARhal_HalCtrl_ST;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

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
|  Function Name:  FUN_CChal_InitHardwareTimer_V                               |
|                                                                              |
|  Prototype at:   ADLATUS_Hal_TDF.H                                           |
|                                                                              |
|  Description:    Initialisation for the global system timer of the           |
|                  SMART ADLATUS.                                              |
|                                                                              |
|  Parameter:      -                                                           |
|                                                                              |
|  Return Value:   -                                                           |
|                                                                              |
+-----------------------------------------------------------------------------*/
void FUN_PARhal_InitHardwareTimer_V (ULONG t_TimerTicksPerMS_UL)
{

  /*--------------------------------------------------------------------------*/
  /* Init the variables                                                       */
  /*--------------------------------------------------------------------------*/
  t_PARhal_HalCtrl_ST.HWTimerValue_UW = 0u;
  t_PARhal_HalCtrl_ST.IntClockFrequency_UL      = 0x00ul;
  t_PARhal_HalCtrl_ST.IntWatchdogTriggerTime_UW = 0x00ul;
  t_PARhal_HalCtrl_ST.IntWtdTimerReloadValue_UW = 0x00u;
  /*--------------------------------------------------------------------------*/
  /* Calculation for 1ms range:                                               */
  /* (I.) Resolution: 1 / (f RTICLK / 2 ) = Resolution                        */
  /* (II.) 1ms / Resolution = Ticks for 1ms                                   */
  /*                                                                          */
  /* Example for 24 MHz:   (I.)  1 / (24 000 000 / 2) 1/s = 83.33 ns          */
  /*                       (II.) 1ms / 83.33 ns = 12 000                      */
  /*--------------------------------------------------------------------------*/
  t_PARhal_HalCtrl_ST.TimerTicksForOneMilliSecond_UL = 
                                      t_TimerTicksPerMS_UL;

  /*--------------------------------------------------------------------------*/
  /*                                                                          */
  /* Timer Setup                                                              */
  /*                                                                          */
  /*--------------------------------------------------------------------------*/

  /*--------------------------------------------------------------------------*/
  /* set the prescaler for counter to 0x0001; -> PRRELD                       */
  /* counter is a 21 bit one; with 24MHz (maximum for flashing) the counter   */
  /* overflows every 174ms                                                    */
  /*--------------------------------------------------------------------------*/
  e_SARRTI_ST.DRTIPCTL_UN.DRTIPCTL_ST.PREDL_B11 = 0x0001u;

  /* enable count in U/P mode */
  e_SARRTI_ST.DRTICNTEN_UN.DRTICNTEN_ST.CNTEN_B2 = 0x00u;

  /* init the timer */
  t_PARhal_HalCtrl_ST.OldTimerValue_UL = e_SARRTI_ST.DRTICNTR_UN.DRTICNTR_ST.DRTICNTR_B21;

  return;

} /*-+- END OF FUNCTION 'FUN_CCtarget_InitHardwareTimer_V' +-+-+-+-+-+-+-+-+-+*/

/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_PARhal_GetHWTimerValue_UW                                |
|                                                                              |
|  Prototype at:   ADLATUS_Hal_TDF.H                                           |
|                                                                              |
|  Description:    This function returns a value from the HW-Timer.            |
|                  The timervalue is needed by the random function to creat    |
|                  a random value.                                             |
|                                                                              |
|  Parameter:      -                                                           |
|                                                                              |
|  Return Value:   t_PARhal_HalCtrl_ST.HWTimerValue_UW .. stored HWTimer value  |
|                                                                              |
+-----------------------------------------------------------------------------*/
UWORD FUN_PARhal_GetHWTimerValue_UW (void )
{
  UWORD t_TimerValue_UW;

  /* return the stored timer value                                            */
  if (t_PARhal_HalCtrl_ST.HWTimerValue_UW == 0u)
  {
    t_TimerValue_UW = (UWORD)t_PARhal_HalCtrl_ST.OldTimerValue_UL;
  }
  else
  {
    t_TimerValue_UW = t_PARhal_HalCtrl_ST.HWTimerValue_UW;
  }

  return (t_TimerValue_UW);
                         
} /*-+- END OF FUNCTION 'FUN_PARhal_GetHWTimerValue_UW' +-+-+-+-+-+-+-+-+-+-+-+*/




/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_PARhal_StoreHwTimerValue_V                               |
|                                                                              |
|  Prototype at:   ADLATUS_Hal_TDF.H                                           |
|                                                                              |
|  Description:    This function stores the current value from the HW-Timer.   |
|                                                                              |
|  Parameter:      -                                                           |
|                                                                              |
|  Return Value:   -                                                           |
|                                                                              |
+-----------------------------------------------------------------------------*/
void FUN_PARhal_StoreHwTimerValue_V (void )
{
  ULONG t_TimerValue_UL;

  /*--------------------------------------------------------------------------*/
  /* Get the timer value                                                      */
  /*--------------------------------------------------------------------------*/
  t_TimerValue_UL = e_SARRTI_ST.DRTICNTR_UN.DRTICNTR_ST.DRTICNTR_B21;

  /*--------------------------------------------------------------------------*/
  /* Store the timer value                                                    */
  /*--------------------------------------------------------------------------*/
  t_PARhal_HalCtrl_ST.HWTimerValue_UW += (UWORD)~t_TimerValue_UL; 

  return;
                         
} /*-+- END OF FUNCTION 'FUN_PARhal_StoreHwTimerValue_V' -+-+-+-+-+-+-+-+-+-+-+*/

/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_PARhal_GetPerformanceCounter_UL                          |
|                                                                              |
|  Prototype at:   ADLATUS_Hal_TDF.H                                           |
|                                                                              |
|  Description:    Returns the current hardware timer value.                   |
|                  To calculate the time quantum from this value devide it by  |
|                  the returnvalue from FUN_PARhal_GetPerfCntTicks_UL.          |
|                                                                              |
|  Parameter:      -                                                           |
|                                                                              |
|  Return Value:   Current timer value.                                        |
|                                                                              |
+-----------------------------------------------------------------------------*/
ULONG  FUN_PARhal_GetPerformanceCounter_UL (void)
{
  ULONG t_return_UL;

  /* Calculate the ticks per ms from the current prescaler settings. */
  t_return_UL = e_SARRTI_ST.DRTICNTR_UN.DRTICNTR_ST.DRTICNTR_B21 << 0x0Bu;
  
  return t_return_UL;
}

/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_PARhal_GetPerfCntTicks_UL                               |
|                                                                              |
|  Prototype at:   ADLATUS_Hal_TDF.H                                           |
|                                                                              |
|  Description:    Returns a reference value for the current timer ticks.      |
|                  The current implementation returns the ticks per            |
|                  millisecond.                                                |
|                                                                              |
|  Parameter:      -                                                           |
|                                                                              |
|  Return Value:   Timer ticks per ms.                                         |
|                                                                              |
+-----------------------------------------------------------------------------*/
ULONG  FUN_PARhal_GetPerfCntTicks_UL (void)
{
  ULONG t_return_UL;

  /*--------------------------------------------------------------------------*/
  /* Calculate the ticks per ms from the current prescaler settings. */
  t_return_UL = (t_PARhal_HalCtrl_ST.TimerTicksForOneMilliSecond_UL) << 11;


  return t_return_UL;
}

/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_PARhal_WaitTime_V                                       |
|                                                                              |
|  Prototype at:   ADLATUS_Hal_TDF.H                                           |
|                                                                              |
|  Description:   This function wait a define time in µs or ms, according to   |
|                 argument 2.                                                  |
|                                                                              |
|  Parameter:     t_Time_UW - Time to wait                                     |
|                 t_Unit_B unit to use                                         |
|                                                                              |
|  Return Value:   none                                                        |
|                                                                              |
+-----------------------------------------------------------------------------*/
void FUN_PARhal_WaitTime_V(UWORD t_Time_UW, BOOL t_Unit_B) /* FIXME: is the method of handling an underflow of the t_Time_UL variable the best one? */
{
  ULONG t_StartTime_UL;
  ULONG t_ActualTime_UL;
  ULONG t_TimerTicksPerDivision_UL;
  ULONG t_TimeSetting_UL = (ULONG) t_Time_UW;
  ULONG t_Time_UL = (ULONG) t_Time_UW-1u;

  /*==========================================================================*/
  /* -1- unit is 100us                                                        */
  /*==========================================================================*/
  if (d_PARhal_Unit100us_B == t_Unit_B)
  {
    /*------------------------------------------------------------------------*/
    /* init t_TimerTicksPerDivision_UL                                        */
    /*------------------------------------------------------------------------*/
    t_TimerTicksPerDivision_UL = FUN_PARhal_GetPerfCntTicks_UL();
    t_TimerTicksPerDivision_UL = t_TimerTicksPerDivision_UL / 10ul;

  } /* ---1- END OF if(..) ---------------------------------------------------*/
  /*==========================================================================*/
  /* -1- unit is ms                                                           */
  /*==========================================================================*/
  else
  {
    /*------------------------------------------------------------------------*/
    /* init t_TimerTicksPerDivision_UL                                        */
    /*------------------------------------------------------------------------*/
    t_TimerTicksPerDivision_UL = FUN_PARhal_GetPerfCntTicks_UL();

  } /* ---1- END OF else(..) -------------------------------------------------*/

  /*--------------------------------------------------------------------------*/
  /* get current timer value                                                  */
  /*--------------------------------------------------------------------------*/
  t_StartTime_UL = FUN_PARhal_GetPerformanceCounter_UL();

  /*==========================================================================*/
  /* -1- continue till time is over                                           */
  /*==========================================================================*/
  while(t_Time_UL <= t_TimeSetting_UL)
  {
    Feed_Watchdog_V();
  
    /*------------------------------------------------------------------------*/
    /* get actual counter value                                               */
    /*------------------------------------------------------------------------*/
    t_ActualTime_UL = FUN_PARhal_GetPerformanceCounter_UL();

    /*========================================================================*/
    /* -2- one division is over                                               */
    /*========================================================================*/
    while ( (t_ActualTime_UL - t_StartTime_UL) > t_TimerTicksPerDivision_UL)
    {
      /*----------------------------------------------------------------------*/
      /* update start time                                                    */
      /*----------------------------------------------------------------------*/
      t_StartTime_UL += t_TimerTicksPerDivision_UL;

      /*----------------------------------------------------------------------*/
      /* decrement time left over                                             */
      /*----------------------------------------------------------------------*/
      t_Time_UL--;

    } /* -2- END OF while(..) ------------------------------------------------*/

  } /* ---1- END OF while(..) ------------------------------------------------*/

  /*--------------------------------------------------------------------------*/
  /* return                                                                   */
  /*--------------------------------------------------------------------------*/
  return;

} /*-+- END OF FUNCTION 'FUN_PARper_WaitTime_V' -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/


/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_PARhal_TimeoutStart_V                                   |
|                                                                              |
|  Prototype at:   ADLATUS_Hal_TDF.H                                           |
|                                                                              |
|  Description:   This function start the timeout monitoring of a process.     |                                                  |
|                                                                              |
|  Parameter:     t_StartTime_PUL - Pointer to a time variable                 |
|                                                                              |
|  Return Value:   none                                                        |
|                                                                              |
+-----------------------------------------------------------------------------*/
void FUN_PARhal_TimeoutStart_V(ULONG* t_StartTime_PUL)
{
  /*--------------------------------------------------------------------------*/
  /* get current timer value                                                  */
  /*--------------------------------------------------------------------------*/
  *t_StartTime_PUL = FUN_PARhal_GetPerformanceCounter_UL();

  /*--------------------------------------------------------------------------*/
  /* return                                                                   */
  /*--------------------------------------------------------------------------*/
  return;

} /*-+- END OF FUNCTION 'FUN_PARhal_TimeoutStart_V' -+-+-+-+-+-+-+-+-+-+-+-+-+-*/


/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_PARhal_TimeoutOccured_B                                 |
|                                                                              |
|  Prototype at:   ADLATUS_Hal_TDF.H                                           |
|                                                                              |
|  Description:    This function check if a timeout has occured.               |                                                  |
|                                                                              |
|  Parameter:     t_StartTime_PUL - Pointer to a time variable                 |
|                 t_Timeout_PUW - Defines the timeout value in milliseconds    |
|                                                                              |
|  Return Value:   FALSE No timeout occured.                                   |
|                  TRUE  Timeout occured.                                      |
|                                                                              |
+-----------------------------------------------------------------------------*/
BOOL FUN_PARhal_TimeoutOccured_B(ULONG* t_StartTime_PUL, UWORD* t_TimeoutMs_PUW)
{
  ULONG t_ActualTime_UL;
  ULONG t_TimerTicksPerMillisec_UL;
  BOOL  t_ReturnValue_B;

  /*--------------------------------------------------------------------------*/
  /* init t_TimerTicksPerDivision_UL                                          */
  /*--------------------------------------------------------------------------*/
  t_TimerTicksPerMillisec_UL = FUN_PARhal_GetPerfCntTicks_UL();

  /*--------------------------------------------------------------------------*/
  /* get current timer value                                                  */
  /*--------------------------------------------------------------------------*/
  t_ActualTime_UL = FUN_PARhal_GetPerformanceCounter_UL();

  /*==========================================================================*/
  /* -1- one millisecond is over                                              */
  /*==========================================================================*/
  while ( ((t_ActualTime_UL - (*t_StartTime_PUL)) > t_TimerTicksPerMillisec_UL) && \
          (0x00u < (*t_TimeoutMs_PUW)) )
  {
    /*------------------------------------------------------------------------*/
    /* actualise the timer values                                             */
    /*------------------------------------------------------------------------*/
    (*t_StartTime_PUL) += t_TimerTicksPerMillisec_UL;
    (*t_TimeoutMs_PUW) -= 0x01u;

  } /* -1- END OF while(..) --------------------------------------------------*/

  /*==========================================================================*/
  /* -1- timeout occured                                                      */
  /*==========================================================================*/
  if(0x00u == (*t_TimeoutMs_PUW))
  {
    t_ReturnValue_B = (BOOL)~FALSE;

  } /* -1- END OF if(..) -----------------------------------------------------*/
  /*==========================================================================*/
  /* -1- no timeout occured                                                   */
  /*==========================================================================*/
  else
  {
    t_ReturnValue_B = FALSE;

  } /* -1- END OF else(..) ---------------------------------------------------*/

  /*--------------------------------------------------------------------------*/
  /* return from function                                                     */
  /*--------------------------------------------------------------------------*/
  return  (t_ReturnValue_B);

} /*-+- END OF FUNCTION 'FUN_PARhal_TimeoutOccured_B' -+-+-+-+-+-+-+-+-+-+-+-+-*/


/*lint +e40 */    
/* ========================================================================== */
/*      E N D   O F   F I L E                                                 */
/* ========================================================================== */
