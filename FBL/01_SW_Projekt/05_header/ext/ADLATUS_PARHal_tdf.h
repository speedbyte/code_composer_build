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
|  Filename:  ADLATUS_HAL_TDF.h                                                |
|                                                                              |
|  Comment:   This file includes all type defines and extern declarations for  |
|             the target processor / controller.                               |
|                                                                              |
+------------------------------------------------------------------------------+
| R E V I S I O N   H I S T O R Y                                              |
+------------------------------------------------------------------------------+
|
| Subversion Info:
| $Id: ADLATUS_Hal_tdf.h 4396 2008-11-04 09:26:32Z maennel $
| $HeadURL: http://smartsvn/svn/adlatusrepository/projekte/S1NN/086350-S1NN-ADLATUS-TMS470R-MucSuc/2sw/5int/01_SW_Projekt/05_header/ext/ADLATUS_Hal_tdf.h $
|
+------------------------------------------------------------------------------+
|      (c) SMART ELECTRONIC DEVELOPMENT GMBH - all rights reserved             |
+-----------------------------------------------------------------------------*/

/* ========================================================================== */
/*      O P E N   M U L T I P L E   I N C L U D E   P R O T E C T I O N       */
/* ========================================================================== */
#ifndef __TDF_PARHAL_
#define __TDF_PARHAL_
/* ========================================================================== */


/* ========================================================================== */
/*                                                                            */
/*  T Y P E   D E F I N I T I O N S                                           */
/*                                                                            */
/* ========================================================================== */
typedef struct _HALCONTROL
{
  /*-------------------------------------------------------------------------------*/
  /* For System Initialisation ..                                                  */
  /*-------------------------------------------------------------------------------*/
  ULONG IntClockFrequency_UL;       /*  4 Byte - Internal clock frequency          */
  UWORD IntWatchdogTriggerTime_UW;  /*  2 Byte - Timeout for int. watchdog trigger */

  /*-------------------------------------------------------------------------------*/
  /* For System in process ..                                                      */
  /*-------------------------------------------------------------------------------*/

  /* Internal Watchdog                                                             */
  /*-------------------------------------------------------------------------------*/
  UWORD IntWtdTimerReloadValue_UW;  /*  2 Byte - Internal Watchdog reload value    */

  /* Internal System Timer                                                         */
  /*-------------------------------------------------------------------------------*/
  UWORD HWTimerValue_UW;            /*  2 Byte - HW Timer value for random value   */
  ULONG OldTimerValue_UL;
  ULONG TimerTicksForOneMilliSecond_UL;

} HALCONTROL_ST;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


/* ========================================================================== */
/*                                                                            */
/*      E X T E R N   D E C L A R A T I O N S                                 */
/*                                                                            */
/* ========================================================================== */
extern HALCONTROL_ST t_PARhal_HalCtrl_ST;

extern const ULONG RAMHAL;
extern const ULONG ROMHAL;

/* ========================================================================== */
/*                                                                            */
/*      G L O B A L   F U N C T I O N   P R O T O T Y P E S                   */
/*                                                                            */
/* ========================================================================== */
void  FUN_PARhal_InitHAL_V                (const HALINTERFACE_ST* t_HalInterface_PST);

void  FUN_PARhal_DisGlobalInterrupt_V     (void);
void  FUN_PARhal_SWReset_V                (void);

void  FUN_PARhal_InitPLL_V                (void);

void  FUN_PARhal_InitHardwareTimer_V      (ULONG);
UWORD FUN_PARhal_HardwareTimerCheck_UW    (void);
UWORD FUN_PARhal_GetHWTimerValue_UW       (void);
void  FUN_PARhal_StoreHwTimerValue_V      (void);

void  FUN_PARhal_InitIntWatchdog_V        (void);
void  FUN_PARhal_EnableIntWatchdog_V      (void);
void  FUN_PARhal_DisableIntWatchdog_V     (void);
void  FUN_PARhal_TriggerIntWatchdog_V     (UWORD t_SysTimeTimerTicks_UW );

void  FUN_PARhal_SetEndInit_V             (void);
void  FUN_PARhal_ClearEndInit_V           (void);

ULONG  FUN_PARhal_GetPerformanceCounter_UL (void);
ULONG  FUN_PARhal_GetPerfCntTicks_UL (void);

void FUN_PARhal_WaitTime_V(UWORD , BOOL);
void FUN_PARhal_TimeoutStart_V(ULONG* );
BOOL FUN_PARhal_TimeoutOccured_B(ULONG* , UWORD* );
/* ========================================================================== */
/*      C L O S E   M U L T I P L E   I N C L U D E   P R O T E C T I O N     */
/* ========================================================================== */
#endif
/* ========================================================================== */

/* ========================================================================== */
/*      E N D   O F   F I L E                                                 */
/* ========================================================================== */
