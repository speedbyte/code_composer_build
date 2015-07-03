/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/
/** 
 * \file 	SDS_UDS_CB_C1.c
 * \brief	This file contains the program to xxxx
 * \brief	This file fulfills the following requirements:
 * 			1. Requirement 1
 * 			2. Requirement 2
 * 			3. Requirement x
*/
/*-----------------------------------------------------------------------------
* $Date: 2009-11-11 13:29:45 +0100 (Mi, 11 Nov 2009) $
* $Rev: 9713 $
* $Author: agrawal $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_SDS_UDS/trunk/src/SDS_UDS_CB_C1.c $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include "tms470r1.h"
#include <string.h>
#include <osek.h>           // vector
#include <osekext.h>        // vector
/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/
#include "Common.h"
#include "sds.h"
#include "uds.h"
#include "tp.h"
#include "deh.h"
#include "VCR.h"
#include "sds_ext.h"
#include "ISRMask_CB_cfg.h"
#include "APP_CAN_Speed.h"
#include "APP_DIAG.h"
#include "can_def.h"
#include "can_cfg.h"
#include "crcmgr.h"
#include "deh_dtc_cfg.h"
#include "sds_cfg.h"
#include "OSEK_HostTask_H1.h"
#include "CSHDL.h"
#include "SDS_UDS_CI.h"
#include "TRACE.h"
#include "SDS_UDS_CB_C1.id"
/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "SDS_ext.h"
#include "SDS_UDS_CI.h"

/*----------------------------------------------------------------------------*/
/* pragma Directives                                                          */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Definitions (avoid as much as possible!)                  */
/*----------------------------------------------------------------------------*/
BooleanType _SDS_UDS_bStopCommunicationMessages = BT_FALSE;

Uint8Type _SDS_UDS_u8ResetToFactory;
/*----------------------------------------------------------------------------*/
/* Private Types                                                              */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the type
*/
/* structure for data ID configuration array cs_DataIdItem: see sds_cfg.h */
/*----------------------------------------------------------------------------*/
/* Private Manifest Constant Definitions                                      */
/*----------------------------------------------------------------------------*/
/************** A T T E N T I O N ***************************** 
   because of speed optimization binary search algorithm is used
   which requires a sorted (ascending !!!) data identifier table */

/*----------------------------------------------------------------------------*/
/* Prototypes of file local Methods                                           */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* File local Code Macros                                                        */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the macros
*/

/*----------------------------------------------------------------------------*/
/* File local Variables                                                       */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the private variable
*/

/*----------------------------------------------------------------------------*/
/* Private Address Calculations                                               */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Function    : SDSext_vResetSecurityAccessEffected                          */
/** 
 \brief      Callback function to inform the application of to exchange of
			 session and thus the call for consideration of the security
			 access status

 \param      none
 
 \return     none
 */
/*----------------------------------------------------------------------------*/
void SDSext_vResetSecurityAccessEffected(void)
{
	ubSecState &= MASK_SEC_ALL_DEACTIVE; // Security zurücknehmen
}

/*----------------------------------------------------------------------------*/
/* Function    : SDSext_vSetProgRequest                                       */
/**
 \brief      Callback function for set interrupt bits

 \brief      Description:
 function to set bits for the diferent interrupts

 \param      none

 \return     none
 */
/*----------------------------------------------------------------------------*/
void SDSext_vSetProgRequest(void)
{
	Set_FlashRequest_Paddern = FLASH_REQUEST_PADDERN_APP;
	/* software reset */
	stSys.SysECR_UW = 0xc000u;
}

/*----------------------------------------------------------------------------*/
/* Function    : SDSext_ubZeroVehicleOrEngineSpeed                            */
/**
 \brief      Callback function for check if vehicle speed is zero

 \param      none

 \return     Vehicle speed
 */
/*----------------------------------------------------------------------------*/
Uint8Type SDSext_ubZeroVehicleOrEngineSpeed(Uint8Type ServiceID)
{
	Uint8Type retVal = UDScub_IN_PROCESS;
	if ( (ServiceID == 0x10) ||  (ServiceID == 0x28) || (ServiceID == 0x85))
	{
		if ( APP_CAN_Speed_un16CurrentVehicleSpeed() == U16_SPEED_ZERO )
		{
			retVal = UDScub_IN_PROCESS;
		}
		else
		{
			retVal = UDScub_VEHICLE_SPEED_TOO_HIGH;
		}
	}
	else
	{
		retVal = UDScub_CONDITIONS_NOT_CORRECT;
	}
	DEBUG_VALUE2(SDS_UDS_SWI_TRC, MOD_SDS_UDS, "SDSext_ubZeroVehicleOrEngineSpeed: Service ID = %02X, retVal = %d",ServiceID,retVal);
	return retVal;
}

/*----------------------------------------------------------------------------*/
/* Function    : SDSext_ubPowerDown                            */
/**
 \brief      Power Down function

 \param      none

 \return     Vehicle speed
 */
/*----------------------------------------------------------------------------*/
Uint8Type SDSext_ubPowerDown(Uint8Type ResetType) {
	ResetType = ResetType;
	return (I2S_TRUE);
}

/*----------------------------------------------------------------------------*/
/* Function    : SDSext_ubComCtrl                                             */
/**
 \brief      Callback function for active or deactivate Tx/Rx message

 \param      ControlType
 Rx/Tx control

 \param      CommNetworkType
 communication/network control

 \return     UDScub_SUBFUNCTION_NOT_SUPPORTED ungültiger ControlType.
 UDScub_FINISHED Sendedaten liegen vor
 UDScub_REQUEST_OUT_OF_RANGE Fehlercode, wenn ein Parameter ungültig ist.
 */
/*----------------------------------------------------------------------------*/
Uint8Type SDSext_ubComCtrl(Uint8Type ControlType, Uint8Type CommNetworkType)
{
	Uint8Type retVal = UDScub_FINISHED;
	DEBUG_VALUE2(SDS_UDS_SWI_TRC, MOD_SDS_UDS, "SDSext_ubComCtrl: ControlType = %02X, CommNetworkType = %02X",ControlType,CommNetworkType);
	switch (ControlType)
	{
		case SDS_28_EnableRxAndTx:
		{
			if( (CommNetworkType == SDS_28_NormalCommunicationMessagesOnly) || ( CommNetworkType == SDS_28_NormalAndNetworkManagmentCommunicationMessages ))
			{
				// API Schnittstellenfunktion zum Aktivieren der zyklischen CAN-Messages
				CanPartOnline(C_SEND_GRP_USER1);
				DEBUG_TEXT(SDS_UDS_SWI_TRC, MOD_SDS_UDS, "Service $28: Enable Rx and Tx");
				_SDS_UDS_bStopCommunicationMessages = BT_FALSE;
			}
			else
			{
				retVal = UDScub_REQUEST_OUT_OF_RANGE;
			}
			break;
		}
		case SDS_28_EnableRxAndDisableTx:
		{
			if (CommNetworkType == SDS_28_NormalCommunicationMessagesOnly)
			{
				// API Schnittstellenfunktion zum Deaktivieren der zyklischen CAN-Messages
				_SDS_UDS_bStopCommunicationMessages = BT_TRUE;
				DEBUG_TEXT(SDS_UDS_SWI_TRC, MOD_SDS_UDS, "Service $28: Enable Rx and Disable Tx");
				CanPartOffline(C_SEND_GRP_USER1);
			}
			else
			{
				retVal = UDScub_REQUEST_OUT_OF_RANGE;
			}
			break;
		}
		case SDS_28_GatewayVehicleWakeUp:
		default:
		{
			retVal = UDScub_SUBFUNCTION_NOT_SUPPORTED;
			break;
		}
	}
	return retVal;
}

/*----------------------------------------------------------------------------*/
/* Function    : SDSext_vDoReset                                              */
/**
 \brief      Callback function for software reset

 \param      ubResetControl
 reset type (hard/soft)

 \return     none
 */
/*----------------------------------------------------------------------------*/
void SDSext_vDoReset(Uint8Type ubResetControl)
{
	switch (ubResetControl)
	{
		case SDS_11_HardReset:
		{
			(void)UDS_ubSetSession(UDS_CHANNEL_INDEX UDScub_DEFAULT_SESSION); /* go back to default session */
			ubSecState &= ~MASK_SEED_ALREADY_SEND; // dieser Zustand wird nicht ins EEPROM gespeichert, bzw. beim Zurücklesen nach Systemstart ausmaskiert
			SDSext_vResetSecurityAccessEffected();
			SDS_UDS_vInitialiseServiceDataDownload();
			SDS_UDS_vInitialiseService31();
			ubResponseFromAppTask = 0;
			_SDS_UDS_u8UserProfile = DEFAULT_USER_PROFILES;
			_SDS_UDS_u8ResetToFactory = DEFAULT_RESET_TO_FACTORY_SETTING;
			break;
		}
		case SDS_11_KeyOffOnReset:
		{
			if ( FAIL == CSHDL_vDiagnoseResetSimulation() )
			{
				// Send negative Response.
			}
			else
			{
				// I will never come here ;)
			}
			break;
		}
		case SDS_11_SoftReset:
		{
			/* disable the CAN Interrupts */
			*((volatile uint32*)INT_REQMASK) &= ~ISR_13_HECCA;

			/* software reset */
			stSys.SysECR_UW = 0xc000u;
			break;
		}
		default:
		{
			break;
		}
	}
}

/* End Of File SDS_UDS_CB_C1.c */
