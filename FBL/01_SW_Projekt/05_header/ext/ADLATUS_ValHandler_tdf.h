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
|  Filename:  ADLATUS_ValHandler_tdf.h                                         |
|                                                                              |
|  Comment:   This file includes all type definitions for the Validation       |
|             Handler.                                                         |
|                                                                              |
+------------------------------------------------------------------------------+
| R E V I S I O N   H I S T O R Y                                              |
+------------------------------------------------------------------------------+
|
| Subversion Info:
| $Id: ADLATUS_ValHandler_tdf.h 4499 2008-11-27 17:56:27Z maennel $
| $HeadURL: http://smartsvn/svn/adlatusrepository/auslieferungen/S1NN/086350-S1NN-ADLATUS-TMS470R-MucSuc/20081201_S1NN_TMS470R_BlfIso-Source_V1.00/01_SW_Projekt/05_header/ext/ADLATUS_ValHandler_tdf.h $
|                                                                              |
+------------------------------------------------------------------------------+
|      (c) SMART ELECTRONIC DEVELOPMENT GMBH - all rights reserved             |
+-----------------------------------------------------------------------------*/

/* ========================================================================== */
/*      O P E N   M U L T I P L E   I N C L U D E   P R O T E C T I O N       */
/* ========================================================================== */
#ifndef __TDF_VALHANDLER_
#define __TDF_VALHANDLER_
/* ========================================================================== */


extern const PTR_TO_FKT_VAL c_CCVhnd_Quickcheck_FUN;
extern const PTR_TO_FKT_VAL c_CCVhnd_AlternateQuickcheck_FUN;
extern const PTR_TO_FKT_VAL c_CCVhnd_Alternate2Quickcheck_FUN;
extern const PTR_TO_FKT_VAL c_CCVhnd_Alternate3Quickcheck_FUN;

/* ******************** GENERAL VALIDATION FUNCTIONS ************************ */


UBYTE FUN_CCVhnd_InitCtrlST_UB     (const VALCONTROL_ST  *Parameters,
                                    PTR_TO_FKT_VAL ChecksumRoutine);

UBYTE FUN_CCVhnd_CalcChecksum_UB   (void);
UBYTE FUN_CCVhnd_CalcChksumChunk_UB(void);


/* ********************** VALIDATION MAP FUNCTIONS *************************** *
*                                                                              *
* FUN_CCVhnd_CheckValidationMap_UB()                                           *
* ==================================                                           *
*                                                                              *
* This function checks the ConfigProject validation map based on a given UWORD *
* as a value and a mask equal to the entries in the validation map. This       *
* function does not check for feasibility of the creterias.                    *
*                                                                              *
* Once called the ValidationMap is scanned for matching entries and the        *
* selected functions are called. After that the function returns with giving   *
* the appropriate result.                                                      *
*                                                                              *
* **************************************************************************** */

/* For bit definitions see at "ADLATUS_Global_cdf.h":                         */
/* ========================================================================== */
/*                                                                            */
/*  G L O B A L   S Y S T E M    D E F I N E S                                */
/*  ------------------------------------------------------------------        */
/*  Definition Area: Global.12                                                */
/*  ------------------------------------------------------------------        */
/*  Validation Map Defines                                                    */
/*                                                                            */
/*                                                                            */
/*      ----------------------------------------------------------------      */
/*      - MemoryMap at ConfigProject.c                                        */
/*                  .. Validation.c                                           */
/*                                                                            */
/* ========================================================================== */

/* INTERN: Funktion hat identische Funktion wie die vorherigen CalcFSWChecksum,
           CalcADLATUSChecksum, RamCheck,... */

UBYTE FUN_CCVhnd_CheckValidationMap_UB (UWORD t_Value_UW, UWORD t_Mask_UW);

/* INTERN: Aus Kompatibilitätsgründen beibehalten. */
void  FUN_CCVhnd_InitValidation_V  (const VALIDATIONMAP_ST* t_MemoryMap_PST );
UWORD FUN_CCVhnd_GetStartupState_UW(void);


/* Checksum calculation routines for use in validation map entries: */

UBYTE FUN_CCval_CRC32_UB                    (VALCONTROL_ST  *Parameters );
UBYTE FUN_CCval_CRC32_onFly_UB              (VALCONTROL_ST  *Parameters, ULONG*);
UBYTE FUN_CCval_CCITT_UB                    (VALCONTROL_ST  *Parameters );

UBYTE FUN_CCval_LinAdd8Acc8Bit_UB           (VALCONTROL_ST  *Parameters );
UBYTE FUN_CCval_LinAdd8Acc16Bit_UB          (VALCONTROL_ST  *Parameters );
UBYTE FUN_CCval_LinAdd16Acc16Bit_UB         (VALCONTROL_ST  *Parameters );

UBYTE FUN_CCval_QuickCheck_UB               (VALCONTROL_ST  *Parameters );
UBYTE FUN_CCval_AlternateQuickCheck_UB      (VALCONTROL_ST  *Parameters );
UBYTE FUN_CCval_Alternate2QuickCheck_UB     (VALCONTROL_ST  *Parameters );
UBYTE FUN_CCval_Alternate3QuickCheck_UB     (VALCONTROL_ST  *Parameters );

UBYTE FUN_CCval_RamPatternTest_UB           (VALCONTROL_ST  *Parameters );
UBYTE FUN_CCval_CanMsgObjectTest_UB         (VALCONTROL_ST  *Parameters );


/* end validation map functions */


/* ========================================================================== */
/*      C L O S E   M U L T I P L E   I N C L U D E   P R O T E C T I O N     */
/* ========================================================================== */
#endif
/* ========================================================================== */


/* ========================================================================== */
/*      E N D   O F   F I L E                                                 */
/* ========================================================================== */
