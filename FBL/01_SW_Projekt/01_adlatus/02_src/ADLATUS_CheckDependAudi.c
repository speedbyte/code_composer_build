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
|  Filename:  ADLATUS_CheckDependAudi.c                                        |
|                                                                              |
|  Comment:   This file an interface for customer specific dependency checks.  |
|                                                                              |
+------------------------------------------------------------------------------+
| R E V I S I O N   H I S T O R Y                                              |
+------------------------------------------------------------------------------+
|
| Subversion Info:
| $Id: ADLATUS_CheckDependAudi.c 4396 2008-11-04 09:26:32Z maennel $
| $HeadURL: http://smartsvn/svn/adlatusrepository/auslieferungen/S1NN/086350-S1NN-ADLATUS-TMS470R-MucSuc/20081201_S1NN_TMS470R_BlfIso-Source_V1.00/01_SW_Projekt/01_adlatus/02_src/ADLATUS_CheckDependAudi.c $
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
#include "ADLATUS_Global_cdf.h"                /* must be the first include   */
#include "ADLATUS_ConfigSystem_cdf.h"
#include "ADLATUS_ConfigProject_cdf.h"


/* ========================================================================== */
/* (2)  I N C L U D E S                                                       */
/* -------------------------------------------------------------------------- */
/*      T Y P E   D E F I N I T I O N   F I L E S  (TDF)                      */
/* ========================================================================== */
#include "ADLATUS_Types_tdf.h"                 /* must be the first include   */
#include "ADLATUS_ConfigSystem_tdf.h"
#include "ADLATUS_ConfigProject_tdf.h"

#include "ADLATUS_NvmHandler_tdf.h"

#include "ADLATUS_CheckDependAudi_tdf.h"
/* ========================================================================== */
/* (3)  C O M P I L E R   S W I T C H   S E T T I N G S                       */
/* -------------------------------------------------------------------------- */
/*      O N L Y   F O R   T H I S   M O D U L                                 */
/* ========================================================================== */
/* Global Compiler switch settings are done at ADLATUS_CompilerSwitches_CDF.h */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* Local compiler switches:  NO ENTRY                                         */

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

/* ========================================================================== */
/* (6)  L O C A L  T Y P E   D E F I N E S                                    */
/* -------------------------------------------------------------------------- */
/*      O N L Y   F O R   T H I S   M O D U L                                 */
/* ========================================================================== */
/* NO ENTRIES                                                                 */

#define d_CCcd_ScanStop           (UBYTE) 0x00u
#define d_CCcd_ScanRun            (UBYTE) 0x01u

#define d_CCcd_ProgStateSWInValid (UBYTE) 0x01u
#define d_CCcd_ProgStateSWValid   (UBYTE) 0x00u

/* ========================================================================== */
/* (7)  L O C A L   V A R I A B L E S  / C O N S T A N T S                    */
/* -------------------------------------------------------------------------- */
/*      O N L Y   F O R   T H I S   M O D U L                                 */
/* ========================================================================== */
/* NO ENTRIES                                                                 */

/* ========================================================================== */
/* (8)  L O C A L   F U N C T I O N   P R O T O T Y P E S                     */
/* -------------------------------------------------------------------------- */
/*      O N L Y   F O R   T H I S   M O D U L                                 */
/* ========================================================================== */
/* NO ENTRIES                                                                 */

/* ========================================================================== */
/* (9)  G L O B A L   V A R I A B L E S                                       */
/* -------------------------------------------------------------------------- */
/*      D E F I N E D   W I T H I N   T H I S   M O D U L                     */
/* ========================================================================== */

/* ========================================================================== */
/* (10) F U N C T I O N   D E F I N I T I O N S                               */
/* -------------------------------------------------------------------------- */
/*      F O R   T H I S   M O D U L                                           */
/* ========================================================================== */

 
/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CCcd_CheckDependAudi_UB                                 |
|                                                                              |
|  Prototype at:   ADLATUS_CheckDependAudi_tdf.h                               |
|                                                                              |
|  Description:    This routine includes the interface for the dependency      |
|                  check.                                                      |
|                                                                              |
|  Parameter:      DOWNLOADMAP_ST* DownloadMap_PST location of download table  |
|                                                                              |
|  Return Value:   UBYTE         d_CCglo_ResponseIndex00  .. dependency OK     |
|                                                                              |
+-----------------------------------------------------------------------------*/
UBYTE  FUN_CCcd_CheckDependAudi_UB (const DOWNLOADMAP_ST* DownloadMap_PST)
{

  UBYTE  t_TableIndex_UB;
  UBYTE  t_EepromTableIndex_UB;
  UBYTE  t_FSWValid_UB;

  UBYTE  t_ReturnValue_UB;
  UBYTE  t_ScanState_UB;

  UBYTE t_Result_UB;

  UBYTE  t_EepromHelp_UB[1];

  /*--------------------------------------------------------------------------*/
  /* Init internal variables                                                  */
  /*--------------------------------------------------------------------------*/
  t_Result_UB = d_CCglo_Ready;
  /*--------------------------------------------------------------------*/
  /* set return value - will be overwritten by an error or other state  */
  /*--------------------------------------------------------------------*/
  /*  points to d_CCkwpaudi_correctResult                               */
  t_Result_UB = d_CCglo_ResponseIndex00;

  /*======================================================================*/
  /* NOTE:                                                                */
  /*       the whole applicationsoftware is only valid if each            */
  /*       softwareblock is valid itself.                                 */
  /*       The Adlatus software block is neglected in this check.         */
  /*======================================================================*/


  /*----------------------------------------------------------------------*/
  /* init index variable, leave out the adlatus software block            */
  /*----------------------------------------------------------------------*/
  t_TableIndex_UB = (UBYTE) 0x01u;
  t_ScanState_UB = d_CCcd_ScanRun;
  
  t_FSWValid_UB = d_CCglo_NoError;

  /*======================================================================*/
  /* -1- search downloadmap entry in the table                            */
  /*======================================================================*/
  while ( (d_CCcd_ScanRun == t_ScanState_UB )  &&
      ( d_CCglo_LastEntry !=                 \
       (d_CCglo_EntryMask & DownloadMap_PST[t_TableIndex_UB].MemoryInfo_UW )))
  {
    /*====================================================================*/
    /* -2- table entry is a flash entry                                   */
    /*====================================================================*/
    if ( (d_CCglo_FlashMemory == \
            (d_CCglo_MemoryTypeMask & DownloadMap_PST[t_TableIndex_UB].MemoryInfo_UW )) ||
         (d_CCglo_EepromMemory == \
            (d_CCglo_MemoryTypeMask & DownloadMap_PST[t_TableIndex_UB].MemoryInfo_UW ))   )
    {

        /*----------------------------------------------------------*/
        /* get the name (= index of the array                       */
        /* c_CCconprj_LogBlockEEPROM_AUB) of the block              */
        /*----------------------------------------------------------*/
        t_EepromTableIndex_UB = DownloadMap_PST[t_TableIndex_UB].Name_UB;
          
       /*------------------------------------------------------------------*/
       /* read the programming state of the software block                 */
       /*------------------------------------------------------------------*/
       t_ReturnValue_UB = FUN_CCnvm_NvmAccess_UB ( \
             c_CCconprj_LogBlockEEPROM_AUB[t_EepromTableIndex_UB][d_CCconprj_SWProgStateIndex],  \
            &t_EepromHelp_UB[0]);

       /*==================================================================*/
       /* -3- the nvm access was successful                                */
       /*==================================================================*/
       if (d_CCglo_NoError == t_ReturnValue_UB)
       {
          /*================================================================*/
          /* -4- the software block is not valid                            */
          /*================================================================*/
          if (d_CCcd_ProgStateSWValid != t_EepromHelp_UB[0])
          {
              /*--------------------------------------------------------------*/
              /* remember the invalid sw block                                */
              /*--------------------------------------------------------------*/
              t_FSWValid_UB = d_CCglo_Error;

              /*--------------------------------------------------------------*/
              /* stop scan                                                    */
              /*--------------------------------------------------------------*/
              t_ScanState_UB = d_CCcd_ScanStop;
  
            } /* -4- END OF if(..) -------------------------------------------*/
        }
        /*==================================================================*/
        /* -3- the nvm access was not successful                            */
        /*==================================================================*/
        else
        {
          /*----------------------------------------------------------------*/
          /* stop scan                                                      */
          /*----------------------------------------------------------------*/
          t_ScanState_UB = d_CCcd_ScanStop;

          /*----------------------------------------------------------------*/
          /* set return value - responseIndex 03                            */
          /*----------------------------------------------------------------*/
          /* points to d_CCkwpbase_NRCConditionsNotCorrect 0x7F .. 0x22     */

          t_Result_UB = d_CCglo_ResponseIndex03;

        } /*----3- END OF else (if(..)) ------------------------------------*/
    } /*------2- END OF if(..) -------------------------------------------*/

    /*--------------------------------------------------------------------*/
    /* Check next entry                                                   */
    /*--------------------------------------------------------------------*/
    t_TableIndex_UB += 0x01;

  } /*--------1- END OF while(..) ----------------------------------------*/

  /*======================================================================*/
  /* -3- one block of the application is invalid                          */
  /*======================================================================*/
  if (d_CCglo_NoError != t_FSWValid_UB) 
  {
        /*------------------------------------------------------------------*/
        /* set return value - will be overwritten by an error or other state*/
        /*------------------------------------------------------------------*/
        /* points to d_CCkwpaudi_incorrectResult                            */

        t_Result_UB = d_CCglo_ResponseIndex02;

  } /* -----4- END OF else (if(..)) ------------------------------------*/

  return t_Result_UB;

} /*-+- END OF FUNCTION 'FUN_CCcd_CheckDependAudi_UB' +-+-+-+-+-+-+-+-+-+-+-+-*/




/* ========================================================================== */
/*      E N D   O F   F I L E                                                 */
/* ========================================================================== */
