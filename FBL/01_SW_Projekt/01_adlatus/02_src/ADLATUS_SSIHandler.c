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
|  Filename:  ADLATUS_SSIHandler.c                                             |
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
| $Id: ADLATUS_SSIHandler.c 4396 2008-11-04 09:26:32Z maennel $
| $HeadURL: http://smartsvn/svn/adlatusrepository/auslieferungen/S1NN/086350-S1NN-ADLATUS-TMS470R-MucSuc/20081201_S1NN_TMS470R_BlfIso-Source_V1.00/01_SW_Projekt/01_adlatus/02_src/ADLATUS_SSIHandler.c $
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
/* (1)  I N C L U D E S                                                       */
/* -------------------------------------------------------------------------- */
/*      T Y P E   D E F I N I T I O N   F I L E S  (TDF)                      */
/* ========================================================================== */
#include "ADLATUS_Types_tdf.h"                 /* must be the first include   */
#include "ADLATUS_SSIHandler_tdf.h"

#include  "ADLATUS_Eep.h"

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
 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* ========================================================================== */
/* (6)  L O C A L  T Y P E   D E F I N E S                                    */
/* -------------------------------------------------------------------------- */
/*      F U N C T I O N   E X E C U T I O N   R E S U L T S  D E F I N E S    */
/*                                                                            */
/*      Result_UB = R  B  E  N  x x x x                                       */
/*                  |  |  |  |  | | | |                                       */
/*                  |  |  |  |  +-+-+-+---   changing Importance              */
/*                  |  |  |  +------------   Not used                         */
/*                  |  |  +---------------   ERROR                            */
/*                  |  +------------------   BUSY                             */
/*                  +---------------------   READY                            */
/*                                                                            */
/* -------------------------------------------------------------------------- */
#define  d_CCssi_InitToZero                d_CCglo_InitToZero
#define  d_CCssi_Idle                      d_CCglo_Idle  
#define  d_CCssi_Error                     d_CCglo_Error  
#define  d_CCssi_Busy                      d_CCglo_Busy
#define  d_CCssi_Ready                     d_CCglo_Ready
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define  d_CCssi_IndexMask                 d_CCglo_IndexMask
#define  d_CCssi_StateMask                 d_CCglo_StateMask

/* -------------------------------------------------------------------------- */
/*      B U F F E R   I N D E X   D E F I N E S                               */
/*                                                                            */
/*   +---------------------------------------------------------------------+  */
/*   | Buffer Index                                                        |  */
/*   +---------+---------+---------+---------+---------+---------+---------+  */
/*   | Byte 00 | Byte 01 | Byte 02 | Byte 03 | Byte 04 |   ...   | Byte xx |  */
/*   +---------+---------+---------+---------+---------+---------+---------+  */
/*                                                                            */
/* -------------------------------------------------------------------------- */
#define d_CCssi_BufferState            d_CCglo_BufferState     /* Byte 00 */
#define d_CCssi_ChannelNbr             d_CCglo_ChannelNbr      /* Byte 01 */
#define d_CCssi_MaxBufferLength        d_CCglo_MaxBufferLength /* Byte 02 */
#define d_CCssi_DLC                    d_CCglo_DLC             /* Byte 03 */
#define d_CCssi_StartOfData            d_CCglo_StartOfData     /* Byte 04 */



/* ========================================================================== */
/* (7)  L O C A L   V A R I A B L E S  / C O N S T A N T S                    */
/* -------------------------------------------------------------------------- */
/*      B U F F E R    S T A T E    D E F I N E S                             */
/*                                                                            */
/*      BufferState_UB = 7 6 5 4  3 2 1 0                                     */
/*                       | | | |  | | | |                                     */
/*                       | | | |  +-+-+-+---   0000 .. Priority  0 (low)      */
/*                       | | | |               1111 .. Priority 15 (high)     */
/*                       | | | +------------      0 .. reserved               */
/*                       | | +--------------      0 .. Receiver buffer        */
/*                       | |                      1 .. Transmitter buffer     */
/*                       | +----------------      0 .. Buffer idle            */
/*                       |                        1 .. Buffer busy            */
/*                       +------------------      0 .. Buffer invalid         */
/*                                                1 .. Buffer valid           */
/*                                                                            */
/* -------------------------------------------------------------------------- */
#define  d_CCssi_BufferInvalid         d_CCglo_BufferInvalid     /* Bit 7 = 0 */
#define  d_CCssi_BufferValid           d_CCglo_BufferValid       /* Bit 7 = 1 */
 
#define  d_CCssi_BufferIdle            d_CCglo_BufferIdle        /* Bit 6 = 0 */
#define  d_CCssi_BufferBusy            d_CCglo_BufferBusy        /* Bit 6 = 1 */

#define  d_CCssi_RxBuffer              d_CCglo_ReceiverBuffer    /* Bit 5 = 0 */
#define  d_CCssi_TxBuffer              d_CCglo_TransmitterBuffer /* Bit 5 = 1 */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define  d_CCssi_PrioMask              0x0F                      /* Bit 3..0 */





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
|  Function Name:  FUN_CCssi_InitSSI_V                                         |
|                                                                              |
|  Prototype at:   ADLATUS_SSIHandler_tdf.h                                    |
|                                                                              |
|  Description:    Init function for the synchron serial Interface.            |
|                                                                              |
|  Parameter:      -                                                           |
|                                                                              |
|  Return Value:   -                                                           | 
|                                                                              |
+-----------------------------------------------------------------------------*/
void FUN_CCssi_InitSSI_V (void)
{
   FUN_BioEepInit();
   
} /*-+- END OF FUNCTION FUN_CCssi_InitSSI_V -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/

/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CCssi_EepromHandler_UB                                  |
|                                                                              |
|  Prototype at:   ADLATUS_SSIHandler_tdf.h                                    |
|                                                                              |
|  Description:    Interface between NVMHandler and EEPROM        .            |
|                                                                              |
|  Parameter:      -                                                           |
|                                                                              |
|  Return Value:   -                                                           |
|                                                                              |
+-----------------------------------------------------------------------------*/
UBYTE FUN_CCssi_EepromHandler_UB(const NVMRUNTIMEENTRY_ST* t_CCee_Runtimeentry_ST)
{
    UBYTE t_Result_UB = d_CCglo_NoError;
    UWORD t_ByteCounter_UW;
    UBYTE t_Direction_UB;
    UBYTE *EepBuffer;
    UWORD EepAddress;
    UBYTE DataLength_UB;

    /* get direction (read or write) */
    t_Direction_UB = t_CCee_Runtimeentry_ST->Access_UB;
    EepBuffer = (UBYTE*)t_CCee_Runtimeentry_ST->Buffer_UB;
    EepAddress = t_CCee_Runtimeentry_ST->Address_UW;
    DataLength_UB = (UBYTE) t_CCee_Runtimeentry_ST->DataLength_UW;

    t_ByteCounter_UW = 0;

    /* call the appropiate function */
    if (d_CCglo_Write == t_Direction_UB)
    {
      t_ByteCounter_UW = (UWORD) FUN_BioEepWrite(EepBuffer, EepAddress, DataLength_UB);
    }
    else
    {
      t_ByteCounter_UW = (UWORD) FUN_BioEepRead(EepBuffer, EepAddress, DataLength_UB);
    }
    /* look for error info */
    if (t_ByteCounter_UW != (UWORD)DataLength_UB) 
    {
      t_Result_UB = ~d_CCglo_NoError;
    }
    return(t_Result_UB);
}

   
/* ========================================================================== */
/*      E N D   O F   F I L E                                                 */
/* ========================================================================== */
