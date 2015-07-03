/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     APP_PHONEBOOK_BAP_REQUEST_C1.c
*-----------------------------------------------------------------------------
* Module Name:    BAP Phonebook Application
*-----------------------------------------------------------------------------
* Description:    Handles all phonebook specific requests towards BAP
*-----------------------------------------------------------------------------
* $Date: 2011-11-10 12:33:30 +0100 (Do, 10 Nov 2011) $
* $Rev: 23259 $
* $Author: nikolov $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_PHONEBOOK_BAP/trunk/src/APP_PHONEBOOK_BAP_REQUEST_C1.c $
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
#include "APP_PHONEBOOK_BAP.h"
#include "APP_PHONEBOOK_BAP_CI.h"

#include "APP_COMMON_Utils.h"
#include "APP_COMMON_BAP_ARRAY.h"
#include "BAP_Dispatcher.h"
#include "APP_PHONEBOOK.h"
#include "APP_PHONEBOOK_BAP_REQUEST.h"

#include "APP_SMS.h"

#include "ATCmdHandler.h"
#include "UART.h"
#include "ERM.h"
#include "POOL.h"

#include "APP_PHONEBOOK_BAP_REQUEST_C1.id"
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
extern AppPhoneBookBapContext gAppPBBapContext;
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
_APP_PHONEBOOK_BAP_REQUEST_sfSendSmsReceivedImmediate(const BapArrayHeaderType *pRecvHeader,const ConvArrayHeaderType *pConvHeader);

static Uint8Type
_APP_PHONEBOOK_BAP_enGetAT2BAPPnTypeMapping(ATPhoneNumberTypeEnum atPnType);
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
/* Function    : _APP_PHONEBOOK_BAP_REQUEST_sfSendMissedCallsIndication       */
/**
    \brief      Send BAP Request with number of missed calls \n

    \brief      Description:\n
                Send BAP Request with number of missed calls

    \param      missedCalls
                Uint16Type

    \return     SuccessFailType
*/
/*----------------------------------------------------------------------------*/
SuccessFailType
_APP_PHONEBOOK_BAP_REQUEST_sfSendMissedCallsIndication(Uint16Type missedCalls, Uint16Type missedNumbers)
{
    BapRequest request;
    Uint8Type * pTmp    = NULL;
    request.lsg_id      = BapLsg_Phonebook;
    request.fct_id      = BapFct_Phonebook_MissedCallIndication;
    request.req_type    = BapReq_Data;
    request.data_type   = BAP_DT_BYTE;

    // allocate 4 byte
    request.payload.byte_seq.length = (Uint16Type)MAP_MAX_BYTE_SEQ_enAppMissedCallsIndication;

    if( SUCCESS != POOL_sfAllocate( POOL_enBapReqDataType,
                                    (void**) &request.payload.byte_seq.data))
    {
        DEBUG_TEXT(APP_PB_BAP_SWI_ERR, MOD_APP_PB_BAP,"POOL_sfAlloc(POOL_enBapFourByteSeq) failed");
        return FAIL;
    }

    /*  parrot does not count missed calls with unknown numbers
        so missedCalls and missedNumbers are assumed to have the same value */
    // SUSANNE: 07.08.09 We are now detecting missed calls on our self, so we also try to differ between
    //                   missed calls and missed numbers

    // missedCalls and missed Numbers are each an UINT16 and therefor expected
    // to be sent in little endian order!!!!!

    pTmp = request.payload.byte_seq.data;
    APP_COMMON_UTILS_vSetUint16Type( missedCalls,
                                     pTmp);
    pTmp += sizeof(Uint16Type);

    APP_COMMON_UTILS_vSetUint16Type( missedNumbers,
                                     pTmp);
    pTmp += sizeof(Uint16Type);

    //memcpy( pTmp, &missedCalls, sizeof(Uint16Type));
    //pTmp += sizeof(Uint16Type);
    //memcpy( pTmp, &missedCalls, sizeof(Uint16Type));

    if( E_OK != APP_COMMON_UTILS_stSendBapRequest( msgPhoneBookBapRequest, &request))
    {
        DEBUG_VALUE1(APP_PB_BAP_SWI_ERR, MOD_APP_PB_BAP, "Sent MissCallInd: %d",
                           missedCalls);
        return FAIL;
    }

    DEBUG_VALUE1(APP_PB_SWI_TRC, MOD_APP_PB_BAP, "Sent MissCallInd: %d", missedCalls);

    return SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_BAP_REQUEST_sfSendPbState                     */
/**
    \brief      update BAP PbState

    \brief      Description:\n
                update BAP PbState \n

    \param      pbDwnlState
                BapPbDwldState

    \param      pbEntries
                Uint16Type

    \return     SuccessFailType
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_PHONEBOOK_BAP_REQUEST_sfSendPbState(BapPbDwldState pbDwnlState,
                                                         Uint16Type pbEntries)
{
    // we just need the lowest byte
    Uint8Type tmpPbDwnlState = (0xFF & (Uint8Type)pbDwnlState);


    if( pbDwnlState != gAppPBBapContext.curPbState )
    {
        BapRequest request;
        Uint8Type *pTmp  = NULL;

        request.lsg_id      = BapLsg_Phonebook;
        request.fct_id      = BapFct_Phonebook_PbState;
        request.req_type    = BapReq_Data;
        request.data_type   = BAP_DT_BYTE;

        // allocate 3 byte
        request.payload.byte_seq.length = (Uint16Type)BAP_MAX_BYTE_SEQ_enAppPBPbState;
        if( SUCCESS != POOL_sfAllocate( POOL_enBapReqDataType,
                                        (void**) &request.payload.byte_seq.data))
        {
            DEBUG_TEXT(APP_PB_BAP_SWI_ERR, MOD_APP_PB_BAP, "SendPbStateBapRequest: POOL_sfAllocate() failed");
            return FAIL;
        }

        pTmp = request.payload.byte_seq.data;
        // downloadState
        *pTmp = tmpPbDwnlState;
        pTmp += 1;
        APP_COMMON_UTILS_vSetUint16Type(pbEntries, pTmp);
        pTmp += sizeof(Uint16Type);

        if( E_OK != APP_COMMON_UTILS_stSendBapRequest( msgPhoneBookBapRequest, &request))
        {
            DEBUG_VALUE2(APP_PB_BAP_SWI_ERR, MOD_APP_PB_BAP, "SendPbState FAIL: %d, size: %d",
                            tmpPbDwnlState, pbEntries);
            return FAIL;
        }

        DEBUG_VALUE2(APP_PB_BAP_SWI_TRC, MOD_APP_PB_BAP, "SendPbState: %d, size: %d",
                        tmpPbDwnlState, pbEntries);

        gAppPBBapContext.curPbState = pbDwnlState;
    }
    else
    {
        DEBUG_VALUE1(APP_PB_BAP_SWI_TRC, MOD_APP_PB_BAP, "SendPbState: alr. state %d, skip. send",
                    tmpPbDwnlState);
    }

    return SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_BAP_REQUEST_sfSendVoiceMailBox                */
/**
    \brief      update BAP VoiceMailboxNumber

    \brief      Description:\n
                update BAP VoiceMailBoxNumber \n

    \return     SuccessFailType
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_PHONEBOOK_BAP_REQUEST_sfSendVoiceMailBox()
{
    BapRequest request;
    Uint16Type bapStrLen                = 0;
    Uint16Type mailBoxNumberLen         = 0;
    const Uint8Type * mailBoxNumber     = NULL;
    const ERMUserProfileType * profile  = NULL;

    request.lsg_id              = BapLsg_Phonebook;
    request.fct_id              = BapFct_Phonebook_VoiceMailboxNumber;
    request.req_type            = BapReq_Data;
    request.data_type           = BAP_DT_BYTE;

    // get mailbox number of active device from ERM
    // TODO: !!!!!!!!!
    profile = ERM_pGetActiveUserProfile();
    if( NULL != profile )
    {
        mailBoxNumber       = profile->voicemailbox;
        mailBoxNumberLen    = profile->voicemailbox_len;
    }

    // allocate memory
    if( SUCCESS != POOL_sfAllocate( POOL_enBapReqDataType,
                                    (void**) &request.payload.byte_seq.data))
    {
        DEBUG_TEXT(APP_PB_BAP_SWI_ERR, MOD_APP_PB_BAP, "SendVcMailboxNumbBapReq: POOL_sfAllocate() failed");
        return FAIL;
    }

    bapStrLen = APP_COMMON_UTILS_u16WriteBapString(request.payload.byte_seq.data,
                                                   mailBoxNumber,
                                                   mailBoxNumberLen);
    request.payload.byte_seq.length = bapStrLen;

    if( E_OK != APP_COMMON_UTILS_stSendBapRequest( msgPhoneBookBapRequest, &request))
    {
        DEBUG_TEXT(APP_PB_BAP_SWI_ERR, MOD_APP_PB_BAP, "SendVMBox: FAIL");
        return FAIL;
    }


    DEBUG_TEXT(APP_PB_BAP_SWI_TRC, MOD_APP_PB_BAP, "SentVMBox: ");

    return SUCCESS;
}



/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_BAP_REQUEST_sfSendSmsAttribOnly               */
/**
    \brief      send SMSReceived BapRequest with Sms Attrib only \n

    \brief      Description:\n
                send SMSReceived BapRequest with Sms Attrib only \n

    \return     SuccessFailType
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType
_APP_PHONEBOOK_BAP_REQUEST_sfSendSmsAttribOnly(const BapArrayHeaderType *recvHeader,
                                               const ConvArrayHeaderType *convHeader)
{
    return _APP_PHONEBOOK_BAP_REQUEST_sfSendSmsReceivedImmediate(recvHeader,
                                                                 convHeader);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_BAP_REQUEST_sfSendSmsAttribOnly               */
/**
    \brief      send SMSReceived BapRequest with Sms Pos only \n

    \brief      Description:\n
                send SMSReceived BapRequest with Sms Pos only \n

    \return     SuccessFailType
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType
_APP_PHONEBOOK_BAP_REQUEST_sfSendSmsPosOnly(const BapArrayHeaderType *recvHeader,
                                            const ConvArrayHeaderType *convHeader)
{
    return _APP_PHONEBOOK_BAP_REQUEST_sfSendSmsReceivedImmediate(recvHeader,
                                                                 convHeader);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_BAP_REQUEST_sfSendSmsReceivedImmediate()      */
/**
    \brief      send SMSReceived BapRequest with Sms Attrib only \n

    \brief      Description:\n
                send SMSReceived BapRequest with Sms Attrib only \n

    \return     SuccessFailType
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
static SuccessFailType
_APP_PHONEBOOK_BAP_REQUEST_sfSendSmsReceivedImmediate(const BapArrayHeaderType *pRecvHeader,
                                                      const ConvArrayHeaderType *pConvHeader )
{
    Uint16Type i;
    BapRequest bapReq;
    SuccessFailType result = SUCCESS;

    Uint16Type pos                          = 0;
    Uint16Type numPosBytes                  = 0;
    Uint8Type headerLength                  = 0;
    Uint8Type *pDataWriteBuf                = NULL;
    BapArrayHeaderType sendHeader;

    // sizes of sms folders
    Uint16Type unreadSms = APP_SMS_u16GetNumUnreadSms();
    Uint16Type readSms   = APP_SMS_u16GetNumReadSms();

    /* build the request and send it to BAP-Dispatcher */
    bapReq.lsg_id       = BapLsg_Phonebook;
    bapReq.fct_id       = BapFct_Phonebook_SMSReceived;
    bapReq.req_type     = BapReq_Data;                    /* Property Status */
    bapReq.data_type    = BAP_DT_BYTE;

    /* get the mode byte of the header */
    sendHeader.mode.data        = pRecvHeader->mode.data;
    /* transmitting position byte is required */
    sendHeader.mode.common.transmitPos = 1;
    /* get the start value of the header */
    sendHeader.start            = pRecvHeader->start;
    /* preset set the elements value to 0 */
    sendHeader.elements         = 0;
    result = POOL_sfAllocate( POOL_enBapReqDataType,
                              (void**)&bapReq.payload.byte_seq.data);

    if(result != SUCCESS)
    {
        DEBUG_TEXT(APP_PB_BAP_SWI_ERR, MOD_APP_PB_BAP, "POOL_sfAllocate(POOL_enBapReqDataType) failed");
        return FAIL;
    }

    /* check start and end pos to be able to set indexSize */
    if( pConvHeader->start > 0xFF ||
        ( pConvHeader->start + pConvHeader->elements ) > 0xFF )
    {
        sendHeader.mode.common.indexSize = 1;
    }

    /* preset the header to get the length (3 or 5) */
    headerLength = APP_COMMON_BAP_ARRAY_u8WriteHeader(  &sendHeader,
                                                        bapReq.payload.byte_seq.data );

    pDataWriteBuf = bapReq.payload.byte_seq.data + headerLength;

    /* build up the array content */
    for(i=pConvHeader->start; i<=pConvHeader->elements; i++)
    {
        // calculate position
        pos = ( BT_TRUE == pConvHeader->forward ) ?
                i : (pConvHeader->start + pConvHeader->elements - 1)/* last pos */ - i;

        /* write the positon of the record */
        numPosBytes = APP_COMMON_BAP_ARRAY_sfSetPos(pDataWriteBuf,
                                pos, sendHeader.mode.common.indexSize);
        pDataWriteBuf += numPosBytes;

        /* increase the number of records by one */
        sendHeader.elements++;

        if(0x2 == pRecvHeader->mode.common.recordAddress )
        {
            ATSmsTypeEnum smsType = APP_SMS_enGetSmsType(i);
            if( AT_SMS_TYPE_UNREAD == smsType )
            {
                *pDataWriteBuf = 0x00;  // unread
            }
            else if( AT_SMS_TYPE_UNREAD == smsType )
            {
                *pDataWriteBuf = 0x01;  // read
            }
            else
            {
                // redo already increased pointer cause there is no next pos
                // this is important for correct length calculation of byte seq
                pDataWriteBuf -= 1;
                break;
            }
            pDataWriteBuf += 1;
        }
    }

    /* write the actual header to the start of the buffer */
    headerLength = APP_COMMON_BAP_ARRAY_u8WriteHeader(  &sendHeader,
                                         bapReq.payload.byte_seq.data );

    /* set the size of the byte sequence */
    bapReq.payload.byte_seq.length =
           (Uint16Type)(pDataWriteBuf - bapReq.payload.byte_seq.data);

    /* send bapRequest */
    if( E_OK != APP_COMMON_UTILS_stSendBapRequest( msgPhoneBookBapRequest, &bapReq ))
    {
        return FAIL;
    }

    return result;
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_BAP_REQUEST_sfSendSmsReceivedNoSmsForIndex()  */
/**
    \brief      send SMSReceived BapRequest indicating that there is no sms at
                this position  \n

    \brief      Description:\n
                send SMSReceived BapRequest indicating that there is no sms at
                this position  \n

    \return     SuccessFailType
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType
_APP_PHONEBOOK_BAP_REQUEST_sfSendSmsReceivedNoSmsForIndex(const BapArrayHeaderType *pRecvHeader)
{
    BapRequest bapReq;
    SuccessFailType result = SUCCESS;
    Uint8Type headerLength                  = 0;
    Uint8Type *pDataWriteBuf                = NULL;
    BapArrayHeaderType sendHeader;

    /* build the request and send it to BAP-Dispatcher */
    bapReq.lsg_id       = BapLsg_Phonebook;
    bapReq.fct_id       = BapFct_Phonebook_SMSReceived;
    bapReq.req_type     = BapReq_Data;                    /* Property Status */
    bapReq.data_type    = BAP_DT_BYTE;

    /* get the mode byte of the header */
    sendHeader.mode.data        = pRecvHeader->mode.data;
    /* transmitting position byte is not required here, cause we have 0 elements */
    sendHeader.mode.common.transmitPos = 0;
    /* get the start value of the header */
    sendHeader.start            = pRecvHeader->start;
    /* preset set the elements value to 0 */
    sendHeader.elements         = 0;
    result = POOL_sfAllocate( POOL_enBapReqDataType,
                              (void**)&bapReq.payload.byte_seq.data);

    if(result != SUCCESS)
    {
        DEBUG_TEXT(APP_PB_BAP_SWI_ERR, MOD_APP_PB_BAP, "POOL_sfAllocate(POOL_enBapReqDataType) failed");
        return FAIL;
    }

    /* check start and end pos to be able to set indexSize */
    if( sendHeader.start > 0xFF )
    {
        sendHeader.mode.common.indexSize = 1;
    }

    /* preset the header to get the length (3 or 5) */
    headerLength = APP_COMMON_BAP_ARRAY_u8WriteHeader(  &sendHeader,
                                                        bapReq.payload.byte_seq.data );

    pDataWriteBuf = bapReq.payload.byte_seq.data + headerLength;

    /* set the size of the byte sequence */
    bapReq.payload.byte_seq.length =
           (Uint16Type)(pDataWriteBuf - bapReq.payload.byte_seq.data);

    /* send bapRequest */
    if( E_OK != APP_COMMON_UTILS_stSendBapRequest( msgPhoneBookBapRequest, &bapReq ))
    {
        return FAIL;
    }

    return result;
}



/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_BAP_REQUEST_sfSendSmsState                    */
/**
    \brief      update sms state \n

    \brief      Description:\n
                update sms state \n

    \param      simReady
                Uint8Type

    \param      storageState
                Uint8Type

    \param      newSms
                Uint16Type

    \return     SuccessFailType
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_PHONEBOOK_BAP_REQUEST_sfSendSmsState(Uint8Type simReady,
                                                          Uint8Type storageState,
                                                          Uint16Type newSms)
{
    BapRequest bapReq;
    SuccessFailType result = SUCCESS;
    Uint8Type smsStateBuf[4];

    /* build the request and send it to BAP-Dispatcher */
    bapReq.lsg_id       = BapLsg_Phonebook;
    bapReq.fct_id       = BapFct_Phonebook_SMSState;
    bapReq.req_type     = BapReq_Data;      /* Property Status */
    bapReq.data_type    = BAP_DT_BYTE;

    // allocate buffer
    result = POOL_sfAllocate( POOL_enBapReqDataType,
                              (void**)&bapReq.payload.byte_seq.data);

    if(result != SUCCESS)
    {
        DEBUG_TEXT(APP_PB_BAP_SWI_ERR, MOD_APP_PB_BAP, "POOL_sfAllocate( POOL_enBapReqDataType) failed");
        return FAIL;
    }


    /* set data into temporary buffer */
    (void)memset(smsStateBuf, 0, sizeof(smsStateBuf));
    // set SIMReady to SIM data available
    smsStateBuf[0] = simReady;
    // leave storage State to SMS storage available
    smsStateBuf[1] = storageState;
    // set number of new sms to one
    // as this is an UINT16 we need to send it in little endian order!!!
    APP_COMMON_UTILS_vSetUint16Type( newSms, &smsStateBuf[2]);

    // copy data into bap message payload buffer
    (void)memcpy( bapReq.payload.byte_seq.data, smsStateBuf, sizeof(smsStateBuf));
    bapReq.payload.byte_seq.length = sizeof(smsStateBuf);

    /* send bapRequest */
    if( E_OK != APP_COMMON_UTILS_stSendBapRequest( msgPhoneBookBapRequest , &bapReq ))
    {
        DEBUG_VALUE2(APP_PB_BAP_SWI_ERR, MOD_APP_PB_BAP, "SendSmsState FAIL storage state %d, sim %d ... ",
                     storageState, simReady);
        DEBUG_VALUE1(APP_PB_BAP_SWI_ERR, MOD_APP_PB_BAP, "SendSmsState FAIL ... newSms %d",
                     newSms );
        return FAIL;
    }

    DEBUG_VALUE2(APP_PB_BAP_SWI_TRC, MOD_APP_PB_BAP, "SendSmsState storage state %d, sim %d ... ",
                 storageState, simReady);
    DEBUG_VALUE1(APP_PB_BAP_SWI_TRC, MOD_APP_PB_BAP, "SendSmsState ... newSms %d",
                 newSms );

    return SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_BAP_REQUEST_sfHandleSmsRequestFinished                */
/**
    \brief      handles sms request finished state

    \brief      Description:\n
                handles sms request finished state \n

    \return     SuccessFailType
                FAIL or SUCCESS
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_PHONEBOOK_BAP_REQUEST_sfHandleSmsRequestFinished()
{
    // send bap array
    int i, j;
    SuccessFailType result  = SUCCESS;
    Uint8Type headerLength  = 0;
    Uint16Type pos          = 0;
    Uint8Type  numPosBytes  = 0;

    BapArrayHeaderType *pRecvHeader  = NULL;
    ConvArrayHeaderType *pConvHeader = NULL;
    BapArrayHeaderType sendHeader;
    Uint8Type *pDataWriteBuf;

    BapRequest bapReq;
    /* build the status request and send it to BAP-Dispatcher */
    bapReq.lsg_id       = BapLsg_Phonebook;
    bapReq.fct_id       = BapFct_Phonebook_SMSReceived;
    bapReq.req_type     = BapReq_Data;  // StatusArray optype wanted
    bapReq.data_type    = BAP_DT_BYTE;

    /* get the array header of the initiating bap message */
    pRecvHeader = &gAppPBBapContext.appSms.arrayHeader;
    pConvHeader = &gAppPBBapContext.appSms.convHeader;

    /* get the mode byte of the header */
    sendHeader.mode.data    = pRecvHeader->mode.data;
    /* set transmitPos to 1, cause always required */
    sendHeader.mode.status.transmitPos  = 0x1;
    /* get the start value of the header */
    sendHeader.start        = pRecvHeader->start;
    /* set the elements value to 0 */
    sendHeader.elements     = 0;

    result = POOL_sfAllocate( POOL_enBapReqDataType,
                              (void**)&bapReq.payload.byte_seq.data);

    if(result != SUCCESS)
    {
        DEBUG_TEXT(APP_PB_BAP_SWI_ERR, MOD_APP_PB_BAP, "POOL_sfAllocate( POOL_enBapReqDataType ) failed");
        return FAIL;
    }

    /* check start and end pos to be able to set indexSize */
    if( pConvHeader->start > 0xFF ||
        ( pConvHeader->start + pConvHeader->elements ) > 0xFF )
    {
        sendHeader.mode.common.indexSize = 1;
    }

    /* preset the header to get the length (3 or 5) */
    headerLength = APP_COMMON_BAP_ARRAY_u8WriteHeader(  &sendHeader,
                                                        bapReq.payload.byte_seq.data );

    pDataWriteBuf = bapReq.payload.byte_seq.data + headerLength;

    // get correct start/endIndex depending on direction
    pos = ( BT_TRUE == pConvHeader->forward ) ?
            pConvHeader->start :
            pConvHeader->start + pConvHeader->elements - 1;

    for(j=0;j<MAX_REQUESTED_SMS_ENTRIES;j++)
    {
        Uint16Type tmpLen        = 0;
        const AppSmsReadSmsType *pPmrd = NULL;

        i = (BT_TRUE == pConvHeader->forward) ?
               j : pConvHeader->elements - j - 1 ;

        if(i>=MAX_REQUESTED_SMS_ENTRIES)
        {
            continue;
        }

        pPmrd = APP_SMS_pGetPmrdContext(i);
        if((NULL == pPmrd) || (BT_FALSE == pPmrd->isValid))
        {
            pos = (BT_TRUE == pConvHeader->forward) ?
                    pos + 1 : pos - 1 ;
            continue;
        }
        if(sendHeader.elements >= pConvHeader->elements)
        {
            break;
        }
        /* increase the number of records by one */
        sendHeader.elements++;

        /* write the positon of the record */
        numPosBytes = APP_COMMON_BAP_ARRAY_sfSetPos(pDataWriteBuf,
                                pos, sendHeader.mode.common.indexSize);
        pDataWriteBuf += numPosBytes;

        // *******************************************************************
        // HINT: RecordAddress 2 and 15 are currently handled from BAP method
        // directly, without AT-Request
        // *******************************************************************

        /*-----------------------------------*/
        // NEEDED FOR RECORD ADDRESS 0, 1, 4    //
        /* write the content of origin       */
        if( 0 == pRecvHeader->mode.common.recordAddress ||
            1 == pRecvHeader->mode.common.recordAddress ||
            4 == pRecvHeader->mode.common.recordAddress)
        {
            /* write the content of PbName */
            // pPmrd->origin should always contain number
            tmpLen = APP_COMMON_UTILS_u16WriteBapString(
                                pDataWriteBuf,
                                pPmrd->data.origin,
                                pPmrd->data.origin_len);
            pDataWriteBuf += tmpLen;

            // check if this was a read / unread sms
            if(pPmrd->type == AT_SMS_TYPE_UNREAD)
            {
                *pDataWriteBuf = 0x00;      // mark as unread
            }
            else
            {
                *pDataWriteBuf = 0x01;      // mark as read
            }
            pDataWriteBuf += 1;
        }

        /*-----------------------------------*/
        // NEEDED FOR RECORD ADDRESS 0, 3    //
        // write the content of telnumber = origin
        // cause Parrot always just sends the number
        if( 0 == pRecvHeader->mode.common.recordAddress ||
            3 == pRecvHeader->mode.common.recordAddress)
        {
            /* write the content of PbName */
            // pPmrd->origin should always contain number
            tmpLen = APP_COMMON_UTILS_u16WriteBapString(
                                pDataWriteBuf,
                                pPmrd->data.origin,
                                pPmrd->data.origin_len);
            pDataWriteBuf += tmpLen;
        }

        /*-----------------------------------*/
        // NEEDED FOR RECORD ADDRESS 0, 1, 4    //
        /* write the content of date / time info       */
        if( 0 == pRecvHeader->mode.common.recordAddress ||
            3 == pRecvHeader->mode.common.recordAddress ||
            4 == pRecvHeader->mode.common.recordAddress)
        {
            /* -------------------------------------------- */
            /* write Day, Month, Year, Hour, Minute, Second */
            // as we do not support date and time, set Day,
            // month, year, hour, minute and second to 0xFF
            SuccessFailType result2 = SUCCESS;
            AppCommonDateType date;
            result2 = APP_COMMON_UTILS_sfConvertAtSmsDate(  pPmrd->data.date,
                                                           pPmrd->data.date_len,
                                                           &date);

            if(result2 == SUCCESS)
            {
                *pDataWriteBuf = date.day;
                pDataWriteBuf++;
                *pDataWriteBuf = date.month;
                pDataWriteBuf++;
                *pDataWriteBuf = date.year;
                pDataWriteBuf++;
                *pDataWriteBuf = date.hour;
                pDataWriteBuf++;
                *pDataWriteBuf = date.minute;
                pDataWriteBuf++;
                *pDataWriteBuf = date.seconds;
                pDataWriteBuf++;
            }
            else
            {
                (void)memset( pDataWriteBuf, 0xFF, 6 );
                pDataWriteBuf += 6;
            }
        }

        /*-----------------------------------*/
        // NEEDED FOR RECORD ADDRESS 0, 1, 4    //
        // write the content of smsText
        if( 0 == pRecvHeader->mode.common.recordAddress ||
            3 == pRecvHeader->mode.common.recordAddress)
        {
            // number of bytes already written
            Uint16Type maxMsgLen        = 0;
            Uint16Type usedBytes        = 0;
            Uint16Type maxAvailBytes    = 0;

            usedBytes = pDataWriteBuf - bapReq.payload.byte_seq.data;
            usedBytes += APP_COMMON_UTILS_u8CalcBapStringNumLenBytes(pPmrd->data.message_len);

            maxAvailBytes = POOL_BapReqDataTypeSize - usedBytes;

            // first check how many bytes are left from allocated memory
            maxMsgLen = (pPmrd->data.message_len > maxAvailBytes) ?
                         maxAvailBytes : pPmrd->data.message_len;

            // second check that we do not exceed number of bytes which are
            // accepted from BAP stack for smsReceived function. To handle this,
            // we will check to copy no more bytes than we can receive from
            // ck5050.
            if( maxMsgLen > AT_MAX_SMS_LEN_TOTAL )
            {
                maxMsgLen = AT_MAX_SMS_LEN_TOTAL;
            }

            // create bap string for msg with maxMsgLen bytes
            tmpLen = APP_COMMON_UTILS_u16WriteBapString(pDataWriteBuf,
                                                        pPmrd->data.message,
                                                        maxMsgLen);
            pDataWriteBuf += tmpLen;
        }
        pos = (BT_TRUE == pConvHeader->forward) ? pos + 1 : pos - 1;
    }


    /* write the actual header to the start of the buffer */
    headerLength = APP_COMMON_BAP_ARRAY_u8WriteHeader(  &sendHeader,
                                                        bapReq.payload.byte_seq.data );

    /* set the size of the byte sequence */
    bapReq.payload.byte_seq.length =
           (Uint16Type)(pDataWriteBuf - bapReq.payload.byte_seq.data);

    // finally send BAP message
    if( E_OK != APP_COMMON_UTILS_stSendBapRequest( msgPhoneBookBapRequest , &bapReq ))
    {
        DEBUG_TEXT(APP_PB_BAP_SWI_ERR, MOD_APP_PB_BAP, "sfHdlSmsReqFinish sent failed");
        return FAIL;
    }

    DEBUG_TEXT(APP_PB_BAP_SWI_DBG, MOD_APP_PB_BAP, "CallList: Sent StatusArray Response to BAP\n");

    // TODO: put into APP_PHONEBOOK
    //_APP_PHONEBOOK_SMS_vResetSmsContext();

    return SUCCESS;
}



/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_BAP_REQUEST_sfSendSmsChangedArray             */
/**
    \brief      Send BAP Changed Array message for received sms\n

    \brief      Description:\n
                Send BAP Changed Array message for received sms\n

    \return     SuccessFailType
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_PHONEBOOK_BAP_REQUEST_sfSendSmsChangedArray( void )
{
    Uint16Type numSms;

    BapRequest request;
    request.lsg_id = BapLsg_Phonebook;
    request.fct_id = BapFct_Phonebook_SMSReceived;

    numSms = APP_SMS_u16GetNumUnreadSms() +
             APP_SMS_u16GetNumReadSms();

    (void)APP_COMMON_BAP_ARRAY_sfGetChangedArray( 0, numSms, &request);

    if( E_OK != APP_COMMON_UTILS_stSendBapRequest( msgPhoneBookBapRequest, &request))
    {
        DEBUG_VALUE2(APP_PB_BAP_SWI_ERR, MOD_APP_PB_BAP, "SmsChangedArr: %d,%d send failed", 0, numSms);
        return FAIL;
    }

    DEBUG_VALUE2(APP_PB_BAP_SWI_TRC, MOD_APP_PB_BAP, "SmsChangedArr: %d,%d", 0, numSms);

    return SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_BAP_REQUEST_sfSendNewSmsChangedArray             */
/**
    \brief      Send BAP Changed Array message for new received sms\n

    \brief      Description:\n
                Send BAP Changed Array message for new received sms\n

    \return     SuccessFailType
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_PHONEBOOK_BAP_REQUEST_sfSendNewSmsChangedArray( void )
{
    //Uint16Type numSms;
    SuccessFailType result;
    Uint16Type headerLength;
    BapArrayHeaderType sendHeader;
    BapRequest request;
    request.lsg_id      = BapLsg_Phonebook;
    request.fct_id      = BapFct_Phonebook_SMSReceived;
    request.req_type    = BapReq_Changed;  /* ChangedArray optype wanted */
    request.data_type   = BAP_DT_BYTE;

    //numSms = APP_SMS_u16GetNumUnreadSms() +
    //         APP_SMS_u16GetNumReadSms();

    /* init the mode byte with 0 */
    sendHeader.mode.data                = 0;
    sendHeader.mode.common.direction    = 0;
    sendHeader.mode.common.shift        = 1; // old sms are moved to higher positions
    /* set the start value       */
    sendHeader.start                    = 1; // put new sms at the beginning of list
    /* set the elements value    */
    sendHeader.elements                 = 1; // one new sms


    result = POOL_sfAllocate( POOL_enBapArrayHeader,
                              (void**)&request.payload.byte_seq.data);

    if(result != SUCCESS)
    {
        DEBUG_TEXT(APP_PB_BAP_SWI_ERR, MOD_APP_PB_BAP, "POOL_sfAllocate( POOL_enBapArrayHeader) failed");
        return FAIL;
    }

    headerLength = APP_COMMON_BAP_ARRAY_u8WriteHeader(  &sendHeader,
                                                        request.payload.byte_seq.data);

    /* set the size of the contained data  */
    request.payload.byte_seq.length = headerLength;

    //APP_COMMON_BAP_ARRAY_sfGetChangedArray( 0, /*numSms*/0, &request);

    (void)APP_COMMON_UTILS_stSendBapRequest( msgPhoneBookBapRequest, &request);

    return SUCCESS;
}
/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_BAP_REQUEST_sfHandleSmsDeleted                */
/**
    \brief      handles sms delete request finished

    \brief      Description:\n
                handles sms delete request finished \n

    \return     SuccessFailType
                FAIL or SUCCESS
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_PHONEBOOK_BAP_REQUEST_sfHandleSmsDeleted(void)
{
    // answer for sms attribute change request
    ConvArrayHeaderType *pConvHeader;
    BapArrayHeaderType sendHeader;
    BapRequest request;
    //Uint8Type i             = 0;
    SuccessFailType result  = SUCCESS;
    Uint8Type *pDataWriteBuf= NULL;
    request.lsg_id          = BapLsg_Phonebook;
    request.fct_id          = BapFct_Phonebook_SMSReceived;
    request.req_type        = BapReq_Data;
    request.data_type       = BAP_DT_BYTE;

    /* get the array header of the initiating bap message */
    pConvHeader = &gAppPBBapContext.appSms.convHeader;

    /* get the mode byte of the header */
    sendHeader.mode.data    = 0;
    /* set record address to 0x1 */
    sendHeader.mode.common.recordAddress = 0x1;
    /* set transmitPos to 0, cause we will not transmit the pos */
    sendHeader.mode.status.transmitPos  = 0x0;
    /* get the start value of the header */
    sendHeader.start        = pConvHeader->start - 1;
    /* set the elements value to  */
    sendHeader.elements     = pConvHeader->elements;


    result = POOL_sfAllocate( POOL_enBapReqDataType,
                              (void**)&request.payload.byte_seq.data);
    if(result != SUCCESS)
    {
        DEBUG_TEXT(APP_PB_BAP_SWI_ERR, MOD_APP_PB_BAP, "_sfHandleRecvdSms: POOL_sfAllocate failed");

        APP_COMMON_UTILS_vSendBapError( BapLsg_Phonebook,
                                        request.fct_id,
                                        AppErr_Unknown);
        return FAIL;
    }

    pDataWriteBuf = request.payload.byte_seq.data;

    /* check start and end pos to be able to set indexSize */
    if( pConvHeader->start > 0xFF )
    {
        sendHeader.mode.common.indexSize = 1;
    }

    /* preset the header to get the length (3 or 5) */
    pDataWriteBuf += APP_COMMON_BAP_ARRAY_u8WriteHeader(  &sendHeader,
                                                        pDataWriteBuf );

    //for(i=0; i<pConvHeader->elements;i++)
    //{
    //    // write pos
    //    pDataWriteBuf += APP_COMMON_BAP_ARRAY_sfSetPos(pDataWriteBuf,
    //                              pConvHeader->start + i,
    //                              sendHeader.mode.common.indexSize);

        // empty pbName bap string
        *pDataWriteBuf = 0x00;
        pDataWriteBuf++;
        // write attrib
        *pDataWriteBuf = 0x00;  // like in bap indication
        pDataWriteBuf++;
    //}

    request.payload.byte_seq.length = pDataWriteBuf - request.payload.byte_seq.data;
    if( E_OK != APP_COMMON_UTILS_stSendBapRequest(msgPhoneBookBapRequest, &request) )
    {
        DEBUG_VALUE2(APP_PB_BAP_SWI_ERR, MOD_APP_PB_BAP, "SmsDeleted send failed start %d, elems %d",
                     sendHeader.start, sendHeader.elements);
        return FAIL;
    }

    DEBUG_VALUE2(APP_PB_BAP_SWI_TRC, MOD_APP_PB_BAP, "SmsDeleted sent start %d, elems %d",
                 sendHeader.start, sendHeader.elements);

    return SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_BAP_REQUEST_sfSendSmsDeletedChangedArray             */
/**
    \brief      Send BAP Changed Array message for sms deleted\n

    \brief      Description:\n
                Send BAP Changed Array message for sms deleted\n

    \return     SuccessFailType
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_PHONEBOOK_BAP_REQUEST_sfSendSmsDeletedChangedArray( void )
{
    //Uint16Type numSms;
    SuccessFailType result;
    Uint16Type headerLength;
    BapArrayHeaderType sendHeader;
    BapRequest request;
    request.lsg_id      = BapLsg_Phonebook;
    request.fct_id      = BapFct_Phonebook_SMSReceived;
    request.req_type    = BapReq_Changed;  /* ChangedArray optype wanted */
    request.data_type   = BAP_DT_BYTE;

    /* init the mode byte with 0 */
    sendHeader.mode.data                = 0;
    sendHeader.mode.common.direction    = 1;
    sendHeader.mode.common.shift        = 1; // old sms are moved to smaller positions

    // all sms are moved number of positions down ( elements )
    /* set the start value       */
    //sendHeader.start  = gAppPBBapContext.appSms.convHeader.start +
    //                    gAppPBBapContext.appSms.convHeader.elements - 1;
    sendHeader.start  = gAppPBBapContext.appSms.convHeader.start - 1;
    /* set the elements value    */
    sendHeader.elements = gAppPBBapContext.appSms.convHeader.elements;


    result = POOL_sfAllocate( POOL_enBapArrayHeader,
                              (void**)&request.payload.byte_seq.data);

    if(result != SUCCESS)
    {
        DEBUG_TEXT(APP_PB_BAP_SWI_ERR, MOD_APP_PB_BAP, "POOL_sfAllocate( POOL_enBapArrayHeader) failed");
        return FAIL;
    }

    headerLength = APP_COMMON_BAP_ARRAY_u8WriteHeader(  &sendHeader,
                                                        request.payload.byte_seq.data);

    /* set the size of the contained data  */
    request.payload.byte_seq.length = headerLength;
    if(E_OK != APP_COMMON_UTILS_stSendBapRequest( msgPhoneBookBapRequest, &request))
    {
        DEBUG_VALUE2(APP_PB_BAP_SWI_ERR, MOD_APP_PB_BAP, "SendSmsDelChangArr failed: start %d, elems %d",
                     sendHeader.start, sendHeader.elements);
        return FAIL;
    }

    DEBUG_VALUE2(APP_PB_BAP_SWI_TRC, MOD_APP_PB_BAP, "SendSmsDelChangArr succ: start %d, elems %d",
                       sendHeader.start, sendHeader.elements);

    return SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_BAP_REQUEST_sfHandleSmsMarkedAsRead                */
/**
    \brief      handles sms delete request finished

    \brief      Description:\n
                handles sms delete request finished \n

    \return     SuccessFailType
                FAIL or SUCCESS
*/
/*----------------------------------------------------------------------------*/
SuccessFailType
_APP_PHONEBOOK_BAP_REQUEST_sfHandleSmsMarkedAsRead(Uint16Type pos,
                                                   BapSmsType type)
{
    // answer for sms attribute change request
    BapArrayHeaderType sendHeader;
    BapRequest request;
    SuccessFailType result      = SUCCESS;
    Uint8Type *pDataWriteBuf    = NULL;
    request.lsg_id              = BapLsg_Phonebook;
    request.fct_id              = BapFct_Phonebook_SMSReceived;
    request.req_type            = BapReq_Data;
    request.data_type           = BAP_DT_BYTE;

    /* get the mode byte of the header */
    sendHeader.mode.data    = 0;
    /* set record address to 0x2 */
    sendHeader.mode.common.recordAddress = 0x2;
    /* set transmitPos to 0, cause we will not transmit the pos */
    sendHeader.mode.status.transmitPos  = 0x1;
    /* get the start value of the header */
    sendHeader.start        = pos;
    /* set the elements value to 0 */
    sendHeader.elements     = 1;


    result = POOL_sfAllocate( POOL_enBapReqDataType,
                              (void**)&request.payload.byte_seq.data);
    if(result != SUCCESS)
    {
        DEBUG_TEXT(APP_PB_BAP_SWI_ERR, MOD_APP_PB_BAP, "sfHandleSmsMarkedAsRead: POOL_sfAllocate failed");

        APP_COMMON_UTILS_vSendBapError( BapLsg_Phonebook,
                                        BapFct_Phonebook_SMSReceived,
                                        AppErr_Unknown);
        return FAIL;
    }

    pDataWriteBuf = request.payload.byte_seq.data;

    // write array header
    if( pos > 0xFF )
    {
        sendHeader.mode.common.indexSize = 1;
    }
    /* preset the header to get the length (3 or 5) */
    pDataWriteBuf += APP_COMMON_BAP_ARRAY_u8WriteHeader(  &sendHeader,
                                                        pDataWriteBuf );
    // write pos
    pDataWriteBuf += APP_COMMON_BAP_ARRAY_sfSetPos(pDataWriteBuf, pos,
                                  sendHeader.mode.common.indexSize);
    // write attrib
    *pDataWriteBuf = (Uint8Type)type;
    pDataWriteBuf += 1;

    request.payload.byte_seq.length = pDataWriteBuf - request.payload.byte_seq.data;
    if( E_OK != APP_COMMON_UTILS_stSendBapRequest(msgPhoneBookBapRequest, &request) )
    {
        DEBUG_VALUE2(APP_PB_BAP_SWI_ERR, MOD_APP_PB_BAP, "SendSmsMarkedAsRead fail: start %d, elems %d",
                           sendHeader.start, sendHeader.elements);
        return FAIL;
    }

    DEBUG_VALUE2(APP_PB_BAP_SWI_TRC, MOD_APP_PB_BAP, "SendSmsMarkedAsRead succ: start %d, elems %d",
                        sendHeader.start, sendHeader.elements);

    return SUCCESS;
}
/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_BAP_REQUEST_sfSendPbSpellerResult             */
/**
    \brief      build up pb speller result \n

    \brief      Description:\n
                build up pb speller result \n

    \return     SuccessFailType
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType
_APP_PHONEBOOK_BAP_REQUEST_sfSendPbSpellerResult(Uint16Type index,
                                                 Uint16Type number)
{
    SuccessFailType result  = SUCCESS;
    Uint8Type *pDataWriteBuf;

    BapRequest bapReq;

    /* build the status request and send it to BAP-Dispatcher */
    bapReq.lsg_id       = BapLsg_Phonebook;
    bapReq.fct_id       = BapFct_Phonebook_PbSpeller;
    bapReq.req_type     = BapReq_Result;
    bapReq.data_type    = BAP_DT_BYTE;

    result = POOL_sfAllocate( POOL_enBapReqDataType,
                              (void**)&bapReq.payload.byte_seq.data);

    if(result != SUCCESS)
    {
        DEBUG_TEXT(APP_PB_BAP_SWI_ERR, MOD_APP_PB_BAP, "POOL_sfAllocate( POOL_enBapReqDataType ) failed");
        return FAIL;
    }

    pDataWriteBuf = bapReq.payload.byte_seq.data;

    // TODO: check result and ggf. change index according to speller mode

    // put values into send buffer
    APP_COMMON_UTILS_vSetUint16Type(number, pDataWriteBuf);
    pDataWriteBuf+= 2;
    APP_COMMON_UTILS_vSetUint16Type(index, pDataWriteBuf);
    pDataWriteBuf+= 2;

    bapReq.payload.byte_seq.length = pDataWriteBuf - bapReq.payload.byte_seq.data;

    // write result into bap byte seq and send it
    if( E_OK != APP_COMMON_UTILS_stSendBapRequest( msgPhoneBookBapRequest , &bapReq ))
    {
        return FAIL;
    }

    DEBUG_VALUE2(APP_PB_BAP_SWI_TRC, MOD_APP_PB_BAP, "PbSpeller: Sent Response to BAP: index: %d, num: %d\n",
                index, number);

    return SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_SMS_sfSendCallListStatusArray                 */
/**
    \brief      build up CallList array for BAP\n

    \brief      Description:\n
                build up CallList array for BAP \n

    \return     SuccessFailType
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType
_APP_PHONEBOOK_BAP_REQUEST_sfSendCallListStatusArray(fctId_t callFctId)
{
    SuccessFailType result  = SUCCESS;
    Uint8Type j             = 0;
    Uint16Type i            = 0;
    Uint8Type headerLength  = 0;
    Uint16Type pos          = 0;
    Uint16Type numPosBytes  = 0;
    Uint8Type pnType        = (Uint8Type)BAP_NR_TYPE_UNKNOWN;

    BapArrayHeaderType *pRecvHeader;
    ConvArrayHeaderType *pConvHeader;
    BapArrayHeaderType sendHeader;
    Uint8Type *pDataWriteBuf;

    BapRequest bapReq;

    /* build the status request and send it to BAP-Dispatcher */
    bapReq.lsg_id       = BapLsg_Phonebook;
    bapReq.fct_id       = callFctId;
    bapReq.req_type     = BapReq_Data;  // StatusArray optype wanted
    bapReq.data_type    = BAP_DT_BYTE;

    /* make sure we got a valid bap fctId */
    switch( callFctId )
    {
        case BapFct_Phonebook_MissedCalls:
            /* get the array header of the initiating bap message */
            pRecvHeader = &gAppPBBapContext.appMissedPb.arrayHeader;
            pConvHeader = &gAppPBBapContext.appMissedPb.convHeader;
            break;
        case BapFct_Phonebook_ReceivedCalls:
            /* get the array header of the initiating bap message */
            pRecvHeader = &gAppPBBapContext.appReceivedPb.arrayHeader;
            pConvHeader = &gAppPBBapContext.appReceivedPb.convHeader;
            break;
        case BapFct_Phonebook_DialedNumbers:
            /* get the array header of the initiating bap message */
            pRecvHeader = &gAppPBBapContext.appDialedPb.arrayHeader;
            pConvHeader = &gAppPBBapContext.appDialedPb.convHeader;
            break;
        default:
            return FAIL;
    }

    /* get the mode byte of the header */
    sendHeader.mode.data    = pRecvHeader->mode.data;
    /* set transmitPos to 0, cause we will not transmit the pos */
    //sendHeader.mode.status.transmitPos  = 0x0;
    /* get the start value of the header */
    sendHeader.start        = pRecvHeader->start;
    /* set the elements value to 0 */
    sendHeader.elements     = 0;

    result = POOL_sfAllocate( POOL_enBapReqDataType,
            (void**)&bapReq.payload.byte_seq.data);

    if(result != SUCCESS)
    {
        DEBUG_TEXT(APP_PB_BAP_SWI_ERR, MOD_APP_PB_BAP, "POOL_sfAllocate( POOL_enBapReqDataType ) failed");
        return FAIL;
    }

    /* check start and end pos to be able to set indexSize */
    if( pConvHeader->start > 0xFF ||
            ( pConvHeader->start + pConvHeader->elements ) > 0xFF )
    {
        sendHeader.mode.common.indexSize = 1;
    }

    /* preset the header to get the length (3 or 5) */
    headerLength = APP_COMMON_BAP_ARRAY_u8WriteHeader(  &sendHeader,
            bapReq.payload.byte_seq.data );

    pDataWriteBuf = bapReq.payload.byte_seq.data + headerLength;

    // get correct start/endIndex depending on direction
    pos = ( BT_TRUE == pConvHeader->forward ) ?
            pConvHeader->start :
    pConvHeader->start + pConvHeader->elements - 1;

    /* build up the array content */
    for(j=0; j<MAX_REQUESTED_PB_ENTRIES; j++)
    {
        Uint16Type tmpLen = 0;
        const AppPbEntryType * pEntry = NULL;

        i = (BT_TRUE == pConvHeader->forward) ?
                j : pConvHeader->elements - j - 1 ;

        pEntry = APP_PHONEBOOK_pGetPpbrContext(i);

        if(i >= MAX_REQUESTED_PB_ENTRIES ||
                pEntry == NULL                || BT_FALSE == pEntry->isValid)
        {
            // skip empty
            pos = (BT_TRUE == pConvHeader->forward) ?
                    pos + 1 : pos - 1 ;
            continue;
        }
        if(sendHeader.elements >= pConvHeader->elements)
        {
            DEBUG_VALUE2(APP_PB_BAP_SWI_ERR, MOD_APP_PB_BAP, "CallListStatusArray break: send hdr elems %d, conv hdr elems %d",
                    sendHeader.elements, pConvHeader->elements);
            break;
        }
        /* increase the number of records by one */
        sendHeader.elements++;

        /* skip writing the positon of the record cause HUs like Blaupunkt are
         * not able to handle position even if TransmitPos bit is set
         * see the next comment */

        /* write the positon of the record, EARLIER: not needed because the known problem with HUs
         * NOW: after discussing this issue with H.Diekelmann on the 20.10.2010(#4832) the transfer of position has to
         * be done again. Please let VW confirm this change again before integrating in the final release. */
        if((pRecvHeader->mode.common.shift == 1)||(pRecvHeader->mode.common.transmitPos == 1))
        {
            numPosBytes = APP_COMMON_BAP_ARRAY_sfSetPos(pDataWriteBuf,
                    pos, sendHeader.mode.common.indexSize);
            pDataWriteBuf += numPosBytes;
        }
        /*-----------------------------------*/
        // NEEDED FOR RECORD ADDRESS 0, 1    //
        /* write the content of PbName       */
        if( 0 == pRecvHeader->mode.common.recordAddress ||
                1 == pRecvHeader->mode.common.recordAddress)
        {
            /* write the content of PbName */
            // pPpbr->name should always contain either name or number ( if not in pb )
            tmpLen = APP_COMMON_UTILS_u16WriteBapString(
                    pDataWriteBuf,
                    pEntry->ppbr.name,
                    pEntry->ppbr.name_len);
            pDataWriteBuf += tmpLen;

            /* ----------------- */
            /* write number type */
            *pDataWriteBuf = (Uint8Type)BAP_NR_TYPE_UNKNOWN;   // default pn type
            if( pEntry->ppbr.pn_type_numer_available )
            {
                pnType = _APP_PHONEBOOK_BAP_enGetAT2BAPPnTypeMapping(pEntry->ppbr.pn_type);
                (void)memcpy( pDataWriteBuf, &pnType, 1);
            }
            pDataWriteBuf += 1;
        }

        if( 0 == pRecvHeader->mode.common.recordAddress ||
                2 == pRecvHeader->mode.common.recordAddress)
        {
            /*--------------------------------*/
            /* write the content of TelNumber */
            if( pEntry->ppbr.phone_number_len )
            {
                tmpLen = APP_COMMON_UTILS_u16WriteBapString(
                        pDataWriteBuf,
                        pEntry->ppbr.phone_number,
                        pEntry->ppbr.phone_number_len);
                pDataWriteBuf += tmpLen;
            }
            else
            {
                /* send null string for TelNumber */
                tmpLen = APP_COMMON_UTILS_u16WriteBapString( pDataWriteBuf,
                        (Uint8Type*)"\0",
                        1);
                pDataWriteBuf += tmpLen;
            }

            /* -------------------------------------------- */
            /* write Day, Month, Year, Hour, Minute, Second */
            // as we do not support date and time, set Day,
            // month, year, hour, minute and second to 0xFF
            (void)memset( pDataWriteBuf, 0xFF, 6 );
            pDataWriteBuf += 6;
        }
        pos = (BT_TRUE == pConvHeader->forward) ? pos + 1 : pos - 1;
    }

    /* activate the position bit if we have elements to send*/
    if((sendHeader.elements > 0) &&
            ((pRecvHeader->mode.common.shift == 1)||(pRecvHeader->mode.common.transmitPos == 1)))
    {
        sendHeader.mode.status.transmitPos = 1;
    }
    else
    {
        sendHeader.mode.status.transmitPos = 0;
    }
    /* write the actual header to the start of the buffer */
    headerLength = APP_COMMON_BAP_ARRAY_u8WriteHeader(  &sendHeader,
            bapReq.payload.byte_seq.data );

    /* set the size of the byte sequence */
    bapReq.payload.byte_seq.length =
            (Uint16Type)(pDataWriteBuf - bapReq.payload.byte_seq.data);


    /* send callInfo bapRequest */
    if( E_OK != APP_COMMON_UTILS_stSendBapRequest( msgPhoneBookBapRequest , &bapReq ))
    {
        DEBUG_VALUE1(APP_PB_BAP_SWI_ERR, MOD_APP_PB_BAP, "CallListStatusArr,send FAIL fct: %d ...",
                callFctId);
        DEBUG_VALUE2(APP_PB_BAP_SWI_ERR, MOD_APP_PB_BAP, "CallListStatusArr,send FAIL ... start: %d, elems: %d",
                sendHeader.start, sendHeader.elements);
        return FAIL;
    }
    else
    {
        // gAppPBBapContext.StatusArrayPending = BT_TRUE;  /*RT13652*/
        switch(callFctId)
        {
            case BapFct_Phonebook_MissedCalls:
                gAppPBBapContext.enStatusArrayPending = APP_PB_BAP_CHANGED_ARRAY_MISSEDCALLS;
                break;

            case BapFct_Phonebook_ReceivedCalls:
                gAppPBBapContext.enStatusArrayPending = APP_PB_BAP_CHANGED_ARRAY_RECEIVEDCALLS;
                break;

            case BapFct_Phonebook_DialedNumbers:
                gAppPBBapContext.enStatusArrayPending = APP_PB_BAP_CHANGED_ARRAY_DIALEDCALLS;
                break;

            default:
                return FAIL;
        }
    }

    DEBUG_VALUE1(APP_PB_BAP_SWI_TRC, MOD_APP_PB_BAP, "CallListStatusArr, fct: %d ...",
            callFctId);
    DEBUG_VALUE2(APP_PB_BAP_SWI_TRC, MOD_APP_PB_BAP, "CallListStatusArr, ... start: %d, elems: %d",
            sendHeader.start, sendHeader.elements);

    return SUCCESS;
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_BAP_REQUEST_sfSendEmptyCallListStatusArray()  */
/**
    \brief      send Calllist BapRequest indicating that there are no entries
                at this position  \n

    \brief      Description:\n
                send Calllist BapRequest indicating that there are no entries
                at this position   \n

    \return     SuccessFailType
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType
_APP_PHONEBOOK_BAP_REQUEST_sfSendEmptyCallListStatusArray(BapArrayHeaderType *pRecvHeader,
                                                          fctId_t fctId )
{
    BapRequest bapReq;
    BapArrayHeaderType sendHeader;
    SuccessFailType result          = SUCCESS;
    Uint8Type headerLength          = 0;
    Uint8Type *pDataWriteBuf        = NULL;

    if(NULL == pRecvHeader)
    {
        DEBUG_TEXT(APP_PB_BAP_SWI_ERR, MOD_APP_PB_BAP, "EmptCallListStatusArry: pRecvHdr NULL");
        return FAIL;
    }

    /* build the request and send it to BAP-Dispatcher */
    bapReq.lsg_id       = BapLsg_Phonebook;
    bapReq.fct_id       = fctId;                          // copy fctId
    bapReq.req_type     = BapReq_Data;                    /* Property Status */
    bapReq.data_type    = BAP_DT_BYTE;

    /* get the mode byte of the header */
    sendHeader.mode.data        = pRecvHeader->mode.data;
    /* transmitting position byte is not required here, cause we have 0 elements */
    sendHeader.mode.common.transmitPos = 0;
    /* get the start value of the header */
    sendHeader.start            = pRecvHeader->start;
    /* preset set the elements value to 0 */
    sendHeader.elements         = 0;
    result = POOL_sfAllocate( POOL_enBapReqDataType,
                              (void**)&bapReq.payload.byte_seq.data);

    if(result != SUCCESS)
    {
        DEBUG_TEXT(APP_PB_BAP_SWI_ERR, MOD_APP_PB_BAP, "POOL_sfAllocate(POOL_enBapReqDataType) failed");
        return FAIL;
    }

    /* check start and end pos to be able to set indexSize */
    if( sendHeader.start > 0xFF )
    {
        sendHeader.mode.common.indexSize = 1;
    }

    /* preset the header to get the length (3 or 5) */
    headerLength = APP_COMMON_BAP_ARRAY_u8WriteHeader(  &sendHeader,
                                                        bapReq.payload.byte_seq.data );

    pDataWriteBuf = bapReq.payload.byte_seq.data + headerLength;

    /* set the size of the byte sequence */
    bapReq.payload.byte_seq.length =
           (Uint16Type)(pDataWriteBuf - bapReq.payload.byte_seq.data);

    /* send bapRequest */
    if( E_OK != APP_COMMON_UTILS_stSendBapRequest( msgPhoneBookBapRequest, &bapReq ))
    {
        return FAIL;
    }

    return result;
}



/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_BAP_sfGetAT2BAPPnTypeMapping                  */
/**
    \brief      map AT phone number type into bap pn type \n

    \brief      Description:\n
                map AT phone number type into bap pn type \n

    \return     SuccessFailType
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
static Uint8Type
_APP_PHONEBOOK_BAP_enGetAT2BAPPnTypeMapping(ATPhoneNumberTypeEnum atPnType)
{
    Uint8Type retVal = 0;
    BapNumberType bapPnType = BAP_NR_TYPE_UNKNOWN;

    switch( atPnType )
    {
        case AT_PNT_GENERAL:
           bapPnType = BAP_NR_TYPE_GENERAL;
            break;
        case AT_PNT_HOME:
            bapPnType = BAP_NR_TYPE_HOME;
            break;
        case AT_PNT_OFFICE:
            bapPnType = BAP_NR_TYPE_OFFICE;
            break;
        case AT_PNT_MOBILE:
            bapPnType = BAP_NR_TYPE_MOBILE;
            break;
        default:
            bapPnType = BAP_NR_TYPE_UNKNOWN;
            break;
    }
    // just take lower byte
    retVal = ( (Uint8Type)bapPnType & 0xFF );
    return retVal;
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_BAP_REQUEST_sfSendNewPbEntryChangedArray             */
/**
    \brief      Send BAP Changed Array message for new pb entry\n

    \brief      Description:\n
                Send BAP Changed Array message for new pb entry\n

    \param      newPbIndex
                Uint16Type
                index of newly added pb entry

    \return     SuccessFailType
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType
_APP_PHONEBOOK_BAP_REQUEST_sfSendNewPbEntryChangedArray( Uint16Type newPbIndex )
{
    //Uint16Type numSms;
    SuccessFailType result;
    Uint16Type headerLength;
    BapArrayHeaderType sendHeader;
    BapRequest request;
    request.lsg_id      = BapLsg_Phonebook;
    request.fct_id      = BapFct_Phonebook_Phonebook;
    request.req_type    = BapReq_Changed;  /* ChangedArray optype wanted */
    request.data_type   = BAP_DT_BYTE;

    /* init the mode byte with 0 */
    sendHeader.mode.data                = 0;
    sendHeader.mode.common.direction    = 0;
    sendHeader.mode.common.shift        = 1; // old pb entries are moved to higher positions
    /* set the start value       */
    sendHeader.start                    = newPbIndex; // put new pb index
    /* set the elements value    */
    sendHeader.elements                 = 1; // one new pb entry


    result = POOL_sfAllocate( POOL_enBapArrayHeader,
                              (void**)&request.payload.byte_seq.data);

    if(result != SUCCESS)
    {
        DEBUG_TEXT(APP_PB_BAP_SWI_ERR, MOD_APP_PB_BAP, "POOL_sfAllocate( POOL_enBapArrayHeader) failed");
        return FAIL;
    }

    headerLength = APP_COMMON_BAP_ARRAY_u8WriteHeader(  &sendHeader,
                                                        request.payload.byte_seq.data);

    /* set the size of the contained data  */
    request.payload.byte_seq.length = headerLength;
    if(E_OK != APP_COMMON_UTILS_stSendBapRequest( msgPhoneBookBapRequest, &request))
    {
        DEBUG_VALUE1(APP_PB_BAP_SWI_ERR, MOD_APP_PB_BAP, "Indicat. new PbIdx %d FAIL", newPbIndex);
        return FAIL;
    }

    DEBUG_VALUE1(APP_PB_BAP_SWI_TRC, MOD_APP_PB_BAP, "Indicated new PbIdx %d", newPbIndex);

    return SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_BAP_REQUEST_sfSendPbChangedArray             */
/**
    \brief      Send BAP Changed Array message with start and elems \n

    \brief      Description:\n
                Send BAP Changed Array message with start and elems \n

    \param      start
                Uint16Type
                startIndex of changed part

    \param      reqElements
                Uint16Type
                number of changed elements

    \return     SuccessFailType
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType
_APP_PHONEBOOK_BAP_REQUEST_sfSendPbChangedArray( Uint16Type start, Uint16Type reqElements )
{
    //Uint16Type numSms;
    BapRequest request;
    request.lsg_id      = BapLsg_Phonebook;
    request.fct_id      = BapFct_Phonebook_Phonebook;
    request.req_type    = BapReq_Changed;  /* ChangedArray optype wanted */
    request.data_type   = BAP_DT_BYTE;

/*RT13652*/
#if 0
    /* check for pending status array transmission */
    if (gAppPBBapContext.StatusArrayPending == BT_FALSE)
    {
        /* set the size of the contained data  */
        (void)APP_COMMON_BAP_ARRAY_sfGetChangedArray( 0, /* pbSize */ 0xFFFF, &request);
        if(E_OK != APP_COMMON_UTILS_stSendBapRequest( msgPhoneBookBapRequest, &request))
        {
            DEBUG_VALUE2(APP_PB_BAP_SWI_ERR, MOD_APP_PB_BAP, "PbChanged %d,%d send FAIL", start, reqElements);
            return FAIL;
        }
    }
    else
    {
        (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_PHONEBOOK_BAP_FCT_PENDING, MSEC(300), 0);
    }
#endif

    /* 15108: [TODO SOP4]:implement the handling for PhoneBook handling in SOP4 */
    /* check for pending status array transmission */
    //if (APP_PB_BAP_CHANGED_ARRAY_PHONEBOOK == gAppPBBapContext.enStatusArrayPending)
    //{
    //    DEBUG_TEXT(APP_PB_BAP_SWI_ERR, MOD_APP_PB_BAP, "PbChanged send FAIL: Same type of changed array(Phone book) sending status pending");
    //    return FAIL;
    //}
    //else
    //{
        /* set the size of the contained data  */
          (void)APP_COMMON_BAP_ARRAY_sfGetChangedArray( 0, /* pbSize */ 0xFFFF, &request);
          if(E_OK != APP_COMMON_UTILS_stSendBapRequest( msgPhoneBookBapRequest, &request))
          {
              DEBUG_VALUE2(APP_PB_BAP_SWI_ERR, MOD_APP_PB_BAP, "PbChanged %d,%d send FAIL", start, reqElements);
              return FAIL;
          }
    //}

    DEBUG_VALUE2(APP_PB_BAP_SWI_TRC, MOD_APP_PB_BAP, "PbChanged %d,%d", start, reqElements);

    return SUCCESS;
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_BAP_sfSendPhonebookPosOnlyStatusArray                 */
/**
    \brief      build up Phonebook array for BAP\n

    \brief      Description:\n
                build up Phonebook array for BAP \n

    \return     SuccessFailType
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType
_APP_PHONEBOOK_BAP_REQUEST_sfSendPhonebookPosOnlyStatusArray(const BapArrayHeaderType *pRecvHeader,
                                                      const ConvArrayHeaderType *pConvHeader )
{
    SuccessFailType result  = SUCCESS;

    Uint16Type i            = 0;
    Uint8Type headerLength  = 0;
    Uint16Type pos          = 0;
    Uint16Type numPosBytes  = 0;

    BapRequest bapReq;
    BapArrayHeaderType sendHeader;
    Uint8Type *pDataWriteBuf;

    /* build the status request and send it to BAP-Dispatcher */
    bapReq.lsg_id       = BapLsg_Phonebook;
    bapReq.fct_id       = BapFct_Phonebook_Phonebook;
    bapReq.req_type     = BapReq_Data;  // StatusArray optype wanted
    bapReq.data_type    = BAP_DT_BYTE;

    /* get the mode byte of the header */
    sendHeader.mode.data    = pRecvHeader->mode.data;
    /* get the start value of the header */
    sendHeader.start        = pRecvHeader->start;
    /* set the elements value to 0 */
    sendHeader.elements     = 0;

    result = POOL_sfAllocate( POOL_enBapReqDataType,
                              (void**)&bapReq.payload.byte_seq.data);
    if(result != SUCCESS)
    {
        DEBUG_TEXT(APP_PB_BAP_SWI_ERR, MOD_APP_PB_BAP, "POOL_sfAllocate( POOL_enBapReqDataType ) failed");
        return FAIL;
    }

    /* check start and end pos to be able to set indexSize */
    if( pConvHeader->start > 0xFF ||
        ( pConvHeader->start + pConvHeader->elements ) > 0xFF )
    {
        sendHeader.mode.common.indexSize = 1;
    }

    /* preset the header to get the length (3 or 5) */
    headerLength = APP_COMMON_BAP_ARRAY_u8WriteHeader(  &sendHeader,
                                                        bapReq.payload.byte_seq.data );

    pDataWriteBuf = bapReq.payload.byte_seq.data + headerLength;

    // get correct start/endIndex depending on direction
    //pos = ( BT_TRUE == pConvHeader->forward ) ?
    //        pConvHeader->start :
    //        pConvHeader->start + pConvHeader->elements - 1;
    /* build up the array content */
    for(i=0; i<pConvHeader->elements; i++)
    {
        pos = ( BT_TRUE == pConvHeader->forward ) ?
                pConvHeader->start + i : (pConvHeader->start + pConvHeader->elements - 1)/* last pos */ - i;

        /* write the positon of the record */
        numPosBytes = APP_COMMON_BAP_ARRAY_sfSetPos(pDataWriteBuf,
                                pos, sendHeader.mode.common.indexSize);
        pDataWriteBuf += numPosBytes;

        /* increase the number of records by one */
        sendHeader.elements++;
    }

    /* activate the position bit if we have elements to send*/
    sendHeader.mode.status.transmitPos = (sendHeader.elements > 0) ? 1 : 0;

    /* write the actual header to the start of the buffer */
    headerLength = APP_COMMON_BAP_ARRAY_u8WriteHeader(  &sendHeader,
                                                        bapReq.payload.byte_seq.data );

    /* set the size of the byte sequence */
    bapReq.payload.byte_seq.length =
           (Uint16Type)(pDataWriteBuf - bapReq.payload.byte_seq.data);

    /* send callInfo bapRequest */
    if( E_OK != APP_COMMON_UTILS_stSendBapRequest( msgPhoneBookBapRequest , &bapReq ))
    {
        DEBUG_TEXT(APP_PB_BAP_SWI_ERR, MOD_APP_PB_BAP, "Send Phonebook rsp for rA 0xF to BAP FAIL");
        return FAIL;
    }

    DEBUG_TEXT(APP_PB_BAP_SWI_TRC, MOD_APP_PB_BAP, "Send Phonebook rsp for rA 0xF to BAP");
    return SUCCESS;
}



/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_SMS_sfSendPhonebookStatusArray                 */
/**
    \brief      build up Phonebook array for BAP\n

    \brief      Description:\n
                build up Phonebook array for BAP \n

    \return     SuccessFailType
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType
_APP_PHONEBOOK_BAP_REQUEST_sfSendPhonebookStatusArray()
{
    SuccessFailType result  = SUCCESS;

    Uint16Type i            = 0;
    Uint16Type k            = 0;
    Uint16Type j            = 0;
    Uint8Type headerLength  = 0;
    Uint16Type pos          = 0;
    Uint16Type numPosBytes  = 0;

    BapRequest bapReq;
    BapArrayHeaderType *pRecvHeader;
    ConvArrayHeaderType *pConvHeader;
    BapArrayHeaderType sendHeader;
    Uint8Type *pDataWriteBuf;

    /* build the status request and send it to BAP-Dispatcher */
    bapReq.lsg_id       = BapLsg_Phonebook;
    bapReq.fct_id       = BapFct_Phonebook_Phonebook;
    bapReq.req_type     = BapReq_Data;  // StatusArray optype wanted
    bapReq.data_type    = BAP_DT_BYTE;

    /* get the array header of the initiating bap message */
    pRecvHeader = &gAppPBBapContext.appUserPb.arrayHeader;
    pConvHeader = &gAppPBBapContext.appUserPb.convHeader;

    /* get the mode byte of the header */
    sendHeader.mode.data    = pRecvHeader->mode.data;
    /* get the start value of the header */
    sendHeader.start        = pRecvHeader->start;
    /* set the elements value to 0 */
    sendHeader.elements     = 0;

    result = POOL_sfAllocate( POOL_enBapReqDataType,
                              (void**)&bapReq.payload.byte_seq.data);

    if(result != SUCCESS)
    {
        DEBUG_TEXT(APP_PB_BAP_SWI_ERR, MOD_APP_PB_BAP, "POOL_sfAllocate( POOL_enBapReqDataType ) failed");
        return FAIL;
    }

    /* check start and end pos to be able to set indexSize */
    if( pConvHeader->start > 0xFF ||
        ( pConvHeader->start + pConvHeader->elements ) > 0xFF )
    {
        sendHeader.mode.common.indexSize = 1;
    }

    /* preset the header to get the length (3 or 5) */
    headerLength = APP_COMMON_BAP_ARRAY_u8WriteHeader(  &sendHeader,
                                                        bapReq.payload.byte_seq.data );

    pDataWriteBuf = bapReq.payload.byte_seq.data + headerLength;

    // get correct start/endIndex depending on direction
    pos = ( BT_TRUE == pConvHeader->forward ) ?
            pConvHeader->start :
            pConvHeader->start + pConvHeader->elements - 1;

    /* build up the array content */
    for(j=0; j<MAX_REQUESTED_PB_ENTRIES; j++)
    {
        Uint16Type tmpLen = 0;
        const AppPbEntryType * pEntry = NULL;

        i = (BT_TRUE == pConvHeader->forward) ?
               j : pConvHeader->elements - j - 1 ;

        pEntry = APP_PHONEBOOK_pGetPpbrContext(i);
        if(i >= MAX_REQUESTED_PB_ENTRIES ||
           pEntry == NULL                || BT_FALSE == pEntry->isValid)
        {
            // skip empty
            pos = (BT_TRUE == pConvHeader->forward) ?
                    pos + 1 : pos - 1 ;
            continue;
        }
        if(sendHeader.elements >= pConvHeader->elements)
        {
            break;
        }
        /* increase the number of records by one */
        sendHeader.elements++;

        // ---------------------------------//
        // NEEDED IN ANY CASE               //
        /* write the positon of the record */
        if((pRecvHeader->mode.common.shift == 1)||(pRecvHeader->mode.common.transmitPos == 1))
        {
            numPosBytes = APP_COMMON_BAP_ARRAY_sfSetPos(pDataWriteBuf,
                                pos, sendHeader.mode.common.indexSize);
            pDataWriteBuf += numPosBytes;
        }
        /*-----------------------------------*/
        // NEEDED FOR RECORD ADDRESS 0, 1, 3 //
        /* write the content of PbName       */
        if( 0 == pRecvHeader->mode.common.recordAddress ||
            1 == pRecvHeader->mode.common.recordAddress ||
            3 == pRecvHeader->mode.common.recordAddress )
        {
            // pPpbr->name should always contain either name or number ( if not in pb )
            tmpLen = APP_COMMON_UTILS_u16WriteBapString(
                                pDataWriteBuf,
                                pEntry->ppbr.name,
                                pEntry->ppbr.name_len);
            pDataWriteBuf += tmpLen;
        }

        /* ------------------------------------------- */
        // NEEDED FOR RECORD ADDRESS 0, 1, 3           //
        // TODO: add Any Voice tag ( upper nibble ) and
        // TelNumberQuantity ( lower Nibble )
        if( 0 == pRecvHeader->mode.common.recordAddress ||
            1 == pRecvHeader->mode.common.recordAddress ||
            3 == pRecvHeader->mode.common.recordAddress )
        {
            *pDataWriteBuf = 0x00;  // first set all bits to zero
            // copy voice tag val
            APP_COMMON_UTILS_vSetUpperNibble( pDataWriteBuf,
                                              (Uint8Type)pEntry->ppbr.voice_tag_recorded);
            // copy pn_size to BAP's TelNumberQuantity
            APP_COMMON_UTILS_vSetLowerNibble( pDataWriteBuf,
                                              pEntry->ppbr.pn_size);
            pDataWriteBuf += 1;
        }
        /*----------------------------------------------------------------*/
        // TelNumber NEEDED FOR RECORD ADDRESS 0, 2                       //
        /* TODO: write the content of TelNumber(s) TelNumberQuanity times */

        // For more than one TelNumber its important to send TelNumber and NumberType
        // in a consecutive manner, not all numbers first, followed by the same
        // number of numberTypes
        if( pEntry->ppbr.pn_size == 1)
        {
            // if we have only one number, take number and numberType from PPBR
            if( 0 == pRecvHeader->mode.common.recordAddress ||
                2 == pRecvHeader->mode.common.recordAddress )
            {
                tmpLen = APP_COMMON_UTILS_u16WriteBapString( pDataWriteBuf,
                                pEntry->ppbr.phone_number,
                                pEntry->ppbr.phone_number_len);
                pDataWriteBuf += tmpLen;
            }
            if( 0 == pRecvHeader->mode.common.recordAddress ||
                2 == pRecvHeader->mode.common.recordAddress ||
                3 == pRecvHeader->mode.common.recordAddress )
            {
                Uint8Type pnType    = 0x00;
                *pDataWriteBuf      = 0x00;    // voice tag (upper nibble) not supported
                pnType = _APP_PHONEBOOK_BAP_enGetAT2BAPPnTypeMapping(pEntry->ppbr.pn_type);
                APP_COMMON_UTILS_vSetLowerNibble( pDataWriteBuf, pnType);
                pDataWriteBuf       += 1;
            }
        }
        // for more than one numbers, take numbers and NumberTypes from PPBP and
        // add them immediately behind PPBR data
        else
        {
            for(k=0; k<MAX_REQUESTED_PB_NUMBERS_PER_ENTRY; k++)
            {
                // if we have more than one number, take them from PPBPs
                if( pEntry->number[k].isValid)
                {
                    if( 0 == pRecvHeader->mode.common.recordAddress ||
                        2 == pRecvHeader->mode.common.recordAddress )
                    {
                        tmpLen = APP_COMMON_UTILS_u16WriteBapString(
                                pDataWriteBuf,
                                pEntry->number[k].ppbp.phone_number,
                                pEntry->number[k].ppbp.phone_number_len);
                        pDataWriteBuf += tmpLen;
                    }
                    if( 0 == pRecvHeader->mode.common.recordAddress ||
                        2 == pRecvHeader->mode.common.recordAddress ||
                        3 == pRecvHeader->mode.common.recordAddress )
                    {
                        // if we have more than one numbers, take pnType from PPBPs
                        Uint8Type pnType    = 0x00;
                        *pDataWriteBuf      = 0x00;    // voice tag (upper nibble) not supported
                        /* write number type to lower nibble */
                        pnType = _APP_PHONEBOOK_BAP_enGetAT2BAPPnTypeMapping(
                                    pEntry->number[k].ppbp.pn_type);
                        APP_COMMON_UTILS_vSetLowerNibble( pDataWriteBuf, pnType);
                        pDataWriteBuf += 1;
                    }
                }
            }
        }
        pos = (BT_TRUE == pConvHeader->forward) ? pos + 1 : pos - 1;
    }

    /* activate the position bit if we have elements to send*/
    if((sendHeader.elements > 0) &&
       ((pRecvHeader->mode.common.shift == 1)||(pRecvHeader->mode.common.transmitPos == 1)))
    {
        sendHeader.mode.status.transmitPos = 1;
    }
    else
    {
        sendHeader.mode.status.transmitPos = 0;
    }

    /* write the actual header to the start of the buffer */
    headerLength = APP_COMMON_BAP_ARRAY_u8WriteHeader(  &sendHeader,
                                                        bapReq.payload.byte_seq.data );

    /* set the size of the byte sequence */
    bapReq.payload.byte_seq.length =
           (Uint16Type)(pDataWriteBuf - bapReq.payload.byte_seq.data);

    /* send callInfo bapRequest */
    if( E_OK != APP_COMMON_UTILS_stSendBapRequest( msgPhoneBookBapRequest , &bapReq ))
    {
        return FAIL;
    }
    //else
    //{
        //gAppPBBapContext.StatusArrayPending = BT_TRUE; /*RT13652*/
        //gAppPBBapContext.enStatusArrayPending = APP_PB_BAP_CHANGED_ARRAY_PHONEBOOK;/* 15108: [TODO SOP4]:implement the handling for SOP4 */
    //}

    DEBUG_TEXT(APP_PB_BAP_SWI_DBG, MOD_APP_PB_BAP, "CallList: Sent StatusArray Response to BAP\n");
    return SUCCESS;
}


SuccessFailType
_APP_PHONEBOOK_BAP_REQUEST_sfSendMobServSupp(const PbMobileServiceSupport *pPbMss)
{
    BapRequest bapReq;
    SuccessFailType result = SUCCESS;

    /* build the request and send it to BAP-Dispatcher */
    bapReq.lsg_id       = BapLsg_Phonebook;
    bapReq.fct_id       = BapFct_Phonebook_MobileServiceSupport;
    bapReq.req_type     = BapReq_Data;      /* Property Status */
    bapReq.data_type    = BAP_DT_BYTE;

    // allocate 6 byte buffer for Mobile service support
    result = POOL_sfAllocate( POOL_enBapReqDataType,
                              (void**)&bapReq.payload.byte_seq.data);

    if(result != SUCCESS)
    {
        DEBUG_TEXT(APP_PB_BAP_SWI_ERR, MOD_APP_PB_BAP, "POOL_sfAllocate(POOL_enBapSixByteSeq) failed");
        return FAIL;
    }

    bapReq.payload.byte_seq.length = sizeof(*pPbMss);

    (void)memcpy( bapReq.payload.byte_seq.data,
            pPbMss,
            bapReq.payload.byte_seq.length);


    /* send bapRequest */
    if( E_OK != APP_COMMON_UTILS_stSendBapRequest( msgPhoneBookBapRequest , &bapReq ))
    {
        return FAIL;
    }

    // sending new mobile service support was successful. Save it in pb context
    gAppPBBapContext.mobServSupp = *pPbMss;

    return SUCCESS;
}



/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_BAP_REQUEST_sfSendMissedCallListChangedArray  */
/**
    \brief      handles notification that missed call list changed

    \brief      Description:\n
                handles notification that missed call list changed\n

    \return     SuccessFailType
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_PHONEBOOK_BAP_REQUEST_sfSendMissedCallListChangedArray()
{
    BapRequest request;
    Uint16Type elems    = 0xFFFF;
    request.lsg_id      = BapLsg_Phonebook;
    request.fct_id      = BapFct_Phonebook_MissedCalls;

    //Von: Diekelmann, Heiko (EXTERN: ECK*IT) [mailto:extern.heiko.diekelmann@volkswagen.de]
    //Gesendet: Mittwoch, 21. Oktober 2009 12:21
    //An: Susanne Goldammer
    //Cc: Huesnue Turkac; Marc Piétriga; Kraemer, Lars (EXTERN: IAVE); ingo.kavemann@iav.de; lars.kraemer@iav.de; Kavemann, Ingo (EXTERN: IAVBI)
    //Betreff: AW: ChangedArray für CallStacks
    //
    //Hallo Susanne,
    //
    //ja, Ihr könnt alle Änderungen der CallStacks immer mit einem Full-Range-Update signalisieren. Die HUs kommen damit klar.
    //
    //Beste Grüße,
    //Heiko
    // as discussed with Heiko Diekelmann ( 21.10.2009 ) we don't need to sent
    // ChangedArray with stack Size for the first update, although it is
    // specified in MSC201 / MSC202 this way.

/*RT13652*/
#if 0
    if (gAppPBBapContext.StatusArrayPending == BT_FALSE)
    {
        (void)APP_COMMON_BAP_ARRAY_sfGetChangedArray( 0, elems , &request);

        if( E_OK != APP_COMMON_UTILS_stSendBapRequest( msgPhoneBookBapRequest, &request))
        {
            DEBUG_VALUE1(APP_PB_BAP_SWI_ERR, MOD_APP_PB_BAP, "MissPbChgdArr fail: start 0, elems %d", elems );
            return FAIL;
        }
    }
    else
    {
        DEBUG_VALUE1(APP_PB_BAP_SWI_TRC, MOD_APP_PB_BAP, "PbChgdArr blocked: start 0, elems %d", elems );
        (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_PHONEBOOK_BAP_FCT_PENDING, MSEC(300), 0);
    }
#endif

    if (APP_PB_BAP_CHANGED_ARRAY_MISSEDCALLS == gAppPBBapContext.enStatusArrayPending)
    {
        DEBUG_TEXT(APP_PB_BAP_SWI_ERR, MOD_APP_PB_BAP, "MissPbChanged send FAIL: Same type of changed array(Missed calls) sending status pending");
        return FAIL;
    }
    else
    {
        (void)APP_COMMON_BAP_ARRAY_sfGetChangedArray( 0, elems , &request);

        if( E_OK != APP_COMMON_UTILS_stSendBapRequest( msgPhoneBookBapRequest, &request))
        {
            DEBUG_VALUE1(APP_PB_BAP_SWI_ERR, MOD_APP_PB_BAP, "MissPbChgdArr fail: start 0, elems %d", elems );
            return FAIL;
        }
    }

    DEBUG_VALUE1(APP_PB_BAP_SWI_TRC, MOD_APP_PB_BAP, "MissPbChgdArr: start 0, elems %d", elems );
    return SUCCESS;

}/* END OF */


/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_BAP_REQUEST_sfSendReceivedCallListChangedArray*/
/**
    \brief      handles notification that received call list changed

    \brief      Description:\n
                handles notification that received call list changed\n

    \return     SuccessFailType
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_PHONEBOOK_BAP_REQUEST_sfSendReceivedCallListChangedArray()
{
    BapRequest request;
    Uint16Type elems    = 0xFFFF;
    request.lsg_id      = BapLsg_Phonebook;
    request.fct_id      = BapFct_Phonebook_ReceivedCalls;

/*RT13652*/
#if 0
    if (gAppPBBapContext.StatusArrayPending == BT_FALSE)
    {
        (void)APP_COMMON_BAP_ARRAY_sfGetChangedArray( 0, elems, &request);
        if( E_OK != APP_COMMON_UTILS_stSendBapRequest( msgPhoneBookBapRequest, &request))
        {
            DEBUG_VALUE1(APP_PB_BAP_SWI_ERR, MOD_APP_PB_BAP, "RecvdPbChgdArr fail: start 0, elems %d", elems );
            return FAIL;
        }
    }
    else
    {
        DEBUG_VALUE1(APP_PB_BAP_SWI_TRC, MOD_APP_PB_BAP, "PbChgdArr blocked: start 0, elems %d", elems );
        (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_PHONEBOOK_BAP_FCT_PENDING, MSEC(300), 0);
    }
#endif

    if (APP_PB_BAP_CHANGED_ARRAY_RECEIVEDCALLS == gAppPBBapContext.enStatusArrayPending)
    {
        DEBUG_TEXT(APP_PB_BAP_SWI_ERR, MOD_APP_PB_BAP, "ReceivedPb send FAIL: Same type of changed array(Received calls) sending status pending");
        return FAIL;
    }
    else
    {
        (void)APP_COMMON_BAP_ARRAY_sfGetChangedArray( 0, elems , &request);

        if( E_OK != APP_COMMON_UTILS_stSendBapRequest( msgPhoneBookBapRequest, &request))
        {
            DEBUG_VALUE1(APP_PB_BAP_SWI_ERR, MOD_APP_PB_BAP, "ReceivedPbChgdArr fail: start 0, elems %d", elems );
            return FAIL;
        }
    }

    DEBUG_VALUE1(APP_PB_BAP_SWI_TRC, MOD_APP_PB_BAP, "RecvdPbChgdArr: start 0, elems %d", elems );
    return SUCCESS;

}/* END OF */

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONEBOOK_BAP_REQUEST_sfSendDialledCallListChangedArray */
/**
    \brief      handles notification that dialed call list changed

    \brief      Description:\n
                handles notification that dialed call list changed\n

    \return     SuccessFailType
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_PHONEBOOK_BAP_REQUEST_sfSendDialledCallListChangedArray()
{
    BapRequest request;
    Uint16Type elems    = 0xFFFF;
    request.lsg_id      = BapLsg_Phonebook;
    request.fct_id      = BapFct_Phonebook_DialedNumbers;

    // as discussed with Heiko Diekelmann ( 21.10.2009 ) we don't need to sent
    // ChangedArray with stack Size for the first update, although it is
    // specified in MSC201 / MSC202 this way.

/*RT13652*/
#if 0
    if (gAppPBBapContext.StatusArrayPending == BT_FALSE)
    {
        (void)APP_COMMON_BAP_ARRAY_sfGetChangedArray( 0, elems, &request);
        if( E_OK != APP_COMMON_UTILS_stSendBapRequest( msgPhoneBookBapRequest, &request))
        {
            DEBUG_VALUE1(APP_PB_BAP_SWI_ERR, MOD_APP_PB_BAP, "DialPbChgdArr fail: start 0, elems %d", elems );
            return FAIL;
        }
    }
    else
    {
        DEBUG_VALUE1(APP_PB_BAP_SWI_TRC, MOD_APP_PB_BAP, "PbChgdArr blocked: start 0, elems %d", elems );
        (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_PHONEBOOK_BAP_FCT_PENDING, MSEC(300), 0);
    }
#endif

    if (APP_PB_BAP_CHANGED_ARRAY_DIALEDCALLS == gAppPBBapContext.enStatusArrayPending)
    {
        DEBUG_TEXT(APP_PB_BAP_SWI_ERR, MOD_APP_PB_BAP, "DialledPb send FAIL: Same type of changed array(Dialled calls) sending status pending");
        return FAIL;
    }
    else
    {
        (void)APP_COMMON_BAP_ARRAY_sfGetChangedArray( 0, elems , &request);

        if( E_OK != APP_COMMON_UTILS_stSendBapRequest( msgPhoneBookBapRequest, &request))
        {
            DEBUG_VALUE1(APP_PB_BAP_SWI_ERR, MOD_APP_PB_BAP, "DialledPbChgdArr fail: start 0, elems %d", elems );
            return FAIL;
        }
    }

    DEBUG_VALUE1(APP_PB_BAP_SWI_TRC, MOD_APP_PB_BAP, "DialledPbChgdArr: start 0, elems %d", elems );
    return SUCCESS;

}/* END OF */




/* End Of File APP_PHONEBOOK_BAP_C1.c */


