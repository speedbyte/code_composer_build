/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     VF45AA_link32.cmd
*-----------------------------------------------------------------------------
* Module Name:    LINKER COMMAND FILE
*-----------------------------------------------------------------------------
* Description:    DEVICE:	TMS470VF45AA	
*-----------------------------------------------------------------------------
* $Date: 2009-01-08 14:10:20 +0100 (Do, 08 Jan 2009) $
* $Rev: 3617 $
* $Author: agrawal $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/SYS_DEV/trunk/MOD/VF45AA_link32.cmd $
*-----------------------------------------------------------------------------
*/

/****************************************************************************************/
/* OPTIONS                                                                  			*/
/****************************************************************************************/
-c                      /* ROM AUTOINITIALIZATION MODEL                     			*/
-l rts32tiabi.lib
/* -l adc32.lib */
/* -l mbadc32.lib */
/* -l can32.lib */
/* -l dma32.lib */
/* -l gio32.lib */
/* -l het32.lib */
/* -l sci32.lib */
/* -l spi32.lib */
-x

/****************************************************************************************/
/* SPECIFY THE SYSTEM MEMORY MAP                                            			*/
/****************************************************************************************/
MEMORY
{
    VECTORS1  (X)   :   origin=0x00000000   length=0x00000020
   	ADLATUS   (RXI) :   origin=0x00000020   length=0x00007FD0
   	/* ROM ( 480K ) Table Start*/
   	VECTORS2  (X)       :  origin=0x00008000   length=0x00000024
   	ROM      (RX)       :  origin=0x00008024   length=0x00077bd8  /* 479K - 40 bytes       */
   	KOMBI_TEXT_ROM (RX) :  origin=0x000b83fc   length=0x00007800  /* 30k for Kombi text set */
   	TABLE_ROM (RX)      :  origin=0x0007fbfc   length=0x00000400  /* 1k for audio set table */
   	CHKS2     (R)       :  origin=0x0007fffc   length=0x00000004  fill=0x12345678
	/* ROM Table End */
   	FLASH_REQ (RW)  :   origin=0x00500000   length=0x00000004   /* RAM cell for flash request pattern */   
   	RAM       (RW)  : 	origin=0x00500004   length=0x00007FFC   /*  32K                 */    
   	/* dedicated RAM */
   	
   	HETPROG   (RW)  :	origin=0x00800000	length=0x00000600   /* HETPROG				*/
   	MPU  	  (RW)  :	origin=0xFFE84000	length=0x00000024   /* MPU control registers*/
    PROGRAM_FLASH (RW):	origin=0xFFE88000	length=0x00004000 /* Flash control registers*/
    
    /* peripherals */
    
    HET       (RW)  :	origin=0xFFF7FC00	length=0x00000100	/* HET					*/
    SPI1      (RW)	:	origin=0xFFF7F800	length=0x00000100	/* SPI1					*/
    SCI2      (RW)	:	origin=0xFFF7F500	length=0x00000100	/* SCI2					*/
    SCI1      (RW)	:	origin=0xFFF7F400	length=0x00000100	/* SCI1					*/
    ADC1	  (RW)	:	origin=0xFFF7F000	length=0x00000100	/* ADC1					*/
    ECP       (RW)  :	origin=0xFFF7EF00	length=0x00000100	/* ECP 					*/
    GIO       (RW)	:	origin=0xFFF7EC00	length=0x00000100	/* GIO					*/
    HECC2     (RW)	:	origin=0xFFF7EA00	length=0x00000200	/* HECC2 Control		*/
    HECC1     (RW)	:	origin=0xFFF7E800	length=0x00000200	/* HECC1 Control		*/
    HECCRAM2  (RW)	:	origin=0xFFF7E600	length=0x00000200	/* HECC2 RAM			*/
    HECCRAM1  (RW)	:	origin=0xFFF7E400	length=0x00000200	/* HECC1 RAM			*/
    SPI3      (RW)	:	origin=0xFFF7D500	length=0x00000100	/* SPI3					*/
    SPI2      (RW)	:	origin=0xFFF7D400	length=0x00000100	/* SPI2					*/
    
	/* sar module addresses */

    SYS       (RW)	:	origin=0xFFFFFFD0	length=0x00000030	/* System				*/
    PSA       (RW)	:	origin=0xFFFFFF40	length=0x00000020	/* Parallel Sig Analysis*/
    CIM       (RW)  :   origin=0xFFFFFF20	length=0x00000020	/* Central Interrupt	*/ 
    RTI       (RW)  :   origin=0xFFFFFF00	length=0x00000020	/* Real Time Interrupt	*/ 
    DMA       (RW)	:	origin=0xFFFFFE80	length=0x00000080	/* DMA					*/
    DEC       (RW)	:	origin=0xFFFFFE00	length=0x00000080	/* Address Manager		*/
    MMC       (RW)	:	origin=0xFFFFFD00	length=0x00000080	/* Memory Controller	*/
    IEM       (RW)	:	origin=0xFFFFFC00	length=0x00000100	/* IEM					*/
    DMACP     (RW)	:	origin=0xFFFFF800	length=0x00000200	/* DMA CMD Buffer		*/	
    
}  
/****************************************************************************************/


/****************************************************************************************/
/* SPECIFY THE SECTIONS ALLOCATION INTO MEMORY                                          */
/****************************************************************************************/
SECTIONS
{
    .vectors : {} > VECTORS1
    .reset   : {} >  0x8000	/*this is new because there is no jmp to _c_int00 otherwise!*/
    .osarmvect : {} >0x8004     /* INTERRUPT VECTORS                                    */ 
    .osvtable : {} > 0x8080     /* IRQ HANDLING                                         */
    .intvecs : {} > VECTORS2    /* INTERRUPT VECTORS                 					*/
    .startup : {} > 0x00008030  /* START ADDRESS                     					*/

    .bss     : {} > RAM         /* GLOBAL & STATIC VARS              					*/
    .sysmem  : {} > RAM  	    /* DYNAMIC MEMORY ALLOCATION AREA    					*/

    .text    : {} > ROM         /* CODE                              					*/
    .const   : {} > ROM         /* CONSTANT DATA                     					*/
    .cinit   : {} > ROM         /* INITIALIZATION TABLES             					*/

    .UHV_KOMBI_TEXT_SETS : {} > KOMBI_TEXT_ROM   /* Kombi Static Texts */
    .UHV_AUDIO_SETS : {} > TABLE_ROM   /* Audio Set table	*/
	
	.SPI1  	 : {_e_SPI0_ST 	 = .;}		> SPI1
	.SPI2  	 : {_e_SPI1_ST 	 = .;}		> SPI2
	.SPI3  	 : {_e_SPI2_ST 	 = .;}		> SPI3
    .HECCRAM1: {_e_CAN0_MB_PST= .;} 	> HECCRAM1
    .HECCRAM2: {_e_CAN1_MB_PST= .;} 	> HECCRAM2
    .HECC1   : {_e_CAN0_ST 	 = .;}		> HECC1
    .HECC2   : {_e_CAN1_ST 	 = .;}		> HECC2
    .GIO	 : {_e_GIO_ST 	 = .;} 		> GIO    
    /* no driver for ECP so far 														*/
    /*.ECP   : { 			 = .;}		> ECP1x											*/    
    .ADC1	 : {_e_ADC0_ST   = .;}		> ADC1
    /*.MibADC1: {_e_MBADC0_ST =.;}		> MibADC1										*/
    .SCI1  	 : {_e_SCI0_ST 	 = .;}		> SCI1
	.SCI2  	 : {_e_SCI1_ST 	 = .;}		> SCI2
    .HET  	 : {_e_HET0_ST 	 = .;}		> HET
	.HETCODE : {_e_HETPROGRAM0_UN = .;} > HETPROG  /* HET PROGRAM 						*/
	
	.MMC     : {_stMmc = .;}  	        > MMC
    .DEC     : {_stAddrMan = .;}  	    > DEC
    .SYS     : {_stSys = .;}  	        > SYS
    .CIM	 : {_e_SARCIM_ST = .;}		> CIM
    .RTI     : {_stRti = .;}		    > RTI
    .DMACP   : {_e_DMACP_PST = .;}  	> DMACP
    .DMA	 : {_e_DMA_ST 	 = .;}  	> DMA
    
    .REQUEST  : {_r_F_REQUEST = .;}     > FLASH_REQ
}
