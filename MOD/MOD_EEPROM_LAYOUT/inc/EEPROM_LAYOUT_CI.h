/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     EEPROM_LAYOUT_CI.h
*-----------------------------------------------------------------------------
* Module Name:		MOD_EEPROM_LAYOUT
*-----------------------------------------------------------------------------
* Description:		EEPROM_LAYOUT Manager and Driver header file
*-----------------------------------------------------------------------------
* $Date: 2009-07-06 17:27:23 +0200 (Mo, 06 Jul 2009) $
* $Rev: 7052 $
* $Author: agrawal $
* $URL: http://subversion-01/svn/SW_HW_System/Plattform/TMS470/MOD/MOD_EEPROM_LAYOUT/trunk/inc/EEPROM_LAYOUT_CI.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _EEPROM_LAYOUT_CI_H
#define _EEPROM_LAYOUT_CI_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/
#define IEEPROM_LAYOUT_PL_SPI 100 /* CONFIGURATE EXPORT INTERFACE(IEEPROM_LAYOUT) */

/*----------------------------------------------------------------------------*/
/* Import configuration                                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Synonym adaptions                                    */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the synonym adaption
*/
#define ITMPL_Func IUART_Func

/*----------------------------------------------------------------------------*/
/* Prototypes of module global methods                                        */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the method
*/
extern Uint8Type _EEPROM_LAYOUT_vLengthMasking_0_31(Uint8Type i);
extern Uint8Type _EEPROM_LAYOUT_vOffsetMasking_0_31(Uint8Type i);
extern Uint8Type _EEPROM_LAYOUT_vLengthMasking_32_63(Uint8Type i);
extern Uint8Type _EEPROM_LAYOUT_vOffsetMasking_32_63(Uint8Type i);
/*----------------------------------------------------------------------------*/
/* Module global code macros                                                         */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Module global variables                                                    */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the module global variable
*/

/*----------------------------------------------------------------------------*/
/* Module global const definitions                                                  */
/*----------------------------------------------------------------------------*/

typedef struct {
  Uint16Type     Address;
  Uint16Type     Len;
} EEPROM_ST_t_EepromCalibrationData;

typedef struct {
	Uint16Type     E2pAddress;
	Uint8Type *     Address;
    Uint16Type     Len;
} EEPROM_ST_t_EepromRamMirrorData;

typedef Uint16Type EepromId;

typedef struct {
	Uint16Type  u16ElemAddr;
	Uint16Type  u16ElemLen;
} EEPROM_ELEM;

#endif /* _EEPROM_LAYOUT_CI_H */

/* End Of File EEPROM_LAYOUT_CI.h */
