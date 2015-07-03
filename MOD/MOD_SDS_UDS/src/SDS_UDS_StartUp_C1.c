/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/
/** 
 * \file 	SDS_UDS_StartUp_C1.c
 * \brief	This file contains the program to xxxx
 * \brief	This file fulfills the following requirements:
 * 			1. Requirement 1
 * 			2. Requirement 2
 * 			3. Requirement x
*/
/*-----------------------------------------------------------------------------
* $Date: 2010-12-08 09:41:12 +0100 (Mi, 08 Dez 2010) $
* $Rev: 16812 $
* $Author: agrawal $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_SDS_UDS/trunk/src/SDS_UDS_StartUp_C1.c $
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
#include "APP_DIAG.h"
#include "can_def.h"
#include "can_cfg.h"
#include "crcmgr.h"
#include "deh_dtc_cfg.h"
#include "sds_cfg.h"
#include "OSEK_HostTask_H1.h"
#include "CSHDL.h"
#include "TRACE.h"
#include "SDS_UDS_StartUp_C1.id"
#include "EEPROM_LAYOUT_CFG.h"
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

/*----------------------------------------------------------------------------*/
/* Private Manifest Constant Definitions                                      */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private Types                                                              */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the type
*/

typedef struct {
  uint16_t     identifier;
  uint16_t     eeprom;
  uint16_t     length;
} t_DiagCalibrationData;

typedef enum
{
	enCurrentStatusStart,
	enCurrentStatusReadSecurity,
	enCurrentStatusService86,
	enCurrentStatusDtcMaskierung,
	enCurrentStatusStartupFinished
} SDS_UDS_ENUM_START_STATES ;

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
/* File local const Definitions                                               */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the private constant
*/

/*----------------------------------------------------------------------------*/
/* File local Variables                                                       */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the private variable
*/

static SDS_UDS_ENUM_START_STATES _SDS_UDS_enCurrentStatus = enCurrentStatusStart;
/*----------------------------------------------------------------------------*/
/* Private Address Calculations                                               */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Function   SDS_UDS_u8ProcessorStartUpDiagnoseSequenceame                                                 */
/** 
    \brief      Short description
  
    \brief      Description:
                XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX 
                XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX 

    \param      InputParameterName1
                InputParameterType1
                value limits
    \param      InputParameterName2
                InputParameterType2
                value limits
    \return     OutputParameterType
                OutputParameterType description
                value limits
   */
/*----------------------------------------------------------------------------*/
Uint8Type SDS_UDS_u8ProcessorStartUpDiagnoseSequence(Uint8Type ubReadInitValue )
{
	Uint8Type i,j;
	Uint8Type u8TmpDTC;
	Uint8Type u8BootloaderTimeLockStateInfo = 0;

	if ( _SDS_UDS_enCurrentStatus == enCurrentStatusStart )
	{
		DEBUG_TEXT(SDS_UDS_SWI_TRC,MOD_SDS_UDS,"STARTUP_DIAGNOSE: Start");
		/* first get security status from application and Bootloader */
		(void)EEPROM_enGetSingleByteFromRamMirror(&ubSecState, EEPROM_LAYOUT_ENUM_SecurityAccessState);
		/* Now check  security status from application and bootloader */
		/* Following bootloader Security State are important          */
		/*  0   -    No security lock                                 */
		/*  >= 3  -  Security Lock                                    */
		u8BootloaderTimeLockStateInfo=SDS_UDS_u8GetBootloaderTimeLockState();
		DEBUG_VALUE1(SDS_UDS_SWI_TRC, MOD_SDS_UDS, "Read: BL TimeLockState 0x%02X", u8BootloaderTimeLockStateInfo);
		if (u8BootloaderTimeLockStateInfo == 0)
		{
			ubSecState &= MASK_SEC_STATUS_UNLOCKED; // die Key Sperre aufheben
		}
		ubSecState &= ~MASK_SEED_ALREADY_SEND;   // dieser Zustand wird nicht ins EEPROM gespeichert, bzw. beim Zurücklesen nach Systemstart ausmaskiert
		if(((ubSecState & MASK_SEC_STATUS_LOCKED) == MASK_SEC_STATUS_LOCKED) ||  (BOOTLOADER_TIMELOC_STATE_LOCKED <= u8BootloaderTimeLockStateInfo))
		{
			/* Setting Security Status to State LOCKED */
			TimerSecurity = SEC_TIMER_DELAY;
			ubSecState |= MASK_SEC_STATUS_LOCKED; // dreimal falscher Key => für 10min sperren !!!
			DEBUG_TEXT(SDS_UDS_SWI_TRC,MOD_SDS_UDS,"Security Status is locked");
	    }
		ubSecState = ((MASK_HIDE_SEED_COUNTER & ubSecState) | (MASK_SEED_COUNTER & u8BootloaderTimeLockStateInfo));
		// ubSecState ins EEPROM speichern
		(void)EEPROM_sfRamWriteOnly(&ubSecState, EEPROM_LAYOUT_ENUM_SecurityAccessState);
		DEBUG_VALUE1(SDS_UDS_SWI_TRC, MOD_SDS_UDS, "ubSecState 0x%02X",  ubSecState);

#ifdef SDSEXT_SERVICE86_ALLOWED
		_SDS_UDS_enCurrentStatus = enCurrentStatusService86;
#else
		_SDS_UDS_enCurrentStatus = enCurrentStatusDtcMaskierung;
#endif
		DEBUG_TEXT(SDS_UDS_SWI_TRC,MOD_SDS_UDS,"STARTUP_DIAGNOSE: Security Status Ready");
	}
#ifdef SDSEXT_SERVICE86_ALLOWED  
	// und die Daten vom Service $86 aus dem EEPROM laden
	else if ( _SDS_UDS_enCurrentStatus == enCurrentStatusService86 )
	{
		(void)EEPROM_enGetByteSequenceFromRamMirror (&u8Service86DataValues[0], VWDB_EE_ADDDRESS_Service86ResponseOnEvent_LEN, EEPROM_LAYOUT_ENUM_Service86ResponseOnEvent );
		if (u8Service86DataValues[Service86EventWindowTime] == Service86EventWindowTime_InfiniteTimeToResponse)
			u8Service86DataValuesInitStatus = Service86init;   // Der UDS-Service ResponseOnEvent ist auf unbestimmte Zeit aktiviert und
		// unabhängig von einem PowerDown-Reset des Servers

		for (i=0; i<(DEH_MAX_NUMBER_OF_EVENTPATHS*3); i++)
			u8Service86DataValuesDTC[i] = 0;
		_SDS_UDS_enCurrentStatus = enCurrentStatusDtcMaskierung;
		DEBUG_TEXT(SDS_UDS_SWI_TRC,MOD_SDS_UDS,"STARTUP_DIAGNOSE: Service 86 Ready");
	}
#endif
	// und die Daten von der DTC Ausmaskierung aus dem EEPROM laden
	else if ( _SDS_UDS_enCurrentStatus == enCurrentStatusDtcMaskierung )
	{
		(void)EEPROM_enGetByteSequenceFromRamMirror (&u8DTC_Maskierung[0], VWDB_EE_ADDDRESS_DTC_Maskierung_LEN, EEPROM_LAYOUT_ENUM_DTC_Maskierung);
		SDS_UDS_u8DTC_Mask_Counter = 0;
		for (i=0; i<MAX_NUMBER_OF_DTC_MASKIERUNG; i++)
		{
			u8TmpDTC = u8DTC_Maskierung[i];
			for (j=0; j<8;j++)
			{
				if ((u8TmpDTC & DTC_MASK_BIT) == DTC_CLEAR_MASK_BIT)
				{  // hier den ausmaskierten DTC speichern
					DTC_Maskiert[SDS_UDS_u8DTC_Mask_Counter][0] = DTC_Mask[i*8+j][0];
					DTC_Maskiert[SDS_UDS_u8DTC_Mask_Counter][1] = DTC_Mask[i*8+j][1];
					DTC_Maskiert[SDS_UDS_u8DTC_Mask_Counter][2] = DTC_Mask[i*8+j][2];
					SDS_UDS_u8DTC_Mask_Counter++;
				}
				u8TmpDTC = u8TmpDTC << 1;
			}
		}
		DEBUG_TEXT(SDS_UDS_SWI_TRC,MOD_SDS_UDS,"STARTUP_DIAGNOSE: DTC Masking Ready");
		_SDS_UDS_enCurrentStatus = enCurrentStatusStartupFinished;
	}
	else if ( _SDS_UDS_enCurrentStatus == enCurrentStatusStartupFinished )
	{

		_SDS_UDS_enCurrentStatus = enCurrentStatusStartupFinished;
		ubReadInitValue |= MASK_CODING_RUNNING;
		ubReadInitValue |= MASK_CODING_READY;
		ubReadInitValue |= MASK_CALIBRATION_READY;                         // geschafft...
		ubReadInitValue |= MASK_CALIBRATION_RUNNING;                       // geschafft...
		DEBUG_TEXT(SDS_UDS_SWI_TRC,MOD_SDS_UDS,"STARTUP_DIAGNOSE: Finished");
	}
return ubReadInitValue;
}

/* End Of File SDS_UDS_StartUp_C1.c */


