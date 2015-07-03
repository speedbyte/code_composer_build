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
|  Filename:  ADLATUS_DummyMain.c                                              |
|                                                                              |
|  Comment:   This file includes a dummy main function for creating the        |
|             flash container.                                                 |
|                                                                              |
+------------------------------------------------------------------------------+
| A U T H O R   I D E N T I T Y                                                |
+------------------------------------------------------------------------------+
|                                                                              |
| Initials     Name                      Company                               |
| --------     ---------------------     ----------------------------------    |
|   KA         Markus Karas              SMART Electronic Development GmbH     |
|                                                                              |
+------------------------------------------------------------------------------+
| R E V I S I O N   H I S T O R Y                                              |
+------------------------------------------------------------------------------+
|
| Subversion Info:
| $Id: ADLATUS_DummyMain.c 4396 2008-11-04 09:26:32Z maennel $
| $HeadURL: http://smartsvn/svn/adlatusrepository/auslieferungen/S1NN/086350-S1NN-ADLATUS-TMS470R-MucSuc/20090305_S1NN_TMS470R_ParrotDriver_Hotfix/01_SW_Projekt/11_parrot/02_src/ADLATUS_DummyMain.c $
|
+------------------------------------------------------------------------------+
|      (c) SMART ELECTRONIC DEVELOPMENT GMBH - all rights reserved             |
+-----------------------------------------------------------------------------*/

/* ========================================================================== */
/* (1)  I N C L U D E S                                                       */
/* -------------------------------------------------------------------------- */
/*      C O N S T A N T   D E F I N I T I O N   F I L E S  (CDF)              */
/* ========================================================================== */
#include "ADLATUS_Global_cdf.h"
#include "ADLATUS_Hal_cdf.h"
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


/* ========================================================================== */
/* (2)  I N C L U D E S                                                       */
/* -------------------------------------------------------------------------- */
/*      T Y P E   D E F I N I T I O N   F I L E S  (TDF)                      */
/* ========================================================================== */
#include "ADLATUS_Types_tdf.h"          /* 1st include: Type definitions      */
#include "ADLATUS_Hal_tdf.h"
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* Project Files                                                              */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#include "ADLATUS_HISflash.h"

#include "ADLATUS_Adr_Info.h"
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


/* ========================================================================== */
/* (7)  L O C A L   V A R I A B L E S  / C O N S T A N T S                    */
/* -------------------------------------------------------------------------- */
/*      O N L Y   F O R   T H I S   M O D U L                                 */
/* ========================================================================== */
tFlashParam FlashParam;

/* ========================================================================== */
/* (8)  L O C A L   F U N C T I O N   P R O T O T Y P E S                     */
/* -------------------------------------------------------------------------- */
/*      O N L Y   F O R   T H I S   M O D U L                                 */
/* ========================================================================== */
void main                             (void);

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
const ULONG c_CCconprj_ClockFrequency_UL = \
{
  /* internal CPU Frequency */ 24000000ul   /* [Hz]                               */
  /* Depends on the clock source and various ajustments of the differnt       */
  /* clocks. The provision of the timer source and the adjustments are made   */
  /* in the startup code.                                                     */
};

const ULONG c_CCconprj_CanClockFrequency_UL = \
{
  /* CAN module input Frequency   */ 24000000ul   /* [Hz]                     */
  /* Depends on the clock source and various ajustments of the differnt       */
  /* clocks.                                                                  */
};

const ULONG c_CCconprj_TimerFrequency_UL = \
{
  /* Timer module input Frequency */ 24000000ul   /* [Hz]                     */
  /* The Timer TAB0 is used. So the adjusted frequency is the input frequency */
  /* of TAB0 timer.                                                           */
  /* Depends on the clock source and various ajustments of the differnt       */
  /* clocks.                                                                  */
};
const UBYTE c_CCconprj_DisableReset_UB = d_CCglo_NoResetViaWdt;

/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  main                                                        |
|                                                                              |
|  Prototype at:   ADLATUS_DummyMain.c                                         |
|                                                                              |
|  Description:    Main-Routine of C-language.                                 |
|                                                                              |
|  Parameter:       --                                                         |
|                                                                              |
|  Return Value:    --                                                         |
|                                                                              |
+-----------------------------------------------------------------------------*/
void main (void)
{
  tHis_Header  *pHisHeader;
  void        (*_FlashInit)    (tFlashParam*);
  void         (*_FlashWrite)  (tFlashParam*);
  void         (*_FlashErase)  (tFlashParam*);
  void         (*_FlashDeinit) (tFlashParam*);

  /*------------------------------------------------------------*/
  /* Init the HIS Driver                                        */
  /*------------------------------------------------------------*/
  pHisHeader = (tHis_Header*) d_CCadrinfo_FlashFctRamStartAdr; /*l_int !e923 */
  _FlashInit = pHisHeader->pfnFlashInit;

  _FlashInit (&FlashParam);

  pHisHeader  = (tHis_Header*) d_CCadrinfo_FlashFctRamStartAdr; /*l_int !e923 */
  _FlashErase = pHisHeader->pfnFlashErase;

  _FlashErase(&FlashParam);

  pHisHeader  = (tHis_Header*) d_CCadrinfo_FlashFctRamStartAdr; /*l_int !e923 */
  _FlashWrite = pHisHeader->pfnFlashWrite;

  _FlashWrite (&FlashParam);

  /*------------------------------------------------------------------*/
  /* Deinit the HIS Flash Driver                                      */
  /*------------------------------------------------------------------*/
  pHisHeader = (tHis_Header*) d_CCadrinfo_FlashFctRamStartAdr; /*l_int !e923 */
  _FlashDeinit = pHisHeader->pfnFlashDeinit;

  _FlashDeinit (&FlashParam);

} /* -+- END OF FUNCTION 'main' -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/


/* ========================================================================== */
/*      E N D   O F   F I L E                                                 */
/* ========================================================================== */
