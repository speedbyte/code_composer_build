/* ------------------------------------------------------------------------- */
/* sar.h : SAR Module Definitions                                            */
/*                                                                           */
/* Copyright (c) Texas Instruments 1997, All right reserved.                 */
/*                                                                           */
/* History:                                                                  */
/*    Rev 1.0  09.09.97  Trevor Jones       - tc-jones@ti.com                */
/*                       Gerhard Wenderlein - g-wenderlein@ti.com            */
/*                                                                           */
/*                       Definition of SAR register set                      */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*  Still missing here: SDP0, SDP1 and TDSU registers                   */
/*                                                                           */
/* ------------------------------------------------------------------------- */

#ifndef __SYS470_H__
#define __SYS470_H__


/* ------------------------------------------------------------------------- */
/* SARMMC_ST                                                                 */
/*      Definition of the Memory Control register map.                       */

typedef volatile struct
{
    unsigned int   : 16;
    unsigned short smcR0_UW;
    unsigned int   : 16;
    unsigned short smcR1_UW;
    unsigned int   : 16;
    unsigned short smcR2_UW;
    unsigned int   : 16;
    unsigned short smcR3_UW;
    unsigned int   : 16;
    unsigned short smcR4_UW;
    unsigned int   : 16;
    unsigned short smcR5_UW;
    unsigned int   : 16;
    unsigned short smcR6_UW;
    unsigned int   : 16;
    unsigned short smcR7_UW;
    unsigned int   : 16;
    unsigned short smcR8_UW;
    unsigned int   : 16;
    unsigned short smcR9_UW;

    unsigned int : 32;

    unsigned int : 16;
    union
    {
        unsigned short wcr0_UW;
        struct
        {
            unsigned int             : 14;
            unsigned int WTWSOvr_B1  : 1;
            unsigned int WBEnable_B1 : 1;
        } wcr0_ST;
    } wcr0_UN;

    unsigned int : 16;
    union
    {
        unsigned short pcr0_UW;
        struct
        {
            unsigned int            : 11;
            unsigned int ClkDiv_B4  : 4;
            unsigned int PEnable_B1 : 1;
        } pcr0_ST;
    } pcr0_UN;

    unsigned int   : 16;
    unsigned short plr_UW;
    unsigned int   : 16;
    unsigned short pprot_UW;

} SARMMC_ST;

extern SARMMC_ST e_SARMMC_ST;


/* ------------------------------------------------------------------------- */
/* SARDEC_ST                                                                 */
/*      Definition of the Address Manager register map.                      */

typedef volatile struct
{
    unsigned int   : 16;
    unsigned short mfbahR0_UW;
    unsigned int   : 16;
    unsigned short mfbalR0_UW;
    unsigned int   : 16;
    unsigned short mfbahR1_UW;
    unsigned int   : 16;
    unsigned short mfbalR1_UW;
    unsigned int   : 16;
    unsigned short mfbahR2_UW;
    unsigned int   : 16;
    unsigned short mfbalR2_UW;
    unsigned int   : 16;
    unsigned short mfbahR3_UW;
    unsigned int   : 16;
    unsigned short mfbalR3_UW;
    unsigned int   : 16;
    unsigned short mfbahR4_UW;
    unsigned int   : 16;
    unsigned short mfbalR4_UW;
    unsigned int   : 16;
    unsigned short mfbahR5_UW;
    unsigned int   : 16;
    unsigned short mfbalR5_UW;

    unsigned int   : 16;
    unsigned short mcbahR0_UW;
    unsigned int   : 16;
    unsigned short mcbalR0_UW;
    unsigned int   : 16;
    unsigned short mcbahR1_UW;
    unsigned int   : 16;
    unsigned short mcbalR1_UW;
    unsigned int   : 16;
    unsigned short mcbahR2_UW;
    unsigned int   : 16;
    unsigned short mcbalR2_UW;
    unsigned int   : 16;
    unsigned short mcbahR3_UW;
    unsigned int   : 16;
    unsigned short mcbalR3_UW;
    unsigned int   : 16;
    unsigned short mcbahR4_UW;
    unsigned int   : 16;
    unsigned short mcbalR4_UW;
    unsigned int   : 16;
    unsigned short mcbahR5_UW;
    unsigned int   : 16;
    unsigned short mcbalR5_UW;

} SARDEC_ST;

extern SARDEC_ST e_SARDEC_ST;


/* ------------------------------------------------------------------------- */
/* SARSYS_ST                                                                 */
/*      Definition of the System Module register map.                        */

typedef volatile struct
{
    unsigned int   : 16;   
    unsigned short ClkCntl_UW;
    unsigned int   : 32;
    unsigned int   : 16;
    unsigned short CramCR_UW;
    unsigned int   : 16;
    unsigned short GlbCtrl_UW;
    unsigned int   : 16;
    unsigned short SysECR_UW;
    unsigned int   : 16;
    unsigned short SysESR_UW;
    unsigned int   : 16;
    unsigned short AbrtESR_UW;
    unsigned int   : 16;
    unsigned short GlbStat_UW;
    unsigned int   : 16;
    unsigned short Dev_UW;
    unsigned int   : 32;
    unsigned int   : 16;
    unsigned short Ssif_UW;
    unsigned int   : 16;
    unsigned short Ssir_UW;
} SARSYS_ST;

extern SARSYS_ST e_SARSYS_ST;


/* ------------------------------------------------------------------------- */
/* SARCIM_ST                                                                 */
/*      Definition of the Central Interrupt Module register map.  (p.5-15)   */

typedef volatile struct
{
    unsigned int   : 24;   
    unsigned char  IrqIVec_UB;
    unsigned int   : 24;
    unsigned char  FiqIVec_UB;
    unsigned int   : 24;
    unsigned char  CimIVec_UB;

    unsigned int   Firqpr_UL;
    unsigned int   Intreq_UL;
    unsigned int   Reqmask_UL;
 
} SARCIM_ST;

extern SARCIM_ST e_SARCIM_ST;


/* ------------------------------------------------------------------------- */
/* SARRTI_ST																					  */
/*		  Definition of the Realtime Interrupt Module register map				  */

typedef volatile struct
{
	unsigned int cntr_UL;
	unsigned int pctl_UL;
	unsigned int cntl_UL;
	unsigned int wkey_UL;
	unsigned int cmp1_UL;
	unsigned int cmp2_UL;
	unsigned int cint_UL;
	unsigned int cnten_UL;
	
} SARRTI_ST;

extern SARRTI_ST e_SARRTI_ST;

#endif

/* ------------------------------------------------------------------------- */
