/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:   APP_COMMON_BAP_ARRAY.h
*-----------------------------------------------------------------------------
* Module Name:  APP_COMMON    
*-----------------------------------------------------------------------------
* Description:  global helper function common for all applications 
*-----------------------------------------------------------------------------
* $Date: 2009-06-09 12:15:57 +0200 (Di, 09 Jun 2009) $
* $Rev: 6597 $
* $Author: brust $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_COMMON/trunk/inc/APP_COMMON_BAP_ARRAY.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _APP_COMMON_BAP_ARRAY_H
#define _APP_COMMON_BAP_ARRAY_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include "Common.h"
/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
//#include "osek.h"
#include "BAP_Dispatcher.h"
//#include "POOL.h"
//#include "bap_types.h"
/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/

//#define BAP_ARRAY_HEADER_SIZE 5


/*----------------------------------------------------------------------------*/
/* Public Types                                                               */
/*----------------------------------------------------------------------------*/

typedef enum
{
    APP_COMMON_BAP_ARRAY_enSpecifiedByStart,
    APP_COMMON_BAP_ARRAY_enLastElementStart
} APP_COMMON_BAP_ARRAY_enStartValue;

typedef enum
{
    APP_COMMON_BAP_ARRAY_enForward,
    APP_COMMON_BAP_ARRAY_enBackward
} APP_COMMON_BAP_ARRAY_enDirection;

//typedef Uint8Type RawArrayHeader[BAP_ARRAY_HEADER_SIZE];

typedef union
{
    struct
    {
        Uint8Type indexSize:1;   
        Uint8Type reserved:1;
        Uint8Type direction:1;
        Uint8Type shift:1;
        Uint8Type recordAddress:4;
    } get;
    struct
    {
        Uint8Type indexSize:1;   
        Uint8Type transmitPos:1;
        Uint8Type direction:1;
        Uint8Type shift:1;
        Uint8Type recordAddress:4;
    } status;
    struct
    {
        Uint8Type indexSize:1;   
        Uint8Type reserved:1;
        Uint8Type direction:1;
        Uint8Type shift:1;
        Uint8Type recordAddress:4;
    } changed;
    struct
    {
        Uint8Type indexSize:1;   
        Uint8Type transmitPos:1;
        Uint8Type direction:1;
        Uint8Type reserved:1;
        Uint8Type recordAddress:4;
    } setget;
    struct
    {
        Uint8Type indexSize:1;   
        Uint8Type transmitPos:1;
        Uint8Type direction:1;
        Uint8Type shift:1;
        Uint8Type recordAddress:4;
    } common;
    Uint8Type data;  
}ArrayHeaderMode;

typedef struct
{
    ArrayHeaderMode mode;
    Uint16Type      start;
    Uint16Type      elements;
}BapArrayHeaderType;


typedef struct
{
    BooleanType     forward;
    Uint16Type      start;
    Uint16Type      elements;
}ConvArrayHeaderType;




/*----------------------------------------------------------------------------*/
/* Global Variables Declarations (avoid as much as possible!)                 */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Methods                                                             */
/*----------------------------------------------------------------------------*/

SuccessFailType APP_COMMON_BAP_ARRAY_sfGetDefaultStatusArray (
                                                    const BapArrayHeaderType *pRecvHeader, 
                                                    BapRequest *pBapRequest              );        
SuccessFailType APP_COMMON_BAP_ARRAY_sfReadHeader(  const Uint8Type *pExtHeader, 
                                                    Uint32Type bufSize,
                                                    BapArrayHeaderType *pArrayHeader );
SuccessFailType APP_COMMON_BAP_ARRAY_sfGetChangedArray (
                                                    Uint16Type start,
                                                    Uint16Type elements,
                                                    BapRequest *pBapRequest );
SuccessFailType APP_COMMON_BAP_ARRAY_sfIsHeaderSizeValid( const Uint8Type *pBuffer, 
                                                          Uint32Type bufSize );
SuccessFailType APP_COMMON_BAP_ARRAY_sfConvSWArrayHeader( const BapArrayHeaderType *pArrayHeader,
                                                          const Uint16Type lastListElement,
                                                          ConvArrayHeaderType *convHeader );
Uint8Type       APP_COMMON_BAP_ARRAY_u8WriteHeader( const BapArrayHeaderType *pArrayHeader,
                                                    Uint8Type *pExtHeader );
Uint8Type       APP_COMMON_BAP_ARRAY_sfSetPos( Uint8Type * pBuf, 
                                               Uint16Type pos, 
                                               Uint8Type indexSize); 
Uint16Type      APP_COMMON_BAP_ARRAY_sGetPos( Uint8Type * pBuf, 
                                              Uint8Type indexSize); 
Uint16Type      APP_COMMON_BAP_ARRAY_u16GetHeaderLength( const BapArrayHeaderType *pArrayHeader );
Uint16Type      APP_COMMON_BAP_ARRAY_sfGetPos( const Uint8Type * pBuf, Uint8Type indexSize); 



#endif /* _APP_COMMON_BAP_ARRAYS_H */

/* End Of File APP_PHONE_UTILS.h */

