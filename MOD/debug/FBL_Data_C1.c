/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     	FBLDATA_PL_SPI_C1.c
*-----------------------------------------------------------------------------
* Module Name:		MOD_FBL_DATA
*-----------------------------------------------------------------------------
* Description:		Specialisation of EEPROM HW Interface for SPI 
*-----------------------------------------------------------------------------
* $Date: 2010-12-08 09:41:12 +0100 (Mi, 08 Dez 2010) $
* $Rev: 16812 $
* $Author: agrawal $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_FBL_DATA/trunk/src/FBL_Data_C1.c $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include <osek.h>
/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "Common.h"
#include "OSEK_HostTask_H1.h"
#include "FBL_Data.h"
#include "Version.h"
#include "EEPROM_LAYOUT_CFG.h"
#include "FBL_DATA_C1.id"
#include "TRACE.h"

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
/** 
\brief	description of the method
*/

/*----------------------------------------------------------------------------*/
/* File local Code Macros                                                     */
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
const Uint8Type ProgramInfoReset     = 0x40U;
const Uint8Type SysProgrammed        = 0x00U;

/*----------------------------------------------------------------------------*/
/* Private Address Calculations                                               */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Function    : FBLDATA_sfUpdateAfterReflash                                  */
/** 
    \brief      Check EEPROM data status for ECU programming info and all \n
                programming attempts.
                
    \return     SuccessFailType
                SUCCESS when no errors 
                FAIL otherwise.
                value limits
   */
/*----------------------------------------------------------------------------*/
SuccessFailType FBLDATA_sfUpdateAfterReflash(void)
{
    Uint8Type ECUProgramInfo = 0x00U;

    /* First Read ECUProgrammingInformation from EEPROM to check Program was reflash */
    (void)EEPROM_enGetSingleByteFromRamMirror(&ECUProgramInfo, EEPROM_LAYOUT_ENUM_ECUProgramInfo);

    if (0x44U == ECUProgramInfo)
    {
    	DEBUG_VALUE1(FBL_DATA_SWI_TRC, MOD_FBL_DATA, "UpdateAfterReflash: Application reflashed (CAN) %x",
                     ECUProgramInfo);
        /* Read VWApplicationCounterOfSuccessProgrammingAttempts 0x0408 out of EEPROM */
        /* $22 is Master !!! */
    	(void)EEPROM_sfRamWriteOnly(
    			 (Uint8Type *)&ProgramInfoReset, EEPROM_LAYOUT_ENUM_ECUProgramInfo);
    	(void)EEPROM_sfRamWriteOnly(
    			 (Uint8Type *)&SysProgrammed, EEPROM_LAYOUT_ENUM_SysProgrammed);
    	/* write software version and ASAM ODX File Version in EEPROM */
    	(void)EEPROM_sfRamWriteOnly(
    			 (Uint8Type *)&SoftwareVersion, EEPROM_LAYOUT_ENUM_AppSoftwareVersionNbr);
    	(void)EEPROM_sfRamWriteOnly(
    			 (Uint8Type *)&SoftwareVersion, EEPROM_LAYOUT_ENUM_Backup_AppSoftwareVersionNbr);
    	(void)EEPROM_sfRamWriteOnly(
    			 (Uint8Type *)&SoftwareVersion, EEPROM_LAYOUT_ENUM_AswSWLogicalBlockVersion);
    	(void)EEPROM_sfRamWriteOnly(
    			 (Uint8Type *)&AsamOdxFileVersion, EEPROM_LAYOUT_ENUM_AsamOdxFileVersion);
    	(void)EEPROM_sfRamWriteOnly(
    			 (Uint8Type *)&AsamOdxFileVersion, EEPROM_LAYOUT_ENUM_Backup_AsamOdxFileVersion);

    } 
    else if (0x40U == ECUProgramInfo)
    {
    	Uint8Type FBL_u8SwVersion[5] = {0};
    	Uint8Type FBL_u8AswSwVersion[5] = {0};
        /* read software version from EEPROM and compare the value with the ROM value */
        /* These is a requirement for the production */
        /* read software version from EEPROM and compare the value with the ROM value */
        /* These is a requirement for the production */
        (void)EEPROM_enGetByteSequenceFromRamMirror(&FBL_u8SwVersion[0],
                                                    VWDB_EE_ADDDRESS_AppSoftwareVersionNbr_LEN, 
                                                    EEPROM_LAYOUT_ENUM_AppSoftwareVersionNbr);

        (void)EEPROM_enGetByteSequenceFromRamMirror(&FBL_u8AswSwVersion[0],
                                                    VWDB_EE_ADDDRESS_AswSWLogicalBlockVersion_LEN, 
                                                    EEPROM_LAYOUT_ENUM_AswSWLogicalBlockVersion);
		FBL_u8SwVersion[4] = '\0';
		FBL_u8AswSwVersion[4] = '\0';
		if ((0 != strcmp((const char *)FBL_u8SwVersion, SoftwareVersion)) ||
		    (0 != strcmp((const char *)FBL_u8AswSwVersion, SoftwareVersion)))
		{
    	    DEBUG_VALUE1(FBL_DATA_SWI_TRC, MOD_FBL_DATA, "UpdateAfterReflash: Application reflashed (JTEG) %x", 
                         ECUProgramInfo);
			/* write software version and ASAM ODX File Version in EEPROM */
        	(void)EEPROM_sfRamWriteOnly(
        			 (Uint8Type *)&SoftwareVersion, EEPROM_LAYOUT_ENUM_AppSoftwareVersionNbr);
        	(void)EEPROM_sfRamWriteOnly(
        			 (Uint8Type *)&SoftwareVersion, EEPROM_LAYOUT_ENUM_Backup_AppSoftwareVersionNbr);
        	(void)EEPROM_sfRamWriteOnly(
        			 (Uint8Type *)&SoftwareVersion, EEPROM_LAYOUT_ENUM_AswSWLogicalBlockVersion);
        	(void)EEPROM_sfRamWriteOnly(
        			 (Uint8Type *)&AsamOdxFileVersion, EEPROM_LAYOUT_ENUM_AsamOdxFileVersion);
        	(void)EEPROM_sfRamWriteOnly(
        			 (Uint8Type *)&AsamOdxFileVersion, EEPROM_LAYOUT_ENUM_Backup_AsamOdxFileVersion);
		}
        else
        {
            DEBUG_TEXT(FBL_DATA_SWI_TRC, MOD_FBL_DATA, "UpdateAfterReflash: Application is up to date"); 
        }
    }
    else
    {
        /* Not in Bootloader update Phase. But we have to check if date */
        /* was update correct if no try to update again                 */
        DEBUG_VALUE1(FBL_DATA_SWI_ERR, MOD_FBL_DATA, "UpdateAfterReflash: Incorrect EEPROM value %x", 
                     ECUProgramInfo);
    }  
    return SUCCESS;
}

/* End Of File FBLDATA_Flashdata_C1.c */
