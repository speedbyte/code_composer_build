/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     APP_PHONE_BAP_C1.c
*-----------------------------------------------------------------------------
* Module Name:    Telephone Application
*-----------------------------------------------------------------------------
* Description:    Handles all phone specific requests from BAP  
*-----------------------------------------------------------------------------
* $Date: 2011-06-07 16:06:17 +0200 (Di, 07 Jun 2011) $
* $Rev: 20049 $
* $Author: nikolov $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_PHONE_BAP/trunk/src/APP_PHONE_BAP_INDICATION_C1.c $
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
#include "APP_PHONE_BAP.h"
#include "APP_PHONE_BAP_CI.h"
#include "APP_PHONE_BAP_ERM.h"
#include "APP_PHONE_BAP_REQUEST.h"
//#include "APP_PHONE_BAP_REQUEST_USER_LIST.h"
#include "APP_PHONE_AT_PNCC.h"
#include "APP_COMMON_Utils.h"
#include "BAP_Dispatcher.h"
#include "ATCommonData.h"
#include "ERM.h"
#include "UART.h"
#include "OSEK_AppTask_Alarm.h"
#include "APP_PHONE.h"
#include "APP_PHONE_BAP_INDICATION_C1.id"
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
extern AppPhoneBapContext gAppPhoneBapContext;
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
static APP_COMMON_enResponseCode 
_APP_PHONE_BAP_rcHandleUserList( const BapIndication * bapInd);

static APP_COMMON_enResponseCode 
_APP_PHONE_BAP_rcHandleCallState( const BapIndication * bapInd);

static APP_COMMON_enResponseCode 
_APP_PHONE_BAP_rcHandleLineState(const BapIndication *bapInd);

static APP_COMMON_enResponseCode
_APP_PHONE_BAP_rcHandleCallInfo( const BapIndication * bapInd); 

static APP_COMMON_enResponseCode 
_APP_PHONE_BAP_rcHandleDialNumber( const BapIndication * bapInd);   

static APP_COMMON_enResponseCode 
_APP_PHONE_BAP_rcHandleHangUpCall( const BapIndication * bapInd);

static APP_COMMON_enResponseCode 
_APP_PHONE_BAP_rcHandleAcceptCall( const BapIndication * bapInd);

static APP_COMMON_enResponseCode 
_APP_PHONE_BAP_rcHandleSendDTMF( const BapIndication * bapInd);
                
static APP_COMMON_enResponseCode 
_APP_PHONE_BAP_rcHandleCallHold( const BapIndication * bapInd);

static APP_COMMON_enResponseCode 
_APP_PHONE_BAP_rcHandleResumeCall( const BapIndication * bapInd);
                
static APP_COMMON_enResponseCode 
_APP_PHONE_BAP_rcHandleHandsFreeOnOff( const BapIndication * bapInd);

static APP_COMMON_enResponseCode 
_APP_PHONE_BAP_rcHandleMicroMuteOnOff( const BapIndication * bapInd);

static APP_COMMON_enResponseCode 
_APP_PHONE_BAP_rcHandleMethodsWithoutData( const BapIndication * bapInd);

static APP_COMMON_enResponseCode 
_APP_PHONE_BAP_rcHandleConferenceCall( const BapIndication * bapInd);

static APP_COMMON_enResponseCode
_APP_PHONE_BAP_rcHandleRestoreFactorySettings(const BapIndication *bapInd);

static APP_COMMON_enResponseCode 
_APP_PHONE_BAP_rcHandleRegisterState(const BapIndication *bapInd);

static APP_COMMON_enResponseCode 
_APP_PHONE_BAP_rcHandleNetworkProvider(const BapIndication *bapInd);

static APP_COMMON_enResponseCode 
_APP_PHONE_BAP_rcHandleSignalQuality(const BapIndication *bapInd);

void _APP_PHONE_BAP_vRetryContext(const BapIndication *pBapInd);
void _APP_PHONE_BAP_vReleaseContext(const BapIndication *pBapInd);
void _APP_PHONE_BAP_vHandleBAPIndication(const BapIndication *bapInd );

/* call back functions */
void _APP_PHONE_BAP_vOnDialNumberCb(APP_COMMON_enResponseCode result);
void _APP_PHONE_BAP_vOnAcceptCallCb(APP_COMMON_enResponseCode result);
void _APP_PHONE_BAP_vOnHangUpCallCb(APP_COMMON_enResponseCode result);
void _APP_PHONE_BAP_vOnMPCallHoldAcceptWaitingCallCb(APP_COMMON_enResponseCode result);
void _APP_PHONE_BAP_vOnMPSwapCb(APP_COMMON_enResponseCode result);
void _APP_PHONE_BAP_vOnCallHoldCb(APP_COMMON_enResponseCode result);
void _APP_PHONE_BAP_vOnResumeCallCb(APP_COMMON_enResponseCode result);
#ifdef UHV_ACTIVATE_FEATURE_CCSPLIT                
void _APP_PHONE_BAP_vOnCCSplitCb(APP_COMMON_enResponseCode result);
#endif
void _APP_PHONE_BAP_vOnCCJoinCb(APP_COMMON_enResponseCode result);
void _APP_PHONE_BAP_vOnRestoreFactorySettingsCb(APP_COMMON_enResponseCode rsp);
void _APP_PHONE_BAP_vOnMPReleaseActiveCallAcceptWaitingCallCb(APP_COMMON_enResponseCode result);
void _APP_PHONE_BAP_vOnSendDtmfCb(APP_COMMON_enResponseCode result);
void _APP_PHONE_BAP_vOnMicroMuteOnOffCb(APP_COMMON_enResponseCode result);
void _APP_PHONE_BAP_cbDialNumberTimeOut(void);
void _APP_PHONE_BAP_cbAcceptCallTimeOut(void);
void _APP_PHONE_BAP_cbHangUpCallTimeOut(void);
void _APP_PHONE_BAP_cbMPCallHoldAcceptWaitingCallTimeOut(void);
void _APP_PHONE_BAP_cbMPSwapTimeOut(void);
void _APP_PHONE_BAP_cbCallHoldTimeOut(void);
void _APP_PHONE_BAP_cbResumeCallTimeOut(void);
void _APP_PHONE_BAP_cbCCJoinTimeOut(void);
void _APP_PHONE_BAP_cbMPReleaseActiveCallAcceptWaitingCallTimeOut(void);

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
void _APP_PHONE_BAP_vRetryContext(const BapIndication *pBapInd)
{
    gAppPhoneBapContext.common.fctId = (APP_PHONE_BAP_enFunctionIdType)pBapInd->fct_id;
    (void)memcpy(&gAppPhoneBapContext.common.bapInd, pBapInd, sizeof(BapIndication));

    if(BT_FALSE == gAppPhoneBapContext.bResumeCallPending)
    {
        (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_PHONE_BAP_FCT_PENDING, MSEC(10), 0);
    }
    else // RT:4062(wait while resume call is in processing)
    {
        (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_PHONE_BAP_FCT_PENDING, MSEC(200), 0);
    }

}
 
void _APP_PHONE_BAP_vReleaseContext(const BapIndication *pBapInd)
{
    (void)APP_COMMON_UTILS_sfFreeBapIndication((BapIndication*)pBapInd);
    (void)memset(&gAppPhoneBapContext.common, 0, sizeof(gAppPhoneBapContext.common));
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_vHandleBAPIndications                             */
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
void _APP_PHONE_BAP_vHandleBAPIndication(const BapIndication *bapInd )
{
    APP_COMMON_enResponseCode result = APP_COMMON_RSP_OK;
    
    switch( bapInd->fct_id )
    {
    
        /* A bzw. B0 functionality */
        case BapFct_Telephone_UserList:
            result = _APP_PHONE_BAP_rcHandleUserList(bapInd);
            break;
        
        case BapFct_Telephone_NetworkProvider:
            result = _APP_PHONE_BAP_rcHandleNetworkProvider(bapInd);
            break;   
        case BapFct_Telephone_SignalQuality:
            result = _APP_PHONE_BAP_rcHandleSignalQuality(bapInd);
            break;   
        case BapFct_Telephone_RegisterState:
            result = _APP_PHONE_BAP_rcHandleRegisterState(bapInd);
            break;   
     
        case BapFct_Telephone_CallState:
            result = _APP_PHONE_BAP_rcHandleCallState(bapInd);
            break;
        case BapFct_Telephone_LineState:
            result = _APP_PHONE_BAP_rcHandleLineState(bapInd);
            break;
            
        case BapFct_Telephone_CallInfo:
            result = _APP_PHONE_BAP_rcHandleCallInfo(bapInd); 
            break;
    
        case BapFct_Telephone_DialNumber:
            result = _APP_PHONE_BAP_rcHandleDialNumber(bapInd);   
            break;
    
        case BapFct_Telephone_HangupCall:
            result = _APP_PHONE_BAP_rcHandleHangUpCall(bapInd);
            break;
            
        case BapFct_Telephone_AcceptCall:
            result = _APP_PHONE_BAP_rcHandleAcceptCall(bapInd);
            break;
        
        case BapFct_Telephone_SendDTMF:
            result = _APP_PHONE_BAP_rcHandleSendDTMF(bapInd);
            break;   
         
        case BapFct_Telephone_RestoreFactorySettings:
            result = _APP_PHONE_BAP_rcHandleRestoreFactorySettings(bapInd);
            break;
        
        /* B0 for KW38 */
        case BapFct_Telephone_CallHold:
            result = _APP_PHONE_BAP_rcHandleCallHold(bapInd);
            break;
            
        case BapFct_Telephone_ResumeCall:
            result = _APP_PHONE_BAP_rcHandleResumeCall(bapInd);
            break;
    
        case BapFct_Telephone_HandsFreeOnOff:
            result = _APP_PHONE_BAP_rcHandleHandsFreeOnOff(bapInd);
            break;
            
        case BapFct_Telephone_MicroMuteOnOff:
            result = _APP_PHONE_BAP_rcHandleMicroMuteOnOff(bapInd);
            break;
    
        /* Handle methods with not params */
        case BapFct_Telephone_MPReleaseActiveCallAcceptWaitingCall:
        case BapFct_Telephone_MPSwap:
        case BapFct_Telephone_MPCallHoldAcceptWaitingCall:
            result = _APP_PHONE_BAP_rcHandleMethodsWithoutData(bapInd);
            break;
#ifdef UHV_ACTIVATE_FEATURE_CCSPLIT                
        case BapFct_Telephone_CCSplit:
#endif
        case BapFct_Telephone_CCJoin:
            result = _APP_PHONE_BAP_rcHandleConferenceCall(bapInd);
            break;
    
        default:
            result = APP_COMMON_RSP_ERROR_BAP_NOT_IMPLEMENTED; 
            break;
    }

    
    // check and handle the result
    switch(result)
    {
        case APP_COMMON_RSP_ERROR_BUSY:

            _APP_PHONE_BAP_vRetryContext(bapInd);

            DEBUG_VALUE1(APP_PHONE_BAP_SWI_TRC, MOD_APP_PHONE_BAP, "HandleBapInd: busy retry fctType %d",
                            bapInd->fct_id );
            break;
        case APP_COMMON_RSP_OK:
            _APP_PHONE_BAP_vReleaseContext(bapInd);
            break;
        case APP_COMMON_RSP_ERROR_BAP_NOT_IMPLEMENTED:

            DEBUG_VALUE2(APP_PHONE_BAP_SWI_ERR, MOD_APP_PHONE_BAP, "HandleBapInd: not implem. error %d fctType: %d", 
                            result, bapInd->fct_id );

            // Send response to BAP Dispatcher about unknown Function ID
            APP_COMMON_UTILS_vSendBapError( BapLsg_Telephone,
                                            bapInd->fct_id,
                                            AppErr_NotSuccessful);
            // release the context
            _APP_PHONE_BAP_vReleaseContext(bapInd);
            break;
        case APP_COMMON_RSP_ERROR_NO_ACTIVE_CALL:
            DEBUG_VALUE1(APP_PHONE_BAP_SWI_ERR, MOD_APP_PHONE_BAP, "HandleBapInd: no act.call err,fctType: %d", 
                            bapInd->fct_id );

            // Send response to BAP Dispatcher about no active call
            APP_COMMON_UTILS_vSendBapError( BapLsg_Telephone,
                                            bapInd->fct_id,
                                            AppErr_NoActiveCall);
            //Release the context
            _APP_PHONE_BAP_vReleaseContext(bapInd);
            break;
        default:
            DEBUG_VALUE2(APP_PHONE_BAP_SWI_ERR,MOD_APP_PHONE_BAP, "HandleBapInd: error %d fctType: %d",
                            result, bapInd->fct_id );

            //Send BapError message
            APP_COMMON_UTILS_vSendBapError( BapLsg_Telephone, 
                                            bapInd->fct_id, 
                                            AppErr_Unknown);
            //release the context
            _APP_PHONE_BAP_vReleaseContext(bapInd);
            break;
    }

}/* END OF _APP_PHONE_vHandleBAPIndication() */



/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_rcHandleDialNumber                                */
/** 
    \brief      handles dial and redial number indication 
  
    \brief      Description:\n
                handles dial and redial number indication \n 

    \param      bap
                const BapIndication * 
                value limits
    
    \return     APP_COMMON_enResponseCode
   
*/
/*----------------------------------------------------------------------------*/
static APP_COMMON_enResponseCode _APP_PHONE_BAP_rcHandleDialNumber(const BapIndication *bap)
{
    // extract number and optional name and put them into struct
    // which can be handles by the ATCmdHandler
    APP_COMMON_enResponseCode result = APP_COMMON_RSP_OK;
 
    switch(bap->ind_type)
    {
        case BapInd_StartResult:
        {
            // get phone number and optional name
            // get length and begin address of TelNumber string 
            const Uint8Type *numberString               = NULL;
            Uint16Type numberLen                        = 0U;
            numberLen = APP_COMMON_UTILS_u16GetBapString(   bap->payload.byte_seq.data, // bap byte seq 
                                                            &numberString);             // begin of number   
            
            // check length of number     
            // if number is a NULL string then make a redial
            if( numberLen == 1 && !(*numberString))
            {
                //Redial
                gAppPhoneBapContext.bDialNumberPending = BT_TRUE; //RT:3838
                result = APP_PHONE_rcRedial( _APP_PHONE_BAP_vOnDialNumberCb );
                AppTask_vSetAlarm(APP_TASK_ALARM_APP_PHONE_BAP_DIALNUMBER,
                                    MSEC(15000),
                                    _APP_PHONE_BAP_cbDialNumberTimeOut);
            }
            // check if this is a valid number
            else if(BT_TRUE == APP_PHONE_bIsValidPhoneNumber(numberString, numberLen)) 
            {
                //Dial
                gAppPhoneBapContext.bDialNumberPending = BT_TRUE; //RT:3838
                result = APP_PHONE_rcDialNumber( _APP_PHONE_BAP_vOnDialNumberCb,
                                                 numberString,
                                                 numberLen); 
                AppTask_vSetAlarm(APP_TASK_ALARM_APP_PHONE_BAP_DIALNUMBER,
                                MSEC(15000),
                                _APP_PHONE_BAP_cbDialNumberTimeOut);
            }
            else
            {
                // not a valid phone number, send back error to HU
                result = APP_COMMON_RSP_ERROR;
            }
            break;
        }
        case BapInd_Abort:
            // what should we do here?
            // TODO: HangUp ??
            gAppPhoneBapContext.bHangUpCallPending = BT_TRUE;
            gAppPhoneBapContext.bHangUpCallIndex = AT_HANGUP_NO_INDEX;
            result = APP_PHONE_rcHangUp(_APP_PHONE_BAP_vOnDialNumberCb,
                                        AT_HANGUP_NO_INDEX );

            /* start the timer for the processing*/
            AppTask_vSetAlarm(APP_TASK_ALARM_APP_PHONE_BAP_HANGUPCALL,
                              MSEC(5000),
                              _APP_PHONE_BAP_cbHangUpCallTimeOut);
            break; 
    
        default:
            // send back an error
            result = APP_COMMON_RSP_ERROR_BAP_NOT_IMPLEMENTED;
            break;
    }

    return result;
}   /* END OF _APP_PHONE_rcHandleDialNumber()                                 */



/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_rcHandleAcceptCall                                */
/** 
    \brief      handle accept call indication 
  
    \brief      Description:\n
                handle accept call indication \n 

    \param      bap
                const BapIndication * 
                value limits

    \return     APP_COMMON_enResponseCode
                APP_COMMON_RSP_OK or FAIL 
*/
/*----------------------------------------------------------------------------*/
static APP_COMMON_enResponseCode _APP_PHONE_BAP_rcHandleAcceptCall(const BapIndication *bap)
{
    APP_COMMON_enResponseCode result = APP_COMMON_RSP_OK;

    switch(bap->ind_type)
    {
        case BapInd_StartResult:
            gAppPhoneBapContext.bAcceptCallPending = BT_TRUE;
            result = APP_PHONE_rcAcceptCall(_APP_PHONE_BAP_vOnAcceptCallCb);

            /* start the timer for the processing*/
            AppTask_vSetAlarm(APP_TASK_ALARM_APP_PHONE_BAP_ACCEPTCALL,
                              MSEC(5000),
                              _APP_PHONE_BAP_cbAcceptCallTimeOut);
            break;

        default:
            result = APP_COMMON_RSP_ERROR_BAP_NOT_IMPLEMENTED;
            break;
    }
    return result;
}   /* END OF _APP_PHONE_rcHandleAcceptCall() */


/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_rcHandleHangUpCall                                */
/** 
    \brief      handle hangup call indication 
  
    \brief      Description:\n
                handle hangup call indication \n 

    \param      bapInd
                const BapIndication * 
                value limits

    \return     APP_COMMON_enResponseCode
                APP_COMMON_RSP_OK or FAIL 
   
*/
/*----------------------------------------------------------------------------*/
static APP_COMMON_enResponseCode _APP_PHONE_BAP_rcHandleHangUpCall(const BapIndication *bapInd)
{
    APP_COMMON_enResponseCode result = APP_COMMON_RSP_OK;
    ATCallHangupEnum callIndex;
    static uint8_t NumHangUpRetriesWhileCallResuming = 0;
    switch(bapInd->ind_type)
    {
        case BapInd_StartResult:
        {    
            // get callId to hung up, callId 2 up to 6 will not be supported cause of
            // Parrot's limit to max 2 calls
            switch( bapInd->payload.u8_value )
            {
            case 0x00:
                callIndex = AT_HANGUP_CALL_0;     
                break;
            case 0x01:
                callIndex = AT_HANGUP_CALL_1;     
                break;
            case 0x02:
                callIndex = AT_HANGUP_CALL_2;
                break;
            // TODO:
            // maybe CallIndex 0xFC, 0xFD, 0xFE, 0xFF need to be handled.
            default:
                callIndex = AT_HANGUP_NO_INDEX;     
                break;
            }
            if(BT_FALSE == gAppPhoneBapContext.bResumeCallPending) // RT:4062 check first if resume call is in processing
            {
                gAppPhoneBapContext.bHangUpCallPending = BT_TRUE;
                gAppPhoneBapContext.bHangUpCallIndex = callIndex;
                result = APP_PHONE_rcHangUp(_APP_PHONE_BAP_vOnHangUpCallCb,
                                            callIndex);

                /* start the timer for the processing*/
                AppTask_vSetAlarm(APP_TASK_ALARM_APP_PHONE_BAP_HANGUPCALL,
                                  MSEC(5000),
                                  _APP_PHONE_BAP_cbHangUpCallTimeOut);
                NumHangUpRetriesWhileCallResuming = 0;
            }
            else
            {
                if(NumHangUpRetriesWhileCallResuming < 10) // wait 2 sec. for call active
                {
                    DEBUG_TEXT(APP_PHONE_BAP_SWI_TRC,MOD_APP_PHONE_BAP, "Wait until resume call processing completed");
                    NumHangUpRetriesWhileCallResuming++;
                    result = APP_COMMON_RSP_ERROR_BUSY;
                }
                else // dismiss the hang up command, no active call after 2 sec.
                {
                    DEBUG_TEXT(APP_PHONE_BAP_SWI_TRC,MOD_APP_PHONE_BAP, "Dismiss the hang up command after 2 sec.");
                    NumHangUpRetriesWhileCallResuming = 0;
                    result = APP_COMMON_RSP_ERROR_NO_ACTIVE_CALL;
                }
            }
        }
        break;

        default:
            result = APP_COMMON_RSP_ERROR_BAP_NOT_IMPLEMENTED;
            break;
    }   
   
    return result;    

}   /* END OF _APP_PHONE_rcHandleHangUpCall() */

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_BAP_rcHandleNetworkProvider                       */
/** 
    \brief      handle network provider indication 
  
    \brief      Description:\n
                handle network provider indication \n 

    \param      bapInd
                const BapIndication * 
                value limits

    \return     APP_COMMON_enResponseCode
                APP_COMMON_RSP_OK or FAIL 
*/
/*----------------------------------------------------------------------------*/
static APP_COMMON_enResponseCode _APP_PHONE_BAP_rcHandleNetworkProvider(const BapIndication *bapInd)
{
    APP_COMMON_enResponseCode result = APP_COMMON_RSP_OK;
    const AT_NetworkProvider * netProv;
        
    switch(bapInd->ind_type)
    {
        case BapInd_DataGet:
            break;

        default:
            return APP_COMMON_RSP_ERROR_BAP_NOT_IMPLEMENTED;
    }   
   
    // get NetworkProvider from APP_PHONE
    netProv = APP_PHONE_GetNetworkProvider();
    if(SUCCESS != _APP_PHONE_BAP_REQUEST_sfSendNetworkProvider(netProv->name,
                                                               netProv->name_len))
    {
        result = APP_COMMON_RSP_ERROR;
    }
    return result; 
}   /* END OF _APP_PHONE_rcHandleNetworkProvider() */


/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_BAP_rcHandleSignalQuality                         */
/** 
    \brief      handle signal quality indication 
  
    \brief      Description:\n
                handle signal quality indication \n 

    \param      bapInd
                const BapIndication * 
                value limits

    \return     APP_COMMON_enResponseCode
                APP_COMMON_RSP_OK or FAIL 
*/
/*----------------------------------------------------------------------------*/
static APP_COMMON_enResponseCode _APP_PHONE_BAP_rcHandleSignalQuality(const BapIndication *bapInd)
{
    APP_COMMON_enResponseCode result = APP_COMMON_RSP_OK;
        
    switch(bapInd->ind_type)
    {
        case BapInd_DataGet:
            break;

        default:
            return APP_COMMON_RSP_ERROR_BAP_NOT_IMPLEMENTED;
    }   
   
    if( SUCCESS != _APP_PHONE_BAP_REQUEST_sfSendSignalQuality(APP_PHONE_GetSignalStrength()))
    {
        result = APP_COMMON_RSP_ERROR;
    }

    return result; 
}   /* END OF _APP_PHONE_rcHandleSignalQuality() */


/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_BAP_rcHandleRegisterState                         */
/** 
    \brief      handle register state indication 
  
    \brief      Description:\n
                handle register state indication \n 

    \param      bapInd
                const BapIndication * 
                value limits

    \return     APP_COMMON_enResponseCode
                APP_COMMON_RSP_OK or FAIL 
*/
/*----------------------------------------------------------------------------*/
static APP_COMMON_enResponseCode _APP_PHONE_BAP_rcHandleRegisterState(const BapIndication *bapInd)
{
    APP_COMMON_enResponseCode result = APP_COMMON_RSP_OK;
        
    switch(bapInd->ind_type)
    {
        case BapInd_DataGet:
            break;

        default:
            return APP_COMMON_RSP_ERROR_BAP_NOT_IMPLEMENTED;
    }   
   
    if( SUCCESS != _APP_PHONE_BAP_REQUEST_sfSendRegisterState(APP_PHONE_GetNetworkAvailable(),
                                                              APP_PHONE_GetRoaming()))
    {
        result = APP_COMMON_RSP_ERROR;
    }

    return result; 
}   /* END OF _APP_PHONE_rcHandlePhoneStatus() */


/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_rcHandleCallState                                 */
/** 
    \brief      handle phone call state indication 
  
    \brief      Description:\n
                handle phone call state indication \n 

    \param      bapInd
                const BapIndication * 
                value limits

    \return     APP_COMMON_enResponseCode
                APP_COMMON_RSP_OK or FAIL 
*/
/*----------------------------------------------------------------------------*/
static APP_COMMON_enResponseCode _APP_PHONE_BAP_rcHandleCallState(const BapIndication *bapInd)
{
    APP_COMMON_enResponseCode result = APP_COMMON_RSP_OK;

    switch(bapInd->ind_type)
    {
        case BapInd_DataGet:
            break;

        default:
            return APP_COMMON_RSP_ERROR_BAP_NOT_IMPLEMENTED;
    } 
    // send call state 
    if(SUCCESS != _APP_PHONE_BAP_REQUEST_sfSendCallState())
    {
        result = APP_COMMON_RSP_ERROR;
    }

    return result;
}   /* END OF _APP_PHONE_rcHandleCallState() */


/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_rcHandleLineState                                 */
/** 
    \brief      handle phone line state indication 
  
    \brief      Description:\n
                handle phone line state indication \n 

    \param      bapInd
                const BapIndication * 
                value limits

    \return     APP_COMMON_enResponseCode
                APP_COMMON_RSP_OK or FAIL 
*/
/*----------------------------------------------------------------------------*/
static APP_COMMON_enResponseCode _APP_PHONE_BAP_rcHandleLineState(const BapIndication *bapInd)
{
    APP_COMMON_enResponseCode result = APP_COMMON_RSP_OK;

    switch(bapInd->ind_type)
    {
        case BapInd_DataGet:
            break;

        default:
            return APP_COMMON_RSP_ERROR_BAP_NOT_IMPLEMENTED;
    } 
    // send call state 
    if(SUCCESS != _APP_PHONE_BAP_REQUEST_sfSendLineState())
    {
        result = APP_COMMON_RSP_ERROR;
    }

    return result;
}   /* END OF _APP_PHONE_rcHandleCallState() */



/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_rcHandleCallInfo                                  */
/** 
    \brief      handle phone call info indication 
  
    \brief      Description:\n
                handle phone call info indication \n 

    \param      bapInd
                const BapIndication * 
                value limits

    \return     APP_COMMON_enResponseCode
                APP_COMMON_RSP_OK or FAIL 
*/
/*----------------------------------------------------------------------------*/
static APP_COMMON_enResponseCode _APP_PHONE_BAP_rcHandleCallInfo(const BapIndication *bapInd)
{
    BapArrayHeaderType arrayHeader;
    APP_COMMON_enResponseCode result = APP_COMMON_RSP_OK;
    SuccessFailType rtnCode = SUCCESS;
 
    switch(bapInd->ind_type)
    {
        case BapInd_DataGet:
            break;

        default:
            return APP_COMMON_RSP_ERROR_BAP_NOT_IMPLEMENTED;
    } 
    
    /* parse array header */
    rtnCode = APP_COMMON_BAP_ARRAY_sfReadHeader(bapInd->payload.byte_seq.data, 
                                                bapInd->payload.byte_seq.length,
                                                &arrayHeader);
    if( rtnCode != SUCCESS )
    {
        return APP_COMMON_RSP_ERROR_BAP_PARSE;
    }
 
    if(SUCCESS != _APP_PHONE_BAP_REQUEST_sfSendCallInfoStatus(&arrayHeader))
    {
        result = APP_COMMON_RSP_ERROR;
    }

    return result;
}   /* END OF _APP_PHONE_rcHandleCallInfo() */


 
/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_rcHandleSendDTMF                                  */
/** 
    \brief      handle sendDTMF indication 
  
    \brief      Description:\n
                handle sendDTMF indication \n 

    \param      bapInd
                const BapIndication * 
                value limits

    \return     APP_COMMON_enResponseCode
                APP_COMMON_RSP_OK or FAIL 
*/
/*----------------------------------------------------------------------------*/

static APP_COMMON_enResponseCode _APP_PHONE_BAP_rcHandleSendDTMF(const BapIndication *bapInd)
{
    APP_COMMON_enResponseCode result = APP_COMMON_RSP_OK;

    switch( bapInd->ind_type )
    {
        case BapInd_Start:
        {
            const Uint8Type * dtmfString    = NULL;
            Uint16Type dtmfStringLen        = 0U;
            
            // check data type
            if(BAP_DT_BYTE != bapInd->data_type)
            {
                return APP_COMMON_RSP_ERROR_BAP_PARSE;
            }

            // get dtmf characters from BAP string
            dtmfStringLen = APP_COMMON_UTILS_u16GetBapString(bapInd->payload.byte_seq.data,
                                                             &dtmfString);

            result = APP_PHONE_rcSendDTMF(_APP_PHONE_BAP_vOnSendDtmfCb,
                                          dtmfString, dtmfStringLen);
            break;
        }
        default:
            return APP_COMMON_RSP_ERROR_BAP_NOT_IMPLEMENTED;
    }

    return result;
}   /* END OF _APP_PHONE_rcHandleSendDTMF() */




// *************************************************** 
// NOT B0 !!! 
// ***************************************************

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_rcHandleCallHold                                  */
/** 
    \brief      handle call hold indication 
  
    \param      bapInd
                const BapIndication * 
                value limits

    \return     APP_COMMON_enResponseCode
                APP_COMMON_RSP_OK or FAIL 
*/
/*----------------------------------------------------------------------------*/

static APP_COMMON_enResponseCode _APP_PHONE_BAP_rcHandleCallHold(const BapIndication *bapInd)
{
    APP_COMMON_enResponseCode result = APP_COMMON_RSP_OK;
    
    switch( bapInd->ind_type )
    {
        case BapInd_StartResult:
        {
            gAppPhoneBapContext.bCallHoldPending = BT_TRUE;
            result = APP_PHONE_rcCallHold(_APP_PHONE_BAP_vOnCallHoldCb);

            /* start the timer for the processing*/
            AppTask_vSetAlarm(APP_TASK_ALARM_APP_PHONE_BAP_CALLHOLD,
                              MSEC(5000),
                              _APP_PHONE_BAP_cbCallHoldTimeOut);
            break;
        }
        default:
            result = APP_COMMON_RSP_ERROR_BAP_NOT_IMPLEMENTED;
            break;
    }

    return result;
}/* END OF _APP_PHONE_rcHandleCallHold() */



 /*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_rcHandleResumeCall                                 */
/** 
    \brief      handle resume call indication 
  
    \param      bapInd
                const BapIndication * 
                value limits

    \return     APP_COMMON_enResponseCode
                APP_COMMON_RSP_OK or FAIL 
*/
/*----------------------------------------------------------------------------*/

static APP_COMMON_enResponseCode _APP_PHONE_BAP_rcHandleResumeCall(const BapIndication *bapInd)
{
    APP_COMMON_enResponseCode result = APP_COMMON_RSP_OK;

    switch( bapInd->ind_type )
    {
        case BapInd_StartResult:
            break;

        default:
            result = APP_COMMON_RSP_ERROR_BAP_NOT_IMPLEMENTED;
            break;
    }
    gAppPhoneBapContext.bResumeCallPending = BT_TRUE; //RT:4062
    result = APP_PHONE_rcResumeCall(_APP_PHONE_BAP_vOnResumeCallCb);

    /* start the timer for the processing*/
    AppTask_vSetAlarm(APP_TASK_ALARM_APP_PHONE_BAP_RESUMECALL,
                      MSEC(5000),
                      _APP_PHONE_BAP_cbResumeCallTimeOut);
    return result; 
}/* END OF _APP_PHONE_BAP_rcHandleResumeCall() */



 /*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_rcHandleHandsFreeOnOff                             */
/** 
    \brief      handle Hands Free On / Off indication 
  
    \param      bapInd
                const BapIndication * 
                value limits

    \return     APP_COMMON_enResponseCode
                APP_COMMON_RSP_OK or FAIL 
*/
/*----------------------------------------------------------------------------*/

static APP_COMMON_enResponseCode _APP_PHONE_BAP_rcHandleHandsFreeOnOff(const BapIndication *bapInd)
{
    /* TODOD: Implement this using HandsFree structs !!!!!!!! */
    APP_COMMON_enResponseCode result    = APP_COMMON_RSP_OK;
  
    switch( bapInd->ind_type)
    {
        case BapInd_DataSetGet:
        {
            // set will be successful on phone only if there is an active call
            // get bit 0 value
            ATAudioTransferModeEnum mode;
            HandsFreeType onOff;    
            (void)memcpy(&onOff, &bapInd->payload.u8_value, 1);
            switch( onOff.hfBit )
            {
            case BAP_HANDSFREE_OFF:
                mode = AT_AUDIO_DISCRETE_MODE;
                (void)_APP_PHONE_BAP_REQUEST_sfSendHandsFreeOnOff(BAP_HANDSFREE_OFF);
                break;
            case BAP_HANDSFREE_ON:
                mode = AT_AUDIO_HANDSFREE_MODE;
                (void)_APP_PHONE_BAP_REQUEST_sfSendHandsFreeOnOff(BAP_HANDSFREE_ON);
                break;
            default:
                return APP_COMMON_RSP_ERROR_BAP_PARSE;
            }
            gAppPhoneBapContext.HandsFreeModeAlreadySent = mode;
            result = APP_PHONE_rcHandsFreeOnOff(NULL,
                                                mode);
            break;
        }    
        default:
            result = APP_COMMON_RSP_ERROR;
            break;
    }

    return result;

}/* END OF _APP_PHONE_BAP_rcHandleHandsFreeOnOff() */

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_rcHandleMicroMuteOnOff                            */
/** 
    \brief      handle micro mute on/off indication 
  
    \param      bapInd
                const BapIndication * 
                value limits

    \return     APP_COMMON_enResponseCode
                APP_COMMON_RSP_OK or FAIL 
*/
/*----------------------------------------------------------------------------*/
static APP_COMMON_enResponseCode _APP_PHONE_BAP_rcHandleMicroMuteOnOff(const BapIndication *bapInd)
{
    APP_COMMON_enResponseCode result    = APP_COMMON_RSP_OK;
   
    switch( bapInd->ind_type)
    {
        case BapInd_DataSetGet:
        {
            // get bit 0 value ( mode )
            ATMicModeEnum mode;
            MicroMuteType mute; 
            (void)memcpy(&mute, &bapInd->payload.u8_value, 1);

            if( mute.muteBit )
            {
                mode = AT_MIC_MUTE;
            } 
            else
            {
                mode = AT_MIC_UNMUTE;
            }
            result = APP_PHONE_rcMicroMuteOnOff(_APP_PHONE_BAP_vOnMicroMuteOnOffCb,
                                                mode);
            break;
        }
        default:
            result = APP_COMMON_RSP_ERROR;
            break;
    }
    
    if(APP_COMMON_RSP_OK != result &&
       APP_COMMON_RSP_ERROR_BUSY != result )
    {
        // set back to old mode and send it to BAP
        MicroMuteMode oldMode;
        oldMode = (APP_PHONE_bIsMicMuted() == BT_TRUE) ?
                   BAP_MICRO_MUTE : BAP_MICRO_UNMUTE;
        if( SUCCESS != _APP_PHONE_BAP_REQUEST_sfSendMicroMuteOnOff(oldMode))
        {
            result = APP_COMMON_RSP_ERROR;    
        }
    }
    return result;
}   /* END OF _APP_PHONE_BAP_rcHandleMicroMuteOnOff() */


static APP_COMMON_enResponseCode _APP_PHONE_BAP_rcHandleMethodsWithoutData(const BapIndication *bapInd)
{
    APP_COMMON_enResponseCode result = APP_COMMON_RSP_OK;
    
    if( BapInd_StartResult != bapInd->ind_type )
    {
        // seems we did not get the expected StartResult Indication
       return APP_COMMON_RSP_ERROR_BAP_NOT_IMPLEMENTED; 
    }

    switch( bapInd->fct_id )
    {
        case BapFct_Telephone_MPSwap:
            gAppPhoneBapContext.bMPSwapPending = BT_TRUE;
            result = APP_PHONE_rcSwitchCalls(_APP_PHONE_BAP_vOnMPSwapCb);

            /* start the timer for the processing*/
            AppTask_vSetAlarm(APP_TASK_ALARM_APP_PHONE_BAP_MPSWAP,
                              MSEC(5000),
                              _APP_PHONE_BAP_cbMPSwapTimeOut);
            break;
        case BapFct_Telephone_MPCallHoldAcceptWaitingCall:
            gAppPhoneBapContext.bMPCallHoldAcceptWaitingCallPending = BT_TRUE;
            // active call is put on hold automatically
            result = APP_PHONE_rcAcceptCall(_APP_PHONE_BAP_vOnMPCallHoldAcceptWaitingCallCb);

            /* start the timer for the processing*/
            AppTask_vSetAlarm(APP_TASK_ALARM_APP_PHONE_BAP_MPCALLHOLDACCEPTWAITINGCALL,
                              MSEC(5000),
                              _APP_PHONE_BAP_cbMPCallHoldAcceptWaitingCallTimeOut);
            break;
        case BapFct_Telephone_MPReleaseActiveCallAcceptWaitingCall:
            //accept the waiting call, then hangup the held call
            gAppPhoneBapContext.bMPReleaseActiveCallAcceptWaitingCallPending = BT_TRUE;
            result = APP_PHONE_rcCallReplace(_APP_PHONE_BAP_vOnMPReleaseActiveCallAcceptWaitingCallCb);

            /* start the timer for the processing*/
            AppTask_vSetAlarm(APP_TASK_ALARM_APP_PHONE_BAP_MPRELEASEACTIVECALLACCEPTWAITINGCALL,
                              MSEC(5000),
                              _APP_PHONE_BAP_cbMPReleaseActiveCallAcceptWaitingCallTimeOut);
            break;
        default:
            result = APP_COMMON_RSP_ERROR_BAP_NOT_IMPLEMENTED; 
            break;
    }     

    return result;
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_rcHandleConferenceCall                            */
/** 
    \brief      handle conference call indications 
  
    \param      bapInd
                const BapIndication * 
                value limits

    \return     APP_COMMON_enResponseCode
                APP_COMMON_RSP_OK or FAIL 
*/
/*----------------------------------------------------------------------------*/
static APP_COMMON_enResponseCode _APP_PHONE_BAP_rcHandleConferenceCall(const BapIndication * bapInd)
{
    APP_COMMON_enResponseCode result = APP_COMMON_RSP_OK;

    switch( bapInd->fct_id )
    {
        case BapFct_Telephone_CCJoin:
            gAppPhoneBapContext.bCCJoinPending = BT_TRUE;
            result = APP_PHONE_rcJoinToConference(_APP_PHONE_BAP_vOnCCJoinCb);

            /* start the timer for the processing*/
            AppTask_vSetAlarm(APP_TASK_ALARM_APP_PHONE_BAP_CCJOIN,
                              MSEC(5000),
                              _APP_PHONE_BAP_cbCCJoinTimeOut);
            break;
#ifdef UHV_ACTIVATE_FEATURE_CCSPLIT                
        case BapFct_Telephone_CCSplit:
            switch(bapInd->payload.u8_value)
            {
                // callIndex is required for this
                case AT_CALL_INDEX_0:
                case AT_CALL_INDEX_1:
                {
                    ATCallIndexEnum callIndex = (ATCallIndexEnum)bapInd->payload.u8_value;
                    result = APP_PHONE_rcSplitCallFromConference(_APP_PHONE_BAP_vOnCCSplitCb, 
                                                                 callIndex);    
                    break;
                }
                default:
                    result = APP_COMMON_RSP_ERROR_BAP_PARSE; 
                    break;
            }
            break;
#endif
        default:
            result = APP_COMMON_RSP_ERROR_BAP_PARSE; 
            break;
    }

    return result;
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_BAP_rcHandleUserList                                  */
/** 
    \brief      handle userList indication 
  
    \param      bapInd
                const BapIndication * 
                value limits

    \return     APP_COMMON_enResponseCode
                APP_COMMON_RSP_OK or FAIL 
*/
/*----------------------------------------------------------------------------*/

static APP_COMMON_enResponseCode _APP_PHONE_BAP_rcHandleUserList(const BapIndication *bapInd)
{
    BapArrayHeaderType arrayHeader;
    APP_COMMON_enResponseCode result    = APP_COMMON_RSP_OK;
    SuccessFailType sF                  = SUCCESS;    

    switch( bapInd->ind_type )
    {
        case BapInd_DataGet:
            break;   
        
        default:
            return APP_COMMON_RSP_ERROR_BAP_NOT_IMPLEMENTED;
    }

    sF = APP_COMMON_BAP_ARRAY_sfReadHeader( bapInd->payload.byte_seq.data, 
                                            bapInd->payload.byte_seq.length,
                                            &arrayHeader);
    if( sF != SUCCESS )
    {
        return APP_COMMON_RSP_ERROR_BAP_PARSE;
    }

    // check if ERM is already initialized, if so and there is still an empty
    // UserList, send it
    // If ERM ist not yet initialized, UserList has not been read from EEPROM,
    // so we need to send temporarily unavailable error
    if( /*0 == ERM_u16GetNumberOfUserProfiles() && */
        BT_FALSE == ERM_bIsStartupDone())
    {
        APP_COMMON_UTILS_vSendBapError( BapLsg_Telephone,
                                         BapFct_Telephone_UserList, 
                                         (enum BapError_t)0x42);
        return APP_COMMON_RSP_OK;
    }
    else if( SUCCESS !=_APP_PHONE_BAP_REQUEST_sfSendUserListStatus(&arrayHeader))
    {
        return APP_COMMON_RSP_ERROR; 
    }
	else
    {
    }
    return result; 
}

static APP_COMMON_enResponseCode 
_APP_PHONE_BAP_rcHandleRestoreFactorySettings(const BapIndication *bapInd)
{
    APP_COMMON_enResponseCode rspCode   = APP_COMMON_RSP_OK;

    if(APP_CSHDL_eGetCradleRestrictionState() == APP_CSHDL_CRADLE_RESTRICTION_ON)
    {
        DEBUG_TEXT(APP_PHONE_BAP_SWI_TRC, MOD_APP_PHONE_BAP, "HaReFactS: not possible (CR_ON)");

        return APP_COMMON_RSP_ERROR;
    }

    // act depending on mode 
    switch( bapInd->payload.u8_value )
    {
        case BAP_RESET_FACTORY_SETTINGS_ALL:
            // delete all user profiles
            return APP_DEVICE_rcResetToFactorySetting(_APP_PHONE_BAP_vOnRestoreFactorySettingsCb);

        case BAP_RESET_FACTORY_SETTINGS_USER:
            // currently there is no user special data to be reset, so send bap
            // ok back 
            if( SUCCESS != APP_COMMON_UTILS_sfSendBapResultRequestNoData(BapLsg_Telephone,
                                                 BapFct_Telephone_RestoreFactorySettings))
            {
                return APP_COMMON_RSP_ERROR;
            }
            break;

        default:
            return APP_COMMON_RSP_ERROR;
    }
    return rspCode;
}


/*----------------------------------------------------------------------------*/
/* Function :   APP_PHONE_BAP_vProcessResumeCall                              */
/**
  \brief        This method is called to send the BAP result notification

  \param        BAP_enResultProcessing
                ResumeState

  \return       void

 */
/*----------------------------------------------------------------------------*/
void APP_PHONE_BAP_vProcessResumeCall(BAP_enResultProcessing ResumeState)
{
    switch(ResumeState)
    {
        case CK5050_RESULT_RECEIVED:
            gAppPhoneBapContext.ResumeCallStates.CK5050result = BT_TRUE;
            break;
        case BAP_CALLSTATE_SENT:
            gAppPhoneBapContext.ResumeCallStates.callstate = BT_TRUE;
            break;
        case BAP_RESULT_ERROR:
            APP_COMMON_UTILS_vSendBapError(BapLsg_Telephone , BapFct_Telephone_ResumeCall, AppErr_Unknown);
            gAppPhoneBapContext.bResumeCallPending = BT_FALSE;
            gAppPhoneBapContext.ResumeCallStates.CK5050result = BT_FALSE;
            gAppPhoneBapContext.ResumeCallStates.callstate = BT_FALSE;
            AppTask_vClearAlarm(APP_TASK_ALARM_APP_PHONE_BAP_RESUMECALL);
        default:
            /* do nothing */
            break;
    }

    if( gAppPhoneBapContext.ResumeCallStates.CK5050result && // send the BAP result in case all info to HU is sent
        gAppPhoneBapContext.ResumeCallStates.callstate)

    {
        (void)APP_COMMON_UTILS_sfSendBapResultRequestNoData(BapLsg_Telephone, BapFct_Telephone_ResumeCall);
        gAppPhoneBapContext.bResumeCallPending = BT_FALSE;
        gAppPhoneBapContext.ResumeCallStates.CK5050result = BT_FALSE;
        gAppPhoneBapContext.ResumeCallStates.callstate = BT_FALSE;
        AppTask_vClearAlarm(APP_TASK_ALARM_APP_PHONE_BAP_RESUMECALL);
    }
}

/*----------------------------------------------------------------------------*/
/* Function :   _APP_PHONE_BAP_vProcessDialNumber                             */
/**
  \brief        This method is called to send the BAP result notification

  \param        BAP_enResultProcessing
                DialState

  \return       void

 */
/*----------------------------------------------------------------------------*/
void APP_PHONE_BAP_vProcessDialNumber(BAP_enResultProcessing DialState)
{
    switch(DialState)
    {
        case CK5050_RESULT_RECEIVED:
            gAppPhoneBapContext.DialNumberStates.CK5050result = BT_TRUE;
            break;
        case BAP_LINESTATE_SENT:
            gAppPhoneBapContext.DialNumberStates.linestate = BT_TRUE;
            break;
        case BAP_CALLSTATE_SENT:
            gAppPhoneBapContext.DialNumberStates.callstate = BT_TRUE;
            break;
        case BAP_CALLINFO_SENT:
            gAppPhoneBapContext.DialNumberStates.callinfo = BT_TRUE;
            break;
        case BAP_RESULT_ERROR:
            if((CKCM_ERROR_CODE_DIALING_OR_WAITING_CALL == ERM_uGetLastExtendedErrorCode())||
                    CKCM_ERROR_CODE_TOO_MANY_ONGOING_CALLS == ERM_uGetLastExtendedErrorCode())
            {
                APP_COMMON_UTILS_vSendBapError(BapLsg_Telephone , BapFct_Telephone_DialNumber, AppErr_ActiveCallPresent_Dialing);
            }
            else
            {
                APP_COMMON_UTILS_vSendBapError(BapLsg_Telephone , BapFct_Telephone_DialNumber, AppErr_Unknown);
            }
            gAppPhoneBapContext.bDialNumberPending = BT_FALSE;
            gAppPhoneBapContext.DialNumberStates.CK5050result = BT_FALSE;
            gAppPhoneBapContext.DialNumberStates.linestate = BT_FALSE;
            gAppPhoneBapContext.DialNumberStates.callstate = BT_FALSE;
            gAppPhoneBapContext.DialNumberStates.callinfo = BT_FALSE;
            AppTask_vClearAlarm(APP_TASK_ALARM_APP_PHONE_BAP_DIALNUMBER);
        default:
            /* do nothing */
            break;
    }

    if( gAppPhoneBapContext.DialNumberStates.CK5050result && // send the BAP result in case all info to HU is sent
        gAppPhoneBapContext.DialNumberStates.linestate &&
        gAppPhoneBapContext.DialNumberStates.callstate &&
        gAppPhoneBapContext.DialNumberStates.callinfo)
    {
        (void)APP_COMMON_UTILS_sfSendBapResultRequestNoData(BapLsg_Telephone, BapFct_Telephone_DialNumber);
        gAppPhoneBapContext.bDialNumberPending = BT_FALSE;
        gAppPhoneBapContext.DialNumberStates.CK5050result = BT_FALSE;
        gAppPhoneBapContext.DialNumberStates.linestate = BT_FALSE;
        gAppPhoneBapContext.DialNumberStates.callstate = BT_FALSE;
        gAppPhoneBapContext.DialNumberStates.callinfo = BT_FALSE;
        AppTask_vClearAlarm(APP_TASK_ALARM_APP_PHONE_BAP_DIALNUMBER);
    }
}


/*----------------------------------------------------------------------------*/
/* Function :   APP_PHONE_BAP_vProcessAcceptCall                              */
/**
  \brief        This method is called to send the BAP result notification

  \param        BAP_enResultProcessing
                CallState

  \return       void

 */
/*----------------------------------------------------------------------------*/
void APP_PHONE_BAP_vProcessAcceptCall(BAP_enResultProcessing CallState)
{
    switch(CallState)
    {
        case CK5050_RESULT_RECEIVED:
            gAppPhoneBapContext.AcceptCallStates.CK5050result = BT_TRUE;
            break;
        case BAP_CALLSTATE_SENT:
            gAppPhoneBapContext.AcceptCallStates.callstate = BT_TRUE;
            break;
        case BAP_RESULT_ERROR:
            APP_COMMON_UTILS_vSendBapError(BapLsg_Telephone , BapFct_Telephone_AcceptCall, AppErr_Unknown);
            gAppPhoneBapContext.bAcceptCallPending = BT_FALSE;
            gAppPhoneBapContext.AcceptCallStates.CK5050result = BT_FALSE;
            gAppPhoneBapContext.AcceptCallStates.callstate = BT_FALSE;
            AppTask_vClearAlarm(APP_TASK_ALARM_APP_PHONE_BAP_ACCEPTCALL);
        default:
            /* do nothing */
            break;
    }

    if( gAppPhoneBapContext.AcceptCallStates.CK5050result && // send the BAP result in case all info to HU is sent
        gAppPhoneBapContext.AcceptCallStates.callstate )
    {
        (void)APP_COMMON_UTILS_sfSendBapResultRequestNoData(BapLsg_Telephone, BapFct_Telephone_AcceptCall);
        gAppPhoneBapContext.bAcceptCallPending = BT_FALSE;
        gAppPhoneBapContext.AcceptCallStates.CK5050result = BT_FALSE;
        gAppPhoneBapContext.AcceptCallStates.callstate = BT_FALSE;
        AppTask_vClearAlarm(APP_TASK_ALARM_APP_PHONE_BAP_ACCEPTCALL);
    }
}

/*----------------------------------------------------------------------------*/
/* Function :   APP_PHONE_BAP_vProcessHangUpCall                              */
/**
  \brief        This method is called to send the BAP result notification

  \param        BAP_enResultProcessing
                CallState

  \return       void

 */
/*----------------------------------------------------------------------------*/
void APP_PHONE_BAP_vProcessHangUpCall(BAP_enResultProcessing CallState)
{
    switch(CallState)
    {
        case CK5050_RESULT_RECEIVED:
            gAppPhoneBapContext.HangUpCallStates.CK5050result = BT_TRUE;
            break;
        case BAP_LINESTATE_SENT:
            gAppPhoneBapContext.HangUpCallStates.linestate = BT_TRUE;
            break;
        case BAP_CALLSTATE_SENT:
            gAppPhoneBapContext.HangUpCallStates.callstate = BT_TRUE;
            break;
        case BAP_CALLINFO_SENT:
            gAppPhoneBapContext.HangUpCallStates.callinfo = BT_TRUE;
            break;
        case BAP_RESULT_ERROR:
            APP_COMMON_UTILS_vSendBapError(BapLsg_Telephone , BapFct_Telephone_HangupCall, AppErr_Unknown);
            gAppPhoneBapContext.bHangUpCallPending = BT_FALSE;
            gAppPhoneBapContext.HangUpCallStates.CK5050result = BT_FALSE;
            gAppPhoneBapContext.HangUpCallStates.linestate = BT_FALSE;
            gAppPhoneBapContext.HangUpCallStates.callstate = BT_FALSE;
            gAppPhoneBapContext.HangUpCallStates.callinfo = BT_FALSE;
            AppTask_vClearAlarm(APP_TASK_ALARM_APP_PHONE_BAP_HANGUPCALL);
        default:
            /* do nothing */
            break;
    }

    if( gAppPhoneBapContext.HangUpCallStates.CK5050result && // send the BAP result in case all info to HU is sent
        gAppPhoneBapContext.HangUpCallStates.linestate &&
        gAppPhoneBapContext.HangUpCallStates.callstate &&
        gAppPhoneBapContext.HangUpCallStates.callinfo)
    {
        (void)APP_COMMON_UTILS_sfSendBapResultRequestNoData(BapLsg_Telephone, BapFct_Telephone_HangupCall);
        gAppPhoneBapContext.bHangUpCallPending = BT_FALSE;
        gAppPhoneBapContext.HangUpCallStates.CK5050result = BT_FALSE;
        gAppPhoneBapContext.HangUpCallStates.linestate = BT_FALSE;
        gAppPhoneBapContext.HangUpCallStates.callstate = BT_FALSE;
        gAppPhoneBapContext.HangUpCallStates.callinfo = BT_FALSE;
        AppTask_vClearAlarm(APP_TASK_ALARM_APP_PHONE_BAP_HANGUPCALL);
    }
}

/*----------------------------------------------------------------------------*/
/* Function :   APP_PHONE_BAP_vProcessMPCallHoldAcceptWaitingCall             */
/**
  \brief        This method is called to send the BAP result notification

  \param        BAP_enResultProcessing
                CallState

  \return       void

 */
/*----------------------------------------------------------------------------*/
void APP_PHONE_BAP_vProcessMPCallHoldAcceptWaitingCall(BAP_enResultProcessing CallState)
{
    switch(CallState)
    {
        case CK5050_RESULT_RECEIVED:
            gAppPhoneBapContext.MPCallHoldAcceptWaitingCallStates.CK5050result = BT_TRUE;
            break;
        case BAP_CALLSTATE_SENT:
            gAppPhoneBapContext.MPCallHoldAcceptWaitingCallStates.callstate = BT_TRUE;
            break;
        case BAP_RESULT_ERROR:
            APP_COMMON_UTILS_vSendBapError(BapLsg_Telephone , BapFct_Telephone_MPCallHoldAcceptWaitingCall, AppErr_Unknown);
            gAppPhoneBapContext.bMPCallHoldAcceptWaitingCallPending = BT_FALSE;
            gAppPhoneBapContext.MPCallHoldAcceptWaitingCallStates.CK5050result = BT_FALSE;
            gAppPhoneBapContext.MPCallHoldAcceptWaitingCallStates.callstate = BT_FALSE;
            AppTask_vClearAlarm(APP_TASK_ALARM_APP_PHONE_BAP_MPCALLHOLDACCEPTWAITINGCALL);
        default:
            /* do nothing */
            break;
    }

    if( gAppPhoneBapContext.MPCallHoldAcceptWaitingCallStates.CK5050result && // send the BAP result in case all info to HU is sent
        gAppPhoneBapContext.MPCallHoldAcceptWaitingCallStates.callstate )
    {
        (void)APP_COMMON_UTILS_sfSendBapResultRequestNoData(BapLsg_Telephone, BapFct_Telephone_MPCallHoldAcceptWaitingCall);
        gAppPhoneBapContext.bMPCallHoldAcceptWaitingCallPending = BT_FALSE;
        gAppPhoneBapContext.MPCallHoldAcceptWaitingCallStates.CK5050result = BT_FALSE;
        gAppPhoneBapContext.MPCallHoldAcceptWaitingCallStates.callstate = BT_FALSE;
        AppTask_vClearAlarm(APP_TASK_ALARM_APP_PHONE_BAP_MPCALLHOLDACCEPTWAITINGCALL);
    }
}

/*----------------------------------------------------------------------------*/
/* Function :   APP_PHONE_BAP_vProcessMPSwap                                  */
/**
  \brief        This method is called to send the BAP result notification

  \param        BAP_enResultProcessing
                CallState

  \return       void

 */
/*----------------------------------------------------------------------------*/
void APP_PHONE_BAP_vProcessMPSwap(BAP_enResultProcessing CallState)
{
    switch(CallState)
    {
        case CK5050_RESULT_RECEIVED:
            gAppPhoneBapContext.MPSwapStates.CK5050result = BT_TRUE;
            break;
        case BAP_CALLSTATE_SENT:
            gAppPhoneBapContext.MPSwapStates.callstate = BT_TRUE;
            break;
        case BAP_RESULT_ERROR:
            APP_COMMON_UTILS_vSendBapError(BapLsg_Telephone , BapFct_Telephone_MPSwap, AppErr_Unknown);
            gAppPhoneBapContext.bMPSwapPending = BT_FALSE;
            gAppPhoneBapContext.MPSwapStates.CK5050result = BT_FALSE;
            gAppPhoneBapContext.MPSwapStates.callstate = BT_FALSE;
            AppTask_vClearAlarm(APP_TASK_ALARM_APP_PHONE_BAP_MPSWAP);
        default:
            /* do nothing */
            break;
    }

    if( gAppPhoneBapContext.MPSwapStates.CK5050result && // send the BAP result in case all info to HU is sent
        gAppPhoneBapContext.MPSwapStates.callstate )
    {
        (void)APP_COMMON_UTILS_sfSendBapResultRequestNoData(BapLsg_Telephone, BapFct_Telephone_MPSwap);
        gAppPhoneBapContext.bMPSwapPending = BT_FALSE;
        gAppPhoneBapContext.MPSwapStates.CK5050result = BT_FALSE;
        gAppPhoneBapContext.MPSwapStates.callstate = BT_FALSE;
        AppTask_vClearAlarm(APP_TASK_ALARM_APP_PHONE_BAP_MPSWAP);
    }
}

/*----------------------------------------------------------------------------*/
/* Function :   APP_PHONE_BAP_vProcessCallHold                                */
/**
  \brief        This method is called to send the BAP result notification

  \param        BAP_enResultProcessing
                CallState

  \return       void

 */
/*----------------------------------------------------------------------------*/
void APP_PHONE_BAP_vProcessCallHold(BAP_enResultProcessing CallState)
{
    switch(CallState)
    {
        case CK5050_RESULT_RECEIVED:
            gAppPhoneBapContext.CallHoldStates.CK5050result = BT_TRUE;
            break;
        case BAP_CALLSTATE_SENT:
            gAppPhoneBapContext.CallHoldStates.callstate = BT_TRUE;
            break;
        case BAP_RESULT_ERROR:
            APP_COMMON_UTILS_vSendBapError(BapLsg_Telephone , BapFct_Telephone_CallHold, AppErr_Unknown);
            gAppPhoneBapContext.bCallHoldPending = BT_FALSE;
            gAppPhoneBapContext.CallHoldStates.CK5050result = BT_FALSE;
            gAppPhoneBapContext.CallHoldStates.callstate = BT_FALSE;
            AppTask_vClearAlarm(APP_TASK_ALARM_APP_PHONE_BAP_CALLHOLD);
        default:
            /* do nothing */
            break;
    }

    if( gAppPhoneBapContext.CallHoldStates.CK5050result && // send the BAP result in case all info to HU is sent
        gAppPhoneBapContext.CallHoldStates.callstate )
    {
        (void)APP_COMMON_UTILS_sfSendBapResultRequestNoData(BapLsg_Telephone, BapFct_Telephone_CallHold);
        gAppPhoneBapContext.bCallHoldPending = BT_FALSE;
        gAppPhoneBapContext.CallHoldStates.CK5050result = BT_FALSE;
        gAppPhoneBapContext.CallHoldStates.callstate = BT_FALSE;
        AppTask_vClearAlarm(APP_TASK_ALARM_APP_PHONE_BAP_CALLHOLD);
    }
}

/*----------------------------------------------------------------------------*/
/* Function :   APP_PHONE_BAP_vProcessCCJoin                                  */
/**
  \brief        This method is called to send the BAP result notification

  \param        BAP_enResultProcessing
                CallState

  \return       void

 */
/*----------------------------------------------------------------------------*/
void APP_PHONE_BAP_vProcessCCJoin(BAP_enResultProcessing CallState)
{
    switch(CallState)
    {
        case CK5050_RESULT_RECEIVED:
            gAppPhoneBapContext.CCJoinStates.CK5050result = BT_TRUE;
            break;
        case BAP_CALLSTATE_SENT:
            gAppPhoneBapContext.CCJoinStates.callstate = BT_TRUE;
            break;
        case BAP_RESULT_ERROR:
            APP_COMMON_UTILS_vSendBapError(BapLsg_Telephone , BapFct_Telephone_CCJoin, AppErr_Unknown);
            gAppPhoneBapContext.bCCJoinPending = BT_FALSE;
            gAppPhoneBapContext.CCJoinStates.CK5050result = BT_FALSE;
            gAppPhoneBapContext.CCJoinStates.callstate = BT_FALSE;
            AppTask_vClearAlarm(APP_TASK_ALARM_APP_PHONE_BAP_CCJOIN);
        default:
            /* do nothing */
            break;
    }

    if( gAppPhoneBapContext.CCJoinStates.CK5050result && // send the BAP result in case all info to HU is sent
        gAppPhoneBapContext.CCJoinStates.callstate )
    {
        (void)APP_COMMON_UTILS_sfSendBapResultRequestNoData(BapLsg_Telephone, BapFct_Telephone_CCJoin);
        gAppPhoneBapContext.bCCJoinPending = BT_FALSE;
        gAppPhoneBapContext.CCJoinStates.CK5050result = BT_FALSE;
        gAppPhoneBapContext.CCJoinStates.callstate = BT_FALSE;
        AppTask_vClearAlarm(APP_TASK_ALARM_APP_PHONE_BAP_CCJOIN);
    }
}


/*----------------------------------------------------------------------------*/
/* Function :   APP_PHONE_BAP_vProcessMPReleaseActiveCallAcceptWaitingCall    */
/**
  \brief        This method is called to send the BAP result notification

  \param        BAP_enResultProcessing
                CallState

  \return       void

 */
/*----------------------------------------------------------------------------*/
void APP_PHONE_BAP_vProcessMPReleaseActiveCallAcceptWaitingCall(BAP_enResultProcessing CallState)
{
    switch(CallState)
    {
        case CK5050_RESULT_RECEIVED:
            gAppPhoneBapContext.MPReleaseActiveCallAcceptWaitingCallStates.CK5050result = BT_TRUE;
            break;
        case BAP_CALLSTATE_SENT:
            gAppPhoneBapContext.MPReleaseActiveCallAcceptWaitingCallStates.callstate = BT_TRUE;
            break;
        case BAP_RESULT_ERROR:
            APP_COMMON_UTILS_vSendBapError(BapLsg_Telephone , BapFct_Telephone_MPReleaseActiveCallAcceptWaitingCall, AppErr_Unknown);
            gAppPhoneBapContext.bMPReleaseActiveCallAcceptWaitingCallPending = BT_FALSE;
            gAppPhoneBapContext.MPReleaseActiveCallAcceptWaitingCallStates.CK5050result = BT_FALSE;
            gAppPhoneBapContext.MPReleaseActiveCallAcceptWaitingCallStates.callstate = BT_FALSE;
            AppTask_vClearAlarm(APP_TASK_ALARM_APP_PHONE_BAP_MPRELEASEACTIVECALLACCEPTWAITINGCALL);
        default:
            /* do nothing */
            break;
    }

    if( gAppPhoneBapContext.MPReleaseActiveCallAcceptWaitingCallStates.CK5050result && // send the BAP result in case all info to HU is sent
        gAppPhoneBapContext.MPReleaseActiveCallAcceptWaitingCallStates.callstate )
    {
        (void)APP_COMMON_UTILS_sfSendBapResultRequestNoData(BapLsg_Telephone, BapFct_Telephone_MPReleaseActiveCallAcceptWaitingCall);
        gAppPhoneBapContext.bMPReleaseActiveCallAcceptWaitingCallPending = BT_FALSE;
        gAppPhoneBapContext.MPReleaseActiveCallAcceptWaitingCallStates.CK5050result = BT_FALSE;
        gAppPhoneBapContext.MPReleaseActiveCallAcceptWaitingCallStates.callstate = BT_FALSE;
        AppTask_vClearAlarm(APP_TASK_ALARM_APP_PHONE_BAP_MPRELEASEACTIVECALLACCEPTWAITINGCALL);
    }
}

/* ---------------------  CALLBACKS ----------------------------------------- */
/* ---------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_BAP_vOnRestoreFactorySettingsCb                   */
/** 
    \brief      handle result\n
  
    \brief      Description:\n

    \param      rsp
                APP_COMMON_enResponseCode
                result of restore factory settings request
    
    \return     void 
*/
/*----------------------------------------------------------------------------*/
void _APP_PHONE_BAP_vOnRestoreFactorySettingsCb(APP_COMMON_enResponseCode rsp)
{
    if( APP_COMMON_RSP_OK == rsp )
    {
        (void)APP_COMMON_UTILS_sfSendBapResultRequestNoData(BapLsg_Telephone,
                               BapFct_Telephone_RestoreFactorySettings);
    }
    else
    {
        APP_COMMON_UTILS_vSendBapError(BapLsg_Telephone,
                                       BapFct_Telephone_RestoreFactorySettings,
                                       AppErr_Unknown);
    }
}
/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_BAP_vOnDialNumberCb                              */
/** 
    \brief      handle result\n
  
    \brief      Description:\n

    \param      result
                APP_COMMON_enResponseCode
                result of Dial Number request
    
    \return     void 
*/
/*----------------------------------------------------------------------------*/
void _APP_PHONE_BAP_vOnDialNumberCb(APP_COMMON_enResponseCode result)
{
    if(gAppPhoneBapContext.bDialNumberPending == BT_TRUE) //RT:3838
    {
        if( APP_COMMON_RSP_OK == result )
        {
            APP_PHONE_BAP_vProcessDialNumber(CK5050_RESULT_RECEIVED);
        }
        else
        {
            APP_PHONE_BAP_vProcessDialNumber(BAP_RESULT_ERROR);
        }
    }
    else
    {
        DEBUG_TEXT(APP_PHONE_BAP_SWI_TRC, MOD_APP_PHONE_BAP, "DialNumberCb: result already sent");
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_BAP_vOnAcceptCallCb                              */
/** 
    \brief      handle result\n
  
    \brief      Description:\n

    \param      result
                APP_COMMON_enResponseCode
                result of accept call request
    
    \return     void 
*/
/*----------------------------------------------------------------------------*/
void _APP_PHONE_BAP_vOnAcceptCallCb(APP_COMMON_enResponseCode result)
{
    if(gAppPhoneBapContext.bAcceptCallPending == BT_TRUE)
    {
        if( APP_COMMON_RSP_OK == result )
        {
            APP_PHONE_BAP_vProcessAcceptCall(CK5050_RESULT_RECEIVED);
        }
        else
        {
            APP_PHONE_BAP_vProcessAcceptCall(BAP_RESULT_ERROR);
        }
    }
    else
    {
        DEBUG_TEXT(APP_PHONE_BAP_SWI_TRC, MOD_APP_PHONE_BAP, "AcceptCallCb: result already sent");
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_BAP_vHangUpCallCb                              */
/** 
    \brief      handle result\n
  
    \brief      Description:\n

    \param      result
                APP_COMMON_enResponseCode
                result of hang up call request
    
    \return     void 
*/
/*----------------------------------------------------------------------------*/
void _APP_PHONE_BAP_vOnHangUpCallCb(APP_COMMON_enResponseCode result)
{
    if(gAppPhoneBapContext.bHangUpCallPending == BT_TRUE)
    {
        if( APP_COMMON_RSP_OK == result )
        {
            APP_PHONE_BAP_vProcessHangUpCall(CK5050_RESULT_RECEIVED);
        }
        else
        {
            APP_PHONE_BAP_vProcessHangUpCall(BAP_RESULT_ERROR);
        }
    }
    else
    {
        DEBUG_TEXT(APP_PHONE_BAP_SWI_TRC, MOD_APP_PHONE_BAP, "HangUpCallCb: result already sent");
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_BAP_vMPCallHoldAcceptWaitingCallCb                   */
/** 
    \brief      handle result\n
  
    \brief      Description:\n

    \param      result
                APP_COMMON_enResponseCode
                result of hold active call and accept waiting call request
    
    \return     void 
*/
/*----------------------------------------------------------------------------*/
void _APP_PHONE_BAP_vOnMPCallHoldAcceptWaitingCallCb(APP_COMMON_enResponseCode result)
{
    if(gAppPhoneBapContext.bMPCallHoldAcceptWaitingCallPending == BT_TRUE)
    {
        if( APP_COMMON_RSP_OK == result )
        {
            APP_PHONE_BAP_vProcessMPCallHoldAcceptWaitingCall(CK5050_RESULT_RECEIVED);
        }
        else
        {
            APP_PHONE_BAP_vProcessMPCallHoldAcceptWaitingCall(BAP_RESULT_ERROR);
        }
    }
    else
    {
        DEBUG_TEXT(APP_PHONE_BAP_SWI_TRC, MOD_APP_PHONE_BAP, "MPCallHoldAcceptWaitingCallCb: result already sent");
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_BAP_vMPSwapCb                                        */
/** 
    \brief      handle result\n
  
    \brief      Description:\n

    \param      result
                APP_COMMON_enResponseCode
                result of switch calls request
    
    \return     void 
*/
/*----------------------------------------------------------------------------*/
void _APP_PHONE_BAP_vOnMPSwapCb(APP_COMMON_enResponseCode result)
{
    if(gAppPhoneBapContext.bMPSwapPending == BT_TRUE)
    {
        if( APP_COMMON_RSP_OK == result )
        {
            APP_PHONE_BAP_vProcessMPSwap(CK5050_RESULT_RECEIVED);
        }
        else
        {
            APP_PHONE_BAP_vProcessMPSwap(BAP_RESULT_ERROR);
        }
    }
    else
    {
        DEBUG_TEXT(APP_PHONE_BAP_SWI_TRC, MOD_APP_PHONE_BAP, "MPSwapCb: result already sent");
    }
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_BAP_vCallHoldCb                                      */
/** 
    \brief      handle result\n
  
    \brief      Description:\n

    \param      result
                APP_COMMON_enResponseCode
                result of hold call request
    
    \return     void 
*/
/*----------------------------------------------------------------------------*/
void _APP_PHONE_BAP_vOnCallHoldCb(APP_COMMON_enResponseCode result)
{
    if(gAppPhoneBapContext.bCallHoldPending == BT_TRUE)
    {
        if( APP_COMMON_RSP_OK == result )
        {
            APP_PHONE_BAP_vProcessCallHold(CK5050_RESULT_RECEIVED);
        }
        else
        {
            APP_PHONE_BAP_vProcessCallHold(BAP_RESULT_ERROR);
        }
    }
    else
    {
        DEBUG_TEXT(APP_PHONE_BAP_SWI_TRC, MOD_APP_PHONE_BAP, "CallHoldCb: result already sent");
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_BAP_vResumeCallCb                                      */
/** 
    \brief      handle result\n
  
    \param      result
                APP_COMMON_enResponseCode
                result of resume call request
    
    \return     void 
*/
/*----------------------------------------------------------------------------*/
void _APP_PHONE_BAP_vOnResumeCallCb(APP_COMMON_enResponseCode result)
{
    if(BT_TRUE == gAppPhoneBapContext.bResumeCallPending) //RT:4062
    {
        if( APP_COMMON_RSP_OK == result )
        {
            APP_PHONE_BAP_vProcessResumeCall(CK5050_RESULT_RECEIVED);
        }
        else
        {
            APP_PHONE_BAP_vProcessResumeCall(BAP_RESULT_ERROR);
        }
    }
    else
    {
        DEBUG_TEXT(APP_PHONE_BAP_SWI_TRC, MOD_APP_PHONE_BAP, "ResumeCallCb: result already sent");
    }
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_BAP_vCCSplitCb                                      */
/** 
    \brief      handle result\n
  
    \param      result
                APP_COMMON_enResponseCode
                result of split call from conference request
    
    \return     void 
*/
/*----------------------------------------------------------------------------*/
#ifdef UHV_ACTIVATE_FEATURE_CCSPLIT                
void _APP_PHONE_BAP_vOnCCSplitCb(APP_COMMON_enResponseCode result)
{
    fctId_t fctId = BapFct_Telephone_CCSplit;

    if( APP_COMMON_RSP_OK == result )
    {
        (void)APP_COMMON_UTILS_sfSendBapResultRequestNoData(BapLsg_Telephone, fctId);
    }
    else
    {
        APP_COMMON_UTILS_vSendBapError(BapLsg_Telephone , fctId, AppErr_Unknown); 
    }
}
#endif
/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_BAP_vCCJoinCb                                      */
/** 
    \brief      handle result\n
  
    \param      result
                APP_COMMON_enResponseCode
                result of joining a call to a conference request
    
    \return     void 
*/
/*----------------------------------------------------------------------------*/
void _APP_PHONE_BAP_vOnCCJoinCb(APP_COMMON_enResponseCode result)
{
    if(gAppPhoneBapContext.bCCJoinPending == BT_TRUE)
    {
        if( APP_COMMON_RSP_OK == result )
        {
            APP_PHONE_BAP_vProcessCCJoin(CK5050_RESULT_RECEIVED);
        }
        else
        {
            APP_PHONE_BAP_vProcessCCJoin(BAP_RESULT_ERROR);
        }
    }
    else
    {
        DEBUG_TEXT(APP_PHONE_BAP_SWI_TRC, MOD_APP_PHONE_BAP, "CCJoinCb: result already sent %d");
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_BAP_vOnMPReleaseActiveCallAcceptWaitingCallCb                                      */
/** 
    \brief      handle result\n
  
    \param      result
                APP_COMMON_enResponseCode   
                result of release active call and accept waiting call request
    
    \return     void 
*/
/*----------------------------------------------------------------------------*/
void _APP_PHONE_BAP_vOnMPReleaseActiveCallAcceptWaitingCallCb(APP_COMMON_enResponseCode result)
{
    if(gAppPhoneBapContext.bMPReleaseActiveCallAcceptWaitingCallPending == BT_TRUE)
    {
        if( APP_COMMON_RSP_OK == result )
        {
            APP_PHONE_BAP_vProcessMPReleaseActiveCallAcceptWaitingCall(CK5050_RESULT_RECEIVED);
        }
        else
        {
            APP_PHONE_BAP_vProcessMPReleaseActiveCallAcceptWaitingCall(BAP_RESULT_ERROR);
        }
    }
    else
    {
        DEBUG_TEXT(APP_PHONE_BAP_SWI_TRC, MOD_APP_PHONE_BAP, "CCJoinCb: result already sent %d");
    }
}
/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_BAP_vOnSendDtmfCb                              */
/** 
    \brief      handle result\n
  
    \param      result
                APP_COMMON_enResponseCode
                result of send dtmf tone request
    
    \return     void 
*/
/*----------------------------------------------------------------------------*/
void _APP_PHONE_BAP_vOnSendDtmfCb(APP_COMMON_enResponseCode result)
{
    fctId_t fctId = BapFct_Telephone_SendDTMF;
    if( APP_COMMON_RSP_OK == result )
    {
        (void)APP_COMMON_UTILS_sfSendBapResultRequestNoData(BapLsg_Telephone, fctId);
    }
    else
    {
        APP_COMMON_UTILS_vSendBapError(BapLsg_Telephone , fctId, AppErr_Unknown); 
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_BAP_vOnMicroMuteOnOffCb                           */
/** 
    \brief      handle result\n
  
    \param      result
                APP_COMMON_enResponseCode
                result of changing micro mute mode
    
    \return     void 
*/
/*----------------------------------------------------------------------------*/
void _APP_PHONE_BAP_vOnMicroMuteOnOffCb(APP_COMMON_enResponseCode result)
{
    if( APP_COMMON_RSP_OK == result )
    {
        // wait for changed notification
    }
    else
    {
        // send back old mode
        MicroMuteMode oldMode;
        oldMode = (APP_PHONE_bIsMicMuted() == BT_TRUE) ?
                   BAP_MICRO_MUTE : BAP_MICRO_UNMUTE;
        (void)_APP_PHONE_BAP_REQUEST_sfSendMicroMuteOnOff(oldMode);
    }
}


/* ------------------- CallBack functions after processing time out -------------------- */
/*---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_BAP_cbDialNumberTimeOut                           */
/**
    \brief      handle processing time out\n

    \param      void

    \return     void
*/
/*----------------------------------------------------------------------------*/
void _APP_PHONE_BAP_cbDialNumberTimeOut(void)
{
    DEBUG_TEXT(APP_PHONE_BAP_SWI_TRC, MOD_APP_PHONE_BAP, "CbDialNumberTimeOut: start result processing failed");
    APP_PHONE_BAP_vProcessDialNumber(BAP_RESULT_ERROR);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_BAP_cbAcceptCallTimeOut                           */
/**
    \brief      handle processing time out\n

    \param      void

    \return     void
*/
/*----------------------------------------------------------------------------*/
void _APP_PHONE_BAP_cbAcceptCallTimeOut(void)
{
    DEBUG_TEXT(APP_PHONE_BAP_SWI_TRC, MOD_APP_PHONE_BAP, "CbDialNumberTimeOut: start result processing failed");
    APP_PHONE_BAP_vProcessAcceptCall(BAP_RESULT_ERROR);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_BAP_cbHangUpCallTimeOut                           */
/**
    \brief      handle processing time out\n

    \param      void

    \return     void
*/
/*----------------------------------------------------------------------------*/
void _APP_PHONE_BAP_cbHangUpCallTimeOut(void)
{
    DEBUG_TEXT(APP_PHONE_BAP_SWI_TRC, MOD_APP_PHONE_BAP, "CbHangUpTimeOut: start result processing failed");
    APP_PHONE_BAP_vProcessHangUpCall(BAP_RESULT_ERROR);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_BAP_cbMPCallHoldAcceptWaitingCallTimeOut          */
/**
    \brief      handle processing time out\n

    \param      void

    \return     void
*/
/*----------------------------------------------------------------------------*/
void _APP_PHONE_BAP_cbMPCallHoldAcceptWaitingCallTimeOut(void)
{
    DEBUG_TEXT(APP_PHONE_BAP_SWI_TRC, MOD_APP_PHONE_BAP, "CbMPCallHoldAcceptWaitingCallTimeOut: start result processing failed");
    APP_PHONE_BAP_vProcessMPCallHoldAcceptWaitingCall(BAP_RESULT_ERROR);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_BAP_cbMPSwapTimeOut                               */
/**
    \brief      handle processing time out\n

    \param      void

    \return     void
*/
/*----------------------------------------------------------------------------*/
void _APP_PHONE_BAP_cbMPSwapTimeOut(void)
{
    DEBUG_TEXT(APP_PHONE_BAP_SWI_TRC, MOD_APP_PHONE_BAP, "CbMPSwapTimeOut: start result processing failed");
    APP_PHONE_BAP_vProcessMPSwap(BAP_RESULT_ERROR);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_BAP_cbCallHoldTimeOut                             */
/**
    \brief      handle processing time out\n

    \param      void

    \return     void
*/
/*----------------------------------------------------------------------------*/
void _APP_PHONE_BAP_cbCallHoldTimeOut(void)
{
    DEBUG_TEXT(APP_PHONE_BAP_SWI_TRC, MOD_APP_PHONE_BAP, "CbCallHoldTimeOut: start result processing failed");
    APP_PHONE_BAP_vProcessCallHold(BAP_RESULT_ERROR);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_BAP_cbResumeCallTimeOut                           */
/**
    \brief      handle processing time out\n

    \param      void

    \return     void
*/
/*----------------------------------------------------------------------------*/
void _APP_PHONE_BAP_cbResumeCallTimeOut(void)
{
    DEBUG_TEXT(APP_PHONE_BAP_SWI_TRC, MOD_APP_PHONE_BAP, "CbResumeCallTimeOut: start result processing failed");
    APP_PHONE_BAP_vProcessResumeCall(BAP_RESULT_ERROR);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_BAP_cbCCJoinTimeOut                               */
/**
    \brief      handle processing time out\n

    \param      void

    \return     void
*/
/*----------------------------------------------------------------------------*/
void _APP_PHONE_BAP_cbCCJoinTimeOut(void)
{
    DEBUG_TEXT(APP_PHONE_BAP_SWI_TRC, MOD_APP_PHONE_BAP, "CbCCJoinTimeOut: start result processing failed");
    APP_PHONE_BAP_vProcessCCJoin(BAP_RESULT_ERROR);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_BAP_cbMPReleaseActiveCallAcceptWaitingCallTimeOut */
/**
    \brief      handle processing time out\n

    \param      void

    \return     void
*/
/*----------------------------------------------------------------------------*/
void _APP_PHONE_BAP_cbMPReleaseActiveCallAcceptWaitingCallTimeOut(void)
{
    DEBUG_TEXT(APP_PHONE_BAP_SWI_TRC, MOD_APP_PHONE_BAP, "CbMPReleaseActiveCallAcceptWaitingCallTimeOut: start result processing failed");
    APP_PHONE_BAP_vProcessMPReleaseActiveCallAcceptWaitingCall(BAP_RESULT_ERROR);
}

/* End Of File APP_PHONE_BAP_C1.c */


