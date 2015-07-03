/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     APP_PHONEBOOK_C1.c
*-----------------------------------------------------------------------------
* Module Name:    APP_PHONEBOOK
*-----------------------------------------------------------------------------
* Description:    Handles all call specific requests from BAP and
                  AT Cmd Handler
*-----------------------------------------------------------------------------
* $Date: 2011-04-27 17:43:05 +0200 (Mi, 27 Apr 2011) $
* $Rev: 19363 $
* $Author: nikolov $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_PHONEBOOK/trunk/src/APP_PHONEBOOK_C1.c $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include <osek.h>           // vector
#include <osekext.h>        // vector
#include "bap_defines.h"    // vw
#include "BAP_Dispatcher.h"
#include "ATCmdHandler.h"
#include "APP_CAN.h"
#include "ERM.h"
#include "UART.h"
#include "APP_COMMON_Utils.h"
#include "APP_PHONE.h"
#include "APP_PHONEBOOK_BAP.h"
#include "APP_SD.h"
#include "APP_CSHDL.h"

#include "APP_PHONEBOOK_C1.id"
#include "TRACE.h"
/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "APP_PHONEBOOK.h"
#include "APP_PHONEBOOK_CI.h"
#include "APP_PHONEBOOK_AT.h"

/*----------------------------------------------------------------------------*/
/* pragma Directives                                                          */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Definitions (avoid as much as possible!)                  */
/*----------------------------------------------------------------------------*/

/**
\brief	variable to save the current call management function currently in progress
*/
AppPhoneBookContext gAppPBContext;
//Uint16Type 	pbDwldEntry = 0U;
//static Uint32Type		pbDwldCounter = 0U;

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
static SuccessFailType _APP_PHONEBOOK_sfSendPendingPbSortOrder(const SCI_ST* puart);
static SuccessFailType _APP_PHONEBOOK_sfSendPendingUpdatePbSize(const SCI_ST* puart);
static SuccessFailType _APP_PHONEBOOK_sfSendPendingUpdateSinglePbSize(const SCI_ST* puart);
static SuccessFailType _APP_PHONEBOOK_sfSendPendingActivatePpds(const SCI_ST* puart);
static SuccessFailType _APP_PHONEBOOK_sfSendPendingPbReadRequest(const SCI_ST* puart);
static SuccessFailType _APP_PHONEBOOK_sfSendPendingSearchRequest(const SCI_ST* puart);
static SuccessFailType _APP_PHONEBOOK_sfSendPendingAddPbEntryRequest(const SCI_ST* puart);
static SuccessFailType _APP_PHONEBOOK_sfHandlePendingFunction( const SCI_ST* puart );
static SuccessFailType _APP_PHONEBOOK_sfSendPendingManualSyncRequest(const SCI_ST* puart);
static SuccessFailType _APP_PHONEBOOK_sfSendPendingCallStackResetRequest(const SCI_ST* puart);
static SuccessFailType _APP_PHONEBOOK_sfSendPendingReqNumVoiceTagsForContact(const SCI_ST* puart);

static void _APP_PHONEBOOK_vUpdatePbCompletlyLoadedForActiveUser(void);
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

/*----------------------------------------------------------------------------*/
/* Private Address Calculations                                               */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONEBOOK_sfInitApp                                      */
/**
    \brief      Callback function for initialization of App Phonebook

    \brief      Description:\n

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType APP_PHONEBOOK_sfInitApp(void)
{
    (void)memset(&gAppPBContext, 0, sizeof(gAppPBContext));

    gAppPBContext.curSelectedPb = APP_PHONEBOOK_enPbUnknown;

//    pbDwldEntry = 0U;

    return SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_vReInitApp                                    */
/**
    \brief      reinit the context

    \brief      Description:\n

    \return     void
*/
/*----------------------------------------------------------------------------*/
void _APP_PHONEBOOK_vReInitApp(void)
{

    /* RT: 2347, after phone disconnect InitApp is called and we need to check if
           a callback waits for result, otherwise it will wait forever */
    if(gAppPBContext.fct.common.callback)
    {
        DEBUG_VALUE1(APP_PB_SWI_WNG,MOD_APP_PB, "PB_vReInitApp: callback(ERR) for %d",
                           gAppPBContext.fct.common.fctId);

        gAppPBContext.fct.common.callback(APP_COMMON_RSP_ERROR);
    }

    (void)ERM_ReleaseUartForPhonebook();

    APP_PHONEBOOK_sfInitApp();
}

void _APP_PHONEBOOK_vReleaseFctContext()
{
    APP_PHONEBOOK_enFctId fctId = gAppPBContext.fct.common.fctId;
    (void)memset(&gAppPBContext.fct, 0, sizeof(gAppPBContext.fct));

    DEBUG_VALUE1(APP_PB_SWI_TRC,MOD_APP_PB, "ReleaseFctContext: fctId %d",
                    fctId);

}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_vSendResultReleaseContext                     */
/**
    \brief      notify caller of last function ( if available ) about result
                and release the function context

    \brief      Description:\n
                notify caller of last function ( if available ) about result\n
                and release the function context

    \param      result
                APP_COMMON_enResponseCode

    \return     void
*/
/*----------------------------------------------------------------------------*/
void _APP_PHONEBOOK_vSendResultReleaseContext(APP_COMMON_enResponseCode result)
{
    DEBUG_VALUE2(APP_PB_SWI_TRC, MOD_APP_PB, "RelContext: fctId %d result %d",
                         gAppPBContext.fct.common.fctId, result );

    if(gAppPBContext.fct.common.callback)
    {
        gAppPBContext.fct.common.callback(result);
    }

    _APP_PHONEBOOK_vReleaseFctContext();


    /* check for queued internal functions */
    if(gAppPBContext.pending.activatePpds)
    {
        gAppPBContext.pending.activatePpds = BT_FALSE;

        (void)_APP_PHONEBOOK_sfActivatePpds(BT_TRUE);
    }
    /*
    else if(gAppPBContext.pending.deactivatePpds)
    */
    else if(gAppPBContext.pending.pbSizeUpdate)
    {
        gAppPBContext.pending.pbSizeUpdate = BT_FALSE;

        (void)_APP_PHONEBOOK_sfUpdatePbSizes();
    }
    else if(gAppPBContext.pending.pbCompletelyLoaded)
    {
        const ERMUserProfileType * profile = ERM_pGetActiveUserProfile();
        if( NULL != profile )
        {
            Uint8Type profileIdx = ERM_u8GetUserProfileIndexForDeviceId(profile->deviceIndex);
            (void)APP_DEVICE_rcSetPbCompletelyLoaded(NULL, profileIdx);
        }
        gAppPBContext.pending.pbCompletelyLoaded = BT_FALSE;
    }
    // If single call stack has been updated after all calls idle again and there was a function already in progress,
    // we need to check here, which call stack has to be updated
    else if(gAppPBContext.pending.updMissedStackSize)
    {
        _APP_PHONEBOOK_sfUpdateSinglePbSize(APP_PHONEBOOK_enMissedCalls);
        gAppPBContext.pending.updMissedStackSize = BT_FALSE;
    }
    else if(gAppPBContext.pending.updReceivedStackSize)
    {
        _APP_PHONEBOOK_sfUpdateSinglePbSize(APP_PHONEBOOK_enReceivedCalls);
        gAppPBContext.pending.updReceivedStackSize = BT_FALSE;
    }
    else if(gAppPBContext.pending.updDialedStackSize)
    {
        _APP_PHONEBOOK_sfUpdateSinglePbSize(APP_PHONEBOOK_enLastDialed);
        gAppPBContext.pending.updDialedStackSize = BT_FALSE;
    }
    else if(gAppPBContext.pending.updPbSize)
    {
        _APP_PHONEBOOK_sfUpdateSinglePbSize(APP_PHONEBOOK_enUsersPB);
        gAppPBContext.pending.updPbSize = BT_FALSE;
    }
    else
    {
    }

}

/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONEBOOK_vResetMissedCallsByHMI                         */
/**
    \brief      one of the HMIs ( VCR, BAP, SD ) has shown / read out the
                missed calls for the user. So we need to notify all the other
                HMIs so they can update their states

    \brief      Description:\n

    \return     void
*/
/*----------------------------------------------------------------------------*/
void APP_PHONEBOOK_vResetMissedCallsByHMI()
{
    // reset missed calls saved in APP_PHONE during KL15off
    APP_PHONE_vResetMissedCalls();
    // Notify Apps about this
    _APP_PHONEBOOK_vNotifyApps(APP_PB_MISSED_CALLS_RESET_BY_HMI);
}


/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONEBOOK_vHandleEvents                                  */
/**
    \brief      Task Function to handle events for Phonebook Application

    \brief      Description:\n
                Waits for incoming events and if one arrives function \n
                reads Messages received From BAP-Dispatcher and \n
                AT_Cmd_Parser and handles them. \n

    \param      event
                EventMaskType
                Bitmask containing all unhandled events

    \return     void
*/
/*----------------------------------------------------------------------------*/

void APP_PHONEBOOK_vHandleEvents(EventMaskType event)
{
    // wait for interesting events

    // ****************************************************
    // check if we have some additional things to do
    // from the last callFctId triggered by BAP
    // This is important if e.g. more than one AT commands
    // have to be send to process one BAP-Indication
    // ***************************************************

    // remove event evAppTaskPhoneBookTResponse and message msgPhoneBookATResponse
    // from APP_PHONE_vHandleEvents to central AT-handling in
    // APP_COMMON_vHandleEvents

    if( event & evAppTaskPhoneBookFctPending )
    {
        const SCI_ST * puart    = NULL;
        (void)ClearEvent( evAppTaskPhoneBookFctPending );

        if(BT_TRUE == gAppPBContext.pending.notifyPbSortingTimout)
        {
            gAppPBContext.pending.notifyPbSortingTimout = BT_FALSE;
            DEBUG_TEXT(APP_PB_SWI_ERR, MOD_APP_PB, "HandleEvents: PbSortTimeout!");

            // no notification for new PB received, set the flag back
            if(gAppPBContext.SortingOrderChangedSentByUser == BT_TRUE)
            {
                // as sorting was not really successful, we don`t want to trigger a "new pb" update
                // so we will first sent PbState 0x00 for not available and then activate the pb
                _APP_PHONEBOOK_vNotifyApps(APP_PB_SORTING_TIMED_OUT);
                gAppPBContext.SortingOrderChangedSentByUser = BT_FALSE;
            }
            else if((AT_PB_LOADING != APP_PHONEBOOK_enGetPBDwldState()) &&
                    BT_FALSE == gAppPBContext.pending.notifyPbSortingTimout)
            {/* sorting order change was requested by APP_DEVICE_Profles, inform the HU and CI that new PB is available
            if after time out no new PB was available after PPNO(e.g. no *PPBC:0 event)*/
                (void)_APP_PHONEBOOK_sfUpdatePbSizes();

                _APP_PHONEBOOK_vNotifyApps(APP_PB_DOWNLOAD_STATE_CHANGED);
            }
        }

        //RS: prevent from handle only one event although more events were set
        if(gAppPBContext.fct.common.fctId != APP_PB_enInvalidFct)
        {
            if(ERM_bIsStartupDone() != BT_TRUE)
            {
                /* we are not able to to request the UART in this state */
                DEBUG_VALUE1(APP_PB_SWI_ERR,MOD_APP_PB, "HandleEvents: CK5050_SHUTDOWN, drop pending, fct %d",
                                     gAppPBContext.fct.common.fctId);

                _APP_PHONEBOOK_vSendResultReleaseContext(APP_COMMON_RSP_ERROR);
            }
            else
            {

                // try to get ressource
                puart = ERM_GetUartForPhonebook();
                if( !puart )
                {
                    // ressource is busy, set an alarm to try again in some cycles
                    // set event once again after 10 ms
                    (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_PHONEBOOK_FCT_PENDING,
                                                        MSEC(ERM_UART_BUSY_RETRY_TIME),
                                                        0);

                }
                else
                {
                    // call common function to handle next step
                   (void) _APP_PHONEBOOK_sfHandlePendingFunction(puart);
                }
            }
        }
    }
}

// Handle Pending BApFct
static SuccessFailType _APP_PHONEBOOK_sfHandlePendingFunction( const SCI_ST* puart )
{
    SuccessFailType result = SUCCESS;
//    Uint32Type pbDwldCounter_temp = 0U;
//    Uint16Type pbDwldEntry_temp = 0U;


    switch( gAppPBContext.fct.common.fctId )
    {
        case APP_PB_enUpdatePbSizes:
            result = _APP_PHONEBOOK_sfSendPendingUpdatePbSize(puart);
            break;
        case APP_PB_enUpdateSinglePbSize:
            result = _APP_PHONEBOOK_sfSendPendingUpdateSinglePbSize(puart);
            break;
        case APP_PB_enActivatePpds:
            result = _APP_PHONEBOOK_sfSendPendingActivatePpds(puart);
            break;
        case APP_PB_enReadCompletePbEntries:
            result = _APP_PHONEBOOK_sfSendPendingPbReadRequest(puart);
            break;
        case APP_PB_enSearchPb:
            result = _APP_PHONEBOOK_sfSendPendingSearchRequest(puart);
            break;
        case APP_PB_enManualPbSync:
            result = _APP_PHONEBOOK_sfSendPendingManualSyncRequest(puart);
            break;
        case APP_PB_enDeleteCallStacks:
            result = _APP_PHONEBOOK_sfSendPendingCallStackResetRequest(puart);
            break;
        case APP_PB_enGetPbSortOrder:
        case APP_PB_enSetPbSortOrder:
            result = _APP_PHONEBOOK_sfSendPendingPbSortOrder(puart);
            break;
        case APP_PB_enAddPbEntry:
            result = _APP_PHONEBOOK_sfSendPendingAddPbEntryRequest(puart);
            break;
        case APP_PB_enRequestNumVoiceTagsForContact:
            result = _APP_PHONEBOOK_sfSendPendingReqNumVoiceTagsForContact(puart);
            break;
        default:
            DEBUG_VALUE1(APP_PB_SWI_ERR,MOD_APP_PB, "HaPendFct: unsupported fctId %d",
                           gAppPBContext.fct.common.fctId);
            result = FAIL;
            break;
    }

    if( SUCCESS != result )
    {
        _APP_PHONEBOOK_vSendResultReleaseContext(APP_COMMON_RSP_ERROR);
        (void)ERM_ReleaseUartForPhonebook();
    }

    return result;
}



/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_sfSendPendingUpdatePbSize                            */
/**
    \brief      Select the next step to be done depending on the current \n
                state updating the sizes of the prior refreshed phonebooks.

    \brief      Description:\n
                Select the next step to be done depending on the current \n
                state updating the sizes of the prior refreshed phonebooks. \n

    \param      puart
                const SCI_ST*

    \return     SuccessFailType
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
static SuccessFailType _APP_PHONEBOOK_sfSendPendingUpdatePbSize(const SCI_ST* puart)
{
    //gAppPBContext.refreshState = _APP_PHONEBOOK_enSelectNextRefreshState();
    SuccessFailType result = SUCCESS;

    switch(gAppPBContext.fct.updatePbSizes.state)
    {
        case APP_PHONEBOOK_UPDATE_NOT_SENT:
            result = _APP_PHONEBOOK_AT_sfSendSelectPbATRequest(APP_PHONEBOOK_enUsersPB, puart);
            gAppPBContext.fct.updatePbSizes.state = APP_PHONEBOOK_SELECT_USER_PB_SENT;
            break;
        case APP_PHONEBOOK_SELECT_USER_PB_SENT:
            //gAppPBContext.curSelectedPb = APP_PHONEBOOK_enUsersPB;
            result = _APP_PHONEBOOK_AT_sfSendPbSizeATRequest(puart);
            gAppPBContext.fct.updatePbSizes.state = APP_PHONEBOOK_REQUEST_USER_PB_SIZE_SENT;
            break;
        case APP_PHONEBOOK_REQUEST_USER_PB_SIZE_SENT:
            result = _APP_PHONEBOOK_AT_sfSendSelectPbATRequest(APP_PHONEBOOK_enMissedCalls, puart);
            gAppPBContext.fct.updatePbSizes.state = APP_PHONEBOOK_SELECT_MISSED_PB_SENT;
            break;
        case APP_PHONEBOOK_SELECT_MISSED_PB_SENT:
            result = _APP_PHONEBOOK_AT_sfSendPbSizeATRequest(puart);
            gAppPBContext.fct.updatePbSizes.state = APP_PHONEBOOK_REQUEST_MISSED_PB_SIZE_SENT;
            break;
        case APP_PHONEBOOK_REQUEST_MISSED_PB_SIZE_SENT:
            result = _APP_PHONEBOOK_AT_sfSendSelectPbATRequest(APP_PHONEBOOK_enReceivedCalls, puart);
            gAppPBContext.fct.updatePbSizes.state = APP_PHONEBOOK_SELECT_RECEIVED_PB_SENT;
            break;
        case APP_PHONEBOOK_SELECT_RECEIVED_PB_SENT:
            result = _APP_PHONEBOOK_AT_sfSendPbSizeATRequest(puart);
            gAppPBContext.fct.updatePbSizes.state = APP_PHONEBOOK_REQUEST_RECEIVED_PB_SIZE_SENT;
            break;
        case APP_PHONEBOOK_REQUEST_RECEIVED_PB_SIZE_SENT:
            result = _APP_PHONEBOOK_AT_sfSendSelectPbATRequest(APP_PHONEBOOK_enLastDialed, puart);
            gAppPBContext.fct.updatePbSizes.state = APP_PHONEBOOK_SELECT_DIALED_PB_SENT;
            break;
        case APP_PHONEBOOK_SELECT_DIALED_PB_SENT:
            result = _APP_PHONEBOOK_AT_sfSendPbSizeATRequest(puart);
            gAppPBContext.fct.updatePbSizes.state = APP_PHONEBOOK_REQUEST_DIALED_PB_SIZE_SENT;
            break;
        default:
            DEBUG_VALUE1(APP_PB_SWI_ERR,MOD_APP_PB, "SendPendUpdPbSize: unsupported state %d",
                           gAppPBContext.fct.updatePbSizes.state );
            result = FAIL;
            break;
    }

    return result;
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_sfSendPendingUpdateCallStackSize                            */
/**
    \brief      Select the next step to be done depending on the current \n
                state updating the sizes of the prior refreshed phonebooks.

    \brief      Description:\n
                Select the next step to be done depending on the current \n
                state updating the sizes of the prior refreshed phonebooks. \n

    \param      puart
                const SCI_ST*

    \return     SuccessFailType
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
static SuccessFailType _APP_PHONEBOOK_sfSendPendingUpdateSinglePbSize(const SCI_ST* puart)
{
    //gAppPBContext.refreshState = _APP_PHONEBOOK_enSelectNextRefreshState();
    SuccessFailType result = SUCCESS;

    switch(gAppPBContext.fct.updSinglePbSize.state)
    {
        case APP_PHONEBOOK_UPDATE_PB_SIZE_NOT_SENT:
            result = _APP_PHONEBOOK_AT_sfSendSelectPbATRequest(gAppPBContext.fct.updSinglePbSize.pbType, puart);
            gAppPBContext.fct.updSinglePbSize.state = APP_PHONEBOOK_SELECT_PB_2_SENT;
            break;
        case APP_PHONEBOOK_SELECT_PB_2_SENT:
            result = _APP_PHONEBOOK_AT_sfSendPbSizeATRequest(puart);
            gAppPBContext.fct.updSinglePbSize.state = APP_PHONEBOOK_REQUEST_PB_SIZE_SENT;
            break;
        default:
            DEBUG_VALUE1(APP_PB_SWI_ERR, MOD_APP_PB, "SendPendUpdStackSize: unsupported state %d",
                           gAppPBContext.fct.updSinglePbSize.state );
            result = FAIL;
            break;
    }

    return result;
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_sfSendPendingActivatePpds                     */
/**
    \brief

    \brief      Description:\n

    \param      puart
                const SCI_ST*

    \return     SuccessFailType
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
static SuccessFailType _APP_PHONEBOOK_sfSendPendingActivatePpds(const SCI_ST* puart)
{
    SuccessFailType result = SUCCESS;



    switch(gAppPBContext.fct.activatePpds.state)
    {
        case APP_PHONEBOOK_ACTIVATE_PPDS_NOT_SENT:

            result = _APP_PHONEBOOK_AT_sfSendActivatePpds( gAppPBContext.fct.activatePpds.enable, puart);
            gAppPBContext.fct.activatePpds.state = APP_PHONEBOOK_ACTIVATE_PPDS_SENT;
            break;
        default:
            DEBUG_VALUE1(APP_PB_SWI_ERR,MOD_APP_PB,"SendPendActPpds: unsupported state %d",
                           gAppPBContext.fct.activatePpds.state );
            result = FAIL;
            break;
    }

    return result;
}

/*----------------------------------------------------------------------------*/

/* Function    : _APP_PHONEBOOK_sfSendPendingManualSyncRequest                   */
/**
    \brief      handles states / next steps to be done when APPs wanna \n
                manual sync the phonebook \n

    \brief      Description:\n
                handles states / next steps to be done when APPs wanna \n
                manual sync the phonebook \n

    \param      puart
                const SCI_ST*

    \return     SuccessFailType
*/
/*----------------------------------------------------------------------------*/
static SuccessFailType _APP_PHONEBOOK_sfSendPendingManualSyncRequest(const SCI_ST* puart)
{
    SuccessFailType result = SUCCESS;

    switch( gAppPBContext.fct.pbSync.state )
    {
        case APP_PHONEBOOK_PB_SYNC_NOT_SENT:
            /* set to 0 to force a pb changed notification */
            //gAppPBContext.usersPersonalPb.pbSize = 0;
//            pbDwldCounter = osGetSystemCounter();
            gAppPBContext.dwldCount = 0U;
            // send PPMS request
            result = _APP_PHONEBOOK_AT_sfSendPbSyncATRequest(puart);
            gAppPBContext.fct.pbSync.state = APP_PHONEBOOK_PB_SYNC_SENT;
       		 DEBUG_VALUE1(APP_PB_SWI_TRC,MOD_APP_PB, "SYS_PERF: PB Download Startup Event: abs. %d ms",
                          osGetSystemCounter() );

            break;

        default:
            DEBUG_VALUE1(APP_PB_SWI_ERR,MOD_APP_PB, "SendPendManualSyncReq: unsup state %d",
                           gAppPBContext.fct.pbSync.state );
            result = FAIL;
    }

    return result;

}

/*----------------------------------------------------------------------------*/

/* Function    : _APP_PHONEBOOK_sfSendPendingPbSortOrder                   */
/**
    \brief      handles states / next steps to be done when APPs wanna \n
                get/set the current sort order of PB \n

    \brief      Description:\n
                handles states / next steps to be done when APPs wanna \n
                get/set the current sort order of PB \n

    \param      puart
                const SCI_ST*

    \return     SuccessFailType
*/
/*----------------------------------------------------------------------------*/
static SuccessFailType _APP_PHONEBOOK_sfSendPendingPbSortOrder(const SCI_ST* puart)
{
    SuccessFailType result = SUCCESS;

    switch( gAppPBContext.fct.pbSortOrder.state )
    {
        case APP_PHONEBOOK_PB_SORTORDER_NOT_SENT:
            result = _APP_PHONEBOOK_AT_sfSendPbSortOrderATRequest(puart,
                                        gAppPBContext.fct.pbSortOrder.mode);
            gAppPBContext.fct.pbSortOrder.state = APP_PHONEBOOK_PB_SORTORDER_SENT;
            break;

        default:
            DEBUG_VALUE1(APP_PB_SWI_ERR,MOD_APP_PB, "SendPendPbSortOrderReq: unsup state %d",
                           gAppPBContext.fct.pbSortOrder.state );
            result = FAIL;
    }

    return result;

}

/*----------------------------------------------------------------------------*/

/* Function    : _APP_PHONEBOOK_sfSendPendingAddPbEntryRequest                */
/**
    \brief      handles states / next steps to be done when APPs wanna \n
                add a new contact to PB \n

    \brief      Description:\n
                handles states / next steps to be done when APPs wanna \n
                add a new contact to PB \n

    \param      puart
                const SCI_ST*

    \return     SuccessFailType
*/
/*----------------------------------------------------------------------------*/
static SuccessFailType _APP_PHONEBOOK_sfSendPendingAddPbEntryRequest(const SCI_ST* puart)
{
    SuccessFailType result = SUCCESS;

    switch( gAppPBContext.fct.pbAddEntry.state )
    {
        case APP_PHONEBOOK_ADD_PB_ENTRY_NOT_SENT:
            result = _APP_PHONEBOOK_AT_sfSendAddPbEntryATRequest(puart,
                                        gAppPBContext.fct.pbAddEntry.name,
                                        gAppPBContext.fct.pbAddEntry.name_len,
                                        gAppPBContext.fct.pbAddEntry.number,
                                        gAppPBContext.fct.pbAddEntry.number_len,
                                        gAppPBContext.fct.pbAddEntry.pnType);
            gAppPBContext.fct.pbAddEntry.state = APP_PHONEBOOK_ADD_PB_ENTRY_SENT;
            break;

        default:
            DEBUG_VALUE1(APP_PB_SWI_ERR,MOD_APP_PB, "SendPendPbSortOrderReqt: unsup state %d",
                           gAppPBContext.fct.pbSortOrder.state );
            result = FAIL;
    }

    return result;

}

static SuccessFailType _APP_PHONEBOOK_sfSendPendingReqNumVoiceTagsForContact(const SCI_ST* puart)
{
    SuccessFailType result = SUCCESS;

    switch( gAppPBContext.fct.numVoiceTags.state )
    {
        case APP_PHONEBOOK_PB_REQ_NUM_TAGS_NOT_SENT:
            result = _APP_PHONEBOOK_AT_sfSendNumVoiceTagsATRequest(puart,
                                        gAppPBContext.fct.numVoiceTags.pbIndex);
            // wait for OK /ERROR
            gAppPBContext.fct.numVoiceTags.state = APP_PHONEBOOK_PB_REQ_NUM_TAGS_SENT;
            break;

        case APP_PHONEBOOK_PB_REQ_NUM_TAGS_SENT:
            // received OK, wait for RVTA:x event
            gAppPBContext.fct.numVoiceTags.state = APP_PHONEBOOK_PB_REQ_NUM_TAGS_WAIT_RSP;
            break;

        case APP_PHONEBOOK_PB_REQ_NUM_TAGS_WAIT_RSP:
            break;

        default:
            DEBUG_VALUE1(APP_PB_SWI_ERR,MOD_APP_PB, "SendPendPbSortOrderReqt: unsup state %d",
                           gAppPBContext.fct.pbSortOrder.state );
            result = FAIL;
    }

    return result;

}

/*----------------------------------------------------------------------------*/

/* Function    : _APP_PHONEBOOK_sfSendPendingCallStackResetRequest                   */
/**
    \brief      handles states / next steps to be done when APPs wanna \n
                reset the call stacks \n

    \brief      Description:\n
                handles states / next steps to be done when APPs wanna \n
                reset the call stacks \n

    \param      puart
                const SCI_ST*

    \return     SuccessFailType
*/
/*----------------------------------------------------------------------------*/
static SuccessFailType _APP_PHONEBOOK_sfSendPendingCallStackResetRequest(const SCI_ST* puart)
{
    SuccessFailType result = SUCCESS;

    switch( gAppPBContext.fct.pbCallStackRst.state )
    {
        case APP_PHONEBOOK_PB_CALL_STACK_RESET_NOT_SENT:
            // send first PPMS request
            result = _APP_PHONEBOOK_AT_sfSendPbCallStackResetATRequest(puart);
            gAppPBContext.fct.pbCallStackRst.state = APP_PHONEBOOK_PB_CALL_STACK_RESET_SENT;
            break;

        default:
            DEBUG_VALUE1(APP_PB_SWI_ERR, MOD_APP_PB, "SendPendCallStackResetReq: unsup state %d",
                           gAppPBContext.fct.pbCallStackRst.state );
            result = FAIL;
    }
    return result;
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_sfSendPendingPbSearchRequest                   */
/**
    \brief      handles states / next steps to be done when APPs wanna \n
                search the phonebook \n

    \brief      Description:\n
                handles states / next steps to be done when APPs wanna \n
                search the phonebook \n

    \param      puart
                const SCI_ST*

    \return     SuccessFailType
*/
/*----------------------------------------------------------------------------*/
static SuccessFailType _APP_PHONEBOOK_sfSendPendingSearchRequest(const SCI_ST* puart)
{

    SuccessFailType result = SUCCESS;

    switch( gAppPBContext.fct.pbSearch.state )
    {
        case APP_PHONEBOOK_EXTENDED_PB_SEARCH_NOT_SENT:
            // send ppasex request
            result = _APP_PHONEBOOK_AT_sfSendPbSearchExtendedATRequest(
                                    gAppPBContext.fct.pbSearch.searchStr,
                                    gAppPBContext.fct.pbSearch.searchStrLen,
                                    puart);
            gAppPBContext.fct.pbSearch.state = APP_PHONEBOOK_EXTENDED_PB_SEARCH_SENT;
            break;


        //case APP_PHONEBOOK_PB_SEARCH_NOT_SENT:
        //    // send first ppas request
        //    result = _APP_PHONEBOOK_AT_sfSendPbSearchATRequest(
        //                            gAppPBContext.fct.pbSearch.searchStr,
        //                            gAppPBContext.fct.pbSearch.searchStrLen,
        //                            puart);
        //    gAppPBContext.fct.pbSearch.state = APP_PHONEBOOK_PB_SEARCH_START_SENT;
        //    break;

        //case APP_PHONEBOOK_PB_SEARCH_START_SENT:
        //{
        //    // get character following to last character in search string
        //    //_APP_PHONEBOOK_u32GetSuccessorCharacter();
        //    // quick hack: increment last byte ( works for ASCII only )
        //    if(gAppPBContext.fct.pbSearch.searchStrLen)
        //    {
        //        gAppPBContext.fct.pbSearch.searchStr[gAppPBContext.fct.pbSearch.searchStrLen-1] += 1;
        //    }
        //    // send second ppas request
        //    result = _APP_PHONEBOOK_AT_sfSendPbSearchATRequest(
        //                            gAppPBContext.fct.pbSearch.searchStr,
        //                            gAppPBContext.fct.pbSearch.searchStrLen,
        //                            puart);
        //    gAppPBContext.fct.pbSearch.state = APP_PHONEBOOK_PB_SEARCH_END_SENT;
        //    break;
        //}
        default:
            DEBUG_VALUE1(APP_PB_SWI_ERR,MOD_APP_PB, "SendPendingPbSearchReq: unsup state %d",
                           gAppPBContext.fct.pbSearch.state );
            result = FAIL;
            break;
    }

    return result;

}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_sfSendPendingPbSearchRequest                   */
/**
    \brief      handles states / next steps to be done when APPs wanna \n
                search the phonebook for voicemailbox number \n

    \brief      Description:\n
                handles states / next steps to be done when APPs wanna \n
                search the phonebook for voicemailbox number\n

    \param      puart
                const SCI_ST*

    \return     SuccessFailType
*/
/*----------------------------------------------------------------------------*/
//SuccessFailType _APP_PHONEBOOK_sfSendPendingVoiceMailboxSearchRequest(const SCI_ST* puart)
//{
//    SuccessFailType result = SUCCESS;
//
//    switch( gAppPBContext.fct.voiceMailboxSearch.state )
//    {
//        case APP_PHONEBOOK_EXTENDED_PB_SEARCH_NOT_SENT:
//        {
//            // send first ppas request
//            const Uint8Type * searchStr = NULL;
//            Uint16Type searchStrLen     = 0;
//            switch( gAppPBContext.fct.voiceMailboxSearch.lang)
//            {
//                case APP_SD_enLANG_German:
//                    /* search for Sprachmailbox, Mailbox, Sprachnachrichten, Anrufbeantworter */
//                    break;
//                case APP_SD_enLANG_Spanish:
//                    /* search for Buzón de Voz, Mensajes de Voz */
//                    break;
//                case APP_SD_enLANG_English:
//                case APP_SD_enLANG_Italian:     // use english values for now
//                case APP_SD_enLANG_French:      // use english values for now
//                case APP_SD_enLANG_Portugese:   // use english values for now
//                    /* search for Voicemail, mailbox, voice messages */
//                    break;
//            }
//
//            result = _APP_PHONEBOOK_AT_sfSendPbSearchATRequest(
//                                    searchStr,
//                                    searchStrLen,
//                                    puart);
//            gAppPBContext.fct.voiceMailboxSearch.state = APP_PHONEBOOK_EXTENDED_PB_SEARCH_SENT;
//            break;
//        }
//        case APP_PHONEBOOK_EXTENDED_PB_SEARCH_SENT:
//        {
//            #if 0
//            // get character following to last character in search string
//            //_APP_PHONEBOOK_u32GetSuccessorCharacter();
//            // quick hack: increment last byte ( works for ASCII only )
//            if(gAppPBContext.fct.pbSearch.searchStrLen)
//            {
//                searchStr[searchStrLen-1] += 1;
//            }
//            // send second ppas request
//            result = _APP_PHONEBOOK_AT_sfSendPbSearchATRequest(
//                                    searchStr,
//                                    searchStrLen,
//                                    puart);
//            gAppPBContext.fct.pbSearch.state = APP_PHONEBOOK_PB_SEARCH_END_SENT;
//            #endif
//
//            break;
//        }
//        default:
//
//            result = FAIL;
//    }
//
//    return result;
//
//
//
//}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_sfSendPendingPbReadRequest                   */
/**
    \brief      handles states / next steps to be done when HU requests \n
                phonebook entries \n

    \brief      Description:\n
                handles states / next steps to be done when HU requests \n
                phonebook entries \n

    \param      puart
                const SCI_ST*

    \return     SuccessFailType
*/
/*----------------------------------------------------------------------------*/
static SuccessFailType _APP_PHONEBOOK_sfSendPendingPbReadRequest(const SCI_ST* puart)
{
    SuccessFailType result = SUCCESS;

//    APP_PHONEBOOK_enSelectedPbType pbToSelect;
//    Uint16Type pbSize;
//
//    pbToSelect  = gAppPBContext.fct.pbRead.pbType;

    //gAppPBContext.fct.pb.entries.requestState = _APP_PHONEBOOK_AT_PPBR_enGetNextRequestEntriesState();

    switch( gAppPBContext.fct.pbRead.state )
    {
        case APP_PHONEBOOK_PB_READ_NOT_SENT:
            result = _APP_PHONEBOOK_AT_sfSendSelectPbATRequest( gAppPBContext.fct.pbRead.pbType,
                                                                puart);
            gAppPBContext.fct.pbRead.state = APP_PHONEBOOK_SELECT_PB_SENT;
            break;

        case APP_PHONEBOOK_SELECT_PB_SENT:
        {
            Uint16Type pbSize = APP_PHONEBOOK_u16GetCurrentPBSize(gAppPBContext.fct.pbRead.pbType);

            DEBUG_VALUE2(APP_PB_SWI_TRC,MOD_APP_PB, "SendPendPbReadReq: start:%d, end:%d",
                           gAppPBContext.fct.pbRead.start, gAppPBContext.fct.pbRead.end );

            // TODO: validate the request range
            result =  _APP_PHONEBOOK_AT_sfSendPbEntriesATRequest( puart,
                                                                  gAppPBContext.fct.pbRead.start,
                                                                  gAppPBContext.fct.pbRead.end,
                                                                  pbSize );

            gAppPBContext.fct.pbRead.state = APP_PHONEBOOK_READ_PB_ENTRIES_SENT;
            break;
        }
        case APP_PHONEBOOK_READ_PB_ENTRIES_SENT:
        {
            result = _APP_PHONEBOOK_AT_sfSendPbNumbersATRequest(
                                    puart,
                                    gAppPBContext.fct.pbRead.pbIndex,
                                    1U,
                                    gAppPBContext.fct.pbRead.pnSize);

            gAppPBContext.fct.pbRead.state = APP_PHONEBOOK_READ_PB_NUMBERS_SENT;
            break;

        }
        default:
            DEBUG_VALUE1(APP_PB_SWI_ERR, MOD_APP_PB, "SendPendPbReadReq: unsup state %d",
                           gAppPBContext.fct.pbRead.state );
            result = FAIL;
    }

    return result;
}

/*----------------------------------------------------------------------------*/

void _APP_PHONEBOOK_vOnUpdatePbResult(ATResponseEnum rsp)
{
    APP_COMMON_enResponseCode response  = APP_COMMON_RSP_ERROR;
    BooleanType               finished  = BT_FALSE;

    if(rsp == AT_RSP_OK)
    {
        switch(gAppPBContext.fct.updatePbSizes.state)
        {
            case APP_PHONEBOOK_SELECT_USER_PB_SENT:
                gAppPBContext.curSelectedPb = APP_PHONEBOOK_enUsersPB;
                break;
            case APP_PHONEBOOK_SELECT_MISSED_PB_SENT:
                gAppPBContext.curSelectedPb = APP_PHONEBOOK_enMissedCalls;
                break;
            case APP_PHONEBOOK_SELECT_RECEIVED_PB_SENT:
                gAppPBContext.curSelectedPb = APP_PHONEBOOK_enReceivedCalls;
                break;
            case APP_PHONEBOOK_SELECT_DIALED_PB_SENT:
                gAppPBContext.curSelectedPb = APP_PHONEBOOK_enLastDialed;
                break;
            case APP_PHONEBOOK_REQUEST_USER_PB_SIZE_SENT:
            case APP_PHONEBOOK_REQUEST_MISSED_PB_SIZE_SENT:
            case APP_PHONEBOOK_REQUEST_RECEIVED_PB_SIZE_SENT:
                /* do nothing special */
                break;
            case APP_PHONEBOOK_REQUEST_DIALED_PB_SIZE_SENT:
                /* we are finished */
                response = APP_COMMON_RSP_OK;
                finished = BT_TRUE;
                break;

            default:
                DEBUG_VALUE1(APP_PB_SWI_ERR, MOD_APP_PB, "OnUpdatePbRes: unsup state %d",
                               gAppPBContext.fct.updatePbSizes.state );
                response = APP_COMMON_RSP_ERROR;
                finished = BT_TRUE;
                break;
        }
    }
    else
    {
        DEBUG_VALUE1(APP_PB_SWI_ERR,MOD_APP_PB, "OnUpdatePbRes: recvd AT_ERROR state %d",
                       gAppPBContext.fct.updatePbSizes.state );

        response = APP_COMMON_RSP_ERROR;
        finished = BT_TRUE;
    }


    if(finished)
    {
        _APP_PHONEBOOK_vSendResultReleaseContext(response);

        // reset refresh indicators! only if not in sync so we do not disturb during startup
        if(AT_PB_LOADING != gAppPBContext.dwldState)
        {
            DEBUG_VALUE1(APP_PB_SWI_TRC, MOD_APP_PB, "OnUpdPbSiz: reset refresh indic. (%d)",gAppPBContext.updatedPbIndicator);
            gAppPBContext.updatedPbIndicator = APP_PB_NONE_REFRESHED;

            /* change user profile in eeprom cause pb was downloaded completely */
            _APP_PHONEBOOK_vUpdatePbCompletlyLoadedForActiveUser();
        }

        /* sync is finished, all apps shall be notified about new pb, now we can reset, if this was a manual sync */
        gAppPBContext.bWasManualSync        = BT_FALSE;
    }
    else
    {
        (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_PHONEBOOK_FCT_PENDING, MSEC(0),0);
    }

}

void _APP_PHONEBOOK_vOnUpdateSinglePbSize(ATResponseEnum rsp)
{

    APP_COMMON_enResponseCode response  = APP_COMMON_RSP_ERROR;
    BooleanType               finished  = BT_FALSE;

    if(rsp == AT_RSP_OK)
    {
        switch(gAppPBContext.fct.updSinglePbSize.state)
        {
            case APP_PHONEBOOK_SELECT_PB_2_SENT:
                gAppPBContext.curSelectedPb = gAppPBContext.fct.updSinglePbSize.pbType;
                break;
            case APP_PHONEBOOK_REQUEST_PB_SIZE_SENT:
                /* we are finished */
                response = APP_COMMON_RSP_OK;
                finished = BT_TRUE;
                break;

            default:
                DEBUG_VALUE1(APP_PB_SWI_ERR, MOD_APP_PB, "OnUpdSglPbSizRes: unsup state %d",
                               gAppPBContext.fct.updSinglePbSize.state );
                response = APP_COMMON_RSP_ERROR;
                finished = BT_TRUE;
                break;
        }
    }
    else
    {
        DEBUG_VALUE1(APP_PB_SWI_ERR,MOD_APP_PB, "OnUpdSglPbSizRes: recvd AT_ERROR state %d",
                       gAppPBContext.fct.updSinglePbSize.state );

        response = APP_COMMON_RSP_ERROR;
        finished = BT_TRUE;
    }


    if(finished)
    {
        _APP_PHONEBOOK_vSendResultReleaseContext(response);

        // reset refresh indicators! only if not in sync so we do not disturb during startup
        if(AT_PB_LOADING != gAppPBContext.dwldState)
        {
            DEBUG_VALUE1(APP_PB_SWI_TRC, MOD_APP_PB, "OnUpdCallStackPbRes: reset refresh indic. (%d)",
                         gAppPBContext.updatedPbIndicator);
            gAppPBContext.updatedPbIndicator = APP_PB_NONE_REFRESHED;
        }
    }
    else
    {
        (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_PHONEBOOK_FCT_PENDING, MSEC(0),0);
    }
}

static void _APP_PHONEBOOK_vUpdatePbCompletlyLoadedForActiveUser()
{
    const ERMUserProfileType * profile = ERM_pGetActiveUserProfile();
    if( NULL != profile )
    {
        if( BT_FALSE == profile->bPbLoadedCompletly )
        {
            /* switch pb download state */
            BooleanType completelyLoaded = BT_FALSE;
            switch( gAppPBContext.dwldState )
            {
                case AT_PB_LOAD_FINISHED:
                    completelyLoaded = BT_TRUE;
                    break;
                case AT_PB_LOAD_FINISHED_BUT:
                    if( 0U < gAppPBContext.usersPersonalPb.pbSize )
                    {
                        completelyLoaded = BT_TRUE;
                    }
                    break;
            }
            if( BT_TRUE == completelyLoaded )
            {
                Uint8Type profileIdx = ERM_u8GetUserProfileIndexForDeviceId(profile->deviceIndex);
                if( APP_COMMON_RSP_ERROR_BUSY == APP_DEVICE_rcSetPbCompletelyLoaded(NULL, profileIdx))
                {
                    gAppPBContext.pending.pbCompletelyLoaded = BT_TRUE;
                }
            }
            else
            {
                DEBUG_VALUE2(APP_PB_SWI_ERR, MOD_APP_PB, "UpdPbCompl: not compl. 4 dev: %d state: %d ... ",
                             profile->deviceIndex, gAppPBContext.dwldState);
                DEBUG_VALUE1(APP_PB_SWI_ERR, MOD_APP_PB, "UpdPbCompl: not compl. ... size: %d",
                             gAppPBContext.usersPersonalPb.pbSize);
            }
        }
    }
}


void _APP_PHONEBOOK_vOnCallStackResetResult(ATResponseEnum rsp)
{
    APP_COMMON_enResponseCode response  = APP_COMMON_RSP_ERROR;
    BooleanType               finished  = BT_FALSE;

    if(rsp == AT_RSP_OK)
    {
        switch(gAppPBContext.fct.pbCallStackRst.state)
        {
        case APP_PHONEBOOK_PB_CALL_STACK_RESET_NOT_SENT:
                /* nothing special to do here, just set alarm (later) and go on*/
                break;
        case APP_PHONEBOOK_PB_CALL_STACK_RESET_SENT:
                /* we are finished */
                response  = APP_COMMON_RSP_OK;
                finished  = BT_TRUE;
                break;
        default:
            DEBUG_VALUE1(APP_PB_SWI_ERR, MOD_APP_PB, "OnCallStackResetRes: unsup state %d",
                           gAppPBContext.fct.pbCallStackRst.state );
            response = APP_COMMON_RSP_ERROR;
            finished = BT_TRUE;
            break;
        }
    }
    else
    {
        DEBUG_VALUE1(APP_PB_SWI_ERR, MOD_APP_PB, "OnCallStackResetRes: recvd AT_ERROR state %d",
                       gAppPBContext.fct.pbCallStackRst.state );

        response = APP_COMMON_RSP_ERROR;
        finished = BT_TRUE;
    }

    if(finished)
    {
        _APP_PHONEBOOK_vSendResultReleaseContext(response);
    }
    else
    {
        (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_PHONEBOOK_FCT_PENDING, MSEC(0),0);
    }
}



void _APP_PHONEBOOK_vOnSyncPbResult(ATResponseEnum rsp)
{
    APP_COMMON_enResponseCode response  = APP_COMMON_RSP_ERROR;
    BooleanType               finished  = BT_FALSE;

    if(rsp == AT_RSP_OK)
    {
        switch(gAppPBContext.fct.pbSync.state)
        {
        case APP_PHONEBOOK_PB_SYNC_NOT_SENT:
                /* nothing special to do here, just set alarm (later) and go on*/
                break;
        case APP_PHONEBOOK_PB_SYNC_SENT:
                /* we are finished */
                response  = APP_COMMON_RSP_OK;
                finished  = BT_TRUE;
                break;
        default:
            DEBUG_VALUE1(APP_PB_SWI_ERR, MOD_APP_PB, "OnSyncPbResult: unsup state %d",
                           gAppPBContext.fct.pbSync.state );
            response = APP_COMMON_RSP_ERROR;
            finished = BT_TRUE;
            break;
        }
    }
    else
    {
        DEBUG_VALUE1(APP_PB_SWI_ERR, MOD_APP_PB, "_vOnSyncPbRes: recvd AT_ERROR state %d",
                       gAppPBContext.fct.pbSync.state );

        response = APP_COMMON_RSP_ERROR;
        finished = BT_TRUE;
    }

    if(finished)
    {
        _APP_PHONEBOOK_vSendResultReleaseContext(response);
    }
    else
    {
        (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_PHONEBOOK_FCT_PENDING, MSEC(0),0);
    }
}



void _APP_PHONEBOOK_vOnSearchPbResult(ATResponseEnum rsp)
{
    APP_COMMON_enResponseCode response  = APP_COMMON_RSP_ERROR;
    BooleanType               finished  = BT_FALSE;

    if(rsp == AT_RSP_OK)
    {
        switch(gAppPBContext.fct.pbSearch.state)
        {
        case APP_PHONEBOOK_EXTENDED_PB_SEARCH_NOT_SENT:
                /* nothing special to do here, just set alarm (later) and go on*/
                break;
        case APP_PHONEBOOK_EXTENDED_PB_SEARCH_SENT:
                /* we are finished */
                response  = APP_COMMON_RSP_OK;
                finished  = BT_TRUE;
                break;
        default:
            DEBUG_VALUE1(APP_PB_SWI_ERR, MOD_APP_PB, "OnSearchPbRes: unsup state %d",
                           gAppPBContext.fct.pbSearch.state );
            response = APP_COMMON_RSP_ERROR;
            finished = BT_TRUE;
            break;
        }
    }
    else
    {
        DEBUG_VALUE1(APP_PB_SWI_ERR, MOD_APP_PB, "OnSearchPbRes: recvd AT_ERROR state %d",
                       gAppPBContext.fct.pbSearch.state );

        response = APP_COMMON_RSP_ERROR;
        finished = BT_TRUE;
    }

    if(finished)
    {
        _APP_PHONEBOOK_vSendResultReleaseContext(response);
    }
    else
    {
        (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_PHONEBOOK_FCT_PENDING, MSEC(0),0);
    }
}



void _APP_PHONEBOOK_vOnReqNumTagsResult(ATResponseEnum rsp)
{
    APP_COMMON_enResponseCode response  = APP_COMMON_RSP_ERROR;
    BooleanType               finished  = BT_FALSE;

    if(rsp == AT_RSP_OK)
    {
        switch(gAppPBContext.fct.numVoiceTags.state)
        {
            case APP_PHONEBOOK_PB_REQ_NUM_TAGS_NOT_SENT:
                    /* nothing special to do here, just set alarm (later) and go on*/
                    break;
            case APP_PHONEBOOK_PB_REQ_NUM_TAGS_SENT:
                    // command was OK, now wait for RVTA:x event
                    gAppPBContext.fct.numVoiceTags.state = APP_PHONEBOOK_PB_REQ_NUM_TAGS_WAIT_RSP;
                    DEBUG_TEXT(APP_PB_SWI_TRC, MOD_APP_PB,"OnReqNumTagsRes: recvd OK, wait rvta:x ev");
                    break;
            case APP_PHONEBOOK_PB_REQ_NUM_TAGS_WAIT_RSP:
                    // ignore, just wait, do not set an event, so return
                    DEBUG_TEXT(APP_PB_SWI_TRC, MOD_APP_PB,"OnReqNumTagsRes: wait rvta:x ev");
                    return;
            case APP_PHONEBOOK_PB_REQ_NUM_TAGS_END:
                    /* we are finished */
                    response  = APP_COMMON_RSP_OK;
                    finished  = BT_TRUE;
                    break;
            default:
                DEBUG_VALUE1(APP_PB_SWI_ERR, MOD_APP_PB, "OnReqNumTagsRes: unsup state %d",
                               gAppPBContext.fct.numVoiceTags.state);
                response = APP_COMMON_RSP_ERROR;
                finished = BT_TRUE;
                break;
        }
    }
    else
    {
        DEBUG_VALUE1(APP_PB_SWI_ERR, MOD_APP_PB, "OnReqNumTagsRes: recvd AT_ERROR state %d",
                       gAppPBContext.fct.numVoiceTags.state);

        response = APP_COMMON_RSP_ERROR;
        finished = BT_TRUE;
    }

    if(finished)
    {
        _APP_PHONEBOOK_vSendResultReleaseContext(response);
    }
    else
    {
        (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_PHONEBOOK_FCT_PENDING, MSEC(0),0);
    }
}

void _APP_PHONEBOOK_vOnAddPbEntryResult(ATResponseEnum rsp)
{
    APP_COMMON_enResponseCode response  = APP_COMMON_RSP_ERROR;
    BooleanType               finished  = BT_FALSE;

    if(rsp == AT_RSP_OK)
    {
        switch(gAppPBContext.fct.pbAddEntry.state)
        {
        case APP_PHONEBOOK_ADD_PB_ENTRY_NOT_SENT:
                /* nothing special to do here, just set alarm (later) and go on*/
                break;
        case APP_PHONEBOOK_ADD_PB_ENTRY_SENT:
                /* we are finished */
                response  = APP_COMMON_RSP_OK;
                finished  = BT_TRUE;
                break;
        default:
            DEBUG_VALUE1(APP_PB_SWI_ERR, MOD_APP_PB, "OnAddPbEntryRes: unsup state %d",
                           gAppPBContext.fct.pbAddEntry.state );
            response = APP_COMMON_RSP_ERROR;
            finished = BT_TRUE;
            break;
        }
    }
    else
    {
        DEBUG_VALUE1(APP_PB_SWI_ERR, MOD_APP_PB, "OnAddPbEntryRes: recvd AT_ERROR state %d",
                       gAppPBContext.fct.pbAddEntry.state );

        response = APP_COMMON_RSP_ERROR;
        finished = BT_TRUE;
    }

    if(finished)
    {
        _APP_PHONEBOOK_vSendResultReleaseContext(response);
    }
    else
    {
        (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_PHONEBOOK_FCT_PENDING, MSEC(0),0);
    }
}




void _APP_PHONEBOOK_vOnPbSortOrderResult(ATResponseEnum rsp)
{
    APP_COMMON_enResponseCode response  = APP_COMMON_RSP_ERROR;
    BooleanType               finished  = BT_FALSE;

    if(rsp == AT_RSP_OK)
    {
        switch(gAppPBContext.fct.pbSortOrder.state)
        {
        case APP_PHONEBOOK_PB_SORTORDER_NOT_SENT:
                /* nothing special to do here, just set alarm (later) and go on*/
                break;
        case APP_PHONEBOOK_PB_SORTORDER_SENT:
                /* we are finished */
                response  = APP_COMMON_RSP_OK;
                finished  = BT_TRUE;
                break;
        default:
            DEBUG_VALUE1(APP_PB_SWI_ERR, MOD_APP_PB, "OnPbSortOrderRes: unsup state %d",
                           gAppPBContext.fct.pbSortOrder.state );
            response = APP_COMMON_RSP_ERROR;
            finished = BT_TRUE;
            break;
        }
    }
    else
    {
        DEBUG_VALUE1(APP_PB_SWI_ERR, MOD_APP_PB, "OnPbSortOrderRes: recvd AT_ERROR state %d",
                       gAppPBContext.fct.pbSortOrder.state );

        response = APP_COMMON_RSP_ERROR;
        finished = BT_TRUE;
    }

    if(finished)
    {
        APP_PHONEBOOK_enFctId fctId = gAppPBContext.fct.common.fctId;

        // if response was ok, sorting was started. Need to notify applications
        // SUSANNE: this is for changing pb sort order, not for requesting current mode!!!
        if((APP_COMMON_RSP_OK == response) &&
            (fctId == APP_PB_enSetPbSortOrder))
        {
            if(gAppPBContext.fct.pbSortOrder.NewUserIsAdded == BT_FALSE)
            {   /* sorting order changed by the user, not triggered by ERM after profile adding
                in this case PB and call stacks have to be send to the HU after receiving PPBC:0*/
                /* implemented by nikolov, RT:11046 */
                gAppPBContext.SortingOrderChangedSentByUser = BT_TRUE;

                /* inform the HU that new phone book is being downloaded */
                _APP_PHONEBOOK_vNotifyApps(APP_PB_SORTING_STARTED);
            }
            DEBUG_TEXT(APP_PB_SWI_TRC, MOD_APP_PB, "OnPbSortOrderRes: not. sort started");

            // start alarm, if we do not receive PPBC:0 or *PPNO event within timeout
            // we need to notify end of sorting
            (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_PHONEBOOK_SORTING_TIMEOUT, MSEC(APP_PHONEBOOK_SORTING_TIMEOUT),0);

        }
        else if((APP_COMMON_RSP_ERROR == response) &&
                (fctId == APP_PB_enSetPbSortOrder) &&
                (gAppPBContext.fct.pbSortOrder.NewUserIsAdded == BT_TRUE))
        {
            if((CKCM_ERROR_CODE_PB_SYNC_IN_PROGRESS == ERM_uGetLastExtendedErrorCode()) && (gAppPBContext.dwldState == AT_PB_LOADING))
            {
                 /* retry to change the PB sorting order one more time after PB sync was completed
                 * this can happen if after new user is paired and connected and the PB sync is started immediately
                 * in this case the target will not accept the PPNO commands. The host has to wait until PB sync is
                 * completely done(e.g. PPBU:1->sync. OK or PPBU:2-> Sync. Failed)*/
                /* implemented by nikolov, RT:11046 */
                DEBUG_VALUE1(APP_PB_SWI_TRC, MOD_APP_PB, "vOnPbSortOrderResult: PPNO command failed error code: %d, retry after PB sync",
                                                          ERM_uGetLastExtendedErrorCode());

                gAppPBContext.notifyPBSortingAfterNewUserFailed = BT_TRUE;
            }
            else
            {
                /* if the PPNO command fails due to another reasons(error code not equal to #156), then just activate the old
                 * PB if already available after PB synced */
                /* implemented by nikolov, RT:11046 */
                DEBUG_VALUE1(APP_PB_SWI_TRC, MOD_APP_PB, "vOnPbSortOrderResult: PPNO command failed, error code: %d, activate the old PB",
                                                        ERM_uGetLastExtendedErrorCode());
                /* activate the old PB if available*/
                if (gAppPBContext.dwldState != AT_PB_LOADING)
                {
                    (void)_APP_PHONEBOOK_sfUpdatePbSizes();
                    _APP_PHONEBOOK_vNotifyApps(APP_PB_DOWNLOAD_STATE_CHANGED);
                }
            }
        }

        // will reset fctId, but we need it for a check afterwards
         _APP_PHONEBOOK_vSendResultReleaseContext(response);
    }
    else
    {
        (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_PHONEBOOK_FCT_PENDING, MSEC(0),0);
    }
}




void _APP_PHONEBOOK_vOnReadCompletePbResult(ATResponseEnum rsp)
{
    APP_COMMON_enResponseCode response  = APP_COMMON_RSP_ERROR;
    BooleanType               finished  = BT_FALSE;

    if(rsp == AT_RSP_OK)
    {
        switch(gAppPBContext.fct.pbRead.state)
        {
        case APP_PHONEBOOK_SELECT_PB_SENT:
                /* nothing special to do here, just set alarm (later) and go on*/
                break;
        case APP_PHONEBOOK_READ_PB_ENTRIES_SENT:
        case APP_PHONEBOOK_READ_PB_NUMBERS_SENT:
        {   // check if one of the pbEntries has more than one phone number, so we need to request
            // all the numbers from Parrot with an extra AT request
            Sint32Type i = 0;
            Sint32Type j = 0;

            for(i=0; i<(Sint32Type)MAX_REQUESTED_PB_ENTRIES; i++)
            {
                if(gAppPBContext.fct.pbRead.entry[i].isValid)
                {
                    Uint16Type pbIndex;
                    Uint16Type pnSize = gAppPBContext.fct.pbRead.entry[i].ppbr.pn_size;
                    // is pn_size > 1 so we need to request all phone numbers?
                    if( pnSize > 1U)
                    {
                        // check if we already requested those numbers:
                        BooleanType bAlreadyRequested = BT_FALSE;
                        for(j=0;j<(Sint32Type)MAX_REQUESTED_PB_NUMBERS_PER_ENTRY;j++)
                        {
                            if(gAppPBContext.fct.pbRead.entry[i].number[j].isValid)
                            {
                                bAlreadyRequested = BT_TRUE;
                                break;
                            }
                        }
                        if(BT_TRUE == bAlreadyRequested)
                        {
                            continue;
                        }

                        /* we found an entry with additional numbers, so request
                         * it */
                        pbIndex = gAppPBContext.fct.pbRead.entry[i].ppbr.pb_idx;
                        gAppPBContext.fct.pbRead.pbIndex    = pbIndex;
                        gAppPBContext.fct.pbRead.pnSize     = pnSize;
                        break;
                    }
                    else if( pnSize == 1U )
                    {
                        // copy info into ppbp struct
                        Uint16Type len               = 0U;
                        ATRspPBReadNumberType * ppbp = &gAppPBContext.fct.pbRead.entry[i].number[0].ppbp;

                        ppbp->pb_idx            = gAppPBContext.fct.pbRead.entry[i].ppbr.pb_idx;
                        ppbp->pn_idx            = 1U;
                        ppbp->pn_type           = gAppPBContext.fct.pbRead.entry[i].ppbr.pn_type;
                        ppbp->phone_number_len  = gAppPBContext.fct.pbRead.entry[i].ppbr.phone_number_len;

                        len = ( ppbp->phone_number_len > sizeof(ppbp->phone_number)) ?
                                sizeof(ppbp->phone_number) : ppbp->phone_number_len;
                        (void)memcpy(ppbp->phone_number, gAppPBContext.fct.pbRead.entry[i].ppbr.phone_number, len);

                        gAppPBContext.fct.pbRead.entry[i].number[0].isValid = BT_TRUE;
                    }
					else
                    {
                    }

                }
            }

            if(i == MAX_REQUESTED_PB_ENTRIES)
            {
                /* we are finished */
                response  = APP_COMMON_RSP_OK;
                finished  = BT_TRUE;
            }
            else
            {
                /* set the state back to request the additional numbers */
                gAppPBContext.fct.pbRead.state = APP_PHONEBOOK_READ_PB_ENTRIES_SENT;
            }
            break;
        }
        default:
            DEBUG_VALUE1(APP_PB_SWI_ERR, MOD_APP_PB, "OnReadCompletePbRes: unsup state %d",
                           gAppPBContext.fct.pbRead.state );
            response = APP_COMMON_RSP_ERROR;
            finished = BT_TRUE;
            break;
        }
    }
    else
    {
        DEBUG_VALUE1(APP_PB_SWI_ERR, MOD_APP_PB, "OnReadCompletePbRes: recvd AT_ERROR state %d",
                       gAppPBContext.fct.pbRead.state );

        response = APP_COMMON_RSP_ERROR;
        finished = BT_TRUE;
    }


    if(finished)
    {
        _APP_PHONEBOOK_vSendResultReleaseContext(response);
    }
    else
    {
        (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_PHONEBOOK_FCT_PENDING, MSEC(0),0);
    }

}


void _APP_PHONEBOOK_vOnActivatePpdsResult(ATResponseEnum rsp)
{
    APP_COMMON_enResponseCode response  = APP_COMMON_RSP_ERROR;
    BooleanType               finished  = BT_FALSE;

    if(rsp == AT_RSP_OK)
    {
        switch(gAppPBContext.fct.activatePpds.state)
        {
        case APP_PHONEBOOK_ACTIVATE_PPDS_SENT:
                /* we are finished */
                response  = APP_COMMON_RSP_OK;
                finished  = BT_TRUE;
                break;
        default:
            DEBUG_VALUE1(APP_PB_SWI_ERR, MOD_APP_PB, "OnActPpdsResult: unsup state %d",
                           gAppPBContext.fct.activatePpds.state );
            response = APP_COMMON_RSP_ERROR;
            finished = BT_TRUE;
            break;
        }
    }
    else
    {
        DEBUG_VALUE1(APP_PB_SWI_ERR,MOD_APP_PB, "vOnActPpdsRes: recvd AT_ERROR state %d",
                       gAppPBContext.fct.activatePpds.state );

        response = APP_COMMON_RSP_ERROR;
        finished = BT_TRUE;
    }

    if(finished)
    {
        _APP_PHONEBOOK_vSendResultReleaseContext(response);
    }
    else
    {
        (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_PHONEBOOK_FCT_PENDING, MSEC(0),0);
    }
}

// ***************  INTERNAL INTERFACES ************************************* //

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_sfUpdatePbSizes                            */
/**
    \brief      Select the next step to be done depending on the current \n
                state updating the sizes of the prior refreshed phonebooks.

    \brief      Description:\n
                Select the next step to be done depending on the current \n
                state updating the sizes of the prior refreshed phonebooks. \n

    \return     SuccessFailType
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_PHONEBOOK_sfUpdatePbSizes(void)
{
    const ERMUserProfileType * user = ERM_pGetActiveUserProfile();

    if( gAppPBContext.fct.common.fctId != APP_PB_enInvalidFct)
    {
        /* queue request */
        gAppPBContext.pending.pbSizeUpdate = BT_TRUE;

        return FAIL;
    }

    // SUSANNE: 06.12.2009 #4055
    // If the user phonebook has not been changed, we will not mark it as invalid
    // cause otherwise HU might get a BAP error and will then stop asking for the phonebook
    // and as the pb has not been changed, we will sent no ChangedArray etc. to HU to make
    // her downloading the pb again.
    // BUT: In some cases, we need to set it to invalid, so we cannot remove the flag at all

    // In these cases we need to mark PB as invalid, cause we have to notify that a new PB is available
    // when we receive the sizes
    if((gAppPBContext.updatedPbIndicator & APP_PB_USERS_PERSONAL_REFRESHED)         ||      // new pb after snyc
       (BT_TRUE == gAppPBContext.bWasManualSync)                                    ||      // manual sync
       ((NULL != user) && (BT_FALSE == user->bPbLoadedCompletly)) )                         // first load at all
    {
        gAppPBContext.usersPersonalPb.bIsValid  = BT_FALSE;
    }
    gAppPBContext.missedCallsPb.bIsValid    = BT_FALSE;
    gAppPBContext.receivedCallsPb.bIsValid  = BT_FALSE;
    gAppPBContext.dialledNumbersPb.bIsValid = BT_FALSE;

    gAppPBContext.fct.common.fctId          = APP_PB_enUpdatePbSizes;
    gAppPBContext.fct.common.callback       = NULL;

    gAppPBContext.fct.updatePbSizes.state   = APP_PHONEBOOK_UPDATE_NOT_SENT;

    (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_PHONEBOOK_FCT_PENDING, MSEC(0),0);

    return SUCCESS;
}

SuccessFailType _APP_PHONEBOOK_sfUpdateSinglePbSize(APP_PHONEBOOK_enSelectedPbType pbType)
{
    if( gAppPBContext.fct.common.fctId != APP_PB_enInvalidFct)
    {
        /* queue request */
        switch(pbType)
        {
            case APP_PHONEBOOK_enLastDialed:
                gAppPBContext.pending.updDialedStackSize = BT_TRUE;
                break;
            case APP_PHONEBOOK_enReceivedCalls:
                gAppPBContext.pending.updReceivedStackSize = BT_TRUE;
                break;
            case APP_PHONEBOOK_enMissedCalls:
                gAppPBContext.pending.updMissedStackSize = BT_TRUE;
                break;
            case APP_PHONEBOOK_enUsersPB:
                gAppPBContext.pending.updPbSize = BT_TRUE;
                break;
            default:
                return FAIL;
        }

        DEBUG_VALUE1(APP_PB_SWI_TRC,MOD_APP_PB, "UpdSglPbSiz: queued for type %d", pbType);
        return FAIL;
    }

    // application is currently not busy, so update call stack size immediately
    switch(pbType)
    {
        case APP_PHONEBOOK_enMissedCalls:
            gAppPBContext.missedCallsPb.bIsValid    = BT_FALSE;
            break;
        case APP_PHONEBOOK_enReceivedCalls:
            gAppPBContext.receivedCallsPb.bIsValid  = BT_FALSE;
            break;
        case APP_PHONEBOOK_enLastDialed:
            gAppPBContext.dialledNumbersPb.bIsValid = BT_FALSE;
            break;
        case APP_PHONEBOOK_enUsersPB:
        {
            // SUSANNE: 06.12.2009 #4055
            const ERMUserProfileType * user = ERM_pGetActiveUserProfile();
            if((gAppPBContext.updatedPbIndicator & APP_PB_USERS_PERSONAL_REFRESHED)         ||
               (BT_TRUE == gAppPBContext.bWasManualSync)                                    ||
               ((NULL != user) && (BT_FALSE == user->bPbLoadedCompletly)) )
            {
                gAppPBContext.usersPersonalPb.bIsValid = BT_FALSE;
            }
        }
        break;
        default:
            DEBUG_VALUE1(APP_PB_SWI_ERR, MOD_APP_PB, "UpdSglPbSiz: inv. type %d", pbType);
            return FAIL;
    }

    gAppPBContext.fct.common.fctId          = APP_PB_enUpdateSinglePbSize;
    gAppPBContext.fct.common.callback       = NULL;

    gAppPBContext.fct.updSinglePbSize.state   = APP_PHONEBOOK_UPDATE_PB_SIZE_NOT_SENT;
    gAppPBContext.fct.updSinglePbSize.pbType  = pbType;

    (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_PHONEBOOK_FCT_PENDING, MSEC(0),0);

    return SUCCESS;

}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_sfActivatePpds                                */
/**
    \brief

    \brief      Description:\n

    \param      enable
                BooleanType

    \return     SuccessFailType
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_PHONEBOOK_sfActivatePpds(BooleanType enable)
{
    if( gAppPBContext.fct.common.fctId != APP_PB_enInvalidFct)
    {
        /* queue request */
        if(enable == BT_TRUE)
        {
            gAppPBContext.pending.activatePpds = BT_TRUE;
        }
        else
        {
            /* gAppPBContext.pending.deactivatePpds = BT_TRUE; */
        }

        return FAIL;
    }

    gAppPBContext.fct.common.fctId          = APP_PB_enActivatePpds;
    gAppPBContext.fct.common.callback       = NULL;

    gAppPBContext.fct.activatePpds.state    = APP_PHONEBOOK_ACTIVATE_PPDS_NOT_SENT;
    gAppPBContext.fct.activatePpds.enable   = enable;

    (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_PHONEBOOK_FCT_PENDING, MSEC(0),0);

    return SUCCESS;
}


/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONEBOOK_vOnAppDeviceNotification                                 */
/**
    \brief      handle Notifications of APP Device

    \brief      Description:\n
                handle Notifications of App Device \n

    \return     void
*/
/*----------------------------------------------------------------------------*/
void APP_PHONEBOOK_vOnAppDeviceNotification(APP_DEVICE_enNotificationId notification)
{
    switch(notification)
    {
        case APP_DEVICE_NOTIFY_USER_PROFILE_ADDED:
            /* check if pb has already been loaded completely */
            _APP_PHONEBOOK_vUpdatePbCompletlyLoadedForActiveUser();
            break;
        default:
            /* ignore notification */
            break;
    }
}


void _APP_PHONEBOOK_vNotifyApps(APP_PB_enNotificationId event)
{
   APP_PHONEBOOK_BAP_vOnAppPBNotification(event);			// todo: missing calls changed not assisgned to SD to never get the missing call Popup on SD
   APP_SD_vOnAppPhoneBookNotification(event);				// todo: missing calls changed not assisgned to SD to never get the missing call Popup on SD
   ERM_vOnPhonebookNotification(event);
   APP_DEVICE_vOnAppPhoneBookNotification(event);
   // APP_VCR_vOnAppPBNotification(event);
}


// ***************  INTERFACES FOR APPS ************************************* //
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
                                ATPhoneNumberTypeEnum pnType)
{
    if(APP_PB_enInvalidFct != gAppPBContext.fct.common.fctId)
    {
        return APP_COMMON_RSP_ERROR_BUSY;
    }

    // set callBack for OK / ERROR response
    gAppPBContext.fct.common.fctId      = APP_PB_enAddPbEntry;
    gAppPBContext.fct.common.callback   = cb;
    gAppPBContext.fct.pbAddEntry.state  = APP_PHONEBOOK_ADD_PB_ENTRY_NOT_SENT;

    // copy params
    // copy name
    gAppPBContext.fct.pbAddEntry.name_len = (name_len > sizeof(AT_CallerId))?
                                            sizeof(AT_CallerId) : name_len;
    (void)memcpy( gAppPBContext.fct.pbAddEntry.name, name,
            gAppPBContext.fct.pbAddEntry.name_len);

    // copy number
    gAppPBContext.fct.pbAddEntry.number_len = (number_len > sizeof(ATPhoneNumberType))?
                                              sizeof(ATPhoneNumberType) : number_len;
    (void)memcpy( gAppPBContext.fct.pbAddEntry.number, number,
            gAppPBContext.fct.pbAddEntry.number_len);

    // copy pn Type
    gAppPBContext.fct.pbAddEntry.pnType = pnType;


    // set alarm
    (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_PHONEBOOK_FCT_PENDING,MSEC(0),0);

    return APP_COMMON_RSP_OK;
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONEBOOK_rcRequestManualSync                            */
/**
    \brief      requests manual PB sync via PPMS / AT_REQ_PB_START_MANUAL_SYNC

    \return     APP_COMMON_enResponseCode
                APP_COMMON_RSP_OK or APP_COMMON_RSP_ERROR
*/
/*----------------------------------------------------------------------------*/

APP_COMMON_enResponseCode APP_PHONEBOOK_rcRequestManualSync(
        void (*cb)(APP_COMMON_enResponseCode))
{
    if(APP_PB_enInvalidFct != gAppPBContext.fct.common.fctId)
    {
        return APP_COMMON_RSP_ERROR_BUSY;
    }

    // set callBack for OK / ERROR response
    gAppPBContext.fct.common.fctId      = APP_PB_enManualPbSync;
    gAppPBContext.fct.common.callback   = cb;
    gAppPBContext.fct.pbSync.state      = APP_PHONEBOOK_PB_SYNC_NOT_SENT;

    // set alarm
    (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_PHONEBOOK_FCT_PENDING,MSEC(0),0);

    gAppPBContext.bWasManualSync        = BT_TRUE;

//		pbDwldEntry = 0U;

    return APP_COMMON_RSP_OK;
}
/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONEBOOK_rcRequestCallStackReset                        */
/**
    \brief      requests deletion of call stacks ( missed , dialed, received )

    \return     APP_COMMON_enResponseCode
                APP_COMMON_RSP_OK or APP_COMMON_RSP_ERROR
*/
/*----------------------------------------------------------------------------*/

APP_COMMON_enResponseCode APP_PHONEBOOK_rcRequestCallStackReset(
        void (*cb)(APP_COMMON_enResponseCode))
{
    if(APP_PB_enInvalidFct != gAppPBContext.fct.common.fctId)
    {
        return APP_COMMON_RSP_ERROR_BUSY;
    }

    // set callBack for OK / ERROR response
    gAppPBContext.fct.common.fctId          = APP_PB_enDeleteCallStacks;
    gAppPBContext.fct.common.callback       = cb;
    gAppPBContext.fct.pbCallStackRst.state  = APP_PHONEBOOK_PB_CALL_STACK_RESET_NOT_SENT;

    // set alarm
    (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_PHONEBOOK_FCT_PENDING,MSEC(0),0);

    return APP_COMMON_RSP_OK;
}
/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONEBOOK_rcRequestCompletePbEntries          */
/**
    \brief      requests pb entries from pb type

    \brief      Description:\n
                requests number of complete phonebook entries using start as
                first index. one entry can contain up to 5 numbers \n

    \param      cb
                void *

    \param      pbType
                APP_PHONEBOOK_enSelectedPbType

    \param      startIndex
                Uint16Type

    \param      elements
                Uint16Type

    \return     APP_COMMON_enResponseCode
                APP_COMMON_RSP_OK or APP_COMMON_RSP_ERROR
*/
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode APP_PHONEBOOK_rcRequestCompletePbEntries(
                                        void (*cb)(APP_COMMON_enResponseCode),
                                        APP_PHONEBOOK_enSelectedPbType pbType,
                                        Uint16Type startIndex,
                                        Uint16Type elements)
{
    Uint16Type maxPbSize = 0U;

    if(APP_PB_enInvalidFct != gAppPBContext.fct.common.fctId)
    {
        return APP_COMMON_RSP_ERROR_BUSY;
    }

    if(NULL == ERM_pGetActiveUserProfile())
    {
        DEBUG_TEXT(APP_PB_SWI_ERR, MOD_APP_PB, "ReqComplPbEntr: no user!");
        return APP_COMMON_RSP_ERROR;
    }

    if(elements == 0U)
    {
        DEBUG_VALUE2(APP_PB_SWI_ERR, MOD_APP_PB, "Read_Req: req invalid elments %d (start %d)",
                      elements, startIndex);

        return APP_COMMON_RSP_ERROR;
    }

    // request entries as required by BAP indication
    // first check if elements is not mor than MAX_REQUESTED_PB_ENTRIES
    if(elements > MAX_REQUESTED_PB_ENTRIES)
    {
        elements = MAX_REQUESTED_PB_ENTRIES;
    }

    DEBUG_VALUE2(APP_PB_SWI_TRC, MOD_APP_PB, "Read_Req: start %d, elems %d",
                 startIndex, elements);

    // set callBack for OK / ERROR response
    gAppPBContext.fct.common.fctId      = APP_PB_enReadCompletePbEntries;
    gAppPBContext.fct.common.callback   = cb;
    gAppPBContext.fct.pbRead.state      = APP_PHONEBOOK_PB_READ_NOT_SENT;
    // copy params
    gAppPBContext.fct.pbRead.pbType     = pbType;
    gAppPBContext.fct.pbRead.start      = startIndex;
    gAppPBContext.fct.pbRead.end        = startIndex + elements - 1U;
    gAppPBContext.fct.pbRead.pbIndex    = 0U;
    gAppPBContext.fct.pbRead.pnSize     = 0U;

    switch(pbType)
    {
        case APP_PHONEBOOK_enUsersPB:
            maxPbSize = gAppPBContext.usersPersonalPb.pbSize;
            break;
        case APP_PHONEBOOK_enLastDialed:
            maxPbSize = gAppPBContext.dialledNumbersPb.pbSize;
            break;
        case APP_PHONEBOOK_enReceivedCalls:
            maxPbSize = gAppPBContext.receivedCallsPb.pbSize;
            break;
        case APP_PHONEBOOK_enMissedCalls:
            maxPbSize = gAppPBContext.missedCallsPb.pbSize;
            break;
    }

    if(gAppPBContext.fct.pbRead.start > maxPbSize)
    {
        gAppPBContext.fct.pbRead.start = maxPbSize;
    }

    if(gAppPBContext.fct.pbRead.end > maxPbSize)
    {
        gAppPBContext.fct.pbRead.end = maxPbSize;
    }

    DEBUG_VALUE2(APP_PB_SWI_TRC, MOD_APP_PB, "Read_Req: start:%d, end:%d",
                 gAppPBContext.fct.pbRead.start, gAppPBContext.fct.pbRead.end );

    // set alarm
    (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_PHONEBOOK_FCT_PENDING,MSEC(0),0);

    //result = _APP_PHONEBOOK_AT_PPBR_sfHandleEntriesRequest(puart);
    return APP_COMMON_RSP_OK;
}

APP_COMMON_enResponseCode APP_PHONEBOOK_rcRequestPbSearch(
                            void (*cb)(APP_COMMON_enResponseCode),
                            const Uint8Type *pStr,
                            Uint16Type strLen)
{
    Uint16Type copyLen = 0U;

    if(APP_PB_enInvalidFct != gAppPBContext.fct.common.fctId)
    {
        return APP_COMMON_RSP_ERROR_BUSY;
    }

    // set callBack for OK / ERROR response
    gAppPBContext.fct.common.fctId          = APP_PB_enSearchPb;
    gAppPBContext.fct.common.callback       = cb;
    gAppPBContext.fct.pbSearch.state        = APP_PHONEBOOK_EXTENDED_PB_SEARCH_NOT_SENT;
    // copy params
    copyLen = MAX_PB_SEARCH_STRING < strLen ? MAX_PB_SEARCH_STRING : strLen;
    (void)memcpy(gAppPBContext.fct.pbSearch.searchStr, pStr, copyLen);
    gAppPBContext.fct.pbSearch.searchStrLen = copyLen;

    // set alarm
    (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_PHONEBOOK_FCT_PENDING,MSEC(0),0);

    //result = _APP_PHONEBOOK_AT_PPBR_sfHandleEntriesRequest(puart);
    return APP_COMMON_RSP_OK;
}


//APP_COMMON_enResponseCode APP_PHONEBOOK_rcSearchForVoicemailboxNumber(
//                            void (*cb)(APP_COMMON_enResponseCode),
//                            APP_SD_ENUM_LANGUAGES language)
//{
//    if(APP_PB_enInvalidFct != gAppPBContext.fct.common.fctId)
//    {
//        return APP_COMMON_RSP_ERROR_BUSY;
//    }
//
//    // set callBack for OK / ERROR response
//    gAppPBContext.fct.common.fctId              = APP_PB_enSearchVoiceMailbox;
//    gAppPBContext.fct.common.callback           = cb;
//    gAppPBContext.fct.voiceMailboxSearch.state  = APP_PHONEBOOK_EXTENDED_PB_SEARCH_NOT_SENT;
//
//    /* check if language is guilty */
//
//    // copy params
//    gAppPBContext.fct.voiceMailboxSearch.lang   = language;
//
//    // set alarm
//    APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_PHONEBOOK_FCT_PENDING,MSEC(0),0);
//    return APP_COMMON_RSP_OK;
//}

APP_COMMON_enResponseCode APP_PHONEBOOK_rcRequestCurrentPbSortOrder(
                            void (*cb)(APP_COMMON_enResponseCode))
{
    if(APP_PB_enInvalidFct != gAppPBContext.fct.common.fctId)
    {
        return APP_COMMON_RSP_ERROR_BUSY;
    }

    // set callBack for OK / ERROR response
    gAppPBContext.fct.common.fctId          = APP_PB_enGetPbSortOrder;
    gAppPBContext.fct.common.callback       = cb;
    gAppPBContext.fct.pbSortOrder.state     = APP_PHONEBOOK_PB_SORTORDER_NOT_SENT;

    // mode is get
    gAppPBContext.fct.pbSortOrder.mode      = AT_PB_SORTORDER_GET;

    // set alarm
    (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_PHONEBOOK_FCT_PENDING,MSEC(0),0);

    return APP_COMMON_RSP_OK;
}

APP_COMMON_enResponseCode APP_PHONEBOOK_rcSetPbSortOrder(
                            void (*cb)(APP_COMMON_enResponseCode),
                            Uint8Type mode,
                            BooleanType IsPBOrderChangedBeforeProfileAdded)
{
    if(APP_PB_enInvalidFct != gAppPBContext.fct.common.fctId)
    {
        return APP_COMMON_RSP_ERROR_BUSY;
    }

    // set callBack for OK / ERROR response
    gAppPBContext.fct.common.fctId          = APP_PB_enSetPbSortOrder;
    gAppPBContext.fct.common.callback       = cb;
    gAppPBContext.fct.pbSortOrder.state     = APP_PHONEBOOK_PB_SORTORDER_NOT_SENT;

    // save mode
    gAppPBContext.fct.pbSortOrder.mode   = mode;

    gAppPBContext.fct.pbSortOrder.NewUserIsAdded = IsPBOrderChangedBeforeProfileAdded;

    gAppPBContext.notifyPBSortingAfterNewUserFailed = BT_FALSE;

    // set alarm
    (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_PHONEBOOK_FCT_PENDING,MSEC(0),0);

    return APP_COMMON_RSP_OK;
}


APP_COMMON_enResponseCode APP_PHONEBOOK_rcRequestNumVoiceTagsForContact(
                            void (*cb)(APP_COMMON_enResponseCode),
                            Uint16Type pbIndex)
{
    if(APP_PB_enInvalidFct != gAppPBContext.fct.common.fctId)
    {
        return APP_COMMON_RSP_ERROR_BUSY;
    }

    // check pbIndex
    if(0 == pbIndex ||
       pbIndex > APP_PHONEBOOK_u16GetCurrentPBSize(APP_PHONEBOOK_enUsersPB))
    {
        return APP_COMMON_RSP_ERROR;
    }

    // set callBack for OK / ERROR response
    gAppPBContext.fct.common.fctId          = APP_PB_enRequestNumVoiceTagsForContact;
    gAppPBContext.fct.common.callback       = cb;
    gAppPBContext.fct.numVoiceTags.state    = APP_PHONEBOOK_PB_REQ_NUM_TAGS_NOT_SENT;

    // save pbIndex
    gAppPBContext.fct.numVoiceTags.pbIndex  = pbIndex;

    // set alarm
    (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_PHONEBOOK_FCT_PENDING,MSEC(0),0);

    return APP_COMMON_RSP_OK;
}

Uint16Type APP_PHONEBOOK_u16GetCurrentPBSize(APP_PHONEBOOK_enSelectedPbType pbType)
{
    Uint16Type size = 0U;

    switch( pbType )
    {
        case APP_PHONEBOOK_enUsersPB:
            size = gAppPBContext.usersPersonalPb.pbSize;
            break;
        case APP_PHONEBOOK_enLastDialed:
            size = gAppPBContext.dialledNumbersPb.pbSize;
            break;
        case APP_PHONEBOOK_enReceivedCalls:
            size = gAppPBContext.receivedCallsPb.pbSize;
            break;
        case APP_PHONEBOOK_enMissedCalls:
            size = gAppPBContext.missedCallsPb.pbSize;
            break;
    }
    return size;
}

ATPhoneBookDwnlState APP_PHONEBOOK_enGetPBDwldState()
{
    DEBUG_VALUE1(APP_PB_SWI_TRC, MOD_APP_PB, "ATPbDwldState: %d", gAppPBContext.dwldState);
    return gAppPBContext.dwldState;
}

BooleanType APP_PHONEBOOK_bIsPbValid(APP_PHONEBOOK_enSelectedPbType pbType)
{
    switch( pbType )
    {
        case APP_PHONEBOOK_enUsersPB:
            return gAppPBContext.usersPersonalPb.bIsValid;
        case APP_PHONEBOOK_enLastDialed:
            return gAppPBContext.dialledNumbersPb.bIsValid;
        case APP_PHONEBOOK_enReceivedCalls:
            return gAppPBContext.receivedCallsPb.bIsValid;
        case APP_PHONEBOOK_enMissedCalls:
            return gAppPBContext.missedCallsPb.bIsValid;
    }
    return BT_FALSE;
}


/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONEBOOK_pGetPpbrContext                          */
/**
    \brief      returns pointer to current ppbr context

    \brief      Description:\n
                returns pointer to current ppbr context

    \param      index
                Uint16Type

    \return     const AppPbEntryType *

*/
/*----------------------------------------------------------------------------*/
const AppPbEntryType *APP_PHONEBOOK_pGetPpbrContext(Uint16Type index)
{
    if( (gAppPBContext.fct.common.fctId == APP_PB_enReadCompletePbEntries) &&
        (index < MAX_REQUESTED_PB_ENTRIES) )
    {
        return &gAppPBContext.fct.pbRead.entry[index];
    }
    else
    {
        return NULL;
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONEBOOK_GetNumVoiceTags                          */
/**
    \brief      returns number of voice tags

    \brief      Description:\n
                returns number of voice tags

    \return     index
                Uint16Type
*/
/*----------------------------------------------------------------------------*/
Uint16Type APP_PHONEBOOK_GetVoiceTagNumberResponse()
{
    if(gAppPBContext.fct.common.fctId == APP_PB_enRequestNumVoiceTagsForContact)
    {
        return gAppPBContext.fct.numVoiceTags.num;
    }

    DEBUG_VALUE1(APP_PB_SWI_ERR, MOD_APP_PB, "GetNumTags: inc. fctId %d",
                 gAppPBContext.fct.common.fctId);
    return 0;
}


/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONEBOOK_u8GetSortOrder                          */
/**
    \brief      returns current Sortorder

    \brief      Description:\n
                returns current sort order

    \return     Uint8Type

*/
/*----------------------------------------------------------------------------*/
Uint8Type APP_PHONEBOOK_u8GetSortOrder(void)
{
    if(gAppPBContext.fct.common.fctId == APP_PB_enGetPbSortOrder)
    {
        return gAppPBContext.fct.pbSortOrder.mode;
    }

    return 0U;
}


/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONEBOOK_u16GetAddedPbIndex                             */
/**
    \brief      returns newly added pbIndex

    \brief      Description:\n
                returns newly added pbIndex

    \return     Uint16Type

*/
/*----------------------------------------------------------------------------*/
Uint16Type APP_PHONEBOOK_u16GetAddedPbIndex(void)
{
    if(gAppPBContext.fct.common.fctId == APP_PB_enAddPbEntry)
    {
        return gAppPBContext.fct.pbAddEntry.newIndex;
    }

    return 0xFFFFU;  // invalid
}



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
                                                 Uint16Type *pNumber)
{
    Uint16Type pbSize = 0U;
    Uint16Type index1 = 0U;
    Uint16Type number = 0U;

    // init params
    *pIndex = 0U;
    *pNumber = 0U;

    if(gAppPBContext.fct.common.fctId != APP_PB_enSearchPb)
    {
        DEBUG_VALUE1(APP_PB_SWI_ERR, MOD_APP_PB, "GetPbSearchResult: invalid fctId %d",
                        gAppPBContext.fct.common.fctId);
        return FAIL;
    }

    // get pb Size
    pbSize = gAppPBContext.usersPersonalPb.pbSize;
    index1 = gAppPBContext.fct.pbSearch.firstIndex;
    number = gAppPBContext.fct.pbSearch.numMatchingEntries;

    // check result, reduce number by one since index1 is count as one
    if( (index1 + number - 1U) > pbSize )
    {
        DEBUG_VALUE2(APP_PB_SWI_ERR, MOD_APP_PB, "GetPbSearchResult: ooR (%d+%d) greater than ...",
                        index1, number);
        DEBUG_VALUE1(APP_PB_SWI_ERR, MOD_APP_PB, "GetPbSearchResult: ooR ... pbSize %d",
                        pbSize);
        return FAIL;
    }


    // copy params
    *pIndex = index1;
    *pNumber = number;

    return SUCCESS;
}


/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONEBOOK_u16GetPbDlStatus                               */
/**
    \brief

    \brief      Description:\n

    \return     Uint16Type
*/
/*----------------------------------------------------------------------------*/
Uint16Type APP_PHONEBOOK_u16GetPbDlStatus(void)
{
    return gAppPBContext.dwldCount;
//    return pbDwldEntry;
}

BooleanType APP_PHONEBOOK_bWasManualSync()
{
    return gAppPBContext.bWasManualSync;
}


BooleanType APP_PHONEBOOK_bPbUpdateInProgress()
{
    // pb update is in progress, if dwldState is LOADING or
    // current fctId is UpdatePbSizes or
    // sorting is in progress
    if((AT_PB_LOADING           == gAppPBContext.dwldState)       ||          // sync
        APP_PB_enUpdatePbSizes  == gAppPBContext.fct.common.fctId )
    {
        return BT_TRUE;
    }
    return BT_FALSE;
}

/* trigegred by CTA_APP_PHONEBOOK_SORTING_TIMEOUT */
ALARMCALLBACK(APP_PHONEBOOK_cbPbSortingTimeout)
{
    // set pending to notify apps about sorting timeout
    gAppPBContext.pending.notifyPbSortingTimout = BT_TRUE;
    // set alarm
    (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_PHONEBOOK_FCT_PENDING,
                                             MSEC(0), 0);
}

/* End Of File APP_PHONEBOOK_C1.c */


