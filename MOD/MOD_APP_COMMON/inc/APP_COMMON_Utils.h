/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:   APP_COMMON_UTILS.h
*-----------------------------------------------------------------------------
* Module Name:  APP_COMMON    
*-----------------------------------------------------------------------------
* Description:  global helper function common for all applications 
*-----------------------------------------------------------------------------
* $Date: 2010-03-01 11:05:29 +0100 (Mo, 01 Mrz 2010) $
* $Rev: 11280 $
* $Author: goldammer $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_COMMON/trunk/inc/APP_COMMON_Utils.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _APP_COMMON_UTILS_H
#define _APP_COMMON_UTILS_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "osek.h"
//#include "BAP_Dispatcher.h"
#include "bap_types.h"
#include "APP_COMMON_BAP_ARRAY.h"
/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/

#define INVALID_CALL_FCT_ID 0x00

#define UTF8_MASK_1_LEFT_MOST_BITS     0x80
#define UTF8_MASK_2_LEFT_MOST_BITS     0xC0
#define UTF8_MASK_3_LEFT_MOST_BITS     0xE0
#define UTF8_MASK_4_LEFT_MOST_BITS     0xF0
#define UTF8_MASK_5_LEFT_MOST_BITS     0xF8

//#define UTF8_VAL_SIZE_1     0x00
//#define UTF8_VAL_SIZE_2     0xC0
//#define UTF8_VAL_SIZE_3     0xE0
//#define UTF8_VAL_SIZE_4     0xF0
//#define UTF8_VAL_FOLLOW     0x80


#define NUM_ELEMENTS_OF_ARRAY(array) (sizeof(array)/sizeof(*array))

/*----------------------------------------------------------------------------*/
/* Public Types                                                               */
/*----------------------------------------------------------------------------*/


typedef enum
{
    UTF8_START_BYTE_SIZE_1 = 0x00,
    UTF8_START_BYTE_SIZE_2 = 0xC0,
    UTF8_START_BYTE_SIZE_3 = 0xE0,
    UTF8_START_BYTE_SIZE_4 = 0xF0, 
    UTF8_FOLLOW_BYTE       = 0x80,
    UTF8_INVALID_BYTE      = 0xFF
}UTF8_ByteType;

typedef enum
{
    APP_COMMON_RSP_OK,
    APP_COMMON_RSP_ERROR,
    APP_COMMON_RSP_ERROR_BAP_PARSE,
    APP_COMMON_RSP_ERROR_BAP_NOT_IMPLEMENTED,
    APP_COMMON_RSP_ERROR_UART_BLOCKED,
    APP_COMMON_RSP_ERROR_BUSY,
    APP_COMMON_RSP_ERROR_AT_SEND,
    APP_COMMON_RSP_ERROR_AT_BUFFER_TOO_SHORT,
    APP_COMMON_RSP_ERROR_NOT_INITIALIZED,
    APP_COMMON_RSP_ERROR_PROFILE_EXISTING,
    APP_COMMON_RSP_ERROR_PROFILE_NOT_EXISTING,
    APP_COMMON_RSP_ERROR_INVALID_DEVICE_ID,
    APP_COMMON_RSP_ERROR_SMS_ALREADY_OF_TYPE_READ,
    APP_COMMON_RSP_ERROR_SMS_INVALID_INDEX,
    APP_COMMON_RSP_ERROR_NO_ACTIVE_CALL 
}APP_COMMON_enResponseCode;

typedef enum
{
    APP_COMMON_DATE_POSITION_YEAR = 0,
    APP_COMMON_DATE_POSITION_MONTH = 3,
    APP_COMMON_DATE_POSITION_DAY = 6,
    APP_COMMON_DATE_POSITION_HOUR = 9,
    APP_COMMON_DATE_POSITION_MINUTE = 12,
    APP_COMMON_DATE_POSITION_SECONDS = 15,
    APP_COMMON_DATE_POSITION_END = 17
} APP_COMMON_en_DatePositionType;

typedef struct
{
    fctId_t fctId;
    BapIndication * bapInd;
}AppCommon;

typedef struct
{
    AppCommon           common; 
    BapArrayHeaderType  arrayHeader;
}AppCommonArray;

typedef struct
{
    Uint8Type   year;
    Uint8Type   month;
    Uint8Type   day;
    Uint8Type   hour;
    Uint8Type   minute;
    Uint8Type   seconds;
}AppCommonDateType;

/*----------------------------------------------------------------------------*/
/* Global Variables Declarations (avoid as much as possible!)                 */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/
#define ARRAY_COUNT(x) (sizeof(x)/sizeof(x[0]))
/*----------------------------------------------------------------------------*/
/* Public Methods                                                             */
/*----------------------------------------------------------------------------*/


Uint16Type APP_COMMON_UTILS_u16GetBapString( const Uint8Type *pBufferBegin, 
                                             const Uint8Type ** pStringBegin );
Uint16Type APP_COMMON_UTILS_u16WriteBapString( Uint8Type *pDst, 
                                               const Uint8Type *pSrc,
                                               Uint16Type u16StrLen );
Uint16Type APP_COMMON_UTILS_u16GetUint16Type( const Uint8Type *pBufferStart ); 
void       APP_COMMON_UTILS_vSetUpperNibble( Uint8Type *ptr, Uint8Type val );
void       APP_COMMON_UTILS_vSetLowerNibble( Uint8Type *ptr, Uint8Type val );
void       APP_COMMON_UTILS_vSetUint16Type( Uint16Type value,
                                            Uint8Type *pBufferStart );

void       APP_COMMON_UTILS_vSendBapError( lsgId_t bapLsg, 
                                           fctId_t bapFct, 
                                           enum BapError_t errorCode );
SuccessFailType APP_COMMON_UTILS_sfFreeBapIndication( const BapIndication *pBapInd );
SuccessFailType APP_COMMON_UTILS_sfFreeBapRequest( const BapRequest *pBapReq );
SuccessFailType APP_COMMON_UTILS_sfFreeAtResponse( const ATRspMessageType *pAtRsp );
SuccessFailType APP_COMMON_UTILS_sfSendBapResultRequestNoData( lsgId_t lsgId,
                                                               fctId_t callFctId );
SuccessFailType APP_COMMON_UTILS_sfSendEmptyBapArray( const BapIndication *pBapInd );
SuccessFailType APP_COMMON_UTILS_sfConvertAtSmsDate( const Uint8Type *pSrc,
                                                     Uint32Type length,
                                                     AppCommonDateType *pDate);
Uint8Type  APP_COMMON_UTILS_u8CalcBapStringNumLenBytes(Uint16Type u16StrLen );
StatusType APP_COMMON_UTILS_stGetTimeoutLeft( AlarmType   inAlarmID, 
                                              TickRefType diffTickTime);
StatusType APP_COMMON_UTILS_stCancelOsekRelAlarm( AlarmType   inAlarmID);
BooleanType APP_COMMON_UTILS_bIsAlarmActive( AlarmType inAlarmID);
StatusType APP_COMMON_UTILS_stSendBapRequest( SymbolicName pMsg, 
                                              BapRequest *pBapReq);
StatusType APP_COMMON_UTILS_stSetOsekRelAlarm( AlarmType   inAlarmID, 
                                               TickType    inIncrement, 
                                               TickType    inCycle);
UTF8_ByteType APP_COMMON_enGetUtf8Type(Uint8Type byte);

Uint32Type APP_COMMON_u32GetSDBM_Hash(const Uint8Type * str, Uint16Type len);




#endif /* _APP_COMMON_UTILS_H */

/* End Of File APP_COMMON_UTILS.h */

