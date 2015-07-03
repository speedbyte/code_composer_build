/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     EEPROM_CE.h
*-----------------------------------------------------------------------------
* Module Name:		MOD_EEPROM
*-----------------------------------------------------------------------------
* Description:		Eeprom Manager and Driver header file
*-----------------------------------------------------------------------------
* $Date: 2010-02-23 12:01:02 +0100 (Di, 23 Feb 2010) $
* $Rev: 11247 $
* $Author: agrawal $
* $URL: http://subversion-01/svn/SW_HW_System/Plattform/TMS470/MOD/MOD_EEPROM/trunk/inc/EEPROM_CE.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _EEPROM_LAYOUT_CE_H
#define _EEPROM_LAYOUT_CE_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/
#define IAPP_DIAG_EEPROM_GET

/*----------------------------------------------------------------------------*/
/* Import configuration                                                       */
/*----------------------------------------------------------------------------*/
#define VW_UHV_EEPROM

#define EEPROM_LAYOUT_PROTECTED_TABLE \
  {VWDB_EE_ADDDRESS_ECUSerialNumber,    VWDB_EE_ADDDRESS_ECUSerialNumber_LEN},\
  {VWDB_EE_ADDDRESS_ECUDataProgrammingInformation,  VWDB_EE_ADDDRESS_ECUDataProgrammingInformation_LEN},\
  {VWDB_EE_ADDDRESS_VWFAZITIdentificationString,    VWDB_EE_ADDDRESS_VWFAZITIdentificationString_LEN},\
  {VWDB_EE_ADDDRESS_VWDataSetRepairShopCodeOrSerialNumber,  VWDB_EE_ADDDRESS_VWDataSetRepairShopCodeOrSerialNumber_LEN},\
  {VWDB_EE_ADDDRESS_VWDataSetProgrammingDate,   VWDB_EE_ADDDRESS_VWDataSetProgrammingDate_LEN},\
  {VWDB_EE_ADDDRESS_VWDataCounterOfProgrammingAttempts, VWDB_EE_ADDDRESS_VWDataCounterOfProgrammingAttempts_LEN},\
  {VWDB_EE_ADDDRESS_VWDataCounterOfSuccessfulProgrammingAttempts,   VWDB_EE_ADDDRESS_VWDataCounterOfSuccessfulProgrammingAttempts_LEN}\

#define MAX_EEPROM_WRITEPROTECT_ELEM  7

#define PROTECTED_AREA_CONDITIONS (BT_TRUE == SDS_UDS_GetAdditionalDataDownloadStatus())

/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the macros
*/
/*----------------------------------------------------------------------------*/
/* Public Constants                                                           */
/*----------------------------------------------------------------------------*/

#endif /* _EEPROM_CE_H */

/* End Of File EEPROM_CE.h */
