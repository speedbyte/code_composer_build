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
|  Filename:  ADLATUS_ValidationAudi_tdf.h                                     |
|                                                                              |
|  Comment:   This file includes the function prototypes for the customer      |
|             specific download validation.                                    |
|                                                                              |
+------------------------------------------------------------------------------+
| R E V I S I O N   H I S T O R Y                                              |
+------------------------------------------------------------------------------+
|
| Subversion Info:
| $Id: ADLATUS_ValidationAudi_tdf.h 4396 2008-11-04 09:26:32Z maennel $
| $HeadURL: http://smartsvn/svn/adlatusrepository/auslieferungen/S1NN/086350-S1NN-ADLATUS-TMS470R-MucSuc/20081201_S1NN_TMS470R_BlfIso-Source_V1.00/01_SW_Projekt/05_header/ext/ADLATUS_ValidationAudi_tdf.h $
|
+------------------------------------------------------------------------------+
|      (c) SMART ELECTRONIC DEVELOPMENT GMBH - all rights reserved             |
+-----------------------------------------------------------------------------*/


/* ========================================================================== */
/*      O P E N   M U L T I P L E   I N C L U D E   P R O T E C T I O N       */
/* ========================================================================== */
#ifndef __TDF_VALIDATIONAUDI_
#define __TDF_VALIDATIONAUDI_
/* ========================================================================== */




/* ========================================================================== */
/*                                                                            */
/*      E X T E R N   D E C L A R A T I O N S                                 */
/*                                                                            */
/* ========================================================================== */


/* ========================================================================== */
/*                                                                            */
/*      G L O B A L   F U N C T I O N   P R O T O T Y P E S                   */
/*                                                                            */
/* ========================================================================== */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Check routines for Download --> Customer Interface                         */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
UBYTE FUN_CCvalaudi_CalcCsumRamDownload_UB   ( ULONG t_StartAddress_UL,   \
                                               ULONG t_StopAddress_UL,    \
                                               ULONG t_ChecksumAddress_UL);

UBYTE FUN_CCvalaudi_CalcCsumFlashDownload_UB (void );
UBYTE FUN_CCvalaudi_InitCalcCsumFlashDownload_UB ( ULONG t_StartAddress_UL,   \
                                                   ULONG t_StopAddress_UL,    \
                                                   ULONG t_ChecksumAddress_UL);
UBYTE FUN_CCvalaudi_CalcCsumEepromDownload_UB (void );
UBYTE FUN_CCvalaudi_InitCalcCsumEepromDownload_UB ( ULONG t_StartAddress_UL,   \
                                                    ULONG t_StopAddress_UL,    \
                                                    ULONG t_ChecksumAddress_UL);

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


/* ========================================================================== */
/*      C L O S E   M U L T I P L E   I N C L U D E   P R O T E C T I O N     */
/* ========================================================================== */
#endif
/* ========================================================================== */

/* ========================================================================== */
/*      E N D   O F   F I L E                                                 */
/* ========================================================================== */

