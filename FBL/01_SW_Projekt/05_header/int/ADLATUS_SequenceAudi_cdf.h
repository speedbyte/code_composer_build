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
|  Filename:  ADLATUS_Sequenceaudi_cdf.h                                       |
|                                                                              |
|  Comment:   This file includes all defines for a audi specific Sequence.     |
|                                                                              |       
|                                                                              |
+------------------------------------------------------------------------------+
| R E V I S I O N   H I S T O R Y                                              |
+------------------------------------------------------------------------------+
|
| Subversion Info:
| $Id: ADLATUS_SequenceAudi_cdf.h 4396 2008-11-04 09:26:32Z maennel $
| $HeadURL: http://smartsvn/svn/adlatusrepository/auslieferungen/S1NN/086350-S1NN-ADLATUS-TMS470R-MucSuc/20081201_S1NN_TMS470R_BlfIso-Source_V1.00/01_SW_Projekt/05_header/int/ADLATUS_SequenceAudi_cdf.h $
|
+------------------------------------------------------------------------------+
|      (c) SMART ELECTRONIC DEVELOPMENT GMBH - all rights reserved             |
+-----------------------------------------------------------------------------*/

/* ========================================================================== */
/*      O P E N   M U L T I P L E   I N C L U D E   P R O T E C T I O N       */
/* ========================================================================== */
#ifndef __CDF_SEQUENCEaudi_
#define __CDF_SEQUENCEaudi_
/* ========================================================================== */

/* -------------------------------------------------------------------------- */
/*                                                                            */
/*  A C T I V E   S E S S I O N   I N F O R M A T I O N                       */
/*                                                                            */
/* -----+------------------------------+------------------------------------- */
#define d_CCseqaudi_DiagSessionStandardMode        (UBYTE) 0x03u /* KWP2000: $10 81 */
#define d_CCseqaudi_DiagSessionEndOfLineModeSysSup (UBYTE) 0x01u /* KWP2000: $10 84 */
#define d_CCseqaudi_DiagSessionProgMode            (UBYTE) 0x00u /* KWP2000: $10 85 */
#define d_CCseqaudi_DiagSessionExtMode             (UBYTE) 0x02u

/* #undef d_CCseqaudi_DiagSessionStandardMode         */
/* #undef d_CCseqaudi_DiagSessionEndOfLineModeSysSup  */

/* UDS defines */
#define d_CCseqaudi_DefaultSession           (UBYTE)  0x01u /* KWP2000: $10 01 */
#define d_CCseqaudi_ProgSession              (UBYTE)  0x02u /* KWP2000: $10 02 */
#define d_CCseqaudi_ExtendedDiagSession      (UBYTE)  0x03u /* KWP2000: $10 03 */

/* for initialisation of session handler */
#define d_CCseqaudi_DiagSessionProgMode      (UBYTE)  0x00u /* KWP2000: $10 02 */


/* -------------------------------------------------------------------------- */
/*                                                                            */
/*      S E R V I C E N U M B E R    A N D   P O S T C O N D I T I O N        */
/*      --------------------------------------------------------------        */
/*      ServiceRequestTable - Reprogramming Mode                              */
/*                                                                            */
/* -------------------------------------------------------------------------- */
/*      DEFINE                      PRECOND-CHECK LINE NUMBER   POSTCONDITION */
/*------------------------------------------------------------+---------------*/
#define  d_CCseqaudi_Always              (UBYTE) 0x00u
#define  d_CCseqaudi_Init                (UBYTE)                     0x00u
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define  d_CCseqaudi_NoPostCondDefined   (UBYTE)                     0x80u
#define  d_CCseqaudi_NegativeResponse    (UBYTE)                     0x40u

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define  d_CCseq_DefaultSession          (UBYTE)  0x01u
#define  d_CCseq_DefaultSessionDone      (UBYTE)                      0x01u
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define  d_CCseq_ExtendedSession         (UBYTE)  0x02u
#define  d_CCseq_ExtendedSessionDone     (UBYTE)                      0x02u
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define  d_CCseq_DTCSetting              (UBYTE)  0x03u
#define  d_CCseq_DTCSettingDone          (UBYTE)                      0x03u
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define  d_CCseq_NDCDisable              (UBYTE)  0x04u
#define  d_CCseq_NDCDisableDone          (UBYTE)                      0x04u
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define  d_CCseq_ProgSession             (UBYTE)  0x05u
#define  d_CCseq_ProgSessionDone         (UBYTE)                      0x05u
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define  d_CCseq_SAGetSeed               (UBYTE)  0x06u
#define  d_CCseq_SAGetSeedDone           (UBYTE)                      0x06u
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define  d_CCseq_SASendKey               (UBYTE)  0x07u
#define  d_CCseq_SASendKeyDone           (UBYTE)                      0x07u
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define  d_CCseq_WriteFingerprint        (UBYTE)  0x08u
#define  d_CCseq_WriteFingerprintDone    (UBYTE)                      0x08u
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define  d_CCseq_RcEraseMemory           (UBYTE)  0x09u
#define  d_CCseq_RcEraseMemoryDone       (UBYTE)                      0x09u
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define  d_CCseq_RequestDownload         (UBYTE)  0x0Au
#define  d_CCseq_RequestDownloadDone     (UBYTE)                      0x0Au
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define  d_CCseq_TransferData            (UBYTE)  0x0Bu
#define  d_CCseq_TransferDataDone        (UBYTE)                      0x0Bu
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define  d_CCseq_RequestTransferExit     (UBYTE)  0x0Cu
#define  d_CCseq_RequestTransferExitDone (UBYTE)                      0x0Cu
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define  d_CCseq_RcCheckMemory           (UBYTE)  0x0Du
#define  d_CCseq_RcCheckMemoryDone       (UBYTE)                      0x0Du
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define  d_CCseq_RcChkProgDependencies   (UBYTE)  0x0Eu
#define  d_CCseq_RcChkProgDependenciesDone (UBYTE)                    0x0Eu
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* -----+------------------------------+------------------------------------- */


/* ========================================================================== */
/*      C L O S E   M U L T I P L E   I N C L U D E   P R O T E C T I O N     */
/* ========================================================================== */
#endif
/* ========================================================================== */


/* ========================================================================== */
/*      E N D   O F   F I L E                                                 */
/* ========================================================================== */
