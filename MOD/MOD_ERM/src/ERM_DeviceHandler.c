/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     ERM_DeviceHandler.c
*-----------------------------------------------------------------------------
* Module Name:    External resource manager - device handler part
*-----------------------------------------------------------------------------
* Description:    External resource manager global functions.
*                 the ERM is responsible for control of the UART resource
*                 towards the different other application modules
*-----------------------------------------------------------------------------
* $Date: 2011-11-23 08:25:57 +0100 (Mi, 23 Nov 2011) $
* $Rev: 24103 $
* $Author: maennel $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_ERM/trunk/src/ERM_DeviceHandler.c $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include "osek.h"
#include "osekext.h"
#include "common.h"
#include "APP_COMMON_Utils.h"
#include "UART.h"
#include "ATParser.h"
#include "ATCmdHandler.h"
#include "POOL.h"
#include "APP_PHONE_AT_PNCC.h"
#include "APP_PHONE_ERM.h"
#include "APP_PHONE_ERM.h"
#include "APP_PHONE_BAP.h"
#include "APP_PHONE_BAP_ERM.h"
#include "APP_PHONEBOOK_ERM.h"
#include "APP_MDI_ERM.h"
#include "APP_MDI_BAP.h"
#include "OSEK_AppTask_H1.h"
#include "VCR.h"
#include "ATCommonData.h"
#include "APP_MDI_ERM.h"
#include "APP_SD.h"
#include "APP_SMS_ERM.h"
#include "EEPROM_LAYOUT_CFG.h"
#include "APP_PHONEBOOK_BAP.h"
#include "CONTEXT_MAN.h"

#include "ERM_DeviceHandler.id"
#include "TRACE.h"

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "ERM.h"
#include "ERM_CE.h"
#include "ERM_AudioSets.h"
#include "ERM_Utils.h"
#include "ERM_MessageHandler.h"
#include "ERM_DeviceHandler.h"

/*----------------------------------------------------------------------------*/
/* pragma Directives                                                          */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Definitions (avoid as much as possible!)                  */
/*----------------------------------------------------------------------------*/
extern ERMContextType        _ERM_gContext;
/*----------------------------------------------------------------------------*/
/* Private Manifest Constant Definitions                                      */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private Types                                                              */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Prototypes of file local Methods                                           */
/*----------------------------------------------------------------------------*/
/**
\brief  description of the method
*/

static SuccessFailType _ERM_sfConsistencyCheck_BTDeviceTable(void);
static SuccessFailType _ERM_sfConsistencyCheck_PhyDeviceTable(void);
static SuccessFailType _ERM_sfConsistencyCheck_UserTable(void);
static SuccessFailType _ERM_sfConsistencyCheck_PlayerHeadsetTable(void);
static SuccessFailType _ERM_sfConsistencyCheck_ActiveProfile_User(void);
static SuccessFailType _ERM_sfConsistencyCheck_ActiveProfile_Player(void);
static SuccessFailType _ERM_sfConsistencyCheck_ActiveProfile_Headset(void);

static void _ERM_vSetStartupDone(void);

/*----------------------------------------------------------------------------*/
/* File local Code Macros                                                     */
/*----------------------------------------------------------------------------*/
/**
\brief  description of the macros
*/

/*----------------------------------------------------------------------------*/
/* File local Variables                                                       */
/*----------------------------------------------------------------------------*/
ERMBTDeviceTableType    _ERM_stBTDeviceTable[AT_MAX_DEVICE_ID+1] = {ERM_DEVICE_STATE_EMPTY,
                                                                    AT_DEVICE_INVALID,
                                                                    AT_FALSE, AT_RTMODE_DEACTIVATE, AT_FALSE,
                                                                    AT_NOT_SUPPORTED, AT_NOT_SUPPORTED, AT_NOT_SUPPORTED, AT_FALSE,
                                                                    0,{0},{0},0,AT_FALSE,
                                                                    AT_NOT_SUPPORTED, AT_FALSE,
                                                                    AT_NOT_SUPPORTED, AT_FALSE,
                                                                    AT_NOT_SUPPORTED, AT_FALSE,
                                                                    AT_NOT_SUPPORTED, AT_FALSE,
                                                                    AT_NOT_SUPPORTED, AT_FALSE,
                                                                    AT_NOT_SUPPORTED, AT_FALSE
                                                                    };

ERMPhysicalBTDeviceType _ERM_stPhyDevTable[ERM_MAX_NUM_PHYSICAL_DEVICES] = {0};
ERMUserProfileType      _ERM_stUserTable[ERM_MAX_NUM_USER_PROFILES] = {0};
ERMPlayerHeadsetProfileType  _ERM_stPlayerHeadsetTable[ERM_MAX_NUM_PLAYER_HS_TABLE] = {0};
ERMActiveProfileListType     _ERM_stActiveProfileList;
/*----------------------------------------------------------------------------*/
/* File local const Definitions                                               */
/*----------------------------------------------------------------------------*/
/**
\brief  description of the private constant
*/

/*----------------------------------------------------------------------------*/
/* Private Address Calculations                                               */
/*----------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------*/
/* Methods                                                                    */
/*----------------------------------------------------------------------------*/


void            _ERM_vPrintTableSize(void)
{
    DEBUG_VALUE2(ERM_SWI_TRC, MOD_ERM,"ERM devicetable: BTDev: %d PhyDev: %d",
            sizeof(_ERM_stBTDeviceTable),
            sizeof(_ERM_stPhyDevTable));
    DEBUG_VALUE2(ERM_SWI_TRC, MOD_ERM,"ERM devicetable: UserTable: %d PlayerHsTable %d",
            sizeof(_ERM_stBTDeviceTable),
            sizeof(_ERM_stPhyDevTable));
    DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM,"ERM devicetable: ActProfileList %d",
            sizeof(_ERM_stActiveProfileList));
}


void _ERM_vInitTmpEpromData(void)
{
    //_ERM_gContext.epr_tmp.deviceId                      = AT_INVALID_DEVICE_ID;
    _ERM_gContext.epr_tmp.bUpdateUserEepromNeeded       = BT_FALSE;
    _ERM_gContext.epr_tmp.bUpdateUserRamNeeded          = BT_FALSE;
    _ERM_gContext.epr_tmp.u8UserIdx                     = AT_INVALID_DEVICE_ID;
    _ERM_gContext.epr_tmp.bUpdatePlayHSEepromNeeded     = BT_FALSE;
    _ERM_gContext.epr_tmp.bUpdatePlayHSRamNeeded        = BT_FALSE;
    _ERM_gContext.epr_tmp.u8PlayHSIdx                   = AT_INVALID_DEVICE_ID;
    _ERM_gContext.epr_tmp.bUpdatePhyEepromNeeded        = BT_FALSE;
    _ERM_gContext.epr_tmp.bUpdatePhyRamNeeded           = BT_FALSE;
    _ERM_gContext.epr_tmp.u8PhyIdx                      = AT_INVALID_DEVICE_ID;
    _ERM_gContext.epr_tmp.bUpdateActProfileEepromNeeded = BT_FALSE;
    _ERM_gContext.epr_tmp.bUpdateActProfileRamNeeded    = BT_FALSE;
    (void)memset(&_ERM_gContext.epr_tmp.user,0x00,
            sizeof(ERMUserProfileType));
    (void)memset(&_ERM_gContext.epr_tmp.playHs,0x00,
            sizeof(ERMPlayerHeadsetProfileType));
    (void)memset(&_ERM_gContext.epr_tmp.phy,0x00,
            sizeof(ERMPhysicalBTDeviceType));
    (void)memset(&_ERM_gContext.epr_tmp.actList,0xFF,
            sizeof(ERMActiveProfileListType));

}

BooleanType ERM_bDevConnectedOnBtLevel(AT_DeviceId devId)
{
    BooleanType bConnected = BT_FALSE;

    if(devId <= AT_MAX_DEVICE_ID)
    {
        switch(_ERM_stBTDeviceTable[devId].device_connected)
        {
            case AT_DEVICE_CONNECTED:   /* japp, device is connected on bt level */
                bConnected = BT_TRUE;
                break;
            default:
                break;
        }
    }

    return bConnected;
}


/*----------------------------------------------------------------------------*/
/* Function    : _ERM_vInitActiveProfileList                                  */
/**
    \brief      init active profile list

    \brief

    \param      none

    \return     none
   */
/*----------------------------------------------------------------------------*/
void            _ERM_vInitActiveProfileList(void)
{
    (void)memset(&_ERM_stActiveProfileList,
            AT_INVALID_DEVICE_ID,
            sizeof(_ERM_stActiveProfileList));
}

/*----------------------------------------------------------------------------*/
/* Function    : _ERM_vInitPhysicalDeviceTable                                */
/**
    \brief      initialise all members of physical device table

    \brief

    \param      none

    \return     none
   */
/*----------------------------------------------------------------------------*/
void _ERM_vInitPhysicalDeviceTable(void)
{
    Uint32Type i = 0;
    /* TODO - read values from EPROM */
    (void)memset(&_ERM_stPhyDevTable[0],0x00,
            sizeof(ERMPhysicalBTDeviceType) * ERM_MAX_NUM_PHYSICAL_DEVICES);
    for (i = (Uint32Type)0U; i < (Uint32Type)ERM_MAX_NUM_PHYSICAL_DEVICES;i++)
    {
        _ERM_stPhyDevTable[i].deviceID = AT_INVALID_DEVICE_ID;
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : _ERM_vInitUserProfileTable                                   */
/**
    \brief      initialise all members of user profile table

    \brief

    \param      none

    \return     none
   */
/*----------------------------------------------------------------------------*/
void _ERM_vInitUserProfileTable(void)
{
    Uint32Type i = 0;
    /* TODO - read values from EPROM */
    (void)memset(&_ERM_stUserTable[0],0x00,
            sizeof(ERMUserProfileType) * ERM_MAX_NUM_USER_PROFILES);
    for (i = 0; i <ERM_MAX_NUM_USER_PROFILES;i++)
    {
        (void)_ERM_sfSetUserProfileEntryToDefault(&_ERM_stUserTable[i]);
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : _ERM_sfSetUserProfileEntryToDefault                          */
/**
    \brief      initialise all members of a user profile structure element

    \brief      used pointer to allow also init of temp storage

    \param      ERMUserProfileType pointer

    \return     none
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ERM_sfSetUserProfileEntryToDefault(ERMUserProfileType * p_entry)
{
    SuccessFailType sF = FAIL;
    if (NULL != p_entry)
    {
        p_entry->deviceIndex          = AT_INVALID_DEVICE_ID;
        p_entry->playerIndex          = AT_INVALID_DEVICE_ID;
        p_entry->headSetIndex         = AT_INVALID_DEVICE_ID;
        p_entry->voicemailbox_len     = 0;
        (void)memset(&(p_entry->voicemailbox[0]),0x00,sizeof(p_entry->voicemailbox));
        p_entry->ringtone             = AT_RTMODE_RINGRING;
        p_entry->ringtone_alternating = BT_FALSE;
        p_entry->audio_mode           = ERM_AUDIO_MODE_CALLS_ONLY;
        p_entry->bPbLoadedCompletly   = BT_FALSE;
        (void)memset(&(p_entry->spare[0]),0x00,sizeof(p_entry->spare));
        sF = SUCCESS;
    }
    return sF;
}


/*----------------------------------------------------------------------------*/
/* Function    : _ERM_vInitPlayerHeadsetProfileTable                          */
/**
    \brief      initialise all members of player profile table

    \brief

    \param      none

    \return     none
   */
/*----------------------------------------------------------------------------*/
void _ERM_vInitPlayerHeadsetProfileTable(void)
{
    Uint32Type i = 0;
    /* TODO - read values from EPROM */

    for (i = (Uint32Type)0U; i < (Uint32Type)ERM_MAX_NUM_PLAYER_HS_TABLE; i++)
    {
        _ERM_stPlayerHeadsetTable[i].deviceIndex    = AT_INVALID_DEVICE_ID;
        _ERM_stPlayerHeadsetTable[i].deviceType     = APP_DEVICE_enTYPE_UNKNOWN;
        _ERM_stPlayerHeadsetTable[i].bAuthNeeded    = BT_FALSE;
        _ERM_stPlayerHeadsetTable[i].bAutoStartPlay = BT_FALSE;
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : _ERM_sfSetPlayerHeadsetProfileEntryToDefault                          */
/**
    \brief      initialise all members of a player / headset profile structure element

    \brief      used pointer to allow also init of temp storage

    \param      ERMPlayerHeadsetProfileType pointer

    \return     none
   */
/*----------------------------------------------------------------------------*/
SuccessFailType
_ERM_sfSetPlayerHeadsetProfileEntryToDefault(ERMPlayerHeadsetProfileType * p_entry,
                                             APP_DEVICE_DeviceType deviceType)
{
    SuccessFailType sF = FAIL;
    if (NULL != p_entry)
    {
        p_entry->deviceIndex    = AT_INVALID_DEVICE_ID;
        p_entry->deviceType     = deviceType;
        p_entry->bAuthNeeded    = BT_FALSE;
        p_entry->bAutoStartPlay = BT_FALSE;     // used for player
        sF = SUCCESS;
    }
    return sF;
}

SuccessFailType
_ERM_sfSetPhysicalDeviceEntryToDefault(ERMPhysicalBTDeviceType * p_entry,
                                       AT_DeviceId deviceId,
                                       const Uint8Type *pName,
                                       Uint16Type nameLen)
{
    const ERMBTDeviceTableType *pDevice = NULL;

    SuccessFailType sF = FAIL;
    if (NULL != p_entry)
    {
        /* set device id */
        p_entry->deviceID              = deviceId;

        /* change the name in tmp storage */
        (void)memset(&(p_entry->name[0]),0x00,sizeof(AT_DeviceNameString));
        if (nameLen > sizeof(AT_DeviceNameString))
        {
            nameLen = (Uint16Type)sizeof(AT_DeviceNameString);
        }
        p_entry->name_len = nameLen;
        (void)memcpy( &(p_entry->name[0]),
                pName,
                p_entry->name_len);

        p_entry->deviceID = deviceId;
        (void)memset(&(p_entry->pincode[0]),0x00,sizeof( ATPinCodeBT));

        p_entry->numVoiceTags       = 0;
        p_entry->bVTDeleteOngoing   = BT_FALSE;

        pDevice = ERM_GetBTDevice(deviceId);
        if(pDevice != NULL)
        {
            (void)memcpy( p_entry->bt_addr,
                    pDevice->bt_addr,
                    sizeof(AT_BT_Address));
        }
        else
        {
            (void)memset(p_entry->bt_addr, 0x00, sizeof(AT_BT_Address));
        }

        sF = SUCCESS;
    }
    return sF;
}


/*----------------------------------------------------------------------------*/
/* Function    : _ERM_vInitBTDeviceTable                                      */
/**
    \brief      initialise all members of bluetooth device table

    \brief      called Init for single device

    \param      none

    \return     none
   */
/*----------------------------------------------------------------------------*/
void _ERM_vInitBTDeviceTable(void)
{
    AT_DeviceId tmp = 0;

    for (tmp = 0; tmp <= AT_MAX_DEVICE_ID;tmp++)
    {
        (void)_ERM_sFInitBTDeviceValues(tmp);
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : _ERM_sFInitBTDeviceValues                                    */
/**
    \brief      initialise given member of the BT device table

    \brief      check that given id is in allowed range and init all members
                for this device

    \param      id - number of device in range 0..AT_MAX_DEVICE_ID
                AT_DeviceId

    \return     SuccessFailType
                SUCCESS if member was initialised and id was in range
                FAIL if id was out of range
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ERM_sFInitBTDeviceValues(AT_DeviceId id)
{
    SuccessFailType sF = SUCCESS;

    if (id <= AT_MAX_DEVICE_ID)
    {   /* id in range - init */
        _ERM_stBTDeviceTable[id].device_state          = ERM_DEVICE_STATE_EMPTY;
        _ERM_stBTDeviceTable[id].device_connected      = AT_DEVICE_INVALID;
        _ERM_stBTDeviceTable[id].device_active         = AT_FALSE;
        _ERM_stBTDeviceTable[id].device_ringtone       = AT_RTMODE_ACTIVATE;
        _ERM_stBTDeviceTable[id].support_dial          = AT_NOT_SUPPORTED;
        _ERM_stBTDeviceTable[id].support_pb_access     = AT_NOT_SUPPORTED;
        _ERM_stBTDeviceTable[id].phone_supported       = AT_NOT_SUPPORTED;
        _ERM_stBTDeviceTable[id].phone_active          = AT_FALSE;

#ifdef ERM_FULL_DEVICE_TABLE
        _ERM_stBTDeviceTable[id].device_name_len       = 0;
        (void)memset(_ERM_stBTDeviceTable[id].device_name,0x00,AT_DEVICE_NAME_MAX_LEN+1);
        (void)memset(_ERM_stBTDeviceTable[id].bt_addr,0x00,AT_BT_ADDRESS_LEN);
        (void)memset( &(_ERM_stBTDeviceTable[id].bt_ClassOfDevice),
                0x00,
                sizeof(_ERM_stBTDeviceTable[id].bt_ClassOfDevice));
        _ERM_stBTDeviceTable[id].bAudioVolumeSet       = AT_FALSE;

        _ERM_stBTDeviceTable[id].dun_supported         = AT_NOT_SUPPORTED;
        _ERM_stBTDeviceTable[id].dun_active            = AT_FALSE;
        _ERM_stBTDeviceTable[id].a2dp_sink_supported   = AT_NOT_SUPPORTED;
        _ERM_stBTDeviceTable[id].a2dp_sink_active      = AT_FALSE;
        _ERM_stBTDeviceTable[id].a2dp_source_supported = AT_NOT_SUPPORTED;
        _ERM_stBTDeviceTable[id].a2dp_source_active    = AT_FALSE;
        _ERM_stBTDeviceTable[id].avrcp_supported       = AT_NOT_SUPPORTED;
        _ERM_stBTDeviceTable[id].avrcp_active          = AT_FALSE;
        _ERM_stBTDeviceTable[id].audio_gw_supported    = AT_NOT_SUPPORTED;
        _ERM_stBTDeviceTable[id].audio_gw_active       = AT_FALSE;
        _ERM_stBTDeviceTable[id].spp_supported         = AT_NOT_SUPPORTED;
        _ERM_stBTDeviceTable[id].spp_active            = AT_FALSE;
        _ERM_stBTDeviceTable[id].sdp_received          = AT_FALSE;
#endif
    }
    else
    {
        DEBUG_VALUE1(ERM_SWI_ERR, MOD_ERM,"InitBTDeviceValues %d ooR",id);

        sF = FAIL;
    }
    return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : _ERM_eGetActiveDeviceRingtone                                */
/**
    \brief      return active device ringtone setting

    \brief

    \param      none

    \return     ATRintoneModeEnum
                ringtone identification for current active device,
                default if no activce device
   */
/*----------------------------------------------------------------------------*/
ATRintoneModeEnum _ERM_eGetActiveDeviceRingtone(void)
{
    ATRintoneModeEnum retVal = AT_RTMODE_DEACTIVATE;
    //if ( (TRUE == _ERM_gContext.u8ActiveBTDevice) &&
    //     (_ERM_gContext.u8ActiveBTId <= AT_MAX_DEVICE_ID)      )
    if ( _ERM_gContext.u8ActiveBTId <= AT_MAX_DEVICE_ID )
    {
        retVal = _ERM_stBTDeviceTable[_ERM_gContext.u8ActiveBTId].device_ringtone;
    }
    return retVal;
}

/*----------------------------------------------------------------------------*/
/* Function    : _ERM_eIntSetActiveDeviceRingtone                             */
/**
    \brief      set ringtone for active device

    \brief      Note: The caller itself will trigger the message to update RT

    \param      ATRintoneModeEnum
                ringtone identification for current active device,
                default if no activce device

    \return     SuccessFailType SUCCESS if setting OK , FAIL otherwise
   */
/*----------------------------------------------------------------------------*/
SuccessFailType   _ERM_eIntSetActiveDeviceRingtone(ATRintoneModeEnum value)
{
    SuccessFailType sF = SUCCESS;

    //if ( (TRUE == _ERM_gContext.u8ActiveBTDevice) &&
    //    (_ERM_gContext.u8ActiveBTId <= AT_MAX_DEVICE_ID)      )
    if ( _ERM_gContext.u8ActiveBTId <= AT_MAX_DEVICE_ID )
    {   /* check value range */
        switch (value)
        {
            case   AT_RTMODE_DEACTIVATE:
            case   AT_RTMODE_ACTIVATE:
            case   AT_RTMODE_CUSTOM8:
            case   AT_RTMODE_CUSTOM9:
            case   AT_RTMODE_VIBORBELLZ:
            case   AT_RTMODE_BIMBAM:
            case   AT_RTMODE_EASYLISTEN:
            case   AT_RTMODE_FUTUREBEAT:
            case   AT_RTMODE_ENDLESS:
            case   AT_RTMODE_RINGRING:
            case   AT_RTMODE_NOSTALGIC:
            case   AT_RTMODE_ORCHESTRA1:
            case   AT_RTMODE_ORCHESTRA2:
            case   AT_RTMODE_SWEETSOFT:
            case   AT_RTMODE_TUTTUT:
            // do not longer exist
            //case   AT_RTMODE_VW_12:
            //case   AT_RTMODE_VW_13:
            //case   AT_RTMODE_VW_14:
            //case   AT_RTMODE_VW_15:
            {
                _ERM_stBTDeviceTable[_ERM_gContext.u8ActiveBTId].device_ringtone = value;
                break;
            }
            default:
            {
                sF = FAIL;
                break;
            }
        }
    }
    else
    {
        sF = FAIL;
    }
    return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : _ERM_eGetActiveDeviceRingtoneAlternate                                */
/**
    \brief      return active device alternate ringtone setting

    \brief

    \param      none

    \return     BooleanType
                ringtone identification for current active device,
                default if no activce device
   */
/*----------------------------------------------------------------------------*/
BooleanType _ERM_eGetActiveDeviceRingtoneAlternate(void)
{
    BooleanType retVal = BT_FALSE;
    //if ( (TRUE == _ERM_gContext.u8ActiveBTDevice) &&
    //     (_ERM_gContext.u8ActiveBTId <= AT_MAX_DEVICE_ID)      )
    if ( _ERM_gContext.u8ActiveBTId <= AT_MAX_DEVICE_ID )
    {
        retVal = (BooleanType)_ERM_stBTDeviceTable[_ERM_gContext.u8ActiveBTId].device_ringtone_alternate;
    }
    return retVal;
}

/*----------------------------------------------------------------------------*/
/* Function    : _ERM_eIntSetActiveDeviceRingtoneAlternate                             */
/**
    \brief      set ringtone for active device

    \brief      Note: The caller itself will trigger the message to update RT

    \param      BooleanType
                ringtone identification for current active device,
                default if no activce device

    \return     SuccessFailType SUCCESS if setting OK , FAIL otherwise
   */
/*----------------------------------------------------------------------------*/
SuccessFailType   _ERM_eIntSetActiveDeviceRingtoneAlternate(BooleanType value)
{
    SuccessFailType sF = SUCCESS;

    //if ( (TRUE == _ERM_gContext.u8ActiveBTDevice) &&
    //    (_ERM_gContext.u8ActiveBTId <= AT_MAX_DEVICE_ID)      )
    if ( _ERM_gContext.u8ActiveBTId <= AT_MAX_DEVICE_ID )
    {
        _ERM_stBTDeviceTable[_ERM_gContext.u8ActiveBTId].device_ringtone_alternate = (ATBooleanEnum)value;
    }
    else
    {
        sF = FAIL;
    }
    return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_GetActiveBTDeviceId                                      */
/**
    \brief      return active device id towards caller

    \brief      function check if there is current active BT device\n
                if so given pointer value is set to current active device id\n
                and function return SUCCESS, otherwise function return failure\n
                and given pointer not modified

    \param      device_id pointer to fill device id in
                AT_DeviceId
                all values

    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType ERM_GetActiveBTDeviceId(AT_DeviceId * device_id)
{
    SuccessFailType sF = FAIL;

    *device_id = ERM_u8GetActivePhone();

    if(*device_id != AT_INVALID_DEVICE_ID)
    {
        sF = SUCCESS;
    }

    /* else return default fail */
    return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_DeleteDeviceIfPpdeTimeout                                */
/**
    \brief      no return value needed

    \brief      function deletes the device with id device_id\n                

    \param      device_id 
                AT_DeviceId
                all values

    \return
   */
/*----------------------------------------------------------------------------*/
void ERM_DeleteDeviceIfPpdeTimeout(AT_DeviceId device_id)
{
    if( device_id <= AT_MAX_DEVICE_ID)
    {
        // delete info about this device and do ConsistencyCheck
        (void)_ERM_sFInitBTDeviceValues(device_id);
        (void)_ERM_sfConsistencyCheck();
    }
    else
    {
        DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM,"ERM_DeleteDeviceIfPpdeTimeout INVALID DEVICEID");
    }   
}

///*----------------------------------------------------------------------------*/
///* Function    : _ERM_CheckActiveBTDevice                                     */
///**
//    \brief      check if an active BT device exists
//
//    \brief      the function check if ERM global variable \n
//                _ERM_gContext.u8ActiveBTDevice\n
//                indicate if an active BT device exists or not
//
//    \param      none
//
//    \return     SuccessFailType sF
//                SUCCESS or FAIL
//   */
///*----------------------------------------------------------------------------*/
//SuccessFailType _ERM_CheckActiveBTDevice(void)
//{
//    SuccessFailType sF = SUCCESS;
//    if ( (FALSE == _ERM_gContext.u8ActiveBTDevice) ||
//         (AT_INVALID_DEVICE_ID == _ERM_gContext.u8ActiveBTId) )
//    {
//        sF = FAIL;
//    }
//    return sF;
//}

/*----------------------------------------------------------------------------*/
/* Function    : _ERM_sFBTDeviceConnectionUpdate                              */
/**
    \brief      update for given device the connection value

    \brief      check that given id is in allowed range

    \param      id - number of device in range 0..AT_MAX_DEVICE_ID
                AT_DeviceId

    \param      connected - indicate if device has connected or disconnected
                ATBooleanEnum

    \return     SuccessFailType
                SUCCESS if member was initialised and id was in range
                FAIL if id was out of range
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ERM_sFBTDeviceConnectionUpdate(AT_DeviceId id,
                                                ATBooleanEnum  connected)
{
    SuccessFailType sF = SUCCESS;
    Uint8Type i;

    if (AT_MAX_DEVICE_ID >= id)
    {   /* id in range - modify, therefore check old / new status */
        if (AT_FALSE == connected)
        {   /* init this device entries */
            /* we have a disconnect */
            _ERM_stBTDeviceTable[id].device_connected = AT_DEVICE_NOT_CONNECTED;
        }
        else
        {   /* we have a connect, set connected state and check if no
               connected at the moment, if so modify to "active" */
            _ERM_stBTDeviceTable[id].device_connected = AT_DEVICE_CONNECTED;
        }

        DEBUG_VALUE2(ERM_SWI_ERR, MOD_ERM,"ERM DeviceConnectionUpdate id: (%d connected %d)",
                id, connected);
        DEBUG_VALUE4(ERM_SWI_ERR, MOD_ERM,"ERM DeviceConnectionUpdate %d %d %d %d",
                _ERM_stBTDeviceTable[0].device_connected,
                _ERM_stBTDeviceTable[1].device_connected,
                _ERM_stBTDeviceTable[2].device_connected,
                _ERM_stBTDeviceTable[3].device_connected);
        DEBUG_VALUE1(ERM_SWI_ERR, MOD_ERM,"ERM DeviceConnectionUpdate .............%d ",
                _ERM_stBTDeviceTable[4].device_connected);
    }
    else
    {

        DEBUG_VALUE1(ERM_SWI_ERR, MOD_ERM,"BTDeviceConnectionUpdate %d ooR",id);
        sF = FAIL;
    }

    /* check if at least one device is connected. */
    for ( i = 0;i < AT_MAX_DEVICE_ID; i++){
        if (AT_DEVICE_CONNECTED == _ERM_stBTDeviceTable[i].device_connected){
            break;
        }
    }
    /* Inform ADD_CAN about BT connection state. */
    if (AT_MAX_DEVICE_ID == i)
    {
        _ERM_vUpdateCANBTConnectionState(BT_FALSE);
    }
    else {
        _ERM_vUpdateCANBTConnectionState(BT_TRUE);
    }

    return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : _ERM_vUpdateCANBTConnectionState                             */
/**
    \brief      This functions update the BT connection state inside APP_CAN.

    \brief      This function is called by BT device connection update function
                and sends the given value as BT connections state to APP_CAN.

    \param      bBTState - state that should be send on CAN.

    \return     none

   */
/*----------------------------------------------------------------------------*/
void _ERM_vUpdateCANBTConnectionState(BooleanType bBTState) {

    APP_PHONE_CanDataType canData;

     canData.common.id = APP_PHONE_CAN_BTCONNECTIONSTATUS;
     canData.BTstate.IsConnected = bBTState;
     (void)SendMessage(msgTelefonStates, &canData);
     DEBUG_STATE1(ERM_SWI_TRC, MOD_ERM, "Send Message to APP_CAN with BT state $BooleanType$ = %d ", bBTState);

}

/*----------------------------------------------------------------------------*/
/* Function    : _ERM_sFBTSdpUpdate                                           */
/**


    \param

    \return     SuccessFailType
                SUCCESS if member was initialised and id was in range
                FAIL if id was out of range
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ERM_sFBTSdpUpdate(const ATRspSdpNotType *pPsdp)
{
    SuccessFailType sF = SUCCESS;

    if (pPsdp->id <= AT_MAX_DEVICE_ID)
    {   /* id in range */
        _ERM_stBTDeviceTable[pPsdp->id].sdp_received = pPsdp->status == AT_SDP_SUCCEED ? AT_TRUE : AT_FALSE;
    }
    else
    {
        DEBUG_VALUE1(ERM_SWI_ERR, MOD_ERM,"_ERM_sFBTSdpUpdate %d ooR",pPsdp->id);
        sF = FAIL;
    }
    return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : _ERM_sFBTSupportedServiceUpdate                              */
/**
    \brief

    \brief

    \param


    \param

    \return     SuccessFailType
                SUCCESS if member was initialised and id was in range
                FAIL if id was out of range
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ERM_sFBTSupportedServiceUpdate(
        const ATResBTServiceConnType *pPbsc)
{
    SuccessFailType sF = SUCCESS;

    if (pPbsc->device_id <= AT_MAX_DEVICE_ID)
    {   /* id in range */
        ERMNotificationType notification;

        /* set phone service support state */
        _ERM_stBTDeviceTable[pPbsc->device_id].phone_supported     = pPbsc->res1;
        /* set A2DP service support state */
        _ERM_stBTDeviceTable[pPbsc->device_id].a2dp_sink_supported = pPbsc->res3;
        /* set AVRCP service support state */
        _ERM_stBTDeviceTable[pPbsc->device_id].avrcp_supported     = pPbsc->res5;
        /* set AUdioGateway service support state */
        _ERM_stBTDeviceTable[pPbsc->device_id].audio_gw_supported  = pPbsc->res6;

        _ERM_gContext.u8LastServiceDeviceId = pPbsc->device_id;

        notification.common.id = ERM_NOTIFY_SERVICE_SUPPORT_CHANGED;
        notification.pbsc.device_id = pPbsc->device_id;

        _ERM_vNotifyApplication(&notification);
    }
    else
    {

        DEBUG_VALUE1(ERM_SWI_ERR, MOD_ERM,"BTSupportedServiceUpdate %d ooR",pPbsc->device_id);
        sF = FAIL;
    }
    return sF;
}
/*----------------------------------------------------------------------------*/
/* Function    : _ERM_sFBTDillDeviceInfo                                      */
/**
    \brief      fill respective device table content with received info

    \brief      check that given id is in allowed range

    \param      pst_msg - pointer to response message
                ATRspPlpdInfoType

    \return     SuccessFailType
                SUCCESS if member was initialised and id was in range
                FAIL if id was out of range
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ERM_sFBTFillDeviceInfo(ATRspPlpdInfoType *p_info)
{
    SuccessFailType sF = FAIL;
    ERMBTDeviceTableType * p_tab = NULL;

    /* check given device id is in range */
    if (AT_MAX_DEVICE_ID >= p_info->device_id)
    {   /* fill values in respective table index */
        p_tab = &_ERM_stBTDeviceTable[p_info->device_id];

        /* we accept no update for a device that will be released */
        if(p_tab->device_state != ERM_DEVICE_STATE_RELEASED)
        {
            p_tab->device_state       = ERM_DEVICE_STATE_USED;
            p_tab->device_active      = p_info->is_active;
            p_tab->support_dial       = p_info->support_dial;
            p_tab->support_pb_access  = p_info->support_pb_access;
            if (AT_TRUE == p_info->is_connected)
            {
                p_tab->device_connected   = AT_DEVICE_CONNECTED;
            }
            else
            {
                p_tab->device_connected   = AT_DEVICE_NOT_CONNECTED;
            }
            sF = SUCCESS;
            /* TODO check here first if this trace is switched on!! */

#ifdef ERM_FULL_DEVICE_TABLE

            (void)memcpy(&(p_tab->bt_ClassOfDevice),&(p_info->bt_ClassOfDevice),sizeof(p_tab->bt_ClassOfDevice));
            (void)memcpy(p_tab->bt_addr,p_info->bt_addr,sizeof(p_tab->bt_addr));

            /* update the device name only if we have no valid name set already */
            if(p_tab->device_name_len == 0 || p_tab->device_name_len > AT_DEVICE_NAME_MAX_LEN)
            {
                p_tab->device_name_len    = p_info->device_name_len > AT_DEVICE_NAME_MAX_LEN ?
                                                AT_DEVICE_NAME_MAX_LEN : p_info->device_name_len;

                (void)memcpy(p_tab->device_name,p_info->device_name,p_tab->device_name_len);
                p_tab->device_name[p_tab->device_name_len] = '\0';
            }
#endif
        }
    }
    else
    {

        DEBUG_VALUE1(ERM_SWI_ERR, MOD_ERM,"FillDeviceInfo id ooR %d",p_info->device_id);
    }

    return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : _ERM_sFBTDeviceServiceUpdate                                      */
/**
    \brief      fill respective device table content with received info

    \brief      check that given id is in allowed range

    \param      pst_msg - pointer to response message
                ATRspPlpdInfoType

    \return     SuccessFailType
                SUCCESS if member was initialised and id was in range
                FAIL if id was out of range
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ERM_sFBTDeviceServiceUpdate(ATResBTServiceNotType *p_info)
{
    SuccessFailType sF = FAIL;
    ATBooleanEnum   *p_service   = NULL;
    ATSupportedEnum *p_supported = NULL;
    /* check given id is in valid range */
    if (AT_MAX_DEVICE_ID >= p_info->device_id)
    {   /* check if given device is valid one */
        if (AT_DEVICE_INVALID != _ERM_stBTDeviceTable[p_info->device_id].device_connected)
        {   /* ok - now check which service was effected */
            switch (p_info->service)
            {
                case AT_SERVICE_PHONE:
                {   /* check result and handle according to table settings */
                    p_service   = &(_ERM_stBTDeviceTable[p_info->device_id].phone_active);
                    p_supported = &(_ERM_stBTDeviceTable[p_info->device_id].phone_supported);
                    break;
                }
#ifdef ERM_FULL_DEVICE_TABLE
                case AT_SERVICE_A2DP_SINK:
                {   /* check result and handle according to table settings */
                    p_service   = &(_ERM_stBTDeviceTable[p_info->device_id].a2dp_sink_active);
                    p_supported = &(_ERM_stBTDeviceTable[p_info->device_id].a2dp_sink_supported);
                    break;
                }
                case AT_SERVICE_AVRCP:
                {   /* check result and handle according to table settings */
                    p_service   = &(_ERM_stBTDeviceTable[p_info->device_id].avrcp_active);
                    p_supported = &(_ERM_stBTDeviceTable[p_info->device_id].avrcp_supported);
                    break;
                }
                case AT_SERVICE_AUDIO_GATEWAY:
                {   /* check result and handle according to table settings */
                    p_service   = &(_ERM_stBTDeviceTable[p_info->device_id].audio_gw_active);
                    p_supported = &(_ERM_stBTDeviceTable[p_info->device_id].audio_gw_supported);
                    break;
                }
#else
                case AT_SERVICE_AVRCP:
                case AT_SERVICE_A2DP_SINK:
                case AT_SERVICE_AUDIO_GATEWAY:
#endif
                case AT_SERVICE_DUN:
                case AT_SERVICE_A2DP_SOURCE:
                case AT_SERVICE_SPP:
                default: /* this service is not yet taken into account */
                {

                    DEBUG_VALUE2(ERM_SWI_TRC, MOD_ERM,"ERM SN %d not supported (%d)",p_info->service,p_info->device_id);
                    break;
                }
            }
            /* check if service pointer is set */
            if (NULL != p_service)
            {
                switch (p_info->result)
                {
                    case AT_SERVRES_START_SUCCEED:
                    {
                        *p_service      = AT_TRUE;
                        /* we can set the supported state if the service is started */
                        *p_supported    = AT_SUPPORTED;
                        sF = SUCCESS;
                        break;
                    }
                    case AT_SERVRES_START_FAILED:
                    {
                        *p_service = AT_FALSE;
                        sF = SUCCESS;
                        break;
                    }
                    case AT_SERVRES_STOP_SUCCEED:
                    {
                        *p_service = AT_FALSE;
                        sF = SUCCESS;
                        break;
                    }
                    case AT_SERVRES_STOP_FAILED:
                    {   /* do not change service */
                        sF = SUCCESS;
                        break;
                    }
                    default:
                    {

                        DEBUG_VALUE2(ERM_SWI_TRC, MOD_ERM,"ERM ServiceRes ooR %d (%d)",p_info->result,p_info->device_id);
                        break;
                    }
                }
            }

            // RS_Redesign: not needed due a proper pbsc handling
            //switch(p_info->service)
            //{
            //    case AT_SERVICE_A2DP_SINK:
            //    case AT_SERVICE_AVRCP:
            //        /* notify mdi about a service change */
            //        _ERM_vNotifyApplication(ERM_NOTIFY_MDI_SERVICE_CHANGED);
            //        break;
            //}

        }
        else
        {   /* received service notification for invalid device */

            DEBUG_VALUE2(ERM_SWI_TRC, MOD_ERM,"ERM Service Update ooR %d (%d)",p_info->device_id,__LINE__);
        }
    }
    return sF;
}




/*----------------------------------------------------------------------------*/
/* Function    : _ERM_vNotifyApplication                                       */
/**
    \brief      function to notify application that something has changed

    \brief      triggers application of the same task context by function call

    \param      new_notification - identifies the changed event
                ERMNotificationType
                values of enum state type

    \return     none

   */
/*----------------------------------------------------------------------------*/
void _ERM_vNotifyApplication(const ERMNotificationType *pNotification)
{
    DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM,"ERM new Notify %d", pNotification->common.id);

    APP_DEVICE_vOnErmNotification(pNotification);
    APP_MDI_ERM_vHandleErmNotification(pNotification);
    APP_MDI_BAP_vHandleErmNotification(pNotification);
    APP_PHONE_vOnErmNotification(pNotification);
    APP_PHONE_BAP_vOnErmNotification(pNotification);
    APP_PHONEBOOK_vOnErmNotification(pNotification);
    APP_PHONEBOOK_BAP_vOnErmNotification(pNotification);
    APP_SD_vOnErmNotification(pNotification);
    APP_SMS_vOnErmNotification(pNotification);
    CONTEXT_MAN_vOnContextManNotification(pNotification);
//    VCR_vOnErmNotification(pNotification);
#ifdef UHV_ACTIVATE_FEATURE_LED
    LED_vHandleErmNotification(pNotification);
#endif
    ERM_vOnErmNotification(pNotification);
}


/*----------------------------------------------------------------------------*/
/* Function    : _ERM_vCheckDeviceTable                                       */
/**
    \brief      check the device table for an update and modify
                variables to indicate active state accordingly



    \param      none

    \return     none
   */
/*----------------------------------------------------------------------------*/
void  _ERM_vCheckDeviceTable(void)
{
    AT_DeviceId deviceId = AT_INVALID_DEVICE_ID;

    /* check the active phone device state */
    deviceId = ERM_u8GetActivePhone();
    if(_ERM_gContext.u8ActiveBTId != deviceId)
    {
        /* if BT is still visible, we need to change this now */
        if (deviceId != AT_INVALID_DEVICE_ID)
        {
            ERM_vBTVisibilityOff();
        }

        _ERM_gContext.u8ActiveBTId = deviceId;
        //RS_Redesign: changed the service state notification handling
        //_ERM_vNotifyApplication(ERM_NOTIFY_PHONE_ACTIVE_STATE_CHANGED);
    }

    deviceId = ERM_u8GetActivePlayer();
    if(_ERM_gContext.u8ActiveMDIId != deviceId)
    {
        /* if BT is still visible, we need to change this now */
        if (deviceId != AT_INVALID_DEVICE_ID)
        {
            ERM_vBTVisibilityOff();
        }

        _ERM_gContext.u8ActiveMDIId = deviceId;
        //RS_Redesign: changed the service state notification handling
        //_ERM_vNotifyApplication(ERM_NOTIFY_PLAYER_ACTIVE_STATE_CHANGED);
    }

    deviceId = ERM_u8GetActiveHeadSet();
    if(_ERM_gContext.u8ActiveHeadsetId != deviceId)
    {
        _ERM_gContext.u8ActiveHeadsetId = deviceId;

        //RS_Redesign: changed the service state notification handling
        //_ERM_vNotifyApplication(ERM_NOTIFY_HEADSET_ACTIVE_STATE_CHANGED);
        /* if BT is still visible, we need to change this now */
        if (deviceId != AT_INVALID_DEVICE_ID)
        {
            ERM_vBTVisibilityOff();
        }
    }

}

/*----------------------------------------------------------------------------*/
/* Function    : _ERM_vCheckDeviceTable                                       */
/**
    \brief      check the device table for an update and modify
                variables to indicate active state accordingly



    \param      none

    \return     none
   */
/*----------------------------------------------------------------------------*/
//void  _ERM_vCheckDeviceTable(void)
//{
////    ATBooleanEnum          search           = AT_FALSE;
////    ATBooleanEnum          previous_active  = AT_FALSE;
//    ERMBTDeviceTableType * p_table              = NULL;
//    Uint32Type             i                    = 0;
//    AT_DeviceId            lastMDIDevice        = AT_INVALID_DEVICE_ID;
//    AT_DeviceId            lastHeadsetDevice    = AT_INVALID_DEVICE_ID;
//    AT_DeviceId            lastActBTDevice      = AT_INVALID_DEVICE_ID;
//
//
////    /* if no active device is indicated currently, run through table and check
////     * for active device */
////    if ( (FALSE == _ERM_gContext.u8ActiveBTDevice) ||
////         (AT_INVALID_DEVICE_ID == _ERM_gContext.u8ActiveBTId) )
////    {
////        search = AT_TRUE;
////    }
////    else
////    {   /* we have active device, check if it still fulfill active requirement */
////        /* means it must be connected and active and phone service running */
////        previous_active = AT_TRUE;
////        p_table = &(_ERM_stBTDeviceTable[_ERM_gContext.u8ActiveBTId]);
////        /* NOTE - check for device_active masked out here as sometimes
////         * device not indicated as active from Parrot - to be clarified
////         * TODO  */
////        if ( (AT_DEVICE_CONNECTED != p_table->device_connected) ||
////            /* (AT_TRUE             != p_table->device_active)    || */
////             (AT_TRUE             != p_table->phone_active)         )
////        {   /* not "active anymore - need to search */
////            search = AT_TRUE;

////            /* set back global variables and table entry active */
////            p_table->device_ERM_active = AT_FALSE;
////            _ERM_gContext.u8ActiveBTDevice      = FALSE;
////            _ERM_gContext.u8ActiveBTId          = AT_INVALID_DEVICE_ID;
////        }
////        /* else this id is still valid - nothing more to do here */
////    }
//
//    if(_ERM_gContext.u8ActiveBTId <= AT_MAX_DEVICE_ID)
//    {
//        /* remember the current active BT device */
//        lastActBTDevice = _ERM_gContext.u8ActiveBTId;
//        /* assume that we don't have an active BT device */
//        _ERM_stBTDeviceTable[_ERM_gContext.u8ActiveBTId].device_ERM_active =
//            AT_FALSE;
//        _ERM_gContext.u8ActiveBTDevice      = FALSE;
//        _ERM_gContext.u8ActiveBTId          = AT_INVALID_DEVICE_ID;
//    }
//
//    if( _ERM_gContext.u8ActiveMDIId <= AT_MAX_DEVICE_ID )
//    {
//        /* remember the current MDI device */
//        lastMDIDevice       = _ERM_gContext.u8ActiveMDIId;
//        /* assume that we don't have an active MDI device */
//        _ERM_gContext.u8ActiveMDIId  = AT_INVALID_DEVICE_ID;
//    }
//
//    if( _ERM_gContext.u8ActiveHeadsetId <= AT_MAX_DEVICE_ID )
//    {
//        /* remember the current Headset device */
//        lastHeadsetDevice       = _ERM_gContext.u8ActiveHeadsetId;
//        /* assume that we don't have an active Headset device */
//        _ERM_gContext.u8ActiveHeadsetId  = AT_INVALID_DEVICE_ID;
//    }
//
//
////    if (AT_TRUE == search)
////    {   /* we have to search for all devices in table if one fulfill active state */
//        for (i = 0; i <= AT_MAX_DEVICE_ID; i++)
//        {
//            p_table = &(_ERM_stBTDeviceTable[i]);
//
//            if ( AT_DEVICE_CONNECTED == p_table->device_connected )
//            {
//                if( AT_TRUE == p_table->phone_active)
//                {    /* found new ACTIVE device - update */
//                    p_table->device_ERM_active = AT_TRUE;
//                    _ERM_gContext.u8ActiveBTDevice  = TRUE;
//                    _ERM_gContext.u8ActiveBTId      = i;
//                    _ERM_gContext.u8ActiveMDIProfId = ERM_u8GetUserProfileId(i);
//
//                    if ( (lastActBTDevice != i) &&
//                         (_ERM_gContext.u8ActiveMDIProfId != AT_INVALID_DEVICE_ID))
//                    {
//                        /* if only active device changed, trigger "pseudo"
//                         * state change towards application */
//                        _ERM_vChangeState(ERM_STATE_CK5050_ACTIVE);
//                        _ERM_vChangeState(ERM_STATE_CK5050_BT_ACTIVE);
//                        /* TODO: synchronize our active user list */
//                        // TODO: store to EPROM
//                        //_ERM_vActiveProfileListAdd( ERM_ACT_USER_PROFILE,
//                        //                            _ERM_gContext.u8ActiveMDIProfId);
//                    }

//                }
//
//                /* check for active MDI device */
//                if( AT_TRUE == p_table->a2dp_sink_active )
//                {
//                    /* found ACTIVE MDI device */
//                    _ERM_gContext.u8ActiveMDIId = i;
//                    _ERM_gContext.u8ActiveMDIProfId = ERM_u8GetPlayerHeadsetProfileId(i);
//
//                    /* check if we found the same MDI device */
//                    if( _ERM_gContext.u8ActiveMDIProfId != AT_INVALID_DEVICE_ID )
//                    {
//                        /* new ACTIVE MDI device */
//                        /* synchronize our active user list */
//                        // TODO: store to EPROM
//                        //_ERM_vActiveProfileListAdd( ERM_ACT_PLAYER_PROFILE,
//                        //                            _ERM_gContext.u8ActiveMDIProfId);
//                    }
//                }
//                /* check for active headset device */
//                if( AT_TRUE == p_table->audio_gw_active )
//                {
//                    /* found ACTIVE Headset device */
//                    _ERM_gContext.u8ActiveHeadsetId = i;
//                    _ERM_gContext.u8ActiveHSProfId = ERM_u8GetPlayerHeadsetProfileId(i);
//
//                    /* check if we found the same Headset device */
//                    if( _ERM_gContext.u8ActiveHSProfId != AT_INVALID_DEVICE_ID )
//                    {
//                        /* new ACTIVE Headset device */
//                        /* synchronize our active user list */
//                        // TODO: store to EPROM
//                        //_ERM_vActiveProfileListAdd( ERM_ACT_HEADSET_PROFILE,
//                        //                            _ERM_gContext.u8ActiveHSProfId);
//                    }
//                }
//            }
//            else
//            {

//            }
//        }
//
//    /* check if the active MDI device has changed */
//    /* the check at this point ensures to recognize a removed MDI device too*/
//    if( _ERM_gContext.u8ActiveMDIId != lastMDIDevice )
//    {
//        /* notify the application that the active MDI device has
//         * changed*/
//        _ERM_vNotifyApplication(ERM_NOTIFY_ACTIVE_MDI_DEVICE);
//    }
//    /* check if the active headset device has changed */
//    /* the check at this point ensures to recognize a removed Headset device too*/
//    if( _ERM_gContext.u8ActiveHeadsetId != lastHeadsetDevice )
//    {
//        /* notify the application that the active Headset device has
//         * changed*/
//        _ERM_vNotifyApplication(ERM_NOTIFY_ACTIVE_HEADSET_DEVICE);
//    }
//
////    }

//}

///*----------------------------------------------------------------------------*/
///* Function    : _ERM_vUpdateActiveUserList                                   */
///**
//    \brief
//
//    \param      none
//
//    \return     none
//   */
///*----------------------------------------------------------------------------*/
//SuccessFailType _ERM_sfUpdateActiveUserList(AT_DeviceId activeUser)
//{
//    Sint32Type   i     = 0;
//    AT_DeviceId  tmpId = AT_INVALID_DEVICE_ID;
//
//    /* try to find the userId in the persistent ActiveUserList */
//    for( i=0; i<AT_MAX_DEVICE_ID; i++)
//    {
//        if(_ERM_gContext.pu8ActiveUserList[i] == activeUser)
//        {
//            break;
//        }
//    }
//
//    if( i==0 )
//    {
//        /* the activeUser is already on first position of the list, so nothing
//         * more to do here */
//        /* we return an invalid user id since there is no id dropped out of the
//         * list */
//        return SUCCESS;
//    }
//
//    if( i == AT_MAX_DEVICE_ID )
//    {
//        /* the activeUser is not contained in our list, so we will drop the
//         * oldest userId */
//        i = AT_MAX_DEVICE_ID - 1;
//    }
//
//    /* swap the elements beginning with activeUser found in our list or if we
//     * didn't find the user in our list, we swap beginning with the oldest
//     * (last) element */
//    for( i=i; i>0; i--)
//    {
//        tmpId                       = _ERM_gContext.pu8ActiveUserList[i];
//        _ERM_gContext.pu8ActiveUserList[i]   = _ERM_gContext.pu8ActiveUserList[i-1];
//        _ERM_gContext.pu8ActiveUserList[i-1] = tmpId;
//    }
//
//    if(_ERM_gContext.pu8ActiveUserList[0] != activeUser)
//    {
//        /* now the first element is an invalid id or the oldest userId */
//        if( _ERM_gContext.pu8ActiveUserList[0] <= AT_MAX_DEVICE_ID )
//        {
//            /* if we have a userId mark it to be released */
//            tmpId = _ERM_gContext.pu8ActiveUserList[0];
//            _ERM_stBTDeviceTable[tmpId].device_state = ERM_DEVICE_STATE_RELEASED;
//        }
//
//        /* overwrite the id with the current user id */
//        _ERM_gContext.pu8ActiveUserList[0] = activeUser;
//    }
//
//    /* store the new active user list persistently */
//    if( _ERM_sfWritePersistentUserList() != SUCCESS )
//    {

//    }
//
//    /* return the id that was dropped out of our list or
//     * AT_INVALID_DEVICE_ID if no element was dropped out */
//    return SUCCESS;
//}



///*----------------------------------------------------------------------------*/
///* Function    : _ERM_sfCheckUserListConsistency                              */
///**
//    \brief
//
//    \param      none
//
//    \return     none
//   */
///*----------------------------------------------------------------------------*/
//BooleanType _ERM_sfCheckUserListConsistency()
//{
//    BooleanType result = BT_TRUE;
//    Uint32Type  i = 0;
//    Uint32Type  j = 0;
//
//    for( i=0; i<=AT_MAX_DEVICE_ID; i++)
//    {
//        if( _ERM_stBTDeviceTable[i].device_state == ERM_DEVICE_STATE_USED )
//        {
//            BooleanType isDeviceConsistent = BT_FALSE;
//
//            for( j=0; j<AT_MAX_DEVICE_ID; j++)
//            {
//                if( _ERM_gContext.pu8ActiveUserList[j] == i )
//                {
//                    isDeviceConsistent = BT_TRUE;
//                    break;
//                }
//            }
//
//            if(isDeviceConsistent == BT_FALSE)
//            {
//                /* the id is not contained in our active id list */
//                _ERM_stBTDeviceTable[i].device_state = ERM_DEVICE_STATE_RELEASED;
//                result = BT_FALSE;
//            }
//        }
//    }
//
//    return result;
//}

/*----------------------------------------------------------------------------*/
/* Function    : _ERM_sfConsistencyCheck                                      */
/**
    \brief

    \param      none

    \return     none
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ERM_sfConsistencyCheck(void)
{
    SuccessFailType result = SUCCESS;


    switch(_ERM_gContext.eConsistCheckState)
    {
        case ERM_CC_ACTIVE:
            /* ok, we go on with consistency check */
            break;
        case ERM_CC_WAIT_FOR_RESPONSE:
            /* we need to wait for some event, to go on */
            return FAIL;
        default:

            DEBUG_VALUE1(ERM_SWI_ERR, MOD_ERM,"ConsistCheck: invalid state %d", _ERM_gContext.eConsistCheckState);
            return FAIL;
    }

    /* 1. Check the consistency of the BT device table */
    if(_ERM_sfConsistencyCheck_BTDeviceTable() != SUCCESS)
    {

        DEBUG_TEXT(ERM_SWI_ERR, MOD_ERM,"BT device table is incons");
        return FAIL;
    }

    /* 2. Check the consistency of the Phy device table */
    if(_ERM_sfConsistencyCheck_PhyDeviceTable() != SUCCESS)
    {

        DEBUG_TEXT(ERM_SWI_ERR, MOD_ERM,"Phy device table is incons");
        return FAIL;
    }

    /* 3. Check the consistency of the UserTable */
    if(_ERM_sfConsistencyCheck_UserTable() != SUCCESS)
    {

        DEBUG_TEXT(ERM_SWI_ERR, MOD_ERM,"UserTable is incons");
        return FAIL;
    }

    /* 4. Check the consistency of the PlayerHeadsetTable */
    if(_ERM_sfConsistencyCheck_PlayerHeadsetTable() != SUCCESS)
    {

        DEBUG_TEXT(ERM_SWI_ERR, MOD_ERM,"PlHsTable table is incons");
        return FAIL;
    }

    /* 5. Check the consistency of the ActiveProfileUser Table */
    if(_ERM_sfConsistencyCheck_ActiveProfile_User() != SUCCESS)
    {

        DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM,"BT device table is incons");
        return FAIL;
    }

    /* 6. Check the consistency of the ActiveProfilePlayer Table */
    if(_ERM_sfConsistencyCheck_ActiveProfile_Player() != SUCCESS)
    {

        DEBUG_TEXT(ERM_SWI_ERR, MOD_ERM,"ActProf_Pl is incons");
        return FAIL;
    }

    /* 7. Check the consistency of the ActiveProfileHeadset Table */
    if(_ERM_sfConsistencyCheck_ActiveProfile_Headset() != SUCCESS)
    {

        DEBUG_TEXT(ERM_SWI_ERR, MOD_ERM,"ActProf_Hs is incons");
        return FAIL;
    }



    if( result == SUCCESS)
    {
        /* now all tables should be consistent and we are able to set persistent values from
         * PhyDeviceTable to BTDeviceTable */
        AT_DeviceId i = 0;
        for( i=0; i<=AT_MAX_DEVICE_ID; i++)
        {
            if( _ERM_stBTDeviceTable[i].device_state == ERM_DEVICE_STATE_USED )
            {
                /* copy the device name from PhyDevTable to BTDevTable */
                _ERM_stBTDeviceTable[i].device_name_len =  _ERM_stPhyDevTable[i].name_len;
                (void)memcpy( _ERM_stBTDeviceTable[i].device_name,
                        _ERM_stPhyDevTable[i].name,
                        _ERM_stBTDeviceTable[i].device_name_len);
            }
        }

        _ERM_vSetStartupDone();
        _ERM_gContext.bStartupDone = BT_TRUE;
        _ERM_gContext.eConsistCheckState = ERM_CC_INVALID;


        DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM,"ConsistencyCheck finished!");
    }

    return result;
}

/*----------------------------------------------------------------------------*/
/* Function    : _ERM_OnConsistencyCheckCb                                    */
/**
    \brief

    \param      none

    \return     none
   */
/*----------------------------------------------------------------------------*/
void _ERM_OnConsistencyCheckCb(APP_COMMON_enResponseCode rsp)
{

    DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM,"OnConsistencyCheckCb rsp %d",rsp);
    /* go on with check */
    _ERM_gContext.eConsistCheckState = ERM_CC_ACTIVE;
    (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_ERM_PENDING, MSEC(0), MSEC(0));
}


/*----------------------------------------------------------------------------*/
/* Function    : _ERM_sfConsistencyCheck_DeviceTable                          */
/**
    \brief

    \param      none

    \return     none
   */
/*----------------------------------------------------------------------------*/
static SuccessFailType _ERM_sfConsistencyCheck_BTDeviceTable(void)
{
    AT_DeviceId          i = 0;

    for( i=0; i<AT_MAX_DEVICE_ID+1; i++)
    {
        BooleanType isDevConsistent = BT_TRUE;

        if( _ERM_stBTDeviceTable[i].device_state != ERM_DEVICE_STATE_USED )
        {
            continue;
        }

        /* 1.1 check the consistency of the BT device table with PhyDev table */
        {
            if(_ERM_stPhyDevTable[i].deviceID == i)
            {
                if(memcmp( _ERM_stPhyDevTable[i].bt_addr,
                           _ERM_stBTDeviceTable[i].bt_addr,
                           sizeof(AT_BT_Address)) != 0  )
                {

                    DEBUG_VALUE1(ERM_SWI_ERR, MOD_ERM,"CCBTDeviceTable: btAddr doesn't match %d",i);
                    isDevConsistent = BT_FALSE;
                }

            }
            else
            {
                /* the device is not available in the PhyDev table */

                DEBUG_VALUE1(ERM_SWI_ERR, MOD_ERM,"CCBTDeviceTable: not found in PhyDevTable %d",i);
                isDevConsistent = BT_FALSE;
            }
        }
        /* 1.2 find a profile for the BT device */
        if(isDevConsistent == BT_TRUE)
        {
            AT_DeviceId userProfile          = AT_INVALID_DEVICE_ID;
            AT_DeviceId playerHeadsetProfile = AT_INVALID_DEVICE_ID;
            AT_DeviceId j = 0;

            for( j=0; j<ERM_MAX_NUM_USER_PROFILES; j++)
            {
                if( _ERM_stUserTable[j].deviceIndex == i )
                {
                    userProfile = j;
                    break;
                }
            }

            for( j=0; j<ERM_MAX_NUM_PLAYER_HS_TABLE; j++)
            {
                if( _ERM_stPlayerHeadsetTable[j].deviceIndex == i )
                {
                    playerHeadsetProfile = j;
                    break;
                }
            }

            if( (userProfile          == AT_INVALID_DEVICE_ID) &&
                (playerHeadsetProfile == AT_INVALID_DEVICE_ID) )
            {
                /* no profile found for the device */

                DEBUG_VALUE1(ERM_SWI_ERR, MOD_ERM,"CCBTDeviceTable: no profile found %d",i);
                isDevConsistent = BT_FALSE;
            }
        }

        if(isDevConsistent != BT_TRUE)
        {
            APP_COMMON_enResponseCode res = APP_COMMON_RSP_OK;
            _ERM_stBTDeviceTable[i].device_state = ERM_DEVICE_STATE_RELEASED;
            _ERM_gContext.u8DeleteDeviceId = i;

            res = _ERM_SendMessage( ERM_REQ_DELETE_DEVICE, NULL );

            if( res == APP_COMMON_RSP_OK )
            {
                /* the state is changed after receiving of PPDE, that
                 * indicates that the device was deleted */
                _ERM_gContext.eConsistCheckState = ERM_CC_WAIT_FOR_RESPONSE;
            }
            else
            {

                DEBUG_VALUE1(ERM_SWI_ERR, MOD_ERM,"CCBTDeviceTable: SendMessage error %d",res);
                /* try again in some mseconds */
                (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_ERM_PENDING, MSEC(10), MSEC(0));
            }
            return FAIL;
        }
    }


    return SUCCESS;
}


/*----------------------------------------------------------------------------*/
/* Function    : _ERM_sfConsistencyCheck_PhyDeviceTable                       */
/**
    \brief

    \param      none

    \return     none
   */
/*----------------------------------------------------------------------------*/
static SuccessFailType _ERM_sfConsistencyCheck_PhyDeviceTable(void)
{
    AT_DeviceId          i = 0;

    for( i=0; i<ERM_MAX_NUM_PHYSICAL_DEVICES; i++)
    {
        BooleanType isDevConsistent = BT_TRUE;
        AT_DeviceId deviceId = _ERM_stPhyDevTable[i].deviceID;
        if( deviceId == AT_INVALID_DEVICE_ID)
        {
            continue;
        }

        /* 2.1 first check that the device is equal to the index */
        if( deviceId != i)
        {

            DEBUG_VALUE1(ERM_SWI_ERR, MOD_ERM,"CCPhyTable: device id invalid %d",deviceId);
            isDevConsistent = BT_FALSE;
        }

        if(isDevConsistent == BT_TRUE)
        {
            /* 2.1 to be consistent we need a device in BT device table */
            if(_ERM_stBTDeviceTable[i].device_state != ERM_DEVICE_STATE_USED)
            {
                /* the device is not available in the BT device table */

                DEBUG_VALUE1(ERM_SWI_ERR, MOD_ERM,"CCPhyTable: not found in BT device table %d",i);
                isDevConsistent = BT_FALSE;
            }
        }

        if(isDevConsistent == BT_TRUE)
        {
            /* 2.2 to be consistent we need a name length that is in range */
            if(_ERM_stPhyDevTable[i].name_len > sizeof(AT_DeviceNameString) )
            {
                /* the length parameter is invalid */

                DEBUG_VALUE2(ERM_SWI_ERR, MOD_ERM,"CCPhyTable: dev name len: %d invalid %d",
                                _ERM_stPhyDevTable[i].name_len,
                                i);

                isDevConsistent = BT_FALSE;
            }
        }

        if(isDevConsistent != BT_TRUE)
        {
            APP_COMMON_enResponseCode rsp = _ERM_rcPrepare_DeletePhysicalDevice(i);
            if( APP_COMMON_RSP_OK == rsp )
            {
                /* set pending msg and callback and start */
                rsp = _ERM_rcPrepare_EepromUpdate(_ERM_OnConsistencyCheckCb, ERM_REQ_CONSISTENCY_CHECK);
            }

            /* check error condition */
            switch( rsp )
            {
                case APP_COMMON_RSP_OK:
                    /* wait for result */
                    _ERM_gContext.eConsistCheckState = ERM_CC_WAIT_FOR_RESPONSE;
                    break;
                case APP_COMMON_RSP_ERROR_BUSY:

                    DEBUG_TEXT(ERM_SWI_ERR, MOD_ERM,"CCPhyTable: DelPersPhyTable: Eeprom write busy");
                    /* try again in some mseconds */
                    (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_ERM_PENDING, MSEC(10), MSEC(0));
                    break;
                default:

                    DEBUG_VALUE1(ERM_SWI_ERR, MOD_ERM,"CCPhyTable: DelPersPhyTable error %d", rsp);
                    break;
            }
            return FAIL;
        }
    }


    return SUCCESS;
}


/*----------------------------------------------------------------------------*/
/* Function    : _ERM_sfConsistencyCheck_UserTable                            */
/**
    \brief

    \param      none

    \return     none
   */
/*----------------------------------------------------------------------------*/
static SuccessFailType _ERM_sfConsistencyCheck_UserTable(void)
{
    AT_DeviceId i = 0;

    for( i=0; i<ERM_MAX_NUM_USER_PROFILES; i++)
    {
        BooleanType isDevConsistent = BT_TRUE;
        AT_DeviceId idx = _ERM_stUserTable[i].deviceIndex;

        if( idx == AT_INVALID_DEVICE_ID)
        {
            continue;
        }

        /* 3.1 first check that the device id not exceeds the MaxDeviceId */
        if( idx > AT_MAX_DEVICE_ID )
        {

            DEBUG_VALUE1(ERM_SWI_ERR, MOD_ERM,"CCUserTable: dev id invalid %d",idx);
            isDevConsistent = BT_FALSE;
        }

        if(isDevConsistent == BT_TRUE)
        {
            /* 3.2 to be consistent we need the corresponding device in BT device table */
            if(_ERM_stBTDeviceTable[idx].device_state != ERM_DEVICE_STATE_USED)
            {
                /* the device is not available in the BT device table */

                DEBUG_VALUE1(ERM_SWI_ERR, MOD_ERM,"CCUserTable: not found in BT dev table %d",idx);
                isDevConsistent = BT_FALSE;
            }
        }

        if(isDevConsistent == BT_TRUE)
        {
            Uint8Type j=0;
            /* 3.3 to be consistent we need the current user profile to be
             * in the active user profile list */
            for( j=0; j<ERM_MAX_NUM_USER_PROFILES; j++)
            {
                if( _ERM_stActiveProfileList.UserProfileList[j] == i )
                {
                    break;
                }
            }

            if(j == ERM_MAX_NUM_USER_PROFILES)
            {
                /* this profile is not contained in the active profile list */

                DEBUG_VALUE1(ERM_SWI_ERR, MOD_ERM,"CCUserTable: not found in ActProfList %d",i);
                isDevConsistent = BT_FALSE;
            }
        }


        if(isDevConsistent != BT_TRUE)
        {
            APP_COMMON_enResponseCode rsp = _ERM_rcPrepare_DeleteUserProfile(i);
            if( APP_COMMON_RSP_OK == rsp )
            {
                /* set pending msg and callback and start */
                rsp = _ERM_rcPrepare_EepromUpdate(_ERM_OnConsistencyCheckCb, ERM_REQ_CONSISTENCY_CHECK);
            }

            /* check error condition */
            switch( rsp )
            {
                case APP_COMMON_RSP_OK:
                    /* wait for result */
                    _ERM_gContext.eConsistCheckState = ERM_CC_WAIT_FOR_RESPONSE;
                    break;
                case APP_COMMON_RSP_ERROR_BUSY:
                    DEBUG_TEXT(ERM_SWI_ERR, MOD_ERM,"CCPhyTable: DelPersUserTable: Eeprom write busy");
                    /* try again in some mseconds */
                    (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_ERM_PENDING, MSEC(10), MSEC(0));
                    break;
                default:

                    DEBUG_VALUE1(ERM_SWI_ERR, MOD_ERM,"CCPhyTable: DelPersUserTable error %d", rsp);
                    break;
            }
            return FAIL;
        }
    }

    return SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* Function    : _ERM_sfConsistencyCheck_PlayerHeadsetTable                   */
/**
    \brief

    \param      none

    \return     none
   */
/*----------------------------------------------------------------------------*/
static SuccessFailType _ERM_sfConsistencyCheck_PlayerHeadsetTable(void)
{
    AT_DeviceId          i = 0;

    for( i=0; i<ERM_MAX_NUM_PLAYER_HS_TABLE; i++)
    {
        BooleanType isDevConsistent = BT_TRUE;
        AT_DeviceId idx = _ERM_stPlayerHeadsetTable[i].deviceIndex;

        if( idx == AT_INVALID_DEVICE_ID)
        {
            continue;
        }

        /* 4.1 first check that the device id not exceeds the MaxDeviceId */
        if( idx > AT_MAX_DEVICE_ID )
        {

            DEBUG_VALUE1(ERM_SWI_ERR, MOD_ERM,"CCPlayHsTable: dev id invalid %d",idx);
            isDevConsistent = BT_FALSE;
        }

        if(isDevConsistent == BT_TRUE)
        {
            /* 4.2 to be consistent we need the corresponding device in BT device table */
            if(_ERM_stBTDeviceTable[idx].device_state != ERM_DEVICE_STATE_USED)
            {
                /* the device is not available in the BT device table */

                DEBUG_VALUE1(ERM_SWI_ERR, MOD_ERM,"CCPlayHsTable: not found in BT devtable %d",idx);
                isDevConsistent = BT_FALSE;
            }
        }



        if(isDevConsistent == BT_TRUE)
        {
            /* 4.3 to be consistent we need the current player/headset profile to be
             * in the active profile list */

            if(_ERM_stPlayerHeadsetTable[i].deviceType == APP_DEVICE_enTYPE_PLAYER)
            {
                Uint8Type j = 0;
                for( j=0; j<ERM_MAX_NUM_PLAYER_HS_TABLE; j++)
                {
                    if( _ERM_stActiveProfileList.PlayerProfileList[j] == i )
                    {
                        break;
                    }
                }

                if(j == ERM_MAX_NUM_PLAYER_HS_TABLE)
                {
                    /* this profile is not contained in the active profile list */

                    DEBUG_VALUE1(ERM_SWI_ERR, MOD_ERM,"CCPlayHsTable: not found in BT devtable %d",idx);
                    isDevConsistent = BT_FALSE;
                }
            }
            else
            {
                Uint8Type j = 0;
                for( j=0; j<ERM_MAX_NUM_HEADSET_PROFILE; j++)
                {
                    if( _ERM_stActiveProfileList.HeadsetProfileList[j] == i )
                    {
                        break;
                    }
                }

                if(j == ERM_MAX_NUM_HEADSET_PROFILE)
                {
                    /* this profile is not contained in the active profile list */

                    DEBUG_VALUE1(ERM_SWI_ERR, MOD_ERM,"CCPlayHsTable: not found in ActProfList(Headset) %d",i);
                    isDevConsistent = BT_FALSE;
                }
            }
        }

        if(isDevConsistent != BT_TRUE)
        {
            APP_COMMON_enResponseCode rsp = _ERM_rcPrepare_DeletePlayerHeadsetProfile(i);
            if( APP_COMMON_RSP_OK == rsp )
            {
                /* set pending msg and callback and start */
                rsp = _ERM_rcPrepare_EepromUpdate(_ERM_OnConsistencyCheckCb, ERM_REQ_CONSISTENCY_CHECK);
            }

            /* check error condition */
            switch( rsp )
            {
                case APP_COMMON_RSP_OK:
                    /* wait for result */
                    _ERM_gContext.eConsistCheckState = ERM_CC_WAIT_FOR_RESPONSE;
                    break;
                case APP_COMMON_RSP_ERROR_BUSY:

                    DEBUG_TEXT(ERM_SWI_ERR, MOD_ERM,"CCPhyTable: DelPersPlHSTable: Eeprom write busy");
                    /* try again in some mseconds */
                    (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_ERM_PENDING, MSEC(10), MSEC(0));
                    break;
                default:

                    DEBUG_VALUE1(ERM_SWI_ERR, MOD_ERM,"CCPhyTable: DelPersPlHSTable error %d", rsp);
                    break;
            }
            return FAIL;
        }
    }

    return SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* Function    : _ERM_sfConsistencyCheck_ActiveProfile_User                   */
/**
    \brief

    \param      none

    \return     none
   */
/*----------------------------------------------------------------------------*/
static SuccessFailType _ERM_sfConsistencyCheck_ActiveProfile_User(void)
{
    AT_DeviceId          i = 0;

    for( i=0; i<ERM_MAX_NUM_USER_PROFILES; i++)
    {
        BooleanType isDevConsistent = BT_TRUE;
        Uint8Type profileIdx = _ERM_stActiveProfileList.UserProfileList[i];

        if( profileIdx == AT_INVALID_DEVICE_ID)
        {
            continue;
        }

        /* 5.1 first check that the device id not exceeds the number of user profiles */
        if( profileIdx >= ERM_MAX_NUM_USER_PROFILES )
        {

            DEBUG_VALUE1(ERM_SWI_ERR, MOD_ERM,"CCActProfileUser: dev id invalid %d",
                          profileIdx);
            isDevConsistent = BT_FALSE;
        }
        else
        {
            /* 5.2 to be consistent we need the corresponding profile id in UserTable */
            if(_ERM_stUserTable[profileIdx].deviceIndex == AT_INVALID_DEVICE_ID)
            {
                /* the profile is not available in the UserTable */

                DEBUG_VALUE1(ERM_SWI_ERR, MOD_ERM,"CCActProfileUser: not found in UserTable %d",
                              profileIdx);
                isDevConsistent = BT_FALSE;
            }
        }

        if(isDevConsistent != BT_TRUE)
        {
            APP_COMMON_enResponseCode rsp = _ERM_rcPrepare_DeleteActiveProfile( profileIdx, APP_DEVICE_enTYPE_PHONE);
            if( APP_COMMON_RSP_OK == rsp )
            {
                /* set pending msg and callback and start */
                rsp = _ERM_rcPrepare_EepromUpdate(_ERM_OnConsistencyCheckCb, ERM_REQ_CONSISTENCY_CHECK);
            }

            /* check error condition */
            switch( rsp )
            {
                case APP_COMMON_RSP_OK:
                    /* wait for result */
                    _ERM_gContext.eConsistCheckState = ERM_CC_WAIT_FOR_RESPONSE;
                    break;
                case APP_COMMON_RSP_ERROR_BUSY:

                    DEBUG_TEXT(ERM_SWI_ERR, MOD_ERM,"CCPhyTable: DelPersActProfUser: Eeprom write busy");
                    /* try again in some mseconds */
                    (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_ERM_PENDING, MSEC(10), MSEC(0));
                    break;
                default:

                    DEBUG_VALUE1(ERM_SWI_ERR, MOD_ERM,"CCPhyTable: DelPersActProfUser error %d", rsp);
                    break;
            }
            return FAIL;
        }
    }

    return SUCCESS;
}


/*----------------------------------------------------------------------------*/
/* Function    : _ERM_sfConsistencyCheck_ActiveProfile_Player                 */
/**
    \brief

    \param      none

    \return     none
   */
/*----------------------------------------------------------------------------*/
static SuccessFailType _ERM_sfConsistencyCheck_ActiveProfile_Player(void)
{
    AT_DeviceId          i = 0;

    for( i=0; i<ERM_MAX_NUM_PLAYER_PROFILE; i++)
    {
        BooleanType isDevConsistent = BT_TRUE;
        Uint8Type profileIdx = _ERM_stActiveProfileList.PlayerProfileList[i];

        if( profileIdx == AT_INVALID_DEVICE_ID)
        {
            continue;
        }

        /* 5.1 first check that the profile index not exceeds the number of PlayerHeadset profiles */
        if( profileIdx >= ERM_MAX_NUM_PLAYER_HS_TABLE )
        {

            DEBUG_VALUE1(ERM_SWI_ERR, MOD_ERM,"CCActProfPlay: dev id invalid %d",
                          profileIdx);
            isDevConsistent = BT_FALSE;
        }
        else
        {
            /* 5.2 to be consistent we need the corresponding profile in the PlayerHeadsetTable */
            if(_ERM_stPlayerHeadsetTable[profileIdx].deviceIndex == AT_INVALID_DEVICE_ID)
            {
                /* the profile is not available in the UserTable */

                DEBUG_VALUE1(ERM_SWI_ERR, MOD_ERM,"CCActProfPlay: not found in PlHsTable %d",
                            profileIdx);
                isDevConsistent = BT_FALSE;
            }
        }

        if(isDevConsistent != BT_TRUE)
        {
            APP_COMMON_enResponseCode rsp = _ERM_rcPrepare_DeleteActiveProfile( profileIdx, APP_DEVICE_enTYPE_PLAYER);
            if( APP_COMMON_RSP_OK == rsp )
            {
                /* set pending msg and callback and start */
                rsp = _ERM_rcPrepare_EepromUpdate(_ERM_OnConsistencyCheckCb, ERM_REQ_CONSISTENCY_CHECK);
            }
            /* check error condition */
            switch( rsp )
            {
                case APP_COMMON_RSP_OK:
                    /* everything looks fine, go on */
                    /* wait for result */
                    _ERM_gContext.eConsistCheckState = ERM_CC_WAIT_FOR_RESPONSE;
                    break;
                case APP_COMMON_RSP_ERROR_BUSY:

                    DEBUG_TEXT(ERM_SWI_ERR, MOD_ERM,"CCPhyTable: _ERM_rcDelPersActProfPl: Eeprom write busy");
                    /* try again in some mseconds */
                    (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_ERM_PENDING, MSEC(10), MSEC(0));
                    break;
                default:

                    DEBUG_VALUE1(ERM_SWI_ERR, MOD_ERM,"CCPhyTable: _ERM_rcDelPersActProfPl error %d", rsp);
                    break;
            }
            return FAIL;
        }
    }

    return SUCCESS;
}


/*----------------------------------------------------------------------------*/
/* Function    : _ERM_sfConsistencyCheck_ActiveProfile_Headset                */
/**
    \brief

    \param      none

    \return     none
   */
/*----------------------------------------------------------------------------*/
static SuccessFailType _ERM_sfConsistencyCheck_ActiveProfile_Headset(void)
{
    AT_DeviceId          i = 0;

    for( i=0; i<ERM_MAX_NUM_HEADSET_PROFILE; i++)
    {
        BooleanType isDevConsistent = BT_TRUE;
        Uint8Type profileIdx = _ERM_stActiveProfileList.HeadsetProfileList[i];

        if( profileIdx == AT_INVALID_DEVICE_ID)
        {
            continue;
        }

        /* 5.1 first check that the profile index not exceeds the number of PlayerHeadset profiles */
        if( profileIdx >= ERM_MAX_NUM_PLAYER_HS_TABLE )
        {

            DEBUG_VALUE1(ERM_SWI_ERR, MOD_ERM,"CCActProfHS: dev id invalid %d",
                          profileIdx);
            isDevConsistent = BT_FALSE;
        }
        else
        {
            /* 5.2 to be consistent we need the corresponding profile in the PlayerHeadsetTable */
            if(_ERM_stPlayerHeadsetTable[profileIdx].deviceIndex == AT_INVALID_DEVICE_ID)
            {
                /* the profile is not available in the UserTable */

                DEBUG_VALUE1(ERM_SWI_ERR, MOD_ERM,"CCActProfHS: not found in PlHsTable %d",
                              profileIdx);
                isDevConsistent = BT_FALSE;
            }
        }

        if(isDevConsistent != BT_TRUE)
        {
            APP_COMMON_enResponseCode rsp = _ERM_rcPrepare_DeleteActiveProfile( profileIdx, APP_DEVICE_enTYPE_HEADSET);
            if( APP_COMMON_RSP_OK == rsp )
            {
                /* set pending msg and callback and start */
                rsp = _ERM_rcPrepare_EepromUpdate(_ERM_OnConsistencyCheckCb, ERM_REQ_CONSISTENCY_CHECK);
            }
            /* check error condition */
            switch( rsp )
            {
                case APP_COMMON_RSP_OK:
                    /* wait for result */
                    _ERM_gContext.eConsistCheckState = ERM_CC_WAIT_FOR_RESPONSE;
                    break;
                case APP_COMMON_RSP_ERROR_BUSY:

                    DEBUG_TEXT(ERM_SWI_ERR, MOD_ERM,"CCPhyTable: _ERM_rcDelPersActProfHs: Eeprom write busy");
                    /* try again in some mseconds */
                    (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_ERM_PENDING, MSEC(10), MSEC(0));
                    break;
                default:

                    DEBUG_VALUE1(ERM_SWI_ERR, MOD_ERM,"CCPhyTable: _ERM_rcDelPersActProfHs error %d", rsp);
                    break;
            }
            return FAIL;
        }
    }

    return SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* Function    : _ERM_u8GetReleasedDevice                                     */
/**
    \brief

    \param      none

    \return     none
   */
/*----------------------------------------------------------------------------*/
//AT_DeviceId _ERM_u8GetReleasedDevice()
//{
//    Uint32Type  i = 0;
//
//    for( i=0; i<=AT_MAX_DEVICE_ID; i++)
//    {
//        if( _ERM_stBTDeviceTable[i].device_state == ERM_DEVICE_STATE_RELEASED )
//        {
//            return i;
//        }
//    }
//
//    return AT_INVALID_DEVICE_ID;
//}

/*----------------------------------------------------------------------------*/
/* Function    : _ERM_sfDeleteDevice                                          */
/**
    \brief

    \param      none

    \return     none
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ERM_sfDeleteDevice(AT_DeviceId deviceId)
{
    if( deviceId > AT_MAX_DEVICE_ID )
    {
        return FAIL;
    }

    _ERM_gContext.u8DeleteDeviceId  = deviceId;
    (void)_ERM_SendMessage(ERM_REQ_DELETE_DEVICE,NULL);
    return SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_GetBTDevice                                              */
/**
    \brief

    \param      none

    \return     none
   */
/*----------------------------------------------------------------------------*/
const ERMBTDeviceTableType *ERM_GetBTDevice(AT_DeviceId deviceId)
{
    if( deviceId > AT_MAX_DEVICE_ID )
    {
        return NULL;
    }

    if( _ERM_stBTDeviceTable[deviceId].device_state != ERM_DEVICE_STATE_USED )
    {
        return NULL;
    }


    return &_ERM_stBTDeviceTable[deviceId];
}


/*----------------------------------------------------------------------------*/
/* Function    : ERM_GetDeviceIdOfUmpteenthPhone                               */
/**
    \brief      get the device id of the umpteenth phone ( x-tes )

    \param      selIdx
                Uint16Type
                Index selected from a continuous list

    \return     deviceId
                Uint16Type
   */
/*----------------------------------------------------------------------------*/
AT_DeviceId ERM_GetDeviceIdOfUmpteenthPhone(Uint16Type selIdx)
{
    Uint16Type count    = 0;
    Uint16Type i        = 0;
    Uint8Type deviceId = AT_INVALID_DEVICE_ID;

    if( selIdx <= ERM_MAX_NUM_USER_PROFILES )
    {
        for(i=0;i<ERM_MAX_NUM_USER_PROFILES;i++)
        {
            if(_ERM_stUserTable[i].deviceIndex != AT_INVALID_DEVICE_ID)
            {
                count++;
                if( count == selIdx )
                {
                    // found the umpteenth device with phone support
                    deviceId = _ERM_stUserTable[i].deviceIndex;
                    break;
                }
            }
        }
    }

    return deviceId;
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_GetDeviceIdOfUmpteenthHeadset                               */
/**
    \brief      get the device id of the umpteenth headset ( x-tes )

    \param      selIdx
                Uint16Type
                Index selected from a continuous list

    \return     deviceId
                Uint16Type
   */
/*----------------------------------------------------------------------------*/
Uint16Type ERM_GetDeviceIdOfUmpteenthHeadset(Uint16Type selIdx)
{
    Uint16Type count    = 0;
    Uint16Type i        = 0;
    Uint16Type deviceId = AT_INVALID_DEVICE_ID;

    if( selIdx <= ERM_MAX_NUM_PLAYER_HS_TABLE )
    {
        for(i=0;i<ERM_MAX_NUM_PLAYER_HS_TABLE;i++)
        {
            if( (_ERM_stPlayerHeadsetTable[i].deviceIndex != AT_INVALID_DEVICE_ID) &&
                (_ERM_stPlayerHeadsetTable[i].deviceType == APP_DEVICE_enTYPE_HEADSET) )
            {
                count++;
                if( count == selIdx )
                {
                    deviceId = _ERM_stPlayerHeadsetTable[i].deviceIndex;
                    break;
                }
            }
        }
    }


    return deviceId;
}


/*----------------------------------------------------------------------------*/
/* Function    : ERM_GetDeviceIdOfUmpteenthPlayer                               */
/**
    \brief      get the device id of the umpteenth Player ( x-tes )

    \param      selIdx
                Uint16Type
                Index selected from a continuous list

    \return     deviceId
                Uint16Type
   */
/*----------------------------------------------------------------------------*/
Uint16Type ERM_GetDeviceIdOfUmpteenthPlayer(Uint16Type selIdx)
{
    Uint16Type count    = 0;
    Uint16Type i        = 0;
    Uint16Type deviceId = AT_INVALID_DEVICE_ID;

    if( selIdx <= ERM_MAX_NUM_PLAYER_HS_TABLE )
    {
        for(i=0;i<ERM_MAX_NUM_PLAYER_HS_TABLE;i++)
        {
            if( (_ERM_stPlayerHeadsetTable[i].deviceIndex != AT_INVALID_DEVICE_ID) &&
                (_ERM_stPlayerHeadsetTable[i].deviceType == APP_DEVICE_enTYPE_PLAYER) )
            {
                count++;
                if( count == selIdx )
                {
                    deviceId = _ERM_stPlayerHeadsetTable[i].deviceIndex;
                    break;
                }
            }
        }
    }

    return deviceId;
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_u8GetActiveHeadSet                               */
/**
    \brief      get the device id of the current active headset or
                AT_INVALID_DEVICE_ID if no headset is connected / active

    \return     deviceId
                AT_DeviceId
   */
/*----------------------------------------------------------------------------*/
AT_DeviceId ERM_u8GetActiveHeadSet(void)
{
    AT_DeviceId deviceId    = AT_INVALID_DEVICE_ID;
    Uint8Type i            = 0;

    for( i=0; i<=AT_MAX_DEVICE_ID; i++)
    {
        // search for device which supports audio gateway, where audio gateway
        // is active and device is connected to uhv
        if( AT_SUPPORTED == _ERM_stBTDeviceTable[i].audio_gw_supported &&
            AT_TRUE      == _ERM_stBTDeviceTable[i].audio_gw_active &&
            AT_DEVICE_CONNECTED      == _ERM_stBTDeviceTable[i].device_connected )
        {
            deviceId = i;
            break;
        }
    }
    return deviceId;
}




/*----------------------------------------------------------------------------*/
/* Function    : ERM_u8GetActivePlayer                               */
/**
    \brief      get the device id of the current active player or
                AT_INVALID_DEVICE_ID if no player is connected / active

    \return     deviceId
                AT_DeviceId
   */
/*----------------------------------------------------------------------------*/
AT_DeviceId ERM_u8GetActivePlayer(void)
{
    AT_DeviceId deviceId    = AT_INVALID_DEVICE_ID;
    Uint8Type i            = 0;

    for( i=0; i<=AT_MAX_DEVICE_ID; i++)
    {
        if( AT_SUPPORTED == _ERM_stBTDeviceTable[i].a2dp_sink_supported &&
            AT_TRUE      == _ERM_stBTDeviceTable[i].a2dp_sink_active &&
            AT_DEVICE_CONNECTED      == _ERM_stBTDeviceTable[i].device_connected )
        {
            deviceId = i;
            break;
        }
    }
    return deviceId;
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_u8GetActiveDeviceAVRCP                                   */
/**
    \brief      get id of the device to which the AVRCP service has been started
                AT_INVALID_DEVICE_ID if no AVRCP service is started

    \return     deviceId
                AT_DeviceId
   */
/*----------------------------------------------------------------------------*/
AT_DeviceId ERM_u8GetActiveDeviceAVRCP(void)
{
    AT_DeviceId deviceId    = AT_INVALID_DEVICE_ID;
    Uint8Type i            = 0;

    for( i=0; i<=AT_MAX_DEVICE_ID; i++)
    {
        if( AT_SUPPORTED == _ERM_stBTDeviceTable[i].avrcp_supported &&
            AT_TRUE      == _ERM_stBTDeviceTable[i].avrcp_active &&
            AT_DEVICE_CONNECTED      == _ERM_stBTDeviceTable[i].device_connected )
        {
            deviceId = i;
            break;
        }
    }
    return deviceId;
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_u8GetActivePhone                               */
/**
    \brief      get the device id of the current active phone or
                AT_INVALID_DEVICE_ID if no player is connected / active

    \return     deviceId
                AT_DeviceId
   */
/*----------------------------------------------------------------------------*/
AT_DeviceId ERM_u8GetActivePhone(void)
{
    AT_DeviceId deviceId    = AT_INVALID_DEVICE_ID;
    Uint8Type i            = 0;

    for( i=0; i<=AT_MAX_DEVICE_ID; i++)
    {
        if( AT_TRUE      == _ERM_stBTDeviceTable[i].phone_active &&
            AT_DEVICE_CONNECTED      == _ERM_stBTDeviceTable[i].device_connected )
        {
            deviceId = i;
            break;
        }
    }
    return deviceId;
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_vSetDisconnectWillFollow                               */
/**
    \brief     store boolean if a disconnect during reconnect will follow

    \return     void
                
   */
/*----------------------------------------------------------------------------*/
void ERM_vSetDisconnectWillFollow(BooleanType bDisconnectWillFollow)
{
    _ERM_gContext.bDisconnectWillFollow = bDisconnectWillFollow;
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_bGetDisconnectWillFollow                               */
/**
    \brief     store boolean if a disconnect during reconnect will follow

    \return     void
                
   */
/*----------------------------------------------------------------------------*/
BooleanType ERM_bGetDisconnectWillFollow(void)
{
    return _ERM_gContext.bDisconnectWillFollow;
}
/*----------------------------------------------------------------------------*/
/* Function    : ERM_u8GetActiveHeadSet                               */
/**
    \brief      get the device id of the last active headset or
                AT_INVALID_DEVICE_ID if no headset is connected / active

    \return     deviceId
                AT_DeviceId
   */
/*----------------------------------------------------------------------------*/
AT_DeviceId ERM_u8GetLastActiveHeadSet(void)
{
    AT_DeviceId deviceId = AT_INVALID_DEVICE_ID;
    Uint8Type profileId  = AT_INVALID_DEVICE_ID;

    profileId = _ERM_stActiveProfileList.HeadsetProfileList[0];

    if( profileId < ERM_MAX_NUM_PLAYER_HS_TABLE)
    {
        deviceId = _ERM_stPlayerHeadsetTable[profileId].deviceIndex;
    }

    return deviceId;
}
/*----------------------------------------------------------------------------*/
/* Function    : ERM_u8GetLastActivePlayer                               */
/**
    \brief      get the device id of the last active player or
                AT_INVALID_DEVICE_ID if no player is connected / active

    \return     deviceId
                AT_DeviceId
   */
/*----------------------------------------------------------------------------*/
AT_DeviceId ERM_u8GetLastActivePlayer(void)
{
    AT_DeviceId deviceId = AT_INVALID_DEVICE_ID;
    Uint8Type profileId  = AT_INVALID_DEVICE_ID;

    profileId = _ERM_stActiveProfileList.PlayerProfileList[0];

    if( profileId < ERM_MAX_NUM_PLAYER_HS_TABLE)
    {
        deviceId = _ERM_stPlayerHeadsetTable[profileId].deviceIndex;
    }

    return deviceId;
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_u8GetLastActivePhone                               */
/**
    \brief      get the device id of the last active phone or
                AT_INVALID_DEVICE_ID if no player is connected / active

    \return     deviceId
                AT_DeviceId
   */
/*----------------------------------------------------------------------------*/
AT_DeviceId ERM_u8GetLastActivePhone(void)
{
    AT_DeviceId deviceId = AT_INVALID_DEVICE_ID;
    Uint8Type profileId  = AT_INVALID_DEVICE_ID;

    profileId = _ERM_stActiveProfileList.UserProfileList[0];

    if( profileId < ERM_MAX_NUM_USER_PROFILES)
    {
        deviceId = _ERM_stUserTable[profileId].deviceIndex;
    }

    return deviceId;


}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_GetUmpteenthRingtone                               */
/**
    \brief      get the umpteenth ringtone ( x-ten )

    \param      selIdx
                Uint16Type
                Index selected from a continuous list

    \return     ringtone
                ATRintoneModeEnum
   */
/*----------------------------------------------------------------------------*/
ATRintoneModeEnum ERM_eGetUmpteenthRingtone(Uint16Type selIdx)
{
    ATRintoneModeEnum ringtone = AT_RTMODE_INVALID;
    /* we start with first ringtone, activate / deactivate are ignored for now */
    switch(selIdx)
    {
    /*
       case 1:
            ringtone = AT_RTMODE_CUSTOM8;
            break;
       case 2:
            ringtone = AT_RTMODE_CUSTOM9;
            break;
    */
        case 1:
            ringtone = AT_RTMODE_VIBORBELLZ;
            break;
        case 2:
            ringtone = AT_RTMODE_BIMBAM;
            break;
        case 3:
            ringtone = AT_RTMODE_EASYLISTEN;
            break;
        case 4:
            ringtone = AT_RTMODE_FUTUREBEAT;
            break;
        case 5:
            ringtone = AT_RTMODE_ENDLESS;
            break;
        case 6:
            ringtone = AT_RTMODE_RINGRING;
            break;
        case 7:
            ringtone = AT_RTMODE_NOSTALGIC;
            break;
        case 8:
            ringtone = AT_RTMODE_ORCHESTRA1;
            break;
        case 9:
            ringtone = AT_RTMODE_ORCHESTRA2;
            break;
        case 10:
            ringtone = AT_RTMODE_SWEETSOFT;
            break;
        case 11:
            ringtone = AT_RTMODE_TUTTUT;
            break;
        //case 12:
        //    ringtone = AT_RTMODE_VW_12;
        //    break;
        //case 13:
        //    ringtone = AT_RTMODE_VW_13;
        //    break;
        //case 14:
        //    ringtone = AT_RTMODE_VW_14;
        //    break;
        //case 15:
        //    ringtone = AT_RTMODE_VW_15;
        //    break;
        default:
            ringtone = AT_RTMODE_INVALID;
            break;
    }
    return ringtone;
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_GetIndexForRingtone                                      */
/**
    \brief      get the index for a given ringtone

    \param      ringtone
                ATRintoneModeEnum
                ringtone

    \return     deviceId
                Uint16Type
   */
/*----------------------------------------------------------------------------*/
Uint16Type ERM_GetIndexForRingtone(ATRintoneModeEnum ringtone)
{
    Uint16Type index = 0;
    /* we start with first ringtone, activate / deactivate are ignored for now */
    switch(ringtone)
    {
        /*
       case AT_RTMODE_CUSTOM8:
            index = 1;
            break;
       case AT_RTMODE_CUSTOM9:
            index = 2;
            break;
        */
        case AT_RTMODE_VIBORBELLZ:
            index = 1;
            break;
        case AT_RTMODE_BIMBAM:
            index = 2;
            break;
        case AT_RTMODE_EASYLISTEN:
            index = 3;
            break;
        case AT_RTMODE_FUTUREBEAT:
            index = 4;
            break;
        case AT_RTMODE_ENDLESS:
            index = 5;
            break;
        case AT_RTMODE_RINGRING:
            index = 6;
            break;
        case AT_RTMODE_NOSTALGIC:
            index = 7;
            break;
        case AT_RTMODE_ORCHESTRA1:
            index = 8;
            break;
        case AT_RTMODE_ORCHESTRA2:
            index = 9;
            break;
        case AT_RTMODE_SWEETSOFT:
            index = 10;
            break;
        case AT_RTMODE_TUTTUT:
            index = 11;
            break;
        //case AT_RTMODE_VW_12:
        //    index = 12;
        //    break;
        //case AT_RTMODE_VW_13:
        //    index = 13;
        //    break;
        //case AT_RTMODE_VW_14:
        //    index = 14;
        //    break;
        //case AT_RTMODE_VW_15:
        //    index = 15;
        //    break;
        default:
            index = 12;
            break;
    }
    return index;
}


Uint16Type ERM_u16GetNumberOfRingtones(void)
{
    return   AT_SIVR_NUMBER_OF_RINGTONES;
}


/*----------------------------------------------------------------------------*/
/* Function    : ERM_GetLastPairedDeviceId                                    */
/**
    \brief      returns device id of last successfully paired device

    \param      none

    \return     deviceId
                AT_DeviceId
   */
/*----------------------------------------------------------------------------*/
AT_DeviceId ERM_GetLastPairedDeviceId(void)
{
    return _ERM_gContext.u8LastPairedDeviceId;
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_GetLastServiceDeviceId                                   */
/**
    \brief      returns device id of last service update

    \param      none

    \return     deviceId
                AT_DeviceId
   */
/*----------------------------------------------------------------------------*/
AT_DeviceId ERM_GetLastServiceDeviceId(void)
{
    return _ERM_gContext.u8LastServiceDeviceId;
}


/*----------------------------------------------------------------------------*/
/* Function    : ERM_GetBTDeviceName                                          */
/**
    \brief

    \param      none

    \return     none
   */
/*----------------------------------------------------------------------------*/
const Uint8Type *ERM_GetBTDeviceName(void)
{
    return _ERM_gContext.stBTName;
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_GetDeviceIdOfBtAddr                                     */
/**
    \brief      Compares the provided BT Address with all known devices and
                returns the ID of the device that matches the BT Address

    \param      pBtAddr
                AT_BT_Address*

    \return     deviceId
                AT_DeviceId
    */
/*----------------------------------------------------------------------------*/
AT_DeviceId ERM_GetDeviceIdOfBtAddr(const AT_BT_Address btAddr)
{
    AT_DeviceId deviceId = AT_INVALID_DEVICE_ID;
    AT_DeviceId i = 0;

    for( i=0; i<=AT_MAX_DEVICE_ID; i++)
    {
        if( (_ERM_stBTDeviceTable[i].device_state == ERM_DEVICE_STATE_USED) &&
            (memcmp(_ERM_stBTDeviceTable[i].bt_addr, btAddr, sizeof(AT_BT_Address)) == 0) )
        {
            deviceId = i;
            break;
        }
    }

    return deviceId;
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_u16GetDeviceCount                                        */
/**
    \brief

    \param      none

    \return     none
   */
/*----------------------------------------------------------------------------*/
Uint16Type ERM_u16GetDeviceCount()
{
    Uint16Type  count   = 0;
    Uint32Type  i       = 0;

    for( i = (Uint32Type)0U; i <= (Uint32Type)AT_MAX_DEVICE_ID; i++)
    {
        if( _ERM_stBTDeviceTable[i].device_state == ERM_DEVICE_STATE_USED )
        {
            count++;
        }
    }

    return count;
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_bIsHeadset                                               */
/**
    \brief

    \return     BooleanType
   */
/*----------------------------------------------------------------------------*/
BooleanType ERM_bIsHeadset(AT_DeviceId deviceId)
{
    if( deviceId <= AT_MAX_DEVICE_ID &&
        _ERM_stBTDeviceTable[deviceId].device_state == ERM_DEVICE_STATE_USED &&
        _ERM_stBTDeviceTable[deviceId].audio_gw_supported  == AT_SUPPORTED )
    {
        return BT_TRUE;
    }

    return BT_FALSE;
}


/*----------------------------------------------------------------------------*/
/* Function    : ERM_bIsPlayer                                               */
/**
    \brief

    \return     BooleanType
   */
/*----------------------------------------------------------------------------*/
BooleanType ERM_bIsPlayer(AT_DeviceId deviceId)
{
    if( deviceId <= AT_MAX_DEVICE_ID &&
        _ERM_stBTDeviceTable[deviceId].device_state == ERM_DEVICE_STATE_USED &&
        AT_SUPPORTED == _ERM_stBTDeviceTable[deviceId].a2dp_sink_supported )
    {
        return BT_TRUE;
    }

    return BT_FALSE;
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_bIsPhone                                                 */
/**
    \brief

    \return     BooleanType
   */
/*----------------------------------------------------------------------------*/
BooleanType ERM_bIsPhone(AT_DeviceId deviceId)
{
    if( deviceId <= AT_MAX_DEVICE_ID &&
        _ERM_stBTDeviceTable[deviceId].device_state == ERM_DEVICE_STATE_USED &&
        _ERM_stBTDeviceTable[deviceId].support_dial == AT_SUPPORTED)
    {
        return BT_TRUE;
    }

    return BT_FALSE;
}


/*----------------------------------------------------------------------------*/
/* Function    : ERM_bSdpReceived                                             */
/**
    \brief

    \return     BooleanType
   */
/*----------------------------------------------------------------------------*/
BooleanType ERM_bSdpReceived(AT_DeviceId deviceId)
{
    if( deviceId <= AT_MAX_DEVICE_ID && _ERM_stBTDeviceTable[deviceId].sdp_received == AT_TRUE )
    {
        return BT_TRUE;
    }

    return BT_FALSE;
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_u16GetNumberOfPairedHeadSets                             */
/**
    \brief      returns the number of paired headsets

    \return     Uint16Type
   */
/*----------------------------------------------------------------------------*/
Uint16Type ERM_u16GetNumberOfPairedHeadSets(void)
{
    Uint16Type  count   = 0;
    AT_DeviceId i       = 0;

    for( i=0; i<=AT_MAX_DEVICE_ID; i++)
    {
        if( ERM_bIsHeadset(i) == BT_TRUE )
        {
            count++;
        }
    }

    return count;
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_u16GetNumberOfHeadsetProfiles                             */
/**
    \brief      returns the number of headset profiles

    \return     Uint16Type
   */
/*----------------------------------------------------------------------------*/
Uint16Type ERM_u16GetNumberOfHeadsetProfiles(void)
{
    Uint16Type  count   = 0;
    AT_DeviceId i       = 0;

    for( i=0; i<ERM_MAX_NUM_PLAYER_HS_TABLE; i++)
    {
        if( AT_INVALID_DEVICE_ID != _ERM_stPlayerHeadsetTable[i].deviceIndex &&
            APP_DEVICE_enTYPE_HEADSET == _ERM_stPlayerHeadsetTable[i].deviceType)
        {
            count++;
        }
    }

    return count;
}



/*----------------------------------------------------------------------------*/
/* Function    : ERM_u16GetNumberOfPairedPlayer                             */
/**
    \brief      returns the number of paired player

    \return     Uint16Type
   */
/*----------------------------------------------------------------------------*/
Uint16Type ERM_u16GetNumberOfPairedPlayer(void)
{
    Uint16Type  count   = 0;
    AT_DeviceId i       = 0;

    for( i=0; i<=AT_MAX_DEVICE_ID; i++)
    {
        if( ERM_bIsPlayer(i) == BT_TRUE )
        {
            count++;
        }
    }

    return count;
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_u16GetNumberOfPlayerProfiles                             */
/**
    \brief      returns the number of player profiles

    \return     Uint16Type
   */
/*----------------------------------------------------------------------------*/
Uint16Type ERM_u16GetNumberOfPlayerProfiles(void)
{
    Uint16Type  count   = 0;
    AT_DeviceId i       = 0;

    for( i=0; i<ERM_MAX_NUM_PLAYER_HS_TABLE; i++)
    {
        if( AT_INVALID_DEVICE_ID != _ERM_stPlayerHeadsetTable[i].deviceIndex &&
            APP_DEVICE_enTYPE_PLAYER == _ERM_stPlayerHeadsetTable[i].deviceType)
        {
            count++;
        }
    }

    return count;
}



/*----------------------------------------------------------------------------*/
/* Function    : ERM_u16GetNumberOfPairedPhones                               */
/**
    \brief      returns the number of paired phones

    \return     Uint16Type
   */
/*----------------------------------------------------------------------------*/
Uint16Type ERM_u16GetNumberOfPairedPhones(void)
{
    Uint16Type  count   = 0;
    AT_DeviceId i       = 0;

    for( i=0; i<=AT_MAX_DEVICE_ID; i++)
    {
        if( ERM_bIsPhone(i) == BT_TRUE )
        {
            count++;
        }
    }
    return count;
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_u16GetNumberOfUserProfiles                               */
/**
    \brief      returns the number of user profile

    \return     Uint16Type
   */
/*----------------------------------------------------------------------------*/
Uint16Type ERM_u16GetNumberOfUserProfiles(void)
{
    Uint16Type  count   = 0;
    AT_DeviceId i       = 0;

    for( i=0; i<ERM_MAX_NUM_USER_PROFILES; i++)
    {
        if( AT_INVALID_DEVICE_ID != _ERM_stUserTable[i].deviceIndex )
        {
            /* found used profile */
            count++;
        }
    }

    return count;
}


/*----------------------------------------------------------------------------*/
/* Function    : ERM_bDeviceAlreadyKnownAsHeadset                            */
/**
    \brief      returns whether a device is already known as headset or if its a
                new headset

    \param      deviceId
                AT_DeviceId

    \return     BooleanType
                BT_TRUE, if a Headset profile exists for deviceId,
                otherwise BT_FALSE
*/
/*----------------------------------------------------------------------------*/
BooleanType ERM_bDeviceAlreadyKnownAsHeadset(AT_DeviceId deviceId)
{
    Uint16Type i = 0;
    for( i=0; i<ERM_MAX_NUM_PLAYER_HS_TABLE; i++)
    {
        if( deviceId == _ERM_stPlayerHeadsetTable[i].deviceIndex &&
            APP_DEVICE_enTYPE_HEADSET == _ERM_stPlayerHeadsetTable[i].deviceType)
        {
            return BT_TRUE;
        }
    }

    /* not known as headset */
    return BT_FALSE;

}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_bDeviceAlreadyKnownAsPlayer                              */
/**
    \brief      returns whether a device is already known as player or if its a
                new player

    \param      deviceId
                AT_DeviceId

    \return     BooleanType
                BT_TRUE, if a Player profile exists for deviceId,
                otherwise BT_FALSE
*/
/*----------------------------------------------------------------------------*/
BooleanType ERM_bDeviceAlreadyKnownAsPlayer(AT_DeviceId deviceId)
{
    Uint16Type i = 0;
    for( i=0; i<ERM_MAX_NUM_PLAYER_HS_TABLE; i++)
    {
        if( deviceId == _ERM_stPlayerHeadsetTable[i].deviceIndex &&
            APP_DEVICE_enTYPE_PLAYER == _ERM_stPlayerHeadsetTable[i].deviceType)
        {
            return BT_TRUE;
        }
    }

    /* not known as headset */
    return BT_FALSE;

}


/*----------------------------------------------------------------------------*/
/* Function    : _ERM_sfReadPersistentUserProfile                             */
/**
    \brief      read back one entry from persistent user profile list

    \param      none

    \return     SuccessFailType SUCCESS if read was successfull
                FAIL is problem occured, e.g. as EPROM was busy  if so caller
                shall retry
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ERM_sfReadPersistentUserProfile(Uint8Type index)
{
    SuccessFailType          sF         = FAIL;
    CommonResponseCodeType   rsp        = COMMON_RSP_OK;
    EEDeviceControlBlockType *pEEApp    = getEEAppTaskControl();

    /* check for valid index */
    if (ERM_MAX_NUM_USER_PROFILES > index)
    {
        rsp = EEPROM_sfRead( pEEApp,
                (EEAddressType)(VWDB_EE_ADDDRESS_BASE_USER_PROFILE_DeviceIndex_0 +
                    index * sizeof(ERMUserProfileType)),
                (Uint8Type *)&(_ERM_stUserTable[index]),
                sizeof(ERMUserProfileType));

        switch(rsp)
        {
            case COMMON_RSP_OK:
                sF = SUCCESS;

                DEBUG_VALUE2(ERM_SWI_DBG, MOD_ERM,"user profile idx %d read %d",index,sF);
                break;
            case COMMON_RSP_ERROR_BUSY:
                // TODO!!!!!!!!!!!!!

                DEBUG_VALUE1(ERM_SWI_ERR, MOD_ERM,"E2P Read BUSY: %d ERM_DeviceHandler.c",__LINE__);
                break;
            default:
                /* error reading e2p */

                DEBUG_VALUE1(ERM_SWI_ERR, MOD_ERM,"E2P Read FAIL: %d ERM_DeviceHandler.c",__LINE__);
                break;
        }
    }
    else
    {

        DEBUG_VALUE2(ERM_SWI_TRC, MOD_ERM,"user profile idx oor %d > %d",
                index,ERM_MAX_NUM_USER_PROFILES);
    }
    return sF;
}


/*----------------------------------------------------------------------------*/
/* Function    : _ERM_rcWritePersistentUserProfile                            */
/**
    \brief      write the values stored in tmp to user profile index in tmp

    \param      none

    \return     APP_COMMON_enResponseCode
   */
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode _ERM_rcWritePersistentUserProfile(void)
{
    CommonResponseCodeType   rC      = COMMON_RSP_OK;
    EEDeviceControlBlockType *pEEApp = getEEAppTaskControl();
    FlagType                 flag    = EEPROM_flagIsEEFree(pEEApp);

    if (ERM_MAX_NUM_USER_PROFILES > _ERM_gContext.epr_tmp.u8UserIdx)
    {
        if(flag == TRUE)
        {

            DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM,"WrPersUserProf: prof %d",
                         _ERM_gContext.epr_tmp.u8UserIdx );

            rC = EEPROM_LAYOUT_rcWrite( pEEApp,
                   (VWDB_EE_ADDDRESS_BASE_USER_PROFILE_DeviceIndex_0 +
                    _ERM_gContext.epr_tmp.u8UserIdx *sizeof(ERMUserProfileType)),
                    (Uint8Type*)&(_ERM_gContext.epr_tmp.user),
                    sizeof(ERMUserProfileType),
                    _ERM_vEepromWriteFinished_Cb );
            if (COMMON_RSP_OK == rC)
            {
                return APP_COMMON_RSP_OK;
            }
        }
        else
        {   /* busy */
            return APP_COMMON_RSP_ERROR_BUSY;
        }
    }

    DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM,"WrPersUserProf: failed prof %d",
                  _ERM_gContext.epr_tmp.u8UserIdx );

    return APP_COMMON_RSP_ERROR;
}




/*----------------------------------------------------------------------------*/
/* Function    : _ERM_vCopyTmpUserProfileToRam                            */
/**
    \brief      called when writing tmp user profile to eeprom was successful \
                tmp user profile will now be copied to ram at position of
                u8UserIdx\n

    \param      none

    \return     SuccessFailType
   */
/*----------------------------------------------------------------------------*/
void _ERM_vCopyTmpUserProfileToRam(void)
{
    /* user profile table was changed */

    /* SUSANNE: 18.03.2010 As described in Wiki and Ticket #5045 do not read E2P,
       just copy from epr_tmp */

//    Uint8Type idx = _ERM_gContext.epr_tmp.u8UserIdx;
//    if(_ERM_sfReadPersistentUserProfile(idx) == SUCCESS)
//    {
//        if(memcmp( &(_ERM_stUserTable[idx]),&_ERM_gContext.epr_tmp.user,
//                   sizeof(ERMUserProfileType)) != 0)
//        {

//        }
//    }
//    else
//    {

//    }

    Uint8Type idx = _ERM_gContext.epr_tmp.u8UserIdx;


    DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM, "CopyTmpUsrProfToRam idx: %d", idx);

    ////* copy tmp user profile into ram at pos userIdx */
    if (ERM_MAX_NUM_USER_PROFILES > idx)
    {
        /* user profile table was changed */

        //* now copy whole tmp user profile to ram */
        (void)memcpy(&(_ERM_stUserTable[idx]),&_ERM_gContext.epr_tmp.user,
                 sizeof(ERMUserProfileType));
    }
}


/*----------------------------------------------------------------------------*/
/* Function    : _ERM_sfReadPersistentPlayerHsProfile                         */
/**
    \brief      read back one entry from persistent player / headset profile list

    \param      none

    \return     SuccessFailType SUCCESS if read was successfull
                FAIL is problem occured, e.g. as EPROM was busy  if so caller
                shall retry
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ERM_sfReadPersistentPlayerHsProfile(Uint8Type index)
{
    SuccessFailType         sF          = FAIL;
    CommonResponseCodeType  rsp         = COMMON_RSP_OK;
    EEDeviceControlBlockType *pEEApp    = getEEAppTaskControl();

    /* check for valid index */
    if (ERM_MAX_NUM_PLAYER_HS_TABLE > index)
    {
        rsp = EEPROM_sfRead( pEEApp,
                (EEAddressType)(VWDB_EE_ADDDRESS_BASE_PLAYERHS_PROFILE_deviceIndex_0 +
                    index * sizeof(ERMPlayerHeadsetProfileType)),
                (Uint8Type *)&(_ERM_stPlayerHeadsetTable[index]),
                sizeof(ERMPlayerHeadsetProfileType));

        switch(rsp)
        {
            case COMMON_RSP_OK:
                sF = SUCCESS;

                DEBUG_VALUE2(ERM_SWI_DBG, MOD_ERM,"user playerhs idx %d read %d",index,sF);
                break;
            case COMMON_RSP_ERROR_BUSY:
                // TODO!!!!!!!!!!!!!!!!

                DEBUG_VALUE1(ERM_SWI_ERR, MOD_ERM,"E2P Read BUSY: %d ERM_DeviceHandler.c",__LINE__);
                break;
            default:
                /* error reading */

                DEBUG_VALUE1(ERM_SWI_ERR, MOD_ERM,"E2P Read FAIL: %d ERM_DeviceHandler.c",__LINE__);
                break;
        }
    }
    else
    {

        DEBUG_VALUE2(ERM_SWI_TRC, MOD_ERM,"playerhs profile idx oor %d > %d",
                index,ERM_MAX_NUM_PLAYER_HS_TABLE);
    }
    return sF;
}

/*----------------------------------------------------------------------------*/
/*                          ADDED BY SUSANNE                                  */
/* Function    : _ERM_rcWritePersistentPlayerHsProfile                        */
/**
    \brief      write the values stored in tmp to player / hs profile index in tmp

    \param      none

    \return     APP_COMMON_enResponseCode
   */
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode _ERM_rcWritePersistentPlayerHsProfile(void)
{
    CommonResponseCodeType    rC      = COMMON_RSP_OK;
    EEDeviceControlBlockType *pEEApp = getEEAppTaskControl();
    FlagType                 flag    = EEPROM_flagIsEEFree(pEEApp);

    if (ERM_MAX_NUM_PLAYER_HS_TABLE > _ERM_gContext.epr_tmp.u8PlayHSIdx)
    {
        if(flag == TRUE)
        {

            DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM,"WrPersPlHsProf: prof %d",
                         _ERM_gContext.epr_tmp.u8PlayHSIdx );

            rC = EEPROM_rcWrite( pEEApp,
                   (VWDB_EE_ADDDRESS_BASE_PLAYERHS_PROFILE_deviceIndex_0 +
                    _ERM_gContext.epr_tmp.u8PlayHSIdx *sizeof(ERMPlayerHeadsetProfileType)),
                    (Uint8Type*)&(_ERM_gContext.epr_tmp.playHs),
                    sizeof(ERMPlayerHeadsetProfileType),
                    _ERM_vEepromWriteFinished_Cb );

            if (COMMON_RSP_OK == rC)
            {
                return APP_COMMON_RSP_OK;
            }
        }
        else
        {
            return APP_COMMON_RSP_ERROR_BUSY;
        }
    }

    DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM,"WrPersPlHsProf: failed prof %d",
                  _ERM_gContext.epr_tmp.u8PlayHSIdx );

    return APP_COMMON_RSP_ERROR;
}

/*----------------------------------------------------------------------------*/
/* Function    : _ERM_vCopyTmpPlayerHsProfileToRam                            */
/**
    \brief      called when writing tmp rplayer / headset profile to
                eeprom was successful \n
                tmp player/headset profile will now be copied to ram at
                position of u8PlayHsIdx\n

    \param      none

    \return     SuccessFailType
   */
/*----------------------------------------------------------------------------*/
void _ERM_vCopyTmpPlayerHsProfileToRam(void)
{
    /* player hs profile table was changed */

    /* SUSANNE: 18.03.2010 As described in Wiki and Ticket #5045 do not read E2P,
       just copy from epr_tmp */

//    Uint8Type idx = _ERM_gContext.epr_tmp.u8PlayHSIdx;
//    if(_ERM_sfReadPersistentPlayerHsProfile(idx) == SUCCESS)
//    {
//        if(memcmp( &(_ERM_stPlayerHeadsetTable[idx]),&_ERM_gContext.epr_tmp.playHs,
//                   sizeof(ERMPlayerHeadsetProfileType)) != 0)
//        {

//        }
//    }
//    else
//    {

//    }


    Uint8Type idx = _ERM_gContext.epr_tmp.u8PlayHSIdx;


    DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM, "CopyTmpPlHsProfToRam idx: %d", idx);

    /* copy tmp user profile into ram at pos playHsIdx */
    if (ERM_MAX_NUM_PLAYER_HS_TABLE > idx)
    {
        /* profile table was changed */
        /* now copy whole tmp profile to ram */
        (void)memcpy(&(_ERM_stPlayerHeadsetTable[idx]),&_ERM_gContext.epr_tmp.playHs,
                 sizeof(ERMPlayerHeadsetProfileType));
    }
}


/*----------------------------------------------------------------------------*/
/* Function    : _ERM_sfReadPersistentPhysicalDevice                          */
/**
    \brief      read back one entry from persistent physical device list

    \param      none

    \return     SuccessFailType SUCCESS if read was successfull
                FAIL is problem occured, e.g. as EPROM was busy  if so caller
                shall retry
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ERM_sfReadPersistentPhysicalDevice(Uint8Type index)
{
    SuccessFailType             sF      = FAIL;
    CommonResponseCodeType      rsp     = COMMON_RSP_OK;
    EEDeviceControlBlockType *pEEApp    = getEEAppTaskControl();

    /* check for valid index */
    if (ERM_MAX_NUM_PHYSICAL_DEVICES > index)
    {
        rsp = EEPROM_sfRead( pEEApp,
                (EEAddressType)(VWDB_EE_ADDDRESS_BASE_PAIRED_DEVICE +
                    index * sizeof(ERMPhysicalBTDeviceType)),
                (Uint8Type *)&(_ERM_stPhyDevTable[index]),
                sizeof(ERMPhysicalBTDeviceType));

        switch(rsp)
        {
            case COMMON_RSP_OK:
                sF = SUCCESS;

                DEBUG_VALUE2(ERM_SWI_DBG, MOD_ERM,"phy device idx %d read %d",index,sF);
                break;
            case COMMON_RSP_ERROR_BUSY:
                // TODO!!!!!!!!!!!!!!!!

                DEBUG_VALUE1(ERM_SWI_ERR, MOD_ERM,"E2P Read BUSY: %d ERM_DeviceHandler.c",__LINE__);
                break;
            default:
                /* error reading */

                DEBUG_VALUE1(ERM_SWI_ERR, MOD_ERM,"E2P Read FAIL: %d ERM_DeviceHandler.c",__LINE__);
                break;
        }
    }
    else
    {

        DEBUG_VALUE2(ERM_SWI_TRC, MOD_ERM,"phy device idx oor %d > %d",
                index,ERM_MAX_NUM_PHYSICAL_DEVICES);
    }
    return sF;
}

/*----------------------------------------------------------------------------*/
/*                          ADDED BY SUSANNE                                  */
/* Function    : _ERM_rcWritePersistentPhysicalDevice                         */
/**
    \brief      write the values stored in tmp to physical device index in tmp

    \param      none

    \return     APP_COMMON_enResponseCode
   */
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode _ERM_rcWritePersistentPhysicalDevice(void)
{
    CommonResponseCodeType    rC      = COMMON_RSP_OK;
    EEDeviceControlBlockType *pEEApp = getEEAppTaskControl();
    FlagType                 flag    = EEPROM_flagIsEEFree(pEEApp);

    if (ERM_MAX_NUM_PHYSICAL_DEVICES > _ERM_gContext.epr_tmp.u8PhyIdx)
    {
        if(flag == TRUE)
        {

            DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM,"WrPersPhyDev: prof %d",
                         _ERM_gContext.epr_tmp.u8PhyIdx );

            rC = EEPROM_LAYOUT_rcWrite( pEEApp,
                   (VWDB_EE_ADDDRESS_BASE_PAIRED_DEVICE +
                    _ERM_gContext.epr_tmp.u8PhyIdx *sizeof(ERMPhysicalBTDeviceType)),
                    (Uint8Type*)&(_ERM_gContext.epr_tmp.phy),
                    sizeof(ERMPhysicalBTDeviceType),
                    _ERM_vEepromWriteFinished_Cb );
            if (COMMON_RSP_OK == rC)
            {
                return APP_COMMON_RSP_OK;
            }
        }
        else
        {
            return APP_COMMON_RSP_ERROR_BUSY;
        }
    }


    DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM,"WrPersPhyDev: failed prof %d",
                  _ERM_gContext.epr_tmp.u8PhyIdx );

    return APP_COMMON_RSP_ERROR;
}

/*----------------------------------------------------------------------------*/
/* Function    : _ERM_vCopyTmpPhyDeviceToRam                            */
/**
    \brief      called when writing tmp phy device to eeprom was successful \n
                tmp phy device will now be copied to ram at
                position of deviceId in epr_tmp\n

    \param      none

    \return     SuccessFailType
   */
/*----------------------------------------------------------------------------*/
void _ERM_vCopyTmpPhyDeviceToRam(void)
{
    /* SUSANNE: 18.03.2010 As described in Wiki and Ticket #5045 do not read E2P,
       just copy from epr_tmp */

//    Uint8Type idx = _ERM_gContext.epr_tmp.u8PhyIdx;
//
//    if(idx >= ARRAY_COUNT(_ERM_stPhyDevTable))
//    {

//        return;
//    }
//
//    if(idx >= ARRAY_COUNT(_ERM_stBTDeviceTable))
//    {

//        return;
//    }
//
//    if(_ERM_sfReadPersistentPhysicalDevice(idx) == SUCCESS)
//    {
//        if(memcmp( &(_ERM_stPhyDevTable[idx]),&_ERM_gContext.epr_tmp.phy,
//                   sizeof(ERMPhysicalBTDeviceType)) != 0)
//        {

//        }
//
//        /* copy device name to BTDeviceTable */
//        // check if device was deleted, if so we don't wanna set device name
//        if( _ERM_stBTDeviceTable[idx].device_state != ERM_DEVICE_STATE_USED )
//        {
//            return;
//        }
//        if( _ERM_stPhyDevTable[idx].name_len > AT_DEVICE_NAME_MAX_LEN )
//        {
//            return;
//        }
//
//        // !!! Do not use ERM_bSetDeviceName cause this will trigger ERM_NOTIFY_ACTIVE_DEVICE_NAME_CHANGED
//        // reset old values
//        (void)memset( _ERM_stBTDeviceTable[idx].device_name,
//                0,
//                sizeof(_ERM_stBTDeviceTable[idx].device_name));
//
//        _ERM_stBTDeviceTable[idx].device_name_len = _ERM_stPhyDevTable[idx].name_len;
//
//        // copy new name
//        (void)memcpy( _ERM_stBTDeviceTable[idx].device_name,
//                _ERM_stPhyDevTable[idx].name,
//                _ERM_stBTDeviceTable[idx].device_name_len);
//    }
//    else
//    {

//    }


    /* make some range checks */
    Uint8Type idx = _ERM_gContext.epr_tmp.u8PhyIdx;


    DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM, "CopyTmpPhyDevToRam idx: %d", idx);

    if(idx >= ARRAY_COUNT(_ERM_stPhyDevTable))
    {

        DEBUG_VALUE2(ERM_SWI_TRC, MOD_ERM, "CopyTmpPhyDevToRam: _ERM_stPhyDevTable ooR (%d,%d)",
                      idx,
                      ARRAY_COUNT(_ERM_stPhyDevTable) );
        return;
    }

    if(idx >= ARRAY_COUNT(_ERM_stBTDeviceTable))
    {

        DEBUG_VALUE2(ERM_SWI_TRC, MOD_ERM, "CopyTmpPhyDevToRam: _ERM_stBTDeviceTable ooR (%d,%d)",
                      idx,
                      ARRAY_COUNT(_ERM_stBTDeviceTable) );
        return;
    }


    /* phy table changed e.g. due to new name for the device */
    /* now copy whole new phy to ram */
    (void)memcpy( &(_ERM_stPhyDevTable[idx]),
            &_ERM_gContext.epr_tmp.phy,
            sizeof(ERMPhysicalBTDeviceType));


    /* copy device name to BTDeviceTable */
    // check if device was deleted, if so we don't wanna set device name
    if( _ERM_stBTDeviceTable[idx].device_state != ERM_DEVICE_STATE_USED )
    {
        return;
    }
    if( _ERM_stPhyDevTable[idx].name_len > AT_DEVICE_NAME_MAX_LEN )
    {
        return;
    }

    // !!! Do not use ERM_bSetDeviceName cause this will trigger ERM_NOTIFY_ACTIVE_DEVICE_NAME_CHANGED
    // reset old values
    (void)memset( _ERM_stBTDeviceTable[idx].device_name,
            0,
            sizeof(_ERM_stBTDeviceTable[idx].device_name));

    _ERM_stBTDeviceTable[idx].device_name_len = _ERM_stPhyDevTable[idx].name_len;

    // copy new name
    (void)memcpy( _ERM_stBTDeviceTable[idx].device_name,
            _ERM_stPhyDevTable[idx].name,
            _ERM_stBTDeviceTable[idx].device_name_len);

}




/*----------------------------------------------------------------------------*/
/* Function    : _ERM_sfReadPersistentActiveProfiles                          */
/**
    \brief      read back complete active profile lists

    \param      none

    \return     SuccessFailType SUCCESS if read was successfull
                FAIL is problem occured, e.g. as EPROM was busy  if so caller
                shall retry
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ERM_sfReadPersistentActiveProfiles(void)
{
    SuccessFailType             sF      = FAIL;
    CommonResponseCodeType      rsp     = COMMON_RSP_OK;
    EEDeviceControlBlockType *pEEApp    = getEEAppTaskControl();

    rsp = EEPROM_sfRead( pEEApp,
            (EEAddressType)(VWDB_EE_ADDDRESS_LASTACTIVEPROFILELISTS),
            (Uint8Type *)&(_ERM_stActiveProfileList),
            sizeof(ERMActiveProfileListType));

    switch(rsp)
    {
        case COMMON_RSP_OK:
            sF = SUCCESS;

            DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM,"RdPersActProf ok");
            break;
        case COMMON_RSP_ERROR_BUSY:
            // TODO!!!!!!!!!!!!!

            DEBUG_VALUE1(ERM_SWI_ERR, MOD_ERM,"E2P Read BUSY: %d ERM_DeviceHandler.c",__LINE__);
            break;
        default:
            /* error reading e2p */

            DEBUG_VALUE1(ERM_SWI_ERR, MOD_ERM,"E2P Read FAIL: %d ERM_DeviceHandler.c",__LINE__);
            break;
    }
    return sF;
}



void _ERM_vCopyTmpActProfileListToRam(void)
{

    /* SUSANNE: 18.03.2010 As described in Wiki and Ticket #5045 do not read E2P,
       just copy from epr_tmp */

//    if(_ERM_sfReadPersistentActiveProfiles() == SUCCESS)
//    {
//        if(memcmp( &(_ERM_stActiveProfileList),&_ERM_gContext.epr_tmp.actList,
//                   sizeof(ERMActiveProfileListType)) != 0)
//        {

//        }
//    }
//    else
//    {

//    }


    DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM, "CopyTmpActProfileListToRam");

    /* copy tmp active profile tables into ram at pos u8ActIdx */
    /* now copy whole new phy to ram */
    (void)memcpy(&_ERM_stActiveProfileList,&_ERM_gContext.epr_tmp.actList,
                        sizeof(ERMActiveProfileListType));
}

/*----------------------------------------------------------------------------*/
/*                          ADDED BY SUSANNE                                  */
/* Function    : _ERM_rcWritePersistentPhysicalDevice                         */
/**
    \brief      write the values stored in tmp to physical device index in tmp

    \param      none

    \return     APP_COMMON_enResponseCode
   */
/*----------------------------------------------------------------------------*/
APP_COMMON_enResponseCode _ERM_rcWritePersistentActiveProfileList(void)
{
    CommonResponseCodeType    rC      = COMMON_RSP_OK;
    EEDeviceControlBlockType *pEEApp = getEEAppTaskControl();
    FlagType                 flag    = EEPROM_flagIsEEFree(pEEApp);

    if(flag == TRUE)
    {

        DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM, "WrPersActProfList Trying");

        /* check which active profile list has been changed */
        /* for each which has not been changed, copy current ram content into epr buffer */

        /* !!!!!! THIS IS IMPORTANT CAUSE THE ACTIVE PROFILE LIST IS WRITTEN TO EEPROM
         * COMPLETLY AND WE DON'T WANT TO WRITE UNINITIALED STUFF FOR UNCHANGED LISTS */
        if( BT_FALSE == _ERM_gContext.epr_tmp.bActUserProfileListChanged )
        {   /* copy ram content of active user profile list to tmp list */
            (void)memcpy( &_ERM_gContext.epr_tmp.actList.UserProfileList,
                    &_ERM_stActiveProfileList.UserProfileList,
                    sizeof(_ERM_gContext.epr_tmp.actList.UserProfileList));
            DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM, "WrPersActProfList UserProfileListChanged");
        }
        if( BT_FALSE == _ERM_gContext.epr_tmp.bActPlayerProfileListChanged )
        {   /* copy ram content of active player profile list to tmp list */
            (void)memcpy( &_ERM_gContext.epr_tmp.actList.PlayerProfileList,
                    &_ERM_stActiveProfileList.PlayerProfileList,
                    sizeof(_ERM_gContext.epr_tmp.actList.PlayerProfileList));
            DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM, "WrPersActProfList PlayerProfileList");
        }
        if( BT_FALSE == _ERM_gContext.epr_tmp.bActHeadsetProfileListChanged )
        {   /* copy ram content of active headset profile list to tmp list */
            (void)memcpy( &_ERM_gContext.epr_tmp.actList.HeadsetProfileList,
                    &_ERM_stActiveProfileList.HeadsetProfileList,
                    sizeof(_ERM_gContext.epr_tmp.actList.HeadsetProfileList));
            DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM, "WrPersActProfList HeadsetProfileList");
        }

        rC = EEPROM_LAYOUT_rcWrite( pEEApp,
                VWDB_EE_ADDDRESS_LASTACTIVEPROFILELISTS,
                (Uint8Type*)&(_ERM_gContext.epr_tmp.actList),
                sizeof(ERMActiveProfileListType),
                _ERM_vEepromWriteFinished_Cb );
        if (COMMON_RSP_OK == rC)
        {
            return APP_COMMON_RSP_OK;
        }
    }
    else
    {
        return APP_COMMON_RSP_ERROR_BUSY;
    }



    DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM,  "WrPersActProfList cant write");
    return APP_COMMON_RSP_ERROR;
}


/*----------------------------------------------------------------------------*/
/* Function    : _ERM_sfReadPersistentUserList                                */
/**
    \brief

    \param      none

    \return     none
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ERM_sfReadPersistentUserList(void)
{
    SuccessFailType             sF      = FAIL;
    CommonResponseCodeType      rsp     = COMMON_RSP_OK;
    EEDeviceControlBlockType *pEEApp    = getEEAppTaskControl();

    rsp = EEPROM_sfRead( pEEApp,
                        (EEAddressType)VWDB_EE_ADDDRESS_MOBILEUSERLIST,
                        &_ERM_gContext.pu8ActiveUserList[0],
                        sizeof(_ERM_gContext.pu8ActiveUserList));
    switch(rsp)
    {
        case COMMON_RSP_OK:
            sF = SUCCESS;

            DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM,"RdPersUserList ok");
            break;
        case COMMON_RSP_ERROR_BUSY:
            // TODO!!!!!!!!!!!!!

            DEBUG_VALUE1(ERM_SWI_ERR, MOD_ERM,"E2P Read BUSY: %d ERM_DeviceHandler.c",__LINE__);
            break;
        default:
            /* error reading e2p */

            DEBUG_VALUE1(ERM_SWI_ERR, MOD_ERM,"E2P Read FAIL: %d ERM_DeviceHandler.c",__LINE__);
            break;
    }
    return sF;
}


/*----------------------------------------------------------------------------*/
/* Function    : _ERM_sfWritePersistentUserList                               */
/**
    \brief

    \param      none

    \return     none
   */
/*----------------------------------------------------------------------------*/
//SuccessFailType _ERM_sfWritePersistentUserList()
//{
//    SuccessFailType          sF      = SUCCESS;
//    CommonResponseCodeType    rC      = COMMON_RSP_OK;
//    EEDeviceControlBlockType *pEEApp = getEEAppTaskControl();
//    FlagType                 flag    = EEPROM_flagIsEEFree(pEEApp);
//
//
//    if(flag == TRUE)
//    {
//        rC = EEPROM_LAYOUT_rcWrite( pEEApp,
//                             VWDB_EE_ADDDRESS_MOBILEUSERLIST,
//                             &_ERM_gContext.pu8ActiveUserList[0],
//                             sizeof(_ERM_gContext.pu8ActiveUserList),
//                             NULL );
//
//        if (COMMON_RSP_OK != rC)
//        {
//            sF = FAIL;
//        }
//
//        _ERM_gContext.u8UserListDirty = FALSE;
//    }
//    else
//    {
//        /* remember to write the user list later */
//        _ERM_gContext.u8UserListDirty = TRUE;
//        sF = FAIL;
//    }
//
//    return sF;
//}


///*----------------------------------------------------------------------------*/
///* Function    : ERM_rcDeletePersistentUserTable                           */
///**
//    \brief
//
//    \param      none
//
//    \return     SuccessFailType
//   */
///*----------------------------------------------------------------------------*/
//APP_COMMON_enResponseCode ERM_rcDeletePersistentUserTable( Uint8Type index,
//                                                           void (*cb)(APP_COMMON_enResponseCode) )
//{
//    /* we wanna delete the given user profile table index */
//    /* first check if index is valid */
//    if( ERM_MAX_NUM_USER_PROFILES <= index )
//    {   /* not valid */
//        return APP_COMMON_RSP_ERROR;
//    }
//
//    if(_ERM_gContext.ePendingEpromMsg != ERM_REQ_NONE)
//    {
//        return APP_COMMON_RSP_ERROR_BUSY;
//    }
//
//    _ERM_gContext.ePendingEpromMsg          = ERM_REQ_DELETE_USER_PROFILE;
//    _ERM_gContext.cb_epromWriteFinished     = cb;
//    _ERM_gContext.epr_tmp.u8UserIdx         = index;
//    _ERM_gContext.epr_tmp.bUpdateUserNeeded = BT_TRUE;
//
//    (void)memset( &_ERM_gContext.epr_tmp.user, 0xFF, sizeof( ERMUserProfileType ));
//
//    /* not needed, make sure they are resetted */
//    _ERM_gContext.epr_tmp.deviceId              = AT_INVALID_DEVICE_ID;
//    _ERM_gContext.epr_tmp.bUpdatePlayHSNeeded   = BT_FALSE;
//    _ERM_gContext.epr_tmp.bUpdatePhyNeeded      = BT_FALSE;
//    _ERM_gContext.epr_tmp.u8PlayHSIdx           = AT_INVALID_DEVICE_ID;
//    _ERM_gContext.epr_tmp.u8PhyIdx              = AT_INVALID_DEVICE_ID;
//
//    _ERM_gContext.epr_tmp.state                 = ERM_EPR_WRITE_STATE_WRITE_PENDING;
//
//    /* now trigger write to EPROM to given index  */
//    (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_ERM_PENDING, MSEC(0), MSEC(0));
//
//
//    return APP_COMMON_RSP_OK;
//}
//
//
///*----------------------------------------------------------------------------*/
///* Function    : _ERM_sfDeletePersistentPhyTable                              */
///**
//    \brief
//
//    \param      none
//
//    \return     SuccessFailType
//   */
///*----------------------------------------------------------------------------*/
//APP_COMMON_enResponseCode ERM_rcDeletePersistentPhyTable( Uint8Type index,
//                                                          void (*cb)(APP_COMMON_enResponseCode) )
//{
//    if( ERM_MAX_NUM_PHYSICAL_DEVICES <= index )
//    {   /* not valid */
//        return APP_COMMON_RSP_ERROR;
//    }
//
//    if(_ERM_gContext.ePendingEpromMsg != ERM_REQ_NONE)
//    {
//        return APP_COMMON_RSP_ERROR_BUSY;
//    }
//
//    _ERM_gContext.ePendingEpromMsg          = ERM_REQ_DELETE_USER_PROFILE;
//    _ERM_gContext.cb_epromWriteFinished     = cb;
//    _ERM_gContext.epr_tmp.u8PhyIdx          = index;
//    _ERM_gContext.epr_tmp.bUpdatePhyNeeded  = BT_TRUE;
//
//    (void)memset( &_ERM_gContext.epr_tmp.user, 0xFF, sizeof( ERMUserProfileType ));
//
//    /* not needed, make sure they are resetted */
//    _ERM_gContext.epr_tmp.deviceId              = AT_INVALID_DEVICE_ID;
//    _ERM_gContext.epr_tmp.bUpdatePlayHSNeeded   = BT_FALSE;
//    _ERM_gContext.epr_tmp.bUpdateUserNeeded     = BT_FALSE;
//    _ERM_gContext.epr_tmp.u8PlayHSIdx           = AT_INVALID_DEVICE_ID;
//    _ERM_gContext.epr_tmp.u8UserIdx             = AT_INVALID_DEVICE_ID;
//
//    _ERM_gContext.epr_tmp.state                 = ERM_EPR_WRITE_STATE_WRITE_PENDING;
//
//    /* now trigger write to EPROM to given index  */
//    (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_ERM_PENDING, MSEC(0), MSEC(0));
//
//    return APP_COMMON_RSP_OK;
//}
//
///*----------------------------------------------------------------------------*/
///* Function    : ERM_rcDeletePersistentPlayerHSTable                          */
///**
//    \brief
//
//    \param      none
//
//    \return     SuccessFailType
//   */
///*----------------------------------------------------------------------------*/
//APP_COMMON_enResponseCode ERM_rcDeletePersistentPlayerHSTable( Uint8Type index,
//                                                          void (*cb)(APP_COMMON_enResponseCode) )
//{
//    if( ERM_MAX_NUM_PHYSICAL_DEVICES <= index )
//    {   /* not valid */
//        return APP_COMMON_RSP_ERROR;
//    }
//
//    if(_ERM_gContext.ePendingEpromMsg != ERM_REQ_NONE)
//    {
//        return APP_COMMON_RSP_ERROR_BUSY;
//    }
//
//    _ERM_gContext.ePendingEpromMsg              = ERM_REQ_DELETE_USER_PROFILE;
//    _ERM_gContext.cb_epromWriteFinished         = cb;
//    _ERM_gContext.epr_tmp.u8PlayHSIdx           = index;
//    _ERM_gContext.epr_tmp.bUpdatePlayHSNeeded   = BT_TRUE;
//
//    (void)memset( &_ERM_gContext.epr_tmp.user, 0xFF, sizeof( ERMUserProfileType ));
//
//    /* not needed, make sure they are resetted */
//    _ERM_gContext.epr_tmp.deviceId              = AT_INVALID_DEVICE_ID;
//    _ERM_gContext.epr_tmp.bUpdatePhyNeeded      = BT_FALSE;
//    _ERM_gContext.epr_tmp.bUpdateUserNeeded     = BT_FALSE;
//    _ERM_gContext.epr_tmp.u8PhyIdx              = AT_INVALID_DEVICE_ID;
//    _ERM_gContext.epr_tmp.u8UserIdx             = AT_INVALID_DEVICE_ID;
//
//    _ERM_gContext.epr_tmp.state                 = ERM_EPR_WRITE_STATE_WRITE_PENDING;
//
//    /* now trigger write to EPROM to given index  */
//    (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_ERM_PENDING, MSEC(0), MSEC(0));
//
//    return APP_COMMON_RSP_OK;
//
//}
//
///*----------------------------------------------------------------------------*/
///* Function    : _ERM_rcDeletePersistentActiveProfileUser                     */
///**
//    \brief
//
//    \param      none
//
//    \return     SuccessFailType
//   */
///*----------------------------------------------------------------------------*/
//APP_COMMON_enResponseCode ERM_rcDeletePersistentActiveProfileUser( Uint8Type index,
//                                                           void (*cb)(APP_COMMON_enResponseCode) )
//{
//
//
//    return APP_COMMON_RSP_OK;
//}
//
///*----------------------------------------------------------------------------*/
///* Function    : _ERM_rcDeletePersistentActiveProfilePlayer                   */
///**
//    \brief
//
//    \param      none
//
//    \return     SuccessFailType
//   */
///*----------------------------------------------------------------------------*/
//APP_COMMON_enResponseCode _ERM_rcDeletePersistentActiveProfilePlayer( Uint8Type index,
//                                                           void (*cb)(APP_COMMON_enResponseCode) )
//{
//
//
//    return APP_COMMON_RSP_OK;
//}
//
///*----------------------------------------------------------------------------*/
///* Function    : _ERM_rcDeletePersistentActiveProfileHeadset                  */
///**
//    \brief
//
//    \param      none
//
//    \return     SuccessFailType
//   */
///*----------------------------------------------------------------------------*/
//APP_COMMON_enResponseCode _ERM_rcDeletePersistentActiveProfileHeadset( Uint8Type index,
//                                                           void (*cb)(APP_COMMON_enResponseCode) )
//{
//
//
//    return APP_COMMON_RSP_OK;
//}


/*----------------------------------------------------------------------------*/
/* Function    : _ERM_sfUserListWasInitialized                                */
/**
    \brief

    \param      none

    \return     none
   */
/*----------------------------------------------------------------------------*/
BooleanType _ERM_bUserListWasInitialized(void)
{
    Uint32Type i = 0;
    Uint8Type  deviceIds[AT_MAX_DEVICE_ID+1];

    (void)memset(deviceIds, 0x00, sizeof(deviceIds));

    for( i=0; i<AT_MAX_DEVICE_ID; i++)
    {
        switch(_ERM_gContext.pu8ActiveUserList[i])
        {
            case 0x00:
            case 0x01:
            case 0x02:
            case 0x03:
            case 0x04:
                deviceIds[_ERM_gContext.pu8ActiveUserList[i]]++;

                /* each device id can appear only once */
                if( deviceIds[_ERM_gContext.pu8ActiveUserList[i]] > 1 )
                {
                    return BT_FALSE;
                }
                break;

            case AT_INVALID_DEVICE_ID:
                /* this value can appear multiple times */
                break;

            default:
                /* no other value is acceptable */
                return BT_FALSE;
         }
    }
    return BT_TRUE;
}

/*----------------------------------------------------------------------------*/
/* Function    : _ERM_bSetDeviceName                                          */
/**
    \brief

    \param      none

    \return     none
   */
/*----------------------------------------------------------------------------*/
BooleanType ERM_bSetDeviceName(AT_DeviceId deviceId, const Uint8Type *pName, Uint16Type length)
{
    AT_DeviceId activeDevice = 0;
    if( deviceId > AT_MAX_DEVICE_ID )
    {
        return BT_FALSE;
    }

    if( _ERM_stBTDeviceTable[deviceId].device_state != ERM_DEVICE_STATE_USED )
    {
        return BT_FALSE;
    }

    if( length > AT_DEVICE_NAME_MAX_LEN )
    {
        return BT_FALSE;
    }

    (void)memset( _ERM_stBTDeviceTable[deviceId].device_name,
            0,
            sizeof(_ERM_stBTDeviceTable[deviceId].device_name));

    _ERM_stBTDeviceTable[deviceId].device_name_len = length;

    (void)memcpy( _ERM_stBTDeviceTable[deviceId].device_name,
            pName,
            _ERM_stBTDeviceTable[deviceId].device_name_len);

    ;

    if( (ERM_GetActiveBTDeviceId(&activeDevice) == SUCCESS) &&
        (activeDevice == deviceId) )
    {
        ERMNotificationType notification;
        notification.common.id = ERM_NOTIFY_ACTIVE_DEVICE_NAME_CHANGED;
        _ERM_vNotifyApplication(&notification);
    }

    return BT_TRUE;
}

Uint8Type ERM_GetLastActiveProfileId( Uint8Type activePos,
                                      APP_DEVICE_DeviceType profileType)
{
    Uint16Type  maxProfileNum = 0;
    AT_DeviceId *pProfileList = NULL;
    Uint8Type profileId = AT_INVALID_DEVICE_ID;

    switch( profileType )
    {
        case APP_DEVICE_enTYPE_PHONE:
            pProfileList    = _ERM_stActiveProfileList.UserProfileList;
            maxProfileNum   =  ERM_MAX_NUM_USER_PROFILES;
            break;
        case APP_DEVICE_enTYPE_PLAYER:
            pProfileList    = _ERM_stActiveProfileList.PlayerProfileList;
            maxProfileNum   =  ERM_MAX_NUM_PLAYER_PROFILE;
            break;
        case APP_DEVICE_enTYPE_HEADSET:
            pProfileList    = _ERM_stActiveProfileList.HeadsetProfileList;
            maxProfileNum   =  ERM_MAX_NUM_HEADSET_PROFILE;
            break;
        default:
            return AT_INVALID_DEVICE_ID;
    }

    if(activePos < maxProfileNum)
    {
        profileId = pProfileList[activePos];
    }


    return profileId;
}

BooleanType ERM_bPlayerIdIsInphoneList(Uint8Type deviceId)
{
    BooleanType bPlayerIdIsInPhoneList = BT_FALSE;
    Uint8Type counter = 0;


    for (counter=0; counter < ERM_MAX_NUM_USER_PROFILES; counter++)
    {
        Uint8Type profileId = ERM_GetLastActiveProfileId( counter,
                                                      APP_DEVICE_enTYPE_PHONE);
        const ERMUserProfileType *pUserProfile = ERM_pGetUserProfile(profileId);
        
        if (NULL == pUserProfile)
        {
            DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM,"ERM_bPlayerIdIsInphgoneList: pUserProfile empty ");
            break;
        }
        
        DEBUG_VALUE2(ERM_SWI_TRC, MOD_ERM,"ERM_bPlayerIdIsInphgoneList: pUserProfile->deviceIndex %d , deviceId %d",
             pUserProfile->deviceIndex , deviceId);

        if (pUserProfile->deviceIndex == deviceId)
        {
            bPlayerIdIsInPhoneList = BT_TRUE;
            break;
        }
    }

    return bPlayerIdIsInPhoneList;
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_bMaxVoiceTagsForUserRecorder                             */
/**
    \brief      the function check if for the active user profile the number
                of recorded voice tags has already reached maximum

    \param      none

    \return     BooleanType BT_TRUE is maximum number is already recorded
                            BT_FALSE is more tags can be recorded
   */
/*----------------------------------------------------------------------------*/

BooleanType ERM_bMaxVoiceTagsForUserRecorder(void)
{
    const ERMUserProfileType * profile = ERM_pGetActiveUserProfile();
    Uint8Type   i = 0;
    Uint8Type   numVoiceTagsOverall    = 0;

    if ( NULL == profile)
    {

        DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM,"bMaxVoiceTagsForUserRecorder no act user profile");
        return BT_TRUE;
    }

    for (i=0; i < ERM_MAX_NUM_PHYSICAL_DEVICES ;i++)
    {
        if ( ERM_MAX_NUM_PHYSICAL_DEVICES > _ERM_stPhyDevTable[i].deviceID )
        {
            numVoiceTagsOverall = numVoiceTagsOverall + _ERM_stPhyDevTable[i].numVoiceTags;
        }
    }


    if (    (ERM_MAX_NUM_VOICE_TAGS_PER_USER > _ERM_stPhyDevTable[profile->deviceIndex].numVoiceTags)
         && (ERM_MAX_NUM_VOICE_TAGS_OVERALL  > numVoiceTagsOverall)  )
    {

        DEBUG_VALUE4(ERM_SWI_TRC, MOD_ERM,"bMaxVcTagsForUsrRec possible User %d %d Overall %d %d",
                 _ERM_stPhyDevTable[profile->deviceIndex].numVoiceTags,
                 ERM_MAX_NUM_VOICE_TAGS_PER_USER,
                 numVoiceTagsOverall,
                 ERM_MAX_NUM_VOICE_TAGS_OVERALL);

        return BT_FALSE;
    }
    else
    {

        DEBUG_VALUE4(ERM_SWI_TRC, MOD_ERM,"bMaxVcTagsForUsrRec not possible User %d %d Overall %d %d",
                 _ERM_stPhyDevTable[profile->deviceIndex].numVoiceTags,
                 ERM_MAX_NUM_VOICE_TAGS_PER_USER,
                 numVoiceTagsOverall,
                 ERM_MAX_NUM_VOICE_TAGS_OVERALL);


        return BT_TRUE;
    }

}


/*----------------------------------------------------------------------------*/
/* Function    : ERM_pGetDevice                                               */
/**
    \brief      returns pointer to given index element of ERM BT Device table

    \brief      Description:\n
                returns pointer to given index element of ERM BT Device table

    \param      void

    \return     const ERMBTDeviceTableType *

*/
/*----------------------------------------------------------------------------*/
const ERMBTDeviceTableType        *ERM_pGetDevice(Uint8Type index)
{
    if (AT_MAX_DEVICE_ID >= index)
    {
        return &_ERM_stBTDeviceTable[index];
    }
    else
    {
        return NULL;
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_pGetPhyDevice                                            */
/**
    \brief      returns pointer to given index element of ERM physical Device table

    \brief      Description:\n
                returns pointer to given index element of ERM physical Device table

    \param      void

    \return     const ERMPhysicalBTDeviceType *

*/
/*----------------------------------------------------------------------------*/
const ERMPhysicalBTDeviceType     *ERM_pGetPhyDevice(Uint8Type index)
{
    if (ERM_MAX_NUM_PHYSICAL_DEVICES > index)
    {
        return &_ERM_stPhyDevTable[index];
    }
    else
    {
        return NULL;
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_pGetUserProfile                                          */
/**
    \brief      returns pointer to given index element of ERM User profile table

    \brief      Description:\n
                returns pointer to given index element of ERM User profile  table

    \param      void

    \return     const ERMUserProfileType *

*/
/*----------------------------------------------------------------------------*/
const ERMUserProfileType          *ERM_pGetUserProfile(Uint8Type index)
{
    if (ERM_MAX_NUM_USER_PROFILES > index)
    {
        return &_ERM_stUserTable[index];
    }
    else
    {
        return NULL;
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_pGetUserProfile_DevId                                    */
/**
    \brief      returns pointer to given device index element of ERM
                user profile table

    \brief      Description:\n
                returns pointer to given device index element of ERM player
                profile table

    \param      void

    \return     const ERMPlayerHeadsetProfileType *

*/
/*----------------------------------------------------------------------------*/
const ERMUserProfileType *ERM_pGetUserProfile_DevId(AT_DeviceId deviceId)
{
    Uint16Type i = 0;

    if( deviceId == AT_INVALID_DEVICE_ID )
    {
        return NULL;
    }

    for(i=0;i<ERM_MAX_NUM_USER_PROFILES;i++)
    {
        if(_ERM_stUserTable[i].deviceIndex == deviceId)
        {
            return &_ERM_stUserTable[i];
        }
    }
    return NULL;
}


/*----------------------------------------------------------------------------*/
/* Function    : ERM_pGetUserProfileForEdit                                   */
/**
    \brief      returns pointer to given index element of ERM User profile table

    \brief      Description:\n
                returns pointer to given index element of ERM User profile  table
                FOR ERM internal use only !!!!!!!!!

    \param      void

    \return     ERMUserProfileType *

*/
/*----------------------------------------------------------------------------*/
ERMUserProfileType *_ERM_pGetUserProfileForEdit(Uint8Type index)
{
    if (ERM_MAX_NUM_USER_PROFILES > index)
    {
        return &_ERM_stUserTable[index];
    }
    else
    {
        return NULL;
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_pGetActivePlayerProfile                                          */
/**
    \brief      returns pointer to ERM player profile table of active player

    \brief      Description:\n
                returns pointer to ERM player profile table of active player

    \param      void

    \return     const ERMPlayerHeadsetProfileType *
*/
/*----------------------------------------------------------------------------*/
const ERMPlayerHeadsetProfileType *ERM_pGetActivePlayerProfile(void)
{
    return ERM_pGetPlayerHeadsetProfile_DevId(ERM_u8GetActivePlayer());
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_pGetActiveHeadsetProfile                                          */
/**
    \brief      returns pointer to ERM headset profile table of active headset

    \brief      Description:\n
                returns pointer to ERM headset profile table of active headset

    \param      void

    \return     const ERMPlayerHeadsetProfileType *
*/
/*----------------------------------------------------------------------------*/
const ERMPlayerHeadsetProfileType *ERM_pGetActiveHeadsetProfile(void)
{
    return ERM_pGetPlayerHeadsetProfile_DevId(ERM_u8GetActiveHeadSet());
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_pGetActiveUserProfile                                          */
/**
    \brief      returns pointer to ERM User profile table of active user

    \brief      Description:\n
                returns pointer to ERM User profile table of active user

    \param      void

    \return     const ERMUserProfileType *

*/
/*----------------------------------------------------------------------------*/
const ERMUserProfileType *ERM_pGetActiveUserProfile(void)
{
    return ERM_pGetUserProfile_DevId(ERM_u8GetActivePhone());
}

/*----------------------------------------------------------------------------*/
/* Function    : _ERM_u8GetUserProfileIndexForDeviceId                            */
/**
    \brief      returns index of user profile table for given deviceId

    \brief      Description:\n
                returns index of user profile table for given deviceId

    \param      deviceId
                AT_DeviceId

    \return     Uint8Type

*/
/*----------------------------------------------------------------------------*/
Uint8Type ERM_u8GetUserProfileIndexForDeviceId(AT_DeviceId deviceId)
{
    Uint8Type i;

    if( deviceId > AT_MAX_DEVICE_ID )
    {
        return AT_INVALID_DEVICE_ID;
    }

    for(i=0;i<ERM_MAX_NUM_USER_PROFILES;i++)
    {
        if(_ERM_stUserTable[i].deviceIndex == deviceId)
        {
            return i;
        }
    }
    return AT_INVALID_DEVICE_ID;
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_pGetPlayerHeadsetProfile                                 */
/**
    \brief      returns pointer to given index element of ERM player/headset profile table

    \brief      Description:\n
                returns pointer to given index element of ERM player profile  table

    \param      void

    \return     const ERMPlayerHeadsetProfileType *

*/
/*----------------------------------------------------------------------------*/
const ERMPlayerHeadsetProfileType *ERM_pGetPlayerHeadsetProfile(Uint8Type index)
{
    if (ERM_MAX_NUM_PLAYER_HS_TABLE > index)
    {
        return &_ERM_stPlayerHeadsetTable[index];
    }
    else
    {
        return NULL;
    }
}


/*----------------------------------------------------------------------------*/
/* Function    : ERM_pGetPlayerHeadsetProfile_DevId                           */
/**
    \brief      returns pointer to given device index element of ERM
                player/headset profile table

    \brief      Description:\n
                returns pointer to given device index element of ERM player
                profile table

    \param      void

    \return     const ERMPlayerHeadsetProfileType *

*/
/*----------------------------------------------------------------------------*/
const ERMPlayerHeadsetProfileType *ERM_pGetPlayerHeadsetProfile_DevId(AT_DeviceId deviceId)
{
    Uint16Type i = 0;

    if( deviceId == AT_INVALID_DEVICE_ID )
    {
        return NULL;
    }

    for(i=0;i<ERM_MAX_NUM_PLAYER_HS_TABLE;i++)
    {
        if(_ERM_stPlayerHeadsetTable[i].deviceIndex == deviceId)
        {
            return &_ERM_stPlayerHeadsetTable[i];
        }
    }
    return NULL;

}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_u8GetPlayerHeadsetProfileIndexForDeviceId                 */
/**
    \brief      returns index of player/headset profile table for given deviceId

    \brief      Description:\n
                returns index of player/headset profile table for given deviceId

    \param      deviceId
                AT_DeviceId

    \return     Uint8Type
*/
/*----------------------------------------------------------------------------*/
Uint8Type ERM_u8GetPlayerHeadsetProfileIndexForDeviceId(AT_DeviceId deviceId)
{
    Uint8Type i;

    if( deviceId > AT_MAX_DEVICE_ID )
    {
        return AT_INVALID_DEVICE_ID;
    }

    for(i=0;i<ERM_MAX_NUM_PLAYER_HS_TABLE;i++)
    {
        if(_ERM_stPlayerHeadsetTable[i].deviceIndex == deviceId)
        {
            return i;
        }
    }
    return AT_INVALID_DEVICE_ID;
}


/*----------------------------------------------------------------------------*/
/* Function    : ERM_bMaxNumberOfUserProfilesReached                                       */
/**
    \brief      returns whether max number of user profiles are reached

    \brief      Description:\n
                returns whether max number of user profiles are reached

    \param      void

    \return     BooleanType

*/
/*----------------------------------------------------------------------------*/
BooleanType ERM_bMaxNumberOfUserProfilesReached(void)
{
    if( ERM_MAX_NUM_USER_PROFILES == ERM_u16GetNumberOfUserProfiles())
    {
        return BT_TRUE;
    }
    return BT_FALSE;
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_bMaxNumberOfPlayerHeadsetProfilesReached                                       */
/**
    \brief      returns whether max number of player / headset profiles are reached

    \brief      Description:\n
                returns whether max number of player / headset profiles are reached

    \param      void

    \return     BooleanType

*/
/*----------------------------------------------------------------------------*/
BooleanType ERM_bMaxNumberOfPlayerHeadsetProfilesReached(void)
{
    Uint16Type numProfiles = ERM_u16GetNumberOfPlayerProfiles() +
                             ERM_u16GetNumberOfHeadsetProfiles();

    if( ERM_MAX_NUM_PLAYER_HS_TABLE == numProfiles )
    {
        return BT_TRUE;
    }
    return BT_FALSE;
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_bMaxNumberOfPlayerProfilesReached                                       */
/**
    \brief      returns whether max number of player profiles are reached

    \brief      Description:\n
                returns whether max number of player profiles are reached

    \param      void

    \return     BooleanType
*/
/*----------------------------------------------------------------------------*/
BooleanType ERM_bMaxNumberOfPlayerProfilesReached(void)
{
    Uint16Type numProfiles = ERM_u16GetNumberOfPlayerProfiles();

    if( ERM_MAX_NUM_PLAYER_PROFILE == numProfiles )
    {
        return BT_TRUE;
    }
    return BT_FALSE;
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_bMaxNumberOfHeadsetProfilesReached                                       */
/**
    \brief      returns whether max number of headset profiles are reached

    \brief      Description:\n
                returns whether max number of headset profiles are reached

    \param      void

    \return     BooleanType

*/
/*----------------------------------------------------------------------------*/
BooleanType ERM_bMaxNumberOfHeadsetProfilesReached(void)
{
    Uint16Type numProfiles = ERM_u16GetNumberOfHeadsetProfiles();

    if( ERM_MAX_NUM_HEADSET_PROFILE == numProfiles )
    {
        return BT_TRUE;
    }
    return BT_FALSE;
}




/*----------------------------------------------------------------------------*/
/* Function    : ERM_bDeviceInUserProfiles                                    */
/**
    \brief      returns whether given deviceId is used in UserProfile table

    \brief      Description:\n
                returns whether given deviceId is used in UserProfile table

    \param      void

    \return     BooleanType
*/
/*----------------------------------------------------------------------------*/
BooleanType ERM_bDeviceInUserProfiles(AT_DeviceId deviceId)
{
    Uint16Type i;
    const ERMUserProfileType * profile  = NULL;
    for(i=0; i<ERM_MAX_NUM_USER_PROFILES; i++)
    {
        profile = ERM_pGetUserProfile(i);
        if( NULL != profile && AT_INVALID_DEVICE_ID != profile->deviceIndex)
        {
            /* check if this is the deviceId searched for */
            if( deviceId == profile->deviceIndex )
            {
                return BT_TRUE;
            }
        }
    }
    return BT_FALSE;
}

#if 0
SuccessFailType _ERM_sfInitUserProfile(AT_DeviceId deviceId, const Uint8Type *pName, Uint16Type length)
{
    /* get first free user profile slot */
    Uint8Type freeIndex = _ERM_u8GetFreeUserProfileSlot();
    if( ERM_MAX_USER_PROFILE_IDX < freeIndex)
    {
        return FAIL;
    }

    /* add deviceId into UserTable */
    _ERM_stUserTable[freeIndex].deviceIndex = deviceId;
    ERM_bSetDeviceName(deviceId, pName, length);

    _ERM_stUserTable[freeIndex].voicemailbox_len = 0;

    // TODO: ggf init rest of struct
    return SUCCESS;
}
#endif

Uint8Type _ERM_u8GetFreeUserProfileSlot()
{
    Uint8Type i = 0;
    for(i=0;i<ERM_MAX_NUM_USER_PROFILES;i++)
    {
        if( AT_INVALID_DEVICE_ID == _ERM_stUserTable[i].deviceIndex )
        {
            return i;
        }
    }
    return AT_INVALID_DEVICE_ID;
}

Uint8Type _ERM_u8GetFreePlayerHeadsetProfileSlot(void)
{
    Uint8Type i = 0;
    for(i=0;i<ERM_MAX_NUM_PLAYER_HS_TABLE;i++)
    {
        if( AT_INVALID_DEVICE_ID == _ERM_stPlayerHeadsetTable[i].deviceIndex )
        {
            return i;
        }
    }
    return AT_INVALID_DEVICE_ID;
}


APP_COMMON_enResponseCode _ERM_rcPrepare_EepromUpdate(
                                    void (*cb)(APP_COMMON_enResponseCode),
                                    ERMIntMessageEnum ePendingEepromMsg )
{
    if( _ERM_gContext.ePendingEpromMsg != ERM_REQ_NONE )
    {
        return APP_COMMON_RSP_ERROR_BUSY;
    }


    DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM,"Prepare_EepromUpdate msg: %d",
                 ePendingEepromMsg);

    _ERM_gContext.ePendingEpromMsg      = ePendingEepromMsg;
    _ERM_gContext.cb_epromWriteFinished = cb;

    /* now trigger write to EPROM to given index, start with user
     * profile table */
    (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_ERM_PENDING, MSEC(0), MSEC(0));


    return APP_COMMON_RSP_OK;
}

APP_COMMON_enResponseCode _ERM_rcPrepare_DeleteUserProfile(Uint8Type index)
{
    if( _ERM_gContext.ePendingEpromMsg != ERM_REQ_NONE )
    {
        return APP_COMMON_RSP_ERROR_BUSY;
    }
    if (ERM_MAX_NUM_USER_PROFILES > index)
    {

        DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM,"Prepare_DeleteUserProfile idx: %d",
                 index);
        _ERM_gContext.epr_tmp.u8UserIdx                 = index;
        _ERM_gContext.epr_tmp.bUpdateUserEepromNeeded   = BT_TRUE;
        _ERM_gContext.epr_tmp.bUpdateUserRamNeeded      = BT_FALSE;
        (void)memset(&_ERM_gContext.epr_tmp.user, 0xFF, sizeof(ERMUserProfileType));
    }
    else
    {   /* not found or invalid, nothing to be done for this table */

        DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM,"Prepare_DeleteUserProfile idx: %d Noth. to do",
                 index);
        _ERM_gContext.epr_tmp.u8UserIdx                = AT_INVALID_DEVICE_ID;
        _ERM_gContext.epr_tmp.bUpdateUserEepromNeeded  = BT_FALSE;
        _ERM_gContext.epr_tmp.bUpdateUserRamNeeded     = BT_FALSE;
    }
    return APP_COMMON_RSP_OK;
}

APP_COMMON_enResponseCode _ERM_rcPrepare_AddUserProfile(
                        AT_DeviceId deviceId, Uint8Type * freeIndex,
                        Uint8Type playerIdx, Uint8Type headsetIdx)
{
    Uint8Type index = AT_INVALID_DEVICE_ID;

    if( _ERM_gContext.ePendingEpromMsg != ERM_REQ_NONE )
    {
        return APP_COMMON_RSP_ERROR_BUSY;
    }

    if( NULL != ERM_pGetUserProfile_DevId(deviceId))
    {

        DEBUG_VALUE1(ERM_SWI_ERR, MOD_ERM,"Prepare_AddUserProfile: existing for dev %d", deviceId);
        return APP_COMMON_RSP_ERROR_PROFILE_EXISTING;
    }

    index = _ERM_u8GetFreeUserProfileSlot();
    if ( ERM_MAX_NUM_USER_PROFILES > index)
    {
        if (SUCCESS == _ERM_sfSetUserProfileEntryToDefault(
                       &_ERM_gContext.epr_tmp.user))
        {


            DEBUG_VALUE4(ERM_SWI_TRC, MOD_ERM,"Prepare_AddUserProfile newSlot: %d (%d) ,play: %d,hs: %d",
                 index, ERM_MAX_NUM_USER_PROFILES, playerIdx, headsetIdx);

            _ERM_gContext.epr_tmp.u8UserIdx                 = index;
            _ERM_gContext.epr_tmp.bUpdateUserEepromNeeded   = BT_TRUE;
            _ERM_gContext.epr_tmp.bUpdateUserRamNeeded      = BT_FALSE;
            _ERM_gContext.epr_tmp.user.deviceIndex          = deviceId;
            *freeIndex                                      = index;
            /* associate current active player / headset profile to new user profile */
            _ERM_gContext.epr_tmp.user.playerIndex          = playerIdx;
            _ERM_gContext.epr_tmp.user.headSetIndex         = headsetIdx;
        }
    }
    else
    {

        DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM,"Prepare_AddUserProfile: newSlot: %d No free slot", index);
        _ERM_gContext.epr_tmp.u8UserIdx                = AT_INVALID_DEVICE_ID;
        _ERM_gContext.epr_tmp.bUpdateUserEepromNeeded  = BT_FALSE;
        _ERM_gContext.epr_tmp.bUpdateUserRamNeeded     = BT_FALSE;
        *freeIndex                                     = AT_INVALID_DEVICE_ID;
        return APP_COMMON_RSP_ERROR;
    }
    return APP_COMMON_RSP_OK;
}


APP_COMMON_enResponseCode
_ERM_rcPrepare_ChangeUserProfile_AssociatedPlayer( Uint8Type userProfileIndex, Uint8Type playerProfileIndex, BooleanType bConsiderInvalidPlayer)
{
    if( _ERM_gContext.ePendingEpromMsg != ERM_REQ_NONE )
    {
        return APP_COMMON_RSP_ERROR_BUSY;
    }

    if (ERM_MAX_NUM_USER_PROFILES > userProfileIndex)
    {
        if ( ERM_MAX_NUM_PLAYER_HS_TABLE > playerProfileIndex)
        {
                     
                /* copy curerent ram content to tmp user */
                (void)memcpy(&_ERM_gContext.epr_tmp.user,&(_ERM_stUserTable[userProfileIndex]),sizeof(ERMUserProfileType));
                /* copy new audio mode to tmp user */

                DEBUG_VALUE2(ERM_SWI_TRC, MOD_ERM,"Prepare_ChangeUserProfile_AssociatedPlayer %d to %d",
                     playerProfileIndex, userProfileIndex);
                _ERM_gContext.epr_tmp.user.playerIndex          = playerProfileIndex;

                _ERM_gContext.epr_tmp.u8UserIdx                 = userProfileIndex;
                _ERM_gContext.epr_tmp.bUpdateUserEepromNeeded   = BT_TRUE;
                _ERM_gContext.epr_tmp.bUpdateUserRamNeeded      = BT_FALSE;
        }
        else if (bConsiderInvalidPlayer == BT_TRUE)
        {
            DEBUG_VALUE2(ERM_SWI_TRC, MOD_ERM,"Prepare_ChangeUserProfile_AssociatedPlayer invalid index %d to %d",
             playerProfileIndex, userProfileIndex);

            _ERM_gContext.epr_tmp.u8UserIdx                = AT_INVALID_DEVICE_ID;
            _ERM_gContext.epr_tmp.bUpdateUserEepromNeeded  = BT_FALSE;
            _ERM_gContext.epr_tmp.bUpdateUserRamNeeded     = BT_FALSE;
            return APP_COMMON_RSP_OK;
        }
        else
        {
            /* copy curerent ram content to tmp user */
            (void)memcpy(&_ERM_gContext.epr_tmp.user,&(_ERM_stUserTable[userProfileIndex]),sizeof(ERMUserProfileType));
            /* copy new audio mode to tmp user */


            DEBUG_VALUE2(ERM_SWI_TRC, MOD_ERM,"Prepare_ChangeUserProfile_AssociatedPlayer %d to %d",
                 playerProfileIndex, userProfileIndex);
            _ERM_gContext.epr_tmp.user.playerIndex          = playerProfileIndex;

            _ERM_gContext.epr_tmp.u8UserIdx                 = userProfileIndex;
            _ERM_gContext.epr_tmp.bUpdateUserEepromNeeded   = BT_TRUE;
            _ERM_gContext.epr_tmp.bUpdateUserRamNeeded      = BT_FALSE;
        }
        
    }
    else
    {

        DEBUG_VALUE2(ERM_SWI_TRC, MOD_ERM,"Prepare_ChangeUserProfile_AssociatedPlayer invalid index %d to %d",
             playerProfileIndex, userProfileIndex);

        _ERM_gContext.epr_tmp.u8UserIdx                = AT_INVALID_DEVICE_ID;
        _ERM_gContext.epr_tmp.bUpdateUserEepromNeeded  = BT_FALSE;
        _ERM_gContext.epr_tmp.bUpdateUserRamNeeded     = BT_FALSE;
        return APP_COMMON_RSP_OK;
    }
    return APP_COMMON_RSP_OK;
}


APP_COMMON_enResponseCode
_ERM_rcPrepare_ChangeUserProfile_AssociatedHeadset( Uint8Type userProfileIndex, Uint8Type hsProfileIndex)
{
    if( _ERM_gContext.ePendingEpromMsg != ERM_REQ_NONE )
    {
        return APP_COMMON_RSP_ERROR_BUSY;
    }

    if (ERM_MAX_NUM_USER_PROFILES > userProfileIndex &&
        ERM_MAX_NUM_PLAYER_HS_TABLE > hsProfileIndex )
    {
        /* copy curerent ram content to tmp user */
        (void)memcpy(&_ERM_gContext.epr_tmp.user,&(_ERM_stUserTable[userProfileIndex]),sizeof(ERMUserProfileType));
        /* copy new audio mode to tmp user */


        DEBUG_VALUE2(ERM_SWI_TRC, MOD_ERM,"Prepare_ChangeUserProfile_AssociatedHeadset %d to %d",
             hsProfileIndex, userProfileIndex);
        _ERM_gContext.epr_tmp.user.headSetIndex         = hsProfileIndex;

        _ERM_gContext.epr_tmp.u8UserIdx                 = userProfileIndex;
        _ERM_gContext.epr_tmp.bUpdateUserEepromNeeded   = BT_TRUE;
        _ERM_gContext.epr_tmp.bUpdateUserRamNeeded      = BT_FALSE;
    }
    else
    {

        DEBUG_VALUE2(ERM_SWI_TRC, MOD_ERM,"Prepare_ChangeUserProfile_AssociatedHeadset invalid %d %d",
             hsProfileIndex, userProfileIndex);

        _ERM_gContext.epr_tmp.u8UserIdx                = AT_INVALID_DEVICE_ID;
        _ERM_gContext.epr_tmp.bUpdateUserEepromNeeded  = BT_FALSE;
        _ERM_gContext.epr_tmp.bUpdateUserRamNeeded     = BT_FALSE;
        return APP_COMMON_RSP_OK;
    }
    return APP_COMMON_RSP_OK;
}


APP_COMMON_enResponseCode
_ERM_rcPrepare_ChangeUserProfile_Ringtone( Uint8Type index, ATRintoneModeEnum ringtone)
{
    if( _ERM_gContext.ePendingEpromMsg != ERM_REQ_NONE )
    {
        return APP_COMMON_RSP_ERROR_BUSY;
    }

    if (ERM_MAX_NUM_USER_PROFILES > index)
    {
        /* copy curerent ram content to tmp user */
        (void)memcpy(&_ERM_gContext.epr_tmp.user,&(_ERM_stUserTable[index]),sizeof(ERMUserProfileType));
        /* copy new audio mode to tmp user */


        DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM,"Prepare_ChangeUserProfile_Ringtone: %d",
             ringtone);
        _ERM_gContext.epr_tmp.user.ringtone             = ringtone;

        _ERM_gContext.epr_tmp.u8UserIdx                 = index;
        _ERM_gContext.epr_tmp.bUpdateUserEepromNeeded   = BT_TRUE;
        _ERM_gContext.epr_tmp.bUpdateUserRamNeeded      = BT_FALSE;
    }
    else
    {

        DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM,"Prepare_ChangeUserProfile_Ringtone: invalid idx %d",
             index);

        _ERM_gContext.epr_tmp.u8UserIdx                = AT_INVALID_DEVICE_ID;
        _ERM_gContext.epr_tmp.bUpdateUserEepromNeeded  = BT_FALSE;
        _ERM_gContext.epr_tmp.bUpdateUserRamNeeded     = BT_FALSE;
        return APP_COMMON_RSP_OK;
    }
    return APP_COMMON_RSP_OK;
}

APP_COMMON_enResponseCode
_ERM_rcPrepare_ChangePhyTable_Name( AT_DeviceId deviceId, AT_DeviceNameString name, Uint16Type nameLen)
{
    if( _ERM_gContext.ePendingEpromMsg != ERM_REQ_NONE )
    {
        return APP_COMMON_RSP_ERROR_BUSY;
    }

    if (ERM_MAX_NUM_PHYSICAL_DEVICES > deviceId)
    {
        /* copy current ram content to tmp phy */
        (void)memcpy(&_ERM_gContext.epr_tmp.phy,&(_ERM_stPhyDevTable[deviceId]),sizeof(ERMPhysicalBTDeviceType));
        /* copy new name to tmp phy */


        DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM,"Prepare_ChangePhyTable_Name: devId %d", deviceId);

        if( nameLen > sizeof(AT_DeviceNameString))
        {
            nameLen = sizeof(AT_DeviceNameString);
        }
        (void)memset(&_ERM_gContext.epr_tmp.phy.name, 0x00, sizeof(AT_DeviceNameString));
        (void)memcpy(&_ERM_gContext.epr_tmp.phy.name, name, nameLen);
        _ERM_gContext.epr_tmp.phy.name_len = nameLen;

        _ERM_gContext.epr_tmp.u8PhyIdx                  = deviceId;
        _ERM_gContext.epr_tmp.bUpdatePhyEepromNeeded    = BT_TRUE;
        _ERM_gContext.epr_tmp.bUpdatePhyRamNeeded       = BT_FALSE;
    }
    else
    {

        DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM,"Prepare_ChangePhyTable_Name: invalid devId %d", deviceId);

        _ERM_gContext.epr_tmp.u8PhyIdx                  = AT_INVALID_DEVICE_ID;
        _ERM_gContext.epr_tmp.bUpdatePhyEepromNeeded    = BT_FALSE;
        _ERM_gContext.epr_tmp.bUpdatePhyRamNeeded       = BT_FALSE;
        return APP_COMMON_RSP_OK;
    }
    return APP_COMMON_RSP_OK;
}


APP_COMMON_enResponseCode
_ERM_rcPrepare_ChangeUserProfile_PbCompletelyLoaded( Uint8Type index)
{
    if( _ERM_gContext.ePendingEpromMsg != ERM_REQ_NONE )
    {
        return APP_COMMON_RSP_ERROR_BUSY;
    }

    if (ERM_MAX_NUM_USER_PROFILES > index)
    {
        /* copy curerent ram content to tmp user */
        (void)memcpy(&_ERM_gContext.epr_tmp.user,&(_ERM_stUserTable[index]),sizeof(ERMUserProfileType));
        /* copy new audio mode to tmp user */


        DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM,"Prepare_ChangeUserProfile_PbCompletelyLoaded %d",
                        index);

        _ERM_gContext.epr_tmp.user.bPbLoadedCompletly   = BT_TRUE;

        _ERM_gContext.epr_tmp.u8UserIdx                 = index;
        _ERM_gContext.epr_tmp.bUpdateUserEepromNeeded   = BT_TRUE;
        _ERM_gContext.epr_tmp.bUpdateUserRamNeeded      = BT_FALSE;
    }
    else
    {

        DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM,"Prepare_ChangeUserProfile_PbCompletelyLoaded invalid idx %d",
                        index);

        _ERM_gContext.epr_tmp.u8UserIdx                = AT_INVALID_DEVICE_ID;
        _ERM_gContext.epr_tmp.bUpdateUserEepromNeeded  = BT_FALSE;
        _ERM_gContext.epr_tmp.bUpdateUserRamNeeded     = BT_FALSE;
        return APP_COMMON_RSP_OK;
    }
    return APP_COMMON_RSP_OK;
}



APP_COMMON_enResponseCode
_ERM_rcPrepare_ChangeUserProfile_RingtoneAlternate( Uint8Type index, BooleanType bAlternate)
{
    if( _ERM_gContext.ePendingEpromMsg != ERM_REQ_NONE )
    {
        return APP_COMMON_RSP_ERROR_BUSY;
    }

    if (ERM_MAX_NUM_USER_PROFILES > index)
    {
        /* copy curerent ram content to tmp user */
        (void)memcpy(&_ERM_gContext.epr_tmp.user,&(_ERM_stUserTable[index]),sizeof(ERMUserProfileType));
        /* copy new audio mode to tmp user */


        DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM,"Prepare_ChangeUserProfile_RingtoneAlternate %d",
                        bAlternate);
        _ERM_gContext.epr_tmp.user.ringtone_alternating = bAlternate;

        _ERM_gContext.epr_tmp.u8UserIdx                 = index;
        _ERM_gContext.epr_tmp.bUpdateUserEepromNeeded   = BT_TRUE;
        _ERM_gContext.epr_tmp.bUpdateUserRamNeeded      = BT_FALSE;
    }
    else
    {

        DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM,"Prepare_ChangeUserProfile_RingtoneAlternate invalid idx %d",
                        index);

        _ERM_gContext.epr_tmp.u8UserIdx                = AT_INVALID_DEVICE_ID;
        _ERM_gContext.epr_tmp.bUpdateUserEepromNeeded  = BT_FALSE;
        _ERM_gContext.epr_tmp.bUpdateUserRamNeeded     = BT_FALSE;
        return APP_COMMON_RSP_OK;
    }
    return APP_COMMON_RSP_OK;
}


APP_COMMON_enResponseCode
_ERM_rcPrepare_ChangeUserProfile_AudioMode( Uint8Type index, ERMUserAudioModeType audioMode)
{
    if( _ERM_gContext.ePendingEpromMsg != ERM_REQ_NONE )
    {
        return APP_COMMON_RSP_ERROR_BUSY;
    }

    if (ERM_MAX_NUM_USER_PROFILES > index)
    {
        /* copy curerent ram content to tmp user */
        (void)memcpy(&_ERM_gContext.epr_tmp.user,&(_ERM_stUserTable[index]),sizeof(ERMUserProfileType));
        /* copy new audio mode to tmp user */


        DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM,"Prepare_ChangeUserProfile_AudioMode %d",
                        audioMode);
        _ERM_gContext.epr_tmp.user.audio_mode           = audioMode;

        _ERM_gContext.epr_tmp.u8UserIdx                 = index;
        _ERM_gContext.epr_tmp.bUpdateUserEepromNeeded   = BT_TRUE;
        _ERM_gContext.epr_tmp.bUpdateUserRamNeeded      = BT_FALSE;
    }
    else
    {

        DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM,"Prepare_ChangeUserProfile_AudioMode invalid idx %d",
                        index);

        _ERM_gContext.epr_tmp.u8UserIdx                = AT_INVALID_DEVICE_ID;
        _ERM_gContext.epr_tmp.bUpdateUserEepromNeeded  = BT_FALSE;
        _ERM_gContext.epr_tmp.bUpdateUserRamNeeded     = BT_FALSE;
        return APP_COMMON_RSP_OK;
    }
    return APP_COMMON_RSP_OK;
}

APP_COMMON_enResponseCode
_ERM_rcPrepare_ChangePhyTable_Voicetags( Uint8Type index, Uint8Type numVoiceTags, BooleanType bDeleteOngoing)
{
    if( _ERM_gContext.ePendingEpromMsg != ERM_REQ_NONE )
    {
        return APP_COMMON_RSP_ERROR_BUSY;
    }

    if (ERM_MAX_NUM_PHYSICAL_DEVICES > index)
    {
        /* copy curerent ram content to tmp phy */
        (void)memcpy(&_ERM_gContext.epr_tmp.phy,&(_ERM_stPhyDevTable[index]),sizeof(ERMPhysicalBTDeviceType));

        /* copy new number of voicetags to tmp phy */

        DEBUG_VALUE2(ERM_SWI_TRC, MOD_ERM,"Prepare_ChangePhyTable_Voicetags num: %d, ongoing: %d",
             numVoiceTags, bDeleteOngoing);
        _ERM_gContext.epr_tmp.phy.numVoiceTags          = numVoiceTags;
        _ERM_gContext.epr_tmp.phy.bVTDeleteOngoing      = bDeleteOngoing;

        _ERM_gContext.epr_tmp.u8PhyIdx                  = index;
        _ERM_gContext.epr_tmp.bUpdatePhyEepromNeeded    = BT_TRUE;
        _ERM_gContext.epr_tmp.bUpdatePhyRamNeeded       = BT_FALSE;
    }
    else
    {

        DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM,"Prepare_ChangePhyTable_Voicetags invalid idx %d",
                        index);

        _ERM_gContext.epr_tmp.u8PhyIdx                = AT_INVALID_DEVICE_ID;
        _ERM_gContext.epr_tmp.bUpdatePhyEepromNeeded  = BT_FALSE;
        _ERM_gContext.epr_tmp.bUpdatePhyRamNeeded     = BT_FALSE;
        return APP_COMMON_RSP_OK;
    }
    return APP_COMMON_RSP_OK;
}


APP_COMMON_enResponseCode
_ERM_rcPrepare_ChangeUserProfile_Mailboxnumber(Uint8Type index,
                                               const Uint8Type *pNumber,
                                               Uint16Type len)
{
    Uint16Type length   = 0;

    if( _ERM_gContext.ePendingEpromMsg != ERM_REQ_NONE )
    {
        return APP_COMMON_RSP_ERROR_BUSY;
    }

    if (ERM_MAX_NUM_USER_PROFILES > index)
    {
        /* copy curerent ram content to tmp user */
        (void)memcpy(&_ERM_gContext.epr_tmp.user,&(_ERM_stUserTable[index]),sizeof(ERMUserProfileType));
        /* copy new mailbox number to tmp user */


        //DEBUG_STRING(ERM_SWI_TRC, MOD_ERM,"Prepare_ChangeUserProfile_Mailboxnumber %s",
        //      pNumber);

        length = ( len > sizeof( ATPhoneNumberType )) ?
                    sizeof( ATPhoneNumberType ) : len;
        (void)memset(&_ERM_gContext.epr_tmp.user.voicemailbox,0x00, sizeof(ATPhoneNumberType));
        (void)memcpy(&_ERM_gContext.epr_tmp.user.voicemailbox, pNumber, length);
        _ERM_gContext.epr_tmp.user.voicemailbox_len = (Uint8Type)length;

        _ERM_gContext.epr_tmp.u8UserIdx                = index;
        _ERM_gContext.epr_tmp.bUpdateUserEepromNeeded  = BT_TRUE;
        _ERM_gContext.epr_tmp.bUpdateUserRamNeeded     = BT_FALSE;
    }
    else
    {

        DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM,"Prepare_ChangeUserProfile_Mailboxnumber invalid idx %d",
             index);

        _ERM_gContext.epr_tmp.u8UserIdx                = AT_INVALID_DEVICE_ID;
        _ERM_gContext.epr_tmp.bUpdateUserEepromNeeded  = BT_FALSE;
        _ERM_gContext.epr_tmp.bUpdateUserRamNeeded     = BT_FALSE;
        return APP_COMMON_RSP_OK;
    }
    return APP_COMMON_RSP_OK;
}

APP_COMMON_enResponseCode
_ERM_rcPrepare_ChangePlayerHeadsetProfile_AuthMode( Uint8Type index, BooleanType bAuthNeeded)
{
    if( _ERM_gContext.ePendingEpromMsg != ERM_REQ_NONE )
    {
        return APP_COMMON_RSP_ERROR_BUSY;
    }

    if (ERM_MAX_NUM_PLAYER_HS_TABLE > index)
    {
        /* copy curerent ram content to tmp player / headset */
        (void)memcpy(&_ERM_gContext.epr_tmp.playHs,&(_ERM_stPlayerHeadsetTable[index]),sizeof(ERMPlayerHeadsetProfileType));
        /* copy new authentication mode to tmp player / headset */


        DEBUG_VALUE2(ERM_SWI_TRC, MOD_ERM,"Prepare_ChangePlayerHeadsetProfile_AuthMode prof %d to mode: %d",
             index, bAuthNeeded);
        _ERM_gContext.epr_tmp.playHs.bAuthNeeded        = bAuthNeeded;

        _ERM_gContext.epr_tmp.u8PlayHSIdx               = index;
        _ERM_gContext.epr_tmp.bUpdatePlayHSEepromNeeded = BT_TRUE;
        _ERM_gContext.epr_tmp.bUpdatePlayHSRamNeeded    = BT_FALSE;
    }
    else
    {

        DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM,"Prepare_ChangePlayerHeadsetProfile_AuthMode prof %d to mode: %d",
             index);

        _ERM_gContext.epr_tmp.u8PlayHSIdx                = AT_INVALID_DEVICE_ID;
        _ERM_gContext.epr_tmp.bUpdatePlayHSEepromNeeded  = BT_FALSE;
        _ERM_gContext.epr_tmp.bUpdatePlayHSRamNeeded     = BT_FALSE;
        return APP_COMMON_RSP_OK;
    }
    return APP_COMMON_RSP_OK;
}



APP_COMMON_enResponseCode _ERM_rcPrepare_DeletePlayerHeadsetProfile(Uint8Type index)
{
    if( _ERM_gContext.ePendingEpromMsg != ERM_REQ_NONE )
    {
        return APP_COMMON_RSP_ERROR_BUSY;
    }
    if (ERM_MAX_NUM_PLAYER_HS_TABLE > index)
    {

        DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM,"Prepare_DeletePlayerHeadsetProfile idx: %d", index);
        _ERM_gContext.epr_tmp.u8PlayHSIdx               = index;
        _ERM_gContext.epr_tmp.bUpdatePlayHSEepromNeeded = BT_TRUE;
        _ERM_gContext.epr_tmp.bUpdatePlayHSRamNeeded    = BT_FALSE;
        (void)memset(&_ERM_gContext.epr_tmp.playHs,0xFF, sizeof(ERMPlayerHeadsetProfileType));
    }
    else
    {   /* not found or invalid, nothing to be done for this table */

        DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM,"Prepare_DeletePlayerHeadsetProfile Invalid idx: %d", index);
        _ERM_gContext.epr_tmp.bUpdatePlayHSEepromNeeded = BT_FALSE;
        _ERM_gContext.epr_tmp.bUpdatePlayHSRamNeeded    = BT_FALSE;
    }
    return APP_COMMON_RSP_OK;
}

APP_COMMON_enResponseCode
_ERM_rcPrepare_AddPlayerHeadsetProfile( AT_DeviceId deviceId,
                                        APP_DEVICE_DeviceType deviceType,
                                        Uint8Type * freeIndex)
{
    Uint8Type index = AT_INVALID_DEVICE_ID;

    if( _ERM_gContext.ePendingEpromMsg != ERM_REQ_NONE )
    {
        return APP_COMMON_RSP_ERROR_BUSY;
    }
    if( NULL != ERM_pGetPlayerHeadsetProfile_DevId(deviceId))
    {

        DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM,"Prepare_AddPlayerHeadsetProfile existing dev: %d", deviceId);
        return APP_COMMON_RSP_ERROR_PROFILE_EXISTING;
    }

    index = _ERM_u8GetFreePlayerHeadsetProfileSlot();

    if (index < ERM_MAX_NUM_PLAYER_HS_TABLE)
    {
        if (SUCCESS == _ERM_sfSetPlayerHeadsetProfileEntryToDefault(
                       &_ERM_gContext.epr_tmp.playHs, deviceType))
        {

            DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM,"Prepare_AddPlayerHeadsetProfile newSlot: %d", index);
            _ERM_gContext.epr_tmp.u8PlayHSIdx               = index;
            _ERM_gContext.epr_tmp.bUpdatePlayHSEepromNeeded = BT_TRUE;
            _ERM_gContext.epr_tmp.bUpdatePlayHSRamNeeded    = BT_FALSE;
            _ERM_gContext.epr_tmp.playHs.deviceIndex        = deviceId;
            *freeIndex                                      = index;
        }
    }
    else
    {

        DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM,"Prepare_AddPlayerHeadsetProfile No free slot");
        _ERM_gContext.epr_tmp.u8PlayHSIdx               = AT_INVALID_DEVICE_ID;
        _ERM_gContext.epr_tmp.bUpdatePlayHSEepromNeeded = BT_FALSE;
        _ERM_gContext.epr_tmp.bUpdatePlayHSRamNeeded    = BT_FALSE;
        *freeIndex                                      = AT_INVALID_DEVICE_ID;
        return APP_COMMON_RSP_ERROR;
    }
    return APP_COMMON_RSP_OK;

}

APP_COMMON_enResponseCode _ERM_rcPrepare_DeletePhysicalDevice(AT_DeviceId deviceId)
{
    if( _ERM_gContext.ePendingEpromMsg != ERM_REQ_NONE )
    {
        return APP_COMMON_RSP_ERROR_BUSY;
    }

    if (ERM_MAX_NUM_PHYSICAL_DEVICES > deviceId)
    {

        DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM,"Prepare_DeletePhysicalDevice devId: %d", deviceId);
        (void)memset(&_ERM_gContext.epr_tmp.phy, 0xFF, sizeof(ERMPhysicalBTDeviceType));
        _ERM_gContext.epr_tmp.u8PhyIdx                  = deviceId;
        _ERM_gContext.epr_tmp.bUpdatePhyEepromNeeded    = BT_TRUE;
        _ERM_gContext.epr_tmp.bUpdatePhyRamNeeded       = BT_FALSE;
    }
    else
    {   /* nothing to be done here */

        DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM,"Prepare_DeletePhysicalDevice invalid id %d", deviceId);
        _ERM_gContext.epr_tmp.u8PhyIdx                  = AT_INVALID_DEVICE_ID;
        _ERM_gContext.epr_tmp.bUpdatePhyEepromNeeded    = BT_FALSE;
        _ERM_gContext.epr_tmp.bUpdatePhyRamNeeded       = BT_FALSE;
    }
    return APP_COMMON_RSP_OK;
}


APP_COMMON_enResponseCode _ERM_rcPrepare_AddPhysicalDevice(AT_DeviceId deviceId,
                                    const Uint8Type *pName, Uint16Type nameLen)
{

    if( _ERM_gContext.ePendingEpromMsg != ERM_REQ_NONE )
    {
        return APP_COMMON_RSP_ERROR_BUSY;
    }
    if (ERM_MAX_NUM_PHYSICAL_DEVICES > deviceId)
    {
        if (SUCCESS == _ERM_sfSetPhysicalDeviceEntryToDefault(&_ERM_gContext.epr_tmp.phy,
                                                              deviceId, pName, nameLen) )
        {

            DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM,"Prepare_AddPhysicalDevice devId: %d", deviceId);
            _ERM_gContext.epr_tmp.u8PhyIdx                  = deviceId;
            _ERM_gContext.epr_tmp.bUpdatePhyEepromNeeded    = BT_TRUE;
            _ERM_gContext.epr_tmp.bUpdatePhyRamNeeded       = BT_FALSE;
        }
    }
    else
    {

        DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM,"Prepare_AddPhysicalDevice invalid id %d", deviceId);
        _ERM_gContext.epr_tmp.u8PhyIdx                  = AT_INVALID_DEVICE_ID;
        _ERM_gContext.epr_tmp.bUpdatePhyEepromNeeded    = BT_FALSE;
        _ERM_gContext.epr_tmp.bUpdatePhyRamNeeded       = BT_FALSE;
        return APP_COMMON_RSP_ERROR;
    }
    return APP_COMMON_RSP_OK;
}


/* add and delete a profile from active profile list */
APP_COMMON_enResponseCode _ERM_rcPrepare_AddActiveProfile(Uint8Type profileIdx, APP_DEVICE_DeviceType profileType)
{
    AT_DeviceId  tmpId = AT_INVALID_DEVICE_ID;
    Sint32Type   i     = 0;

    AT_DeviceId *pProfileList = NULL;
    Uint16Type  maxProfileNum = 0;



    DEBUG_VALUE2(ERM_SWI_TRC, MOD_ERM,"Prepare_AddActiveProfile %d, type: %d",
                 profileIdx, profileType);

    if( _ERM_gContext.ePendingEpromMsg != ERM_REQ_NONE )
    {
        return APP_COMMON_RSP_ERROR_BUSY;
    }

    /* copy current content of profile type into tmp buffer */
    switch( profileType )
    {
        case APP_DEVICE_enTYPE_PHONE:
            pProfileList    = _ERM_gContext.epr_tmp.actList.UserProfileList;
            maxProfileNum   =  ERM_MAX_NUM_USER_PROFILES;

            /* do a copy of user's ram list to the tmp list that will be written to eprom */
            (void)memcpy( &_ERM_gContext.epr_tmp.actList.UserProfileList,
                    &_ERM_stActiveProfileList.UserProfileList,
                    sizeof(_ERM_gContext.epr_tmp.actList.UserProfileList));
            _ERM_gContext.epr_tmp.bActUserProfileListChanged = BT_TRUE;
            break;

        case APP_DEVICE_enTYPE_PLAYER:
            pProfileList    = _ERM_gContext.epr_tmp.actList.PlayerProfileList;
            maxProfileNum   =  ERM_MAX_NUM_PLAYER_PROFILE;

            /* do a copy of player's ram list to the tmp list that will be written to eprom */
            (void)memcpy( &_ERM_gContext.epr_tmp.actList.PlayerProfileList,
                    &_ERM_stActiveProfileList.PlayerProfileList,
                    sizeof(_ERM_gContext.epr_tmp.actList.PlayerProfileList));
            _ERM_gContext.epr_tmp.bActPlayerProfileListChanged = BT_TRUE;
            break;

        case APP_DEVICE_enTYPE_HEADSET:
            pProfileList    = _ERM_gContext.epr_tmp.actList.HeadsetProfileList;
            maxProfileNum   =  ERM_MAX_NUM_HEADSET_PROFILE;

            /* do a copy of headset's ram list to the tmp list that will be written to eprom */
            (void)memcpy( &_ERM_gContext.epr_tmp.actList.HeadsetProfileList,
                    &_ERM_stActiveProfileList.HeadsetProfileList,
                    sizeof(_ERM_gContext.epr_tmp.actList.HeadsetProfileList));
            _ERM_gContext.epr_tmp.bActHeadsetProfileListChanged = BT_TRUE;
            break;

        default:

            DEBUG_VALUE1(ERM_SWI_ERR, MOD_ERM,"Prepare_AddActiveProfile unknown profile %d",
                          profileType);
            return APP_COMMON_RSP_ERROR;
    }

    /* try to find the device id in the persistent active device list */
    for( i=0; i<maxProfileNum; i++)
    {
        if(pProfileList[i] == profileIdx)
        {
            break;
        }
    }

    if( i==0 )
    {
        /* the activeUser is already on first position of the list, so nothing
         * more to do here, we will NOT write to EPROM */
        return APP_COMMON_RSP_OK;
    }

    if( i == maxProfileNum )
    {
        /* the activeUser is not contained in our list, so we will drop the
         * oldest userId */
        i = maxProfileNum - 1;
    }

    /* swap the elements beginning with active device found in our list or if we
     * didn't find the user in our list, we swap beginning with the oldest
     * (last) element */
        for( i=i; i>0; i--)
        {
            tmpId  = pProfileList[i];
            pProfileList[i]   = pProfileList[i-1];
            pProfileList[i-1] = tmpId;
        }

    if(pProfileList[0] != profileIdx)
    {
        /* now the first element is an empty (invalid) id or the oldest userId */
        if( pProfileList[0] != AT_INVALID_DEVICE_ID )
        {

            DEBUG_VALUE2(ERM_SWI_TRC, MOD_ERM,"Overwrite old act dev Id %d (profile %d)",
                          pProfileList[0],
                          profileType);
        }

        /* overwrite the id with the new active device id */
        pProfileList[0] = profileIdx;
    }

    DEBUG_VALUE2(ERM_SWI_TRC, MOD_ERM,"Prepare_AddActiveProfile profileIdx=%d i=%d",
                 profileIdx,
                 i);

    _ERM_gContext.epr_tmp.bUpdateActProfileEepromNeeded = BT_TRUE;
    _ERM_gContext.epr_tmp.bUpdateActProfileRamNeeded    = BT_FALSE;

    return APP_COMMON_RSP_OK;
}

/* add and delete a phone profile from/to  active profile list (put it to the first free slot) when this phone is not connected*/
APP_COMMON_enResponseCode _ERM_rcPrepare_AddActivePhoneProfileWhenNotConnectedAsPhone(Uint8Type profileIdx)
{    
    Sint32Type   i     = 0;

    AT_DeviceId *pProfileList = NULL;
    Uint16Type  maxProfileNum = 0;

    DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM,"Prepare_AddActivePhoneProfileWhenNotConnectedAsPhone %d",
                 profileIdx);

    if( _ERM_gContext.ePendingEpromMsg != ERM_REQ_NONE )
    {
        return APP_COMMON_RSP_ERROR_BUSY;
    }

    /* copy current content of profile type into tmp buffer */   
    pProfileList    = _ERM_gContext.epr_tmp.actList.UserProfileList;
    maxProfileNum   =  ERM_MAX_NUM_USER_PROFILES;

    /* do a copy of user's ram list to the tmp list that will be written to eprom */
    (void)memcpy( &_ERM_gContext.epr_tmp.actList.UserProfileList,
            &_ERM_stActiveProfileList.UserProfileList,
            sizeof(_ERM_gContext.epr_tmp.actList.UserProfileList));
    _ERM_gContext.epr_tmp.bActUserProfileListChanged = BT_TRUE;      
    

    /* try to find the device id in the persistent active device list */
    for( i=0; i<maxProfileNum; i++)
    {
        if((pProfileList[i] == profileIdx)||(pProfileList[i] == AT_INVALID_DEVICE_ID))
        {
            break;
        }
    }

    if( i == maxProfileNum )
    {
        /* the activeUser is not contained in our list, so we will drop the
         * oldest userId */
        i = maxProfileNum - 1;
    }
    
    DEBUG_VALUE4(ERM_SWI_TRC, MOD_ERM,"Prepare_AddActivePhoneProfileWhenNotConnectedAsPhone pProfileList[0]=%d, [1]=%d, [2]=%d, [3]=%d",
                 pProfileList[0],
                 pProfileList[1],
                 pProfileList[2],
                 pProfileList[3]);       

    DEBUG_VALUE2(ERM_SWI_TRC, MOD_ERM,"Prepare_AddActivePhoneProfileWhenNotConnectedAsPhone profileIdx=%d i=%d",
                 profileIdx,
                 i);                  
                 
    pProfileList[i] = profileIdx;
    
    _ERM_gContext.epr_tmp.bUpdateActProfileEepromNeeded = BT_TRUE;
    _ERM_gContext.epr_tmp.bUpdateActProfileRamNeeded    = BT_FALSE;

    return APP_COMMON_RSP_OK;
}
APP_COMMON_enResponseCode _ERM_rcPrepare_DeleteActiveProfile(Uint8Type profileIndex, APP_DEVICE_DeviceType profileType)
{
    AT_DeviceId *pProfileList = NULL;
    Uint16Type  maxProfileNum = 0;
    AT_DeviceId         tmpId = AT_INVALID_DEVICE_ID;
    Sint32Type              i = 0;


    if( _ERM_gContext.ePendingEpromMsg != ERM_REQ_NONE )
    {
        return APP_COMMON_RSP_ERROR_BUSY;
    }


    DEBUG_VALUE2(ERM_SWI_TRC, MOD_ERM,"Prepare_DeleteActiveProfile %d, type: %d",
                 profileIndex, profileType);

    switch( profileType )
    {
        case APP_DEVICE_enTYPE_PHONE:
            pProfileList    = _ERM_gContext.epr_tmp.actList.UserProfileList;
            maxProfileNum   =  ERM_MAX_NUM_USER_PROFILES;

            /* do a copy of user's ram list to the tmp list that will be written to eprom */
            (void)memcpy( &_ERM_gContext.epr_tmp.actList.UserProfileList,
                    &_ERM_stActiveProfileList.UserProfileList,
                    sizeof(_ERM_gContext.epr_tmp.actList.UserProfileList));
            _ERM_gContext.epr_tmp.bActUserProfileListChanged = BT_TRUE;
            break;

        case APP_DEVICE_enTYPE_PLAYER:
            pProfileList    = _ERM_gContext.epr_tmp.actList.PlayerProfileList;
            maxProfileNum   =  ERM_MAX_NUM_PLAYER_PROFILE;

            /* do a copy of player's ram list to the tmp list that will be written to eprom */
            (void)memcpy( &_ERM_gContext.epr_tmp.actList.PlayerProfileList,
                    &_ERM_stActiveProfileList.PlayerProfileList,
                    sizeof(_ERM_gContext.epr_tmp.actList.PlayerProfileList));
            _ERM_gContext.epr_tmp.bActPlayerProfileListChanged = BT_TRUE;
            break;

        case APP_DEVICE_enTYPE_HEADSET:
            pProfileList    = _ERM_gContext.epr_tmp.actList.HeadsetProfileList;
            maxProfileNum   =  ERM_MAX_NUM_HEADSET_PROFILE;

            /* do a copy of headset's ram list to the tmp list that will be written to eprom */
            (void)memcpy( &_ERM_gContext.epr_tmp.actList.HeadsetProfileList,
                    &_ERM_stActiveProfileList.HeadsetProfileList,
                    sizeof(_ERM_gContext.epr_tmp.actList.HeadsetProfileList));
            _ERM_gContext.epr_tmp.bActHeadsetProfileListChanged = BT_TRUE;
            break;

        default:

            DEBUG_VALUE1(ERM_SWI_ERR, MOD_ERM,"Prepare_DeleteActiveProfile unknown profile %d",
                          profileType);
            return APP_COMMON_RSP_ERROR;
    }

    /* try to find the device Id in the persistent active device list */
    for( i=0; i<maxProfileNum; i++)
    {
        if(pProfileList[i] == profileIndex)
        {
            break;
        }
    }

    if( i == maxProfileNum )
    {
        /* the activeUser is not contained in our list, so we have nothing to do
         * and we will NOT write the active list to EPROM */
        return APP_COMMON_RSP_OK;
    }

    /* swap the elements beginning with device index to remove */
    for( i=i; i<maxProfileNum-1; i++)
    {
        tmpId  = pProfileList[i];
        pProfileList[i]   = pProfileList[i+1];
        pProfileList[i+1] = tmpId;
    }

    /* now the last element should be the device id to remove, so just set it
     * to empty device id */
    if(pProfileList[maxProfileNum-1] != profileIndex)
    {
        /* should not happen, but better safe than sorry */

        DEBUG_VALUE2(ERM_SWI_TRC, MOD_ERM,"Prep_DelActProf dev to remove not match profIdx (%d,%d)",
                      pProfileList[maxProfileNum-1],
                      profileIndex );
    }

    pProfileList[maxProfileNum-1] = AT_INVALID_DEVICE_ID;

    _ERM_gContext.epr_tmp.bUpdateActProfileEepromNeeded = BT_TRUE;
    _ERM_gContext.epr_tmp.bUpdateActProfileRamNeeded    = BT_FALSE;

    return APP_COMMON_RSP_OK;
}

Uint8Type ERM_u8GetUserProfileId(AT_DeviceId deviceId)
{
    Uint8Type i = 0;

    if( deviceId > AT_MAX_DEVICE_ID )
    {
        return AT_INVALID_DEVICE_ID;
    }

    for(i=0;i<ERM_MAX_NUM_USER_PROFILES;i++)
    {
        if(_ERM_stUserTable[i].deviceIndex == deviceId)
        {
            return i;
        }
    }
    return AT_INVALID_DEVICE_ID;
}

Uint8Type ERM_u8GetPlayerHeadsetProfileId(AT_DeviceId deviceId)
{
    Uint8Type i = 0;

    if( deviceId > AT_MAX_DEVICE_ID )
    {
        return AT_INVALID_DEVICE_ID;
    }

    for(i=0;i<ERM_MAX_NUM_PLAYER_HS_TABLE;i++)
    {
        if(_ERM_stPlayerHeadsetTable[i].deviceIndex == deviceId)
        {
            return i;
        }
    }
    return AT_INVALID_DEVICE_ID;
}


static void _ERM_vSetStartupDone(void)
{
    ERMNotificationType notification;

    _ERM_gContext.bStartupDone = BT_TRUE;
    notification.common.id = ERM_NOTIFY_STARTUP_DONE;
    _ERM_vNotifyApplication(&notification);
}

Uint8Type ERM_u8GetOldestNoActiveUserProfileId(void)
{
    Uint8Type i = 0;
    Uint8Type profileId = AT_INVALID_DEVICE_ID;

    for(i=0;i<ERM_MAX_NUM_USER_PROFILES;i++)
    {
        if((_ERM_stActiveProfileList.UserProfileList[i] != AT_INVALID_DEVICE_ID) && \
           (_ERM_stUserTable[_ERM_stActiveProfileList.UserProfileList[i]].deviceIndex != ERM_u8GetActivePhone()))
        {
            profileId = _ERM_stActiveProfileList.UserProfileList[i];
        }
    }

    return profileId;
}

Uint8Type ERM_u8GetOldestUserProfileId(void)
{
    Uint8Type i = 0;
    Uint8Type profileId = AT_INVALID_DEVICE_ID;

    for(i=0;i<ERM_MAX_NUM_USER_PROFILES;i++)
    {
        if(_ERM_stActiveProfileList.UserProfileList[i] != AT_INVALID_DEVICE_ID)
        {
            profileId = _ERM_stActiveProfileList.UserProfileList[i];
        }
    }

    return profileId;
}

Uint8Type ERM_u8GetOldestPlayerProfileId(void)
{
    Uint8Type i = 0;
    Uint8Type j = 0;
    Uint8Type profileId = AT_INVALID_DEVICE_ID;

    for(i=0;i<ERM_MAX_NUM_PLAYER_PROFILE;i++)
    {
        const ERMPlayerHeadsetProfileType *pPlayerProfile =
                     ERM_pGetPlayerHeadsetProfile(_ERM_stActiveProfileList.PlayerProfileList[i]);

        if(pPlayerProfile == NULL)
        {
            continue;
        }

        /* make sure that the player device is not used as phone device */
        for(j=0;j<ERM_MAX_NUM_USER_PROFILES;j++)
        {
            const ERMUserProfileType *pUserProfile =
                            ERM_pGetUserProfile(_ERM_stActiveProfileList.UserProfileList[j]);

            if( (pUserProfile != NULL) &&
                (pUserProfile->deviceIndex == pPlayerProfile->deviceIndex) )
            {
                /* we found a player profile that is a phone device, too */
                break;
            }
        }

        if(j == ERM_MAX_NUM_USER_PROFILES)
        {
            profileId = _ERM_stActiveProfileList.PlayerProfileList[i];
        }

    }

    return profileId;
}


Uint8Type ERM_u8GetOldestHeadsetProfileId(void)
{
    Uint8Type i = 0;
    Uint8Type profileId = AT_INVALID_DEVICE_ID;

    for(i=0;i<ERM_MAX_NUM_HEADSET_PROFILE;i++)
    {
        if(_ERM_stActiveProfileList.HeadsetProfileList[i] != AT_INVALID_DEVICE_ID)
        {
            profileId = _ERM_stActiveProfileList.HeadsetProfileList[i];
        }
    }

    return profileId;
}
//RT13844 Test
BooleanType ERM_vCheckforprofileId(AT_DeviceId id)
{
    Uint16Type i=0,temp;
    i=ERM_u16GetNumberOfUserProfiles();

    DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM,"ERM_vCheckforprofileId CALLED");
    for(temp=0; temp<i; temp++)
    {
        if(_ERM_stUserTable[temp].deviceIndex==id)
        {
            DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM,"ERM_vCheckforprofileId VALID");
            return BT_TRUE;
        }
    }
    DEBUG_TEXT(ERM_SWI_TRC, MOD_ERM,"ERM_vCheckforprofileId INVALID");
    return BT_FALSE;

}


/* End Of File ERM_DeviceHandler.c */
