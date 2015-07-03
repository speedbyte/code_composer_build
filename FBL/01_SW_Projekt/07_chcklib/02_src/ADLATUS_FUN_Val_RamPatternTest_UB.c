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
|  Filename:  ADLATUS_FUN_Val_RamPatternTest_UB.c                              |
|                                                                              |
|  Comment:   This file includes a checksum calculations functions.            |
|                                                                              |
+------------------------------------------------------------------------------+
| R E V I S I O N   H I S T O R Y                                              |
+------------------------------------------------------------------------------+
|
| Subversion Info:
| $Id: ADLATUS_FUN_Val_RamPatternTest_UB.c 4396 2008-11-04 09:26:32Z maennel $
| $HeadURL: http://smartsvn/svn/adlatusrepository/auslieferungen/S1NN/086350-S1NN-ADLATUS-TMS470R-MucSuc/20081201_S1NN_TMS470R_BlfIso-Source_V1.00/01_SW_Projekt/07_chcklib/02_src/ADLATUS_FUN_Val_RamPatternTest_UB.c $
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

/* -------------------------------------------------------------------------- */
/*                                                                            */
/*      R A M   T E S T   C O N F I G U R A T I O N                           */
/*      --------------------------------------------------------------------- */
/*      RAM test pattern table                                                */
/*                                                                            */
/* -------------------------------------------------------------------------- */
const UWORD c_CCval_RamTestPattern_AUW[3u] = \
{
  /* Test Pattern 00 */ 0x55AAu,
  /* Test Pattern 01 */ 0xAA55u,
  /* Test Pattern 02 */ 0x0000u 
  /*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
  /* NOTE: If the check condition on the MemoryMapTable on ConfigProject.c    */
  /*       is cofigured to d_CCglo_InitRam, then the value of test pattern 02 */
  /*       will be stored as init variable to each RAM cell !!!               */
  /*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
}; /* -+- END OF TABLE -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/


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

/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CCval_RamPatternTest_UB                                 |
|                                                                              |
|  Prototype at:   ADLATUS_Validation_tdf.h                                    |
|                                                                              |
|  Description:    This function checks the RAM functionality (read/write)     |
|                  by writing different patterns into each RAM cell and        |
|                  reading them back and compare the two values.               |
|                                                                              |
|  Parameter:      VALCONTROL_ST  *Parameters .. control parameter struct      |
|                                                                              |
|  Return Value:   UBYTE t_Result_UB ..  d_CCVhnd_Ready .. its all fine        |
|                                        d_CCVhnd_Error .. something is wrong  |
|                                        d_CCVhnd_[N]OK .. result              |
|                                                                              |
+-----------------------------------------------------------------------------*/
UBYTE FUN_CCval_RamPatternTest_UB (VALCONTROL_ST  *Parameters )
{

  UBYTE          t_RamTestResult_UB;     /* (1) Result of RAM Test (--> MemMapEntry )*/
  UBYTE          t_ScanState_UB;         /* (2) Scanning state                       */
  UWORD          t_CheckCondition_UW;    /* (3) CheckCondition     (--> MemMapEntry )*/
  UWORD          t_TempRamValue_UW;      /*     Storrage variable for RAM value      */
  UBYTE          t_PatternIndex_UB;      /*     Index for Pattern array entry        */
  VHND_PTR_BUF_UWORD  volatile t_TempAddress_PUW;      /* (5) Temp Checkaddress                    */
  VHND_PTR_BUF_UWORD  t_StopAddress_PUW;      /* (6) Stop address       (--> MemMapEntry )*/

  ADL_HAL_VALIDATION_INIT
  ADL_HAL_VALIDATION_ENTRY( Parameters->StartAddress_PBUF )

  /*==========================================================================*/
  /* -1- check result                                                         */
  /*==========================================================================*/
  if ( d_CCVhnd_CheckForResult == (d_CCVhnd_CheckForResult & Parameters->Control_UB) )
  {
    /* return the result */
    if (d_CCVhnd_CheckNOK == (UBYTE)(Parameters->WorkingBuffer_UL & (ULONG)d_CCVhnd_CheckNOK)) 
    {
      t_RamTestResult_UB = (d_CCVhnd_CheckNOK | d_CCVhnd_Ready);
    }
    else 
    {
      t_RamTestResult_UB = (d_CCVhnd_CheckOK | d_CCVhnd_Ready);
    }
  }
  else 
  {
    /* Do the check -> */
  /*--------------------------------------------------------------------------*/
  /* Init local variables:                                                    */
  /*--------------------------------------------------------------------------*/
  t_TempAddress_PUW       = (VHND_PTR_BUF_UWORD)  Parameters->StartAddress_PBUF;
  t_StopAddress_PUW       = (VHND_PTR_BUF_UWORD)  Parameters->EndAddress_PBUF;
  t_RamTestResult_UB      =                  d_CCVhnd_Ready;
    t_ScanState_UB          =                  d_CCval_ScanRunning;
  t_CheckCondition_UW     =                  Parameters->CheckCondition_UW;
  t_TempRamValue_UW       = (UWORD)          0x0000u;
  t_PatternIndex_UB       = (UBYTE)          0x00u;

  /*==========================================================================*/
  /* -1- Check each RAM cell                                                  */
  /*==========================================================================*/
    while ( (d_CCval_ScanRunning == t_ScanState_UB    ) && 
            (t_TempAddress_PUW   <= t_StopAddress_PUW )    )    /*l_int !e946 */
  {
    /*------------------------------------------------------------------------*/
    /* Store the real RAM cell value temporary                                */
    /*------------------------------------------------------------------------*/
    t_TempRamValue_UW = *t_TempAddress_PUW;

    /*========================================================================*/
    /* -2- Check each RAM cell with a pattern                                 */
    /*========================================================================*/
    for ( t_PatternIndex_UB =  0x00;  
          t_PatternIndex_UB <= 0x02;  
          t_PatternIndex_UB ++       )
    {
      /*----------------------------------------------------------------------*/
      /* Write pattern into Ram cell                                          */
      /*----------------------------------------------------------------------*/
      *t_TempAddress_PUW = c_CCval_RamTestPattern_AUW[t_PatternIndex_UB];

      /*======================================================================*/
      /* -3- RAM cell and test pattern are not equal!!                        */
      /*======================================================================*/
      if ( *t_TempAddress_PUW != c_CCval_RamTestPattern_AUW[t_PatternIndex_UB])
      {
        /*--------------------------------------------------------------------*/
        /* Set Result value                                                   */
        /*--------------------------------------------------------------------*/

          /* Prepare return value for next call -> getting the results */
          Parameters->WorkingBuffer_UL = d_CCVhnd_CheckNOK;
          /* Set return value to stop state machine */
          t_RamTestResult_UB = (UBYTE) (d_CCVhnd_CheckNOK | d_CCVhnd_Ready);

        /*--------------------------------------------------------------------*/
        /* Stop current loop                                                  */
        /*--------------------------------------------------------------------*/
        t_PatternIndex_UB = 0x03;
        /*--------------------------------------------------------------------*/
        /* Stop test routine [--> while (..) ]                                */
        /*--------------------------------------------------------------------*/
        t_ScanState_UB = d_CCval_StopScan;
          
      } /* -3- END OF if(..) -------------------------------------------------*/
    } /* ---2- END OF for (..) -----------------------------------------------*/

    /*========================================================================*/
    /* -2- Write the original RAM value back to RAM cell                      */
    /*========================================================================*/
    if ((d_CCval_ScanRunning == t_ScanState_UB ) && \
        (d_CCglo_DontInitRam == (t_CheckCondition_UW & d_CCglo_RamConditionMask)) )
    {
      /*----------------------------------------------------------------------*/
      /* Store the original value back to RAM cell                            */
      /*----------------------------------------------------------------------*/
      *t_TempAddress_PUW = t_TempRamValue_UW;
    } /* -2- END OF if(..) ---------------------------------------------------*/

    /*------------------------------------------------------------------------*/
    /* Check next RAM cell                                                    */
    /*------------------------------------------------------------------------*/
    t_TempAddress_PUW ++;
 
  } /* ---1- END OF while(..) ------------------------------------------------*/
  }

  ADL_HAL_VALIDATION_EXIT( Parameters->StartAddress_PBUF )
  return (t_RamTestResult_UB);

} /*-+- END OF FUNCTION 'FUN_CCval_RamPatternTest_UB' +-+-+-+-+-+-+-+-+-+-+-+-*/


/* ========================================================================== */
/*      E N D   O F   F I L E                                                 */
/* ========================================================================== */
