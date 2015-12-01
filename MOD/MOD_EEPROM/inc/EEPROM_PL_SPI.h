/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     EEPROM_PL_SPI.h
*-----------------------------------------------------------------------------
* Module Name:		MOD_EEPROM
*-----------------------------------------------------------------------------
* Description:		EEHwInterface header file
*-----------------------------------------------------------------------------
* $Date: 2009-06-10 10:12:27 +0200 (Mi, 10 Jun 2009) $
* $Rev: 6604 $
* $Author: rammelt $
* $URL: http://subversion-01/svn/SW_HW_System/Plattform/TMS470/MOD/MOD_EEPROM/trunk/inc/EEPROM_PL_SPI.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _EEPROM_PL_SPI_H
#define _EEPROM_PL_SPI_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "EEPROM_CI.h"
/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Types                                                               */
/*----------------------------------------------------------------------------*/

typedef enum{
    INIT_FOR_WRITE,
    INIT_FOR_READ
} EEInitMode;

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

/** 
\brief	description of the interface IEEPROM_PL_SPI
*/
#ifdef IEEPROM_PL_SPI /* BEGIN DECLARE INTERFACE(IEEPROM_PL_SPI) */

/*----------------------------------------------------------------------------*/
/* Function    : EEPROM_sfHwInit                                                 */
/** 
    \brief      Sends the Write Enable before sending the actual data to be written
  
    \param      mode
                EEInitMode
                value limits
    \return     SuccessFailType
                retuns Success when Write mode is properly initialised., FAIL otherwise.
                value limits
   */
/*----------------------------------------------------------------------------*/
SuccessFailType EEPROM_sfHwInit(EEInitMode mode);

/*----------------------------------------------------------------------------*/
/* Function    : EEPROM_sfHwWriteBlock                                                 */
/** 
    \brief      Writes at the given address the sepcified amount of bytes.
    			All work is done in one Chip Select Cycle.
  
    \brief      Description:\n
    			The write commands are enqueued but only one command pending is allowed per client.
				Write at the given address the specified amount of bytes\n   
                from the given adress. The command is asynchron. It just\n   
                post a request which is handled wihtin EEPROM_vManager\n
                A write over several pages may be interrupted through another write\n
                operation if its priority is higher. As the progress about the flashing\n
                is stored within the client object, we can do that.
                Precondition: Once has to  habe been regsistered calling EEPROM_sfInit()\n
                Before calling that method.
 
    \param      selectionAddress
                Uint16Type
                First Low Byte and then High Byte.
    \param      data
                Uint8Type *
                pointer to the data to be read starting at the given address.
    \param      length
                Uint16Type
                The data to be stored starting at the given address.

    \return     SuccessFailType
                It returns TRUE whether the request has been successfully  *
                written to EEPROM, FALSE otherwise  
*/
/*----------------------------------------------------------------------------*/
SuccessFailType EEPROM_sfHwWriteBlock(Union16Type selectionAddress, const Uint8Type *data, Uint16Type length);

/*----------------------------------------------------------------------------*/
/* Function    : EEPROM_sfHwReadBlock                                                 */
/** 
    \brief      Reads at the given address the sepcified amount of bytes.
    			All work is done in one CS Cycle.
  
    \brief      Description:\n
                The read command is performed "synchronously" through the client.\n
                Precondition: Once has to  habe been regsistered calling EEPROM_sfinit()\n
                Before calling that method.
 
    \param      selectionAddress
                Uint16Type
                First Low Byte and then High Byte.
    \param      data
                Uint8Type *
                pointer to the data to be read starting at the given address.
    \param      length
                Uint16Type
                The data to be stored starting at the given address.

    \return     SuccessFailType
                It returns TRUE whether the address has been successfully  *
                read to EEPROM, FALSE otherwise  
*/
/*----------------------------------------------------------------------------*/
SuccessFailType EEPROM_sfHwReadBlock(Union16Type selectionAddress,  Uint8Type *data, Uint16Type length);

#endif /* END DECLARE INTERFACE(IEEPROM_PL_SPI) */


#endif /* _EEPROM_PL_SPI_H */

/* End Of File EEPROM_PL_SPI.h */


