/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     EEPROM.h
*-----------------------------------------------------------------------------
* Module Name:	  MOD_EERPOM
*-----------------------------------------------------------------------------
* Description:	  Eeprom Manager and Driver header file
*-----------------------------------------------------------------------------
* $Date: 2010-05-14 17:11:24 +0200 (Fr, 14 Mai 2010) $
* $Rev: 13085 $
* $Author: agrawal $
* $URL: http://subversion-01/svn/SW_HW_System/Plattform/TMS470/MOD/MOD_EEPROM/trunk/inc/EEPROM.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _EEPROM_H
#define _EEPROM_H

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
#include "EEPROM_CE.h"
/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Types                                                               */
/*----------------------------------------------------------------------------*/
typedef Uint16Type EEAddressType;
typedef void (*EEWriteCallback)(EEAddressType);
typedef enum
{
    EE_STATE_IDLE,
    EE_STATE_WRITE,
    EE_STATE_READ
} EEStateType;

typedef enum _EEPROM_STATUS_ENUM
{
    EE_STATUS_READY         = 0x00,
    EE_STATUS_WORKING       = 0x01,
    EE_STATUS_DONE          = 0x02,
    EE_STATUS_FAILED        = 0x04
} EEPROM_STATUS_ENUM;

typedef enum
{
    EEPROM_INVALID_FCT = 0,
    EEPROM_SERVICE_READ,
    EEPROM_SERVICE_WRITE
} EEPROM_enFunctionIdType;

typedef struct
{
    FlagType    eeBusy;
    Union16Type eeAddress;
    Union16Type eeAddressMemento;
    Uint16Type  eeCountRemaining;
    Uint8Type   eeSegCount;
    Uint8Type   *eeData;
    EEStateType eeState;
    EEPROM_STATUS_ENUM   eeStatus;
    Uint8Type   priority;
    void(*callback)(CommonResponseCodeType);
} EEDeviceControlBlockType;


/*----------------------------------------------------------------------------*/
/* Global Variables Declarations (avoid as much as possible!)                 */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global const Declarations                                                  */
/*----------------------------------------------------------------------------*/

/****************************************************************************/
/* EEPROM  Definitions                                                      */
/****************************************************************************/
#define EE_CHIP_SELECT_ADDRESS      0xA0
#define EE_WRITE                    0x00
#define EE_READ                     0x01

#define M95160_WREN         0x06            /* Write Enable                 */
#define M95160_WRDI         0x04            /* Write Disable                */
#define M95160_RDSR         0x05            /* Read Status Register         */
#define M95160_WRSR         0x01            /* Write Status Register        */
#define M95160_READ         0x03            /* Read from Memory Array       */
#define M95160_WRITE        0x02            /* Write to Memory Array        */
/*--------------------------------------------------------------------------*/
/* M95256 Status Register                                                   */
/*--------------------------------------------------------------------------*/
#define M95160_WIP          (1 << 0)        /* Write in Progress            */

#define EE_WRITE_ENABLE     M95160_WREN     /* Write Enable                 */
#define EE_WRITE_DISABLE    M95160_WRDI     /* Write Disable                */
#define EE_READ_STATUS      M95160_RDSR     /* Read Status Register         */
#define EE_WRITE_STATUS     M95160_WRSR     /* Write Status Register        */
#define EE_READ_MEMORY      M95160_READ     /* Read from Memory Array       */
#define EE_WRITE_MEMORY     M95160_WRITE    /* Write to Memory Array        */
#define EE_WIP              M95160_WIP      /* Write in Progress            */

#define SIZEOF_EE           EEPROM_ADDR_MAX
#define SIZEOF_EE_HDR       3
#define SIZEOF_EE_PAGE      32

#define NOF_EEPromClients       6
#define EEPromHIGH_PRIORITY     9
#define EEPromLOW_PRIORITY      0
/*----------------------------------------------------------------------------*/
/* Public Methods                                                             */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/

/** 
\brief	description of the interface IEEPROM
*/
#ifdef IEEPROM /* BEGIN DECLARE INTERFACE(IEEPROM) */

SuccessFailType EEPROM_sfInit(EEDeviceControlBlockType *this);
SuccessFailType EEPROM_sfUnregister(const EEDeviceControlBlockType *myControlType);

/*----------------------------------------------------------------------------*/
/* Function    : EEPROM_vManager                                    */
/** 
    \brief      Unregister the given client. It may not access the Flash again.

    \brief      Description:
                Takes the next command with the highest priority and write  *
                the requested data within EEProm.It segments for performance*
                reason the write in several pages. So once have to call this*
                method until every has been handled                         *
 
    \return     void
*/
/*----------------------------------------------------------------------------*/
void EEPROM_vManager(void);

/*----------------------------------------------------------------------------*/
/* Function    : EEPROM_sfRead                                                 */
/** 
    \brief      Reads at the given address the sepcified amount of bytes
  
    \brief      Description:
                The read command is performed "synchronously" through the client.
                Precondition: Once has to  habe been regsistered calling EEPROM_sfinit()
                Before calling that method.
 
    \param      thisObj
                EEDeviceControlBlockType
                pointer to the EEDeviceControlBlockType
    \param      address
                EEAddressType
                First High Byte and then Low Byte.
    \param      data
                Uint8Type *
                pointer to the data to be read starting at the given address.
    \param      length
                Uint16Type
                The data to be stored starting at the given address.

    \return     SuccessFailType
                Returns TRUE if information is successfully read.
                FALSE otherwise
*/
/*----------------------------------------------------------------------------*/
CommonResponseCodeType EEPROM_sfRead(
    EEDeviceControlBlockType *thisObj,
    EEAddressType address,
    Uint8Type *data,
    Uint16Type length);

/*----------------------------------------------------------------------------*/
/* Function    : EEPROM_rcWrite                                               */
/** 
    \brief      Writes at the given address the sepcified amount of bytes
  
    \brief      Description:
    			The write commands are enqueued but only one command pending is allowed per client.
				Write at the given address the specified amount of bytes
                from the given adress. The command is asynchron. It just
                post a request which is handled wihtin EEPROM_vManager
                A write over several pages may be interrupted through another write
                operation if its priority is higher. As the progress about the flashing
                is stored within the client object, we can do that.
                Precondition: Once has to  habe been regsistered calling EEPROM_sfinit()
                Before calling that method.
 
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

    \return     EEPROM_rcWrite
                It returns TRUE whether the request has been successfully  *
                posted, FALSE otherwise  
*/
/*----------------------------------------------------------------------------*/
CommonResponseCodeType EEPROM_rcWrite( EEDeviceControlBlockType *thisObj,
                                EEAddressType address,
                                Uint8Type *data,
                                Uint16Type length,
                                void(*cb)(CommonResponseCodeType));

/*----------------------------------------------------------------------------*/
/* Function    : EEPROM_flagIsEEFree                                                 */
/** 
    \brief      Returns true if the Status of the Block is Busy.
  
    \brief      Description:
                Since Each client has its own block, the status information is
                stored in this block.

    \param      this
                EEDeviceControlBlockType
                pointer to the EEDeviceControlBlockType

    \return     FlagType
                Returns 1 if the Block is busy working other commands.
*/
/*----------------------------------------------------------------------------*/
FlagType EEPROM_flagIsEEFree(EEDeviceControlBlockType *thisObj);


#endif /* END DECLARE INTERFACE(IEEPROM) */

#endif /* _EEPROM_H */

/* End Of File EEPROM.h */
