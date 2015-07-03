/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     APP_PHONEBOOK_AT_C1.c
*-----------------------------------------------------------------------------
* Module Name:    Phonebook Application
*-----------------------------------------------------------------------------
* Description:    Handles all phonebook specific responses from
                  AT Cmd Parser
*-----------------------------------------------------------------------------
* $Date: 2011-11-07 10:01:13 +0100 (Mo, 07 Nov 2011) $
* $Rev: 23070 $
* $Author: maennel $
* $URL:
* http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_PHONE/trunk/src/APP_PHONEBOOK_AT_C1.c $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/
#include <osek.h>           // vector
#include <osekext.h>        // vector
#include "bap_types.h"



/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "POOL.h"
#include "APP_SD.h"
#include "APP_COMMON_Utils.h"
#include "ATCmdHandler.h"
#include "ATParser.h"

#include "APP_PHONEBOOK_CI.h"
#include "APP_PHONEBOOK.h"
#include "APP_PHONEBOOK_AT.h"
#include "APP_PHONEBOOK_ERM.h"

#include "APP_PHONEBOOK_AT_C1.id"
#include "TRACE.h"
/*----------------------------------------------------------------------------*/
/* pragma Directives                                                          */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Definitions (avoid as much as possible!)                  */
/*----------------------------------------------------------------------------*/

extern AppPhoneBookContext gAppPBContext;
//extern Uint16Type	pbDwldEntry;
//BooleanType				firstTimePB_dwl = BT_TRUE;				// fix for phonebook not downloaded at the startup
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

static SuccessFailType
_APP_PHONEBOOK_AT_sfHandlePPBC(const ATRspPpbcType *pPpbc);

static SuccessFailType
_APP_PHONEBOOK_AT_sfHandlePPBU(const ATRspPpbuType *pPpbu);

static SuccessFailType
_APP_PHONEBOOK_AT_sfHandlePFPA();

static SuccessFailType
_APP_PHONEBOOK_AT_sfHandlePPBR(ATRspPBReadEntryType *pPpbr);

static SuccessFailType
_APP_PHONEBOOK_AT_sfHandlePPBP(const ATRspPBReadNumberType *pPpbp);

static SuccessFailType
_APP_PHONEBOOK_AT_sfHandlePbSizeResponse(const ATRspPBReadEntryType *pPpbr);

static SuccessFailType
_APP_PHONEBOOK_AT_sfHandlePbEntriesResponse(const ATRspPBReadEntryType *pPpbr);

//static SuccessFailType
//_APP_PHONEBOOK_AT_sfHandlePbSearchResponse(const ATResPBAlphaSearchType *pPpas);

static SuccessFailType
_APP_PHONEBOOK_AT_sfHandlePbSearchExtResponse(const ATResPBAlphaSearchExtType *pPpas);

static SuccessFailType
_APP_PHONEBOOK_AT_sfHandlePbSortOrderResponse(const ATResPBSortModeType * pPpno);

static SuccessFailType
_APP_PHONEBOOK_AT_sfHandleAddPbEntryResponse(const ATResPBAddEntryType * pPpad);

static SuccessFailType
_APP_PHONEBOOK_AT_sfHandlePPDS(const ATRspPbDwnlStatusType *pPpds);

static SuccessFailType
_APP_PHONEBOOK_AT_sfHandleNumVoiceTags(const ATResVTMgtType *pRvta);

/*----------------------------------------------------------------------------*/
/* File local Code Macros                                                     */
/*----------------------------------------------------------------------------*/
/**
\brief	description of the macros
*/

/*----------------------------------------------------------------------------*/
/* File local Variables                                                       */
/*----------------------------------------------------------------------------*/


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
/* Function    : _APP_PHONEBOOK_vHandleATResponse(ATAllResponseType *atRsp)       */
/**
    \brief      Inits the phonebook application

    \brief      Description:\n
                Inits the Phonebook Application \n

    \return     APP_PHONEBOOK_enStatusCode
*/
/*----------------------------------------------------------------------------*/
APP_PHONEBOOK_enStatusCode APP_PHONEBOOK_AT_ePhonebookStatusGet(void)
{
    APP_PHONEBOOK_enStatusCode eRetVal = APP_PHONEBOOK_STATUS_NOT_AVAILABLE;

    switch (gAppPBContext.dwldState)
    {
        case AT_PB_LOADING:
        {
            eRetVal = APP_PHONEBOOK_STATUS_DOWNLOAD_IPR;
            break;
        }
        case AT_PB_LOAD_FINISHED:
        case AT_PB_LOAD_FINISHED_BUT:
        {   /* check if there are entries in the phonebook */
            if (0 == gAppPBContext.usersPersonalPb.pbSize)
            {
                eRetVal = APP_PHONEBOOK_STATUS_EMPTY;
            }
            else
            {
                eRetVal = APP_PHONEBOOK_STATUS_AVAILABLE;
            }
            break;
        }
//        case BAP_PB_LOAD_ABORTED:
//        /* default states for APP_PHONEBOOK_STATUS_NOT_AVAILABLE */
//        case BAP_PB_NOT_AVAILABLE:
        default:
        {
            break;
        }
    }
    return eRetVal;
}


/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONEBOOK_vHandleATResponse                                */
/**
    \brief      handle response msgs from AT Cmd Parser

    \brief      Description:\n
                handles responses from AT Cmd Parser \n

    \param      atRsp
                ATRspAllType *

    \return     void

*/
/*----------------------------------------------------------------------------*/
void APP_PHONEBOOK_vHandleATResponse(const ATRspMessageType *atRsp)
{
    SuccessFailType result = SUCCESS;

    // check the message
    if (AT_RSP_WITH_DATA == atRsp->dataInd && NULL == atRsp->data )
    {
        // seems to be something wrong with the message, ignore it
        return;
    }
    else if (   AT_RSP_WITHOUT_DATA == atRsp->dataInd &&
                NULL != atRsp->data )
    {
        return;
    }

    switch( atRsp->common.rsp_type )
    {
        case AT_RSP_PB_AVAIL_NAV:
            result = _APP_PHONEBOOK_AT_sfHandlePFPA();
            break;
        case AT_RSP_PB_CHANGE_IND_EVENT:          /* PPBU */
            result = _APP_PHONEBOOK_AT_sfHandlePPBU( &atRsp->data->ppbu);
            break;
        case AT_RSP_PB_CHANGE_IND_TYPE:           /* PPBC */
            result = _APP_PHONEBOOK_AT_sfHandlePPBC( &atRsp->data->ppbc);
            break;

        case AT_RSP_PB_READ_ENTRIES:              /* PPBR Entry or Size */
            result = _APP_PHONEBOOK_AT_sfHandlePPBR(&atRsp->data->ppbr);
            break;

        case AT_RSP_PB_READ_NUMBERS:
            result = _APP_PHONEBOOK_AT_sfHandlePPBP(&atRsp->data->ppbp);
            break;

        //case AT_RSP_PB_ALPHA_SEARCH:
        //    result = _APP_PHONEBOOK_AT_sfHandlePbSearchResponse(&atRsp->data->ppas);
        //    break;

        case AT_RSP_PB_ALPHA_SEARCH_EXT:
            result = _APP_PHONEBOOK_AT_sfHandlePbSearchExtResponse(&atRsp->data->ppasex);
            break;

        case AT_RSP_PB_NAME_ORDER_SELECT:
            result = _APP_PHONEBOOK_AT_sfHandlePbSortOrderResponse(&atRsp->data->ppno);
            break;

        case AT_RSP_PB_ADD_ENTRY:
            result = _APP_PHONEBOOK_AT_sfHandleAddPbEntryResponse(&atRsp->data->ppad);
            break;

        case AT_RSP_PB_DWNLOAD_STATUS:
            result = _APP_PHONEBOOK_AT_sfHandlePPDS(&atRsp->data->ppds);
            break;

        case AT_RSP_PB_UPDATE_STATUS:
            /* To Do write Application function*/
            break;

        case AT_RSP_SIVR_MGR_VT:
            result = _APP_PHONEBOOK_AT_sfHandleNumVoiceTags(&atRsp->data->rvta);
            break;

        default:
            break;
    }

    (void)result;
}/* END OF APP_PHONEBOOK_vHandleATResponse() */


/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_AT_sfHandlePPDS                               */
/**
    \brief      handle phonebook load state response msg from AT Cmd Parser

    \brief      Description:\n
                handles phonebook load state response msgs from AT Cmd Parser \n

    \param      pPpds
                ATRspPbDwnlStatusType *

    \return     SuccessFailType

*/
/*----------------------------------------------------------------------------*/
static SuccessFailType _APP_PHONEBOOK_AT_sfHandlePPDS(const ATRspPbDwnlStatusType *pPpds)
{
    gAppPBContext.dwldCount = pPpds->dl_status;
    _APP_PHONEBOOK_vNotifyApps(APP_PB_DOWNLOAD_COUNT_CHANGED);

    return SUCCESS;
}/* END OF _APP_PHONEBOOK_vHandlePPDS() */

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_AT_sfHandlePPBU                               */
/**
    \brief      handle phonebook load state response msg from AT Cmd Parser

    \brief      Description:\n
                handles phonebook load state response msgs from AT Cmd Parser \n

    \param      pPpbu
                ATRspPpbuType *

    \return     SuccessFailType

*/
/*----------------------------------------------------------------------------*/
static SuccessFailType _APP_PHONEBOOK_AT_sfHandlePPBU(const ATRspPpbuType *pPpbu)
{
    SuccessFailType result = SUCCESS;

    switch( pPpbu->type )
    {
        case AT_PB_LOADING:
//        		pbDwldEntry = 0;
            // new syncro has been started, reset indicator for updated pbs
            gAppPBContext.updatedPbIndicator = APP_PB_NONE_REFRESHED;
            // save pb download state
            gAppPBContext.dwldState = AT_PB_LOADING;
            _APP_PHONEBOOK_vNotifyApps(APP_PB_DOWNLOAD_STATE_CHANGED);

            if(BT_FALSE == gAppPBContext.bFirstPPBU0ReceivedAlready )
            {   // this is the first PPBU:0 received after connect. As mentioned by Parrot,
                // we can request PbSizes now. Trigger this.
                // Following PbSize requests will be triggered when PPBU:1 or PPBU:2 are
                // received.
                // UPDATE PB SIZES only, if this is an already known Phone which PB has been synced
                // before
                const ERMUserProfileType * profile = ERM_pGetActiveUserProfile();
                if((NULL != profile) && (BT_TRUE == profile->bPbLoadedCompletly))
                {
                    (void)_APP_PHONEBOOK_sfUpdatePbSizes();
                }
                else
                {
                    DEBUG_TEXT(APP_PB_SWI_TRC, MOD_APP_PB, "HdlPPBU: ign. ");
                }
                gAppPBContext.bFirstPPBU0ReceivedAlready = BT_TRUE;
            }
            break;

        case AT_PB_LOAD_FINISHED:

            gAppPBContext.dwldState = AT_PB_LOAD_FINISHED;

            if(gAppPBContext.notifyPBSortingAfterNewUserFailed == BT_TRUE)
            {
                /* inform the application to retry to change the PB sorting order if not successful after the first time
                 * one more retry will be triggered and the HMI will still not be informed about the new PB */
                /* implemented by nikolov, RT:11046 */

                DEBUG_TEXT(APP_PB_SWI_TRC, MOD_APP_PB, "sfHandlePPBU: retry PPNO after PB sync");

                _APP_PHONEBOOK_vNotifyApps(APP_PB_DOWNLOAD_STATE_CHANGED_RETRY_PB_SORTING_FOR_NEWUSER);

                /* set  the flag back when the PB sync is done */
                gAppPBContext.notifyPBSortingAfterNewUserFailed = BT_FALSE;
            }
            else
            {
                (void)_APP_PHONEBOOK_sfUpdatePbSizes();
                _APP_PHONEBOOK_vNotifyApps(APP_PB_DOWNLOAD_STATE_CHANGED);
            }
            break;

        case AT_PB_LOAD_FINISHED_BUT:

            gAppPBContext.dwldState = AT_PB_LOAD_FINISHED_BUT;

            if(gAppPBContext.notifyPBSortingAfterNewUserFailed == BT_TRUE)
            {
                /* inform the application to retry to change the PB sorting order if not successful after the first time
                 * one more retry will be triggered and the HMI will still not be informed about the new PB*/
                 /* implemented by nikolov, RT:11046 */
                DEBUG_TEXT(APP_PB_SWI_TRC, MOD_APP_PB, "sfHandlePPBU: retry PPNO after PB sync failed");

                _APP_PHONEBOOK_vNotifyApps(APP_PB_DOWNLOAD_STATE_CHANGED_RETRY_PB_SORTING_FOR_NEWUSER);

                /* set  the flag back when the PB sync is done(even if incompletely synced PPBU:2) */
                gAppPBContext.notifyPBSortingAfterNewUserFailed = BT_FALSE;
            }
            else
            {
                (void)_APP_PHONEBOOK_sfUpdatePbSizes();
                _APP_PHONEBOOK_vNotifyApps(APP_PB_DOWNLOAD_STATE_CHANGED);
            }
            break;

        default:
            DEBUG_VALUE1(APP_PB_SWI_ERR, MOD_APP_PB, "HandlePPBU: invalid ppbu type %d",
                               pPpbu->type );
            result = FAIL;
            break;
    }

    return result;
}/* END OF _APP_PHONEBOOK_vHandlePPBU() */

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_AT_sfHandlePFPA                               */
/**
    \brief      handle phonebook available notification from AT Cmd Parser

    \brief      Description:\n
                handle phonebook available notification from AT Cmd Parser \n

    \return     SuccessFailType
*/
/*----------------------------------------------------------------------------*/
static SuccessFailType _APP_PHONEBOOK_AT_sfHandlePFPA()
{
    gAppPBContext.updatedPbIndicator = APP_PB_NONE_REFRESHED;

    // SUSANNE 15.11.2009
    // PFPA is received when we conenct to a phone an make a call take over.
    // in this case we do not get a PPBU:0 event until the call is terminated
    // BUT: Parrot added an event which indicates that PB is available even
    // though sync has not be started yet. This event is PFPA

    // we trigger the first pb size update after receiving Pfpa analog as we would have
    // received PPBU:0. But, when we connect and no call is active, both events are sent by the
    // Ck5050.
    // so after connect when we received PFPA first, we do not need to retrigger
    // update of pb sizes when PPBU:0 is received afterwards
    if(BT_FALSE == gAppPBContext.bFirstPPBU0ReceivedAlready )
    {   // this is the first PPBU:0 received after connect. As mentioned by Parrot,
        // we can request PbSizes now. Trigger this.
        // Following PbSize requests will be triggered when PPBU:1 or PPBU:2 are
        // received.
        // UPDATE PB SIZES only, if this is an already known Phone which PB has been synced
        // before
        const ERMUserProfileType * profile = ERM_pGetActiveUserProfile();
        if((NULL != profile) && (BT_TRUE == profile->bPbLoadedCompletly))
        {
            DEBUG_TEXT(APP_PB_SWI_TRC, MOD_APP_PB, "HdlPfpa: trig. UpdPbSize");
            (void)_APP_PHONEBOOK_sfUpdatePbSizes();
        }
        else
        {
            DEBUG_TEXT(APP_PB_SWI_TRC, MOD_APP_PB, "HdlPfpa: ign. ");
        }
        gAppPBContext.bFirstPPBU0ReceivedAlready = BT_TRUE;
    }
    else
    {
        DEBUG_VALUE1(APP_PB_SWI_TRC, MOD_APP_PB, "HdlPfpa: ign., %d", gAppPBContext.bFirstPPBU0ReceivedAlready );
    }

    return SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_AT_sfHandleNumVoiceTags                      */
/**
    \brief      handle RVTA responses from AT Parser

    \brief      Description:\n
                handles RVTA responses from AT Parser \n

    \param      pRvta
                ATResVTMgtType

    \return     SuccessFailType
*/
/*----------------------------------------------------------------------------*/
static SuccessFailType _APP_PHONEBOOK_AT_sfHandleNumVoiceTags(const ATResVTMgtType *pRvta)
{
    SuccessFailType result = SUCCESS;

    // check if we are in fct requesting number of voice tags
    if( APP_PB_enRequestNumVoiceTagsForContact == gAppPBContext.fct.common.fctId )
    {
        DEBUG_VALUE1(APP_PB_SWI_TRC, MOD_APP_PB, "HandleRVTA: save %d",
                            pRvta->nb_voice_tag );

        // save number of voice tags
        gAppPBContext.fct.numVoiceTags.num = pRvta->nb_voice_tag;

        // forward state for fctId and set alarm so function can be finished
        gAppPBContext.fct.numVoiceTags.state = APP_PHONEBOOK_PB_REQ_NUM_TAGS_END;
        (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_PHONEBOOK_FCT_PENDING, MSEC(0),0);
    }
    // else just ignore
    else
    {
        DEBUG_VALUE1(APP_PB_SWI_TRC, MOD_APP_PB, "HandleRVTA: ignore %d",
                            pRvta->nb_voice_tag );
    }
    return result;
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_AT_sfHandlePPBR                      */
/**
    \brief      handle PPBR responses from AT Cmd Parser

    \brief      Description:\n
                handles PPBR responses from AT Cmd Parser \n

    \param      pPpbr
                ATRspPBReadEntryType

    \return     SuccessFailType
*/
/*----------------------------------------------------------------------------*/
static SuccessFailType _APP_PHONEBOOK_AT_sfHandlePPBR(ATRspPBReadEntryType *pPpbr)
{
    SuccessFailType result = SUCCESS;

    DEBUG_VALUE1(APP_PB_SWI_TRC, MOD_APP_PB, "HaPPBR: %d",
                    pPpbr->is_size_only);

    if( AT_TRUE == pPpbr->is_size_only )
    {
        result = _APP_PHONEBOOK_AT_sfHandlePbSizeResponse(pPpbr);
    }
    else
    {
        result = _APP_PHONEBOOK_AT_sfHandlePbEntriesResponse(pPpbr);
    }

    return result;
}/* END OF _APP_PHONEBOOK_AT_PPBR_sfHandlePPBR() */

/*--------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_AT_sfHandlePPBP                             */
/**
    \brief      handle PPBP responses from AT Cmd Parser

    \brief      Description:\n
                handles PPBP responses from AT Cmd Parser \n

    \param      pPpbp
                ATRspPBReadNumberType *

    \return     SuccessFailType
*/
/*----------------------------------------------------------------------------*/
static SuccessFailType _APP_PHONEBOOK_AT_sfHandlePPBP(const ATRspPBReadNumberType *pPpbp)
{
    // save the numbers for a phonebook entry
    // HU counts starting with 0
    // Parrots starts counting with 1

    if(gAppPBContext.fct.common.fctId == APP_PB_enReadCompletePbEntries)
    {
        Uint16Type ppbrIdx = pPpbp->pb_idx - gAppPBContext.fct.pbRead.start;
        Uint16Type ppbpIdx = pPpbp->pn_idx - 1; /* array starts from 0!! */

        // copy phone number entry
        (void)memcpy( &gAppPBContext.fct.pbRead.entry[ppbrIdx].number[ppbpIdx].ppbp,
                pPpbp,
                sizeof(ATRspPBReadNumberType));

        gAppPBContext.fct.pbRead.entry[ppbrIdx].number[ppbpIdx].isValid = BT_TRUE;

        DEBUG_VALUE2(APP_PB_SWI_DBG, MOD_APP_PB, "HaPPBP: pn_idx %d pb_idx %d",
                        pPpbp->pn_idx, pPpbp->pb_idx);
    }
    else
    {
        DEBUG_VALUE1(APP_PB_SWI_ERR, MOD_APP_PB, "HaPPBP: msg ignored - current fctId %d",
                       gAppPBContext.fct.common.fctId );

    }

    return SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_AT_PPBR_sfHandlePbSizeResponse                      */
/**
    \brief      handle PPBR size responses from AT Cmd Parser

    \brief      Description:\n
                handles PPBR size responses from AT Cmd Parser \n

    \param      pPpbr
                ATRspPBReadEntryType

    \return     SuccessFailType
*/
/*----------------------------------------------------------------------------*/
static SuccessFailType _APP_PHONEBOOK_AT_sfHandlePbSizeResponse(const ATRspPBReadEntryType *pPpbr)
{
    const ERMUserProfileType * user = ERM_pGetActiveUserProfile();

    // save the size of pb
    switch( gAppPBContext.curSelectedPb )
    {
        case APP_PHONEBOOK_enUsersPB:
        {
            // copy new size in any case
            gAppPBContext.usersPersonalPb.pbSize    = pPpbr->pb_size;
            gAppPBContext.dwldCount                 = pPpbr->pb_size;          //RT #908
            _APP_PHONEBOOK_vNotifyApps(APP_PB_USER_PB_UPDATE_FINISHED);          //RT #908
            switch(gAppPBContext.dwldState)
            {
                case AT_PB_LOADING:
                    // receiving a pb Size in this dwld state should happen only right after connect
                    // to be able to access phonebook immediately before sync has been finished
                    if((NULL != user) && (BT_TRUE == user->bPbLoadedCompletly) && (0 < pPpbr->pb_size))
                    {   // we have a user profile, and the pb has been loaded completely before and the
                        // pb size is not 0
                        // PB is valid and we can activate it
                        gAppPBContext.usersPersonalPb.bIsValid = BT_TRUE;
                        if( gAppPBContext.updatedPbIndicator & APP_PB_USERS_PERSONAL_REFRESHED )
                        {
                            DEBUG_VALUE2(APP_PB_SWI_ERR, MOD_APP_PB, "HdlPbSize: usr:%d state %d, ch. load.", pPpbr->pb_size, gAppPBContext.dwldState);
                            _APP_PHONEBOOK_vNotifyApps(APP_PB_USERS_PB_CHANGED);
                        }
                        else
                        {
                            DEBUG_VALUE2(APP_PB_SWI_ERR, MOD_APP_PB, "HdlPbSize: usr:%d state %d, nch. load.", pPpbr->pb_size, gAppPBContext.dwldState);
                            _APP_PHONEBOOK_vNotifyApps(APP_PB_USERS_PB_NOT_CHANGED);
                        }
                    }
                    else
                    {
                        DEBUG_VALUE2(APP_PB_SWI_WNG, MOD_APP_PB, "HdlPbSize: ign. dwldState %d usr:%d", gAppPBContext.dwldState, pPpbr->pb_size);
                    }
                    break;

                case AT_PB_LOAD_FINISHED:
                    // successful sync
                    // pb is valid in any case, check if it has been changed
                    gAppPBContext.usersPersonalPb.bIsValid = BT_TRUE;
                    // new pb is indicated when pb has been changed or when it was updated due to a manual sync
                    // or it was the first time we loaded it from mobile   /*RT13307: Phone book size check for non zero value added*/
                    if(((gAppPBContext.updatedPbIndicator & APP_PB_USERS_PERSONAL_REFRESHED)&&(0 < pPpbr->pb_size))         ||
                       (BT_TRUE == gAppPBContext.bWasManualSync)                                    ||
                       ((NULL != user) && (BT_FALSE == user->bPbLoadedCompletly) && (0 < pPpbr->pb_size)) )
                    {
                        DEBUG_VALUE2(APP_PB_SWI_ERR, MOD_APP_PB, "HdlPbSize: usr:%d state %d, ch. compl.", pPpbr->pb_size, gAppPBContext.dwldState);
                        _APP_PHONEBOOK_vNotifyApps(APP_PB_USERS_PB_CHANGED);
                    }
                    else if (0 < pPpbr->pb_size)
                    {

                        DEBUG_VALUE2(APP_PB_SWI_ERR, MOD_APP_PB, "HdlPbSize: usr:%d state %d, nch. compl.", pPpbr->pb_size, gAppPBContext.dwldState);
                        _APP_PHONEBOOK_vNotifyApps(APP_PB_USERS_PB_NOT_CHANGED);
                    }
                    else
                    {
                        DEBUG_VALUE2(APP_PB_SWI_ERR, MOD_APP_PB, "HdlPbSize: usr:%d state %d, not avail.", pPpbr->pb_size, gAppPBContext.dwldState);
                        gAppPBContext.usersPersonalPb.bIsValid = BT_FALSE;
                        gAppPBContext.usersPersonalPb.pbSize    = 0;
                        _APP_PHONEBOOK_vNotifyApps(APP_PB_USERS_PB_NOT_AVAILABLE);
                    }
                    break;

                case AT_PB_LOAD_FINISHED_BUT:
                    // there was an error during sync process
                    // if pbsize is now empty, the phonebook is not available
                    // if the size is not empty, we need to assume that it is incomplete
                    // check if pb has been updated
                    if((gAppPBContext.updatedPbIndicator & APP_PB_USERS_PERSONAL_REFRESHED)         ||
                       (BT_TRUE == gAppPBContext.bWasManualSync)                                    ||
                       ((NULL != user) && (BT_FALSE == user->bPbLoadedCompletly) && (0 < pPpbr->pb_size)) )
                    {
                        DEBUG_VALUE2(APP_PB_SWI_ERR, MOD_APP_PB, "HdlPbSize: usr:%d state %d, ch. incompl.", pPpbr->pb_size, gAppPBContext.dwldState);
                        gAppPBContext.usersPersonalPb.bIsValid = BT_TRUE;
                        _APP_PHONEBOOK_vNotifyApps(APP_PB_USERS_PB_CHANGED_INCOMPLETE);
                    }
                    else if (0 < pPpbr->pb_size)
                    {
                        DEBUG_VALUE2(APP_PB_SWI_ERR, MOD_APP_PB, "HdlPbSize: usr:%d state %d, nch. incompl.", pPpbr->pb_size, gAppPBContext.dwldState);
                        gAppPBContext.usersPersonalPb.bIsValid = BT_TRUE;
                        _APP_PHONEBOOK_vNotifyApps(APP_PB_USERS_PB_NOT_CHANGED_INCOMPLETE);
                    }
                    else
                    {
                        DEBUG_VALUE2(APP_PB_SWI_ERR, MOD_APP_PB, "HdlPbSize: usr:%d state %d, not avail.", pPpbr->pb_size, gAppPBContext.dwldState);
                        gAppPBContext.usersPersonalPb.pbSize    = 0;
                        gAppPBContext.usersPersonalPb.bIsValid = BT_FALSE;
                        _APP_PHONEBOOK_vNotifyApps(APP_PB_USERS_PB_NOT_AVAILABLE);
                    }
                    break;
                default:
                    DEBUG_VALUE2(APP_PB_SWI_ERR, MOD_APP_PB, "HdlPbSize: inv. state %d usr:%d ", gAppPBContext.dwldState, pPpbr->pb_size);
                    break;
            }
            break;
        }

        case APP_PHONEBOOK_enLastDialed:
        {
            gAppPBContext.dialledNumbersPb.pbSize = pPpbr->pb_size;
            gAppPBContext.dialledNumbersPb.bIsValid = BT_TRUE;
            if((gAppPBContext.updatedPbIndicator & APP_PB_DIALLED_NUMBERS_REFRESHED))
            {
                DEBUG_VALUE1(APP_PB_SWI_ERR, MOD_APP_PB, "HdlPbSize: dialled:%d chgd", pPpbr->pb_size);
                _APP_PHONEBOOK_vNotifyApps(APP_PB_DIALED_PB_CHANGED);
            }
            else
            {
                DEBUG_VALUE1(APP_PB_SWI_ERR, MOD_APP_PB, "HdlPbSize: dialled:%d not chgd", pPpbr->pb_size);
                _APP_PHONEBOOK_vNotifyApps(APP_PB_DIALED_PB_NOT_CHANGED);
            }
            break;
        }
        case APP_PHONEBOOK_enReceivedCalls:
        {
            gAppPBContext.receivedCallsPb.pbSize    = pPpbr->pb_size;
            gAppPBContext.receivedCallsPb.bIsValid  = BT_TRUE;
            if((gAppPBContext.updatedPbIndicator & APP_PB_RECEIVED_CALLS_REFRESHED))
            {
                DEBUG_VALUE1(APP_PB_SWI_ERR, MOD_APP_PB, "HdlPbSize: rcvd:%d chgd", pPpbr->pb_size);
                _APP_PHONEBOOK_vNotifyApps(APP_PB_RECEIVED_PB_CHANGED);
            }
            else
            {
                DEBUG_VALUE1(APP_PB_SWI_ERR, MOD_APP_PB, "HdlPbSize: rcvd:%d not chgd", pPpbr->pb_size);
                _APP_PHONEBOOK_vNotifyApps(APP_PB_RECEIVED_PB_NOT_CHANGED);
            }
            break;
        }
        case APP_PHONEBOOK_enMissedCalls:
        {
            gAppPBContext.missedCallsPb.pbSize = pPpbr->pb_size;
            gAppPBContext.missedCallsPb.bIsValid = BT_TRUE;
            if((gAppPBContext.updatedPbIndicator & APP_PB_MISSED_CALLS_REFRESHED))
            {
                DEBUG_VALUE1(APP_PB_SWI_ERR, MOD_APP_PB, "HdlPbSize: missd:%d chgd", pPpbr->pb_size);
                _APP_PHONEBOOK_vNotifyApps(APP_PB_MISSED_PB_CHANGED);
            }
            else
            {

                DEBUG_VALUE1(APP_PB_SWI_ERR, MOD_APP_PB, "HdlPbSize: missd:%d not chgd", pPpbr->pb_size);
                _APP_PHONEBOOK_vNotifyApps(APP_PB_MISSED_PB_NOT_CHANGED);
            }
            break;
        }
        default:
            DEBUG_VALUE1(APP_PB_SWI_ERR, MOD_APP_PB, "HaPbSizeRsp: invalid curSelectedPb %d",
                           gAppPBContext.curSelectedPb );
            return FAIL;
    }

   return SUCCESS;
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_AT_sfHandlePbEntriesResponse                      */
/**
    \brief      handle PPBR entries responses from AT Cmd Parser

    \brief      Description:\n
                handles PPBR entries responses from AT Cmd Parser \n

    \param      pPpbr
                ATRspPBReadEntryType *

    \return     SuccessFailType
*/
/*----------------------------------------------------------------------------*/
static SuccessFailType _APP_PHONEBOOK_AT_sfHandlePbEntriesResponse(const ATRspPBReadEntryType *pPpbr)
{
    // TODO:
    // save the entry
    // HU counts starting with 0
    // Parrots starts counting with 1

    Uint8Type     arrayIndex = pPpbr->pb_idx - gAppPBContext.fct.pbRead.start;

    if(gAppPBContext.fct.common.fctId == APP_PB_enReadCompletePbEntries &&
       arrayIndex < MAX_REQUESTED_PB_ENTRIES )  /* make sure we are not out of bound */
    {
        AppPbEntryType *pPbEntry = &gAppPBContext.fct.pbRead.entry[arrayIndex];

        // first copy the current content into our own buffer, caus pPpbr is const and we cannot change data
        (void)memcpy( &pPbEntry->ppbr, pPpbr, sizeof(ATRspPBReadEntryType));

        // To avoid empty entries in the call stacks, we need to replace an unknown number through a text 'unknown'
        if( BT_TRUE == APP_PHONE_bIsUnknownCallerId(pPbEntry->ppbr.name, pPbEntry->ppbr.name_len))
        {
            // we have an unknown caller id and need to replace the name field with a text 'unknown'
            // get the text first
            Uint16Type  textLen     = 0;
            const Uint8Type *pText  = NULL;
            SuccessFailType result  = APP_SD_sfGetTextForNumberType(APP_SD_NUMBER_TYPE_UNKNOWN_CALLER_ID, &pText, &textLen);

            if( (SUCCESS == result) && (pText != NULL) )
            {
                // check textLen and buffer size boundaries
                textLen = ( textLen > sizeof(pPbEntry->ppbr.name)) ? sizeof(pPbEntry->ppbr.name) : textLen;
                // memset the name to zeroes
                memset(&(pPbEntry->ppbr.name[0]), 0x00, sizeof(pPbEntry->ppbr.name));
                // copy the text into the buffer
                memcpy(&(pPbEntry->ppbr.name[0]), pText, textLen);

                pPbEntry->ppbr.name_len = textLen;

                DEBUG_TEXT(APP_PB_SWI_TRC, MOD_APP_PB, "Repl. unkn. num");
            }
        }

        gAppPBContext.fct.pbRead.entry[arrayIndex].isValid = BT_TRUE;


        /* we support currently max 5 numbers */
        gAppPBContext.fct.pbRead.entry[arrayIndex].ppbr.pn_size =
            ( pPpbr->pn_size > MAX_REQUESTED_PB_NUMBERS_PER_ENTRY ) ?
                    MAX_REQUESTED_PB_NUMBERS_PER_ENTRY : pPpbr->pn_size;

        DEBUG_VALUE1(APP_PB_SWI_TRC, MOD_APP_PB, "HaPbEntrRsp: %d",
                        pPpbr->pb_idx);
    }
    else
    {
        DEBUG_VALUE2(APP_PB_SWI_ERR, MOD_APP_PB, "HaPbEntriesRsp: msg ignored - fctId %d, arrIdx %d",
                       gAppPBContext.fct.common.fctId, arrayIndex );

    }

    return SUCCESS;
}
/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_AT_sfHandlePbSearchResponse                      */
/**
    \brief      handle pb search responses from AT Cmd Parser

    \brief      Description:\n
                handles pb search responses from AT Cmd Parser \n

    \param      pPpas
                ATResPBAlphaSearchType *

    \return     SuccessFailType
*/
/*----------------------------------------------------------------------------*/
SuccessFailType
_APP_PHONEBOOK_AT_sfHandlePbSearchResponse(const ATResPBAlphaSearchType *pPpas)
{
//    if(gAppPBContext.fct.common.fctId == APP_PB_enSearchPb)
//    {
//        // copy index depending on state
//        switch(gAppPBContext.fct.pbSearch.state)
//        {
//            case APP_PHONEBOOK_PB_SEARCH_START_SENT:
//                gAppPBContext.fct.pbSearch.firstIndex = pPpas->pb_index;
//                break;
//            case APP_PHONEBOOK_PB_SEARCH_END_SENT:
//                gAppPBContext.fct.pbSearch.endIndex = pPpas->pb_index;
//                break;
//            default:
//                
//                        break;
//        }
//    }
//    else
//    {
//        
//    }

    return SUCCESS;
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_AT_sfHandlePbSearchExtResponse                      */
/**
    \brief      handle extended pb search responses from AT Cmd Parser

    \brief      Description:\n
                handles extended pb search responses from AT Cmd Parser \n

    \param      pPpas
                ATResPBAlphaSearchExtType *

    \return     SuccessFailType
*/
/*----------------------------------------------------------------------------*/
static SuccessFailType
_APP_PHONEBOOK_AT_sfHandlePbSearchExtResponse(const ATResPBAlphaSearchExtType *pPpas)
{
    if(gAppPBContext.fct.common.fctId == APP_PB_enSearchPb)
    {
        Uint16Type pbSize = APP_PHONEBOOK_u16GetCurrentPBSize(APP_PHONEBOOK_enUsersPB);
        // copy pbIndex and number of matching Entries
        gAppPBContext.fct.pbSearch.firstIndex = pPpas->pb_index;
        gAppPBContext.fct.pbSearch.numMatchingEntries = pPpas->numMatchingContacts;

        // to avoid an underflow we need to make sure that the firstIndex is not greater than the pbSize
        if(gAppPBContext.fct.pbSearch.firstIndex > pbSize)
        {
            // set firstIndex to pbSize and numEntries to zero, cause there are no more entries
            // within our range
            gAppPBContext.fct.pbSearch.firstIndex = pbSize;
            gAppPBContext.fct.pbSearch.numMatchingEntries = 0;
        }

        // for the case that CK5050 has a larger phonebook than we support, we need to check
        // if firstIndex + numMatchingEntries is greater than pbSize. If yes, we need to reduce
        // numMatchingEntries
        if(gAppPBContext.fct.pbSearch.firstIndex + gAppPBContext.fct.pbSearch.numMatchingEntries
                    > pbSize)
        {
            // +1 to get the last entry, too
            gAppPBContext.fct.pbSearch.numMatchingEntries = pbSize - gAppPBContext.fct.pbSearch.firstIndex + 1;
        }
    }
    else
    {
        DEBUG_VALUE1(APP_PB_SWI_ERR, MOD_APP_PB, "HaPbSearch: msg ignored - fctId %d",
                       gAppPBContext.fct.common.fctId );
    }

    return SUCCESS;
}




static SuccessFailType
_APP_PHONEBOOK_AT_sfHandlePbSortOrderResponse(const ATResPBSortModeType * pPpno)
{
    if(gAppPBContext.fct.common.fctId == APP_PB_enGetPbSortOrder ||
       gAppPBContext.fct.common.fctId == APP_PB_enSetPbSortOrder )
    {
        switch( pPpno->mode )
        {
            case AT_PB_SORTORDER_FIRSTLAST:
            case AT_PB_SORTORDER_LASTFIRST:
                gAppPBContext.fct.pbSortOrder.mode = pPpno->mode;
                break;

            default:
                DEBUG_VALUE1(APP_PB_SWI_ERR, MOD_APP_PB, "HaPbSortOrderRsp: unknown mode %d",
                        pPpno->mode );
                gAppPBContext.fct.pbSortOrder.mode = AT_PB_SORTORDER_LASTFIRST;
                break;
        }
    }
    else
    {
        DEBUG_VALUE1(APP_PB_SWI_ERR, MOD_APP_PB, "HaPbSortOrderRsp: msg ignored - fctId %d",
                       gAppPBContext.fct.common.fctId );
    }
    return SUCCESS;
}


static SuccessFailType
_APP_PHONEBOOK_AT_sfHandleAddPbEntryResponse(const ATResPBAddEntryType * pPpad)
{
    /* we need to inform VCR so far - workaround as message sequence is
     * not correct */
    VCR_vNotifyPBEntryAdded(pPpad->pb_index);
    if(gAppPBContext.fct.common.fctId == APP_PB_enAddPbEntry)
    {
        gAppPBContext.fct.pbAddEntry.newIndex = pPpad->pb_index;
        DEBUG_VALUE1(APP_PB_SWI_ERR, MOD_APP_PB, "HaAddPbEntryRsp: contact added at idx %d",
                       gAppPBContext.fct.pbAddEntry.newIndex );

        // trigger pb syncro to get new sizes
        gAppPBContext.pending.pbSizeUpdate = BT_TRUE;

        // FIX FOR VCR UNTIL 2.00 beta11 of Parrots HostSw becomes available
        // FOR now, PPAD:index event is received after OK
       // _APP_PHONEBOOK_vNotifyApps(APP_PB_ENTRY_ADDED);
    }
    else
    {
        DEBUG_VALUE1(APP_PB_SWI_ERR, MOD_APP_PB, "HaAddPbEntryRsp: msg ignored - fctId %d",
                       gAppPBContext.fct.common.fctId );
    }

    return SUCCESS;


}



/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_AT_sfHandlePPBC                               */
/**
    \brief      handle response msg from AT Cmd Parser
                update indication for phonebook type \n

    \brief      Description:\n
                handle response msg from AT Cmd Parser
                update indication for phonebook type \n

    \param      pPpbc
                ATRspPpbcType *

    \return     void

*/
/*----------------------------------------------------------------------------*/
static SuccessFailType _APP_PHONEBOOK_AT_sfHandlePPBC(const ATRspPpbcType *pPpbc)
{
    // *** BEGIN Susanne 07.08.09 Ck5050 also just sends single PPBC for the callstacks
    // if we are not in a sync started with PPBU:0, we need to update the pb size of the
    // stack indicated as being changed!!

    // Ticket #4347
    // Sometimes we did not get a PPBU:1 nor PPBU:2 event even if we are in sync
    // Now when we get a PPBC event for one of the call stacks due to a call
    // we would never update the size of this stack  and though HU will never
    // get a trigger that stack has changed.
    // FIX: Update Pb size for stacks always, no matter if we are in pb sync or not

    switch(pPpbc->type)
    {
        case AT_PBTYPE_MISSED:
            gAppPBContext.updatedPbIndicator |= APP_PB_MISSED_CALLS_REFRESHED;
            _APP_PHONEBOOK_sfUpdateSinglePbSize(APP_PHONEBOOK_enMissedCalls);
            break;
        case AT_PBTYPE_LAST_DIALLED:
            gAppPBContext.updatedPbIndicator |= APP_PB_DIALLED_NUMBERS_REFRESHED;
            _APP_PHONEBOOK_sfUpdateSinglePbSize(APP_PHONEBOOK_enLastDialed);
            break;
        case AT_PBTYPE_LAST_RECEIVED:
            gAppPBContext.updatedPbIndicator |= APP_PB_RECEIVED_CALLS_REFRESHED;
            _APP_PHONEBOOK_sfUpdateSinglePbSize(APP_PHONEBOOK_enReceivedCalls);
            break;
        // SUSANNE: 28.10.2009: After change of sorting Ck5050 sends a single PPBC:0
        // so we need to update pbSize
        case AT_PBTYPE_USER_PERSONAL:
// RT: 13178
//            if((gAppPBContext.SortingOrderChangedSentByUser == BT_FALSE)&&
//                    (AT_PB_LOADING != APP_PHONEBOOK_enGetPBDwldState()))
//            {
//                /* if the PB sorting order was changed after user adding, don`t inform the HMI
//                 about new PB available, first change the sorting order, will be triggered over PPBU */
//                /* implemented by nikolov, RT:11046 */
//                gAppPBContext.updatedPbIndicator |= APP_PB_USERS_PERSONAL_REFRESHED;
//
//                // cancel alarm for pb sort
//                (void)APP_COMMON_UTILS_stCancelOsekRelAlarm( CTA_APP_PHONEBOOK_SORTING_TIMEOUT );
//
//            }
//            else
                if((gAppPBContext.SortingOrderChangedSentByUser == BT_TRUE)&&
                    (AT_PB_LOADING != APP_PHONEBOOK_enGetPBDwldState()))
            {
                /* implemented by nikolov, RT:11046 */
                gAppPBContext.updatedPbIndicator |= APP_PB_USERS_PERSONAL_REFRESHED;
                gAppPBContext.SortingOrderChangedSentByUser = BT_FALSE;
                // cancel alarm for PB sort because notification for new PB available is received
                (void)APP_COMMON_UTILS_stCancelOsekRelAlarm( CTA_APP_PHONEBOOK_SORTING_TIMEOUT );
                // finally update the size
                (void)_APP_PHONEBOOK_sfUpdatePbSizes();
            }
            else
            {
                // do nothing, wait for PPBU:1 or PPBU:2
                // SUSANNE: 05.02.2010: but in any case save that the pb has been changed, so BAP can sent changed array later!!!
                gAppPBContext.updatedPbIndicator |= APP_PB_USERS_PERSONAL_REFRESHED;
                // RT13178: cancel sorting order time out with received PPBC:0 regardless the download state */
                DEBUG_TEXT(APP_PB_SWI_TRC,MOD_APP_PB, "PPBC:0 received -> cancel sorting order timeout!");
                (void)APP_COMMON_UTILS_stCancelOsekRelAlarm( CTA_APP_PHONEBOOK_SORTING_TIMEOUT );
            }
            break;
    }

    if(AT_PB_LOADING != gAppPBContext.dwldState)
    {
        DEBUG_VALUE1(APP_PB_SWI_TRC, MOD_APP_PB, "PPBC: Recvd Single PPBC:%d",pPpbc->type);
    }
    else
    {
        DEBUG_VALUE1(APP_PB_SWI_TRC, MOD_APP_PB, "PPBC: Recvd Sync PPBC:%d",pPpbc->type);
    }

    return SUCCESS;
}/* END OF _APP_PHONEBOOK_sfHandlePPBC() */









/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_AT_sfSendSelectPbATRequest                    */
/**
    \brief      Send AT Request to select the given Phonebook \n

    \brief      Description:\n
                Send AT Request to select the given Phonebook \n

    \param      pbToSelect
                APP_PHONEBOOK_enSelectedPbType

    \param      puart
                const SCI_ST*

    \return     SuccessFailType
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/

/* select given pb */
SuccessFailType
_APP_PHONEBOOK_AT_sfSendSelectPbATRequest(APP_PHONEBOOK_enSelectedPbType pbToSelect,
                                       const SCI_ST * puart )
{
    ATReqAllType request;
    request.common.req_type = AT_REQ_PB_SELECT;

    if( !puart )
    {
        return FAIL;
    }
    switch( pbToSelect )
    {
        case APP_PHONEBOOK_enUsersPB:
            request.ppbs.type = AT_PBTYPE_USER_PERSONAL;
            request.ppbs.mode = AT_PBMODE_ALL;
            break;
        case APP_PHONEBOOK_enLastDialed:
            request.ppbs.type = AT_PBTYPE_LAST_DIALLED;
            break;
        case APP_PHONEBOOK_enReceivedCalls:
            request.ppbs.type = AT_PBTYPE_LAST_RECEIVED;
            break;
        case APP_PHONEBOOK_enMissedCalls:
            request.ppbs.type = AT_PBTYPE_MISSED;
            break;
    }

    return AT_Cmd_Handler_Phonebook(&request, puart);
}


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
_APP_PHONEBOOK_AT_sfSendActivatePpds(BooleanType enable, const SCI_ST * puart )
{
    ATReqAllType request;
    request.common.req_type = AT_REQ_PB_EN_DWNLOAD_STATUS;
    request.peds.enable     = (ATBooleanEnum)enable;

    return AT_Cmd_Handler_Phonebook(&request, puart);
}


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
                                          const SCI_ST * puart )
{
    ATReqAllType request;
    request.common.req_type = AT_REQ_PB_ALPHA_SEARCH;

    if( !puart )
    {
        return FAIL;
    }

    if( searchStrLen > AT_CALLER_ID_STRING_MAX_LEN )
    {
        searchStrLen = AT_CALLER_ID_STRING_MAX_LEN;
    }

    (void)memcpy(request.ppas.initial, searchStr, searchStrLen);
    request.ppas.initial_len = searchStrLen;

    return AT_Cmd_Handler_Phonebook(&request, puart);
}


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
                                          const SCI_ST * puart )
{
    ATReqAllType request;
    request.common.req_type = AT_REQ_PB_ALPHA_SEARCH_EXT;

    if( !puart )
    {
        return FAIL;
    }
    if( searchStrLen > AT_CALLER_ID_STRING_MAX_LEN )
    {
        searchStrLen = AT_CALLER_ID_STRING_MAX_LEN;
    }

    (void)memcpy(request.ppasex.initial, searchStr, searchStrLen);
    request.ppasex.initial_len = searchStrLen;

    return AT_Cmd_Handler_Phonebook(&request, puart);
}



/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_AT_sfSendPbSyncATRequest                    */
/**
    \brief      Send AT Request to manually sync Phonebook \n

    \brief      Description:\n
                Send AT Request to manually sync the Phonebook \n

    \param      puart
                const SCI_ST*

    \return     SuccessFailType
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType
_APP_PHONEBOOK_AT_sfSendPbSyncATRequest( const SCI_ST * puart )
{
    ATReqAllType request;
    request.common.req_type = AT_REQ_PB_START_MANUAL_SYNC;

    if( !puart )
    {
        return FAIL;
    }
    return AT_Cmd_Handler_Phonebook(&request, puart);
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_AT_sfSendGetPhonebookUpdateStatus             */
/**
    \brief      Send AT Request to get the Phonebook update status  \n

    \brief      Description:\n
    Send AT Request to get the Phonebook update status  \n

    \param      puart
                const SCI_ST*

    \return     SuccessFailType
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType
_APP_PHONEBOOK_AT_sfSendGetPhonebookUpdateStatus( const SCI_ST * puart )
{
    ATReqAllType request;
    request.common.req_type = AT_REQ_PB_UPDATE_STATUS;

    if( !puart )
    {
        return FAIL;
    }
    return AT_Cmd_Handler_Phonebook(&request, puart);
}

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
_APP_PHONEBOOK_AT_sfSendPbSortOrderATRequest( const SCI_ST * puart, Uint8Type mode )
{
    ATReqAllType request;
    request.common.req_type = AT_REQ_PB_NAME_ORDER_SELECT;
    request.ppno.mode       = mode;

    if( !puart )
    {
        return FAIL;
    }

    return AT_Cmd_Handler_Phonebook(&request, puart);
}

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
                                            ATPhoneNumberTypeEnum pnType)
{
    ATReqAllType request;
    request.common.req_type = AT_REQ_PB_ADD_ENTRY;

    // copy name
    request.ppad.name_len = (name_len > sizeof(AT_CallerId))?
                            sizeof(AT_CallerId) : name_len;
    (void)memcpy(request.ppad.name, name, request.ppad.name_len);

    // copy number
    request.ppad.phone_number_len = (number_len > sizeof(ATPhoneNumberType))?
                                    sizeof(ATPhoneNumberType) : number_len;
    (void)memcpy(request.ppad.phone_number, number, request.ppad.phone_number_len);

    // copy pn Type
    request.ppad.pn_type = pnType;

    if( !puart )
        return FAIL;

    return AT_Cmd_Handler_Phonebook(&request, puart);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_AT_sfSendNumVoiceTagsATRequest                    */
/**
    \brief      Send AT Request to request number of voice tags for a contact in Phonebook \n

    \brief      Description:\n
                Send AT Request to request number of voice tags for a contact in Phonebook  \n

    \param      puart
                const SCI_ST*

    \param      pbIndex
                Uint16Type

    \return     SuccessFailType
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType
_APP_PHONEBOOK_AT_sfSendNumVoiceTagsATRequest(const SCI_ST * puart,
                                              Uint16Type pbIndex)
{
    ATReqAllType request;
    request.common.req_type = AT_REQ_SIVR_MANAGE_VOICE_TAG;

    request.rvta.action = AT_VTMGR_ACT_RETRIEVE;
    // copy pbIndex
    request.rvta.identifier = pbIndex;
    // type is contact
    request.rvta.type       = AT_SIVR_ITEMTYPE_CONTACT;
    request.rvta.type_avail = AT_TRUE;

    if( !puart )
        return FAIL;

    return AT_Cmd_Handler_Phonebook(&request, puart);
    //return AT_Cmd_Handler_VCR(&request, puart);
}



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
_APP_PHONEBOOK_AT_sfSendPbCallStackResetATRequest( const SCI_ST * puart )
{
    ATReqAllType request;
    //request.common.req_type = AT_REQ_PB_...;

    if( !puart )
    {
        return FAIL;
    }
    return AT_Cmd_Handler_Phonebook(&request, puart);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_AT_sfSendPbEntriesATRequest              */
/**
    \brief      send AT Request to get phonebook entries from start \n
                to end \n

    \brief      Description:\n
                send AT Request to get phonebook entries from start \n
                to end \n

    \param      puart
                const SCI_ST*

    \param      start
                Uint16Type

    \param      end
                Uint16Type

    \return     SuccessFailType
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_PHONEBOOK_AT_sfSendPbEntriesATRequest(const SCI_ST*puart,
                                                                Uint16Type start,
                                                                Uint16Type end,
                                                                Uint16Type pbSize )
{
    ATReqAllType request;
    (void)memset( &request, 0, sizeof( request));

    if(!puart)
        return FAIL;

    request.common.req_type = AT_REQ_PB_READ_ENTRIES;
    // we want to read entries from the current selected phonebook
    request.ppbr.is_get_pb_size = AT_FALSE;
    request.ppbr.start_idx      = start;
    request.ppbr.end_idx        = end;
    request.ppbr.pbSize         = pbSize;
    return AT_Cmd_Handler_Phonebook( &request, puart);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_AT_sfSendPbNumbersATRequest              */
/**
    \brief      send AT Request to get phonebook numbers from start \n
                to end \n

    \brief      Description:\n
                send AT Request to get phonebook numbers from start \n
                to end \n
    \param      puart
                const SCI_ST*
    \param      pbIndex
                Uint16Type
    \param      start
                Uint16Type
    \param      end
                Uint16Type
    \return     SuccessFailType
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_PHONEBOOK_AT_sfSendPbNumbersATRequest( const SCI_ST*puart,
                                                            Uint16Type pbIndex,
                                                            Uint16Type start,
                                                            Uint16Type end )
{
    ATReqAllType request;
    (void)memset( &request, 0, sizeof( request));

    if(!puart)
        return FAIL;

    request.common.req_type = AT_REQ_PB_READ_NUMBERS;
    // we want to read phone numbers for pbIndex
    request.ppbp.pb_idx     = pbIndex;
    request.ppbp.pn_idx1    = start;
    request.ppbp.pn_idx2    = end;
    return AT_Cmd_Handler_Phonebook( &request, puart);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_AT_sfSendPbSizeATRequest              */
/**
    \brief      Send AT Request to get the size of the current selected \n
                phonebook \n

    \brief      Description:\n
                Send AT Request to get the size of the current selected \n
                phonebook \n

    \param      puart
                const SCI_ST*

    \return     SuccessFailType
*/
/*----------------------------------------------------------------------------*/
/* request size of current selected pb */
SuccessFailType _APP_PHONEBOOK_AT_sfSendPbSizeATRequest(const SCI_ST* puart)
{
    ATReqAllType request;
    (void)memset( &request, 0, sizeof( request));

    if(!puart)
        return FAIL;

    request.common.req_type = AT_REQ_PB_READ_ENTRIES;
    // we just want the size of the current selected phonebook
    request.ppbr.is_get_pb_size = AT_TRUE;
    return AT_Cmd_Handler_Phonebook( &request, puart);
}

//SuccessFailType _APP_PHONEBOOK_AT_sfHandlePPBRValidState()
//{
//    // TODO: notify APPS
//
///*
//    switch( gAppPBContext.fctCtx.common.fctId )
//    {
//        case BapFct_Phonebook_MissedCalls:
//        case BapFct_Phonebook_ReceivedCalls:
//        case BapFct_Phonebook_DialedNumbers:
//            _APP_PHONEBOOK_AT_PPBR_sfSendCallListStatusArray(gAppPBContext.fctCtx.common.fctId);
//            break;
//        case BapFct_Phonebook_Phonebook:
//            _APP_PHONEBOOK_AT_PPBR_sfSendPhonebookStatusArray();
//            break;
//        default:
//            return FAIL;
//    }
//*/
//    _APP_PHONEBOOK_AT_PPBR_vResetPbEntriesContext();
//    return SUCCESS;
//}



//void _APP_PHONEBOOK_AT_PPBR_vResetPbEntriesContext()
//{
//    int i = 0;
//    int j = 0;
//    gAppPBContext.fctCtx.pb.entries.requestState    = APP_PHONEBOOK_enInvalid;
//    gAppPBContext.fctCtx.pb.entries.start           = 0;
//    gAppPBContext.fctCtx.pb.entries.end             = 0;
//
//    for( i=0; i<MAX_REQUESTED_PB_ENTRIES; i++)
//    {
//        if( gAppPBContext.fctCtx.pb.entries.pPpbr[i] != NULL )
//        {
//            gAppPBContext.fctCtx.pb.entries.pbNumbers[i].start = 0;
//            gAppPBContext.fctCtx.pb.entries.pbNumbers[i].end   = 0;
//            POOL_sfFree(gAppPBContext.fctCtx.pb.entries.pPpbr[i]);
//            gAppPBContext.fctCtx.pb.entries.pPpbr[i] = NULL;
//            for(j=0; j<MAX_REQUESTED_PB_NUMBERS_PER_ENTRY; j++)
//            {
//                if( gAppPBContext.fctCtx.pb.entries.pbNumbers[i].pPpbp[j] != NULL)
//                {
//                    POOL_sfFree(gAppPBContext.fctCtx.pb.entries.pbNumbers[i].pPpbp[j]);
//                    gAppPBContext.fctCtx.pb.entries.pbNumbers[i].pPpbp[j] = NULL;
//                }
//            }
//        }
//    }
//}




/* End Of File APP_PHONEBOOK_AT_C1.c */


