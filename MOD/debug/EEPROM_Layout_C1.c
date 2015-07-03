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
* $Date: 2011-04-20 18:17:44 +0200 (Mi, 20 Apr 2011) $
* $Rev: 19248 $
* $Author: agrawal $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_EEPROM_LAYOUT/trunk/src/EEPROM_Layout_C1.c $
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
#include "EEPROM.h"
#include "OSEK_HostTask_H1.h"
#include "OSEK_MonitorTask_H1.h"
#include "TRACE.h"
#include "EEPROM.h"
#include "EEPROM_LAYOUT_CFG.h"
#ifdef VW_UHV_EEPROM
#include "sds_ext.h"
#endif
/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "EEPROM_LAYOUT.h"
#include "EEPROM_LAYOUT_generated_H1.h"
#include "EEPROM_Layout_CI.h"
#include "EEPROM_Layout_C1.id"

/*----------------------------------------------------------------------------*/
/* pragma Directives                                                          */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Definitions (avoid as much as possible!)                  */
/*----------------------------------------------------------------------------*/
const Uint8Type EEPROM_astBlockTable[64] =
{
		RAMBLOCK_TO_E2PBLOCK_MAP
};

BooleanType EEPROM_LAYOUT_bAllBlocksEmpty = BT_TRUE;
/*----------------------------------------------------------------------------*/
/* Private Manifest Constant Definitions                                      */
/*----------------------------------------------------------------------------*/

const EEPROM_ST_t_EepromRamMirrorData_Blockwise EEPROM_au8RamMirrorTable[ENUM_EEPROM_LAYOUT_MAX] =
{
		EEPROM_OFFSET_TABLE
};

static const Uint8Type _EEPROM_au8E2PBlockDetails[ENUM_EEPROM_LAYOUT_MAX] =
{
		EEPROM_BLOCK_DETAILS
};

static const EEPROM_ELEM _EEPROM_ElemValues[MAX_EEPROM_WRITEPROTECT_ELEM] =
{
        EEPROM_LAYOUT_PROTECTED_TABLE
};
/*----------------------------------------------------------------------------*/
/* Private Types                                                              */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the type
*/

/*----------------------------------------------------------------------------*/
/* Prototypes of file local Methods                                           */
/*----------------------------------------------------------------------------*/
static void _EEPROM_LAYOUT_cbE2PWriteComplete(CommonResponseCodeType retVal);

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
static volatile Uint32Type _EEPROM_u32FlagEEPROMLayout_0_31_Blocks;
static volatile Uint32Type _EEPROM_u32FlagEEPROMLayout_32_63_Blocks;
static BooleanType _EEPROM_LAYOUT_bWriteCompleted = BT_TRUE;
/*----------------------------------------------------------------------------*/
/* File local const Definitions                                               */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the private constant
*/
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

    \param		Buffer_u8

    \return     CommonResponseCodeType
   */
/*----------------------------------------------------------------------------*/
void EEPROM_vLayoutInit(void)
{
    Uint8Type i = 0;
    Uint8Type *ptrToDataBlock = EEPROM_au8DataBlock;
    EEDeviceControlBlockType    *pEEHost;
    pEEHost = (EEDeviceControlBlockType *)getEEHostControl();
    DEBUG_VALUE1(EEPROM_LAYOUT_SWI_TRC, MOD_EEPROM_LAYOUT, "SYS_PERF:EEPROM ReadStart:%dms",osGetSystemCounter());
    /* 0<<0 + 0<<1 + 0<<2 + 1<<3 +  ... */
    _EEPROM_u32FlagEEPROMLayout_0_31_Blocks =
				INIT_BLOCK_0_31;
    		_EEPROM_u32FlagEEPROMLayout_32_63_Blocks =
    				INIT_BLOCK_32_63;
    for ( i = 0; i < 32 ;i++)
    {
		if ( ((_EEPROM_u32FlagEEPROMLayout_0_31_Blocks>>i) & 0x01 ) == 0x01 )
		{
			(void)EEPROM_sfRead(pEEHost,(EEAddressType)i*SIZEOF_EE_PAGE, (Uint8Type *)ptrToDataBlock, SIZEOF_EE_PAGE);
			ptrToDataBlock = ptrToDataBlock + SIZEOF_EE_PAGE;
		}
    }
	for ( i = 32; i < 64 ;i++)
	{
		if ( ((_EEPROM_u32FlagEEPROMLayout_32_63_Blocks>>(i-32)) & 0x01 ) == 0x01 )
		{
			(void)EEPROM_sfRead(pEEHost,(EEAddressType)i*SIZEOF_EE_PAGE, (Uint8Type *)ptrToDataBlock, SIZEOF_EE_PAGE);
			ptrToDataBlock = ptrToDataBlock + SIZEOF_EE_PAGE;
		}
	}
    DEBUG_VALUE2(EEPROM_LAYOUT_SWI_TRC, MOD_EEPROM_LAYOUT,  "SYS_PERF:EEPROM ReadEnd:%dms %d",osGetSystemCounter(),i);
    _EEPROM_u32FlagEEPROMLayout_0_31_Blocks = 0;
    _EEPROM_u32FlagEEPROMLayout_32_63_Blocks = 0;
}

/*----------------------------------------------------------------------------*/
/* Function    : EEPROM_sfRamWriteOnly                                 */
/**
    \brief      Write Values in the RAM and then in the EEPROM

    \param      *Value
				Uint8Type

    \return     CommonResponseCodeType
   */
/*----------------------------------------------------------------------------*/
SuccessFailType EEPROM_sfRamWriteOnly(Uint8Type *data,EEPROM_ENUM_GET_EEPROM enAreaToWrite)
{
	SuccessFailType sF = SUCCESS;
	Uint8Type i;
    Uint8Type j = 0;
	Uint8Type count;
	if  (enAreaToWrite < ENUM_EEPROM_LAYOUT_MAX)
	{
		i = enAreaToWrite;
		if ( EEPROM_au8RamMirrorTable[i].RamShadowBlockNr == EEPROM_RAMBLOCK_NOTDEFINED)
		{
			sF = FAIL;
			DEBUG_TEXT(EEPROM_LAYOUT_SWI_ERR, MOD_EEPROM_LAYOUT, "No Block defined in RAM Shadow");
			return sF;
		}
	}
	else
	{
		DEBUG_VALUE1(EEPROM_LAYOUT_SWI_ERR, MOD_EEPROM_LAYOUT, "Exceeding Enum value %d Please synchronise the enum and excel sheet", enAreaToWrite);
		sF = FAIL;
		return sF;
	}
	if ( sF == SUCCESS)
	{
		EEPROM_LAYOUT_bAllBlocksEmpty = BT_FALSE;
		count = ((EEPROM_au8RamMirrorTable[i].Offset +  EEPROM_au8RamMirrorTable[i].Len ) / 32) + 1;
		(void)memcpy(&EEPROM_au8DataBlock[((EEPROM_au8RamMirrorTable[i].RamShadowBlockNr)*SIZEOF_EE_PAGE+EEPROM_au8RamMirrorTable[i].Offset)], data, EEPROM_au8RamMirrorTable[i].Len);
		DEBUG_STATE2(EEPROM_LAYOUT_SWI_TRC, MOD_EEPROM_LAYOUT, "Saved Value in RAM for enum $EEPROM_ENUM_GET_EEPROM$ %d with length %d", enAreaToWrite, EEPROM_au8RamMirrorTable[i].Len);
		do
		{
			if ( (_EEPROM_au8E2PBlockDetails[i]+j) < 32)
			{
				semP(EEPROM_LAYOUT_LockResource);
				_EEPROM_u32FlagEEPROMLayout_0_31_Blocks |= (1 << (_EEPROM_au8E2PBlockDetails[i]+j));
				semV(EEPROM_LAYOUT_LockResource);
				DEBUG_VALUE1(EEPROM_LAYOUT_SWI_TRC, MOD_EEPROM_LAYOUT, "Raising Flag for E2P Write %d", _EEPROM_au8E2PBlockDetails[i]+j);
				j = j+1;
			}
			else if ( (_EEPROM_au8E2PBlockDetails[i]+j) < 64)
			{
				semP(EEPROM_LAYOUT_LockResource);
				_EEPROM_u32FlagEEPROMLayout_32_63_Blocks |= (1 << (_EEPROM_au8E2PBlockDetails[i]+j - 32 ));
				semV(EEPROM_LAYOUT_LockResource);
				DEBUG_VALUE1(EEPROM_LAYOUT_SWI_TRC, MOD_EEPROM_LAYOUT, "Raising Flag for E2P Write %d", _EEPROM_au8E2PBlockDetails[i]+j);
				j = j+1;
			}
			else
			{
				DEBUG_TEXT(EEPROM_LAYOUT_SWI_ERR, MOD_EEPROM_LAYOUT, "Failed writing into EEPROM Flags");
				break;
			}
		} while ( --count > 0);
	}
	return ( sF );
}

/*----------------------------------------------------------------------------*/
/* Function    : IsElemInBlock                                        */
/**
    \brief      Check if a specified element is in specified Block

	\brief      Check if a specified element is in specified Block

    \param      EEP_ElementPosition
                Uint16Type

	\param      ElementSize_u16
                Uint16Type

    \param      EEP_BlockStartPosition
                Uint16Type

    \param      DataBlockSize_u16
                Uint16Type

    \return     BooleanType
   */
/*----------------------------------------------------------------------------*/
BooleanType IsElemInBlock(Uint16Type EEP_ElementPosition, Uint16Type ElementSize_u16,
							Uint16Type EEP_BlockStartPosition, Uint16Type DataBlockSize_u16)
{
	BooleanType bRes = BT_FALSE;

	if( (EEP_BlockStartPosition<=EEP_ElementPosition && EEP_ElementPosition<(EEP_BlockStartPosition+DataBlockSize_u16)) ||
		(EEP_ElementPosition<EEP_BlockStartPosition && EEP_BlockStartPosition<=(EEP_ElementPosition+ElementSize_u16)) )
		bRes = BT_TRUE;

	return bRes;
}

/*----------------------------------------------------------------------------*/
/* Function    : AvoidEEPROMWrite                                        */
/**
    \brief      Avoid to overwrite a specific element in EEPROM

	\brief      Avoid to overwrite a specific element in EEPROM

    \param      EEP_ElementPosition
                Uint16Type

	\param      ElementSize_u16
                Uint16Type

    \param      EEP_BlockStartPosition
                Uint16Type

    \param      DataBlockSize_u16
                Uint16Type

    \param      Buffer_u8
                Uint8Type *
                pointer to the Uint8Type

    \param      EEPROMElemData
                Uint8Type *
                pointer to the Uint8Type
   */
/*----------------------------------------------------------------------------*/
void AvoidEEPROMWrite(Uint16Type EEP_ElementPosition, Uint16Type ElementSize_u16,
		Uint16Type EEP_BlockStartPosition, Uint16Type DataBlockSize_u16, Uint8Type *Buffer_u8, Uint8Type *EEPROMElemData)
{
	Uint16Type StartPosition, EndPosition;
	Uint8Type i;

	StartPosition = ((EEP_ElementPosition<EEP_BlockStartPosition) ? EEP_BlockStartPosition : EEP_ElementPosition) - EEP_BlockStartPosition;
	EndPosition = (((EEP_BlockStartPosition+DataBlockSize_u16)<(EEP_ElementPosition+ElementSize_u16)) ? (EEP_BlockStartPosition+DataBlockSize_u16) : (EEP_ElementPosition+ElementSize_u16)) - EEP_BlockStartPosition;

	DEBUG_VALUE2(EEPROM_LAYOUT_SWI_TRC, MOD_EEPROM_LAYOUT, "AvoidEEPROMWrite: StartPosition=%d EndPosition=%d",
			     StartPosition, EndPosition);
	for(i=StartPosition; i<EndPosition; i++)
	{
		Buffer_u8[i] = EEPROMElemData[i];
	}
}



/*----------------------------------------------------------------------------*/
/* Function    : EEPROM_LAYOUT_rcWrite                                        */
/**
    \brief      Write Function for all application to write direct in the EEPROM

    \param      thisObj
                EEDeviceControlBlockType *
                pointer to the EEDeviceControlBlockType
    \param      address
                EEAddressType
                First Low Byte and then High Byte.
    \param      data
                Uint8Type *
                pointer to the data to be read starting at the given address.
    \param      length
                Uint16Type
                The data to be stored starting at the given address.
    \param      cb
                void *

    \return     CommonResponseCodeType
   */
/*----------------------------------------------------------------------------*/
CommonResponseCodeType EEPROM_LAYOUT_rcWrite( EEDeviceControlBlockType *thisObj,
                                             Uint16Type address,
                                             Uint8Type *data,
                                             Uint16Type length,
                                             void(*cb)(CommonResponseCodeType))
{
	Uint8Type i;
    CommonResponseCodeType rsp = COMMON_RSP_OK;
    Uint8Type u8EEPROMElemData[SIZEOF_EE_PAGE];
    Uint8Type ErrorLengthProblem = BT_FALSE;

    //Handling for DataDownload
    //Note: In each case, after DataDownload, a RESET should be guaranteed by Tester!!!
    /*
     * The below code takes care of the protected area. If the area should be protected, then the table
     * in EEPROM_LAYOUT_CE.h should be extended.
     */

    if ( PROTECTED_AREA_CONDITIONS )
    {
        if( BT_FALSE == EEPROM_LAYOUT_bAllBlocksEmpty )
        {
            rsp = COMMON_RSP_ERROR_BUSY;
            DEBUG_VALUE2(EEPROM_LAYOUT_SWI_TRC, MOD_EEPROM_LAYOUT, "Write Registered for Address %02X,%d is BUSY", address, length);
        }
        else
        {
            for( i=0; i<MAX_EEPROM_WRITEPROTECT_ELEM; i++ )
            {
            	/* Check if at least one write protect element is in the Block under Test. If Yes, read out the block, break and go to the Write routine */
            	if( BT_TRUE == IsElemInBlock(_EEPROM_ElemValues[i].u16ElemAddr,
                                             _EEPROM_ElemValues[i].u16ElemLen, address, length) )
                {
            		if ( SIZEOF_EE_PAGE < length )
            		{
            			/* error length problem for the data to write to eeprom if length of data is greater then SIZEOF_EE_PAGE    */
            			/* No EEProm write Protection possible for this data so no read and copy section (AvoidEEPROMWrite) is done */
            			ErrorLengthProblem = BT_TRUE;
            			DEBUG_TEXT(EEPROM_LAYOUT_SWI_ERR, MOD_EEPROM_LAYOUT, "Length of data for AvoidEEPROMWrite is to long !!!!");
            		}
            		else
            		{
            			//read out the interesting Block from EEPROM only one time
            			rsp = EEPROM_sfRead(thisObj, address, &u8EEPROMElemData[0], length);
            			ErrorLengthProblem = BT_FALSE;
            		}
                    break;
                }
            }
            if( (COMMON_RSP_OK == rsp) && (i != MAX_EEPROM_WRITEPROTECT_ELEM) && (BT_FALSE == ErrorLengthProblem) )
            {
                for( i=0; i<MAX_EEPROM_WRITEPROTECT_ELEM; i++)
                {
                    if( BT_TRUE == IsElemInBlock(_EEPROM_ElemValues[i].u16ElemAddr,
                                                 _EEPROM_ElemValues[i].u16ElemLen, address, length) )
                    {
                        AvoidEEPROMWrite(_EEPROM_ElemValues[i].u16ElemAddr,
                                         _EEPROM_ElemValues[i].u16ElemLen, address, length,
                                         data, (Uint8Type*) &u8EEPROMElemData[0] );
                    }
                }
            }
        }
    }
    if( COMMON_RSP_OK == rsp )
    {
        DEBUG_VALUE2(EEPROM_LAYOUT_SWI_TRC, MOD_EEPROM_LAYOUT, "Write Registered for Address %02X,%d is OK", address, length);
        rsp = EEPROM_rcWrite( thisObj, (EEAddressType)address, data, length, cb);
    }

    return rsp;
}

/*----------------------------------------------------------------------------*/
/* Function    : EEPROM_LAYOUT_vCheckPendingFlags                                        */
/**
    \brief      Check pending flags and if available then write into the E2P.

   */
/*----------------------------------------------------------------------------*/
void EEPROM_LAYOUT_vCheckPendingFlags(void)
{
	/* CommonResponseCodeType retValWriteE2P = COMMON_RSP_OK; */
	static Uint8Type _SYS_OSEK_INTERFACE_au8TransitBuffer[32];
    EEDeviceControlBlockType    *pEEMon = getEEMonitorControl();
  	FlagType                 flag;
  	Uint8Type *data;
  	Uint8Type i;
  	Uint8Type Offset = 0;
  	Uint16Type length = 32;

    if ( _EEPROM_u32FlagEEPROMLayout_0_31_Blocks != 0 )
    {
		for ( i = 0; i < 32 ;i++)
		{
			if ( ((_EEPROM_u32FlagEEPROMLayout_0_31_Blocks>>i) & 0x01 ) == 0x01 )
			{
				if ( EEPROM_astBlockTable[i] != 255 )
				{
					flag = EEPROM_flagIsEEFree(pEEMon);
					if (( _EEPROM_LAYOUT_bWriteCompleted == BT_TRUE) && ( flag == TRUE) )
					{
						data = EEPROM_astBlockTable[i]*SIZEOF_EE_PAGE + EEPROM_au8DataBlock;
						DEBUG_VALUE1(EEPROM_LAYOUT_SWI_TRC, MOD_EEPROM_LAYOUT, "Flag 0_31 is set %d", i);
						semP(EEPROM_LAYOUT_LockResource);
						memcpy(_SYS_OSEK_INTERFACE_au8TransitBuffer, data , SIZEOF_EE_PAGE );
						_EEPROM_LAYOUT_bWriteCompleted = BT_FALSE;
						_EEPROM_u32FlagEEPROMLayout_0_31_Blocks&=~(0x01<<i); // reset the flag
						semV(EEPROM_LAYOUT_LockResource);
						/* hier überschreiten die Pages, Programmierung zum ausmaskiern*/
						// example 4 ( conflict ) ,6,7,8,13 ( conflict) ,15 ( conflict ),16,62

						Offset = _EEPROM_LAYOUT_vOffsetMasking_0_31(i);
						length = _EEPROM_LAYOUT_vLengthMasking_0_31(i);

						if( EEPROM_rcWrite(pEEMon,(SIZEOF_EE_PAGE*(i))+Offset,
						        (_SYS_OSEK_INTERFACE_au8TransitBuffer+Offset),length,
						        _EEPROM_LAYOUT_cbE2PWriteComplete) != COMMON_RSP_OK )
						{
						    // Something went wrong
						}
						break;
					}
					else
					{
						DEBUG_VALUE2(EEPROM_LAYOUT_SWI_TRC, MOD_EEPROM_LAYOUT, "Write Registered for Address %02X,%d is BUSY", (SIZEOF_EE_PAGE*(i))+Offset, length);
					}
				}
				else
				{
					semP(EEPROM_LAYOUT_LockResource);
					_EEPROM_u32FlagEEPROMLayout_0_31_Blocks&=~(0x01<<i); // reset the flag
					semV(EEPROM_LAYOUT_LockResource);
					DEBUG_VALUE1(EEPROM_LAYOUT_SWI_TRC, MOD_EEPROM_LAYOUT, "UNDEFINED RAM BLOCK %d", i);
				}
			}
		}
    }
    else if ( _EEPROM_u32FlagEEPROMLayout_32_63_Blocks != 0 )
    {
		for ( i = 0; i < 32 ;i++)
		{
			if ( ((_EEPROM_u32FlagEEPROMLayout_32_63_Blocks>>i) & 0x01 ) == 0x01 )
			{
				if ( EEPROM_astBlockTable[i+32] != 255 )
				{
					flag = EEPROM_flagIsEEFree(pEEMon);
					if (( _EEPROM_LAYOUT_bWriteCompleted == BT_TRUE) && ( flag == TRUE) )
					{
						data = EEPROM_astBlockTable[i+32]*SIZEOF_EE_PAGE + EEPROM_au8DataBlock;
						DEBUG_VALUE1(EEPROM_LAYOUT_SWI_TRC, MOD_EEPROM_LAYOUT, "Flag 32_63 is set %d", (i+32));
						semP(EEPROM_LAYOUT_LockResource);
						memcpy(_SYS_OSEK_INTERFACE_au8TransitBuffer, data , SIZEOF_EE_PAGE );
						_EEPROM_LAYOUT_bWriteCompleted = BT_FALSE;
						_EEPROM_u32FlagEEPROMLayout_32_63_Blocks&=~(0x01<<i); // reset the flag
						semV(EEPROM_LAYOUT_LockResource);

						Offset = _EEPROM_LAYOUT_vOffsetMasking_32_63(i);
						length = _EEPROM_LAYOUT_vLengthMasking_32_63(i);

						if( EEPROM_rcWrite(pEEMon,(SIZEOF_EE_PAGE*(i+32))+Offset,
						        (_SYS_OSEK_INTERFACE_au8TransitBuffer+Offset),length,
						        _EEPROM_LAYOUT_cbE2PWriteComplete) != COMMON_RSP_OK )
						{
						    // Something went wrong
						}
						break;
					}
					else
					{
						DEBUG_VALUE2(EEPROM_LAYOUT_SWI_TRC, MOD_EEPROM_LAYOUT, "Write Registered for Address %02X,%d is BUSY", (SIZEOF_EE_PAGE*(i))+Offset, length);
					}
				}
				else
				{
					semP(EEPROM_LAYOUT_LockResource);
					_EEPROM_u32FlagEEPROMLayout_32_63_Blocks&=~(0x01<<i); // reset the flag
					semV(EEPROM_LAYOUT_LockResource);
					DEBUG_VALUE1(EEPROM_LAYOUT_SWI_TRC, MOD_EEPROM_LAYOUT, "UNDEFINED RAM BLOCK %d", (i+32));
				}
			}
		}
    }
    else
    {
    	EEPROM_LAYOUT_bAllBlocksEmpty = BT_TRUE;
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : _EEPROM_LAYOUT_cbE2PWriteComplete                                        */
/**
    \brief      Callback function from E2P

    \param      retVal
                CommonResponseCodeType
                return Value from E2P

   */
/*----------------------------------------------------------------------------*/
static void _EEPROM_LAYOUT_cbE2PWriteComplete(CommonResponseCodeType retVal)
{
	_EEPROM_LAYOUT_bWriteCompleted = BT_TRUE;
	DEBUG_TEXT(EEPROM_LAYOUT_SWI_TRC, MOD_EEPROM_LAYOUT, "Hardware Write Successful");
}

/* End Of File EEPROM_Layout_C1.c */
