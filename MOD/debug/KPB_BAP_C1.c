/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     KPB_C1.c
*-----------------------------------------------------------------------------
* Module Name:    KPB 
*-----------------------------------------------------------------------------
* Description:    Handles and parses KPB specific BAP messages  
*-----------------------------------------------------------------------------
* $Date: 2010-11-02 09:21:36 +0100 (Di, 02 Nov 2010) $
* $Rev: 16010 $
* $Author: weber $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_KPB/trunk/src/KPB_BAP_C1.c $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include "Pool.h"
/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/
#include "bap_types.h"

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "KPB.h"
#include "KPB_CI.h"
#include "KPB_BAP.h"

#include "APP_COMMON_Utils.h"
#include "BAP_Dispatcher_InitSendBuffer.h"

#include "KPB_BAP_C1.id"
#include "Trace.h"
/*----------------------------------------------------------------------------*/
/* pragma Directives                                                          */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Definitions (avoid as much as possible!)                  */
/*----------------------------------------------------------------------------*/
extern KpbContext gKpbContext;
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
_KPB_sfParseBapOperationState(const BapIndication *pBapInd, KpbIndType *pKpbInd);
static SuccessFailType 
_KPB_sfParseBapAsgCapabilities(const BapIndication *pBapInd, KpbIndType *pKpbInd);
static SuccessFailType 
_KPB_sfParseBapAsgConfig(const BapIndication *pBapInd, KpbIndType *pKpbInd);
static SuccessFailType 
_KPB_sfParseBapLsgStatus(const BapIndication *pBapInd, KpbIndType *pKpbInd);
static SuccessFailType 
_KPB_sfParseMflBlockDefinition(const BapIndication *pBapInd, KpbIndType *pKpbInd);
static SuccessFailType 
_KPB_sfParseFrameStatus(const BapIndication *pBapInd, KpbIndType *pKpbInd);
static SuccessFailType 
_KPB_sfParseFrameData(const BapIndication *pBapInd, KpbIndType *pKpbInd);
static SuccessFailType 
_KPB_sfParseFrameDataAck(const BapIndication *pBapInd, KpbIndType *pKpbInd);
static SuccessFailType 
_KPB_sfParseScrollBar(const BapIndication *pBapInd, KpbIndType *pKpbInd);

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
/* Function :   _KPB_sfParseBapIndication                                     */
/** 
  \brief        Main entry point for each KPB specific BAP message  
  \brief        Description:\n
                It filters for supported BAP function and invokes a specific
                parser that reads and checks the respective parameters. Returns
                SUCCESS if the parsing was successful and FAIL otherwise.

  \param        pBapInd
                const BapIndication *

  \param        pKpbInd
                KpbIndType *

  \return       SuccessFailType
                SUCCESS or FAIL

 */
/*----------------------------------------------------------------------------*/
SuccessFailType _KPB_sfParseBapIndication(  const BapIndication *pBapInd, 
                                            KpbIndType *pKpbInd)
{
    SuccessFailType result = SUCCESS;

    switch(pBapInd->fct_id)
    {
        case BapFct_SD_FSG_OperationState:
            result = _KPB_sfParseBapOperationState(pBapInd, pKpbInd);
            break;
        case BapFct_SD_ASG_Capabilities:
            result = _KPB_sfParseBapAsgCapabilities(pBapInd, pKpbInd);
            break;
        case BapFct_SD_ASG_Config:
            result = _KPB_sfParseBapAsgConfig(pBapInd, pKpbInd);
            break;
        case BapFct_SD_LsgStatus:
            result = _KPB_sfParseBapLsgStatus(pBapInd, pKpbInd); 
            break;
        case BapFct_SD_MFL_BlockDefinition:
            result = _KPB_sfParseMflBlockDefinition(pBapInd, pKpbInd);
            break;
        case BapFct_SD_FrameStatus:
            result = _KPB_sfParseFrameStatus(pBapInd, pKpbInd);
            break;
        case BapFct_SD_FrameData:
            result = _KPB_sfParseFrameData(pBapInd, pKpbInd);
            break;
        case BapFct_SD_FrameDataAck:
            result = _KPB_sfParseFrameDataAck(pBapInd, pKpbInd);
            break;
        case BapFct_SD_ScrollBar:
            result = _KPB_sfParseScrollBar(pBapInd, pKpbInd);
            break;

        default:
            // Send response to BAP Dispatcher about unknown Function ID
            APP_COMMON_UTILS_vSendBapError( BapLsg_SD, 
                                            pBapInd->fct_id,
                                            (enum BapError_t) 0xFF);
            result = FAIL;
            break;
    }

    if(result == FAIL)
    {

        DEBUG_VALUE1(KPB_SWI_ERR, MOD_KPB, "ParseBapInd: failed fctId: 0x%02x", pBapInd->fct_id);

         APP_COMMON_UTILS_vSendBapError( BapLsg_SD, 
                                         pBapInd->fct_id,
                                         /* RT 1020: "Appl. - out of range" */
                                         (enum BapError_t) 0x41);
    }

    return result;
}

/*----------------------------------------------------------------------------*/
/* Function :   _KPB_sfParseBapOperationState                                 */
/** 
  \brief        Parses the KPB OperationState  

  \param        pBapInd
                const BapIndication *

  \param        pKpbInd
                KpbIndType *

  \return       SuccessFailType
                SUCCESS or FAIL

 */
/*----------------------------------------------------------------------------*/
static SuccessFailType _KPB_sfParseBapOperationState(  const BapIndication *pBapInd, 
                                                KpbIndType *pKpbInd)
{
    Uint8Type opState = 0xFF;

    if( pBapInd->ind_type != BapInd_DataGet )
    {

        DEBUG_VALUE1(KPB_SWI_ERR, MOD_KPB, "sfParseBapOperationState invalid IndType: 0x%02x", pBapInd->ind_type);

        return FAIL;
    }

    pKpbInd->common.fctType = KPB_FSG_OPERATION_STATE;
    pKpbInd->common.opType  = KPB_OP_TYPE_GET;


    opState = pBapInd->payload.u8_value;

    switch(opState)
    {
        case KPB_OP_STATE_NORMAL_OPERATION:
        case KPB_OP_STATE_OFF:
        case KPB_OP_STATE_KL_15_FOLLOW_UP:
            pKpbInd->opState.state = (KPB_enOpState)opState;
            break;

        default:
        {

            DEBUG_VALUE1(KPB_SWI_ERR, MOD_KPB, "sfParseBapOperationState unknown opstate: 0x%02x", opState);

            return FAIL;
        }
    }

    return SUCCESS;
}


/*----------------------------------------------------------------------------*/
/* Function :   _KPB_sfParseBapAsgCapabilities                                */
/** 
  \brief        Parses the KPB OperationState  

  \param        pBapInd
                const BapIndication *

  \param        pKpbInd
                KpbIndType *

  \return       SuccessFailType
                SUCCESS or FAIL

 */
/*----------------------------------------------------------------------------*/
static SuccessFailType _KPB_sfParseBapAsgCapabilities( const BapIndication *pBapInd, 
                                                KpbIndType *pKpbInd)
{
    pKpbInd->common.fctType = KPB_ASG_CAPABILITIES;


    switch( pBapInd->ind_type)
    {
        case BapInd_DataGet:
            pKpbInd->common.opType  = KPB_OP_TYPE_GET;
            break;
        
        case BapInd_DataSetGet:
            pKpbInd->common.opType  = KPB_OP_TYPE_SET_GET;
            break;

        default:
        {

            DEBUG_VALUE1(KPB_SWI_ERR, MOD_KPB, "sfParseBapAsgCapabilities invalid IndType: 0x%02x", pBapInd->ind_type);

            return FAIL;
        }
    }

    if(pKpbInd->common.opType == KPB_OP_TYPE_GET)
    {
        return SUCCESS;
    }

    if( pBapInd->payload.byte_seq.length != (Uint16Type)BAP_DISP_SEND_BUFFER_enSDAsgCapabilities )
    {

        DEBUG_VALUE1(KPB_SWI_ERR, MOD_KPB, "sfParseBapAsgCapabilities invalid payload length: %d", pBapInd->payload.byte_seq.length);

        return FAIL;
    }

    pKpbInd->asgCap.dispSize     
            = (KPB_enDisplaySizeClass)*(pBapInd->payload.byte_seq.data);
    pKpbInd->asgCap.visibleElements 
            = *(pBapInd->payload.byte_seq.data+1);

    switch(pKpbInd->asgCap.dispSize)
    {
        case KPB_DISP_SIZE_SMALL:
        case KPB_DISP_SIZE_MEDIUM:
            /* the value is  valid */
            break;
        case KPB_DISP_SIZE_LARGE:
            /* large is invalid and shall be mapped to medium class */
            pKpbInd->asgCap.dispSize = KPB_DISP_SIZE_MEDIUM;
            break;
    
        default:
        {

            DEBUG_VALUE1(KPB_SWI_ERR, MOD_KPB, "sfParseBapAsgCapabilities invalid displaySize: %d", pKpbInd->asgCap.dispSize);

            return FAIL;
        }
    }

    return SUCCESS; 
}


/*----------------------------------------------------------------------------*/
/* Function :   _KPB_sfParseBapAsgConfig                                      */
/** 
  \brief        Parses the KPB AsgConfig message  

  \param        pBapInd
                const BapIndication *

  \param        pKpbInd
                KpbIndType *

  \return       SuccessFailType
                SUCCESS or FAIL

 */
/*----------------------------------------------------------------------------*/
static SuccessFailType _KPB_sfParseBapAsgConfig( const BapIndication *pBapInd, 
                                                KpbIndType *pKpbInd)
{
    Uint8Type majorVersion = 0x00;
    Uint8Type minorVersion = 0x00;
   
    pKpbInd->common.fctType = KPB_ASG_CONFIG;


    switch( pBapInd->ind_type)
    {
        case BapInd_DataGet:
            pKpbInd->common.opType  = KPB_OP_TYPE_GET;
            break;
        
        case BapInd_DataSetGet:
            pKpbInd->common.opType  = KPB_OP_TYPE_SET_GET;
            break;

        default:
        {

            DEBUG_VALUE1(KPB_SWI_ERR, MOD_KPB, "sfParseBapAsgConfig invalid IndType: 0x%02x", pBapInd->ind_type);

            return FAIL;
        }
    }

    if(pKpbInd->common.opType != KPB_OP_TYPE_SET_GET)
    {
        return SUCCESS;
    }

    if( pBapInd->payload.byte_seq.length != (Uint16Type)BAP_DISP_SEND_BUFFER_enSDAsgConfig )
    {

        DEBUG_VALUE1(KPB_SWI_ERR, MOD_KPB, "sfParseBapAsgConfig invalid payload length: %d", pBapInd->payload.byte_seq.length);

        return FAIL;
    }

    majorVersion = *(pBapInd->payload.byte_seq.data);
    minorVersion = *(pBapInd->payload.byte_seq.data+1);

    if( (majorVersion == 1) && (minorVersion == 0) )
    {
        pKpbInd->asgConf.version = KPB_VERSION_1_0;
    }
    else if( (majorVersion == 1) && (minorVersion == 1) )
    {
        pKpbInd->asgConf.version = KPB_VERSION_1_1;
    }
    else
    { 

        DEBUG_VALUE2(KPB_SWI_WNG, MOD_KPB, "sfParseBapAsgConfig: unsup version: (%d.%d), fb 1.1", majorVersion, minorVersion);

        pKpbInd->asgConf.version = KPB_VERSION_1_1;
    }

    return SUCCESS;
}



/*----------------------------------------------------------------------------*/
/* Function :   _KPB_sfParseBapLsgStatus                                      */
/** 
  \brief        Parses the KPB LsgStatus message  

  \param        pBapInd
                const BapIndication *

  \param        pKpbInd
                KpbIndType *

  \return       SuccessFailType
                SUCCESS or FAIL

 */
/*----------------------------------------------------------------------------*/
static SuccessFailType _KPB_sfParseBapLsgStatus(const BapIndication *pBapInd,
                                               KpbIndType *pKpbInd)
{
    Uint8Type alive = 0x00;
   
    pKpbInd->common.fctType = KPB_LSG_STATUS;


    switch( pBapInd->ind_type )
    {
        case BapInd_DataSet:
            pKpbInd->common.opType  = KPB_OP_TYPE_SET;
            break;
        
        default:
        {

            DEBUG_VALUE1(KPB_SWI_ERR, MOD_KPB, "sfParseBapLsgStatus invalid IndType: 0x%02x", pBapInd->ind_type);

            return FAIL;
        }
    }

    alive = pBapInd->payload.u8_value;


    if( alive == 0 )
    {
        pKpbInd->lsgStatus.alive = KPB_LSG_ALIVE_FALSE;
    }
    else
    {
        /* other values then 0 are considered as true */
        pKpbInd->lsgStatus.alive = KPB_LSG_ALIVE_TRUE;
    }

    return SUCCESS;
}


/*----------------------------------------------------------------------------*/
/* Function :   _KPB_sfParseMflBlockDefinition                                */
/** 
  \brief        Parses the KPB MflBlockDefinition message  

  \param        pBapInd
                const BapIndication *

  \param        pKpbInd
                KpbIndType *

  \return       SuccessFailType
                SUCCESS or FAIL

 */
/*----------------------------------------------------------------------------*/
static SuccessFailType _KPB_sfParseMflBlockDefinition(const BapIndication *pBapInd,
                                               KpbIndType *pKpbInd)
{
    Uint32Type i = 0;
   
    pKpbInd->common.fctType = KPB_MFL_BLOCK_DEFINITION;

    switch( pBapInd->ind_type )
    {
        case BapInd_DataGet:
            pKpbInd->common.opType  = KPB_OP_TYPE_GET;
            break;
        case BapInd_DataSetGet:
            pKpbInd->common.opType  = KPB_OP_TYPE_SET_GET;
            break;
        
        default:

            DEBUG_VALUE1(KPB_SWI_ERR, MOD_KPB, "sfParseMflBlockDefinition invalid IndType: 0x%02x", pBapInd->ind_type);

            return FAIL;
    }

    if(pKpbInd->common.opType != KPB_OP_TYPE_SET_GET)
    {
        return SUCCESS;
    }

    if( pBapInd->payload.byte_seq.length != (Uint16Type)BAP_DISP_SEND_BUFFER_enSDMflBlockDefinition )
    {

        DEBUG_VALUE1(KPB_SWI_ERR, MOD_KPB, "sfParseMflBlockDefinition invalid payload length: %d", pBapInd->payload.byte_seq.length);

        return FAIL;
    }

    for(i=0; i<8U; i++)
    {
        pKpbInd->mflBlockDef.keyBlock[i] = 
            (KPB_enKeyBlock)*(pBapInd->payload.byte_seq.data + i);

        /* only the first 3 KeyBlocks are using all identifier */
        if( i < 3U)
        {
            switch(pKpbInd->mflBlockDef.keyBlock[i])
            {
                case KPB_KB_NO_KEYS:
                case KPB_KB_ALL_SHARED_MFL_KEYS:
                case KPB_KB_UP_DOWN:
                case KPB_KB_LEFT_RIGHT:
                case KPB_KB_UP_DOWN_LEFT_RIGHT_OK:
                case KPB_KB_ROTARY_1:
                case KPB_KB_ROTARY_2:
                case KPB_KB_OK:
                case KPB_KB_BACK:
                    /* the value is valid */
                    break;
                default:
                {

                    DEBUG_VALUE2(KPB_SWI_ERR, MOD_KPB, "sfParseMflBlockDefinition invalid keyBlock[%d]: %d", i, pKpbInd->mflBlockDef.keyBlock[i]);

                    return FAIL;
                }
            } 
        }
        else
        {
            switch(pKpbInd->mflBlockDef.keyBlock[i])
            {
                case KPB_KB_NO_KEYS:
                case KPB_KB_ALL_SHARED_MFL_KEYS:
                    /* the value is valid */
                    break;
                default:

                    DEBUG_VALUE2(KPB_SWI_ERR, MOD_KPB, "sfParseMflBlockDefinition invalid keyBlock[%d]: %d", i, pKpbInd->mflBlockDef.keyBlock[i]);

                    return FAIL;
            }
        } 
    }

    return SUCCESS;
}


/*----------------------------------------------------------------------------*/
/* Function :   _KPB_sfParseFrameStatus                                       */
/** 
  \brief        Parses the KPB FrameStatus message  

  \param        pBapInd
                const BapIndication *

  \param        pKpbInd
                KpbIndType *

  \return       SuccessFailType
                SUCCESS or FAIL

 */
/*----------------------------------------------------------------------------*/
static SuccessFailType _KPB_sfParseFrameStatus(const BapIndication *pBapInd,
                                               KpbIndType *pKpbInd)
{
    pKpbInd->common.fctType = KPB_FRAME_STATUS;

    switch( pBapInd->ind_type )
    {
        case BapInd_DataGet:
            pKpbInd->common.opType  = KPB_OP_TYPE_GET;
            break;
        case BapInd_DataSetGet:
            pKpbInd->common.opType  = KPB_OP_TYPE_SET_GET;
            break;
        
        default:

            DEBUG_VALUE1(KPB_SWI_ERR, MOD_KPB, "sfParseFrameStatus invalid IndType: 0x%02x", pBapInd->ind_type);

            return FAIL;
    }

    if(pKpbInd->common.opType != KPB_OP_TYPE_SET_GET)
    {
        return SUCCESS;
    }


    if( pBapInd->payload.byte_seq.length != (Uint16Type)BAP_DISP_SEND_BUFFER_enSDFrameStatus )
    {

        DEBUG_VALUE1(KPB_SWI_ERR, MOD_KPB, "sfParseFrameStatus invalid payload length: %d", pBapInd->payload.byte_seq.length);

        return FAIL;
    }

    /* get the frame id value */ 
    pKpbInd->frameStatus.frameId = *(pBapInd->payload.byte_seq.data);

    /* get the relative priority */
    pKpbInd->frameStatus.relPrio = 
            (KPB_enRelativePriority)*(pBapInd->payload.byte_seq.data+1);

    switch(pKpbInd->frameStatus.relPrio)
    {
        case KPB_RELATIVE_PRIO_LOW:
        case KPB_RELATIVE_PRIO_HIGH:
            /* the relative priority is valid */
            break;
        default:
            /* other values are reserved */

            DEBUG_VALUE1(KPB_SWI_ERR, MOD_KPB, "sfParseFrameStatus invalid rel Prio: %d", pKpbInd->frameStatus.relPrio);

            return FAIL; 
    }
         
    /* get the focussedFrameId as first part of record MFL_assigned */
    pKpbInd->frameStatus.mflAssigned.focussedFrameId = *(pBapInd->payload.byte_seq.data+2);
    if(pKpbInd->frameStatus.mflAssigned.focussedFrameId == 0xFE)
    {
        /* the value 0xFE is reserved and shall not be used */

        DEBUG_VALUE1(KPB_SWI_ERR, MOD_KPB, "sfParseFrameStatus invalid focussedFrameId: %d", pKpbInd->frameStatus.mflAssigned.focussedFrameId);

        return FAIL;
    }
    
    /* get MFL_blocks as second part of record MFL_assigned */
    pKpbInd->frameStatus.mflAssigned.mflBlocks.rawByte = 
                           *(pBapInd->payload.byte_seq.data+3);

    /* get ASG_Response value */
    pKpbInd->frameStatus.asgResponse = 
        (KPB_enAsgResponse)*(pBapInd->payload.byte_seq.data+4);

    switch(pKpbInd->frameStatus.asgResponse)
    {
        case KPB_ASG_SET_V_IS_OFF:
        case KPB_ASG_SET_V_IS_ON:
        case KPB_ASG_QUEUED_UP:
        case KPB_ASG_REQ_V_IS_ON:
        case KPB_ASG_NO_RESPONSE:
            /* value is valid */
            break;
        default:
        {
            /* other values are not valid */

            DEBUG_VALUE1(KPB_SWI_ERR, MOD_KPB, "sfParseFrameStatus invalid AsgResponse: %d", pKpbInd->frameStatus.asgResponse);

            return FAIL;
        }
    }
        
    /* get FSG_Request value */
    pKpbInd->frameStatus.fsgRequest = 
        (KPB_enFsgRequest)*(pBapInd->payload.byte_seq.data+5);

    switch(pKpbInd->frameStatus.fsgRequest)
    {
        case KPB_FSG_NO_REQUEST:
            /* only NoRequest is valid for SetGet */
            break;
        default:
            /* other values are not valid */

            DEBUG_VALUE1(KPB_SWI_ERR, MOD_KPB, "sfParseFrameStatus invalid FsgRequest: %d", pKpbInd->frameStatus.fsgRequest);

            return FAIL;
    }

    return SUCCESS;
}


/*----------------------------------------------------------------------------*/
/* Function :   _KPB_sfParseFrameData                                         */
/** 
  \brief        Parses the KPB FrameData message  

  \param        pBapInd
                const BapIndication *

  \param        pKpbInd
                KpbIndType *

  \return       SuccessFailType
                SUCCESS or FAIL

 */
/*----------------------------------------------------------------------------*/
static SuccessFailType _KPB_sfParseFrameData(const BapIndication *pBapInd,
                                            KpbIndType *pKpbInd)
{
    SuccessFailType result = SUCCESS;

    pKpbInd->common.fctType = KPB_FRAME_DATA;

    switch( pBapInd->ind_type )
    {
        case BapInd_DataGet:
            pKpbInd->common.opType = KPB_OP_TYPE_GET;
            break;
        default:
        {

            DEBUG_VALUE1(KPB_SWI_ERR, MOD_KPB, "sfParseFrameData invalid IndType: 0x%02x", pBapInd->ind_type);

            return FAIL;
        }
    }

    /* 4 bytes are needed at least for frameId and ArrayHeader */
    if( pBapInd->payload.byte_seq.length < 4 )
    {

        DEBUG_VALUE1(KPB_SWI_ERR, MOD_KPB, "sfParseFrameData invalid payload length: %d", pBapInd->payload.byte_seq.length);

        return FAIL;
    }

    /* get the frame id value */
    switch( *(pBapInd->payload.byte_seq.data) )
    {
        case KPB_FRAME_ID_1:
        case KPB_FRAME_ID_2:
        case KPB_FRAME_ID_3:
        case KPB_FRAME_ID_4:
        case KPB_FRAME_ID_5:
        case KPB_FRAME_ID_6:
        case KPB_FRAME_ID_7:
            /* good frameId */
            pKpbInd->frameData.frameId = (KPB_enFrameIdType)*(pBapInd->payload.byte_seq.data);
            break;
        case KPB_FRAME_ID_ANY_FRAME: /* not valid for FrameData.GetArray() */
        default:
        {

            DEBUG_VALUE1(KPB_SWI_ERR, MOD_KPB, "sfParseFrameData: inval frameId %d", *(pBapInd->payload.byte_seq.data));

            return FAIL;
        }
    }
    
    result = APP_COMMON_BAP_ARRAY_sfReadHeader(pBapInd->payload.byte_seq.data+1,
                                               pBapInd->payload.byte_seq.length-1,
                                               &(pKpbInd->frameData.arrayHeader));
    if(result != SUCCESS)
    {

        DEBUG_TEXT(KPB_SWI_ERR, MOD_KPB, "sfParseFrameData sfReadHeader failed");

        return FAIL;
    }

    return SUCCESS;
}


/*----------------------------------------------------------------------------*/
/* Function :   _KPB_sfParseFrameDataAck                                      */
/** 
  \brief        Parses the KPB FrameDataAck message  

  \param        pBapInd
                const BapIndication *

  \param        pKpbInd
                KpbIndType *

  \return       SuccessFailType
                SUCCESS or FAIL

 */
/*----------------------------------------------------------------------------*/
static SuccessFailType _KPB_sfParseFrameDataAck(const BapIndication *pBapInd, 
                                               KpbIndType *pKpbInd)
{
    pKpbInd->common.fctType = KPB_FRAME_DATA_ACK;

    switch( pBapInd->ind_type )
    {
        case BapInd_DataSet:
            pKpbInd->common.opType  = KPB_OP_TYPE_SET;
            break;
        
        default:
        {

            DEBUG_VALUE1(KPB_SWI_ERR, MOD_KPB, "sfParseFrameDataAck invalid IndType: 0x%02x", pBapInd->ind_type);

            return FAIL;
        }
    }

    pKpbInd->frameDataAck.frameId = pBapInd->payload.u8_value;

    switch(pKpbInd->frameDataAck.frameId)
    {
        case 0x00:
        case 0xFE:
        case 0xFF:
            /* these values are reserved, so this message have to be ignored */

            DEBUG_VALUE1(KPB_SWI_ERR, MOD_KPB, "sfParseFrameDataAck reserved FrameId: 0x%02x", pKpbInd->frameDataAck.frameId);

            return FAIL;
        default:
            /* the other values are valid */
            break;
    }    

    return SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* Function :   _KPB_sfParseScrollBar                                         */
/** 
  \brief        Parses the KPB ScrollBar message  

  \param        pBapInd
                const BapIndication *

  \param        pKpbInd
                KpbIndType *

  \return       SuccessFailType
                SUCCESS or FAIL

 */
/*----------------------------------------------------------------------------*/
static SuccessFailType _KPB_sfParseScrollBar(const BapIndication *pBapInd, 
                                            KpbIndType *pKpbInd)
{
    pKpbInd->common.fctType = KPB_SCROLL_BAR;

    switch( pBapInd->ind_type )
    {
        case BapInd_DataGet:
            pKpbInd->common.opType  = KPB_OP_TYPE_GET;
            break;
        
        default:
        {

            DEBUG_VALUE1(KPB_SWI_ERR, MOD_KPB, "sfParseFrameDataAck invalid IndType: 0x%02x", pBapInd->ind_type);

            return FAIL;
        }
    }

    return SUCCESS;
}


/*----------------------------------------------------------------------------*/
/* Function :   _KPB_vBapSendOperationState                                   */
/** 
  \brief        Builds and sends the KPB OparationState message  

  \param        opState
                const KPB_enOpState

  \return       void

 */
/*----------------------------------------------------------------------------*/
void _KPB_vBapSendOperationState(const KPB_enOpState opState)
{
    BapRequest bapReq;

    bapReq.lsg_id    = BapLsg_SD;
    bapReq.fct_id    = BapFct_SD_FSG_OperationState;
    bapReq.req_type  = BapReq_Data;
    bapReq.data_type = BAP_DT_INT8;

    bapReq.payload.u8_value = (Uint8Type)opState;

    
    (void)APP_COMMON_UTILS_stSendBapRequest(  msgKpbBapRequest, &bapReq);
}

/*----------------------------------------------------------------------------*/
/* Function :   _KPB_vBapSendAsgCapabilities                                  */
/** 
  \brief        Builds and sends the KPB AsgCapabilities message  

  \param        dispClass
                const KPB_enDisplaySizeClass

  \param        visibleElements
                const Uint8Type

  \return       void

 */
/*----------------------------------------------------------------------------*/
void _KPB_vBapSendAsgCapabilities(const KPB_enDisplaySizeClass dispClass,
                                  const Uint8Type visibleElements)
{
    SuccessFailType result = SUCCESS;
    BapRequest bapReq;

    bapReq.lsg_id    = BapLsg_SD;
    bapReq.fct_id    = BapFct_SD_ASG_Capabilities;
    bapReq.req_type  = BapReq_Data;
    bapReq.data_type = BAP_DT_BYTE;

    bapReq.payload.byte_seq.length = (Uint16Type)BAP_DISP_SEND_BUFFER_enSDAsgCapabilities;

    result = POOL_sfAllocate( POOL_enBapReqDataType, 
                              (void**)&bapReq.payload.byte_seq.data );

    if(result != SUCCESS)
    {

        DEBUG_TEXT(KPB_SWI_ERR, MOD_KPB, "vBapSendAsgCapabilities: POOL_sfAllocate() failed");

        return;
    }

    *(bapReq.payload.byte_seq.data)     = (Uint8Type)dispClass;
    *(bapReq.payload.byte_seq.data+1)   = visibleElements;

    
    (void)APP_COMMON_UTILS_stSendBapRequest(  msgKpbBapRequest, &bapReq);
}

/*----------------------------------------------------------------------------*/
/* Function :   _KPB_vBapSendAsgConfig                                        */
/** 
  \brief        Builds and sends the KPB AsgConfig message  

  \param        versionNumber
                const KPB_enVersionNumber

  \return       void

 */
/*----------------------------------------------------------------------------*/
void _KPB_vBapSendAsgConfig(const KPB_enVersionNumber versionNumber)
{
    SuccessFailType result = SUCCESS;
    BapRequest bapReq;
    Uint8Type majorNumber = 0;
    Uint8Type minorNumber = 0;

    bapReq.lsg_id    = BapLsg_SD;
    bapReq.fct_id    = BapFct_SD_ASG_Config;
    bapReq.req_type  = BapReq_Data;
    bapReq.data_type = BAP_DT_BYTE;


    switch(versionNumber)
    {
    case KPB_VERSION_1_0: 
        majorNumber = 1;
        minorNumber = 0;
        break;
    case KPB_VERSION_1_1:
        majorNumber = 1;
        minorNumber = 1;
        break;
    default:

            DEBUG_VALUE1(KPB_SWI_ERR, MOD_KPB, "vBapSendAsgConfig invalid KPB version: %d", versionNumber);

        return;
    }
    

    bapReq.payload.byte_seq.length = (Uint16Type)BAP_DISP_SEND_BUFFER_enSDAsgConfig;

    result = POOL_sfAllocate( POOL_enBapReqDataType, 
                              (void**)&bapReq.payload.byte_seq.data );

    if(result != SUCCESS)
    {

        DEBUG_TEXT(KPB_SWI_ERR, MOD_KPB, "vBapSendAsgConfig: POOL_sfAllocate() failed");

        return;
    }

    *(bapReq.payload.byte_seq.data)     = majorNumber;
    *(bapReq.payload.byte_seq.data+1)   = minorNumber;

    
    (void)APP_COMMON_UTILS_stSendBapRequest(  msgKpbBapRequest, &bapReq);

}


/*----------------------------------------------------------------------------*/
/* Function :   _KPB_vBapSendLsgStatus                                        */
/** 
  \brief        Builds and sends the KPB LsgStatus message  

  \param        alive
                const BooleanType

  \return       void

 */
/*----------------------------------------------------------------------------*/
void _KPB_vBapSendLsgStatus(const BooleanType alive)
{
    BapRequest bapReq;

    bapReq.lsg_id    = BapLsg_SD;
    bapReq.fct_id    = BapFct_SD_LsgStatus;
    bapReq.req_type  = BapReq_Data;
    bapReq.data_type = BAP_DT_INT8;

    bapReq.payload.u8_value = (Uint8Type)alive;
 
       
    (void)APP_COMMON_UTILS_stSendBapRequest(  msgKpbBapRequest, &bapReq);
}


/*----------------------------------------------------------------------------*/
/* Function :   _KPB_vBapSendMflBlockDefinition                               */
/** 
  \brief        Builds and sends the KPB MflBlockDefinition message  

  \param        keyBlock
                const KPB_KeyBlockType

  \return       void

 */
/*----------------------------------------------------------------------------*/
void _KPB_vBapSendMflBlockDefinition(const KPB_KeyBlockType keyBlock)
{
    SuccessFailType result  = SUCCESS;
    Uint32Type      i       = 0;
    BapRequest      bapReq;
    

    bapReq.lsg_id    = BapLsg_SD;
    bapReq.fct_id    = BapFct_SD_MFL_BlockDefinition;
    bapReq.req_type  = BapReq_Data;
    bapReq.data_type = BAP_DT_BYTE;



    bapReq.payload.byte_seq.length = (Uint16Type)BAP_DISP_SEND_BUFFER_enSDMflBlockDefinition;

    result = POOL_sfAllocate( POOL_enBapReqDataType, 
                              (void**)&bapReq.payload.byte_seq.data );

    if(result != SUCCESS)
    {

            DEBUG_TEXT(KPB_SWI_ERR, MOD_KPB, "vBapSendMflBlockDefinition: POOL_sfAllocate() failed");

        return;
    }

    for(i=0; i<8U; i++)
    {
        *(bapReq.payload.byte_seq.data+i) = (Uint8Type)keyBlock[i];
    }

    (void)APP_COMMON_UTILS_stSendBapRequest(  msgKpbBapRequest, &bapReq);
}


/*----------------------------------------------------------------------------*/
/* Function :   _KPB_vBapSendFrameStatus                                      */
/** 
  \brief        Builds and sends the KPB FrameStatus message  

  \param        pFrameStatus
                const KpbFrameStatusType *

  \return       void

 */
/*----------------------------------------------------------------------------*/
void _KPB_vBapSendFrameStatus(const KpbFrameStatusType *pFrameStatus)
{
    SuccessFailType result  = SUCCESS;
    BapRequest      bapReq;

    bapReq.lsg_id    = BapLsg_SD;
    bapReq.fct_id    = BapFct_SD_FrameStatus;
    bapReq.req_type  = BapReq_Data;
    bapReq.data_type = BAP_DT_BYTE;

    bapReq.payload.byte_seq.length = (Uint16Type)BAP_DISP_SEND_BUFFER_enSDFrameStatus;

    result = POOL_sfAllocate( POOL_enBapReqDataType, 
                              (void**)&bapReq.payload.byte_seq.data );

    if(result != SUCCESS)
    {

        DEBUG_TEXT(KPB_SWI_ERR, MOD_KPB, "vBapSendFrameStatus: POOL_sfAllocate() failed");

        return;
    }

    *(bapReq.payload.byte_seq.data)     = pFrameStatus->frameId;
    *(bapReq.payload.byte_seq.data + 1) = (Uint8Type)pFrameStatus->relPrio;
    *(bapReq.payload.byte_seq.data + 2) = pFrameStatus->mflAssigned.focussedFrameId;
    *(bapReq.payload.byte_seq.data + 3) = pFrameStatus->mflAssigned.mflBlocks.rawByte;
    *(bapReq.payload.byte_seq.data + 4) = (Uint8Type)pFrameStatus->asgResponse;
    *(bapReq.payload.byte_seq.data + 5) = (Uint8Type)pFrameStatus->fsgRequest;

    
    (void)APP_COMMON_UTILS_stSendBapRequest( msgKpbBapRequest, &bapReq);
}


/*----------------------------------------------------------------------------*/
/* Function :   _KPB_vBapSendScrollBar                                        */
/** 
  \brief        Builds and sends the KPB ScrollBar message  

  \param        pScrollBar
                const KpbScrollBarReqType *

  \return       void

 */
/*----------------------------------------------------------------------------*/
void _KPB_vBapSendScrollBar(const KpbScrollBarReqType *pScrollBar)
{
    SuccessFailType result  = SUCCESS;
    BapRequest      bapReq;

    bapReq.lsg_id    = BapLsg_SD;
    bapReq.fct_id    = BapFct_SD_ScrollBar;
    bapReq.req_type  = BapReq_Data;
    bapReq.data_type = BAP_DT_BYTE;

    bapReq.payload.byte_seq.length = (Uint16Type)BAP_DISP_SEND_BUFFER_enSDScrollBar;

    result = POOL_sfAllocate( POOL_enBapReqDataType, 
                              (void**)&bapReq.payload.byte_seq.data );

    if(result != SUCCESS)
    {

        DEBUG_TEXT(KPB_SWI_ERR, MOD_KPB, "vBapSendScrollBar: POOL_sfAllocate() failed");

        return;
    }

    *(bapReq.payload.byte_seq.data)     = (Uint8Type)pScrollBar->frameId;
    *(bapReq.payload.byte_seq.data + 1) = pScrollBar->attribute.rawByte;
    *(bapReq.payload.byte_seq.data + 2) = pScrollBar->sliderLength;
    *(bapReq.payload.byte_seq.data + 3) = pScrollBar->sliderPosition;
    

    (void)APP_COMMON_UTILS_stSendBapRequest(  msgKpbBapRequest, &bapReq);
}

/*----------------------------------------------------------------------------*/
/* Function :   _KPB_sfBapSendFrameDataEmpty                                  */
/** 
  \brief        Builds and sends an empty KPB FrameData message  

  \param        pFrameData
                const KpbFrameDataReqType *

  \return       void

 */
/*----------------------------------------------------------------------------*/
SuccessFailType _KPB_sfBapSendFrameDataEmpty( const KPB_enFrameIdType frameId, 
                                              const BapArrayHeaderType *pGetHeader)
{
    SuccessFailType     result      = SUCCESS;
    Uint8Type           *pWritePos  = NULL;
    Uint8Type           *pArrayPos  = NULL;
    Uint16Type          i           = 0;
    Uint16Type          elements    = 0;
    BapArrayHeaderType  sendHeader;
    BapRequest          bapReq;

    bapReq.lsg_id    = BapLsg_SD;
    bapReq.fct_id    = BapFct_SD_FrameData;
    bapReq.req_type  = BapReq_Data;
    bapReq.data_type = BAP_DT_BYTE;

    if(pGetHeader == NULL)
    {

        DEBUG_TEXT(KPB_SWI_ERR, MOD_KPB, "sfBapSendFrameDataEmpty: pGetHeader == NULL");

        return FAIL;
    }


    if(frameId == KPB_FRAME_ID_INVALID)
    {

        DEBUG_TEXT(KPB_SWI_ERR, MOD_KPB, "sfBapSendFrameDataEmpty: frameId is invalid");

        return FAIL;
    }

    result = POOL_sfAllocate( POOL_enBapReqDataType, 
                              (void**)&bapReq.payload.byte_seq.data );

    if(result != SUCCESS)
    {

        DEBUG_TEXT(KPB_SWI_ERR, MOD_KPB, "sfBapSendFrameData: POOL_sfAllocate() failed");

        return FAIL;
    }


    pWritePos = bapReq.payload.byte_seq.data;

    *pWritePos = (Uint8Type)frameId;
    pWritePos++;

    /* set ListElementOnTop */
    *pWritePos = 0;
    pWritePos++;

    /* set ListOperation */
    *pWritePos = (Uint8Type)KPB_NO_LIST_OPERATION;
    pWritePos++;



    /* ---------------------- */
    /* write the array header */
    (void)memset(&sendHeader, 0, sizeof(sendHeader));

    sendHeader.mode.common.recordAddress = 0;

    /* set the default record address */
    sendHeader.mode.common.recordAddress = KPB_FRAME_DATA_REC_ADDR_NAME |
                                           KPB_FRAME_DATA_REC_ADDR_TYPE |
                                           KPB_FRAME_DATA_REC_ADDR_ATTR;
    
    /* remember the current buffer pos */
    pArrayPos = pWritePos;
    
    /* write the header */
    i = APP_COMMON_BAP_ARRAY_u8WriteHeader(&sendHeader, pArrayPos);

    pWritePos = pWritePos + i;

    /* write the array content */
    for(i=0; i<pGetHeader->elements; i++)
    {
        /* set the cell type */
        *pWritePos = 0;
        pWritePos++;

        /* set the cell attributes */
        *pWritePos = 0;
        pWritePos++;

        /* set the cell name */
        *pWritePos = 0;
        pWritePos++;

        /* break if the limit has been reached */
        if( ++elements == KPB_MAX_CELLS_PER_ARRAY )
        {
            break;
        } 
    } 
    
    /* set the elements to the actual number */
    sendHeader.elements = elements;
    sendHeader.start    = pGetHeader->start;

    /* rewrite the header because the elements might be changed */
    (void)APP_COMMON_BAP_ARRAY_u8WriteHeader(&sendHeader, pArrayPos);

    /* set the length of the whole bap message */
    bapReq.payload.byte_seq.length = 
        (Uint16Type)(pWritePos - bapReq.payload.byte_seq.data);

    /* Send BAP message */
    if (APP_COMMON_UTILS_stSendBapRequest( msgKpbBapRequest, &bapReq) != E_OK )
    {

        DEBUG_TEXT(KPB_SWI_ERR, MOD_KPB, "_KPB_sfBapSendFrameDataEmpty: SendBapRequest() failed");

        return FAIL;
    }

    return SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* Function :   _KPB_sfBapSendFrameData                                       */
/** 
  \brief        Builds and sends the KPB FrameData message  

  \param        pFrameData
                const KpbFrameDataReqType *

  \return       void

 */
/*----------------------------------------------------------------------------*/
SuccessFailType _KPB_sfBapSendFrameData(const KpbFrameDataReqType *pFrameData)
{
    SuccessFailType     result      = SUCCESS;
    Uint8Type           *pWritePos  = NULL;
    Uint8Type           *pArrayPos  = NULL;
    Uint16Type          i           = 0;
    Sint32Type          elements    = 0;
    Sint32Type          cellSets    = 0;
    BapArrayHeaderType  sendHeader;
    BapRequest          bapReq;

    bapReq.lsg_id    = BapLsg_SD;
    bapReq.fct_id    = BapFct_SD_FrameData;
    bapReq.req_type  = BapReq_Data;
    bapReq.data_type = BAP_DT_BYTE;

    if(pFrameData == NULL)
    {

        DEBUG_TEXT(KPB_SWI_ERR, MOD_KPB, "sfBapSendFrameData: provided frameData is NULL!!!");

        return FAIL;
    }

    result = POOL_sfAllocate( POOL_enBapReqDataType, 
                              (void**)&bapReq.payload.byte_seq.data );


    if(result != SUCCESS)
    {

        DEBUG_TEXT(KPB_SWI_ERR, MOD_KPB, "sfBapSendFrameData: POOL_sfAllocate() failed");

        return FAIL;
    }


    pWritePos = bapReq.payload.byte_seq.data;

    *pWritePos = (Uint8Type)pFrameData->frameId;
    pWritePos++;

    *pWritePos = pFrameData->listElemOnTop;
    pWritePos++;

#ifdef UHV_ACTIVATE_FEATURE_ANIMATION
    *pWritePos = (Uint8Type)pFrameData->listOperation;
#else
    *pWritePos = (Uint8Type)KPB_NO_LIST_OPERATION;
#endif
    pWritePos++;

    /* ---------------------- */
    /* write the array header */
    (void)memcpy(&sendHeader, &(pFrameData->arrayHeader), sizeof(sendHeader));

    if(sendHeader.mode.common.recordAddress == 0)
    {
        /* set the default record address */
        sendHeader.mode.common.recordAddress = KPB_FRAME_DATA_REC_ADDR_NAME |
                                               KPB_FRAME_DATA_REC_ADDR_TYPE |
                                               KPB_FRAME_DATA_REC_ADDR_ATTR;
    }
    
    /* we always transmit the list pos */
    sendHeader.mode.common.transmitPos = 1;

    /* remember the current buffer pos */
    pArrayPos = pWritePos;
    
    /* write the header */
    i = APP_COMMON_BAP_ARRAY_u8WriteHeader(&sendHeader, pArrayPos);

    pWritePos = pWritePos + i;

    /* write the array content */
    for(i=0; i<KPB_MAX_FRAME_CELL_NUMBER; i++)
    {

        if(pFrameData->cell[i].isSet != BT_TRUE)
        {
            continue;
        }

        cellSets++;

        //*pWritePos = (Uint8Type)i;
        //pWritePos++;
        if( (pFrameData->partToSend == KPB_CELL_PART_2) &&
            (cellSets < KPB_MAX_CELLS_PER_ARRAY) )
        {
            /* skip the cells of part1 */
            continue;
        }

        if(elements == 0U)
        {
            /* thats the first cell, set it as start cell */
            sendHeader.start = i;
        }
        
        *pWritePos = pFrameData->cell[i].pos;
        pWritePos++;

        if(sendHeader.mode.common.recordAddress & KPB_FRAME_DATA_REC_ADDR_TYPE)
        {
            *pWritePos = (Uint8Type)pFrameData->cell[i].type;
            pWritePos++;
        }

        if(sendHeader.mode.common.recordAddress & KPB_FRAME_DATA_REC_ADDR_ATTR)
        {
            *pWritePos = pFrameData->cell[i].attribute.rawByte;
            pWritePos++;
        }

        if(sendHeader.mode.common.recordAddress & KPB_FRAME_DATA_REC_ADDR_NAME)
        {
            *pWritePos = pFrameData->cell[i].name.length;
            pWritePos++;

            (void)memcpy( pWritePos, 
                          pFrameData->cell[i].name.content,
                          pFrameData->cell[i].name.length);
    
            pWritePos = pWritePos + pFrameData->cell[i].name.length;
        }
         
        /* break if the limit has been reached */
        if( (++elements) == KPB_MAX_CELLS_PER_ARRAY )
        {
            break;
        } 
    } 
    
    /* set the elements to the actual number */
    sendHeader.elements = (Uint16Type)elements;
    
    /* rewrite the header because the elements might be changed */
    (void)APP_COMMON_BAP_ARRAY_u8WriteHeader(&sendHeader, pArrayPos);

    /* set the length of the whole bap message */
    bapReq.payload.byte_seq.length = 
        (Uint16Type)(pWritePos - bapReq.payload.byte_seq.data);

    /* Send BAP message */
    if (APP_COMMON_UTILS_stSendBapRequest( msgKpbBapRequest, &bapReq) != E_OK )
    {

        DEBUG_TEXT(KPB_SWI_ERR, MOD_KPB, "sfBapSendFrameData: SendBapRequest() failed");

        return FAIL;
    }

    return SUCCESS;
}


/* End Of File KPB_C1.c */


