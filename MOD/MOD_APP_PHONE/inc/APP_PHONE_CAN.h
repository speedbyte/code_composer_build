/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:   APP_PHONE_CAN.h
*-----------------------------------------------------------------------------
* Module Name:  APP_PHONE    
*-----------------------------------------------------------------------------
* Description:  Global Interfaces, Enum, Structs for APP_PHONE
*-----------------------------------------------------------------------------
* $Date: 2011-05-13 09:39:35 +0200 (Fr, 13 Mai 2011) $
* $Rev: 19667 $
* $Author: maennel $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_PHONE/trunk/inc/APP_PHONE_CAN.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _APP_PHONE_CAN_H
#define _APP_PHONE_CAN_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include "common.h"
/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/
//#include "APP_COMMON_Utils.h"
/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Types                                                               */
/*----------------------------------------------------------------------------*/

typedef enum
{
    APP_PHONE_CAN_PHONE_ON,
    APP_PHONE_CAN_PHONE_MUTE,
    APP_PHONE_CAN_ACTIVE_CALL,
    APP_PHONE_CAN_PHONE_RING,
    APP_PHONE_CAN_PROVIDER,
    APP_PHONE_CAN_FIELD_STRENGTH,
    APP_PHONE_CAN_REGISTER_STATE,
    APP_PHONE_CAN_BTCONNECTIONSTATUS
}APP_PHONE_enCanDataId;

typedef enum
{
    APP_PHONE_REG_STATE_NO_SERVICE = 0,
    APP_PHONE_REG_STATE_REGISTERED = 1,
    APP_PHONE_REG_STATE_SEARCHING = 2,
    APP_PHONE_REG_STATE_REGISTERED_ROAMING = 5,
    APP_PHONE_REG_STATE_NOT_AVAILABLE = 7
}APP_PHONE_enCanRegisterState;


typedef struct
{
    APP_PHONE_enCanDataId id;
}APP_PHONE_CanDataCommon;

typedef struct
{
    APP_PHONE_CanDataCommon common;
    BooleanType             isOn;
}APP_PHONE_CanPhoneOn;

typedef struct
{
    APP_PHONE_CanDataCommon     common;
    Uint8Type                   muteUnmute;
    APP_CAN_enSourceIdType      signalSource;
}APP_PHONE_CanPhoneMuteBehavior;

typedef struct
{
    APP_PHONE_CanDataCommon common;
    BooleanType             isActive;
}APP_PHONE_CanActiveCall;

typedef struct
{
    APP_PHONE_CanDataCommon common;
    BooleanType             isRinging;
}APP_PHONE_CanPhoneRing;

typedef struct
{
    APP_PHONE_CanDataCommon common;
    AT_IndicatorString2     name;
    Uint16Type              name_len;
}APP_PHONE_CanProvider;

typedef struct
{
    APP_PHONE_CanDataCommon common;
    AT_IndicatorValue       value;
}APP_PHONE_CanFieldStrenght;

typedef struct
{
    APP_PHONE_CanDataCommon      common;
    APP_PHONE_enCanRegisterState state;
}APP_PHONE_CanRegisterState;

typedef struct
{
    APP_PHONE_CanDataCommon     common;
    BooleanType                 IsConnected;
}APP_PHONE_BTConnectionState;
typedef union
{
    APP_PHONE_CanDataCommon         common;
    APP_PHONE_CanPhoneOn            phoneOn;
    APP_PHONE_CanPhoneMuteBehavior  muteBehavior;
    APP_PHONE_CanActiveCall         activeCall;
    APP_PHONE_CanPhoneRing          phoneRing;
    APP_PHONE_CanProvider           provider;
    APP_PHONE_CanFieldStrenght      fieldStrength;   
    APP_PHONE_CanRegisterState      registerState;
    APP_PHONE_BTConnectionState     BTstate;
}APP_PHONE_CanDataType;

/*----------------------------------------------------------------------------*/
/* Global Variables Declarations (avoid as much as possible!)                 */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Methods                                                             */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/
#endif /* _APP_PHONE_CAN_H */

/* End Of File APP_PHONE_CAN.h */

