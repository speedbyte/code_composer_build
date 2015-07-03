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
|  Filename:  ADLATUS_Kwp2000audi_cdf.h                                        |
|                                                                              |
|  Comment:   This file includes all defines for the audi specific             |
|             KWP2000-Protokol handling.                                       |
|                                                                              |
+------------------------------------------------------------------------------+
| R E V I S I O N   H I S T O R Y                                              |
+------------------------------------------------------------------------------+
|
| Subversion Info:
| $Id: ADLATUS_Kwp2000Audi_cdf.h 4396 2008-11-04 09:26:32Z maennel $
| $HeadURL: http://smartsvn/svn/adlatusrepository/auslieferungen/S1NN/086350-S1NN-ADLATUS-TMS470R-MucSuc/20081201_S1NN_TMS470R_BlfIso-Source_V1.00/01_SW_Projekt/05_header/ext/ADLATUS_Kwp2000Audi_cdf.h $
|
+------------------------------------------------------------------------------+
|      (c) SMART ELECTRONIC DEVELOPMENT GMBH - all rights reserved             |
+-----------------------------------------------------------------------------*/

/* ========================================================================== */
/*      O P E N   M U L T I P L E   I N C L U D E   P R O T E C T I O N       */
/* ========================================================================== */
#ifndef __CDF_KWP2000audi_
#define __CDF_KWP2000audi_
/* ========================================================================== */

                 

/* ========================================================================== */
/*                                                                            */
/*                                                                            */
/*      K W P 2 0 0 0   D E F I N E S                                         */
/*      -----------------------------------------------------------           */
/*      audi SPECIFIC                                                        */
/*                                                                            */
/* ========================================================================== */


/* ========================================================================== */
/*      SID $10 $84 - StastDiagnosticSession - EndOfLine Supplier Mode        */
/*      ---------------------------------------------------------             */
/*      N E G A T I V E   R E S P O N S E   C O D E S   ( N R C )             */
/* ========================================================================== */
#define  d_CCkwpaudi_NRCaudiSystemCheckError        (UBYTE)  0xFAu

/* ========================================================================== */
/*      SID $11 - ECU Reset                                                   */
/*      ---------------------------------------------------------             */
/*      Local ID (LID)                                                        */
/* ========================================================================== */
#define  d_CCkwpaudi_LIDaudiSpecific                (UBYTE)  0xFAu

/* ========================================================================== */
/*      SID $1A - Read ECU Identification                                     */
/*      ---------------------------------------------------------             */
/*      Local ID (LID)                                                        */
/* ========================================================================== */
#define  d_CCkwpaudi_LIDHardwareNbraudi             (UBYTE)  0x91u
#define  d_CCkwpaudi_LIDECUIdentaudi                (UBYTE)  0x9Bu
#define  d_CCkwpaudi_LIDFlashStatus                 (UBYTE)  0x9Cu
#define  d_CCkwpaudi_LIDECUHWNR                     (UBYTE)  0x86u
/* ========================================================================== */
/*      SID $27 - Security Access                                             */
/*      ---------------------------------------------------------             */
/*      Local ID (LID)                                                        */
/* ========================================================================== */
#ifdef cs_CCUDS_Audi 
#define  d_CCkwpaudi_LIDSaGetSeedMode1              (UBYTE)  0x11u
#define  d_CCkwpaudi_LIDSaSendKeyMode1              (UBYTE)  0x12u
#else
#define  d_CCkwpaudi_LIDSaGetSeedMode1              (UBYTE)  0x01u
#define  d_CCkwpaudi_LIDSaSendKeyMode1              (UBYTE)  0x02u
#endif

/* ========================================================================== */
/*      SID $27 - Security Access                                             */
/*      ---------------------------------------------------------             */
/*      Negative return Code (NRC)                                            */
/* ========================================================================== */
#define  d_CCkwpaudi_InvalidKey                     (UBYTE)  0xFAu

/* ========================================================================== */
/*      SID $31 - RoutineCountrol                                             */
/*      ---------------------------------------------------------             */
/*      Routine Identifier (RID)                                              */
/* ========================================================================== */
#define  d_CCkwpaudi_RIDchkProgPreCondB0            (UBYTE)  0x02u
#define  d_CCkwpaudi_RIDchkProgPreCondB1            (UBYTE)  0x03u

#define  d_CCkwpaudi_RIDcheckMemoryByte0            (UBYTE)  0x02u
#define  d_CCkwpaudi_RIDcheckMemoryByte1            (UBYTE)  0x02u

#define  d_CCkwpaudi_RIDeraseMemoryByte0            (UBYTE)  0xFFu
#define  d_CCkwpaudi_RIDeraseMemoryByte1            (UBYTE)  0x00u

#define  d_CCkwpaudi_RIDchkProgDependB0             (UBYTE)  0xFFu
#define  d_CCkwpaudi_RIDchkProgDependB1             (UBYTE)  0x01u

/* ========================================================================== */
/*      SID $31 - RoutineCountrol                                             */
/*      ---------------------------------------------------------             */
/*      routine results                                                       */
/* ========================================================================== */
#define  d_CCkwpaudi_correctResult                  (UBYTE)  0x00u
#define  d_CCkwpaudi_incorrectResult                (UBYTE)  0x01u

/* -------------------------------------------------------------------------- */
/* S C A N   S T A T E   D E F I N E S                                        */
/* -------------------------------------------------------------------------- */
#define d_CCkwpaudi_ScanRun   0x01
#define d_CCkwpaudi_ScanStop  0x00 

/* ========================================================================== */
/*      SID $31 - StartRoutineByLocalID                                       */
/*      ---------------------------------------------------------             */
/*      Local ID (LID)                                                        */
/* ========================================================================== */
#define  d_CCkwpaudi_LIDStrblidEraseMemory          (UBYTE)  0xC4u
#define  d_CCkwpaudi_LIDStrblidCalcChecksum         (UBYTE)  0xC5u

/* ========================================================================== */
/*      SID $31 - StartRoutineByLocalID                                       */
/*      ---------------------------------------------------------             */
/*      Negative return Code (NRC)                                            */
/*                                                                            */
/*      SID $38 - StartRoutineByAddress                                       */
/*      ---------------------------------------------------------             */
/*      Negative return Code (NRC)                                            */
/*                                                                            */
/*      SID $34 - RequestDownload                                             */
/*      ---------------------------------------------------------             */
/*      Negative return Code (NRC)                                            */
/* ========================================================================== */
#define  d_CCkwpaudi_NRCInvalidChecksum        (UBYTE)  0xFEu

#define  d_CCkwpaudi_NoProgramm                (UBYTE)  0x90u

#define  d_CCkwpaudi_AddressCheckNotCorrect    (UBYTE)  0xFAu

/*----------------------------------------------------------------------------*/
/*      Control structure                                                     */
/*      -----------------------------------------------------------------     */
/*      defines for managing the control structure of the application         */
/*----------------------------------------------------------------------------*/
#define d_CCkwpaudi_AppInconsistentMask       (UBYTE)     0x0Cu
#define d_CCkwpaudi_AppInconsistent           (UBYTE)     0x04u
#define d_CCkwpaudi_AppProgrammed             (UBYTE)     0x02u
#define d_CCkwpaudi_AppConsistent             (UBYTE)     0x00u


/* ========================================================================== */
/*      C L O S E   M U L T I P L E   I N C L U D E   P R O T E C T I O N     */
/* ========================================================================== */
#endif
/* ========================================================================== */

/* ========================================================================== */
/*      E N D   O F   F I L E                                                 */
/* ========================================================================== */
