/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     BAP_Dispatcher.c
*-----------------------------------------------------------------------------
* Module Name:    BAP Dispatcher
*-----------------------------------------------------------------------------
* Description:    Implementation of BAP indications, that would be called by
*                 BAPTask() to post BAP indications coming from Head Unit to
*                 BAP Dispatcher and posted to different Applications which
*                 handle the doing for the diffrent LSgID anf Fctid.
*                 Request to the Bap stack also handle here.
*-----------------------------------------------------------------------------
* $Date: 2011-10-18 10:30:40 +0200 (Di, 18 Okt 2011) $
* $Rev: 22438 $
* $Author: maennel $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_BAP_Dispatcher/trunk/src/BAP_Dispatcher_C1.c $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include "APP_COMMON_Utils.h"
#include "APP_COMMON_Pool.h"
#include "POOL.h"
#include "CAN_CB.h"
#include "CAN_CB.h"
#include "APP_Phone_BAP.h"
#include "APP_Phonebook_BAP.h"

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/
#include "osek.h"
#include "bap.h"
#include "bap_user_stdtypes.h"
#include "bap_defines.h"

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "BAP_Dispatcher_CI.h"
#include "BAP_Dispatcher.h"
#include "BAP_Dispatcher_Ack.h"
#include "BAP_Dispatcher_InitSendBuffer.h"

#include "BAP_Dispatcher_C1.id"
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
static SuccessFailType BAP_DISP_IndicationManager(BapIndication *bap_ind);
static void BAP_DISP_ProcessBapRequestMessage(const BapRequest *pRequest);

/*----------------------------------------------------------------------------*/
/* File local Code Macros                                                        */
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
/* Function    : BAP_DISP_HandleEvent                                         */
/**
    \brief      This function is called from the OSEK HostTask if an event
                occured.

    \param      eventMask
                EventMaskType

    \return     void
   */
/*----------------------------------------------------------------------------*/
void BAP_DISP_HandleEvent(EventMaskType eventMask)
{
    
    if(eventMask & evHostTaskPhoneBapReq)
    {
        BapRequest request;
        (void)ClearEvent(evHostTaskPhoneBapReq);
        /* Read and process the bap request message */
        if( ReceiveMessage(msgPhoneBapRequest, &request) == E_OK )
        {

            DEBUG_VALUE4(BAP_DISPATCHER_SWI_TRC, MOD_BAP_DISPATCHER, "From APP: Lsg=0x%02X Fct=0x%02X, Type=%02X", request.lsg_id, request.fct_id, request.req_type, 0);

            BAP_DISP_ProcessBapRequestMessage(&request);

            (void)APP_COMMON_UTILS_sfFreeBapRequest(&request);
        }
    }

    if(eventMask & evHostTaskPhoneBookBapReq)
    {
        BapRequest request;
        (void)ClearEvent(evHostTaskPhoneBookBapReq);
        /* Read and process the bap request message */
        if( ReceiveMessage(msgPhoneBookBapRequest, &request) == E_OK )
        {

            DEBUG_VALUE4(BAP_DISPATCHER_SWI_TRC, MOD_BAP_DISPATCHER, "From APP: Lsg=0x%02X Fct=0x%02X, Type=%02X", request.lsg_id, request.fct_id, request.req_type, 0);

            BAP_DISP_ProcessBapRequestMessage(&request);
            
            (void)APP_COMMON_UTILS_sfFreeBapRequest(&request);
        }
    }
    
    if(eventMask & evHostTaskMdiBapReq)
    {
        BapRequest request;
        (void)ClearEvent(evHostTaskMdiBapReq);
        /* Read and process the bap request message */
        if( ReceiveMessage(msgMdiBapRequest, &request) == E_OK )
        {

            DEBUG_VALUE4(BAP_DISPATCHER_SWI_TRC, MOD_BAP_DISPATCHER, "From APP: Lsg=0x%02X Fct=0x%02X, Type=%02X", request.lsg_id, request.fct_id, request.req_type, 0);

#ifdef UHV_ACTIVATE_FEATURE_BAP_MDI
            BAP_DISP_ProcessBapRequestMessage(&request);
#endif
            
            (void)APP_COMMON_UTILS_sfFreeBapRequest(&request);
        }
    }
    
    if(eventMask & evHostTaskKpbBapReq)
    {
        BapRequest request;
        (void)ClearEvent(evHostTaskKpbBapReq);
        /* Read and process the bap request message */
        if( ReceiveMessage(msgKpbBapRequest, &request) == E_OK )
        {

            DEBUG_VALUE4(BAP_DISPATCHER_SWI_TRC, MOD_BAP_DISPATCHER, "From APP: Lsg=0x%02X Fct=0x%02X, Type=%02X", request.lsg_id, request.fct_id, request.req_type, 0);

            BAP_DISP_ProcessBapRequestMessage(&request);

            (void)APP_COMMON_UTILS_sfFreeBapRequest(&request);
        }
    }
}


/*----------------------------------------------------------------------------*/
/* Function    : BAP_DISP_Initialization                                      */
/**
    \brief      This module is responsible for the initialization of the BAP
                stack and buffer for the property class related data values.
                It ultimately brings the BAP communication in running state.
                In case of any error condition while initializing the BAP
                stack or buffer or starting the BAP communication,the function
                exits, thereby not allowing BAP communication.
                the function will also set-up the required queues for BAP ack
                handling and the related control blocksThis function populates the bap_indication structure and

    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType BAP_DISP_Initialization()
{
    
    /*-----------------------------------------------------------------------*/
    /* Initialize the BAP stack for LSG Telephone */
    if (BAP_Init(BapLsg_Telephone) != BapErr_OK)
    {

        DEBUG_TEXT(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "BAP_Init(BapLsg_Telephone) failed");

        return(FAIL);
    }
    /* Initialize the BAP stack for LSG Phonebook */
    if (BAP_Init(BapLsg_Phonebook) != BapErr_OK)
    {

        DEBUG_TEXT(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "BAP_Init(BapLsg_Phonebook) failed");
        return(FAIL);
    }
#ifdef UHV_ACTIVATE_FEATURE_BAP_MDI
    /* Initialize the BAP stack for LSG MDI */
    if (BAP_Init(BapLsg_MDI) != BapErr_OK)
    {

        DEBUG_TEXT(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "BAP_Init(BapLsg_MDI) failed");

        return(FAIL);
    }
#endif

    /* Initialize the BAP stack for LSG SD */
    if (BAP_Init(BapLsg_SD) != BapErr_OK)
    {

        DEBUG_TEXT(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "BAP_Init(BapLsg_SD) failed");

        return(FAIL);
    }
    /*-----------------------------------------------------------------------*/

    /*-----------------------------------------------------------------------*/
    /* Initialize the BAP send buffers for LSG Telephone */
    if (BAP_DISP_InitSendbuffer(BapLsg_Telephone) != SUCCESS)
    {

        DEBUG_TEXT(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "InitSendbuffer(BapLsg_Telephone) failed");

        return(FAIL);
    }
    /* Initialize the BAP send buffers for LSG Phonebook */
    if (BAP_DISP_InitSendbuffer(BapLsg_Phonebook) != SUCCESS)
    {

        DEBUG_TEXT(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "InitSendbuffer(BapLsg_Phonebook) failed");

        return(FAIL);
    }
#ifdef UHV_ACTIVATE_FEATURE_BAP_MDI
    /* Initialize the BAP send buffers for LSG MDI */
    if (BAP_DISP_InitSendbuffer(BapLsg_MDI) != SUCCESS)
    {

        DEBUG_TEXT(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "InitSendbuffer(BapLsg_MDI) failed");

        return(FAIL);
    }
#endif
    /* Initialize the BAP send buffers for LSG SD */
    if (BAP_DISP_InitSendbuffer(BapLsg_SD) != SUCCESS)
    {

        DEBUG_TEXT(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "InitSendbuffer(BapLsg_SD) failed");

        return(FAIL);
    }
    /*-----------------------------------------------------------------------*/

    return BAP_DISP_Start();
}

/*----------------------------------------------------------------------------*/
/* Function    : BAP_DISP_Start                                               */
/**
    \brief      This function encapsulate the start of the BAP-Stack for all
                used LsgIds
    
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType BAP_DISP_Start(void)
{
    /* Start the BAP Stack for LSG Telephone */
    if (BAP_Start(BapLsg_Telephone) != BapErr_OK)
    {

        DEBUG_TEXT(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "BAP_Start(BapLsg_Telephone) failed");

		return(FAIL);
	}

    /* Start the BAP Stack for LSG Phonebook */
    if (BAP_Start(BapLsg_Phonebook) != BapErr_OK)
    {

        DEBUG_TEXT(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "BAP_Start(BapLsg_Phonebook) failed");

		return(FAIL);
	}

#ifdef UHV_ACTIVATE_FEATURE_BAP_MDI
    /* Start the BAP Stack for LSG MDI */
    if (BAP_Start(BapLsg_MDI) != BapErr_OK)
    {

        DEBUG_TEXT(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "BAP_Start(BapLsg_MDI) failed");

		return(FAIL);
	}
#endif
    
    /* Start the BAP Stack for LSG SD */
    if (BAP_Start(BapLsg_SD) != BapErr_OK)
    {

        DEBUG_TEXT(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "BAP_Start(BapLsg_SD) failed");

		return(FAIL);
	}


    DEBUG_VALUE1(BAP_DISPATCHER_SWI_TRC, MOD_BAP_DISPATCHER, "SYS_PERF: BAP Startup Event: %d ms (BAP is ready)", osGetSystemCounter());

    return SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* Function    : BAP_SD_Start                                               */
/**
    \brief      This function encapsulate the start of the KPB-Stack

    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType BAP_SD_Start(void)
{
    BapError_et BapError;

    BapError = BAP_Start(BapLsg_SD);

    /* Start the BAP Stack for LSG SD */
    if (BapError != BapErr_OK)
    {
        DEBUG_VALUE1(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "BAP_SD_Start(BapLsg_SD) failed, error: %h", BapError);

        return(FAIL);
    }

    return SUCCESS;
}


/*----------------------------------------------------------------------------*/
/* Function    : BAP_DISP_ShutDown                                               */
/**
    \brief      This function encapsulate the shut down of the BAP-Stack for all
                used LsgIds

    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType BAP_DISP_ShutDown(void)
{
    /* Stop the BAP Stack for LSG Telephone */
    if (BAP_Shutdown(BapLsg_Telephone) != BapErr_OK)
    {

        DEBUG_TEXT(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "BAP_Shutdown(BapLsg_Telephone) failed");

        return(FAIL);
    }

    /* Stop the BAP Stack for LSG Phonebook */
    if (BAP_Shutdown(BapLsg_Phonebook) != BapErr_OK)
    {

        DEBUG_TEXT(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "BAP_Shutdown(BapLsg_Phonebook) failed");

        return(FAIL);
    }

#ifdef UHV_ACTIVATE_FEATURE_BAP_MDI
    /* Stop the BAP Stack for LSG MDI */
    if (BAP_Shutdown(BapLsg_MDI) != BapErr_OK)
    {

        DEBUG_TEXT(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "BAP_Shutdown(BapLsg_MDI) failed");

        return(FAIL);
    }
#endif

    /* Stop the BAP Stack for LSG SD */
    if (BAP_Shutdown(BapLsg_SD) != BapErr_OK)
    {

        DEBUG_TEXT(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "BAP_Shutdown(BapLsg_SD) failed");

        return(FAIL);
    }


    DEBUG_VALUE1(BAP_DISPATCHER_SWI_TRC, MOD_BAP_DISPATCHER, "SYS_PERF: BAP Startup Event: %d ms (BAP is ready)", osGetSystemCounter());

    return SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* Function    : BAP_SD_ShutDown                                               */
/**
    \brief      This function encapsulate the shut down of the KPB-Stack

    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType BAP_SD_ShutDown(void)
{
    BapError_et BapError;

    BapError = BAP_Shutdown(BapLsg_SD);

    /* Stop the BAP Stack for LSG SD */
    if (BapError != BapErr_OK)
    {
        DEBUG_VALUE1(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "BAP_SD_Shutdown(BapLsg_SD) failed, error %h", BapError);

        return(FAIL);
    }

    return SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* Function    : BAP_IndicationVoid                                      */
/**
    \brief      This function populates the bap_indication structure and
                assigns it to IndicationManager

    \param      aLsgId
                lsgId_t
    \param      aFctId
                fctId_t
    \param      aeIndication
                enum BapIndication_t
    \return     void
   */
/*----------------------------------------------------------------------------*/
void BAP_IndicationVoid(lsgId_t aLsgId, fctId_t aFctId, enum BapIndication_t aeIndication)
{
	BapIndication bap_ind;

	bap_ind.lsg_id 		= aLsgId;	
	bap_ind.fct_id 		= aFctId;	
	bap_ind.ind_type	= aeIndication;
	bap_ind.data_type	= BAP_DT_VOID;

    if( BAP_DISP_IndicationManager(&bap_ind) != SUCCESS)
    {

        DEBUG_VALUE2(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "IndVoid: IndManager failed (0x%02x, 0x%02x)", aLsgId, aFctId);
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : BAP_IndicationInt8                                      */
/**
    \brief      This function populates the bap_indication structure and
                assigns it to IndicationManager

    \param      aLsgId
                lsgId_t
    \param      aFctId
                fctId_t
    \param      aeIndication
                enum BapIndication_t
    \param      au8Value
                uint8_t
    \return     void
   */
/*----------------------------------------------------------------------------*/
void BAP_IndicationInt8(lsgId_t aLsgId, fctId_t aFctId, enum BapIndication_t aeIndication, uint8_t au8Value)
{
	BapIndication bap_ind;
         
	bap_ind.lsg_id 				= aLsgId;	
	bap_ind.fct_id 				= aFctId;	
	bap_ind.ind_type	        = aeIndication;
	bap_ind.data_type			= BAP_DT_INT8;
	bap_ind.payload.u8_value	= au8Value;

    if( BAP_DISP_IndicationManager(&bap_ind) != SUCCESS)
    {

        DEBUG_VALUE2(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "IndInt8: IndManager failed (0x%02x, 0x%02x)", aLsgId, aFctId);
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : BAP_IndicationInt16                                          */
/**
    \brief      This function populates the bap_indication structure and
                assigns it to IndicationManager

    \param      aLsgId
                lsgId_t
    \param      aFctId
                fctId_t
    \param      aeIndication
                enum BapIndication_t
    \param      au16Value
                uint16_t
    \return     void
   */
/*----------------------------------------------------------------------------*/
void BAP_IndicationInt16(lsgId_t aLsgId, fctId_t aFctId, enum BapIndication_t aeIndication, uint16_t au16Value)
{
	BapIndication bap_ind;
         
	bap_ind.lsg_id 		= aLsgId;	
	bap_ind.fct_id 		= aFctId;	
	bap_ind.ind_type	= aeIndication;
	bap_ind.data_type	= BAP_DT_INT16;
	bap_ind.payload.u16_value	= au16Value;

    if( BAP_DISP_IndicationManager(&bap_ind) != SUCCESS)
    {

        DEBUG_VALUE2(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "IndInt16: IndManager failed (0x%02x, 0x%02x)", aLsgId, aFctId);
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : BAP_IndicationInt32                                          */
/**
    \brief      This function populates the bap_indication structure and
                assigns it to IndicationManager

    \param      aLsgId
                lsgId_t
    \param      aFctId
                fctId_t
    \param      aeIndication
                enum BapIndication_t
    \param      au32Value
                uint32_t
    \return     void
   */
/*----------------------------------------------------------------------------*/
void BAP_IndicationInt32(lsgId_t aLsgId, fctId_t aFctId, enum BapIndication_t aeIndication, uint32_t au32Value)
{
	BapIndication bap_ind;
         
	bap_ind.lsg_id 		= aLsgId;	
	bap_ind.fct_id 		= aFctId;	
	bap_ind.ind_type	= aeIndication;
	bap_ind.data_type	= BAP_DT_INT32;
	bap_ind.payload.u32_value	= au32Value;

    if( BAP_DISP_IndicationManager(&bap_ind) != SUCCESS)
    {

        DEBUG_VALUE2(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "IndInt32: IndManager failed (0x%02x, 0x%02x)", aLsgId, aFctId);
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : BAP_IndicationByteSequence                                   */
/**
    \brief      This function populates the bap_indication structure and
                assigns it to IndicationManager

    \param      aLsgId
                lsgId_t
    \param      aFctId
                fctId_t
    \param      aeIndication
                enum BapIndication_t
    \param      apValue
                const volatile_ptr_t
    \param      au16Length
                uint16_t
    \return     void
   */
/*----------------------------------------------------------------------------*/
void BAP_IndicationByteSequence(lsgId_t aLsgId, fctId_t aFctId,
                                    enum BapIndication_t aeIndication,
                                    const volatile_ptr_t apValue,
                                    uint16_t au16Length)
{
    SuccessFailType  sF;
    POOL_enPoolNames poolName;
	BapIndication    bap_ind;
         
	bap_ind.lsg_id 		= aLsgId;	
	bap_ind.fct_id 		= aFctId;	
	bap_ind.ind_type	= aeIndication;
	bap_ind.data_type	= BAP_DT_BYTE;

    
    switch (aLsgId)
    {
        case BapLsg_Telephone:
            poolName = POOL_enBapIndDataTypeAppPhone;
            if(au16Length>MAX_BAP_INDICATION_SIZE_APP_PHONE)
            {
                au16Length = MAX_BAP_INDICATION_SIZE_APP_PHONE;
                

                DEBUG_VALUE1(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "IndByteSeq: indSize shortened for LsgId 0x%02x!", aLsgId);
            }
            break;

        case BapLsg_Phonebook:
    
            if(au16Length <= MAX_BAP_INDICATION_SIZE_APP_PB)
            { 
                poolName = POOL_enBapIndDataTypeAppPB;

            }
            else if(au16Length <= MAX_BAP_INDICATION_SIZE_APP_PB_LARGE)
            {
                poolName = POOL_enBapIndDataTypeAppPBLarge;
            }
            else
            {
                au16Length = MAX_BAP_INDICATION_SIZE_APP_PB;
                
                poolName = POOL_enBapIndDataTypeAppPB;
                

                DEBUG_VALUE1(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "IndByteSeq: indSize shortened for LsgId 0x%02x!", aLsgId);
            }
            break;

        case BapLsg_MDI:
            poolName = POOL_enBapIndDataTypeAppMdi;
            if(au16Length>MAX_BAP_INDICATION_SIZE_APP_MDI)
            {
                au16Length = MAX_BAP_INDICATION_SIZE_APP_MDI;
                

                DEBUG_VALUE1(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "IndByteSeq: indSize shortened for LsgId 0x%02x!", aLsgId);
            }
            break;
        
        case BapLsg_SD:
            poolName = POOL_enBapIndDataTypeKpb;
            if(au16Length>MAX_BAP_INDICATION_SIZE_KPB)
            {
                au16Length = MAX_BAP_INDICATION_SIZE_KPB;
                

                DEBUG_VALUE1(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "IndByteSeq: indSize shortened for LsgId 0x%02x!", aLsgId);
            }
            break;

        default:

            DEBUG_VALUE1(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "Unknown LsgID: LsgId 0x%02x", aLsgId);
            return;
    }

    sF = POOL_sfAllocate( poolName, (void**)&bap_ind.payload.byte_seq.data);
    if( sF != SUCCESS )
    {

        DEBUG_VALUE1(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "IndByteSeq: Allocate failed LsgId: 0x%02x", aLsgId);
        return;
    }
    
	bap_ind.payload.byte_seq.length = au16Length;

    (void)memcpy(bap_ind.payload.byte_seq.data, (Uint8Type*)apValue, au16Length);

    if( BAP_DISP_IndicationManager(&bap_ind) != SUCCESS)
    {

        DEBUG_VALUE2(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "IndByteSeq: IndManager failed (0x%02x, 0x%02x)", aLsgId, aFctId);

        /* release the allocated memory */
        (void)POOL_sfFree(bap_ind.payload.byte_seq.data);
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : BAP_IndicationError                                          */
/**
    \brief      This function populates the bap_indication structure and
                assigns it to IndicationManager

    \param      aLsgId
                lsgId_t
    \param      aFctId
                fctId_t
    \param      aeErrorCode
                enum BapError_t
    \return     void
   */
/*----------------------------------------------------------------------------*/
void BAP_IndicationError(lsgId_t aLsgId, fctId_t aFctId, enum BapError_t aeErrorCode)
{
	BapIndication bap_ind;

	bap_ind.lsg_id 		= aLsgId;	
	bap_ind.fct_id 		= aFctId;	
	bap_ind.data_type	= BAP_DT_ERROR;
    bap_ind.payload.error_code = aeErrorCode;

    if( BAP_DISP_IndicationManager(&bap_ind) != SUCCESS)
    {

        DEBUG_VALUE2(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "IndError: IndManager failed (0x%02x, 0x%02x)", aLsgId, aFctId);
    }
}


/*----------------------------------------------------------------------------*/
/* Function    : BAP_DISP_IndicationManager                                   */
/**
    \brief      This function send the incoming BapIndication to the responsible
                application through a dedicated osek message queue

    \param      pBapInd
                BapIndication *

    \return     SuccessFailType sF
                SUCCESS or FAIL   
 */
/*----------------------------------------------------------------------------*/
static SuccessFailType BAP_DISP_IndicationManager(BapIndication *pBapInd)
{
    SuccessFailType result = SUCCESS;

    if(pBapInd->ind_type == BapInd_Processing_CNF)
    {
        /* we answer the processing indication directly */
        BapRequest request;

        switch(pBapInd->fct_id)
        {
//        case BapFct_MDI_MDactivate:
//            request.lsg_id           = pBapInd->lsg_id;
//            request.fct_id           = pBapInd->fct_id;
//            request.req_type         = BapReq_Processing;
//            request.data_type        = BAP_DT_INT8;
//            request.payload.u8_value = 0;
//            break;

        default:
            request.lsg_id    = pBapInd->lsg_id;
            request.fct_id    = pBapInd->fct_id;
            request.req_type  = BapReq_Processing;
            request.data_type = pBapInd->data_type;
            break;
        }
        
        BAP_DISP_SendMessage(&request);     
    }
    else
    {
        StatusType sendResult = E_OK;


        switch (pBapInd->lsg_id)
        {
            case BapLsg_Telephone:

                DEBUG_VALUE1(BAP_DISPATCHER_SWI_TRC, MOD_BAP_DISPATCHER, "Send BAPMsg (FctID 0x%02x) to AppTelephone", pBapInd->fct_id);

                sendResult = SendMessage(msgPhoneBapIndication, pBapInd);
                break;
            case BapLsg_Phonebook:

                DEBUG_VALUE1(BAP_DISPATCHER_SWI_TRC, MOD_BAP_DISPATCHER, "Send BAPMsg (FctID 0x%02x) to AppPhonebook", pBapInd->fct_id);

                sendResult = SendMessage(msgPhoneBookBapIndication, pBapInd);
                break;
            case BapLsg_MDI:

                DEBUG_VALUE1(BAP_DISPATCHER_SWI_TRC, MOD_BAP_DISPATCHER, "Send BAPMsg (FctID 0x%02x) to AppMDI", pBapInd->fct_id);

                sendResult = SendMessage(msgMdiBapIndication, pBapInd);
                break;
            case BapLsg_SD:

                DEBUG_VALUE1(BAP_DISPATCHER_SWI_TRC, MOD_BAP_DISPATCHER, "Send BAPMsg (FctID 0x%02x) to KPB", pBapInd->fct_id);

                sendResult = SendMessage(msgKpbBapIndication, pBapInd);
                break;
            default:

                DEBUG_VALUE2(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "Unhandled BAP Message: LsgId 0x%02x FctID 0x%02x", pBapInd->lsg_id, pBapInd->fct_id);

                sendResult = !E_OK;
                break;
        }

        if(sendResult != E_OK)
        {

            DEBUG_VALUE2(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "SendMessage failed: LsgId 0x%02x FctID 0x%02x", pBapInd->lsg_id, pBapInd->fct_id);

            result = FAIL;
        }
    }
    
    return result;
}


/*----------------------------------------------------------------------------*/
/* Function    : BAP_DISP_ProcessBapRequestMessage                            */
/**
    \brief      This module is responsible for the processing request from the
                Application to be sent to ASG.

    \param      pRequest
                const BapRequest *

    \return     void 
 */
/*----------------------------------------------------------------------------*/
void BAP_DISP_ProcessBapRequestMessage(const BapRequest *pRequest)
{
    BAP_DISP_AckQueueCtrl *pCtrl = NULL;

	/* check if function id used ack mechanism , if so call
	 * otherwise directly call bap_send_message */
    pCtrl = BAP_DISP_GetAckQueueCtrl(pRequest->lsg_id, pRequest->fct_id);

	if ( pCtrl != NULL )
	{   
        /* this function id uses the BAP ack mechanism -> use specific handler
	    function to take care about possible required queuing and related
	    flag settings */
	    BAP_DISP_SendMessageWithAck(pCtrl, pRequest);

	}
	else
	{   
        /* directly send the message */
	    BAP_DISP_SendMessage(pRequest);
	}
}


/*----------------------------------------------------------------------------*/
/* Function    : BAP_DISP_SendMessage                                         */
/**
    \brief      This function sends the bap request to the bap stack

    \param      request
                const BapRequest * 

    \return     void */
/*----------------------------------------------------------------------------*/

void BAP_DISP_SendMessage( const BapRequest *request )
{   
    /* Stores the return values of the BAP APIs */
    BapError_et eErr = BapErr_OK;   


    DEBUG_VALUE1(BAP_DISPATCHER_SWI_TRC, MOD_BAP_DISPATCHER, "SendMsg: (FctID 0x%02x) to BapStack", request->fct_id);

	switch (request->data_type)
	{
	    case BAP_DT_INT8:
	    {
	        /* Call the BAP_RequestInt8 to send the message
	         * from application to BCL buffer	         */
	        eErr = BAP_RequestInt8(	request->lsg_id, 
                                    request->fct_id,
	        		                request->req_type, 
                                    request->payload.u8_value);
	        break;
	    }
	    case BAP_DT_INT16:
	    {
	        /* Call the BAP_RequestIn16 to send the message
	         * from application to BCL buffer	         */
	        eErr = BAP_RequestInt16(   request->lsg_id,	
                                       request->fct_id,
	        		                   request->req_type, 
                                       request->payload.u16_value);
	        break;
	    }
	    case BAP_DT_INT32:
	    {
	        /* Call the BAP_RequestIn32 to send the message
	         * from application to BCL buffer	         */
	        eErr = BAP_RequestInt32(   request->lsg_id,
                                       request->fct_id,
                                       request->req_type, 
                                       request->payload.u32_value);
	        break;
	    }
	    case BAP_DT_BYTE:
	    {
	        /* Call the BAP_RequestByteSequence to send
	         * the message from application to BCL buffer*/
	        eErr = BAP_RequestByteSequence( request->lsg_id, 
                                            request->fct_id,
                                            request->req_type, 
                                            request->payload.byte_seq.data,
                                            request->payload.byte_seq.length );
	        break;
	    }
	    case BAP_DT_VOID:
	    {
	        /* Call the BAP_RequestVoid to send the message
	         * from application to BCL buffer	         */
	        eErr = BAP_RequestVoid(	request->lsg_id, 
                                    request->fct_id,
	        		                request->req_type );
	        break;
	    }
	    case BAP_DT_ERROR:
	    {
	        /* Call the BAP_RequestError to send the
	         * message from application to BCL buffer    */
	        eErr = BAP_RequestError( request->lsg_id,	
                                     request->fct_id,
	        		                 request->payload.error_code );
	        break;
	    }
	    default:
	    {

            DEBUG_VALUE2(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "SendMsg: INVALID REQ:fctid=0x%02X type=0x%02X", request->fct_id, request->data_type);

	        break;
	    }
    }/*End switch*/

	/* check for error */
	if (eErr != BapErr_OK)
    {    

        DEBUG_VALUE4(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "SendMsg: send_err 0x%02X 0x%02X err=%d", request->fct_id, request->data_type, eErr, 0);

    }
}


#ifdef BAP_USES_ACKNOWLEDGE
/*----------------------------------------------------------------------------*/
/* Function    : BAP_DISP_Acknowledge                                         */
/**
    \brief      callback for bap stack v1.4 to handle acknowledged\n
                for prototype and german explanation see bap.h\n

    \param      aLsgId
                lsgId_t
    \param      aFctId
                fctId_t
    \param      aeAcknowledge
                BapAcknowledge_et
    \return     void
*/
/*----------------------------------------------------------------------------*/
BAP_FAR void BAP_Acknowledge(   lsgId_t aLsgId,
        fctId_t aFctId,
        BapAcknowledge_et aeAcknowledge)
{
    BAP_DISP_AckQueueCtrl *pCtrl = NULL;

    /* check if for function ID Ack is used */
    pCtrl = BAP_DISP_GetAckQueueCtrl(aLsgId, aFctId);
    if (pCtrl)
    {

        DEBUG_VALUE4( BAP_DISPATCHER_SWI_TRC, MOD_BAP_DISPATCHER, "Ack used for lsg=0x%02X fct=0x%02X ack=%d",
                aLsgId, aFctId, aeAcknowledge, 0);

        if((aLsgId == BapLsg_Telephone)&&(aFctId == BapFct_Telephone_CallInfo)) /*changes needed to serialize the BAP transfer */
        {
            APP_PHONE_BAP_vOnBAPDispatcherNotification(APP_BAPDISPATCHER_NOTIFY_CALLINFO_SENT);
        }

        if (aLsgId == BapLsg_Phonebook)
        {
            switch (aFctId)
            {
                case BapFct_Phonebook_MissedCalls:
                case BapFct_Phonebook_ReceivedCalls:
                case BapFct_Phonebook_DialedNumbers:
                case BapFct_Phonebook_Phonebook:
                {
                    APP_PHONEBOOK_BAP_vOnBAPDispatcherNotification(APP_BAPDISPATCHER_NOTIFY_STATUSARRAY_SENT, aFctId); /*RT13652*/
                    break;
                }
                default:
                    /* do nothing for other function identifier */
                    break;
            }
        }

        if (TRUE == pCtrl->ack_pending)
        {
            SuccessFailType result = SUCCESS;
            BapRequest request;

            result = BAP_DISP_RemoveQueueElement(pCtrl, &request);

            if (result == SUCCESS)
            {
                BAP_DISP_SendMessage(&request);

                if(request.data_type != BAP_DT_BYTE)
                {

                    DEBUG_VALUE2( BAP_DISPATCHER_SWI_WNG, MOD_BAP_DISPATCHER, "BAP_Ack: fct 0x%02X data_type %d",
                            request.fct_id, request.data_type);
                }

                /* release the allocated payload data */ 
                (void)APP_COMMON_UTILS_sfFreeBapRequest(&request);


                DEBUG_VALUE2( BAP_DISPATCHER_SWI_TRC, MOD_BAP_DISPATCHER, "BAP_Ack: fct 0x%02X length %d succ sent",
                        request.fct_id, request.payload.byte_seq.length);
            }
            else
            {
                /* else - no queued messages to be send!!! */
                pCtrl->ack_pending = FALSE;
            }
        }
        else
        {   
            /* oops - mismatch??? raise trace */

            DEBUG_VALUE2( BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "BAP_Ack: ack recvd w/o pending lsg=0x%02X fct=0x%02X",
                    aLsgId, aFctId);
        }
    }
    else
    {   /* ack is not used - just ignore!!! */

        DEBUG_VALUE2( BAP_DISPATCHER_SWI_TRC, MOD_BAP_DISPATCHER, "Ack UNused for lsg=0x%02X fct=0x%02X",
                aLsgId, aFctId);
    }
}
#endif

/*----------------------------------------------------------------------------*/
/* Function    : BAP_DISP_SendDebugInfo                                            */
/**
    \brief      Sendet die Diagnosemeldungen von BAP auf den CAN aus.\n
                for prototype and german explanation see bap.h\n

    \param      pDebugInfo 
                Speichert die aktuelle Debug-Information

    \return     BapError_t bapErr
                Fehlercodes von BAP. Diese werden als Rueckgabewert\n
                bei Funktionsaufrufen zurueckgegeben\n
*/
/*----------------------------------------------------------------------------*/
enum BapError_t
BAP_SendDebugInfo(const uint8_t pDebugInfo[7])
{
    enum BapError_t bapErr = BapErr_OK;

#if 0
    /* todo:  muss wenn gebraucht für uns angepasst werden      */
	/*  integration has to be done in second step               */
	/*  code is only copy from BAP Orginal VW Code example      */
    uint8_t u8InterruptStatus;

    /* Abbrechen wenn CAN Tx Handle bereits in Verwendung */
    if(!GET_CAN_CONFIRMATION_FLAG(CanTxmBAP_F1_10))
    {
        return(BapErr_Busy);
    }

    (void) memcpy((void*)&mBAP_F1_10, (void*)pDebugInfo, 7u);/*lint !e668
    Der Bap-Stack uebergibt einen Zeiger auf die Diagnosedaten, der nicht NULL ist */

    disableint(&u8InterruptStatus);
    CLEAR_CAN_CONFIRMATION_FLAG(CanTxmBAP_F1_10);
    restoreint(u8InterruptStatus);

    if(kCanTxOk != CanTransmit((CanTransmitHandle)CanTxmBAP_F1_10))
    {
        disableint(&u8InterruptStatus);
        SET_CAN_CONFIRMATION_FLAG(CanTxmBAP_F1_10);
        restoreint(u8InterruptStatus);

        bapErr = BapErr_Busy;
    }
#endif

    return bapErr;
}


void disableint(uint8_t *apu8SaveValue)
{
    DisableIrq();
}

void restoreint(uint8_t au8RestoreValue)
{
    EnableIrq();
}



/* End Of File BAP_Dispatcher.c */
