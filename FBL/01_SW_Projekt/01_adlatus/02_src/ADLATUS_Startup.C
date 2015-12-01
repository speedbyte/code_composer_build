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
|  Filename:  ADLATUS_Startup.c                                                |
|                                                                              |
|  Comment:   This file includes the neccessary startup entry point and almost |
|             all system initialisation                                        |
|                                                                              |
+------------------------------------------------------------------------------+
| A U T H O R   I D E N T I T Y                                                |
+------------------------------------------------------------------------------+
|                                                                              |
| Initials     Name                      Company                               |
| --------     ---------------------     ----------------------------------    |
|   JW         Juergen Werner            SMART Electronic Development GmbH     |
|                                                                              |
+------------------------------------------------------------------------------+
| R E V I S I O N   H I S T O R Y                                              |
+------------------------------------------------------------------------------+
|
| Subversion Info:
| $Id: ADLATUS_Startup.C 5771 2010-07-06 06:49:42Z werner $
| $HeadURL: http://smartsvn/svn/adlatusrepository/auslieferungen/S1NN/S1NN-ADLATUS-TMS470R1SF55B-MucSuc/20100706_S1NN_TMS470S_BlfIso-Source_V1.00/01_SW_Projekt/01_adlatus/02_src/ADLATUS_Startup.C $
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

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


/* ========================================================================== */
/* (2)  I N C L U D E S                                                       */
/* -------------------------------------------------------------------------- */
/*      T Y P E   D E F I N I T I O N   F I L E S  (TDF)                      */
/* ========================================================================== */
#include "ADLATUS_Types_tdf.h"          /* 1st include: Type definitions      */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* Customer Specific Files:                                                   */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* #include "sys470.h" */
#include "startup.h"
#include "tms470regs.h"

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
/* bootblock stack (for supervisor mode) */

#define S_BBSuperStackSize      100 /* max. 127 */

#ifndef FLASHWS
#define FLASHWS 0x00        /* set to define Flash Wait States 				*/
#endif
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
/* cinit as defined by C++ 1.01 linker */
extern unsigned *__cinit__;                                      /*lint !e970 */ 

extern ULONG g_BBSuperStack_UL[S_BBSuperStackSize];

const ULONG c_BBSuperStackTop_UL= (ULONG)&g_BBSuperStack_UL[S_BBSuperStackSize]; /*l_int !e923 */

/* ========================================================================== */
/* (8)  L O C A L   F U N C T I O N   P R O T O T Y P E S                     */
/* -------------------------------------------------------------------------- */
/*      O N L Y   F O R   T H I S   M O D U L                                 */
/* ========================================================================== */
void c_int00 (void);


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

asm(" .text"); /*l_int !e950 */  

asm(" .global _BBSuperStack_"); /*l_int !e950 */  

asm("b_stack: .long _g_BBSuperStack_UL + 512"); /*l_int !e950 */  

/***************************************************************************/
/* the name c_int00 has a special meaning for the TMS470 compiler          */
/* DONT CHANGE IT!                                                         */
/***************************************************************************/
/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  c_int00                                                     |
|                                                                              |
|  Prototype at:   ADLATUS_Startup.c                                           |
|                                                                              |
|  Description:    Init almost all sysregs.                                    |
|                                                                              |
|  Parameter:       --                                                         |
|                                                                              |
|  Return Value:    --                                                         |
|                                                                              |
+-----------------------------------------------------------------------------*/
void c_int00(void)
{
   e_SARSYS_ST.ClkCntl_UW = S_CLKCNTL_UW;

   /* disable Pipeline Mode */
 
   /* setup PERIPHERAL chip selects SAR module                             */
   
   e_SARMMC_ST.pprot_UW        = S_PPROT_UW;
   e_SARMMC_ST.plr_UW          = S_PLR_UW;
   e_SARMMC_ST.pcr0_UN.pcr0_UW = S_PCR0_UW;		/*Enable Peripheral Control	*/

   e_SARDEC_ST.mfbahR0_UW = S_MFBAHR0_UW;      /* FLASH                    */
   e_SARDEC_ST.mfbalR0_UW = S_MFBALR0_UW;
   
   e_SARDEC_ST.mfbahR1_UW = S_MFBAHR1_UW;      /* FLASH                    */
   e_SARDEC_ST.mfbalR1_UW = S_MFBALR1_UW;
   
   e_SARDEC_ST.mfbahR2_UW = S_MFBAHR2_UW;      /* int SRAM block 0         */
   e_SARDEC_ST.mfbalR2_UW = S_MFBALR2_UW;
   
   e_SARDEC_ST.mfbahR3_UW = S_MFBAHR3_UW;      /* int SRAM block 1         */
   e_SARDEC_ST.mfbalR3_UW = S_MFBALR3_UW;
   
   e_SARDEC_ST.mfbahR4_UW = S_MFBAHR4_UW;      /* HET RAM                  */
   e_SARDEC_ST.mfbalR4_UW = S_MFBALR4_UW;
   
   e_SARDEC_ST.mcbahR0_UW = S_MCBAHR0_UW;      
   e_SARDEC_ST.mcbalR0_UW = S_MCBALR0_UW;
   
   e_SARDEC_ST.mcbahR1_UW = S_MCBAHR1_UW;      /* ext FLASH                */
   e_SARDEC_ST.mcbalR1_UW = S_MCBALR1_UW;
   
   e_SARDEC_ST.mcbahR2_UW = S_MCBAHR2_UW;
   e_SARDEC_ST.mcbalR2_UW = S_MCBALR2_UW;
   
   e_SARDEC_ST.mcbahR3_UW = S_MCBAHR3_UW;
   e_SARDEC_ST.mcbalR3_UW = S_MCBALR3_UW;
   
   e_SARDEC_ST.mcbahR4_UW = S_MCBAHR4_UW;
   e_SARDEC_ST.mcbalR4_UW = S_MCBALR4_UW;
   
   /* setup memory configuration                                           */
   e_SARMMC_ST.smcR0_UW        = S_SMCR0_UW;
   e_SARMMC_ST.smcR1_UW        = S_SMCR1_UW;
   e_SARMMC_ST.smcR2_UW        = S_SMCR2_UW;
   e_SARMMC_ST.smcR3_UW        = S_SMCR3_UW;
   e_SARMMC_ST.smcR4_UW        = S_SMCR4_UW;
   e_SARMMC_ST.smcR5_UW        = S_SMCR5_UW;
   e_SARMMC_ST.smcR6_UW        = S_SMCR6_UW;
   e_SARMMC_ST.smcR7_UW        = S_SMCR7_UW;
   e_SARMMC_ST.smcR8_UW        = S_SMCR8_UW;
   e_SARMMC_ST.smcR9_UW        = S_SMCR9_UW;
   e_SARMMC_ST.wcr0_UN.wcr0_UW = S_WCR0_UW;
   
   e_SARDEC_ST.mfbalR0_UW  |= 0x0100 ;

   * (volatile unsigned int *) 0xFFF7CC00 = 0x100;  /* enable 1 wait state and pipeline  */

   e_SARSYS_ST.GlbCtrl_UW = 0x0004;  	   /* CLK setup				   */
   e_SARSYS_ST.CramCR_UW  = S_CRAMCR_UW;
   e_SARSYS_ST.SysECR_UW  = S_SYSECR_UW; 
    
   /***********************************************************************/
   /* all interrupts are disabled */
   e_SARCIM_ST.Reqmask_UL = 0u;    
    
   /************************************************************************/
   /* --- STACKS --------------------------------------------------------- */
   /* set supervisor stack (INITIAL MODE AFTER RESET)                      */
   asm(" ldr sp,b_stack ");                                   /*l_int !e950 */

    /* no initalisation if start of table is -1 */
    if (*(unsigned *)&__cinit__ != 0xFFFFFFFFul)
    {
        unsigned *table  = (unsigned *)&__cinit__;
        unsigned  length = *table++;
    
      {
        while (length != 0u)
        {
            unsigned char *address = (unsigned char *)*table++;
            unsigned char *data    = (unsigned char *)table;

            while (length > 0u)
            {
                *address++ = *data++; length--;
            }
            /* realign cinit pointer to point to next entry */
            table  = (unsigned *)(((unsigned)data + 3u) & ~3u);
            length = *table++;
        }
      }
    } 
  /* exit startup and jump to ADLATUS main routine */
  /* -> SUPERVISOR MODE!!!                         */
  main();
  
} /* -+- END OF FUNCTION 'c_int00' -+-+-+-+-+-+-+-*/


/* ========================================================================== */
/*      E N D   O F   F I L E                                                 */
/* ========================================================================== */




