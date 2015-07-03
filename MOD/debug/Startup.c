/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     Startup.c
 *-----------------------------------------------------------------------------
 * Module Name:   Startup
 *-----------------------------------------------------------------------------
 * Description:   Startup initialization for TMS470VFxxxx
 *-----------------------------------------------------------------------------
 * $Date: 2010-09-29 13:58:03 +0200 (Mi, 29 Sep 2010) $
 * $Rev: 15443 $
 * $Author: agrawal $
 * $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/SYS_MAIN/trunk/src/Startup.c $
 *-----------------------------------------------------------------------------
 */
/******************************************************************************/
#include "tms470r1.h"
#include "SYS_MAIN.h"
/*--------------------------------------------------------------------------*/

#ifndef FLASHWS
#define FLASHWS 0x11        /* set to define Flash Wait States 				*/
#endif

#ifndef FLASHPM
#define FLASHPM 1           /* set to 1 to enable pipeline mode 			*/
#endif


/*--------------------------------------------------------------------------*/
/* extern reference to cinit section                                        */

extern unsigned *__cinit__; 
extern unsigned *__pinit__;

#define FWPROGRAM ((FLASH_PST)(TMS470R1_BASEADDRESS_FLASHMEM))

/*--------------------------------------------------------------------------*/
/* jump to _c_int00 after reset and power-up                                */

asm("   .sect .reset");
asm("_resetISR: .global     _resetISR");
asm("	b _c_int00");


/* the name c_int00 has a special meaning for the TMS470 compiler           */
/* DONT CHANGE IT!                                                          */

void c_int00()
{
#if (defined(PROCESSOR_TMS470R1VF45AA) || defined(PROCESSOR_TMS470R1VF55BA))
    /* --- ENABLE FLASH WRAPPER ACCESS ------------------------------------ */
    stSys.GlbCtrl_UW     |= 0x0010;

    /* --- FLASH MODULE SETUP --------------------------------------------- */
    /* --- FOR FURTHER INFORMATION PLEASE REFER TO THE FLASH MODULE
    	   REFERENCE GUIDE : SPNU213x.PDF --------------------------------- */

    /* --- SET VREAD to 5V ------------------------------------------------ */
    FWPROGRAM->FMTCR  = 0x2FC0;
    FWPROGRAM->FMPTR4 = 0xA000;
    FWPROGRAM->FMTCR  = 0x03C0;

    /* --- SET FLASH WAIT STATES ------------------------------------------ */
    FWPROGRAM->FMMAC2 = 0xFFF8;                  /* bank0                   */
    FWPROGRAM->FMBAC1 = 0x00FF;                  /*     active              */
    FWPROGRAM->FMBAC2 = 0x7F00 | FLASHWS;        /*     wait states         */

    FWPROGRAM->FMMAC2 = 0xFFF9;                  /* bank1                   */
    FWPROGRAM->FMBAC1 = 0x00FF;                  /*     active              */
    FWPROGRAM->FMBAC2 = 0x7F00 | FLASHWS;        /*     wait states         */

#if (defined(PROCESSOR_TMS470R1VF55BA))
    FWPROGRAM->FMMAC2 = 0xFFFA;                  /* bank2                   */
    FWPROGRAM->FMBAC1 = 0x00FF;                  /*     active              */
    FWPROGRAM->FMBAC2 = 0x7F00 | FLASHWS;        /*     wait states         */
#endif

    /* - ENABLE PIPELINE MODE --------------------------------------------- */
    FWPROGRAM->FMREGOPT = FLASHPM & 1;

    /* - DISABLE FLASH WRAPPER ACCESS ------------------------------------- */
    stSys.GlbCtrl_UW &= ~0x0010;
#endif

#if (defined(PROCESSOR_TMS470R1SF55BA))
	/* enable 1 wait statet an pipeline mode                                */
    /* REFERENCE GUIDE: TMS470R1SF F035 Flash Module spnu434.pdf            */
    * (volatile unsigned int *)TMS470R1_BASEADDRESS_FLASHMEM = 0x101;
    /*                                                           | |        */
    /*                                                           | |        */
    /*                                                  wait state |        */
    /*                                          enable pipeline mode        */
#endif
    /* --- SYSTEM MODULE SETUP -------------------------------------------- */
	/* --- FOR FURTHER INFORMATION PLEASE REFER TO THE SYSTEM MODULE
		   REFERENCE GUIDE : SPNU189x.PDF --------------------------------- */
    
    /* setup system registers SAR module                                    */

     stSys.GlbCtrl_UW = (REG_16)enSysMainSysClock60Mhz;

    /* --- FOR FURTHER INFORMATION PLEASE REFER TO THE ZPLL MODULE 
     	   REFERENCE GUIDE : SPNU212x.PDF --------------------------------- */
 	
 	stSys.ClkCntl_UW = 0x0060;		            /* CLKOUT as SYSCLK    	    */
    
    /* --- ADDRESS MAMAGER SETUP ------------------------------------------ */

    /* setup ROM/RAM chip selects                                           */

    /* nEMUCS/nCS0 is the external RAM (emulator) or internal FLASH/ROM 0   */
    stAddrMan.mfbahR0_UW = 0x0000;              /* address: 0x00000000      */
    stAddrMan.mfbalR0_UW = 0x00A0;              /* size:     512k           */
    
#if (defined(PROCESSOR_TMS470R1VF55BA) || defined(PROCESSOR_TMS470R1SF55BA))
    /* nEMUCS/nCS1 is the external RAM (emulator) or internal FLASH/ROM 1   */
    stAddrMan.mfbahR1_UW = 0x0008;              /* address: 0x00080000      */
    stAddrMan.mfbalR1_UW = 0x0090;              /* size:     256k           */
#endif
    
#if (defined(PROCESSOR_TMS470R1VF45AA) || defined(PROCESSOR_TMS470R1VF55BA))
    /* nEMUCS2 internal RAM block0                                          */
    stAddrMan.mfbahR2_UW = 0x0050;              /* address: 0x00500000      */
    stAddrMan.mfbalR2_UW = 0x0060;              /* size:      32k           */
#endif

#if (defined(PROCESSOR_TMS470R1VF55BA))
    /* nEMUCS3 internal RAM block1                                          */
    stAddrMan.mfbahR3_UW = 0x0050;              /* address: 0x00508000      */
    stAddrMan.mfbalR3_UW = 0x8050;              /* size:      16k           */
#endif

#if (defined(PROCESSOR_TMS470R1SF55BA))
    /* nEMUCS2 internal RAM block0                                          */
    stAddrMan.mfbahR2_UW = 0x0040;              /* address: 0x00400000      */
    stAddrMan.mfbalR2_UW = 0x0060;              /* size:      32k           */

    /* nEMUCS3 internal RAM block1                                          */
    stAddrMan.mfbahR3_UW = 0x0040;              /* address: 0x00408000      */
    stAddrMan.mfbalR3_UW = 0x8050;              /* size:      16k           */
#endif


    /* internal HET RAM                                                     */
    stAddrMan.mfbahR4_UW = 0x0080;              /* address: 0x00800000      */
    stAddrMan.mfbalR4_UW = 0x0020;              /* size:       2k           */

    /* --- MEMORY CONTROLLER SETUP ---------------------------------------- */

    /* setup peripheral chip selects                                        */

    /* HET RAM                                                              */
    stMmc.smcR1_UW        = 0x0072;             /* 7 wait states, 32 bit    */
    /* write control                                                        */
    stMmc.wcr0_UN.wcr0_UW = 0x0003;             /* no trailing wait states  */
    /* protected/user mode access                                           */
    stMmc.pprot_UW        = 0x0000;             /* user access on all       */
    /* peripheral location internal/external                                */
    stMmc.plr_UW          = 0x0000;             /* all internal             */
    /* peripheral clock/control                                             */
    /* !!!! Please do not use a odd number as a clock divider. 
        Please refer Silicon errate sheet SPNZ109A ( Match 2009 ) */
    stMmc.pcr0_UN.pcr0_UW = (REG_16)enSysMainPeriClock15Mhz;       // Divide by 4,
	
    /* finaly enable the CHIP SELECTS                                       */
    stAddrMan.mfbalR0_UW |= 0x0100;


    /* -------------------- WE ARE NOW IN SYSTEM MODE --------------------- */

    /* initialise the C global variables 									*/

    /* no initialisation if start of table is -1 							*/
    if ((unsigned *)&__cinit__ != (unsigned *)0xFFFFFFFF)
    {
        unsigned *table  = (unsigned *)&__cinit__;
        unsigned  length = *table++;
    
        while (length != 0)
        {
            unsigned char *address = (unsigned char *)*table++;
            unsigned char *data    = (unsigned char *)table;

            while (length > 0)
            {
                *address++ = *data++; length--;
            }
            /* realign cinit pointer to point to next entry 				*/
            table  = (unsigned *)(((unsigned)data + 3) & ~3);
            length = *table++;
        }
    }
    
    /* inialise contructors 												*/
    if ((unsigned *)&__pinit__ != (unsigned *)0xFFFFFFFF)
    {
        void (**p0)() = (void *)&__pinit__;
        void (*p)()   = *p0++;

        while (p != 0)
        {
            p();
            p = *p0++;
        }
    }

    /* -------------------- WE ARE NOW IN USER MODE ----------------------- */
    main();
}


