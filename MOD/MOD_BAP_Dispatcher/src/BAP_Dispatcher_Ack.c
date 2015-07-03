/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     BAP_Dispatcher_Ack.c
*-----------------------------------------------------------------------------
* Module Name:    BAP Dispatcher
*-----------------------------------------------------------------------------
* Description:    Implementation of BAP indications, that would be called by
*                 BAPTask() to post BAP indications coming from Head Unit to
*                 BAP Dispatcher and posted to different Applications which
*                 handle the doing for the diffrent LSgID anf Fctid.
*                 Request to the Bap stack also handle here.
*-----------------------------------------------------------------------------
* $Date: 2011-11-18 15:50:43 +0100 (Fr, 18 Nov 2011) $
* $Rev: 23630 $
* $Author: maennel $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_BAP_Dispatcher/trunk/src/BAP_Dispatcher_Ack.c $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/
#include "osek.h"
#include "bap_defines.h"
#include "APP_COMMON_Utils.h"
#include "POOL.h"

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "BAP_Dispatcher_Ack.h"
#include "BAP_Dispatcher.h"
#include "BAP_Dispatcher_CI.h"

#include "BAP_Dispatcher_Ack.id"
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
//static void BAP_DISP_InitAckQueues(void);
static SuccessFailType BAP_DISP_HandleNewQueueElem(BAP_DISP_AckQueueCtrl *pCtrl,
                                                   const BapRequest *pRequest);
static SuccessFailType _BAP_DISP_AddQueueElement( BAP_DISP_AckQueueCtrl *pCtrl,
                                                  const BapRequest *pRequest);

Uint32Type _BAP_DISP_GetQueueSize(const BAP_DISP_AckQueueCtrl *pCtrl);

/*----------------------------------------------------------------------------*/
/* File local Code Macros                                                        */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* File local Variables                                                       */
/*----------------------------------------------------------------------------*/
/**
\brief Configuration of the BAP-Ack queue
*/
static BAP_DISP_AckQueueCtrl _BAP_DISP_QueueCtrlList[] = {
/*  lsg_id, fct_id, ack_pending, num_queue_elem, payload_size, *queue_tail */
     { BapLsg_Phonebook, BapFct_Phonebook_SMSReceived, FALSE, 1, BAP_DISP_enSMSRecvdSize, NULL },
     { BapLsg_Phonebook, BapFct_Phonebook_PbState,  FALSE, 1, BAP_DISP_enPbStateSize, NULL },
     { BapLsg_Phonebook, BapFct_Phonebook_MissedCalls,  FALSE, 1, BAP_DISP_enCallStackSize, NULL },
     { BapLsg_Phonebook, BapFct_Phonebook_ReceivedCalls,  FALSE, 1, BAP_DISP_enCallStackSize, NULL },
     { BapLsg_Phonebook, BapFct_Phonebook_DialedNumbers,  FALSE, 1, BAP_DISP_enCallStackSize, NULL },
     { BapLsg_Telephone, BapFct_Telephone_CallInfo, FALSE, 1, BAP_DISP_enCallInfoSize, NULL },
     { BapLsg_Telephone, BapFct_Telephone_UserList, FALSE, 1, BAP_DISP_enUserListSize, NULL },
     { BapLsg_SD,        BapFct_SD_FrameStatus,     FALSE, 1, BAP_DISP_enSD_FrameStatus, NULL },
     { BapLsg_SD,        BapFct_SD_FrameData,       FALSE, 1, BAP_DISP_enSD_FrameData , NULL }
};


/*----------------------------------------------------------------------------*/
/* File local const Definitions                                               */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private Address Calculations                                               */
/*----------------------------------------------------------------------------*/




/*----------------------------------------------------------------------------*/
/* Function :   _BAP_DISP_GetQueueSize                                        */
/** 
  \brief        Function to return the queue control for a specific LsgId and FctId   

  \param        aLsgId
                BAP LsgID

  \param        aFctId
                BAP FctId

  \return       Uint32Type
                Number of queued elements

 */
/*----------------------------------------------------------------------------*/
BAP_DISP_AckQueueCtrl *BAP_DISP_GetAckQueueCtrl(lsgId_t aLsgId,
                                                fctId_t aFctId)
{
    Uint32Type i;
    Uint32Type elements = NUM_ELEMENTS_OF_ARRAY(_BAP_DISP_QueueCtrlList);
    BAP_DISP_AckQueueCtrl *pCtrl = NULL;

    for(i=0; i<elements; i++)
    {
        pCtrl = &_BAP_DISP_QueueCtrlList[i];

        if( (pCtrl->lsg_id == aLsgId) && 
                (pCtrl->fct_id == aFctId) )
        {
            /* we found the ack control */
            return pCtrl;
        }
    }
    
    /* if we reach this point, there is no ack control available */
    return NULL;
}

/*----------------------------------------------------------------------------*/
/* Function :   _BAP_DISP_GetQueueSize                                        */
/** 
  \brief        Function to return the number of queued elements  

  \param        pCtrl
                pointer to control block element


  \return       Uint32Type
                Number of queued elements

 */
/*----------------------------------------------------------------------------*/
Uint32Type _BAP_DISP_GetQueueSize(const BAP_DISP_AckQueueCtrl *pCtrl)
{
    Uint32Type counter = 0;
    BAP_DISP_AckQueueElem *pCur = pCtrl->queue_tail;
    
    while(pCur)
    {
        counter++;   
        pCur = pCur->next;
    }
    
    return counter;
}

/*----------------------------------------------------------------------------*/
/* Function :   _BAP_DISP_AddQueueElement                                     */
/** 
  \brief        Function to add a new element to the queue  

  \param        pCtrl
                pointer to control block element

  \param        pRequest
                pointer to the BAP request

  \return       SuccessFailType
                SUCCESS if adding of the queue element was possible, FAIL otherwise

 */
/*----------------------------------------------------------------------------*/
SuccessFailType _BAP_DISP_AddQueueElement(  BAP_DISP_AckQueueCtrl *pCtrl,
                                            const BapRequest *pRequest)
{
    SuccessFailType retVal          = SUCCESS;
    BAP_DISP_AckQueueElem *pNewElem = NULL;
    
    retVal = POOL_sfAllocate( POOL_enBapDispAckQueueElem,
                              (void**)&pNewElem); 

    if(retVal != SUCCESS)
    {
        /* queuing failure!!! */

        DEBUG_VALUE1(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "GetNewAckQueueElement Header alloc failed fct_id: 0x%2.2X", pCtrl->fct_id);
        return FAIL;
    }
    
 
    (void)memcpy(&pNewElem->request, pRequest, sizeof(*pRequest)); 
    
    if(pNewElem->request.data_type == BAP_DT_BYTE)
    {
        /* we need to allocate memory for the payload */
        retVal = POOL_sfAllocate( POOL_enBapDispAckQueuePayloadType,
                                  (void**)&pNewElem->request.payload.byte_seq.data); 

        if(retVal != SUCCESS)
        {
            /* queuing failure!!! */

            DEBUG_VALUE2(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "GetNewAckQueueElement: Payload alloc failed fct_id: 0x%2.2X, %d", pCtrl->fct_id, POOL_enGetAllocateErrorCode());

            /* release the allocated memory for the queue element*/
            (void)POOL_sfFree(pNewElem);
    
            return FAIL;
        }
        
        (void)memcpy( pNewElem->request.payload.byte_seq.data, 
                pRequest->payload.byte_seq.data, 
                pRequest->payload.byte_seq.length); 
    }    

    
    /* add the new element to the tail of the queue*/
    pNewElem->next      = pCtrl->queue_tail;
    pCtrl->queue_tail   = pNewElem;

    return retVal;
}

/*----------------------------------------------------------------------------*/
/* Function :   BAP_DISP_RemoveQueueElement                                   */
/** 
  \brief        Function to remove and return the oldest element of the queue  

  \param        pCtrl
                pointer to control block element

  \param        pRequest
                pointer to the removed queued BAP request

  \return       SuccessFailType
                SUCCESS if removing of a queue element was possible, FAIL otherwise

 */
/*----------------------------------------------------------------------------*/
SuccessFailType BAP_DISP_RemoveQueueElement(BAP_DISP_AckQueueCtrl *pCtrl, BapRequest *pRequest)
{
    SuccessFailType retVal          = SUCCESS;
    BAP_DISP_AckQueueElem *pCur     = NULL;
    BAP_DISP_AckQueueElem *pHead    = NULL;

    pCur = pCtrl->queue_tail;
    
    if(pCur == NULL)
    {
        /* no element available */
        retVal = FAIL;
    }
    else
    {
        if(pCur->next == NULL)
        {   
            /* one element is available */
            pHead = pCur;
            pCtrl->queue_tail = NULL;
        }
        else
        {
            /* more then one element are available */
            while(pCur->next->next != NULL)        
            {
                pCur = pCur->next;
            }

            pHead = pCur->next;
            pCur->next = NULL; 
        }
        
        (void)memcpy(pRequest, &pHead->request, sizeof(*pRequest));
        (void)POOL_sfFree(pHead);
    }

    return retVal;
}

/*----------------------------------------------------------------------------*/
/* Function :   BAP_DISP_RemoveAllQueueElements                               */
/**
  \brief        Function to remove all of the queued elements in case of re-initialization

  \param        void

  \return       SuccessFailType
                SUCCESS if removing of a queue element was possible, FAIL otherwise

 */
/*----------------------------------------------------------------------------*/
SuccessFailType BAP_DISP_RemoveAllQueueElements(void)
{

    Uint32Type i;
    Uint32Type elements = NUM_ELEMENTS_OF_ARRAY(_BAP_DISP_QueueCtrlList);
    BAP_DISP_AckQueueCtrl *pCtrl = NULL;
    SuccessFailType retVal = SUCCESS;
    BapRequest pTmpReq;

    for(i=0; i<elements; i++)
    {
        pCtrl = &_BAP_DISP_QueueCtrlList[i];

        while(_BAP_DISP_GetQueueSize(pCtrl)>0)
        {
            retVal = BAP_DISP_RemoveQueueElement(pCtrl, &pTmpReq);
            if(retVal != SUCCESS)
            {
                return retVal;
            }
            else
            {
                /* release the allocated payload data */
                retVal = APP_COMMON_UTILS_sfFreeBapRequest(&pTmpReq);
            }
        }

        pCtrl->ack_pending = FALSE;
    }

    /* if we reach this point, there is no ack control available */
    return retVal;
}

/*----------------------------------------------------------------------------*/
/* Function :   BAP_DISP_HandleNewQueueElem                                   */
/** 
  \brief        Function to add a bap request to a queue element  

  \param        pCtrl
                pointer to control block element

  \param        pRequest
                pointer to bap request

  \return       SuccessFailType
                SUCCESS if queuing was possible, FAIL otherwise

 */
/*----------------------------------------------------------------------------*/
SuccessFailType BAP_DISP_HandleNewQueueElem( BAP_DISP_AckQueueCtrl *pCtrl, 
                                             const BapRequest *pRequest)
{
    SuccessFailType retVal = SUCCESS;
	
    /* first check if there exist queue elements for queuing, otherwise
	 * message is "discarded" */
    if(pCtrl->num_queue_elements == 0u)
	{
	   /* no queueing elements - discard message */
	   /* queuing failure!!! */

        DEBUG_VALUE4( BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "HaNewQueueElem no queue elem for 0x%02X pend %d numq %d",
                      (Uint8Type)pCtrl->fct_id, (Uint8Type)pCtrl->ack_pending, (Uint8Type)pCtrl->num_queue_elements, 0);

	   retVal = FAIL;
    }
    else
    {
	    /* queue with at least one element */
        Uint32Type queueSize = _BAP_DISP_GetQueueSize(pCtrl);

	    if (pCtrl->num_queue_elements == queueSize)
		{   
            /* queue is already full -> remove oldest queue element 
             * which is stored in head */
            BapRequest pTmpReq;


            DEBUG_VALUE2(BAP_DISPATCHER_SWI_TRC, MOD_BAP_DISPATCHER, "HaNewQuElem: full, drop oldest elem (%d %d)", pCtrl->lsg_id, pCtrl->fct_id);

            retVal = BAP_DISP_RemoveQueueElement(pCtrl, &pTmpReq);
        
            if(retVal != SUCCESS)
            {

                DEBUG_VALUE2(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "HaNewQueueElem failed to remove old queue elem(%d %d)", pCtrl->lsg_id, pCtrl->fct_id);

                return retVal;
            }

            /* release the allocated payload data */ 
            retVal = APP_COMMON_UTILS_sfFreeBapRequest(&pTmpReq);

            if(retVal != SUCCESS)
            {

                DEBUG_VALUE2(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "HaNewQueueElem failed to free old queue elem(%d %d)", pCtrl->lsg_id, pCtrl->fct_id);

                return retVal;
            }


            retVal = _BAP_DISP_AddQueueElement(pCtrl, pRequest);

            if(retVal != SUCCESS)
            {

                DEBUG_VALUE2(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "HaNewQueueElem failed to add new queue elem(%d %d)", pCtrl->lsg_id, pCtrl->fct_id);
            }
        }
        else
        {
		    /* there is still space in the queue -> add element at tail */
            retVal = _BAP_DISP_AddQueueElement(pCtrl, pRequest);
		}
	}

	return retVal;
}


/*----------------------------------------------------------------------------*/
/* Function    : BAP_DISP_SendMessageWithAck                                  */
/**
    \brief      This function sends a bap request to bap stack layer
                using the ack mechanism within function several checks 
                are done if required control

    \param      pCtrl
                pointer to control block element

    \param      pRequest
                pointer to bap request

    \return     void 
 */
/*----------------------------------------------------------------------------*/

void BAP_DISP_SendMessageWithAck( BAP_DISP_AckQueueCtrl *pCtrl, 
        const BapRequest *pRequest)
{
    /* Note that the check if this function uses ack has already been done
     * in bap_process_request_from_al before this messages is called      */
    if ( pCtrl->fct_id == pRequest->fct_id )
    {
        /* check if no ack ongoing, if so just send and modify flag */
        if (FALSE == pCtrl->ack_pending)
        {
            pCtrl->ack_pending = TRUE;


            DEBUG_VALUE2(BAP_DISPATCHER_SWI_TRC, MOD_BAP_DISPATCHER, "SendMsgWithAck: fct 0x%02X and length %d directly send", pRequest->fct_id, pRequest->payload.byte_seq.length);

            BAP_DISP_SendMessage(pRequest);
        }
        else
        {
            /* message has to be queued */
            if (SUCCESS == BAP_DISP_HandleNewQueueElem(pCtrl,pRequest))
            {

                DEBUG_VALUE2(BAP_DISPATCHER_SWI_TRC, MOD_BAP_DISPATCHER, "SendMsgWithAck: fct 0x%02X length %d succ queued", pRequest->fct_id, pRequest->payload.byte_seq.length);
            }
            else
            {

                DEBUG_VALUE1(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "SendMsgWithAck: fct 0x%02X QUEUING ERROR!!!", pRequest->fct_id);
            }
        }
    }
    else
    {
        /* some problems here - just send the message WITHOUT using the ack
	       mechanism */

        DEBUG_VALUE4(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "SendMsgWithAck: mismatch fct=0x%02X(0x%02X) lsg=0x%02X(0x%02X)", pRequest->fct_id, pCtrl->fct_id, pRequest->lsg_id, pCtrl->lsg_id);

        BAP_DISP_SendMessage(pRequest);
    }
}


/* End Of File BAP_Dispatcher.c */
