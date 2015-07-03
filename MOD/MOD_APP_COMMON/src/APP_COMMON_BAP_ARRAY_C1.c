/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     APP_COMMON_BapArray_C1.c
*-----------------------------------------------------------------------------
* Module Name:    Common Application
*-----------------------------------------------------------------------------
* Description:    contains some helper functions common for all applications   
*-----------------------------------------------------------------------------
* $Date: 2010-11-02 09:21:36 +0100 (Di, 02 Nov 2010) $
* $Rev: 16010 $
* $Author: weber $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_COMMON/trunk/src/APP_COMMON_BAP_ARRAY_C1.c $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
//#include "osek.h"
#include "Pool.h"    
/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/
//#include "bap_types.h"
/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "APP_COMMON_BAP_ARRAY.h"

#include "APP_COMMON_BAP_ARRAY_C1.id"
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

/*----------------------------------------------------------------------------*/
/* Prototypes of file local Methods                                           */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* File local Code Macros                                                     */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* File local Variables                                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* File local const Definitions                                               */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private Address Calculations                                               */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Function    : APP_COMMON_BAP_ARRAY_sfGetDefaultStatusArray                */
/** 
    \brief      Support function to build a default status array response
  
    \param      pRecvHeader
                The request bap array header
                
    \param      pBapRequest
                A pointer to the response bap message

    \return     SuccessFailType
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType APP_COMMON_BAP_ARRAY_sfGetDefaultStatusArray (
                           const BapArrayHeaderType *pRecvHeader, 
                           BapRequest *pBapRequest              )        
{

    SuccessFailType result  = SUCCESS;
    Uint8Type headerLength;
   
    BapArrayHeaderType sendHeader;

    sendHeader.mode.data    = pRecvHeader->mode.data; 
    sendHeader.start        = pRecvHeader->start;
    sendHeader.elements     = 0;
    
    pBapRequest->req_type   = BapReq_Data;  // StatusArray optype wanted 
    pBapRequest->data_type  = BAP_DT_BYTE;

    result = POOL_sfAllocate( POOL_enBapArrayHeader,
                              (void**)&pBapRequest->payload.byte_seq.data);
    
    if(result != SUCCESS)
    {

        DEBUG_TEXT(APP_COMMON_SWI_ERR, MOD_APP_COMMON, "POOL_sfAllocate( POOL_enBapArrayHeader) failed");
        return FAIL;
    }
       
    headerLength = APP_COMMON_BAP_ARRAY_u8WriteHeader(  &sendHeader,
                                                        pBapRequest->payload.byte_seq.data); 
    
    /* set the size of the contained data  */
    pBapRequest->payload.byte_seq.length = headerLength;

    return SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_COMMON_BAP_ARRAY_sfGetChangedArray                       */
/** 
    \brief      Support function to build a default changed array response
  
    \param      start
                Start index of the changed array

    \param      elements
                Number of elements that have changed
                
    \param      pBapRequest
                A pointer to the response bap message

    \return     SuccessFailType
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType APP_COMMON_BAP_ARRAY_sfGetChangedArray (
                           Uint16Type start,
                           Uint16Type elements,
                           BapRequest *pBapRequest )        
{
    SuccessFailType result  = SUCCESS;
    Uint8Type headerLength;
   
    BapArrayHeaderType sendHeader;

    /* init the mode byte with 0 */
    sendHeader.mode.data    = 0; 
    if((start > 127) || (elements > 127))
    {
        sendHeader.mode.common.indexSize = 1;    
    }
    /* set the start value       */
    sendHeader.start        = start;
    /* set the elements value    */
    sendHeader.elements     = elements;
    
    pBapRequest->req_type   = BapReq_Changed;  /* ChangedArray optype wanted */
    pBapRequest->data_type  = BAP_DT_BYTE;

    result = POOL_sfAllocate( POOL_enBapArrayHeader,
                              (void**)&pBapRequest->payload.byte_seq.data);
    
    if(result != SUCCESS)
    {

        DEBUG_TEXT(APP_COMMON_SWI_ERR, MOD_APP_COMMON, "POOL_sfAllocate( POOL_enBapArrayHeader) failed");
        return FAIL;
    }
       
    headerLength = APP_COMMON_BAP_ARRAY_u8WriteHeader(  &sendHeader,
                                                        pBapRequest->payload.byte_seq.data); 
    
    /* set the size of the contained data  */
    pBapRequest->payload.byte_seq.length = headerLength;

    return SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_COMMON_BAP_ARRAY_u16GetHeaderLength                      */
/** 
    \brief      Utility function to get the header size
  
    \param      pArrayHeader
                The request bap array header

    \return     Uint16Type
                Header size 3 or 5 bytes
*/
/*----------------------------------------------------------------------------*/
Uint16Type APP_COMMON_BAP_ARRAY_u16GetHeaderLength(const BapArrayHeaderType *pArrayHeader)
{
    if(pArrayHeader->mode.common.indexSize)
    {
        return 5;
    }
    else
    {
        return 3;
    } 
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_COMMON_BAP_ARRAY_sfReadHeader                           */
/** 
    \brief      Utility function to parse the incoming header 
  
    \param      pExtHeader
                The incoming BAP array header to parse in byte buffer
                
    \param      bufSize
                The size of the byte buffer

    \param      pArrayHeader
                The parsed header as result

    \return     SuccessFailType
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType APP_COMMON_BAP_ARRAY_sfReadHeader( const Uint8Type *pExtHeader,
                                                   Uint32Type bufSize,
                                                   BapArrayHeaderType *pArrayHeader)
{
    SuccessFailType result = SUCCESS;

    result = APP_COMMON_BAP_ARRAY_sfIsHeaderSizeValid(  pExtHeader, bufSize ); 

    if(result != SUCCESS)
    {
        return FAIL;
    }

    /* get the first byte as mode byte */
    pArrayHeader->mode.data = pExtHeader[0];

    /* check if we have 8 or 16 BIT for start and elements */
    if(pArrayHeader->mode.common.indexSize == 0)
    {
        /* we got 8 BIT values */
        pArrayHeader->start     = pExtHeader[1];
        pArrayHeader->elements  = pExtHeader[2];
    }
    else
    {      
        /* we got 16 BIT values */
        pArrayHeader->start     = APP_COMMON_UTILS_u16GetUint16Type( &pExtHeader[1] ); 
        pArrayHeader->elements  = APP_COMMON_UTILS_u16GetUint16Type( &pExtHeader[3] ); 
    }
   
    return SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_COMMON_BAP_ARRAY_u8WriteHeader                           */
/** 
    \brief      Utility function to convert the array header back to buffer format 
  
    \param      pArrayHeader
                A parsed BAP array header
                
    \param      pRawHeader
                The BAP array header in byte buffer format as output

    \return     SuccessFailType
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
Uint8Type APP_COMMON_BAP_ARRAY_u8WriteHeader( const BapArrayHeaderType *pArrayHeader,
                                                    Uint8Type *pRawHeader )
{
    Uint8Type bytesWritten = 0;
    
    pRawHeader[0] = pArrayHeader->mode.data;
   
    if(pArrayHeader->mode.common.indexSize == 0)
    {
        pRawHeader[1] = pArrayHeader->start;
        pRawHeader[2] = pArrayHeader->elements;
    
        bytesWritten = 3;
    }
    else
    {
        
        APP_COMMON_UTILS_vSetUint16Type( pArrayHeader->start,
                                         &pRawHeader[1]); 

        APP_COMMON_UTILS_vSetUint16Type( pArrayHeader->elements,
                                         &pRawHeader[3]); 
        bytesWritten = 5;
    }

    return bytesWritten;
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_COMMON_BAP_ARRAY_sfIsHeaderSizeValid                     */
/** 
    \brief      Utility function to validate the correct header size 
  
    \param      pBuffer
                The BAP array header in byte buffer format

    \param      bufSize
                The size of the byte buffer

    \return     SuccessFailType
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType APP_COMMON_BAP_ARRAY_sfIsHeaderSizeValid( const Uint8Type *pBuffer, 
                                                          Uint32Type bufSize)
{
    ArrayHeaderMode mode;

    /* check for the min size */
    if(bufSize < 3UL )
    {
        return FAIL;
    }

    /* get the mode byte */
    mode.data = pBuffer[0];

    if( (mode.common.indexSize == 1U) && (bufSize < 5) )
    {
        return FAIL;
    }

    return SUCCESS; 
}



/*----------------------------------------------------------------------------*/
/* Function    : APP_COMMON_BAP_ARRAY_sfConvSWArrayHeader                     */
/** 
    \brief      This utility function interprets the setting of the received
                BAP array header (reverse bit, shift bit, ...) and creates a 
                generic description of the requested array range.
  
    \param      pArrayHeader
                Pointer of the BAP array header to convert
                
    \param      lastListElement
                The index of the last element to the corresponding array list

    \param      pConvHeader
                The converted header result as output

    \return     SuccessFailType
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType APP_COMMON_BAP_ARRAY_sfConvSWArrayHeader( const BapArrayHeaderType *pArrayHeader,
                                                          const Uint16Type lastListElement,
                                                          ConvArrayHeaderType *pConvHeader ) 
{
    /* check if we have the special reverse case  */
    if( (pArrayHeader->mode.common.shift == 1) && 
        (pArrayHeader->mode.common.direction == 1) && 
        (pArrayHeader->start == 0) )
    {
        /* we have to start with the last element */
        pConvHeader->start       = lastListElement;
        pConvHeader->elements    = pArrayHeader->elements;
        pConvHeader->forward     = BT_FALSE;


        DEBUG_VALUE4(APP_COMMON_SWI_DBG, MOD_APP_COMMON, "Converted ArrayHeader: mode: %x, start: %x, elements:%x ", pArrayHeader->mode, pArrayHeader->start, pArrayHeader->elements, 0);
        DEBUG_VALUE4(APP_COMMON_SWI_DBG, MOD_APP_COMMON, "to ConvHeader: forward: %d, start: %x, elements:%x", pConvHeader->forward, pConvHeader->start, pConvHeader->elements, 0);

        return SUCCESS;
    }
    else
    {
        pConvHeader->start    = pArrayHeader->start;
        pConvHeader->elements = pArrayHeader->elements;

        // check for backwards
        if(pArrayHeader->mode.common.direction == 1)
        {
            pConvHeader->forward    = BT_FALSE;

            if(pArrayHeader->mode.common.shift == 1)
            {
                // if shift and direction is set we need to start with the
                // predecessor of the given start element 
                pConvHeader->start = pConvHeader->start - 1;
            }
        }
        else
        {
            pConvHeader->forward    = BT_TRUE;

            // if shift is set in forward direction we need to start with the
            // successor of the given start element 
            if(pArrayHeader->mode.common.shift == 1)
            {
                pConvHeader->start = pConvHeader->start + 1;
            }
        }
        
    }

    
    /* now check the boundaries for start */ 
    //if(pConvHeader->start == 0)
    //{
    //    pConvHeader->start = 1;
    //}
    /* we always need to increase the start by one, since the headunit request 
     * is zero-based but parrot expects a value range that is one-based */
    pConvHeader->start = pConvHeader->start + 1;


    if(pConvHeader->start > lastListElement)
    {
        pConvHeader->start = lastListElement;
            
        if( pConvHeader->forward == BT_TRUE)
        {
            pConvHeader->elements   = 0;
    

            DEBUG_VALUE4(APP_COMMON_SWI_DBG, MOD_APP_COMMON, "Converted ArrayHeader: mode: %x, start: %x, elements:%x ", pArrayHeader->mode, pArrayHeader->start, pArrayHeader->elements, 0);
            DEBUG_VALUE4(APP_COMMON_SWI_DBG, MOD_APP_COMMON, "to ConvHeader: forward: %d, start: %x, elements:%x", pConvHeader->forward, pConvHeader->start, pConvHeader->elements, 0);


            return SUCCESS;
        }
    }

    /* now check the boundaries for elements */ 
    if(pConvHeader->forward)
    {
        
        if( (pConvHeader->start + pConvHeader->elements) > lastListElement)
        {     
           pConvHeader->elements = lastListElement - pConvHeader->start + 1;
        }
    }
    else
    {
        if( pConvHeader->start < pConvHeader->elements )
        {
            // backwards, start is smaller than number of requested elements
            if(pArrayHeader->mode.common.shift == 1)
            {
                // we shall send answer starting with predecessor element of
                // start
                pConvHeader->elements = pConvHeader->start - 1;
            }
            else
            {
                // we shall send answer starting with start itself 
                pConvHeader->elements = pConvHeader->start;
            }
        }
    }

    /* now map the backward direction to ascending order */
    if(pConvHeader->forward == BT_FALSE)
    {
        pConvHeader->start = pConvHeader->start - pConvHeader->elements + 1; 
    }


        DEBUG_VALUE4(APP_COMMON_SWI_DBG, MOD_APP_COMMON, "ConvSWArrayHeader: mode: %x, start: %x, elements:%x ", pArrayHeader->mode, pArrayHeader->start, pArrayHeader->elements, 0);
        DEBUG_VALUE4(APP_COMMON_SWI_DBG, MOD_APP_COMMON, "to ConvHeader: forward: %d, start: %x, elements:%x", pConvHeader->forward, pConvHeader->start, pConvHeader->elements, 0);

    return SUCCESS;
}


/*----------------------------------------------------------------------------*/
/* Function    : APP_COMMON_BAP_ARRAY_sfSetPos                                */
/** 
    \brief      sets BAP array position into buf, depending on indexSize 
    
    \param      pBuf
                Uint8Type *
                Buffer to write pos into         
                
    \param      pos
                Uint16Type
                bap array position

    \param      indexSize
                Uint8Type
                indexSize ( mode byte of bap array header ) 

    \return     Uint8Type 
                number of bytes used by position ( 1 or 2 )
*/
/*----------------------------------------------------------------------------*/
Uint8Type 
APP_COMMON_BAP_ARRAY_sfSetPos( Uint8Type * pBuf, Uint16Type pos, Uint8Type indexSize) 
{
    Uint8Type len = 0;

    /* make the pos zero-based as expected by the HU */
    pos = pos - 1;

    if(0 == indexSize )
    {
        if( pos > 0xFF )
        {

            DEBUG_VALUE1(APP_COMMON_SWI_DBG, MOD_APP_COMMON, "sfSetPOs: IndexSize is not set although pos (%x) is > 0xFF", pos);
        }
        *pBuf  = pos;
        len = 1;
    }
    else
    {
        APP_COMMON_UTILS_vSetUint16Type(pos, pBuf); 
        len = 2;
    }
    return len;
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_COMMON_BAP_ARRAY_sfGetPos                                */
/** 
    \brief      get BAP array position from buf, depending on indexSize 
    
    \param      pBuf
                Uint8Type *
                Buffer to write pos into         
                
    \param      indexSize
                Uint8Type
                indexSize ( mode byte of bap array header ) 

    \return     Uint16Type 
                pos
*/
/*----------------------------------------------------------------------------*/
Uint16Type 
APP_COMMON_BAP_ARRAY_sfGetPos( const Uint8Type * pBuf, Uint8Type indexSize) 
{
    Uint16Type pos = 0;

    if(0 == indexSize )
    {
        pos = *pBuf;
    }
    else
    {
        pos = APP_COMMON_UTILS_u16GetUint16Type(pBuf);
    }
    return pos;
}


/* End Of File APP_COMMON_BAP_ARRAY_C1.c */


