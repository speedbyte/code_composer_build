/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     EEPROM_C1.c
*-----------------------------------------------------------------------------
* Module Name:		MOD_EEPROM
*-----------------------------------------------------------------------------
* Description:
*-----------------------------------------------------------------------------
* $Date: 2010-09-15 14:57:24 +0200 (Mi, 15 Sep 2010) $
* $Rev: 15173 $
* $Author: agrawal $
* $URL: http://subversion-01/svn/SW_HW_System/Plattform/TMS470/MOD/MOD_EEPROM/trunk/src/EEPROM_C1.c $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include <osek.h>
#include <osekext.h>
/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/
#include "Common.h"
//#include"Debug.h"
#include "TRACE.h"
#include "EEPROM_C1.id"
/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "EEPROM.h"
#include "EEPROM_PL_SPI.h"

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
static void dummy(EEAddressType address);
/*----------------------------------------------------------------------------*/
/* File local Code Macros                                                        */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the macros
*/

/*----------------------------------------------------------------------------*/
/* File local Variables                                                       */
/*----------------------------------------------------------------------------*/
/** 
\brief	The list of registered clients. Array of pointers to EEDeviceControlBlockType.
        All the pointers in the array are initialised with NULL.
*/
static EEDeviceControlBlockType* _EEPROM_pstEepromClients[NOF_EEPromClients] =
       {NULL, NULL, NULL, NULL, NULL, NULL};
/** 
\brief	The list of clients that have posted a command - The client may
  		only be put once within the list. The reason is that the client
  		information owns also some memento informations about the
 	 	progress of the write commands.
 	 	Array of pointers to EEDeviceControlBlockType.
        All the pointers in the array are initialised with NULL.
*/
static EEDeviceControlBlockType* _EEPROM_pstCommandPending[NOF_EEPromClients]=
       {NULL, NULL, NULL, NULL, NULL, NULL};
       
/** 
    \brief	The command which is currently being worked out
*/
static EEDeviceControlBlockType* _EEPROM_pstCurrentCommand = NULL;
/*----------------------------------------------------------------------------*/
/* File local const Definitions                                               */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the private constant
*/

/*----------------------------------------------------------------------------*/
/* Private Address Calculations                                               */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Function    : EEPROM_sfInit                                                 */
/** 
    \brief      Register the given client. It may then access the EEProm. 
  
    \brief      Description:
                Precondition: The given command should not be present already within
*               the list 
 
    \param      myControlType
                EEDeviceControlBlockType
                pointer to the EEDeviceControlBlockType

    \return     SuccessFailType
                Returns TRUE whether the client could have been registered
                FALSE otherwise
*/
/*----------------------------------------------------------------------------*/
SuccessFailType EEPROM_sfInit(EEDeviceControlBlockType *myControlType)
{
    SuccessFailType retval = FAIL;
    Uint16Type i = 0;
    // search for a free entry with the register
    for (i=0; i <NOF_EEPromClients; i++)
    {
        if (_EEPROM_pstEepromClients[i]==NULL)
        {
            // we found an entry
            _EEPROM_pstEepromClients[i] = myControlType;
            myControlType->eeBusy   = FALSE;
            myControlType->eeState  = EE_STATE_IDLE;
            myControlType->eeStatus = EE_STATUS_READY;
            retval = SUCCESS;
            break;
        } 
    }
//    (void)memset(&gEEPROMContext, 0, sizeof(gEEPROMContext));
    return retval;
}


/*----------------------------------------------------------------------------*/
/* Function    : dummy                                                 */
/** 
    \brief      A dummy function for callback..
  
    \brief      Description:
                Since Each client has its own block, the status information is
                stored in this block.

    \param      address
                EEAddressType
                pointer to the EEDeviceControlBlockType

    \return     void
                returns if the commands pertaining to the block has been finished.
                value limits
*/
/*----------------------------------------------------------------------------*/
static void dummy(EEAddressType address)
{
    EEAddressType eepDummy = address;
    eepDummy = eepDummy;
}



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
    Uint16Type length)
{
	CommonResponseCodeType retval = COMMON_RSP_OK;

	if (thisObj->eeBusy)
    {
        // the same client as a write command pending
		retval = COMMON_RSP_ERROR_BUSY;
		DEBUG_VALUE2(EEPROM_SWI_TRC, MOD_EEPROM, "Read Address %02X, of Length %d is BUSY", address, length);
		return retval;
    }
        
    /* Enter Critical section */
    semP(EEPROM_LockResource);
    if (_EEPROM_pstCurrentCommand != NULL)
    {
        /* there is a command that is currently being worked out
         * or the EEProm device is currently busy, so do not 
         * lock the task by trying to read
         */ 
    	retval = COMMON_RSP_ERROR_BUSY;
    	DEBUG_VALUE2(EEPROM_SWI_TRC, MOD_EEPROM, "Read Address %02X, of Length %d is BUSY", address, length);
    }
    else
    {
        thisObj->eeBusy   = TRUE;
        thisObj->eeStatus = EE_STATUS_WORKING;
        thisObj->eeAddress.u16 = address;
        retval = (CommonResponseCodeType)EEPROM_sfHwReadBlock(thisObj->eeAddress, data,length);
        if (retval == COMMON_RSP_OK)
        {
            thisObj->eeStatus = EE_STATUS_DONE;
        	DEBUG_VALUE1(EEPROM_SWI_DBG, MOD_EEPROM, "First Byte of the Read Data = %02X", data[0]);
        	DEBUG_VALUE2(EEPROM_SWI_TRC, MOD_EEPROM, "Read Address %02X, of Length %d is OK", address, length);
        }
        else
        {
            thisObj->eeStatus = (EEPROM_STATUS_ENUM)(EE_STATUS_DONE | EE_STATUS_FAILED);
            //IntComError.ErrorBits.EEPROM=ERROR_DETECTED;
            retval = COMMON_RSP_ERROR;
            DEBUG_VALUE2(EEPROM_SWI_TRC, MOD_EEPROM, "Read Address %02X, of Length %d is ERROR", address, length);
        }
        thisObj->eeBusy = FALSE;
    }
    /* Ends critical section. */
    semV(EEPROM_LockResource);

    
    return(retval);
}
/*----------------------------------------------------------------------------*/
/* Function    : EEPROM_rcWrite                                                 */
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

    \return     CommonResponseCodeType
*/
/*----------------------------------------------------------------------------*/
CommonResponseCodeType EEPROM_rcWrite( EEDeviceControlBlockType *thisObj,
                                EEAddressType address,
                                Uint8Type *data,
                                Uint16Type length,
                                void(*cb)(CommonResponseCodeType))
{
    CommonResponseCodeType status = COMMON_RSP_OK;
    TickType ticksLeft;
	Uint16Type i = 0;
    DEBUG_VALUE1(EEPROM_SWI_TRC, MOD_EEPROM, "Calling Task %08X", (Uint32Type)thisObj);
    if (EEPROM_ADDR_MAX >= (address + length))
    {
        if ((thisObj->eeBusy == FALSE))
        {   
            thisObj->eeBusy = TRUE;
            thisObj->eeAddress.u16     = address;
            // The original request
            thisObj->eeAddressMemento.u16= address;
            thisObj->eeCountRemaining  = length;
            thisObj->eeData            = data;
            thisObj->eeStatus          = EE_STATUS_WORKING;
            thisObj->eeState           = EE_STATE_WRITE;
            thisObj->callback          = cb;
            /* The client may have only one command in the queue. */
            /* Let store the request and exit. It will be handled asynchronously */
            for (i=0; i<NOF_EEPromClients; i++)
            {
                if (_EEPROM_pstCommandPending[i] == NULL)
                {
                    _EEPROM_pstCommandPending[i] = thisObj;
                    break;
                }
            }
            if( GetAlarm(CTA_DATABASE_EEPROM_WRITE, &ticksLeft) == E_OK )
            {
            	DEBUG_TEXT(EEPROM_SWI_TRC, MOD_EEPROM, "Alarm Already Running");
            	//the alarm is running hence do not do anything.
            	;
            }
            else
            {
            	(void)SetRelAlarm(CTA_DATABASE_EEPROM_WRITE, MSEC(1), 0);
            }
            status = COMMON_RSP_OK;
            DEBUG_VALUE2(EEPROM_SWI_TRC, MOD_EEPROM, "Write Registered for Address %02X,%d is OK", address, length);
        }
        else
        {
            /* one have to wait that its previous command is correctly worke out, until */
            /* it may send anothe request. */
            status = COMMON_RSP_ERROR_BUSY;
            DEBUG_VALUE2(EEPROM_SWI_ERR, MOD_EEPROM, "Write Registered for Address %02X,%d is BUSY", address, length);
        }
    }
    else
    {
        /* EEROM address out of range or length of data to long */
        status = COMMON_RSP_ERROR_ADDR_OUT_OF_RANGE;
        DEBUG_VALUE2(EEPROM_SWI_ERR, MOD_EEPROM, "Write Registered for Address %02X,%d is BUSY", address, length);
    }
    return (status);
}

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
void EEPROM_vManager(void)
{
    SuccessFailType sF = SUCCESS;
//    Uint8Type ChipSelectionAddr=0;
    EEDeviceControlBlockType* highestPrioCommand = NULL;
    Uint16Type i = 0;
    // enter critical section
    semP(EEPROM_LockResource);
    // search within the command pending for the one having the highest prio
    for (i=0; i<NOF_EEPromClients; i++)
    {
        if (_EEPROM_pstCommandPending[i] != NULL)
        {
            if (highestPrioCommand == NULL)
            {
                highestPrioCommand = _EEPROM_pstCommandPending[i];
                break;
            }
        }
    }
    _EEPROM_pstCurrentCommand = highestPrioCommand;
    semV(EEPROM_LockResource);
    // Release critical section
    
    if (_EEPROM_pstCurrentCommand == NULL)
    {
        /* nothing to be handled- or device not ready
        * Reset the command variable, that may have been set, as the 
        * variable mirrored what we are working
        */
        _EEPROM_pstCurrentCommand = NULL;
        return;
    }
    switch (_EEPROM_pstCurrentCommand->eeState)
    {
    case EE_STATE_WRITE:
        /*--------------------------------------------------------------*/
        /* Segment the data within page boundaries                      */
        /*--------------------------------------------------------------*/       
        _EEPROM_pstCurrentCommand->eeSegCount = SIZEOF_EE_PAGE -
                           (_EEPROM_pstCurrentCommand->eeAddress.u16 % SIZEOF_EE_PAGE);
        if (_EEPROM_pstCurrentCommand->eeSegCount >= _EEPROM_pstCurrentCommand->eeCountRemaining)
        {
            _EEPROM_pstCurrentCommand->eeSegCount = _EEPROM_pstCurrentCommand->eeCountRemaining;
        }
        /*--------------------------------------------------------------*/
        /* Write segment                                                */
        /*--------------------------------------------------------------*/
        /* Write enable                                                 */
        /* EEProm chip address selection */
        sF = EEPROM_sfHwInit(INIT_FOR_WRITE);
        
        /* Write data                                                   */
        if (sF == SUCCESS)
        {
            sF =  EEPROM_sfHwWriteBlock(_EEPROM_pstCurrentCommand->eeAddress, _EEPROM_pstCurrentCommand->eeData, _EEPROM_pstCurrentCommand->eeSegCount);
        }
        if (sF == SUCCESS)
        {
        	DEBUG_VALUE2(EEPROM_SWI_TRC, MOD_EEPROM, "Hardware Write on Address %02X for Length %d is OK", _EEPROM_pstCurrentCommand->eeAddress.u16, _EEPROM_pstCurrentCommand->eeSegCount);
        	_EEPROM_pstCurrentCommand->eeCountRemaining -= _EEPROM_pstCurrentCommand->eeSegCount;
            if (_EEPROM_pstCurrentCommand->eeCountRemaining != 0)
            {
                _EEPROM_pstCurrentCommand->eeAddress.u16 += _EEPROM_pstCurrentCommand->eeSegCount;
                _EEPROM_pstCurrentCommand->eeData        += _EEPROM_pstCurrentCommand->eeSegCount;
                (void)SetRelAlarm(CTA_DATABASE_EEPROM_WRITE, MSEC(6), 0);
            }
            else
            {
                _EEPROM_pstCurrentCommand->eeState   = EE_STATE_IDLE;
                _EEPROM_pstCurrentCommand->eeStatus  = EE_STATUS_DONE;
                //EEPROM_vRemoveCurrentCommand();
                /* RS: set an alarm to retrigger the call of EEPROM_vManager, it will check if
                       there is another write request queued */
                (void)SetRelAlarm(CTA_DATABASE_EEPROM_WRITE, MSEC(6), 0);
            }
        }
        else
        {
            _EEPROM_pstCurrentCommand->eeState   = EE_STATE_IDLE;
            _EEPROM_pstCurrentCommand->eeStatus = (EEPROM_STATUS_ENUM)(EE_STATUS_DONE | EE_STATUS_FAILED);
            /* TODO Errorhandling flashWrite went wrong !!! */ 
            //IntComError.ErrorBits.EEPROM=ERROR_DETECTED;
            //EEPROM_vRemoveCurrentCommand();
            /* RS: set an alarm to retrigger the call of EEPROM_vManager, it will check if
                   there is another write request queued */
            (void)SetRelAlarm(CTA_DATABASE_EEPROM_WRITE, MSEC(6), 0);
        }
        break;

    case EE_STATE_IDLE:
    {
    	// The command has to be removed in the next cycle because the read and write should have some gap between them.
    	_EEPROM_pstCurrentCommand->eeBusy = FALSE;
        for (i=0; i<NOF_EEPromClients; i++)
        {
            if (_EEPROM_pstCommandPending[i] == _EEPROM_pstCurrentCommand)
            {
                if (_EEPROM_pstCommandPending[i]->callback != NULL)
                {
                    _EEPROM_pstCommandPending[i]->callback(COMMON_RSP_OK);
                }
                _EEPROM_pstCommandPending[i] = NULL;
            }
        }
        _EEPROM_pstCurrentCommand = NULL;
        // Check if there are further pending commands in the queue
        (void)SetRelAlarm(CTA_DATABASE_EEPROM_WRITE, MSEC(6), 0);
    	break;
    }

    case EE_STATE_READ:
    default:
        break;
    }
}
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
FlagType EEPROM_flagIsEEFree(EEDeviceControlBlockType *this)
{
   	return(!this->eeBusy);
}

/* End Of File EEPROM_C1.c */
