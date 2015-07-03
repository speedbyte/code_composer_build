/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:  CSHDL_CE.h
*-----------------------------------------------------------------------------
* Module Name: CSHDL (Central state handler)
*-----------------------------------------------------------------------------
* Description: managing the system states for the TMS470 and Bluetooth module
*-----------------------------------------------------------------------------
* $Date: 2009-08-13 12:33:29 +0200 (Do, 13 Aug 2009) $
* $Rev: 7796 $
* $Author: agrawal $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_CSHDL/trunk/inc/CSHDL_CE.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _CSHDL_CE_H
#define _CSHDL_CE_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include "common.h"
#include "CSHDL_c1.h"

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Import configuration                                                       */
/*----------------------------------------------------------------------------*/
/**
\brief	description of configured interface
*/

#define ICSHDL_BLUETOOTH_UPDATE 1
#define ICSHDL_DIAG
/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/
/**
\brief	macro for the different System states
*/

/*----------------------------------------------------------------------------*/
/* Public Constants                                                           */
/*----------------------------------------------------------------------------*/
/**
\brief	defines to sign if the state is active or not
*/
#define CSHDL_STATE_SET         1
#define CSHDL_STATE_NOT_SET     0
/**
\brief	allow the state change from SystemInit to Peripheral state
*/
#define CSHDL_SYSTEMI_TO_PERIPHERAL_ALLOWED     0x01
/**
\brief	allow the state change from Peripheral to CAN state
*/
#define CSHDL_PERIPHERAL_TO_CAN_ALLOWED         0x03
/**
\brief	allow the state change from CAN to Bluetooth module state
*/
#define CSHDL_CAN_TO_BT_ALLOWED                 0x07
/**
\brief	allow the state change from Bluetooth module to MainAppl state
*/
#define CSHDL_BT_TO_MAINAPPL_ALLOWED            0x0F
/**
\brief	allow the state change from MainAppl to SystemRun
*/
#define CSHDL_MAINAPPL_TO_SYSTEMR_ALLOWED       0x1F


#endif /* _CSHDL_CE_H */

/* End Of File CSHDL_CE.h */

