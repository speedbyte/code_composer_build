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
|  Filename:  ADLATUS_FUN_Global_CollectData.c                                 |
|                                                                              |
|  Comment:   This file includes a function for global use.                    |
|                                                                              |
+------------------------------------------------------------------------------+
| R E V I S I O N   H I S T O R Y                                              |
+------------------------------------------------------------------------------+
|
| Subversion Info:
| $Id: ADLATUS_FUN_Global_CollectData.c 4396 2008-11-04 09:26:32Z maennel $
| $HeadURL: http://smartsvn/svn/adlatusrepository/auslieferungen/S1NN/086350-S1NN-ADLATUS-TMS470R-MucSuc/20081201_S1NN_TMS470R_BlfIso-Source_V1.00/01_SW_Projekt/08_corelib/02_src/ADLATUS_FUN_Global_CollectData.c $
|
+------------------------------------------------------------------------------+
|      (c) SMART ELECTRONIC DEVELOPMENT GMBH - all rights reserved             |
+-----------------------------------------------------------------------------*/


/* ========================================================================== */
/* (1)  I N C L U D E S                                                       */
/* -------------------------------------------------------------------------- */
/*      C O N S T A N T   D E F I N I T I O N   F I L E S  (CDF)              */
/* ========================================================================== */
#include "ADLATUS_CompilerSwitches_cdf.h"    /* must be the first CDF-include */
#include "ADLATUS_Global_cdf.h"

/* ========================================================================== */
/* (2)  I N C L U D E S                                                       */
/* -------------------------------------------------------------------------- */
/*      T Y P E   D E F I N I T I O N   F I L E S  (TDF)                      */
/* ========================================================================== */
#include "ADLATUS_Types_tdf.h"                 /* must be the first include */
#include "ADLATUS_Global_tdf.h"
#include "ADLATUS_ConfigProject_cdf.h"
#include "ADLATUS_Timer_tdf.h"
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
UBYTE t_CC_CollectBuffer_BUF[2u * d_CCconprj_PageLength];
UWORD t_DataPointer_UW;
UWORD t_PagePointer_UW;
UWORD t_DataInBuffer_UW;

/* ========================================================================== */
/* (5)  L O C A L   D E F I N E S                                             */
/* -------------------------------------------------------------------------- */
/*      O N L Y   F O R   T H I S   M O D U L                                 */
/* ========================================================================== */
/* NO ENTRY                                                                   */

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


/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CCcry_CollectData_UB                                    |
|                                                                              |
|  Prototype at:   ADLATUS_Global_tdf.h                                        |
|                                                                              |
|  Description:    This routine inits the collect data variables.              |
|                                                                              |
|  Parameter:      -                                                           |
|                                                                              |
|  Return Value:   -                                                           |
|                                                                              |
+-----------------------------------------------------------------------------*/
void  FUN_CCglo_InitCollectData_V (void)
{
  /*--------------------------------------------------------------------------*/
  /* init the variables                                                       */
  /*--------------------------------------------------------------------------*/
  t_DataPointer_UW  = 0x00;
  t_PagePointer_UW  = 0x00;
  t_DataInBuffer_UW = 0x00;

} /*-+- END OF FUNCTION 'FUN_CCglo_InitCollectData_V' +-+-+-+-+-+-+-+-+-+-+-+-*/

/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CCglo_GetIntermediateBufferNumbers_V                    |
|                                                                              |
|  Prototype at:   ADLATUS_Global_tdf.h                                        |
|                                                                              |
|  Description:    This routine sends the actual numbers from                  |
|                  intermediate Buffer                                         |
|  Parameter:      -                                                           |
|                                                                              |
|  Return Value:   -                                                           |
|                                                                              |
+-----------------------------------------------------------------------------*/
UWORD FUN_CCglo_GetIntermediateBufferNumbers_V (void)
{
  /*--------------------------------------------------------------------------*/
  /* send numbers of intermediate Buffer                                      */
  /*--------------------------------------------------------------------------*/
  return t_DataInBuffer_UW;

} /*-+- END OF FUNCTION 'FUN_CCglo_GetIntermediateBufferNumbers_V' +-+-+-+-+-+*/


/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CCcry_CollectData_UB                                    |
|                                                                              |
|  Prototype at:   ADLATUS_Global_tdf.h                                        |
|                                                                              |
|  Description:    This routine collect data for a page.                       |
|                                                                              |
|  Parameter:      PTR_BUFFER_UB t_DataBuffer_PBUF .. Ptr input buffer         |
|                  PTR_BUFFER_UB t_DataPtr_PBUF .. Ptr to target buffer        |
|                                                                              |
|  Return Value:   UBYTE         d_CCglo_Ready  .. Encryption OK               |
|                                                                              |
+-----------------------------------------------------------------------------*/
UWORD  FUN_CCglo_CollectData_UB ( PTR_BUFFER_UB t_DataBuffer_PBUF, \
                                  PTR_BUFFER_UB t_DataPtr_PBUF  )
{
  UWORD t_Result_UW;
  UWORD t_Counter_UW;
  UWORD t_DataLength_UW;
  PTR_BUFFER_UB *t_TempDest_pul;

  /*--------------------------------------------------------------------------*/
  /* Set Pointer to addresses                                                 */
  /*--------------------------------------------------------------------------*/
  t_TempDest_pul  = (PTR_BUFFER_UB*) t_DataPtr_PBUF;
  t_DataLength_UW =  t_DataBuffer_PBUF[d_CCglo_DLC];

#ifdef cs_CCUDS_Audi
  for(t_Counter_UW = 2; t_Counter_UW<t_DataLength_UW; t_Counter_UW++)
#else
  for(t_Counter_UW = 1; t_Counter_UW<t_DataLength_UW; t_Counter_UW++)
#endif
  {
    t_CC_CollectBuffer_BUF[t_DataPointer_UW] = t_DataBuffer_PBUF[t_Counter_UW + (UWORD)d_CCglo_StartOfData];
    t_DataPointer_UW++;
    t_DataInBuffer_UW++;
    if (t_DataPointer_UW == (2u * d_CCconprj_PageLength))
    {
      t_DataPointer_UW = 0x00;
    }
    FUN_CCtim_SystemTimerCheck_V();
  }

  if (t_DataInBuffer_UW >= d_CCconprj_PageLength)
  {
    t_Result_UW        = d_CCconprj_PageLength;
    t_DataInBuffer_UW -= d_CCconprj_PageLength;

    *t_TempDest_pul = &t_CC_CollectBuffer_BUF[t_PagePointer_UW];

    if (t_PagePointer_UW == 0x00u)
    {
      t_PagePointer_UW =  d_CCconprj_PageLength;
    }
    else
    {
      t_PagePointer_UW =  0x00;
    }
  }
  else
  {
    *t_TempDest_pul = &t_CC_CollectBuffer_BUF[t_PagePointer_UW];
    t_Result_UW = 0x00;
  }

  return t_Result_UW;

} /*-+- END OF FUNCTION 'FUN_CCglo_CollectData_UB' -+-+-+-+-+-*/ /*l_int !e818 */

   
/* ========================================================================== */
/*      E N D   O F   F I L E                                                 */
/* ========================================================================== */
