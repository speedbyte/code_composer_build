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
|  Filename:  ADLATUS_FUN_Val_LinAdd8Acc8Bit_UB.c                              |
|                                                                              |
|  Comment:   This file includes a checksum calculations functions.            |
|                                                                              |
+------------------------------------------------------------------------------+
| R E V I S I O N   H I S T O R Y                                              |
+------------------------------------------------------------------------------+
|
| Subversion Info:
| $Id: ADLATUS_FUN_Val_LinAdd8Acc8Bit_UB.c 4396 2008-11-04 09:26:32Z maennel $
| $HeadURL: http://smartsvn/svn/adlatusrepository/auslieferungen/S1NN/086350-S1NN-ADLATUS-TMS470R-MucSuc/20081201_S1NN_TMS470R_BlfIso-Source_V1.00/01_SW_Projekt/07_chcklib/02_src/ADLATUS_FUN_Val_LinAdd8Acc8Bit_UB.c $
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
#include "ADLATUS_Hal_cdf.h"                 /* global defines                */
#include "ADLATUS_ValHandler_cdf.h"

/* ========================================================================== */
/* (2)  I N C L U D E S                                                       */
/* -------------------------------------------------------------------------- */
/*      T Y P E   D E F I N I T I O N   F I L E S  (TDF)                      */
/* ========================================================================== */
#include "ADLATUS_Types_tdf.h"               /* must be the first TDF include */
#include "ADLATUS_Hal_tdf.h"
#include "ADLATUS_ValHandler_tdf.h"


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

/* Implementing Validation Functions:
  ====================================

  Validation functions have basically two states, which can be determined by
  the flag d_CCVhnd_CheckForResult in the variable Control_UB. First they are 
  called a necessary number of times in calculation mode - once for each chunk 
  to calculate. The size is determined by the system depending on the time 
  consumption of the routine. After calculating along the requested Range the 
  funtion is called again to present the results. QuickCheck routines are only 
  called once - to present the result, because no calculation should be done.

  Calculation State:
  ==================
  Calculate the checksum for the requested memory area, it is possible to
  store a result for the next cycle in the working buffer of the control struct.

  Return Values:
  - d_CCVhnd_Ready     All calculatiopn was OK, no Errors. Function is called
                       again for the next chunk. Not setting this bit means an
                       Error occured (see d_CCVhnd_Error).

  - d_CCVhnd_Error     An Error occured - function is called again once to
                       present further results.

  - d_CCVhnd_[N]OK     Stops the calculation before finishing the whole address
                       space. No error conditions are set. Function is called
                       again to present the results.

  Result State:
  =============
  - d_CCVhnd_Error     An Error occured.

  - d_CCVhnd_[N]OK     Calculation result OK/NOK.


  *** ATTENTION! ***
  ==================
  Do not use constant values, which are placed in rom! CPUs with paged memory may
  switch the page to access all memory areas. This is done automatically by the
  System. If you need constants, place them to non paged areas using compiler
  pragmas or a similar mechanism.

  INTERN:
  Die ADLATUS CheckLIB muss im Fall von Paged Memory im Non-Paged-Bereich abgelegt
  werden. Das stellt sicher, dass die Konstanten der integrierten Validierungs-
  funktionen immer erreichbar sind. Beste Möglichkeit das zu erreichen ist das
  Linker-Command-File. Auch Pragmas an antsprechenden Stellen sind möglich.

*/

/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  ADLATUS_FUN_val_LinAdd8Acc8Bit_UB                           |
|                                                                              |
|  Prototype at:   ADLATUS_ValHandler_tdf.h                                    |
|                                                                              |
|  Description:    This function calculates a checksum by using the            |
|                  algorithm of xyz.                                           |
|                                                                              |
|  Parameter:      VALCONTROL_ST  *Parameters .. pointer to Validation Control |
|                                                                              |
|  Return Value:   UBYTE t_Result_UB ..  d_CCVhnd_Ready .. its all fine        |
|                                        d_CCVhnd_Error .. something is wrong  |
|                                        d_CCVhnd_[N]OK .. result              |
|                                                                              |
+-----------------------------------------------------------------------------*/
UBYTE FUN_CCval_LinAdd8Acc8Bit_UB (VALCONTROL_ST  *Parameters )
{

  UBYTE             t_Result_UB;           /* Result of check                 */

  VHND_PTR_BUF_UBYTE t_TempAddress_PUB;     /* temp address */
  VHND_PTR_BUF_UBYTE t_BufferAddress_PUB;     /* temp address */

  ADL_HAL_VALIDATION_INIT
  ADL_HAL_VALIDATION_ENTRY( Parameters->StartAddress_PBUF )
  t_BufferAddress_PUB = (VHND_PTR_BUF_UBYTE)&Parameters->WorkingBuffer_UL;

  /*--------------------------------------------------------------------------*/
  /* Init the calculation                                                     */
  /*--------------------------------------------------------------------------*/
  t_Result_UB            = d_CCVhnd_Ready;
  t_TempAddress_PUB       = Parameters->StartAddress_PBUF;

  /*==========================================================================*/
  /* -1- check result                                                         */
  /*==========================================================================*/
  if ( d_CCVhnd_CheckForResult == (d_CCVhnd_CheckForResult & Parameters->Control_UB) )
  {
    /* Checksum correct? */
    if ( (*t_BufferAddress_PUB) != (*Parameters->ChecksumAddress_PBUF)) 
    {
      t_Result_UB = (d_CCVhnd_CheckNOK | d_CCVhnd_Ready);
    }
    else 
    {
      t_Result_UB = (d_CCVhnd_CheckOK | d_CCVhnd_Ready);
    }
  }
  else 
  {
  /*==========================================================================*/
  /* -1- Calculate Checksum                                                   */
  /*==========================================================================*/
    while (t_TempAddress_PUB <= Parameters->EndAddress_PBUF)    /*l_int !e946 */
  {
    /* Do calculation here and store value in working buffer */
    /* Increment Address                                                      */
    *t_BufferAddress_PUB += *t_TempAddress_PUB;
    t_TempAddress_PUB ++;
  } /* ---1- END OF while (..) -----------------------------------------------*/
    t_Result_UB = (d_CCVhnd_Ready);
  }

  ADL_HAL_VALIDATION_EXIT( Parameters->StartAddress_PBUF )
  return ( t_Result_UB );

} /*-+- END OF FUNCTION 'ADLATUS_FUN_Val_LinAdd8Acc8Bit_UB' -+-+-+-+-+-+-+-+-+-+-*/


/* ========================================================================== */
/*      E N D   O F   F I L E                                                 */
/* ========================================================================== */
