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
|  Filename:  FUN_CCval_CanMsgObjectTest_UB.c                                  |
|                                                                              |
|  Comment:   This file includes a checksum calculations functions.            |
|                                                                              |
+------------------------------------------------------------------------------+
| R E V I S I O N   H I S T O R Y                                              |
+------------------------------------------------------------------------------+
|
| Subversion Info:
| $Id: ADLATUS_FUN_Val_CanMsgObjectTest_UB.c 4396 2008-11-04 09:26:32Z maennel $
| $HeadURL: http://smartsvn/svn/adlatusrepository/auslieferungen/S1NN/086350-S1NN-ADLATUS-TMS470R-MucSuc/20081201_S1NN_TMS470R_BlfIso-Source_V1.00/01_SW_Projekt/07_chcklib/02_src/ADLATUS_FUN_Val_CanMsgObjectTest_UB.c $
|
+------------------------------------------------------------------------------+
|      (c) SMART ELECTRONIC DEVELOPMENT GMBH - all rights reserved             |
+-----------------------------------------------------------------------------*/

/* ========================================================================== */
/*      G L O B A L   C O M P I L E R   S W I T C H E S                       */
/* ========================================================================== */
/* DONT REMOVE: This file has to be included to all ADLATUS project files     */
/*               and has to be the first include!                             */
#include "ADLATUS_CompilerSwitches_cdf.h"
/* = END:DONT REMOVE ======================================================== */

/* ========================================================================== */
/* (1)  I N C L U D E S                                                       */
/* -------------------------------------------------------------------------- */
/*      C O N S T A N T   D E F I N I T I O N   F I L E S  (CDF)              */
/* ========================================================================== */
#include "ADLATUS_Global_cdf.h"              /* global defines                */
#include "ADLATUS_Hal_cdf.h"
#include "ADLATUS_ConfigSystem_cdf.h"
#include "ADLATUS_ValHandler_cdf.h"
#include "ADLATUS_CanHandler_cdf.h"

/* ========================================================================== */
/* (2)  I N C L U D E S                                                       */
/* -------------------------------------------------------------------------- */
/*      T Y P E   D E F I N I T I O N   F I L E S  (TDF)                      */
/* ========================================================================== */
#include "ADLATUS_Types_tdf.h"               /* must be the first TDF include */
#include "ADLATUS_Hal_tdf.h"
#include "ADLATUS_ValHandler_tdf.h"
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* Communication Buffer Test (e.g. CAN Message Object Test )                  */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#include "ADLATUS_CanHandler_tdf.h"


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
#define  d_CCval_StopScan                       (UBYTE) 0x00u
#define  d_CCval_ScanRunning                    (UBYTE) 0x01u
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


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
/* NO ENTRY                                                                   */

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

UBYTE FUN_CCval_CanMsgObjectTest_UB (VALCONTROL_ST  *Parameters )
{
  UBYTE t_Result_UB;

  ADL_HAL_VALIDATION_INIT
  ADL_HAL_VALIDATION_ENTRY( Parameters->StartAddress_PBUF )

  /*==========================================================================*/
  /* -1- check result                                                         */
  /*==========================================================================*/
  if ( d_CCVhnd_CheckForResult == (d_CCVhnd_CheckForResult & Parameters->Control_UB) )
  {
    t_Result_UB = FUN_CCcan_CheckMsgObjBuffer_UB();
    
    if ((d_CCVhnd_CheckOK | d_CCVhnd_Ready ) != t_Result_UB)
    {
      t_Result_UB = (d_CCVhnd_CheckNOK | d_CCVhnd_Ready );
    }

  }
  else
  {
    /*------------------------------------------------------------------------*/
    /* do no further calculation, just ckeck result next time                 */
    /*------------------------------------------------------------------------*/
    t_Result_UB = (d_CCVhnd_CheckNOK | d_CCVhnd_Ready);
  }

  ADL_HAL_VALIDATION_EXIT( Parameters->StartAddress_PBUF )
  return ( t_Result_UB );
} /*-+- END OF FUNCTION 'FUN_CCval_CanMsgObjectTest_UB' +-+-+-*/ /*l_int !e818 */

/* ========================================================================== */
/*      E N D   O F   F I L E                                                 */
/* ========================================================================== */
