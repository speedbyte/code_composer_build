/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/
/**
 * \file 	EEPROM_LAYOUT_CFG.h
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

#ifndef _EEPROM_LAYOUT_CFG_H
#define _EEPROM_LAYOUT_CFG_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include "common.h"
#include "tms470r1.h"
/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/
#include "EEPROM_LAYOUT.h"
/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "EEPROM_LAYOUT_CFG_CE.h"

/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Types                                                               */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Declarations (avoid as much as possible!)                 */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global const Declarations                                                  */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Methods                                                             */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/

extern void EEPROM_vLayoutCfgInit(void);

void EEPROM_LAYOUT_vGetFallbackLanguage(Uint8Type* Value);
void EEPROM_LAYOUT_vGetVoiceControl(Uint8Type * Value);
void EEPROM_LAYOUT_vGetPhonebookSortOrder(Uint8Type * Value);
void EEPROM_LAYOUT_vGetAudioParameterSet(Uint8Type* Value);
void EEPROM_LAYOUT_vGetCooperativeMuteType(Uint8Type* Value);
void EEPROM_LAYOUT_vGetCDMADetection(Uint8Type* Value);
void EEPROM_LAYOUT_vGetPlayerEnabled(BooleanType * Value);
void EEPROM_LAYOUT_vGetBaseplateDiag(BooleanType* Value);
void EEPROM_LAYOUT_vGetCradleProtection(BooleanType *Value);
void EEPROM_LAYOUT_vGetAntennaDiag(BooleanType* Value);
void EEPROM_LAYOUT_vGetRooflineMode(Uint8Type* Value);


extern CommonResponseCodeType EEPROM_enGetMuteDelay(Uint16Type* Value);

extern CommonResponseCodeType EEPROM_enGetMaskingLanguage(Uint16Type* Value);

extern CommonResponseCodeType EEPROM_enGetBluetoothPin(Uint8Type* pau8BTPinValue, Uint8Type number_len);

extern CommonResponseCodeType EEPROM_enGetEepromLayoutVersionNumber(Uint8Type* ptrau8Value, Uint8Type number_len);



#endif /* _EEPROM_LAYOUT_CFG_H */

/* End Of File EEPROM.h */
