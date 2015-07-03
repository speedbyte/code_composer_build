/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:   APP_PHONEBOOK.h
*-----------------------------------------------------------------------------
* Module Name:  APP_PHONEBOOK    
*-----------------------------------------------------------------------------
* Description:  Global Interfaces, Enum, Structs for APP_PHONEBOOK
*-----------------------------------------------------------------------------
* $Date: 2011-04-27 17:43:05 +0200 (Mi, 27 Apr 2011) $
* $Rev: 19363 $
* $Author: nikolov $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_PHONEBOOK/trunk/inc/APP_PHONEBOOK.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _APP_PHONEBOOK_H
#define _APP_PHONEBOOK_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include "common.h"
#include "APP_DEVICE.h"
/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "APP_PHONEBOOK_CE.h"
#include "APP_COMMON_UTILS.h"
#include "APP_SD_FrameDataBase.h"
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
#ifndef IAPP_PHONEBOOK_INTERNAL
#define IAPP_PHONEBOOK_INTERNAL
#endif
#ifdef IAPP_PHONEBOOK_INTERNAL /* BEGIN DECLARE INTERFACE(IAPP_PHONEBOOK_INTERNAL) */
void _APP_PHONEBOOK_vOnUpdatePbResult(ATResponseEnum rsp);
void _APP_PHONEBOOK_vOnActivatePpdsResult(ATResponseEnum rsp);
void _APP_PHONEBOOK_vOnReadCompletePbResult(ATResponseEnum rsp);
void _APP_PHONEBOOK_vOnSearchPbResult(ATResponseEnum rsp);
void _APP_PHONEBOOK_vOnSyncPbResult(ATResponseEnum rsp);
void _APP_PHONEBOOK_vOnCallStackResetResult(ATResponseEnum rsp);
void _APP_PHONEBOOK_vOnPbSortOrderResult(ATResponseEnum rsp);
void _APP_PHONEBOOK_vOnAddPbEntryResult(ATResponseEnum rsp);
void _APP_PHONEBOOK_vOnUpdateSinglePbSize(ATResponseEnum rsp);
void _APP_PHONEBOOK_vOnReqNumTagsResult(ATResponseEnum rsp);
void _APP_PHONEBOOK_vReleaseFctContext();
void _APP_PHONEBOOK_vNotifyApps(APP_PB_enNotificationId event);
#endif                         /* END DECLARE INTERFACE(IAPP_PHONEBOOK_INTERNAL) */

#ifndef IAPP_PHONEBOOK_EXTERNAL
#define IAPP_PHONEBOOK_EXTERNAL
#endif
#ifdef IAPP_PHONEBOOK_EXTERNAL /* BEGIN DECLARE INTERFACE(IAPP_PHONEBOOK_EXTERNAL) */
SuccessFailType APP_PHONEBOOK_sfInitApp(void);
void APP_PHONEBOOK_vHandleEvents(EventMaskType event);

APP_COMMON_enResponseCode APP_PHONEBOOK_rcSearchForVoicemailboxNumber(
                            void (*cb)(APP_COMMON_enResponseCode), 
                            APP_SD_ENUM_LANGUAGES language);

APP_COMMON_enResponseCode APP_PHONEBOOK_rcRequestPbSearch(
                            void (*cb)(APP_COMMON_enResponseCode), 
                            const Uint8Type *pStr, 
                            Uint16Type strLen);

APP_COMMON_enResponseCode APP_PHONEBOOK_rcRequestCompletePbEntries(
                            void (*cb)(APP_COMMON_enResponseCode),   
                            APP_PHONEBOOK_enSelectedPbType pbType,
                            Uint16Type startIndex,
                            Uint16Type elements);

APP_COMMON_enResponseCode APP_PHONEBOOK_rcSetPbSortOrder(
                            void (*cb)(APP_COMMON_enResponseCode),
                            Uint8Type mode,
                            BooleanType IsPBOrderChangedBeforeProfileAdded);

APP_COMMON_enResponseCode APP_PHONEBOOK_rcRequestCurrentPbSortOrder(
                            void (*cb)(APP_COMMON_enResponseCode));

APP_COMMON_enResponseCode APP_PHONEBOOK_rcRequestNumVoiceTagsForContact(
                            void (*cb)(APP_COMMON_enResponseCode),
                            Uint16Type pbIndex); 
/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONEBOOK_rcRequestCallStackReset                        */
/** 
    \brief      requests deletion of call stacks ( missed , dialed, received ) 
     
    \return     APP_COMMON_enResponseCode
                APP_COMMON_RSP_OK or APP_COMMON_RSP_ERROR 
*/
/*----------------------------------------------------------------------------*/

APP_COMMON_enResponseCode APP_PHONEBOOK_rcRequestCallStackReset(
        void (*cb)(APP_COMMON_enResponseCode));

/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONEBOOK_rcRequestAddPbEntry                            */
/** 
    \brief      requests ck5050 to add a given pb entry to users pb
     
    \return     APP_COMMON_enResponseCode
                APP_COMMON_RSP_OK or APP_COMMON_RSP_ERROR 
*/
/*----------------------------------------------------------------------------*/

APP_COMMON_enResponseCode APP_PHONEBOOK_rcRequestAddPbEntry(
                                void (*cb)(APP_COMMON_enResponseCode),
                                const Uint8Type * name, 
                                Uint16Type name_len, 
                                const Uint8Type * number, 
                                Uint16Type number_len,
                                ATPhoneNumberTypeEnum pnType);


/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONEBOOK_rcRequestManualSync                            */
/** 
    \brief      requests to start a manual PB synchronisation  
  
    \brief      Description:\n
                requests to start a manual PB synchronisation 

    
    \return     APP_COMMON_enResponseCode
                APP_COMMON_RSP_OK or APP_COMMON_RSP_ERROR 
*/
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode APP_PHONEBOOK_rcRequestManualSync(
                                        void (*cb)(APP_COMMON_enResponseCode));



Uint16Type              APP_PHONEBOOK_u16GetCurrentPBSize(APP_PHONEBOOK_enSelectedPbType pbType);
BooleanType             APP_PHONEBOOK_bIsPbValid(APP_PHONEBOOK_enSelectedPbType pbType);
const AppPbEntryType *  APP_PHONEBOOK_pGetPpbrContext(Uint16Type index);
Uint16Type              APP_PHONEBOOK_GetVoiceTagNumberResponse();
ATPhoneBookDwnlState    APP_PHONEBOOK_enGetPBDwldState();
BooleanType             APP_PHONEBOOK_bPbUpdateInProgress();
/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONEBOOK_pGetSortOrderContext                          */
/** 
    \brief      returns current Sortorder 
  
    \brief      Description:\n
                returns current sort order

    \param      void 
    
    \return     Uint8Type  
   
*/
/*----------------------------------------------------------------------------*/
Uint8Type APP_PHONEBOOK_u8GetSortOrder();

/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONEBOOK_u16GetAddedPbIndex                             */
/** 
    \brief      returns newly added pbIndex 
  
    \brief      Description:\n
                returns newly added pbIndex

    \param      void 
    
    \return     Uint16Type  
   
*/
/*----------------------------------------------------------------------------*/
Uint16Type APP_PHONEBOOK_u16GetAddedPbIndex();

/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONEBOOK_pGetPbSearchResult                             */
/** 
    \brief      returns pb search result 
  
    \brief      Description:\n
                returns pb search result

    \param      pIndex
                const Uint16Type * 
    
    \param      pNumber
                const Uint16Type *

    \return     SuccessFailType 
   
*/
/*----------------------------------------------------------------------------*/
SuccessFailType APP_PHONEBOOK_pGetPbSearchResult(Uint16Type *pIndex,
                                                 Uint16Type *pNumber);


/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONEBOOK_u16GetPbDlStatus                               */
/** 
    \brief       
  
    \brief      Description:\n

    \param      void 
    
    \return     Uint16Type  
*/
/*----------------------------------------------------------------------------*/
Uint16Type APP_PHONEBOOK_u16GetPbDlStatus();


BooleanType APP_PHONEBOOK_bWasManualSync();
void APP_PHONEBOOK_vResetMissedCallsByHMI();

void APP_PHONEBOOK_vOnAppDeviceNotification(APP_DEVICE_enNotificationId notification);


/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONEBOOK_vHandleATResponse(ATAllResponseType *atRsp)       */
/**
    \brief      Inits the phonebook application

    \brief      Description:\n
                Inits the Phonebook Application \n

    \param      atRsp
                ATRspMessageType *
                response or asynchron message struct from AT Parser

    \return     void

*/
/*----------------------------------------------------------------------------*/
void APP_PHONEBOOK_vHandleATResponse(const ATRspMessageType *atRsp);

#endif

/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/


#endif /* _APP_PHONEBOOK_H */

/* End Of File APP_PHONEBOOK.h */

