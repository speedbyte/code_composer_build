/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/
/** 
 * \file 	EEPROM_Layout_C1.c
 * \brief	This file contains the program to xxxx
 * \brief	This file fulfills the following requirements:
 * 			1. Requirement 1
 * 			2. Requirement 2
 * 			3. Requirement x
*/
/*-----------------------------------------------------------------------------
* $Date: 2010-05-10 09:40:09 +0200 (Mo, 10 Mai 2010) $
* $Rev: 12954 $
* $Author: agrawal $
* $URL: http://subversion-01/svn/SW_HW_System/Plattform/TMS470/MOD/MOD_EEPROM/branch/MOD_EEPROM_SOP2_REDESIGN_branch/src/EEPROM_Layout_C1.c $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include <osek.h>           // vector
#include <osekext.h>        // vector
#include "stdio.h"
/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/
#include "APP_COMMON_Utils.h"
#include "APP_DIAG.h"
#include "EEPROM.h"
#include "Version.h"
#include "OSEK_HostTask_H1.h"
#include "OSEK_MonitorTask_H1.h"
#include "TRACE.h"
#include "EEPROM.h"
#include "EEPROM_LAYOUT.h"
/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "EEPROM_LAYOUT_CFG.h"
#include "EEPROM_LAYOUT_generated_H1.h"
#include "EEPROM_LAYOUT_generated_H2.h"
#include "EEPROM_Layout_Cfg_CI.h"
#include "EEPROM_Layout_Cfg_C1.id"

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
/** 
\brief	description of the private constant
*/

#define EEPROM_GET_FALLBACK_LANG 0
#define EEPROM_GET_VOICE_CONTROL_PHONEBOOKSORT_ORDER 1
#define EEPROM_GET_AUDIO_PARAM 2
#define EEPROM_GET_MUTE_TYPE 3
#define EEPROM_GET_MEDIA  4
#define EEPROM_GET_AERIAL_DIAG  5
#define EEPROM_GET_BASEPLATE_DIAG  6
#define EEPROM_GET_ROOFLINE_MODULE  7

static const Uint8Type _EEPROM_au8DefaultPinNumber[]                = {0,0,0,0,'\0'};
static const Uint8Type _EEPROM_au8DefaultE2PVersion[] = "00000000";


static const Uint8Type    _EEPROM_u8DefaultFallbackLanguage = 0xA;
static const Uint8Type    _EEPROM_u8DefaultVoiceControl_PhoneBookSortOrder = 0x0;  // VCR needs this for activate sprachbedienung.
static const Uint8Type    _EEPROM_u8DefaultAudioParameterSet = 0x0; // stored coding audio parameter set value for directly request from ERM module
static const Uint8Type    _EEPROM_u8DefaultCooperativeMuteType       = 0x0;
static const BooleanType  _EEPROM_bDefaultMultimedia      = BT_TRUE; // stored coding multimedia value for directly request from VCR module
static const BooleanType  _EEPROM_bDefaultAerial          = BT_FALSE;
static const BooleanType  _EEPROM_bDefaultBaseplate       = BT_TRUE;  // Baseplate with cradlezwang
static const Uint8Type    _EEPROM_u8DefaultRooflineModuleType = 0x0;

static const Uint16Type   _EEPROM_u16DefaultMaskingLanguages = 0xFFFF;
static const Uint16Type   _EEPROM_u16DefaultMuteDelay                 = 0x0;

// static const Uint8Type _EEPROM_u8ToBeDefined = 0;
/*----------------------------------------------------------------------------*/
/* Private Address Calculations                                               */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Function    : FunctionName                                                 */
/**
    \brief      Short description

    \brief      Description:
                XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
                XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

    \param      EEP_Position

    \param      BlockSize_u16

    \param      Buffer_u8

    \return     CommonResponseCodeType
   */
/*----------------------------------------------------------------------------*/
void EEPROM_vLayoutCfgInit(void)
{
    Uint8Type u8CDMA = 0xFF;
    /*if E2P Element CDMA_Detection is uninitialized (0xFF) then initialize it with default value 0x00 == OFF*/
    EEPROM_enGetSingleByteFromRamMirror(&u8CDMA, EEPROM_LAYOUT_ENUM_CDMA_Detection);
    if( u8CDMA == 0xFF )
    {
        u8CDMA = 0x00; /*set default value, to OFF*/
        (void)EEPROM_sfRamWriteOnly(&u8CDMA,EEPROM_LAYOUT_ENUM_CDMA_Detection);
    }
}


/*----------------------------------------------------------------------------*/
/* Function    : EEPROM_enGetFallbackLanguage                              */
/**
 \brief       function for a request to get the fallback language coding

 \param		  Value

 \return      CommonResponseCodeType
 */
/*----------------------------------------------------------------------------*/
void EEPROM_LAYOUT_vGetFallbackLanguage(Uint8Type* Value)
{
	CommonResponseCodeType sF = COMMON_RSP_OK;
    *Value = EEPROM_au8DataBlock[((EEPROM_au8RamMirrorTable[EEPROM_LAYOUT_ENUM_VWCodingValue].RamShadowBlockNr)*SIZEOF_EE_PAGE+EEPROM_au8RamMirrorTable[EEPROM_LAYOUT_ENUM_VWCodingValue].Offset)+ EEPROM_GET_FALLBACK_LANG];
    if ( sF == COMMON_RSP_ERROR)
    {
    	*Value = _EEPROM_u8DefaultFallbackLanguage;
    }
}
/*----------------------------------------------------------------------------*/
/* Function    : EEPROM_enGetAudioParameterSet                              */
/**
 \brief       function for a request in ERM module to get the audio parameter set

 \param		  Value

 \return      CommonResponseCodeType
 */
/*----------------------------------------------------------------------------*/
void EEPROM_LAYOUT_vGetAudioParameterSet(Uint8Type* Value)
{
	CommonResponseCodeType sF = COMMON_RSP_OK;
    *Value = EEPROM_au8DataBlock[((EEPROM_au8RamMirrorTable[EEPROM_LAYOUT_ENUM_VWCodingValue].RamShadowBlockNr)*SIZEOF_EE_PAGE+EEPROM_au8RamMirrorTable[EEPROM_LAYOUT_ENUM_VWCodingValue].Offset)+ EEPROM_GET_AUDIO_PARAM];
    if ( sF == COMMON_RSP_ERROR)
    {
    	*Value = _EEPROM_u8DefaultAudioParameterSet;
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : EEPROM_LAYOUT_vGetPlayerEnabled                              */
/**
 \brief       function for a request in MDI module to get the multimedia coding

 */
/*----------------------------------------------------------------------------*/
void EEPROM_LAYOUT_vGetPlayerEnabled(BooleanType * Value)
{
	CommonResponseCodeType sF = COMMON_RSP_OK;
	Uint8Type data;
    data = EEPROM_au8DataBlock[((EEPROM_au8RamMirrorTable[EEPROM_LAYOUT_ENUM_VWCodingValue].RamShadowBlockNr)*32+EEPROM_au8RamMirrorTable[EEPROM_LAYOUT_ENUM_VWCodingValue].Offset)+ EEPROM_GET_MEDIA];
	if ( data == 0x0 )
	{
		*Value = BT_TRUE;
	}
	else if ( data == 0x1)
	{
		*Value = BT_FALSE;
	}

    if ( sF == COMMON_RSP_ERROR)
    {
    	*Value = _EEPROM_bDefaultMultimedia;
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : EEPROM_enGetVoiceControl                                     */
/**
 \brief       function for a request in VCR module to get the voice control

 \param		  Value

 \return      void

 */
/*----------------------------------------------------------------------------*/
void EEPROM_LAYOUT_vGetVoiceControl(Uint8Type * Value)
{
	CommonResponseCodeType sF = COMMON_RSP_OK;

	*Value = EEPROM_au8DataBlock[((EEPROM_au8RamMirrorTable[EEPROM_LAYOUT_ENUM_VWCodingValue].RamShadowBlockNr)*32+EEPROM_au8RamMirrorTable[EEPROM_LAYOUT_ENUM_VWCodingValue].Offset)+ EEPROM_GET_VOICE_CONTROL_PHONEBOOKSORT_ORDER];
    if (sF == COMMON_RSP_ERROR)
    {
    	*Value = _EEPROM_u8DefaultVoiceControl_PhoneBookSortOrder;
    }
    /* Low nibble is  Voice control */
    *Value = *Value & 0x0F;
}


/*----------------------------------------------------------------------------*/
/* Function    : EEPROM_LAYOUT_vGetPhonebookSortOrder                         */
/**
 \brief       function for request coding information for phonebook sort order

 \param		 Value

 \return     void

 */
/*----------------------------------------------------------------------------*/
void EEPROM_LAYOUT_vGetPhonebookSortOrder(Uint8Type * Value)
{
	CommonResponseCodeType sF = COMMON_RSP_OK;

    *Value = EEPROM_au8DataBlock[((EEPROM_au8RamMirrorTable[EEPROM_LAYOUT_ENUM_VWCodingValue].RamShadowBlockNr)*32+EEPROM_au8RamMirrorTable[EEPROM_LAYOUT_ENUM_VWCodingValue].Offset)+ EEPROM_GET_VOICE_CONTROL_PHONEBOOKSORT_ORDER];
    if ( sF == COMMON_RSP_ERROR)
    {
    	*Value = _EEPROM_u8DefaultVoiceControl_PhoneBookSortOrder;
    }
    /* High nibble is phonebook sort order */
    *Value = ( *Value & 0xF0)>> 4;
}


/*----------------------------------------------------------------------------*/
/* Function    : EEPROM_enGetMaskingLanguage                              */
/**
 \brief       function for a request to get the masking languages


 \param		  Value

 \return      CommonResponseCodeType

 */
/*----------------------------------------------------------------------------*/
CommonResponseCodeType EEPROM_enGetMaskingLanguage(Uint16Type* Value)
{
	    RamMirrorRequest bmReadEeprom;
	    CommonResponseCodeType sF = COMMON_RSP_OK;
	    bmReadEeprom.Eeprom_id = EEPROM_LAYOUT_ENUM_MaskingOfLanguages;
	    bmReadEeprom.payload_ptr.u16_value = Value;
	    bmReadEeprom.payload_ptr.byte_seq.length = 2;
	    sF = EEPROM_enGetEepromValues(&bmReadEeprom);
	    if ( sF == COMMON_RSP_ERROR)
	    {
	    	*Value = _EEPROM_u16DefaultMaskingLanguages;
	    }
	    return sF;
}


/*----------------------------------------------------------------------------*/
/* Function    : EEPROM_enGetRooflineMode                              */
/**
 \brief       function for a request to get the Roofline Mode

 \param		  Value

 \return      CommonResponseCodeType

 */
/*----------------------------------------------------------------------------*/
void EEPROM_LAYOUT_vGetRooflineMode(Uint8Type* Value)
{
	CommonResponseCodeType sF = COMMON_RSP_OK;
    *Value = EEPROM_au8DataBlock[((EEPROM_au8RamMirrorTable[EEPROM_LAYOUT_ENUM_VWCodingValue].RamShadowBlockNr)*SIZEOF_EE_PAGE+EEPROM_au8RamMirrorTable[EEPROM_LAYOUT_ENUM_VWCodingValue].Offset)+ EEPROM_GET_ROOFLINE_MODULE];
    if ( sF == COMMON_RSP_ERROR)
    {
    	*Value = _EEPROM_u8DefaultRooflineModuleType;
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : EEPROM_enGetAntennaDiag                              */
/**
 \brief       function for a request to get the Antenna Mode

 \param		  Value

 \return      CommonResponseCodeType

 */
/*----------------------------------------------------------------------------*/
void EEPROM_LAYOUT_vGetAntennaDiag(BooleanType* Value)
{
	CommonResponseCodeType sF = COMMON_RSP_OK;
	Uint8Type data;
    data = EEPROM_au8DataBlock[((EEPROM_au8RamMirrorTable[EEPROM_LAYOUT_ENUM_VWCodingValue].RamShadowBlockNr)*SIZEOF_EE_PAGE+EEPROM_au8RamMirrorTable[EEPROM_LAYOUT_ENUM_VWCodingValue].Offset)+ EEPROM_GET_AERIAL_DIAG];
	if ( data == 0x0 )
	{
		*Value = BT_TRUE;
	}
	else if ( data == 0x1)
	{
		*Value = BT_FALSE;
	}
    if ( sF == COMMON_RSP_ERROR)
    {
    	*Value = _EEPROM_bDefaultAerial;
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : EEPROM_enGetAntennaDiag                              */
/**
 \brief       function for a request to get the Baseplate Mode

 \param		  Value

 \return      void

 */
/*----------------------------------------------------------------------------*/
void EEPROM_LAYOUT_vGetBaseplateDiag(BooleanType* Value)
{
	CommonResponseCodeType sF = COMMON_RSP_OK;
    Uint8Type tmpValue;
    tmpValue = EEPROM_au8DataBlock[((EEPROM_au8RamMirrorTable[EEPROM_LAYOUT_ENUM_VWCodingValue].RamShadowBlockNr)*SIZEOF_EE_PAGE+EEPROM_au8RamMirrorTable[EEPROM_LAYOUT_ENUM_VWCodingValue].Offset)+ EEPROM_GET_BASEPLATE_DIAG];
    if ( sF == COMMON_RSP_ERROR)
    {
    	tmpValue = _EEPROM_bDefaultBaseplate;
    }
    if ( (tmpValue == 0x0) || (tmpValue == 0x2) )
    {
    	*Value = BT_TRUE;
    }
    else
    {
    	*Value = BT_FALSE;
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : EEPROM_enGetCradleProtection                          */
/**
    \brief      Get configured value for Cradle protection

    \return     BooleanType

   */
/*----------------------------------------------------------------------------*/
void EEPROM_LAYOUT_vGetCradleProtection(BooleanType *Value)
{
	CommonResponseCodeType sF = COMMON_RSP_OK;
	Uint8Type tmpValue;
    tmpValue = EEPROM_au8DataBlock[((EEPROM_au8RamMirrorTable[EEPROM_LAYOUT_ENUM_VWCodingValue].RamShadowBlockNr)*32+EEPROM_au8RamMirrorTable[EEPROM_LAYOUT_ENUM_VWCodingValue].Offset)+ EEPROM_GET_BASEPLATE_DIAG];
    if ( sF == COMMON_RSP_ERROR)
    {
    	tmpValue = _EEPROM_bDefaultBaseplate;
    }
    if ( (tmpValue == 0x0) || (tmpValue == 0x1) )
    {
    	*Value = BT_FALSE;
    }
    else
    {
    	*Value = BT_TRUE;
    }
}


/*----------------------------------------------------------------------------*/
/* Function    : EEPROM_enGetMuteDelay                              */
/**
 \brief       function for a request to get the Baseplate Mode

 \param		  Value

 \return      CommonResponseCodeType
 */
/*----------------------------------------------------------------------------*/
CommonResponseCodeType EEPROM_enGetMuteDelay(Uint16Type* Value)
{
	CommonResponseCodeType sF = COMMON_RSP_OK;
	RamMirrorRequest bmReadEeprom;
    bmReadEeprom.Eeprom_id = EEPROM_LAYOUT_ENUM_MuteDelay;
    bmReadEeprom.payload_ptr.u16_value = Value;
    bmReadEeprom.payload_ptr.byte_seq.length = 1;
    sF = EEPROM_enGetEepromValues(&bmReadEeprom);
    if ( sF == COMMON_RSP_ERROR )
    {
    	*Value = _EEPROM_u16DefaultMuteDelay;
    }
	return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : EEPROM_enGetCooperativeMuteType                              */
/**
 \brief       function for a request to get the Baseplate Mode

 \param		  Value

 \return      CommonResponseCodeType

 */
/*----------------------------------------------------------------------------*/
void EEPROM_LAYOUT_vGetCooperativeMuteType(Uint8Type* Value)
{
	CommonResponseCodeType sF = COMMON_RSP_OK;
    *Value = EEPROM_au8DataBlock[((EEPROM_au8RamMirrorTable[EEPROM_LAYOUT_ENUM_VWCodingValue].RamShadowBlockNr)*32+EEPROM_au8RamMirrorTable[EEPROM_LAYOUT_ENUM_VWCodingValue].Offset)+ EEPROM_GET_MUTE_TYPE];
    if ( sF == COMMON_RSP_ERROR )
    {
    	*Value = _EEPROM_u8DefaultCooperativeMuteType;
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : EEPROM_enGetCDMADetection                              */
/**
 \brief       function for a request to get the CDMA_Detection Mode

 \param       Value

 \return      CommonResponseCodeType

 */
/*----------------------------------------------------------------------------*/
void EEPROM_LAYOUT_vGetCDMADetection(Uint8Type* Value)
{
    CommonResponseCodeType sF;
    sF = EEPROM_enGetSingleByteFromRamMirror(Value, EEPROM_LAYOUT_ENUM_CDMA_Detection);
    if(sF == COMMON_RSP_ERROR)
    {
        *Value = 0x00; /*set default value, to OFF*/
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : EEPROM_enGetBluetoothPin                              */
/**
 \brief       function for a request to get Bluetooth pin

 \param		  pau8BTPinValue

 \param		  number_len

 \return	  CommonResponseCodeType

 */
/*----------------------------------------------------------------------------*/
CommonResponseCodeType EEPROM_enGetBluetoothPin(Uint8Type* pau8BTPinValue, Uint8Type number_len)
{
	CommonResponseCodeType sF;
	RamMirrorRequest bmReadEeprom;
	Uint8Type u8TempPinCodeLen = 0;
	Uint8Type u8tmpdata[ID_2E_2420_BLUETOOTH_SET_PIN_CODE_MAX_LEN];
	Uint8Type i=0;
    bmReadEeprom.Eeprom_id = EEPROM_LAYOUT_ENUM_BluetoothSetPINCode;
    bmReadEeprom.payload_ptr.byte_seq.data = u8tmpdata;
    bmReadEeprom.payload_ptr.byte_seq.length = ID_2E_2420_BLUETOOTH_SET_PIN_CODE_MAX_LEN;
    sF = EEPROM_enGetEepromValues(&bmReadEeprom);
    if ( sF == COMMON_RSP_OK )
    {
    	while ( i < 2 ) 
    	{
    		pau8BTPinValue[u8TempPinCodeLen] = (((*(bmReadEeprom.payload_ptr.byte_seq.data +i)) & 0xF0) >> 4);
    		if ( pau8BTPinValue[u8TempPinCodeLen]  >  0x9 ) 
    		{
    			sF = COMMON_RSP_ERROR;
#ifndef USE_DEBUGGING_WITH_TRACE_ID
    			(void)DEBUG_Printf(UHV_WARNING, UHV_EEPROM, "Invalid BTPIN-EEPROM");
#endif
    			break;
    		}
    		u8TempPinCodeLen++;
    		pau8BTPinValue[u8TempPinCodeLen] = ((*(bmReadEeprom.payload_ptr.byte_seq.data +i)) & 0x0F);
    		if ( pau8BTPinValue[u8TempPinCodeLen]  >  0x9 ) 
    		{
    			sF = COMMON_RSP_ERROR;
#ifndef USE_DEBUGGING_WITH_TRACE_ID
    			(void)DEBUG_Printf(UHV_WARNING, UHV_EEPROM, "Invalid BTPIN-EEPROM");
#endif
    			break;
    		}
	    	u8TempPinCodeLen++;
	    	i++;
    	}
    	pau8BTPinValue[u8TempPinCodeLen] = '\0';

    }
    if ( sF == COMMON_RSP_ERROR )
    {
    	(void)memcpy(pau8BTPinValue, _EEPROM_au8DefaultPinNumber, (AT_MAX_PIN_CODE_LEN+1));
#ifndef USE_DEBUGGING_WITH_TRACE_ID
    	(void)DEBUG_Printf(UHV_WARNING, UHV_EEPROM, "DefaultPin=0000");
#endif

    }
	return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : EEPROM_enGetEepromLayoutVersionNumber                  */
/**
 \brief       function for request to get Eeprom Layout sw version number

 \param		  ptrau8Value

 \param		  number_len

 \return      CommonResponseCodeType
 */
/*----------------------------------------------------------------------------*/
CommonResponseCodeType EEPROM_enGetEepromLayoutVersionNumber(Uint8Type* ptrau8Value, Uint8Type number_len)
{
	CommonResponseCodeType sF;
	RamMirrorRequest bmReadEeprom;
	Uint8Type u8TempVersionLen = 0;
	Uint8Type u8tmpdata[VWDB_EE_ADDDRESS_UHV_EEPROM_Version_LEN];
	Uint8Type i=0;
    bmReadEeprom.Eeprom_id = EEPROM_LAYOUT_ENUM_UHV_EEPROM_Version;
    bmReadEeprom.payload_ptr.byte_seq.data = u8tmpdata;
    bmReadEeprom.payload_ptr.byte_seq.length = VWDB_EE_ADDDRESS_UHV_EEPROM_Version_LEN;
    sF = EEPROM_enGetEepromValues(&bmReadEeprom);
    if ( sF == COMMON_RSP_OK )
    {
    	while ( i < 4 )
    	{
    		ptrau8Value[u8TempVersionLen] = (((*(bmReadEeprom.payload_ptr.byte_seq.data +i)) & 0xF0) >> 4);
    		if ( ptrau8Value[u8TempVersionLen]  >  0x9 )
    		{
    			sF = COMMON_RSP_ERROR;

                DEBUG_TEXT(EEPROM_LAYOUT_SWI_ERR, MOD_EEPROM_LAYOUT, "Invalid E2PVersion");
    			break;
    		}
    		u8TempVersionLen++;
    		ptrau8Value[u8TempVersionLen] = ((*(bmReadEeprom.payload_ptr.byte_seq.data +i)) & 0x0F);
    		if ( ptrau8Value[u8TempVersionLen]  >  0x9 )
    		{
    			sF = COMMON_RSP_ERROR;

                DEBUG_TEXT(EEPROM_LAYOUT_SWI_WNG, MOD_EEPROM_LAYOUT, "Invalid E2PVersion");
    			break;
    		}
	    	u8TempVersionLen++;
	    	i++;
    	}
        // copy len number of digits
        for (i=0;i<VWDB_EE_ADDDRESS_UHV_EEPROM_Version_LEN_ASCII;i++)
        {   /* convert number to ascii character */
        	ptrau8Value[i] = ptrau8Value[i] + 0x30;
        }
        ptrau8Value[u8TempVersionLen] = '\0';
#ifndef USE_DEBUGGING_WITH_TRACE_ID
    	(void)DEBUG_Printf(UHV_DEBUG_1, UHV_EEPROM, "Version=%s", ptrau8Value);
#endif
    }
    if ( sF == COMMON_RSP_ERROR )
    {
    	(void)memcpy(ptrau8Value, _EEPROM_au8DefaultE2PVersion, (VWDB_EE_ADDDRESS_UHV_EEPROM_Version_LEN_ASCII+1));

        DEBUG_TEXT(EEPROM_LAYOUT_SWI_WNG, MOD_EEPROM_LAYOUT, "DefaultE2PVersion=0");
    }
	return sF;
}



/* End Of File EEPROM_Layout_Cfg_C1.c */
