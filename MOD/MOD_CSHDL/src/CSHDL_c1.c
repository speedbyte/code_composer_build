/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name: CSHDL_c1.c
*-----------------------------------------------------------------------------
* Module Name: CSHDL (Central state handler)
*-----------------------------------------------------------------------------
* Description: managing the system states for the TMS470 and Bluetooth module
*-----------------------------------------------------------------------------
* $Date: 2011-10-20 18:50:42 +0200 (Do, 20 Okt 2011) $

* $Rev: 22583 $
* $Author: nikolov $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_CSHDL/trunk/src/CSHDL_c1.c $

*-----------------------------------------------------------------------------
*/
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include <osek.h>

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "Common.h"
#include "Startup.h"
#include "ERM.h"
#include "CSHDL.h"
#include "CSHDL_CI.h"
#include "GIO.h"
#include "PORT.h"
#include "can_inc.h"
#include "APP_CAN.h"
#include "deh.h"
#include "sds_ext.h"
#include "DEH_DTC_CFG.h"
#include "VCR_Utils.h"
#include "WD_c1.h"
#include "CSHDL_c1.id"
#include "TRACE.h"


/*----------------------------------------------------------------------------*/
/* pragma Directives                                                          */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Definitions (avoid as much as possible!)                  */
/*----------------------------------------------------------------------------*/
CSHDL_GesSystemStateType     cshdlGesamtSystemState;
CSHDL_CANStateType           cshdlCANState;
CSHDL_CK5050StateType        cshdlCK5050State;
static CSHDL_SystemInStateType      cshdlMainSystemInState;
static CSHDL_SystemInStateType      cshdlCANSystemInState;
static CSHDL_SystemInStateType      cshdlCK5050SystemInState;
static CSHDL_enIGNITION_OFF_DELAY_States  cshdlIgnitionOFFDelayState;

/*----------------------------------------------------------------------------*/
/* Private Manifest Constant Definitions                                      */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Private Types                                                              */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Prototypes of file local Methods                                           */
/*----------------------------------------------------------------------------*/
static void SystemPowerdown(void);
static void _CSHDL_InfoFromPowerManagerHandler(void);
static SuccessFailType CSHDL_CANoff_s0(EventMaskType event);
static SuccessFailType CSHDL_CANinit_s1(EventMaskType event);
static SuccessFailType CSHDL_CANready_s2(EventMaskType event);
static SuccessFailType CSHDL_CK5050off_s0(EventMaskType event);
static SuccessFailType CSHDL_CK5050init_s1(EventMaskType event);
static SuccessFailType CSHDL_CK5050ready_s2(EventMaskType event);
static SuccessFailType CSHDL_SYSinit_s1(EventMaskType event);
static SuccessFailType CSHDL_TMS470ready_s2(EventMaskType event);
static SuccessFailType CSHDL_SYSrun_s3(EventMaskType event);
static SuccessFailType CSHDL_BTSW_Update_s4(EventMaskType event);
static SuccessFailType CSHDL_Gesamtsystem(EventMaskType eventMask);
static SuccessFailType CSHDL_CAN(EventMaskType eventMask);
static SuccessFailType CSHDL_CK5050(EventMaskType eventMask);
static void CSHDL_vSendSystemStateToApp(CSHDL_enSystemStates state, Uint32Type line);
static void CSHDL_vCK5050_HardwareStart (void);
static void CSHDL_vCK5050_HardwareStop (void);
static void CSHDL_vCK5050_SetNewState (CSHDL_CK5050StateType NewCK5050State);
#if defined(CRADLE_RESTRICTION_ACTIVE)
static void CSHDL_vCradleConnectivity_Debouncing (void);
static void CSHDL_vCradleRestrictionStateMachineChangeState (CSHDL_enCradleRestriction_Internal_States newState);
static void CSHDL_vCradleRestrictionStateMachine (void);
static BooleanType CSHDL_bCradleRestrictionGetBluetoothAllowed(void);
#endif


/*----------------------------------------------------------------------------*/
/* File local Code Macros                                                     */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* File local Variables                                                       */
/*----------------------------------------------------------------------------*/
static PWMGR_enVoltageStates    cshdlVoltageState;
static Uint8Type AppTaskCK5050_OFF_ACK;
static Uint8Type PrepareCK5050_OFF_Sent;
static TickType  cshdlIgnitionOffDelay_StartTime;
static TickType  PrepareCK5050_OFF_SentTime;
static TickType diffTime = 0;
//static CSHDL_enSystemStates SystemStates;
static Uint8Type _CSHDL_BluetoothUpdateTrigger;

//static Uint8Type _CSHDL_sysPerfTraceDone;
static Uint8Type _CSHDL_StartCkOnOff = 0;

CanSignalMessageType     cshdlCanSignalInfo;

#if defined(CRADLE_RESTRICTION_ACTIVE)
static CSHDL_enCradleRestriction_Internal_States  _CSHDL_CradleRestriction_State;
static CSHDL_enCradleRestriction_Internal_States  _CSHDL_CradleRestriction_oldState;
static TickType  _CSHDL_CradleRestriction_StateTime;
static Uint32Type _CSHDL_CradleConnectivity_Debounce;
static CSHDL_enCK5050_AT_Communication_States _CSHDL_CK5050_AT_Interface;
static CSHDL_enSystemStates _CSHDL_CradleRestrictionSendNotify;
#endif
/*----------------------------------------------------------------------------*/
/* File local const Definitions                                               */
/*----------------------------------------------------------------------------*/
#define RADIO_ONOFF_INFO        cshdlCanSignalInfo.CanSignalMessageBit.Radio.onOffState
#define KL_15_INFO              cshdlCanSignalInfo.CanSignalMessageBit.BSG3.klemme15
#define KL_S_INFO               cshdlCanSignalInfo.CanSignalMessageBit.BSG3.klemmeS
#define INFOTAINMENT_WARN_INFO  cshdlCanSignalInfo.CanSignalMessageBit.BSG2.warnInfo
#define INFOTAINMENT_AUS_INFO   cshdlCanSignalInfo.CanSignalMessageBit.BSG2.ausInfo
#define REMOTE_INFO             cshdlCanSignalInfo.CanSignalMessageBit.Remote.onOffState
/*----------------------------------------------------------------------------*/
/* Private Address Calculations                                               */
/*----------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------*/
/* Function    : CSHDL_vGetCkOnOffStatus                                      */
/**
    \brief      Get the current reset status of the CK5050

    \return     Uint8Type
                1 - CK5050+ out of Reset
                0 - CK5050+ in reset
*/
/*----------------------------------------------------------------------------*/
Uint8Type CSHDL_vGetCkOnOffStatus(void)
{
    return (_CSHDL_StartCkOnOff);
}

/*----------------------------------------------------------------------------*/
/* Function    : CSHDL_init                                                   */
/**
    \brief      Central state handler function to initialize the CSHDL variables

    \brief      Description:
                function to init the system status and the system variables
                flow

    \param      void
    \return     SuccessFailType
   */
/*----------------------------------------------------------------------------*/
SuccessFailType CSHDL_init(void)
{
    SuccessFailType sF = SUCCESS;

    cshdlCanSignalInfo.instr = 0;

    cshdlVoltageState = PWMGR_NormalVoltage;
    cshdlGesamtSystemState = CSHDL_SYSinit;
    cshdlCANState = CSHDL_CANinit;
    cshdlCK5050State = CSHDL_CK5050off;
    cshdlMainSystemInState = construct;
    cshdlCANSystemInState = stat;
    cshdlCK5050SystemInState = construct;
    AppTaskCK5050_OFF_ACK = FALSE;
    PrepareCK5050_OFF_Sent = FALSE;
    _CSHDL_BluetoothUpdateTrigger = 0;
//    _CSHDL_sysPerfTraceDone = 0;
    cshdlIgnitionOFFDelayState = IGNITION_OFF_DELAY_NOT_STARTED;

    _CSHDL_StartCkOnOff = 0;
#if defined(CRADLE_RESTRICTION_ACTIVE)
    _CSHDL_CradleRestriction_State = CRADLE_RESTRICTION_INIT;
    _CSHDL_CradleRestriction_oldState = CRADLE_RESTRICTION_INIT;
    _CSHDL_CradleRestriction_StateTime = osGetSystemCounter();
    _CSHDL_CradleConnectivity_Debounce = 0;
    _CSHDL_CK5050_AT_Interface = CK5050_AT_COMMUNICATION_NOT_STARTED;
    _CSHDL_CradleRestrictionSendNotify = INVALID_SYSTEM_STATE;
#endif

    return sF;
}

/*
DEBUG_STATE_ENUM_NAME='CSHDL_GesSystemStateType' DEBUG_STATE_NAME='CSHDL_Poff'          DEBUG_STATE_NUMBER='0'
DEBUG_STATE_ENUM_NAME='CSHDL_GesSystemStateType' DEBUG_STATE_NAME='CSHDL_SYSinit'       DEBUG_STATE_NUMBER='1'
DEBUG_STATE_ENUM_NAME='CSHDL_GesSystemStateType' DEBUG_STATE_NAME='CSHDL_TMS470ready'   DEBUG_STATE_NUMBER='2'
DEBUG_STATE_ENUM_NAME='CSHDL_GesSystemStateType' DEBUG_STATE_NAME='CSHDL_SYSrun'        DEBUG_STATE_NUMBER='3'
DEBUG_STATE_ENUM_NAME='CSHDL_GesSystemStateType' DEBUG_STATE_NAME='CSHDL_BTSWupdate'    DEBUG_STATE_NUMBER='4'
*/

/*
DEBUG_STATE_ENUM_NAME='CSHDL_CANStateType' DEBUG_STATE_NAME='CSHDL_CANoff'          DEBUG_STATE_NUMBER='0'
DEBUG_STATE_ENUM_NAME='CSHDL_CANStateType' DEBUG_STATE_NAME='CSHDL_CANinit'         DEBUG_STATE_NUMBER='1'
DEBUG_STATE_ENUM_NAME='CSHDL_CANStateType' DEBUG_STATE_NAME='CSHDL_CANready'        DEBUG_STATE_NUMBER='2'
*/


/*
DEBUG_STATE_ENUM_NAME='CSHDL_CK5050StateType' DEBUG_STATE_NAME='CSHDL_CK5050off'          DEBUG_STATE_NUMBER='0'
DEBUG_STATE_ENUM_NAME='CSHDL_CK5050StateType' DEBUG_STATE_NAME='CSHDL_CK5050init'         DEBUG_STATE_NUMBER='1'
DEBUG_STATE_ENUM_NAME='CSHDL_CK5050StateType' DEBUG_STATE_NAME='CSHDL_CK5050ready'        DEBUG_STATE_NUMBER='2'
*/


/*
DEBUG_STATE_ENUM_NAME='CSHDL_SystemInStateType' DEBUG_STATE_NAME='construct'          DEBUG_STATE_NUMBER='0'
DEBUG_STATE_ENUM_NAME='CSHDL_SystemInStateType' DEBUG_STATE_NAME='stat'               DEBUG_STATE_NUMBER='1'
DEBUG_STATE_ENUM_NAME='CSHDL_SystemInStateType' DEBUG_STATE_NAME='destruct'           DEBUG_STATE_NUMBER='2'
*/

void CSHDL_PrintDebug()
{
    static CSHDL_GesSystemStateType     cshdlGesamtSystemState_last = CSHDL_Poff;
    static CSHDL_CANStateType           cshdlCANState_last = CSHDL_CANoff;
    static CSHDL_CK5050StateType        cshdlCK5050State_last = CSHDL_CK5050off;
    static CSHDL_SystemInStateType      cshdlMainSystemInState_last = construct;
    static CSHDL_SystemInStateType      cshdlCANSystemInState_last = construct;
    static CSHDL_SystemInStateType      cshdlCK5050SystemInState_last = construct;

    if(cshdlGesamtSystemState_last != cshdlGesamtSystemState)
    {
//        DEBUG_VALUE2(CSHDL_SWI_TRC, MOD_CSHDL, "cshdlGesamtSystemState: %d -> %d", cshdlGesamtSystemState_last, cshdlGesamtSystemState);
        DEBUG_STATE2(CSHDL_SWI_TRC, MOD_CSHDL, "GesamtSystemState: $CSHDL_GesSystemStateType$ %d -> $CSHDL_GesSystemStateType$ %d", cshdlGesamtSystemState_last, cshdlGesamtSystemState);

        cshdlGesamtSystemState_last = cshdlGesamtSystemState;
    }

    if(cshdlCANState_last != cshdlCANState)
    {
//        DEBUG_VALUE2(CSHDL_SWI_TRC, MOD_CSHDL, "cshdlCANState: %d -> %d", cshdlCANState_last, cshdlCANState);

        DEBUG_STATE2(CSHDL_SWI_TRC, MOD_CSHDL, "CANState: $CSHDL_CANStateType$ %d -> $CSHDL_CANStateType$ %d",cshdlCANState_last, cshdlCANState);
        cshdlCANState_last = cshdlCANState;
    }

    if(cshdlCK5050State_last != cshdlCK5050State)
    {
//        DEBUG_VALUE2(CSHDL_SWI_TRC, MOD_CSHDL, "cshdlCK5050State: %d -> %d", cshdlCK5050State_last, cshdlCK5050State);

        DEBUG_STATE2(CSHDL_SWI_TRC, MOD_CSHDL, "CK5050State: $CSHDL_CK5050StateType$ %d -> $CSHDL_CK5050StateType$ %d", cshdlCK5050State_last, cshdlCK5050State);
        cshdlCK5050State_last = cshdlCK5050State;
    }

    if(cshdlMainSystemInState_last != cshdlMainSystemInState)
    {
//        DEBUG_VALUE2(CSHDL_SWI_TRC, MOD_CSHDL, "cshdlMainSystemInState: %d -> %d", cshdlMainSystemInState_last, cshdlMainSystemInState);

        DEBUG_STATE2(CSHDL_SWI_TRC, MOD_CSHDL, "MainSystemInState: $CSHDL_SystemInStateType$ %d -> $CSHDL_SystemInStateType$ %d", cshdlMainSystemInState_last, cshdlMainSystemInState);

        cshdlMainSystemInState_last = cshdlMainSystemInState;
    }

    if(cshdlCANSystemInState_last != cshdlCANSystemInState)
    {
//        DEBUG_VALUE2(CSHDL_SWI_TRC, MOD_CSHDL, "cshdlCANSystemInState: %d -> %d", cshdlCANSystemInState_last, cshdlCANSystemInState);

        DEBUG_STATE2(CSHDL_SWI_TRC, MOD_CSHDL, "CANSystemInState: $CSHDL_SystemInStateType$ %d -> $CSHDL_SystemInStateType$ %d", cshdlCANSystemInState_last, cshdlCANSystemInState);

        cshdlCANSystemInState_last = cshdlCANSystemInState;
    }

    if(cshdlCK5050SystemInState_last != cshdlCK5050SystemInState)
    {
//        DEBUG_VALUE2(CSHDL_SWI_TRC, MOD_CSHDL, "cshdlCK5050SystemInState: %d -> %d", cshdlCK5050SystemInState_last, cshdlCK5050SystemInState);

        DEBUG_STATE2(CSHDL_SWI_TRC, MOD_CSHDL, "CK5050SystemInState: $CSHDL_SystemInStateType$ %d -> $CSHDL_SystemInStateType$ %d", cshdlCK5050SystemInState_last, cshdlCK5050SystemInState);

        cshdlCK5050SystemInState_last = cshdlCK5050SystemInState;
    }
}


/*----------------------------------------------------------------------------*/
/* Function    : CSHDL_manager                                                */
/**
    \brief      Central state handler to manage the system states

    \brief      Description:
                function to manage the system status and the system
                flow

    \param      void
    \return     SuccessFailType
   */
/*----------------------------------------------------------------------------*/
SuccessFailType CSHDL_manager(void)
{
    EventMaskType event;  /* received event */
    SuccessFailType sF = SUCCESS;

    /* process event */
	(void)GetEvent(OsSystemTask_event,&event);

    /* call before Gesamtystem to use the actual status of*/
#if defined(CRADLE_RESTRICTION_ACTIVE)
    CSHDL_vCradleRestrictionStateMachine();
#endif

    sF = CSHDL_Gesamtsystem(event);
    if(SUCCESS == sF)
    {

    }

    sF = CSHDL_CAN(event);
    if(SUCCESS == sF)
    {

    }

    sF = CSHDL_CK5050(event);
    if(SUCCESS == sF)
    {

    }

    CSHDL_PrintDebug();

   return(sF);
}/* END OF Function CSHDL_Manager */


/*----------------------------------------------------------------------------*/
/* Function    : CSHDL_Gesamtsystem                                           */
/**
    \brief      Central state handler to manage the main system states

    \brief      Description:
                function to manage the main system status and the system
                flow

    \param      EventMaskType
    \return     SuccessFailType
   */
/*----------------------------------------------------------------------------*/
static SuccessFailType CSHDL_Gesamtsystem(EventMaskType eventMask)
{
    SuccessFailType sF = SUCCESS;

    if (eventMask & evSystemTaskCANInfotainmentInfo)
    {
        (void)ReceiveMessage(msgCANInfotainmentInfo, &cshdlCanSignalInfo);
        if( E_COM_NOMSG == GetMessageStatus(msgCANInfotainmentInfo))
        {
            // clear only if all msg received
            (void)ClearEvent(evSystemTaskCANInfotainmentInfo);
        }
    }

    if (eventMask & evSystemTaskVoltageState)
    {
        (void)ReceiveMessage(msgPwrmgrState, &cshdlVoltageState);
        (void)ClearEvent(evSystemTaskVoltageState);
    }

    if (eventMask & evSystemTaskCallbackShutdown)
    {
        (void)ClearEvent(evSystemTaskCallbackShutdown);
        AppTaskCK5050_OFF_ACK = TRUE;
    }

    switch(cshdlGesamtSystemState)
    {
        case CSHDL_Poff:
            /* Switch Power supply off */
		    SystemPowerdown();
            break;

        /* this state is equal to UHV deactive */
        case CSHDL_SYSinit:
            sF = CSHDL_SYSinit_s1(eventMask);
            if(SUCCESS == sF)
            {

            }
            break;

        case CSHDL_TMS470ready:
            sF = CSHDL_TMS470ready_s2(eventMask);
            if(SUCCESS == sF)
            {

            }
            break;

        case CSHDL_SYSrun:
            sF = CSHDL_SYSrun_s3(eventMask);
            if(SUCCESS == sF)
            {

            }
            break;

        case CSHDL_BTSWupdate:
            sF = CSHDL_BTSW_Update_s4(eventMask);
            if(SUCCESS == sF)
            {

            }
            break;

        default:
            /* Error management */
            break;
    }

    return(sF);
}


/*----------------------------------------------------------------------------*/
/* Function    : CSHDL_CAN                                                    */
/**
    \brief      Central state handler to manage the CAN system states

    \brief      Description:
                function to manage the CAN system status and the system
                flow

    \param      EventMaskType
    \return     SuccessFailType
   */
/*----------------------------------------------------------------------------*/
static SuccessFailType CSHDL_CAN(EventMaskType eventMask)
{
    SuccessFailType sF = SUCCESS;

    switch(cshdlCANState)
    {
        case CSHDL_CANoff:
            sF = CSHDL_CANoff_s0(eventMask);
            if(SUCCESS == sF)
            {

            }
            break;

        case CSHDL_CANinit:
            sF = CSHDL_CANinit_s1(eventMask);
            if(SUCCESS == sF)
            {

            }
            break;

        case CSHDL_CANready:
            sF = CSHDL_CANready_s2(eventMask);
            if(SUCCESS == sF)
            {

            }
            break;
    }

    return(sF);
}


/*----------------------------------------------------------------------------*/
/* Function    : CSHDL_CK5050                                                 */
/**
    \brief      Central state handler to manage the CK5050 system states

    \brief      Description:
                function to manage the CK5050 system status and the system
                flow

    \param      EventMaskType
    \return     SuccessFailType
   */
/*----------------------------------------------------------------------------*/
static SuccessFailType CSHDL_CK5050(EventMaskType eventMask)
{
    SuccessFailType sF = SUCCESS;

    switch(cshdlCK5050State)
    {
        case CSHDL_CK5050off:
            sF = CSHDL_CK5050off_s0(eventMask);
            if(SUCCESS == sF)
            {

            }
            break;

        case CSHDL_CK5050init:
            sF = CSHDL_CK5050init_s1(eventMask);
            if(SUCCESS == sF)
            {

            }
            break;

        case CSHDL_CK5050ready:
            sF = CSHDL_CK5050ready_s2(eventMask);
            if(SUCCESS == sF)
            {

            }
            break;
    }

    return(sF);
}


/*----------------------------------------------------------------------------*/
/* Function    : SystemPowerdown                                                */
/**
    \brief      function to handle the system shutoff

    \brief      Description:
                function to manage the system shutoff or initiated the restart
                if Pin INH = High will be detected.

    \param      void
    \return     void
   */
/*----------------------------------------------------------------------------*/
static void SystemPowerdown(void)
{
    Uint32Type u32WdToggleTimer = 0;
    /* Switch Power supply off */
    GIO_vPinWrite(PORT_MON_PWRHOLD, LOW, GIOD);
    DEBUG_TEXT(CSHDL_SWI_TRC, MOD_CSHDL, "Pulling Power Hold to Low, Complete power off in around 100 ms !!!!");
    u32WdToggleTimer = osGetSystemCounter() + 2;
    for (;;)
    {
        if (u32WdToggleTimer <= osGetSystemCounter())
        {
            (void)WD_u8ToggleWD();
            if (TRUE == GIO_u8PinRead(PORT_CAN_INHIBIT, GIOA))
            {
                /* software reset */
                c_int00();
            }
            u32WdToggleTimer = osGetSystemCounter() + 2;
        }
    }
}

/******************************************************************************/
/******************* START CAN STATEMACHINE STATEFUNCTIONS ********************/

/*----------------------------------------------------------------------------*/
/* Function    : CSHDL_CANoff_s0                                              */
/**
    \brief      Central state handler function for CAN statemachine state CANoff

    \brief      Description:
                function to handle the actions in CAN state CANoff

    \param      EventMaskType
    \return     SuccessFailType
   */
/*----------------------------------------------------------------------------*/
static SuccessFailType CSHDL_CANoff_s0(EventMaskType event)
{
    SuccessFailType sF          = SUCCESS;
    Uint8Type       returnCode  = FALSE;

    switch(cshdlCANSystemInState)
    {
        case construct:
            /* set CAN to stop operation mode */
            returnCode = CanStop();
            if(kCanOk == returnCode)
            {
                sF = SUCCESS;
            }
            else
            {
                sF = FAIL;
            }
            cshdlCANSystemInState = stat;
            break;

        case stat:
            if(CSHDL_SYSinit == cshdlGesamtSystemState)
            {
                cshdlCANState = CSHDL_CANinit;
                cshdlCANSystemInState = construct;
            }
            break;

        case destruct:

            break;
    }
    return(sF);
}


/*----------------------------------------------------------------------------*/
/* Function    : CSHDL_CANinit_s1                                             */
/**
    \brief      Central state handler function for CAN statemachine state CANinit

    \brief      Description:
                function to handle the actions in CAN state CANinit

    \param      EventMaskType
    \return     SuccessFailType
   */
/*----------------------------------------------------------------------------*/
static SuccessFailType CSHDL_CANinit_s1(EventMaskType event)
{
    SuccessFailType sF = SUCCESS;

    switch(cshdlCANSystemInState)
    {
        case construct:
//		    sF = SYSINIT_sfSwInit();	/* initialize the Software parts of the System. */
	        if(SUCCESS == sF)
	        {
	            cshdlCANSystemInState = stat;
	        }
            break;

        case stat:
            CanOnline();	            /* activate the can bus */
            cshdlCANSystemInState = destruct;
            break;

        case destruct:
            cshdlCANState = CSHDL_CANready;
            cshdlCANSystemInState = construct;
            break;
    }
    return(sF);
}


/*----------------------------------------------------------------------------*/
/* Function    : CSHDL_CANready_s2                                            */
/**
    \brief      Central state handler function for CAN statemachine state CANready

    \brief      Description:
                function to handle the actions in CAN state CANready

    \param      EventMaskType
    \return     SuccessFailType
   */
/*----------------------------------------------------------------------------*/
static SuccessFailType CSHDL_CANready_s2(EventMaskType event)
{
    SuccessFailType sF = SUCCESS;

    switch(cshdlCANSystemInState)
    {
        case construct:
            cshdlCANSystemInState = stat;
            break;

        case stat:
//            cshdlCANSystemInState = destruct;
            break;

        case destruct:
            cshdlCANSystemInState = construct;
            break;
    }
    return(sF);
}

/******************** END CAN STATEMACHINE STATEFUNCTIONS *********************/
/******************************************************************************/

/******************************************************************************/
/****************** START CK5050 STATEMACHINE STATEFUNCTIONS ******************/

/*----------------------------------------------------------------------------*/
/* Function    : CSHDL_CK5050off_s0                                           */
/**
    \brief      Central state handler function for CK5050 statemachine
                state CK5050off

    \brief      Description:
                function to handle the actions in CK5050 state CK5050off

    \param      EventMaskType
    \return     SuccessFailType
   */
/*----------------------------------------------------------------------------*/
static SuccessFailType CSHDL_CK5050off_s0(EventMaskType event)
{
    SuccessFailType sF          = SUCCESS;

    switch(cshdlCK5050SystemInState)
    {
        case construct:
            /* set Bluetooth module in reset */
            CSHDL_vCK5050_HardwareStop();
            (void)ERM_sysPerfResetStartUpEventTraceDone((Uint8Type) 0);
            cshdlCK5050SystemInState = stat;
            break;

        case stat:
            if(cshdlGesamtSystemState > CSHDL_SYSinit)
            {
                cshdlCK5050SystemInState = destruct;
            }
            break;

        case destruct:
            cshdlCK5050State = CSHDL_CK5050init;
            cshdlCK5050SystemInState = construct;
            break;
    }
    return(sF);
}


/*----------------------------------------------------------------------------*/
/* Function    : CSHDL_CK5050init_s1                                          */
/**
    \brief      Central state handler function for CK5050 statemachine
                state CK5050init

    \brief      Description:
                function to handle the actions in CK5050 state CK5050init

    \param      EventMaskType
    \return     SuccessFailType
   */
/*----------------------------------------------------------------------------*/
static SuccessFailType CSHDL_CK5050init_s1(EventMaskType event)
{
    SuccessFailType sF = SUCCESS;

    switch(cshdlCK5050SystemInState)
    {
        case construct:
            /* start Bluetooth module */
#if defined(CRADLE_RESTRICTION_ACTIVE)
            if (CSHDL_bCradleRestrictionGetBluetoothAllowed())
#endif
            {
               CSHDL_vCK5050_HardwareStart();
               /* activate the BT-Module UpTimer, to watch if BT module give any answer */
               (void)SetRelAlarm(CTA_BTMOD_TIMEOUT, MSEC(100000), 0);
               cshdlCK5050SystemInState = stat;
            }
            break;

        case stat:
            if(event & evSystemStateBtModInitiated)
            {
	            (void)ClearEvent(evSystemStateBtModInitiated);
	            cshdlCK5050State = CSHDL_CK5050ready;
	            cshdlCK5050SystemInState = construct;
	            (void)CancelAlarm(CTA_BTMOD_TIMEOUT);
            }

            if(cshdlGesamtSystemState < CSHDL_TMS470ready)
            {
                cshdlCK5050State = CSHDL_CK5050off;
                (void)CancelAlarm(CTA_BTMOD_TIMEOUT);
                cshdlCK5050SystemInState = construct;
            }
            break;

        case destruct:

            break;
    }
    return(sF);
}


/*----------------------------------------------------------------------------*/
/* Function    : CSHDL_CK5050ready_s2                                         */
/**
    \brief      Central state handler function for CK5050 statemachine
                state CK5050ready

    \brief      Description:
                function to handle the actions in CK5050 state CK5050ready

    \param      EventMaskType
    \return     SuccessFailType
   */
/*----------------------------------------------------------------------------*/
static SuccessFailType CSHDL_CK5050ready_s2(EventMaskType event)
{
    SuccessFailType sF = SUCCESS;

    switch(cshdlCK5050SystemInState)
    {
        case construct:
            cshdlCK5050SystemInState = stat;
            break;

        case stat:
            if(cshdlGesamtSystemState < CSHDL_SYSrun)
            {
                cshdlCK5050SystemInState = destruct;
            }
            break;

        case destruct:
            cshdlCK5050State = CSHDL_CK5050off;
            cshdlCK5050SystemInState = construct;
//            (void)CancelAlarm(CTA_BTMOD_TIMEOUT);
            break;
    }
    return(sF);
}

/******************* END CK5050 STATEMACHINE STATEFUNCTIONS *******************/
/******************************************************************************/

/******************************************************************************/
/******************* START MAIN STATEMACHINE STATEFUNCTIONS *******************/

/*----------------------------------------------------------------------------*/
/* Function    : CSHDL_SYSinit_s1                                             */
/**
    \brief      Central state handler function for main statemachine state SYSinit

    \brief      Description:
                function to handle the actions in main state SYSinit

    \param      EventMaskType
    \return     SuccessFailType
   */
/*----------------------------------------------------------------------------*/
static SuccessFailType CSHDL_SYSinit_s1(EventMaskType event)
{
    SuccessFailType sF = SUCCESS;

    if(event & evSystemTaskShutdown)
	  {
	    cshdlMainSystemInState = destruct;
		  (void)ClearEvent(evSystemTaskShutdown);
    }

    switch(cshdlMainSystemInState)
    {
        case construct:
	        if(SUCCESS == sF)
	        {
	            cshdlMainSystemInState = stat;
	        }
            break;

        case stat:
#if defined(UHV_BLUETOOTH_ZERTIFIZIERUNG_PBTESTMODE) || defined(UHV_BLUETOOTH_ZERTIFIZIERUNG_PBFREQEX)|| defined(UHV_BLUETOOTH_NO_CAN)
            CSHDL_vSendSystemStateToApp(UHV_ACTIVE_KL15_ON, __LINE__);
            cshdlGesamtSystemState = CSHDL_TMS470ready;
            cshdlMainSystemInState = construct;
#endif
            /* dependencies from Power concept UHV_NAR 22.09.08 */
            if ((KL_15_INFO == OFF) && 
                (KL_S_INFO == ON) && 
                (RADIO_ONOFF_INFO == ON) &&
                (OFF == INFOTAINMENT_WARN_INFO) &&
                (OFF == INFOTAINMENT_AUS_INFO))

            {
                if(cshdlCANState == CSHDL_CANready)
                {
                   // UHV_DEACTIVE -> UHV_ACTIVE
                   // Info about KL15 OFF
                   CSHDL_vSendSystemStateToApp(UHV_ACTIVE_KL15_OFF, __LINE__);
                   cshdlGesamtSystemState = CSHDL_TMS470ready;
                   cshdlMainSystemInState = construct;
                }
            }
            else if((KL_15_INFO == ON) && (REMOTE_INFO == OFF))
            {
                if(cshdlCANState == CSHDL_CANready)
                {
                   // UHV_DEACTIVE -> UHV_ACTIVE
                   // Info about KL15 ON
                   CSHDL_vSendSystemStateToApp(UHV_ACTIVE_KL15_ON, __LINE__);
                   cshdlGesamtSystemState = CSHDL_TMS470ready;
                   cshdlMainSystemInState = construct;
                }
            }
            break;

        case destruct:
                cshdlGesamtSystemState = CSHDL_Poff;
                cshdlMainSystemInState = construct;
            break;
    }
    return(sF);
}


/*----------------------------------------------------------------------------*/
/* Function    : CSHDL_TMS470ready_s2                                          */
/**
    \brief      Central state handler function for main statemachine state TMS470init

    \brief      Description:
                function to handle the actions in main state TMS470init

    \param      EventMaskType
    \return     SuccessFailType
   */
/*----------------------------------------------------------------------------*/
static SuccessFailType CSHDL_TMS470ready_s2(EventMaskType event)
{
    static BooleanType KL15isOn = BT_FALSE;
    SuccessFailType sF = SUCCESS;

    if(event & evSystemTaskShutdown)
    {
	    (void)ClearEvent(evSystemTaskShutdown);
	    cshdlMainSystemInState = destruct;
    }

    _CSHDL_InfoFromPowerManagerHandler();

    switch(cshdlMainSystemInState)
    {
        case construct:
            cshdlMainSystemInState = stat;
//            GotoMode(Awake);

            KL15isOn = BT_FALSE;
            break;

        case stat:
            if(cshdlCK5050State == CSHDL_CK5050ready)
            {
                cshdlGesamtSystemState = CSHDL_SYSrun;
                cshdlMainSystemInState = construct;
            }
#if defined(UHV_BLUETOOTH_ZERTIFIZIERUNG_PBTESTMODE) || defined(UHV_BLUETOOTH_ZERTIFIZIERUNG_PBFREQEX) || defined(UHV_BLUETOOTH_NO_CAN)

#else

            /* SD Application needs always information about ignition Status */
            if ((KL_15_INFO == ON) && (KL15isOn == BT_FALSE))
            {
               // Info about KL15 ON
#ifndef USE_DEBUGGING_WITH_TRACE_ID
               //(void)DEBUG_Printf(UHV_TRACE,UHV_CSHDL,"KL15_ON  --> send UHV_ACTIVE_KL15_ON");
#endif
               CSHDL_vSendSystemStateToApp(UHV_ACTIVE_KL15_ON, __LINE__);
               KL15isOn = BT_TRUE;
            }

            if ((KL_15_INFO == OFF) && (KL15isOn == BT_TRUE))
            {
                // Info about KL15 OFF
                CSHDL_vSendSystemStateToApp(KL15_OFF, __LINE__);
                KL15isOn = BT_FALSE;
            }

            if(((KL_15_INFO == OFF) && (KL_S_INFO == OFF)) || \
               ((KL_15_INFO == OFF) && (KL_S_INFO == ON) && (RADIO_ONOFF_INFO == OFF)))
            {
                if (PrepareCK5050_OFF_Sent == FALSE)
                {
                    // UHV_ACTIVE -> UHV_DEACTIVE -> send PREPARE_CK5050_OFF
                    AppTaskCK5050_OFF_ACK = FALSE;
                    CSHDL_vSendSystemStateToApp(PREPARE_CK5050_OFF, __LINE__);
                    PrepareCK5050_OFF_SentTime = osGetSystemCounter();
                    PrepareCK5050_OFF_Sent = TRUE;
                }
            }

            if (PrepareCK5050_OFF_Sent == TRUE)
            {
                diffTime = (osGetSystemCounter() - PrepareCK5050_OFF_SentTime);
                if ( (TRUE == AppTaskCK5050_OFF_ACK) || (diffTime > PREPARE_CK5050_TIMEOUT) )
                {
                    // ACK or Timeout from App Task, change State now
                    DEBUG_VALUE2(CSHDL_SWI_TRC, MOD_CSHDL, "ACK received from App Task after :%d ms - Timeout:%d", diffTime, PREPARE_CK5050_TIMEOUT);

                    CSHDL_vSendSystemStateToApp(WAIT_FOR_SHUTDOWN, __LINE__);

                    PrepareCK5050_OFF_Sent = FALSE;
                    cshdlGesamtSystemState = CSHDL_SYSinit;
                    cshdlMainSystemInState = construct;
                }
            }
#endif

            break;


        case destruct:
            cshdlGesamtSystemState = CSHDL_Poff;
            cshdlMainSystemInState = construct;
            break;
    }
    return(sF);
}


/*----------------------------------------------------------------------------*/
/* Function    : CSHDL_SYSrun_s3                                              */
/**
    \brief      Central state handler function for main statemachine state SYSrun

    \brief      Description:
                function to handle the actions in main state SYSrun

    \param      EventMaskType
    \return     SuccessFailType
   */
/*----------------------------------------------------------------------------*/

static SuccessFailType CSHDL_SYSrun_s3(EventMaskType event)
{
    static BooleanType KL15isOn = BT_FALSE;

    SuccessFailType sF = SUCCESS;

    if(event & evSystemTaskShutdown)
	 {
	     (void)ClearEvent(evSystemTaskShutdown);
        cshdlMainSystemInState  = destruct;
    }

    _CSHDL_InfoFromPowerManagerHandler();

    switch(cshdlMainSystemInState)
    {
        case construct:
            cshdlMainSystemInState = stat;
            KL15isOn = BT_FALSE;
            break;

        case stat:
        {
#if defined(UHV_BLUETOOTH_ZERTIFIZIERUNG_PBTESTMODE) || defined(UHV_BLUETOOTH_ZERTIFIZIERUNG_PBFREQEX)|| defined(UHV_BLUETOOTH_NO_CAN)

#else
            
            /* SD Application needs always information about ignition Status */
            
            static BooleanType bStartDelayForJingle = BT_FALSE;
            
            if ((KL_15_INFO == ON) && (KL15isOn == BT_FALSE))
            {
               // Info about KL15 ON
               CSHDL_vSendSystemStateToApp(UHV_ACTIVE_KL15_ON, __LINE__);
               KL15isOn = BT_TRUE;
            }

            if ((KL_15_INFO == OFF) && (KL15isOn == BT_TRUE))
            {
                // Info about KL15 OFF
                CSHDL_vSendSystemStateToApp(KL15_OFF, __LINE__);
                KL15isOn = BT_FALSE;
            }

            
            /* dependencies from Power concept UHV_NAR 22.09.08 */
            if(((KL_15_INFO == OFF) && (KL_S_INFO == OFF)) ||
               ((KL_15_INFO == OFF) && (KL_S_INFO == ON) && (RADIO_ONOFF_INFO == OFF)))
            {
                if ((cshdlIgnitionOFFDelayState == IGNITION_OFF_DELAY_NOT_STARTED) && (PrepareCK5050_OFF_Sent == FALSE))
                {
                    // UHV_ACTIVE -> START of Power OFF delay - > trigger ERM to disconnect devices
                    cshdlIgnitionOFFDelayState = IGNITION_OFF_DELAY_KL15_OFF_BEFORE_CK5050_OFF_RUNNING;
                    cshdlIgnitionOffDelay_StartTime = osGetSystemCounter();
                    CSHDL_vSendSystemStateToApp(UHV_ACTIVE_KL15_OFF_BEFORE_CK5050_OFF, __LINE__);
                }

            }
            else
            {
                //UHV on conditiojn is satisfied reinit flag 1 for shutdown
                bStartDelayForJingle = BT_FALSE;
            }

            // do next actions after triggering disconnect, trigger Power OFF to CK5050
            // this state is a internal of the cshdl state and must always move to next state
            if ((cshdlIgnitionOFFDelayState == IGNITION_OFF_DELAY_KL15_OFF_BEFORE_CK5050_OFF_RUNNING) && (PrepareCK5050_OFF_Sent == FALSE))
            {
                
                if (((osGetSystemCounter() - cshdlIgnitionOffDelay_StartTime) > UHV_ACTIVE_KL15_OFF_BEFORE_CK5050_OFF_TIMEOUT)  
                      || (ERM_u8GetActivePhone() == AT_INVALID_DEVICE_ID && ERM_u8GetActivePlayer() == AT_INVALID_DEVICE_ID))
                {
                    static BooleanType bStartDelayForJingleResetOnlyIfATPOFF = BT_FALSE;                    
                    
                    //if no jingles coded and no device connected send poff directly 
                    //if jingles coded and no device connected give time 4 seconds time and the send at*poff
                    
                    if(( BT_TRUE == ERM_bGetBTAckSignalActive()) && ((bStartDelayForJingle == BT_FALSE && bStartDelayForJingleResetOnlyIfATPOFF == BT_FALSE ) || 
                        osGetSystemCounter() - cshdlIgnitionOffDelay_StartTime <= UHV_ACTIVE_KL15_OFF_BEFORE_CK5050_OFF_TIMEOUT  && 
                        ERM_u8GetActivePhone() == AT_INVALID_DEVICE_ID && ERM_u8GetActivePlayer() == AT_INVALID_DEVICE_ID))
                    {
                        
                        if (bStartDelayForJingle == BT_FALSE && bStartDelayForJingleResetOnlyIfATPOFF == BT_FALSE)
                        {
                            //set shutdown flags to BT_TRUE such that this code is running only one per shutdown 
                            if(AT_AUDIO_STATE_CK5050 == APP_DEVICE_GetPASAEvent())
                            {
                                cshdlIgnitionOffDelay_StartTime = osGetSystemCounter() - UHV_ACTIVE_KL15_OFF_BEFORE_CK5050_OFF_TIMEOUT + 9000;
                                DEBUG_TEXT(CSHDL_SWI_TRC, MOD_CSHDL, "change cshdlIgnitionOffDelay_StartTime, correction factor 9 sec");
                            }
                            else
                            {
                                cshdlIgnitionOffDelay_StartTime = osGetSystemCounter() - UHV_ACTIVE_KL15_OFF_BEFORE_CK5050_OFF_TIMEOUT + 4000;
                                DEBUG_TEXT(CSHDL_SWI_TRC, MOD_CSHDL, "change cshdlIgnitionOffDelay_StartTime, correction factor 4 sec");
                            }
                            bStartDelayForJingle = BT_TRUE;
                            bStartDelayForJingleResetOnlyIfATPOFF = BT_TRUE;
                        }    
                    }
                    else
                    {
                        //shutdown will follow reset flags
                        bStartDelayForJingle = BT_FALSE;
                        bStartDelayForJingleResetOnlyIfATPOFF = BT_FALSE;
                        // UHV_ACTIVE -> UHV_DEACTIVE -> send PREPARE_CK5050_OFF
                        AppTaskCK5050_OFF_ACK = FALSE;
                        CSHDL_vSendSystemStateToApp(PREPARE_CK5050_OFF, __LINE__);
                        PrepareCK5050_OFF_SentTime = osGetSystemCounter();
                        cshdlIgnitionOFFDelayState = IGNITION_OFF_DELAY_KL15_OFF_PREPARE_CK5050_SENT;
                        PrepareCK5050_OFF_Sent = TRUE;
                    }    
                }
            }

            // POFF was sent to CK5050 we wait for POFF:END Event or Timeout
            if ((cshdlIgnitionOFFDelayState == IGNITION_OFF_DELAY_KL15_OFF_PREPARE_CK5050_SENT) && (PrepareCK5050_OFF_Sent == TRUE))
            {
                diffTime = (osGetSystemCounter() - PrepareCK5050_OFF_SentTime);
                if ( (TRUE == AppTaskCK5050_OFF_ACK) || (diffTime > PREPARE_CK5050_TIMEOUT) )
                {
                    // ACK or Timeout from App Task, change State now
                    DEBUG_VALUE2(CSHDL_SWI_TRC, MOD_CSHDL, "ACK received from App Task after :%d ms - Timeout:%d", diffTime, PREPARE_CK5050_TIMEOUT);

                    CSHDL_vSendSystemStateToApp(WAIT_FOR_SHUTDOWN, __LINE__);

                    // set internal state back
                    PrepareCK5050_OFF_Sent = FALSE;
                    cshdlIgnitionOFFDelayState = IGNITION_OFF_DELAY_NOT_STARTED;

                    // go to init state
                    cshdlGesamtSystemState = CSHDL_SYSinit;
                    cshdlMainSystemInState = construct;
                }
            }


            if (BT_TRUE == CSHDL_bGetBuetoothUpdateActive())
            {
                _VCR_vStopSupervision();
                CSHDL_vSendSystemStateToApp(PREPARE_CK5050_OFF, __LINE__);
                cshdlGesamtSystemState = CSHDL_BTSWupdate;
            }
#endif  // (UHV_BLUETOOTH_ZERTIFIZIERUNG_PBTESTMODE) || defined(UHV_BLUETOOTH_ZERTIFIZIERUNG_PBFREQEX) || defined(UHV_BLUETOOTH_NO_CAN)
            break;
        }

        case destruct:
            cshdlGesamtSystemState = CSHDL_Poff;
            cshdlMainSystemInState = construct;
            break;
    }
    return(sF);
}


static SuccessFailType CSHDL_BTSW_Update_s4(EventMaskType event)
{
    SuccessFailType sF = SUCCESS;


    if (BT_FALSE == CSHDL_bGetBuetoothUpdateActive())
    {
       // update was canceled
       cshdlGesamtSystemState = CSHDL_SYSinit;
       cshdlMainSystemInState = construct;
    }

    return(sF);

}
/******************* END MAIN STATEMACHINE STATEFUNCTIONS *********************/
/******************************************************************************/
/*----------------------------------------------------------------------------*/
/* Function    : _CSHDL_InfoFromPowerManagerHandler                                      */
/**
    \brief      function to handle information from Power manager.

    \brief      Description:
                This function has to be used in case there is any action to be done on messages from power manager.

    \param      void
    \return     void
   */
/*----------------------------------------------------------------------------*/
static void _CSHDL_InfoFromPowerManagerHandler(void)
{
	// Currently the CSHDL does not have any handling for information from Power Manager. It may be required in future.
	switch(cshdlVoltageState)
	{
		case PWMGR_NormalLowVoltage:
		break;

		case PWMGR_NormalLowRecoverVoltage:
		break;

		case PWMGR_NormalHighVoltage:
		break;

		case PWMGR_NormalHighRecoverVoltage:
		break;

		case PWMGR_NormalVoltage:
		break;

		default:
		break;
	}
}



/*----------------------------------------------------------------------------*/
/* Function    : CSHDL_bGetBluetoothUpdateActive                              */
/**
    \brief      Central state handler function to get the status if a bluetooth
                was triggerd

   \param      void
   \return     BT_FALSE = NO Update is running (default)
               BT_TRUE = Update is running
   */
/*----------------------------------------------------------------------------*/


BooleanType CSHDL_bGetBuetoothUpdateActive(void)
{
   if (ERM_u8GetBluetoothUpdateActive())
   {
      return BT_TRUE;
   }
   else
   {
      return BT_FALSE;
   }
}




/*----------------------------------------------------------------------------*/
/* Function    : CSHDL_vTriggerBluetoothUpdate                                */
/**
    \brief      Central state handler function to trigger a bluetooth update

   \param      0 = Stop Update
               1 = Start BT Update (unsigned PLF)
               2 = Start secure BT Update  (signed PLF)
   \return     void
   */
/*----------------------------------------------------------------------------*/

void CSHDL_vTriggerBluetoothUpdate(Uint8Type StartStopUpdate)
{
    switch (StartStopUpdate)
    {
        // inactiv -> activ
        case BT_UPDATE_NORMAL_MODE:
            _CSHDL_BluetoothUpdateTrigger = BT_UPDATE_NORMAL_MODE;
            break;

        case BT_UPDATE_SECURED_MODE:
            _CSHDL_BluetoothUpdateTrigger = BT_UPDATE_SECURED_MODE;
            break;

        default:
            break;
	}
}

Uint8Type CSHDL_u8GetBluetoothUpdateTrigger(void)
{
   return(_CSHDL_BluetoothUpdateTrigger);
}


/*----------------------------------------------------------------------------*/
/* Function    : CSHDL_vDiagnoseResetSimulation                               */
/**
    \brief      This function triggers a reset of the processor.

   \param      void

   \return     SuccessFailType
   */
/*----------------------------------------------------------------------------*/
SuccessFailType CSHDL_vDiagnoseResetSimulation(void)
{
	 SuccessFailType sF = SUCCESS;
	 if( sF == SUCCESS )
	 {
		 c_int00();
	 }
	 return sF;
}


/*----------------------------------------------------------------------------*/
/* Function    : CSHDL_enGetIgnitionOFFDelayState                             */
/**
    \brief      This function get the information if delay off state is active.

    \param      void

    \return     CSHDL_enIGNITION_OFF_DELAY_States
*/
/*----------------------------------------------------------------------------*/
CSHDL_enIGNITION_OFF_DELAY_States CSHDL_enGetIgnitionOFFDelayState(void)
{
    return (cshdlIgnitionOFFDelayState);
}

/*----------------------------------------------------------------------------*/
/* Function    : CSHDL_vSendSystemStateToApp                                  */
/**
    \brief      Send the new SysState to APP_CSHDL if it differs from the old one.

    \param      CSHDL_enSystemStates

    \param      Uint32Type

    \return     void
*/
/*----------------------------------------------------------------------------*/

/*
DEBUG_STATE_ENUM_NAME='CSHDL_enSystemStates' DEBUG_STATE_NAME='INVALID_SYSTEM_STATE'                     DEBUG_STATE_NUMBER='0'
DEBUG_STATE_ENUM_NAME='CSHDL_enSystemStates' DEBUG_STATE_NAME='KL15_OFF'                                 DEBUG_STATE_NUMBER='1'
DEBUG_STATE_ENUM_NAME='CSHDL_enSystemStates' DEBUG_STATE_NAME='UHV_ACTIVE_KL15_OFF'                      DEBUG_STATE_NUMBER='2'
DEBUG_STATE_ENUM_NAME='CSHDL_enSystemStates' DEBUG_STATE_NAME='UHV_ACTIVE_KL15_ON'                       DEBUG_STATE_NUMBER='3'
DEBUG_STATE_ENUM_NAME='CSHDL_enSystemStates' DEBUG_STATE_NAME='UHV_ACTIVE_KL15_OFF_BEFORE_CK5050_OFF'    DEBUG_STATE_NUMBER='4'
DEBUG_STATE_ENUM_NAME='CSHDL_enSystemStates' DEBUG_STATE_NAME='PREPARE_CK5050_OFF'                       DEBUG_STATE_NUMBER='5'
DEBUG_STATE_ENUM_NAME='CSHDL_enSystemStates' DEBUG_STATE_NAME='WAIT_FOR_SHUTDOWN'                        DEBUG_STATE_NUMBER='6'
DEBUG_STATE_ENUM_NAME='CSHDL_enSystemStates' DEBUG_STATE_NAME='CRADLE_RESTRICTION_ON'                    DEBUG_STATE_NUMBER='7'
DEBUG_STATE_ENUM_NAME='CSHDL_enSystemStates' DEBUG_STATE_NAME='CRADLE_RESTRICTION_OFF'                   DEBUG_STATE_NUMBER='8'
DEBUG_STATE_ENUM_NAME='CSHDL_enSystemStates' DEBUG_STATE_NAME='CRADLE_RESTRICTION_ON_DISCONNECT_DEVICES' DEBUG_STATE_NUMBER='9'
*/

static void CSHDL_vSendSystemStateToApp(CSHDL_enSystemStates state, Uint32Type line)
{
    static CSHDL_enSystemStates old_state = INVALID_SYSTEM_STATE;

    DEBUG_VALUE1(CSHDL_SWI_TRC, MOD_CSHDL, "SendSysStateToApp: called in line %d", line);
    DEBUG_STATE2(CSHDL_SWI_TRC, MOD_CSHDL, "SystemStateToApp: $CSHDL_enSystemStates$ %d -> $CSHDL_enSystemStates$ %d", old_state, state);

    old_state = state;

    if (CRADLE_RESTRICTION_ON == state)
    {
        _CSHDL_CradleRestrictionSendNotify = CRADLE_RESTRICTION_ON;
    }
    else if (CRADLE_RESTRICTION_OFF == state)
    {
        _CSHDL_CradleRestrictionSendNotify = CRADLE_RESTRICTION_OFF;
    }
    (void)SendMessage(msgAppTaskSystemStates, &state);
}


static void CSHDL_vCK5050_HardwareStart (void)
{
    GIO_vPinWrite(PORT_MON_NRESETS, HIGH, GIOD);
    _CSHDL_StartCkOnOff = 1;

    DEBUG_VALUE1(CSHDL_SWI_TRC, MOD_CSHDL, "Resetfreigabe Parrotmodul: %d ms", osGetSystemCounter() );
}

static void CSHDL_vCK5050_HardwareStop (void)
{
    GIO_vPinWrite(PORT_MON_NRESETS, LOW, GIOD);
    _CSHDL_StartCkOnOff = 0;

    // ReInit this flag
    _CSHDL_CK5050_AT_Interface = CK5050_AT_COMMUNICATION_NOT_STARTED;


    DEBUG_VALUE1(CSHDL_SWI_TRC, MOD_CSHDL, "Reset Parrotmodul: %d ms", osGetSystemCounter() );
}


static void CSHDL_vCK5050_SetNewState (CSHDL_CK5050StateType NewCK5050State)
{
    switch (NewCK5050State)
    {
        case CSHDL_CK5050off:
             cshdlCK5050State = CSHDL_CK5050off;
             cshdlCK5050SystemInState = construct;
        break;

        case CSHDL_CK5050init:
             // restart BT-Modul only, if not already started,
             // otheriwse we have a further reboot of the CK5050 into or out of the Diag-Session
             if (CSHDL_CK5050off == cshdlCK5050State)
             {
                 // Put CK5050 to init state
                 cshdlCK5050State = CSHDL_CK5050init;
                 cshdlCK5050SystemInState = construct;
             }
        break;

        case CSHDL_CK5050ready:
        default:
            DEBUG_TEXT(CSHDL_SWI_ERR, MOD_CSHDL, "INVALID PARAMETER given to function CSHDL_vCK5050_SetNewState");
        break;
    }
}

#if defined(CRADLE_RESTRICTION_ACTIVE)


#define CRADLE_CONNECTIVITY_STABLE_CONNECTED    0x00FFFFF  // 20 bits for 2 seconds
#define CRADLE_CONNECTIVITY_STABLE_REMOVED      0x0000000

static void CSHDL_vCradleConnectivity_Debouncing (void)
{
   static Uint8Type DebouceCycle = 0;

   if (++DebouceCycle == 5)   // 5 * 20ms = 100ms
   {
      _CSHDL_CradleConnectivity_Debounce <<= 1;
      if (TS_TRUE == PHY_BUT_DRV_bGetCradleConnectivity())
      {
         _CSHDL_CradleConnectivity_Debounce |= 1;
      }
      DebouceCycle = 0;
   }
}

static void CSHDL_vCradleRestrictionStateMachineChangeState (CSHDL_enCradleRestriction_Internal_States newState)
{
    _CSHDL_CradleRestriction_oldState = _CSHDL_CradleRestriction_State;
    _CSHDL_CradleRestriction_State = newState;
    _CSHDL_CradleRestriction_StateTime = osGetSystemCounter();
}



static void CSHDL_vCradleRestrictionStateMachine (void)
{
    if ((OFF == INFOTAINMENT_AUS_INFO) ||
        ((ON == INFOTAINMENT_AUS_INFO) && (ON == KL_15_INFO)))
    {
        CSHDL_vCradleConnectivity_Debouncing();


        // Diag has higher prio
        if (_CSHDL_CradleRestriction_State != CRADLE_RESTRICTION_DIAG_SESSION_ACTIVE)
        {
            if (UDScub_DEFAULT_SESSION != (Uint8Type)UDS_ubGetSession())
            {
                // Regel#3: Diagnosesession hebt den Cradle-Zwang auf
                // Diag is active => CK5050 is needed
                DEBUG_VALUE1(CSHDL_SWI_TRC, MOD_CSHDL, "CR_STATE %d ->DIAG_SESSION_ACTICE", _CSHDL_CradleRestriction_State );

                CSHDL_vCradleRestrictionStateMachineChangeState(CRADLE_RESTRICTION_DIAG_SESSION_ACTIVE);

                CSHDL_vCK5050_SetNewState(CSHDL_CK5050init);
            }
        }
    }
    switch (_CSHDL_CradleRestriction_State)
    {
        case CRADLE_RESTRICTION_INIT:
        {
            if ((ON == INFOTAINMENT_AUS_INFO) && 
                (OFF == KL_15_INFO))
            {
                DEBUG_VALUE2(CSHDL_SWI_TRC, MOD_CSHDL, "OnOff STATE INIT -> CHECK_CODING InfotainmentAus = %d, KL15 = %d", 
                             INFOTAINMENT_AUS_INFO, KL_15_INFO);
                CSHDL_vCradleRestrictionStateMachineChangeState(CRADLE_RESTRICTION_CHECKING_CODING);
            }
            else
            {
                if ( PHY_BUT_DRV_bGetCradleConnectivity() != TS_UNKNOWN )
                {
                    // Driver is ready
                    DEBUG_VALUE1(CSHDL_SWI_TRC, MOD_CSHDL, "CR_STATE INIT->CHECK_CODING %d ms", osGetSystemCounter() );

                    CSHDL_vCradleRestrictionStateMachineChangeState(CRADLE_RESTRICTION_CHECKING_CODING);
                }
        	    else if (osGetSystemCounter() - _CSHDL_CradleRestriction_StateTime >= 300)
        	    {
                    // Driver is not ready, start the State Machine
                    DEBUG_TEXT(CSHDL_SWI_TRC, MOD_CSHDL, "INIT->CHECK_CODING (Timeout - get no value from driver");
                    CSHDL_vCradleRestrictionStateMachineChangeState(CRADLE_RESTRICTION_CHECKING_CODING);
        	    }
            }
            break;
        }
        case CRADLE_RESTRICTION_CHECKING_CODING:
        {
            if ((ON == INFOTAINMENT_AUS_INFO) && 
                (OFF == KL_15_INFO))
            {
                DEBUG_VALUE2(CSHDL_SWI_TRC, MOD_CSHDL, "OnOff STATE CHECK_CODING -> SHUTDOWN_CK5050 InfotainmentAus = %d, KL15 = %d", 
                             INFOTAINMENT_AUS_INFO, KL_15_INFO);
                CSHDL_vCradleRestrictionStateMachineChangeState(CRADLE_RESTRICTION_CODING_ON_CRADLE_NOT_CONNECTED_SHUTDOWN_CK5050);
            }
            else
            {
                // default of Vikas implementation is: Cradle protection is off
                if (BT_TRUE == LOGI_BUT_MAN_bGetCradleProtection())
                {
                    DEBUG_TEXT(CSHDL_SWI_TRC, MOD_CSHDL, "CR_STATE CHECK_CODING->CODING_ON");
                    CSHDL_vCradleRestrictionStateMachineChangeState(CRADLE_RESTRICTION_CODING_ON_CHECKING_CRADLE_CONNECTION);
                }
                else
                {
                    // CradleRestriction is not coded
                    DEBUG_TEXT(CSHDL_SWI_TRC, MOD_CSHDL, "CR_STATE CHECK_CODING->CODING_OFF");
                    CSHDL_vCradleRestrictionStateMachineChangeState(CRADLE_RESTRICTION_CODING_OFF_CHECKING_CODING_CHANGE);

                    // => Send first Notification to APP
                    if (APP_CSHDL_eGetCradleRestrictionState() != APP_CSHDL_CRADLE_RESTRICTION_OFF)
                    {
                        CSHDL_vSendSystemStateToApp(CRADLE_RESTRICTION_OFF, __LINE__);
                    }

                    // init states => (first) start of BT-Modul again
                    CSHDL_vCK5050_SetNewState(CSHDL_CK5050init);
                }
            }
            break;
        }
        case CRADLE_RESTRICTION_CODING_OFF_CHECKING_CODING_CHANGE:
        {
            if (BT_TRUE == LOGI_BUT_MAN_bGetCradleProtection())
            {
                DEBUG_TEXT(CSHDL_SWI_TRC, MOD_CSHDL, "CR_STATE CHECK_CODING_CHANGE->CODING_ON");
                CSHDL_vCradleRestrictionStateMachineChangeState(CRADLE_RESTRICTION_CODING_ON_CHECKING_CRADLE_CONNECTION);
            }
            break;
        }
        case CRADLE_RESTRICTION_CODING_ON_CHECKING_CRADLE_CONNECTION:
        {
            if (TS_TRUE == PHY_BUT_DRV_bGetCradleConnectivity())
            {
                DEBUG_TEXT(CSHDL_SWI_TRC, MOD_CSHDL, "CR_STATE CHECK_CON->CONNECTED");

                // Driver is also debouncing, Init to a stable conencted otherwise we will get a state chnage immediatly
                _CSHDL_CradleConnectivity_Debounce = CRADLE_CONNECTIVITY_STABLE_CONNECTED;

                CSHDL_vCradleRestrictionStateMachineChangeState(CRADLE_RESTRICTION_CODING_ON_CRADLE_CONNECTED);

                // CradleRestriction is coded and Cradle is connected => Send first Notification to APP
                CSHDL_vSendSystemStateToApp(CRADLE_RESTRICTION_OFF, __LINE__);

                // last change
                // reinit states => restart BT-Modul again
                CSHDL_vCK5050_SetNewState(CSHDL_CK5050init);
            }
            else
            {
                DEBUG_TEXT(CSHDL_SWI_TRC, MOD_CSHDL, "CR_STATE CHECK_CON->NOT CONNECTED");
                CSHDL_vCradleRestrictionStateMachineChangeState(CRADLE_RESTRICTION_CODING_ON_CRADLE_NOT_CONNECTED);

                // CradleRestriction is coded but Cradle is not connected => Send first Notification to APP
                CSHDL_vSendSystemStateToApp(CRADLE_RESTRICTION_ON, __LINE__);
            }
            break;
        }
        case CRADLE_RESTRICTION_CODING_ON_CRADLE_NOT_CONNECTED:
        {
            if ((_CSHDL_CradleConnectivity_Debounce & CRADLE_CONNECTIVITY_STABLE_CONNECTED) == CRADLE_CONNECTIVITY_STABLE_CONNECTED)
            {
                // not connected -> stable connected
                DEBUG_TEXT(CSHDL_SWI_TRC, MOD_CSHDL, "CR_STATE NOT CONNECTED->CONNECTED");

                CSHDL_vCradleRestrictionStateMachineChangeState(CRADLE_RESTRICTION_CODING_ON_CRADLE_CONNECTED);

                // CradleRestriction is coded and Cradle is connected => Send Notification to APP
                CSHDL_vSendSystemStateToApp(CRADLE_RESTRICTION_OFF, __LINE__);

                // reinit states => restart BT-Modul again
                CSHDL_vCK5050_SetNewState(CSHDL_CK5050init);
            }
            else if ((_CSHDL_CradleConnectivity_Debounce & CRADLE_CONNECTIVITY_STABLE_CONNECTED) == CRADLE_CONNECTIVITY_STABLE_REMOVED)
            {
                if (  (_CSHDL_CradleRestriction_oldState == CRADLE_RESTRICTION_CODING_ON_CRADLE_CONNECTED) ||
                      (_CSHDL_CradleRestriction_oldState == CRADLE_RESTRICTION_DIAG_SESSION_ACTIVE))
                {
                    // User didn't follow the Screen -> Shutdown CK5050
                    DEBUG_TEXT(CSHDL_SWI_TRC, MOD_CSHDL, "CR_STATE NOT CONNECTED->SHUTDOWN_CK5050");
                    CSHDL_vCradleRestrictionStateMachineChangeState(CRADLE_RESTRICTION_CODING_ON_CRADLE_NOT_CONNECTED_SHUTDOWN_CK5050);

                    // first diconnect devices, send POFF in next statee
                    CSHDL_vSendSystemStateToApp(CRADLE_RESTRICTION_ON_DISCONNECT_DEVICES, __LINE__);
                }
            }
            break;
        }
        case CRADLE_RESTRICTION_CODING_ON_CRADLE_CONNECTED:
        {
            if ((_CSHDL_CradleConnectivity_Debounce & CRADLE_CONNECTIVITY_STABLE_CONNECTED) == CRADLE_CONNECTIVITY_STABLE_REMOVED)
            {
                // connected -> stable disconnected
                DEBUG_TEXT(CSHDL_SWI_TRC, MOD_CSHDL, "CR_STATE CONNECTED->NOT CONNECTED");
                CSHDL_vCradleRestrictionStateMachineChangeState(CRADLE_RESTRICTION_CODING_ON_CRADLE_NOT_CONNECTED);

                // CradleRestriction is coded and Cradle is not connected => Send Notification to APP
                CSHDL_vSendSystemStateToApp(CRADLE_RESTRICTION_ON, __LINE__);
            }
            break;
        }
        case CRADLE_RESTRICTION_CODING_ON_CRADLE_NOT_CONNECTED_SHUTDOWN_CK5050:
        {
            if ((ON == INFOTAINMENT_AUS_INFO) && 
                (OFF == KL_15_INFO))
            {
                DEBUG_VALUE2(CSHDL_SWI_TRC, MOD_CSHDL, "OnOff STATE SHUTDOWN_CK5050 -> WAIT_CK5050_POFF_END InfotainmentAus = %d, KL15 = %d", 
                             INFOTAINMENT_AUS_INFO, KL_15_INFO);
                // prepare CK5050 obige Flags noch richtig setzen
                CSHDL_vSendSystemStateToApp(PREPARE_CK5050_OFF, __LINE__);
                CSHDL_vCradleRestrictionStateMachineChangeState(CRADLE_RESTRICTION_CODING_ON_CRADLE_NOT_CONNECTED_WAIT_CK5050_POFF_END);
            }
            else
            {
                if (osGetSystemCounter() - _CSHDL_CradleRestriction_StateTime > 10000)
                {
                    //Send POFF  Shutdown CK5050
                    CSHDL_vCradleRestrictionStateMachineChangeState(CRADLE_RESTRICTION_CODING_ON_CRADLE_NOT_CONNECTED_WAIT_CK5050_POFF_END);

                    DEBUG_TEXT(CSHDL_SWI_TRC, MOD_CSHDL, "CR_STATE SHUTDOWN_CK5050->WAIT_CK5050_POFF_END");

                    // prepare CK5050 obige Flags noch richtig setzen
                    CSHDL_vSendSystemStateToApp(PREPARE_CK5050_OFF, __LINE__);
                }
                else if (CRADLE_RESTRICTION_OFF != _CSHDL_CradleRestrictionSendNotify)
                {
                   if ((_CSHDL_CradleConnectivity_Debounce & CRADLE_CONNECTIVITY_STABLE_CONNECTED) == CRADLE_CONNECTIVITY_STABLE_CONNECTED)
                   {
                       // disconnected -> stable connected
                       // trigger no state change in CSHDL, because we are already in shutdown, notify only the Apps
                           CSHDL_vSendSystemStateToApp(CRADLE_RESTRICTION_OFF, __LINE__);
                   }
                }
                else if (CRADLE_RESTRICTION_ON != _CSHDL_CradleRestrictionSendNotify)
                {
                   if ((_CSHDL_CradleConnectivity_Debounce & CRADLE_CONNECTIVITY_STABLE_CONNECTED) == CRADLE_CONNECTIVITY_STABLE_REMOVED)
                   {
                       // connected -> stable disconnected
                       // trigger no state change in CSHDL, because we are already in shutdown, notify only the Apps
                       CSHDL_vSendSystemStateToApp(CRADLE_RESTRICTION_ON, __LINE__);
                   }
                }
            }
            break;
        }
        case CRADLE_RESTRICTION_CODING_ON_CRADLE_NOT_CONNECTED_WAIT_CK5050_POFF_END:
        {
            if ((ON == INFOTAINMENT_AUS_INFO) && 
                (OFF == KL_15_INFO) && 
                (((CK5050_AT_COMMUNICATION_FINISHED == _CSHDL_CK5050_AT_Interface) &&
                  (osGetSystemCounter() - _CSHDL_CradleRestriction_StateTime > 200)) ||
                  (osGetSystemCounter() - _CSHDL_CradleRestriction_StateTime > 10000)))
            {
                DEBUG_VALUE2(CSHDL_SWI_TRC, MOD_CSHDL, "OnOff STATE WAIT_CK5050_POFF_END -> WAIT_CK5050_POFF_END InfotainmentAus = %d, KL15 = %d", 
                             INFOTAINMENT_AUS_INFO, KL_15_INFO);
                CSHDL_vCK5050_SetNewState(CSHDL_CK5050off);
                CSHDL_vCradleRestrictionStateMachineChangeState(CRADLE_RESTRICTION_WAIT_CK5050_RESET_TIME);
            }
            else
            {
                if ( ((CK5050_AT_COMMUNICATION_FINISHED == _CSHDL_CK5050_AT_Interface) &&
                      (osGetSystemCounter() - _CSHDL_CradleRestriction_StateTime > 200)) ||
                      (osGetSystemCounter() - _CSHDL_CradleRestriction_StateTime > 10000))
                {
                    // shutdown of CK5050 done succesful -> reinit State Machine
                    // A hardware reset has to be done to start again the module.
                    if (osGetSystemCounter() - _CSHDL_CradleRestriction_StateTime > 10000)
                    {
                        DEBUG_TEXT(CSHDL_SWI_TRC, MOD_CSHDL, "CR_STATE SHUTDOWN_CK5050->WAIT_CK5050_POFF_END");
                    }
                    else
                    {
#ifndef USE_DEBUGGING_WITH_TRACE_ID
                        (void)DEBUG_Printf( UHV_TRACE, UHV_CSHDL, "WAIT_CK5050_POFF_END->CK5050_RESET(ACK)%d", osGetSystemCounter());
#endif
                    }
                    CSHDL_vCK5050_SetNewState(CSHDL_CK5050off);
                    CSHDL_vCradleRestrictionStateMachineChangeState(CRADLE_RESTRICTION_WAIT_CK5050_RESET_TIME);
                }
                else if (CRADLE_RESTRICTION_OFF != _CSHDL_CradleRestrictionSendNotify)
                {
                   if ((_CSHDL_CradleConnectivity_Debounce & CRADLE_CONNECTIVITY_STABLE_CONNECTED) == CRADLE_CONNECTIVITY_STABLE_CONNECTED)
                   {
                       // disconnected -> stable connected
                       // trigger no state change in CSHDL, because we are already in shutdown, notify only the Apps
                       CSHDL_vSendSystemStateToApp(CRADLE_RESTRICTION_OFF, __LINE__);
                   }
                }
                else if (CRADLE_RESTRICTION_ON != _CSHDL_CradleRestrictionSendNotify)
                {
                    if ((_CSHDL_CradleConnectivity_Debounce & CRADLE_CONNECTIVITY_STABLE_CONNECTED) == CRADLE_CONNECTIVITY_STABLE_REMOVED)
                    {
                        // connected -> stable disconnected
                        // trigger no state change in CSHDL, because we are already in shutdown, notify only the Apps
                        CSHDL_vSendSystemStateToApp(CRADLE_RESTRICTION_ON, __LINE__);
                    }
                }
            }
            break;
        }
        case CRADLE_RESTRICTION_WAIT_CK5050_RESET_TIME:
        {
            if (osGetSystemCounter() - _CSHDL_CradleRestriction_StateTime > 510)
            {
                // we switch OFF by SW, but if this not works we use the HW-Solution => wait 500 ms according to CK5050+ - Hardware Specification
                CSHDL_vCradleRestrictionStateMachineChangeState(CRADLE_RESTRICTION_INIT);
                DEBUG_TEXT(CSHDL_SWI_TRC, MOD_CSHDL, "CR_STATE CK5050_RESET->INIT");
            }
            break;
        }
        case CRADLE_RESTRICTION_DIAG_SESSION_ACTIVE:
        {
            if (UDScub_DEFAULT_SESSION == (Uint8Type)UDS_ubGetSession())
            {
               // Diagnosis is not active anymore
               // now check the actual coding and set state machine in the right state
               if (BT_TRUE == LOGI_BUT_MAN_bGetCradleProtection())
               {
                    if (TS_TRUE == PHY_BUT_DRV_bGetCradleConnectivity())
                    {
                        // Feature codiert, cradle connected => ck5050 kann anbleiben
                        DEBUG_TEXT(CSHDL_SWI_TRC, MOD_CSHDL, "CR_STATE DIAG_SESSION_ACTICE->CONNECTED");

                        CSHDL_vCradleRestrictionStateMachineChangeState(CRADLE_RESTRICTION_CODING_ON_CRADLE_CONNECTED);
                        CSHDL_vSendSystemStateToApp(CRADLE_RESTRICTION_OFF, __LINE__);
                    }
                    else
                    {
                        // Feature codiert, cradle not connected => ck5050 muss ausgehen
                        DEBUG_TEXT(CSHDL_SWI_TRC, MOD_CSHDL, "CR_STATE DIAG_SESSION_ACTICE->NOT CONNECTED");
                        CSHDL_vCradleRestrictionStateMachineChangeState(CRADLE_RESTRICTION_CODING_ON_CRADLE_NOT_CONNECTED);
                        CSHDL_vSendSystemStateToApp(CRADLE_RESTRICTION_ON, __LINE__);
                    }
               }
               else
               {
                   // Feature auscodiert => ck5050 kann anbleiben
                   DEBUG_TEXT(CSHDL_SWI_TRC, MOD_CSHDL, "CR_STATE DIAG_SESSION_ACTICE->CODING_OFF");
                   CSHDL_vCradleRestrictionStateMachineChangeState(CRADLE_RESTRICTION_CODING_OFF_CHECKING_CODING_CHANGE);
                   // => Send first Notification to APP
                   CSHDL_vSendSystemStateToApp(CRADLE_RESTRICTION_OFF, __LINE__);
               }
            }
            break;
        }
        default:
            _CSHDL_CradleRestriction_State = CRADLE_RESTRICTION_INIT;
            break;
    }
}

static BooleanType CSHDL_bCradleRestrictionGetBluetoothAllowed(void)
{
    if (UDScub_DEFAULT_SESSION != (Uint8Type)UDS_ubGetSession())
    {
        // Diagnose is active => CK5050 is needed
        return (BT_TRUE);
    }
    else
    {
        // no Diag active, switch CK5050 dependend of CradleRestriction and Connection
        switch (_CSHDL_CradleRestriction_State)
        {
            // CK5050=OFF States
            case CRADLE_RESTRICTION_INIT:                                 /*we are in INIT-Phase => CK5050=0 */
            case CRADLE_RESTRICTION_CHECKING_CODING:                      /*we are still in INIT-Phase => CK5050=0 */
            case CRADLE_RESTRICTION_CODING_ON_CHECKING_CRADLE_CONNECTION: /*we are initialzed, Feature is not codedbut we are allowed to switch on but we don't know if cradle is connected => switch OFF BT */
            case CRADLE_RESTRICTION_CODING_ON_CRADLE_NOT_CONNECTED:       /*we are initialzed and we are allowed to switch on but Cradle is NOT connected => switch OFF BT */
            case CRADLE_RESTRICTION_WAIT_CK5050_RESET_TIME:               /*CK5050 in Reset*/
                 return (BT_FALSE);

            // CK5050=ON States
            case CRADLE_RESTRICTION_DIAG_SESSION_ACTIVE:                  /* Diagnosis is running CK5050 always needed => CK5050=1 */
            case CRADLE_RESTRICTION_CODING_OFF_CHECKING_CODING_CHANGE:    /*we are initialzed, Feature is NOT coded => CK5050=1 */
            case CRADLE_RESTRICTION_CODING_ON_CRADLE_CONNECTED:           /*we are initialzed, Feature is coded and Cradle is connected => CK5050=1 */
            case CRADLE_RESTRICTION_CODING_ON_CRADLE_NOT_CONNECTED_SHUTDOWN_CK5050:      /*CK5050 was on, shutdown started*/
            case CRADLE_RESTRICTION_CODING_ON_CRADLE_NOT_CONNECTED_WAIT_CK5050_POFF_END:  /*end of shutdown*/
                 return (BT_TRUE);

            default:
                 //  should not occur (Statemachine in illegal state) -> default switch BT On
                 return (BT_TRUE);
        }
    }
}


/*----------------------------------------------------------------------------*/
/* Function    : CSHDL_bGetBluetoothUpdateActive                              */
/**
    \brief     Central state handler function to notify the CSHDL,
               that CK5050 has responded with the Event *POFF:END
   \param      void
   \return     void
*/
/*----------------------------------------------------------------------------*/
void CSHDL_vCK5050_PowerOFF_ACK(void)
{
    _CSHDL_CK5050_AT_Interface = CK5050_AT_COMMUNICATION_FINISHED;
    DEBUG_TEXT(CSHDL_SWI_TRC, MOD_CSHDL, "received Power off ACK from CK5050 (*POFF:END)");
}

void CSHDL_vCK5050_Startup(void)
{
    _CSHDL_CK5050_AT_Interface = CK5050_AT_COMMUNICATION_RUNNING;
    DEBUG_TEXT(CSHDL_SWI_TRC, MOD_CSHDL, "received Start up event from CK5050 (*PSTU)");
}

/*----------------------------------------------------------------------------*/
/* Function    : CSHDL_bIsKl15On                                              */
/**
    \brief     returns the current state of KL15

   \return     BooleanType
*/
/*----------------------------------------------------------------------------*/
BooleanType CSHDL_bIsKl15On(void)
{
    return KL_15_INFO == ON ? BT_TRUE : BT_FALSE;
}

/*----------------------------------------------------------------------------*/
/* Function    : CSHDL_bIsUHVOnConditionSatisfied                                              */
/**
    \brief     returns the current state of Radio

   \return     BooleanType
*/
/*----------------------------------------------------------------------------*/
BooleanType CSHDL_bIsUHVOnConditionSatisfied(void)
{
    if(((KL_15_INFO == OFF) && (KL_S_INFO == OFF)) ||
      ((KL_15_INFO == OFF) && (KL_S_INFO == ON) && (RADIO_ONOFF_INFO == OFF)))
    {
        return BT_FALSE;
    }
    else
    {
        return BT_TRUE;
    }
}


/*----------------------------------------------------------------------------*/
/* Function    : ctaBtTimeout_cb                                              */
/** 
    \brief      Callback function to check if the timeout module is broken
  
    \brief      Description:\n
                function to reset the alarm which is set by startup the BT\n
                module to check if the BT module is active or has a valid\n
                software included.
                will be called when CTA_BTMOD_TIMEOUT is timedout.
                
    \return     none
   */
/*----------------------------------------------------------------------------*/
ALARMCALLBACK(ctaBtTimeout_cb)
{
	/* software reset */	
    c_int00();
    //stSys.SysECR_UW = 0xc000u;
}

#endif
/* End Of File CSHDL_c1.c */
