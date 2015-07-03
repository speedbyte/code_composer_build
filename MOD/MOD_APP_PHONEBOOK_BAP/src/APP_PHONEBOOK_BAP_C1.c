/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     APP_PHONEBOOK_BAP_C1.c
*-----------------------------------------------------------------------------
* Module Name:    BAP Phonebook Application
*-----------------------------------------------------------------------------
* Description:    Handles all phonebook specific requests from BAP
*-----------------------------------------------------------------------------
* $Date: 2011-10-18 10:30:40 +0200 (Di, 18 Okt 2011) $
* $Rev: 22438 $
* $Author: maennel $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_PHONEBOOK_BAP/trunk/src/APP_PHONEBOOK_BAP_C1.c $
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
#include "bap_defines.h"    // vw

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "APP_COMMON_Utils.h"
#include "APP_COMMON_BAP_ARRAY.h"
#include "BAP_Dispatcher.h"
#include "APP_PHONEBOOK.h"
#include "APP_PHONEBOOK_AT.h"
#include "APP_SMS.h"
#include "ATCmdHandler.h"
#include "UART.h"
#include "ERM.h"
#include "POOL.h"

#include "APP_PHONEBOOK_BAP.h"
#include "APP_PHONEBOOK_BAP_CI.h"
#include "APP_PHONEBOOK_BAP_REQUEST.h"
#include "APP_PHONEBOOK_BAP_ERM.h"

#include "APP_PHONEBOOK_BAP_C1.id"
#include "TRACE.h"
/*----------------------------------------------------------------------------*/
/* pragma Directives                                                          */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Definitions (avoid as much as possible!)                  */
/*----------------------------------------------------------------------------*/

/**
\brief	variable to save the current call management function currently in progress
*/
AppPhoneBookBapContext gAppPBBapContext;
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

static void
_APP_PHONEBOOK_BAP_vHandleBAPIndication(const BapIndication *bapInd);

static APP_COMMON_enResponseCode
_APP_PHONEBOOK_rcHandlePbSpeller(const BapIndication *bap);

static APP_COMMON_enResponseCode
_APP_PHONEBOOK_sfHandleMissedCallIndication(const BapIndication *bap);

static APP_COMMON_enResponseCode
_APP_PHONEBOOK_sfHandleCallArrays(const BapIndication *bap);

static APP_COMMON_enResponseCode
_APP_PHONEBOOK_sfHandlePhonebook(const BapIndication* bapInd);

static APP_COMMON_enResponseCode
_APP_PHONEBOOK_sfHandlePbState(const BapIndication *bap);

static APP_COMMON_enResponseCode
_APP_PHONEBOOK_sfHandleVoiceMailBox(const BapIndication *bap);

static APP_COMMON_enResponseCode
_APP_PHONEBOOK_sfHandleSmsSend(const BapIndication * bapInd);

static APP_COMMON_enResponseCode
_APP_PHONEBOOK_sfHandleSMSState(const BapIndication *bap);

static APP_COMMON_enResponseCode
_APP_PHONEBOOK_sfHandleReceivedSms(const BapIndication * bapInd);

static APP_COMMON_enResponseCode
_APP_PHONEBOOK_rcHandlePbStartDownload(const BapIndication * bapInd);

static BooleanType
_APP_PHONEBOOK_bIsDeleteRequest(const BapArrayHeaderType *pRecvHeader,
                                const Uint8Type * pData);

static BooleanType
_APP_PHONEBOOK_bIsMarkAsReadRequest(const BapArrayHeaderType *pRecvHeader,
                                    const Uint8Type * pData);


static SuccessFailType _APP_PHONEBOOK_BAP_sfHandlePendingPbActivation(void);
static SuccessFailType _APP_PHONEBOOK_BAP_sfHandlePendingPbIncompleteActivation(void);
static SuccessFailType _APP_PHONEBOOK_BAP_sfHandlePendingOldPbActivation(void);

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
//ATResponseAllType   atRsp;

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
/* Function    : APP_PHONEBOOK_BAP_sfInitApp                                  */
/**
    \brief      initialization of App Phonebook BAP

    \brief      Description:\n

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType APP_PHONEBOOK_BAP_sfInitApp(void)
{
    (void)memset(&gAppPBBapContext, 0, sizeof(gAppPBBapContext));

    return SUCCESS;
}




/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONEBOOK_BAP_vHandleEvents                              */
/**
    \brief      Task Function to handle events for Phonebook Application BAP

    \brief      Description:\n
                Waits for incoming events and if one arrives function \n
                reads Messages received From BAP-Dispatcher\n

    \param      event
                EventMaskType
                Bitmask containing all unhandled events

    \return     void
*/
/*----------------------------------------------------------------------------*/

void APP_PHONEBOOK_BAP_vHandleEvents(EventMaskType event)
{
    // wait for interesting events

    // ****************************************************
    // check if we have some additional things to do
    // from the last callFctId triggered by BAP
    // This is important if e.g. more than one AT commands
    // have to be send to process one BAP-Indication
    // ***************************************************
    if( event & evAppTaskPhoneBookBapFctPending )
    {
        (void)ClearEvent( evAppTaskPhoneBookBapFctPending );
        if( gAppPBBapContext.common.fctId != PHONEBOOK_BAP_INVALID_FCT_ID )
        {
            // check if we have an internal function to handle or a bap indication
            if( gAppPBBapContext.common.fctId == PHONEBOOK_BAP_ACTIVATE_PB )
            {
                // check if we have to activate a phonebook, one event only should
                // occurr at the same time, so else/if
                (void)_APP_PHONEBOOK_BAP_sfHandlePendingPbActivation();
            }
            else if( gAppPBBapContext.common.fctId == PHONEBOOK_BAP_ACTIVATE_INC_PB )
            {
                // check if we have to activate a phonebook, one event only should
                // occurr at the same time, so else/if
                (void)_APP_PHONEBOOK_BAP_sfHandlePendingPbIncompleteActivation();
            }
            else if( gAppPBBapContext.common.fctId == PHONEBOOK_BAP_ACTIVATE_OLD_PB_AFTER_SORT_TIMEOUT )
            {
                (void)_APP_PHONEBOOK_BAP_sfHandlePendingOldPbActivation();
            }
            else
            {   // bap indication to be handled
                _APP_PHONEBOOK_BAP_vHandleBAPIndication(&gAppPBBapContext.common.bapInd);
                DEBUG_TEXT(APP_PB_BAP_SWI_TRC, MOD_APP_PB_BAP, "Handle pending BAPMsg");
            }
        }
    }

    // *****************************************************
    // look for events from BAPDispatcher and process them
    // check for new indication messages from BAP Dispatcher
    // *****************************************************
	if(event & evAppTaskPhoneBookBapInd )
	{
        (void)ClearEvent(evAppTaskPhoneBookBapInd);

        if( gAppPBBapContext.common.fctId == PHONEBOOK_BAP_INVALID_FCT_ID )
        {
            BapIndication bapInd;
            (void)ReceiveMessage( msgPhoneBookBapIndication, &bapInd);

            _APP_PHONEBOOK_BAP_vHandleBAPIndication(&bapInd);

            DEBUG_VALUE1(APP_PB_BAP_SWI_TRC, MOD_APP_PB_BAP, "Recvd BAPMsg: fctId %d",
                         bapInd.fct_id);
        }

        // check queue Status
        if( E_COM_NOMSG != GetMessageStatus(msgPhoneBookBapIndication))
        {
            // there are still some messages
            (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_PHONEBOOK_BAP_IND,
                                               MSEC(10),
                                               0);
        }
    }
}

void _APP_PHONEBOOK_BAP_vRetryContext(const BapIndication *pBapInd)
{
    gAppPBBapContext.common.fctId = (APP_PHONEBOOK_BAP_enFunctionIdType)pBapInd->fct_id;
    (void)memcpy(&gAppPBBapContext.common.bapInd, pBapInd, sizeof(BapIndication));

    (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_PHONEBOOK_BAP_FCT_PENDING, MSEC(10), 0);
}

void _APP_PHONEBOOK_BAP_vReleaseContext(const BapIndication *pBapInd)
{
    (void)APP_COMMON_UTILS_sfFreeBapIndication((BapIndication*)pBapInd);
    (void)memset(&gAppPBBapContext.common, 0, sizeof(gAppPBBapContext.common));

    // is activation of pb pending? if yes, now its the best chance to make it
    if(BT_TRUE == gAppPBBapContext.bActPbPending)
    {
        gAppPBBapContext.bActPbPending = BT_FALSE;
        gAppPBBapContext.common.fctId = PHONEBOOK_BAP_ACTIVATE_PB;
        // set alarm
        (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_PHONEBOOK_BAP_FCT_PENDING, MSEC(10), 0);
        // state to start with must have been already set !!
    }
    else if(BT_TRUE == gAppPBBapContext.bActPbIncompletePending)
    {
        gAppPBBapContext.bActPbIncompletePending = BT_FALSE;
        gAppPBBapContext.common.fctId            = PHONEBOOK_BAP_ACTIVATE_INC_PB;
        // set alarm
        (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_PHONEBOOK_BAP_FCT_PENDING, MSEC(10), 0);
        // state to start with must have been already set !!
    }
    else if(BT_TRUE == gAppPBBapContext.bActOldPbPending)
    {
        gAppPBBapContext.bActOldPbPending   = BT_FALSE;
        gAppPBBapContext.common.fctId       = PHONEBOOK_BAP_ACTIVATE_OLD_PB_AFTER_SORT_TIMEOUT;
        // set alarm
        (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_PHONEBOOK_BAP_FCT_PENDING, MSEC(10), 0);
        // state to start with must have been already set !!
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_vHandleBAPIndication                          */
/**
    \brief      handle indications from bap dispatcher

    \brief      Description:\n
                handles indications from bap dispatcher \n

    \param      bapInd
                const BapIndication *
                no limits

    \return     void
*/
/*----------------------------------------------------------------------------*/
static void _APP_PHONEBOOK_BAP_vHandleBAPIndication(const BapIndication *bapInd)
{
    APP_COMMON_enResponseCode result = APP_COMMON_RSP_OK;

    switch( bapInd->fct_id )
    {
        case BapFct_Phonebook_MissedCallIndication:
            result = _APP_PHONEBOOK_sfHandleMissedCallIndication(bapInd);
            break;

        case BapFct_Phonebook_MissedCalls:
        case BapFct_Phonebook_ReceivedCalls:
        case BapFct_Phonebook_DialedNumbers:
            result = _APP_PHONEBOOK_sfHandleCallArrays(bapInd);
            break;

        case BapFct_Phonebook_Phonebook:
            result = _APP_PHONEBOOK_sfHandlePhonebook(bapInd);
            break;

        case BapFct_Phonebook_PbState:
            result = _APP_PHONEBOOK_sfHandlePbState(bapInd);
            break;

        case BapFct_Phonebook_VoiceMailboxNumber:
            result = _APP_PHONEBOOK_sfHandleVoiceMailBox(bapInd);
            break;

        case BapFct_Phonebook_PbSpeller:
            result = _APP_PHONEBOOK_rcHandlePbSpeller(bapInd);
            break;

        case BapFct_Phonebook_PbStartDownload:
            result = _APP_PHONEBOOK_rcHandlePbStartDownload(bapInd);
            break;
#ifdef UHV_ACTIVATE_FEATURE_SMS
        /* sms specific */
        case BapFct_Phonebook_SMSState:
            result = _APP_PHONEBOOK_sfHandleSMSState(bapInd);
            break;
        case BapFct_Phonebook_SMSSend:
            result = _APP_PHONEBOOK_sfHandleSmsSend(bapInd);
            break;

        case BapFct_Phonebook_SMSReceived:
            result = _APP_PHONEBOOK_sfHandleReceivedSms(bapInd);
            break;
#endif
        default:
            // Send response to BAP Dispatcher about unknown Function ID
            APP_COMMON_UTILS_vSendBapError( BapLsg_Phonebook,
                                            bapInd->fct_id,
                                            AppErr_NotSuccessful);
            result = APP_COMMON_RSP_ERROR;
            break;
    }



    // check and handle the result
    switch(result)
    {
        case APP_COMMON_RSP_ERROR_BUSY:

            _APP_PHONEBOOK_BAP_vRetryContext(bapInd);

            DEBUG_VALUE1(APP_PB_BAP_SWI_TRC, MOD_APP_PB_BAP, "busy retry fctType %d",
                        bapInd->fct_id );
            break;
        case APP_COMMON_RSP_OK:
            /* SMS function release the context in the callback function reason: memory save */
            if(BapFct_Phonebook_SMSSend != bapInd->fct_id)
            {
                _APP_PHONEBOOK_BAP_vReleaseContext(bapInd);
            }
            else
            {
                gAppPBBapContext.common.fctId = (APP_PHONEBOOK_BAP_enFunctionIdType)bapInd->fct_id;
                (void)memcpy(&gAppPBBapContext.common.bapInd, bapInd, sizeof(BapIndication));
            }
            break;
        case APP_COMMON_RSP_ERROR_BAP_NOT_IMPLEMENTED:
            APP_COMMON_UTILS_vSendBapError( BapLsg_Phonebook,
                                            bapInd->fct_id,
                                            AppErr_NotSuccessful);
            break;
        case APP_COMMON_RSP_ERROR:
        default:
		    DEBUG_VALUE2(APP_PB_BAP_SWI_ERR, MOD_APP_PB_BAP, "vHandleBapIndication: error %d fctType: %d",
                            result, bapInd->fct_id );

            //Send BapError message
            APP_COMMON_UTILS_vSendBapError( BapLsg_Phonebook,
                                            bapInd->fct_id,
                                            AppErr_Unknown);

            _APP_PHONEBOOK_BAP_vReleaseContext(bapInd);
            break;
    }
}/* END OF _APP_PHONEBOOK_vHandleBAPIndication() */

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_rcHandlePbSpeller                             */
/**
    \brief      handles pb speller request

    \brief      Description:\n
                handles pb speller request \n

    \param      bap
                const BapIndication *
                value limits

    \return     APP_COMMON_enResponseCode
*/
/*----------------------------------------------------------------------------*/
static APP_COMMON_enResponseCode
_APP_PHONEBOOK_rcHandlePbSpeller(const BapIndication *bap)
{
    APP_COMMON_enResponseCode rspCode = APP_COMMON_RSP_OK;
    Uint8Type mode;
    Uint16Type strLen;
    const Uint8Type *pStr;
    const Uint8Type *pData = bap->payload.byte_seq.data;

    switch(bap->ind_type)
    {
        case BapInd_StartResult:
        {
            break;
        }
        default:
            return APP_COMMON_RSP_ERROR;
    }

    // get speller mode and search string
    mode = *pData;
    pData++;
    strLen = APP_COMMON_UTILS_u16GetBapString(pData, &pStr);
    pData += strLen;

    // check params
    switch((BapSpellerMode)mode)
    {
        case BAP_SPELLER_MATCH:
        case BAP_SPELLER_NEXT:
        case BAP_SPELLER_PREV:
            break;
        default:
            // invalid mode
            return APP_COMMON_RSP_ERROR;
    }

    // try to trigger AT request
    rspCode = APP_PHONEBOOK_rcRequestPbSearch(
                            _APP_PHONEBOOK_BAP_vOnSearchPbCb,
                            pStr, strLen);

    if( APP_COMMON_RSP_OK == rspCode )
    {
        // save speller mode and wait for callback
        gAppPBBapContext.appUserPb.spellerMode = (BapSpellerMode)mode;
    }

    return rspCode;
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_sfHandleMissedCallIndication                  */
/**
    \brief      handles missed call indication

    \brief      Description:\n
                handles missed call indication indication \n

    \param      bap
                const BapIndication *
                value limits

    \return     APP_COMMON_enResponseCode
*/
/*----------------------------------------------------------------------------*/
static APP_COMMON_enResponseCode _APP_PHONEBOOK_sfHandleMissedCallIndication(const BapIndication *bap)
{
    APP_COMMON_enResponseCode result = APP_COMMON_RSP_OK;
    // we should always have the correct size of this pb so just send the BAP
    // Request
    switch(bap->ind_type)
    {
        case BapInd_DataGet:
        {
            // send current number of missed calls in list
            // *** BEGIN Susanne: 07.08.09 Use internal detected number of missed calls / missed numbers
            //Uint16Type pbSize = APP_PHONEBOOK_u16GetCurrentPBSize(APP_PHONEBOOK_enMissedCalls);
            Uint16Type missedCalls = APP_PHONE_u16MissedCalls();
            Uint16Type missedNumbers = APP_PHONE_u16MissedNumbers();
            if( SUCCESS != _APP_PHONEBOOK_BAP_REQUEST_sfSendMissedCallsIndication(missedCalls, missedNumbers))
            {
                result = APP_COMMON_RSP_ERROR;
            }
            // *** END Susanne: 07.08.09 Use internal detected number of missed calls / missed numbers
            break;
        }
        case BapInd_DataSetGet:
            // HU wants to reset number of Missed Calls, just send back the data

            // #1051
            // SUSANNE: 30.07.2009, Reset of missed calls is now handled cvalling a function in APP_PHONEBOOK
            // this function notifies all interested applications, so APP_SD also gets a chance to change its frames
            // when APP_PHONEBOOK_BAP receives this notification, it sends MissedCallIndication=0, so we do not need to send it here again!
            // old
            //if( SUCCESS != _APP_PHONEBOOK_BAP_REQUEST_sfSendMissedCallsIndication(0))
            //{
            //    result = APP_COMMON_RSP_ERROR;
            //}
            // new

            // SUSANNE: 06.12.2009 Technisat RNS315 tries to reset MissedCallIndication, when a user gets disconnected
            // in this case, we will not reset them via APP_PHONEBOOK, cause this has been done already.
            // But we need to answer the request in any case
            if(NULL != ERM_pGetActiveUserProfile())
            {
                APP_PHONEBOOK_vResetMissedCallsByHMI();
            }
            // try to answer the SetGet and make HU think we accepted and reset MissedCalls
            else if( SUCCESS != _APP_PHONEBOOK_BAP_REQUEST_sfSendMissedCallsIndication(0,0))
            {
                result = APP_COMMON_RSP_ERROR;
            }
            break;

        default:
            result = APP_COMMON_RSP_ERROR;
            break;
    }
    return result;
}   /* END OF _APP_PHONEBOOK_sfHandleMissedCallIndication()                   */



/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_sfHandlePbState                               */
/**
    \brief      handles PbState indication

    \brief      Description:\n
                handles PbState indication \n

    \param      bap
                const BapIndication *
                value limits

    \return     APP_COMMON_enResponseCode
*/
/*----------------------------------------------------------------------------*/
static APP_COMMON_enResponseCode _APP_PHONEBOOK_sfHandlePbState(const BapIndication *bap)
{
    APP_COMMON_enResponseCode result    = APP_COMMON_RSP_OK;

    switch(bap->ind_type)
    {
        case BapInd_DataGet:
            break;

        default:
            return APP_COMMON_RSP_ERROR;
    }

    if(SUCCESS != _APP_PHONEBOOK_BAP_sfUpdatePbDwldState())
    {
        result = APP_COMMON_RSP_ERROR;
    }
    return result;
}   /* END OF _APP_PHONEBOOK_sfHandlePbState()                     */


/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_sfHandleVoiceMailBox                          */
/**
    \brief      handles VoicemailboxNumber indication

    \brief      Description:\n
                handles VoicemailboxNumber indication \n

    \param      bap
                const BapIndication *
                value limits

    \return     APP_COMMON_enResponseCode
*/
/*----------------------------------------------------------------------------*/
static APP_COMMON_enResponseCode _APP_PHONEBOOK_sfHandleVoiceMailBox(const BapIndication *bap)
{
    APP_COMMON_enResponseCode result    = APP_COMMON_RSP_OK;

    switch(bap->ind_type)
    {
        case BapInd_DataGet:
            // get voice mailbox number for active user and send it back
            (void)_APP_PHONEBOOK_BAP_REQUEST_sfSendVoiceMailBox();
            break;

        case BapInd_DataSetGet:
        {
            // check number, save it and send it back
            // TODO:
            //AT_DeviceId deviceId        = AT_INVALID_DEVICE_ID;
            const Uint8Type * number    = NULL;
            Uint16Type numberLen        = APP_COMMON_UTILS_u16GetBapString(bap->payload.byte_seq.data, &number);
            Uint8Type profileId         = ERM_u8GetUserProfileIndexForDeviceId(ERM_u8GetActivePhone());
            (void)APP_DEVICE_rcSetVoiceMailboxNumber(NULL, profileId, number, numberLen);
            // TODO: send it back, PUT THIS INTO CALLBACK
            (void)_APP_PHONEBOOK_BAP_REQUEST_sfSendVoiceMailBox();
            break;
        }
        default:
            return APP_COMMON_RSP_ERROR;
    }

    return result;
}   /* END OF _APP_PHONEBOOK_sfHandlePbState()                     */



/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_sfHandlePhonebook                            */
/**
    \brief      handles phonebook

    \brief      Description:\n
                handles phonebook \n

    \param      bapInd
                const BapIndication *
                value limits

    \return     APP_COMMON_enResponseCode
*/
/*----------------------------------------------------------------------------*/
static APP_COMMON_enResponseCode
_APP_PHONEBOOK_sfHandlePhonebook(const BapIndication* bapInd)
{
    BapArrayHeaderType  recvHeader;
    ConvArrayHeaderType convHeader;
    APP_COMMON_enResponseCode rspCode       = APP_COMMON_RSP_OK;
    SuccessFailType result                  = SUCCESS;
    Uint16Type pbSize                       = 0;
    void (*cb)(APP_COMMON_enResponseCode)   = NULL;
    APP_PHONEBOOK_enSelectedPbType pbType;

    switch( bapInd->ind_type )
    {
        case BapInd_DataGet:
            break;
        default:
            return APP_COMMON_RSP_ERROR;
    }

    /* read array header information into the application context */
    result = APP_COMMON_BAP_ARRAY_sfReadHeader( bapInd->payload.byte_seq.data,
                                                bapInd->payload.byte_seq.length,
                                                &recvHeader);
    if( result != SUCCESS )
    {
        // TODO: array handling in case of an error
        return APP_COMMON_RSP_ERROR_BAP_PARSE;
    }

    if((Uint32Type)(BT_FALSE == APP_PHONEBOOK_bIsPbValid(APP_PHONEBOOK_enUsersPB)) ||
       (Uint32Type)(BT_FALSE == ERM_bIsStartupDone()))
    {
        DEBUG_VALUE2(APP_PB_BAP_SWI_ERR, MOD_APP_PB_BAP, "HdlPb: read Pb err: valid:%d,Startup:%d",
                     APP_PHONEBOOK_bIsPbValid(APP_PHONEBOOK_enUsersPB), ERM_bIsStartupDone());

        APP_COMMON_UTILS_vSendBapError( BapLsg_Phonebook,
                                        BapFct_Phonebook_Phonebook,
                                       (enum BapError_t)0x42 /* temporarily not available */);
        return APP_COMMON_RSP_OK;
    }

    // get some params from APP_PHONEBOOK
    cb     = _APP_PHONEBOOK_BAP_vOnReadUsersPbCb;
    pbSize = APP_PHONEBOOK_u16GetCurrentPBSize(APP_PHONEBOOK_enUsersPB);
    pbType = APP_PHONEBOOK_enUsersPB;

    // get start /
    result = APP_COMMON_BAP_ARRAY_sfConvSWArrayHeader(
                           &recvHeader,
                           pbSize,
                           &convHeader);

    if( result != SUCCESS )
    {
        return APP_COMMON_RSP_ERROR_BAP_PARSE;
    }

    // special case record address is 0xF
    if(0xFU == recvHeader.mode.common.recordAddress )
    {
        if( SUCCESS != _APP_PHONEBOOK_BAP_REQUEST_sfSendPhonebookPosOnlyStatusArray(
                                        &recvHeader, &convHeader))
        {
            return APP_COMMON_RSP_ERROR;
        }
    }
    else
    {
        if(convHeader.elements > 0)
        {
            rspCode = APP_PHONEBOOK_rcRequestCompletePbEntries(
                                cb, pbType,
                                convHeader.start,
                                convHeader.elements);

            // save array and conv header ad wait for callback
            (void)memcpy(&gAppPBBapContext.appUserPb.arrayHeader, &recvHeader, sizeof(recvHeader));
            (void)memcpy(&gAppPBBapContext.appUserPb.convHeader, &convHeader, sizeof(convHeader));
        }
        else
        {
            /* nothing to request if elements is 0 */
            rspCode = APP_COMMON_RSP_OK;

            // save array and conv header and call the send function
            (void)memcpy(&gAppPBBapContext.appUserPb.arrayHeader, &recvHeader, sizeof(recvHeader));
            (void)memcpy(&gAppPBBapContext.appUserPb.convHeader, &convHeader, sizeof(convHeader));

            (void)_APP_PHONEBOOK_BAP_REQUEST_sfSendPhonebookStatusArray();
        }
    }

    return rspCode;
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_sfHandleCallArrays                            */
/**
    \brief      handles call arrays
                ( received and missed calls and dialed numbers )

    \brief      Description:\n
                handles call arrays \n
                ( received and missed calls and dialed numbers )

    \param      bapInd
                const BapIndication *
                value limits

    \return     APP_COMMON_enResponseCode
*/
/*----------------------------------------------------------------------------*/
static APP_COMMON_enResponseCode
_APP_PHONEBOOK_sfHandleCallArrays(const BapIndication* bapInd)
{
    BapArrayHeaderType  recvHeader;
    ConvArrayHeaderType convHeader;
    APP_COMMON_enResponseCode rspCode       = APP_COMMON_RSP_OK;
    SuccessFailType result                  = SUCCESS;
    Uint16Type pbSize                       = 0;
    void (*cb)(APP_COMMON_enResponseCode)   = NULL;
    APP_PHONEBOOK_enSelectedPbType pbType;

    switch( bapInd->ind_type )
    {
        case BapInd_DataGet:
            break;
        default:
            return APP_COMMON_RSP_ERROR;
    }

    /* read array header information into the application context */
    result = APP_COMMON_BAP_ARRAY_sfReadHeader( bapInd->payload.byte_seq.data,
            bapInd->payload.byte_seq.length,
            &recvHeader);
    if( result != SUCCESS )
    {
        // TODO: array handling in case of an error
        return APP_COMMON_RSP_ERROR_BAP_PARSE;
    }

    // check for call list type
    switch( bapInd->fct_id )
    {
        case BapFct_Phonebook_MissedCalls:
            if(BT_FALSE == APP_PHONEBOOK_bIsPbValid(APP_PHONEBOOK_enMissedCalls) ||
                    BT_FALSE == ERM_bIsStartupDone())
            {
                // SUSANNE: 27.10.2009 ticket #3043:
                // We should sent an Empty Array when not available instead of error code
                _APP_PHONEBOOK_BAP_REQUEST_sfSendEmptyCallListStatusArray(&recvHeader, BapFct_Phonebook_MissedCalls);
                //APP_COMMON_UTILS_vSendBapError( BapLsg_Phonebook,
                //                                BapFct_Phonebook_MissedCalls,
                //                                (enum BapError_t)0x42 /* temporarily not available */);
                /* return ok, fctCtx not yet set and need not to be reset */
                return APP_COMMON_RSP_OK;
            }
            cb     = _APP_PHONEBOOK_BAP_vOnReadMissedCallsCb;
            pbSize = APP_PHONEBOOK_u16GetCurrentPBSize(APP_PHONEBOOK_enMissedCalls);
            pbType = APP_PHONEBOOK_enMissedCalls;
            break;

        case BapFct_Phonebook_ReceivedCalls:
            if(BT_FALSE == APP_PHONEBOOK_bIsPbValid(APP_PHONEBOOK_enReceivedCalls) ||
                    BT_FALSE == ERM_bIsStartupDone())

            {
                // SUSANNE: 27.10.2009 ticket #3043:
                // We should sent an Empty Array when not available instead of error code
                _APP_PHONEBOOK_BAP_REQUEST_sfSendEmptyCallListStatusArray(&recvHeader, BapFct_Phonebook_ReceivedCalls);
                //APP_COMMON_UTILS_vSendBapError( BapLsg_Phonebook,
                //                                BapFct_Phonebook_ReceivedCalls,
                //                                (enum BapError_t)0x42 /* temporarily not available */);
                /* return ok, fctCtx not yet set and need not to be reset */
                return APP_COMMON_RSP_OK;
            }
            cb     = _APP_PHONEBOOK_BAP_vOnReadRecvCallsListCb;
            pbSize = APP_PHONEBOOK_u16GetCurrentPBSize(APP_PHONEBOOK_enReceivedCalls);
            pbType = APP_PHONEBOOK_enReceivedCalls;
            break;

        case BapFct_Phonebook_DialedNumbers:
            if(BT_FALSE == APP_PHONEBOOK_bIsPbValid(APP_PHONEBOOK_enLastDialed) ||
                    BT_FALSE == ERM_bIsStartupDone())

            {
                // SUSANNE: 27.10.2009 ticket #3043:
                // We should sent an Empty Array when not available instead of error code
                _APP_PHONEBOOK_BAP_REQUEST_sfSendEmptyCallListStatusArray(&recvHeader, BapFct_Phonebook_DialedNumbers);
                //APP_COMMON_UTILS_vSendBapError( BapLsg_Phonebook,
                //                                BapFct_Phonebook_DialedNumbers,
                //                                (enum BapError_t)0x42 /* temporarily not available */);
                /* return ok, fctCtx not yet set and need not to be reset */
                return APP_COMMON_RSP_OK;
            }
            cb     = _APP_PHONEBOOK_BAP_vOnReadDialedListCb;
            pbSize = APP_PHONEBOOK_u16GetCurrentPBSize(APP_PHONEBOOK_enLastDialed);
            pbType = APP_PHONEBOOK_enLastDialed;
            break;

        default:
            return APP_COMMON_RSP_ERROR_BAP_PARSE;
    }

    // get start / elements etc
    result = APP_COMMON_BAP_ARRAY_sfConvSWArrayHeader(
            &recvHeader,
            pbSize,
            &convHeader);

    if( result != SUCCESS )
    {
        return APP_COMMON_RSP_ERROR_BAP_PARSE;
    }

    if(convHeader.elements > 0)
    {
        rspCode = APP_PHONEBOOK_rcRequestCompletePbEntries(
                cb, pbType,
                convHeader.start,
                convHeader.elements);

        switch(pbType)
        {
            case APP_PHONEBOOK_enMissedCalls:
                // save array and conv header ad wait for callback
                (void)memcpy(&gAppPBBapContext.appMissedPb.arrayHeader, &recvHeader, sizeof(recvHeader));
                (void)memcpy(&gAppPBBapContext.appMissedPb.convHeader, &convHeader, sizeof(convHeader));
                break;
            case APP_PHONEBOOK_enReceivedCalls:
                // save array and conv header ad wait for callback
                (void)memcpy(&gAppPBBapContext.appReceivedPb.arrayHeader, &recvHeader, sizeof(recvHeader));
                (void)memcpy(&gAppPBBapContext.appReceivedPb.convHeader, &convHeader, sizeof(convHeader));
                break;
            case APP_PHONEBOOK_enLastDialed:
                // save array and conv header ad wait for callback
                (void)memcpy(&gAppPBBapContext.appDialedPb.arrayHeader, &recvHeader, sizeof(recvHeader));
                (void)memcpy(&gAppPBBapContext.appDialedPb.convHeader, &convHeader, sizeof(convHeader));
                break;
            default:
                DEBUG_VALUE1(APP_PB_BAP_SWI_ERR, MOD_APP_PB_BAP, "HaCallArrays: invalid pbType %d", pbType);
                break;
        }
    }
    else
    {
        /* nothing to request if elements is 0 */
        rspCode = APP_COMMON_RSP_OK;

        switch(pbType)
        {
            case APP_PHONEBOOK_enMissedCalls:
                // save array and conv header ad wait for callback
                (void)memcpy(&gAppPBBapContext.appMissedPb.arrayHeader, &recvHeader, sizeof(recvHeader));
                (void)memcpy(&gAppPBBapContext.appMissedPb.convHeader, &convHeader, sizeof(convHeader));
                (void)_APP_PHONEBOOK_BAP_REQUEST_sfSendCallListStatusArray(BapFct_Phonebook_MissedCalls);
                break;
            case APP_PHONEBOOK_enReceivedCalls:
                // save array and conv header ad wait for callback
                (void)memcpy(&gAppPBBapContext.appReceivedPb.arrayHeader, &recvHeader, sizeof(recvHeader));
                (void)memcpy(&gAppPBBapContext.appReceivedPb.convHeader, &convHeader, sizeof(convHeader));
                (void)_APP_PHONEBOOK_BAP_REQUEST_sfSendCallListStatusArray(BapFct_Phonebook_ReceivedCalls);
                break;
            case APP_PHONEBOOK_enLastDialed:
                // save array and conv header ad wait for callback
                (void)memcpy(&gAppPBBapContext.appDialedPb.arrayHeader, &recvHeader, sizeof(recvHeader));
                (void)memcpy(&gAppPBBapContext.appDialedPb.convHeader, &convHeader, sizeof(convHeader));
                (void)_APP_PHONEBOOK_BAP_REQUEST_sfSendCallListStatusArray(BapFct_Phonebook_DialedNumbers);
                break;
            default:
                DEBUG_VALUE1(APP_PB_BAP_SWI_ERR, MOD_APP_PB_BAP, "HaCallArrays: invalid pbType %d", pbType);
                break;
        }
    }

    return rspCode;
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_sfHandleSmsSend                            */
/**
    \brief      tries to send sms

    \brief      Description:\n
                tries to send sms \n

    \param      bapInd
                const BapIndication *
                value limits

    \return     APP_COMMON_enResponseCode
*/
/*----------------------------------------------------------------------------*/
static APP_COMMON_enResponseCode
_APP_PHONEBOOK_sfHandleSmsSend(const BapIndication * bapInd)
{
    APP_COMMON_enResponseCode result    = APP_COMMON_RSP_OK;
    const Uint8Type *telNr              = NULL;
    const Uint8Type *msg                = NULL;
    Uint16Type telNrLen                 = 0;
    Uint16Type msgLen                   = 0;
    Uint16Type offset                   = 0;

    // check if allowed
    if( 0U == (Uint16Type)gAppPBBapContext.mobServSupp.SMSSend )
    {
        return APP_COMMON_RSP_ERROR_BAP_NOT_IMPLEMENTED;
    }

    switch(bapInd->ind_type)
    {
        case BapInd_StartResult:
            break;

        default:
            return APP_COMMON_RSP_ERROR;
    }

    /* read phonenumber */
    telNrLen = APP_COMMON_UTILS_u16GetBapString(   bapInd->payload.byte_seq.data,   // bap byte seq
                                                   &telNr);                         // begin of number
    // get offset for message within bap string
    // 1. skip len bytes of telNr
    offset = (telNrLen>127) ? 2 : 1;
    // 2. skip telNr
    offset += telNrLen;
    // 3. skip code byte cause Parrot decides codec itself
    offset += 1;

    /* read message */
    msgLen = APP_COMMON_UTILS_u16GetBapString(   bapInd->payload.byte_seq.data + offset, // bap byte seq
                                                 &msg);                         // begin of number

    DEBUG_VALUE2(APP_PB_BAP_SWI_TRC, MOD_APP_PB_BAP, "UHV_APP_PHONE: SMS msgLen : %d, telLen : %d",
                msgLen, telNrLen);

    if (AT_MAX_SMS_LEN_TOTAL >= msgLen )
    {
        result = APP_SMS_rcSendSms(_APP_PHONEBOOK_BAP_vOnSendSmsCb,
                                       telNr, telNrLen, msg, msgLen);
    }
    else
    {
        result = APP_SMS_rcSendConcSms(_APP_PHONEBOOK_BAP_vOnSendSmsCb,
                                       telNr, telNrLen, msg, msgLen);
    }


    return result;
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_sfHandleReceivedSms                           */
/**
    \brief      requests read / unread sms from Parrot

    \brief      Description:\n
                requests read / unread sms from Parrot \n

    \param      bapInd
                const BapIndication *
                value limits

    \return     APP_COMMON_enResponseCode
*/
/*----------------------------------------------------------------------------*/
static APP_COMMON_enResponseCode
_APP_PHONEBOOK_sfHandleReceivedSms(const BapIndication * bapInd)
{
    BapArrayHeaderType  recvHeader;
    ConvArrayHeaderType convHeader;
    Uint16Type readFolderSize           = 0;
    Uint16Type unreadFolderSize         = 0;
    SuccessFailType result              = SUCCESS;
    APP_COMMON_enResponseCode rspCode   = APP_COMMON_RSP_OK;


    if( 0 == gAppPBBapContext.mobServSupp.SMSReceived )
    {
        return APP_COMMON_RSP_ERROR_BAP_NOT_IMPLEMENTED;
    }


    /* read array header information into the application context */
    result = APP_COMMON_BAP_ARRAY_sfReadHeader( bapInd->payload.byte_seq.data,
                                                bapInd->payload.byte_seq.length,
                                                &recvHeader);
    if( result != SUCCESS )
    {
        return APP_COMMON_RSP_ERROR_BAP_PARSE;
    }

    // for record address 0x0 and 0x3 ( SMS Text included ) we
    // must ensure that only one sms is requested, cause of memory limits
    if(0x0 == recvHeader.mode.common.recordAddress ||
       0x3 == recvHeader.mode.common.recordAddress )
    {
        if( 0 < recvHeader.elements)
        {
            recvHeader.elements = 1;
        }
    }

    readFolderSize   = APP_SMS_u16GetNumReadSms();
    unreadFolderSize = APP_SMS_u16GetNumUnreadSms();

    DEBUG_VALUE2(APP_PB_BAP_SWI_TRC, MOD_APP_PB_BAP, "SmsRecvd: unread:%d,read:%d",
                unreadFolderSize, readFolderSize);

    result = APP_COMMON_BAP_ARRAY_sfConvSWArrayHeader(
                        &recvHeader,
                        readFolderSize + unreadFolderSize,
                        &convHeader);
    if( result != SUCCESS )
    {
        DEBUG_VALUE2(APP_PB_BAP_SWI_ERR, MOD_APP_PB_BAP, "SmsRecvd: conv.hdr.fail: start:%x,elems:%x ...",
                     convHeader.start, convHeader.elements);
        DEBUG_VALUE1(APP_PB_BAP_SWI_ERR, MOD_APP_PB_BAP, "SmsRecvd: conv.hdr.fail: ... recAddr:%x",
                     recvHeader.mode.common.recordAddress);
        return APP_COMMON_RSP_ERROR_BAP_PARSE;
    }

    switch(bapInd->ind_type)
    {
        case BapInd_DataGet:
            /* request sms from parrot */
            // if we are currently synchronizing
            if( BT_FALSE == APP_SMS_bIsSmsInitialized())
            {
                // send temporarily not available
                APP_COMMON_UTILS_vSendBapError( BapLsg_Phonebook,
                                                BapFct_Phonebook_SMSReceived,
                                                (enum BapError_t)0x42);
                return APP_COMMON_RSP_OK;
            }
            else
            {

                DEBUG_VALUE2(APP_PB_BAP_SWI_TRC, MOD_APP_PB_BAP, "Sms Req start: %d, elems: %d ...",
                             convHeader.start, convHeader.elements);
                DEBUG_VALUE1(APP_PB_BAP_SWI_TRC, MOD_APP_PB_BAP, "Sms Req ... recAddr: %x",
                             recvHeader.mode.common.recordAddress);

                // RecordAddress 0x2 (SmsAttrib only) and 0xF (Pos only)
                // can be answered immediately
                if(0x2 == recvHeader.mode.common.recordAddress )
                {
                    if(SUCCESS != _APP_PHONEBOOK_BAP_REQUEST_sfSendSmsAttribOnly(
                                                &recvHeader,&convHeader))
                    {
                        rspCode = APP_COMMON_RSP_ERROR;
                    }
                }
                else if(0xF == recvHeader.mode.common.recordAddress )
                {
                    if(SUCCESS !=_APP_PHONEBOOK_BAP_REQUEST_sfSendSmsPosOnly(
                                                &recvHeader,&convHeader))
                    {
                        rspCode = APP_COMMON_RSP_ERROR;
                    }
                }
                else
                { // RecordAddress 0, 1, 3 and 4 needs interaction with parrot

                    // #2328 check if there is any element to be requested, else
                    // start = 0 will sent an AT*PMRD=folder,0,319, but index 0 is not valid!!!
                    // so if there are no
                    if( 0 == convHeader.elements )
                    {
                        // send empty array with received record address and start, but elems = 0
                        (void)_APP_PHONEBOOK_BAP_REQUEST_sfSendSmsReceivedNoSmsForIndex(&recvHeader);
                        return APP_COMMON_RSP_OK;
                    }
                    else
                    {
                        rspCode = APP_SMS_rcReadSms(_APP_PHONEBOOK_BAP_vOnReadSmsCb,
                                                    convHeader.start,
                                                    convHeader.elements);
                    }
                }
            }
            break;

        case BapInd_DataSetGet:
        {
            Uint8Type * pDataWriteBuf       = NULL;
            // get bap array data
            pDataWriteBuf   = bapInd->payload.byte_seq.data;
            // skip recvHeader
            pDataWriteBuf   += APP_COMMON_BAP_ARRAY_u16GetHeaderLength(&recvHeader);

            // check for delete request
            if( BT_TRUE == _APP_PHONEBOOK_bIsDeleteRequest(&recvHeader,pDataWriteBuf))
            {
                DEBUG_VALUE2(APP_PB_BAP_SWI_TRC, MOD_APP_PB_BAP, "Sms Delete Req start: %d, elems: %d",
                             convHeader.start, convHeader.elements);

                (void)_APP_PHONEBOOK_BAP_REQUEST_sfHandleSmsDeleted();

                rspCode = APP_SMS_rcDeleteSms(_APP_PHONEBOOK_BAP_vOnHandleDeleteSmsCb,
                                           convHeader.start,
                                           convHeader.elements,
                                           APP_SMS_enRspBap);
            }
            // check for mark as Read request
            else if( BT_TRUE == _APP_PHONEBOOK_bIsMarkAsReadRequest(&recvHeader,pDataWriteBuf))
            {
                DEBUG_VALUE2(APP_PB_BAP_SWI_TRC, MOD_APP_PB_BAP, "Sms MarkAsRead Req start: %d, elems: %d",
                             convHeader.start, convHeader.elements);

                rspCode = APP_SMS_rcMarkAsRead(_APP_PHONEBOOK_BAP_vOnMarkSmsAsReadCb,
                                            convHeader.start);

                // check whether type is already read, then send this back instead
                // of bap error message
                if( APP_COMMON_RSP_ERROR_SMS_ALREADY_OF_TYPE_READ == rspCode )
                {
                    // read sms and send back status with type read
                    rspCode = APP_SMS_rcReadSms(_APP_PHONEBOOK_BAP_vOnReadSmsCb,
                                         convHeader.start,
                                         convHeader.elements);
                    // do not return error to avoid BAP error
                    rspCode = APP_COMMON_RSP_OK;
                }
                else if(APP_COMMON_RSP_ERROR_SMS_INVALID_INDEX == rspCode)
                {
                    // send status with 0 elements cause we have no sms here
                    (void)_APP_PHONEBOOK_BAP_REQUEST_sfSendSmsReceivedNoSmsForIndex(&recvHeader);
                    // do not return error to avoid BAP error, but immediately return cause we do not need
                    // to copy anything
                    return APP_COMMON_RSP_OK;
                }
                else
                {

                }
            }
            // else read sms range and send data back AS IS
            else
            {
                DEBUG_VALUE2(APP_PB_BAP_SWI_TRC, MOD_APP_PB_BAP, "Sms Unknown SetGet req. start: %d, elems: %d",
                             convHeader.start, convHeader.elements);
                // #2328 check if there is any element to be requested, else
                // start = 0 will sent an AT*PMRD=folder,0,319, but index 0 is not valid!!!
                // so if there are no
                if( 0 == convHeader.elements )
                {
                    // send empty array with received record address and start, but elems = 0
                    (void)_APP_PHONEBOOK_BAP_REQUEST_sfSendSmsReceivedNoSmsForIndex(&recvHeader);
                    return APP_COMMON_RSP_OK;
                }
                else
                {
                    rspCode = APP_SMS_rcReadSms(_APP_PHONEBOOK_BAP_vOnReadSmsCb,
                                                convHeader.start,
                                                convHeader.elements);
                }
            }
            break;
        }
        default:
            return APP_COMMON_RSP_ERROR;
    }

    if( APP_COMMON_RSP_OK == rspCode )
    {
        // save array and conv header ad wait for callback
        (void)memcpy(&gAppPBBapContext.appSms.arrayHeader, &recvHeader, sizeof(recvHeader));
        (void)memcpy(&gAppPBBapContext.appSms.convHeader, &convHeader, sizeof(convHeader));
    }
    return rspCode;
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_rcHandlePbStartDownload                           */
/**
    \brief      handled the request to update the phonebook

    \brief      Description:\n
                manual update of the phonebook from the HU over BAP \n

    \param      bapInd
                const BapIndication *
                value limits

    \return     APP_COMMON_enResponseCode
*/
/*----------------------------------------------------------------------------*/
static APP_COMMON_enResponseCode
_APP_PHONEBOOK_rcHandlePbStartDownload(const BapIndication * bapInd)
{
    APP_COMMON_enResponseCode rspCode =
    APP_PHONEBOOK_rcRequestManualSync(_APP_PHONEBOOK_BAP_vOnPbStartDownloadCb);

    if( rspCode != APP_COMMON_RSP_OK )
    {
        rspCode = APP_COMMON_RSP_ERROR;
    }

//            gAppSdContext.manualPbSyncInProgress    = BT_TRUE;
    return rspCode;
}


static BooleanType _APP_PHONEBOOK_bIsDeleteRequest(const BapArrayHeaderType *pRecvHeader,
                                            const Uint8Type * pData)
{
    const Uint8Type * pReadBuf = pData;
    // delete request is indicated via - record address 0x1,
    //                                 - PBName="" and
    //                                 - SmsAttrib==0
    if( 0x1 != pRecvHeader->mode.common.recordAddress)
    {
        return BT_FALSE;
    }

    // check if transmit pos is set
    if( pRecvHeader->mode.setget.transmitPos )
    {
        // skip pos of sms
        pReadBuf  += pRecvHeader->mode.setget.indexSize ? 2 : 1;
    }

    if( 0x00 != *pReadBuf)  /* bap empty string */
    {
        return BT_FALSE;
    }

    // skip BapStringLen
    (void)*pReadBuf++;

    if( 0x00 != *pReadBuf ) // sms attrib = unread
    {
        return BT_FALSE;
    }

    return BT_TRUE;
}

static BooleanType _APP_PHONEBOOK_bIsMarkAsReadRequest(const BapArrayHeaderType *pRecvHeader,
                                                const Uint8Type * pData)
{
    const Uint8Type *pReadBuf = pData;
    // mark as read request is indicated via - record address 0x2,
    //                                       - elements = 0x01
    //                                       - SmsAttrib= 1
    if( 0x2 != pRecvHeader->mode.common.recordAddress)
    {
        return BT_FALSE;
    }

    if( 0x01 != pRecvHeader->elements)
    {
        return BT_FALSE;
    }

    // check if transmit pos is set
    if( pRecvHeader->mode.setget.transmitPos )
    {
        // skip pos of sms
        pReadBuf  += pRecvHeader->mode.setget.indexSize ? 2 : 1;
    }

    if( 0x01 != *pReadBuf ) // sms attrib = read
    {
        return BT_FALSE;
    }

    return BT_TRUE;
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_sfHandleSmsState                               */
/**
    \brief      handles PbState indication

    \brief      Description:\n
                handles PbState indication \n

    \param      bap
                const BapIndication *
                value limits

    \return     APP_COMMON_enResponseCode
*/
/*----------------------------------------------------------------------------*/
static APP_COMMON_enResponseCode
_APP_PHONEBOOK_sfHandleSMSState(const BapIndication *bap)
{
    Uint8Type simReady = 0;
    Uint8Type storageState = 0;
    SuccessFailType result = SUCCESS;

    if( 0 == gAppPBBapContext.mobServSupp.SMSState )
    {
        return APP_COMMON_RSP_ERROR_BAP_NOT_IMPLEMENTED;
    }

    switch(bap->ind_type)
    {
        case BapInd_DataGet:
            //break;
        case BapInd_DataSetGet:
        {
            if(bap->payload.byte_seq.length != 4)
            {
                // send back an error
                return APP_COMMON_RSP_ERROR;
            }

            switch (APP_SMS_enGetSmsSupportState())
            {
                case APP_SMS_NOT_INITIALIZED:
                case APP_SMS_NOT_INITIALIZED_AND_SUPPORTED:
                case APP_SMS_INITIALIZED_AND_UNSUPPORTED:
                    simReady = 0x00;
                    break;
                case APP_SMS_INITIALIZED_AND_SUPPORTED:
                    simReady = 0x01;
                    break;
                default:
                    break;
            }
            // simReady        = (Uint8Type)*(bap->payload.byte_seq.data);
            // storageState    = (Uint8Type)*(bap->payload.byte_seq.data+1);

            /* the SMSState resets the unread folder, but at this point the
             * unread folder should be empty already */
            /* if(0 != APP_SMS_u16GetNumUnreadSms())
            {
                
            }*/
            break;
        }
        default:
            // send back an error
            return APP_COMMON_RSP_ERROR;
    }

    result = _APP_PHONEBOOK_BAP_REQUEST_sfSendSmsState(simReady,
                                                       storageState,
                                                       APP_SMS_u16GetNumUnreadSms());
    if( SUCCESS != result )
    {
        return APP_COMMON_RSP_ERROR;
    }
    return APP_COMMON_RSP_OK;
}   /* END OF _APP_PHONEBOOK_sfHandleSMSState()                     */


void _APP_PHONEBOOK_BAP_vSetBasicPbMobileServiceSupport(PbMobileServiceSupport *pPbMss)
{
    (void)memset(pPbMss, 0, sizeof(*pPbMss));

    //pPbMss->GetAll           = 1;
    pPbMss->BapConfig        = 1;
    pPbMss->FunctionList     = 1;
    pPbMss->Heartbeat        = 1;
    pPbMss->FsgSetup         = 1;
    pPbMss->FsgOpState       = 1;
    pPbMss->MobileServiceSupport = 1;
    pPbMss->MissedCallIndication = 1;
    pPbMss->MissedCalls      = 1;
    pPbMss->ReceivedCalls    = 1;
    pPbMss->DialedNumbers    = 1;
    pPbMss->PbState          = 1;
    pPbMss->Phonebook        = 1;
    pPbMss->PbSpeller        = 1;
    pPbMss->VoiceMailboxNumber   = 1;
    pPbMss->PbStartDownload  = 1;
}


void APP_PHONEBOOK_BAP_vDiagSmsSupportChanged(BooleanType bIsSmsAllowed)
{
    DEBUG_VALUE1(APP_PB_BAP_SWI_TRC, MOD_APP_PB_BAP, "Trigger from Diag = %d",bIsSmsAllowed);
	if( BT_TRUE == bIsSmsAllowed )
    {
        // first check if sms is supported and already intialized before
        // activating sms
        if(BT_TRUE == APP_SMS_bIsSmsInitialized() &&
           APP_SMS_INITIALIZED_AND_SUPPORTED == APP_SMS_enGetSmsSupportState() )
        {
            // activate all sms specific functions
            gAppPBBapContext.mobServSupp.SMSState      = 1;
            gAppPBBapContext.mobServSupp.SMSReceived   = 1;
            gAppPBBapContext.mobServSupp.SMSSend       = 1;
            (void)_APP_PHONEBOOK_BAP_REQUEST_sfSendMobServSupp(&gAppPBBapContext.mobServSupp);
        }
    }
    else
    {
        // deactivate all sms specific functions
        gAppPBBapContext.mobServSupp.SMSState      = 0;
        gAppPBBapContext.mobServSupp.SMSReceived   = 0;
        gAppPBBapContext.mobServSupp.SMSSend       = 0;
        (void)_APP_PHONEBOOK_BAP_REQUEST_sfSendMobServSupp(&gAppPBBapContext.mobServSupp);
    }
}

void APP_PHONEBOOK_BAP_vDeleteSms(Uint16Type start)
{
    gAppPBBapContext.appSms.convHeader.start = start;
    gAppPBBapContext.appSms.convHeader.elements = 1;
    _APP_PHONEBOOK_BAP_REQUEST_sfHandleSmsDeleted();
}

static SuccessFailType _APP_PHONEBOOK_BAP_sfHandlePendingPbActivation()
{
    BooleanType bFinished = BT_FALSE;

    DEBUG_VALUE1(APP_PB_BAP_SWI_TRC, MOD_APP_PB_BAP, "HdlPendPbAct: state %d",gAppPBBapContext.pbActState);

    switch(gAppPBBapContext.pbActState)
    {
        case APP_PB_BAP_ACT_PB__PB_STATE_LOADING:
        {
            ATPhoneBookDwnlState dwldState = APP_PHONEBOOK_enGetPBDwldState();
            Uint16Type pbSize = APP_PHONEBOOK_u16GetCurrentPBSize(APP_PHONEBOOK_enUsersPB);
            /* do not send this, if pb load had an error */
            if( (AT_PB_LOAD_FINISHED == dwldState) ||
                ((AT_PB_LOAD_FINISHED_BUT == dwldState) && 0 != pbSize && (BT_TRUE == APP_PHONEBOOK_bIsPbValid(APP_PHONEBOOK_enUsersPB))))
            {
                (void)_APP_PHONEBOOK_BAP_REQUEST_sfSendPbState(BAP_PB_LOADING, 0xFFFF);
            }
            gAppPBBapContext.pbActState = APP_PB_BAP_ACT_PB__PB_STATE_COMPLETE;
            (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_PHONEBOOK_BAP_FCT_PENDING, MSEC(100),MSEC(0));
            break;
        }
        case APP_PB_BAP_ACT_PB__PB_STATE_COMPLETE:
        {
            ATPhoneBookDwnlState dwldState = APP_PHONEBOOK_enGetPBDwldState();
            Uint16Type pbSize = APP_PHONEBOOK_u16GetCurrentPBSize(APP_PHONEBOOK_enUsersPB);
            (void)_APP_PHONEBOOK_BAP_REQUEST_sfSendPbState(BAP_PB_LOAD_COMPLETE, pbSize);
            #if 0
            switch(dwldState)
            {
                case AT_PB_LOADING:
                    /* we are now able to get the phonebook when sync is currently in progress,
                       in this case, when the phonebook is valid, we can activate it although we are in
                       sync, this shoudl happen only for the very first PPBU:= received, not for manual syncs during uptime */
                case AT_PB_LOAD_FINISHED:
                    (void)_APP_PHONEBOOK_BAP_REQUEST_sfSendPbState(BAP_PB_LOAD_COMPLETE, pbSize);
                    break;
                case AT_PB_LOAD_FINISHED_BUT:
                    if(0 < pbSize && (BT_TRUE == APP_PHONEBOOK_bIsPbValid(APP_PHONEBOOK_enUsersPB)))
                    {
                        (void)_APP_PHONEBOOK_BAP_REQUEST_sfSendPbState(BAP_PB_LOAD_INCOMPLETE_AVAILABLE, pbSize);
                    }
                    /* else: pb error, so not available */
                    break;
            }
            #endif
            gAppPBBapContext.pbActState = APP_PB_BAP_ACT_PB__CHANGED_ARRAY;
            // here we need some more time before sending ChangedArray, cause
            // PbState is queued in BapStack (cause we send two PbStates
            // directly after each other ) and we
            // need to ensure that PbState complete arrives before ChangedArray
            // in HeadUnit
            (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_PHONEBOOK_BAP_FCT_PENDING, MSEC(200),MSEC(0));
            break;
        }
        case APP_PB_BAP_ACT_PB__CHANGED_ARRAY:
            (void)_APP_PHONEBOOK_BAP_REQUEST_sfSendPbChangedArray(0,0xFFFF);
            bFinished = BT_TRUE;
            break;
        default:
            bFinished = BT_TRUE;
            break;
    }

    if(BT_TRUE == bFinished)
    {
        DEBUG_TEXT(APP_PB_BAP_SWI_TRC, MOD_APP_PB_BAP, "HdlPendPbAct: finished");
        gAppPBBapContext.bActPbPending = BT_FALSE;
        /* to ensure correct start when pb is syncronized again without a reconnect */
        gAppPBBapContext.pbActState = APP_PB_BAP_ACT_PB__INVALID;
        /* no alarm needed */
        // release current context, bapInd is NULL due to an internal function
        _APP_PHONEBOOK_BAP_vReleaseContext(NULL);
    }

    return SUCCESS;
}

static SuccessFailType _APP_PHONEBOOK_BAP_sfHandlePendingPbIncompleteActivation()
{
    BooleanType bFinished = BT_FALSE;

    DEBUG_VALUE1(APP_PB_BAP_SWI_TRC, MOD_APP_PB_BAP, "HdlPendIncPbAct: state %d",gAppPBBapContext.pbActState);

    switch(gAppPBBapContext.pbActState)
    {
        case APP_PB_BAP_ACT_PB__PB_STATE_LOADING:
        {
            ATPhoneBookDwnlState dwldState = APP_PHONEBOOK_enGetPBDwldState();
            Uint16Type pbSize = APP_PHONEBOOK_u16GetCurrentPBSize(APP_PHONEBOOK_enUsersPB);
            /* do not send this, if pb load had an error */
            if( (AT_PB_LOAD_FINISHED == dwldState) ||
                ((AT_PB_LOAD_FINISHED_BUT == dwldState) && 0 != pbSize && (BT_TRUE == APP_PHONEBOOK_bIsPbValid(APP_PHONEBOOK_enUsersPB))))
            {
                (void)_APP_PHONEBOOK_BAP_REQUEST_sfSendPbState(BAP_PB_LOADING, 0xFFFF);
            }
            gAppPBBapContext.pbActState = APP_PB_BAP_ACT_PB__PB_STATE_COMPLETE;
            (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_PHONEBOOK_BAP_FCT_PENDING, MSEC(100),MSEC(0));
            break;
        }
        case APP_PB_BAP_ACT_PB__PB_STATE_COMPLETE:
        {
            #if 0
            ATPhoneBookDwnlState dwldState = APP_PHONEBOOK_enGetPBDwldState();
            #endif
            Uint16Type pbSize = APP_PHONEBOOK_u16GetCurrentPBSize(APP_PHONEBOOK_enUsersPB);
            (void)_APP_PHONEBOOK_BAP_REQUEST_sfSendPbState(BAP_PB_LOAD_INCOMPLETE_AVAILABLE, pbSize);
            #if 0
            switch(dwldState)
            {
                case AT_PB_LOADING:
                    /* we are now able to get the phonebook when sync is currently in progress,
                       in this case, when the phonebook is valid, we can activate it although we are in
                       sync, this shoudl happen only for the very first PPBU:= received, not for manual syncs during uptime */
                case AT_PB_LOAD_FINISHED:
                    (void)_APP_PHONEBOOK_BAP_REQUEST_sfSendPbState(BAP_PB_LOAD_INCOMPLETE, pbSize);
                    break;
                case AT_PB_LOAD_FINISHED_BUT:
                    if(0 < pbSize && (BT_TRUE == APP_PHONEBOOK_bIsPbValid(APP_PHONEBOOK_enUsersPB)))
                    {
                        (void)_APP_PHONEBOOK_BAP_REQUEST_sfSendPbState(BAP_PB_LOAD_INCOMPLETE_AVAILABLE, pbSize);
                    }
                    /* else: pb error, so not available */
                    break;
            }
            #endif
            gAppPBBapContext.pbActState = APP_PB_BAP_ACT_PB__CHANGED_ARRAY;
            // here we need some more time before sending ChangedArray, cause
            // PbState is queued in BapStack (cause we send two PbStates
            // directly after each other ) and we
            // need to ensure that PbState complete arrives before ChangedArray
            // in HeadUnit
            (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_PHONEBOOK_BAP_FCT_PENDING, MSEC(200),MSEC(0));
            break;
        }
        case APP_PB_BAP_ACT_PB__CHANGED_ARRAY:
            (void)_APP_PHONEBOOK_BAP_REQUEST_sfSendPbChangedArray(0,0xFFFF);
            bFinished = BT_TRUE;
            break;
        default:
            bFinished = BT_TRUE;
            break;
    }

    if(BT_TRUE == bFinished)
    {
        DEBUG_TEXT(APP_PB_BAP_SWI_TRC, MOD_APP_PB_BAP, "HdlPendIncPbAct: finished");
        gAppPBBapContext.bActPbIncompletePending = BT_FALSE;
        /* to ensure correct start when pb is syncronized again without a reconnect */
        gAppPBBapContext.pbActState = APP_PB_BAP_ACT_PB__INVALID;
        /* no alarm needed */
        // release current context, bapInd is NULL due to an internal function
        _APP_PHONEBOOK_BAP_vReleaseContext(NULL);
    }

    return SUCCESS;
}


static SuccessFailType _APP_PHONEBOOK_BAP_sfHandlePendingOldPbActivation()
{
    BooleanType bFinished = BT_FALSE;

    DEBUG_VALUE1(APP_PB_BAP_SWI_TRC, MOD_APP_PB_BAP, "HdlPendOldPbAct: state %d",gAppPBBapContext.pbActOldPbState);

    switch(gAppPBBapContext.pbActOldPbState)
    {
        case APP_PB_BAP_ACT_OLD_PB__PB_STATE_NOT_AVAILABLE:
        {   /* this step is needed cause during sorting we have PbState Loading! If we don't reset this state but
               send PbComplete direclty, user would be informed about a new pb, which isn't the case */
            (void)_APP_PHONEBOOK_BAP_REQUEST_sfSendPbState(BAP_PB_NOT_AVAILABLE, 0x0000);
            gAppPBBapContext.pbActOldPbState = APP_PB_BAP_ACT_OLD_PB__PB_STATE_COMPLETE;
            (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_PHONEBOOK_BAP_FCT_PENDING, MSEC(100),MSEC(0));
            break;
        }
        case APP_PB_BAP_ACT_OLD_PB__PB_STATE_COMPLETE:
        {
            Uint16Type pbSize = APP_PHONEBOOK_u16GetCurrentPBSize(APP_PHONEBOOK_enUsersPB);
            (void)_APP_PHONEBOOK_BAP_REQUEST_sfSendPbState(BAP_PB_LOAD_COMPLETE, pbSize);
            gAppPBBapContext.pbActOldPbState = APP_PB_BAP_ACT_OLD_PB__CHANGED_ARRAY;
            // here we need some more time before sending ChangedArray, cause
            // PbState is queued in BapStack (cause we send two PbStates
            // directly after each other ) and we
            // need to ensure that PbState complete arrives before ChangedArray
            // in HeadUnit
            (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_PHONEBOOK_BAP_FCT_PENDING, MSEC(200),MSEC(0));
            break;
        }
        case APP_PB_BAP_ACT_OLD_PB__CHANGED_ARRAY:
            (void)_APP_PHONEBOOK_BAP_REQUEST_sfSendPbChangedArray(0,0xFFFF);
            bFinished = BT_TRUE;
            break;
        default:
            bFinished = BT_TRUE;
            break;
    }

    if(BT_TRUE == bFinished)
    {
        DEBUG_TEXT(APP_PB_BAP_SWI_TRC, MOD_APP_PB_BAP, "HdlPendOldPbAct: finished");
        gAppPBBapContext.bActOldPbPending = BT_FALSE;
        /* to ensure correct start when pb is syncronized again without a reconnect */
        gAppPBBapContext.pbActOldPbState = APP_PB_BAP_ACT_OLD_PB__INVALID;
        /* no alarm needed */
        // release current context, bapInd is NULL due to an internal function
        _APP_PHONEBOOK_BAP_vReleaseContext(NULL);
    }

    return SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONEBOOK_BAP_vOnBAPDispatcherNotification               */
/**
    \brief      handle BAP Dispatcher notifications

    \brief      Description:\n
                handle BAP Dispatcher notifications  \n

    \param      notification
                APP_BAPDispatcher_NotificationType

    \return     void
*/
/*----------------------------------------------------------------------------*/
void APP_PHONEBOOK_BAP_vOnBAPDispatcherNotification(APP_BAPDispatcher_NotificationType notification, fctId_t aFctId) /*RT13652*/
{
    switch(notification)
    {
        case APP_BAPDISPATCHER_NOTIFY_STATUSARRAY_SENT:
        {
            //gAppPBBapContext.StatusArrayPending = BT_FALSE;
            //DEBUG_TEXT(APP_PB_BAP_SWI_TRC, MOD_APP_PB_BAP, "Phonebook Status Array transmission finished");
            //break;

            gAppPBBapContext.enStatusArrayPending = APP_PB_BAP_CHANGED_ARRAY_INVALID;

            switch(aFctId)
            {
                case BapFct_Phonebook_MissedCalls:
                    DEBUG_TEXT(APP_PB_BAP_SWI_TRC, MOD_APP_PB_BAP, "Phonebook Status Array(Missed calls) transmission finished");
                    break;
                case BapFct_Phonebook_ReceivedCalls:
                    DEBUG_TEXT(APP_PB_BAP_SWI_TRC, MOD_APP_PB_BAP, "Phonebook Status Array(Received calls) transmission finished");
                    break;
                case BapFct_Phonebook_DialedNumbers:
                    DEBUG_TEXT(APP_PB_BAP_SWI_TRC, MOD_APP_PB_BAP, "Phonebook Status Array(Dialed calls) transmission finished");
                    break;
                case BapFct_Phonebook_Phonebook:
                    DEBUG_TEXT(APP_PB_BAP_SWI_TRC, MOD_APP_PB_BAP, "Phonebook Status Array(Phone book) transmission finished");
                    break;
                default:
                    DEBUG_TEXT(APP_PB_BAP_SWI_TRC, MOD_APP_PB_BAP, "Phonebook Status Array transmission :Unsupported fct id");
                    break;
            }
            break;
        }
        default:
            DEBUG_VALUE1(APP_PB_BAP_SWI_TRC, MOD_APP_PB_BAP, "vBAPDispatcherNot: unsup %d",
                                notification);
            break;
    }
}

/* End Of File APP_PHONEBOOK_BAP_C1.c */


