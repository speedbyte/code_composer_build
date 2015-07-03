/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     KPB_C1.c
*-----------------------------------------------------------------------------
* Module Name:    KPB
*-----------------------------------------------------------------------------
* Description:    This module handles the KPB logic. It receives and process
                  incoming Requests/Responses from the KI as well as presentation
                  requests from the APP_SD module.
*-----------------------------------------------------------------------------
* $Date: 2012-02-09 09:48:25 +0100 (Do, 09 Feb 2012) $
* $Rev: 25642 $
* $Author: maennel $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_KPB/trunk/src/KPB_C1.c $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include <osek.h>           // vector
#include <osekext.h>        // vector
#include "bap_defines.h"    // vw
#include "Pool.h"
#include "APP_SD.h"
#include "APP_SD_FrameBuilder.h"
#include "APP_COMMON_Utils.h"
#include "BAP_Dispatcher.h"
#include "VCR.h"
#include "EEPROM_LAYOUT_CFG.h"
#include "OSEK_AppTask_H1.h"
#include "APP_PHONE.h"
#include "CONTEXT_MAN.h"
#include "CSHDL.h"
#include "NM_CB.h"
#include "OSEK_AppTask_Alarm.h"

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "KPB.h"
#include "KPB_CI.h"
#include "KPB_BAP.h"
#include "KPB_Alarm.h"

#include "KPB_C1.id"
#include "Trace.h"
/*----------------------------------------------------------------------------*/
/* pragma Directives                                                          */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Definitions (avoid as much as possible!)                  */
/*----------------------------------------------------------------------------*/

/**
\brief	variable that contains the module global variables
*/
KpbContext          gKpbContext;
KpbFrameDataContext gKpbFrameBuffer;
KpbFrameDataReqType gKpbLastFrameDataSent;

static Uint32Type gKpbObservedCellsHash[ KPB_OBSERVED_CELLS_MAX ];

static Uint8Type const gKpbObservedCells[KPB_FRAME_ID_7+1] =
    { 0,
      KPB_OBSERVED_CELLS_FRAME_ID_1,
      KPB_OBSERVED_CELLS_FRAME_ID_2,
      KPB_OBSERVED_CELLS_FRAME_ID_3,
      KPB_OBSERVED_CELLS_FRAME_ID_4,
      KPB_OBSERVED_CELLS_FRAME_ID_5,
      KPB_OBSERVED_CELLS_FRAME_ID_6,
      KPB_OBSERVED_CELLS_FRAME_ID_7
    };

static Uint8Type const gKpbObservedCellsOffset[KPB_FRAME_ID_7+1] =
    { 0,
      KPB_OBSERVED_CELLS_OFFSET_FRAME_ID_1,
      KPB_OBSERVED_CELLS_OFFSET_FRAME_ID_2,
      KPB_OBSERVED_CELLS_OFFSET_FRAME_ID_3,
      KPB_OBSERVED_CELLS_OFFSET_FRAME_ID_4,
      KPB_OBSERVED_CELLS_OFFSET_FRAME_ID_5,
      KPB_OBSERVED_CELLS_OFFSET_FRAME_ID_6,
      KPB_OBSERVED_CELLS_OFFSET_FRAME_ID_7
    };
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
static SuccessFailType _KPB_sfHandlePendingSendFrameData(void);
static SuccessFailType _KPB_sfHandlePendingShutdown(void);
static void _KPB_vHandleKpbIndication(KpbIndType *pKpbInd);
static void _KPB_vHandleOperationState(const KpbOpStateType *pOpState);
static void _KPB_vHandleAsgCapabilities(const KpbAsgCapType *pAsgCap);
static void _KPB_vHandleAsgConfig(const KpbAsgConfType *pAsgConf);
static void _KPB_vHandleLsgStatus(const KpbLsgStatusType *pLsgStatus);
static void _KPB_vHandleMflBlockDefinition(const KpbMflBlockDefType *pMflBlockDef);
static void _KPB_vHandleFrameStatus(KpbFrameStatusType *pFrameStatus);
static void _KPB_vHandleFrameStatus_SendFd(KpbFrameStatusType *pFrameStatus);
static void _KPB_vHandleFrameStatus_Shutdown(KpbFrameStatusType *pFrameStatus);
static void _KPB_vHandleFrameStatus_Default(const KpbFrameStatusType *pFrameStatus);
static void _KPB_vHandleFrameData(KpbFrameDataIndType *pFrameData);
static void _KPB_vHandleFrameDataAck(const KpbFrameDataAckType *pFrameDataAck);
static void _KPB_vHandleFrameDataAck_SendFd(const KpbFrameDataAckType *pFrameDataAck);
static void _KPB_vSendFrameStatusVisOn(const KpbFrameStatusVal *pFs, Uint32Type line);
static void _KPB_vSendFrameStatusVisOff(const KpbFrameStatusVal *pFs, Uint32Type line);
static void _KPB_vSendFrameStatusIntPresReqOff(const KpbFrameStatusType *pFs);
static void _KPB_vProcessWaiting(void);
static void _KPB_vSetNextJob(KpbPending *pPending);
static void _KPB_vReleasePendingContext(Uint32Type line, BooleanType bSucc);
static Uint16Type _KPB_u16GetCellsSet(const KpbFrameDataReqType *pData);
static void _KPB_vNotifyApplication(KPB_enNotificationId new_notification);
static void _KPB_vNotifyApplicationImmediate(KPB_enNotificationId new_notification);
static void _KPB_vSetKpbInitState(KPB_enFunctionIdType initPart);
static void _KPB_vKpbInitStateAfterProtocolReset(KPB_enFunctionIdType initPart);
static BooleanType _KPB_bIsKpbInitDone();
static KpbLastModeType _KPB_stGetLastMode();
static void _KPB_vSetAsgVisibility(BooleanType visible, Uint32Type line);
static void _KPB_vLimitCellSize(KpbFrameDataReqType *pFrameData, Uint32Type maxChars);
static void _KPB_vRemoveRedundantCell(KpbFrameDataReqType *pFrameData);
static void _KPB_vSetLastModeVisible(BooleanType visible);

/* call back functions */
void _KPB_cbStartASGSupervision(void);

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
Uint8Type MOD_KPB_Protocol_ErrorCounter = 0;

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
/* Function    : KPB_sfInit                                                   */
/**
    \brief      Callback function for initialization of MOD_KPB


    \return     SuccessFailType
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType KPB_sfInitApp()
{
    SuccessFailType sF = SUCCESS;

    (void)memset(&gKpbContext, 0, sizeof(gKpbContext));
    (void)memset(&gKpbObservedCellsHash, 0, sizeof(gKpbObservedCellsHash));

    gKpbContext.opState         = KPB_OP_STATE_OFF;
    gKpbContext.versionNumber   = KPB_VERSION_1_0;

    gKpbContext.lastMode        = _KPB_stGetLastMode();

    gKpbFrameBuffer.free = BT_TRUE;

    _KPB_vChangeAsgState(KPB_ASG_STATUS_UNDEFINED);

    return sF;
}


/*----------------------------------------------------------------------------*/
/* Function    : _KPB_stGetLastModeVisible                                      */
/**
    \brief      Reads the Last Mode Visible state from the EEPROM

    \return     BooleanType
    			BT_TRUE, BT_FALSE
*/
/*----------------------------------------------------------------------------*/
static KpbLastModeType _KPB_stGetLastMode()
{
    KpbLastModeType lastMode;
    CommonResponseCodeType retVal_E2P = COMMON_RSP_OK;
    memset(&lastMode, 0, sizeof(lastMode));

    /* cancel alarm to prevent from overwriting the lastMode value */
    (void)APP_COMMON_UTILS_stCancelOsekRelAlarm(CTA_KPB_DISPLAY_STATUS_OFF);

    retVal_E2P = EEPROM_enGetSingleByteFromRamMirror((Uint8Type *)&lastMode, 
                                                     EEPROM_LAYOUT_ENUM_KPB_Display_Status);
    if(retVal_E2P == COMMON_RSP_ERROR_BUSY)
    {
        DEBUG_TEXT(KPB_SWI_ERR, MOD_KPB, "EEProm(LAST_MODE): failed");
    }
    else
    {
        DEBUG_VALUE2(KPB_SWI_TRC, MOD_KPB, "EEProm(LAST_MODE): %d,%d", lastMode.inPresentation, lastMode.cradleRestriction);
    }

    return lastMode;
}
/*----------------------------------------------------------------------------*/
/* Function    : _KPB_vSetNextJob                                         */
/**
  \brief      Starts the processing of waiting functions

  \return     void
 */
/*----------------------------------------------------------------------------*/
static void _KPB_vSetNextJob(KpbPending *pPending)
{
    switch(pPending->common.id)
    {
        case KPB_PENDING_SEND_FD:
            if(gKpbContext.waitForProcessing.sendFd.common.id != KPB_PENDING_INVALID)
            {
                DEBUG_TEXT( KPB_SWI_ERR, MOD_KPB, "_KPB_vSetNextJob: overwriting sendFd job!!");
            }
            gKpbContext.waitForProcessing.sendFd = pPending->sendFd;
            break;
        case KPB_PENDING_SHUTDOWN:
            if(gKpbContext.waitForProcessing.shutdown.common.id != KPB_PENDING_INVALID)
            {
                DEBUG_TEXT( KPB_SWI_ERR, MOD_KPB, "_KPB_vSetNextJob: overwriting shutdown job!!");
            }
            gKpbContext.waitForProcessing.shutdown = pPending->shutdown;
            break;
        default:
            DEBUG_VALUE1(  KPB_SWI_ERR, MOD_KPB, "_KPB_vSetNextJob: unknown job %d!!",
                           pPending->common.id);
            break;
    }

    _KPB_vProcessWaiting();
}

/*----------------------------------------------------------------------------*/
/* Function    : _KPB_vProcessWaiting                                         */
/**
  \brief      Starts the processing of waiting functions

  \return     void
 */
/*----------------------------------------------------------------------------*/
static void _KPB_vProcessWaiting()
{
    /* check if no processing is currently active */
    if(gKpbContext.pending.common.id == KPB_PENDING_INVALID)
    {
        if(gKpbContext.waitForProcessing.sendFd.common.id == KPB_PENDING_SEND_FD)
        {
            gKpbContext.pending.sendFd = gKpbContext.waitForProcessing.sendFd;

            (void)memset( &gKpbContext.waitForProcessing.sendFd,
                          0,
                          sizeof(gKpbContext.waitForProcessing.sendFd));

        }
        else if(gKpbContext.waitForProcessing.shutdown.common.id == KPB_PENDING_SHUTDOWN)
        {
            gKpbContext.pending.shutdown = gKpbContext.waitForProcessing.shutdown;

            (void)memset( &gKpbContext.waitForProcessing.shutdown,
                          0,
                          sizeof(gKpbContext.waitForProcessing.shutdown));
        }
        else
        {

            DEBUG_TEXT(KPB_SWI_DBG, MOD_KPB, "_KPB_vProcessWaiting: no waiting process avail");
            return;
        }

        /* set alarm to process next */
        (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_KPB_FCT_PENDING,MSEC(0),0);
    }
    else
    {
        DEBUG_VALUE1(KPB_SWI_WNG, MOD_KPB, "_KPB_vProcessWaiting: unable to process next, current id (%d)", 
                     gKpbContext.pending.common.id);
    }
}

/*----------------------------------------------------------------------------*/
/* Function : _KPB_vReleasePendingContext                                     */
/**
  \brief      Releases the current function context

  \brief      Description:\n
              Releases the current function context and invokes the processing
              of waiting function afterwards

  \return     void
 */
/*----------------------------------------------------------------------------*/
static void _KPB_vReleasePendingContext(Uint32Type line, BooleanType bSucc)
{

        DEBUG_VALUE2(KPB_SWI_TRC, MOD_KPB, "vRelPendCon: release %d, bSucc= %d", gKpbContext.pending.common.id, bSucc);
        DEBUG_VALUE1(KPB_SWI_TRC, MOD_KPB, "in line %d", line);

    if( gKpbContext.pending.common.id == KPB_PENDING_SEND_FD &&
        gKpbContext.pending.sendFd.pData != NULL)
    {

        DEBUG_TEXT(KPB_SWI_TRC, MOD_KPB, "vRelPendCon: sendFd release frameData");

        KPB_vReleaseFrameDataElement(&(gKpbContext.pending.sendFd.pData), bSucc);
    }
    else if( gKpbContext.pending.common.id == KPB_PENDING_SHUTDOWN )
    {
        if( CSHDL_bIsKl15On() == BT_TRUE )
        {

            DEBUG_TEXT(KPB_SWI_TRC, MOD_KPB, "vRelPendCon: shutDown KL15_ON");

            KPB_vOnAppCshdlNotification(APP_CSHDL_NOTIFY_UHV_ACTIVE_KL_15_ON);
        }
    }

    (void)memset( &gKpbContext.pending, 0, sizeof(gKpbContext.pending));

    _KPB_vProcessWaiting();
}

/*----------------------------------------------------------------------------*/
/* Function    : KPB_vHandleEvents                                            */
/**
    \brief      Task Function to handle events from Application Task

    \brief      Description:\n
                Waits for incoming events and calls the event handler

    \param      event
                EventMaskType
                Bitmask containing all unhandled events

    \return     void
*/
/*----------------------------------------------------------------------------*/
void KPB_vHandleEvents(EventMaskType event)
{
    // wait for interesting events

    // moved from the bottom to the top for ticket 3201
    if(gKpbContext.notifyPresentationOff == BT_TRUE)
    {
        gKpbContext.notifyPresentationOff = BT_FALSE;

        //RT #5153: do not change the lastMode to OFF in queued up state
        if(gKpbContext.isFsgQueuedUp != BT_TRUE)
        {
            gKpbContext.lastMode.inPresentation = LAST_MODE_OFF;
        }

        if( (_KPB_bIsKpbInitDone()) && (BT_FALSE == gKpbContext.isAppsNotified))
        {
            _KPB_vNotifyApplication(KPB_NOTIFY_VISIBLE_OFF);
        }
    }


    // *****************************************************
    // look for events from BAPDispatcher and process them
    // check for new indication messages from BAP Dispatcher
    // *****************************************************
	if(event & evAppTaskKpbBapInd )
	{
        BapIndication bapInd;
        KpbIndType kpbInd;

        (void)ClearEvent(evAppTaskKpbBapInd);

        (void)ReceiveMessage( msgKpbBapIndication , &bapInd);

        if( _KPB_sfParseBapIndication(&bapInd, &kpbInd) == SUCCESS )
        {
            _KPB_vHandleKpbIndication(&kpbInd);
        }
        (void)APP_COMMON_UTILS_sfFreeBapIndication(&bapInd);

        // check queue Status
        if( E_COM_NOMSG != GetMessageStatus(msgKpbBapIndication))
        {
            // there are still some messages
            (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_APP_KPB_BAP_IND,
                                               MSEC(10),
                                               0);
        }
    }


    if(event & evAppTaskKpbPending)
    {
        SuccessFailType result = SUCCESS;

        (void)ClearEvent(evAppTaskKpbPending);

        switch(gKpbContext.pending.common.id)
        {
            case KPB_PENDING_SEND_FD:
                result = _KPB_sfHandlePendingSendFrameData();
                break;
            case KPB_PENDING_SHUTDOWN:
                result = _KPB_sfHandlePendingShutdown();
                break;
            default:

                DEBUG_VALUE1(KPB_SWI_ERR, MOD_KPB, "KPB_vHandleEvents: inval. pend. id %d", gKpbContext.pending.common.id);

                result = FAIL;
                break;
        }

        if(result != SUCCESS)
        {
            _KPB_vReleasePendingContext(__LINE__, BT_FALSE);
        }
    }
	/* ASG supervision part. In the following code the ASG supervision is implemented regarding CR21 */
	if(event & evAppTaskKpbSupervision)//changed by nikolov, RT:11346
    {
	
		(void)ClearEvent(evAppTaskKpbSupervision);
		/* in normal case the status remains to alive*/
		if(KPB_enGetAsgState() == KPB_ASG_STATUS_ALIVE)
		{
			if(gKpbContext.isAsgResponseOutstanding == BT_FALSE)
			{/* normally ASG status is delivered every 3 sec on time*/
				_KPB_vStartAsgSupervision();

				/* if any from the ASG alive message were lost and the threshold is still not reached ->
				 * reset the failute counter*/
				if (gKpbContext.ASGSupervisionFailureCounter > 0)
				{
					gKpbContext.ASGSupervisionFailureCounter = 0;
				}
			}
			else
			{
				gKpbContext.ASGSupervisionFailureCounter++;

				/* no more supervision retries, threshold reached, proceed KPB reinitializing  */
				if(gKpbContext.ASGSupervisionFailureCounter == KPB_N_ACK_NEW)
				{
					/* change the ASG state to DEAD */
					_KPB_vChangeAsgState(KPB_ASG_STATUS_DEAD);
					
					/* call the alarm to shut down the BAP stack for SD*/
					(void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_ASG_SV_TIMEOUT, 
															MSEC(100), 
															0);
				}
				else
				{/* proceed further supervision retries(given by KPB_N_ACK_NEW), if no ASG status is delivered */

					_KPB_vStartAsgSupervision();
				}
			}
		}
		/* check if ASG is dead, if YES, shut down KPB */
		else if(KPB_enGetAsgState() == KPB_ASG_STATUS_DEAD)
        {
            SuccessFailType BAP_SD_ShutDownState = BAP_SD_ShutDown();

            /* first shut down KPB */
            if ((BAP_SD_ShutDownState == FAIL)&&(MOD_KPB_Protocol_ErrorCounter < 3))
            {
                DEBUG_TEXT(KPB_SWI_TRC, MOD_KPB, "cbAsgSupervision: BAP_Error after BAP_Shut down called, retry after 100 ms");

                /* todo: error handling, handling still not implemented since SOP1 */
                /* what is to do if BAP not shut down, proceed further retries */

                MOD_KPB_Protocol_ErrorCounter++;
				
				/* call the alarm to start again the BAP stack for SD*/
				(void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_ASG_SV_TIMEOUT, 
														MSEC(100), 
														0);
            }
            else if (BAP_SD_ShutDownState == SUCCESS)
            {
                DEBUG_TEXT(KPB_SWI_TRC, MOD_KPB, "cbAsgSupervision: shut down KPB first after ASG failure");

                MOD_KPB_Protocol_ErrorCounter = 0;

                _KPB_vChangeAsgState(KPB_ASG_STATUS_SHUT_DOWN);
				
				/* call the alarm to start again the BAP stack for SD*/
				(void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_ASG_SV_TIMEOUT, 
														MSEC(100), 
														0);
            }
            else if((BAP_SD_ShutDownState == FAIL)&&(MOD_KPB_Protocol_ErrorCounter == 3))
            {
                DEBUG_TEXT(KPB_SWI_TRC, MOD_KPB, "cbAsgSupervision: stop trying to shut down KPB(3 retries proceeded)");

                _KPB_vChangeAsgState(KPB_ASG_STATUS_UNDEFINED);
            }
        }

        /* check if ASG is shut down, if YES, restart KPB */
        else if(KPB_enGetAsgState() == KPB_ASG_STATUS_SHUT_DOWN)
        {
            SuccessFailType BAP_SD_StartState = BAP_SD_Start();

            /* first start KPB */
            if ((BAP_SD_StartState == FAIL)&&(MOD_KPB_Protocol_ErrorCounter < 3))
            {
                DEBUG_TEXT(KPB_SWI_TRC, MOD_KPB, "cbAsgSupervision: BAP_Error after BAP_Start called, retry after 100 ms");

                /* todo: error handling, handling still not implemented since SOP1 */
                /* what is to do if BAP not comes up, proceed further retries  */
                MOD_KPB_Protocol_ErrorCounter++;
				
				/* call the alarm to start again the BAP stack for SD*/
				(void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_ASG_SV_TIMEOUT, 
														MSEC(100), 
														0);
            }
            else if (BAP_SD_StartState == SUCCESS)
            {
                DEBUG_TEXT(KPB_SWI_TRC, MOD_KPB, "cbAsgSupervision: restart KPB after 100 ms");

                MOD_KPB_Protocol_ErrorCounter = 0;

                _KPB_vChangeAsgState(KPB_ASG_STATUS_STARTING);

                /* remove all of the elements in the ack queue */
                BAP_DISP_RemoveAllQueueElements();
            }
            else if((BAP_SD_StartState == FAIL)&&(MOD_KPB_Protocol_ErrorCounter == 3))
            {
                DEBUG_TEXT(KPB_SWI_TRC, MOD_KPB, "cbAsgSupervision: stop trying to start KPB(3 retries proceeded)");

                _KPB_vChangeAsgState(KPB_ASG_STATUS_UNDEFINED);
            }
        }
	}
}

/*----------------------------------------------------------------------------*/
/* Function :   _KPB_sfHandlePendingSendFrameData                             */
/**
  \brief        Handles the sending of KPB FrameData messages

  \return       SuccessFailType
                SUCCESS or FAIL


 */
/*----------------------------------------------------------------------------*/
static SuccessFailType _KPB_sfHandlePendingSendFrameData()
{
    SuccessFailType result = SUCCESS;
    static Uint8Type TryAgainToSetVisibleCounter=0;

    DEBUG_VALUE1(KPB_SWI_TRC, MOD_KPB, "vHPSFData: state %d", gKpbContext.pending.sendFd.state);

    switch(gKpbContext.pending.sendFd.state)
    {
        case KPB_FRAME_DATA_EMPTY_NOT_SENT:
        {
            if(gKpbContext.pending.sendFd.emptyFrameId != KPB_FRAME_ID_INVALID) 
            {
                result = _KPB_sfBapSendFrameDataEmpty( gKpbContext.pending.sendFd.emptyFrameId,
                                                       &(gKpbContext.pending.sendFd.getHeader));
            }
            else
            {

                DEBUG_VALUE1(KPB_SWI_ERR, MOD_KPB, "vHPSFData:no emptyFrameId %d", KPB_FRAME_ID_INVALID);

                result = FAIL;
            }
            
            if( result != SUCCESS )
            {

                DEBUG_VALUE1(KPB_SWI_ERR, MOD_KPB, "vHPSFData: Send failed in state %d", gKpbContext.pending.sendFd.state);

                result = FAIL;
            }
            else
            {
                /* start FrameDataAck timeout */
                _KPB_vStartFrameDataAckTimer();
                gKpbContext.pending.sendFd.state = KPB_FRAME_DATA_EMPTY_SENT;
            }
            break;
        }
        case KPB_FRAME_DATA_EMPTY_SENT:
            /* in this state we are triggered by the FrameDataAckTimer */
            if(gKpbContext.pending.sendFd.retries > KPB_FRAME_DATA_MAX_RETRY)
            {

                DEBUG_VALUE1(KPB_SWI_ERR, MOD_KPB, "vHPSFData: no FrameDataAck after %d retries", gKpbContext.pending.sendFd.retries);

                /* max frame data retry exceeded */
                 _KPB_vChangeAsgState(KPB_ASG_STATUS_DEAD);

                result = FAIL;
            }
            else
            {
                /* resend the empty frame data request */
                if(gKpbContext.pending.sendFd.emptyFrameId != KPB_FRAME_ID_INVALID) 
                {
                    result = _KPB_sfBapSendFrameDataEmpty( gKpbContext.pending.sendFd.emptyFrameId,
                                                           &(gKpbContext.pending.sendFd.getHeader));
                }
                else
                {

                    DEBUG_VALUE1(KPB_SWI_ERR, MOD_KPB, "vHPSFData:no emptyFrameId in state %d", gKpbContext.pending.sendFd.state);

                    result = FAIL;
                }
                
                if( result != SUCCESS )
                {

                    DEBUG_VALUE1(KPB_SWI_ERR, MOD_KPB, "vHPSFData: _KPB_vBapSendFrameData failed in state %d", gKpbContext.pending.sendFd.state);

                    result = FAIL;
                }
                else
                {

                    DEBUG_VALUE2(KPB_SWI_WNG, MOD_KPB, "vHPSFData: retry %d state %d", gKpbContext.pending.sendFd.retries, gKpbContext.pending.sendFd.state);

                    gKpbContext.pending.sendFd.retries++;

                    /* start FrameDataAck timeout */
                    _KPB_vStartFrameDataAckTimer();
                }
            }
            break;
        case KPB_FRAME_DATA_EMPTY_ACK_RECVD:
            /* check if we have to send the second part of our empty frame */
            if(gKpbContext.pending.sendFd.emptyFrameId == KPB_FRAME_ID_INVALID) 
            {

                DEBUG_VALUE1(KPB_SWI_ERR, MOD_KPB, "vHPSFData:no emptyFrameId in state %d", gKpbContext.pending.sendFd.state);

                result = FAIL;
            }
            else
            {
                if( (gKpbContext.pending.sendFd.partToSend == KPB_CELL_PART_1) &&
                    (gKpbContext.pending.sendFd.getHeader.elements > KPB_MAX_CELLS_PER_ARRAY) )
                {
                    /* set the start to the beginning of the second part */
                    gKpbContext.pending.sendFd.partToSend = KPB_CELL_PART_2;
                    gKpbContext.pending.sendFd.getHeader.elements = 
                            gKpbContext.pending.sendFd.getHeader.elements - KPB_MAX_CELLS_PER_ARRAY;

                    result = _KPB_sfBapSendFrameDataEmpty( gKpbContext.pending.sendFd.emptyFrameId,
                                                           &(gKpbContext.pending.sendFd.getHeader));

                    if( result != SUCCESS )
                    {

                        DEBUG_VALUE1(KPB_SWI_ERR, MOD_KPB, "vHPSFData: Send failed in state %d", gKpbContext.pending.sendFd.state);

                        result = FAIL;
                    }
                    else
                    {
                        /* start FrameDataAck timeout */
                        _KPB_vStartFrameDataAckTimer();
                        gKpbContext.pending.sendFd.retries  = 0U;
                        gKpbContext.pending.sendFd.state    = KPB_FRAME_DATA_EMPTY_SENT;
                    }
                }
                else
                {
                    /* Final state reached */
                    _KPB_vReleasePendingContext(__LINE__, BT_TRUE);
                }
            }
            break;
        case KPB_FRAME_DATA_NOT_SENT:

            if(_KPB_u16GetCellsSet(gKpbContext.pending.sendFd.pData) > 0)
            {
                if( _KPB_sfBapSendFrameData(gKpbContext.pending.sendFd.pData) != SUCCESS )
                {

                    DEBUG_VALUE1(KPB_SWI_ERR, MOD_KPB, "vHPSFData: Send failed in state %d", 
                                 gKpbContext.pending.sendFd.state);

                    result = FAIL;
                }
                else
                {
                    /* start FrameDataAck timeout */
                    //_KPB_vStartFrameDataAckTimer();
                    gKpbContext.pending.sendFd.state = KPB_FRAME_DATA_SENDING_IN_PROGRESS;
                    (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_FRAME_DATA_ACK_TIMEOUT, 
                                                               MSEC(10), 
                                                               0);
                }
            }
            else
            {
                DEBUG_VALUE1( KPB_SWI_TRC, MOD_KPB, "vHPSFData: frameId %d, skip sending empty frameData ", 
                              gKpbContext.pending.sendFd.pData->frameId);
                /* nothing to send, go on with processing */
                gKpbContext.pending.sendFd.state = KPB_FRAME_DATA_ACK_RECVD;
                (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_FRAME_DATA_ACK_TIMEOUT, 
                                                           MSEC(10), 
                                                           0);
            }
            break;
            
        case KPB_FRAME_DATA_SENDING_IN_PROGRESS:
        {
            /* in this state we are triggered by the FrameDataAckTimer */
            BAP_DISP_AckQueueCtrl *pCtrl = NULL;
            
            /* check if for function ID Ack is used */
            pCtrl = BAP_DISP_GetAckQueueCtrl(BapLsg_SD, BapFct_SD_FrameData);
            
            if ( pCtrl != NULL )
            {
                /* check if BAP-Stack is pending */ 
                if (TRUE == pCtrl->ack_pending)
                {
                    (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_FRAME_DATA_ACK_TIMEOUT, 
                                                               MSEC(50), 
                                                               0);
                    break;
                }
            }
            gKpbContext.pending.sendFd.state = KPB_FRAME_DATA_SENT;
            /* start FrameDataAck timeout */
            _KPB_vStartFrameDataAckTimer();
            break;
        }
        case KPB_FRAME_DATA_SENT:
            /* in this state we are triggered by the FrameDataAckTimer */
            if(gKpbContext.pending.sendFd.retries > KPB_FRAME_DATA_MAX_RETRY)
            {

                DEBUG_VALUE1(KPB_SWI_ERR, MOD_KPB, "vHPSFData: no FrameDataAck after %d retries", gKpbContext.pending.sendFd.retries);

                /* max frame data retry exceeded */
                 _KPB_vChangeAsgState(KPB_ASG_STATUS_DEAD);

                result = FAIL;
            }
            else
            {
                /* resend the frame data request */
                if( _KPB_sfBapSendFrameData(gKpbContext.pending.sendFd.pData) != SUCCESS )
                {

                    DEBUG_VALUE1(KPB_SWI_ERR, MOD_KPB, "vHPSFData: _KPB_vBapSendFrameData failed in state %d", gKpbContext.pending.sendFd.state);

                    result = FAIL;
                }
                else
                {

                    DEBUG_VALUE1(KPB_SWI_WNG, MOD_KPB, "vHPSFData: retry %d", gKpbContext.pending.sendFd.retries);

                    gKpbContext.pending.sendFd.retries++;

                    /* start FrameDataAck timeout */
                    //_KPB_vStartFrameDataAckTimer();
                    gKpbContext.pending.sendFd.state = KPB_FRAME_DATA_SENDING_IN_PROGRESS;
                    (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_FRAME_DATA_ACK_TIMEOUT, 
                                                               MSEC(10), 
                                                               0);
                }
            }
            break;
        case KPB_FRAME_DATA_ACK_RECVD:
            /* check if we have to send the second part of our frame */
            if(gKpbContext.pending.sendFd.pData == NULL)
            {

                DEBUG_VALUE1(KPB_SWI_ERR, MOD_KPB, "vHPSFData: no pData in state %d", gKpbContext.pending.sendFd.state);
                
                result = FAIL;
            }
            else
            {
                if( (gKpbContext.pending.sendFd.pData->partToSend == KPB_CELL_PART_1) &&
                    (_KPB_u16GetCellsSet(gKpbContext.pending.sendFd.pData) > KPB_MAX_CELLS_PER_ARRAY) )
                {
                    /* set the start to the beginning of the second part */
                    gKpbContext.pending.sendFd.pData->partToSend = KPB_CELL_PART_2;

                    if( _KPB_sfBapSendFrameData(gKpbContext.pending.sendFd.pData) != SUCCESS )
                    {

                        DEBUG_VALUE1(KPB_SWI_ERR, MOD_KPB, "vHPSFData: Send failed in state %d", gKpbContext.pending.sendFd.state);

                        result = FAIL;
                    }
                    else
                    {
                        /* start FrameDataAck timeout */
                        //_KPB_vStartFrameDataAckTimer();
                        gKpbContext.pending.sendFd.retries  = 0U;
                        gKpbContext.pending.sendFd.state    = KPB_FRAME_DATA_SENDING_IN_PROGRESS;
                        (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_FRAME_DATA_ACK_TIMEOUT, 
                                                                   MSEC(10), 
                                                                   0);
                    }
                }
                else
                {
                    //RS: only activation is left, but we can not release the frameData memory here,
                    //    we have to wait for frame activation first, otherwise we do not know
                    //    if to activate or update the next frameData (KPB_sfSendFrameData) 
                    // frameData is now released within ReleasePendingContext
    
                    /* check if an activation request is pending */
                    if( ( gKpbContext.pending.sendFd.activateFrame.id != KPB_FRAME_ID_INVALID &&
                          //RS: make sure the activation is really necessary
                          gKpbContext.pending.sendFd.activateFrame.id != gKpbContext.fs.activeFrameId ) ||
                          //RS: we need this case to consider the context activation triggered by ourself
                        ( gKpbContext.pending.sendFd.activateFrame.id != KPB_FRAME_ID_INVALID &&
                          gKpbContext.fs.isVisible != BT_TRUE ) )
                    {
                        if(   gKpbContext.fs.isVisible == BT_TRUE ||
                            ( gKpbContext.isFsgQueuedUp == BT_TRUE &&
                              gKpbContext.fs.activeFrameId != KPB_FRAME_ID_INVALID &&
                              gKpbContext.pending.sendFd.activateFrame.id != gKpbContext.fs.activeFrameId ) )
                        {
                            /* first switch the old frame off */
                            _KPB_vSendFrameStatusVisOff(&(gKpbContext.fs), __LINE__);
                            gKpbContext.pending.sendFd.state = KPB_FRAME_REQ_V_IS_OFF_SENT;
                        }
                        else
                        {
                            /* we don't need to switch the the old frame off, just switch the new on */
                            gKpbContext.fs.activeFrameId = gKpbContext.pending.sendFd.activateFrame.id;
                            gKpbContext.fs.priority      = gKpbContext.pending.sendFd.activateFrame.priority;

                            gKpbContext.pending.sendFd.activateFrame.id = KPB_FRAME_ID_INVALID;
                            gKpbContext.pending.sendFd.state = KPB_FRAME_REQ_V_IS_ON_SENT;

                            _KPB_vSendFrameStatusVisOn(&(gKpbContext.fs),__LINE__);

                            //RS: don't release the context, we need to wait for the response (RT:2490)
                            ////* Final state reached */
                            //_KPB_vReleasePendingContext(__LINE__, BT_TRUE);
                        }
                    }
                    else
                    {

                        DEBUG_VALUE4(KPB_SWI_TRC, MOD_KPB, "vHPSFData: act skipped %d %d %d", gKpbContext.pending.sendFd.activateFrame.id, gKpbContext.fs.activeFrameId, gKpbContext.fs.isVisible, 0);

                        /* Final state reached */
                        gKpbLastFrameDataSent = *gKpbContext.pending.sendFd.pData;
                        _KPB_vReleasePendingContext(__LINE__, BT_TRUE);
                    }
                }
            }
            break;

        case KPB_FRAME_SET_V_IS_OFF_RECVD:
            /* check if an activation request is pending */
            if( gKpbContext.pending.sendFd.activateFrame.id != KPB_FRAME_ID_INVALID)
            {
                gKpbContext.fs.activeFrameId = gKpbContext.pending.sendFd.activateFrame.id;
                gKpbContext.fs.priority      = gKpbContext.pending.sendFd.activateFrame.priority;

                gKpbContext.pending.sendFd.activateFrame.id = KPB_FRAME_ID_INVALID;
                gKpbContext.pending.sendFd.state = KPB_FRAME_REQ_V_IS_ON_SENT;

                _KPB_vSendFrameStatusVisOn(&(gKpbContext.fs),__LINE__);
        
                //RS: don't release the context, we need to wait for the response (RT:2490)
            }
            else
            {
                /* Final state reached */
                _KPB_vReleasePendingContext(__LINE__, BT_TRUE);
            }

            if(TryAgainToSetVisibleCounter > 0)
                TryAgainToSetVisibleCounter=0;
            break;

        case KPB_FRAME_REQ_V_IS_OFF_REFUSED:
            /* trying to set the visibility again after releasing the context*/
            if((TryAgainToSetVisibleCounter < 2)&&(gKpbContext.fs.activeFrameId != KPB_FRAME_ID_INVALID))
            {
                DEBUG_TEXT(KPB_SWI_TRC, MOD_KPB, "Try to set the visibility again when refused ");

                /* try again to set the VisOff for the frame*/
                _KPB_vSendFrameStatusVisOff(&(gKpbContext.fs),__LINE__);

                /* increase the retry counter*/
                TryAgainToSetVisibleCounter++;

            }
            else
            {
                DEBUG_TEXT(KPB_SWI_TRC, MOD_KPB, "Visibility not set, release the context ");
                /* Final state reached */
                _KPB_vReleasePendingContext(__LINE__, BT_TRUE);
                TryAgainToSetVisibleCounter=0;
            }
            break;

        case KPB_FRAME_SET_V_IS_ON_RECVD:
            /* Final state reached */
            gKpbLastFrameDataSent = *gKpbContext.pending.sendFd.pData;

            _KPB_vReleasePendingContext(__LINE__, BT_TRUE);

            if(TryAgainToSetVisibleCounter > 0)
                TryAgainToSetVisibleCounter=0;
            break;

        case KPB_FRAME_REQ_V_IS_ON_REFUSED:
            /* trying to set the visibility again after releasing the context*/
            if((TryAgainToSetVisibleCounter < 2)&&(gKpbContext.fs.activeFrameId != KPB_FRAME_ID_INVALID))
            {
                DEBUG_TEXT(KPB_SWI_TRC, MOD_KPB, "Try to set the visibility again when refused ");

                /* try again to set the VisOn for the frame*/
                _KPB_vSendFrameStatusVisOn(&(gKpbContext.fs),__LINE__);

                /* increase the retry counter*/
                TryAgainToSetVisibleCounter++;
            }
            else
            {
                DEBUG_TEXT(KPB_SWI_TRC, MOD_KPB, "Visibility not set, release the context ");
                /* Final state reached */
                _KPB_vReleasePendingContext(__LINE__, BT_TRUE);
                TryAgainToSetVisibleCounter=0;
            }
            break;

        case KPB_FRAME_SET_V_IS_OFF_WHILE_FDPENDING: //RT:6492
            /* Final state reached */
            DEBUG_TEXT(KPB_SWI_TRC, MOD_KPB, "KL15 OFF while frame activating, release the context ");

            /* inform the applications with visibility off */
            if( CSHDL_bIsKl15On() == BT_FALSE )
                _KPB_vNotifyApplication(KPB_NOTIFY_VISIBLE_OFF);

            /* release the context */
            _KPB_vReleasePendingContext(__LINE__, BT_TRUE);

            if(TryAgainToSetVisibleCounter > 0)
                TryAgainToSetVisibleCounter=0;
            break;

        case KPB_FRAME_REQ_V_IS_OFF_SENT:
        case KPB_FRAME_REQ_V_IS_ON_SENT:
        {
            /* in case the FrameDataAck is received 50ms later it correlates with the "CTA_FRAME_DATA_ACK_TIMEOUT" that expires
            / 50ms later too, so the second event should not be processed, only break the state machine                       */
            DEBUG_VALUE1(KPB_SWI_ERR, MOD_KPB, "vHPSFData: unexpected state, wait for the ASG FrameStatus now %d", gKpbContext.pending.sendFd.state); //RT:6197
            break;
        }
        default:

            DEBUG_VALUE1(KPB_SWI_ERR, MOD_KPB, "vHPSFData: invalid state %d, send error to application", gKpbContext.pending.sendFd.state);

            result = FAIL;
            break;
    }

    return result;
}

/*----------------------------------------------------------------------------*/
/* Function :   _KPB_sfHandlePendingShutdown                                  */
/**
  \brief        Handles the sending of KPB Shutdown messages

  \return       SuccessFailType
                SUCCESS or FAIL
 */
/*----------------------------------------------------------------------------*/
static SuccessFailType _KPB_sfHandlePendingShutdown()
{
    SuccessFailType result = SUCCESS;

    switch(gKpbContext.pending.shutdown.state)
    {
        case KPB_SHUTDOWN_STATUS_WRITE_PERSISTENT:
        {
            DEBUG_VALUE2(KPB_SWI_TRC, MOD_KPB, "HandlePendingShutdown: EEprom write LastMode %d,%d", 
                         gKpbContext.lastMode.inPresentation, 
                         gKpbContext.lastMode.cradleRestriction);

            (void)EEPROM_sfRamWriteOnly((Uint8Type*)&gKpbContext.lastMode,
                                        EEPROM_LAYOUT_ENUM_KPB_Display_Status);

            /* go on with the next state */
            gKpbContext.pending.shutdown.state = KPB_SHUTDOWN_STATUS_REQ_V_IS_OFF_NOT_SENT;
            (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_KPB_FCT_PENDING,MSEC(0),0);
            break;
        }
        case KPB_SHUTDOWN_STATUS_REQ_V_IS_OFF_NOT_SENT:
            /* skip the switch off of the display if we are not visible */
            if(gKpbContext.fs.isVisible == BT_TRUE)
            {
                KPB_vSendFrameStatusPresReqOff();
                gKpbContext.pending.shutdown.state = KPB_SHUTDOWN_STATUS_REQ_V_IS_OFF_SENT;
            }
            else
            {   
                gKpbContext.pending.shutdown.state = KPB_SHUTDOWN_STATUS_SET_V_IS_OFF_RECVD;
                (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_KPB_FCT_PENDING,MSEC(0),0);
            }
            break;
        case KPB_SHUTDOWN_STATUS_SET_V_IS_OFF_RECVD:
            gKpbContext.opState = KPB_OP_STATE_OFF;
            _KPB_vBapSendOperationState(gKpbContext.opState);
            gKpbContext.pending.shutdown.state = KPB_SHUTDOWN_STATUS_OPSTATE_OFF_SENT;
            /* give the BAP stack a little bit time to send the opstate */
            (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_KPB_FCT_PENDING,MSEC(50),0);
            break;
        case KPB_SHUTDOWN_STATUS_OPSTATE_OFF_SENT:
            /* notify the CSHDL that we have finished shutdown */
            //APP_CSHDL_vCk5050ShutDownHandled(APP_CSHDL_KPB);
            /* Final state reached */
            _KPB_vReleasePendingContext(__LINE__, BT_TRUE);
            break;
        default:

            DEBUG_VALUE1(KPB_SWI_ERR, MOD_KPB, "sfHandlePendingShutdown: invalid state %d", gKpbContext.pending.shutdown.state);

            result = FAIL;
            break;
    }

    return result;
}



/*----------------------------------------------------------------------------*/
/* Function :   _KPB_u16GetCellsSet                                           */
/**
  \brief        Helper function to return the number of used cells

  \param        pFrameData
                KpbFrameDataReqType *

  \return       Uint16Type
                number of used cells

 */
/*----------------------------------------------------------------------------*/
static Uint16Type _KPB_u16GetCellsSet(const KpbFrameDataReqType *pFrameData)
{
    Uint16Type cellsSet = 0U;
    Uint8Type i = 0U;

    if(pFrameData != NULL)
    {
        for(i=0U; i<KPB_MAX_FRAME_CELL_NUMBER; i++)
        {
            if(pFrameData->cell[i].isSet != BT_TRUE)
            {
                continue;
            }

            cellsSet++;
        }
    }

    return cellsSet;
}

/*----------------------------------------------------------------------------*/
/* Function :   _KPB_vHandleKpbIndication                                     */
/**
  \brief        Filter incoming KPB requests and invoke the right handler

  \param        pKpbInd
                KpbIndType *

  \return       void

 */
/*----------------------------------------------------------------------------*/
static void _KPB_vHandleKpbIndication(KpbIndType *pKpbInd)
{

    switch( pKpbInd->common.fctType )
    {
        case KPB_FSG_OPERATION_STATE:
            _KPB_vHandleOperationState(&(pKpbInd->opState));
            break;
        case KPB_ASG_CAPABILITIES:
            _KPB_vHandleAsgCapabilities(&(pKpbInd->asgCap));
            break;
        case KPB_ASG_CONFIG:
            _KPB_vHandleAsgConfig(&(pKpbInd->asgConf));
            break;
        case KPB_LSG_STATUS:
            _KPB_vHandleLsgStatus(&(pKpbInd->lsgStatus));
            break;
        case KPB_MFL_BLOCK_DEFINITION:
            _KPB_vHandleMflBlockDefinition(&(pKpbInd->mflBlockDef));
            break;
        case KPB_FRAME_DATA_ACK:
            _KPB_vHandleFrameDataAck(&(pKpbInd->frameDataAck));
            break;
        case KPB_FRAME_STATUS:
            _KPB_vHandleFrameStatus(&(pKpbInd->frameStatus));
            break;
        case KPB_FRAME_DATA:
            _KPB_vHandleFrameData(&(pKpbInd->frameData));
            break;
        case KPB_SCROLL_BAR:
        default:
            /* Unknown KPB Indication */

            DEBUG_VALUE1(KPB_SWI_ERR, MOD_KPB, "vHandleKpbIndication failed for: %d", pKpbInd->common.fctType);

            break;
    }

    //_KPB_vNotifySdApplication(pKpbInd);
}




/*----------------------------------------------------------------------------*/
/* Function :   _KPB_vHandleOperationState                                    */
/**
  \brief        Handler for KPB OpState

  \param        pOpState
                KpbOpStateType *

  \return       void

 */
/*----------------------------------------------------------------------------*/
static void _KPB_vHandleOperationState(const KpbOpStateType *pOpState)
{
    switch(pOpState->common.opType)
    {
        case KPB_OP_TYPE_GET:
            _KPB_vBapSendOperationState(gKpbContext.opState);
            break;

        default:

            DEBUG_VALUE1(KPB_SWI_ERR, MOD_KPB, "vHandleOperationState failed, invalid opType: %d", pOpState->common.opType);

            break;
    }
}

/*----------------------------------------------------------------------------*/
/* Function :   _KPB_vHandleAsgCapabilities                                   */
/**
  \brief        Handler for KPB AsgCapabilities

  \param        pAsgCap
                KpbAsgCapType *

  \return       void

 */
/*----------------------------------------------------------------------------*/
static void _KPB_vHandleAsgCapabilities(const KpbAsgCapType *pAsgCap)
{

    switch(pAsgCap->common.opType)
    {
        case KPB_OP_TYPE_GET:
            _KPB_vBapSendAsgCapabilities(gKpbContext.displayClass,
                                         gKpbContext.visibleElements);
            break;

        case KPB_OP_TYPE_SET_GET:
            gKpbContext.displayClass    = pAsgCap->dispSize;
            gKpbContext.visibleElements = pAsgCap->visibleElements;

            _KPB_vBapSendAsgCapabilities(gKpbContext.displayClass,
                                         gKpbContext.visibleElements);

            //_KPB_vSetAsgAvailable(BT_TRUE);
            if(BT_FALSE == _KPB_bIsKpbInitDone())
            {
                _KPB_vSetKpbInitState(KPB_ASG_CAPABILITIES);
            }
            else // RT:4813, in case of bus off error and BAP_KPB restart
            {
                /* re-init the states and wait for the start-up sequence */
                gKpbContext.asgInitStateAfterReset.capabilities = 0;
                gKpbContext.asgInitStateAfterReset.config = 0;
                gKpbContext.asgInitStateAfterReset.mflBlockDef = 0;

                /* waiting for the init sequence, see the MSC */
                _KPB_vKpbInitStateAfterProtocolReset(KPB_ASG_CAPABILITIES);
            }
            break;

        default:

            DEBUG_VALUE1(KPB_SWI_ERR, MOD_KPB, "vHandleAsgCapabilities failed, invalid opType: %d", pAsgCap->common.opType);

            break;
    }
}

/*----------------------------------------------------------------------------*/
/* Function :   _KPB_vHandleAsgConfig                                         */
/**
  \brief        Handler for KPB AsgConfig

  \param        pAsgConf
                KpbAsgConfType *

  \return       void

 */
/*----------------------------------------------------------------------------*/
static void _KPB_vHandleAsgConfig(const KpbAsgConfType *pAsgConf)
{

    switch(pAsgConf->common.opType)
    {
        case KPB_OP_TYPE_GET:
            _KPB_vBapSendAsgConfig(gKpbContext.versionNumber);
            break;

        case KPB_OP_TYPE_SET_GET:
            gKpbContext.versionNumber   = pAsgConf->version;
            _KPB_vBapSendAsgConfig(gKpbContext.versionNumber);


            if(BT_FALSE == _KPB_bIsKpbInitDone())
            {
                _KPB_vSetAsgAvailable(BT_TRUE);
                _KPB_vSetKpbInitState(KPB_ASG_CONFIG);
            }
            else // RT:4813, in case of bus off error and BAP_KPB restart
            {
                _KPB_vKpbInitStateAfterProtocolReset(KPB_ASG_CONFIG);
            }
            break;

        default:

            DEBUG_VALUE1(KPB_SWI_ERR, MOD_KPB, "vHandleAsgConfig failed, invalid opType: %d", pAsgConf->common.opType);

            break;
    }
}


/*----------------------------------------------------------------------------*/
/* Function :   _KPB_vHandleLsgStatus                                         */
/**
  \brief        Handler for KPB LsgStatus

  \param        pLsgStatus
                KpbLsgStatusType *

  \return       void

 */
/*----------------------------------------------------------------------------*/
static void _KPB_vHandleLsgStatus(const KpbLsgStatusType *pLsgStatus)
{

    switch(pLsgStatus->common.opType)
    {
        case KPB_OP_TYPE_SET:
            if(pLsgStatus->alive == KPB_LSG_ALIVE_TRUE)
            {
                gKpbContext.isAsgResponseOutstanding = BT_FALSE;
            }
            break;

        default:

            DEBUG_VALUE1(KPB_SWI_ERR, MOD_KPB, "vHandleLsgStatus failed, invalid opType: %d", pLsgStatus->common.opType);

            break;
    }
}


/*----------------------------------------------------------------------------*/
/* Function :   _KPB_vHandleMflBlockDefinition                                */
/**
  \brief        Handler for KPB MflBlockDefinition

  \param        pMflBlockDef
                KpbMflBlockDefType *

  \return       void

 */
/*----------------------------------------------------------------------------*/
static void _KPB_vHandleMflBlockDefinition(const KpbMflBlockDefType *pMflBlockDef)
{
    switch(pMflBlockDef->common.opType)
    {
        case KPB_OP_TYPE_GET:
            _KPB_vBapSendMflBlockDefinition(gKpbContext.mflBlockDef);
            break;
        case KPB_OP_TYPE_SET_GET:
            (void)memcpy(gKpbContext.mflBlockDef, pMflBlockDef->keyBlock, sizeof(gKpbContext.mflBlockDef));
            _KPB_vBapSendMflBlockDefinition(gKpbContext.mflBlockDef);

            if(BT_FALSE == _KPB_bIsKpbInitDone())
            {
                _KPB_vSetKpbInitState(KPB_MFL_BLOCK_DEFINITION);
            }
            else // RT:4813, in case of bus off error and BAP_KPB restart
            {
                _KPB_vKpbInitStateAfterProtocolReset(KPB_MFL_BLOCK_DEFINITION);
            }
            break;

        default:

            DEBUG_VALUE1(KPB_SWI_ERR, MOD_KPB, "vHandleMflBlockDefinition failed, invalid opType: %d", pMflBlockDef->common.opType);

            break;
    }
}



/*----------------------------------------------------------------------------*/
/* Function :   _KPB_vHandleFrameData                                         */
/**
  \brief        Handler for KPB FrameData indication

  \param        pFrameData
                KpbFrameDataIndType *

  \return       void

 */
/*----------------------------------------------------------------------------*/
static void _KPB_vHandleFrameData(KpbFrameDataIndType *pFrameData)
{
    switch(pFrameData->common.opType)
    {
        case KPB_OP_TYPE_GET:
        {
            KpbFrameDataReqType *pFrameBuffer = NULL;
            SuccessFailType result = SUCCESS;
            //SuccessFailType result = POOL_sfAllocate( POOL_enKpbFrameDataReqType, 
            //                                          (void**)&pFrameBuffer );


            result = KPB_sfAllocFrameBuffer( &pFrameBuffer, BT_FALSE );

            if(result == SUCCESS)
            {

                if(pFrameBuffer->frameId != pFrameData->frameId)
                {
                    KpbPending pending;
                    // free the buffer immediately
                    //(void)POOL_sfFree(pFrameBuffer);
                    KPB_sfFreeFrameBuffer();
                    //RS: we have no valid data available, send empty data

                    DEBUG_VALUE2(KPB_SWI_TRC, MOD_KPB, "vHaFrData: Get(1) %d %d", 
                                  pFrameBuffer->frameId, pFrameData->frameId);

                    pending.sendFd.common.id      = KPB_PENDING_SEND_FD;
                    pending.sendFd.emptyFrameId   = pFrameData->frameId;
                    pending.sendFd.getHeader      = pFrameData->arrayHeader;
                    pending.sendFd.pData          = NULL;
                    pending.sendFd.retries        = 0U;
                    pending.sendFd.state          = KPB_FRAME_DATA_EMPTY_NOT_SENT;
                    pending.sendFd.partToSend     = KPB_CELL_PART_1;
                    pending.sendFd.activateFrame.id = KPB_FRAME_ID_INVALID;
                    
                    _KPB_vSetNextJob(&pending);
                }
                else
                {
                    KpbPending pending;

                    //RS: the requested frameId is still available, so prepare to send the data,
                    //    since we have only one frameData, no sendFd should be pending

                    DEBUG_VALUE2(KPB_SWI_TRC, MOD_KPB, "vHaFrData: Get(2) %d %d", 
                                 pFrameBuffer->frameId, pFrameData->frameId);

                    pending.sendFd.common.id      = KPB_PENDING_SEND_FD;
                    pending.sendFd.emptyFrameId   = KPB_FRAME_ID_INVALID;
                    pending.sendFd.pData          = pFrameBuffer;
                    pending.sendFd.retries        = 0U;
                    pending.sendFd.state          = KPB_FRAME_DATA_NOT_SENT;
                    pending.sendFd.partToSend     = KPB_CELL_PART_1;
                    pending.sendFd.activateFrame.id = KPB_FRAME_ID_INVALID;

                    _KPB_vSetNextJob(&pending);
                }
            }
            else
            {
                /* Alloc fails, so someone is trying to send too,
                 * make sure that no waitForProcessing exists */
                if(gKpbContext.waitForProcessing.sendFd.common.id == KPB_PENDING_SEND_FD)
                {

                    DEBUG_VALUE2(KPB_SWI_ERR, MOD_KPB, "vHaFrData: overload, waitForProc blocking (%d %d)", 
                                  pFrameBuffer->frameId, pFrameData->frameId);

                    APP_COMMON_UTILS_vSendBapError( BapLsg_SD, BapFct_SD_FrameData,
                                                    /* RT 1020: "Appl. - out of range" */
                                                    (enum BapError_t) 0x41);
                    
                    return;
                }

                if(gKpbContext.pending.common.id == KPB_PENDING_SEND_FD)
                {
                    if( gKpbContext.pending.sendFd.pData != NULL &&
                        gKpbContext.pending.sendFd.pData->frameId == pFrameData->frameId)
                    {
                        //ignore the request, since the same frameId is going to be sent out 

                    DEBUG_VALUE2(KPB_SWI_TRC, MOD_KPB, "vHaFrData: Get(3) %d %d ignored", 
                                  gKpbContext.pending.sendFd.pData->frameId, pFrameData->frameId);

                    }
                    else
                    {
                        KpbPending pending;
                        //RS: we have no valid data available, send empty data

                        DEBUG_VALUE1(KPB_SWI_TRC, MOD_KPB, "vHaFrData: Get(4) %d", pFrameData->frameId);

                        pending.sendFd.common.id      = KPB_PENDING_SEND_FD;
                        pending.sendFd.emptyFrameId   = pFrameData->frameId;
                        pending.sendFd.getHeader      = pFrameData->arrayHeader;
                        pending.sendFd.pData          = NULL;
                        pending.sendFd.retries        = 0U;
                        pending.sendFd.state          = KPB_FRAME_DATA_EMPTY_NOT_SENT;
                        pending.sendFd.partToSend     = KPB_CELL_PART_1;
                        pending.sendFd.activateFrame.id = KPB_FRAME_ID_INVALID;
                        
                        _KPB_vSetNextJob(&pending);
                    }
                }
                else
                {
                    KpbPending pending;
                    // no sendFd is pending and no frameBuffer is available, so it seems that the next
                    // frame is just build up, so since we don't know what the next will be we send
                    // out an empty frame

                    DEBUG_VALUE1(KPB_SWI_TRC, MOD_KPB, "vHaFrData: Get(5) %d", pFrameData->frameId);

                    pending.sendFd.common.id      = KPB_PENDING_SEND_FD;
                    pending.sendFd.emptyFrameId   = pFrameData->frameId;
                    pending.sendFd.getHeader      = pFrameData->arrayHeader;
                    pending.sendFd.pData          = NULL;
                    pending.sendFd.retries        = 0U;
                    pending.sendFd.state          = KPB_FRAME_DATA_EMPTY_NOT_SENT;
                    pending.sendFd.partToSend     = KPB_CELL_PART_1;
                    pending.sendFd.activateFrame.id = KPB_FRAME_ID_INVALID;
                    
                    _KPB_vSetNextJob(&pending);
                }
            }
            break;
        }
        default:

            DEBUG_VALUE1(KPB_SWI_ERR, MOD_KPB, "_KPB_vHandleFrameData: unsup opType: %d", pFrameData->common.opType);

            break;
    }
}

/*----------------------------------------------------------------------------*/
/* Function :   _KPB_vHandleFrameStatus                                       */
/**
  \brief        Handler for KPB FrameStatus

  \param        pFrameStatus
                KpbFrameStatusType *

  \return       void

 */
/*----------------------------------------------------------------------------*/
static void _KPB_vHandleFrameStatus(KpbFrameStatusType *pFrameStatus)
{
    switch(pFrameStatus->common.opType)
    {
        case KPB_OP_TYPE_GET:
        {
            KpbFrameStatusType frameStatusRsp;

            if(gKpbContext.fs.activeFrameId == KPB_FRAME_ID_INVALID)
            {
                frameStatusRsp.fsgRequest = KPB_FSG_REQ_V_IS_OFF;
            }
            else
            {
                frameStatusRsp.fsgRequest = KPB_FSG_REQ_V_IS_ON;
            }

            frameStatusRsp.asgResponse  = KPB_ASG_NO_RESPONSE;

            _KPB_vBapSendFrameStatus(&frameStatusRsp);
            break;
        }
        default:
            /* reset always the queuedUp flag, in the case of a new queued up, 
             * we set it back in the handler */
            //gKpbContext.isFsgQueuedUp = BT_FALSE;
            switch(gKpbContext.pending.common.id)
            {
                case KPB_PENDING_SEND_FD:
                    _KPB_vHandleFrameStatus_SendFd(pFrameStatus);
                    break;
                case KPB_PENDING_SHUTDOWN:
                    _KPB_vHandleFrameStatus_Shutdown(pFrameStatus);
                    break;
                default:
                    _KPB_vHandleFrameStatus_Default(pFrameStatus);
                    break;
            }
            break;
    }
}

/*----------------------------------------------------------------------------*/
/* Function :   _KPB_vHandleFrameStatus_Default                               */
/**
  \brief        Default handler for KPB FrameStatus responses

  \param        pFrameStatus
                KpbFrameStatusType *

  \return       void

 */
/*----------------------------------------------------------------------------*/
static void _KPB_vHandleFrameStatus_Default(const KpbFrameStatusType *pFrameStatus)
{
    KpbFrameStatusType frameStatusRsp;
    
    /* set frameId to invalid */
    frameStatusRsp.frameId = KPB_FRAME_ID_INVALID;


    DEBUG_VALUE2(KPB_SWI_TRC, MOD_KPB, "vHFS_Def: opType %d, asgRsp %d", pFrameStatus->common.opType, pFrameStatus->asgResponse);

    switch(pFrameStatus->common.opType)
    {
        case KPB_OP_TYPE_SET_GET:
            switch(pFrameStatus->asgResponse)
            {
                case KPB_ASG_SET_V_IS_OFF:
                    frameStatusRsp.fsgRequest   = KPB_FSG_REQ_V_IS_OFF;
                    if(pFrameStatus->frameId != KPB_FRAME_ID_INVALID)  //RT: 4727(only change the visibility if ID valid)
                    {
                        _KPB_vSetAsgVisibility(BT_FALSE, __LINE__);
                    }
                    else
                    {
                        DEBUG_TEXT(KPB_SWI_WNG, MOD_KPB, "vHFS_Def: No frame being shown to set VisOff, assign only MFL blocks, ");
                    }
                    //RS: assign frame id 0xFF, to handle an unsubscribe of all frames
                    if(pFrameStatus->frameId == (Uint8Type)KPB_FRAME_ID_NOT_AVAIL)
                    {
                        frameStatusRsp.frameId = (Uint8Type)KPB_FRAME_ID_NOT_AVAIL;
                        /* inform the applications with visibility off RT:6492*/
                        if( CSHDL_bIsKl15On() == BT_FALSE )
                            _KPB_vNotifyApplication(KPB_NOTIFY_VISIBLE_OFF);
                        /* is status is queued - up and all frames are released, set back the flag*/
                        if(KPB_bIsFsgQueuedUp())
                            gKpbContext.isFsgQueuedUp = BT_FALSE;
                    }
                    break;

                case KPB_ASG_SET_V_IS_ON:
                    if((Uint8Type)gKpbContext.fs.activeFrameId != pFrameStatus->frameId)
                    {

                        DEBUG_VALUE2(KPB_SWI_ERR, MOD_KPB, "vHFS_Def: SET_V_IS_ON rcvd frameId %d != %d", pFrameStatus->frameId, gKpbContext.fs.activeFrameId);

                        /* RT #1023:
                        send the end of presentation if the frameID from the ASG 
                        not matched to the actual frameID in the FSG  */
                        _KPB_vSendFrameStatusIntPresReqOff(pFrameStatus);
                        return;
                    }

                    frameStatusRsp.fsgRequest   = KPB_FSG_REQ_V_IS_ON;
                    _KPB_vSetAsgVisibility(BT_TRUE, __LINE__);
                    
                    if(KPB_bIsFsgQueuedUp())
                        gKpbContext.isFsgQueuedUp = BT_FALSE;

                    break;

                case KPB_ASG_REQ_V_IS_ON:
                    
                    /* RT #1021:
                       If the ASG requests the FSG to present a specific frame (valid frame-ID) 
                       during an active visualisation, the FSG shall refuse the ASG request! */
                    if( ( gKpbContext.fs.activeFrameId == KPB_FRAME_ID_INVALID ) ||
                        ( gKpbContext.fs.isVisible == BT_TRUE &&
                          (Uint8Type)gKpbContext.fs.activeFrameId != pFrameStatus->frameId) )
                    {
                        frameStatusRsp.fsgRequest = KPB_FSG_REFUSE_ASG_REQ;

                        DEBUG_VALUE4(KPB_SWI_WNG, MOD_KPB, "vHFS_Default: REFUSE (%d,%d,%d)", gKpbContext.fs.activeFrameId, gKpbContext.fs.isVisible, pFrameStatus->frameId, 0);

                    }
                    else
                    {
                        frameStatusRsp.fsgRequest = KPB_FSG_ACCEPT_ASG_REQ;
					    _KPB_vSetAsgVisibility(BT_TRUE, __LINE__);
					    if(gKpbContext.pending.common.id == KPB_PENDING_SEND_FD) // RT #4727
                        {    
                                        /* check if an activation request is pending */
                            if( gKpbContext.pending.sendFd.activateFrame.id != KPB_FRAME_ID_INVALID)
                            {/* set the active frame to this one being sent RT: #8536 */
					            gKpbContext.fs.activeFrameId = gKpbContext.pending.sendFd.activateFrame.id;
                                DEBUG_TEXT(KPB_SWI_TRC, MOD_KPB, "ASG_REQ_V_IS_ON activateFrame.id == KPB_FRAME_ID  VALID");
                            }
                            else
                            {
                                DEBUG_TEXT(KPB_SWI_TRC, MOD_KPB, "ASG_REQ_V_IS_ON activateFrame.id == KPB_FRAME_ID  INVALID");
                            }
					        /* additionally inform APP_SD that the visibility is changed by the ASG while frame data pending*/
					        APP_SD_vOnKpbNotification(KPB_NOTIFY_VISIBLE_ON_BY_ASG);
                        }
					}
                    break;

                case KPB_ASG_QUEUED_UP:
                    frameStatusRsp.fsgRequest = KPB_FSG_REQ_V_IS_ON;
                    gKpbContext.isFsgQueuedUp = BT_TRUE;
                    _KPB_vSetAsgVisibility(BT_FALSE, __LINE__);
                    break;

                case KPB_ASG_NO_RESPONSE:
                default:

                    DEBUG_VALUE1(KPB_SWI_ERR, MOD_KPB, "vHFS_Default: asgResponse %d not handled", pFrameStatus->common.opType);

                    return;
            }

            gKpbContext.fs.mflAssigned.mflBlocks = pFrameStatus->mflAssigned.mflBlocks;
            if(gKpbContext.fs.mflAssigned.mflBlocks.rawByte == 0)
            {
                /* there are no buttons assigned */
                gKpbContext.fs.mflAssigned.focussedFrameId = 0;
            }
            else
            {
                /* there are buttons assigned, set the according frameId */
                gKpbContext.fs.mflAssigned.focussedFrameId = (Uint8Type)gKpbContext.fs.activeFrameId;
            }
            frameStatusRsp.asgResponse  = pFrameStatus->asgResponse;
            break;

        default:

            DEBUG_VALUE1(KPB_SWI_ERR, MOD_KPB, "vHFS_Default: failed, invalid opType: %d", pFrameStatus->common.opType);

            return;
    }

    
    /* if the frameId is not set already, set it to the active frameId (default)*/    
    frameStatusRsp.frameId = (frameStatusRsp.frameId == KPB_FRAME_ID_INVALID) ? 
                                    (Uint8Type)gKpbContext.fs.activeFrameId : frameStatusRsp.frameId;
    frameStatusRsp.relPrio = gKpbContext.fs.priority;
    frameStatusRsp.mflAssigned.mflBlocks       = gKpbContext.fs.mflAssigned.mflBlocks;
    frameStatusRsp.mflAssigned.focussedFrameId = (Uint8Type)gKpbContext.fs.mflAssigned.focussedFrameId;

    _KPB_vBapSendFrameStatus(&frameStatusRsp);
}


/*----------------------------------------------------------------------------*/
/* Function :   _KPB_vHandleFrameStatus_SendFd                                */
/**
  \brief        Specific handler for KPB FrameStatus
  \brief        Description:\n
                Handles the FrameStatus responses after a FrameData message was sent

  \param        pFrameStatus
                KpbFrameStatusType *

  \return       void

 */
/*----------------------------------------------------------------------------*/
static void _KPB_vHandleFrameStatus_SendFd(KpbFrameStatusType *pFrameStatus)
{
    //KpbFrameStatusType frameStatusRsp;


    DEBUG_VALUE2(KPB_SWI_TRC, MOD_KPB, "vHFS_SendFd: opType %d, asgRsp %d", pFrameStatus->common.opType, pFrameStatus->asgResponse);

    switch(pFrameStatus->common.opType)
    {
        case KPB_OP_TYPE_SET_GET:
            switch(pFrameStatus->asgResponse)
            {
                case KPB_ASG_SET_V_IS_OFF:
                {
                    /* check if the KPB_ASG_SET_V_IS_OFF was triggered by ourself */
                    switch(gKpbContext.pending.sendFd.state)
                    {
                        case KPB_FRAME_REQ_V_IS_OFF_SENT:
                        case KPB_FRAME_REQ_V_IS_OFF_REFUSED:
                            DEBUG_VALUE2(KPB_SWI_TRC, MOD_KPB, "vHFS_SendFd: RecvFrameID %d, isQueuedUp %d", pFrameStatus->frameId, KPB_bIsFsgQueuedUp());
                            DEBUG_VALUE1(KPB_SWI_TRC, MOD_KPB, "vHFS_SendFd: CurrFrameID %d", gKpbContext.fs.activeFrameId);
                            
                            if((((Uint8Type)gKpbContext.fs.activeFrameId == pFrameStatus->frameId) && (pFrameStatus->frameId != KPB_FRAME_ID_NOT_AVAIL)) || 
                              ((pFrameStatus->frameId == KPB_FRAME_ID_NOT_AVAIL) && (KPB_bIsFsgQueuedUp())))
                            {
                                gKpbContext.pending.sendFd.state = KPB_FRAME_SET_V_IS_OFF_RECVD;
                                (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_KPB_FCT_PENDING,MSEC(0),0);
                            }
                            else if((pFrameStatus->frameId == KPB_FRAME_ID_NOT_AVAIL) && (!KPB_bIsFsgQueuedUp()))//RT:6492
                            {
                                gKpbContext.pending.sendFd.state = KPB_FRAME_SET_V_IS_OFF_WHILE_FDPENDING;
                                (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_KPB_FCT_PENDING,MSEC(0),0);
                            }
                            else
                            {
                                DEBUG_VALUE2(KPB_SWI_TRC, MOD_KPB, "vHFS_SendFd: Unexpected state, SentFrID %d, RecvdFrID %d", (Uint8Type)gKpbContext.fs.activeFrameId, pFrameStatus->frameId);
                                (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_KPB_FCT_PENDING,MSEC(0),0);
                            }
                            break;
                        case KPB_FRAME_REQ_V_IS_ON_SENT:
                        case KPB_FRAME_REQ_V_IS_ON_REFUSED:
                            DEBUG_VALUE2(KPB_SWI_TRC, MOD_KPB, "vHFS_SendFd: RecvFrameID %d, isQueuedUp %d", pFrameStatus->frameId, KPB_bIsFsgQueuedUp());
                            DEBUG_VALUE1(KPB_SWI_TRC, MOD_KPB, "vHFS_SendFd: CurrFrameID %d", gKpbContext.fs.activeFrameId);
                            if((((Uint8Type)gKpbContext.fs.activeFrameId == pFrameStatus->frameId) && (pFrameStatus->frameId != KPB_FRAME_ID_NOT_AVAIL)) || 
                              ((pFrameStatus->frameId == KPB_FRAME_ID_NOT_AVAIL) && (KPB_bIsFsgQueuedUp())))
                            {

                                DEBUG_TEXT(KPB_SWI_WNG, MOD_KPB, "vHFS_SendFd: KPB_FRAME_REQ_V_IS_ON_SENT (refused)");

                                gKpbContext.pending.sendFd.state = KPB_FRAME_REQ_V_IS_ON_REFUSED;
                                (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_KPB_FCT_PENDING,MSEC(200),0);
                            }
                            else if((pFrameStatus->frameId == KPB_FRAME_ID_NOT_AVAIL) && (!KPB_bIsFsgQueuedUp()))//RT:6492
                            {
                                gKpbContext.pending.sendFd.state = KPB_FRAME_SET_V_IS_OFF_WHILE_FDPENDING;
                                (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_KPB_FCT_PENDING,MSEC(0),0);
                            }
                            else
                            {
                                DEBUG_VALUE2(KPB_SWI_TRC, MOD_KPB, "vHFS_SendFd: Unexpected state, SentFrID %d, RecvdFrID %d", (Uint8Type)gKpbContext.fs.activeFrameId, pFrameStatus->frameId);
                                (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_KPB_FCT_PENDING,MSEC(0),0);
                            }
                            break;
                        default:

                            DEBUG_VALUE1(KPB_SWI_WNG, MOD_KPB, "vHFS_SendFd: KPB_ASG_SET_V_IS_OFF, state %d not changed", gKpbContext.pending.sendFd.state);

                            break;
                    }

                    _KPB_vHandleFrameStatus_Default(pFrameStatus);
                    break;
                }
                case KPB_ASG_SET_V_IS_ON:
                    /* check if the KPB_ASG_SET_V_IS_ON was triggered by ourself */
                    if(( gKpbContext.pending.sendFd.state == KPB_FRAME_REQ_V_IS_ON_SENT )||( gKpbContext.pending.sendFd.state == KPB_FRAME_REQ_V_IS_ON_REFUSED ))
                    {
                        gKpbContext.pending.sendFd.state = KPB_FRAME_SET_V_IS_ON_RECVD;
                        (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_KPB_FCT_PENDING,MSEC(0),0);
                    }
                    else if ((gKpbContext.pending.sendFd.state == KPB_FRAME_REQ_V_IS_OFF_SENT)||(gKpbContext.pending.sendFd.state == KPB_FRAME_REQ_V_IS_OFF_REFUSED))
                    {
                        gKpbContext.pending.sendFd.state = KPB_FRAME_REQ_V_IS_OFF_REFUSED;
                        (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_KPB_FCT_PENDING,MSEC(200),0);
                        DEBUG_TEXT(KPB_SWI_WNG, MOD_KPB, "vHFS_SendFd: KPB_FRAME_REQ_V_IS_OFF_SENT (refused)");
                    }
                    else if ( KPB_bIsFsgQueuedUp()&&
                            ((gKpbContext.pending.sendFd.state == KPB_FRAME_DATA_NOT_SENT)||
                            (gKpbContext.pending.sendFd.state == KPB_FRAME_DATA_SENDING_IN_PROGRESS) ||
                            (gKpbContext.pending.sendFd.state == KPB_FRAME_DATA_SENT)))
                    {   /* RT:12260, set the frame to be activated if the previous state was queued-up
                            needed to avoid blind controlling(one screen is shown, another is controlled over MFL)*/
                        gKpbContext.pending.sendFd.activateFrame.id = gKpbContext.pending.sendFd.frameId;
                    }
                    else
                    {
                        DEBUG_VALUE1(KPB_SWI_WNG, MOD_KPB, "vHFS_SendFd: KPB_ASG_SET_V_IS_ON, state %d not changed", gKpbContext.pending.sendFd.state);
                    }
                    _KPB_vHandleFrameStatus_Default(pFrameStatus);
                    break;
                case KPB_ASG_QUEUED_UP:
                    if( gKpbContext.pending.sendFd.state == KPB_FRAME_REQ_V_IS_ON_SENT &&
                        gKpbContext.fs.activeFrameId == pFrameStatus->frameId )
                    {

                         DEBUG_TEXT(KPB_SWI_WNG, MOD_KPB, "vHFS_SendFd: KPB_FRAME_REQ_V_IS_ON_SENT (FSG is queued up)");

                         gKpbContext.pending.sendFd.state = KPB_FRAME_SET_V_IS_ON_RECVD;
                         (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_KPB_FCT_PENDING,MSEC(0),0);
                    }
                    else if( gKpbContext.pending.sendFd.state == KPB_FRAME_REQ_V_IS_OFF_SENT &&
                            gKpbContext.fs.activeFrameId == pFrameStatus->frameId )
                    {

                         DEBUG_TEXT(KPB_SWI_WNG, MOD_KPB, "vHFS_SendFd: KPB_FRAME_REQ_V_IS_OFF_SENT (FSG is queued up)");

                         gKpbContext.pending.sendFd.state = KPB_FRAME_SET_V_IS_OFF_RECVD;
                         (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_KPB_FCT_PENDING,MSEC(0),0);
                    }

                    _KPB_vHandleFrameStatus_Default(pFrameStatus);
                    break;
                default:
                {

                    DEBUG_VALUE1(KPB_SWI_TRC, MOD_KPB, "vHFS_SendFd: asgResp %d (default handl)", pFrameStatus->asgResponse);

                    _KPB_vHandleFrameStatus_Default(pFrameStatus);
                    break;
                }
            }
            break;

        default:

            DEBUG_VALUE1(KPB_SWI_ERR, MOD_KPB, "vHFS_SendFd: failed, invalid opType: %d", pFrameStatus->common.opType);

            break;
    }
}


/*----------------------------------------------------------------------------*/
/* Function :   _KPB_vHandleFrameStatus_Shutdown                              */
/**
  \brief        Specific handler for KPB FrameStatus
  \brief        Description:\n
                Handles the FrameStatus responses after a Shutdown message was sent

  \param        pFrameStatus
                KpbFrameStatusType *

  \return       void

 */
/*----------------------------------------------------------------------------*/
static void _KPB_vHandleFrameStatus_Shutdown(KpbFrameStatusType *pFrameStatus)
{
    KpbFrameStatusType frameStatusRsp;

    switch(pFrameStatus->common.opType)
    {
        case KPB_OP_TYPE_SET_GET:
            if( pFrameStatus->asgResponse == KPB_ASG_SET_V_IS_OFF)
            {
                if( gKpbContext.pending.shutdown.state != KPB_SHUTDOWN_STATUS_REQ_V_IS_OFF_SENT )
                {

                    DEBUG_VALUE1(KPB_SWI_ERR, MOD_KPB, "vHFS_Shutdown: invalid state %d", gKpbContext.pending.shutdown.state);
                }

                gKpbContext.pending.shutdown.state = KPB_SHUTDOWN_STATUS_SET_V_IS_OFF_RECVD;
                
                /* go on with processing the next state after some msec */
                (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_KPB_FCT_PENDING,MSEC(0),0);

                frameStatusRsp.fsgRequest   = KPB_FSG_REQ_V_IS_OFF;
                _KPB_vSetAsgVisibility(BT_FALSE, __LINE__);
#if 0
                //RS: #3843 assign frame id 0xFF, to handle an unsubscribe of all frames
                if(pFrameStatus->frameId == (Uint8Type)KPB_FRAME_ID_NOT_AVAIL)
                {
                    gKpbContext.fs.activeFrameId = KPB_FRAME_ID_NOT_AVAIL;
                }
#endif
                gKpbContext.fs.mflAssigned.mflBlocks = pFrameStatus->mflAssigned.mflBlocks;

                if(gKpbContext.fs.mflAssigned.mflBlocks.rawByte == 0)
                {
                    /* there are no buttons assigned */
                    gKpbContext.fs.mflAssigned.focussedFrameId = 0;
                }
                else
                {
                    /* there are buttons assigned, set the according frameId */
                    gKpbContext.fs.mflAssigned.focussedFrameId = (Uint8Type)gKpbContext.fs.activeFrameId;
                }

                frameStatusRsp.asgResponse  = pFrameStatus->asgResponse;
            }
            else
            {

                DEBUG_VALUE1(KPB_SWI_DBG, MOD_KPB, "vHFS_Shutdown: asgResp %d (default handl)", pFrameStatus->asgResponse);

                _KPB_vHandleFrameStatus_Default(pFrameStatus);
                return;
            }
            break;

        default:

            DEBUG_VALUE1(KPB_SWI_ERR, MOD_KPB, "vHFS_Shutdown: failed, invalid opType: %d", pFrameStatus->common.opType);

            return;
    }

    frameStatusRsp.frameId          = (Uint8Type)gKpbContext.fs.activeFrameId;
    frameStatusRsp.relPrio          = gKpbContext.fs.priority;
    frameStatusRsp.mflAssigned.mflBlocks       = gKpbContext.fs.mflAssigned.mflBlocks;
    frameStatusRsp.mflAssigned.focussedFrameId = (Uint8Type)gKpbContext.fs.mflAssigned.focussedFrameId;

    _KPB_vBapSendFrameStatus(&frameStatusRsp);
}


/*----------------------------------------------------------------------------*/
/* Function :   _KPB_vHandleFrameDataAck                                      */
/**
  \brief        Handler for KPB FrameDataAck

  \param        pFrameDataAck
                KpbFrameDataAckType *

  \return       void

 */
/*----------------------------------------------------------------------------*/
static void _KPB_vHandleFrameDataAck(const KpbFrameDataAckType *pFrameDataAck)
{
    switch(gKpbContext.pending.common.id)
    {
        case KPB_PENDING_SEND_FD:
            _KPB_vHandleFrameDataAck_SendFd(pFrameDataAck);
            break;
        default:

            DEBUG_VALUE1(KPB_SWI_ERR, MOD_KPB, "_KPB_vHandleFrameDataAck: no handling avail. (%d)", gKpbContext.pending.common.id);

            break;
    }

}

/*----------------------------------------------------------------------------*/
/* Function :   _KPB_vHandleFrameDataAck_SendFd                               */
/**
  \brief        Specific handler for KPB FrameDataAck

  \param        pFrameDataAck
                KpbFrameDataAckType *

  \return       void

 */
/*----------------------------------------------------------------------------*/
static void _KPB_vHandleFrameDataAck_SendFd(const KpbFrameDataAckType *pFrameDataAck)
{
    switch(pFrameDataAck->common.opType)
    {
        case KPB_OP_TYPE_SET:
            /* value is valid */
            break;

        default:

            DEBUG_VALUE1(KPB_SWI_ERR, MOD_KPB, "vHandleFrameDataAck failed, invalid opType: %d", pFrameDataAck->common.opType);

            return;
    }

    switch(gKpbContext.pending.sendFd.state)
    {
        case KPB_FRAME_DATA_SENT:
        case KPB_FRAME_DATA_SENDING_IN_PROGRESS:
            if(gKpbContext.pending.sendFd.pData == NULL)
            {

                DEBUG_TEXT(KPB_SWI_ERR, MOD_KPB, "vHandleFrameDataAck missing FrameData");

                return;
            }
            
            /* compare the received with the expected FrameId */
            if(pFrameDataAck->frameId != (Uint8Type)gKpbContext.pending.sendFd.pData->frameId)
            {

                DEBUG_VALUE2(KPB_SWI_ERR, MOD_KPB, "vHandleFrameDataAck expected FrameId: %d received: %d", gKpbContext.pending.sendFd.pData->frameId, pFrameDataAck->frameId);

                return;
            }
            
            gKpbContext.pending.sendFd.state = KPB_FRAME_DATA_ACK_RECVD;
            break;
        case KPB_FRAME_DATA_EMPTY_SENT:
            if(gKpbContext.pending.sendFd.emptyFrameId == KPB_FRAME_ID_INVALID)
            {

                DEBUG_TEXT(KPB_SWI_ERR, MOD_KPB, "vHandleFrameDataAck: missing emptyFrameId");

                return;
            }
            
            /* compare the received with the expected FrameId */
            if(pFrameDataAck->frameId != (Uint8Type)gKpbContext.pending.sendFd.emptyFrameId)
            {

                DEBUG_VALUE2(KPB_SWI_ERR, MOD_KPB, "vHandleFrameDataAck: exp emptyFrameId: %d rcvd: %d", gKpbContext.pending.sendFd.emptyFrameId, pFrameDataAck->frameId);

                return;
            }
            
            gKpbContext.pending.sendFd.state = KPB_FRAME_DATA_EMPTY_ACK_RECVD;
            break;
        default:

            DEBUG_VALUE1(KPB_SWI_ERR, MOD_KPB, "vHandleFrameDataAck: invalid sendFd.state %d", gKpbContext.pending.sendFd.state);

            return;
    }

    /* everything looks fine, we received an valid ack for the last FrameData */
    _KPB_vStopFrameDataAckTimer();
    /* go on with processing the next state */
    (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_KPB_FCT_PENDING,MSEC(0),0);
}


/*----------------------------------------------------------------------------*/
/* Function :   _KPB_vChangeAsgState                                          */
/**
  \brief        Sets the internal ASG state

  \param        asgState
                KPB_enAsgStatus

  \return       void

 */
/*----------------------------------------------------------------------------*/
void _KPB_vChangeAsgState(KPB_enAsgStatus asgState)
{
    gKpbContext.asgState = asgState;

    DEBUG_VALUE1(KPB_SWI_TRC, MOD_KPB, "_KPB_vChangeAsgState: asgState.state = %d", asgState);
}

/*----------------------------------------------------------------------------*/
/* Function :   _KPB_vSetAsgAvailable                                         */
/**
  \brief        This method is called to update the ASG init state

  \param        KPB_enAsgInitPart
                initPart

  \return       void

 */
/*----------------------------------------------------------------------------*/
void _KPB_vSetKpbInitState(KPB_enFunctionIdType initPart)
{
    switch(initPart)
    {
        case KPB_ASG_CAPABILITIES:
            gKpbContext.asgInitState.capabilities = 1;
            break;
        case KPB_ASG_CONFIG:
            gKpbContext.asgInitState.config = 1;
            break;
        case KPB_MFL_BLOCK_DEFINITION:
            gKpbContext.asgInitState.mflBlockDef = 1;
            break;
        default:
            /* do nothing */
            break;
    }

    if( gKpbContext.asgInitState.capabilities &&
        gKpbContext.asgInitState.config &&
        gKpbContext.asgInitState.mflBlockDef )
    {
        /* start the supervision after 500 ms in case to avoid inconsistence with KPB spec */
        AppTask_vSetAlarm(APP_TASK_ALARM_KPB_STARTASGSUPERVISIONDELAYTIME,
                                      MSEC(400),
                                      _KPB_cbStartASGSupervision);
        _KPB_vChangeAsgState(KPB_ASG_STATUS_ALIVE);

       if(gKpbContext.lastMode.inPresentation == LAST_MODE_ON)
       {
           _KPB_vNotifyApplication(KPB_NOTIFY_ASG_INIT_DONE_ACTIVE);
       }
       else
       {
           _KPB_vNotifyApplication(KPB_NOTIFY_ASG_INIT_DONE);
       }
    }
}

/*----------------------------------------------------------------------------*/
/* Function :   _KPB_vKpbInitStateAfterProtocolReset                                         */
/**
  \brief        This method is called to update the ASG init state

  \param        KPB_enAsgInitPart
                initPart

  \return       void

 */
/*----------------------------------------------------------------------------*/
void _KPB_vKpbInitStateAfterProtocolReset(KPB_enFunctionIdType initPart)
{
    switch(initPart)
    {
        case KPB_ASG_CAPABILITIES:
            gKpbContext.asgInitStateAfterReset.capabilities = 1;
            break;
        case KPB_ASG_CONFIG:
            gKpbContext.asgInitStateAfterReset.config = 1;
            break;
        case KPB_MFL_BLOCK_DEFINITION:
            gKpbContext.asgInitStateAfterReset.mflBlockDef = 1;
            break;
        default:
            /* do nothing */
            break;
    }

    if( gKpbContext.asgInitStateAfterReset.capabilities &&
        gKpbContext.asgInitStateAfterReset.config &&
        gKpbContext.asgInitStateAfterReset.mflBlockDef )
    {
        AppTask_vSetAlarm(APP_TASK_ALARM_KPB_STARTASGSUPERVISIONDELAYTIME,
                                      MSEC(400),
                                      _KPB_cbStartASGSupervision);
        _KPB_vChangeAsgState(KPB_ASG_STATUS_ALIVE);

        KPB_vSetAsgPresentationOff();                                               //set the visibility
        (void)memset(&gKpbObservedCellsHash, 0, sizeof(gKpbObservedCellsHash));     // reset the hash table
        (void)APP_COMMON_UTILS_stCancelOsekRelAlarm(CTA_KPB_DISPLAY_STATUS_OFF);    // stop the alarm

        if(gKpbContext.pending.common.id == KPB_PENDING_SEND_FD)
        {
            gKpbContext.pending.sendFd.state = KPB_FRAME_SET_V_IS_ON_RECVD;
            (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_KPB_FCT_PENDING,MSEC(0),0);
        }
        _KPB_vNotifyApplication(KPB_NOTIFY_BUSOFF_END);
    }
}

/*----------------------------------------------------------------------------*/
/* Function :   _KPB_vSetAsgAvailable                                         */
/**
  \brief        This method is called to signalize whether a ASG is available or not

  \param        isAvail
                BooleanType

  \return       void

 */
/*----------------------------------------------------------------------------*/
void _KPB_vSetAsgAvailable(BooleanType isAvail)
{

    DEBUG_VALUE1(KPB_SWI_TRC, MOD_KPB, "vSetAsgAvailable: ASG is %d", isAvail);

    gKpbContext.asgAvailable = isAvail;
    //gKpbContext.opState = KPB_OP_STATE_NORMAL_OPERATION;

    if( isAvail )
    {
        _KPB_vNotifyApplication(KPB_NOTIFY_ASG_AVAILABLE);
    }

    VCR_vInstrumentClusterAvailable(gKpbContext.asgAvailable);
}




/*----------------------------------------------------------------------------*/
/* Function :   _KPB_vSendFrameStatusVisOff                                   */
/**
  \brief        Invoke the sending of KPB FrameStatusVisOff message
  \brief        Description:\n
                Added necessary KPB information

  \param        pFs
                KpbFrameStatusVal *

  \return       void

 */
/*----------------------------------------------------------------------------*/
static void _KPB_vSendFrameStatusVisOff(const KpbFrameStatusVal *pFs, Uint32Type line)
{
    KpbFrameStatusType frameStatus;

    DEBUG_VALUE2(KPB_SWI_TRC, MOD_KPB, "Send ReqVisOff for FrID: %d, in line : %d", pFs->activeFrameId, line);

    frameStatus.frameId      = (Uint8Type)pFs->activeFrameId;
    frameStatus.relPrio      = pFs->priority;
    frameStatus.mflAssigned  = pFs->mflAssigned;
    frameStatus.asgResponse  = KPB_ASG_NO_RESPONSE;
    frameStatus.fsgRequest   = KPB_FSG_REQ_V_IS_OFF;

    _KPB_vBapSendFrameStatus(&frameStatus);
}

/*----------------------------------------------------------------------------*/
/* Function :   _KPB_vSendFrameStatusVisOn                                    */
/**
  \brief        Invoke the sending of KPB FrameStatusVisOn message
  \brief        Description:\n
                Added necessary KPB information

  \param        pFs
                KpbFrameStatusVal *

  \return       void

 */
/*----------------------------------------------------------------------------*/
static void _KPB_vSendFrameStatusVisOn(const KpbFrameStatusVal *pFs, Uint32Type line)
{
    KpbFrameStatusType frameStatus;

    DEBUG_VALUE2(KPB_SWI_TRC, MOD_KPB, "Send ReqVisOn for FrID: %d, in line : %d", pFs->activeFrameId, line);

    frameStatus.frameId      = (Uint8Type)pFs->activeFrameId;
    frameStatus.relPrio      = pFs->priority;
    frameStatus.mflAssigned  = pFs->mflAssigned;
    frameStatus.asgResponse  = KPB_ASG_NO_RESPONSE;
    frameStatus.fsgRequest   = KPB_FSG_REQ_V_IS_ON;

    _KPB_vBapSendFrameStatus(&frameStatus);
}

/*----------------------------------------------------------------------------*/
/* Function    : KPB_sfShutdown                                               */
/**
    \brief      Interface to invoke the shutdown of KPB communication


    \return     SuccessFailType
                SUCCESS or FAIL

   */
/*----------------------------------------------------------------------------*/
SuccessFailType KPB_sfShutdown( )
{
    KpbPending pending;

    if(gKpbContext.opState == KPB_OP_STATE_OFF)
    {

        DEBUG_TEXT(KPB_SWI_ERR, MOD_KPB, "sfShutdown: invalid state KPB_OP_STATE_OFF!");

        return FAIL;
    }


    DEBUG_VALUE1(KPB_SWI_TRC, MOD_KPB, "sfShutdown: cur state %d", gKpbContext.opState);

    gKpbContext.opState = KPB_OP_STATE_OFF;

    /* set the ID to invalid until we wait to shut down - be aware if KL15 on again without bus sleep -
     * DO NOT USE SetVisOn/OFF with invalid FrID - This change in necessary to be done to reach a good start up sequence*/
    gKpbContext.fs.activeFrameId = KPB_FRAME_ID_INVALID;

    pending.shutdown.common.id = KPB_PENDING_SHUTDOWN;
    //gKpbContext.waitForProcessing.shutdown.state     = KPB_SHUTDOWN_STATUS_REQ_V_IS_OFF_NOT_SENT;
    pending.shutdown.state     = KPB_SHUTDOWN_STATUS_WRITE_PERSISTENT;

    _KPB_vSetNextJob(&pending);

    return SUCCESS;
}


/*----------------------------------------------------------------------------*/
/* Function    : _KPB_vRemoveRedundantCell                                    */
/**
    \brief      Removes redunda

    \return     void

   */
/*----------------------------------------------------------------------------*/
void _KPB_vRemoveRedundantCell(KpbFrameDataReqType *pFrameData)
{
    Uint8Type i = 0;

    if( pFrameData->frameId > KPB_FRAME_ID_7 || pFrameData->frameId == KPB_FRAME_ID_INVALID )
    {
        DEBUG_VALUE1( KPB_SWI_ERR, MOD_KPB, "_KPB_vRemoveRedundantCell: frameId %d not supported", 
                      pFrameData->frameId);
        return;
    }

    for(i=0; i<gKpbObservedCells[pFrameData->frameId]; i++)
    {
        Uint32Type *pOldHash = &(gKpbObservedCellsHash[gKpbObservedCellsOffset[pFrameData->frameId] + i]);

		if((pFrameData->cell[i].name.length > 0) && (pFrameData->cell[i].isSet == BT_TRUE))
		{
			Uint32Type   newHash = APP_COMMON_u32GetSDBM_Hash(pFrameData->cell[i].name.content, pFrameData->cell[i].name.length);

			if( (*pOldHash == newHash)&& // first check if the screen can be updated without being visible(see "BAP_KPB_FrameCatalogue")
			    !((pFrameData->frameId >= KPB_FRAME_ID_3) && (pFrameData->frameId <= KPB_FRAME_ID_6) && (pFrameData->frameId != gKpbContext.fs.activeFrameId)) )
			{
				pFrameData->cell[i].isSet = BT_FALSE;
				DEBUG_VALUE2( KPB_SWI_TRC, MOD_KPB, "_KPB_vRemoveRedundantCell: frameId %d, cell %d deactivated",
							  pFrameData->frameId,
							  i);
			}
			else
			{
				*pOldHash = newHash;
				DEBUG_VALUE2( KPB_SWI_TRC, MOD_KPB, "_KPB_vRemoveRedundantCell: frameId %d, cell %d",
							  pFrameData->frameId,
							  i);
				DEBUG_VALUE1( KPB_SWI_TRC, MOD_KPB, "_KPB_vRemoveRedundantCell: hash %d saved", newHash);
			}
		}
		else if((pFrameData->cell[i].name.length == 0) && (pFrameData->cell[i].isSet == BT_TRUE)) // if already old data, sent blank cell information
		{
			pFrameData->cell[i].isSet = BT_TRUE;
			pFrameData->cell[i].type                = KPB_CELL_TYPE_ANY_STRING;
			pFrameData->cell[i].attribute.rawByte   = KPB_CELL_ATTRIBUTE_VISIBLE;
			pFrameData->cell[i].name.length         = 1;
			(void)memcpy( (pFrameData->cell[i].name.content), "", 1);

			*pOldHash = 0;

			DEBUG_VALUE2( KPB_SWI_TRC, MOD_KPB, "Send blank cell information for frameId %d, cell %d",
					pFrameData->frameId,
					i);
		}
	}
}


/*----------------------------------------------------------------------------*/
/* Function    : _KPB_vLimitCellSize                                          */
/**
    \brief      Limits the number of chars per cell

    \return     void

   */
/*----------------------------------------------------------------------------*/
void _KPB_vLimitCellSize(KpbFrameDataReqType *pFrameData, Uint32Type maxChars)
{
    Uint32Type i = 0;

    for(i=0; i<KPB_MAX_FRAME_CELL_NUMBER; i++)
    {
        Uint32Type numChars = 0;
        Uint32Type j = 0;
        BooleanType bCellContainsValidPhoneNumber = BT_FALSE;


        /* BAP-KPB_FrameCatalogue_UHV_V1.12.pdf S.13: 
        The displays size class specific restrictions for string length do not apply for cells 0-6 of this
        frame!! This means, these cells can be filled up to their maximum length even in case of
        small or medium size display class. */
        if( pFrameData->frameId == KPB_FRAME_ID_5 && 
            i < 7 )
        {
            continue;
        }
        
        if(pFrameData->cell[i].isSet != BT_TRUE)
        {
            continue;
        }
    
        if(pFrameData->cell[i].name.length <= maxChars)
        {
            continue;
        }
    
        bCellContainsValidPhoneNumber = APP_PHONE_bIsValidPhoneNumber(pFrameData->cell[i].name.content,pFrameData->cell[i].name.length);

        if((KPB_CELL_TYPE_PHONE_NUMBER == pFrameData->cell[i].type) &&
           (BT_TRUE == bCellContainsValidPhoneNumber))
        {
            // a valid phone number contains only ascii characters, so number of bytes is equal to number of characters
            // no utf8 handling needed
            if(pFrameData->cell[i].name.length > MAX_CHARACTERS_PER_CELL_FOR_PHONE_NUMBERS)
            {

                DEBUG_VALUE4(KPB_SWI_TRC, MOD_KPB, "vLimitCellSize: shortend cell %d from %d to %d (number)", i, pFrameData->cell[i].name.length, MAX_CHARACTERS_PER_CELL_FOR_PHONE_NUMBERS, 0);

                pFrameData->cell[i].name.length = MAX_CHARACTERS_PER_CELL_FOR_PHONE_NUMBERS;
            }
        }
        else
        {
            for(j=0; j<pFrameData->cell[i].name.length; j++)
            {
                BooleanType loop = BT_TRUE;

                switch(APP_COMMON_enGetUtf8Type(pFrameData->cell[i].name.content[j]))
                {
                    case UTF8_START_BYTE_SIZE_1:
                    case UTF8_START_BYTE_SIZE_2:
                    case UTF8_START_BYTE_SIZE_3:
                    case UTF8_START_BYTE_SIZE_4:
                        if(numChars >= maxChars)
                        {

                           DEBUG_VALUE4(KPB_SWI_TRC, MOD_KPB, "vLimitCellSize: shortend cell %d from %d to %d", i, pFrameData->cell[i].name.length, j, 0);

                           pFrameData->cell[i].name.length = j;
                           loop = BT_FALSE;
                        
                        }
                        else
                        {
                            /* count this byte as utf8 char */
                            numChars++;
                        }
                        break;
                    case UTF8_FOLLOW_BYTE:
                        /* we don't check the right number of follow bytes */
                        break;
                    default:

                         DEBUG_VALUE1(KPB_SWI_ERR, MOD_KPB, "vLimitCellSize: invalid utf8 char: 0x%02x", pFrameData->cell[i].name.content[j]);

                        /* since we can not parse this byte string, we just set 
                           the length to the maxChars size */
                        pFrameData->cell[i].name.length = maxChars;
                        loop = BT_FALSE;
                        break;
                }

                if( loop != BT_TRUE)
                {
                    break;
                }
            }
        }
    }
}

// --------------------------------------------------------------
// ----------------- Public Interfaces --------------------------


/*----------------------------------------------------------------------------*/
/* Function    : KPB_sfSendFrameData                                          */
/**
    \brief      Handles incoming  KPB FrameData requests

    \brief      Description:\n
                Special handler for incoming FrameData requests from SD Application

    \param      pFrameData
                KpbFrameDataReqType *

    \param      activate
                BooleanType

    \param      priority
                KPB_enRelativePriority

    \return     SuccessFailType
                SUCCESS or FAIL

   */
/*----------------------------------------------------------------------------*/
SuccessFailType KPB_sfSendFrameData( KpbFrameDataReqType *pFrameData,
                                     BooleanType activate,
                                     KPB_enRelativePriority priority)
{
    KpbPending pending;
    if(gKpbContext.pending.sendFd.pData != NULL)
    {

        DEBUG_TEXT(KPB_SWI_ERR, MOD_KPB, "SendFrData: pending frameData blocks!");

        return FAIL;
    }

    if(gKpbContext.opState == KPB_OP_STATE_OFF)
    {

        DEBUG_TEXT(KPB_SWI_ERR, MOD_KPB, "SendFrData: ignored in opState OFF!");

        return FAIL;
    }

    /* limits the number of characters as specified by the display class RT 1016 */
    _KPB_vLimitCellSize(pFrameData, KPB_u16GetNumChar());
    /* removes cells that were not changed */
    _KPB_vRemoveRedundantCell(pFrameData);

    /* store the message to resend or second part send is needed after receiving
     * ack */
    //gKpbContext.waitForProcessing.sendFd.common.id      = KPB_PENDING_SEND_FD;
    //gKpbContext.waitForProcessing.sendFd.emptyFrameId   = KPB_FRAME_ID_INVALID;
    //gKpbContext.waitForProcessing.sendFd.pData          = pFrameData;
    //gKpbContext.waitForProcessing.sendFd.retries        = 0U;
    //gKpbContext.waitForProcessing.sendFd.state          = KPB_FRAME_DATA_NOT_SENT;

    pending.sendFd.common.id      = KPB_PENDING_SEND_FD;
    pending.sendFd.emptyFrameId   = KPB_FRAME_ID_INVALID;
    pending.sendFd.frameId        = pFrameData->frameId;
    pending.sendFd.pData          = pFrameData;
    pending.sendFd.retries        = 0U;
    pending.sendFd.state          = KPB_FRAME_DATA_NOT_SENT;
    pending.sendFd.activateFrame.priority = priority; //RT:5848

    if(activate == BT_TRUE || gKpbContext.isFsgQueuedUp )
    {
        //RS: do the isVisible check later, as short as possible before sending the frame activation
        if( (gKpbContext.fs.activeFrameId != pFrameData->frameId) ||
            (gKpbContext.fs.isVisible == BT_FALSE) )
        {
            //gKpbContext.waitForProcessing.sendFd.activateFrame.id = pFrameData->frameId;
            //gKpbContext.waitForProcessing.sendFd.activateFrame.priority = priority;
            pending.sendFd.activateFrame.id = pFrameData->frameId;
            //pending.sendFd.activateFrame.priority = priority; //RT:5848

            if(activate == BT_FALSE && gKpbContext.isFsgQueuedUp)
            {

                DEBUG_VALUE1(KPB_SWI_TRC, MOD_KPB, "SendFrData: activate %d (queued)", pFrameData->frameId);
            }
            else
            {

                DEBUG_VALUE1(KPB_SWI_TRC, MOD_KPB, "SendFrData: activate %d", pFrameData->frameId);
            }
        }
        else
        {
            //gKpbContext.waitForProcessing.sendFd.activateFrame.id = KPB_FRAME_ID_INVALID;
            pending.sendFd.activateFrame.id = KPB_FRAME_ID_INVALID;

            DEBUG_VALUE1(KPB_SWI_TRC, MOD_KPB, "SendFrData: update %d", pFrameData->frameId);
        }
    }
    else
    {
        //RT 4394: in the case that we are visible in KPB but no activate from SD don't overwrite 
        //         the activeFrameId, if we are not visible we need the last updated screen for reactivation
        //if(gKpbContext.fs.isVisible == BT_FALSE)
        //{
            gKpbContext.fs.activeFrameId = pFrameData->frameId;
        //}

        pending.sendFd.activateFrame.id = KPB_FRAME_ID_INVALID;

        DEBUG_VALUE1(KPB_SWI_TRC, MOD_KPB, "SendFrData: no activate %d", pFrameData->frameId);
    }

    _KPB_vSetNextJob(&pending);

    return SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* Function :   KPB_vSendFrameStatusPresReqOff                                    */
/**
  \brief        Invoke the sending of KPB FrameStatusReqOff message
  \brief        Description:\n
                Added necessary KPB information to switch the display to MMI menu

  \return       void

 */
/*----------------------------------------------------------------------------*/
void KPB_vSendFrameStatusPresReqOff(void)
{
    KpbFrameStatusType frameStatus;

    frameStatus.frameId      = 0xFF;
    frameStatus.relPrio      = gKpbContext.fs.priority; //RT:5848
    frameStatus.mflAssigned  = gKpbContext.fs.mflAssigned;
    frameStatus.asgResponse  = KPB_ASG_NO_RESPONSE;
    frameStatus.fsgRequest   = KPB_FSG_REQ_V_IS_OFF;

    DEBUG_TEXT(KPB_SWI_TRC, MOD_KPB, "Release all phone frames, leave the context");
    _KPB_vBapSendFrameStatus(&frameStatus);
}

/*----------------------------------------------------------------------------*/
/* Function :   _KPB_vSendFrameStatusIntPresReqOff                                    */
/**
  \brief        Invoke the sending of KPB FrameStatusReqOff message
  \brief        Description:\n
                Added necessary KPB information to switch the display to MMI menu

    \param      pFs
                KpbFrameStatusType *

  \return       void

 */
/*----------------------------------------------------------------------------*/
static void _KPB_vSendFrameStatusIntPresReqOff(const KpbFrameStatusType *pFs)
{
    KpbFrameStatusType frameStatus;

    frameStatus.frameId      = 0xFF;
    frameStatus.relPrio      = pFs->relPrio; //RT:5848
    frameStatus.mflAssigned  = pFs->mflAssigned;
    frameStatus.asgResponse  = KPB_ASG_NO_RESPONSE;
    frameStatus.fsgRequest   = KPB_FSG_REQ_V_IS_OFF;

    _KPB_vBapSendFrameStatus(&frameStatus);
}

/*----------------------------------------------------------------------------*/
/* Function :   KPB_vReleaseFrameDataElement                                  */
/**
  \brief        Release the FrameData resource

  \param        pFrameData
                KpbFrameDataReqType *

  \param        bSucc
                BooleanType

  \return       void

 */
/*----------------------------------------------------------------------------*/
void KPB_vReleaseFrameDataElement(KpbFrameDataReqType **pFrameData, BooleanType bSucc)
{
    if(*pFrameData != NULL)
    {
        //(void)POOL_sfFree(*pFrameData);
        KPB_sfFreeFrameBuffer();
        *pFrameData = NULL;

        /* notify APP_SD that the FrameData is available again */
        APP_SD_vFrameDataTransfered(bSucc);
    }

}

/*----------------------------------------------------------------------------*/
/* Function :   KPB_enGetKpbVersionNumber                                     */
/**
  \brief        Returns the KPB version of the KI

  \return       KPB_enVersionNumber

 */
/*----------------------------------------------------------------------------*/
KPB_enVersionNumber KPB_enGetKpbVersionNumber()
{
    return gKpbContext.versionNumber;
}

/*----------------------------------------------------------------------------*/
/* Function    : KPB_bIsAsgPresentation                                       */
/**
    \brief      Returns whether the KI is currently showing a frame or not

    \return     BooleanType
                BT_TRUE or BT_FALSE
   */
/*----------------------------------------------------------------------------*/
BooleanType KPB_bIsAsgPresentation()
{
    return gKpbContext.fs.isVisible;
}

/*----------------------------------------------------------------------------*/
/* Function    : KPB_vSetAsgPresentationOff                                   */
/**
    \brief      Switch the visibility for the internal handling off to show next screen

    \return     BooleanType
                BT_TRUE or BT_FALSE
   */
/*----------------------------------------------------------------------------*/
void KPB_vSetAsgPresentationOff(void)
{
    gKpbContext.fs.isVisible = BT_FALSE;
}

/*----------------------------------------------------------------------------*/
/* Function    : KPB_enGetAsgState                                            */
/**
    \brief      Returns whether the KI is alive or dead

    \return     KPB_enAsgStatus
   */
/*----------------------------------------------------------------------------*/
KPB_enAsgStatus KPB_enGetAsgState()
{
    return gKpbContext.asgState;
}


/*----------------------------------------------------------------------------*/
/* Function    : KPB_bIsKeyAssigned                                           */
/**
    \brief      Returns true if the specified key is assigned, false otherwise

    \param      key
                ENUM_MFL_BUTTON

    \return     BooleanType
                BT_TRUE or BT_FALSE
   */
/*----------------------------------------------------------------------------*/
BooleanType KPB_bIsKeyAssigned(LOGI_BUT_MAN_ENUM_BUTTON_NAMES key)
{
    BooleanType res = BT_FALSE;

    if(gKpbContext.fs.isVisible != BT_TRUE)
    {
        /* if we are not visible we return always FALSE */
        return BT_FALSE;
    }

    switch(key)
    {
        case enLogiButManLssUp:
        case enLogiButManLssDown:
        case enLogiButManMflUp:
        case enLogiButManMflDown:
            if( (gKpbContext.mflBlockDef[0] == KPB_KB_UP_DOWN_LEFT_RIGHT_OK) ||
                (gKpbContext.mflBlockDef[0] == KPB_KB_UP_DOWN) )
            {
                res = (BooleanType)gKpbContext.fs.mflAssigned.mflBlocks.keyBlock.kb0;
            }
            else if( (gKpbContext.mflBlockDef[1] == KPB_KB_UP_DOWN_LEFT_RIGHT_OK) ||
                     (gKpbContext.mflBlockDef[1] == KPB_KB_UP_DOWN) )
            {
                res = (BooleanType)gKpbContext.fs.mflAssigned.mflBlocks.keyBlock.kb1;
            }
            else if( (gKpbContext.mflBlockDef[2] == KPB_KB_UP_DOWN_LEFT_RIGHT_OK) ||
                     (gKpbContext.mflBlockDef[2] == KPB_KB_UP_DOWN) )
            {
                res = (BooleanType)gKpbContext.fs.mflAssigned.mflBlocks.keyBlock.kb2;
            }
            break;
        case enLogiButManLssOk:
        case enLogiButManMflOk:
            if( (gKpbContext.mflBlockDef[0] == KPB_KB_UP_DOWN_LEFT_RIGHT_OK) ||
                (gKpbContext.mflBlockDef[0] == KPB_KB_OK) )
            {
                res = (BooleanType)gKpbContext.fs.mflAssigned.mflBlocks.keyBlock.kb0;
            }
            else if( (gKpbContext.mflBlockDef[1] == KPB_KB_UP_DOWN_LEFT_RIGHT_OK) ||
                     (gKpbContext.mflBlockDef[1] == KPB_KB_OK) )
            {
                res = (BooleanType)gKpbContext.fs.mflAssigned.mflBlocks.keyBlock.kb1;
            }
            else if( (gKpbContext.mflBlockDef[2] == KPB_KB_UP_DOWN_LEFT_RIGHT_OK) ||
                     (gKpbContext.mflBlockDef[2] == KPB_KB_OK) )
            {
                res = (BooleanType)gKpbContext.fs.mflAssigned.mflBlocks.keyBlock.kb2;
            }
            break;
        case enLogiButManMflBack:
            if( gKpbContext.mflBlockDef[0] == KPB_KB_BACK )
            {
                res = (BooleanType)gKpbContext.fs.mflAssigned.mflBlocks.keyBlock.kb0;
            }
            else if( gKpbContext.mflBlockDef[1] == KPB_KB_BACK )
            {
                res = (BooleanType)gKpbContext.fs.mflAssigned.mflBlocks.keyBlock.kb1;
            }
            else if( gKpbContext.mflBlockDef[2] == KPB_KB_BACK )
            {
                res = (BooleanType)gKpbContext.fs.mflAssigned.mflBlocks.keyBlock.kb2;
            }
            break;
        case enLogiButManMflHook:
        case enLogiButManVirtualButtonInfo:
        case enLogiButManVirtualButtonService:
        case enLogiButManVirtualButtonSos:
            /* no keyblock defines this key, so it is TRUE per default */
            res = BT_TRUE;
            break;

        default:
            res = BT_FALSE;
            break;
    }

    return res;
}


/*----------------------------------------------------------------------------*/
/* Function    : KPB_bIsAsgAvailable                                          */
/**
    \brief      Returns true if there is a KI available, false otherwise

    \return     BooleanType
                BT_TRUE or BT_FALSE
   */
/*----------------------------------------------------------------------------*/
BooleanType KPB_bIsAsgAvailable()
{
    return gKpbContext.asgAvailable;
}


/*----------------------------------------------------------------------------*/
/* Function    : KPB_u16GetNumChar                                            */
/**
    \brief      Returns the number of visible chars per cell for the current KI

    \return     Uint16Type
                number of available character per cell
   */
/*----------------------------------------------------------------------------*/
Uint16Type KPB_u16GetNumChar()
{
    Uint16Type dispSize = 0U;

    switch(gKpbContext.displayClass)
    {
        case KPB_DISP_SIZE_SMALL:
            dispSize = KPB_NUM_CHAR_DISP_CLASS_SMALL;
            break;
        default:
            dispSize = KPB_NUM_CHAR_DISP_CLASS_MEDIUM;
            break;
    }

    return dispSize;
}

/*----------------------------------------------------------------------------*/
/* Function    : KPB_u16GetVisListElements                                    */
/**
    \brief      Returns the number of visible list elements for the current KI

    \return     Uint8Type
                number of available list elements
   */
/*----------------------------------------------------------------------------*/
Uint8Type KPB_u16GetVisListElements()
{
    return gKpbContext.visibleElements;
}

/*----------------------------------------------------------------------------*/
/* Function: KPB_vOnAppCshdlNotification                                      */
/**
  \brief      Handles notification of the CSHDL

  \param      notification APP_CSHDL_NotificationType

  \return     void
 */
/*----------------------------------------------------------------------------*/
void KPB_vOnAppCshdlNotification(APP_CSHDL_NotificationType notification)
{

	static BooleanType bKl15ActiveWhileKl15OffFollowing = BT_FALSE; //(rt:5671)

    DEBUG_VALUE4(KPB_SWI_TRC, MOD_KPB, "vOnAppCshdlNot: notId %d, KPB opState %d, LM %d,%d", 
                 notification, gKpbContext.opState, 
                 gKpbContext.lastMode.inPresentation, gKpbContext.lastMode.cradleRestriction);

    switch(notification)
    {
        case APP_CSHDL_NOTIFY_UHV_ACTIVE_KL_15_ON:
        {

            if(gKpbContext.waitForProcessing.shutdown.common.id != KPB_PENDING_SHUTDOWN)
            {
                switch(gKpbContext.opState)
                {
                    case KPB_OP_STATE_OFF:

                        // re-init the hash table to avoid empty screen after fast clamp changed
                        (void)memset(&gKpbObservedCellsHash, 0, sizeof(gKpbObservedCellsHash)); //RT:14521

                        gKpbContext.opState = KPB_OP_STATE_NORMAL_OPERATION;
                        _KPB_vBapSendOperationState(gKpbContext.opState);

                        if((_KPB_bIsKpbInitDone())&& (BT_FALSE == bKl15ActiveWhileKl15OffFollowing))
                        {
                            if(gKpbContext.lastMode.inPresentation == LAST_MODE_ON)
                            {
                                _KPB_vNotifyApplication(KPB_NOTIFY_ASG_INIT_DONE_ACTIVE);
                            }
                            else
                            {
                                _KPB_vNotifyApplication(KPB_NOTIFY_ASG_INIT_DONE);
                            }
                        }
                        else
                        {

                            DEBUG_VALUE1(KPB_SWI_TRC, MOD_KPB, "vOnAppCshdlNot: KPB_OP_STATE_OFF, KPB init not done, notId: %d", notification);
                        }
                        break;
                    case KPB_OP_STATE_KL_15_FOLLOW_UP:
                        gKpbContext.opState = KPB_OP_STATE_NORMAL_OPERATION;
                        _KPB_vBapSendOperationState(gKpbContext.opState);

                        // re-init the hash table to avoid empty screen after fast clamp changed
                        (void)memset(&gKpbObservedCellsHash, 0, sizeof(gKpbObservedCellsHash)); //RT:6400

                        /* KL.15 On again while KL.15 in following off state, inform APP_SD to show the reconnect screen (RT:5671)*/
                        if(gKpbContext.lastMode.inPresentation == LAST_MODE_ON)
                        {
                            _KPB_vNotifyApplication(KPB_NOTIFY_ASG_INIT_DONE_ACTIVE);
                        }
                        else
                        {
                            _KPB_vNotifyApplication(KPB_NOTIFY_ASG_INIT_DONE);
                        }

                        bKl15ActiveWhileKl15OffFollowing = BT_TRUE;

                        break;
                    case KPB_OP_STATE_NORMAL_OPERATION:
                        if(gKpbContext.lastMode.inPresentation == LAST_MODE_ON &&
                            gKpbContext.fs.isVisible != BT_TRUE )
                        {
                            if(gKpbContext.fs.activeFrameId != KPB_FRAME_ID_INVALID)
                            {
                                gKpbContext.fs.priority = KPB_RELATIVE_PRIO_LOW; //RT:5848
                                _KPB_vSendFrameStatusVisOn(&(gKpbContext.fs),__LINE__);
                            }
                            else
                            {

                                DEBUG_VALUE1(KPB_SWI_ERR, MOD_KPB, "vOnAppCshdlNotific: KL15On in NORMAL_OP, but frameId %d", gKpbContext.fs.activeFrameId);
                            }
                        }
                        break;
                    default:
                        /* do nothing */
                        break;
                }
            }
            else
            {

                DEBUG_TEXT(KPB_SWI_WNG, MOD_KPB, "vOnAppCshdlNotific: ignored KL15On (shutdown)");

            }
            break;
        }
        case APP_CSHDL_NOTIFY_UHV_ACTIVE_KL_15_OFF:
        {
            //gKpbContext.lastModeVisible = _KPB_bGetLastModeVisible();

            if( gKpbContext.opState != KPB_OP_STATE_NORMAL_OPERATION )
            {
                // re-init the hash table to avoid empty screen after fast clamp changed
                (void)memset(&gKpbObservedCellsHash, 0, sizeof(gKpbObservedCellsHash)); //RT:14521

                gKpbContext.opState = KPB_OP_STATE_NORMAL_OPERATION;
                _KPB_vBapSendOperationState(gKpbContext.opState);

                if(_KPB_bIsKpbInitDone())
                {
                    if(gKpbContext.lastMode.inPresentation == LAST_MODE_ON)
                    {
                        _KPB_vNotifyApplication(KPB_NOTIFY_ASG_INIT_DONE_ACTIVE);
                    }
                    else
                    {
                        _KPB_vNotifyApplication(KPB_NOTIFY_ASG_INIT_DONE);
                    }
                }
                else
                {

                    DEBUG_VALUE1(KPB_SWI_TRC, MOD_KPB, "vOnAppCshdlNot: KPB init not yet done, notId: %d", notification);
                }
            }
            break;
        }
        case APP_CSHDL_NOTIFY_KL_15_OFF:
            (void)APP_COMMON_UTILS_stCancelOsekRelAlarm(CTA_KPB_DISPLAY_STATUS_OFF);
            bKl15ActiveWhileKl15OffFollowing = BT_FALSE;
            gKpbContext.opState = KPB_OP_STATE_KL_15_FOLLOW_UP;
            _KPB_vBapSendOperationState(gKpbContext.opState);
            break;
        case APP_CSHDL_NOTIFY_PREPARE_CK5050_SHUTDOWN:
            APP_CSHDL_vCk5050ShutDownHandled(APP_CSHDL_KPB);
            break;
        case APP_CSHDL_NOTIFY_WAIT_FOR_SHUTDOWN:
        	/* first check if the KBP have to be shut down*/
        	if(BT_FALSE == bKl15ActiveWhileKl15OffFollowing) //(RT:5671)
        	{
        		(void)KPB_sfShutdown();
        	}
        	else
        	{
        		DEBUG_TEXT(KPB_SWI_TRC, MOD_KPB, "No KPB shutdown in KL15 off following and KL15 on again!");
        	}
        	break;
        case APP_CSHDL_NOTIFY_CRADLE_RESTRICTION_ON:
            gKpbContext.lastMode.cradleRestriction = LAST_MODE_ON;

            DEBUG_VALUE2(KPB_SWI_TRC, MOD_KPB, "vOnAppCshdl: CR LM -> %d, %d", gKpbContext.lastMode.inPresentation, gKpbContext.lastMode.cradleRestriction);
            break;
        case APP_CSHDL_NOTIFY_CRADLE_RESTRICTION_OFF:
            gKpbContext.lastMode.cradleRestriction = LAST_MODE_OFF;

            DEBUG_VALUE2(KPB_SWI_TRC, MOD_KPB, "vOnAppCshdl: CR LM -> %d, %d", gKpbContext.lastMode.inPresentation, gKpbContext.lastMode.cradleRestriction);
            break;
        default:

            DEBUG_VALUE1(KPB_SWI_WNG, MOD_KPB, "vOnAppCshdlNotific: ignoring notId %d", notification);
            break;
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : _KPB_bIsKpbInitDone                                          */
/**
    \brief      Indicates that the KPB startup is done or not 

    \param      void

    \return     BooleanType

   */
/*----------------------------------------------------------------------------*/
BooleanType _KPB_bIsKpbInitDone()
{
    if( gKpbContext.asgInitState.capabilities &&
        gKpbContext.asgInitState.config &&
        gKpbContext.asgInitState.mflBlockDef )
    {
        return BT_TRUE;
    }
    
    return BT_FALSE;
}


/*----------------------------------------------------------------------------*/
/* Function    : _KPB_vNotifyApplication                                       */
/**
    \brief      function to notify application that something has changed

    \brief      triggers application of the same task context by function call

    \param      new_notification - identifies the changed event
                KPBNotificationType
                values of enum state type

    \return     none

   */
/*----------------------------------------------------------------------------*/
void _KPB_vNotifyApplication(KPB_enNotificationId new_notification)
{
    APP_SD_vOnKpbNotification(new_notification);
    CONTEXT_MAN_vOnKpbNotification(new_notification);
}

/*----------------------------------------------------------------------------*/
/* Function    : _KPB_vNotifyApplicationImmediate                                       */
/**
    \brief      function to notify application that something has changed

    \brief      the sign off notice to appl. is send immediately here.

    \param      new_notification - identifies the changed event
                KPBNotificationType
                values of enum state type

    \return     none

   */
/*----------------------------------------------------------------------------*/
static void _KPB_vNotifyApplicationImmediate(KPB_enNotificationId new_notification)
{
    CONTEXT_MAN_vOnKpbNotification(new_notification);
}

/*----------------------------------------------------------------------------*/
/* Function :   _KPB_vSetAsgVisibility                               */
/** 
  \brief        Sets the ASG visible state  

  \param        visible
                visible = true means that ASG visible, else not

  \param        line
                line where functionw as called, for debug output only  

  \return       void

 */
/*----------------------------------------------------------------------------*/
static void _KPB_vSetAsgVisibility(BooleanType visible, Uint32Type line)
{


    DEBUG_VALUE2(KPB_SWI_TRC, MOD_KPB, "SetAsgVis: %d -> %d", gKpbContext.fs.isVisible, visible);
    DEBUG_VALUE1(KPB_SWI_TRC, MOD_KPB, "called from line %d", line);

    gKpbContext.fs.isVisible = visible;
 
    _KPB_vSetLastModeVisible(visible);
}

static void _KPB_vSetLastModeVisible(BooleanType visible)
{
    /* we recognize lastMode changes only if the KL_15 is ON */
    if( CSHDL_bIsKl15On() == BT_TRUE )
    {
        gKpbContext.isAppsNotified = BT_FALSE; //RT #4727 (set the flag back)

        if(visible == BT_TRUE)
        {
            (void)APP_COMMON_UTILS_stCancelOsekRelAlarm(CTA_KPB_DISPLAY_STATUS_OFF);

            gKpbContext.lastMode.inPresentation = LAST_MODE_ON;


            DEBUG_TEXT(KPB_SWI_TRC, MOD_KPB, "SetLastModeVis: lastMode TRUE");

            _KPB_vNotifyApplication(KPB_NOTIFY_VISIBLE_ON);
        }
        else
        {
            /* we set the lastMode of the Display indirectly with the help of an alarm-callback
             * to handle shutdown delay */
            /* The context manager needs the notification immediately */
            _KPB_vNotifyApplicationImmediate(KPB_NOTIFY_VISIBLE_OFF);
            (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_KPB_DISPLAY_STATUS_OFF,
                                                      MSEC(KPB_DISPLAY_STATUS_OFF_TIMEOUT),
                                                      0);
            DEBUG_TEXT(KPB_SWI_TRC, MOD_KPB, "SetLastModeVis: start lastMode FALSE timer");

            if(gKpbContext.pending.common.id != KPB_PENDING_SEND_FD) // RT #4727
            {
                gKpbContext.isAppsNotified = BT_TRUE;
                _KPB_vNotifyApplication(KPB_NOTIFY_VISIBLE_OFF);
                DEBUG_TEXT(KPB_SWI_TRC, MOD_KPB, "Notify Apps immediately with Vis OFF after ASG request");
            }
            else
            {
                DEBUG_TEXT(KPB_SWI_TRC, MOD_KPB, "Notify Apps with Vis OFF after lastMode timer expired");
            }
        }
    }
    else
    {

        DEBUG_VALUE1(KPB_SWI_TRC, MOD_KPB, "SetLastModeVis: ign %d", visible);
    }
}

/*----------------------------------------------------------------------------*/
/* Function :   KPB_sfAllocFrameBuffer                                        */
/**
  \brief        Allocate KPB FrameBuffer (not threadsafe)
                we need our own allocater to handle later FrameDataGet to access
                our old buffer

  \param        pFrameBuffer
                KpbFrameDataReqType *

  \return       SuccessFailType

 */
/*----------------------------------------------------------------------------*/
SuccessFailType KPB_sfAllocFrameBuffer( KpbFrameDataReqType **pFrameBuffer, BooleanType initBuff )
{
    SuccessFailType result = FAIL;

    *pFrameBuffer = NULL;

    if(gKpbFrameBuffer.free == BT_TRUE)
    {
        if(initBuff == BT_TRUE)
        {
           memset(&(gKpbFrameBuffer.buffer), 0, sizeof(gKpbFrameBuffer.buffer)); 
        }

        *pFrameBuffer = &(gKpbFrameBuffer.buffer);

        gKpbFrameBuffer.free = BT_FALSE;
    
        result = SUCCESS;
    }

    return result;
}


/*----------------------------------------------------------------------------*/
/* Function :   KPB_sfFreeFrameBuffer                                        */
/**
  \brief        Frees KPB FrameBuffer (not threadsafe)

  \param        pFrameBuffer
                KpbFrameDataReqType *

  \return       SuccessFailType

 */
/*----------------------------------------------------------------------------*/
SuccessFailType KPB_sfFreeFrameBuffer( )
{
    SuccessFailType result = FAIL;

    if(gKpbFrameBuffer.free != BT_TRUE)
    {
        gKpbFrameBuffer.free = BT_TRUE;
        result = SUCCESS;
    }

    return result;
}

/*----------------------------------------------------------------------------*/
/* Function :   KPB_bIsFsgQueuedUp                                            */
/**
  \brief        Returns whether the FSG presentation is currently queued up or not


  \return       BooleanType

 */
/*----------------------------------------------------------------------------*/
BooleanType KPB_bIsFsgQueuedUp()
{
    return gKpbContext.isFsgQueuedUp;
}


/*----------------------------------------------------------------------------*/
/* Function :   KPB_bGetLastCradleRestriction                                 */
/**
  \brief        Returns whether the Cradle Restriction was on or off at last


  \return       BooleanType

 */
/*----------------------------------------------------------------------------*/
BooleanType KPB_bGetLastCradleRestriction()
{
    return (gKpbContext.lastMode.cradleRestriction == LAST_MODE_ON) ? BT_TRUE : BT_FALSE;
}

/*----------------------------------------------------------------------------*/
/* Function    : _KPB_cbStartASGSupervision                               */
/**
    \brief      handle processing time out\n

    \param      void

    \return     void
*/
/*----------------------------------------------------------------------------*/
void _KPB_cbStartASGSupervision(void)
{
    _KPB_vStartAsgSupervision();
}

/*13591*/
/*----------------------------------------------------------------------------*/
/* Function    : _KPB_bGetVisbilitySetOnOff                               */
/**
    \brief      returns visbility set on off status\n

    \param      void

    \return     BT_TRUE : If visibility is set to ON
                BT_FALSE: If visibility is set to OFF
*/
/*----------------------------------------------------------------------------*/
BooleanType _KPB_bGetVisbilitySetOnOff(void)
{
    return gKpbContext.fs.isVisible;
}

/* End Of File KPB_C1.c */


