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
|  Filename:  ADLATUS_Global_cdf.h                                             |
|                                                                              |
|  Comment:   This file includes all global defines and constants for the      |
|             SMART ADLATUS.                                                   |
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
| $Id: ADLATUS_Global_cdf.h 4415 2008-11-11 08:35:58Z maennel $
| $HeadURL: http://smartsvn/svn/adlatusrepository/auslieferungen/S1NN/086350-S1NN-ADLATUS-TMS470R-MucSuc/20081201_S1NN_TMS470R_BlfIso-Source_V1.00/01_SW_Projekt/05_header/ext/ADLATUS_Global_cdf.h $
|
+------------------------------------------------------------------------------+
|      (c) SMART ELECTRONIC DEVELOPMENT GMBH - all rights reserved             |
+-----------------------------------------------------------------------------*/

/* ========================================================================== */
/*      O P E N   M U L T I P L E   I N C L U D E   P R O T E C T I O N       */
/* ========================================================================== */
#ifndef __CDF_GLOBAL_
#define __CDF_GLOBAL_
/* ========================================================================== */



/* ========================================================================== */
/*                                                                            */
/*  G L O B A L   S Y S T E M    D E F I N E S                                */
/*  ------------------------------------------------------------------        */
/*  Definition Area: Global.01                                                */
/*  ------------------------------------------------------------------        */
/*  Init                                                                      */
/*                                                                            */
/* ========================================================================== */
#define  d_CCglo_InitToZero        (UBYTE) 0x00u
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */



/* ========================================================================== */
/*                                                                            */
/*                                                                            */
/*  G L O B A L   S Y S T E M    D E F I N E S                                */
/*  ------------------------------------------------------------------        */
/*  Definition Area: Global.02                                                */
/*  ------------------------------------------------------------------        */
/*  Function Execution Results                                                */
/*                                                                            */
/*    Result_UB = R  B  E  I  I I I I                                         */
/*                |  |  |  |  | | | |                                         */
/*                |  |  |  +--+-+-+-+---   Response Index                     */
/*                |  |  |                  (changing Importance)              */
/*                |  |  +---------------   ERROR                              */
/*                |  +------------------   BUSY                               */
/*                +---------------------   READY                              */
/*                                                                            */
/* ========================================================================== */
#define  d_CCglo_Idle              (UBYTE) 0x00u
#define  d_CCglo_Error             (UBYTE) 0x20u
#define  d_CCglo_Busy              (UBYTE) 0x40u
#define  d_CCglo_Ready             (UBYTE) 0x80u
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define  d_CCglo_RespIndexMask     (UBYTE) 0x1Fu
#define  d_CCglo_ResultInfoMask    (UBYTE) 0xE0u
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */




/* ========================================================================== */
/*                                                                            */
/*                                                                            */
/*  G L O B A L   S Y S T E M    D E F I N E S                                */
/*  ------------------------------------------------------------------        */
/*  Definition Area: Global.03                                                */
/*  ------------------------------------------------------------------        */
/*  Buffer Index                                                              */
/*                                                                            */
/*   +---------------------------------------------------------------------+  */
/*   | Buffer Index                                                        |  */
/*   +---------+---------+---------+---------+---------+---------+---------+  */
/*   | Byte 00 | Byte 01 | Byte 02 | Byte 03 | Byte 04 |   ...   | Byte xx |  */
/*   +---------+---------+---------+---------+---------+---------+---------+  */
/*   | State   | Channel |MaxLength|  DLC    |  Data 0 |   ...   | Data N-1|  */
/*   +---------+---------+---------+---------+---------+---------+---------+  */
/*                                                                            */
/* ========================================================================== */
#define d_CCglo_BufferState         (UBYTE) 0x00u  /* Byte 00                 */
#define d_CCglo_ChannelNbr          (UBYTE) 0x01u  /* Byte 01                 */
#define d_CCglo_MaxBufferLength     (UBYTE) 0x02u  /* Byte 02                 */
#define d_CCglo_DLC                 (UBYTE) 0x03u  /* Byte 03                 */
#define d_CCglo_StartOfData         (UBYTE) 0x04u  /* Byte 04                 */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */




/* ========================================================================== */
/*                                                                            */
/*                                                                            */
/*  G L O B A L   S Y S T E M    D E F I N E S                                */
/*  ------------------------------------------------------------------        */
/*  Definition Area: Global.04                                                */
/*  ------------------------------------------------------------------        */
/*  Buffer State                                                              */
/*                                                                            */
/*  BufferState_UB = 7 6 5 4  3 2 1 0                                         */
/*                   | | | |  | | | |                                         */
/*                   | | | +--+-+-+-+---  00000 .. not used                   */
/*                   | | +--------------      0 .. Receiver buffer            */
/*                   | |                      1 .. Transmitter buffer         */
/*                   | +----------------      0 .. Buffer idle                */
/*                   |                        1 .. Buffer busy                */
/*                   +------------------      0 .. Buffer invalid             */
/*                                            1 .. Buffer valid               */
/*                                                                            */
/* ========================================================================== */
#define  d_CCglo_BufferInvalid      (UBYTE) 0x00u   /* Bit 7 = 0              */
#define  d_CCglo_BufferValid        (UBYTE) 0x80u   /* Bit 7 = 1              */

#define  d_CCglo_BufferIdle         (UBYTE) 0x00u   /* Bit 6 = 0              */
#define  d_CCglo_BufferBusy         (UBYTE) 0x40u   /* Bit 6 = 1              */

#define  d_CCglo_ReceiverBuffer     (UBYTE) 0x00u   /* Bit 5 = 0              */
#define  d_CCglo_TransmitterBuffer  (UBYTE) 0x20u   /* Bit 5 = 1              */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */



/* ========================================================================== */
/*                                                                            */
/*                                                                            */
/*  G L O B A L   S Y S T E M    D E F I N E S                                */
/*  ------------------------------------------------------------------        */
/*  Definition Area: Global.05                                                */
/*  ------------------------------------------------------------------        */
/*  Channel Information                                                       */
/*                                                                            */
/*  ChannelInfo_UB = 7 6 5 4  3 2 1 0                                         */
/*                   | | | |  | | | |                                         */
/*                   | | | |  +-+-+-+--- 0000 .. 1111 ... Channel Nbr         */
/*                   +-+-+-+------------ 0000 .. 1111 ... SubSet              */
/*                                                                            */
/* ========================================================================== */
#define  d_CCglo_ChannelMask  (UBYTE) 0x0Fu
#define  d_CCglo_SubSetMask   (UBYTE) 0xF0u
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define  d_CCglo_Channel00    (UBYTE) 0x00u /* Bit 3..0 = 0000 -> Channel 00  */
#define  d_CCglo_Channel01    (UBYTE) 0x01u /* Bit 3..0 = 0001 -> Channel 01  */
#define  d_CCglo_Channel02    (UBYTE) 0x02u /* Bit 3..0 = 0010 -> Channel 02  */
#define  d_CCglo_Channel03    (UBYTE) 0x03u /* Bit 3..0 = 0011 -> Channel 03  */
#define  d_CCglo_Channel04    (UBYTE) 0x04u /* Bit 3..0 = 0100 -> Channel 04  */
#define  d_CCglo_Channel05    (UBYTE) 0x05u /* Bit 3..0 = 0101 -> Channel 05  */
#define  d_CCglo_Channel06    (UBYTE) 0x06u /* Bit 3..0 = 0110 -> Channel 06  */
#define  d_CCglo_Channel07    (UBYTE) 0x07u /* Bit 3..0 = 0111 -> Channel 07  */
#define  d_CCglo_Channel08    (UBYTE) 0x08u /* Bit 3..0 = 1000 -> Channel 08  */
#define  d_CCglo_Channel09    (UBYTE) 0x09u /* Bit 3..0 = 1001 -> Channel 09  */
#define  d_CCglo_Channel10    (UBYTE) 0x0Au /* Bit 3..0 = 1010 -> Channel 10  */
#define  d_CCglo_Channel11    (UBYTE) 0x0Bu /* Bit 3..0 = 1011 -> Channel 11  */
#define  d_CCglo_Channel12    (UBYTE) 0x0Cu /* Bit 3..0 = 1100 -> Channel 12  */
#define  d_CCglo_Channel13    (UBYTE) 0x0Du /* Bit 3..0 = 1101 -> Channel 13  */
#define  d_CCglo_Channel14    (UBYTE) 0x0Eu /* Bit 3..0 = 1110 -> Channel 14  */
#define  d_CCglo_AllChannels  (UBYTE) 0x0Fu /* Bit 3..0 = 1111 -> All Channels */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define  d_CCglo_SubSet00     (UBYTE) 0x00u /* Bit 7..4 = 0000 -> SubSet 00   */
#define  d_CCglo_SubSet01     (UBYTE) 0x10u /* Bit 7..4 = 0001 -> SubSet 01   */
#define  d_CCglo_SubSet02     (UBYTE) 0x20u /* Bit 7..4 = 0010 -> SubSet 02   */
#define  d_CCglo_SubSet03     (UBYTE) 0x30u /* Bit 7..4 = 0011 -> SubSet 03   */
#define  d_CCglo_SubSet04     (UBYTE) 0x40u /* Bit 7..4 = 0100 -> SubSet 04   */
#define  d_CCglo_SubSet05     (UBYTE) 0x50u /* Bit 7..4 = 0101 -> SubSet 05   */
#define  d_CCglo_SubSet06     (UBYTE) 0x60u /* Bit 7..4 = 0110 -> SubSet 06   */
#define  d_CCglo_SubSet07     (UBYTE) 0x70u /* Bit 7..4 = 0111 -> SubSet 07   */
#define  d_CCglo_SubSet08     (UBYTE) 0x80u /* Bit 7..4 = 1000 -> SubSet 08   */
#define  d_CCglo_SubSet09     (UBYTE) 0x90u /* Bit 7..4 = 1001 -> SubSet 09   */
#define  d_CCglo_SubSet10     (UBYTE) 0xA0u /* Bit 7..4 = 1010 -> SubSet 10   */
#define  d_CCglo_SubSet11     (UBYTE) 0xB0u /* Bit 7..4 = 1011 -> SubSet 11   */
#define  d_CCglo_SubSet12     (UBYTE) 0xC0u /* Bit 7..4 = 1100 -> SubSet 12   */
#define  d_CCglo_SubSet13     (UBYTE) 0xD0u /* Bit 7..4 = 1101 -> SubSet 13   */
#define  d_CCglo_SubSet14     (UBYTE) 0xE0u /* Bit 7..4 = 1110 -> SubSet 14   */
#define  d_CCglo_SubSet15     (UBYTE) 0xF0u /* Bit 7..4 = 1111 -> SubSet 15   */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#define d_CCglo_AddIdentifier (UBYTE) 0x04u
#define d_CCglo_FixedMsgLgnth (UBYTE) 0x10u
#define d_CCglo_MbSendFromRam (UBYTE) 0x40u
/* ========================================================================== */
/*                                                                            */
/*  G L O B A L   S Y S T E M    D E F I N E S                                */
/*  ------------------------------------------------------------------        */
/*  Definition Area: Global.06                                                */
/*  ------------------------------------------------------------------        */
/*  Length Information                                                        */
/*                                                                            */
/* ========================================================================== */
#define  d_CCglo_00Byte       (UBYTE) 0x00u
#define  d_CCglo_01Byte       (UBYTE) 0x01u
#define  d_CCglo_02Byte       (UBYTE) 0x02u
#define  d_CCglo_03Byte       (UBYTE) 0x03u
#define  d_CCglo_04Byte       (UBYTE) 0x04u
#define  d_CCglo_05Byte       (UBYTE) 0x05u
#define  d_CCglo_06Byte       (UBYTE) 0x06u
#define  d_CCglo_07Byte       (UBYTE) 0x07u
#define  d_CCglo_08Byte       (UBYTE) 0x08u
#define  d_CCglo_09Byte       (UBYTE) 0x09u
#define  d_CCglo_10Byte       (UBYTE) 0x0Au
#define  d_CCglo_11Byte       (UBYTE) 0x0Bu
#define  d_CCglo_12Byte       (UBYTE) 0x0Cu
#define  d_CCglo_13Byte       (UBYTE) 0x0Du
#define  d_CCglo_14Byte       (UBYTE) 0x0Eu
#define  d_CCglo_15Byte       (UBYTE) 0x0Fu
#define  d_CCglo_16Byte       (UBYTE) 0x10u
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */



/* ========================================================================== */
/*                                                                            */
/*  G L O B A L   S Y S T E M    D E F I N E S                                */
/*  ------------------------------------------------------------------        */
/*  Definition Area: Global.07                                                */
/*  ------------------------------------------------------------------        */
/* Session Information                                                        */
/*                                                                            */
/* ========================================================================== */
#define  d_CCglo_Session00           (UBYTE) 0x00u
#define  d_CCglo_Session01           (UBYTE) 0x01u
#define  d_CCglo_Session02           (UBYTE) 0x02u
#define  d_CCglo_Session03           (UBYTE) 0x03u
#define  d_CCglo_Session04           (UBYTE) 0x04u
#define  d_CCglo_Session05           (UBYTE) 0x05u
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define d_CCglo_AllowedSessionMask   (UBYTE) 0x0Fu
#define d_CCglo_AllowedInAllSession  (UBYTE) 0x00u
#define d_CCglo_AllowedInStdSession  (UBYTE) 0x01u
#define d_CCglo_AllowedInDefSession  (UBYTE) 0x01u

#define d_CCglo_AllowedInExtSession  (UBYTE) 0x02u
#define d_CCglo_AllowedInProgSession (UBYTE) 0x04u
#define d_CCglo_SecurityAccessRequired (UBYTE) 0x08u

#define d_CCglo_AddressAllowedMask   (UBYTE) 0xF0u
#define d_CCglo_AllAddressAllowed    (UBYTE) 0x00u
#define d_CCglo_FuncAddressAllowed   (UBYTE) 0x10u
#define d_CCglo_PhyAddressAllowed    (UBYTE) 0x20u


/* ========================================================================== */
/*                                                                            */
/*  G L O B A L   S Y S T E M    D E F I N E S                                */
/*  ------------------------------------------------------------------        */
/*  Definition Area: Global.08                                                */
/*  ------------------------------------------------------------------        */
/*  Position Information                                                      */
/*                                                                            */
/* ========================================================================== */
#define  d_CCglo_Byte00             (UBYTE)  0x00u
#define  d_CCglo_Byte01             (UBYTE)  0x01u
#define  d_CCglo_Byte02             (UBYTE)  0x02u
#define  d_CCglo_Byte03             (UBYTE)  0x03u
#define  d_CCglo_Byte04             (UBYTE)  0x04u
#define  d_CCglo_Byte05             (UBYTE)  0x05u
#define  d_CCglo_Byte06             (UBYTE)  0x06u
#define  d_CCglo_Byte07             (UBYTE)  0x07u
#define  d_CCglo_Byte08             (UBYTE)  0x08u
#define  d_CCglo_Byte09             (UBYTE)  0x09u
#define  d_CCglo_Byte10             (UBYTE)  0x0Au
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */



/* ========================================================================== */
/*                                                                            */
/*  G L O B A L   S Y S T E M    D E F I N E S                                */
/*  ------------------------------------------------------------------        */
/*  Definition Area: Global.09                                                */
/*  ------------------------------------------------------------------        */
/* Response Index                                                             */
/*                                                                            */
/* ========================================================================== */
#define  d_CCglo_ResponseIndex00    (UBYTE)  0x00u
#define  d_CCglo_ResponseIndex01    (UBYTE)  0x01u
#define  d_CCglo_ResponseIndex02    (UBYTE)  0x02u
#define  d_CCglo_ResponseIndex03    (UBYTE)  0x03u
#define  d_CCglo_ResponseIndex04    (UBYTE)  0x04u
#define  d_CCglo_ResponseIndex05    (UBYTE)  0x05u
#define  d_CCglo_ResponseIndex06    (UBYTE)  0x06u
#define  d_CCglo_ResponseIndex07    (UBYTE)  0x07u
#define  d_CCglo_ResponseIndex08    (UBYTE)  0x08u
#define  d_CCglo_ResponseIndex09    (UBYTE)  0x09u
#define  d_CCglo_ResponseIndex10    (UBYTE)  0x0Au
#define  d_CCglo_ResponseIndex11    (UBYTE)  0x0Bu
#define  d_CCglo_ResponseIndex12    (UBYTE)  0x0Cu
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */



/* ========================================================================== */
/*                                                                            */
/*  G L O B A L   S Y S T E M    D E F I N E S                                */
/*  ------------------------------------------------------------------        */
/*  Definition Area: Global.10                                                */
/*  ------------------------------------------------------------------        */
/* Timer Information                                                          */
/*                                                                            */
/* ========================================================================== */
#define  d_CCglo_TimerNotElapsed    (UBYTE)  0x00u
#define  d_CCglo_TimerElapsed       (UBYTE)  0x01u
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


/* ========================================================================== */
/*                                                                            */
/*  G L O B A L   S Y S T E M    D E F I N E S                                */
/*  ------------------------------------------------------------------        */
/*  Definition Area: Global.11                                                */
/*  ------------------------------------------------------------------        */
/*  - Sequence Table    at ADLATUS_SequenceXXX.c                              */
/*  - ResponseCodeTable at ADLATUS_KWP2000XXX.c                               */
/*                                                                            */
/* ========================================================================== */

/*----------------------------------------------------------------------------*/
/* Global Configuration Defines                                               */
/*----------------------------------------------------------------------------*/
#define d_CCglo_NoEntry            (UBYTE)  0xFFu
#define d_CCglo_No                 (UBYTE)  0x00u
#define d_CCglo_Yes                (UBYTE)  0x01u
#define d_CCglo_Reserved           (UBYTE)  0xFFu
/*----------------------------------------------------------------------------*/
/* Especially Configuration Defines for                                       */
/* - Sequence.c                                                               */
/* - Session.c                                                                */
/*----------------------------------------------------------------------------*/
#define  d_CCglo_DontCheckLength   (UBYTE)  0x00u
#define  d_CCglo_NoPostCondDefined (UBYTE)  0x80u
#define  d_CCglo_NegativeResponse  (UBYTE)  0x40u

/*----------------------------------------------------------------------------*/
/* Sequence Initialisation                                                    */
/*----------------------------------------------------------------------------*/
#define d_CCglo_ResetChannel       (UBYTE)  0x00u
#define d_CCglo_FlashRequest       (UBYTE)  0x01u
#define d_CCglo_ResetRequest       (UBYTE)  0x02u


/* ========================================================================== */
/*                                                                            */
/*  G L O B A L   S Y S T E M    D E F I N E S                                */
/*  ------------------------------------------------------------------        */
/*  Definition Area: Global.12                                                */
/*  ------------------------------------------------------------------        */
/*  Validation Map Defines                                                    */
/*                                                                            */
/* CheckState = x x x x  x x x x  x x x x  x x x x                            */
/*              | | | |  | | | |  | | | |  | +-+-+-  QuickCheck length:       */
/*              | | | |  | | | |  | | | |  |         001 .. 1 Byte            */
/*              | | | |  | | | |  | | | |  |         010 .. 2 Byte            */
/*              | | | |  | | | |  | | | |  |         011 .. 3 Byte            */
/*              | | | |  | | | |  | | | |  |         100 .. 4 Byte            */
/*              | | | |  | | | |  | | | |  +-------  Quick Check Condition:   */
/*              | | | |  | | | |  | | | |              0 .. Dont do Quickcheck*/
/*              | | | |  | | | |  | | | |              1 .. Dont do Quickcheck*/
/*              | | | |  | | | |  | +-+-+----------  Not used                 */
/*              | | | |  | | | |  +----------------  Quick check              */
/*              | | | |  | | | |                       0 .. Only one Checksum */
/*              | | | |  | | | |                       0 .. double checksum   */
/*              | | | |  | | | +-------------------  ADLATUS Area entry       */
/*              | | | |  | | |                         0 .. ASW entry         */
/*              | | | |  | | |                         1 .. ADLATUS           */
/*              | | | |  | +-+---------------------  Error reaktion:          */
/*              | | | |  |                            00 .. Stay within ADLATUS */
/*              | | | |  |                            01 .. Stay in endless loop*/
/*              | | | |  |                            10 .. Jump to FSW       */
/*              | | | |  |                            11 .. not used          */
/*              | | | |  +-------------------------  Ram Check Condition:     */
/*              | | | |                                0 .. Don't init RAM    */
/*              | | | |                                1 .. Init RAM          */
/*              | | +-+----------------------------  Memory Type:             */
/*              | |                                   00 .. Flash             */
/*              | |                                   01 .. RAM               */
/*              | |                                   10 .. Can Msg Obj       */
/*              | |                                   11 .. not used          */
/*              | +--------------------------------  Check Condition:         */
/*              |                                      0 .. Check disabled    */
/*              |                                      1 .. Check enabled     */
/*              +----------------------------------  Entry State              */
/*                                                     0 .. Normal entry      */
/*                                                     1 .. Last entry        */
/*                                                                            */
/*      ----------------------------------------------------------------      */
/*      - MemoryMap at ConfigProject.c                                        */
/*                  .. Validation.c                                           */
/*                                                                            */
/* ========================================================================== */
#define d_CCglo_NotUsed            (UWORD)  0x0000u    /* 0000 0000 0000 0000 */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#define d_CCglo_NormalEntry        (UWORD)  0x0000u    /* 0000 0000 0000 0000 */
#define d_CCglo_LastEntry          (UWORD)  0x8000u    /* 1000 0000 0000 0000 */
#define d_CCglo_EntryMask          (UWORD)  0x8000u    /* 1000 0000 0000 0000 */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#define d_CCglo_CheckDisabled      (UWORD)  0x0000u    /* 0000 0000 0000 0000 */
#define d_CCglo_CheckEnabled       (UWORD)  0x4000u    /* 0100 0000 0000 0000 */
#define d_CCglo_MemoryCheckMask    (UWORD)  0x4000u    /* 0100 0000 0000 0000 */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Defines for Memory Type                                                    */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#define d_CCglo_FlashMemory        (UWORD)  0x0000u    /* 0000 0000 0000 0000 */
#define d_CCglo_RamMemory          (UWORD)  0x1000u    /* 0001 0000 0000 0000 */
#define d_CCglo_CanMsgObject       (UWORD)  0x2000u    /* 0010 0000 0000 0000 */
#define d_CCglo_EepromMemory       (UWORD)  0x3000u    /* 0011 0000 0000 0000 */
#define d_CCglo_Parrot             (UWORD)  0x0800u    /* 0000 1000 0000 0000 */
#define d_CCglo_Parrot_Boot        (UWORD)  0x1800u    /* 0001 1000 0000 0000 */
#define d_CCglo_Parrot_Upd         (UWORD)  0x2800u    /* 0010 1000 0000 0000 */
#define d_CCglo_MemoryTypeMask     (UWORD)  0x3800u    /* 0011 1000 0000 0000 */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Defines for RAM Memory Check                                               */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#define d_CCglo_DontInitRam        (UWORD)  0x0000u    /* 0000 0000 0000 0000 */
#define d_CCglo_InitRam            (UWORD)  0x0400u    /* 0000 0100 0000 0000 */
#define d_CCglo_RamConditionMask   (UWORD)  0x0400u    /* 0000 0100 0000 0000 */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Defines for ADLATUS Area entry                                             */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#define d_CCglo_AswArea            (UWORD)  0x0000u    /* 0000 0000 0000 0000 */
#define d_CCglo_AdlatusArea        (UWORD)  0x0100u    /* 0000 0001 0000 0000 */
#define d_CCglo_AdlatusAreaMask    (UWORD)  0x0100u    /* 0000 0001 0000 0000 */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Defines for quick check                                                    */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#define d_CCglo_OneCheckSumMethod  (UWORD)  0x0000u
#define d_CCglo_AlternateMethod    (UWORD)  0x0080u
#define d_CCglo_ChecksumMask       (UWORD)  0x0080u
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Defines for quick check data type                                          */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#define d_CCglo_OneCheckSumMethod2 (UWORD)  0x0000u
#define d_CCglo_AlternateMethod2   (UWORD)  0x0040u
#define d_CCglo_ChecksumMask2      (UWORD)  0x0040u

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Defines for Flash Memory Check                                             */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#define d_CCglo_DontDoQuickCheck   (UWORD)  0x0000u    /* 0000 0000 0000 0000 */
#define d_CCglo_DoQuickCheck       (UWORD)  0x0008u    /* 0000 0000 0000 1000 */
#define d_CCglo_QuickCheckMask     (UWORD)  0x0008u    /* 0000 0000 0000 1000 */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* NOTE: Length information is defined above (e.g. "d_CCglo_02Byte"           */
#define d_CCglo_QuickCheckLengthMask (UWORD) 0x0007u    /* 0000 0000 0000 0111*/
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* ========================================================================== */
/*                                                                            */
/*  G L O B A L   S Y S T E M    D E F I N E S                                */
/*  ------------------------------------------------------------------        */
/*  Definition Area: Global.13                                                */
/*  ------------------------------------------------------------------        */
/*  Memory Map Defines                                                        */
/*                                                                            */
/*  Memory Info = x x x x  x x x x  x x x x  x x x x                          */
/*                | | | |  | | | |  | | | |  | | | |                          */
/*                | | | |  | | | |  +-+-+-+--+-+-+-+-  Error Code             */
/*                | | | |  | | | +-------------------  Address Check Result:  */
/*                | | | |  | | |                         0 .. Check NOK       */
/*                | | | |  | | |                         1 .. Check OK        */
/*                | | | |  | | +---------------------  Programming state:     */
/*                | | | |  | |                           0 .. Not Programable */
/*                | | | |  | |                           1 .. Programmable    */
/*                | | | |  | |                        					      */
/*                | | | |  | +-----------------------  Erase state:           */
/*                | | | |  |                             0 .. Not Eraseable   */
/*                | | | |  |                             1 .. Eraseable       */
/*                | | +-+--+-------------------------  Memory Type:           */
/*                | |                                   000 .. Flash          */
/*                | |                                   010 .. RAM            */
/*                | |                                   100 .. CanMsgObj      */
/*                | |                                   110 .. EEPROM         */
/*                | |                                   001 .. Parrot Modul   */
/*                | +--------------------------------  Check state:           */
/*                |                                      0 .. Range check     */
/*                |                                      1 .. Boundary check  */
/*                +----------------------------------  Entry State            */
/*                                                       0 .. Normal entry    */
/*                                                       1 .. Last entry      */
/*                                                                            */
/*      ----------------------------------------------------------------      */
/*      - MemoryMap at ConfigProject.c                                        */
/*                  .. KWP2000Customer.c                                      */
/*                                                                            */
/* ========================================================================== */
/* -->  d_CCglo_NotUsed                     0x0000       0000 0000 0000 0000  */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* -->  d_CCglo_NormalEntry                 0x0000       0000 0000 0000 0000  */
/* -->  d_CCglo_LastEntry                   0x8000       1000 0000 0000 0000  */
/* -->  d_CCglo_EntryMask                   0x8000       1000 0000 0000 0000  */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Defines for Check State                                                    */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#define d_CCglo_CheckRange         (UWORD)  0x0000u   /* 0000 0000 0000 0000  */
#define d_CCglo_CheckBoundary      (UWORD)  0x4000u   /* 0100 0000 0000 0000  */
#define d_CCglo_CheckStateMask     (UWORD)  0x4000u   /* 0100 0000 0000 0000  */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Defines for Memory Type                                                    */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* -->  #define d_CCglo_FlashMemory        (UWORD)  0x0000u    *//* 0000 0000 0000 0000  */
/* -->  #define d_CCglo_RamMemory          (UWORD)  0x1000u    *//* 0001 0000 0000 0000  */
/* -->  #define d_CCglo_CanMsgObject       (UWORD)  0x2000u    *//* 0010 0000 0000 0000  */
/* -->  #define d_CCglo_EepromMemory       (UWORD)  0x3000u    *//* 0011 0000 0000 0000  */
/* -->  #define d_CCglo_Parrot             (UWORD)  0x0800u    *//* 0000 1000 0000 0000  */
/* -->  #define d_CCglo_Parrot_Boot        (UWORD)  0x1800u    *//* 0000 1000 0000 0000  */
/* -->  #define d_CCglo_MemoryTypeMask     (UWORD)  0x3800u    *//* 0011 1000 0000 0000  */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Defines for Erase State                                                    */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#define d_CCglo_NotErasable        (UWORD)  0x0000u   /* 0000 0000 0000 0000  */
#define d_CCglo_Erasable           (UWORD)  0x0400u   /* 0000 0100 0000 0000  */
#define d_CCglo_EraseStateMask     (UWORD)  0x0400u   /* 0000 0100 0000 0000  */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Defines for Programm State                                                 */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#define d_CCglo_NotProgramable     (UWORD)  0x0000u   /* 0000 0000 0000 0000  */
#define d_CCglo_Programable        (UWORD)  0x0200u   /* 0000 0010 0000 0000  */
#define d_CCglo_ProgramStateMask   (UWORD)  0x0200u   /* 0000 0010 0000 0000  */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Defines for Address Check Result */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#define d_CCglo_AddressCheckNOK    (UWORD)  0x0000u   /* 0000 0000 0000 0000  */
#define d_CCglo_AddressCheckOK     (UWORD)  0x0100u   /* 0000 0001 0000 0000  */
#define d_CCglo_AddressCheckMask   (UWORD)  0x0100u   /* 0000 0001 0000 0000  */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Defines for Error Code */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#define d_CCglo_StartGreaterThanEndCheckNOK  (UWORD)  0x0001u   /* 0000 0000 0000 0001  */
#define d_CCglo_StartToLessCheckNOK          (UWORD)  0x0002u   /* 0000 0000 0000 0010  */
#define d_CCglo_StartToBigCheckNOK           (UWORD)  0x0004u   /* 0000 0000 0000 0100  */
#define d_CCglo_EndToLessCheckNOK            (UWORD)  0x0008u   /* 0000 0000 0000 1000  */
#define d_CCglo_EndToBigCheckNOK             (UWORD)  0x0010u   /* 0000 0000 0001 0000  */
#define d_CCglo_EraseNotAllowedCheckNOK      (UWORD)  0x0020u   /* 0000 0000 0010 0000  */
#define d_CCglo_ProgramingNotAllowedCheckNOK (UWORD)  0x0040u   /* 0000 0000 0100 0000  */


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#define d_CCglo_Download           (UBYTE)  0x01u
#define d_CCglo_Erase              (UBYTE)  0x00u
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/




/* ========================================================================== */
/*                                                                            */
/*  G L O B A L   S Y S T E M    D E F I N E S                                */
/*  ------------------------------------------------------------------        */
/*  Definition Area: Global.14                                                */
/*  ------------------------------------------------------------------        */
/*  Error Information                                                         */
/*                                                                            */
/* ========================================================================== */
#define d_CCglo_NoError                (UBYTE) 0x00u
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* System Startup errors                                                      */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#define d_CCglo_RamCheckError          (UBYTE) 0x01u
#define d_CCglo_NoRamCheckError        d_CCglo_NoError
#define d_CCglo_AdlatusChecksumError   (UBYTE) 0x02u
#define d_CCglo_NoAdlatusChecksumError d_CCglo_NoError
#define d_CCglo_NoFswInSystem          (UBYTE) 0x04u
#define d_CCglo_FswInSystem            d_CCglo_NoError
#define d_CCglo_FSWChecksumError       (UBYTE) 0x08u
#define d_CCglo_NoFSWChecksumError     d_CCglo_NoError
#define d_CCglo_FSWFlashRequest        (UBYTE) 0x10u
#define d_CCglo_NoFSWFlashRequest      d_CCglo_NoError
#define d_CCglo_FSWEcuResetRequest     (UBYTE) 0x20u
#define d_CCglo_NoFSWEcuResetRequest   d_CCglo_NoError
#define d_CCglo_FSWDefSessionRequest   (UBYTE) 0x40u  /* [OPL286] */
#define d_CCglo_NoFSWDefSessionRequest d_CCglo_NoError
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Return values of validation routines                                       */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#define d_CCglo_ChecksumError          (UBYTE) 0xFFu
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Return values of communication stack routines                              */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#define d_CCglo_FatalSystemError       (UBYTE) 0x0Fu
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Return values of download address check                                    */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#define d_CCglo_OutOfRange             0x0000u
#define d_CCglo_EndaddressIncorrect    0x0001u
#define d_CCglo_ProtectedEraseArea     0x0002u
#define d_CCglo_ProtectedDownloadArea  0x0003u

/* ========================================================================== */
/*                                                                            */
/*  G L O B A L   S Y S T E M    D E F I N E S                                */
/*  ------------------------------------------------------------------        */
/*  Definition Area: Global.15                                                */
/*  ------------------------------------------------------------------        */
/*  Watchdog state register                                                   */
/*                                                                            */
/*  Watchdog State_UB  = 7 6 5 4  3 2 1 0                                     */
/*                       | | | |  | | | |                                     */
/*                       | | | |  | | | +---  enable watchdog                 */
/*                       | | | |  | | +-----  disable watchdog                */
/*                       | | | |  | +-------  trigger watchdog                */
/*                       | | | |  +---------  not used                        */
/*                       | | | |                                              */
/*                       | | | +------------  not used                        */
/*                       | | +--------------  not used                        */
/*                       | +----------------  not used                        */
/*                       +------------------  watchdog enabled                */
/*                                                                            */
/* ========================================================================== */
#define d_CCglo_InitExternalWatchdogState         (UBYTE) 0x00u
#define d_CCglo_FlagEnableExternalWatchdog        (UBYTE) 0x01u
#define d_CCglo_FlagDisableExternalWatchdog       (UBYTE) 0x02u
#define d_CCglo_FlagTriggerExternalWatchdog       (UBYTE) 0x04u
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#define d_CCglo_StateExternalWatchdogEnabled      (UBYTE) 0x80u
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#define d_CCglo_ExternalWatchdogFlagMask          (UBYTE) 0x07u
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/



/* ========================================================================== */
/*                                                                            */
/*  G L O B A L   S Y S T E M    D E F I N E S                                */
/*  ------------------------------------------------------------------        */
/*  Definition Area: Global.16                                                */
/*  ------------------------------------------------------------------        */
/*  NVM Access                                                                */
/*                                                                            */
/* ========================================================================== */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Storrage Types                                                             */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#define d_CCglo_SingleBuffering      (UBYTE)  0x01u
#define d_CCglo_DoubleBuffering      (UBYTE)  0x02u
#define d_CCglo_TribbleBuffering     (UBYTE)  0x03u
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#define d_CCglo_WithCheckSum         (UBYTE)  0x80u
#define d_CCglo_WithoutCheckSum      (UBYTE)  0x00u



/* ========================================================================== */
/*                                                                            */
/*  G L O B A L   S Y S T E M    D E F I N E S                                */
/*  ------------------------------------------------------------------        */
/*  Definition Area: Global.17                                                */
/*  ------------------------------------------------------------------        */
/*  NVM State register                                                        */
/*                                                                            */
/*  NVM State_UB  = 7 6 5 4  3 2 1 0                                          */
/*                  | | | |  | | | |                                          */
/*                  | | | |  | | | +---  not used                             */
/*                  | | | |  | | +-----  not used                             */
/*                  | | | |  | +-------  not used                             */
/*                  | | | |  +---------  not used                             */
/*                  | | | |                                                   */
/*                  | | | +------------  not used                             */
/*                  | | +--------------  ERROR                                */
/*                  | +----------------  BUSY                                 */
/*                  +------------------  READY                                */
/*                                                                            */
/* ========================================================================== */
#define d_CCglo_InitNvmState          (UBYTE)    0x00u
#define d_CCglo_NvmIdle               (UBYTE)    0x00u
#define d_CCglo_NVMReady              (UBYTE)    0x08u
#define d_CCglo_NVMBusy               (UBYTE)    0x04u
#define d_CCglo_NVMError              (UBYTE)    0x02u
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#define d_CCglo_NVMStateMask          (UBYTE)    0xE0u
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#define d_CCglo_NvmTrigger            (UBYTE)    0x01u
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* ========================================================================== */
/*                                                                            */
/*  G L O B A L   S Y S T E M    D E F I N E S                                */
/*  ------------------------------------------------------------------        */
/*  Definition Area: Global.18                                                */
/*  ------------------------------------------------------------------        */
/*  General access handles                                                    */
/*                                                                            */
/* ========================================================================== */
#define d_CCglo_Read                  (UBYTE)   0x00u
#define d_CCglo_Write                 (UBYTE)   0x80u

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* ========================================================================== */
/*                                                                            */
/*  G L O B A L   S Y S T E M    D E F I N E S                                */
/*  ------------------------------------------------------------------        */
/*  Definition Area: Global.19                                                */
/*  ------------------------------------------------------------------        */
/*  subfunction mask for 'suppress positive response message indication'      */
/*                                                                            */
/* ========================================================================== */
#define d_CCglo_LIDSubFunctionMask    (UBYTE)   0x7Fu
#define d_CCglo_LIDSuppressPosRspMask (UBYTE)   0x80u
#define d_CCglo_LIDSuppressPosRsp     (UBYTE)   0x80u
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* ========================================================================== */
/*                                                                            */
/*  G L O B A L   S Y S T E M    D E F I N E S                                */
/*  ------------------------------------------------------------------        */
/*  Definition Area: Global.20                                               */
/*  ------------------------------------------------------------------        */
/*  Reset the System either by a "software reset" or by a watchdog timeout    */
/*                                                                            */
/* ========================================================================== */
#define d_CCglo_ResetViaWdt           (UBYTE)   0x5Au
#define d_CCglo_NoResetViaWdt         (UBYTE)   0x00u
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* ========================================================================== */
/*      C L O S E   M U L T I P L E   I N C L U D E   P R O T E C T I O N     */
/* ========================================================================== */
#endif
/* ========================================================================== */


/* ========================================================================== */
/*      E N D   O F   F I L E                                                 */
/* ========================================================================== */
