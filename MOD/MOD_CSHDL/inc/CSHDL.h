/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     CSHDL.h
*-----------------------------------------------------------------------------
* Module Name:
*-----------------------------------------------------------------------------
* Description:
*-----------------------------------------------------------------------------
* $Date: 2009-03-09 16:19:25 +0100 (Mo, 09 Mrz 2009) $
* $Rev: 4933 $
* $Author: agrawal $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_LED/trunk/inc/LED.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _CSHDL_H
#define _CSHDL_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include <osek.h>
#include <osekext.h>

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "Common.h"
#include "CSHDL_CE.h"

/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global const Declarations                                                  */
/*----------------------------------------------------------------------------*/
/**
\brief	Maximum number of buttons to be handled
*/
/*----------------------------------------------------------------------------*/
/* Public Types                                                               */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Global Variables Declarations (avoid as much as possible!)                 */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Methods                                                             */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/
/**
\brief	description of the interface ICSHDL_BLUETOOTH_UPDATE
*/
#ifndef ICSHDL_BLUETOOTH_UPDATE
#define ICSHDL_BLUETOOTH_UPDATE
#endif

#ifdef ICSHDL_BLUETOOTH_UPDATE /* BEGIN DECLARE INTERFACE(ICSHDL_BLUETOOTH_UPDATE) */

extern BooleanType CSHDL_bGetBuetoothUpdateActive(void);
extern void CSHDL_vTriggerBluetoothUpdate(Uint8Type StartStopUpdate);
#ifdef ICSHDL_DIAG
extern SuccessFailType CSHDL_vDiagnoseResetSimulation(void);
#endif

#endif /* END DECLARE INTERFACE(ICSHDL_BLUETOOTH_UPDATE) */

extern Uint8Type CSHDL_vGetCkOnOffStatus(void);
extern void CSHDL_vCK5050_PowerOFF_ACK(void);
extern void CSHDL_vCK5050_Startup(void);
extern BooleanType CSHDL_bIsKl15On(void);
extern BooleanType CSHDL_bIsUHVOnConditionSatisfied(void);
extern Uint8Type CSHDL_u8GetBluetoothUpdateTrigger(void);


#endif /* _CSHDL_H */

/* End Of File CSHDL.h */
