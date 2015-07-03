/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     tms470r1.h
*-----------------------------------------------------------------------------
* Module Name:    
*-----------------------------------------------------------------------------
* Description: This file has the structures of all the Peripherals in the TMS Processor.
*-----------------------------------------------------------------------------
* $Date: 2010-09-29 13:58:03 +0200 (Mi, 29 Sep 2010) $
* $Rev: 15443 $
* $Author: agrawal $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/SYS_MAIN/trunk/inc/tms470r1.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef TMS470R1_H_
#define TMS470R1_H_

/******************************************************************************/
/*  This file is part of the uVision/ARM development tools                    */
/*  Copyright KEIL ELEKTRONIK GmbH 2002-2006                                  */
/******************************************************************************/
/*                                                                            */
/*  TMS470.h:   Header file for Texas Instruments TMS470R1xxxx              */
/*                                                                            */
/******************************************************************************/

/******************************************************************************/
/* Abbreviations used in this file:                                           */
/* REG = Register                                                             */
/* INT = Interrupt                                                            */
/* REQ = Request                                                              */
/******************************************************************************/
/* Absolute addresses of register structures                                  */

typedef unsigned char      REG_8;
typedef unsigned short int REG_16;
typedef unsigned int       REG_32;
#define TMS470_ICLOCK  15000000
#define MPU_BASE       0xFFFFFE00       /* Memory Protection Unit address     */
#define SM_BASE        0xFFFFFD00       /* System Module address              */
#define IEM_BASE       0xFFFFFC00       /* Interrupt Expansion Module address */
#define MSM1_BASE      0xFFFFF700       /* Memory Security Module 1 address   */
#define MSM2_BASE      0xFFFFF600       /* Memory Security Module 2 address   */
#define HET_BASE       0xFFF7FC00       /* HET  address                       */
#define TMS470R1_BASEADDRESS_SPI1 	  (REG_32)0xFFF7F800       /* Ser Peripheral Interface 1 address */
#define TMS470R1_BASEADDRESS_RESERVED (REG_32)0xFFF7F600       /* NOT Defined in TMS470r1 uc                       */
/**
 \brief  Base Address of  SCI2 Interface
*/
#define TMS470R1_BASEADDRESS_SCI2 	  (REG_32)0xFFF7F500
/**
 \brief  Base Address of  SCI1 Interface
*/
#define TMS470R1_BASEADDRESS_SCI1 	  (REG_32)0xFFF7F400
#define TMS470R1_BASEADDRESS_ADC      (REG_32)0xFFF7F000       /* MIBADC address                     */
#define ECP_BASE       0xFFF7EF00       /* External Clock Prescale address    */
#define EBM_BASE       0xFFF7ED00       /* Expansion Bus Module address       */
#define TMS470R1_BASEADDRESS_GIO      (REG_32)0xFFF7EC00       /* GIO address                        */
#define HECC3_BASE     0xFFF7D000       /* High-End CAN3 Controller address   */
#define HECC2_BASE     0xFFF7EA00       /* High-End CAN2 Controller address   */
#define HECC1_BASE     0xFFF7E800       /* High-End CAN1 Controller address   */
#define HECC3_RAM_BASE 0xFFF7D200       /* High-End CAN3 RAM address          */
#define HECC2_RAM_BASE 0xFFF7E600       /* High-End CAN2 RAM address          */
#define HECC1_RAM_BASE 0xFFF7E400       /* High-End CAN1 RAM address          */
#define SCC2_BASE      0xFFF7E200       /* Standard CAN2 Controller address   */
#define SCC1_BASE      0xFFF7E000       /* Standard CAN1 Controller address   */
#define SCC2_RAM_BASE  0xFFF7DE00       /* Standard CAN2 RAM address          */
#define SCC1_RAM_BASE  0xFFF7DC00       /* Standard CAN1 RAM address          */
#define I2C4_BASE      0xFFF7DB00       /* Inter-Integrated Circuit 4 address */
#define I2C3_BASE      0xFFF7DA00       /* Inter-Integrated Circuit 3 address */
#define I2C2_BASE      0xFFF7D900       /* Inter-Integrated Circuit 2 address */
#define I2C1_BASE      0xFFF7D800       /* Inter-Integrated Circuit 1 address */
#define I2C5_BASE      0xFFF7D500       /* Inter-Integrated Circuit 5 address */
#define TMS470R1_BASEADDRESS_SPI2     (REG_32)0xFFF7D400       /* Ser Peripheral Interface 2 address */
#define TMS470R1_BASEADDRESS_SPI3     (REG_32)0xFFF7D500       /* Ser Peripheral Interface 3 address */
#define TMS470R1_BASEADDRESS_SPI4     (REG_32)0xFFF7D600       /* Ser Peripheral Interface 4 address */
#define TMS470R1_BASEADDRESS_SPI5     (REG_32)0xFFF7D700       /* Ser Peripheral Interface 5 address */
#define C2SIB_BASE     0xFFF7C800       /* CS2IB address                      */

#if (defined(PROCESSOR_TMS470R1VF45AA) || defined(PROCESSOR_TMS470R1VF55BA))
#define TMS470R1_BASEADDRESS_FLASHMEM         0xFFE88000       /* Flash Module address               */
#elif (defined(PROCESSOR_TMS470R1SF55BA))
#define TMS470R1_BASEADDRESS_FLASHMEM         0xFFF7CC00       /* Flash Module address               */
#endif

//#define TMS470R1_BASEADDRESS_CIM        (REG_32)0xFFFFFF20

// Global Structure Typedefs
/**
 \brief	This is a  register map for SCI peripheral in the form of a structure.
 */
typedef volatile struct _SCI_ST
{
    REG_32  : 24;
    union
    {
        REG_8 ccr_UB;
        struct
        {
            REG_32 Stop_B1       : 1;
            REG_32 Parity_B1     : 1;
            REG_32 ParityEN_B1   : 1;
            REG_32 Async_B1      : 1;
            REG_32 Mode_B1       : 1;
            REG_32 CharLength_B3 : 3;
        } ccr_ST;
    } ccr_UN;

    REG_32 : 24;
    union
    {
        REG_8 ctl1_UB;
        struct
        {
            REG_32             : 1;
            REG_32 RxDmaAll_B1 : 1;
            REG_32 RxDmaEN_B1  : 1;
            REG_32 Idle_B1     : 1;
            REG_32 Sleep_B1    : 1;
            REG_32 RxRdy_B1    : 1;
            REG_32 RxWake_B1   : 1;
            REG_32 RxEN_B1     : 1;
        } ctl1_ST;
    } ctl1_UN;

    REG_32 : 24;
    union
    {
        REG_8 ctl2_UB;
        struct
        {
            REG_32 Cont_B1     : 1;
            REG_32 LoopBack_B1 : 1;
            REG_32 TxDmaEN_B1  : 1;
            REG_32             : 1;
            REG_32 TxEmpty_B1  : 1;
            REG_32 TxRdy_B1    : 1;
            REG_32 TxWake_B1   : 1;
            REG_32 TxEN_B1     : 1;
        } ctl2_ST;
    } ctl2_UN;

    REG_32 : 24;
    union
    {
        REG_8 ctl3_UB;
        struct
        {
            REG_32 SwReset_B1     : 1;
            REG_32 PowerDown_B1   : 1;
            REG_32 Clock_B1       : 1;
            REG_32 RxActEN_B1     : 1;
            REG_32 TxActEN_B1     : 1;
            REG_32 WakeupIntEN_B1 : 1;
            REG_32 BrkdtIntEN_B1  : 1;
            REG_32 RxErrIntEN_B1  : 1;
        } ctl3_ST;
    } ctl3_UN;

    REG_32 : 24;
    union
    {
        REG_8 rxst_UB;
        struct
        {
            REG_32 Busy_B1   : 1;
            REG_32           : 1;
            REG_32 FE_B1     : 1;
            REG_32 OE_B1     : 1;
            REG_32 PE_B1     : 1;
            REG_32 Wakeup_B1 : 1;
            REG_32 Brkdt_B1  : 1;
            REG_32 RxErr_B1  : 1;
        } rxst_ST;
    } rxst_UN;

    REG_32 : 24;
    REG_8    HBaud_UB;  

    REG_32 : 24;
    REG_8    MBaud_UB;  

    REG_32 : 24;
    REG_8    LBaud_UB;  

    REG_32 : 24;
    REG_8    RxEmu_UB;  

    REG_32 : 24;
    REG_8    RxBuf_UB;  

    REG_32 : 24;
    REG_8    TxBuf_UB;  

    struct 
    {
        REG_32 : 24;
        union
        {
            REG_8 pc_UB;
            struct
            {
                REG_32            : 4;
                REG_32 DataIn_B1  : 1;
                REG_32 DataOut_B1 : 1;
                REG_32 Func_B1    : 1;
                REG_32 DataDir_B1 : 1;
            } pc_ST;
        } pc_UN;
    } pc_PST[3];
} SCI_ST, *SCI_PST;


/* ------------------------------------------------------------------------- */
/* GIO_ST                                                                    */
/*      Definition of the GIO register map.                                  */

typedef volatile struct _GIO_ST
{
    struct
    {
        REG_32         : 24;
        REG_32         : 7;
        REG_32 Pwdn_B1 : 1;
    } Pwdn_ST;

    REG_32 : 24;
    REG_8    Ena_UB;

    REG_32 : 24;
    REG_8    Pol_UB;

    REG_32 : 24;
    REG_8    Flg_UB;

    REG_32 : 24;
    REG_8    Pry_UB;

    REG_32 : 24;
    REG_8    OffA_UB;
    
    REG_32 : 24;
    REG_8    EmuA_UB;

    REG_32 : 24;
    REG_8    OffB_UB;

    REG_32 : 24;
    REG_8    EmuB_UB;

    struct
    {
        REG_32 Dir_UL;
        REG_32 Din_UL;
        REG_32 Dout_UL;
        REG_32 Dset_UL;
        REG_32 Dclr_UL;
    } Port_ST[8];

    REG_32 Reserved[143];

    union ecpctrl
    {
        REG_32 ECPCTRL_UL;
        struct
        {
            REG_32 : 16;
            REG_16    ECPCTRL_UW;

        }ECPCTRL_ST;

    } ECPCTRL_UN;

} GIO_ST, *GIO_PST;


/* ------------------------------------------------------------------------- */
/* SPI_ST                                                                    */
/*      Definition of the SPI register map.                                  */

typedef  volatile struct _SPI_ST
{
    union spi_ctrl1
    {
        REG_32 ctrl1_UL;
        struct 
        {
            REG_32              : 19;
            REG_32 TapSel_B8    : 8;
            REG_32 CharLen_B5   : 5;
        } ctrl1_ST;
    } ctrl1_UN;

    union
    {
        REG_32 ctrl2_UL;
        struct
        {
            REG_32              : 26;
            REG_32 ClkMod_B1    : 1;
            REG_32 SpiEN_B1     : 1;
            REG_32 Master_B1    : 1;
            REG_32 PowerDown_B1 : 1;
            REG_32 Polarity_B1  : 1;
            REG_32 Phase_B1     : 1;
        } ctrl2_ST;
    } ctrl2_UN;

    union
    {
        REG_32 ctrl3_UL;
        struct
        {
            REG_32               : 26;
            REG_32 TriStateEN_B1 : 1;
            REG_32 DmaReqEN_B1   : 1;
            REG_32 OvrnIntEN_B1  : 1;
            REG_32 RcvrOvrn_B1   : 1;
            REG_32 IntEN_B1      : 1;
            REG_32 IntFlag_B1    : 1;
        } ctrl3_ST;
    } ctrl3_UN;

    REG_32 : 16;
    REG_16    dat0_UW;
    REG_32 : 16;
    REG_16    dat1_UW;

    union spi_buf
    {
        REG_32 buf_UL;
        struct
        {
        	REG_32             : 14;
        	REG_32 RcvrOvrn_B1 : 1;
        	REG_32 IntFlag_B1  : 1;
        	REG_16    buf_UW;
        } buf_ST;
    } buf_UN;
    
    REG_32 : 16;
    REG_16    emu_UW;
    
    union
    {
        REG_32 pc1_UL;
        struct
        {
            REG_32              : 27;
            REG_32 SCSDir_B1    : 1;
            REG_32 SOMIDir_B1   : 1;
            REG_32 SIMODir_B1   : 1;
            REG_32 ClkDir_B1    : 1;
            REG_32 EnableDir_B1 : 1;
        } pc1_ST;
    } pc1_UN;
    
    union
    {
        REG_32 pc2_UL;
        struct
        {
            REG_32              : 27;
            REG_32 SCSDin_B1    : 1;
            REG_32 SOMIDin_B1   : 1;
            REG_32 SIMODin_B1   : 1;
            REG_32 ClkDin_B1    : 1;
            REG_32 EnableDin_B1 : 1;
        } pc2_ST;
    } pc2_UN;

    union
    {
        REG_32 pc3_UL;
        struct
        {
            REG_32               : 27;
            REG_32 SCSDout_B1    : 1;
            REG_32 SOMIDout_B1   : 1;
            REG_32 SIMODout_B1   : 1;
            REG_32 ClkDout_B1    : 1;
            REG_32 EnableDout_B1 : 1;
        } pc3_ST;
    } pc3_UN;

    union
    {
        REG_32 pc4_UL;
        struct
        {
            REG_32                  : 27;
            REG_32 SCSDoutSet_B1    : 1;
            REG_32 SOMIDoutSet_B1   : 1;
            REG_32 SIMODoutSet_B1   : 1;
            REG_32 ClkDoutSet_B1    : 1;
            REG_32 EnableDoutSet_B1 : 1;
        } pc4_ST;
    } pc4_UN;
    
    union
    {
        REG_32 pc5_UL;
        struct
        {
            REG_32                  : 27;
            REG_32 SCSDoutClr_B1    : 1;
            REG_32 SOMIDoutClr_B1   : 1;
            REG_32 SIMODoutClr_B1   : 1;
            REG_32 ClkDoutClr_B1    : 1;
            REG_32 EnableDoutClr_B1 : 1;
        } pc5_ST;
    } pc5_UN;

    union
    {
        REG_32 pc6_UL;
        struct
        {
            REG_32              : 27;
            REG_32 SCSFun_B1    : 1;
            REG_32 SOMIFun_B1   : 1;
            REG_32 SIMOFun_B1   : 1;
            REG_32 ClkFun_B1    : 1;
            REG_32 EnableFun_B1 : 1;
        } pc6_ST;
    } pc6_UN;

} SPI_ST, *SPI_PST;

/* ------------------------------------------------------------------------- */
/* ADC_ST ADC register map definition                                        */

typedef volatile struct
{ 
    /*------------------------------------------------------------*/
    union madcr1
    {
        REG_32 ADCR1_UL;		 
        struct
        {
            REG_32                 : 18; /* Address 0x00 */
            REG_32 CALEN_B1        : 1;
            REG_32 CALSTR_B1       : 1;
            REG_32 BRIDGEN_B1      : 1;
            REG_32 HILO_B1         : 1;
            REG_32 FSTEST_B1       : 1;
            REG_32 POWERDOWN_B1    : 1;
            REG_32                 : 2;
            REG_32 ADCENA_B1       : 1;
            REG_32 ACQ_B2          : 2;
            REG_32 PS_B3           : 3;
          
        } ADCR1_ST;
        struct
        {
            REG_32       : 16; 	
            REG_16 ADCR1_UW;
        } ADCR1W_ST;
    } ADCR1_UN;
    /*------------------------------------------------------------*/

    union madcr2
    {
        REG_32 ADCR2_UL;
        struct
        {
            REG_32                 : 20;  /* Address 0x04 */
            REG_32 EVMODE_B1       : 1;
            REG_32 FRZEV_B1        : 1;
            REG_32 ENAEVINT_B1     : 1;
            REG_32 EVEDGSEL_B1     : 1;
            REG_32                 : 2;
            REG_32 GP1MODE_B1      : 1;
            REG_32 FRZGP1_B1       : 1;
            REG_32 ENAGP1INT_B1    : 1;
            REG_32 GP2MODE_B1      : 1;
            REG_32 FRZGP2_B1       : 1;
            REG_32 ENAGP2INT_B1    : 1;
          
        } ADCR2_ST;
        struct
        {
            REG_32                 : 16;
            REG_16 ADCR2_UW;
        } ADCR2W_ST;
    } ADCR2_UN;

    /*------------------------------------------------------------*/
    union madsr
    {
        REG_32 ADSR_UL;
        struct
        {
            REG_32                 : 18;  /* Address 0x08 */
            REG_32 CONVGP1BUSY_B1  : 1;
            REG_32 CONVGP2BUSY_B1  : 1;
            REG_32 CONVEVBUSY_B1   : 1;
            REG_32 CONVGP1STOP_B1  : 1;
            REG_32 CONVGP2STOP_B1  : 1;
            REG_32 CONVEVSTOP_B1   : 1;
            REG_32                 : 5;
            REG_32 CONVGP1END_B1   : 1;
            REG_32 CONVGP2END_B1   : 1;
            REG_32 CONVEVEND_B1    : 1;
          
        } ADSR_ST;
        struct
        {
            REG_32                 : 16;
            REG_16 ADSR_UW;
        } ADSRW_ST;
                    
    } ADSR_UN;
    /*------------------------------------------------------------*/

    REG_32      : 16;		 		/* Address 0x0C */
    REG_16 ADEISR_UW;

    REG_32      : 16;				/* Address 0x10 */
    REG_16 ADISR1_UW;

    REG_32      : 16;				/* Address 0x14 */
    REG_16 ADISR2_UW;

    REG_32      : 16;				/* Address 0x18 */
    REG_16 ADCALR_UW;
//#if COMPATIBILITY_MODE    
    struct
    {
    	REG_32 ADDR_UL;
    	REG_32 ADEMDR_UL;
    } ADDRREG[16];
//#endif
#if BUFFERED_MODE
    /*------------------------------------------------------------*/
    REG_32      : 32;				/* Address 0x1C */
    struct
    {
        REG_32	  : 16;			/* Address 0x20 */ 
        REG_16 ADBUFE_UW;
    } ADBUFE_ST[8];
    
    struct
    {
    	REG_32	  : 16;			/* Address 0x40 */
    	REG_16 ADBUF1_UW;
    } ADBUF1_ST[8];
    
    struct
    {
    	REG_32 	  : 16;			/* Address 0x60 */
    	REG_16 ADBUF2_UW;
    } ADBUF2_ST[8];
    
	unsigned dummy1[4];
    /*------------------------------------------------------------*/
   	REG_32 	  : 16;				/* Address 0x90 */
    REG_16 ADEMBUFE_UW;
      
    REG_32	  : 16;				/* Address 0x94 */
    REG_16 ADEMBUF1_UW;

    REG_32	  : 16;				/* Address 0x98 */
    REG_16 ADEMBUF2_UW;
    /*------------------------------------------------------------*/
#endif
    REG_32      : 16;					/* Address 0x9C */
    REG_16 ADIN_UW;
    /*------------------------------------------------------------*/
    union madcpr
    {
        REG_32 ADCPR_UL;
        struct
        {
            REG_32           : 28;	/* Address 0xA0 */
            REG_32 EVTIN_B1  : 1;
            REG_32 EVTOUT_B1 : 1;
            REG_32           : 1;
            REG_32 EVTDIR_B1 : 1;
        }ADCPR_ST;

        struct
        {
            REG_32           : 16;	
            REG_16 ADCPR_UW;
        }ADCPRW_ST;

    } ADCPR_UN;
    /*------------------------------------------------------------*/
    REG_32 dummy2[3];
    
    /**************************************************************/		
    union madsampev
    {
        REG_32 ADSAMPEV_UL;
        struct
        {
            REG_32			: 16;		/* Address 0xB0 */
            REG_32 SEN_B1		:  1;
            REG_32 			:  7;
            REG_32 EVACQ_B8	:  8;
        } ADSAMPEV_ST;
		
    } ADSAMPEV_UN;   
    /*------------------------------------------------------------*/
    union madsamp1
    {
        REG_32 ADSAMP1_UL;
        struct
        {
            REG_32			: 24;		/* Address 0xB4 */
            REG_32 GP1ACQ_B8	:  8;
        } ADSAMP1_ST;
		
    } ADSAMP1_UN;   
    /*------------------------------------------------------------*/
    union madsamp2
    {
        REG_32 ADSAMP2_UL;
        struct
        {
            REG_32			: 24;		/* Address 0xB8 */
            REG_32 GP2ACQ_B8	:  8;
        } ADSAMP2_ST;
		
    } ADSAMP2_UN;  
    /*------------------------------------------------------------*/
    union madbcr1
    {
        REG_32 ADBCR1_UL;
        struct
        {
            REG_32 	  		: 16;			/* Address 0xBC */
            REG_32 BUFEN_B1	:  1;
            REG_32 BNDA_B7	:  7;
            REG_32 BNDB_B8	:  8;
        }ADBCR1_ST;
		
        struct
        {
            REG_32		: 16;
            REG_16 ADBCR1_UW ;
        }ADBCR1W_ST;

    } ADBCR1_UN;
    /*------------------------------------------------------------*/
    union madbcr2
    {
        REG_32 ADBCR2_UL;
        struct
        {
            REG_32 			: 29;			/* Address 0xC0 */
            REG_32 BNDEND_B3	:  3; 
        } ADBCR2_ST;
        struct
        {
            REG_32		: 16;
            REG_16 ADBCR2_UW ;
        }ADBCR2W_ST;
    } ADBCR2_UN;
    /*------------------------------------------------------------*/
    union madbcr3
    {
        REG_32 ADBCR3_UL;
        struct
        {
            REG_32 				: 16;			/* Address 0xC4 */
            REG_32 EVDMAEN_B1		:  1;
            REG_32 GP1DMAEN_B1	:  1;
            REG_32 GP2DMAEN_B1	:  1;
            REG_32 EVOVRINTEN_B1	:  1;
            REG_32 GP1OVRINTEN_B1	:  1;
            REG_32 GP2OVRINTEN_B1	:  1;
            REG_32 EVBUFINTEN_B1	:  1;
            REG_32 GP1BUFINTEN_B1	:  1;
            REG_32 GP2BUFINTEN_B1	:  1;
            REG_32 				:  3;
            REG_32 EV8BITEN_B1	:  1;
            REG_32 GP18BITEN_B1	:  1;
            REG_32 GP28BITEN_B1	:  1;
            REG_32 CHIDEN_B1		:  1;
        }ADBCR3_ST;
    	
        struct
        {
            REG_32 	: 16;
            REG_16 ADBCR3_UW;
        }ADBCR3W_ST;
    }ADBCR3_UN;
    /*------------------------------------------------------------*/
    union madbst
    {
        REG_32 ADBST_UL;
		
        struct
        {
            REG_32					: 21;		/* Address 0xC8 */
            REG_32 EVINTFLAG_B1		:  1;
            REG_32 GP1INTFLAG_B1		:  1;
            REG_32 GP2INTFLAG_B1		:  1;
            REG_32 					:  1;
            REG_32 EVOVERRUN_B1		:  1;
            REG_32 GP1OVERRUN_B1		:  1;
            REG_32 GP2OVERRUN_B1		:  1;
            REG_32					:  1;
            REG_32 EVEMPTY_B1			:  1;
            REG_32 GP1EMPTY_B1		:  1;
            REG_32 GP2EMPTY_B1		:  1;
        }ADBST_ST;
		
        struct
        {
			REG_32 		: 16;
			REG_16 ADBST_UW;
        }ADBSTW_ST;
    }ADBST_UN;
	
    /*------------------------------------------------------------*/
    union madthrev
    {
        REG_32 ADTHREV_UL;
        struct
        {
           REG_32 			: 24;			/* Address 0xCC*/
           REG_32 EVTHR_B8	:  8;
        }ADTHREV_ST;
		
    } ADTHREV_UN;  
    /*------------------------------------------------------------*/
    union madthrg1
    {
        REG_32 ADTHRG1_UL;
        struct
        { 
            REG_32 			: 24;			/* Address 0xD0 */
            REG_32 GP1THR_B8	:  8;
        }ADTHRG1_ST;
		
    } ADTHRG1_UN;   
    /*------------------------------------------------------------*/
    union madthrg2
    {
        REG_32 ADTHRG2_UL;
        struct
        {
            REG_32 			: 24;           /* Address 0xD4 */
            REG_32 GP2THR_B8	:  8;
        }ADTHRG2_ST;
		
    } ADTHRG2_UN;   
    /*------------------------------------------------------------*/
    
    
    union madevtsrc
    {
        REG_32 ADEVTSRC_UL;
        struct
        {
            REG_32 				: 16;		/* Address 0xD8 */
            REG_32				:  7;
            REG_32 GP1ENA_B1 		:  1;
            REG_32 GP1EDGSEL_B1	:  1;
            REG_32 				:  1;
            REG_32 GP1SRC_B2		:  2;
            REG_32 				:  2;
            REG_32 EVSRC_B2		:  2;
        }ADEVTSRC_ST;
		
        struct										
        {
            REG_32 				: 16;
            REG_16 ADEVTSRC_UW;
        }ADEVTSRCW_ST;
    }ADEVTSRC_UN;
    /*------------------------------------------------------------*/
	                                
} ADC_ST, *ADC_PST;


/*------------------------------------------------------------*
 * SYSTEM STRUCTURES
 */
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
/* ------------------------------------------------------------------------- */
/* SARMMC_ST                                                                 */
/*      Definition of the Memory Control register map.                       */

typedef volatile struct
{
    REG_32   : 16;
    REG_16 smcR0_UW;
    REG_32   : 16;
    REG_16 smcR1_UW;
    REG_32   : 16;
    REG_16 smcR2_UW;
    REG_32   : 16;
    REG_16 smcR3_UW;
    REG_32   : 16;
    REG_16 smcR4_UW;
    REG_32   : 16;
    REG_16 smcR5_UW;
    REG_32   : 16;
    REG_16 smcR6_UW;
    REG_32   : 16;
    REG_16 smcR7_UW;
    REG_32   : 16;
    REG_16 smcR8_UW;
    REG_32   : 16;
    REG_16 smcR9_UW;

    REG_32 : 32;

    REG_32 : 16;
    union
    {
        REG_16 wcr0_UW;
        struct
        {
            REG_32             : 14;
            REG_32 WTWSOvr_B1  : 1;
            REG_32 WBEnable_B1 : 1;
        } wcr0_ST;
    } wcr0_UN;

    REG_32 : 16;
    union
    {
        REG_16 pcr0_UW;
        struct
        {
            REG_32            : 11;
            REG_32 ClkDiv_B4  : 4;
            REG_32 PEnable_B1 : 1;
        } pcr0_ST;
    } pcr0_UN;

    REG_32   : 16;
    REG_16 plr_UW;
    REG_32   : 16;
    REG_16 pprot_UW;

} MMC_ST, *MMC_PST;

extern MMC_ST stMmc; 

/* ------------------------------------------------------------------------- */
/* SARDEC_ST                                                                 */
/*      Definition of the Address Manager register map.                      */

typedef volatile struct
{
    REG_32   : 16;
    REG_16 mfbahR0_UW;
    REG_32   : 16;
    REG_16 mfbalR0_UW;
    REG_32   : 16;
    REG_16 mfbahR1_UW;
    REG_32   : 16;
    REG_16 mfbalR1_UW;
    REG_32   : 16;
    REG_16 mfbahR2_UW;
    REG_32   : 16;
    REG_16 mfbalR2_UW;
    REG_32   : 16;
    REG_16 mfbahR3_UW;
    REG_32   : 16;
    REG_16 mfbalR3_UW;
    REG_32   : 16;
    REG_16 mfbahR4_UW;
    REG_32   : 16;
    REG_16 mfbalR4_UW;
    REG_32   : 16;
    REG_16 mfbahR5_UW;
    REG_32   : 16;
    REG_16 mfbalR5_UW;

    REG_32   : 16;
    REG_16 mcbahR0_UW;
    REG_32   : 16;
    REG_16 mcbalR0_UW;
    REG_32   : 16;
    REG_16 mcbahR1_UW;
    REG_32   : 16;
    REG_16 mcbalR1_UW;
    REG_32   : 16;
    REG_16 mcbahR2_UW;
    REG_32   : 16;
    REG_16 mcbalR2_UW;
    REG_32   : 16;
    REG_16 mcbahR3_UW;
    REG_32   : 16;
    REG_16 mcbalR3_UW;
    REG_32   : 16;
    REG_16 mcbahR4_UW;
    REG_32   : 16;
    REG_16 mcbalR4_UW;
    REG_32   : 16;
    REG_16 mcbahR5_UW;
    REG_32   : 16;
    REG_16 mcbalR5_UW;

} ADDRMAN_ST, *ADDRMAN_PST;

extern ADDRMAN_ST stAddrMan;

/* ------------------------------------------------------------------------- */
/* SARSYS_ST                                                                 */
/*      Definition of the System Module register map.                        */

typedef volatile struct
{
    REG_32   : 16;   
    REG_16 ClkCntl_UW;
    REG_32   : 32;
    REG_32   : 16;
    REG_16 CramCR_UW;
    REG_32   : 16;
    REG_16 GlbCtrl_UW;
    REG_32   : 16;
    REG_16 SysECR_UW;
    REG_32   : 16;
    REG_16 SysESR_UW;
    REG_32   : 16;
    REG_16 AbrtESR_UW;
    REG_32   : 16;
    REG_16 GlbStat_UW;
    REG_32   : 16;
    REG_16 Dev_UW;
    REG_32   : 32;
    REG_32   : 16;
    REG_16 Ssif_UW;
    REG_32   : 16;
    REG_16 Ssir_UW;
} SYS_ST, *SYS_PST;

extern SYS_ST stSys;

/* ------------------------------------------------------------------------- */
/* SARCIM_ST                                                                 */
/*      Definition of the Central Interrupt Module register map.  (p.5-15)   */

typedef volatile struct
{
    REG_32   : 24;   
    REG_8  IrqIVec_UB;
    REG_32   : 24;
    REG_8  FiqIVec_UB;
    REG_32   : 24;
    REG_8  CimIVec_UB;

    REG_32   Firqpr_UL;
    REG_32   Intreq_UL;
    REG_32   Reqmask_UL;
 
} CIM_ST, *CIM_PST;

extern CIM_ST stCim;

/* ------------------------------------------------------------------------- */
/* SARRTI_ST																					  */
/*		  Definition of the Realtime Interrupt Module register map				  */

typedef volatile struct
{
	REG_32 cntr_UL;
	REG_32 pctl_UL;
	REG_32 cntl_UL;
	REG_32 wkey_UL;
	REG_32 cmp1_UL;
	REG_32 cmp2_UL;
	REG_32 cint_UL;
	REG_32 cnten_UL;
	
} RTI_ST, *RTI_PST;

extern RTI_ST stRti;

/* ------------------------------------------------------------------------- */
/* CAN_ST                                                                   */
/*      Definition of the CAN register map.                                 */

typedef volatile struct _CAN_ST
{
    REG_32   ME_UL;
    REG_32   MD_UL;
    REG_32   TRS_UL;
    REG_32   TRR_UL;
    REG_32   TA_UL;
    REG_32   AA_UL;
    REG_32   RMP_UL;
    REG_32   RML_UL;
    REG_32   RFP_UL;

    union gam
    {
        REG_32 GAM_UL;
        struct
        {
            REG_32 Ami_B1       : 1;
            REG_32              : 2;
            REG_32 Gam_B29      : 29;
        }GAM_ST;
    } GAM_UN;

    union mc
    {
        REG_32 MC_UL;
        struct
        {
            REG_32              : 16;
            REG_32 Lntc_B1      : 1;
            REG_32 Lntm_B1      : 1;
            REG_32 Scm_B1       : 1;
            REG_32 Ccr_B1       : 1;
            REG_32 Pdr_B1       : 1;
            REG_32 Dbo_B1       : 1;
            REG_32 Wuba_B1      : 1;
            REG_32 Crd_B1       : 1;
            REG_32 Abo_B1       : 1;
            REG_32 Stm_B1       : 1;
            REG_32 Sres_B1      : 1;
            REG_32 Mbnr_B5      : 5;
        }MC_ST;
    } MC_UN;

    REG_32   BTC_UL;
    REG_32   ES_UL;
    REG_32   TEC_UL;
    REG_32   REC_UL;
    REG_32   GIF0_UL;
    REG_32   GIM_UL;
    REG_32   GIF1_UL;
    REG_32   MIM_UL;
    REG_32   MIL_UL;
    REG_32   OPC_UL;
    
    union tioc
    {
        REG_32 MC_UL;
        struct
        {
            REG_32              : 28;
            REG_32 TxFunc_B1    : 1;
            REG_32 TxDir_B1     : 1;
            REG_32 TxOut_B1     : 1;
            REG_32 TxIn_B1      : 1;

        }TIOC_ST;
    } TIOC_UN;

    union rioc
    {
        REG_32 RIOC_UL;
        struct
        {
            REG_32              : 28;
            REG_32 RxFunc_B1    : 1;
            REG_32 RxDir_B1     : 1;
            REG_32 RxOut_B1     : 1;
            REG_32 RxIn_B1      : 1;
        }RIOC_ST;
    } RIOC_UN;

} CAN_ST, *CAN_PST;

/* - FLASH WRAPPER REGISTERS ---------------------------------------------- */

typedef volatile struct flash
{
    unsigned         : 16;
    unsigned short FMBAC1;
    unsigned         : 16;
    unsigned short FMBAC2;
    unsigned         : 16;
    unsigned short FMBSEA;
    unsigned         : 16;
    unsigned short FMBSEB;
    unsigned         : 16;
    unsigned short FMBRDY;

    unsigned dummy1[1787];
    
    unsigned FMREGOPT;
    unsigned : 32;
    unsigned FMBBUSY;
    unsigned FMPKEY;

    unsigned dummy2[768];

    unsigned         : 16;
    unsigned short FMPTR4;

    unsigned         : 16;
    unsigned short FMPRDY;
    
    unsigned dummy3[1274];

    unsigned         : 16;
    unsigned short FMMAC1;
    unsigned         : 16;
    unsigned short FMMAC2;
    unsigned         : 16;
    unsigned short FMPAGP;
    unsigned         : 16;
    unsigned short FMMSTAT;
    unsigned         : 16;
    unsigned short FMTCR;
} FLASH_ST, *FLASH_PST;
/* ------------------------------------------------------------------------- */
#endif /*TMS470R1_H_*/
