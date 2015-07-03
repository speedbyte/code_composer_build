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
|  Filename:  ADLATUS_EventHandlerAudi_cdf.h                                   |
|                                                                              |
|  Comment:   This file includes all defines and constants for the specific    |
|             event handler.                                                   |
|                                                                              |
+------------------------------------------------------------------------------+
| R E V I S I O N   H I S T O R Y                                              |
+------------------------------------------------------------------------------+
|
| Subversion Info:
| $Id: ADLATUS_EventHandlerAudi_cdf.h 4396 2008-11-04 09:26:32Z maennel $
| $HeadURL: http://smartsvn/svn/adlatusrepository/auslieferungen/S1NN/086350-S1NN-ADLATUS-TMS470R-MucSuc/20081201_S1NN_TMS470R_BlfIso-Source_V1.00/01_SW_Projekt/05_header/ext/ADLATUS_EventHandlerAudi_cdf.h $
|
+------------------------------------------------------------------------------+
|      (c) SMART ELECTRONIC DEVELOPMENT GMBH - all rights reserved             |
+-----------------------------------------------------------------------------*/

/* ========================================================================== */
/*      O P E N   M U L T I P L E   I N C L U D E   P R O T E C T I O N       */
/* ========================================================================== */
#ifndef __CDF_EVENTHANDLERAUDI_
#define __CDF_EVENTHANDLERAUDI_
/* ========================================================================== */


/* ========================================================================== */
/*                                                                            */
/*  C O M M O N   D E F I N E S   F O R   F L A G   R E G I S T E R S         */
/*                                                                            */
/* ========================================================================== */
#define d_CCeventaudi_Set_UB                           0x20
#define d_CCeventaudi_Look_UB                          0x40
#define d_CCeventaudi_Reset_UB                         0x80
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define d_CCeventaudi_CmdMask_UB                      (UBYTE) 0xE0u
#define d_CCeventaudi_FlagMask_UB                     (UBYTE) 0x1Fu
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

/* ========================================================================== */
/*                                                                            */
/*  E V E N T   H A N D L E   D E F I N E S for UDS                          */
/*                                                                            */
/* ========================================================================== */
#define  d_CCeventaudi_InitEventHandler_UB        (UBYTE) (d_CCeventaudi_Set_UB   + 0x00 )
/*----------------------------------------------------------------------------*/
/* SYSTEM RESET                                                               */
/*----------------------------------------------------------------------------*/
#define  d_CCeventaudi_SET_____SystemReset_UB     (UBYTE) ( d_CCeventaudi_Set_UB   + 0x01 )
#define  d_CCeventaudi_LOOKFOR_SystemReset_UB     (UBYTE) ( d_CCeventaudi_Look_UB  + 0x02 )
#define  d_CCeventaudi_RESET___SystemReset_UB     (UBYTE) ( d_CCeventaudi_Reset_UB + 0x03 )
/*----------------------------------------------------------------------------*/
/* SYSTEM SHUTDOWN State 1                                                    */
/*----------------------------------------------------------------------------*/
#define  d_CCeventaudi_SET_____CloseChannel_UB    (UBYTE) ( d_CCeventaudi_Set_UB   + 0x04 )
#define  d_CCeventaudi_LOOKFOR_CloseChannel_UB    (UBYTE) ( d_CCeventaudi_Look_UB  + 0x05 )
#define  d_CCeventaudi_RESET___CloseChannel_UB    (UBYTE) ( d_CCeventaudi_Reset_UB + 0x06 )
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
#define  d_CCeventaudi_SET_____OpenChannel_UB     (UBYTE) ( d_CCeventaudi_Set_UB   + 0x07 )
#define  d_CCeventaudi_LOOKFOR_OpenChannel_UB     (UBYTE) ( d_CCeventaudi_Look_UB  + 0x08 )
#define  d_CCeventaudi_RESET___OpenChannel_UB     (UBYTE) ( d_CCeventaudi_Reset_UB + 0x09 )
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* DefaultSession Request                                                     */  /* [OPL286] */
/*----------------------------------------------------------------------------*/
#define  d_CCeventaudi_SET_____DefSessionRequ_UB  (UBYTE) ( d_CCeventaudi_Set_UB   + 0x0A ) 
#define  d_CCeventaudi_LOOKFOR_DefSessionRequ_UB  (UBYTE) ( d_CCeventaudi_Look_UB  + 0x0B )
#define  d_CCeventaudi_RESET___DefSessionRequ_UB  (UBYTE) ( d_CCeventaudi_Reset_UB + 0x0C )
/*----------------------------------------------------------------------------*/
/* FSW is not valid flag                                                      */
/*----------------------------------------------------------------------------*/
#define  d_CCeventaudi_SET_____FSWInvalid_UB      (UBYTE) ( d_CCeventaudi_Set_UB   + 0x0D )
#define  d_CCeventaudi_LOOKFOR_FSWInvalid_UB      (UBYTE) ( d_CCeventaudi_Look_UB  + 0x0E )
#define  d_CCeventaudi_RESET___FSWInvalid_UB      (UBYTE) ( d_CCeventaudi_Reset_UB + 0x0F )

/*----------------------------------------------------------------------------*/
/* Flash request received                                                     */
/*----------------------------------------------------------------------------*/
#define  d_CCeventaudi_SET_____FlashRequest_UB    (UBYTE) ( d_CCeventaudi_Set_UB   + 0x10 )
#define  d_CCeventaudi_LOOKFOR_FlashRequest_UB    (UBYTE) ( d_CCeventaudi_Look_UB  + 0x11 )
#define  d_CCeventaudi_RESET___FlashRequest_UB    (UBYTE) ( d_CCeventaudi_Reset_UB + 0x12 )

/*----------------------------------------------------------------------------*/
/* ECU reset request                                                          */
/*----------------------------------------------------------------------------*/
#define  d_CCeventaudi_SET_____ECUResetRequest_UB (UBYTE) ( d_CCeventaudi_Set_UB   + 0x13 )
#define  d_CCeventaudi_LOOKFOR_ECUResetRequest_UB (UBYTE) ( d_CCeventaudi_Look_UB  + 0x14 )
#define  d_CCeventaudi_RESET___ECUResetRequest_UB (UBYTE) ( d_CCeventaudi_Reset_UB + 0x15 )
         
/*----------------------------------------------------------------------------*/
/* Timelock (security access)                                                 */
/*----------------------------------------------------------------------------*/
#define  d_CCeventaudi_SET_____Timelock_UB        (UBYTE) ( d_CCeventaudi_Set_UB   + 0x16 )
#define  d_CCeventaudi_LOOKFOR_Timelock_UB        (UBYTE) ( d_CCeventaudi_Look_UB  + 0x17 )
#define  d_CCeventaudi_RESET___Timelock_UB        (UBYTE) ( d_CCeventaudi_Reset_UB + 0x18 )

/*----------------------------------------------------------------------------*/
/* EVENT EVALUATION                                                           */
/* ATTENTION: Must be the last entry!                                         */
/*----------------------------------------------------------------------------*/
#define  d_CCeventaudi_LOOKFOR_AllEvents_UB        (UBYTE) ( d_CCeventaudi_Look_UB  + 0x19 )


/* ========================================================================== */
/*                                                                            */
/*  E V E N T   F L A G   R E G I S T E R   D E F I N E S                     */
/*                                                                            */
/* ========================================================================== */

#define  d_CCeventaudi_INIT_EventFlags_UB                (UBYTE) 0x00u
/*----------------------------------------------------------------------------*/
/* SYSTEM RESET                                                               */
/*----------------------------------------------------------------------------*/
#define  d_CCeventaudi_SETFLAG___SystemReset_UB          (UBYTE) 0x01u
#define  d_CCeventaudi_RESETFLAG_SystemReset_UB          (UBYTE) 0xFEu
/*----------------------------------------------------------------------------*/
/* SYSTEM SHUTDOWN State 1                                                    */
/*----------------------------------------------------------------------------*/
#define  d_CCeventaudi_SETFLAG___CloseChannel_UB         (UBYTE) 0x02u
#define  d_CCeventaudi_RESETFLAG_CloseChannel_UB         (UBYTE) 0xFDu
/*----------------------------------------------------------------------------*/
/* SYSTEM SHUTDOWN State 2                                                    */
/*----------------------------------------------------------------------------*/
#define  d_CCeventaudi_SETFLAG___OpenChannel_UB          (UBYTE) 0x04u
#define  d_CCeventaudi_RESETFLAG_OpenChannel_UB          (UBYTE) 0xFBu
/*----------------------------------------------------------------------------*/
/* DefaultSession Request                                                     */  /* [OPL286] */
/*----------------------------------------------------------------------------*/
#define  d_CCeventaudi_SETFLAG___DefSessionRequ_UB       (UBYTE) 0x08u
#define  d_CCeventaudi_RESETFLAG_DefSessionRequ_UB       (UBYTE) 0xF7u
/*----------------------------------------------------------------------------*/
/* FSW is not valid flag                                                      */
/*----------------------------------------------------------------------------*/
#define  d_CCeventaudi_SETFLAG___FSWInvalid_UB           (UBYTE) 0x10u
#define  d_CCeventaudi_RESETFLAG_FSWInvalid_UB           (UBYTE) 0xEFu

/*----------------------------------------------------------------------------*/
/* Flash request received                                                     */
/*----------------------------------------------------------------------------*/
#define  d_CCeventaudi_SETFLAG___FlashRequest_UB         (UBYTE) 0x20u
#define  d_CCeventaudi_RESETFLAG_FlashRequest_UB         (UBYTE) 0xDFu

/*----------------------------------------------------------------------------*/
/* ECU reset request                                                          */
/*----------------------------------------------------------------------------*/
#define  d_CCeventaudi_SETFLAG___ECUResetRequest_UB      (UBYTE) 0x40u
#define  d_CCeventaudi_RESETFLAG_ECUResetRequest_UB      (UBYTE) 0xBFu

/*----------------------------------------------------------------------------*/
/* Timelock (security access)                                                 */
/*----------------------------------------------------------------------------*/
#define  d_CCeventaudi_SETFLAG___Timelock_UB             (UBYTE) 0x80u
#define  d_CCeventaudi_RESETFLAG_Timelock_UB             (UBYTE) 0x7Fu

/*----------------------------------------------------------------------------*/
/* LOOK FOR ALL ENTRIES                                                       */
/*----------------------------------------------------------------------------*/
#define  d_CCeventaudi_SETFLAG___AllEntries_UB           (UBYTE) 0xFFu
#define  d_CCeventaudi_RESETFLAG_AllEntries_UB           (UBYTE) 0x00u
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


/* ========================================================================== */
/*      C L O S E   M U L T I P L E   I N C L U D E   P R O T E C T I O N     */
/* ========================================================================== */
#endif
/* ========================================================================== */

/* ========================================================================== */
/*      E N D   O F   F I L E                                                 */
/* ========================================================================== */
