/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     BAP_Dispatcher.h
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
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_BAP_Dispatcher/trunk/inc/BAP_Dispatcher.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _BAP_DISPATCHER_H
#define _BAP_DISPATCHER_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include "Common.h"
/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/
#include "bap_types.h"      // vw
/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "BAP_DISPATCHER_CE.h"
/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Types                                                               */
/*----------------------------------------------------------------------------*/


/* Telephone specific */

typedef enum
{
    BAP_HANDSFREE_OFF   = 0x00,
    BAP_HANDSFREE_ON    = 0x01
}HandsFreeMode;

typedef enum
{
    BAP_MICRO_UNMUTE    = 0x00,
    BAP_MICRO_MUTE      = 0x01
}MicroMuteMode;

typedef enum
{
    BAP_RESET_FACTORY_SETTINGS_RESERVED = 0x00,
    BAP_RESET_FACTORY_SETTINGS_ALL      = 0x01,
    BAP_RESET_FACTORY_SETTINGS_USER     = 0x02
}BapRestoreFactorySettingsMode;

typedef enum
{
    BAP_LINE_STATE_LINE_FREE = 0x00,            /* 0  */
    BAP_LINE_STATE_NO_LINE,                     /* 1  */ 
    BAP_LINE_STATE_CONNECTED_LINE,              /* 2  */
    BAP_LINE_STATE_SYSTEM_BUSY,                 /* 3  */
    BAP_LINE_STATE_NUMBER_BUSY,                 /* 4  */
    BAP_LINE_STATE_NUMBER_NOT_ASSIGNED,         /* 5  */
    BAP_LINE_STATE_NUMBER_NOT_REACHABLE,        /* 6  */
    BAP_LINE_STATE_NETWORK_FAILURE,             /* 7  */ 
    BAP_LINE_STATE_CALL_BARRING_ACTIVE,         /* 8  */
    BAP_LINE_STATE_USER_NOT_RESPONDING,         /* 9  */
    BAP_LINE_STATE_CALL_REJECT,                 /* 10 */
    BAP_LINE_STATE_NUMBER_CHANGED,              /* 11 */
    BAP_LINE_STATE_NUMBER_INVALID_INCOMPLETE,   /* 12 */
    BAP_LINE_STATE_SERVICE_NOT_AVAILABLE,       /* 13 */
    BAP_LINE_STATE_NO_INFO_AVAILABLE            /* 14 */
} LineState;

typedef enum
{
    BAP_IDLE = 0,           /* 0 */  
    BAP_RINGING_WAITING,    /* 1 */
    BAP_ACTIVE,             /* 2 */
    BAP_DIALING,            /* 3 */
    BAP_DISCONNECTING,      /* 4 */
    BAP_ON_HOLD,            /* 5 */
    BAP_CONNECTED_CIB       /* 6 */
} CallState; 

typedef enum
{
    BAP_UNKNOWN_CALL_TYPE = 0,  /* 0 */
    BAP_REGULAR_VOICE_CALL,     /* 1 */
    BAP_DATA_CALL,              /* 2 */
    BAP_FAX_CALL,               /* 3 */
    BAP_EMERGENCY_CALL,         /* 4 */
    BAP_CONFERENCE,             /* 5 */
    BAP_INFO_CALL,              /* 6 */
    BAP_SERVICE_CALL            /* 7 */
} CallType;

typedef enum
{
    BAP_NOT_REGISTERED_AND_NOT_SEARCHING    = 0x00,
    BAP_REGISTERED                          = 0x01,
    BAP_NOT_REGISTERED_AND_SEARCHING        = 0x02,
    BAP_REGISTRATION_DENIED                 = 0x03,
    BAP_REGISTERED_AND_ROAMING              = 0x04,
    BAP_REGISTERED_AND_ROAMING_ALTERNATIVE  = 0x05,
    BAP_FUNCTION_NOT_SUPPORTED_BY_ME        = 0xFF
}BapRegisterState;

typedef enum
{
    BAP_OPSTATE_PHONE_INIT   = 0x00,
    BAP_OPSTATE_PHONE_DEFECT,
    BAP_OPSTATE_PHONE_MOD_OFF,
    BAP_OPSTATE_PHONE_MOD_SWITCHING_ON,
    BAP_OPSTATE_PHONE_MOD_ON,
    BAP_OPSTATE_PHONE_MOD_SWITCHING_OFF,
    BAP_OPSTATE_PHONE_MOD_OFF_HIGH_TEMP,
    BAP_OPSTATE_MOBILE_OFF,
    BAP_OPSTATE_MOBILE_SWITCHING_ON,
    BAP_OPSTATE_MOBILE_ON,
    BAP_OPSTATE_MOBILE_SWITCHING_OFF,
    BAP_OPSTATE_MOBILE_OFF_NO_SWITCH_ON,
    BAP_OPSTATE_MOBILE_NOT_ATTACHED,
    BAP_OPSTATE_MOBILE_ATTACHED_NOT_READY,
    BAP_OPSTATE_MOBILE_ATTACHED_NOT_FUNCTIONAL 
}FsgOperationState;


typedef enum
{
    BAP_NO_RECEPTION            = 0x00,     // 0
    BAP_VERY_LOW_RECEPTION      = 0x14,     // 20 
    BAP_LOW_RECEPTION           = 0x28,     // 40
    BAP_MID_RECEPTION           = 0x3C,     // 60
    BAP_GOOD_RECEPTION          = 0x50,     // 80
    BAP_BEST_RECEPTION          = 0x63,     // 99
    BAP_RECEPTION_NOT_SUPPORTED = 0xFF

}BapSignalQuality;

typedef enum
{
    BAP_FSGSTP_MCT_NO_CONNECTION            = 0x00,
    BAP_FSGSTP_MCT_INTERNAL_SIM_CARD_READER,
    BAP_FSGSTP_MCT_CABLE_CONNECTION,
    BAP_FSGSTP_MCT_HANDSFREE_PROFILE,
    BAP_FSGSTP_MCT_REMOTE_SIM_ACCESS_PROFILE
}BapFsgSetupMobileConnectionType;

/* End Telephone specific */


/* Phonebook specific */
typedef enum 
{
    BAP_SMS_TYPE_UNREAD         = 0x00,
    BAP_SMS_TYPE_READ           = 0x01,
    BAP_SMS_TYPE_UNREAD_CONCAT  = 0x02,
    BAP_SMS_TYPE_READ_CONCAT    = 0x03
}BapSmsType;

typedef enum
{
    BAP_PB_NOT_AVAILABLE                = 0x00,
    BAP_PB_LOADING                      = 0x01, 
    BAP_PB_LOAD_COMPLETE                = 0x02,
    BAP_PB_LOAD_INCOMPLETE_AVAILABLE    = 0x03,
    BAP_PB_LOAD_ABORTED                 = 0x04
} BapPbDwldState;

typedef enum
{
    BAP_NR_TYPE_UNKNOWN     = 0x00,
    BAP_NR_TYPE_GENERAL     = 0x01,
    BAP_NR_TYPE_MOBILE      = 0x02,
    BAP_NR_TYPE_OFFICE      = 0x03,
    BAP_NR_TYPE_HOME        = 0x04,
    BAP_NR_TYPE_FAX         = 0x05,
    BAP_NR_TYPE_PAGER       = 0x06,
    BAP_NR_TYPE_CAR         = 0x07, 
    BAP_NR_TYPE_SIM         = 0x08
}BapNumberType;

typedef enum
{
    BAP_SPELLER_MATCH       = 0x00,
    BAP_SPELLER_NEXT        = 0x01,
    BAP_SPELLER_PREV        = 0x02
}BapSpellerMode;

/* End Phonebook specific */


/* Data type in message from or to BAP */
typedef enum
{
    BAP_DT_VOID,
    BAP_DT_INT8,
    BAP_DT_INT16,
    BAP_DT_INT32,
    BAP_DT_BYTE,
    BAP_DT_ERROR
} BapDatatype;

typedef enum
{
    APP_BAPDISPATCHER_NOTIFY_CALLINFO_SENT,
    APP_BAPDISPATCHER_NOTIFY_USERLIST_SENT,
    APP_BAPDISPATCHER_NOTIFY_STATUSARRAY_SENT
}
APP_BAPDispatcher_NotificationType;

/* Holds the data for byte sequence data type */
typedef struct
{
	Uint16Type  length;
	Uint8Type	*data;
} BapByteSequence;

/* Message from BAP-Dispatcher to Application */
typedef struct
{
    lsgId_t                 lsg_id;
    fctId_t                 fct_id;
    enum BapIndication_t    ind_type;
    BapDatatype             data_type;
    union {
		BapByteSequence		byte_seq;
		Uint8Type  			u8_value;		
		Uint16Type			u16_value;		
		Uint32Type			u32_value;
		enum BapError_t		error_code;
	}						payload;
} BapIndication;

/* Message from Application to BAP-Dispatcher */
typedef struct
{
    lsgId_t                 lsg_id;
    fctId_t                 fct_id;
    enum BapRequest_t       req_type;
    BapDatatype             data_type;
    union
    {
		BapByteSequence		byte_seq;
		Uint8Type  			u8_value;		
		Uint16Type			u16_value;		
		Uint32Type			u32_value;
		enum BapError_t		error_code;
    }                       payload;
} BapRequest;




/*
#define BAP_FBLOCK_TELEPHONE            0x28
#define BAP_FBLOCK_PHONEBOOK            0x29
#define BAP_FBLOCK_SECONDARY_DISPLAY    0x2B
#define BAP_FBLOCK_MDI                  0x2F
*/

/*----------------------------------------------------------------------------*/
/* Global Variables Declarations (avoid as much as possible!)                 */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global const Declarations                                                  */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Methods                                                             */
/*----------------------------------------------------------------------------*/

SuccessFailType BAP_DISP_Initialization(void);
SuccessFailType BAP_DISP_Start(void);
SuccessFailType BAP_SD_Start(void);
SuccessFailType BAP_DISP_ShutDown(void);
SuccessFailType BAP_SD_ShutDown(void);
void    BAP_DISP_HandleEvent(EventMaskType eventMask);
void    BAP_IndicationVoid(lsgId_t aLsgId, fctId_t aFctId, enum BapIndication_t aeIndication);
void    BAP_IndicationInt8(lsgId_t aLsgId, fctId_t aFctId, enum BapIndication_t aeIndication, uint8_t au8Value);
void    BAP_IndicationInt16(lsgId_t aLsgId, fctId_t aFctId, enum BapIndication_t aeIndication, uint16_t au16Value);
void    BAP_IndicationInt32(lsgId_t aLsgId, fctId_t aFctId, enum BapIndication_t aeIndication, uint32_t au32Value);
void    BAP_IndicationError(lsgId_t aLsgId, fctId_t aFctId, enum BapError_t aeErrorCode);
void    BAP_IndicationByteSequence(lsgId_t aLsgId, fctId_t aFctId,
                                    enum BapIndication_t aeIndication,
                                    const volatile_ptr_t apValue,
                                    uint16_t au16Length);
enum BapError_t BAP_SendDebugInfo(const uint8_t pDebugInfo[7]);

#ifdef BAP_USES_ACKNOWLEDGE
BAP_FAR void BAP_Acknowledge(lsgId_t aLsgId
    , fctId_t aFctId
    , BapAcknowledge_et aeAcknowledge);
#endif
#endif /* _BAP_DISPATCHER_H */

/* End Of File BAP_Dispatcher.h */
