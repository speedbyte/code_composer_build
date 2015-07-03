/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/
/**
 * \file    EEPROM_Layout_RamMirroe_C1.c
 * \brief   This file contains the program to xxxx
 * \brief   This file fulfills the following requirements:
 *          1. Requirement 1
 *          2. Requirement 2
 *          3. Requirement x
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
#include "EEPROM.h"
#include "TRACE.h"
/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "EEPROM_LAYOUT_CFG.h"
#include "EEPROM_LAYOUT_generated_H1.h"
#include "EEPROM_LAYOUT_generated_H2.h"
#include "EEPROM_Layout_Cfg_CI.h"
#include "EEPROM_Layout_RamMirror_C1.id"

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
\brief  description of the type
*/
/*----------------------------------------------------------------------------*/
/* Prototypes of file local Methods                                           */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* File local Code Macros                                                        */
/*----------------------------------------------------------------------------*/
/**
\brief  description of the macros
*/
/*----------------------------------------------------------------------------*/
/* File local Variables                                                       */
/*----------------------------------------------------------------------------*/
/**
\brief  description of the private variable
*/
Uint8Type EEPROM_au8DataBlock[SIZEOF_EE_PAGE*RAM_BLOCK_COUNT];
/*----------------------------------------------------------------------------*/
/* File local const Definitions                                               */
/*----------------------------------------------------------------------------*/
/**
\brief  description of the private constant
*/
// static const Uint8Type _EEPROM_u8ToBeDefined = 0;
/*----------------------------------------------------------------------------*/
/* Private Address Calculations                                               */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Function    : _EEPROM_LAYOUT_vOffsetMasking_0_31                                        */
/**
    \brief      Return masking offset for the corresponding block. If the block is not present, then
                the default value is 0. The reason is that not all bytes are in the RAM Mirror and hence
                a start offset needs to be created.

    \param      i
                Uint8Type

    \return     Uint8Type

   */
/*----------------------------------------------------------------------------*/
Uint8Type _EEPROM_LAYOUT_vOffsetMasking_0_31(Uint8Type i)
{
    Uint8Type Offset = 0;
    EEPROM_LAYOUT_OFFSET_MASK_0_31
    /*if ( i == 0 )
    {
        Offset = 0;
    }
    else if ( i == 1 )
    {
        Offset = 21;
    }
    else if ( i == 2 )
    {
        Offset = 23;
    }
    else if ( i == 3 )
    {
        Offset = 20;
    }
    else if ( i == 4 )
    {
        Offset = 16;
    }
    else if ( i == 8 )
    {
        Offset = 1;
    }
    else if ( i == 9 )
    {
        Offset = 2;
    }
    else if ( i == 13 )
    {
        Offset = 3;
    }
    else if ( i == 14 )
    {
        Offset = 26;
    }
    else if ( i == 19 )
    {
        Offset = 3;
    }
    else if ( i == 20 )
    {
        Offset = 13;
    }*/
    return Offset;
}

/*----------------------------------------------------------------------------*/
/* Function    : _EEPROM_LAYOUT_vLengthMasking_0_31                                        */
/**
    \brief      Return masking length for the corresponding block. If the block is not present, then
                the default value is 0. The reason is that not all bytes are in the RAM Mirror and hence
                a length needs to be created.

    \param      i
                Uint8Type

    \return     Uint8Type

   */
/*----------------------------------------------------------------------------*/
Uint8Type _EEPROM_LAYOUT_vLengthMasking_0_31(Uint8Type i)
{
    Uint8Type Length = 32;
    EEPROM_LAYOUT_LENGTH_MASK_0_31
    /*if ( i == 0 )
    {
        length = 15;
    }
    else if ( i == 1 )
    {
        length = 6;
    }
    else if ( i == 2 )
    {
        length = 1;
    }
    else if ( i == 3 )
    {
        length = 1;
    }
    else if ( i == 4 )
    {
        length = 18;
    }
    else if ( i == 8 )
    {
        length = 29;
    }
    else if ( i == 9 )
    {
        length = 13;
    }
    else if ( i == 13 )
    {
        length = 8;
    }
    else if ( i == 14 )
    {
        length = 7;
    }
    else if ( i == 19 )
    {
        length = 4;
    }
    else if ( i == 20 )
    {
        length = 6;
    }*/
    return Length;
}

/*----------------------------------------------------------------------------*/
/* Function    : _EEPROM_LAYOUT_vLengthMasking_32_64                                        */
/**
    \brief      Return masking offset for the corresponding block. If the block is not present, then
                the default value is 0. The reason is that not all bytes are in the RAM Mirror and hence
                a start offset needs to be created.

    \param      i
                Uint8Type

    \return     Uint8Type

   */
/*----------------------------------------------------------------------------*/
Uint8Type _EEPROM_LAYOUT_vOffsetMasking_32_63(Uint8Type i)
{
    Uint8Type Offset = 0;
    EEPROM_LAYOUT_OFFSET_MASK_32_63
    /*if ( i == 30 )
    {
        Offset = 0;
    }*/
    return Offset;
}

/*----------------------------------------------------------------------------*/
/* Function    : _EEPROM_LAYOUT_vLengthMasking_32_64                                        */
/**
    \brief      Return masking length for the corresponding block. If the block is not present, then
                the default value is 0. The reason is that not all bytes are in the RAM Mirror and hence
                a length needs to be created.

    \param      i
                Uint8Type

    \return     Uint8Type

   */
/*----------------------------------------------------------------------------*/
Uint8Type _EEPROM_LAYOUT_vLengthMasking_32_63(Uint8Type i)
{
    Uint8Type Length = 32;
    EEPROM_LAYOUT_LENGTH_MASK_32_63
    /*if ( i == 30 )
    {
        length = 6;
    }*/
    return Length;
}

/*----------------------------------------------------------------------------*/
/* Function    : EEPROM_enGetEepromValues                                                 */
/**
    \brief      Short description

    \brief      Description:
                XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
                XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

    \param      stReadEepromId

    \return     CommonResponseCodeType
*/
/*----------------------------------------------------------------------------*/
CommonResponseCodeType EEPROM_enGetEepromValues(RamMirrorRequest* stReadEepromId)
{
// use the new concept, independent from the enumeration
    CommonResponseCodeType sF = COMMON_RSP_OK;
    Uint8Type i=0;
    Uint8Type *data;
    if  (stReadEepromId->Eeprom_id < ENUM_EEPROM_LAYOUT_MAX)
    {
        i = stReadEepromId->Eeprom_id;
        if ( EEPROM_au8RamMirrorTable[i].RamShadowBlockNr == EEPROM_RAMBLOCK_NOTDEFINED)
        {
            DEBUG_VALUE1(EEPROM_LAYOUT_SWI_ERR, MOD_EEPROM_LAYOUT, "Error=$EEPROM_ENUM_GET_EEPROM$ %d - RAM block not defined", stReadEepromId->Eeprom_id);
            sF = COMMON_RSP_ERROR;
        }
        else if ( stReadEepromId->payload_ptr.byte_seq.length > EEPROM_au8RamMirrorTable[i].Len )
        {
            Uint8Type x;
            for ( x = 0; x < 100; x++ ) // be sure to be seen on the trace client
            {
                DEBUG_STATE1(EEPROM_LAYOUT_SWI_ERR, MOD_EEPROM_LAYOUT, "Error=$EEPROM_ENUM_GET_EEPROM$ %d - incorrect length", stReadEepromId->Eeprom_id);
            }
            sF = COMMON_RSP_ERROR;
        }
    }
    else
    {
        DEBUG_STATE1(EEPROM_LAYOUT_SWI_ERR, MOD_EEPROM_LAYOUT, "Error=$EEPROM_ENUM_GET_EEPROM$ %d - Eeprom_id not found", stReadEepromId->Eeprom_id);
        sF = COMMON_RSP_ERROR;
    }

    if ( sF == COMMON_RSP_OK )
    {
        DEBUG_STATE1(EEPROM_LAYOUT_SWI_TRC, MOD_EEPROM_LAYOUT, "Read $EEPROM_ENUM_GET_EEPROM$ from Ram Mirror Successful %d", stReadEepromId->Eeprom_id);
        data = &EEPROM_au8DataBlock[((EEPROM_au8RamMirrorTable[i].RamShadowBlockNr)*SIZEOF_EE_PAGE+EEPROM_au8RamMirrorTable[i].Offset)];
        (void)memcpy(stReadEepromId->payload_ptr.byte_seq.data, (data), EEPROM_au8RamMirrorTable[i].Len);

        // print some debug output
        if (1 == stReadEepromId->payload_ptr.byte_seq.length)
        {
            DEBUG_VALUE1(EEPROM_LAYOUT_SWI_TRC, MOD_EEPROM_LAYOUT, "Val=%02X", stReadEepromId->payload_ptr.byte_seq.data[0]);
        }
        else if (2 == stReadEepromId->payload_ptr.byte_seq.length)
        {
            DEBUG_VALUE2(EEPROM_LAYOUT_SWI_TRC, MOD_EEPROM_LAYOUT, "Val=%02X %02X", stReadEepromId->payload_ptr.byte_seq.data[0],stReadEepromId->payload_ptr.byte_seq.data[1]);
        }
        else if (3 == stReadEepromId->payload_ptr.byte_seq.length)
        {
            DEBUG_VALUE4(EEPROM_LAYOUT_SWI_TRC, MOD_EEPROM_LAYOUT, "Val=%02X %02X %02X %c", stReadEepromId->payload_ptr.byte_seq.data[0],stReadEepromId->payload_ptr.byte_seq.data[1],stReadEepromId->payload_ptr.byte_seq.data[2],0);
        }
        else
        {
            DEBUG_VALUE4(EEPROM_LAYOUT_SWI_TRC, MOD_EEPROM_LAYOUT, "Val=%02X %02X %02X %02X", stReadEepromId->payload_ptr.byte_seq.data[0],stReadEepromId->payload_ptr.byte_seq.data[1],stReadEepromId->payload_ptr.byte_seq.data[2],stReadEepromId->payload_ptr.byte_seq.data[3]);
        }
    }
    return sF;
}



/*----------------------------------------------------------------------------*/
/* Function    : EEPROM_enGetSingleByteFromRamMirror                          */
/**
 \brief       function for a request to read a byte from eeprom
              DEPRECATED: use the U8 function instead

 \param       Value (Uint8Type*)
              pointer for the result

 \param       enAreaToRead (EEPROM_ENUM_GET_EEPROM)
              the requested data from eeprom

 \return      CommonResponseCodeType
 */
/*----------------------------------------------------------------------------*/
CommonResponseCodeType EEPROM_enGetSingleByteFromRamMirror(Uint8Type* Value, EEPROM_ENUM_GET_EEPROM enAreaToRead)
{
    CommonResponseCodeType sF = COMMON_RSP_ERROR;
    sF = EEPROM_enGetByteSequenceFromRamMirror(Value, 1, enAreaToRead);
    return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : EEPROM_enGetU8FromRamMirror                                  */
/**
 \brief       function for a request to read a byte from eeprom

 \param       Value (Uint8Type*)
              pointer for the result

 \param       enAreaToRead (EEPROM_ENUM_GET_EEPROM)
              the requested data from eeprom

 \return      CommonResponseCodeType
 */
/*----------------------------------------------------------------------------*/
CommonResponseCodeType EEPROM_enGetU8FromRamMirror(Uint8Type* Value, EEPROM_ENUM_GET_EEPROM enAreaToRead)
{
    CommonResponseCodeType sF = COMMON_RSP_ERROR;
    sF = EEPROM_enGetByteSequenceFromRamMirror(Value, 1, enAreaToRead);
    return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : EEPROM_enGetU16FromRamMirror                                 */
/**
 \brief       function for a request to read a short from eeprom

 \param       Value (Uint16Type*)
              pointer for the result

 \param       enAreaToRead (EEPROM_ENUM_GET_EEPROM)
              the requested data from eeprom

 \return      CommonResponseCodeType
 */
/*----------------------------------------------------------------------------*/
CommonResponseCodeType EEPROM_enGetU16FromRamMirror(Uint16Type* Value, EEPROM_ENUM_GET_EEPROM enAreaToRead)
{
    CommonResponseCodeType sF = COMMON_RSP_ERROR;
    RamMirrorRequest temp; // using the union for pointer conversion
    temp.payload_ptr.u16_value = Value;
    sF = EEPROM_enGetByteSequenceFromRamMirror(temp.payload_ptr.byte_seq.data, 2, enAreaToRead);
    return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : EEPROM_enGetU32FromRamMirror                                 */
/**
 \brief       function for a request to read a long from eeprom

 \param       Value (Uint32Type*)
              pointer for the result

 \param       enAreaToRead (EEPROM_ENUM_GET_EEPROM)
              the requested data from eeprom

 \return      CommonResponseCodeType
 */
/*----------------------------------------------------------------------------*/
CommonResponseCodeType EEPROM_enGetU32FromRamMirror(Uint32Type* Value, EEPROM_ENUM_GET_EEPROM enAreaToRead)
{
    CommonResponseCodeType sF = COMMON_RSP_ERROR;
    RamMirrorRequest temp; // using the union for pointer conversion
    temp.payload_ptr.u32_value = Value;
    sF = EEPROM_enGetByteSequenceFromRamMirror(temp.payload_ptr.byte_seq.data, 4, enAreaToRead);
    return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : EEPROM_enGetApplicationSwVersionNumber                  */
/**
 \brief       function for request to get Application sw version number

 \param       ptrau8Value

 \param       number_len

 \return      CommonResponseCodeType
 */
/*----------------------------------------------------------------------------*/
CommonResponseCodeType EEPROM_enGetByteSequenceFromRamMirror(Uint8Type* ptrau8Value, Uint8Type number_len, EEPROM_ENUM_GET_EEPROM enAreaToRead)
{
    CommonResponseCodeType sF;
    RamMirrorRequest bmReadEeprom;
    bmReadEeprom.Eeprom_id = enAreaToRead;
    bmReadEeprom.payload_ptr.byte_seq.data = ptrau8Value;
    bmReadEeprom.payload_ptr.byte_seq.length = number_len;
    sF = EEPROM_enGetEepromValues(&bmReadEeprom);
    if ( sF == COMMON_RSP_ERROR )
    {
        DEBUG_STATE1(EEPROM_LAYOUT_SWI_ERR, MOD_EEPROM_LAYOUT, "Error in Reading Ram Mirror $EEPROM_ENUM_GET_EEPROM$ %d", enAreaToRead);
    }
    return sF;
}





/* End Of File EEPROM_Layout_RamMirror_C1.c */
