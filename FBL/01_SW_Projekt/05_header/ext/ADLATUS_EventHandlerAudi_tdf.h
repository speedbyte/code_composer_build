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
|  Filename:  ADLATUS_EventHandlerAudi_tdf.h                                   |
|                                                                              |
|  Comment:   This file includes all type defines and extern declarations for  |
|             the customer specific event handler.                             |
|                                                                              |
+------------------------------------------------------------------------------+
| R E V I S I O N   H I S T O R Y                                              |
+------------------------------------------------------------------------------+
|
| Subversion Info:
| $Id: ADLATUS_EventHandlerAudi_tdf.h 4396 2008-11-04 09:26:32Z maennel $
| $HeadURL: http://smartsvn/svn/adlatusrepository/auslieferungen/S1NN/086350-S1NN-ADLATUS-TMS470R-MucSuc/20081201_S1NN_TMS470R_BlfIso-Source_V1.00/01_SW_Projekt/05_header/ext/ADLATUS_EventHandlerAudi_tdf.h $
|
+------------------------------------------------------------------------------+
|      (c) SMART ELECTRONIC DEVELOPMENT GMBH - all rights reserved             |
+-----------------------------------------------------------------------------*/

/* ========================================================================== */
/*      O P E N   M U L T I P L E   I N C L U D E   P R O T E C T I O N       */
/* ========================================================================== */
#ifndef __TDF_EVENTHANDLERAUDI_
#define __TDF_EVENTHANDLERAUDI_
/* ========================================================================== */

/* ========================================================================== */
/*                                                                            */
/*      T Y P E D E F                                                         */
/*                                                                            */
/* ========================================================================== */

/* -------------------------------------------------------------------------- */
/*      T Y P E - D E F I N E S                                               */
/*  ---------------------------------------------------------------------     */
/*  Event Handler Control Structure                                           */
/* -------------------------------------------------------------------------- */
typedef struct _EVENTCONTROLAUDI
{
  UBYTE     EventRegister_AUB;
                                   /*  1 Byte - Event Flag Register           */
  UBYTE     ProjectState_UB;       /*  1 Byte - Project State Register        */

  TIMER_16  FlashCoreEntry_T16;    /*  2 Byte - Timer for FlashCore Entry     */
  
  TIMER_16  TimelockTimer_T16;     /* for timelock */

  UWORD     TimelockTimerHelpCounterFor_msec_UW;
                                   /* --------------------------------------- */
} EVENTCONTROLAUDI_ST;             /*                                         */



/* ========================================================================== */
/*                                                                            */
/*      E X T E R N   D E C L A R A T I O N S                                 */
/*                                                                            */
/* ========================================================================== */
extern const TIMER_16 c_CCconprj_EraseResetTime_T16;

#ifdef cs_CCTP20
  extern const TIMER_16 c_CCconprj_StopCommResetTime_T16;
#endif

extern const TIMER_16 c_CCconprj_Timelock_T16;

extern EVENTCONTROLAUDI_ST   t_CCeventaudi_PrjCtrl_ST;


/* ========================================================================== */
/*                                                                            */
/*      G L O B A L   F U N C T I O N   P R O T O T Y P E S                   */
/*                                                                            */
/* ========================================================================== */

/*----------------------------------------------------------------------------*/
/* Project file initialisation                                                */
/*----------------------------------------------------------------------------*/
void FUN_CCeventaudi_EventInit_V   ( void );
/*----------------------------------------------------------------------------*/
/* EventHandler - set/reset/lookfor flags                                     */
/*----------------------------------------------------------------------------*/
void  FUN_CCeventaudi_ProjectTimer_V     ( UWORD t_PrjTimerTicks_UW );
void  FUN_CCeventaudi_EventProcessing_V  ( void);
BOOL  FUN_CCeventaudi_EventHandler_B     ( UBYTE t_Handle_UB);
void  FUN_CCeventaudi_SetProjectTimer_V  ( TIMER_16 TimerValue_T);

/* ========================================================================== */
/*      C L O S E   M U L T I P L E   I N C L U D E   P R O T E C T I O N     */
/* ========================================================================== */
#endif
/* ========================================================================== */

/* ========================================================================== */
/*      E N D   O F   F I L E                                                 */
/* ========================================================================== */
