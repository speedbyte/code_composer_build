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
|  Filename:  ADLATUS_CanHandler_cdf.h                                         |
|                                                                              |
|  Comment:   This file includes all defines of the                            |
|             CAN handler for the SMART ADLATUS.                               |
|                                                                              |
+------------------------------------------------------------------------------+
| R E V I S I O N   H I S T O R Y                                              |
+------------------------------------------------------------------------------+
|
| Subversion Info:
| $Id: ADLATUS_CanHandler_cdf.h 4396 2008-11-04 09:26:32Z maennel $
| $HeadURL: http://smartsvn/svn/adlatusrepository/auslieferungen/S1NN/086350-S1NN-ADLATUS-TMS470R-MucSuc/20081201_S1NN_TMS470R_BlfIso-Source_V1.00/01_SW_Projekt/05_header/ext/ADLATUS_CanHandler_cdf.h $
|
+------------------------------------------------------------------------------+
|      (c) SMART ELECTRONIC DEVELOPMENT GMBH - all rights reserved             |
+-----------------------------------------------------------------------------*/

/* ========================================================================== */
/*      O P E N   M U L T I P L E   I N C L U D E   P R O T E C T I O N       */
/* ========================================================================== */
#ifndef __CDF_CANHANDLER_
#define __CDF_CANHANDLER_
/* ========================================================================== */

/* ========================================================================== */
/*                                                                            */
/*      C A N   D E F I N E S                                                 */
/*                                                                            */
/* ========================================================================== */

#define  d_CCcan_NbrOfLogChannel         d_CCconsys_NbrOfLogChannel
/*----------------------------------------------------------------------------*/
/* Can Controller Selection Information                                       */
/*----------------------------------------------------------------------------*/
#define d_CCcan_NodeA          (UWORD) 0x00u
#define d_CCcan_NodeB          (UWORD) 0x01u

/*----------------------------------------------------------------------------*/
/* Config Information                                                         */
/*----------------------------------------------------------------------------*/
#define d_CCcan_Deactivate     (UBYTE) 0x00u
#define d_CCcan_Activate       (UBYTE) 0x01u

#define d_CCcan_Receive        (UBYTE) 0x00u
#define d_CCcan_Transmit       (UBYTE) 0x01u

#define d_CCcan_11BitID        (UBYTE) 0x00u
#define d_CCcan_29BitID        (UBYTE) 0x01u

#ifdef cs_CChal_variable_CANID  
 #define d_CCcan_11BitID_Mask   (UBYTE) 0x0Fu

 #define d_CCcan_AdditionalID   (UBYTE) 0x80u
 #define d_CCcan_AdditionalID_Mask   (UBYTE) 0xF0u
#endif

/*============================================================================*/
/* Baudrate defines                                                           */
/*============================================================================*/
#define  d_CCcan_1000000Bd      (UBYTE)   0x00u
#define  d_CCcan_500000Bd       (UBYTE)   0x01u
#define  d_CCcan_333333Bd       (UBYTE)   0x02u
#define  d_CCcan_250000Bd       (UBYTE)   0x03u
#define  d_CCcan_200000Bd       (UBYTE)   0x04u
#define  d_CCcan_125000Bd       (UBYTE)   0x07u
#define  d_CCcan_100000Bd       (UBYTE)   0x09u
#define  d_CCcan_833333Bd       (UBYTE)   0x0Au

/* -------------------------------------------------------------------------- */
/* CAN Functions - Error Result Index                                         */
/* -------------------------------------------------------------------------- */
#define  d_CCcan_BufferNotInvalid     (UBYTE) 0x01u
#define  d_CCcan_MsgObjInvalid        (UBYTE) 0x02u
#define  d_CCcan_MessageLostError     (UBYTE) 0x03u
#define  d_CCcan_NoConfigTableEntry   (UBYTE) 0x04u
#define  d_CCcan_TxNotReadyError      (UBYTE) 0x05u
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

/* -------------------------------------------------------------------------- */
/* Define for internal buffer handling                                        */
/* -------------------------------------------------------------------------- */
#define  d_CCcan_NbrOfInternalCanBuffer  (UBYTE) 0x04u
#define  d_CCcan_ReadIndexMask           (UBYTE) 0x03u
#define  d_CCcan_WriteIndexMask          (UBYTE) 0x03u
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

/* -------------------------------------------------------------------------- */
/*      C A N    S T A T E    D E F I N E S                                   */
/*                                                                            */
/*      BufferState_UB = 7 6 5 4  3 2 1 0                                     */
/*                       | | | |  | | | |                                     */
/*                       | | | |  | | | +---  Stop Scan                       */
/*                       | | | |  | | +-----  Message Received                */
/*                       | | | |  | +-------  Additional ID defined           */
/*                       | | | |  +---------  Correct Table entry found       */
/*                       +-+-+-+------------  not used                        */
/*                                                                            */
/* -------------------------------------------------------------------------- */
#define  d_CCcan_InitState            (UBYTE)    0x00u
#define  d_CCcan_StopScan             (UBYTE)    0x01u
#define  d_CCcan_MsgReceived          (UBYTE)    0x02u
#define  d_CCcan_AddIdentifier                   d_CCglo_AddIdentifier
#define  d_CCcan_CorrectMsgEntryFound (UBYTE)    0x08u
#define  d_CCcan_FixedMsgLgnth                   d_CCglo_FixedMsgLgnth
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#define d_CCcan_NoMbforRamDefined      (UBYTE)    0xFFu


/*-------------------------------------------------------------------------------*/
/* NODE A STATUS REGISTER (ASR)                                                  */
/*----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----*/
/* 15 | 14 | 13 | 12 | 11 | 10 |  9 |  8 |  7 |  6 |  5 |  4 |  3 |  2 |  1 |  0 */
/*----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----*/
/*                 0x00                  | B  | E  | 0  | RX | TX |    LEC       */
/*                                       | OFF| WRN|    | OK | OK |              */
/*----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----*/
#define  d_CCcan_RxOk                             0x0010u
#define  d_CCcan_TxOK                             0x0008u


#define d_CCcan_NoMbforRamDefined      (UBYTE)    0xFFu
/* ========================================================================== */
/*      C L O S E   M U L T I P L E   I N C L U D E   P R O T E C T I O N     */
/* ========================================================================== */
#endif
/* ========================================================================== */


/* ========================================================================== */
/*      E N D   O F   F I L E                                                 */
/* ========================================================================== */
