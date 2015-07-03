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
|  Filename:  ADLATUS_EventHandlerAUDI.c                                       |
|                                                                              |
|  Comment:   This file includes the event handler and specific functions for  |
|             the AUDI programming sequence.                                   |
|                                                                              |
+------------------------------------------------------------------------------+
| R E V I S I O N   H I S T O R Y                                              |
+------------------------------------------------------------------------------+
|
| Subversion Info:
| $Id: ADLATUS_EventHandlerAudi.c 4396 2008-11-04 09:26:32Z maennel $
| $HeadURL: http://smartsvn/svn/adlatusrepository/auslieferungen/S1NN/086350-S1NN-ADLATUS-TMS470R-MucSuc/20081201_S1NN_TMS470R_BlfIso-Source_V1.00/01_SW_Projekt/03_fsw/02_src/ADLATUS_EventHandlerAudi.c $
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
#include "ADLATUS_EventHandlerAudi_cdf.h"
#include "ADLATUS_ConfigSystem_cdf.h"
#include "ADLATUS_CusInterfaceAudi_cdf.h"
#include "ADLATUS_NvmHandlerAudi_cdf.h"
#include "ADLATUS_ConfigProject_cdf.h"

/* ========================================================================== */
/* (2)  I N C L U D E S                                                       */
/* -------------------------------------------------------------------------- */
/*      T Y P E   D E F I N I T I O N   F I L E S  (TDF)                      */
/* ========================================================================== */
#include "ADLATUS_Types_tdf.h"                 /* must be the first include */
#include "ADLATUS_ApplicationInterface_tdf.h"
#include "ADLATUS_EventHandlerAudi_tdf.h"
#include "ADLATUS_ValHandler_cdf.h"
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Project specific functions                                                 */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#include "ADLATUS_Hal_tdf.h"
#include "ADLATUS_CanHandler_tdf.h"
#include "ADLATUS_Kwp2000Audi_tdf.h"      
#include "ADLATUS_NvmHandler_tdf.h"      
#include "ADLATUS_CusInterfaceAudi_tdf.h"
#include "ADLATUS_Tp_tdf.h"
#include "ADLATUS_SessionHandler_tdf.h"
#include "ADLATUS_ValHandler_tdf.h"
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

/* -------------------------------------------------------------------------- */
/*      P R O J E C T   S T A T E   R E G I S T E R   D E F I N E S           */
/*                                                                            */
/*      ProjectState_UB =   7 6 5 4  3 2 1 0                                  */
/*                          | | | |  | | | |                                  */
/*                          | | | |  | | | +---  FlashCoreEntryTimer elapsed  */
/*                          | | | |  | | +-----  not used                     */
/*                          | | | |  | +-------  not used                     */
/*                          | | | |  +---------  not used                     */
/*                          | | | |                                           */
/*                          | | | +------------  not used                     */
/*                          | | +--------------  not used                     */
/*                          | +----------------  not used                     */
/*                          +------------------  not used                     */
/*                                                                            */
/* -------------------------------------------------------------------------- */
#define  d_CCeventaudi_ProjectStateInit           (UBYTE) 0x00u
#define  d_CCeventaudi_FlashCoreEntryTimerElapsed (UBYTE) 0x01u
#define  d_CCeventaudi_TimelockTimerElapsed       (UBYTE) 0x02u
#define  d_CCeventaudi_TimelockRunning            (UBYTE) 0x04u

/* -------------------------------------------------------------------------- */
/* FlashCore entry time - time after reset during flashcore is waiting for    */
/*                        a service request                                   */
/* -------------------------------------------------------------------------- */
#define  d_CCtim_FlashCoreEntryTime               5200u  /* ms                */

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

/*----------------------------------------------------------------------------*/
/*  Project Control Structure                                                 */
/*----------------------------------------------------------------------------*/
EVENTCONTROLAUDI_ST   t_CCeventaudi_PrjCtrl_ST;


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*  S Y S T E M   E V E N T   F L A G S                                       */
/*                                                                            */
/*----------------------------------------------------------------------------*/
const UBYTE c_CCeventaudi_EventFlags_AUB[] = 
{ 
  /* 00 */ d_CCeventaudi_INIT_EventFlags_UB,
  /*--------------------------------------------------------------------------*/
  /* SYSTEM RESET                                                             */
  /*--------------------------------------------------------------------------*/
  /* 01 */ d_CCeventaudi_SETFLAG___SystemReset_UB,
  /* 02 */ d_CCeventaudi_SETFLAG___SystemReset_UB,
  /* 03 */ d_CCeventaudi_RESETFLAG_SystemReset_UB,
  /*--------------------------------------------------------------------------*/
  /* Channel is opened                                                        */
  /*--------------------------------------------------------------------------*/
  /* 04 */ d_CCeventaudi_SETFLAG___CloseChannel_UB,
  /* 05 */ d_CCeventaudi_SETFLAG___CloseChannel_UB,
  /* 06 */ d_CCeventaudi_RESETFLAG_CloseChannel_UB,
  /*--------------------------------------------------------------------------*/
  /* Channel has to be opened                                                 */
  /*--------------------------------------------------------------------------*/
  /* 07 */ d_CCeventaudi_SETFLAG___OpenChannel_UB,
  /* 08 */ d_CCeventaudi_SETFLAG___OpenChannel_UB,
  /* 09 */ d_CCeventaudi_RESETFLAG_OpenChannel_UB,  
  /*--------------------------------------------------------------------------*/
  /* Default Session Request                                                  */
  /*--------------------------------------------------------------------------*/
  /* 0A */ d_CCeventaudi_SETFLAG___DefSessionRequ_UB,  /* [OPL286] */
  /* 0B */ d_CCeventaudi_SETFLAG___DefSessionRequ_UB,
  /* 0C */ d_CCeventaudi_RESETFLAG_DefSessionRequ_UB,
  /*--------------------------------------------------------------------------*/
  /* FSW is not valid flag                                                    */
  /*--------------------------------------------------------------------------*/
  /* 0D */ d_CCeventaudi_SETFLAG___FSWInvalid_UB,
  /* 0E */ d_CCeventaudi_SETFLAG___FSWInvalid_UB,
  /* 0F */ d_CCeventaudi_RESETFLAG_FSWInvalid_UB,
  /*--------------------------------------------------------------------------*/
  /* Flash request received                                                   */
  /*--------------------------------------------------------------------------*/
  /* 10 */ d_CCeventaudi_SETFLAG___FlashRequest_UB,
  /* 11 */ d_CCeventaudi_SETFLAG___FlashRequest_UB,
  /* 12 */ d_CCeventaudi_RESETFLAG_FlashRequest_UB,
  /*--------------------------------------------------------------------------*/
  /* Reserved                                                                 */
  /*--------------------------------------------------------------------------*/
  /* 13 */ d_CCeventaudi_SETFLAG___ECUResetRequest_UB,
  /* 14 */ d_CCeventaudi_SETFLAG___ECUResetRequest_UB,
  /* 15 */ d_CCeventaudi_RESETFLAG_ECUResetRequest_UB,

  /*--------------------------------------------------------------------------*/
  /* Limelock running                                                         */
  /*--------------------------------------------------------------------------*/
  /* 16 */ d_CCeventaudi_SETFLAG___Timelock_UB,
  /* 17 */ d_CCeventaudi_SETFLAG___Timelock_UB,
  /* 18 */ d_CCeventaudi_RESETFLAG_Timelock_UB,
  /*--------------------------------------------------------------------------*/
  /* LOOK FOR ALL ENTRIES                                                     */
  /*--------------------------------------------------------------------------*/
  
  /* 19 */ d_CCeventaudi_SETFLAG___AllEntries_UB

}; /* -+- END OF TABLE -------------------------------------------------------*/


/* ========================================================================== */
/* (8)  L O C A L   F U N C T I O N   P R O T O T Y P E S                     */
/* -------------------------------------------------------------------------- */
/*      O N L Y   F O R   T H I S   M O D U L                                 */
/* ========================================================================== */
void  FUN__CCeventaudi_ProjectInitIntern_V(void); 

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
|  Function Name:  FUN_CCeventaudi_EventInit_V                                 |
|                                                                              |
|  Prototype at:   ADLATUS_EventHandlerAudi_tdf.h                              |
|                                                                              |
|  Description:    Init routine for project control register.                  |
|                                                                              |
|  Parameter:      -                                                           |
|                                                                              |
|  Return Value:   -                                                           |
|                                                                              |
+-----------------------------------------------------------------------------*/
void FUN_CCeventaudi_EventInit_V (void)
{
  t_CCeventaudi_PrjCtrl_ST.EventRegister_AUB = 0u;
  t_CCeventaudi_PrjCtrl_ST.FlashCoreEntry_T16 = d_CCeventaudi_FlashCoreEntryTimerElapsed;
  t_CCeventaudi_PrjCtrl_ST.TimelockTimer_T16 =  d_CCeventaudi_FlashCoreEntryTimerElapsed;
  t_CCeventaudi_PrjCtrl_ST.TimelockTimerHelpCounterFor_msec_UW = 0u;

  /* Init ProjectStateRegister                                                */ 
  t_CCeventaudi_PrjCtrl_ST.ProjectState_UB = d_CCeventaudi_ProjectStateInit;

  FUN__CCeventaudi_ProjectInitIntern_V(); 
}

/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN__CCeventaudi_ProjectInitIntern_V                        |
|                                                                              |
|  Prototype at:   ADLATUS_EventHandlerAudi_tdf.h                              |
|                                                                              |
|  Description:    Init routine for project control register.                  |
|                                                                              |
|  Parameter:      -                                                           |
|                                                                              |
|  Return Value:   -                                                           |
|                                                                              |
+-----------------------------------------------------------------------------*/
void FUN__CCeventaudi_ProjectInitIntern_V (void)
{
  UBYTE t_temp_UB;
  
  /* Init ProjectStateRegister                                                */ 
  t_CCeventaudi_PrjCtrl_ST.ProjectState_UB = d_CCeventaudi_ProjectStateInit;

  /* set special states */
  t_temp_UB =  (t_CCeventaudi_PrjCtrl_ST.EventRegister_AUB & \
                ( d_CCeventaudi_SETFLAG___FSWInvalid_UB | \
                  d_CCeventaudi_SETFLAG___Timelock_UB ));  /* [OPL286] */

  t_CCeventaudi_PrjCtrl_ST.EventRegister_AUB = t_temp_UB;            

  /* Init ProjectTimer                                                        */ 
  t_CCeventaudi_PrjCtrl_ST.FlashCoreEntry_T16 = (TIMER_16) d_CCtim_FlashCoreEntryTime;

  if(d_CCeventaudi_TimelockRunning == (d_CCeventaudi_TimelockRunning & 
     t_CCeventaudi_PrjCtrl_ST.ProjectState_UB))
  {
    /* set Timelocktimer */ 
    t_CCeventaudi_PrjCtrl_ST.TimelockTimer_T16 = c_CCconprj_Timelock_T16;
  }
  else
  {						   
    /* Init Timelocktimer                                                       */ 
    t_CCeventaudi_PrjCtrl_ST.TimelockTimer_T16 = d_CCeventaudi_FlashCoreEntryTimerElapsed;
  }
  
} /*-+- END OF FUNCTION 'FUN_CCeventaudi_EventInit_V' -+-+-+-+-+-+-+-+-+-+-+-+*/


/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+*/
void FUN_CCeventaudi_SetProjectTimer_V (TIMER_16 TimerValue_T)
{
  /* Init ProjectTimer                                                        */ 
  t_CCeventaudi_PrjCtrl_ST.FlashCoreEntry_T16 = TimerValue_T;
  
  t_CCeventaudi_PrjCtrl_ST.ProjectState_UB &= ~d_CCeventaudi_FlashCoreEntryTimerElapsed;

}

/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CCeventaudi_EventProcessing_V                           |
|                                                                              |
|  Prototype at:   ADLATUS_EventHandlerAudi_tdf.h                              |
|                                                                              |
|  Description:    Process routine for the occured events.                     |
|                                                                              |
|  Parameter:      --                                                          |
|                                                                              |
|  Return Value:   --                                                          |
|                                                                              |
+-----------------------------------------------------------------------------*/
void FUN_CCeventaudi_EventProcessing_V ( void )
{
  UBYTE t_Counter_UB;
  ULONG t_StartTime_UL;
  ULONG t_ActualTime_UL;
  ULONG t_TimerTicksPerMs_UL;

  /*==========================================================================*/
  /* -1- SYSTEM RESET REQUEST                                                 */
  /*==========================================================================*/
  if (d_CCeventaudi_SETFLAG___SystemReset_UB == (d_CCeventaudi_SETFLAG___SystemReset_UB &  \
                            t_CCeventaudi_PrjCtrl_ST.EventRegister_AUB))
  {
        /* Reset Flag                                                             */
        t_CCeventaudi_PrjCtrl_ST.EventRegister_AUB &= d_CCeventaudi_RESETFLAG_SystemReset_UB; 

    /*------------------------------------------------------------------------*/
    /* we have to wait some time that the response pending CAN message        */
    /* is already sent wenn we perform the reset                              */  /* [OPL273] */
    /*------------------------------------------------------------------------*/

    /*------------------------------------------------------------------------*/
    /* set the wait time to 3ms                                               */
    /*------------------------------------------------------------------------*/
    t_Counter_UB = (UBYTE)0x03;

    /*------------------------------------------------------------------------*/
    /* get the counter relevant data from the HAL                             */
    /*------------------------------------------------------------------------*/
    t_TimerTicksPerMs_UL = FUN_CChal_GetPerfCntTicks_UL();
    t_StartTime_UL = FUN_CChal_GetPerformanceCounter_UL();

    /*========================================================================*/
    /* -2- wait untill the parameterised milliseconds are over                */
    /*========================================================================*/
    while (t_Counter_UB != (UBYTE)0x00)
    {
      t_ActualTime_UL = FUN_CChal_GetPerformanceCounter_UL();
      
      /*======================================================================*/
      /* -3- one millisecond is over                                          */
      /*======================================================================*/
      if ( (t_ActualTime_UL - t_StartTime_UL) > t_TimerTicksPerMs_UL)
      {
        t_StartTime_UL += t_TimerTicksPerMs_UL;
        t_Counter_UB--;

      } /* ---3- END OF if(..) -----------------------------------------------*/

      /*----------------------------------------------------------------------*/
      /* call the system timer for the watchdog                               */
      /*----------------------------------------------------------------------*/
      FUN_CCtim_SystemTimerCheck_V();

    } /* -----2- END OF while(..) --------------------------------------------*/

    /*------------------------------------------------------------------------*/
    /* call the reset function                                                */
    /*------------------------------------------------------------------------*/
        FUN_CChal_SWReset_V();
 
  } /* -------1- END OF if(..) -----------------------------------------------*/

  /*==========================================================================*/
  /* -1- Check the next event                                                 */
  /*==========================================================================*/
  else
  {
    /*==================================================================*/
    /* -2- the ECU reset request is set                                 */
    /*==================================================================*/
    if (d_CCeventaudi_SETFLAG___ECUResetRequest_UB ==   \
        (d_CCeventaudi_SETFLAG___ECUResetRequest_UB & \
          t_CCeventaudi_PrjCtrl_ST.EventRegister_AUB))
    {
       /* Reset Flag                                                    */
       t_CCeventaudi_PrjCtrl_ST.EventRegister_AUB &=   \
                              d_CCeventaudi_RESETFLAG_ECUResetRequest_UB; 

      /*----------------------------------------------------------------*/
      /* send positiv response for session control programming session  */
      /*----------------------------------------------------------------*/
       FUN_CCcan_TxDiagResponse_V(d_CCconprj_ECUResetRspTG);

      /* Reset Flash Request                                            */
      FUN_CCapplint_ResetFlashRequest_V();

    } /* -2- END OF if(..) ---------------------------------------------*/
    /*==================================================================*/
    /* -2- Check the next event                                         */
    /*==================================================================*/
    else
    {
      /*==================================================================*/
      /* -3- the Default Session Request is set                           */  /* [OPL286] */
      /*==================================================================*/
      if (d_CCeventaudi_SETFLAG___DefSessionRequ_UB ==   \
          (d_CCeventaudi_SETFLAG___DefSessionRequ_UB & \
            t_CCeventaudi_PrjCtrl_ST.EventRegister_AUB))
      {
         /* Reset Flag                                                    */
         t_CCeventaudi_PrjCtrl_ST.EventRegister_AUB &=   \
                                d_CCeventaudi_RESETFLAG_DefSessionRequ_UB; 

        /*----------------------------------------------------------------*/
        /* send positiv response for session control programming session  */
        /*----------------------------------------------------------------*/
         FUN_CCcan_TxDiagResponse_V(d_CCconprj_DefSessionRspTG);

        /* Reset Flash Request                                            */
        FUN_CCapplint_ResetFlashRequest_V();

      } /* -5- END OF if(..) ---------------------------------------------*/
      /*==================================================================*/
      /* -5- Check the next event                                         */
      /*==================================================================*/
      else
      {

      } /* -5- END OF else (if(..)) --------------------------------------*/
    } /* ---4- END OF else (if(..)) --------------------------------------*/
  } /* ---------1- END OF else (if(..)) --------------------------------------*/

  return;

} /* -+- END OF FUNCTION 'FUN_CCeventaudi_EventProcessing_V' -+-+-+-+-+-+-+-+-*/


/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CCeventaudi_EventHandler_B                              |
|                                                                              |
|  Prototype at:   ADLATUS_EventHandlerAudi_tdf.h                              |
|                                                                              |
|  Description:    Flaghandler for project specific system events.             |
|                                                                              |
|  Parameter:      UBYTE Handle_UB   .. Instruction handle for flag register   |
|                                                                              |
|  Return Value:   BOOL ~FALSE       .. Flag is set                            |
|                  BOOL  FALSE       .. Flag is not set                        |
|                                                                              |
+-----------------------------------------------------------------------------*/
BOOL FUN_CCeventaudi_EventHandler_B (UBYTE t_Handle_UB)
{
  BOOL  t_Return_b;
  UBYTE t_Flag_UB;

  /*--------------------------------------------------------------------------*/
  /* Init local variables                                                     */
  /*--------------------------------------------------------------------------*/
  t_Return_b = FALSE;
  t_Flag_UB  = c_CCeventaudi_EventFlags_AUB[t_Handle_UB & d_CCeventaudi_FlagMask_UB];

  /*==========================================================================*/
  /* -1- Evaluate Handle                                                      */
  /*==========================================================================*/
  switch (t_Handle_UB & d_CCeventaudi_CmdMask_UB)
  {
    /*------------------------------------------------------------------------*/
    /* SET FLAG                                                               */
    /*------------------------------------------------------------------------*/
    case d_CCeventaudi_Set_UB:
      t_CCeventaudi_PrjCtrl_ST.EventRegister_AUB |= t_Flag_UB; 
      t_Return_b = (BOOL) ~FALSE;
      break;
      /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

    /*------------------------------------------------------------------------*/
    /* RESET FLAG                                                             */
    /*------------------------------------------------------------------------*/
    case d_CCeventaudi_Reset_UB:
      t_CCeventaudi_PrjCtrl_ST.EventRegister_AUB &= t_Flag_UB; 
      t_Return_b = (BOOL) ~FALSE;
      break;
      /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

    /*------------------------------------------------------------------------*/
    /* LOOKFOR FLAG                                                           */
    /*------------------------------------------------------------------------*/
    case d_CCeventaudi_Look_UB:
      /*======================================================================*/
      /* -2- Flag is set                                                      */
      /*======================================================================*/
      if (  t_Flag_UB == \
           (t_CCeventaudi_PrjCtrl_ST.EventRegister_AUB & t_Flag_UB) )
      {
        t_Return_b = (BOOL) ~FALSE;
      } /* -2- END OF if(..) -------------------------------------------------*/

      break;
      /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

    /*------------------------------------------------------------------------*/
    /* INIT                                                                   */
    /*------------------------------------------------------------------------*/
    default:
      t_CCeventaudi_PrjCtrl_ST.EventRegister_AUB = t_Flag_UB; 
      break;
      /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  } /* -1- END OF switch (..) ------------------------------------------------*/
  
  return (t_Return_b);

} /*-+- END OF FUNCTION 'FUN__CCeventaudi_EventHandler_B' -+-+-+-+-+-+-+-+-+-+*/

/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CCeventaudi_ProjectTimer_V                              |
|                                                                              |
|  Prototype at:   ADLATUS_EventHandlerAudi_tdf.h                              |
|                                                                              |
|  Description:    Timer for project specific global system events.            |
|                                                                              |
|  Parameter:      UWORD t_TimerTicks_UW .. timerticks since last call         |
|                                                                              |
|  Return Value:   -                                                           |
|                                                                              |
+-----------------------------------------------------------------------------*/
void FUN_CCeventaudi_ProjectTimer_V ( UWORD t_TimerTicks_UW)
{
  /*==========================================================================*/
  /* -1- Timer has at least <TimerTicks> milliseconds left, so decrement it   */
  /*==========================================================================*/
  if (t_CCeventaudi_PrjCtrl_ST.FlashCoreEntry_T16 >= t_TimerTicks_UW  )
  {
    t_CCeventaudi_PrjCtrl_ST.FlashCoreEntry_T16 -= t_TimerTicks_UW;

    /* if we hit zero while decrementing, set the "elapsed"-flag */
    if (t_CCeventaudi_PrjCtrl_ST.FlashCoreEntry_T16 == 0x0000u)
    {	
      t_CCeventaudi_PrjCtrl_ST.ProjectState_UB |= d_CCeventaudi_FlashCoreEntryTimerElapsed;				
    } 
  }   
  /*==========================================================================*/
  /* -1- Timer has elapsed (less than <TimerTicks> ms left                    */
  /*==========================================================================*/
  else
  {
    /*========================================================================*/
    /* -2- Timer was active before ( != 0x0000 )                              */
    /*========================================================================*/
    if ( 0x0000u != t_CCeventaudi_PrjCtrl_ST.FlashCoreEntry_T16 )
    {
      t_CCeventaudi_PrjCtrl_ST.FlashCoreEntry_T16 = 0x0000u;
      t_CCeventaudi_PrjCtrl_ST.ProjectState_UB |= d_CCeventaudi_FlashCoreEntryTimerElapsed;				
    }
  } 

  /*==========================================================================*/
  /* look for timelock timer */
  /*==========================================================================*/
  /* TimelockTimerHelpCounterFor_msec_UW counts the msec                      */
  /* if 1 sec elapsed (TimelockTimerHelpCounterFor_msec_UW >= 1000)           */
  /* TimelockTimer_T16 decrement of 1 sec                                     */
  /*==========================================================================*/
  if (t_CCeventaudi_PrjCtrl_ST.TimelockTimer_T16 > 0x00u  )	  /* timer active ??? */
  {
    t_CCeventaudi_PrjCtrl_ST.TimelockTimerHelpCounterFor_msec_UW += t_TimerTicks_UW;
  
    if (t_CCeventaudi_PrjCtrl_ST.TimelockTimerHelpCounterFor_msec_UW < 1000u)
    {
      /* do nothing	*/
    }
    else /* 1 sec elapsed */
    {
      t_CCeventaudi_PrjCtrl_ST.TimelockTimer_T16 -= 1u;	 /* decrement 1 sec. */
      t_CCeventaudi_PrjCtrl_ST.TimelockTimerHelpCounterFor_msec_UW -= 1000u; /* decrement 1000 msec.	*/

      /* if we hit zero while decrementing, set the "elapsed"-flag */
      if (t_CCeventaudi_PrjCtrl_ST.TimelockTimer_T16 == 0x0000u)
      {	
        t_CCeventaudi_PrjCtrl_ST.ProjectState_UB |= d_CCeventaudi_TimelockTimerElapsed;
        t_CCeventaudi_PrjCtrl_ST.TimelockTimerHelpCounterFor_msec_UW = 0x00u;				
      }
    } 
  }

  return;

} /* -+- END OF FUNCTION 'FUN_CCeventaudi_ProjectTimer_V' -+-+-+-+-+-+-+-+-+-+*/


/* ========================================================================== */
/*      E N D   O F   F I L E                                                 */
/* ========================================================================== */
