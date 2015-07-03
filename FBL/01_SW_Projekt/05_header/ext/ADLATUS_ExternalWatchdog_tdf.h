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
|  Filename:  ADLATUS_ExternalWatchdog_tdf.h                                   |
|                                                                              |
|  Comment:   This file includes all type defines and extern declarations for  |
|             the SMART ADLATUS External watchdog handling.                    |
|                                                                              |
+------------------------------------------------------------------------------+
| R E V I S I O N   H I S T O R Y                                              |
+------------------------------------------------------------------------------+
|
| Subversion Info:
| $Id: ADLATUS_ExternalWatchdog_tdf.h 4396 2008-11-04 09:26:32Z maennel $
| $HeadURL: http://smartsvn/svn/adlatusrepository/auslieferungen/S1NN/086350-S1NN-ADLATUS-TMS470R-MucSuc/20081201_S1NN_TMS470R_BlfIso-Source_V1.00/01_SW_Projekt/05_header/ext/ADLATUS_ExternalWatchdog_tdf.h $
|
+------------------------------------------------------------------------------+
|      (c) SMART ELECTRONIC DEVELOPMENT GMBH - all rights reserved             |
+-----------------------------------------------------------------------------*/

/* ========================================================================== */
/*      O P E N   M U L T I P L E   I N C L U D E   P R O T E C T I O N       */
/* ========================================================================== */
#ifndef __TDF_EXTERNALWATCHDOG_
#define __TDF_EXTERNALWATCHDOG_
/* ========================================================================== */



/* ========================================================================== */
/*                                                                            */
/*      E X T E R N   D E C L A R A T I O N S                                 */
/*                                                                            */
/* ========================================================================== */
/* No entry                                                                   */

extern const ULONG RAMWDT;
extern const ULONG ROMWDT;

/* ========================================================================== */
/*                                                                            */
/*      G L O B A L   F U N C T I O N   P R O T O T Y P E S                   */
/*                                                                            */
/* ===+==============================+======================================= */
void  FUN_CCwtdog_InitExtWatchdog_V         (void);
void  FUN_CCwtdog_InitExtWatchdogIntern_V   (void);
void  FUN_CCwtdog_EnableExtWatchdog_V       (void);
void  FUN_CCwtdog_DisableExtWatchdog_V      (void);
void  FUN_CCwtdog_TriggerWatchdog_V         (void);
void  FUN_CCwtdog_TriggerWatchdogEraseMem_V (void);
void  FUN_CCwtdog_TriggerWatchdogProgMem_V  (void);
void  FUN_CCwtdog_TriggerExtWatchdog_V      (UWORD t_WtdogTimerTicks_UW);
UBYTE FUN_CCwtdog_GetExtWatchdogState_UB    (void);
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

typedef struct _WATCHDOGCONTROL
{
  ULONG     TXDelayCounter_UL;

  UBYTE     WatchdogState_UB;      /*  1 Byte - Watchdog State Register       */
  UBYTE     PinState_UB;           /*  1 Byte - Reserved for ULONG alignment  */
  TIMER_16  WatchdogTimer_T16;     /*  2 Byte - WatchdogTimer                 */

                                   /* ----------------------------------------*/
} WATCHDOGCONTROL_ST;              /*  4 Byte                                 */


/* ========================================================================== */
/* (7)  L O C A L   V A R I A B L E S  / C O N S T A N T S                    */
/* -------------------------------------------------------------------------- */
/*      O N L Y   F O R   T H I S   M O D U L                                 */
/* ========================================================================== */
extern WATCHDOGCONTROL_ST   t_CCper_WatchdogCtrl_ST;


/* ========================================================================== */
/*      C L O S E   M U L T I P L E   I N C L U D E   P R O T E C T I O N     */
/* ========================================================================== */
#endif
/* ========================================================================== */

/* ========================================================================== */
/*      E N D   O F   F I L E                                                 */
/* ========================================================================== */
