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
|  Filename:  ADLATUS_Sequenceaudi.c                                           |
|                                                                              |
|  Comment:   This file includes the sequence structures for the               |
|             Audi specific reprogramming communication.                       |
|                                                                              |
+------------------------------------------------------------------------------+
| R E V I S I O N   H I S T O R Y                                              |
+------------------------------------------------------------------------------+
|                                                                              |
| Subversion Info:                                                             |
| $Id: ADLATUS_SequenceAudi.c 4396 2008-11-04 09:26:32Z maennel $
| $HeadURL: http://smartsvn/svn/adlatusrepository/auslieferungen/S1NN/086350-S1NN-ADLATUS-TMS470R-MucSuc/20081201_S1NN_TMS470R_BlfIso-Source_V1.00/01_SW_Projekt/09_audilib/02_src/ADLATUS_SequenceAudi.c $
|
+------------------------------------------------------------------------------+
|      (c) SMART ELECTRONIC DEVELOPMENT GMBH - all rights reserved             |
+-----------------------------------------------------------------------------*/


/* ========================================================================== */
/* (1)  I N C L U D E S                                                       */
/* -------------------------------------------------------------------------- */
/*      C O N S T A N T   D E F I N I T I O N   F I L E S  (CDF)              */
/* ========================================================================== */
#include "ADLATUS_CompilerSwitches_cdf.h"
#include "ADLATUS_Global_cdf.h"
#include "ADLATUS_Kwp2000Base_cdf.h"
#include "ADLATUS_Kwp2000Audi_cdf.h"
#include "ADLATUS_SequenceAudi_cdf.h"


/* ========================================================================== */
/* (2)  I N C L U D E S                                                       */
/* -------------------------------------------------------------------------- */
/*      T Y P E   D E F I N I T I O N   F I L E S  (TDF)                      */
/* ========================================================================== */
#include "ADLATUS_Types_tdf.h"         /* must be the first tdf-include     */
#include "ADLATUS_Kwp2000Audi_tdf.h"


/* ========================================================================== */
/* (3)  C O M P I L E R   S W I T C H   S E T T I N G S                       */
/* -------------------------------------------------------------------------- */
/*      O N L Y   F O R   T H I S   M O D U L                                 */
/* ========================================================================== */
/* Global Compiler switch settings are done at ADLATUS_CompilerSwitches_CDF.h */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* Local compiler switches: No Entry                                          */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#ifndef cs_CCtypes_32BitSequence
  #ifndef cs_CCtypes_16BitSequence 
       #error (undefined sequence width)
  #endif
#endif
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
/* NO ENTRY                                                                   */

/* ========================================================================== */
/* (7)  L O C A L   V A R I A B L E S  / C O N S T A N T S                    */
/* -------------------------------------------------------------------------- */
/*      O N L Y   F O R   T H I S   M O D U L                                 */
/* ========================================================================== */

/* ========================================================================== */
/*                                                                            */
/*      P R E C O N D I T I O N   T A B L E   ( P C T )                       */
/*                                                                            */
/*      Session: Standard programming session                                 */
/*                                                                            */
/* ========================================================================== */
const UWORD c_CCseqaudi_PCTProgSession_AUW [] =
{
  /*----+----------------++----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------*/
  /* L  |                || 00(0x00) | 01(0x01) | 02(0x02) | 03(0x03) | 04(0x04) | 05(0x05) | 06(0x06) | 07(0x07) | 08(0x08) | 09(0x09) | 10(0x0A) | 11(0x0B) | 12(0x0C) | 13(0x0D) | 14(0x0E) | 15(0x0F) */
  /* I  | Current        ++----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------*/
  /* N  | Condition      || INIT     | Default- | Extended | DTC      | NDC      | Prog     | GetSeed  | SendKey  | Write    | Erase    | RD       | TD       | RTE      | Check    | Check    |          */
  /* E  |                ||          | Session  | Session  | Setting  | Disable  | Session  | 27 01    | 27 02    | FingerPrt| Memory   |          |          |          | Memory   | ProgDep  |          */
  /* -  |                ||          | Done     | Done     | Done     | Done     | Done     | Done     | Done     | Done     | Done     | Done     | Done     | Done     | Done     | Done     |          */
  /* I  +----------------++----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------*/
  /* N  |            Bit || 15       | 14       | 13       | 12       | 11       | 10       | 09       | 08       | 07       | 06       | 05       | 04       | 03       | 02       | 01       | 00       */
  /* D  |            Hex || 0x8000   | 0x4000   | 0x2000   | 0x1000   | 0x0800   | 0x0400   | 0x0200   | 0x0100   | 0x0080   | 0x0040   | 0x0020   | 0x0010   | 0x0008   | 0x0004   | 0x0002   | 0x0001   */
  /* E  +----------------++----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------*/
  /* X  | Service Name   ++----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------*/
  /*----+----------------++----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------*/
  /* 00 |always          */  0x8000u |  0x4000u |  0x2000u |  0x1000u |  0x0800u |  0x0400u |  0x0200u |  0x0100u |  0x0080u |  0x0040u |  0x0020u |  0x0010u |  0x0008u |  0x0004u |  0x0002u |  0x0001u,
  /*----+----------------++----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------*/
  /* 01 |DefaultSession  */  0x8000u |  0x4000u |  0x2000u |  0x1000u |  0x0800u |  0x0400u |  0x0200u |  0x0100u |  0x0080u |  0x0040u |  0x0020u |  0x0010u |  0x0008u |  0x0004u |  0x0002u |  0x0001u,
  /*----+----------------++----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------*/
  /* 02 |ExtendedSession */  0u      |  0x4000u |  0x2000u |  0x1000u |  0x0800u |  0u      |  0u      |  0u      |  0u      |  0u      |  0u      |  0u      |  0u      |  0u      |  0u      |  0u     ,
  /*----+----------------++----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------*/
  /* 03 |DTCSetting      */  0u      |  0u      |  0x2000u |  0x1000u |  0x0800u |  0u      |  0u      |  0u      |  0u      |  0u      |  0u      |  0u      |  0u      |  0u      |  0u      |  0u     ,
  /*----+----------------++----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------*/
  /* 04 |NDCDisable      */  0u      |  0u      |  0x2000u |  0x1000u |  0x0800u |  0u      |  0u      |  0u      |  0u      |  0u      |  0u      |  0u      |  0u      |  0u      |  0u      |  0u     ,
  /*----+----------------++----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------*/
  /* 05 |ProgSession     */  0u      |  0u      |  0x2000u |  0x1000u |  0x0800u |  0x0400u |  0x0200u |  0x0100u |  0x0080u |  0x0040u |  0x0020u |  0x0010u |  0x0008u |  0x0004u |  0x0002u |  0u     ,
  /*----+----------------++----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------*/
  /* 06 |SA GetSeed      */  0u      |  0u      |  0u      |  0u      |  0u      |  0x0400u |  0x0200u |  0x0100u |  0x0080u |  0x0040u |  0x0020u |  0x0010u |  0x0008u |  0x0004u |  0x0002u |  0u     ,
  /*----+----------------++----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------*/
  /* 07 |SA SendKey      */  0u      |  0u      |  0u      |  0u      |  0u      |  0u      |  0x0200u |  0u      |  0u      |  0u      |  0u      |  0u      |  0u      |  0u      |  0u      |  0u     ,
  /*----+----------------++----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------*/
  /* 08 |WrtFingerprint  */  0u      |  0u      |  0u      |  0u      |  0u      |  0u      |  0u      |  0x0100u |  0x0080u |  0u      |  0u      |  0u      |  0u      |  0u      |  0u      |  0u     ,
  /*----+----------------++----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------*/
  /* 09 |Erase Memory    */  0u      |  0u      |  0x2000u |  0u      |  0u      |  0x0400u |  0x0200u |  0x0100u |  0x0080u |  0x0040u |  0u      |  0u      |  0x0008u |  0x0004u |  0u      |  0u     ,
  /*----+----------------++----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------*/
  /* 0A |RD              */  0u      |  0u      |  0u      |  0u      |  0u      |  0u      |  0u      |  0u      |  0x0000u |  0x0040u |  0u      |  0u      |  0x0008u |  0u      |  0u      |  0u     ,
  /*----+----------------++----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------*/
  /* 0B |TD              */  0u      |  0u      |  0u      |  0u      |  0u      |  0u      |  0u      |  0u      |  0u      |  0u      |  0x0020u |  0x0010u |  0u      |  0u      |  0u      |  0u     ,
  /*----+----------------++----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------*/
  /* 0C |RTE             */  0u      |  0u      |  0u      |  0u      |  0u      |  0u      |  0u      |  0u      |  0u      |  0u      |  0u      |  0x0010u |  0u      |  0u      |  0u      |  0u     ,
  /*----+----------------++----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------*/
  /* 0D |Check Memory    */  0u      |  0u      |  0x2000u |  0u      |  0u      |  0x0400u |  0x0200u |  0x0100u |  0x0080u |  0x0040u |  0x0020u |  0x0010u |  0x0008u |  0u      |  0u      |  0u     ,
  /*----+----------------++----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------*/
  /* 0E |Check ProgDep   */  0u      |  0u      |  0u      |  0u      |  0u      |  0u      |  0u      |  0u      |  0u      |  0u      |  0u      |  0u      |  0u      |  0x0004u |  0u      |  0u     
  /*----+----------------++----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------*/
  /* L  | Service Name   ++----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------*/
  /* I  +----------------++----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------*/
  /* N  |            Bit || 15       | 14       | 13       | 12       | 11       | 10       | 09       | 08       | 07       | 06       | 05       | 04       | 03       | 02       | 01       | 00       */
  /* E  |            Hex || 0x8000   | 0x4000   | 0x2000   | 0x1000   | 0x0800   | 0x0400   | 0x0200   | 0x0100   | 0x0080   | 0x0040   | 0x0020   | 0x0010   | 0x0008   | 0x0004   | 0x0002   | 0x0001   */
  /* -  +----------------++----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------*/
  /* I  | Condition      || INIT     | STDS -   | SA seed  | SA Key   | RD       | SRBLID   | RRBLID   | TD       | RTE      | SRBLID   | RRBLID   | not used | not used | not used | not used | not used */
  /* N  |                ||          | ProgMode | 2701     | 2702     |          | Erase    | Erase    |          |          | Chcksum  |          |          |          |          |          |          */
  /* D  | ==             ||          | Done     | Done     | Done     | Done     | Done     | Done     | Done     | Done     | Done     | Done     | Done     | Done     | Done     |          |          */
  /* E  | Current        ++----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------*/
  /* X  | Condition      || 00(0x00) | 01(0x01) | 02(0x02) | 03(0x03) | 04(0x04) | 05(0x05) | 06(0x06) | 07(0x07) | 08(0x08) | 09(0x09) | 10(0x0A) | 11(0x0B) | 12(0x0C) | 13(0x0D) | 14(0x0E) | 15(0x0F) */
  /*----+----------------++----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------+----------*/

};/* --- END OF TABLE --------------------------------------------------------*/                                                       

/* ========================================================================== */
/*                                                                            */
/*      C O M M A N D   I N I T I A L I S A T I O N   T A B L E   (C I T )    */
/*                                                                            */
/*      Session: VW/audi - Standard programming Mode                          */
/*                                                                            */
/* ========================================================================== */
const COMMANDINIT_ST c_CCseqaudi_InitSessionProg_AST[3] = \
{
   /*---------------++---------------------------------+--------------------------------------+-----------------------------------*/
   /*               || New Active Session              | Init Condition                       | Init Allowed Sessions             */
   /*---------------++---------------------------------+--------------------------------------+-----------------------------------*/
 { /* Reset Channel */ d_CCseqaudi_DiagSessionProgMode , d_CCseq_DefaultSessionDone           , d_CCglo_AllowedInDefSession       },
   /*---------------++---------------------------------+--------------------------------------+-----------------------------------*/
 { /* Flash Request */ d_CCseqaudi_DiagSessionProgMode , d_CCseq_ProgSessionDone              , d_CCglo_AllowedInProgSession      }, 
   /*---------------++---------------------------------+--------------------------------------+-----------------------------------*/
 { /* Restart       */ d_CCseqaudi_DiagSessionProgMode , d_CCseq_DefaultSessionDone           , d_CCglo_AllowedInDefSession       } 
   /*---------------++---------------------------------+--------------------------------------+-----------------------------------*/

};/* --- END OF TABLE --------------------------------------------------------*/                                                       


/*============================================================================*/
/*                                                                            */
/*      S E R V I C E   R E Q U E S T S   T A B L E  ( S R T )                */
/*      ------------------------------------------------------                */
/*      Session: Standard programming session (0x10 0x02)                     */
/*                                                                            */
/*      NOTE: There are some dummy functions for error handling. The errors   */
/*            - Request Length Error                                          */
/*            - Sequence Error                                                */
/*            - Service Not Supportet                                         */
/*            - Subfunction Not Supported                                     */
/*            are processed like a correct service. Be sure that this         */
/*            routines are inserted correctly!                                */
/*                                                                            */
/*============================================================================*/
const COMMAND_ST  c_CCseqaudi_SRTKwp2000ProgSession_AST[] = \
{
  /*==========================================================================*/
  /*      Request Length Error                                                */
  /*      Note: This must be the first entry in structure!                    */
  /*            D O N ' T   C H A N G E   T H I S   S E Q U E N C E !!!       */
  /*      Note: For code reduction use the routine of KWP2000Base.c for each  */
  /*            of "Request Length Error" !                                   */
  /*==========================================================================*/
  /*                                                       |  SID | LID | CMD */
  { 
    /* SID           */ d_CCglo_NoEntry,                   /*  -              */
    /* SID Position  */ d_CCglo_NoEntry,
    /* LID           */ d_CCglo_NoEntry,                   /*        -        */
    /* LID Position  */ d_CCglo_NoEntry,
    /* CMD           */ d_CCglo_NoEntry,                   /*              -  */
    /* SelectionInfo */ d_CCglo_NoEntry,
    /* RequestLength */ d_CCglo_DontCheckLength,
    /* Session       */ (d_CCglo_AllAddressAllowed | d_CCglo_AllowedInAllSession),
    /* PreCondLine   */ &c_CCseqaudi_PCTProgSession_AUW[d_CCseqaudi_Always],
    /* ServiceFkt    */ FUN_CCkwpaudi_RequestLengthError_UB,
    /* RespSelection */ &c_CCkwpaudi_RCTRequestLengthError_AUB[0]
  },
  /*==========================================================================*/
  /*      Sequence Error                                                      */
  /*      Note: This must be the second entry in structure!                   */
  /*            D O N ' T   C H A N G E   T H I S   S E Q U E N C E !!!       */
  /*      Note: For code reduction use the routine of KWP2000Base.c for each  */
  /*            of "Sequence Error" !                                         */
  /*==========================================================================*/
  /*                                                       |  SID | LID | CMD */
  { 
    /* SID           */ d_CCglo_NoEntry,                   /*  -              */
    /* SID Position  */ d_CCglo_NoEntry,
    /* LID           */ d_CCglo_NoEntry,                   /*        -        */
    /* LID Position  */ d_CCglo_NoEntry,
    /* CMD           */ d_CCglo_NoEntry,                   /*              -  */
    /* SelectionInfo */ d_CCglo_NoEntry,
    /* RequestLength */ d_CCglo_DontCheckLength,
    /* Session       */ (d_CCglo_AllAddressAllowed | d_CCglo_AllowedInAllSession),
    /* PreCondLine   */ &c_CCseqaudi_PCTProgSession_AUW[d_CCseqaudi_Always],
    /* ServiceFkt    */ FUN_CCkwpaudi_SequenceError_UB,
    /* RespSelection */ &c_CCkwpaudi_RCTSequenceError_AUB[0]
  },
  /*==========================================================================*/
  /*      Service not allowed in actual session                               */
  /*      Note: This must be the third entry in structure!                    */
  /*            D O N ' T   C H A N G E   T H I S   S E Q U E N C E !!!       */
  /*      Note: For code reduction use the routine of KWP2000Base.c for each  */
  /*            of "Session Error" !                                          */
  /*==========================================================================*/
  /*                                                       |  SID | LID | CMD */
  { 
    /* SID           */ d_CCglo_NoEntry,                   /*  -              */
    /* SID Position  */ d_CCglo_NoEntry,
    /* LID           */ d_CCglo_NoEntry,                   /*        -        */
    /* LID Position  */ d_CCglo_NoEntry,
    /* CMD           */ d_CCglo_NoEntry,                   /*              -  */
    /* SelectionInfo */ d_CCglo_NoEntry,
    /* RequestLength */ d_CCglo_DontCheckLength,
    /* Session       */ (d_CCglo_AllAddressAllowed | d_CCglo_AllowedInAllSession),
    /* PreCondLine   */ &c_CCseqaudi_PCTProgSession_AUW[d_CCseqaudi_Always],
    /* ServiceFkt    */ FUN_CCkwpaudi_ServiceNotAllowed_UB,
    /* RespSelection */ &c_CCkwpaudi_RCTServiceNotAllowed_AUB[0]
  },
  /*==========================================================================*/
  /*      Service not allowed with actual addressing method                   */
  /*      Note: This must be the fourth entry in structure!                   */
  /*            D O N ' T   C H A N G E   T H I S   S E Q U E N C E !!!       */
  /*      Note: For code reduction use the routine of KWP2000Base.c for each  */
  /*            of "Addressing Error" !                                       */
  /*==========================================================================*/
  /*                                                       |  SID | LID | CMD */
  { 
    /* SID           */ d_CCglo_NoEntry,                   /*  -              */
    /* SID Position  */ d_CCglo_NoEntry,
    /* LID           */ d_CCglo_NoEntry,                   /*        -        */
    /* LID Position  */ d_CCglo_NoEntry,
    /* CMD           */ d_CCglo_NoEntry,                   /*              -  */
    /* SelectionInfo */ d_CCglo_NoEntry,
    /* RequestLength */ d_CCglo_DontCheckLength,
    /* Session       */ (d_CCglo_AllAddressAllowed | d_CCglo_AllowedInAllSession),
    /* PreCondLine   */ &c_CCseqaudi_PCTProgSession_AUW[d_CCseqaudi_Always],
    /* ServiceFkt    */ FUN_CCkwpaudi_AddressingNotAllowed_UB,
    /* RespSelection */ &c_CCkwpaudi_RCTAddressingNotAllowed_AUB[0]
  },
  /*==========================================================================*/
  /*      physical addressing with no responce                                */
  /*      Note: This must be the fifth entry in structure!                    */
  /*            D O N ' T   C H A N G E   T H I S   S E Q U E N C E !!!       */
  /*      Note: For code reduction use the routine of KWP2000Base.c for each  */
  /*            of "Request Length Error" !                                   */
  /*==========================================================================*/
  /*                                                       |  SID | LID | CMD */
  { 
    /* SID           */ d_CCglo_NoEntry,                   /*  -              */
    /* SID Position  */ d_CCglo_NoEntry,
    /* LID           */ d_CCglo_NoEntry,                   /*        -        */
    /* LID Position  */ d_CCglo_NoEntry,
    /* CMD           */ d_CCglo_NoEntry,                   /*              -  */
    /* SelectionInfo */ d_CCglo_NoEntry,
    /* RequestLength */ d_CCglo_DontCheckLength,
    /* Session       */ (d_CCglo_AllAddressAllowed | d_CCglo_AllowedInAllSession),
    /* PreCondLine   */ &c_CCseqaudi_PCTProgSession_AUW[d_CCseqaudi_Always],
    /* ServiceFkt    */ FUN_CCkwpaudi_AddressingNotAllowed_UB,
    /* RespSelection */ &c_CCkwpaudi_RCTAddressingNotAllowed_AUB[0]
  },
  /* (OPL_364 - begin, reference to new error handling function in KWP2000 )*/
  /*==========================================================================*/
  /*      physical addressing with no responce                                */
  /*      Note: This must be the fifth entry in structure!                    */
  /*            D O N ' T   C H A N G E   T H I S   S E Q U E N C E !!!       */
  /*      Note: For code reduction use the routine of KWP2000Base.c for each  */
  /*            of "SecurityAccessDenied Error" !                                   */
  /*==========================================================================*/
  /*                                                       |  SID | LID | CMD */
  { 
    /* SID           */ d_CCglo_NoEntry,                   /*  -              */
    /* SID Position  */ d_CCglo_NoEntry,
    /* LID           */ d_CCglo_NoEntry,                   /*        -        */
    /* LID Position  */ d_CCglo_NoEntry,
    /* CMD           */ d_CCglo_NoEntry,                   /*              -  */
    /* SelectionInfo */ d_CCglo_NoEntry,
    /* RequestLength */ d_CCglo_DontCheckLength,
    /* Session       */ (d_CCglo_AllAddressAllowed | d_CCglo_AllowedInAllSession),
    /* PreCondLine   */ &c_CCseqaudi_PCTProgSession_AUW[d_CCseqaudi_Always],
    /* ServiceFkt    */ FUN_CCkwpaudi_SecurityAccessDenied_UB,
    /* RespSelection */ &c_CCkwpaudi_RCTSecurityAccessDenied_AUB[0]
  },
  /* (OPL_364 - end)*/

  /*==========================================================================*/
  /*==========================================================================*/
  /* Begin of real KWP2000 Service functions, reference via SID and LID       */
  /*==========================================================================*/  
  
  /*==========================================================================*/
  /* DiagnosticSessionControl - DefaultSession                                */
  /*==========================================================================*/
  /*                                                       |  SID | LID | CMD */
  { 
    /* SID           */ d_CCkwpbase_SIDDiagSessionControl, /* $10             */
    /* SID Position  */ d_CCglo_Byte00,
    /* LID           */ d_CCkwpbase_LIDDefaultSession,     /*       $01       */
    /* LID Position  */ d_CCglo_Byte01,
    /* CMD           */ d_CCglo_NoEntry,                   /*              -  */
    /* SelectionInfo */ d_CCglo_Byte01,
    /* RequestLength */ d_CCglo_02Byte ,
    /* Session       */ (d_CCglo_AllAddressAllowed | d_CCglo_AllowedInAllSession),
    /* PreCondLine   */ &c_CCseqaudi_PCTProgSession_AUW[d_CCseq_DefaultSession],
    /* ServiceFkt    */ FUN_CCkwpaudi_DSCDefaultSession_UB,
    /* RespSelection */ &c_CCkwpaudi_RCTDSCDefaultSession_AUB[0]
  },
  /*==========================================================================*/
  /* DiagnosticSessionControl - ProgrammingSession                            */
  /*==========================================================================*/
  /*                                                       |  SID | LID | CMD */
  { 
    /* SID           */ d_CCkwpbase_SIDDiagSessionControl, /* $10             */
    /* SID Position  */ d_CCglo_Byte00,
    /* LID           */ d_CCkwpbase_LIDProgSession, /*       $02       */
    /* LID Position  */ d_CCglo_Byte01,
    /* CMD           */ d_CCglo_NoEntry,                   /*              -  */
    /* SelectionInfo */ d_CCglo_Byte01,
    /* RequestLength */ d_CCglo_02Byte ,
    /* Session       */ (d_CCglo_AllAddressAllowed | d_CCglo_AllowedInExtSession |d_CCglo_AllowedInProgSession),
    /* PreCondLine   */ &c_CCseqaudi_PCTProgSession_AUW[d_CCseq_ProgSession],
    /* ServiceFkt    */ FUN_CCkwpaudi_DSCProgSession_UB,
    /* RespSelection */ &c_CCkwpaudi_RCTDSCProgSession_AUB[0]
  },
  /*==========================================================================*/
  /* DiagnosticSessionControl - ExtendedSession                               */
  /*==========================================================================*/
  /*                                                       |  SID | LID | CMD */
  { 
    /* SID           */ d_CCkwpbase_SIDDiagSessionControl, /* $10             */
    /* SID Position  */ d_CCglo_Byte00,
    /* LID           */ d_CCkwpbase_LIDExtendedSession,    /*       $03       */
    /* LID Position  */ d_CCglo_Byte01,
    /* CMD           */ d_CCglo_NoEntry,                   /*              -  */
    /* SelectionInfo */ d_CCglo_Byte01,
    /* RequestLength */ d_CCglo_02Byte,
    /* Session       */ (d_CCglo_AllAddressAllowed | d_CCglo_AllowedInStdSession | d_CCglo_AllowedInExtSession),    
    /* PreCondLine   */ &c_CCseqaudi_PCTProgSession_AUW[d_CCseq_ExtendedSession],
    /* ServiceFkt    */ FUN_CCkwpaudi_DSCExtendedSession_UB,
    /* RespSelection */ &c_CCkwpaudi_RCTDSCExtendedSession_AUB[0]
  },
  /*==========================================================================*/
  /* DiagnosticSessionControl                                                 */
  /*==========================================================================*/
  /*                                                       |  SID | LID | CMD */
  { 
    /* SID           */ d_CCkwpbase_SIDDiagSessionControl, /* $10             */
    /* SID Position  */ d_CCglo_Byte00,
    /* LID           */ d_CCglo_NoEntry,                   /*        -        */
    /* LID Position  */ d_CCglo_Byte01,
    /* CMD           */ d_CCglo_NoEntry,                   /*              -  */
    /* SelectionInfo */ d_CCglo_Byte01,
    /* RequestLength */ d_CCglo_02Byte,
    /* Session       */ (d_CCglo_AllAddressAllowed | d_CCglo_AllowedInAllSession),
    /* PreCondLine   */ &c_CCseqaudi_PCTProgSession_AUW[d_CCseqaudi_Always],
    /* ServiceFkt    */ FUN_CCkwpaudi_SubfunctionNotSupported_UB,
    /* RespSelection */ &c_CCkwpaudi_RCTSubfunctionNotSupported_AUB[0]
  },
  /*==========================================================================*/
  /* ControlDCTSetting - DTCSetting off                                       */
  /*==========================================================================*/
  /*                                                       |  SID | LID | CMD */
  { 
    /* SID           */ d_CCkwpbase_SIDControlDTCSetting,  /* $85             */
    /* SID Position  */ d_CCglo_Byte00,
    /* LID           */ d_CCkwpbase_LIDDTCOff,             /*       $02       */
    /* LID Position  */ d_CCglo_Byte01,
    /* CMD           */ d_CCglo_NoEntry,                   /*              -  */
    /* SelectionInfo */ d_CCglo_Byte01,
    /* RequestLength */ d_CCglo_05Byte,
    /* Session       */ (d_CCglo_AllAddressAllowed | d_CCglo_AllowedInExtSession),
    /* PreCondLine   */ &c_CCseqaudi_PCTProgSession_AUW[d_CCseq_DTCSetting],
    /* ServiceFkt    */ FUN_CCkwpaudi_ControlDTCSetting_UB,
    /* RespSelection */ &c_CCkwpaudi_RCTControlDTCSetting_AUB[0]
  },
  /*==========================================================================*/
  /* ControlDCTSetting - DTCSetting on                                        */
  /*==========================================================================*/
  /*                                                       |  SID | LID | CMD */
  { 
    /* SID           */ d_CCkwpbase_SIDControlDTCSetting,  /* $85             */
    /* SID Position  */ d_CCglo_Byte00,
    /* LID           */ d_CCkwpbase_LIDDTCOn,              /*       $01       */
    /* LID Position  */ d_CCglo_Byte01,
    /* CMD           */ d_CCglo_NoEntry,                   /*              -  */
    /* SelectionInfo */ d_CCglo_Byte01,
    /* RequestLength */ d_CCglo_05Byte,
    /* Session       */ (d_CCglo_AllAddressAllowed | d_CCglo_AllowedInExtSession),
    /* PreCondLine   */ &c_CCseqaudi_PCTProgSession_AUW[d_CCseq_DTCSetting],
    /* ServiceFkt    */ FUN_CCkwpaudi_ControlDTCSetting_UB,
    /* RespSelection */ &c_CCkwpaudi_RCTControlDTCSetting_AUB[0]
  },
  /*==========================================================================*/
  /* ControlDCTSetting                                                        */
  /*==========================================================================*/
  /*                                                       |  SID | LID | CMD */
  { 
    /* SID           */ d_CCkwpbase_SIDControlDTCSetting,  /* $85             */
    /* SID Position  */ d_CCglo_Byte00,
    /* LID           */ d_CCglo_NoEntry,                   /*        -        */
    /* LID Position  */ d_CCglo_Byte01,
    /* CMD           */ d_CCglo_NoEntry,                   /*              -  */
    /* SelectionInfo */ d_CCglo_Byte01,
    /* RequestLength */ d_CCglo_05Byte,
    /* Session       */ (d_CCglo_AllAddressAllowed | d_CCglo_AllowedInExtSession),
    /* PreCondLine   */ &c_CCseqaudi_PCTProgSession_AUW[d_CCseqaudi_Always],
    /* ServiceFkt    */ FUN_CCkwpaudi_SubfunctionNotSupported_UB,
    /* RespSelection */ &c_CCkwpaudi_RCTSubfunctionNotSupported_AUB[0]
  },
  /*==========================================================================*/
  /* CommunicationControl - enableRxAndDisableTx                              */
  /*==========================================================================*/
  /*                                                       |  SID | LID | CMD */
  { 
    /* SID           */ d_CCkwpbase_SIDCommunicationControl, /* $28             */
    /* SID Position  */ d_CCglo_Byte00,
    /* LID           */ d_CCkwpbase_LIDenableRxAndDisableTx, /*       $01       */
    /* LID Position  */ d_CCglo_Byte01,
    /* CMD           */ d_CCglo_NoEntry,                     /*              -  */
    /* SelectionInfo */ d_CCglo_Byte01,
    /* RequestLength */ d_CCglo_03Byte,
    /* Session       */ (d_CCglo_AllAddressAllowed | d_CCglo_AllowedInExtSession),
    /* PreCondLine   */ &c_CCseqaudi_PCTProgSession_AUW[d_CCseq_NDCDisable],
    /* ServiceFkt    */ FUN_CCkwpaudi_CommunicationControl_UB,
    /* RespSelection */ &c_CCkwpaudi_RCTCommunicationControl_AUB[0]
  },
  /*==========================================================================*/
  /* CommunicationControl - enableRxAndDisableTx                              */
  /*==========================================================================*/
  /*                                                       |  SID | LID | CMD */
  { 
    /* SID           */ d_CCkwpbase_SIDCommunicationControl, /* $28             */
    /* SID Position  */ d_CCglo_Byte00,
    /* LID           */ d_CCkwpbase_LIDenableRxAndTx,        /*       $00       */
    /* LID Position  */ d_CCglo_Byte01,
    /* CMD           */ d_CCglo_NoEntry,                     /*              -  */
    /* SelectionInfo */ d_CCglo_Byte01,
    /* RequestLength */ d_CCglo_03Byte,
    /* Session       */ (d_CCglo_AllAddressAllowed | d_CCglo_AllowedInExtSession),
    /* PreCondLine   */ &c_CCseqaudi_PCTProgSession_AUW[d_CCseqaudi_Always],
    /* ServiceFkt    */ FUN_CCkwpaudi_CommunicationControl_UB,
    /* RespSelection */ &c_CCkwpaudi_RCTCommunicationControl_AUB[0]
  },
  /*==========================================================================*/
  /* CommunicationControl                                                     */
  /*==========================================================================*/
  /*                                                       |  SID | LID | CMD */
  { 
    /* SID           */ d_CCkwpbase_SIDCommunicationControl, /* $28             */
    /* SID Position  */ d_CCglo_Byte00,
    /* LID           */ d_CCglo_NoEntry,                     /*        -        */
    /* LID Position  */ d_CCglo_Byte01,
    /* CMD           */ d_CCglo_NoEntry,                     /*              -  */
    /* SelectionInfo */ d_CCglo_Byte01,
    /* RequestLength */ d_CCglo_03Byte,
    /* Session       */ (d_CCglo_AllAddressAllowed | d_CCglo_AllowedInExtSession),
    /* PreCondLine   */ &c_CCseqaudi_PCTProgSession_AUW[d_CCseqaudi_Always],
    /* ServiceFkt    */ FUN_CCkwpaudi_SubfunctionNotSupported_UB,
    /* RespSelection */ &c_CCkwpaudi_RCTSubfunctionNotSupported_AUB[0]
  },
  /*==========================================================================*/
  /* SecurityAccess - GetSeed                                                 */
  /*==========================================================================*/
  /*                                                       |  SID | LID | CMD */
  { 
    /* SID           */ d_CCkwpbase_SIDSecurityAccess,     /* $27             */
    /* SID Position  */ d_CCglo_Byte00,
    /* LID           */ d_CCkwpaudi_LIDSaGetSeedMode1,     /*       $11       */
    /* LID Position  */ d_CCglo_Byte01,
    /* CMD           */ d_CCglo_NoEntry,                   /*              -  */
    /* SelectionInfo */ d_CCglo_Byte01,
    /* RequestLength */ d_CCglo_02Byte,
    /* Session       */ (d_CCglo_PhyAddressAllowed | d_CCglo_AllowedInProgSession),
    /* PreCondLine   */ &c_CCseqaudi_PCTProgSession_AUW[d_CCseq_SAGetSeed],
    /* ServiceFkt    */ FUN_CCkwpaudi_SaGetSeed_UB,
    /* RespSelection */ &c_CCkwpaudi_RCTSecurityAccessGetSeed_AUB[0]
  },
  /*==========================================================================*/
  /* SecurityAccess - SendKey                                                 */
  /*==========================================================================*/
  /*                                                       |  SID | LID | CMD */
  { 
    /* SID           */ d_CCkwpbase_SIDSecurityAccess,     /* $27             */
    /* SID Position  */ d_CCglo_Byte00,
    /* LID           */ d_CCkwpaudi_LIDSaSendKeyMode1,     /*       $12       */
    /* LID Position  */ d_CCglo_Byte01,
    /* CMD           */ d_CCglo_NoEntry,                   /*              -  */
    /* SelectionInfo */ d_CCglo_Byte01,
    /* RequestLength */ d_CCglo_06Byte,
    /* Session       */ (d_CCglo_PhyAddressAllowed | d_CCglo_AllowedInProgSession),
    /* PreCondLine   */ &c_CCseqaudi_PCTProgSession_AUW[d_CCseq_SASendKey],
    /* ServiceFkt    */ FUN_CCkwpaudi_SaSendKey_UB,
    /* RespSelection */ &c_CCkwpaudi_RCTSecurityAccessSendKey_AUB[0]
  },
  /*==========================================================================*/
  /* SecurityAccess                                                           */
  /*==========================================================================*/
  /*                                                       |  SID | LID | CMD */
  { 
    /* SID           */ d_CCkwpbase_SIDSecurityAccess,     /* $27             */
    /* SID Position  */ d_CCglo_Byte00,
    /* LID           */ d_CCglo_NoEntry,                   /*        -        */
    /* LID Position  */ d_CCglo_Byte01,
    /* CMD           */ d_CCglo_NoEntry,                   /*              -  */
    /* SelectionInfo */ d_CCglo_Byte01,
    /* RequestLength */ d_CCglo_DontCheckLength,
    /* Session       */ (d_CCglo_PhyAddressAllowed | d_CCglo_AllowedInProgSession),
    /* PreCondLine   */ &c_CCseqaudi_PCTProgSession_AUW[d_CCseqaudi_Always],
    /* ServiceFkt    */ FUN_CCkwpaudi_SubfunctionNotSupported_UB,
    /* RespSelection */ &c_CCkwpaudi_RCTSubfunctionNotSupported_AUB[0]
  },
  /*==========================================================================*/
  /* WriteDataByIdentifier                                                    */
  /*==========================================================================*/
  /*                                                       |  SID | LID | CMD */
  { 
    /* SID           */ d_CCkwpbase_SIDWriteDataById,      /* $2E             */
    /* SID Position  */ d_CCglo_Byte00,
    /* LID           */ d_CCglo_NoEntry,                   /*        -        */
    /* LID Position  */ d_CCglo_Byte01,
    /* CMD           */ d_CCglo_NoEntry,                   /*              -  */
    /* SelectionInfo */ d_CCglo_Byte00,
    /* RequestLength */ d_CCglo_12Byte,
    /* Session       */ (d_CCglo_PhyAddressAllowed | d_CCglo_AllowedInProgSession \
                        | d_CCglo_SecurityAccessRequired),
    /* PreCondLine   */ &c_CCseqaudi_PCTProgSession_AUW[d_CCseq_WriteFingerprint],
    /* ServiceFkt    */ FUN_CCkwpaudi_WriteDataByID_UB,
    /* RespSelection */ &c_CCkwpaudi_RCTWriteDataByID_AUB[0]
  },
  /*==========================================================================*/
  /* RequestDownload                                                          */
  /*==========================================================================*/
  /*                                                       |  SID | LID | CMD */
  { 
    /* SID           */ d_CCkwpbase_SIDRequestDownload,    /* $34             */
    /* SID Position  */ d_CCglo_Byte00,
    /* LID           */ d_CCglo_NoEntry,                   /*        -        */
    /* LID Position  */ d_CCglo_Byte01,
    /* CMD           */ d_CCglo_NoEntry,                   /*              -  */
    /* SelectionInfo */ d_CCglo_Byte00,
    /* RequestLength */ d_CCglo_DontCheckLength,
    /* Session       */ (d_CCglo_PhyAddressAllowed | d_CCglo_AllowedInProgSession\
                        | d_CCglo_SecurityAccessRequired),
    /* PreCondLine   */ &c_CCseqaudi_PCTProgSession_AUW[d_CCseq_RequestDownload],
    /* ServiceFkt    */ FUN_CCkwpaudi_RequestDownload_UB,
    /* RespSelection */ &c_CCkwpaudi_RCTRequestDownload_AUB[0]
  },
  /*==========================================================================*/
  /* TransferData                                                             */
  /*==========================================================================*/
  /*                                                       |  SID | LID | CMD */
  { 
    /* SID           */ d_CCkwpbase_SIDTransferData,       /* $36             */
    /* SID Position  */ d_CCglo_Byte00,
    /* LID           */ d_CCglo_NoEntry,                   /*        -        */
    /* LID Position  */ d_CCglo_Byte01,
    /* CMD           */ d_CCglo_NoEntry,                   /*              -  */
    /* SelectionInfo */ d_CCglo_Byte00,
    /* RequestLength */ d_CCglo_DontCheckLength,
    /* Session       */ (d_CCglo_PhyAddressAllowed | d_CCglo_AllowedInProgSession\
                        | d_CCglo_SecurityAccessRequired),
    /* PreCondLine   */ &c_CCseqaudi_PCTProgSession_AUW[d_CCseq_TransferData],
    /* ServiceFkt    */ FUN_CCkwpaudi_TransferData_UB,
    /* RespSelection */ &c_CCkwpaudi_RCTTransferData_AUB[0]
  },
  /*==========================================================================*/
  /* RequestTransferExit                                                      */
  /*==========================================================================*/
  /*                                                       |  SID | LID | CMD */
  { 
    /* SID           */ d_CCkwpbase_SIDRequestTransferExit,/* $37             */
    /* SID Position  */ d_CCglo_Byte00,
    /* LID           */ d_CCglo_NoEntry,                   /*        -        */
    /* LID Position  */ d_CCglo_Byte01,
    /* CMD           */ d_CCglo_NoEntry,                   /*              -  */
    /* SelectionInfo */ d_CCglo_Byte00,
    /* RequestLength */ d_CCglo_01Byte,
    /* Session       */ (d_CCglo_PhyAddressAllowed | d_CCglo_AllowedInProgSession\
                        | d_CCglo_SecurityAccessRequired),
    /* PreCondLine   */ &c_CCseqaudi_PCTProgSession_AUW[d_CCseq_RequestTransferExit],
    /* ServiceFkt    */ FUN_CCkwpaudi_RequestTransferExit_UB,
    /* RespSelection */ &c_CCkwpaudi_RCTRequestTransferExit_AUB[0]
  },
  /*==========================================================================*/
  /* RoutineControl - CheckProgrammingPreConditions                           */
  /*==========================================================================*/
  /*                                                       |  SID | RID | RID */
  { 
    /* SID           */ d_CCkwpbase_SIDRoutineControl,      /* $31             */
    /* SID Position  */ d_CCglo_Byte00,
    /* LID           */ d_CCkwpaudi_RIDchkProgPreCondB0,    /*         $02       */
    /* LID Position  */ d_CCglo_Byte02,
    /* CMD           */ d_CCkwpaudi_RIDchkProgPreCondB1,    /*               $03 */
    /* SelectionInfo */ d_CCglo_Byte03,
    /* RequestLength */ d_CCglo_04Byte,
    /* Session       */ (d_CCglo_AllAddressAllowed | d_CCglo_AllowedInExtSession),
    /* PreCondLine   */ &c_CCseqaudi_PCTProgSession_AUW[d_CCseqaudi_Always],
    /* ServiceFkt    */ FUN_CCkwpaudi_RcCheckProgramPreCond_UB,
    /* RespSelection */ &c_CCkwpaudi_RCTRcCheckProgramPreCond_AUB[0]
  },
  /*==========================================================================*/
  /* RoutineControl - CheckMemory                                             */
  /*==========================================================================*/
  /*                                                       |  SID | RID | RID */
  { 
    /* SID           */ d_CCkwpbase_SIDRoutineControl,     /* $31             */
    /* SID Position  */ d_CCglo_Byte00,
    /* LID           */ d_CCkwpaudi_RIDcheckMemoryByte0,   /*       $02       */
    /* LID Position  */ d_CCglo_Byte02,
    /* CMD           */ d_CCkwpaudi_RIDcheckMemoryByte1,   /*             $02 */
    /* SelectionInfo */ d_CCglo_Byte03,
    /* RequestLength */ d_CCglo_DontCheckLength,
    /* Session       */ (d_CCglo_PhyAddressAllowed | d_CCglo_AllowedInProgSession\
                        | d_CCglo_AllowedInExtSession),
    /* PreCondLine   */ &c_CCseqaudi_PCTProgSession_AUW[d_CCseq_RcCheckMemory],
    /* ServiceFkt    */ FUN_CCkwpaudi_RcCheckMemory_UB,
    /* RespSelection */ &c_CCkwpaudi_RCTRcCheckMemory_AUB[0]
  },
  /*==========================================================================*/
  /* RoutineControl - CheckProgrammingDependencies                            */
  /*==========================================================================*/
  /* NOTE: the last byte of the routine identifier is checked first           */
  /*       because the first byte is 0xFF and equals NoEntry!                 */
  /*                                                       |  SID | RID | RID */
  { 
    /* SID           */ d_CCkwpbase_SIDRoutineControl,     /* $31             */
    /* SID Position  */ d_CCglo_Byte00,
    /* LID           */ d_CCkwpaudi_RIDchkProgDependB1,      /*             $01 */
    /* LID Position  */ d_CCglo_Byte03,
    /* CMD           */ d_CCkwpaudi_RIDchkProgDependB0,      /*       $FF       */
    /* SelectionInfo */ d_CCglo_Byte02,
    /* RequestLength */ d_CCglo_04Byte,
    /* Session       */ (d_CCglo_PhyAddressAllowed | d_CCglo_AllowedInProgSession\
                        | d_CCglo_SecurityAccessRequired ),
    /* PreCondLine   */ &c_CCseqaudi_PCTProgSession_AUW[d_CCseq_RcChkProgDependencies],
    /* ServiceFkt    */ FUN_CCkwpaudi_RcChkProgDependencies_UB,
    /* RespSelection */ &c_CCkwpaudi_RCTRcChkProgDependencies_AUB[0]
  },
  /*==========================================================================*/
  /* RoutineControl - EraseMemory                                             */
  /*==========================================================================*/
  /* NOTE: the last byte of the routine identifier is checked first           */
  /*       because the first byte is 0xFF and equals NoEntry!                 */
  /*                                                       |  SID | RID | RID */
  { 
    /* SID           */ d_CCkwpbase_SIDRoutineControl,       /* $31             */
    /* SID Position  */ d_CCglo_Byte00,
    /* LID           */ d_CCkwpaudi_RIDeraseMemoryByte1,   /*             $00 */
    /* LID Position  */ d_CCglo_Byte03,
    /* CMD           */ d_CCkwpaudi_RIDeraseMemoryByte0,   /*       $FF       */
    /* SelectionInfo */ d_CCglo_Byte02,
    /* RequestLength */ d_CCglo_DontCheckLength,
    /* Session       */ (d_CCglo_PhyAddressAllowed | d_CCglo_AllowedInProgSession\
                        | d_CCglo_AllowedInExtSession ),
    /* PreCondLine   */ &c_CCseqaudi_PCTProgSession_AUW[d_CCseq_RcEraseMemory],
    /* ServiceFkt    */ FUN_CCkwpaudi_RcEraseMemory_UB,
    /* RespSelection */ &c_CCkwpaudi_RCTRcEraseMemory_AUB[0]
  },
  /*==========================================================================*/
  /* RoutineControl -                                                         */
  /*==========================================================================*/
  /*                                                       |  SID | LID | CMD */
  { 
    /* SID           */ d_CCkwpbase_SIDRoutineControl,     /* $31             */
    /* SID Position  */ d_CCglo_Byte00,
    /* LID           */ d_CCglo_NoEntry,                   /*        -        */
    /* LID Position  */ d_CCglo_Byte01,
    /* CMD           */ d_CCglo_NoEntry,                   /*              -  */
    /* SelectionInfo */ d_CCglo_Byte00,
    /* RequestLength */ d_CCglo_DontCheckLength,
    /* Session       */ (d_CCglo_AllAddressAllowed | d_CCglo_AllowedInExtSession),
    /* PreCondLine   */ &c_CCseqaudi_PCTProgSession_AUW[d_CCseqaudi_Always],
    /* ServiceFkt    */ FUN_CCkwpaudi_RcRoutineIdNotSupported_UB,
    /* RespSelection */ &c_CCkwpaudi_RCTRcRoutineIdNotSupported_AUB[0]
  },
  /*==========================================================================*/
  /* ReadDataByIdentifier                                                     */
  /*==========================================================================*/
  /*                                                       |  SID | LID | CMD */
  { 
    /* SID           */ d_CCkwpbase_SIDReadDataById,       /* $22             */
    /* SID Position  */ d_CCglo_Byte00,
    /* LID           */ d_CCglo_NoEntry,                   /*        -        */
    /* LID Position  */ d_CCglo_Byte01,
    /* CMD           */ d_CCglo_NoEntry,                   /*              -  */
    /* SelectionInfo */ d_CCglo_Byte00,
    /* RequestLength */ d_CCglo_DontCheckLength,
    /* Session       */ (d_CCglo_PhyAddressAllowed | d_CCglo_AllowedInAllSession),
    /* PreCondLine   */ &c_CCseqaudi_PCTProgSession_AUW[d_CCseqaudi_Always],
    /* ServiceFkt    */ FUN_CCkwpaudi_ReadDataById_UB,
    /* RespSelection */ &c_CCkwpaudi_RCTReadDataById_AUB[0]
  },
  /*==========================================================================*/
  /* TesterPresent - ZeroSubFuction                                           */
  /*==========================================================================*/
  /*                                                       |  SID | LID | CMD */
  { 
    /* SID           */ d_CCkwpbase_SIDTesterPresent,      /* $3E             */
    /* SID Position  */ d_CCglo_Byte00,
    /* LID           */ d_CCkwpbase_LIDzeroSubFunction,    /*       $00       */
    /* LID Position  */ d_CCglo_Byte01,
    /* CMD           */ d_CCglo_NoEntry,                   /*              -  */
    /* SelectionInfo */ d_CCglo_Byte01,
    /* RequestLength */ d_CCglo_02Byte,
    /* Session       */ (d_CCglo_AllAddressAllowed | d_CCglo_AllowedInAllSession),
    /* PreCondLine   */ &c_CCseqaudi_PCTProgSession_AUW[d_CCseqaudi_Always],
    /* ServiceFkt    */ FUN_CCkwpaudi_TesterPresent_UB,
    /* RespSelection */ &c_CCkwpaudi_RCTTesterPresent_AUB[0]
  },
  /*==========================================================================*/
  /* TesterPresent                                                            */
  /*==========================================================================*/
  /*                                                       |  SID | LID | CMD */
  { 
    /* SID           */ d_CCkwpbase_SIDTesterPresent,      /* $3E             */
    /* SID Position  */ d_CCglo_Byte00,
    /* LID           */ d_CCglo_NoEntry,                   /*        -        */
    /* LID Position  */ d_CCglo_Byte01,
    /* CMD           */ d_CCglo_NoEntry,                   /*              -  */
    /* SelectionInfo */ d_CCglo_Byte01,
    /* RequestLength */ d_CCglo_02Byte,
    /* Session       */ (d_CCglo_AllAddressAllowed | d_CCglo_AllowedInAllSession),
    /* PreCondLine   */ &c_CCseqaudi_PCTProgSession_AUW[d_CCseqaudi_Always],
    /* ServiceFkt    */ FUN_CCkwpaudi_SubfunctionNotSupported_UB,
    /* RespSelection */ &c_CCkwpaudi_RCTSubfunctionNotSupported_AUB[0]
  },
  /*==========================================================================*/
  /* ECU-Reset - hardReset                                                    */
  /*==========================================================================*/
  /*                                                       |  SID | LID | CMD */
  { 
    /* SID           */ d_CCkwpbase_SIDEcuReset,           /* $11             */
    /* SID Position  */ d_CCglo_Byte00,
    /* LID           */ d_CCkwpbase_LIDhardReset,          /*       $01       */
    /* LID Position  */ d_CCglo_Byte01,
    /* CMD           */ d_CCglo_NoEntry,                   /*              -  */
    /* SelectionInfo */ d_CCglo_Byte01,
    /* RequestLength */ d_CCglo_02Byte,
    /* Session       */ (d_CCglo_AllAddressAllowed | d_CCglo_AllowedInAllSession),
    /* PreCondLine   */ &c_CCseqaudi_PCTProgSession_AUW[d_CCseqaudi_Always],
    /* ServiceFkt    */ FUN_CCkwpaudi_ECUReset_UB,
    /* RespSelection */ &c_CCkwpaudi_RCTECUReset_AUB[0]
  },
  /*==========================================================================*/
  /* ECU-Reset                                                                */
  /*==========================================================================*/
  /*                                                       |  SID | LID | CMD */
  { 
    /* SID           */ d_CCkwpbase_SIDEcuReset,           /* $11             */
    /* SID Position  */ d_CCglo_Byte00,
    /* LID           */ d_CCglo_NoEntry,                   /*        -        */
    /* LID Position  */ d_CCglo_Byte01,
    /* CMD           */ d_CCglo_NoEntry,                   /*              -  */
    /* SelectionInfo */ d_CCglo_Byte01,
    /* RequestLength */ d_CCglo_02Byte,
    /* Session       */ (d_CCglo_AllAddressAllowed | d_CCglo_AllowedInAllSession),
    /* PreCondLine   */ &c_CCseqaudi_PCTProgSession_AUW[d_CCseqaudi_Always],
    /* ServiceFkt    */ FUN_CCkwpaudi_SubfunctionNotSupported_UB,
    /* RespSelection */ &c_CCkwpaudi_RCTSubfunctionNotSupported_AUB[0]
  },
  /*==========================================================================*/
  /*      Service Not Supported                                               */
  /*      Note: This must be the last entry in structure!                     */
  /*            D O N ' T   C H A N G E   T H I S   S E Q U E N C E !!!       */
  /*      Note: For code reduction use the routine of KWP2000Base.c for each  */
  /*            of "Service Not Supported" !                                  */
  /*==========================================================================*/
  /*                                                       |  SID | LID | CMD */
  { 
    /* SID           */ d_CCglo_NoEntry,                   /*  -              */
    /* SID Position  */ d_CCglo_Byte00,
    /* LID           */ d_CCglo_NoEntry,                   /*        -        */
    /* LID Position  */ d_CCglo_Byte01,
    /* CMD           */ d_CCglo_NoEntry,                   /*              -  */
    /* SelectionInfo */ d_CCglo_Byte00,
    /* RequestLength */ d_CCglo_DontCheckLength,
    /* Session       */ (d_CCglo_AllAddressAllowed | d_CCglo_AllowedInAllSession),
    /* PreCondLine   */ &c_CCseqaudi_PCTProgSession_AUW[d_CCseqaudi_Always],
    /* ServiceFkt    */ FUN_CCkwpaudi_ServiceNotSupported_UB,
    /* RespSelection */ &c_CCkwpaudi_RCTServiceNotSupported_AUB[0]
  }
  /*==========================================================================*/
  /* Structure end                                                            */
  /*==========================================================================*/

};/* --- END OF TABLE --------------------------------------------------------*/                                                       


/* ========================================================================== */
/* (8)  L O C A L   F U N C T I O N   P R O T O T Y P E S                     */
/* -------------------------------------------------------------------------- */
/*      O N L Y   F O R   T H I S   M O D U L                                 */
/* ========================================================================== */
/* No entry                                                                   */

/* ========================================================================== */
/* (9)  G L O B A L   V A R I A B L E S                                       */
/* -------------------------------------------------------------------------- */
/*      D E F I N E D   W I T H I N   T H I S   M O D U L                     */
/* ========================================================================== */
/* No entry                                                                   */

/* ========================================================================== */
/* (10) F U N C T I O N   D E F I N I T I O N S                               */
/* -------------------------------------------------------------------------- */
/*      F O R   T H I S   M O D U L                                           */
/* ========================================================================== */
/* NO ENTRY                                                                   */

/* ========================================================================== */
/*      E N D   O F   F I L E                                                 */
/* ========================================================================== */
