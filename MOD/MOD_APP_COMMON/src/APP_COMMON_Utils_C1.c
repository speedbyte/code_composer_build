/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     APP_COMMON_UTILS_C1.c
*-----------------------------------------------------------------------------
* Module Name:    Common Application
*-----------------------------------------------------------------------------
* Description:    contains some helper functions common for all applications   
*-----------------------------------------------------------------------------
* $Date: 2010-11-02 09:21:36 +0100 (Di, 02 Nov 2010) $
* $Rev: 16010 $
* $Author: weber $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_COMMON/trunk/src/APP_COMMON_Utils_C1.c $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
//#include "osek.h"    
#include "osekcom.h"
#include "stdlib.h"
/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/
//#include "bap_types.h"
/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "APP_COMMON_Utils.h"
#include "Pool.h"

#include "APP_COMMON_Utils_C1.id"
#include "Trace.h"
/*----------------------------------------------------------------------------*/
/* pragma Directives                                                          */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Definitions (avoid as much as possible!)                  */
/*----------------------------------------------------------------------------*/

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
/* Function    : APP_COMMON_UTILS_u16GetBapString                             */
/** 
    \brief      Extracts a BAP string from a byte sequence 
  

    \param      pBufferBegin
                const Uint8Type*

    \param      pStringBegin
                Uint8Type**

    \return     Uint16Type 
*/
/*----------------------------------------------------------------------------*/
Uint16Type APP_COMMON_UTILS_u16GetBapString(const Uint8Type *pBufferBegin, const Uint8Type ** pStringBegin )
{
    const Uint8Type BAP_STRING_LEN_INFO_MASK = 0x80;
    Uint16Type strLength = 0;
    
    *pStringBegin = NULL;

    // check if we have 1 or two bytes length information
    if( *pBufferBegin & BAP_STRING_LEN_INFO_MASK)
    {
        /* two byte len info */
        Uint16Type lenMask = 0x7FFF;

        // we have two bytes and do not need twist them 
        // get the high byte
        strLength = *pBufferBegin;
        // shift the high byte to the left ( to make it the real high byte )
        strLength <<= 8;
        // now get the second byte
        strLength |= *(pBufferBegin + 1);
        
        // remove highest bit ( only len indicator )
        strLength &= lenMask;
        // set start of bap string
        *pStringBegin = pBufferBegin + 2;
    }
    else    /* one byte len info */ 
    {
        strLength  = *pBufferBegin;
        *pStringBegin = pBufferBegin + 1;
    }
    
    return strLength;
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_COMMON_UTILS_u8CalcBapStringLen                          */
/** 
    \brief      Returns the number of bytes used for length information if a
                buffer of u16StrLen would be converted to a bap string 
  
    \param      u16StrLen  
                Uint16Type
    
    \return     Uint8Type
                1 or 2 
*/
/*----------------------------------------------------------------------------*/
Uint8Type APP_COMMON_UTILS_u8CalcBapStringNumLenBytes(Uint16Type u16StrLen )
{
    if( u16StrLen > 127 )
    {
        return 2;         
    }
    return 1;
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_COMMON_UTILS_u16WriteBapString                           */
/** 
    \brief      Writes a BAP string to a byte sequence 

    \param      pDst
                const Uint8Type*
     
    \param      pSrc
                Uint8Type*
    
    \param      u16StrLen
                Uint16Type  

    \return     Uint16Type
                number of written bytes ( including Bap String len bytes )     
*/
/*----------------------------------------------------------------------------*/
Uint16Type APP_COMMON_UTILS_u16WriteBapString(  Uint8Type *pDst, 
                                         const Uint8Type *pSrc,
                                         Uint16Type u16StrLen)
{
    Uint16Type bytesWritten = 0;

    if(u16StrLen > 127)
    {
        *pDst = (u16StrLen>>8) & 0x7F; 
        /* set indicator bit */
        *pDst |= 0x80;
        pDst++;

        *pDst = (u16StrLen & 0xFF);
        pDst++;

        bytesWritten +=2;
    }
    else
    {
        *pDst = u16StrLen;
        pDst++;

        bytesWritten +=1;
    } 

    (void)memcpy(pDst, pSrc, u16StrLen);

    bytesWritten += u16StrLen;

    return bytesWritten;
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_COMMON_UTILS_vGetUint16Type                              */
/** 
    \brief      Extracts an Uint16Type from a byte sequence 
  
    \param      pBufferStart
                const Uint8Type*

    \return     Uint16Type
                The converted Uint16 value 
*/
/*----------------------------------------------------------------------------*/
Uint16Type APP_COMMON_UTILS_u16GetUint16Type( const Uint8Type *pBufferStart) 
{
    Uint16Type val = 0;

    // BAP sends right byte first, then the left byte ( when Uint16 read in
    // BigEndian

    // first copy right byte of received Uint16 and shift it to the left byte
    // of val
    val = *(pBufferStart + 1);
    val <<= 8;
    // copy left byte of received Uint16 to the right byte of val    
    val |= *pBufferStart;
        
    return val;
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_COMMON_UTILS_vSetUint16Type                              */
/** 
    \brief      Writes an Uint16Type to a byte sequence 
  
    \param      value
                Uint16Type

    \param      pBufferStart
                Uint8Type*

    \return     void 
*/
/*----------------------------------------------------------------------------*/
void APP_COMMON_UTILS_vSetUint16Type( Uint16Type value,
                                      Uint8Type *pBufferStart ) 
{
    // make right byte of value to left byte of Uint16 in pBufferStart 
    pBufferStart[0] = (Uint8Type)(value & 0xFF);
    // make left byte of value to right byte of Uint16 in pBufferStart 
    pBufferStart[1] = (Uint8Type)((value >>8) & 0xFF);
}
/*----------------------------------------------------------------------------*/
/* Function    : APP_COMMON_UTILS_vSetUpperNibble                             */
/** 
    \brief      Sets upper nibble of a byte to val 
  
    \param      ptr
                Uint8Type*
                Pointer to buffer

    \param      val 
                Value to set

    \return     void 
*/
/*----------------------------------------------------------------------------*/
void APP_COMMON_UTILS_vSetUpperNibble(Uint8Type *ptr, Uint8Type val)
{
    /* set upper 4 bits to zero */
    *ptr &= 0x0F; 
    /* set upper 4 bits to val */
    *ptr |= val<<4 ;
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_COMMON_UTILS_vSetLowerNibble                             */
/** 
    \brief      Sets lower nibble of a byte to val 
  
    \param      ptr
                Uint8Type*
                Pointer to buffer

    \param      val 
                Value to set

    \return     void 
*/
/*----------------------------------------------------------------------------*/
void APP_COMMON_UTILS_vSetLowerNibble(Uint8Type *ptr, Uint8Type val)
{
    /* set lower 4 bits to zero */
    *ptr &= 0xF0; 
    /* set lower 4 bits to val */
    *ptr |= (val & 0x0F) ;
}


/*----------------------------------------------------------------------------*/
/* Function    : APP_COMMON_UTILS_vSendBapError                               */
/** 
    \brief      Sends BapError msg for given fctId 
  
    \param      bapLsg 
                lsgId_t

    \param      bapFct 
                fctId_t

    \param      errorCode 
                BapError_t

    \return     void 
*/
/*----------------------------------------------------------------------------*/
void APP_COMMON_UTILS_vSendBapError(lsgId_t bapLsg, fctId_t bapFct, enum BapError_t errorCode)
{
    BapRequest request;    
    
    request.lsg_id      = bapLsg; 
    request.req_type    = BapReq_Invalid; /* should not be used by BAP Dispatcher */ 
    request.data_type   = BAP_DT_ERROR;
    request.fct_id      = bapFct;

    request.payload.error_code = errorCode;  

    switch( bapLsg )
    {
        case BapLsg_Telephone:
            (void)APP_COMMON_UTILS_stSendBapRequest(msgPhoneBapRequest, &request);
            break;
        
        case BapLsg_Phonebook:
            (void)APP_COMMON_UTILS_stSendBapRequest(msgPhoneBookBapRequest, &request);
            break;
        
        case BapLsg_MDI:
            (void)APP_COMMON_UTILS_stSendBapRequest(msgMdiBapRequest, &request);
            break;

        case BapLsg_SD:
            (void)APP_COMMON_UTILS_stSendBapRequest(msgKpbBapRequest, &request);
            break;
        
        default:

            DEBUG_VALUE1(APP_COMMON_SWI_ERR, MOD_APP_COMMON, "APP_COMMON_UTILS_vSendBapError: LsgId unknown 0x%02x", bapLsg);

            break;
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_COMMON_UTILS_vSetOsekRelAlarm                            */
/** 
    \brief      Checks if the specified alarm is currently active, in that case
                the alarm is cancel before set, otherwise it is set directly  
  
    \param      inAlarmID
                Osek alarm id

    \param      inIncrement

    \param      inCycle
                
    \return     StatusType 
*/
/*----------------------------------------------------------------------------*/
StatusType APP_COMMON_UTILS_stSetOsekRelAlarm(  AlarmType   inAlarmID, 
                                                TickType    inIncrement, 
                                                TickType    inCycle)
{
    StatusType status = E_OK;
    TickType ticksLeft;

    /* first check if the specified alarm is running */
    if( GetAlarm(inAlarmID, &ticksLeft) == E_OK )
    {
        /* the alarm is running, so cancel it */
        (void)CancelAlarm(inAlarmID);
    }
    
    /* now set the alarm */
    status = SetRelAlarm(inAlarmID, inIncrement, inCycle);

    return status;
}


/*----------------------------------------------------------------------------*/
/* Function    : APP_COMMON_UTILS_bIsAlarmActive                              */
/** 
    \brief      Checks if the specified alarm is currently active 
  
    \param      inAlarmID
                Osek alarm id

                
    \return     BooleanType 
*/
/*----------------------------------------------------------------------------*/
BooleanType APP_COMMON_UTILS_bIsAlarmActive( AlarmType inAlarmID)
{
    TickType ticksLeft;

    /* first check if the specified alarm is running */
    if( GetAlarm(inAlarmID, &ticksLeft) == E_OK )
    {
        return BT_TRUE;
    }
    
    return BT_FALSE;
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_COMMON_UTILS_stCancelOsekRelAlarm                        */
/** 
    \brief      Cancel a given Osek Alarm
  
    \param      inAlarmID
                AlarmType
                
    \return     StatusType 
*/
/*----------------------------------------------------------------------------*/
StatusType APP_COMMON_UTILS_stCancelOsekRelAlarm(  AlarmType   inAlarmID)
{
    StatusType status = E_OK;
    TickType ticksLeft;

    /* first check if the specified alarm is running */
    if( GetAlarm(inAlarmID, &ticksLeft) == E_OK )
    {
        /* the alarm is running, so cancel it */
        status = CancelAlarm(inAlarmID);
    }

    return status;
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_COMMON_UTILS_stGetTimeoutLeft                            */
/** 
    \brief      Returns the time that is left for the specified larm 
  
    \param      inAlarmID
                Osek alarm id

    \param      diffTickTime
                The time that is left in ticks
                
    \return     StatusType
*/
/*----------------------------------------------------------------------------*/
StatusType APP_COMMON_UTILS_stGetTimeoutLeft( AlarmType    inAlarmID, 
                                              TickRefType  diffTickTime)
{
    return GetAlarm(inAlarmID, diffTickTime);
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_COMMON_UTILS_sfFreeBapIndication                         */
/** 
    \brief      Frees the memory for BapIndication of type BAP_DT_BYTE, other
                BapIndication are ignored  
  
    \param      pBapInd
                BapIndication to free

    \return     SuccessFailType
                SUCCESS or FAIL 
*/
/*----------------------------------------------------------------------------*/
SuccessFailType APP_COMMON_UTILS_sfFreeBapIndication(const BapIndication *pBapInd)
{
    SuccessFailType result = SUCCESS;

    if(pBapInd->data_type == BAP_DT_BYTE)
    {
        result = POOL_sfFree(pBapInd->payload.byte_seq.data);

        if(result != SUCCESS)
        {


            DEBUG_VALUE4(APP_COMMON_SWI_ERR, MOD_APP_COMMON, "POOL_sfFree Ind FAILED error code: %d lsgId: %d fctId: %d", POOL_enGetFreeErrorCode(), pBapInd->lsg_id, pBapInd->fct_id, 0);
        }
    }

  
    return result;
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_COMMON_UTILS_sfFreeBapRequest                            */
/** 
    \brief      Frees the memory for BapRequest of type BAP_DT_BYTE, other
                BapRequests are ignored  
  
    \param      pBapReq
                Pointer to the BapRequest to free

    \return     SuccessFailType
                SUCCESS or FAIL 
*/
/*----------------------------------------------------------------------------*/
SuccessFailType APP_COMMON_UTILS_sfFreeBapRequest(const BapRequest *pBapReq)
{
    SuccessFailType result = SUCCESS;

    if(pBapReq->data_type == BAP_DT_BYTE)
    {
        result = POOL_sfFree(pBapReq->payload.byte_seq.data);

        if(result != SUCCESS)
        {

            DEBUG_VALUE4(APP_COMMON_SWI_ERR, MOD_APP_COMMON, "POOL_sfFree Req FAILED error code: %d lsgId: %d fctId: %d", POOL_enGetFreeErrorCode(), pBapReq->lsg_id, pBapReq->fct_id, 0);

        }
    }


    return result;
}
/*----------------------------------------------------------------------------*/
/* Function    : APP_COMMON_UTILS_sfFreeAtResponse                            */
/** 
    \brief      Frees the memory for ATRspMessageType if AT_RSP_WITH_DATA is set 
  
    \param      pAtRsp
                Pointer to the ATRspMessageType to free

    \return     SuccessFailType
                SUCCESS or FAIL 
*/
/*----------------------------------------------------------------------------*/
SuccessFailType APP_COMMON_UTILS_sfFreeAtResponse(const ATRspMessageType *pAtRsp)
{
    SuccessFailType result = SUCCESS;

    if( (pAtRsp->dataInd == AT_RSP_WITH_DATA) && (pAtRsp->data != NULL) )
    {
        result = POOL_sfFree(pAtRsp->data);
        
        if(result != SUCCESS)
        {

            DEBUG_VALUE4(APP_COMMON_SWI_ERR, MOD_APP_COMMON, "POOL_sfFree ATRsp FAILED: error code: %d lsgId: %d fctId: %d", POOL_enGetFreeErrorCode(), pAtRsp->common.rsp_type, pAtRsp->common.receiver, 0);

        }
    }

    return result;
}

/*----------------------------------------------------------------------------*/
/* Function  :  APP_COMMON_UTILS_stSendBapRequest                             */
/** 
    \brief      Wrapper function to send a BAP message                   
  
    \brief      Description:\n
                Wrapper function to send a specified BAP message, if sending fails
                the dynamic memory is immediately freed

    \param      pMsg
                The Symbolic name of the message queue

    \param      pBapReq
                Pointer to the BapRequest to send
                
    \return     StatusType
                E_OK or !E_OK 
*/
/*----------------------------------------------------------------------------*/
StatusType APP_COMMON_UTILS_stSendBapRequest(  SymbolicName pMsg, 
                                               BapRequest *pBapReq)
{
    StatusType result = E_OK;
    SymbolicName pMsgName = NULL;

    switch(pBapReq->lsg_id)
    {
        case BapLsg_Telephone:
            pMsgName = msgPhoneBapRequest;
            break;
        case BapLsg_Phonebook:
            pMsgName = msgPhoneBookBapRequest;
            break;
        case BapLsg_MDI:
            pMsgName = msgMdiBapRequest;
            break;
        case BapLsg_SD:
            pMsgName = msgKpbBapRequest;
            break;
        default:
            pMsgName = NULL;
            break;
    }

    if(pMsgName != pMsg)
    {

        DEBUG_TEXT(APP_COMMON_SWI_DBG, MOD_APP_COMMON, "stSendBapRequest: SymbolicNames doesn't match");
    }

    if(pMsgName)
    {
        result = SendMessage(pMsg, pBapReq);
    }
    else
    {
        result = !E_OK;
    }


    if( result != E_OK )
    {
        /* in error case, check if we have allocated memory */
        if(pBapReq->data_type == BAP_DT_BYTE)
        {
            (void)POOL_sfFree(pBapReq->payload.byte_seq.data);
        }
    }


    return result;
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_COMMON_UTILS_sfSendBapResultRequestNoData                */
/** 
    \brief      send BAP Result without data for given lsg , fct id
  
    \param      lsgId
                lsgId_t

    \param      callFctId
                fctId_t    
                
    \return     SuccessFailType
                SUCCESS or FAIL 
*/
/*----------------------------------------------------------------------------*/
SuccessFailType APP_COMMON_UTILS_sfSendBapResultRequestNoData( lsgId_t lsgId,
                                                               fctId_t callFctId)
{
    SuccessFailType result = SUCCESS;    
    BapRequest request;

    /* set all common values */
    request.lsg_id      = lsgId; 
    request.req_type    = BapReq_Result;
    request.data_type   = BAP_DT_VOID;
    request.fct_id      = callFctId; 

    switch( lsgId )
    {
        case BapLsg_Telephone:
            (void)APP_COMMON_UTILS_stSendBapRequest(msgPhoneBapRequest, &request);
            break;
        case BapLsg_Phonebook:
            (void)APP_COMMON_UTILS_stSendBapRequest(msgPhoneBookBapRequest, &request);
            break;
        case BapLsg_MDI: 
            (void)APP_COMMON_UTILS_stSendBapRequest(msgMdiBapRequest, &request);
            break;
        default:
            result = FAIL;
            break;
    }
    
    return result;
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_COMMON_UTILS_sfSendEmptyBapArray                         */
/** 
    \brief      Send empty BAP Array
  
    \param      pBapInd
                
    \return     SuccessFailType
                SUCCESS or FAIL 
*/
/*----------------------------------------------------------------------------*/
SuccessFailType APP_COMMON_UTILS_sfSendEmptyBapArray(const BapIndication *pBapInd)
{
    SuccessFailType result = SUCCESS;
    BapArrayHeaderType sendHeader;
    BapRequest bapReq;
    Uint32Type headerLength;

    if( pBapInd->ind_type != BapInd_DataGet )
    {
            APP_COMMON_UTILS_vSendBapError( pBapInd->lsg_id, 
                                            pBapInd->fct_id, 
                                            AppErr_NotSuccessful);
            return FAIL;
    }

    /* build the callInfo request and send it to BAP-Dispatcher */
    bapReq.lsg_id       = pBapInd->lsg_id;
    bapReq.fct_id       = pBapInd->fct_id;
    bapReq.req_type     = BapReq_Data;  // StatusArray optype wanted 
    bapReq.data_type    = BAP_DT_BYTE;

    /* get the array header of the initiating bap message */
    result = APP_COMMON_BAP_ARRAY_sfReadHeader( pBapInd->payload.byte_seq.data, 
                                                pBapInd->payload.byte_seq.length,
                                                &sendHeader);

    /* set the elements value of the header */ 
    sendHeader.elements     = 0;

    result = POOL_sfAllocate( POOL_enBapReqDataType,
                              (void**)&bapReq.payload.byte_seq.data);
    
    if(result != SUCCESS)
    {

        DEBUG_TEXT(APP_COMMON_SWI_ERR, MOD_APP_COMMON, "SendBapResultRequestNoData: POOL_sfAllocate() failed");

        return FAIL;
    }
    
    /* preset the header to get the length (3 or 5) */    
    headerLength = APP_COMMON_BAP_ARRAY_u8WriteHeader( &sendHeader,
                                                       bapReq.payload.byte_seq.data );
    bapReq.payload.byte_seq.length = (Uint16Type)headerLength; 

    switch( pBapInd->lsg_id )
    {
        case BapLsg_Telephone:
            (void)APP_COMMON_UTILS_stSendBapRequest(msgPhoneBapRequest, &bapReq);
            break;
        case BapLsg_Phonebook:
            (void)APP_COMMON_UTILS_stSendBapRequest(msgPhoneBookBapRequest, &bapReq);
            break;
        case BapLsg_MDI: 
            (void)APP_COMMON_UTILS_stSendBapRequest(msgMdiBapRequest, &bapReq);
            break;
        default:

            DEBUG_VALUE1(APP_COMMON_SWI_ERR, MOD_APP_COMMON, "SendBapResReqNoData: not sup lsgId %d", pBapInd->lsg_id);

            result = FAIL;
            break;
    }
    
    return result;
}


/*----------------------------------------------------------------------------*/
/* Function    : APP_COMMON_UTILS_sfConvertAtSmsDate                          */
/** 
    \brief      Converts the sms date received from CK5050 to a easier accessible
                format
  

    \param      atSmsDate
                Sms data as received from CK5050

    \param      length 
                Size in bytes of the atSmsDate

    \param      pDate 
                Pointer to the converted date as output
                
    \return     SuccessFailType
                SUCCESS or FAIL 
*/
/*----------------------------------------------------------------------------*/
SuccessFailType APP_COMMON_UTILS_sfConvertAtSmsDate(    const ATSmsDate atSmsDate,
                                                        Uint32Type length,
                                                        AppCommonDateType *pDate)
{
    Uint8Type pSrc[APP_COMMON_DATE_POSITION_END+1];

    if(length != (Uint32Type)APP_COMMON_DATE_POSITION_END)
    {
        return FAIL;
    }

    (void)memset(pDate, 0, sizeof(*pDate));
    (void)memcpy(pSrc, atSmsDate, APP_COMMON_DATE_POSITION_END); 

    /* terminate the last part of the date */
    pSrc[APP_COMMON_DATE_POSITION_END] = '\0';

    /* read the year */
    pDate->year     = (Uint8Type)atoi((char*)&pSrc[APP_COMMON_DATE_POSITION_YEAR]); 
    /* read the month */
    pDate->month    = (Uint8Type)atoi((char*)&pSrc[APP_COMMON_DATE_POSITION_MONTH]); 
    /* read the day */
    pDate->day      = (Uint8Type)atoi((char*)&pSrc[APP_COMMON_DATE_POSITION_DAY]); 
    /* read the hour */
    pDate->hour     = (Uint8Type)atoi((char*)&pSrc[APP_COMMON_DATE_POSITION_HOUR]); 
    /* read the minute */
    pDate->minute   = (Uint8Type)atoi((char*)&pSrc[APP_COMMON_DATE_POSITION_MINUTE]); 
    /* read the seconds */
    pDate->seconds  = (Uint8Type)atoi((char*)&pSrc[APP_COMMON_DATE_POSITION_SECONDS]); 

    return SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* Function :   APP_COMMON_enGetUtf8Type                                      */
/** 
  \brief        Returns the UTF-8 type of a provided byte  

  \param        byte
                UTF-8 byte

  \return       UTF8_ByteType
                UTF-8 type depending on the detected type
                UTF8_INVALID_BYTE if no UTF-8 type matched
 */
/*----------------------------------------------------------------------------*/
UTF8_ByteType APP_COMMON_enGetUtf8Type(Uint8Type byte)
{
    UTF8_ByteType utf8Type = UTF8_INVALID_BYTE;

    if( (byte & UTF8_MASK_1_LEFT_MOST_BITS)      == (Uint8Type)UTF8_START_BYTE_SIZE_1 )
    {
        utf8Type = UTF8_START_BYTE_SIZE_1;
    }
    else if( (byte & UTF8_MASK_2_LEFT_MOST_BITS) == (Uint8Type)UTF8_FOLLOW_BYTE )
    {
        utf8Type = UTF8_FOLLOW_BYTE;
    }
    else if( (byte & UTF8_MASK_3_LEFT_MOST_BITS) == (Uint8Type)UTF8_START_BYTE_SIZE_2 )
    {
        utf8Type = UTF8_START_BYTE_SIZE_2;
    }
    else if( (byte & UTF8_MASK_4_LEFT_MOST_BITS) == (Uint8Type)UTF8_START_BYTE_SIZE_3 )
    {
        utf8Type = UTF8_START_BYTE_SIZE_3;
    }
    else if( (byte & UTF8_MASK_5_LEFT_MOST_BITS) == (Uint8Type)UTF8_START_BYTE_SIZE_4 )
    {
        utf8Type = UTF8_START_BYTE_SIZE_4;
    }
        

    DEBUG_VALUE1(APP_COMMON_SWI_DBG, MOD_APP_COMMON, "GetUtf8Type: utf8Type 0x%02x", utf8Type);

    return utf8Type;
}


/*----------------------------------------------------------------------------*/
/* Function :   APP_COMMON_u32GetSDBM_Hash                                      */
/** 
  \brief        Returns a hash for the given string  
                Hash Code Source: http://www.fantasy-coders.de/projects/gh/html/x435.html
                7.2.4. SDBM Algorithmus
                
                Dieser Algorithmus wird in Sleepycat's Datenbank BDB (Berkeley DataBase) verwendet.
                Original-Code:
                unsigned long
                sdbm(unsigned char *str)
                {
                  unsigned long hash = 0;
                  int c;
                
                  while (c = *str++)
                    hash = c + (hash << 6) + (hash << 16) - hash;
                
                  return hash;
                }

  \param        const Uint8Type * str
                byte buffer to calculate the hash for

  \param        Uint16Type len
                length of buffer

  \return       Uint32Type hash
                hash for given buffer
 */
/*----------------------------------------------------------------------------*/
Uint32Type APP_COMMON_u32GetSDBM_Hash(const Uint8Type * str, Uint16Type len)
{
    Uint32Type hash = 0;
    Uint8Type c;
    Uint16Type idx = 0;
    
    // for each character
    for(idx=0; idx<len;idx++)
    {
        c =  str[idx];
        hash = c + (hash << 6) + (hash << 16) - hash;
    }

    return hash;
}
            


/* End Of File APP_COMMON_UTILS_C1.c */


