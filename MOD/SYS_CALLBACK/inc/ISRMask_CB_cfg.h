/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     CAN_CB_cfg.h
*-----------------------------------------------------------------------------
* Module Name:	  MOD_CAN
*-----------------------------------------------------------------------------
* Description:	  configuration header for the module MOD_CAN
*-----------------------------------------------------------------------------
* $Date: 2009-02-04 15:23:16 +0100 (Mi, 04 Feb 2009) $
* $Rev: 4348 $
* $Author: rammelt $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/SYS_CALLBACK/trunk/inc/ISRMask_CB_cfg.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _ISRMASK_CB_CFG_H
#define _ISRMASK_CB_CFG_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include <osek.h>
#include <osekext.h>

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Import configuration                                                       */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/
#define ISR_00_SPI1         0x00000001
#define ISR_01_COMP2        0x00000002
#define ISR_02_COMP1        0x00000004
#define ISR_03_TAP          0x00000008
#define ISR_04_SPI2         0x00000010
#define ISR_05_GIOA         0x00000020
#define ISR_06_EXT6         0x00000040
#define ISR_07_HET1         0x00000080
#define ISR_08_EXT8         0x00000100
#define ISR_09_SCIERR       0x00000200
#define ISR_10_SCI1RX       0x00000400
#define ISR_11_EXT11        0x00000800
#define ISR_12_EXT12        0x00001000
#define ISR_13_HECCA        0x00002000
#define ISR_14_EXT14        0x00004000
#define ISR_15_SPI3         0x00008000
#define ISR_16_ADCEV        0x00010000
#define ISR_17_SCI2RX       0x00020000
#define ISR_18_DMA1         0x00040000
#define ISR_19_EXT19        0x00080000
#define ISR_20_SCI1TX       0x00100000
#define ISR_21_SSI          0x00200000
#define ISR_22_EXT22        0x00400000
#define ISR_23_HET2         0x00800000
#define ISR_24_HECCB        0x01000000
#define ISR_25_EXT25        0x02000000
#define ISR_26_SCI2TX       0x04000000
#define ISR_27_ADCEG1       0x08000000
#define ISR_28_DMA2         0x10000000
#define ISR_29_GIOB         0x20000000
#define ISR_30_ADCEG2       0x40000000
#define ISR_31_EXT31        0x80000000


/* UART Tx ISR enable */ 
/* UART Rx ISR enable */
/* CAN1 ISR enable */ 
#define INTERRUPT_FLAGS     (   \
								ISR_26_SCI2TX | \
								ISR_17_SCI2RX | \
								ISR_13_HECCA  | \
								ISR_20_SCI1TX | \
								ISR_10_SCI1RX | \
								ISR_09_SCIERR   \
							)


/** 
\brief	define the memory area of the ISR REQMASK register 
*/
#define INT_REQMASK 		osdREQMASK

/** 
\brief	define the memory area of the ISR FIRQPR register 
*/
#define INT_FIRQPR			osdFIRQPR

/** 
\brief	define the memory area of the ISR INTREQ register 
*/
#define INT_INTREQ			osdINTREQ

#endif /* _ISRMASK_CB_CFG_H */

/* End Of File ISRMask_CB_cfg.h */
