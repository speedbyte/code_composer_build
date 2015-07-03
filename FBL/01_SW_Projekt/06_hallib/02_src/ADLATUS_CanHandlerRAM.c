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
|  Filename:  ADLATUS_CanHandler.c                                             |
|                                                                              |
|  Processor:  TMS470                                                          |
|  Compiler:   TMS470 C/C++ Compiler             Version 2.24                  |
|                                                                              |
+------------------------------------------------------------------------------+
| A U T H O R   I D E N T I T Y                                                |
+------------------------------------------------------------------------------+
|                                                                              |
| Initials     Name                      Company                               |
| --------     ---------------------     ----------------------------------    |
|   RS         Ralf Schmidgall           SMART Electronic Development GmbH     |
|   MB         Michael Becker            SMART Electronic Development GmbH     |
|                                                                              |
+------------------------------------------------------------------------------+
| R E V I S I O N   H I S T O R Y                                              |
+------------------------------------------------------------------------------+
|
| Subversion Info:
| $Id: ADLATUS_CanHandlerRAM.c 4396 2008-11-04 09:26:32Z maennel $
| $HeadURL: http://smartsvn/svn/adlatusrepository/auslieferungen/S1NN/086350-S1NN-ADLATUS-TMS470R-MucSuc/20081201_S1NN_TMS470R_BlfIso-Source_V1.00/01_SW_Projekt/06_hallib/02_src/ADLATUS_CanHandlerRAM.c $
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
#include "ADLATUS_Canhandler_cdf.h"
#include "ADLATUS_ConfigSystem_cdf.h"

/* ========================================================================== */
/* (2)  I N C L U D E S                                                       */
/* -------------------------------------------------------------------------- */
/*      T Y P E   D E F I N I T I O N   F I L E S  (TDF)                      */
/* ========================================================================== */
#include "ADLATUS_Types_tdf.h"                 /* must be the first include   */
#include "ADLATUS_Canhandler_tdf.h"
#include "ADLATUS_HAL_tdf.h"

#include "ADLATUS_ConfigSystem_tdf.h"

#include "tms470regs.h"
/* ========================================================================== */
/* (3)  C O M P I L E R   S W I T C H   S E T T I N G S                       */
/* -------------------------------------------------------------------------- */
/*      O N L Y   F O R   T H I S   M O D U L                                 */
/* ========================================================================== */
/* Global Compiler switch settings are done at ADLATUS_CompilerSwitches_CDF.h */
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

/* ========================================================================== */
/* (6)  L O C A L  T Y P E   D E F I N E S                                    */
/* -------------------------------------------------------------------------- */
/*      O N L Y   F O R   T H I S   M O D U L                                 */
/* ========================================================================== */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

/* ========================================================================== */
/* (7)  L O C A L   V A R I A B L E S  / C O N S T A N T S                    */
/* -------------------------------------------------------------------------- */
/*      O N L Y   F O R   T H I S   M O D U L                                 */
/* ========================================================================== */

/* -------------------------------------------------------------------------- */
/*      C A N                                                                 */
/*      --------------------------------------------------------------------- */
/*      CAN Control structure                                                 */
/*                                                                            */
/* -------------------------------------------------------------------------- */
/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CCcan_TxDataFromRam_V                                   |
|                                                                              |
|  Prototype at:   ADLATUS_CanHandler.c                                        |
|                                                                              |
|  Description:    HAL-Function CAN Transmitter. The functions acts out of RAM |
|                  and sends a formerly defined message. Is is used by the     |
|                  erase functionality and will be called by the ext watchdog  |
|                  function.                                                   |
|                                                                              |
|  Parameter:      -                                                           |
|  Return Value:   -                                                           |
|                                                                              |
+-----------------------------------------------------------------------------*/
#ifdef cs_CCUDS_Audi

void FUN_CCcan_TxDataFromRam_V(void)
{
  /* Set timer for endless loop protection                                */
  ULONG t_EndlessLoopProtection_UL = 0xFFFFFFFFul;
      
  UBYTE t_Mailbox_UB;

  /* if a mailbox for ram is defined - use it */
  if(d_CCcan_NoMbforRamDefined != CanCtrl_ST.MbUsedinRAM_UB)
  {
    /* get formerly stored mailbox number */
    t_Mailbox_UB = 0x00000001ul << CanCtrl_ST.MbUsedinRAM_UB;

    /* due to a bug in the TMS470 SCC, an incoming message may be */
    /* discarded if a transmit is started at the same time */
    while (e_SCC_ST.CANES_UN.CANES_ST.RM_B1) 
    {
      ;
    }

    /* wait for mailbox free (transmitter ready) */
    while ((e_SCC_ST.CANTRS_UL & (ULONG)t_Mailbox_UB) != 0ul)
    {
      ;
    }
   
    /*======================================================================*/
    /* -3- Wait during message is sending                                   */
    /*======================================================================*/
    while (((e_SCC_ST.CANTA_UL & (ULONG)t_Mailbox_UB) == 0ul) && 
               (0x0000ul != t_EndlessLoopProtection_UL))
    {
      /* Decrement protection counter                                       */
      t_EndlessLoopProtection_UL -= 0x0001ul;

    }
    
    /* Get out here, the world is waiting for you */
    e_SCC_ST.CANTA_UL  |= (ULONG)t_Mailbox_UB;       
    e_SCC_ST.CANTRS_UL |= (ULONG)t_Mailbox_UB;
   }
   return;
} /* END OF FUNCTION 'FUN_CCcan_TxDataFromRam_V' +-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/

#endif
/* ========================================================================== */
/*      E N D   O F   F I L E                                                 */
/* ========================================================================== */
