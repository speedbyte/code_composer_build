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
|  Filename:  ADLATUS_ValHandler_cdf.h                                         |
|                                                                              |
|  Comment:   This file includes all defines for the Validation Handler.       |
|                                                                              |
+------------------------------------------------------------------------------+
| R E V I S I O N   H I S T O R Y                                              |
+------------------------------------------------------------------------------+
|
| Subversion Info:
| $Id: ADLATUS_ValHandler_cdf.h 4396 2008-11-04 09:26:32Z maennel $
| $HeadURL: http://smartsvn/svn/adlatusrepository/auslieferungen/S1NN/086350-S1NN-ADLATUS-TMS470R-MucSuc/20081201_S1NN_TMS470R_BlfIso-Source_V1.00/01_SW_Projekt/05_header/ext/ADLATUS_ValHandler_cdf.h $
|
+------------------------------------------------------------------------------+
|      (c) SMART ELECTRONIC DEVELOPMENT GMBH - all rights reserved             |
+-----------------------------------------------------------------------------*/

/* ========================================================================== */
/*      O P E N   M U L T I P L E   I N C L U D E   P R O T E C T I O N       */
/* ========================================================================== */
#ifndef __CDF_VALHANDLER_
#define __CDF_VALHANDLER_
/* ========================================================================== */

#define d_CCVhnd_InitChunkSize_UW        0x0020u

#define d_CCVhnd_Idle              (UBYTE) 0x00u

#define d_CCVhnd_CheckOK           (UBYTE) 0x01u
#define d_CCVhnd_CheckNOK          (UBYTE) 0x02u
#define d_CCVhnd_Break             (UBYTE) 0x04u
#define d_CCVhnd_CalibrationDone   (UBYTE) 0x08u

#define d_CCVhnd_Init              (UBYTE) 0x10u
#define d_CCVhnd_Error             (UBYTE) 0x20u
#define d_CCVhnd_Busy              (UBYTE) 0x40u
#define d_CCVhnd_Ready             (UBYTE) 0x80u


/* -------------------------------------------------------------------------- */
/*                                                                            */
/*      D E F I N E S   f o r                                                 */
/*      V a l i d a t i o n   h a n d l e r   c o n t r o l   f l a g s       */
/*      -----------------------------------------------------------------     */
/*      Control_UB  =  7 6 5 4  3 2 1 0                                       */
/*                     | | | |  | | | |                                       */
/*                     | | | |  | | | +---  not set: do normal checksum       */
/*                     | | | |  | | |                 calculation             */
/*                     | | | |  | | |                                         */
/*                     | | | |  | | |       set: check the result of checksum */
/*                     | | | |  | | |            calculation                  */
/*                     | | | |  | | |                                         */
/*                     | | | |  | | +-----  not used                          */
/*                     | | | |  | +-------  not used                          */
/*                     | | | |  +---------  not used                          */
/*                     | | | |                                                */
/*                     | | | +------------  not used                          */
/*                     | | +--------------  not used                          */
/*                     | +----------------  not used                          */
/*                     +------------------  not used                          */
/*                                                                            */
/* -------------------------------------------------------------------------- */
#define d_CCVhnd_InitControlFlags      0x00
#define d_CCVhnd_CheckForResult        0x01


/* ========================================================================== */
/*      C L O S E   M U L T I P L E   I N C L U D E   P R O T E C T I O N     */
/* ========================================================================== */
#endif
/* ========================================================================== */


/* ========================================================================== */
/*      E N D   O F   F I L E                                                 */
/* ========================================================================== */
