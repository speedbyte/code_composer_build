/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     APP_PHONE_CAN_DATA_C1.c
*-----------------------------------------------------------------------------
* Module Name:    Telephone Application
*-----------------------------------------------------------------------------
* Description:       
*-----------------------------------------------------------------------------
* $Date: 2008-08-21 10:54:08 +0200 (Do, 21 Aug 2008) $
* $Rev: 857 $
* $Author: schmidt $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_PHONE/trunk/src/APP_PHONE_CAN_DATA_C1.c $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include <osek.h>           // vector
#include <osekext.h>        // vector
/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "APP_PHONE.h"
#include "APP_PHONE_AT_PNCC.h"
#include "APP_PHONE_CAN_DATA.h"

#include "APP_PHONE_CAN_DATA_C1.id"
#include "TRACE.h"
/*----------------------------------------------------------------------------*/
/* pragma Directives                                                          */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Definitions (avoid as much as possible!)                  */
/*----------------------------------------------------------------------------*/

/** 
\brief	variable to save the current call management function currently in progress
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
/** 
\brief	description of the method
*/

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
/* Function    : APP_PHONE_CAN_DATA_vOnAppPhoneNotification                   */
/** 
    \brief      receives and processes notifications from app phone 
  
    \brief      Description:\n
                receives and processes notifications from app phone

    \return     void
*/
/*----------------------------------------------------------------------------*/
void APP_PHONE_CAN_DATA_vOnAppPhoneNotification(APP_PHONE_enNotificationId notification)
{
    switch(notification)
    {
        case APP_PHONE_OPERATOR_NAME_CHANGED:
            _APP_PHONE_CAN_DATA_vSendProvider(APP_PHONE_GetNetworkProvider()->name,
                                              APP_PHONE_GetNetworkProvider()->name_len);
            break;
        case APP_PHONE_NW_SIGSTRENGHT_CHANGED:
            _APP_PHONE_CAN_DATA_vSendFieldStrength(APP_PHONE_GetSignalStrength());
            break;
        case APP_PHONE_ROAMING_CHANGED:
        case APP_PHONE_SERV_AVAIL_CHANGED:
            _APP_PHONE_CAN_DATA_vMapAndSendRegisterState(APP_PHONE_GetNetworkAvailable(),
                                                         APP_PHONE_GetRoaming());
            break;
        case APP_PHONE_CALL_STATE_CHANGED:
            if(APP_PHONE_AT_PNCC_bAllCallStatesIdle() == BT_FALSE)
            {
                _APP_PHONE_CAN_DATA_vSendPhoneActiveCall(BT_TRUE);
            }
            else
            {
                _APP_PHONE_CAN_DATA_vSendPhoneActiveCall(BT_FALSE);
            }

            if( APP_PHONE_AT_PNCC_u16GetNumberOfIncomingCalls() > 0 || 
                APP_PHONE_AT_PNCC_u16GetNumberOfWaitingCalls() > 0 )
            {
                _APP_PHONE_CAN_DATA_vSendPhoneRing(BT_TRUE);
            }
            else
            {
                _APP_PHONE_CAN_DATA_vSendPhoneRing(BT_FALSE);
            }
            break;
        default:
            /* ignore notification */
            break;
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_CAN_DATA_vSendPhoneOn                        */
/** 
    \brief      sends the phoneOn state to the CAN module 
  
    \brief      Description:\n
                sends the phoneOn state to the CAN module

    \return     void
*/
/*----------------------------------------------------------------------------*/
void _APP_PHONE_CAN_DATA_vSendPhoneOn(BooleanType isPhoneOn)
{
    APP_PHONE_CanDataType canData;

    canData.common.id       = APP_PHONE_CAN_PHONE_ON;
    canData.phoneOn.isOn    = isPhoneOn;

    DEBUG_VALUE1(APP_PHONE_SWI_TRC, MOD_APP_PHONE, "CanData: phoneOn %d",
                    canData.phoneOn.isOn );

    (void)SendMessage(msgTelefonStates ,&canData);
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_CAN_DATA_vSendPhoneActiveCall                     */
/** 
    \brief      sends the activeCall state to the CAN module 
  
    \brief      Description:\n
                sends the activeCall state to the CAN module

    \return     void
*/
/*----------------------------------------------------------------------------*/
void _APP_PHONE_CAN_DATA_vSendPhoneActiveCall(BooleanType isActive)
{
    APP_PHONE_CanDataType canData;

    canData.common.id           = APP_PHONE_CAN_ACTIVE_CALL;
    canData.activeCall.isActive = isActive;

    DEBUG_VALUE1(APP_PHONE_SWI_TRC, MOD_APP_PHONE, "CanData: CallActive %d",
                    canData.activeCall.isActive );

    (void)SendMessage(msgTelefonStates ,&canData);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_CAN_DATA_vSendPhoneRing                           */
/** 
    \brief      sends the ringing state to the CAN module 
  
    \brief      Description:\n
                sends the ringing state to the CAN module

    \return     void
*/
/*----------------------------------------------------------------------------*/
void _APP_PHONE_CAN_DATA_vSendPhoneRing(BooleanType isRinging)
{
    APP_PHONE_CanDataType canData;

    canData.common.id           = APP_PHONE_CAN_PHONE_RING;
    canData.phoneRing.isRinging = isRinging;

    DEBUG_VALUE1(APP_PHONE_SWI_TRC, MOD_APP_PHONE, "CanData: Ringing %d",
                    canData.phoneRing.isRinging );

    (void)SendMessage(msgTelefonStates ,&canData);
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_CAN_DATA_vSendProvider                            */
/** 
    \brief      sends the ringing state to the CAN module 
  
    \brief      Description:\n
                sends the ringing state to the CAN module

    \return     void
*/
/*----------------------------------------------------------------------------*/
void _APP_PHONE_CAN_DATA_vSendProvider(const Uint8Type *pProvName, Uint16Type length)
{
    APP_PHONE_CanDataType canData;

    canData.common.id         = APP_PHONE_CAN_PROVIDER;
    canData.provider.name_len = length;

    if(canData.provider.name_len > (Uint16Type)sizeof(canData.provider.name))
    {
        canData.provider.name_len = sizeof(canData.provider.name);
    }

    if(canData.provider.name_len > 0)
    {
        (void)memcpy(canData.provider.name, pProvName, canData.provider.name_len);
    }


    DEBUG_VALUE1(APP_PHONE_SWI_TRC, MOD_APP_PHONE, "CanData: Prov_len %d",
                    canData.provider.name_len );

    (void)SendMessage(msgTelefonStates ,&canData);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_CAN_DATA_vSendFieldStrength                       */
/** 
    \brief      sends the FieldStrength state to the CAN module 
  
    \brief      Description:\n
                sends the ringing state to the CAN module

    \return     void
*/
/*----------------------------------------------------------------------------*/
void _APP_PHONE_CAN_DATA_vSendFieldStrength(AT_IndicatorValue fieldStrength)
{
    APP_PHONE_CanDataType canData;

    canData.common.id           = APP_PHONE_CAN_FIELD_STRENGTH;
    canData.fieldStrength.value = fieldStrength;

    DEBUG_VALUE1(APP_PHONE_SWI_TRC, MOD_APP_PHONE, "CanData: Field_str %d",
                    canData.fieldStrength.value );

    (void)SendMessage(msgTelefonStates ,&canData);
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_CAN_DATA_vSendRegisterState                       */
/** 
    \brief      sends the RegisterState state to the CAN module 
  
    \brief      Description:\n
                sends the ringing state to the CAN module

    \return     void
*/
/*----------------------------------------------------------------------------*/
void _APP_PHONE_CAN_DATA_vSendRegisterState(APP_PHONE_enCanRegisterState regState)
{
    APP_PHONE_CanDataType canData;

    canData.common.id           = APP_PHONE_CAN_REGISTER_STATE;
    canData.registerState.state = regState;

    DEBUG_VALUE1(APP_PHONE_SWI_TRC, MOD_APP_PHONE, "CanData: RegState %d",
                    canData.registerState.state );

    (void)SendMessage(msgTelefonStates ,&canData);
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_CAN_DATA_vMapAndSendRegisterState                 */
/** 
    \brief      sends the Register state to the CAN module 
  
    \brief      Description:\n
                sends the ringing state to the CAN module

    \return     void
*/
/*----------------------------------------------------------------------------*/
void _APP_PHONE_CAN_DATA_vMapAndSendRegisterState(AT_IndicatorValue atServiceValue, AT_IndicatorValue atRoamingValue)
{
    APP_PHONE_enCanRegisterState regState = APP_PHONE_REG_STATE_NOT_AVAILABLE;

    if( !atServiceValue && !atRoamingValue )
    {
        // not registered and not searching
        regState = APP_PHONE_REG_STATE_NO_SERVICE;
    }   
    else if( !atServiceValue && atRoamingValue)
    {
        // not registered and searching
        regState = APP_PHONE_REG_STATE_SEARCHING;
    }
    else if( atServiceValue && !atRoamingValue)
    {
        // registered
        regState = APP_PHONE_REG_STATE_REGISTERED;
    }
    else if( atServiceValue && atRoamingValue)
    {   
        // registered and roaming
        regState = APP_PHONE_REG_STATE_SEARCHING;
    }

    DEBUG_VALUE4(APP_PHONE_SWI_TRC, MOD_APP_PHONE, "CanData: Map SvcVal %d, RoamVal %d to RegState %d %c",
                    atServiceValue, 
                    atRoamingValue,
                    regState,
                    ' ' );   // just a fill, cause there is no DEBUG_VALUE3

    _APP_PHONE_CAN_DATA_vSendRegisterState(regState);
}

/* End Of File APP_PHONE_CAN_DATA_C1.c */


