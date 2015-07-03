/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     APP_MDI_StateHandler.c
*-----------------------------------------------------------------------------
* Module Name:    APP_MDI
*-----------------------------------------------------------------------------
* Description:    Handles the control of the player specific communication to
                  the CK5050
*-----------------------------------------------------------------------------
* $Date: 2009-11-22 14:11:24 +0100 (So, 22 Nov 2009) $
* $Rev: 9974 $
* $Author: goldammer $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_MDI/trunk/src/APP_MDI_StateHandler.c $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include <osek.h>           // vector
#include <osekext.h>        // vector
#include "bap_defines.h"    // vw
#include "BAP_Dispatcher.h"
#include "ATCmdHandler.h"
#include "ERM.h"
#include "OSEK_AppTask_Alarm.h"
#include "UART.h"
#include "APP_COMMON_Utils.h"
#include "APP_CSHDL.h"
#include "APP_MDI_BAP.h"
#include "APP_DEVICE_AudioManager.h"
#include "TRACE.h"
#include "APP_MDI_StateHandler.id"
/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "APP_MDI.h"
#include "APP_MDI_AT.h"
#include "APP_MDI_CI.h"

/*----------------------------------------------------------------------------*/
/* pragma Directives                                                          */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Definitions (avoid as much as possible!)                  */
/*----------------------------------------------------------------------------*/

/**
\brief	variable to hold module global data
*/
extern AppMdiContext gAppMdiContext;

extern AppMdiJobType gAppMdiJobTable[APP_MDI_JOB_MAX];
/*----------------------------------------------------------------------------*/
/* Private Manifest Constant Definitions                                      */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private Types                                                              */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Prototypes of file local Methods                                           */
/*----------------------------------------------------------------------------*/
static BooleanType     _APP_MDI_bIsJobAvailable();
static SuccessFailType _APP_MDI_sfHandleDply(SCI_ST const *pUart, AppMdiDplyJobType *pDply);



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
/* Function    : _APP_MDI_vReInitApp                                    */
/** 
    \brief      reinit the MDI context
  
    \brief      Description:\n
               
    \return     void
*/
/*----------------------------------------------------------------------------*/
void _APP_MDI_vReInitApp(void)
{
    DEBUG_VALUE1( APP_MDI_SWI_WNG, MOD_APP_MDI, "vReInitApp: reset fctId %d", 
                  gAppMdiContext.fct.common.id);


    (void)ERM_ReleaseUartForMDI(); 

    APP_MDI_sfReInitApp();
}

/*----------------------------------------------------------------------------*/
/* Function :   _APP_MDI_vReleaseContext                                      */
/**
  \brief        Reset the current function context

  \return       void

 */
/*----------------------------------------------------------------------------*/
void _APP_MDI_vReleaseContext()
{
    (void)memset(&gAppMdiContext.fct, 0, sizeof(gAppMdiContext.fct));

    if(_APP_MDI_bIsJobAvailable() == BT_TRUE)
    {
        (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_MDI_FCT_PENDING, MSEC(0), 0);
        DEBUG_TEXT( APP_MDI_SWI_TRC, MOD_APP_MDI, "RelCtxt: call for next job");
    }
}

/*----------------------------------------------------------------------------*/
/* Function :                                      */
/**
  \brief        Is called to finished the processing of a function

  \brief        Description:\n
                This function signalizes asynchronously the result of the function \n
                to the caller application by invoking the provided callback. After \n
                it the function context is released.

  \param        result
                signalize the result code (OK, ERROR, ...)
                APP_COMMON_enResponseCode

  \return       void

 */
/*----------------------------------------------------------------------------*/
void _APP_MDI_vSendResultReleaseContext(APP_COMMON_enResponseCode result, Uint32Type line)
{
    DEBUG_VALUE2( APP_MDI_SWI_TRC, MOD_APP_MDI, "SendResRelCtxt: fctId %d res %d",
                  gAppMdiContext.fct.common.id,
                  result );
    DEBUG_VALUE1( APP_MDI_SWI_TRC, MOD_APP_MDI, "SendResRelCtxt: line %d", line);

    _APP_MDI_vReleaseContext();
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_MDI_vJobManager                                       */
/**
    \brief      The job manager is responsible for incoming events

    \brief      Description:\n
                The job manager is called for notifications and state changes
                received from other application and even for non-busy function
                calls. It serializes and priorizes the incoming events.

    \param      pJob
                Pointer to the new job.

    \return     void
*/
/*----------------------------------------------------------------------------*/
void _APP_MDI_vJobManager(const AppMdiJobType *pJob)
{
    DEBUG_VALUE1( APP_MDI_SWI_TRC, MOD_APP_MDI, "vJM: new job [%d]", pJob->common.id);


    if(pJob->common.id < APP_MDI_JOB_MAX)
    {
        if(gAppMdiJobTable[pJob->common.id].common.id != APP_MDI_JOB_INVALID)
        {           
            DEBUG_VALUE1( APP_MDI_SWI_TRC, MOD_APP_MDI, "vJM: overwrite job [%d]", pJob->common.id);
        }
        gAppMdiJobTable[pJob->common.id] = *pJob;
            
    }
    else
    {
       DEBUG_VALUE1( APP_MDI_SWI_ERR, MOD_APP_MDI, "vJM: ERROR invalid job [%d]", pJob->common.id);
    }

    if(gAppMdiContext.fct.common.id == APP_MDI_JOB_INVALID)
    {
        (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_MDI_FCT_PENDING, MSEC(0), 0);
    }
    else
    {
        DEBUG_VALUE1( APP_MDI_SWI_TRC, MOD_APP_MDI, "vJM: pending job [%d]", gAppMdiContext.fct.common.id);
    }
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_MDI_sfGetNextJob                                       */
/**
    \brief      The job manager is responsible for incoming events

    \brief      Description:\n
                Returns the next job with highest priority

    \param      pJob
                Pointer to the new job.

    \return     SuccessFailType
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_MDI_sfGetNextJob(AppMdiJobType *pJob)
{
    APP_MDI_enJobId i = APP_MDI_JOB_INVALID;

    for(i = (APP_MDI_enJobId)1; i < APP_MDI_JOB_MAX; i++)
    {
        if(gAppMdiJobTable[i].common.id != APP_MDI_JOB_INVALID)
        {
            *pJob = gAppMdiJobTable[i];
            
            (void)memset(&gAppMdiJobTable[i], 0, sizeof(gAppMdiJobTable[i]));

            //gAppMdiJobTable[i].common.id = APP_MDI_JOB_INVALID;

            return SUCCESS;
        }
    }

    return FAIL;
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_MDI_bIsJobAvailable                                     */
/**
    \brief      The job manager is responsible for incoming events

    \brief      Description:\n
                Returns BT_TRUE if there is a job available, otherwise BT_FALSE

    \return     BooleanType
*/
/*----------------------------------------------------------------------------*/
BooleanType _APP_MDI_bIsJobAvailable()
{
    APP_MDI_enJobId i = APP_MDI_JOB_INVALID;

    for(i = (APP_MDI_enJobId)1; i < APP_MDI_JOB_MAX; i++)
    {
        if(gAppMdiJobTable[i].common.id != APP_MDI_JOB_INVALID)
        {
            return BT_TRUE;
        }
    }

    return BT_FALSE;
}


/*----------------------------------------------------------------------------*/
/* Function :   _APP_MDI_sfHandlePendingFunction                              */
/**
  \brief        Coordinate the processing of mulistated function

  \param        pUart
                const SCI_ST*

  \return       SuccessFailType
                SUCCESS or FAIL

 */
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_MDI_sfHandlePendingFunction( const SCI_ST* pUart )
{
    SuccessFailType result = SUCCESS;

    DEBUG_VALUE1( APP_MDI_SWI_TRC, MOD_APP_MDI, "HaPenFu %d", gAppMdiContext.fct.common.id);

    switch( gAppMdiContext.fct.common.id )
    {
        case APP_MDI_JOB_DPLY:
            result = _APP_MDI_sfHandleDply(pUart, &gAppMdiContext.fct.dply);
            break;
        case APP_MDI_JOB_ALCK:
            result = _APP_MDI_sfSendALCKATRequest(pUart, gAppMdiContext.fct.alck.lockType);
            break;
        case APP_MDI_JOB_SKIP:
            result = _APP_MDI_sfSendPlayerStateATRequest(pUart, gAppMdiContext.fct.skip.skipModeAt);
            break;
        case APP_MDI_JOB_PGMT:
            result = _APP_MDI_sfSendGetMetadataTypeATRequest(pUart, gAppMdiContext.fct.pgmt.deviceId);
            break;
        case APP_MDI_JOB_DGED:
            result = _APP_MDI_sfSendDgedATRequest(pUart, gAppMdiContext.fct.dged.deviceId, gAppMdiContext.fct.dged.fieldmask);
            break;
        case APP_MDI_JOB_DGPS:
            result = _APP_MDI_sfSendDgpsATRequest(pUart, gAppMdiContext.fct.dgps.deviceId);
            break;
        case APP_MDI_JOB_DSRA:
            result = _APP_MDI_sfSendRandomModeATRequest(pUart, gAppMdiContext.fct.dsra.RandomMode);
            break;
        case APP_MDI_JOB_DSRE:
            result = _APP_MDI_sfSendRepeatModeATRequest(pUart, gAppMdiContext.fct.dsre.repeatMode);
            break;
        case APP_MDI_JOB_DCPN:
            result = _APP_MDI_sfSendAutomaticPlayerNotificationATRequest(pUart, gAppMdiContext.fct.dcpn.notifyMode);
            break;
        case APP_MDI_JOB_PBSV:
            result = _APP_MDI_sfSendPbsvATRequest(pUart, gAppMdiContext.fct.pbsv.service, gAppMdiContext.fct.pbsv.action);
            break;
        default:
            result = FAIL;
            break;
    }

    if(result == FAIL)
    {
        DEBUG_VALUE1( APP_MDI_SWI_ERR, MOD_APP_MDI, "FAIL for %d", gAppMdiContext.fct.common.id);
        _APP_MDI_vSendResultReleaseContext(APP_COMMON_RSP_ERROR, __LINE__);
        (void)ERM_ReleaseUartForMDI();
    }

    return SUCCESS;
}

SuccessFailType _APP_MDI_sfHandleDply(SCI_ST const *pUart, AppMdiDplyJobType *pDply)
{
    SuccessFailType result = SUCCESS;

    switch(pDply->mute)
    {
        case APP_MDI_MUTE_GET:
            (void)ERM_ReleaseUartForMDI();
            result = APP_DEVICE_AM_sfGetMute(APP_MDI_cbMuteGetAck, APP_CAN_enAudioStream, __LINE__ );
            if(result != SUCCESS)
            {
                DEBUG_TEXT( APP_MDI_SWI_ERR, MOD_APP_MDI, "HaPeFu: GetMute() failed, safe the unmute request to not loose it");

                if(pDply->playModeAt == AT_MDB_RESUME_PLAYING)
                {
                    gAppMdiContext.unmute_request = APP_MDI_RESUME;
                }
                else if(pDply->playModeAt == AT_MDB_PAUSE_PLAYING)
                {
                    gAppMdiContext.unmute_request = APP_MDI_PAUSE;
                }

                gAppMdiContext.IsCK5050ResponseOngoing = BT_FALSE;
                AppTask_vClearAlarm(APP_TASK_ALARM_MDI_DPLY_RESPONSE_TIMEOUT);
            }
            break;
        case APP_MDI_MUTE_RELEASE:
            APP_DEVICE_AM_vReleaseMute(NULL, APP_CAN_enAudioStream, __LINE__);
            result = _APP_MDI_sfSendPlayerStateATRequest(pUart, pDply->playModeAt);
            gAppMdiContext.bPauseMdiPoweroffCK5050 = BT_FALSE; //RT14519
            break;
        default:
            result = _APP_MDI_sfSendPlayerStateATRequest(pUart, pDply->playModeAt);
            gAppMdiContext.bPauseMdiPoweroffCK5050 = BT_FALSE;//RT14519
            break;
    } 
    
    return result;
}

void APP_MDI_cbMuteGetAck(APP_COMMON_enResponseCode rsp)
{
    DEBUG_VALUE1( APP_MDI_SWI_TRC, MOD_APP_MDI, "vMuteGetAck: RESPONSE %d", rsp);


    switch(gAppMdiContext.fct.common.id)
    {
        case APP_MDI_JOB_DPLY:
            if(rsp == APP_COMMON_RSP_OK)
            {
                gAppMdiContext.fct.dply.mute = APP_MDI_MUTE_NO_ACTION;
                (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_MDI_FCT_PENDING, MSEC(0), 0);
            }
            else
            {
                _APP_MDI_vSendResultReleaseContext(APP_COMMON_RSP_ERROR, __LINE__);
                DEBUG_VALUE1( APP_MDI_SWI_ERR, MOD_APP_MDI, "vMuteGetAck: JOB_DPLY failed %d", rsp);

                gAppMdiContext.IsCK5050ResponseOngoing = BT_FALSE;
                AppTask_vClearAlarm(APP_TASK_ALARM_MDI_DPLY_RESPONSE_TIMEOUT);
            }
            break;
        default:
            if(rsp != APP_COMMON_RSP_OK)
            {
                DEBUG_VALUE1( APP_MDI_SWI_ERR, MOD_APP_MDI, "vMuteGetAck: DEFAULT failed %d", rsp);
            }
            break;
    }

}


/* End Of File APP_MDI_StateHandler.c */


