/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     BAP_Dispatcher_Ack.h
*-----------------------------------------------------------------------------
* Module Name:    BAP Dispatcher
*-----------------------------------------------------------------------------
* Description:    Implementation of BAP indications, that would be called by
*                 BAPTask() to post BAP indications coming from Head Unit to 
*                 BAP Dispatcher and posted to different Applications which
*                 handle the doing for the diffrent LSgID anf Fctid.
*                 Request to the Bap stack also handle here. 
*-----------------------------------------------------------------------------
* $Date: 2011-07-20 15:26:10 +0200 (Mi, 20 Jul 2011) $
* $Rev: 20966 $
* $Author: maennel $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_BAP_Dispatcher/trunk/inc/BAP_Dispatcher_Ack.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _BAP_DISPATCHER_ACK_H
#define _BAP_DISPATCHER_ACK_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/

#include "osek.h"
#include "BAP_Dispatcher.h"
/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/
#define     BAP_ACK_NOT_USED            0x00 /* indicator for functions not using ack */
#define     BAP_ACK_USED                0xFF /* used for initial map setup */

//#define     BAP_MAX_SUPPORTED_FCID      BapFct_Telephone_RestoreFactorySettings
//#define     BAP_DISP_FCID_MAP_ARRAY_SIZE  (BAP_MAX_SUPPORTED_FCID + 1)

/* to be incremented for each added function that uses Ack */

#define     BAP_QUEUE_SINGLE_ELEMENT    0x01
#define     BAP_QUEUE_ZERO              0x00
#define     BAP_QUEUE_MODULO            0x04

typedef enum
{
    /* the order is important!: 
     * smallest first -> biggest last */
    BAP_DISP_enPbStateSize      = 3,
    BAP_DISP_enSD_FrameStatus   = 6,
    BAP_DISP_enSD_FrameData     = 520,
    BAP_DISP_enUserListSize     = 525,
    BAP_DISP_enCallStackSize    = 735,
    BAP_DISP_enSMSRecvdSize     = 947,
    BAP_DISP_enCallInfoSize     = 985,
    // the last element specifies the overall max size (last element + 1) ´
    BAP_DISP_enAckQueueMaxSize
}BAP_DISP_enAckQueueSizes;

/* type is used for the pool handler */
typedef Uint8Type BAP_DISP_AckQueuePayloadType[BAP_DISP_enAckQueueMaxSize-1];


typedef struct BAP_DISP_AckQueueElem_struct
{
    struct BAP_DISP_AckQueueElem_struct *next;
    BapRequest                          request;
}BAP_DISP_AckQueueElem;

/* structures used for BAP acknowledge control */
typedef struct {
	lsgId_t lsg_id;            /* BAP lsg id for cross checks         */
	fctId_t fct_id;            /* BAP function id for cross checks         */
	Uint32Type  ack_pending;       /* indicator if an ACK is pending           */
	Uint32Type  num_queue_elements;/* number of total avaialble queue elements */
//	Uint32Type  element_size;      /* size in byte of one queue element        */
	BAP_DISP_enAckQueueSizes  payload_size;      /* payload size in byte of one queue element*/
//	Uint32Type  num_used_elements; /* number of used elements                  */
//	BapRequest *queue_start;       /* pointer to start address of queue        */
//	BapRequest *queue_end;         /* pointer to last element in queue         */
//	BAP_DISP_AckQueueElem *queue_head;/* pointer to head of cyclic queue     */
	BAP_DISP_AckQueueElem *queue_tail;/* pointer to tail of cyclic queue     */
} BAP_DISP_AckQueueCtrl;

typedef struct {
	Uint32Type  ack_pending;       /* indicator if an ACK is pending           */
	Uint32Type  num_queue_elements;/* number of total avaialble queue elements */
//	Uint32Type  element_size;      /* size in byte of one queue element        */
	BAP_DISP_enAckQueueSizes  payload_size;      /* payload size in byte of one queue element*/
//	Uint32Type  num_used_elements; /* number of used elements                  */
//	BapRequest *queue_start;       /* pointer to start address of queue        */
//	BapRequest *queue_end;         /* pointer to last element in queue         */
//	BAP_DISP_AckQueueElem *queue_head;/* pointer to head of cyclic queue     */
	BAP_DISP_AckQueueElem *queue_tail;/* pointer to tail of cyclic queue     */
} BAP_DISP_AckQueueCtrl2;

/*----------------------------------------------------------------------------*/
/* Import configuration                                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Public Constants                                                           */
/*----------------------------------------------------------------------------*/


void BAP_DISP_SendMessageWithAck( BAP_DISP_AckQueueCtrl *pCtrl,
                                  const BapRequest *pRequest);

BAP_DISP_AckQueueCtrl *BAP_DISP_GetAckQueueCtrl( lsgId_t aLsgId,
                                                 fctId_t aFctId);

SuccessFailType BAP_DISP_RemoveQueueElement( BAP_DISP_AckQueueCtrl *pCtrl,
                                             BapRequest *pRequest);

SuccessFailType BAP_DISP_RemoveAllQueueElements(void);

#endif /* _BAP_DISPATCHER_ACK_H */

/* End Of File BAP_Dispatcher_Ack.h */
