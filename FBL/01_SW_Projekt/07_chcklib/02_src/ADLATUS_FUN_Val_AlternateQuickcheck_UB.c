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
|  Filename:  ADLATUS_FUN_Val_AlternateQuickCheck_UB.c                         |
|                                                                              |
|  Comment:   This file includes a checksum calculations functions.            |
|                                                                              |
+------------------------------------------------------------------------------+
| R E V I S I O N   H I S T O R Y                                              |
+------------------------------------------------------------------------------+
|
| Subversion Info:
| $Id: ADLATUS_FUN_Val_AlternateQuickcheck_UB.c 4396 2008-11-04 09:26:32Z maennel $
| $HeadURL: http://smartsvn/svn/adlatusrepository/auslieferungen/S1NN/086350-S1NN-ADLATUS-TMS470R-MucSuc/20081201_S1NN_TMS470R_BlfIso-Source_V1.00/01_SW_Projekt/07_chcklib/02_src/ADLATUS_FUN_Val_AlternateQuickcheck_UB.c $
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
|  Function Name:  FUN_CCval_AlternateQuickCheck_UB                                     |
|                                                                              |
|  Prototype at:   ADLATUS_ValHandler_tdf.h                                    |
|                                                                              |
|  Description:    This function calculates a checksum by using the            |
|                  algorithm of xyz.                                           |
|                                                                              |
|  Parameter:      VALCONTROL_ST  *Parameters .. pointer to Validation Control |
|                                                                              |
|  Return Value:   UBYTE t_Result_UB .. 0x00 - No Error                        |
|                                       0x01 - Checksum Error                  |
|                                                                              |
+-----------------------------------------------------------------------------*/
UBYTE FUN_CCval_AlternateQuickCheck_UB (VALCONTROL_ST  *Parameters )
{  
  UBYTE             t_Result_UB;           /* Result of check                 */

  UBYTE             t_CheckLength_UB;
  
  VHND_PTR_BUF_UBYTE t_TempPtr_PUB;
  VHND_PTR_BUF_UWORD t_TempPtr_PUW;
  VHND_PTR_BUF_ULONG t_TempPtr_PUL;
  
  VHND_PTR_BUF_UBYTE t_TempPtr2_PUB;
  VHND_PTR_BUF_UWORD t_TempPtr2_PUW;
  VHND_PTR_BUF_ULONG t_TempPtr2_PUL;

  ADL_HAL_VALIDATION_INIT
  ADL_HAL_VALIDATION_ENTRY( Parameters->StartAddress_PBUF )

  t_CheckLength_UB = (d_CCglo_QuickCheckLengthMask & Parameters->CheckCondition_UW);

  /*==========================================================================*/
  /* -1- check result                                                         */
  /*==========================================================================*/
  if ( d_CCVhnd_CheckForResult == (d_CCVhnd_CheckForResult & Parameters->Control_UB) )
  {
    t_Result_UB = (d_CCVhnd_CheckOK | d_CCVhnd_Ready);
    /*====================================================================*/
    /*  -4- Select access width                                           */
    /*====================================================================*/
    switch (t_CheckLength_UB)
    {
      /*------------------------------------------------------------------*/
      /* QuickCheck length: 1 Byte                                        */
      /*------------------------------------------------------------------*/
    case d_CCglo_01Byte:
      /* Convert the address                                            */
      t_TempPtr_PUB =     Parameters->ChecksumAddress_PBUF;
      t_TempPtr2_PUB =   (Parameters->EndAddress_PBUF + 1);
      
      /*================================================================*/
      /*  -5- No FSW programmed                                         */
      /*================================================================*/
      if( (*t_TempPtr_PUB) != (*t_TempPtr2_PUB) ) 
      {
          t_Result_UB = (d_CCVhnd_CheckNOK | d_CCVhnd_Ready);
      }
      else
      {
        if( ((*t_TempPtr_PUB) == (UBYTE) 0x00u) || ((*t_TempPtr_PUB) == (UBYTE) 0xFFu) )
        {
          t_Result_UB = (d_CCVhnd_CheckNOK | d_CCVhnd_Ready);
        }
      }

      break;
      /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
      
      /*------------------------------------------------------------------*/
      /* QuickCheck length: 2 Byte                                        */
      /*------------------------------------------------------------------*/
    case d_CCglo_02Byte:
      /* Convert the address                                            */
      t_TempPtr_PUW = \
        (VHND_PTR_BUF_UWORD) Parameters->ChecksumAddress_PBUF;
      t_TempPtr2_PUW = \
        (VHND_PTR_BUF_UWORD) (Parameters->EndAddress_PBUF+1);
      
      /*================================================================*/
      /*  -5- No FSW programmed                                         */
      /*================================================================*/
      if( (*t_TempPtr_PUW) != (*t_TempPtr2_PUW) ) 
      {
          t_Result_UB = (d_CCVhnd_CheckNOK | d_CCVhnd_Ready);
      }
      else
      {
        if( ((*t_TempPtr_PUW) == 0x0000u) || ((*t_TempPtr_PUW) == 0xFFFFu) )
        {
          t_Result_UB = (d_CCVhnd_CheckNOK | d_CCVhnd_Ready);
        }
      }
      break;
      /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
      
      /*------------------------------------------------------------------*/
      /* QuickCheck length: 4 Byte                                        */
      /*------------------------------------------------------------------*/
    case d_CCglo_04Byte:
      t_TempPtr_PUL = \
        (VHND_PTR_BUF_ULONG) Parameters->ChecksumAddress_PBUF;
      t_TempPtr2_PUL = \
        (VHND_PTR_BUF_ULONG) (Parameters->EndAddress_PBUF+1);
            
      /*================================================================*/
      /*  -5- No FSW programmed                                         */
      /*================================================================*/
      if( (*t_TempPtr_PUL) != (*t_TempPtr2_PUL) ) 
      {
          t_Result_UB = (d_CCVhnd_CheckNOK | d_CCVhnd_Ready);
      }
      else
      {
        if( ((*t_TempPtr_PUL) == 0x00000000ul) || ((*t_TempPtr_PUL) == 0xFFFFFFFFul) )
        {
          t_Result_UB = (d_CCVhnd_CheckNOK | d_CCVhnd_Ready);
        }
      }
      break;
      /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
      
      /*------------------------------------------------------------------*/
      /* DEFAULT                                                          */
      /*------------------------------------------------------------------*/
    default:
      /* Incorrect conditions */
      t_Result_UB = (d_CCVhnd_Error);
      break;
      /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
    }
    
  }
  else
  {
    /*------------------------------------------------------------------------*/
    /* do no further calculation, just ckeck result next time                 */
    /*------------------------------------------------------------------------*/
    t_Result_UB = (d_CCVhnd_CheckOK);
  }
  
  ADL_HAL_VALIDATION_EXIT( Parameters->StartAddress_PBUF )
  return ( t_Result_UB );
  
} /*-+- END OF FUNCTION 'FUN_CCval_AlternateQuickCheck_UB' -+-*/ /*l_int !e818 */


/* ========================================================================== */
/*      E N D   O F   F I L E                                                 */
/* ========================================================================== */
