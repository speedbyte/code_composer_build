/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/**
 * \file 	EEPROM_LAYOUT.h
 * \brief	This file contains prototypes of all the functions that can be used
 * by other modules. Detailed description found below.
 * \brief	It also contains enum, structure, constant definition etc. that can
 * be used by other modules. The application/system must include this file
 * in order to use the interfaces provided by this module.
*/
/*-----------------------------------------------------------------------------
* $Date: 2010-04-29 15:14:05 +0200 (Do, 29 Apr 2010) $
* $Rev: 12700 $
* $Author: agrawal $
* $URL: http://subversion-01/svn/SW_HW_System/Plattform/TMS470/MOD/MOD_EEPROM/trunk/inc/EEPROM.h $
*------------------------------------- ----------------------------------------
*/
/******************************************************************************/

#ifndef _EEPROM_LAYOUT_H
#define _EEPROM_LAYOUT_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include "common.h"
#include "tms470r1.h"
/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "EEPROM_LAYOUT_CE.h"
#include "EEPROM_LAYOUT_generated_ADR.h"
#include "EEPROM_LAYOUT_generated_ENUM.h"

/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Types                                                               */
/*----------------------------------------------------------------------------*/
/* Achtung, changing the sequence of the elements in struct will result in
 * more space required
 */
typedef struct {
	Uint8Type     Offset;
	Uint8Type  RamShadowBlockNr;
	Uint16Type Len;
} EEPROM_ST_t_EepromRamMirrorData_Blockwise;


/* Holds the data for byte sequence data type */
typedef struct
{
    Uint8Type   *data;
    Uint16Type  length;
} RamMirrorByteSequence;

/* Message from Application to EEprom */
typedef struct
{
    EEPROM_ENUM_GET_EEPROM      Eeprom_id;
    union
    {
        RamMirrorByteSequence   byte_seq;
        Uint8Type               *u8_value;
        Uint16Type              *u16_value;
        Uint32Type              *u32_value;
        BooleanType             *bool_value;
    } payload_ptr;
} RamMirrorRequest;


/*----------------------------------------------------------------------------*/
/* Global Variables Declarations (avoid as much as possible!)                 */
/*----------------------------------------------------------------------------*/
extern BooleanType EEPROM_LAYOUT_bAllBlocksEmpty;

extern Uint8Type EEPROM_au8DataBlock[SIZEOF_EE_PAGE*RAM_BLOCK_COUNT];
/*----------------------------------------------------------------------------*/
/* Global const Declarations                                                  */
/*----------------------------------------------------------------------------*/
extern const Uint8Type EEPROM_astBlockTable[64];

extern const EEPROM_ST_t_EepromRamMirrorData_Blockwise EEPROM_au8RamMirrorTable[];
/*----------------------------------------------------------------------------*/
/* Public Methods                                                             */
/*----------------------------------------------------------------------------*/
#ifdef IAPP_DIAG_EEPROM_GET

void EEPROM_vLayoutInit(void);

SuccessFailType EEPROM_sfRamWriteOnly(Uint8Type *data,EEPROM_ENUM_GET_EEPROM enAreaToWrite);

CommonResponseCodeType EEPROM_LAYOUT_rcWrite( EEDeviceControlBlockType *thisObj,
                                             Uint16Type address,
                                             Uint8Type *data,
                                             Uint16Type length,
                                             void(*cb)(CommonResponseCodeType));
extern void EEPROM_LAYOUT_vCheckPendingFlags(void);

extern CommonResponseCodeType EEPROM_enGetEepromValues(RamMirrorRequest* stReadEepromId) ;

extern CommonResponseCodeType EEPROM_enGetByteSequenceFromRamMirror(Uint8Type* ptrau8Value, Uint8Type number_len, EEPROM_ENUM_GET_EEPROM enAreaToRead);

extern CommonResponseCodeType EEPROM_enGetSingleByteFromRamMirror(Uint8Type* Value, EEPROM_ENUM_GET_EEPROM enAreaToRead); // DEPRECATED: use the U8 function instead

extern CommonResponseCodeType EEPROM_enGetU8FromRamMirror(Uint8Type* Value, EEPROM_ENUM_GET_EEPROM enAreaToRead);
extern CommonResponseCodeType EEPROM_enGetU16FromRamMirror(Uint16Type* Value, EEPROM_ENUM_GET_EEPROM enAreaToRead);
extern CommonResponseCodeType EEPROM_enGetU32FromRamMirror(Uint32Type* Value, EEPROM_ENUM_GET_EEPROM enAreaToRead);

#endif

/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/


#endif /* _EEPROM_LAYOUT_H */

/* End Of File EEPROM.h */
