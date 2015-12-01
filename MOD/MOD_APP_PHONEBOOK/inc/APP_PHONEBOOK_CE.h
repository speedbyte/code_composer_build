/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:   APP_PHONEBOOK_CE.h
*-----------------------------------------------------------------------------
* Module Name:  APP_PHONEBOOK
*-----------------------------------------------------------------------------
* Description:  
*-----------------------------------------------------------------------------
* $Date: 2011-04-27 17:43:05 +0200 (Mi, 27 Apr 2011) $
* $Rev: 19363 $
* $Author: nikolov $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_PHONEBOOK/trunk/inc/APP_PHONEBOOK_CE.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _APP_PHONEBOOK_CE_H
#define _APP_PHONEBOOK_CE_H

/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Import configuration                                                       */
/*----------------------------------------------------------------------------*/
#include "ATParser.h"
/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Constants                                                           */
/*----------------------------------------------------------------------------*/

/* HU should request max 5 elements in one GetArray request */
#define MAX_REQUESTED_PB_ENTRIES                5
#define MAX_REQUESTED_PB_NUMBERS_PER_ENTRY      5
#define MAX_PB_SEARCH_STRING                    50

typedef enum
{
    APP_PHONEBOOK_enUsersPB             = 0x00,
    APP_PHONEBOOK_enLastDialed          = 0x01, 
    APP_PHONEBOOK_enReceivedCalls       = 0x02,
    APP_PHONEBOOK_enMissedCalls         = 0x03,
    APP_PHONEBOOK_enPbUnknown   = 0xFF
}APP_PHONEBOOK_enSelectedPbType;

typedef enum
{
    APP_PHONEBOOK_STATUS_NOT_AVAILABLE,
    APP_PHONEBOOK_STATUS_DOWNLOAD_IPR,
    APP_PHONEBOOK_STATUS_EMPTY,
    APP_PHONEBOOK_STATUS_AVAILABLE     
}APP_PHONEBOOK_enStatusCode;


typedef enum
{
/* 0  */    APP_PB_USERS_PB_CHANGED,
/* 1  */    APP_PB_USERS_PB_CHANGED_INCOMPLETE,
/* 2  */    APP_PB_MISSED_PB_CHANGED,
/* 3  */    APP_PB_RECEIVED_PB_CHANGED,
/* 4  */    APP_PB_DIALED_PB_CHANGED,
/* 5  */    APP_PB_MISSED_PB_NOT_CHANGED,
/* 6  */    APP_PB_RECEIVED_PB_NOT_CHANGED,
/* 7  */    APP_PB_DIALED_PB_NOT_CHANGED,
/* 8  */    APP_PB_DOWNLOAD_STATE_CHANGED,
/* 9  */    APP_PB_DOWNLOAD_COUNT_CHANGED,
/* 10 */    APP_PB_ENTRY_ADDED,
/* 11 */    APP_PB_USER_PB_UPDATE_FINISHED,
/* 12 */    APP_PB_USER_PB_READ_FINISHED,
/* 13 */    APP_PB_USERS_PB_NOT_CHANGED,
/* 14 */    APP_PB_USERS_PB_NOT_AVAILABLE,
/* 15 */    APP_PB_USERS_PB_NOT_CHANGED_INCOMPLETE,
/* 16 */    APP_PB_MISSED_CALLS_RESET_BY_HMI,
/* 17 */    APP_PB_SORTING_STARTED,
/* 18 */    APP_PB_SORTING_TIMED_OUT,
/* 18 */    APP_PB_DOWNLOAD_STATE_CHANGED_RETRY_PB_SORTING_FOR_NEWUSER
} APP_PB_enNotificationId;

//typedef struct
//{ 
//    Uint16Type              start;          /* ppbp start index */
//    Uint16Type              end;            /* ppbp end index */
//    ATRspPBReadNumberType   *pPpbp[MAX_REQUESTED_PB_NUMBERS_PER_ENTRY];    
//}AppPbReadNumbersContext;
//
//
//typedef struct 
//{
//    APP_PHONEBOOK_enReadStateType   requestState;   /* current state */
//    Uint16Type                      start;      /* ppbr start index */
//    Uint16Type                      end;        /* ppbr end index */
//    AppPbReadNumbersContext         pbNumbers[MAX_REQUESTED_PB_ENTRIES];
//}AppPbReadEntriesContext;

typedef struct
{
    BooleanType                     isValid;
    ATRspPBReadNumberType           ppbp;

}AppPbNumberEntryType;

typedef struct
{
    BooleanType                     isValid;
    ATRspPBReadEntryType            ppbr;
    AppPbNumberEntryType            number[MAX_REQUESTED_PB_NUMBERS_PER_ENTRY];
}AppPbEntryType;


#endif /* _APP_PHONEBOOK_CE_H */

/* End Of File APP_PHONEBOOK_CE.h */
