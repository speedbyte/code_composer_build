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
|  Filename:  ADLATUS_SessionHandler_tdf.h                                     |
|                                                                              |
|  Comment:   This file includes the function prototypes and the extern        |
|             declarations for the modul ADLATUS_SessionHandler.c .            |
|                                                                              |
+------------------------------------------------------------------------------+
| R E V I S I O N   H I S T O R Y                                              |
+------------------------------------------------------------------------------+
|
| Subversion Info:
| $Id: ADLATUS_SessionHandler_tdf.h 4396 2008-11-04 09:26:32Z maennel $
| $HeadURL: http://smartsvn/svn/adlatusrepository/auslieferungen/S1NN/086350-S1NN-ADLATUS-TMS470R-MucSuc/20081201_S1NN_TMS470R_BlfIso-Source_V1.00/01_SW_Projekt/05_header/int/ADLATUS_SessionHandler_tdf.h $
|
+------------------------------------------------------------------------------+
|      (c) SMART ELECTRONIC DEVELOPMENT GMBH - all rights reserved             |
+-----------------------------------------------------------------------------*/


/* ========================================================================== */
/*      O P E N   M U L T I P L E   I N C L U D E   P R O T E C T I O N       */
/* ========================================================================== */
#ifndef __TDF_SESSIONHANDLER_
#define __TDF_SESSIONHANDLER_
/* ========================================================================== */



/* ========================================================================== */
/* (1)  E X T E R N   D E C L A R A T I O N S                                 */
/* -------------------------------------------------------------------------- */
/*      all extern available declarations of the modul                        */
/* ========================================================================== */
/* No entry                                                                   */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* ========================================================================== */
/* (2)  G L O B A L   F U N C T I O N   P R O T O T Y P E S                   */
/* -------------------------------------------------------------------------- */
/*      all global available function prototypes of the modul                 */
/* ========================================================================== */
void  FUN_CCses_InitSessionLayer_V  (  const SESSIONINTERFACE_ST* t_SessionInterface_PST );

UBYTE FUN_CCses_GetActiveSession_UB ( UBYTE t_Channel_UB );

void  FUN_CCses_InitChannel_V       ( UBYTE t_Channel_UB,       \
                                      UBYTE t_ActiveSession_UB, \
                                      UBYTE t_InitHandle_UB     );

UBYTE FUN_CCses_GetActiceSession_UB ( UBYTE t_Channel_UB );

UBYTE FUN_CCses_RxSession_UB        ( PTR_BUFFER_UB t_InBuffer_PBUF, \
                                      PTR_BUFFER_UB t_OutBuffer_PBUF );

UBYTE FUN_CCses_TxSession_UB        ( PTR_BUFFER_UB t_InBuffer_PBUF, \
                                      PTR_BUFFER_UB t_OutBuffer_PBUF );


/* ========================================================================== */
/*      C L O S E   M U L T I P L E   I N C L U D E   P R O T E C T I O N     */
/* ========================================================================== */
#endif
/* ========================================================================== */


/* ========================================================================== */
/*      E N D   O F   F I L E                                                 */
/* ========================================================================== */
