/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:   APP_PHONEBOOK_AT.h
*-----------------------------------------------------------------------------
* Module Name:  APP_PHONEBOOK
*-----------------------------------------------------------------------------
* Description:  Module Global Interfaces, Enum, Structs for APP_PHONEBOOK
*-----------------------------------------------------------------------------
* $Date: 2010-03-15 12:13:46 +0100 (Mo, 15 Mrz 2010) $
* $Rev: 11558 $
* $Author: weber $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_PHONEBOOK/trunk/inc/APP_PHONEBOOK_AT.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _APP_PHONEBOOK_AT_H
#define _APP_PHONEBOOK_AT_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include "osek.h"

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "UART.h"
#include "APP_PHONEBOOK_CE.h"
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
/* Public Methods                                                             */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONEBOOK_AT_ePhonebookStatusGet(void)       */
/**
    \brief      return the current phonebook status

    \brief      Description:\n
                return the current phonebook status \n

    \return     APP_PHONEBOOK_enStatusCode

*/
/*----------------------------------------------------------------------------*/
APP_PHONEBOOK_enStatusCode APP_PHONEBOOK_AT_ePhonebookStatusGet(void);

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_AT_sfSendPbCallStackResteATRequest                    */
/**
    \brief      Send AT Request to reset call stacks \n

    \brief      Description:\n
                Send AT Request to reset call stacks\n

    \param      puart
                const SCI_ST*

    \return     SuccessFailType
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType
_APP_PHONEBOOK_AT_sfSendPbCallStackResetATRequest( const SCI_ST * puart );


SuccessFailType
_APP_PHONEBOOK_AT_sfSendSelectPbATRequest( APP_PHONEBOOK_enSelectedPbType pbToSelect,
                                           const SCI_ST *puart );

SuccessFailType
_APP_PHONEBOOK_AT_sfSendPbSizeATRequest(const SCI_ST* puart);

SuccessFailType
_APP_PHONEBOOK_AT_sfSendPbNumbersATRequest( const SCI_ST*puart,
                                            Uint16Type pbIndex,
                                            Uint16Type start,
                                            Uint16Type end );
SuccessFailType
_APP_PHONEBOOK_AT_sfSendPbEntriesATRequest(const SCI_ST*puart,
                                           Uint16Type start,
                                           Uint16Type end,
                                           Uint16Type pbSize  );

SuccessFailType
_APP_PHONEBOOK_AT_sfSendPbSyncATRequest( const SCI_ST * puart );

SuccessFailType
_APP_PHONEBOOK_AT_sfSendGetPhonebookUpdateStatus( const SCI_ST * puart );


SuccessFailType
_APP_PHONEBOOK_AT_sfSendNumVoiceTagsATRequest(const SCI_ST * puart,
                                              Uint16Type pbIndex);

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_AT_sfSendPbSortOrderATRequest                    */
/**
    \brief      Send AT Request to get current sort order of Phonebook \n

    \brief      Description:\n
                Send AT Request to get current sort order of Phonebook \n

    \param      puart
                const SCI_ST*

    \param      mode
                Uint8Type

    \return     SuccessFailType
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType
_APP_PHONEBOOK_AT_sfSendPbSortOrderATRequest(const SCI_ST * puart,
                                             Uint8Type mode );

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_AT_sfSendAddPbEntryATRequest                    */
/**
    \brief      Send AT Request to add new contact to Phonebook \n

    \brief      Description:\n
                Send AT Request to add new contact to Phonebook \n

    \param      puart
                const SCI_ST*

    \param      name
                const AT_CallerId

    \param      name_len
                Uint16Type

    \param      number
                const ATPhoneNumberType

    \param      number_len
                Uint16Type

    \param      pnType
                ATPhoneNumberTypeEnum

    \return     SuccessFailType
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType
_APP_PHONEBOOK_AT_sfSendAddPbEntryATRequest(const SCI_ST * puart,
                                            const AT_CallerId name,
                                            Uint16Type name_len,
                                            const ATPhoneNumberType number,
                                            Uint16Type number_len,
                                            ATPhoneNumberTypeEnum pnType);


/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_AT_sfSendPbSearchATRequest                    */
/**
    \brief      Send AT Request to search Phonebook for the given search string \n

    \brief      Description:\n
                Send AT Request to search Phonebook for the given search string \n

    \param      searchStr
                const Uint8Type *

    \param      searchStrLen
                Uint8Type

    \param      puart
                const SCI_ST*

    \return     SuccessFailType
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType
_APP_PHONEBOOK_AT_sfSendPbSearchATRequest(const Uint8Type *searchStr,
                                          Uint8Type searchStrLen,
                                          const SCI_ST * puart );

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_AT_sfSendPbSearchExtendedATRequest                    */
/**
    \brief      Send AT Request to ( extended ) search Phonebook for the given search string \n

    \brief      Description:\n
                Send AT Request to ( extended ) search Phonebook for the given search string \n

    \param      searchStr
                const Uint8Type *

    \param      searchStrLen
                Uint8Type

    \param      puart
                const SCI_ST*

    \return     SuccessFailType
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType
_APP_PHONEBOOK_AT_sfSendPbSearchExtendedATRequest(const Uint8Type *searchStr,
                                          Uint8Type searchStrLen,
                                          const SCI_ST * puart );



/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_AT_sfSendActivatePpds                         */
/**
    \brief      Send AT Request to activate PPDS events \n

    \brief      Description:\n
                Send AT Request to activate PPDS events \n

    \param      enable
                BooleanType

    \param      puart
                const SCI_ST*

    \return     SuccessFailType
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType
_APP_PHONEBOOK_AT_sfSendActivatePpds(BooleanType enable, const SCI_ST * puart);

#endif /* _APP_PHONEBOOK_AT_H */

/* End Of File APP_PHONEBOOK_AT.h */

