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
|  Filename:  ADLATUS_Timer.c                                                  |
|                                                                              |
|  Comment:   This file includes the Software timer handling for the           |
|             SMART ADLATUS.                                                   |
|                                                                              |
+------------------------------------------------------------------------------+
| R E V I S I O N   H I S T O R Y                                              |
+------------------------------------------------------------------------------+
|
| Subversion Info:
| $Id: ADLATUS_Timer.c 4396 2008-11-04 09:26:32Z maennel $
| $HeadURL: http://smartsvn/svn/adlatusrepository/auslieferungen/S1NN/086350-S1NN-ADLATUS-TMS470R-MucSuc/20081201_S1NN_TMS470R_BlfIso-Source_V1.00/01_SW_Projekt/09_audilib/02_src/ADLATUS_Timer.c $
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
#include "ADLATUS_Types_tdf.h"                 /* must be the first include   */
#include "ADLATUS_Timer_tdf.h"                 /* Timer module internal fkts  */
#include "ADLATUS_Hal_tdf.h"                   /* HW-Timer Access             */

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
typedef struct _TIMERCONTROL
{
  const PTR_TO_SWTIMER_FKT* SWTimerFunctions_PST; /* Ptr to all SW Timer Functions  */

} TIMERCTRL_ST;

/* ========================================================================== */
/* (7)  L O C A L   V A R I A B L E S  / C O N S T A N T S                    */
/* -------------------------------------------------------------------------- */
/*      O N L Y   F O R   T H I S   M O D U L                                 */
/* ========================================================================== */
TIMERCTRL_ST  t_CCtime_TimerCtrl_ST; /*l_int !e765 */

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

/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CCtim_InitSWTimerHandler_V                              |
|                                                                              |
|  Prototype at:   ADLATUS_Timer_tdf.h                                         |
|                                                                              |
|  Description:    This function inits the Software Timer Handler for the      |
|                  ADLATUS.                                                    |
|                                                                              |
|  Parameter:      const SWTIMERHANDLERINTERFACE_ST* ..                        |
|                                                 Ptr to interface structure   |
|                                                                              |
|  Return Value:   --                                                          |
|                                                                              |
+-----------------------------------------------------------------------------*/
void FUN_CCtim_InitSWTimerHandler_V (const SWTIMERHANDLERINTERFACE_ST* t_Interface_PST)
{
  t_CCtime_TimerCtrl_ST.SWTimerFunctions_PST = t_Interface_PST->SWTimerFunctions_PST;

  return;

} /*-+- END OF FUNCTION 'FUN_CCtim_InitSWTimerHandler_V' +-+-+-+-+-+-+-+-+-+-+*/




/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CCtim_SystemTimerCheck_V                                |
|                                                                              |
|  Prototype at:   ADLATUS_Timer_tdf.h                                         |
|                                                                              |
|  Description:    This function checks the global system HW timer. If it is   |
|                  elapsed the timer will be restarted an all SW timer will be |
|                  decremented.                                                |
|                                                                              |
|  Parameter:      --                                                          |
|                                                                              |
|  Return Value:   --                                                          |
|                                                                              |
+-----------------------------------------------------------------------------*/
void FUN_CCtim_SystemTimerCheck_V (void)
{

  UWORD t_TimerTicks_UW;
  UBYTE t_Index_UB;


  /*--------------------------------------------------------------------------*/
  /* Init internal variables                                                  */
  /*--------------------------------------------------------------------------*/
  t_Index_UB = 0x00;

  /*--------------------------------------------------------------------------*/
  /* Check the target system timer                                            */
  /*--------------------------------------------------------------------------*/
  t_TimerTicks_UW = FUN_CChal_HardwareTimerCheck_UW();

  /*==========================================================================*/
  /* -1- Target system timer is elapsed                                       */
  /*==========================================================================*/
  if (0x0000u != t_TimerTicks_UW )
  {    
    /*========================================================================*/
    /* -2- Trigger all SW Timers                                              */
    /*========================================================================*/
    while ( ((PTR_TO_SWTIMER_FKT) 0xFFFFu) !=                   /*l_int !e923 */
                         t_CCtime_TimerCtrl_ST.SWTimerFunctions_PST[t_Index_UB])
    {      
      /* Call all the trigger functions                                       */
      t_CCtime_TimerCtrl_ST.SWTimerFunctions_PST[t_Index_UB] (t_TimerTicks_UW); 

      /* Check next entry                                                     */
      t_Index_UB += 0x01;

    } /* -2- END OF while (..) -----------------------------------------------*/
  } /* ---1- END OF if(..) ---------------------------------------------------*/

  return;

} /*-+- END OF FUNCTION 'FUN_CCtim_SystemTimerCheck_V' +-+-+-+-+-+-+-+-+-+-+-+*/
   
/* ========================================================================== */
/*      E N D   O F   F I L E                                                 */
/* ========================================================================== */
