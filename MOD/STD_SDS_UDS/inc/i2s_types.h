/**************************************************************************
 *  COPYRIGHT (C) In2Soft GmbH, 2005-2007                                 *
 *  COPYRIGHT (C) Volkswagen AG, 2006-2007                                *
 *  ALLE RECHTE VORBEHALTEN. ALL RIGHTS RESERVED.                         *
 *                                                                        *
 **************************************************************************
 *  FILE: I2S_TYPES.H
 **************************************************************************
 *  PURPOSE:
 *  C typedefs
 *
 *************************************************************************
 *  CHANGE HISTORY: In2Soft GmbH
 *
 *  Version    Name     Date   Description
 **************************************************************************
 *  V_00_01_00 Ki 12.05.2005 - VW-Audi Conformance 
 *  V_00_01_01 Ki 30.05.2005 - no change 
 *  V_00_01_02 Ki 03.06.2005 - Further adjustments 
 *  V_00_01_03 Ki 07.06.2005 - C++ comment eliminated (MISRA conformance)  
 *  V_00_01_04 Ki 22.07.2005 - no change  
 *  V_00_01_05 Ki 25.07.2005 - no change  
 *  V_00_01_06 Ki 09.08.2005 - no change  
 *  V_00_01_07 Ki 19.09.2005 - no change
 *  V_00_01_08 Ki 05.12.2005 - no change
 *  V_01_00_00 Ki 18.01.2006 - prefix I2S_ for TRUE and FALSE
 *                           - filename changed
 *  V_01_01_00 Ki 09.03.2006 - no change
 *  V_01_02_00 Ki 02.06.2006 - no change
 *  V_01_02_01 Ki 04.09.2006 - no change 
 *  V_01_02_02 Ki 10.10.2006 - no change 
 *  V_01_02_03 Ki 02.11.2006 - Std_VersionInfoType, IN2SOFT_VENDOR_ID
 *  V_01_02_04 Ki 19.12.2006 - prefix I2S_ for TRUE and FALSE 
 *  V_01_02_05 Ki 12.01.2007 - vendor id set to zero 
 *  V_01_02_06 Ki 22.01.2007 - no change
 *  V_01_02_07 Ki 11.02.2007 - Std_VersionInfoType --> I2S_VersionInfoType 
 *  V_01_02_08 Ki 02.03.2007 - no change 
 *  V_01_02_09 Ki 02.07.2007 - no change 
 *  V_01_02_10 Ki 11.07.2007 - no change 
 *  V_01_02_11 Ki 23.07.2007 - no change 
 *  V_01_02_12 Ki 01.11.2007 - no change 
 *  V_01_02_13 Ki 18.06.2007 - no change 
 *  V_01_02_14 Ki 23.07.2008 - no change 
 *************************************************************************/

#ifndef _I2S_TYPES_H
#define _I2S_TYPES_H

/******************************************************************************
These defines allow for easier porting to other compilers. 
******************************************************************************/
typedef unsigned char  byte;
typedef unsigned short word;
typedef unsigned long  dword;

/******************************************************************************
Boolean values 
******************************************************************************/
#define I2S_TRUE    (byte)1
#define I2S_FALSE   (byte)0

#define IN2SOFT_VENDOR_ID     (word)0x0000  /* vendor id = 0, defined by VW/AUDI */

/******************************************************************************
Version Information 
******************************************************************************/
typedef struct 
{ 
word vendorID; 
byte moduleID; 
byte sw_major_version; 
byte sw_minor_version; 
byte sw_patch_version; 
} I2S_VersionInfoType; 

#endif
