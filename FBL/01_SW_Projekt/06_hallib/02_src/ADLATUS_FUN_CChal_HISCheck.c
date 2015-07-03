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
|  Filename:  see Windows Explorer !                                           |
|   		                             				                       |
|  Comment:   This file implements the HIS driver check                        |
|   		                             			                           |
+------------------------------------------------------------------------------+
| A U T H O R   I D E N T I T Y                                                |
+------------------------------------------------------------------------------+
|                                                                              |
| Initials     Name                      Company                               |
| --------     ---------------------     ----------------------------------    |
|   MB          Michael Becker            SMART Electronic Development GmbH    |
|   JS          Jürgen Schade             SMART Electronic Development GmbH    |
|   JW          Jürgen Werner             SMART Electronic Development GmbH    |
+------------------------------------------------------------------------------+
| R E V I S I O N   H I S T O R Y                                              |
+------------------------------------------------------------------------------+
|
| Subversion Info:
| $Id: ADLATUS_FUN_CChal_HISCheck.c 4477 2008-11-27 12:11:59Z maennel $
| $HeadURL: http://smartsvn/svn/adlatusrepository/auslieferungen/S1NN/086350-S1NN-ADLATUS-TMS470R-MucSuc/20081201_S1NN_TMS470R_BlfIso-Source_V1.00/01_SW_Projekt/06_hallib/02_src/ADLATUS_FUN_CChal_HISCheck.c $
|
+------------------------------------------------------------------------------+
|      (c) SMART ELECTRONIC DEVELOPMENT GMBH - all rights reserved             |
+-----------------------------------------------------------------------------*/

/* ========================================================================== */
/* (1)  I N C L U D E S                                                       */
/* -------------------------------------------------------------------------- */
/*      C O N S T A N T   D E F I N I T I O N   F I L E S                     */
/* ========================================================================== */
#include "ADLATUS_Types_tdf.h"                 /* must be the first include */
#include "ADLATUS_Global_cdf.h"

#include "ADLATUS_HISflash.h"
#include "ADLATUS_Hal_tdf.h"

#include "ADLATUS_Adr_Info.h"

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
/* ========================================================================== */
/* (9)  G L O B A L   V A R I A B L E S                                       */
/* -------------------------------------------------------------------------- */
/*      D E F I N E D   W I T H I N   T H I S   M O D U L                     */
/* ========================================================================== */
tFlashParam FlashParam;
extern HALCONTROL_ST   t_CChal_HalCtrl_ST;

/* ========================================================================== */
/* (10) F U N C T I O N   D E F I N I T I O N S                               */
/* -------------------------------------------------------------------------- */
/*      F O R   T H I S   M O D U L                                           */
/* ========================================================================== */

/*---------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                 |
+----------------------------------------------------------------------------+
|                                                                            |
|  Function Name:  FUN_CChal_HISDriverCheck_UB                               |
|                                                                            |
|  Prototype at:   ADLATUS_HISflash.h                                        |
|                                                                            |
|  Description:    This routine checks the correct version of the            |
|                  HIS-FlashDriver.                                          |
|                                                                            |
|  Parameter:      --                                                        |
|                                                                            |
|  Return Value:   UBYTE  t_Result_UB                                        |
|                                                                            |
|                         R B E I  I I I I                                   |
|                         | | | |  | | | |                                   |
|                         | | | +--+-+-+-+---    RESPONSE INDEX              |
|                         | | +--------------    ERROR                       |
|                         | +----------------    BUSY                        |
|                         +------------------    READY                       |
|                                                                            |
+---------------------------------------------------------------------------*/
UBYTE FUN_CChal_HISDriverCheck_UB ( tHis_Header *t_ExpHisHeader_PST )
{
  UBYTE       t_Result_UB;
  tHis_Header *pHisHeader;

  t_Result_UB = d_CCglo_Ready;
  /* Init the Ptr to HIS-Driver Header                                        */
  pHisHeader = (tHis_Header*) d_CCadrinfo_FlashFctRamStartAdr;   /*l_int !e923 */

  FlashParam.Delaytime_UL = C_FlashDelay_UL;
  FlashParam.errorAddress = t_CChal_HalCtrl_ST.TimerTicksForOneMilliSecond_UL;
  /*==========================================================================*/
  /* -1- No HIS-Driver programmed                                             */
  /*==========================================================================*/
  if (((pHisHeader->ubInterfaceVersion == 0xFF) || (pHisHeader->ubInterfaceVersion == 0x00)) && \
      ((pHisHeader->ubMask             == 0xFF) || (pHisHeader->ubMask             == 0x00)) && \
      ((pHisHeader->ubMCU              == 0xFF) || (pHisHeader->ubMCU              == 0x00))    )
  {
    t_Result_UB = (d_CCglo_Error | 0x01);
  } /* -1- END OF if(..) -----------------------------------------------------*/

  /*==========================================================================*/
  /* -1- HIS-Driver is programmed                                             */
  /*==========================================================================*/
  else
  {
    /*========================================================================*/
    /* -2- HIS-Driver is not correct                                          */
    /*========================================================================*/
    /*==========================================================================*/
      /* -1- HIS-Driver is not correct                                            */
      /*==========================================================================*/
      if ( (pHisHeader->ubInterfaceVersion != t_ExpHisHeader_PST->ubInterfaceVersion) || \
           (pHisHeader->ubMask             != t_ExpHisHeader_PST->ubMask            ) || \
           (pHisHeader->ubMCU              != t_ExpHisHeader_PST->ubMCU             )    )
    {
      t_Result_UB = (d_CCglo_Error | 0x02);
    } /* -2- END OF if(..) ---------------------------------------------------*/
  } /* ---1- END OF else (if(..)) --------------------------------------------*/

  return t_Result_UB;
 } /*-+- END OF FUNCTION 'FUN_CChal_HISDriverCheck_UB' -+-+-+-+-+-+-+-+-+-+-*/
