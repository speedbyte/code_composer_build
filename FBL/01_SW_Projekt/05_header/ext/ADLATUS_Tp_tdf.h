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
|  Filename:  ADLATUS_TP_tdf.h                                                 |
|                                                                              |
|  Comment:   This file includes all the prototypes for the modul.             |
|                                                                              |
+------------------------------------------------------------------------------+
| R E V I S I O N   H I S T O R Y                                              |
+------------------------------------------------------------------------------+
|
| Subversion Info:
| $Id: ADLATUS_Tp_tdf.h 4396 2008-11-04 09:26:32Z maennel $
| $HeadURL: http://smartsvn/svn/adlatusrepository/auslieferungen/S1NN/086350-S1NN-ADLATUS-TMS470R-MucSuc/20081201_S1NN_TMS470R_BlfIso-Source_V1.00/01_SW_Projekt/05_header/ext/ADLATUS_Tp_tdf.h $
|
+------------------------------------------------------------------------------+
|      (c) SMART ELECTRONIC DEVELOPMENT GMBH - all rights reserved             |
+-----------------------------------------------------------------------------*/

/* ========================================================================== */
/*      O P E N   M U L T I P L E   I N C L U D E   P R O T E C T I O N       */
/* ========================================================================== */
#ifndef __TDF_TP_
#define __TDF_TP_
/* ========================================================================== */


/* ========================================================================== */
/*                                                                            */
/*  T Y P E   D E F I N I T I O N                                             */
/*  ---------------------------------------------------------------------     */
/*  Type definition: Tp.01                                                    */
/*  ---------------------------------------------------------------------     */
/*  TP Layer Interface                                                        */
/*                                                                            */
/* ========================================================================== */
typedef struct _TPINTERFACE
{
  UWORD  STmin_UW;      /*  2 Byte - SeparationTime                           */
  UWORD  STmax_UW;      /*  2 Byte - Timeout Separation Time                  */
  UWORD  P2min_UW;      /*  2 Byte - P2 min timing                            */
  UWORD  P2max_UW;      /*  2 Byte - Timeout P2                               */
  UWORD  P3max_UW;      /*  2 Byte - Timeout P3                               */

  UBYTE  BSmax_UB;
                        /*--------------------------------------------------- */
} TPINTERFACE_ST;       /* 10 Byte                                            */

/* -+-  END OF Type Definition 'Tp.01'  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/



/* ========================================================================== */
/*                                                                            */
/*      E X T E R N   D E C L A R A T I O N S                                 */
/*                                                                            */
/* ========================================================================== */

/*----------------------------------------------------------------------------*/
/* Description:   Transport Layer Interface Table                             */
/*----------------------------------------------------------------------------*/
/* NOTE: The interface is defined at ADLATUS_ConfigProject.c                  */
extern const TPINTERFACE_ST       c_CCconprj_TpInterface_ST;


/* ========================================================================== */
/*                                                                            */
/*      G L O B A L   F U N C T I O N   P R O T O T Y P E S                   */
/*                                                                            */
/* ========================================================================== */
UBYTE FUN_CCtp_TxTp_UB        ( PTR_BUFFER_UB t_InBuffer_PBUF, \
                                PTR_BUFFER_UB t_OutBuffer_PBUF );

UBYTE FUN_CCtp_RxTp_UB        ( PTR_BUFFER_UB t_InBuffer_PBUF, \
                                PTR_BUFFER_UB t_OutBuffer_PBUF );

void FUN_CCTp_Timer_V         ( UWORD t_TpTimerTicks_UW);

void FUN_CCtp_Init_V(const TPINTERFACE_ST *t_TpInterface_PST);

void FUN_CCtp_InitInternal_V  ( void );


/* ========================================================================== */
/*      C L O S E   M U L T I P L E   I N C L U D E   P R O T E C T I O N     */
/* ========================================================================== */
#endif
/* ========================================================================== */

/* ========================================================================== */
/*      E N D   O F   F I L E                                                 */
/* ========================================================================== */

