/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/
/** 
 * \file 	SDS_UDS_Service_DTC_C1.c
 * \brief	This file contains the program to xxxx
 * \brief	This file fulfills the following requirements:
 * 			1. Requirement 1
 * 			2. Requirement 2
 * 			3. Requirement x
*/
/*-----------------------------------------------------------------------------
* $Date: 2009-08-20 19:55:57 +0200 (Do, 20 Aug 2009) $
* $Rev: 7960 $
* $Author: agrawal $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_SDS_UDS/trunk/src/SDS_UDS_Service_DTC_C1.c $
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
#include "SDS_UDS_Service_DTC_C1.id"
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
const Uint8Type DTC_Mask[MAX_NUMBER_OF_DTC_MASKIERUNG * 8] [3] = {
// Byte 0, BIT 7
{0x00,0x00,0x00},   // Byte 0 Bit 7 :   Reserved
{0xA0,0x00,0x47},   // Byte 0 Bit 6 :   Steuergerät defekt, Watchdogfehler 0xA00047
{0xA0,0x00,0x46},   // Byte 0 Bit 5 :   Steuergerät defekt, EEPROM-Fehler 0xA00046
{0xA0,0x00,0x45},   // Byte 0 Bit 4 :   Steuergerät defekt, Programmspeicherfehler/Rom-Fehler 0xA00045
{0x00,0x00,0x00},   // Byte 0 Bit 3 :   Reserved
{0xA0,0x05,0x00},   // Byte 0 Bit 2 :   Ungültiger Datensatz 0xA00500
{0xD0,0x14,0x00},   // Byte 0 Bit 1 :   Steuergerät falsch codiert 0xD01400
{0xD0,0x13,0x00},   // Byte 0 Bit 0 :   Steuergerät nicht codiert 0xD01300
// Byte 1, BIT 7
{0x00,0x00,0x00},   // Byte 1 Bit 7 :   Reserved
{0x00,0x00,0x00},   // Byte 1 Bit 6 :   Reserved
{0x00,0x00,0x00},   // Byte 1 Bit 5 :   Reserved
{0xD0,0x12,0x00},   // Byte 1 Bit 4 :   Klemme 30, oberer Grenzwert überschritten 0xD01200
{0xD0,0x11,0x00},   // Byte 1 Bit 3 :   Klemme 30, unterer Grenzwert unterschritten 0xD01100
{0x90,0x50,0x00},   // Byte 1 Bit 2 :   Funktionseinschränkung durch Übertemperatur0x905000
{0xD4,0x01,0x00},   // Byte 1 Bit 1 :   Funktionseinschränkung durch Überspannung 0xD40100
{0xD4,0x00,0x00},   // Byte 1 Bit 0 :   Funktionseinschränkung durch Unterspannung 0xD40000
// Byte 2, BIT 7
{0x00,0x00,0x00},   // Byte 2 Bit 7 :   Reserved
{0x00,0x00,0x00},   // Byte 2 Bit 6 :   Reserved
{0x00,0x00,0x00},   // Byte 2 Bit 5 :   Reserved
{0x00,0x00,0x00},   // Byte 2 Bit 4 :   Reserved
{0xC0,0x65,0x00},   // Byte 2 Bit 3 :   Infotainment CAN, keine Kommunikation 0xC06500
{0xC0,0x64,0x00},   // Byte 2 Bit 2 :   Infotainment CAN defekt 0xC06400
{0x00,0x00,0x22},   // Byte 2 Bit 1 :   Datenbus Komfort, keine Kommunikation 0x000022
{0x00,0x00,0x21},   // Byte 2 Bit 0 :   Datenbus Komfort defekt 0x000021
// Byte 3, BIT 7
{0x00,0x00,0x00},   // Byte 3 Bit 7 :   Reserved
{0x00,0x00,0x00},   // Byte 3 Bit 6 :   Reserved
{0x00,0x00,0x00},   // Byte 3 Bit 5 :   Reserved
{0xC1,0x40,0x00},   // Byte 3 Bit 4 :   Bodycomputer 1/ Bordnetzsteuergerät / el. ZE, keine Kommunikation 0xC14000
{0xC1,0x55,0x00},   // Byte 3 Bit 3 :   Kombiinstrument, keine Kommunikation 0xC15500
{0xC2,0x12,0x00},   // Byte 3 Bit 2 :   Steuergerät für Lenksäulenelektronik, keine Kommunikation 0xC21200
{0x00,0x00,0x16},   // Byte 3 Bit 1 :   Steuergerät Radio (RNS), kein Signal/Kommunikation 0x000016
{0xD0,0x09,0x00},   // Byte 3 Bit 0 :   Gateway, keine Kommunikation 0xD00900
// Byte 4, BIT 7
{0x90,0x4E,0xF0},   // Byte 4 Bit 7 :   Audiokanal rechts, Kurzschluss untereinander 0x904EF0
{0x90,0x4E,0x13},   // Byte 4 Bit 6 :   Audiokanal rechts, Unterbrechung 0x904E13
{0x90,0x4E,0x12},   // Byte 4 Bit 5 :   Audiokanal rechts, Kurzschluß nach Plus 0x904E12
{0x90,0x4E,0x11},   // Byte 4 Bit 4 :   Audiokanal rechts, Kurzschluß nach Masse 0x904E11
{0x90,0x4F,0xF0},   // Byte 4 Bit 3 :   Audiokanal links, Kurzschluss untereinander 0x904FF0
{0x90,0x4F,0x13},   // Byte 4 Bit 2 :   Audiokanal links, Unterbrechung 0x904F13
{0x90,0x4F,0x12},   // Byte 4 Bit 1 :   Audiokanal links, Kurzschluß nach Plus 0x904F12
{0x90,0x4F,0x11},   // Byte 4 Bit 0 :   Audiokanal links, Kurzschluß nach Masse 0x904F11
// Byte 5, BIT 7
{0x00,0x00,0x00},   // Byte 5 Bit 7 :   Reserved
{0x90,0x48,0x13},   // Byte 5 Bit 6 :   Mikrofon für Telefon, Unterbrechung 0x904813
{0x90,0x48,0x12},   // Byte 5 Bit 5 :   Mikrofon für Telefon, Kurzschluß nach Plus 0x904812
{0x90,0x48,0x11},   // Byte 5 Bit 4 :   Mikrofon für Telefon, Kurzschluß nach Masse 0x904811
{0x00,0x00,0x00},   // Byte 5 Bit 3 :   Reserved
{0x00,0x00,0x00},   // Byte 5 Bit 2 :   Reserved
{0x00,0x00,0x00},   // Byte 5 Bit 1 :   Reserved
{0x00,0x00,0x00},   // Byte 5 Bit 0 :   Reserved
// Byte 6, BIT 7
{0x90,0x57,0x07},   // Byte 6 Bit 7 :   Handyhalter (Cradle), mechanischer Fehler 0x905707
{0x90,0x51,0x13},   // Byte 6 Bit 6 :   Halterung für Handyhalter (Baseplate), Unterbrechung 0x905113
{0x90,0x51,0x12},   // Byte 6 Bit 5 :   Halterung für Handyhalter (Baseplate), Kurzschluß nach Plus 0x905112
{0x90,0x51,0x11},   // Byte 6 Bit 4 :   Halterung für Handyhalter (Baseplate), Kurzschluß nach Masse 0x905111
{0x00,0x00,0x00},   // Byte 6 Bit 3 :   Reserved
{0x00,0x00,0x00},   // Byte 6 Bit 2 :   Reserved
{0x90,0x53,0x15},   // Byte 6 Bit 1 :   GSM-Antenne, Unterbrechung/Kurzschluß nach Plus 0x905315
{0x90,0x53,0x11},   // Byte 6 Bit 0 :   GSM-Antenne, Kurzschluß nach Masse 0x905311
// Byte 7, BIT 7
{0x00,0x00,0x00},   // Byte 7 Bit 7 :   Reserved
{0x00,0x00,0x00},   // Byte 7 Bit 6 :   Reserved
{0x00,0x00,0x00},   // Byte 7 Bit 5 :   Reserved
{0x00,0x00,0x00},   // Byte 7 Bit 4 :   Reserved
{0x90,0x52,0x07},   // Byte 7 Bit 3 :   Bedieneinheit für Handyvorbereitung (Drei Tasten Module), mechanischer Fehler 0x905207
{0x90,0x52,0x13},   // Byte 7 Bit 2 :   Bedieneinheit für Handyvorbereitung (Drei Tasten Module), Unterbrechung 0x905213
{0x90,0x52,0x12},   // Byte 7 Bit 1 :   Bedieneinheit für Handyvorbereitung (Drei Tasten Module), Kurzschluß nach Plus 0x905212
{0x90,0x52,0x11}    // Byte 7 Bit 0 :   Bedieneinheit für Handyvorbereitung (Drei Tasten Module), Kurzschluß nach Masse 0x905211
};

Uint8Type DTC_Maskiert[MAX_NUMBER_OF_DTC_MASKIERUNG * 8] [3];

Uint8Type u8DTC_Maskierung[MAX_NUMBER_OF_DTC_MASKIERUNG] = { 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
Uint8Type SDS_UDS_u8DTC_Mask_Counter = 0;

/*----------------------------------------------------------------------------*/
/* Private Manifest Constant Definitions                                      */
/*----------------------------------------------------------------------------*/
/* return values of SDSext functions  */


/*----------------------------------------------------------------------------*/
/* Private Types                                                              */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the type
*/

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
/* File local const Definitions                                               */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Private Address Calculations                                               */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Function    : SDS_UDS_vInitialiseService_DTC                              */
/** 
 \brief      To reset the Static variables
 
 \brief      Description:
 this function resets the static variables.

  
 \return     void 
 */
/*----------------------------------------------------------------------------*/
void SDS_UDS_vInitialiseServiceDTC(void) 
{
	;
}

/*----------------------------------------------------------------------------*/
/* Function   SDS_UDS_u16CalculateDtcMaskierung                                                 */
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
Uint16Type SDS_UDS_u16CalculateDtcMaskierung(Uint16Type index, Uint8Type *RxTxBuf)
{
	Uint16Type tmpIndex = 0;
	Uint8Type ubDTCmaskiert = FALSE;
	Uint8Type ubDTCvisible = FALSE;
	int i, j;
	Uint8Type tmpRxTxBuf[DEH_MAX_NUMBER_OF_EVENTPATHS*4];

	// Überprüfung der DTC Ausmaskierung
	ubDTCmaskiert = FALSE;
	for (i=0; i<MAX_NUMBER_OF_DTC_MASKIERUNG; i++)
    {
		if (u8DTC_Maskierung[i] != 0xFF)
        {
			ubDTCmaskiert = TRUE;
        }
    }
	if (ubDTCmaskiert == TRUE)
    {
		tmpIndex = 0;
		DEBUG_VALUE1(SDS_UDS_SWI_TRC, MOD_SDS_UDS, "Initial Index = %d", index);
		for (i=2; i<index; i+=4)
        {
			ubDTCvisible = TRUE;
			for (j=0; j<SDS_UDS_u8DTC_Mask_Counter; j++)
            {
				if ((RxTxBuf[i+0] == DTC_Maskiert[j][0]) &&
                    (RxTxBuf[i+1] == DTC_Maskiert[j][1]) &&
                    (RxTxBuf[i+2] == DTC_Maskiert[j][2]))
                {
					ubDTCvisible = FALSE; // DTC ausmaskieren
                }
			}
			if (ubDTCvisible == TRUE)
            {
				tmpRxTxBuf[tmpIndex++] = RxTxBuf[i+0];
				tmpRxTxBuf[tmpIndex++] = RxTxBuf[i+1];
				tmpRxTxBuf[tmpIndex++] = RxTxBuf[i+2];
				tmpRxTxBuf[tmpIndex++] = RxTxBuf[i+3];
			}
		}
		if (tmpIndex > 0)
        {
			for (i=0; i<tmpIndex; i++)
            {
				RxTxBuf[i+2] = tmpRxTxBuf[i];
            }
		}
		index = tmpIndex + 2;
		DEBUG_VALUE1(SDS_UDS_SWI_TRC, MOD_SDS_UDS, "Final Index = %d", index);
	}
	return index;
}


/*----------------------------------------------------------------------------*/
/* Function   SDS_UDS_bGetKundenDienstBit                                                 */
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
Uint8Type SDS_UDS_u8GetKundenDienstBit(void)
{
	tDEH_Return retVal;
	tDEH_DTCAndStatus das;
	Uint8Type j;
	Uint8Type au8TempArrayErrorNumberString[3];
	Uint8Type u8KundenDienstBit = 0x0;
	do
	{
		retVal = DEH_ReadDTCByStatus( (tDEH_DTCInfoType)0x02,(tDEH_DTCStatusMask)0x09, &das);
		au8TempArrayErrorNumberString[0] = (byte)(das.DTC>>16);
		au8TempArrayErrorNumberString[1] = (byte)(das.DTC>>8);
		au8TempArrayErrorNumberString[2] = (byte)(das.DTC);
		if ( u8KundenDienstBit  == 0x00 )
		{
			for (j=0; j<SDS_UDS_u8DTC_Mask_Counter; j++)
	        {
				if ((au8TempArrayErrorNumberString[0] == DTC_Maskiert[j][0]) &&
	                (au8TempArrayErrorNumberString[1] == DTC_Maskiert[j][1]) &&
	                (au8TempArrayErrorNumberString[2] == DTC_Maskiert[j][2]))
	            {
					break;
	            }
			}
			if ( j == SDS_UDS_u8DTC_Mask_Counter )
			{
				// End of search and no maskierung found for the given error entry
				u8KundenDienstBit = 0x01; // DTC ausmaskieren
			}
		}
	}
	while( retVal == DEH_E_DATA_STILL_AVAILABLE );

	return u8KundenDienstBit;
}
/* End Of File SDS_UDS_Service_DTC_C1.c */
