/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/
/**
 * \file 	GIO.h
 * \brief	Module Header File.
 * 			This file must be included by the application/system in order to use
 * 			the interfaces provided by this module. Detailed description found below.
 * \brief	This file mainly contains:
 * 			1. prototypes of all the functions that can be used by other modules.
 * 			2. enum, structure, constant definition etc. that can be used by other
 * 			modules.
*/
/*-----------------------------------------------------------------------------
* $Date: 2010-11-17 07:43:24 +0100 (Mi, 17 Nov 2010) $
* $Rev: 16328 $
* $Author: weber $
* $URL: http://subversion-01/svn/SW_HW_System/Plattform/TMS470/MOD/MOD_GIO/trunk/inc/GIO.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _GIO_H
#define _GIO_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include "common.h"
#include "tms470r1.h"
/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/
#include "PORT.h"
/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
/* include the export configuration file */
#include "GIO_CE.h"

/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Types                                                               */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Declarations (avoid as much as possible!)                 */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global const Declarations                                                  */
/*----------------------------------------------------------------------------*/
#define GPIO		0
#define PERIPHERAL 	1

#define INPUT 		0
#define OUTPUT 		1

#define PIN_LOW		0
#define PIN_HIGH	1

typedef enum _GIOPOLARITY
{
	FallingEdge = 0,
	RisingEdge = 1

} GIOPOLARITY;

typedef enum _GIOPRIORITY
{
    LowPriority = 0,
    HighPriority = 1
} GIOPRIORITY;

#define PIN0  0x00000001
#define PIN1  0x00000002
#define PIN2  0x00000004
#define PIN3  0x00000008
#define PIN4  0x00000010
#define PIN5  0x00000020
#define PIN6  0x00000040
#define PIN7  0x00000080
#define PIN8  0x00000100
#define PIN9  0x00000200
#define PIN10 0x00000400
#define PIN11 0x00000800
#define PIN12 0x00001000
#define PIN13 0x00002000
#define PIN14 0x00004000
#define PIN15 0x00008000
#define PIN16 0x00010000
#define PIN17 0x00020000
#define PIN18 0x00040000
#define PIN19 0x00080000
#define PIN20 0x00100000
#define PIN21 0x00200000
#define PIN22 0x00400000
#define PIN23 0x00800000
#define PIN24 0x01000000
#define PIN25 0x02000000
#define PIN26 0x04000000
#define PIN27 0x08000000
#define PIN28 0x10000000
#define PIN29 0x20000000
#define PIN30 0x40000000
#define PIN31 0x80000000

#define INT0 0x01
#define INT1 0x02
#define INT2 0x04
#define INT3 0x08
#define INT4 0x10
#define INT5 0x20
#define INT6 0x40
#define INT7 0x80
// Only port A is interrupt enabled.

typedef struct _GIO_INTCNF_ST
{
    Uint8Type Enable_UB;
    Uint8Type Priority_UB;
    Uint8Type Polarity_UB;
} GIO_INTCNF_ST, *GIO_INTCNF_PST;


#define GIOA 0
#define GIOB 1
#define GIOC 2
#define GIOD 3

#ifndef GIO_WIDTH
#define GIO_WIDTH 8
#endif


#define PORTMASK ~(0xFFFFFFFFU << GIO_WIDTH)
/* -------------------------------------------------------------------------  */
/* PORTMASK: the driver needs to know the number of bits implemented in the   */
/*           GIO module for the GIO_PortRead_UL routine.  We assume an        */
/*           environment variable set to the number of bits.                  */
/*                                                                            */
/*           GIO_WIDTH : 1 <= GIO_WIDTH <= 32                                 */
/*----------------------------------------------------------------------------*/
/* Public Methods                                                             */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/

#ifdef IGIO_READ

Uint8Type GIO_u8PinRead(Uint32Type PinMask_N, Uint8Type Dev_N);

Uint32Type GIO_u32PortRead(Uint8Type Dev_N);
/*----------------------------------------------------------------------------*/
#endif

#ifdef IGIO_WRITE

void GIO_vPinModify(Uint8Type Dev_N, Uint32Type PinMask_N, PORTQUAL PinQual_N );

void GIO_vPinWrite(Uint32Type PinMask_N, Uint8Type PinData_B, Uint8Type Dev_N);

void GIO_vPortSet(Uint32Type PinMask_N, Uint8Type Dev_N);

void GIO_vPortClear(Uint32Type PinMask_N, Uint8Type Dev_N);
/*----------------------------------------------------------------------------*/
#endif

#ifdef IGIO_INTERRUPT

void GIO_vIntEnable(Uint8Type IntId_N, GIOPOLARITY Edge_B, GIOPRIORITY Priority_B);	// Global

void GIO_vIntDisable(Uint8Type  IntId_N);

void GIO_IntGlobalInit_V(const GIO_INTCNF_PST pstCnt, void (*ReqHigh_PV)(Uint8Type), void (*ReqLow_PV)(Uint8Type));
/* -------------------------------------------------------------------------- */
/* GIO_INTREQ                                                                 */
/*      User supplied interrupt callback routine                              */

typedef void GIO_INTREQ(Uint8Type Index_UB);

#endif

#endif /* _GIO_H */

/* End Of File GIO.h */
