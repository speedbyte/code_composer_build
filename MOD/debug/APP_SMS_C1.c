/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     APP_SMS_C1.c
*-----------------------------------------------------------------------------
* Module Name:    APP_SMS
*-----------------------------------------------------------------------------
* Description:    Handles all sms specific
                  AT Cmd Handler
*-----------------------------------------------------------------------------
* $Date: 2010-11-02 09:21:36 +0100 (Di, 02 Nov 2010) $
* $Rev: 16010 $
* $Author: weber $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_SMS/trunk/src/APP_SMS_C1.c $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/

#include "UART.h"
/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/
#include <osek.h>           // vector
#include <osekext.h>        // vector

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "APP_SMS.h"
#include "APP_SMS_CI.h"
#include "APP_SMS_Requests.h"
#include "APP_SMS_ERM.h"
#include "ATCmdHandlerData.h"
#include "ATCmdHandler.h"

#include "APP_PHONEBOOK_BAP.h"
#include "APP_SD.h"

#include "APP_CAN.h"
#include "ERM.h"
#include "APP_COMMON_Utils.h"

#include "APP_SMS_C1.id"
#include "TRACE.h"

#include "APP_PHONE_AT_PNCC.h"

/*----------------------------------------------------------------------------*/
/* pragma Directives                                                          */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Definitions (avoid as much as possible!)                  */
/*----------------------------------------------------------------------------*/

/**
\brief	variable to save the current call management function currently in progress
*/
AppSmsContext gAppSmsContext;

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
static void _APP_SMS_vSendResultReleaseContext(APP_COMMON_enResponseCode result);

static SuccessFailType _APP_SMS_sfHandlePendingFunction(const SCI_ST* puart);
static SuccessFailType _APP_SMS_sfSendPendingSmsInit(const SCI_ST *puart);
static SuccessFailType _APP_SMS_sfSendPendingConcSmsSendRequest(const SCI_ST* puart);
static SuccessFailType _APP_SMS_sfSendPendingSmsReadRequest(const SCI_ST *puart);
static SuccessFailType _APP_SMS_sfSendPendingSmsReadPreviewRequest(const SCI_ST *puart);
static SuccessFailType _APP_SMS_sfSendPendingSmsDeleteRequest(const SCI_ST *puart);
static SuccessFailType _APP_SMS_sfSendPendingMarkAsReadRequest(const SCI_ST *puart);


static void _APP_SMS_vCheckSmsInitialization();

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
/* Function    : APP_SMS_sfInitApp                                            */
/**
    \brief      Callback function for initialization of App Phonebook

    \brief      Description:\n

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType APP_SMS_sfInitApp(void)
{
    SuccessFailType sF = SUCCESS;

    (void)memset(&gAppSmsContext, 0, sizeof(gAppSmsContext));

    gAppSmsContext.supportState       = APP_SMS_NOT_INITIALIZED;

    // sms not supported unless sms init has been processed
    gAppSmsContext.bInitSmsFinished   = BT_FALSE;

    gAppSmsContext.unreadFolderSize   = 0;
    gAppSmsContext.readFolderSize     = 0;
    gAppSmsContext.bSmsSendAllowed    = BT_TRUE;

    return(sF);
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_SMS_vReInitApp                                          */
/**
    \brief      reinits sms context

    \brief      Description:\n
                reinits sms context \n

    \return     void
*/
/*----------------------------------------------------------------------------*/
void _APP_SMS_vReInitApp(void)
{
    /* RT: 2347, after phone disconnect InitApp is called and we need to check if
           a callback waits for result, otherwise it will wait forever */
    if(gAppSmsContext.fct.common.callback)
    {
        DEBUG_VALUE1(APP_SMS_SWI_WNG, MOD_APP_SMS, "ReInitApp: callback(ERR) for %d", gAppSmsContext.fct.common.fctId);

        gAppSmsContext.fct.common.callback(APP_COMMON_RSP_ERROR);
    }

    ERM_ReleaseUartForSMS();

    APP_SMS_sfInitApp();
}

void _APP_SMS_vReleaseFctContext()
{
    (void)memset(&gAppSmsContext.fct, 0, sizeof(gAppSmsContext.fct));
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SMS_vSendResultReleaseContext                           */
/**
    \brief      notify caller of last function ( if available ) about result
                and release the function context

    \brief      Description:\n
                notify caller of last function ( if available ) about result\n
                and release the function context

    \return     void
*/
/*----------------------------------------------------------------------------*/
static void _APP_SMS_vSendResultReleaseContext(APP_COMMON_enResponseCode result)
{
    DEBUG_VALUE2(APP_SMS_SWI_TRC, MOD_APP_SMS, "RelContext: fctId %d res %d", gAppSmsContext.fct.common.fctId, result );

    if(gAppSmsContext.fct.common.callback)
    {
        gAppSmsContext.fct.common.callback(result);
    }

    _APP_SMS_vReleaseFctContext();

    /* make sure sms is intialized correctly */
    _APP_SMS_vCheckSmsInitialization();
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_SMS_vHandleEvents                                                 */
/**
    \brief      Task Function to handle events for Sms Application

    \brief      Description:\n

    \param      event
                EventMaskType
                Bitmask containing all unhandled events

    \return     void
*/
/*----------------------------------------------------------------------------*/
void APP_SMS_vHandleEvents(EventMaskType event)
{
    // wait for interesting events

    // ****************************************************
    // check if we have some additional things to do
    // from the last callFctId triggered by BAP
    // This is important if e.g. more than one AT commands
    // have to be send to process one BAP-Indication
    // ***************************************************

    // remove event evAppTaskSmsTResponse and message msgSmsATResponse
    // from APP_PHONE_vHandleEvents to central AT-handling in
    // APP_COMMON_vHandleEvents

    if( event & evAppTaskSmsFctPending )
    {
        const SCI_ST * puart    = NULL;
        (void)ClearEvent( evAppTaskSmsFctPending );

        if(ERM_bIsStartupDone() != BT_TRUE)
        {
            /* we are not able to to request the UART in this state */
            DEBUG_VALUE1( APP_SMS_SWI_ERR, MOD_APP_SMS, "HandleEvents: CK5050_SHUTDOWN, drop pending, fct %d", gAppSmsContext.fct.common.fctId);

            _APP_SMS_vSendResultReleaseContext(APP_COMMON_RSP_ERROR);
        }
        else
        {
            // try to get ressource
            puart = ERM_GetUartForSMS();
            if( !puart )
            {
                // ressource is busy, set an alarm to try again in some cycles
                // set event once again after 10 ms
                (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_SMS_FCT_PENDING,
                                                    MSEC(ERM_UART_BUSY_RETRY_TIME),
                                                    0);
            }
            else
            {
                // call common function to handle next step
                (void)_APP_SMS_sfHandlePendingFunction(puart);
            }
        }
    }
}

// Handle Pending BApFct
static SuccessFailType _APP_SMS_sfHandlePendingFunction( const SCI_ST* puart )
{
    SuccessFailType result = SUCCESS;

    DEBUG_VALUE1( APP_SMS_SWI_TRC, MOD_APP_SMS, "HaPendFct: %d", gAppSmsContext.fct.common.fctId);

    switch( gAppSmsContext.fct.common.fctId )
    {
        case APP_SMS_enSmsInit:
            result = _APP_SMS_sfSendPendingSmsInit(puart);
            break;
        case APP_SMS_enReadSms:
            result = _APP_SMS_sfSendPendingSmsReadRequest(puart);
            break;
        case APP_SMS_enSendConcSms:
            result = _APP_SMS_sfSendPendingConcSmsSendRequest(puart);
            break;
        case APP_SMS_enReadSmsPreview:
            result = _APP_SMS_sfSendPendingSmsReadPreviewRequest(puart);
            break;
        case APP_SMS_enDeleteSms:
            result = _APP_SMS_sfSendPendingSmsDeleteRequest(puart);
            break;
        case APP_SMS_enMarkAsRead:
            result = _APP_SMS_sfSendPendingMarkAsReadRequest(puart);
            break;
        default:
            DEBUG_VALUE1 (  APP_SMS_SWI_ERR, MOD_APP_SMS, "Unknown Pending Fct: %d", gAppSmsContext.fct.common.fctId);
            result = FAIL;
            break;
    }

    if( SUCCESS != result )
    {
        _APP_SMS_vSendResultReleaseContext(APP_COMMON_RSP_ERROR);
        (void)ERM_ReleaseUartForSMS();
    }

    return result;
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SMS_sfSendPendingSmsInit                                */
/**
    \brief      processing with sms initialization \n

    \brief      Description:\n
                processing with sms initialization \n

    \return     SuccessFailType
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
static SuccessFailType _APP_SMS_sfSendPendingSmsInit(const SCI_ST* puart)
{
    SuccessFailType result = SUCCESS;

    gAppSmsContext.fct.smsInit.state = _APP_SMS_enGetNextInitState(gAppSmsContext.fct.smsInit.state);

    DEBUG_VALUE1 (  APP_SMS_SWI_TRC, MOD_APP_SMS, "SendPendSmsInit: state %d", gAppSmsContext.fct.smsInit.state);

    switch( gAppSmsContext.fct.smsInit.state )
    {
        case APP_SMS_enSetCharset:
            result = _APP_SMS_sfSendSetCharsetATRequest(puart);
            break;

        case APP_SMS_enStartSynchro:
            result = _APP_SMS_sfSendStartSynchroATRequest(puart);
            break;

        case APP_SMS_enWaitSynchro:    /* wait for *PMLS:END */
        case APP_SMS_enFinishedSynchro:    /* *PMLS:END received */
            (void)ERM_ReleaseUartForSMS();
            break;

        case APP_SMS_enGetFolderSizes:
            result = _APP_SMS_sfSendSmsFolderSizeATRequest(puart);
            break;

        default:
            DEBUG_VALUE1 (  APP_SMS_SWI_ERR, MOD_APP_SMS, "SendPendSmsInit: Unsup state %d", gAppSmsContext.fct.smsInit.state);
            result = FAIL;
            break;
    }

    return result;
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SMS_sfSendPendingConcSmsSendRequest             */
/**
    \brief      handle next step requesting sms

    \brief      Description:\n
                handle next step requesting sms \n

    \param      puart
                const SCI_ST*

    \return     SuccessFailType
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
static SuccessFailType _APP_SMS_sfSendPendingConcSmsSendRequest(const SCI_ST* puart)
{
    SuccessFailType result = SUCCESS;
    ATReqAllType request;

    DEBUG_VALUE1 ( APP_SMS_SWI_DBG, MOD_APP_SMS, "HaPendConcSmsSendReq: mode %d", gAppSmsContext.fct.smsConcSend.mode);

    // get next state to be handled
    switch(gAppSmsContext.fct.smsConcSend.mode)
    {
        case APP_SMS_enConcSmsNumberSend:
        {
            request.common.req_type         = AT_REQ_SMS_CONC_SEND;
            request.pmsdex.mode             = AT_SMSCONC_SET_DESTINATION;
            // copy receiver number
            (void)memset( request.pmsdex.phone_number, 0, sizeof( request.pmsdex.phone_number));
            (void)memcpy( request.pmsdex.phone_number,
                    gAppSmsContext.fct.smsConcSend.telNr,
                    gAppSmsContext.fct.smsConcSend.telNrLen);

            request.pmsdex.phone_number_len   = gAppSmsContext.fct.smsConcSend.telNrLen;

            if(SUCCESS != AT_Cmd_Handler_Sms(&request,puart))
            {
                (void)ERM_ReleaseUartForSMS();
            }
            break;
        }
        case APP_SMS_enConcSmsMessagesSend:
        {
            request.common.req_type         = AT_REQ_SMS_CONC_SEND;
            request.pmsdex.mode             = AT_SMSCONC_SEND;

            if(/*AT_MAX_SMS_LEN_TOTAL*/AT_MAX_CONC_SMS_LEN < (gAppSmsContext.fct.smsConcSend.msgLen - gAppSmsContext.fct.smsConcSend.offset))
            {
                request.pmsdex.message_len = AT_MAX_CONC_SMS_LEN/*AT_MAX_SMS_LEN_TOTAL*/;
            }
            else
            {
                request.pmsdex.message_len = gAppSmsContext.fct.smsConcSend.msgLen - gAppSmsContext.fct.smsConcSend.offset;
            }

            // copy message text
            (void)memset( request.pmsdex.message, 0, sizeof( request.pmsdex.message));
            (void)memcpy( request.pmsdex.message,
                    gAppSmsContext.fct.smsConcSend.msg + gAppSmsContext.fct.smsConcSend.offset,
                    request.pmsdex.message_len);

            gAppSmsContext.fct.smsConcSend.offset = gAppSmsContext.fct.smsConcSend.offset + request.pmsdex.message_len;


            if(SUCCESS != AT_Cmd_Handler_Sms(&request,puart))
            {
                (void)ERM_ReleaseUartForSMS();
            }
            break;
        }
        case APP_SMS_enConcSmsFinalParamSend:
        {

            request.common.req_type         = AT_REQ_SMS_CONC_SEND;
            request.pmsdex.mode             = AT_SMSCONC_SEND_END;

            if(SUCCESS != AT_Cmd_Handler_Sms(&request,puart))
            {
                (void)ERM_ReleaseUartForSMS();
            }
            break;
        }
        default:
            DEBUG_VALUE1 (  APP_SMS_SWI_ERR, MOD_APP_SMS, "HaPendConcSmsSend: invalid state %d", gAppSmsContext.fct.smsConcSend.mode);

            result = FAIL;
            break;
    }

    if(result != SUCCESS)
    {
    	DEBUG_TEXT( APP_SMS_SWI_ERR, MOD_APP_SMS, "HaPendConcSmsSend: error sending SMS read req");

        _APP_SMS_vSendResultReleaseContext(APP_COMMON_RSP_ERROR_AT_SEND);
    }

    return result;
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SMS_sfSendPendingSmsReadRequest             */
/**
    \brief      handle next step requesting sms

    \brief      Description:\n
                handle next step requesting sms \n

    \param      puart
                const SCI_ST*

    \return     SuccessFailType
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
static SuccessFailType _APP_SMS_sfSendPendingSmsReadRequest(const SCI_ST* puart)
{
    SuccessFailType result = SUCCESS;

    DEBUG_VALUE1 ( APP_SMS_SWI_DBG, MOD_APP_SMS, "HaPendSmsReadReq: idx %d", gAppSmsContext.fct.smsRecvd.nextIndex);

    // get next state to be handled
    switch(gAppSmsContext.fct.smsRecvd.state)
    {
        case APP_SMS_enSmsRequestEntries:
        {
            // check if next sms to be read is in read or unread folder
            ATSmsTypeEnum type  = APP_SMS_enGetSmsType(gAppSmsContext.fct.smsRecvd.nextIndex);
            Uint16Type index    = APP_SMS_u16GetSmsTypeIndex(gAppSmsContext.fct.smsRecvd.nextIndex);

            if( type == AT_SMS_TYPE_INVALID )
            {
                DEBUG_VALUE2(  APP_SMS_SWI_ERR, MOD_APP_SMS, "HaPendSmsReadReq: invalid range idx %d type %d", index, type);
                result = FAIL;
            }
            else
            {
                result = _APP_SMS_sfRequestSms(puart, type, index, BT_FALSE);
                if(result == SUCCESS)
                {
                    gAppSmsContext.fct.smsRecvd.lastRequestedSmsType = type;
                }
            }
            break;
        }
        default:
            DEBUG_VALUE1 (  APP_SMS_SWI_ERR, MOD_APP_SMS, "HaPendSmsReadReq: invalid state %d", gAppSmsContext.fct.smsRecvd.state);

            result = FAIL;
            break;
    }


    if(result != SUCCESS)
    {
    	DEBUG_TEXT( APP_SMS_SWI_ERR, MOD_APP_SMS, "HaPendSmsReadReq: error sending SMS read req");

        _APP_SMS_vSendResultReleaseContext(APP_COMMON_RSP_ERROR_AT_SEND);
    }


    return result;
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SMS_sfSendPendingSmsReadPreviewRequest             */
/**
    \brief      handle next step requesting sms preview

    \brief      Description:\n
                handle next step requesting sms preview \n

    \param      puart
                const SCI_ST*

    \return     SuccessFailType
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
static SuccessFailType _APP_SMS_sfSendPendingSmsReadPreviewRequest(const SCI_ST* puart)
{
    SuccessFailType result = SUCCESS;

    DEBUG_VALUE1 ( APP_SMS_SWI_DBG, MOD_APP_SMS, "HaPendSmsReadPreviewReq: idx %d", gAppSmsContext.fct.smsPreview.nextIndex);

    // get next state to be handled
    switch(gAppSmsContext.fct.smsPreview.state)
    {
        case APP_SMS_enSmsRequestEntries:
        {
            // check if next sms to be read is in read or unread folder
            ATSmsTypeEnum type  = APP_SMS_enGetSmsType(gAppSmsContext.fct.smsPreview.nextIndex);
            Uint16Type index    = APP_SMS_u16GetSmsTypeIndex(gAppSmsContext.fct.smsPreview.nextIndex);

            if( type == AT_SMS_TYPE_INVALID )
            {
                DEBUG_VALUE2 (  APP_SMS_SWI_ERR, MOD_APP_SMS, "HaPendSmsReadPreviewReq: invalid range index %d type %d", index, type);
                result = FAIL;
            }
            else
            {
                result = _APP_SMS_sfRequestSmsPreview(puart, type, index);
                if(result == SUCCESS)
                {
                    gAppSmsContext.fct.smsPreview.lastRequestedSmsType = type;
                }
            }
            break;
        }
        default:
            DEBUG_VALUE1 (  APP_SMS_SWI_ERR, MOD_APP_SMS, "HaPendSmsReadPreviewReq: invalid state %d", gAppSmsContext.fct.smsPreview.state);

            result = FAIL;
            break;
    }


    if(result != SUCCESS)
    {
    	DEBUG_TEXT(   APP_SMS_SWI_ERR, MOD_APP_SMS, "HaPendSmsReadPreviewReq: error sending SMS read prev. req");

        _APP_SMS_vSendResultReleaseContext(APP_COMMON_RSP_ERROR_AT_SEND);
    }


    return result;
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SMS_sfSendPendingSmsDeleteRequest             */
/**
    \brief      handle next step deleting sms

    \brief      Description:\n
                handle next step deleting sms \n

    \param      puart
                const SCI_ST*

    \return     SuccessFailType
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
static SuccessFailType _APP_SMS_sfSendPendingSmsDeleteRequest(const SCI_ST* puart)
{
    SuccessFailType result = SUCCESS;

    switch(gAppSmsContext.fct.smsDelete.state)
    {
        case APP_SMS_DELETE_NOT_SENT:
        {
            // check if next sms to be read is in read or unread folder
            ATSmsTypeEnum type  = APP_SMS_enGetSmsType(gAppSmsContext.fct.smsDelete.index);
            Uint16Type index    = APP_SMS_u16GetSmsTypeIndex(gAppSmsContext.fct.smsDelete.index);

            DEBUG_VALUE2 ( APP_SMS_SWI_DBG, MOD_APP_SMS, "SendPendSmsDeleteReq: type %d, index %d", type, index);

            if( type == AT_SMS_TYPE_INVALID )
            {
                DEBUG_VALUE2 (  APP_SMS_SWI_ERR, MOD_APP_SMS, "SendPendSmsDeleteReq: Out Of Range type %d, index %d", type, index);
                result = FAIL;
            }
            else
            {
                result = _APP_SMS_sfSendSmsDeleteATRequest( puart, type, index);
                if(result == SUCCESS)
                {
                    gAppSmsContext.fct.smsDelete.lastSmsType    = type;
                    gAppSmsContext.fct.smsDelete.state          = APP_SMS_DELETE_SENT;
                }
            }

            break;
        }
        default:

            DEBUG_VALUE1 (  APP_SMS_SWI_ERR, MOD_APP_SMS, "SendPendSmsDeleteReq: invalid state %d",
                            gAppSmsContext.fct.smsDelete.state);

            result = FAIL;
            break;
    }


    if(result != SUCCESS)
    {
    	DEBUG_TEXT(   APP_SMS_SWI_ERR, MOD_APP_SMS, "SendPendSmsDeleteReq: critical error");

        _APP_SMS_vSendResultReleaseContext(APP_COMMON_RSP_ERROR_AT_SEND);
    }


    return result;
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SMS_sfSendPendingMarkAsReadRequest                      */
/**
    \brief      handle next step deleting sms

    \brief      Description:\n
                handle next step deleting sms \n

    \param      puart
                const SCI_ST*

    \return     SuccessFailType
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
static SuccessFailType _APP_SMS_sfSendPendingMarkAsReadRequest(const SCI_ST* puart)
{

    SuccessFailType result = SUCCESS;

    DEBUG_VALUE2 ( APP_SMS_SWI_DBG, MOD_APP_SMS, "SendPendMarkAsReadReq: type %d, index %d",
                    gAppSmsContext.fct.markAsRead.type,
                    gAppSmsContext.fct.markAsRead.index);

    switch(gAppSmsContext.fct.markAsRead.state)
    {
        case APP_SMS_MARK_AS_READ_NOT_SENT:
            result = _APP_SMS_sfRequestSms( puart,
                                            gAppSmsContext.fct.markAsRead.type,
                                            gAppSmsContext.fct.markAsRead.index,
                                            BT_TRUE);

            gAppSmsContext.fct.markAsRead.state = APP_SMS_MARK_AS_READ_SENT;
            break;
        default:
            DEBUG_VALUE1(  APP_SMS_SWI_ERR, MOD_APP_SMS,
                            "SendPendMarkAsReadReq: invalid state %d",
                            gAppSmsContext.fct.smsDelete.state);

            result = FAIL;
            break;
    }


    if(result != SUCCESS)
    {
    	DEBUG_TEXT(   APP_SMS_SWI_ERR, MOD_APP_SMS, "SendPendMarkAsReadReq: critical error");

        _APP_SMS_vSendResultReleaseContext(APP_COMMON_RSP_ERROR_AT_SEND);
    }

    return result;
}


void _APP_SMS_vOnSmsInitResult(APP_SMS_enResult result)
{
    SuccessFailType sF = SUCCESS;

    if( result == APP_SMS_enResultOk)
    {
        switch( gAppSmsContext.fct.smsInit.state )
        {
            case APP_SMS_enSetCharset:
            case APP_SMS_enStartSynchro:
            case APP_SMS_enWaitSynchro:
            case APP_SMS_enFinishedSynchro:
            case APP_SMS_enGetFolderSizes:
                (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_SMS_FCT_PENDING, MSEC(10), 0);
                break;
            case APP_SMS_enInitFinished:
                /* we are finished */
                gAppSmsContext.bInitSmsFinished = BT_TRUE;
                gAppSmsContext.supportState     = APP_SMS_INITIALIZED_AND_SUPPORTED;
                _APP_SMS_vNotifyApps(APP_SMS_SUPPORTED);
                _APP_SMS_vReleaseFctContext();
                break;
            default:
    	        DEBUG_VALUE1(   APP_SMS_SWI_ERR, MOD_APP_SMS,
    	                        "OnSmsInitRes: rcvd AT_OK in unsup state: %02x",
    	                        gAppSmsContext.fct.smsInit.state );
                sF = FAIL;
                break;
        }
    }
    else
    {
    	DEBUG_VALUE1(   APP_SMS_SWI_ERR, MOD_APP_SMS,
    	                "OnSmsInitRes: rcvd AT_ERROR during state: %02x",
    	                gAppSmsContext.fct.smsInit.state );

        // #1947 When we try to start SMS sync via AT*PMLS during an ongoing call, we will receive
        // an error. In this case we need to retry sms initialization when all calls are idle again
        if(BT_FALSE == APP_PHONE_AT_PNCC_bAllCallStatesIdle())
        {
    	    DEBUG_TEXT(APP_SMS_SWI_ERR, MOD_APP_SMS, "OnSmsInitRes: probably received error cause of ongoing call, retrying later");
        }

        sF = FAIL;
    }

    if(sF != SUCCESS)
    {
        _APP_SMS_vNotifyApps(APP_SMS_UNSUPPORTED);
        _APP_SMS_vReleaseFctContext();
    }
}

void _APP_SMS_vOnReadSmsResult(APP_SMS_enResult result)
{
    APP_COMMON_enResponseCode response  = APP_COMMON_RSP_ERROR;
    BooleanType               finished  = BT_FALSE;

    if( result == APP_SMS_enResultOk)
    {
        switch(gAppSmsContext.fct.smsRecvd.state)
        {
            case APP_SMS_enSmsRequestEntries:
                // check if this was the last sms
                if(gAppSmsContext.fct.smsRecvd.nextIndex >= gAppSmsContext.fct.smsRecvd.endIndex)
                {
                    /* we are finished requesting sms */
                    gAppSmsContext.fct.smsRecvd.state = APP_SMS_enSmsRequestFinished;

                    finished = BT_TRUE;
                    response = APP_COMMON_RSP_OK;
                }
                else
                {
                    /* go on and send the next sms request */
                    (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_SMS_FCT_PENDING, MSEC(10), 0);
                }
                break;

            default:

    	        DEBUG_VALUE1(   APP_SMS_SWI_ERR, MOD_APP_SMS, "OnReadSmsRes: invalid state %02x",
    	                        gAppSmsContext.fct.smsRecvd.state);

                finished = BT_TRUE;
                response = APP_COMMON_RSP_ERROR;
                break;
        }
    }
    else
    {
        finished = BT_TRUE;
        response = APP_COMMON_RSP_ERROR;

    	DEBUG_VALUE1(   APP_SMS_SWI_WNG, MOD_APP_SMS, "OnReadSmsRes: rcvd AT_ERROR during state:%02x",
    	                gAppSmsContext.fct.smsRecvd.state );
    }

    if(finished)
    {
        _APP_SMS_vSendResultReleaseContext(response);
    }
}

void _APP_SMS_vOnReadSmsPreviewResult(APP_SMS_enResult result)
{
    APP_COMMON_enResponseCode response  = APP_COMMON_RSP_ERROR;
    BooleanType               finished  = BT_FALSE;

    if( result == APP_SMS_enResultOk)
    {
        switch(gAppSmsContext.fct.smsPreview.state)
        {
            case APP_SMS_enSmsRequestEntries:
                // check if this was the last sms
                if(gAppSmsContext.fct.smsPreview.nextIndex >= gAppSmsContext.fct.smsPreview.endIndex)
                {
                    /* we are finished requesting sms */
                    gAppSmsContext.fct.smsPreview.state = APP_SMS_enSmsRequestFinished;

                    finished = BT_TRUE;
                    response = APP_COMMON_RSP_OK;
                }
                else
                {
                    /* go on and send the next sms request */
                    (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_SMS_FCT_PENDING, MSEC(10), 0);
                }
                break;

            default:

    	        DEBUG_VALUE1(   APP_SMS_SWI_ERR, MOD_APP_SMS,
    	                        "OnReadSmsPreviewRes: invalid state %02x",
    	                        gAppSmsContext.fct.smsPreview.state);

                finished = BT_TRUE;
                response = APP_COMMON_RSP_ERROR;
                break;
        }
    }
    else
    {
        finished = BT_TRUE;
        response = APP_COMMON_RSP_ERROR;

    	DEBUG_VALUE1(   APP_SMS_SWI_WNG, MOD_APP_SMS,
    	                "OnReadSmsPreviewRes: rcvd AT_ERROR during state:%02x",
    	                gAppSmsContext.fct.smsPreview.state );
    }

    if(finished)
    {
        _APP_SMS_vSendResultReleaseContext(response);
    }
}


void _APP_SMS_vOnSendSmsResult(APP_SMS_enResult result)
{
    if(result == APP_SMS_enResultOk)
    {
        _APP_SMS_vSendResultReleaseContext(APP_COMMON_RSP_OK);
    }
    else
    {
        _APP_SMS_vSendResultReleaseContext(APP_COMMON_RSP_ERROR);
    }
}

void _APP_SMS_vOnSendConcSmsResult(APP_SMS_enResult result)
{
    APP_COMMON_enResponseCode response  = APP_COMMON_RSP_ERROR;
    BooleanType               finished  = BT_FALSE;

    if(result == APP_SMS_enResultOk)
    {
        switch(gAppSmsContext.fct.smsConcSend.mode)
        {
            case APP_SMS_enConcSmsNumberSend:
            {
                gAppSmsContext.fct.smsConcSend.mode = APP_SMS_enConcSmsMessagesSend;
                (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_SMS_FCT_PENDING, MSEC(10), 0);
                response = APP_COMMON_RSP_OK;
                break;
            }
            case APP_SMS_enConcSmsMessagesSend:
            {
                if(gAppSmsContext.fct.smsConcSend.offset == gAppSmsContext.fct.smsConcSend.msgLen)
                {
                    gAppSmsContext.fct.smsConcSend.mode = APP_SMS_enConcSmsFinalParamSend;
                }

                (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_SMS_FCT_PENDING, MSEC(10), 0);
                response = APP_COMMON_RSP_OK;
                break;
            }
            case APP_SMS_enConcSmsFinalParamSend:
            {
                gAppSmsContext.fct.smsConcSend.mode = APP_SMS_enConcSmsFinalParamSend;

                /* we are finished */
                finished = BT_TRUE;
                response = APP_COMMON_RSP_OK;
                break;
            }
            default:

                DEBUG_VALUE1 (  APP_SMS_SWI_ERR, MOD_APP_SMS, "OnConcSmsSendRes: invalid state %d",
                                                            gAppSmsContext.fct.smsConcSend.mode);

                finished = BT_TRUE;
                response = APP_COMMON_RSP_ERROR;
                break;
        }
    }
    else
    {
        finished = BT_TRUE;
        response = APP_COMMON_RSP_ERROR;
    }

    if(finished)
    {
        _APP_SMS_vSendResultReleaseContext(response);
    }
}

void _APP_SMS_vOnDeleteSmsResult(APP_SMS_enResult result)
{
    APP_COMMON_enResponseCode response  = APP_COMMON_RSP_ERROR;
    BooleanType               finished  = BT_FALSE;

    if(result == APP_SMS_enResultOk)
    {
        switch(gAppSmsContext.fct.smsDelete.state)
        {
            case APP_SMS_DELETE_SENT:
                switch(gAppSmsContext.fct.smsDelete.lastSmsType)
                {
                    case AT_SMS_TYPE_UNREAD:
                        gAppSmsContext.unreadFolderSize--;
                        break;
                    case AT_SMS_TYPE_READ:
                        gAppSmsContext.readFolderSize--;
                        break;
                    default:
                        DEBUG_VALUE1 (  APP_SMS_SWI_ERR, MOD_APP_SMS, "OnDeleteSmsRes: invalid lastSmsType %d",
                                                                    gAppSmsContext.fct.smsDelete.lastSmsType);
                        break;
                }

                gAppSmsContext.fct.smsDelete.elements--;

                if(gAppSmsContext.fct.smsDelete.elements > 0)
                {
                    gAppSmsContext.fct.smsDelete.state = APP_SMS_DELETE_NOT_SENT;
                    (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_SMS_FCT_PENDING, MSEC(10), 0);
                }
                else
                {
                    /* we are finished */
                    finished = BT_TRUE;
                    response = APP_COMMON_RSP_OK;
                    _APP_SMS_vNotifyApps(APP_SMS_DELETED_SMS);
                }
                break;
            default:
                DEBUG_VALUE1 (  APP_SMS_SWI_ERR, MOD_APP_SMS, "OnDeleteSmsRes: invalid state %d",
                                                                    gAppSmsContext.fct.smsDelete.state);

                finished = BT_TRUE;
                response = APP_COMMON_RSP_ERROR;
                _APP_SMS_vNotifyApps(APP_SMS_DELETED_SMS);
                break;
        }

    }
    else
    {
        finished = BT_TRUE;
        response = APP_COMMON_RSP_ERROR;
    }


    if(finished)
    {
        _APP_SMS_vSendResultReleaseContext(response);
    }

}

void _APP_SMS_vOnMarkAsReadResult(APP_SMS_enResult result)
{
    if(result == APP_SMS_enResultOk)
    {
        if( gAppSmsContext.unreadFolderSize > 0 )
        {
            gAppSmsContext.unreadFolderSize--;
            gAppSmsContext.readFolderSize++;


            DEBUG_VALUE1( APP_SMS_SWI_ERR, MOD_APP_SMS,"Sms (%d) was marked read.",
                                                        gAppSmsContext.fct.markAsRead.index);
            DEBUG_VALUE2( APP_SMS_SWI_ERR, MOD_APP_SMS,"(New: unread: %d, read %d )",
                                                        gAppSmsContext.unreadFolderSize,
                                                        gAppSmsContext.readFolderSize);

            /* first release context so e.g. app pb bap can send status array
             * and then notify about new order of sms */
            _APP_SMS_vSendResultReleaseContext(APP_COMMON_RSP_OK);
            _APP_SMS_vNotifyApps(APP_SMS_NUMBER_OF_NEW_SMS_CHANGED);
        }
        else
        {
            _APP_SMS_vSendResultReleaseContext(APP_COMMON_RSP_OK);
    	    DEBUG_TEXT( APP_SMS_SWI_ERR, MOD_APP_SMS, "OnMarkAsReadRes: unreadFolderSize is less then 0");
            _APP_SMS_vSendResultReleaseContext(APP_COMMON_RSP_OK);
        }
    }
    else
    {
        _APP_SMS_vSendResultReleaseContext(APP_COMMON_RSP_ERROR);
    }
}



// ***************  INTERFACES FOR APPS ************************************* //

/*----------------------------------------------------------------------------*/
/* Function    : APP_SMS_rcSendSms                           */
/**
    \brief      sends sms

    \brief      Description:\n
                sends sms\n

    \param      cb
                void *

    \param      telNr
                const Uint8Type *

    \param      telNrLen
                Uint16Type

    \param      msg
                const Uint8Type *

    \param      msgLen
                Uint16Type

    \return     APP_COMMON_enResponseCode
                APP_COMMON_RSP_OK or APP_COMMON_RSP_ERROR
*/
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode APP_SMS_rcSendSms( void (*cb)(APP_COMMON_enResponseCode),
                                             const Uint8Type *telNr,
                                             Uint16Type telNrLen,
                                             const Uint8Type *msg,
                                             Uint16Type msgLen)
{
#ifdef UHV_ACTIVATE_FEATURE_SMS
    const SCI_ST *puart = NULL;
    ATReqAllType request;

    if(BT_FALSE == APP_SMS_bIsSmsInitialized())
    {
        return APP_COMMON_RSP_ERROR_NOT_INITIALIZED;
    }

    if(gAppSmsContext.fct.common.fctId != APP_SMS_enInvalidFct)
    {
        return APP_COMMON_RSP_ERROR_BUSY;
    }

    puart = ERM_GetUartForSMS();
    if( !puart )
    {
        return APP_COMMON_RSP_ERROR_BUSY;
    }
    request.common.req_type = AT_REQ_SMS_SEND;

    telNrLen = ( telNrLen > AT_SMS_PHONENUM_MAX_LEN ) ?
                            AT_SMS_PHONENUM_MAX_LEN : telNrLen;
    msgLen = ( msgLen > AT_MAX_SMS_LEN_TOTAL ) ?
                        AT_MAX_SMS_LEN_TOTAL : msgLen;

    // copy receiver number
    (void)memset( request.pmsd.phone_number, 0, sizeof( request.pmsd.phone_number));
    (void)memcpy( request.pmsd.phone_number, telNr, telNrLen);

    // copy message text
    (void)memset( request.pmsd.message, 0, sizeof( request.pmsd.message));
    (void)memcpy( request.pmsd.message, msg, msgLen);

    request.pmsd.message_len        = msgLen;
    request.pmsd.phone_number_len   = telNrLen;

    if(SUCCESS != AT_Cmd_Handler_Sms(&request,puart))
    {
        (void)ERM_ReleaseUartForSMS();
        return APP_COMMON_RSP_ERROR_AT_SEND;
    }

    gAppSmsContext.fct.common.fctId      = APP_SMS_enSendSms;
    gAppSmsContext.fct.common.callback   = cb;

    return APP_COMMON_RSP_OK;

#else
    return APP_COMMON_RSP_ERROR_NOT_INITIALIZED;
#endif
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_SMS_rcSendConcSms                           */
/**
    \brief      sends sms

    \brief      Description:\n
                sends sms\n

    \param      cb
                void *

    \param      telNr
                const Uint8Type *

    \param      telNrLen
                Uint16Type

    \param      msg
                const Uint8Type *

    \param      msgLen
                Uint16Type

    \return     APP_COMMON_enResponseCode
                APP_COMMON_RSP_OK or APP_COMMON_RSP_ERROR
*/
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode APP_SMS_rcSendConcSms( void (*cb)(APP_COMMON_enResponseCode),
                                                 const Uint8Type *telNr,
                                                 Uint16Type telNrLen,
                                                 const Uint8Type *msg,
                                                 Uint16Type msgLen)
{
#ifdef UHV_ACTIVATE_FEATURE_SMS
    if(BT_FALSE == APP_SMS_bIsSmsInitialized())
    {
        return APP_COMMON_RSP_ERROR_NOT_INITIALIZED;
    }

    if(gAppSmsContext.fct.common.fctId != APP_SMS_enInvalidFct)
    {
        return APP_COMMON_RSP_ERROR_BUSY;
    }
	DEBUG_VALUE2( APP_SMS_SWI_TRC,  MOD_APP_SMS, "Conc SMS msgLen : %d, telLen : %d", msgLen, telNrLen);

    telNrLen = ( telNrLen > AT_SMS_PHONENUM_MAX_LEN ) ?
                            AT_SMS_PHONENUM_MAX_LEN : telNrLen;
    msgLen = ( msgLen > AT_MAX_CONC_SMS_LEN_TOTAL ) ?
                        AT_MAX_CONC_SMS_LEN_TOTAL : msgLen;

    	    DEBUG_VALUE2( APP_SMS_SWI_TRC, MOD_APP_SMS, "Conc SMS msgLen : %d, telLen : %d", msgLen, telNrLen);

    // copy receiver number
    (void)memset( gAppSmsContext.fct.smsConcSend.telNr, 0, sizeof( gAppSmsContext.fct.smsConcSend.telNr));
    (void)memcpy( gAppSmsContext.fct.smsConcSend.telNr, telNr, telNrLen);

    // copy params and start request
    gAppSmsContext.fct.common.fctId             = APP_SMS_enSendConcSms;
    gAppSmsContext.fct.common.callback          = cb;
    gAppSmsContext.fct.smsConcSend.mode         = APP_SMS_enConcSmsNumberSend;
//    gAppSmsContext.fct.smsConcSend.telNr        = telNr;
    gAppSmsContext.fct.smsConcSend.telNrLen     = telNrLen;
    gAppSmsContext.fct.smsConcSend.msg          = msg;
    gAppSmsContext.fct.smsConcSend.msgLen       = msgLen;
    gAppSmsContext.fct.smsConcSend.offset       = 0;

    (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_SMS_FCT_PENDING, MSEC(0), 0);

    return APP_COMMON_RSP_OK;
#else
    return APP_COMMON_RSP_ERROR_NOT_INITIALIZED;
#endif
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_SMS_rcReadSms                                  */
/**
    \brief      interface method for applications to read sms from ck5050

    \brief      Description:\n
                interface method for applications to read sms from ck5050\n

    \return     APP_COMMON_enResponseCode
                OK or ERROR ...
*/
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode APP_SMS_rcReadSms( void (*cb)(APP_COMMON_enResponseCode),
                                             Uint16Type start,
                                             Uint16Type elements)
{
#ifdef UHV_ACTIVATE_FEATURE_SMS
    Uint16Type totalSms = gAppSmsContext.unreadFolderSize + gAppSmsContext.readFolderSize;

    if(BT_FALSE == APP_SMS_bIsSmsInitialized())
    {
        return APP_COMMON_RSP_ERROR_NOT_INITIALIZED;
    }
    if(gAppSmsContext.fct.common.fctId != APP_SMS_enInvalidFct)
    {
        return APP_COMMON_RSP_ERROR_BUSY;
    }

    if(APP_SMS_enGetSmsType(start) == AT_SMS_TYPE_INVALID)
    {
        return APP_COMMON_RSP_ERROR_SMS_INVALID_INDEX;
    }

    if( (start + elements) > totalSms)
    {
        elements = totalSms - start + 1;
    }

    if( elements > MAX_REQUESTED_SMS_ENTRIES)
    {
        elements = MAX_REQUESTED_SMS_ENTRIES;
    }

    // copy params and start request
    gAppSmsContext.fct.common.fctId             = APP_SMS_enReadSms;
    gAppSmsContext.fct.common.callback          = cb;
    gAppSmsContext.fct.smsRecvd.startIndex      = start;
    gAppSmsContext.fct.smsRecvd.nextIndex       = start;
    gAppSmsContext.fct.smsRecvd.endIndex        = start + elements;
    gAppSmsContext.fct.smsRecvd.state           = APP_SMS_enSmsRequestEntries;

    (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_SMS_FCT_PENDING, MSEC(0), 0);

    return APP_COMMON_RSP_OK;
#else
    return APP_COMMON_RSP_ERROR_NOT_INITIALIZED;
#endif
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_SMS_rcReadSmsPreview                                  */
/**
    \brief      interface method for applications to read sms preview ( origin
                and date/time only ) from ck5050

    \brief      Description:\n
                interface method for applications to read sms from ck5050\n

    \return     APP_COMMON_enResponseCode
                OK or ERROR ...
*/
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode APP_SMS_rcReadSmsPreview( void (*cb)(APP_COMMON_enResponseCode),
                                             Uint16Type start,
                                             Uint16Type elements)
{
#ifdef UHV_ACTIVATE_FEATURE_SMS
    Uint16Type totalSms = gAppSmsContext.unreadFolderSize + gAppSmsContext.readFolderSize;

    if(BT_FALSE == APP_SMS_bIsSmsInitialized())
    {
        return APP_COMMON_RSP_ERROR_NOT_INITIALIZED;
    }
    if(gAppSmsContext.fct.common.fctId != APP_SMS_enInvalidFct)
    {
        return APP_COMMON_RSP_ERROR_BUSY;
    }

    if(APP_SMS_enGetSmsType(start) == AT_SMS_TYPE_INVALID)
    {
        return APP_COMMON_RSP_ERROR;
    }

    if( (start + elements) > totalSms)
    {
        elements = totalSms - start + 1;
    }

    if( elements > MAX_REQUESTED_SMS_ENTRIES)
    {
        elements = MAX_REQUESTED_SMS_ENTRIES;
    }

    // copy params and start request
    gAppSmsContext.fct.common.fctId             = APP_SMS_enReadSmsPreview;
    gAppSmsContext.fct.common.callback          = cb;
    gAppSmsContext.fct.smsPreview.startIndex    = start;
    gAppSmsContext.fct.smsPreview.nextIndex     = start;
    gAppSmsContext.fct.smsPreview.endIndex      = start + elements;
    gAppSmsContext.fct.smsPreview.state         = APP_SMS_enSmsRequestEntries;

    (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_SMS_FCT_PENDING, MSEC(0), 0);

    return APP_COMMON_RSP_OK;
#else
    return APP_COMMON_RSP_ERROR_NOT_INITIALIZED;
#endif
}



/*----------------------------------------------------------------------------*/
/* Function    : APP_SMS_rcDeleteSms                                          */
/**
    \brief      interface method for applications to delete sms from ck5050

    \brief      Description:\n
                interface method for applications to delete sms from ck5050\n

    \return     APP_COMMON_enResponseCode
                OK or ERROR ...
*/
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode APP_SMS_rcDeleteSms( void (*cb)(APP_COMMON_enResponseCode),
                                               Uint16Type start,
                                               Uint16Type elements,
                                               APP_SMS_enCallId caller)
{
#ifdef UHV_ACTIVATE_FEATURE_SMS
    Uint16Type totalSms = gAppSmsContext.unreadFolderSize + gAppSmsContext.readFolderSize;

    if(BT_FALSE == APP_SMS_bIsSmsInitialized())
    {
        return APP_COMMON_RSP_ERROR_NOT_INITIALIZED;
    }
    if(gAppSmsContext.fct.common.fctId != APP_SMS_enInvalidFct)
    {
        return APP_COMMON_RSP_ERROR_BUSY;
    }

    if(APP_SMS_enGetSmsType(start) == AT_SMS_TYPE_INVALID)
    {
        return APP_COMMON_RSP_ERROR;
    }

    if( (start + elements) > totalSms)
    {
        elements = totalSms - start + 1;
    }

    // copy params and start request
    gAppSmsContext.fct.common.fctId         = APP_SMS_enDeleteSms;
    gAppSmsContext.fct.common.callback      = cb;
    gAppSmsContext.fct.smsDelete.state      = APP_SMS_DELETE_NOT_SENT;
    gAppSmsContext.fct.smsDelete.index      = start;
    gAppSmsContext.fct.smsDelete.elements   = elements;

    (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_SMS_FCT_PENDING, MSEC(0), 0);

    switch (caller)
    {
        case APP_SMS_enRspSd:
        case APP_SMS_enRspVcr:
            /* index on CK5050+ starts with 1, but BAP index started with 0 */
            APP_PHONEBOOK_BAP_vDeleteSms(start);
            break;
        case APP_SMS_enRspBap:
        default:
            break;
    }

    return APP_COMMON_RSP_OK;
#else
    return APP_COMMON_RSP_ERROR_NOT_INITIALIZED;
#endif
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_SMS_rcMarkAsRead                                         */
/**
    \brief      interface method for applications to delete sms from ck5050

    \brief      Description:\n
                interface method for applications to delete sms from ck5050\n

    \return     APP_COMMON_enResponseCode
                OK or ERROR ...
*/
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode APP_SMS_rcMarkAsRead( void (*cb)(APP_COMMON_enResponseCode),
                                                Uint16Type index)
{
#ifdef UHV_ACTIVATE_FEATURE_SMS
    ATSmsTypeEnum type = AT_SMS_TYPE_INVALID;

    if(BT_FALSE == APP_SMS_bIsSmsInitialized())
    {
        return APP_COMMON_RSP_ERROR_NOT_INITIALIZED;
    }
    if(gAppSmsContext.fct.common.fctId != APP_SMS_enInvalidFct)
    {
        return APP_COMMON_RSP_ERROR_BUSY;
    }

    type  = APP_SMS_enGetSmsType(index);
    index = APP_SMS_u16GetSmsTypeIndex(index);

    if(type != AT_SMS_TYPE_UNREAD)
    {
        // give specific error message so app like BAP are able to handle
        // problems
        if( AT_SMS_TYPE_READ == type )
        {
            return APP_COMMON_RSP_ERROR_SMS_ALREADY_OF_TYPE_READ;
        }
        return APP_COMMON_RSP_ERROR_SMS_INVALID_INDEX;
    }

    // copy params and start request
    gAppSmsContext.fct.common.fctId     = APP_SMS_enMarkAsRead;
    gAppSmsContext.fct.common.callback  = cb;
    gAppSmsContext.fct.markAsRead.state = APP_SMS_MARK_AS_READ_NOT_SENT;
    gAppSmsContext.fct.markAsRead.type  = AT_SMS_TYPE_UNREAD;
    gAppSmsContext.fct.markAsRead.index = index;

    (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_SMS_FCT_PENDING, MSEC(0), 0);

    return APP_COMMON_RSP_OK;
#else
    return APP_COMMON_RSP_ERROR_NOT_INITIALIZED;
#endif
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_SMS_bIsSmsInitialized                                    */
/**
    \brief      returns whether SMS is supported and initialization has been
                finished

    \brief      Description: returns whether SMS is supported and initialization has been
                finished \n

    \return     BooleanType
*/
/*----------------------------------------------------------------------------*/
BooleanType APP_SMS_bIsSmsInitialized()
{
    return gAppSmsContext.bInitSmsFinished;
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_SMS_u16GetNumReadSms                                     */
/**
    \brief      returns number of read sms

    \brief      Description: returns number of read sms\n

    \return     Uint16Type
*/
/*----------------------------------------------------------------------------*/
Uint16Type APP_SMS_u16GetNumReadSms()
{
    return gAppSmsContext.readFolderSize;
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_SMS_u16GetNumUnReadSms                                     */
/**
    \brief      returns number of unread sms

    \brief      Description: returns number of unread sms\n

    \return     Uint16Type
*/
/*----------------------------------------------------------------------------*/
Uint16Type APP_SMS_u16GetNumUnreadSms()
{
    return gAppSmsContext.unreadFolderSize;
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_SMS_enGetSmsSupportState                                 */
/**
    \brief      returns current state of sms support

    \brief      Description: returns surrent state of sms support\n

    \return     APP_SMS_enSupportState
*/
/*----------------------------------------------------------------------------*/
APP_SMS_enSupportState APP_SMS_enGetSmsSupportState()
{
    return gAppSmsContext.supportState;
}


/*----------------------------------------------------------------------------*/
/* Function    : APP_SMS_enGetSmsType                                         */
/**
    \brief      returns type and index of next sms to be requested

    \brief      Description:\n
                returns type and index of next sms to be requested \n

    \param      index
                Uint16Type

    \return     ATSmsTypeEnum
*/
/*----------------------------------------------------------------------------*/
ATSmsTypeEnum APP_SMS_enGetSmsType( Uint16Type index )
{
    /* sizes of sms folders into tmp variable */
    Uint16Type unreadSms    = gAppSmsContext.unreadFolderSize;
    Uint16Type readSms      = gAppSmsContext.readFolderSize;

    // #2328: index 0 is not allowed!!!
    if((0 < index) && (index <= unreadSms ))
    {
        return AT_SMS_TYPE_UNREAD;
    }
    else if((0 < index) && (index <= (unreadSms + readSms)))
    {
        return AT_SMS_TYPE_READ;
    }
    else
    {
        // out of range
    }

    return AT_SMS_TYPE_INVALID;
}


/*----------------------------------------------------------------------------*/
/* Function    : APP_SMS_u16GetSmsTypeIndex                                   */
/**
    \brief      returns type and index of next sms to be requested

    \brief      Description:\n
                returns type and index of next sms to be requested \n

    \param      index
                Uint16Type

    \return     Uint16Type
*/
/*----------------------------------------------------------------------------*/
Uint16Type APP_SMS_u16GetSmsTypeIndex( Uint16Type index )
{
    /* sizes of sms folders into tmp variable */
    Uint16Type unreadSms    = gAppSmsContext.unreadFolderSize;
    Uint16Type readSms      = gAppSmsContext.readFolderSize;

    // #2328: index 0 is not allowed!!!
    if((0 < index) && (index <= unreadSms))
    {
        return index;
    }
    else if((0 < index) && (index <= (unreadSms + readSms)))
    {
        return (index-unreadSms);
    }
    else
    {
        // out of range
    }

    return (Uint16Type)-1;
}
/*----------------------------------------------------------------------------*/
/* Function    : APP_SMS_pGetPmrdContext                          */
/**
    \brief      returns pointer to current pmrd context

    \brief      Description:\n
                returns pointer to current pmrd context

    \param      index
                Uint16Type

    \return     const AppSmsReadSmsType *
*/
/*----------------------------------------------------------------------------*/
const AppSmsReadSmsType *APP_SMS_pGetPmrdContext(Uint16Type index)
{
    if( (gAppSmsContext.fct.common.fctId == APP_SMS_enReadSms ) &&
        (index < MAX_REQUESTED_SMS_ENTRIES) )
    {
        return &gAppSmsContext.fct.smsRecvd.sms[index];
    }
    else if( (gAppSmsContext.fct.common.fctId == APP_SMS_enReadSmsPreview) &&
        (index < MAX_REQUESTED_SMS_ENTRIES) )
    {
        return &gAppSmsContext.fct.smsPreview.sms[index];
    }
    else
    {
        return NULL;
    }
}

void _APP_SMS_vNotifyApps(APP_SMS_enNotificationId event)
{

    DEBUG_VALUE1( APP_SMS_SWI_TRC, MOD_APP_SMS,"NotifyApps (%d)",
                                                        event);
    APP_PHONEBOOK_BAP_vOnAppSmsNotification(event);
    APP_SD_vOnAppSmsNotification(event);
}


/*----------------------------------------------------------------------------*/
/* Function    : APP_SMS_vOnPhoneNotification                                   */
/**
    \brief      handle Notifications ofAPP_PHONE

    \brief      Description:\n
                handle Notifications of APP_PHONE \n

    \return     void
*/
/*----------------------------------------------------------------------------*/
void APP_SMS_vOnPhoneNotification(APP_PHONE_enNotificationId notId)
{
    switch(notId)
    {
        case APP_PHONE_CALL_STATE_CHANGED:
            _APP_SMS_vCheckSmsInitialization();
            break;
        default:
            /* ignore notification */
            break;
    }
}


void _APP_SMS_vCheckSmsInitialization()
{
    /* check if sms is supported but not yet intiialized */
    if(APP_SMS_NOT_INITIALIZED_AND_SUPPORTED != gAppSmsContext.supportState)
    {   /* not supported or already initialized or not yet received PMST:x */
        return;
    }

    /* check if all calls are idle to avoid ERROR for AT*PMLS */
    if(BT_FALSE == APP_PHONE_AT_PNCC_bAllCallStatesIdle())
    {   /* not interesting */
        return;
    }
    /* check that we are not currently processing sms init */
    if(APP_SMS_enSmsInit    == gAppSmsContext.fct.common.fctId ||
       APP_SMS_enInvalidFct != gAppSmsContext.fct.common.fctId )
    {   /* already in process or currently busy */
        return;
    }

    /* else start intialization (probably again) */
    gAppSmsContext.fct.common.fctId     = APP_SMS_enSmsInit;
    gAppSmsContext.fct.smsInit.state    = APP_SMS_enRestartInit;
    (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_SMS_FCT_PENDING, MSEC(10), 0);

    DEBUG_TEXT(APP_SMS_SWI_TRC, MOD_APP_SMS, "CheckSmsInit: starting initialization");

}
/* End Of File APP_PHONE_C1.c */


