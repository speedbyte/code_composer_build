/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/*****************************************************************************/
/* File Name:   APP_PHONEBOOK_CI.h
*-----------------------------------------------------------------------------
* Module Name:  APP_PHONEBOOK
*-----------------------------------------------------------------------------
* Description:  modul global constants, declarations
*-----------------------------------------------------------------------------
* $Date: 2011-04-27 17:43:05 +0200 (Mi, 27 Apr 2011) $
* $Rev: 19363 $
* $Author: nikolov $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_PHONEBOOK/trunk/inc/APP_PHONEBOOK_CI.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _APP_PHONEBOOK_CI_H
#define _APP_PHONEBOOK_CI_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include "common.h"

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "APP_PHONEBOOK.h"

#include "APP_COMMON_Utils.h" 
#include "BAP_Dispatcher.h"

/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/
//SUSANNE 14.11.2009 As specified by Parrot the PPBC event after PPNO=x
// will be sent after 30 seconds. But its not always sent within 30 seconds
// i increase the value here for kindness and make a ticket for Parrot     
#define APP_PHONEBOOK_SORTING_TIMEOUT 40000

/*----------------------------------------------------------------------------*/
/* Import configuration                                                       */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of configured interface
*/

/*----------------------------------------------------------------------------*/
/* Synonym adaptions                                    */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the synonym adaption
*/

/*----------------------------------------------------------------------------*/
/* module global enums                                                        */
/*----------------------------------------------------------------------------*/


/*  states to update the sizes for refreshed phonebooks */
typedef enum
{
    APP_PHONEBOOK_UPDATE_NOT_SENT      = 0x00,
    APP_PHONEBOOK_SELECT_USER_PB_SENT,
    APP_PHONEBOOK_REQUEST_USER_PB_SIZE_SENT,
    APP_PHONEBOOK_SELECT_MISSED_PB_SENT,
    APP_PHONEBOOK_REQUEST_MISSED_PB_SIZE_SENT,
    APP_PHONEBOOK_SELECT_RECEIVED_PB_SENT,
    APP_PHONEBOOK_REQUEST_RECEIVED_PB_SIZE_SENT,
    APP_PHONEBOOK_SELECT_DIALED_PB_SENT,
    APP_PHONEBOOK_REQUEST_DIALED_PB_SIZE_SENT
}APP_PHONEBOOK_enPbSizeUpdateStateType;

/*  states to update the sizes for refreshed phonebooks */
typedef enum
{
    APP_PHONEBOOK_UPDATE_PB_SIZE_NOT_SENT      = 0x00,
    APP_PHONEBOOK_SELECT_PB_2_SENT,
    APP_PHONEBOOK_REQUEST_PB_SIZE_SENT
}APP_PHONEBOOK_enSinglePbSizeUpdateStateType;


typedef enum
{
    APP_PHONEBOOK_ACTIVATE_PPDS_NOT_SENT = 0x00,
    APP_PHONEBOOK_ACTIVATE_PPDS_SENT
}APP_PHONEBOOK_enActivatePpdsStateType;

/*  states to be processed to request phonebook entries from a special 
    phonebook and states after requesting phonebook entries and waiting for
    them to arrive */
typedef enum
{
    APP_PHONEBOOK_PB_READ_NOT_SENT        = 0x00,
    APP_PHONEBOOK_SELECT_PB_SENT,
    APP_PHONEBOOK_READ_PB_ENTRIES_SENT,
    APP_PHONEBOOK_READ_PB_NUMBERS_SENT
}APP_PHONEBOOK_enReadStateType;

/*  states to be processed for manual pb sync */
typedef enum
{
    APP_PHONEBOOK_PB_SYNC_NOT_SENT        = 0x00,
    APP_PHONEBOOK_PB_SYNC_SENT
}APP_PHONEBOOK_enPbManualSyncStateType;

/*  states to be processed for adding a new contact */
typedef enum
{
    APP_PHONEBOOK_ADD_PB_ENTRY_NOT_SENT        = 0x00,
    APP_PHONEBOOK_ADD_PB_ENTRY_SENT
}APP_PHONEBOOK_enAddPbEntryStateType;



/*  states to be processed for call stack reset */
typedef enum
{
    APP_PHONEBOOK_PB_CALL_STACK_RESET_NOT_SENT        = 0x00,
    APP_PHONEBOOK_PB_CALL_STACK_RESET_SENT
}APP_PHONEBOOK_enPbCallStackResetStateType;


typedef enum
{
    APP_PHONEBOOK_PB_SORTORDER_NOT_SENT        = 0x00,
    APP_PHONEBOOK_PB_SORTORDER_SENT
}APP_PHONEBOOK_enPbSortOrderStateType;

typedef enum
{
    APP_PHONEBOOK_PB_REQ_NUM_TAGS_NOT_SENT        = 0x00,
    APP_PHONEBOOK_PB_REQ_NUM_TAGS_SENT,
    APP_PHONEBOOK_PB_REQ_NUM_TAGS_WAIT_RSP,
    APP_PHONEBOOK_PB_REQ_NUM_TAGS_END
}APP_PHONEBOOK_enPbNumTagsStateType;



typedef enum
{
//    APP_PHONEBOOK_PB_SEARCH_NOT_SENT        = 0x00,
//    APP_PHONEBOOK_PB_SEARCH_START_SENT,
//    APP_PHONEBOOK_PB_SEARCH_END_SENT

    APP_PHONEBOOK_EXTENDED_PB_SEARCH_NOT_SENT = 0x00,
    APP_PHONEBOOK_EXTENDED_PB_SEARCH_SENT
}APP_PHONEBOOK_enSearchStateType;


typedef enum
{
    APP_PB_enInvalidFct = 0,
    APP_PB_enReadCompletePbEntries,
    APP_PB_enManualPbSync,
    APP_PB_enAddPbEntry,
    APP_PB_enDeleteCallStacks,
    APP_PB_enUpdatePbSizes,
    APP_PB_enSearchPb,
    APP_PB_enGetPbSortOrder,
    APP_PB_enSetPbSortOrder,
    APP_PB_enActivatePpds,
    APP_PB_enSearchVoiceMailbox,
    APP_PB_enUpdateSinglePbSize,
    APP_PB_enRequestNumVoiceTagsForContact
}APP_PHONEBOOK_enFctId;

/*----------------------------------------------------------------------------*/
/* Prototypes of module global methods                                        */
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_PHONEBOOK_sfUpdatePbSizes();
SuccessFailType _APP_PHONEBOOK_sfUpdateSinglePbSize(APP_PHONEBOOK_enSelectedPbType callStackType);
SuccessFailType _APP_PHONEBOOK_sfActivatePpds(BooleanType enable);
void _APP_PHONEBOOK_vReInitApp(void);
void _APP_PHONEBOOK_vSendResultReleaseContext(APP_COMMON_enResponseCode result);


/*----------------------------------------------------------------------------*/
/* Module global code macros                                                         */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the macros
*/
//#define sqrt(x) ((x)*(x))

/*----------------------------------------------------------------------------*/
/* Module global variables                                                    */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the module global variable
*/




/*----------------------------------------------------------------------------*/
/* Module global const definitions                                                  */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the define
*/
#define APP_PB_NONE_REFRESHED               0x00
#define APP_PB_USERS_PERSONAL_REFRESHED     0x01
#define APP_PB_MISSED_CALLS_REFRESHED       0x02
#define APP_PB_DIALLED_NUMBERS_REFRESHED    0x04
#define APP_PB_RECEIVED_CALLS_REFRESHED     0x08

#define CKCM_ERROR_CODE_PB_SYNC_IN_PROGRESS 156
/*----------------------------------------------------------------------------*/
/* Module internal struct                                                     */
/*----------------------------------------------------------------------------*/

typedef struct
{
    APP_PHONEBOOK_enFctId fctId;
    void (*callback)(APP_COMMON_enResponseCode);
}AppPbCommonType;

typedef struct
{
    BooleanType     bIsValid;
    Uint16Type      pbSize;
}CallListContext;

//typedef struct
//{
//    AppPbCommonType                 common;
//    APP_PHONEBOOK_enSelectedPbType  pbType;
//    AppPbReadEntriesContext         entries; 
//}PbReadContext;

typedef struct
{
    AppPbCommonType                 common;
    APP_PHONEBOOK_enReadStateType   state;   /* current state */
    Uint16Type                      start;   /* ppbr start index */
    Uint16Type                      end;     /* ppbr end index */
    Uint16Type                      pbIndex; /* pb index to an entry with more
                                              * then 1 number */
    Uint16Type                      pnSize;

    APP_PHONEBOOK_enSelectedPbType  pbType;
    AppPbEntryType                  entry[MAX_REQUESTED_PB_ENTRIES]; 
}PbReadContext;

typedef struct
{
    AppPbCommonType                 common;
    APP_PHONEBOOK_enSearchStateType state;   /* current state */
    
    Uint8Type                       searchStr[MAX_PB_SEARCH_STRING];
    Uint16Type                      searchStrLen;

    Uint16Type                      firstIndex;
//    Uint16Type                      endIndex;
    Uint16Type                      numMatchingEntries;
}PbSearchContext;

typedef struct
{
    AppPbCommonType                 common;
    APP_PHONEBOOK_enSearchStateType state;   /* current state */

    APP_SD_ENUM_LANGUAGES           lang;    

    Uint16Type                      firstIndex;
    Uint16Type                      endIndex;
}PbSearchVoiceMailboxContext;



typedef struct
{
    AppPbCommonType                         common;
    APP_PHONEBOOK_enPbManualSyncStateType   state;   /* current state */
}ManualPbSyncContext;


typedef struct
{
    AppPbCommonType                     common;
    APP_PHONEBOOK_enAddPbEntryStateType state;   /* current state */
    AT_CallerId                         name;
    Uint16Type                          name_len; 
    ATPhoneNumberType                   number; 
    Uint16Type                          number_len;
    ATPhoneNumberTypeEnum               pnType;
    Uint16Type                          newIndex; // response will be saved here
}AddPbEntryContext;



typedef struct
{
    AppPbCommonType                         common;
    Uint8Type                               mode;
    BooleanType                             NewUserIsAdded;
    APP_PHONEBOOK_enPbSortOrderStateType    state;   /* current state */
}PbSortOrderContext;

typedef struct
{
    AppPbCommonType                         common;
    Uint16Type                              pbIndex;
    Uint16Type                              num;
    APP_PHONEBOOK_enPbNumTagsStateType      state;   /* current state */
} PbReqNumVoiceTagsContext;

typedef struct
{
    AppPbCommonType                             common;
    APP_PHONEBOOK_enPbCallStackResetStateType   state;   /* current state */
}CallStackResetContext;

typedef struct
{
    AppPbCommonType                         common;
    APP_PHONEBOOK_enPbSizeUpdateStateType   state;
}UpdatePbSizesContext;

typedef struct
{
    AppPbCommonType                                 common;
    APP_PHONEBOOK_enSelectedPbType                  pbType;  
    APP_PHONEBOOK_enSinglePbSizeUpdateStateType     state;
}UpdSinglePbSizeContext;

typedef struct
{
    AppPbCommonType                         common;
    APP_PHONEBOOK_enActivatePpdsStateType   state;
    BooleanType                             enable;
}ActivatePpdsContext;

typedef struct
{
    BooleanType     activatePpds;
    BooleanType     pbSizeUpdate;
    BooleanType     pbCompletelyLoaded;
    // update single pb / stack size    
    BooleanType     updMissedStackSize;
    BooleanType     updDialedStackSize;
    BooleanType     updReceivedStackSize;
    BooleanType     updPbSize;
    BooleanType     notifyPbSortingTimout;

}InternalFctPending;

   
typedef struct
{
    /* current state when loading / refreshing phonebook(s) */
    ATPhoneBookDwnlState                dwldState; 
    Uint16Type                          dwldCount; 
    /* current selected Phonebook */
    APP_PHONEBOOK_enSelectedPbType      curSelectedPb;  
    /* save sizes of available phonebooks */ 
    CallListContext                     usersPersonalPb;
    CallListContext                     missedCallsPb;
    CallListContext                     receivedCallsPb;
    CallListContext                     dialledNumbersPb;
    
    InternalFctPending                  pending;

    BooleanType                         bWasManualSync;

    Uint8Type                           updatedPbIndicator;

    BooleanType                         bFirstPPBU0ReceivedAlready;
    
    BooleanType                         notifyPBSortingAfterNewUserFailed;

    BooleanType                         SortingOrderChangedSentByUser;

    // add struct for each fct which can be pending and having
    // more than one state
    union
    {
        AppPbCommonType                 common;
        PbReadContext                   pbRead;
        UpdatePbSizesContext            updatePbSizes;
        ActivatePpdsContext             activatePpds;
        PbSearchContext                 pbSearch;
        PbSearchVoiceMailboxContext     voiceMailboxSearch;
        PbSortOrderContext              pbSortOrder;
        PbReqNumVoiceTagsContext        numVoiceTags;
        ManualPbSyncContext             pbSync;
        AddPbEntryContext               pbAddEntry;
        CallStackResetContext           pbCallStackRst;
        UpdSinglePbSizeContext          updSinglePbSize;
    }fct;
}AppPhoneBookContext;


#endif /* _APP_PHONEBOOK_CI_H */

/* End Of File APP_PHONEBOOK_CI.h */
