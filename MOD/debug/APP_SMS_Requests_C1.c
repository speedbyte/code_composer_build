/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     APP_SMS_Request_C1.c
*-----------------------------------------------------------------------------
* Module Name:    Sms Application ( sms part )
*-----------------------------------------------------------------------------
* Description:    Handles all sms specific responses from   
                  AT Cmd Parser 
*-----------------------------------------------------------------------------
* $Date: 2010-04-08 17:53:42 +0200 (Do, 08 Apr 2010) $
* $Rev: 12137 $
* $Author: goldammer $
* $URL:
* http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_PHONE/trunk/src/APP_AT_C1.c $
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

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "UART.h"
#include "APP_SMS.h"
#include "APP_SMS_Requests.h"
#include "APP_SMS_ERM.h"
#include "APP_SD.h"

#include "POOL.h"
#include "ATCmdHandler.h"

#include "APP_SMS_Requests_C1.id"
#include "TRACE.h"

/*----------------------------------------------------------------------------*/
/* pragma Directives                                                          */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Definitions (avoid as much as possible!)                  */
/*----------------------------------------------------------------------------*/

extern AppSmsContext gAppSmsContext;
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
_APP_SMS_sfHandleSmsSupport(const ATRspSmsSupNotType * pPmst);

static SuccessFailType 
_APP_SMS_sfHandleSmsMsg(const ATRspSmsReceiveMsgType *pPmrd);

static SuccessFailType 
_APP_SMS_sfHandleSmsCount(const ATRspSmsReceiveNumType *pPmrd);

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
//*----------------------------------------------------------------------------*/



///*----------------------------------------------------------------------------*/
///* Function    : _APP_SMS_vInitSmsContext                           */
///** 
//    \brief      inits sms context 
//  
//    \brief      Description:\n
//                inits sms context \n
//
//    \return     void 
//*/
///*----------------------------------------------------------------------------*/
//void _APP_SMS_vInitSmsContext(void)
//{
//    (void)memset(&gAppSmsContext, 0, sizeof(gAppSmsContext));
//
//    gAppSmsContext.supportState       = APP_SMS_NOT_INITIALIZED;  
//
//    // sms not supported unless sms init has been processed
//    gAppSmsContext.bInitSmsFinished   = BT_FALSE;
//
//    gAppSmsContext.unreadFolderSize   = 0;
//    gAppSmsContext.readFolderSize     = 0;   
//    gAppSmsContext.bSmsSendAllowed    = BT_TRUE;
//}

/*----------------------------------------------------------------------------*/
/* Function    : APP_SMS_vHandleSmsATResponse                          */
/** 
    \brief      handle response msgs from AT Cmd Parser 
  
    \brief      Description:\n
                handles responses from AT Cmd Parser \n

    \param      atRsp 
                const ATRspAllType *
    
    \return     void 
*/
/*----------------------------------------------------------------------------*/
void APP_SMS_vHandleATResponse(const ATRspMessageType *atRsp)
{

    // check the message 
    if (AT_RSP_WITH_DATA == atRsp->dataInd && NULL == atRsp->data )
    {
        // seems to be something wrong with the message, ignore it
        return;
    }

#ifdef UHV_ACTIVATE_FEATURE_SMS        
    switch( atRsp->common.rsp_type )
    {
        case AT_RSP_SMS_SUPPORT_NOT:
            (void)_APP_SMS_sfHandleSmsSupport(&atRsp->data->pmst);
            break;

        case AT_RSP_SMS_NEW_NOT:
            if(BT_TRUE == APP_SMS_bIsSmsInitialized())
            {
                gAppSmsContext.unreadFolderSize++;
                _APP_SMS_vNotifyApps(APP_SMS_NEW_SMS);
            }
            else
            {
                
                DEBUG_TEXT(APP_SMS_SWI_WNG, MOD_APP_SMS, "HdlATRsp: Ign. new sms, not init.");
            }
            break;

        case AT_RSP_SMS_READING_NUMBER:
            (void)_APP_SMS_sfHandleSmsCount(&atRsp->data->pmrd_num);
            break;

        case AT_RSP_SMS_READING:
            (void)_APP_SMS_sfHandleSmsMsg(&atRsp->data->pmrd_msg);
            break;

        case AT_RSP_SMS_LIST:   
            // synchronisation finished
            if(gAppSmsContext.fct.common.fctId == APP_SMS_enSmsInit)
            {
                gAppSmsContext.fct.smsInit.state = APP_SMS_enFinishedSynchro;
                (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_SMS_FCT_PENDING, MSEC(10), 0);
            }
            break;

        default:
            
            DEBUG_VALUE1(APP_SMS_SWI_TRC, MOD_APP_SMS, "HdlATRsp: Ign. msg %d", atRsp->common.rsp_type);
            break;

    }
#else
    
    DEBUG_VALUE1(APP_SMS_SWI_TRC, MOD_APP_SMS, "HdlATRsp: Ign. msg %d due to sms feat. deact.!", atRsp->common.rsp_type);
#endif
 
}/* END OF APP_SMS_vHandleSmsATResponse() */


/*----------------------------------------------------------------------------*/
/* Function    : _APP_SMS_sfHandleSmsMsg                           */
/** 
    \brief      handles sms msg response msg from AT Cmd Parser 
  
    \brief      Description:\n
                handles sms msg response msgs from AT Cmd Parser \n

    \param      pPmrd 
                ATRspSmsReceiveMsgType *
    
    \return     SuccessFailType 
*/
/*----------------------------------------------------------------------------*/
static SuccessFailType _APP_SMS_sfHandleSmsMsg(const ATRspSmsReceiveMsgType *pPmrd)
{
    Uint16Type arrayIndex;
    SuccessFailType result = SUCCESS; 
    AppSmsRecvdType * pSmsCtx = NULL;
    Uint16Type  textLen = 0;
    const Uint8Type *pText = NULL;

    /* check if we need to handle the response */
    if(gAppSmsContext.fct.common.fctId != APP_SMS_enReadSms && 
       gAppSmsContext.fct.common.fctId != APP_SMS_enReadSmsPreview )
    {
        /* no handling needed, e.g. for APP_SMS_enMarkAsRead */
        return SUCCESS;
    }  

    if( gAppSmsContext.fct.common.fctId != APP_SMS_enReadSms )
    {
        pSmsCtx = &gAppSmsContext.fct.smsRecvd; 
    }
    else
    {
        pSmsCtx = &gAppSmsContext.fct.smsPreview; 
    }
  
    /* increase the received number */     
    pSmsCtx->nextIndex++;

    /* get the array index that belongs to the item */
    arrayIndex = pSmsCtx->nextIndex - pSmsCtx->startIndex - 1; 

    switch(pSmsCtx->lastRequestedSmsType )
    {
        case AT_SMS_TYPE_UNREAD:
        case AT_SMS_TYPE_READ:
            pSmsCtx->sms[arrayIndex].type = pSmsCtx->lastRequestedSmsType;
            break;
        default:
            DEBUG_VALUE1( APP_SMS_SWI_ERR, MOD_APP_SMS, "HandleSmsMsg: invalid req sms type %d", pSmsCtx->lastRequestedSmsType);
            break;  
    }
    
    pSmsCtx->sms[arrayIndex].isValid = BT_TRUE;
    
    // copy content 
    (void)memcpy( &(pSmsCtx->sms[arrayIndex].data),
            pPmrd, 
            sizeof(ATRspSmsReceiveMsgType));

    if (BT_TRUE == APP_PHONE_bIsUnknownCallerId(pPmrd->origin, pPmrd->origin_len))
    {
        result = APP_SD_sfGetTextForNumberType(APP_SD_NUMBER_TYPE_UNKNOWN_CALLER_ID, &pText, &textLen);
        if ((SUCCESS == result) && (pText != NULL))
        {
            (void)memcpy(&(pSmsCtx->sms[arrayIndex].data.origin), pText, textLen);
            pSmsCtx->sms[arrayIndex].data.origin_len = textLen;
            DEBUG_TEXT( APP_SMS_SWI_TRC, MOD_APP_SMS, "HandleSmsMsg: replace unknown origin");
        }
    } 

    return result; 
}/* END OF _APP_SMS_vHandleSmsMsg() */

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SMS_sfHandleSmsCount                           */
/** 
    \brief      handles sms msg count response msg from AT Cmd Parser 
  
    \brief      Description:\n
                handles sms msg count response msgs from AT Cmd Parser \n

    \param      pPmrd
                ATRspSmsReceiveNumType *
    
    \return     SuccessFailType 
*/
/*----------------------------------------------------------------------------*/
static SuccessFailType _APP_SMS_sfHandleSmsCount(const ATRspSmsReceiveNumType *pPmrd)
{
    SuccessFailType result = SUCCESS;   

    // save sizes of read / unread folders
    gAppSmsContext.unreadFolderSize   = pPmrd->unread;
    gAppSmsContext.readFolderSize     = pPmrd->read;

    if( gAppSmsContext.fct.common.fctId == APP_SMS_enSmsInit )
    {
        gAppSmsContext.fct.smsInit.state = APP_SMS_enInitFinished;
    }
    
    return result; 
}/* END OF _APP_SMS_vHandleSmsCount() */


/*----------------------------------------------------------------------------*/
/* Function    : _APP_SMS_sfHandleSmsSupport                         */
/** 
    \brief      updates sms support\n
  
    \brief      Description:\n
                updates sms support\n

    \param      pPmst 
                ATRspSmsSupNotType 
    
    \return     SuccessFailType 
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
static SuccessFailType _APP_SMS_sfHandleSmsSupport(const ATRspSmsSupNotType * pPmst)
{
    if(gAppSmsContext.bInitSmsFinished == BT_TRUE)
    {
        return SUCCESS;
    }

    // trigger to start sms synchro
    if(AT_TRUE == pPmst->sms_supported)
    {   
        gAppSmsContext.fct.common.fctId     = APP_SMS_enSmsInit;
        gAppSmsContext.fct.smsInit.state    = APP_SMS_enRestartInit;
        gAppSmsContext.supportState         = APP_SMS_NOT_INITIALIZED_AND_SUPPORTED;
        (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_SMS_FCT_PENDING, MSEC(10), 0);
    }
    else
    {
        gAppSmsContext.supportState       = APP_SMS_INITIALIZED_AND_UNSUPPORTED;  
        _APP_SMS_vNotifyApps(APP_SMS_UNSUPPORTED);
    }

    return SUCCESS;
}
/*----------------------------------------------------------------------------*/
/* Function    : _APP_SMS_enGetNextInitState   */
/** 
    \brief      returns next init state \n
  
    \brief      Description:\n
                return next init state \n
    
    \return     SuccessFailType 
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
APP_SMS_enInitState _APP_SMS_enGetNextInitState(APP_SMS_enInitState state)
{
    switch( state )
    {
        case APP_SMS_enRestartInit:
            /* will be used when an error occurres */
            return APP_SMS_enSetCharset;
        case APP_SMS_enSetCharset:
            return APP_SMS_enStartSynchro;
        case APP_SMS_enStartSynchro:
            /* state will be present until *PMLS:END was received */
            return APP_SMS_enWaitSynchro;
        case APP_SMS_enWaitSynchro:
            return APP_SMS_enWaitSynchro;
        case APP_SMS_enFinishedSynchro:
            return APP_SMS_enGetFolderSizes;
        case APP_SMS_enGetFolderSizes:
            /* state will be present until *PMRD:w,x,y,z was received */
            return APP_SMS_enGetFolderSizes;
        case APP_SMS_enInitFinished:
            return APP_SMS_enInitFinished;
    }
    return APP_SMS_enStateInvalid;
}


///*----------------------------------------------------------------------------*/
///* Function    : _APP_SMS_sfHandleNewSmsNotificatioNotification   */
///** 
//    \brief      notifies HU that a new sms was received \n
//  
//    \brief      Description:\n
//                notifies HU that a new sms was received \n
//    
//    \return     SuccessFailType 
//                SUCCESS or FAIL
//*/
///*----------------------------------------------------------------------------*/
//SuccessFailType _APP_SMS_sfHandleNewSmsNotification()
//{
//
//    // 1. get size of read / unread folders
//    // 2. send changed array with start = numOfReadSms 
//    //    cause we will place the unread sms within the array behind the read sms
//    //    AND update smsState to number of unread sms
//    // 3. HU will ask for these elements with smsAttrib = unread
//    // 4. read those elements with maxLen set to > 160 e.g. 200, this will cause
//    //    the sms to stay in unread folder
//    // 5. if User reads sms on HU, we will be noticed by SMSReceived.SetGet,
//    //    which will be the trigger to read the sms again from Parrot, now
//    //    without maxLen, which will cause the sms the be moved into read folder
//    // 6. the move will cause the array positions to be shifted backwards
//    
//
//    // TODO:request sms folder sizes  
//    
//    // quick hack, increase nr. of unread sms
//    // update number of unread sms
//    _APP_SMS_sfSendSmsStateBapRequest( 0x01, 0x00,
//                      ++gAppSmsContext.unreadFolderSize );
//
//    // send Changed Array 
//    _APP_SMS_sfSendBAPChangedArrays();
//
//    return SUCCESS;
//}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SMS_sfSendSmsCharsetATRequest   */
/** 
    \brief      try to set sms charset to UTF8 \n
  
    \brief      Description:\n
                try to set sms charset to UTF8 \n

    \param      puart 
                const SCI_ST *    

    \return     SuccessFailType 
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SMS_sfSendSetCharsetATRequest(const SCI_ST * puart) 
{
    SuccessFailType result = SUCCESS;
    ATReqAllType request;

    request.common.req_type = AT_REQ_SMS_CHARSET;
    request.pmcs.charset = AT_SMS_CHARSET_UTF8;

    result = AT_Cmd_Handler_Sms(&request, puart);
    if( SUCCESS != result )
    {
        (void)ERM_ReleaseUartForSMS();
    }

    return result;
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SMS_sfSendStartSynchroATRequest   */
/** 
    \brief      start sms synchronisation \n
  
    \brief      Description:\n
                start sms synchronisation \n

    \param      puart 
                const SCI_ST *    

    \return     SuccessFailType 
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SMS_sfSendStartSynchroATRequest(const SCI_ST * puart)
{
    SuccessFailType result = SUCCESS;
    ATReqAllType request;

    request.common.req_type = AT_REQ_SMS_LIST;

    result = AT_Cmd_Handler_Sms(&request, puart);
    if( SUCCESS != result )
    {
        (void)ERM_ReleaseUartForSMS();
    }
    return result;
}
 
/*----------------------------------------------------------------------------*/
/* Function    : _APP_SMS_sfSendSmsFolderSizesATRequest   */
/** 
    \brief      get sms folder sizes \n
  
    \brief      Description:\n
                get sms folder sizes \n

    \param      puart 
                const SCI_ST *    

    \return     SuccessFailType 
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SMS_sfSendSmsFolderSizeATRequest(const SCI_ST *puart)
{
    SuccessFailType result = SUCCESS;
    ATReqAllType request;

    /* only req_type is required */
    request.common.req_type = AT_REQ_SMS_READING_GET;

    result = AT_Cmd_Handler_Sms(&request, puart);
    if( SUCCESS != result )
    {
        (void)ERM_ReleaseUartForSMS();
    }
    return result;
}


///*----------------------------------------------------------------------------*/
///* Function    : _APP_SMS_sfSendConcSms                              */
///** 
//    \brief      request sms for given folder type  
//  
//    \brief      Description:\n
//                request sms for given folder type \n
//
//    \param      puart
//                const SCI_ST*
//
//    \param      type
//                ATSmsTypeEnum 
//
//    \param      index
//                Uint16Type 
//
//    \param      markAsRead
//                BooleanType 
//
//    \return     SuccessFailType
//                SUCCESS or FAIL 
//*/
///*----------------------------------------------------------------------------*/
//SuccessFailType _APP_SMS_sfSendConcSms(const SCI_ST* puart,
//                                       ATSmsTypeEnum type, 
//                                       Uint16Type index,
//                                       BooleanType markAsRead )
//{
//    ATReqAllType request;
//
//    request.common.req_type         = AT_REQ_SMS_CONC_SEND;       
//    request.pmsdex.phone_number     = type;
//    request.pmsdex.phone_number_len = index;  
//    request.pmsdex.message          = AT_FALSE;
//    request.pmsdex.message_len      = AT_FALSE;
//     
//    switch( type )
//    {
//        case AT_SMS_TYPE_READ:
//            request.pmrd.maxlen_avail   = AT_FALSE;
//            break;
//
//        case AT_SMS_TYPE_UNREAD:
//            if(markAsRead)
//            {
//                request.pmrd.maxlen_avail = AT_FALSE;
//            }
//            else
//            {
//                /* first read with max len set to ensure sms stays in unread folder */
//                request.pmrd.maxlen_avail   = AT_TRUE;
//                request.pmrd.maxlen         = APP_SMS_UNREAD_UTF8_MAX_LEN;
//            }
//            break;
//
//        default:
//    	    
//            return FAIL;
//    }
//
//   return AT_Cmd_Handler_Sms(&request, puart);
//}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_SMS_sfRequestSms                              */
/** 
    \brief      request sms for given folder type  
  
    \brief      Description:\n
                request sms for given folder type \n

    \param      puart
                const SCI_ST*

    \param      type
                ATSmsTypeEnum 

    \param      index
                Uint16Type 

    \param      markAsRead
                BooleanType 

    \return     SuccessFailType
                SUCCESS or FAIL 
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SMS_sfRequestSms(const SCI_ST* puart,
                                      ATSmsTypeEnum type, 
                                      Uint16Type index,
                                      BooleanType markAsRead )
{
    ATReqAllType request;

    request.common.req_type     = AT_REQ_SMS_READING;       
    request.pmrd.type           = type;
    request.pmrd.index          = index;  
    request.pmrd.offset_avail   = AT_FALSE;
     
    switch( type )
    {
        case AT_SMS_TYPE_READ:
            request.pmrd.maxlen_avail   = AT_FALSE;
            break;

        case AT_SMS_TYPE_UNREAD:
            if(markAsRead)
            {
                request.pmrd.maxlen_avail = AT_FALSE;
            }
            else
            {
                /* first read with max len set to ensure sms stays in unread folder */
                request.pmrd.maxlen_avail   = AT_TRUE;
                request.pmrd.maxlen         = APP_SMS_UNREAD_UTF8_MAX_LEN;
            }
            break;

        default:
    	    DEBUG_VALUE1( APP_SMS_SWI_ERR, MOD_APP_SMS, "RequestSms: invalid req sms type %d", type);
            return FAIL;
    }

   return AT_Cmd_Handler_Sms(&request, puart);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_SMS_sfRequestSmsPreview                              */
/** 
    \brief      request sms preview for given folder type  
  
    \brief      Description:\n
                request sms preview for given folder type \n

    \param      puart
                const SCI_ST*

    \param      type
                ATSmsTypeEnum 

    \param      index
                Uint16Type 

    \return     SuccessFailType
                SUCCESS or FAIL 
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SMS_sfRequestSmsPreview(const SCI_ST* puart,
                                      ATSmsTypeEnum type, 
                                      Uint16Type index )
{
    ATReqAllType request;

    request.common.req_type     = AT_REQ_SMS_READING;       
    request.pmrd.type           = type;
    request.pmrd.index          = index;  
    request.pmrd.offset_avail   = AT_FALSE;
 
    // read with maxlen 0 to get just date / time and origin 
    // Hopefully unread sms stay in unread folder   
    request.pmrd.maxlen_avail   = AT_TRUE;
    request.pmrd.maxlen         = 0;

   return AT_Cmd_Handler_Sms(&request, puart);
}



/*----------------------------------------------------------------------------*/
/* Function    : _APP_SMS_sfSendSmsDeleteATRequest                            */
/** 
    \brief      returns type and index of next sms to be requested   
  
    \brief      Description:\n
                returns type and index of next sms to be requested \n

    \param      puart
                const SCI_ST*

    \param      type
                ATSmsTypeEnum *

    \param      index
                Uint16Type

    \return     void 
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_SMS_sfSendSmsDeleteATRequest(  const SCI_ST* puart, 
                                                    ATSmsTypeEnum type,
                                                    Uint16Type index )
{
    ATReqAllType request;

    request.common.req_type = AT_REQ_SMS_DELETE;
    request.pmde.type       = type;
    request.pmde.index      = index;     
    
    return AT_Cmd_Handler_Sms(&request, puart);
}

       
/* End Of File APP_SMS_C1.c */


