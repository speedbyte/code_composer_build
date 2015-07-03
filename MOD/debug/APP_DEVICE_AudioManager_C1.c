/******************************************************************************/
/*****************  Copyright (c) 2010 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     APP_DEVICE_AudioManager_C1.c
*-----------------------------------------------------------------------------
* Module Name:    APP_DEVICE
*-----------------------------------------------------------------------------
* Description:    Handles all call specific requests from BAP and
                  AT Cmd Handler
*-----------------------------------------------------------------------------
* $Date: 2010-03-13 16:44:37 +0100 (Sa, 13 Mrz 2010) $
* $Rev: 11532 $
* $Author: goldammer $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_DEVICE/trunk/src/APP_DEVICE_C1.c $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/
#include <osek.h>           // vector
#include <osekext.h>        // vector
/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "APP_COMMON_Utils.h"
#include "APP_DEVICE.h"
#include "GIO.h"
#include "APP_DEVICE_AudioManager.h"
#include "APP_DEVICE_AudioManager_CI.h"
#include "APP_DEVICE_AudioManager_Wire.h"
#include "APP_CSHDL.h"
#include "OSEK_AppTask_Alarm.h"
#include "APP_DEVICE_AudioManager_C1.id"
#include "TRACE.h"
#include "EEPROM_LAYOUT_CFG.h"
#include "APP_SD.h"
#include "APP_PHONE_AT_PNCC.h"

/*----------------------------------------------------------------------------*/
/* pragma Directives                                                          */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Definitions (avoid as much as possible!)                  */
/*----------------------------------------------------------------------------*/

/**
\brief	variable to save the current device function in progress
*/

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

/*----------------------------------------------------------------------------*/
/* File local Code Macros                                                     */
/*----------------------------------------------------------------------------*/
#define TIMER_HU_TIMEOUT_MAX    3000    /* max timeout ACK received */
#define TIMER_OFFSET_HU_OFF     1500    /* if Headunit is off => offset */
#define TIMER_HU_ACK_DELAY      500     /* normal Headunit ACK delay */
#define TIMER_HU_ACK_SET        500     /* mute is active */
#define TIMER_CB_APPL           50      /* internal switch */
#define TIMER_RELEASE_DELAY     2500    /* release delay mute */
/*----------------------------------------------------------------------------*/
/* File local Variables                                                       */
/*----------------------------------------------------------------------------*/
AppDeviceAudioManagerType gAudioManagerContext;
/*----------------------------------------------------------------------------*/
/* File local const Definitions                                               */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private Address Calculations                                               */
/*----------------------------------------------------------------------------*/
static void _APP_DEVICE_AM_vSendDataToCan(APP_DEVICE_AM_AudioStatusType status, APP_CAN_enSourceIdType source);
static void _APP_DEVICE_AM_vOustedApllication(APP_CAN_enSourceIdType src);
static Uint16Type _APP_DEVICE_AM_u16TimeoutTime(void);



/*
DEBUG_STATE_ENUM_NAME='APP_CAN_enSourceIdType'   DEBUG_STATE_NAME='APP_CAN_enTelephoneCall     '   DEBUG_STATE_NUMBER='0'
DEBUG_STATE_ENUM_NAME='APP_CAN_enSourceIdType'   DEBUG_STATE_NAME='APP_CAN_enVoicePrompts      '   DEBUG_STATE_NUMBER='1'
DEBUG_STATE_ENUM_NAME='APP_CAN_enSourceIdType'   DEBUG_STATE_NAME='APP_CAN_enEmergencyCall     '   DEBUG_STATE_NUMBER='2'
DEBUG_STATE_ENUM_NAME='APP_CAN_enSourceIdType'   DEBUG_STATE_NAME='APP_CAN_enNavigation        '   DEBUG_STATE_NUMBER='3'
DEBUG_STATE_ENUM_NAME='APP_CAN_enSourceIdType'   DEBUG_STATE_NAME='APP_CAN_enOperateCall       '   DEBUG_STATE_NUMBER='4'
DEBUG_STATE_ENUM_NAME='APP_CAN_enSourceIdType'   DEBUG_STATE_NAME='APP_CAN_enRing              '   DEBUG_STATE_NUMBER='5'
DEBUG_STATE_ENUM_NAME='APP_CAN_enSourceIdType'   DEBUG_STATE_NAME='APP_CAN_enBroadBandCall     '   DEBUG_STATE_NUMBER='6'
DEBUG_STATE_ENUM_NAME='APP_CAN_enSourceIdType'   DEBUG_STATE_NAME='APP_CAN_enAudioStream       '   DEBUG_STATE_NUMBER='7'
DEBUG_STATE_ENUM_NAME='APP_CAN_enSourceIdType'   DEBUG_STATE_NAME='APP_CAN_enInvalidSource     '   DEBUG_STATE_NUMBER='255'
*/


/*----------------------------------------------------------------------------*/
/* Function    : APP_DEVICE_sfInitApp                                         */
/**
    \brief      Callback function for initialization of App DEVICE

    \return     SuccessFailType
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
void APP_DEVICE_AudioManager_sfInitApp(void)
{
    (void)memset(&gAudioManagerContext, 0, sizeof(gAudioManagerContext));
    gAudioManagerContext.enActiveSource = APP_CAN_enInvalidSource;

    EEPROM_LAYOUT_vGetCooperativeMuteType((Uint8Type *)&gAudioManagerContext.enCooperativeMute);
    EEPROM_enGetMuteDelay((Uint16Type *)&gAudioManagerContext.muteDelay);
    (void)_APP_DEVICE_AM_vSendDataToCan(APP_DEVICE_AM_enAudioUnmute, APP_CAN_enInvalidSource);
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_DEVICE_AM_sfGetMute                                      */
/**
    \brief      Get HU mute

    \param      cb
                APP_COMMON_enResponseCode

    \param		source
    			APP_CAN_enSourceIdType

    \param      line
                Uint32Type

    \return     SuccessFailType
   */
/*----------------------------------------------------------------------------*/
SuccessFailType APP_DEVICE_AM_sfGetMute(void (*cb)(APP_COMMON_enResponseCode),
                                        APP_CAN_enSourceIdType source,
                                        Uint32Type line)
{
    SuccessFailType sF = SUCCESS;

    /* no source active */
    if (gAudioManagerContext.enActiveSource == APP_CAN_enInvalidSource)
    {
        DEBUG_STATE1(APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "GetMute: source $APP_CAN_enSourceIdType$=%d", source );
        AppTask_vSetAlarm(APP_TASK_ALARM_APP_DEVICE_CAN_ACK,
                        MSEC(_APP_DEVICE_AM_u16TimeoutTime()),
                        APP_DEVICE_AM_cbWaitForCANAck);
        (void)_APP_DEVICE_AM_vSendDataToCan(APP_DEVICE_AM_enAudioMute,
                        source);
    }
    /* active source was ousted through new source */
    else if((gAudioManagerContext.enActiveSource > source) ||
            ((APP_CAN_enRing == source) && (APP_CAN_enVoicePrompts == gAudioManagerContext.enActiveSource)&&(APP_SD_CONTEXT_S642 != APP_SD_getCurrentCtx())))//RT13185 Needs to verify the current context
    {
        APP_CAN_enSourceIdType oldSource = gAudioManagerContext.enActiveSource;

        DEBUG_STATE2(APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "GetMute: ousted application through $APP_CAN_enSourceIdType$ = %d, active src $APP_CAN_enSourceIdType$ = %d",
                     source, gAudioManagerContext.enActiveSource);
        if(gAudioManagerContext.callbackGet != NULL)
        {
            gAudioManagerContext.callbackGet(APP_COMMON_RSP_ERROR);
            gAudioManagerContext.callbackGet = NULL;
        }

        /* it's important to change the active source here, so that the releaseMute will have no effect */
        gAudioManagerContext.enActiveSource = source;

        _APP_DEVICE_AM_vOustedApllication(oldSource);

        if(AT_AUDIO_STATE_NO_ACTIVE == APP_DEVICE_GetPASAEvent())
        {
            DEBUG_TEXT(APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "GetMute: already Pasa:0");
            AppTask_vSetAlarm( APP_TASK_ALARM_APP_DEVICE_CAN_ACK,
                               MSEC(_APP_DEVICE_AM_u16TimeoutTime()),
                               APP_DEVICE_AM_cbWaitForCANAck);
            (void)_APP_DEVICE_AM_vSendDataToCan(APP_DEVICE_AM_enAudioMute,
                                gAudioManagerContext.enActiveSource);
        }
        /* special mute handling for HU RNS315
           The incoming call with ringtone is in this HU only mono
           Therefore is the ringtone mute source 5 and the call 1*/
        else if((AT_AUDIO_STATE_COMMUNICATION == APP_DEVICE_GetPASAEvent()) &&
                (APP_CAN_enTelephoneCall == gAudioManagerContext.enActiveSource))
        {

            DEBUG_TEXT(APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "GetMute: already Pasa:2");
            AppTask_vSetAlarm( APP_TASK_ALARM_APP_DEVICE_CAN_ACK,
                               MSEC(_APP_DEVICE_AM_u16TimeoutTime()),
                               APP_DEVICE_AM_cbWaitForCANAck);
            (void)_APP_DEVICE_AM_vSendDataToCan(APP_DEVICE_AM_enAudioMute,
                                gAudioManagerContext.enActiveSource);
        }
        else if((AT_AUDIO_STATE_CK5050 == APP_DEVICE_GetPASAEvent()) &&
                (APP_CAN_enRing == gAudioManagerContext.enActiveSource))
        {

            DEBUG_TEXT(APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "GetMute: already Pasa:1");
            AppTask_vSetAlarm( APP_TASK_ALARM_APP_DEVICE_CAN_ACK,
                               MSEC(_APP_DEVICE_AM_u16TimeoutTime()),
                               APP_DEVICE_AM_cbWaitForCANAck);
            (void)_APP_DEVICE_AM_vSendDataToCan(APP_DEVICE_AM_enAudioMute,
                                gAudioManagerContext.enActiveSource);
        }
        else
        {
            DEBUG_TEXT(APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "GetMute: wait for Pasa");
            gAudioManagerContext.waitForPasa   = BT_TRUE;
        }
    }
    /* active source is equal requested source */
    else if (gAudioManagerContext.enActiveSource == source)
    {
        DEBUG_STATE1(APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "GetMute: equal source $APP_CAN_enSourceIdType$=%d", source);
        AppTask_vSetAlarm(APP_TASK_ALARM_APP_DEVICE_CAN_ACK,
                            MSEC(TIMER_CB_APPL),
                            APP_DEVICE_AM_cbWaitForCANAck);
    }
    /* requested source is lower than active source */
    else
    {
        /* special mute handling for HU RNS315
           The incoming call with ringtone is in this HU only mono
           Therefore is the ringtone mute source 5 and the call 1*/
//        if((AT_AUDIO_STATE_SIVR_PROMPTS == APP_DEVICE_GetPASAEvent()) &&
//            (APP_CAN_enRing == source) &&
//            (APP_CAN_enVoicePrompts == gAudioManagerContext.enActiveSource))
//        {
//            DEBUG_TEXT(APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "GetMute: Special handling for incoming call by active VCR");
//            AppTask_vSetAlarm(APP_TASK_ALARM_APP_DEVICE_CAN_ACK,
//                            MSEC(_APP_DEVICE_AM_u16TimeoutTime()),
//                            APP_DEVICE_AM_cbWaitForCANAck);
//            (void)_APP_DEVICE_AM_vSendDataToCan(APP_DEVICE_AM_enAudioMute,
//                            source);
//        }
//        else
//        {
            DEBUG_STATE2(APP_DEVICE_SWI_WNG, MOD_APP_DEVICE, "GetMute: active src $APP_CAN_enSourceIdType$=%d < request src $APP_CAN_enSourceIdType$=%d",
                         gAudioManagerContext.enActiveSource, source);
            sF = FAIL;
//        }
    }

    if(sF == SUCCESS)
    {
        /* PLEASE CHECK ALWAYS -> is any job in pending and waiting for call back invocation by audio manager
         * It is dangerous to overwrite this call back without to check it is not pointing to zero*/


        if(gAudioManagerContext.callbackGet != NULL)
        {
            DEBUG_TEXT(APP_DEVICE_SWI_TRC, MOD_APP_DEVICE,"GetMute:Call back is not pointing to zero, notify the application first before overwriting it!!!");
            /* inform the application in pending with error */
			/* changed on 12.04.2011 by nikolov, RT:10389 */
            /* if check inserted by maennel on 01.09.2011, RT: 8441*/
            if (gAudioManagerContext.enActiveSource < source)
            {
                gAudioManagerContext.callbackGet(APP_COMMON_RSP_ERROR);
            }
            else
            {
                gAudioManagerContext.callbackGet(APP_COMMON_RSP_OK);
            }
        }
        gAudioManagerContext.callbackGet = cb;
        gAudioManagerContext.enActiveSource = source;

        if(source != APP_CAN_enAudioStream)
        {
            /* always mute player */
            APP_MDI_bMutePlayer(APP_MDI_MUTE_AUDIO_MANAGER);
        }
        else
        {
            /* A2DP streaming and player active, unlock the audio source (only possible when receiving DGPS)*/
            (void)APP_MDI_bUnlockPlayer();
        }

        AppTask_vClearAlarm( APP_TASK_ALARM_APP_DEVICE_RELEASE_MUTE);
    }


    /* check if cooperative mute active */
	return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_DEVICE_AM_vReleaseMute                                   */
/**
    \brief      Release mute for this application

    \param      cb
                APP_COMMON_enResponseCode

    \param		source
    			APP_CAN_enSourceIdType

    \param      line
                Uint32Type

    \return     void
   */
/*----------------------------------------------------------------------------*/
void APP_DEVICE_AM_vReleaseMute(void (*cb)(APP_COMMON_enResponseCode),
                                APP_CAN_enSourceIdType source,
                                Uint32Type line)
{
    if ( APP_CAN_enForceUnmuteAtShutDown == source )
    {
        //APP_MDI_bMutePlayer(APP_MDI_MUTE_AUDIO_MANAGER);
    	gAudioManagerContext.enActiveSource = APP_CAN_enInvalidSource;
    	AppTask_vSetAlarm( APP_TASK_ALARM_APP_DEVICE_RELEASE_MUTE,
                           MSEC(0),
                           APP_DEVICE_AM_cbReleaseCanMute);
    }
    else if((gAudioManagerContext.enActiveSource == source) ||
       ((APP_CAN_enRing == gAudioManagerContext.enActiveSource) && (APP_CAN_enTelephoneCall == source)))
    {
        if ((APP_CAN_enRing == gAudioManagerContext.enActiveSource) && (APP_CAN_enTelephoneCall == source))
        {
            DEBUG_TEXT(APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "ReleaseMute: Special handling for incoming call by active VCR");
        }
        else
        {
            DEBUG_STATE2(APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "ReleaseMute: active src $APP_CAN_enSourceIdType$=%d == request src $APP_CAN_enSourceIdType$=%d",
                         gAudioManagerContext.enActiveSource, source);
        }
        if(gAudioManagerContext.callbackRelease != NULL)
        {
            gAudioManagerContext.callbackRelease(APP_COMMON_RSP_ERROR);
            gAudioManagerContext.callbackRelease = NULL;
        }
        /* always unmute player */
        if( source != APP_CAN_enAudioStream)
        {
			/* if unmuting after phone call, wait for 2,5 seconds */
			/* changed on 12.04.2011 by nikolov, RT:10389 */
            if(APP_CAN_enTelephoneCall == source)
            {
                APP_MDI_bUnmutePlayer(APP_MDI_MUTE_AUDIO_MANAGER, UNMUTE_DELAYED_AFTER_CALL);
            }
            else
            {
                APP_MDI_bUnmutePlayer(APP_MDI_MUTE_AUDIO_MANAGER, UNMUTE_IMMEDIATELY);
            }
        }

        /* player is not going to start playback*/
        AppTask_vSetAlarm( APP_TASK_ALARM_APP_DEVICE_RELEASE_MUTE,
                           MSEC(TIMER_RELEASE_DELAY),
                           APP_DEVICE_AM_cbReleaseCanMute);

        gAudioManagerContext.callbackRelease = cb;
        gAudioManagerContext.enActiveSource = APP_CAN_enInvalidSource;

    }
    else
    {
        DEBUG_STATE2(APP_DEVICE_SWI_WNG, MOD_APP_DEVICE, "ReleaseMute: active src $APP_CAN_enSourceIdType$=%d != request src $APP_CAN_enSourceIdType$=%d",
                     gAudioManagerContext.enActiveSource, source);
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_DEVICE_AM_vSendMuteOut                                  */
/**
    \brief      Release mute for this application

    \return     void
   */
/*----------------------------------------------------------------------------*/
static Uint16Type _APP_DEVICE_AM_u16TimeoutTime(void)
{
    Uint16Type delayTime;
    if (APP_DEVICE_AM_enCooperativeMute == gAudioManagerContext.enCooperativeMute)
    {
        /* start timer for CAN ACK */
        DEBUG_VALUE1(APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "TimeoutTime: cooperative mute active radio: %d",
                     gAudioManagerContext.radioOnOff);
        if(gAudioManagerContext.radioCanAck == ON)
        {
            //delayTime = TIMER_HU_ACK_SET;
            DEBUG_TEXT(APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "TimeoutTime: Radio ACK already received");
            delayTime = TIMER_HU_ACK_SET;   //RT #5788
        }
        else
        {
            /* check if radio is ON */
            if (gAudioManagerContext.radioOnOff == OFF)
            {
                //delayTime = TIMER_HU_ACK_DELAY + TIMER_OFFSET_HU_OFF;
                delayTime = TIMER_HU_TIMEOUT_MAX; //RT #5788
            }
            else
            {
                //delayTime = TIMER_HU_ACK_DELAY;
                delayTime = TIMER_HU_TIMEOUT_MAX; //RT #5788
            }
        }
    }
    else
    {
        /* start timer without CAN ACK */
        DEBUG_VALUE1(APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "TimeoutTime: cooperative mute inactive radio: %d",
                     gAudioManagerContext.radioOnOff);
        /* check if radio is ON */
        if (gAudioManagerContext.radioOnOff == OFF)
        {
            delayTime = TIMER_OFFSET_HU_OFF + gAudioManagerContext.muteDelay;
        }
        else
        {
            delayTime = TIMER_CB_APPL + gAudioManagerContext.muteDelay;
        }
    }

    DEBUG_VALUE1(APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "TimeoutTime: return delayTime %d",
                 delayTime);

    return delayTime;
}


/*----------------------------------------------------------------------------*/
/* Function    : APP_DEVICE_AM_vPasaReceived                                  */
/**
    \brief

    \param      state
				ATAudioSourceActStateEnum
    \return     void
   */
/*----------------------------------------------------------------------------*/
void APP_DEVICE_AM_vHandlePASA(ATAudioSourceActStateEnum pasa)
{
    switch(pasa)
    {
        case AT_AUDIO_STATE_NO_ACTIVE:
            if(gAudioManagerContext.waitForPasa == BT_TRUE)
            {
                gAudioManagerContext.waitForPasa = BT_FALSE;
                DEBUG_TEXT(APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "Pasa:0 received");
                AppTask_vSetAlarm( APP_TASK_ALARM_APP_DEVICE_CAN_ACK,
                                   MSEC(_APP_DEVICE_AM_u16TimeoutTime()),
                                   APP_DEVICE_AM_cbWaitForCANAck);
                (void)_APP_DEVICE_AM_vSendDataToCan(APP_DEVICE_AM_enAudioMute,
                                gAudioManagerContext.enActiveSource);
            }
            break;

        case AT_AUDIO_STATE_CK5050:
        case AT_AUDIO_STATE_COMMUNICATION: //RT:8901
            if(gAudioManagerContext.waitForPasa == BT_TRUE)
            {
               gAudioManagerContext.waitForPasa = BT_FALSE;
               DEBUG_VALUE1(APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "Pasa:%d received", pasa);
               AppTask_vSetAlarm( APP_TASK_ALARM_APP_DEVICE_CAN_ACK,
                                  MSEC(_APP_DEVICE_AM_u16TimeoutTime()),
                                  APP_DEVICE_AM_cbWaitForCANAck);
               (void)_APP_DEVICE_AM_vSendDataToCan(APP_DEVICE_AM_enAudioMute,
                               gAudioManagerContext.enActiveSource);
            }
            break;

        case AT_AUDIO_STATE_STREAMING_A2DP:
            /*RT#6209 Adaptation "Lautstärke Medienwiedergabe" failed*/
            ERM_vOnAppDeviceNotification(APP_DEVICE_NOTIFY_A2DP_STREAMING_ACTIVE);
            break;

        default:
            /* ignore other events */
            break;
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_DEVICE_AM_vOnAppCshdlNotification                        */
/**
    \brief      to shut down sending mute commands in case of shutdown.

    \brief

    \param      notification
				APP_CSHDL_NotificationType
    \return     void
   */
/*----------------------------------------------------------------------------*/
void APP_DEVICE_AM_vOnAppCshdlNotification(APP_CSHDL_NotificationType notification)
{
    switch(notification)
    {
        case APP_CSHDL_NOTIFY_WAIT_FOR_SHUTDOWN:
        {
        	/* on this notification if mute is active it should be deactivated*/
			DEBUG_VALUE1(APP_DEVICE_SWI_WNG, MOD_APP_DEVICE, "vOnAppCshdl: Mute in Shutdown state: %d",
						 gAudioManagerContext.enActiveSource );

			APP_DEVICE_AM_vReleaseMute(NULL,APP_CAN_enForceUnmuteAtShutDown,__LINE__);
            break;
        }
        default:
            /* ignore notification */
            break;
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_DEVICE_AM_vSetMuteDelayDiagnos                           */
/**
    \brief      store the Audio Mute Delay in the static variable acc to WIRE or CAN

    \param      u8MuteDelayValue
    			Uint8Type
    			The value send is in milliseconds i.e 0x64 means 100ms

    \return     void
   */
/*----------------------------------------------------------------------------*/
void APP_DEVICE_AM_vSetMuteDelayDiag(Uint16Type u16MuteDelayValue)
{
	gAudioManagerContext.muteDelay = u16MuteDelayValue;
    DEBUG_VALUE1(APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "Mute Delay = %d msecs",
                 gAudioManagerContext.muteDelay );
}


/*----------------------------------------------------------------------------*/
/* Function    : APP_DEVICE_AM_vSetCooperativeMuteDiag                        */
/**
    \brief      Set Coperative Mute in the static variable

    \param      u8MuteDelayValue
    			Uint8Type
    			The value send is in milliseconds i.e 0x64 means 100ms

    \return     void
   */
/*----------------------------------------------------------------------------*/
void APP_DEVICE_AM_vSetCooperativeMuteDiag(Uint8Type u8CooperativeMuteValue)
{
	 gAudioManagerContext.enCooperativeMute = (APP_DEVICE_AM_CooperativeMuteType)u8CooperativeMuteValue;
    DEBUG_VALUE1(APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "Cooperative Mute = %d",
                 gAudioManagerContext.enCooperativeMute );
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_DEVICE_AM_vSendDataToCan                                */
/**
    \brief

    \param      APP_DEVICE_AM_MuteType
    			APP_DEVICE_AM_enMuteByCAN or APP_DEVICE_AM_enMuteByWire

    \param      APP_CAN_enSourceIdType

    \return     void
   */
/*----------------------------------------------------------------------------*/
static void _APP_DEVICE_AM_vSendDataToCan(APP_DEVICE_AM_AudioStatusType status, APP_CAN_enSourceIdType source)
{
    gAudioManagerContext.sendMuteDataToCan.common.id = APP_PHONE_CAN_PHONE_MUTE;
    gAudioManagerContext.sendMuteDataToCan.muteBehavior.muteUnmute = (Uint8Type)status;
    gAudioManagerContext.sendMuteDataToCan.muteBehavior.signalSource = source;
    (void)SendMessage(msgTelefonStates, &gAudioManagerContext.sendMuteDataToCan);

    /* set always hardware mute if CAN mute is active */
    if (APP_DEVICE_AM_enAudioUnmute == status)
    {
	    APP_DEVICE_AM_MUTE_OFF_BY_WIRE
	    DEBUG_TEXT(APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "SendDataToCan wire unMUTE");
    }
    else /* if ( APP_DEVICE_AM_enAudioMute == status) */
    {
	    APP_DEVICE_AM_MUTE_ON_BY_WIRE
	    DEBUG_TEXT(APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "SendDataToCan wire MUTE");
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_DEVICE_AM_cbWaitForCANAck                                */
/**
    \brief

    \param      APP_DEVICE_AM_MuteType
    			APP_DEVICE_AM_enMuteByCAN or APP_DEVICE_AM_enMuteByWire

    \param      APP_CAN_enSourceIdType

    \return     void
   */
/*----------------------------------------------------------------------------*/
void APP_DEVICE_AM_cbWaitForCANAck()
{
    DEBUG_TEXT(APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "cbWaitForCANAck: called" );

    if (gAudioManagerContext.callbackGet)
    {
        gAudioManagerContext.callbackGet(APP_COMMON_RSP_OK);
        gAudioManagerContext.callbackGet = NULL;
    }
}


/*----------------------------------------------------------------------------*/
/* Function    : APP_DEVICE_AM_cbReleaseCanMute                                */
/**
    \brief

    \return     void
   */
/*----------------------------------------------------------------------------*/
void APP_DEVICE_AM_cbReleaseCanMute()
{
    DEBUG_TEXT(APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "cbReleaseCanMute: called" );
    (void)_APP_DEVICE_AM_vSendDataToCan(APP_DEVICE_AM_enAudioUnmute, APP_CAN_enInvalidSource);
    if (gAudioManagerContext.callbackRelease)
    {
        gAudioManagerContext.callbackRelease(APP_COMMON_RSP_OK);
        gAudioManagerContext.callbackRelease = NULL;
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_DEVICE_AM_vCanAckReceived                                */
/**
    \brief

    \param      APP_DEVICE_AM_MuteType
    			APP_DEVICE_AM_enMuteByCAN or APP_DEVICE_AM_enMuteByWire

    \param      APP_CAN_enSourceIdType

    \return     void
   */
/*----------------------------------------------------------------------------*/
void APP_DEVICE_AM_vRadioInfoReceived(APP_CAN_RadioType radioInfo)
{
    gAudioManagerContext.radioCanAck    = radioInfo.mute;
    gAudioManagerContext.radioOnOff     = radioInfo.onOff;
    //gAudioManagerContext.enMuteType     = radioInfo.muteType;
    if (APP_DEVICE_AM_enCooperativeMute == gAudioManagerContext.enCooperativeMute)
    {
        if (1 == gAudioManagerContext.radioCanAck)
        {
            DEBUG_TEXT(APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "Radio ACK received" );
            if(gAudioManagerContext.callbackGet)
            {
                AppTask_vClearAlarm(APP_TASK_ALARM_APP_DEVICE_CAN_ACK);
                gAudioManagerContext.callbackGet(APP_COMMON_RSP_OK);
                gAudioManagerContext.callbackGet = NULL;
            }
        }
    }
    else
    {
        DEBUG_TEXT(APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "Reject Radio ACK: not coded by Diagnose" );
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_DEVICE_AM_vOustedApllication                            */
/**
    \brief

    \param      APP_CAN_enSourceIdType
    			all defined enum types

    \return     void
   */
/*----------------------------------------------------------------------------*/
static void _APP_DEVICE_AM_vOustedApllication(APP_CAN_enSourceIdType src)
{
    switch (src)
    {
        case APP_CAN_enVoicePrompts:
        {
            const ATRspCallStatValType *plcc = APP_PHONE_AT_PNCC_pGetPlccContext(AT_CALL_INDEX_0);   // RT #8005
            /* info to VCR */
            // RT #3713
            // send stop command to VCR that no text will be spoken during outgoing call
            if(gAudioManagerContext.enActiveSource==APP_CAN_enRing)//RT13185
            {
                VCR_vStopSIVRForCall();
            }
            else
            {
                VCR_vStopSIVRForCall();
                if (plcc == NULL)
                {
                    APP_SD_vOnAppDeviceNotification(APP_DEVICE_NOTIFY_OUTGOING_CALL_VIA_VCR);
                }
            }
            break;
        }
        case APP_CAN_enAudioStream:
            /* MDI is always muted in GetMute() */
            break;
        case APP_CAN_enRing:
            if( APP_SD_CONTEXT_S642 == APP_SD_getCurrentCtx())
            {
                (void)APP_DEVICE_rcStopRingtoneNonBlock();
                _APP_SD_vnotifiedRingtonestate(); //RT14853
                DEBUG_STATE1(APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "OustedApplication: Stop Ringtone activeSource $APP_CAN_enSourceIdType$=%d",
                            gAudioManagerContext.enActiveSource);
            }
            break;
        default:
            break;
    }
}


/*----------------------------------------------------------------------------*/
/* Function    : APP_DEVICE_AM_sfSetDiagAudioMuteByWire                       */
/**
    \brief      to set events for Audio Mute and demute. This is to be only used
    			in case when no CAN signals are available for example old Radios etc.

    \param      enMuteValue
    			APP_CAN_enAudioStatusType

    \return     void
   */
/*----------------------------------------------------------------------------*/
void APP_DEVICE_AM_vSetAudioMuteByWireDiag(APP_CAN_enAudioStatusType enMuteValue)
{
	if ( APP_CAN_enAudioUnmute == enMuteValue )
	{
		APP_DEVICE_AM_MUTE_OFF_BY_WIRE
		DEBUG_TEXT(APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "SetDiagAudioMuteByWire unMUTE successful");
	}
	else /* if ( APP_CAN_enAudioMute == enMuteValue) */
	{
		APP_DEVICE_AM_MUTE_ON_BY_WIRE
		DEBUG_TEXT(APP_DEVICE_SWI_TRC, MOD_APP_DEVICE, "SetDiagAudioMuteByWire MUTE successful");
	}
}

/*----------------------------------------------------------------------------*/
/* Function :   APP_DEVICE_AMGetActiveSource                                      */
/**
  \brief        Get actiev Audio Source of Audio manager context

  \param        none


  \return       current audio source

 */
/*----------------------------------------------------------------------------*/
APP_CAN_enSourceIdType APP_DEVICE_AMGetActiveSource(void)
{
    return (gAudioManagerContext.enActiveSource);
}

