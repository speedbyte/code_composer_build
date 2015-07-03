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
|  Filename:  ADLATUS_Global_tdf.h                                             |
|                                                                              |
|  Comment:   This file includes all type defines and extern declarations for  |
|             global SMART ADLATUS files.                                      |
|                                                                              |
+------------------------------------------------------------------------------+
| R E V I S I O N   H I S T O R Y                                              |
+------------------------------------------------------------------------------+
|
| Subversion Info:
| $Id: ADLATUS_Global_tdf.h 4396 2008-11-04 09:26:32Z maennel $
| $HeadURL: http://smartsvn/svn/adlatusrepository/auslieferungen/S1NN/086350-S1NN-ADLATUS-TMS470R-MucSuc/20081201_S1NN_TMS470R_BlfIso-Source_V1.00/01_SW_Projekt/05_header/ext/ADLATUS_Global_tdf.h $
|
+------------------------------------------------------------------------------+
|      (c) SMART ELECTRONIC DEVELOPMENT GMBH - all rights reserved             |
+-----------------------------------------------------------------------------*/

/* ========================================================================== */
/*      O P E N   M U L T I P L E   I N C L U D E   P R O T E C T I O N       */
/* ========================================================================== */
#ifndef __TDF_GLOBAL_
#define __TDF_GLOBAL_
/* ========================================================================== */


/* ========================================================================== */
/*                                                                            */
/*      E X T E R N   D E C L A R A T I O N S                                 */
/*                                                                            */
/* ========================================================================== */
/* No entry                                                                   */

/* ========================================================================== */
/*                                                                            */
/*      G L O B A L   F U N C T I O N   P R O T O T Y P E S                   */
/*                                                                            */
/* ========================================================================== */
void  FUN_CCglo_AMemCopy_V        ( ULONG         t_Destination_UL,   
                                    ULONG         t_Source_UL,        
                                    ULONG         t_NbrOfElements_UL );
                           
void  FUN_CCglo_PMemCopy_V        ( FAR_PTR_UBYTE t_Destination_PUB, 
                                    FAR_PTR_UBYTE t_Source_PUB,      
                                    ULONG         t_NumOfElems_UL    );

UWORD  FUN_CCglo_CollectData_UB ( PTR_BUFFER_UB t_EncryptBuffer_PBUF, \
                                  PTR_BUFFER_UB t_DataPtr_PUB  );

void  FUN_CCglo_InitCollectData_V (void);

UWORD FUN_CCglo_GetIntermediateBufferNumbers_V (void);

ULONG FUN_CCglo_GetRandomValue_UL ( void );



/* ========================================================================== */
/*      C L O S E   M U L T I P L E   I N C L U D E   P R O T E C T I O N     */
/* ========================================================================== */
#endif
/* ========================================================================== */

/* ========================================================================== */
/*      E N D   O F   F I L E                                                 */
/* ========================================================================== */

