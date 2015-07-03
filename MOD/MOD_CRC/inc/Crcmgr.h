/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/
/** 
 * \file 	CRCMGR.h
 * \brief	This file contains prototypes of all the functions that can be used
 * by other modules. Detailed description found below. 
 * \brief	It also contains enum, structure, constant definition etc. that can 
 * be used by other modules. The application/system must include this file 
 * in order to use the interfaces provided by this module.
*/
/*-----------------------------------------------------------------------------
* $Date: 2010-04-29 18:40:14 +0200 (Do, 29 Apr 2010) $
* $Rev: 12708 $
* $Author: agrawal $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_CRC/trunk/inc/Crcmgr.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/
#ifndef _CRCMGR_H
#define _CRCMGR_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/
#define CRCINITVALUE   0xFFFFFFFF
#define CRC32WORD_LEN  4 


/* Backup definitions */
#define D_EE_ADDDRESS_START_DATAREGION  VWDB_EE_ADDDRESS_VWSparePartNbr    
#define D_EE_ADDDRESS_END_DATAREGION    VWDB_EE_ADDDRESS_CRCSgIdent  
#define D_EE_ADDDRESS_START_BACKUP      VWDB_EE_ADDDRESS_Backup_VWSparePartNbr
#define D_EE_ADDDRESS_END_BACKUP        VWDB_EE_ADDDRESS_Backup_Data_CRCSgIdent  
#define D_EE_BACKUP_DELTA               D_EE_ADDDRESS_START_BACKUP - D_EE_ADDDRESS_START_DATAREGION


/*----------------------------------------------------------------------------*/
/* Public Types                                                               */
/*----------------------------------------------------------------------------*/
typedef struct {
    EEAddressType data;
    EEAddressType CRC;
    Uint8Type     datalength;
} CRCLookup;


typedef enum
{
    CRC_BACKUP_NORMALMODE_NO_CRC_ERROR=0,  
    CRC_BACKUP_READ_BACKUPDATA,  
    CRC_BACKUP_RESET_CRC, 
    CRC_BACKUP_WRITE_BACKUPDATA,  
    CRC_BACKUP_CALULATE_NEW_CRC,  
    CRC_BACKUP_CHECK_RECOVER_DATA_AGAINST_NEW_CRC
} CrcmgrBackupStatesType;

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


void CrcManager(void);
void StartCRCJob(void);
Uint8Type CompleteEECrcCheckDone(void);
FlagType CRCIsAnyJobPending(void);
SuccessFailType CRC_sfManagerInit(void);



#endif
