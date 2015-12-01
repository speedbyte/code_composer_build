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
* $Date: 2009-03-19 13:27:43 +0100 (Do, 19 Mrz 2009) $
* $Rev: 5399 $
* $Author: agrawal $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/SYS_CALLBACK/trunk/inc/CAN_CB_cfg.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _CAN_CB_CFG_H
#define _CAN_CB_CFG_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include <osek.h>
#include <osekext.h>

#include "Common.h"
/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/
#define ITRANSCEIVER 100 /* CONFIGURATE EXPORT INTERFACE(ITRANSCEIVER) */

/*----------------------------------------------------------------------------*/
/* Import configuration                                                       */
/*----------------------------------------------------------------------------*/
/** 
\brief	interface for the function to change the Can transceiver state
*/
#define ITRANSCEIVER_CFG 100
#if (ITRANSCEIVER_CFG != ITRANSCEIVER)
	#error ("Incompatible interface: configured ifc = %d, reference ifc = %d!!!" ,ITRANSCEIVER_CFG, ITRANSCEIVER)
#endif


/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* CAN Transceiver EN and STB_ Pin setting macro                              */

/** 
\brief	define for internal use of CAN_EN and CAN_STB_ Pin
*/
#define CAN_EN		PIN4, GIOB
#define CAN_STB_	PIN5, GIOB

/** 
\brief	macro to set Pin EN or STB on the CAN transceiver to HIGH
*/
#define CAN_SET_IO			GIO_vPortSet
/** 
\brief	macro to set Pin EN or STB on the CAN transceiver to LOW
*/
#define CAN_CLEAR_IO		GIO_vPortClear
/*----------------------------------------------------------------------------*/                           



/*----------------------------------------------------------------------------*/
/* enable Flag for interrupt in CIM                                           */
/** 
\brief	define Flag to enable CAN transceive interrupt (SCC1) in CIM register
*/
#define CAN1_INT_FLAG_CIM 		(1<<13)

/** 
\brief	define the memory area of the ISR REQMASK register 
*/
#define CAN_INT_REQMASK 		osdREQMASK

/** 
\brief	define the memory area of the ISR FIRQPR register 
*/
#define CAN_INT_FIRQPR			osdFIRQPR

/** 
\brief	define the memory area of the ISR INTREQ register 
*/
#define CAN_INT_INTREQ			osdINTREQ


#endif /* _CAN_CB_CFG_H */

/* End Of File CAN_CB_cfg.h */
