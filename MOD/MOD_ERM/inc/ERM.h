/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     ERM.h
*-----------------------------------------------------------------------------
* Module Name:    External Resource Manager
*-----------------------------------------------------------------------------
* Description:    External resource manager global functions.
*                 the ERM is responsible for control of the UART resource
*                 towards the different other application modules
*-----------------------------------------------------------------------------
* $Date: 2011-11-23 08:25:57 +0100 (Mi, 23 Nov 2011) $
* $Rev: 24103 $
* $Author: maennel $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_ERM/trunk/inc/ERM.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _ERM_H
#define _ERM_H


/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include "ERM_CI.h"
#include "ERM_CE.h"
#include "tms470r1.h"
/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                             */
/*----------------------------------------------------------------------------*/
#include "osek.h"

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "ATParser.h"
#include "ATCmdHandlerData.h"
#include "APP_COMMON_UTILS.h"
#include "EEprom.h"

/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/

#define BT_UPDATE_NORMAL_MODE      1  // no security check
#define BT_UPDATE_SECURED_MODE     2  // with security check

// Timings are in ms
#define BT_UPDATE_TRACE_INTERVALL                1000
#define BT_UPDATE_TRANSFER_AND_FLASH_TIMEOUT  3600000

/*----------------------------------------------------------------------------*/
/* Private Enumerations                                                       */
/*----------------------------------------------------------------------------*/

/* RT 2429, moved VCR support enum from MOD_VCR to here */
typedef enum {
    ERM_SIVR_NOT_SUPPORTED   = 0,
    ERM_SIVR_FULL_SUPPORTED  = 1,
    ERM_SIVR_LIMITED_SUPPORT = 2
}ERMSIVRSupportEnum;

typedef enum
{
    ERM_STATE_IDLE              = 0,
    ERM_STATE_START_UP,
    ERM_STATE_WAIT_ADDRESS,
    ERM_STATE_SSP_CONFIG,
    ERM_STATE_PSDM,
    ERM_STATE_CK5050_ACTIVE,
    ERM_STATE_CK5050_BT_ACTIVE
} ERMStateType;


typedef enum
{
    ERM_SW_UNDEF   = 0,
    ERM_SW_1xx     = 1,
    ERM_SW_200RC1A = 2,
    ERM_SW_200RC2  = 3,
    ERM_SW_200RC3  = 4,
    ERM_SW_200RC4  = 5,
    ERM_SW_NEWER   = 6
} ERMSwVersionType;

typedef enum
{
    ERM_NO_RESET      = 0,
    ERM_PENDING_RESET = 1,
    ERM_EXECUTE_RESET = 2,
    ERM_EXECUTE_RESET_PWNG4 = 3,
    ERM_EXECUTE_RESET_VR_WATCHDOG = 4
} ERMResetState;


typedef enum
{
    ERM_BT_UPDATE_NOT_STARTED,
    ERM_BT_UPDATE_MODE_RETRY_TRIGGER,
    ERM_BT_UPDATE_WORKAROUND_CK5050,
    ERM_BT_UPDATE_MODE_ACTIVATED,
    ERM_BT_UPDATE_TRANSFERING_FILE,
    ERM_BT_UPDATE_TRANSFER_FINISHED,
    ERM_BT_UPDATE_DECISION_RETRY_TRIGGER,
    ERM_BT_UPDATE_FINISHED,
    ERM_BT_UPDATE_ERROR
}ERM_enBTUpdateStates;


typedef enum
{
    ERM_BT_UPDATE_OFF,
    ERM_BT_UPDATE_TRIGGER_NORMAL_MODE,
    ERM_BT_UPDATE_TRIGGER_SECURED_MODE,
    ERM_BT_UPDATE_DEACTIVATE_SW_CAPABILITY,
    ERM_BT_UPDATE_START_TRANSFERING_OF_FILE,
    ERM_BT_UPDATE_SUCCESS_TRANSFERING_OF_FILE,
    ERM_BT_UPDATE_MD5_KEY_RESULT_OKAY,
    ERM_BT_UPDATE_CALL_CYCLICAL,
    ERM_BT_UPDATE_CK5050_REBOOT    // any restart of CK5050, this is good after update, bad in other cases

}ERM_enBTUpdateParams;


typedef enum
{
    ERM_BT_TESTMODE_OFF,
    ERM_BT_PBTESTMODE_SENDING,
    ERM_BT_PBFREQEX_SENDING,
    ERM_BT_TESTMODE_ACTIVE
}ERM_enBTTestmodeStates;

typedef enum
{
    ERM_BT_TESTMODE_SEND_PBTESTMODE,
    ERM_BT_TESTMODE_SEND_PBFREQEX,
    ERM_BT_TESTMODE_CALL_CYCLICAL
}ERM_enBTTestmodeParams;

typedef enum
{
    ERM_SSP_CONFIG_START,
    ERM_SSP_CONFIG_ASK,
    ERM_SSP_CONFIG_CHANGED,
    ERM_SSP_CONFIG_RESET,
    ERM_SSP_CONFIG_DONE
}ERM_enSSPConfigStates;

typedef enum {
    ERM_EXT_ERR_NO_ERROR = 0,
    ERM_EXT_ERR_UNKNOWN_ERROR = 1,                            // 1
    //EXTENDED ERROR CODE : MISTAKEN COMMAND
    ERM_EXT_ERR_SYNTAX_NOT_ALLOWED = 100,                     // 100
    ERM_EXT_ERR_ACTION_NEEDS_ANOTHER_PARAMETERS,              // 101
    ERM_EXT_ERR_ONE_OR_SEVERAL_WRONG_PARAMETERS,              // 102
    ERM_EXT_ERR_PARAMETER_OUT_OF_BOUNDS,                      // 103
    ERM_EXT_ERR_STRING_TOO_LONG,                              // 104
    ERM_EXT_ERR_STRING_TOO_SHORT_OR_INVALID_CHARACTERS,       // 105
    ERM_EXT_ERR_INVALID_BLUETOOTH_ADDRESS,                    // 106
    ERM_EXT_ERR_INVALID_DEVICE_ID,                            // 107
    ERM_EXT_ERR_DEVICE_NOT_CONNECTED,                         // 108
    ERM_EXT_ERR_DEVICE_CONNECTED,                             // 109
    ERM_EXT_ERR_BLUETOOTH_ADDRESS_ALREADY_LOADED,             // 110
    ERM_EXT_ERR_DEVICE_LETTER_NOT_EXPECTED,                   // 111
    ERM_EXT_ERR_INVALID_PARAMETER_IN_CURRENT_CONTEXT,         // 112
    ERM_EXT_ERR_TOO_MANY_PARAMETERS,                          // 113
    ERM_EXT_ERR_BAD_NUMBER_OF_ARGUMENTS,                      // 114
    ERM_EXT_ERR_INVALID_COMMAND_IN_CURRENT_CONTEXT,           // 115
    //EXTENDED ERROR CODE : WRONG INTERNAL STATE
    ERM_EXT_ERR_NO_ACTIVE_DEVICE = 150,                       // 150
    ERM_EXT_ERR_CK_IS_BOOTING,                                // 151
    ERM_EXT_ERR_CK_IS_PAIRING,                                // 152
    ERM_EXT_ERR_NO_CURRENT_PAIRING_REQUEST,                   // 153
    ERM_EXT_ERR_CK_SHALL_BE_IN_IDLE_STATE,                    // 154
    ERM_EXT_ERR_CK_SHALL_BE_IN_INQUIRY_STATE,                 // 155
    ERM_EXT_ERR_SYNCHRONIZATION_IN_PROGRESS,                  // 156
    ERM_EXT_ERR_THERE_IS_NO_PAIRED_DEVICE,                    // 157
    ERM_EXT_ERR_DEVICE_NOT_PAIRED,                            // 158
    ERM_EXT_ERR_INVALID_BITMASK_OR_NO_MATCHING_PAIRED_DEVICE, // 159
    ERM_EXT_ERR_USER_PHONEBOOK_NOT_SELECTED,                  // 160
    ERM_EXT_ERR_CALL_HISTORY_PHONEBOOK_NOT_SELECTED,          // 161
    ERM_EXT_ERR_INVALID_BLUETOOTH_STATUS,                     // 162
    ERM_EXT_ERR_AT_COMMAND_NOT_AUTHORIZED,                    // 163
    ERM_EXT_ERR_AUTOMATIC_PHONEBOOK_STORAGE_ACTIVE,           // 164
    ERM_EXT_ERR_PARAMETER_INCOHERENT_WITH_CURRENT_STATE,      // 165
    ERM_EXT_ERR_EXTENDED_SLC_NOT_READY,                       // 166
    ERM_EXT_ERR_INQUIRY_IN_PROGRESS,                          // 167
    ERM_EXT_ERR_INVALID_PHONEBOOK_TYPE_SELECTED,              // 168
    ERM_EXT_ERR_PHONEBOOK_SORTING_IS_PROGRESS,                // 169
    ERM_EXT_ERR_CK_ALREADY_TREATS_THE_SAME_COMMAND,           // 170
    ERM_EXT_ERR_BLUETOOTH_IS_OFF,                             // 171
    ERM_EXT_ERR_SYNCHRO_TABLE_NOT_AVAILABLE,                  // 172
    //EXTENDED ERROR CODE : BLUETOOTH SERVICE
    ERM_EXT_ERR_SERVICE_ALREADY_STARTED = 200,                // 200
    ERM_EXT_ERR_SERVICE_NOT_SUPPORTED,                        // 201
    ERM_EXT_ERR_SERVICE_NOT_STARTED,                          // 202
    ERM_EXT_ERR_HFP_NOT_STARTED,                              // 203
    ERM_EXT_ERR_AVRCP_NOT_STARTED,                            // 204
    ERM_EXT_ERR_DUN_NOT_STARTED,                              // 205
    ERM_EXT_ERR_A2DP_ALREADY_STARTED,                         // 206
    ERM_EXT_ERR_A2DP_NOT_STARTED,                             // 207
    ERM_EXT_ERR_AUDIO_GATEWAY_NOT_STARTED,                    // 208
    ERM_EXT_ERR_HFP_IS_STARTED,                               // 209
    ERM_EXT_ERR_MAXIMUM_NUMBER_OF_AVDTP_REACHED,              // 210
    ERM_EXT_ERR_KEY_FOR_SCMS_T_NOT_GENERATED,                 // 211
    ERM_EXT_ERR_TYPE_OF_METADATA_YET_UNKNOWN,                 // 212
    //EXTENDED ERROR CODE : CALL MANAGEMENT
    ERM_EXT_ERR_NO_ONGOING_CALL = 250,                        // 250
    ERM_EXT_ERR_NO_WAITING_CALL,                              // 251
    ERM_EXT_ERR_DIALING_OR_WAITING_CALL,                      // 252
    ERM_EXT_ERR_CALL_IN_PROGRESS,                             // 253
    ERM_EXT_ERR_ONLY_ONE_CALL_OR_CALLS_IN_BAD_STATE,          // 254
    ERM_EXT_ERR_TOO_MANY_ONGOING_CALLS,                       // 255
    ERM_EXT_ERR_CALL_IN_HAND_FREE_MODE,                       // 256
    ERM_EXT_ERR_CALL_IN_DISCRETE_MODE,                        // 257
    ERM_EXT_ERR_HFP15_OR_REQUESTED_FEATURE_NOT_SUPPORTED,     // 258 // by the phone
    ERM_EXT_ERR_NO_ACTIVE_CALL,                               // 259
    ERM_EXT_ERR_HELD_DIALING_INCOMMING_OR_WAITING_CALL,       // 260
    ERM_EXT_ERR_NO_HELD_CALL,                                 // 261
    ERM_EXT_ERR_HELD_CALL_CANNOT_BE_HANGUP,                   // 262
    ERM_EXT_ERR_ALREADY_ONE_CALL_IN_STATE_INCOMING_HELD,      // 263
    //EXTENDED ERROR CODE : AUDIO STATE
    ERM_EXT_ERR_SOURCE_ACTIVE = 300,                          // 300
    ERM_EXT_ERR_NO_STREAMING_IN_PROGRESS,                     // 301
    ERM_EXT_ERR_SCO_OVER_BLUETOOTH_IN_PROGRESS,               // 302
    ERM_EXT_ERR_I2S_INPUT_ALREADY_ENABLED,                    // 303
    ERM_EXT_ERR_I2S_INPUT_ALREADY_DISABLED,                   // 304
    ERM_EXT_ERR_STREAMING_HOST_2_LOCAL_STARTED,               // 305
    ERM_EXT_ERR_STREAMING_HOST_2_LOCAL_NOT_STARTED,           // 306
    ERM_EXT_ERR_STREAMING_MICROPHONE_2_HEADSET_STARTED,       // 307
    ERM_EXT_ERR_STREAMING_MICROPHONE_2_HEADSET_NOT_STARTED,   // 308
    ERM_EXT_ERR_AUDIO_FROM_USB_INPUT_ENABLED,                 // 309
    ERM_EXT_ERR_AUDIO_FROM_USB_INPUT_DISABLED,                // 310
    ERM_EXT_ERR_FUNK_MODE_ENABLED,                            // 311
    ERM_EXT_ERR_FUNK_MODE_DISABLED,                           // 312
    ERM_EXT_ERR_WRONG_AUDIO_PATH,                             // 313
    ERM_EXT_ERR_AUDIO_ALREADY_LOCKED,                         // 314
    ERM_EXT_ERR_AUDIO_ALREADY_UNLOCKED,                       // 315
    ERM_EXT_ERR_I2S_OUTPUT_ALREADY_STARTED,                   // 316
    ERM_EXT_ERR_I2S_OUTPUT_ALREADY_DISABLED,                  // 317
    ERM_EXT_ERR_AUDIO_SOURCE_NOT_ACTIVE,                      // 318
    //EXTENDED ERROR CODE : RECO
    ERM_EXT_ERR_NO_VOCAL_RECOGNITION_IN_PROGRESS = 350,       // 350
    ERM_EXT_ERR_CONTACT_TRAINING_IN_PROGRESS,                 // 351
    ERM_EXT_ERR_NO_CONTACT_TRAINING_IN_PROGRESS,              // 352
    ERM_EXT_ERR_VOCAL_RECOGNITION_NOT_SUPPORTED,              // 353 // by the phone
    ERM_EXT_ERR_CONTACT_NOT_TRAINED,                          // 354
    ERM_EXT_ERR_KEYWORD_NOT_TRAINED,                          // 355
    ERM_EXT_ERR_NO_TRAINED_CONTACT_IN_PHONEBOOK,              // 356
    ERM_EXT_ERR_PARROT_RECOGNITION_SHOULD_BE_SELECTED,        // 357
    ERM_EXT_ERR_PHONE_RECOGNITION_IN_PROGRESS,                // 358
    //EXTENDED ERROR CODE : SMS
    ERM_EXT_ERR_SMS_FUNCTIONALITY_NOT_STARTED = 400,          // 400
    ERM_EXT_ERR_RETRIEVING_THE_PHONE_SMS,                     // 401
    ERM_EXT_ERR_FAILURE_DURING_SMS_READING,                   // 402
    ERM_EXT_ERR_FAILURE_DURING_SMS_SENDING,                   // 403
    ERM_EXT_ERR_FAILURE_DURING_SMS_DELETING,                  // 404
    //EXTENDED ERROR CODE : SOPRANO
    ERM_EXT_ERR_SIVR_FUNCTIONALITY_NOT_READY = 450,           // 450
    ERM_EXT_ERR_SIVR_TTS_NOT_PLAYING,                         // 451
    ERM_EXT_ERR_SIVR_RECOGNITION_NOT_IN_PROGRESS,             // 452
    ERM_EXT_ERR_SIVR_RECOGNITION_IN_PROGRESS,                 // 453
    ERM_EXT_ERR_SIVR_AUTO_CONTINUE_MODE_ENABLED,              // 454
    ERM_EXT_ERR_SIVR_RECOGNITION_HAS_LOWER_AUDIO_PRIORITY,    // 455
    ERM_EXT_ERR_SIVR_TTS_ALREADY_PLAYING,                     // 456
    ERM_EXT_ERR_NO_VOICETAG,                                  // 457
    ERM_EXT_ERR_SIVR_CONTACT_SYNC_FAILURE,                    // 458
    //EXTENDED ERROR CODE : DISCO
    ERM_EXT_ERR_DATABASE_NOT_READY = 500,                     // 500
    ERM_EXT_ERR_DATABASE_IS_OPENED,                           // 501
    ERM_EXT_ERR_DATABASE_DOES_NOT_SUPPORT_BROWSING,           // 502
    ERM_EXT_ERR_INVALID_DATABASE_LETTER,                      // 503
    ERM_EXT_ERR_INVALID_DATABASE_ID,                          // 504
    ERM_EXT_ERR_DATABASE_NOT_OPENED,                          // 505
    ERM_EXT_ERR_PLAYER_IS_NOT_PLAYING,                        // 506
    ERM_EXT_ERR_PLAYER_IS_NOT_STOPPED,                        // 507
    ERM_EXT_ERR_DISCO_INIT_IS_NOT_DONE,                       // 508
    ERM_EXT_ERR_COMMAND_NOT_SUPPORTED,                        // 509
    ERM_EXT_ERR_CONTEXT_RESTORATION_DISABLED,                 // 510
    ERM_EXT_ERR_NO_VALID_CONTEXT,                             // 511
    ERM_EXT_ERR_TOO_MANY_OPENED_DATABASES,                    // 512
    ERM_EXT_ERR_DATABASE_IS_OPENING,                          // 513
    //EXTENDED ERROR CODE : MISCELLANEOUS
    ERM_EXT_ERR_ACTION_FAILED_WITHOUT_CAUSE = 550,            // 550
    ERM_EXT_ERR_MUX_ALREADY_STARTED,                          // 551
    ERM_EXT_ERR_MUX_NOT_STARTED,                              // 552
    ERM_EXT_ERR_MUX_KEEPS_STARTED_BECAUSE_STILL_NEEDED,       // 553
    ERM_EXT_ERR_PLOOPBACK_ALREADY_STARTED,                    // 554
    ERM_EXT_ERR_PLOOPBACK_NOT_STARTED,                        // 555
    ERM_EXT_ERR_DUN_UPDATE_ALREADY_STARTED,                   // 556
    ERM_EXT_ERR_USB_UPDATE_ALREADY_STARTED,                   // 557
    ERM_EXT_ERR_FEATURE_NOT_SUPPORTED,                        // 558
    ERM_EXT_ERR_INPUT_FILE_NOT_ALLOWED_FOR_THIS_COMMAND,      // 559
    ERM_EXT_ERR_INPUT_FILE_DOES_NOT_EXIST,                    // 560
    //EXTENDED ERROR CODE : ALGO
    ERM_EXT_ERR_FILTER_NOT_IN_RX_CHAIN = 600,                 // 600
    ERM_EXT_ERR_FILTER_NOT_IN_TX_CHAIN,                       // 601
    //EXTENDED ERROR CODE : MAP
    ERM_EXT_ERR_MAP_ACCOUNT_NOT_CONNECTED = 800,              // 800
    ERM_EXT_ERR_MAP_ACCOUNT_BUSY,                             // 801
    ERM_EXT_ERR_MAP_ACCOUNT_INVALID,                          // 802
    ERM_EXT_ERR_MAP_CACHE_STATE_ERROR,                        // 803
    ERM_EXT_ERR_MAP_NO_LISTING_IN_PROGRESS,                   // 804
    ERM_EXT_ERR_MAP_ANOTHER_LISTING_IN_PROGRESS,              // 805
    ERM_EXT_ERR_MAP_MISC_ERROR,                               // 806
    ERM_EXT_ERR_MAP_DISCOVER_ALREADY_IN_PROGRESS,             // 807
    //EXTENDED ERROR CODE : MEMO
    ERM_EXT_ERR_NOT_ENOUGH_FREE_SPACE_ON_FLASH_MEMORY = 900,  // 900
    ERM_EXT_ERR_MAX_MEMO_RECORDING_DURATION_REACHED,          // 901
    ERM_EXT_ERR_INVALID_RECORD_ID                             // 902
} ERMExtendedErrorCode;
/*----------------------------------------------------------------------------*/
/* Public Types                                                               */
/*----------------------------------------------------------------------------*/
typedef Uint8Type ERM_UserProfileId;
typedef Uint8Type ERM_HeadsetPlayerProfileId;


#define ERM_CK5050SW_VERSION_MAX_LEN  30
#define ERM_PF_CODE_MAX_LEN  32

typedef Uint8Type ERM_CK5050SWVersion[ERM_CK5050SW_VERSION_MAX_LEN+1];

typedef Uint8Type ERM_PF_Code[ERM_PF_CODE_MAX_LEN+1];

typedef struct {
    ERM_CK5050SWVersion     version;
    Uint16Type              version_len;
    Uint16Type              u16Description;
} ERMSWVersionStringType;

typedef struct {
    ERM_PF_Code             pfcode;
    Uint16Type              pfcode_len;
} ERMPF_CodeStringType;

typedef enum
{
    ERM_DEVICE_STATE_EMPTY      = 0,
    ERM_DEVICE_STATE_USED,
    ERM_DEVICE_STATE_RELEASED
} ERMUserListDeviceType;

/* NOTE: For EPROM layout this structure MUST have a size of 64 byte,
 * therefore if new members are added each time check the size !!! */
typedef struct {
    /* note: as name_len is only 16 bit value it is added in front to have
     * structure packed */
    Uint16Type           name_len;     /* length of the name */
    AT_DeviceNameString  name;         /* BT device name which is also used for each user profile that
                                          refers to this physical device */
    AT_DeviceId          deviceID;     /* device id in range 0..x 0xFF -> invalid */
    ATPinCodeBT          pincode;      /* Note: Pincode is limited to 4(+1) in ATCommonData.h */
    Uint8Type            numVoiceTags; /* number of recorded voice tags for this device */
    BooleanType          bVTDeleteOngoing;
    AT_BT_Address        bt_addr;
    Uint8Type            spare[11];
} ERMPhysicalBTDeviceType;


typedef struct {
    /* device informations */
    ERMUserListDeviceType   device_state;
//    ATBooleanEnum           device_ERM_active;
    ATDevConnStatusEnum     device_connected;
    ATBooleanEnum           device_active;
    ATRintoneModeEnum       device_ringtone;
    ATBooleanEnum           device_ringtone_alternate;
    ATSupportedEnum         support_dial;
    ATSupportedEnum         support_pb_access;

    ATSupportedEnum         phone_supported;
    ATBooleanEnum           phone_active;

    /*ERM_FULL_DEVICE_TABLE is set in file ucopts*/
//#ifdef ERM_FULL_DEVICE_TABLE
    Uint8Type           device_name_len;
    AT_DeviceNameString device_name;
    AT_BT_Address       bt_addr;
    AT_BTClassOfDevice  bt_ClassOfDevice;
    ATBooleanEnum       bAudioVolumeSet;
    /* service indicators */

    ATSupportedEnum     dun_supported;
    ATBooleanEnum       dun_active;
    ATSupportedEnum     a2dp_sink_supported;
    ATBooleanEnum       a2dp_sink_active;
    ATSupportedEnum     a2dp_source_supported;
    ATBooleanEnum       a2dp_source_active;
    ATSupportedEnum     avrcp_supported;
    ATBooleanEnum       avrcp_active;
    ATSupportedEnum     audio_gw_supported;
    ATBooleanEnum       audio_gw_active;
    ATSupportedEnum     spp_supported;
    ATBooleanEnum       spp_active;
    ATBooleanEnum       sdp_received;
//#endif /* ERM_FULL_DEVICE_TABLE */
} ERMBTDeviceTableType;

typedef enum
{
    ERM_AUDIO_MODE_CALLS_ONLY = 0,
    ERM_AUDIO_MODE_ALL_AUDIO
}ERMUserAudioModeType;

/* NOTE: For EPROM layout this structure MUST have a size of 20 byte,
 * or max 24 byte if "old" active user list is removed!!!
 * therefore if new members are added each time check the size !!! */
typedef struct {
    AT_DeviceId  UserProfileList[ERM_MAX_NUM_USER_PROFILES];
    AT_DeviceId  PlayerProfileList[ERM_MAX_NUM_PLAYER_PROFILE];
    AT_DeviceId  HeadsetProfileList[ERM_MAX_NUM_HEADSET_PROFILE];
} ERMActiveProfileListType;


/* NOTE: For EPROM layout this structure MUST have a size of 64 byte,
 * therefore if new members are added each time check the size !!! */
typedef struct {
    Uint8Type               deviceIndex;        /* index to physical device table; range 0..ERM_MAX_PHYSICAL_DEVICE_IDX */
    Uint8Type               playerIndex;        /* index to player table    */
    Uint8Type               headSetIndex;       /* index to head set table */
    ATPhoneNumberType       voicemailbox;       /* additional phone number for mailbox */
    Uint8Type               voicemailbox_len;   /* length of voicemailbox number */
    ATRintoneModeEnum       ringtone;           /* ringtone value - we also can use ATRintoneModeEnum as we have small enums  */
    BooleanType             ringtone_alternating; /* which ringtone option to be used - likely also enum will be needed  */
    ERMUserAudioModeType    audio_mode;   /* phone audio mode */
    BooleanType             bPbLoadedCompletly;
    Uint8Type               spare[23];
} ERMUserProfileType;


typedef struct
{
    ATPairingCommandEnum    command;
    AT_DeviceId             deviceId;
    ATPairingDeviceType     deviceType;
    BooleanType             pinAvailable;
    ATPinCodeBT             pin;
    Uint8Type               numPinDigits;
}ERMPairingType;

typedef struct
{
    ATRintoneModeEnum       ringtoneId;
    ATPprtActionType        action;
}ERMRingtonePlayType;

typedef struct
{
    BooleanType             bIsValid;
    ATPbcvHwVersion         hw_version;
    ATPbcvSwVersion         sw_version;
    ATPbcvManufacturer      manufacturer;
}ERMBtChipVersionType;

typedef struct
{
    AT_DeviceId             remote_id;      
    ATPairingDeviceType     device_type;
    AT_BTDeviceName         device_name;
    Uint16Type              name_len;
}ERMPairReqType;

typedef struct
{
    AT_DeviceId               remote_id;      
    ATPairingDeviceType       device_type;
    ATSSPPinCodeBT            passphrase;  
    ATSSPModeEnum             mode;        
    ATSSPIoCapParameterEnum   capability;
}ERMSspReqType;

typedef struct
{
    ERMNotificationIdType id;

}ERMNotificationCommonType;

typedef struct
{
    ERMNotificationCommonType   common;
    AT_DeviceId                 device_id;
    ATBTServiceResultEnum       result;
}ERMNotificationPbsnType;

typedef struct
{
    ERMNotificationCommonType   common;
    AT_DeviceId                 device_id;
    ATConnStatusEnum            status;
}ERMNotificationPsbdType;

typedef struct
{
    ERMNotificationCommonType   common;
    AT_DeviceId                 device_id;
    ATPairingResultEnum         result;
}ERMNotificationPprsType;

typedef struct
{
    ERMNotificationCommonType   common;
    AT_DeviceId                 device_id;
}ERMNotificationPbscType;

typedef struct
{
    ERMNotificationCommonType       common;
    AT_DeviceId                     device_id;
    ATBTFeatureEnum                 feature;
    ATBTFeatureSupportResultEnum    result;
}ERMNotificationPbsfType;

typedef union
{
    ERMNotificationCommonType   common;
    ERMNotificationPbsnType     pbsn_phone;
    ERMNotificationPbsnType     pbsn_a2dp;
    ERMNotificationPbsnType     pbsn_avrcp;
    ERMNotificationPsbdType     psbd;
    ERMNotificationPprsType     pprs;
    ERMNotificationPbscType     pbsc;
    ERMNotificationPbsfType     pbsf;
}ERMNotificationType;





/* NOTE: For EPROM layout this structure MUST have a size of 4 byte,
 * therefore if new members are added each time check the size !!! */
#include "APP_DEVICE.h"
typedef struct {
    AT_DeviceId              deviceIndex; /* index to physical device table; range 0..9 */
    APP_DEVICE_DeviceType    deviceType;  /* device type to identify headset or player  */
    BooleanType              bAuthNeeded; /* indicate if authentication is required for this device */
    BooleanType              bAutoStartPlay; /* indicate if play shall be resumed */
} ERMPlayerHeadsetProfileType;


typedef struct
{
    BooleanType             isValid;
    ATRspGetInquiryResType  data;
}ERMInquiryDataType;

typedef struct
{
    Uint16Type              startIndex;
    Uint16Type              endIndex;
    APP_DEVICE_DeviceType   deviceType;
    Uint16Type              reqStartIndex;
    Uint16Type              reqEndIndex;
    Uint16Type              recvdCount;
    ERMInquiryDataType      result[MAX_REQUESTED_INQUIRY_RESULT_ENTRIES];
}ERMInquiryResultType;



typedef struct
{
    Uint16Type              deviceId;
}ERMDiscPhoneServiceType;

typedef ERMDiscPhoneServiceType ERMConnPhoneServiceType;
typedef ERMDiscPhoneServiceType ERMConnPlayerAvrcpServiceType;
typedef ERMDiscPhoneServiceType ERMConnPlayerA2dpSinkServiceType;
typedef ERMDiscPhoneServiceType ERMDeleteDeviceType;
typedef ERMDiscPhoneServiceType ERMConnectDeviceType;
typedef ERMDiscPhoneServiceType ERMDisconnectDeviceType;
typedef ERMDiscPhoneServiceType ERMDiscPlayerAvrcpServiceType;
typedef ERMDiscPhoneServiceType ERMDiscPlayerA2dpSinkServiceType;
typedef ERMDiscPhoneServiceType ERMDiscHeadSetServiceType;
typedef ERMDiscPhoneServiceType ERMStartHeadSetServiceType;

typedef struct
{
    BooleanType bPlayCallerName;
}ERMSetPlayCallerNameType;

typedef ERMSetPlayCallerNameType ERMGetPlayCallerNameType;

typedef struct
{
    BooleanType bAllAudio;
}ERMSetAudioModeType;

typedef ERMSetAudioModeType ERMGetAudioModeType;


/*----------------------------------------------------------------------------*/
/* Global Variables Declarations (avoid as much as possible!)                 */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global const Declarations                                                  */
/*----------------------------------------------------------------------------*/

#define ERM_BARRING_OFF             0
#define ERM_BARRING_ON              1
/*----------------------------------------------------------------------------*/
/* Public Methods                                                             */
/*----------------------------------------------------------------------------*/

extern void          ERM_vReInitAudioParameterSet(Uint8Type u8NewAudioSetIdx);

extern Uint8Type                    ERM_GetMicGain(void);
extern Uint8Type                    ERM_GetPromptVolume(void);
extern Uint8Type                    ERM_GetCommVolume(void);
extern Uint8Type                    ERM_GetStreamVolume(void);
extern Uint8Type                    ERM_GetAlcVolume(void);
extern APP_COMMON_enResponseCode    ERM_sFSetAudioParameterSet(Uint8Type AudioParamterSet,\
        void (*cb)(APP_COMMON_enResponseCode));

void ERM_Manager(EventMaskType events);
extern SuccessFailType ERM_Startup(void);
extern void ERM_sysPerfResetStartUpEventTraceDone(Uint8Type reset);
extern SuccessFailType ERM_Shutdown(void);
extern ERMSwVersionType ERM_GetCK5050SWVersion(void);
extern const SCI_ST*  ERM_GetUartForTel(void);
extern const SCI_ST*  ERM_GetUartForVCR(void);
extern const SCI_ST*  ERM_GetUartForMDI(void);
extern const SCI_ST*  ERM_GetUartForSMS(void);
extern const SCI_ST*  ERM_GetUartForERM(void);
extern const SCI_ST*  ERM_GetUartForDevice(void);
extern const SCI_ST*  ERM_GetUartForPhonebook(void);
extern SuccessFailType ERM_CheckUartOwnership(APPSubModuleIdEnum owner,const SCI_ST* ptr );

extern SuccessFailType ERM_ReleaseUartForTel(void);
extern SuccessFailType ERM_ReleaseUartForVCR(void);
extern SuccessFailType ERM_ReleaseUartForMDI(void);
extern SuccessFailType ERM_ReleaseUartForSMS(void);
extern SuccessFailType ERM_ReleaseUartForPhonebook(void);
extern SuccessFailType ERM_ReleaseUartForErm(void);
extern SuccessFailType ERM_ReleaseUartForDevice(void);

extern SuccessFailType ERM_GetActiveBTDeviceId(AT_DeviceId * device_id);
extern void ERM_DeleteDeviceIfPpdeTimeout(AT_DeviceId device_id);

extern SuccessFailType ERM_FlashErase(void);
extern void            ERM_BTVisibilityBarringOn(Uint8Type value);

extern BooleanType ERM_bGetBTAckSignalActive(void);


extern void                         ERM_vBTVisibilityBarringOn(Uint8Type value);
extern void                         ERM_vCb3BM_BTVisibility(void);
extern void                         ERM_vBTVisibilityOff(void);
extern Uint16Type                   ERM_u16GetDeviceCount(void);
extern BooleanType                  ERM_bPhoneAvailable(void);
extern ERMSIVRSupportEnum           ERM_enIsVcrSupported(void);          /* RT 2429 */
Uint8Type                           ERM_u8GetTotalInquiryResult(void);
extern Uint8Type                    ERM_u8GetInquiryResult(APP_DEVICE_DeviceType deviceType);
extern const ERMInquiryDataType *   ERM_pGetPgirContext(Uint16Type index);

extern void                         ERM_GetBTPinCodeAsChar(ATPinCodeType * pinCode);

extern const ERMBTDeviceTableType       *ERM_GetBTDevice(AT_DeviceId deviceId);
extern const Uint8Type                  *ERM_GetBTDeviceName();
extern AT_DeviceId                      ERM_GetDeviceIdOfUmpteenthPhone(Uint16Type selIdx);
extern Uint16Type                       ERM_GetDeviceIdOfUmpteenthHeadset(Uint16Type selIdx);
extern Uint16Type                       ERM_GetDeviceIdOfUmpteenthPlayer(Uint16Type selIdx);
extern ATRintoneModeEnum                ERM_eGetUmpteenthRingtone(Uint16Type selIdx);
extern Uint16Type                       ERM_GetIndexForRingtone(ATRintoneModeEnum ringtone);
extern Uint16Type                       ERM_u16GetNumberOfRingtones(void);
extern AT_DeviceId                      ERM_GetLastPairedDeviceId();
extern AT_DeviceId                      ERM_GetLastServiceDeviceId();
extern AT_DeviceId                      ERM_GetDeviceIdOfBtAddr(const AT_BT_Address btAddr);

extern BooleanType ERM_bIsPlayer(AT_DeviceId deviceId);
extern BooleanType ERM_bIsHeadset(AT_DeviceId deviceId);
extern BooleanType ERM_bIsPhone(AT_DeviceId deviceId);
extern BooleanType ERM_bSdpReceived(AT_DeviceId deviceId);
//extern Uint16Type ERM_u16GetNumberOfPairedHeadSets();
//extern Uint16Type ERM_u16GetNumberOfPairedPlayer();
//extern Uint16Type ERM_u16GetNumberOfPairedPhones();

extern AT_DeviceId ERM_u8GetActiveHeadSet();
extern AT_DeviceId ERM_u8GetActivePlayer();
extern AT_DeviceId ERM_u8GetActivePhone();

extern AT_DeviceId ERM_u8GetLastActiveHeadSet();
extern AT_DeviceId ERM_u8GetLastActivePlayer();

extern Uint8Type ERM_u8GetOldestNoActiveUserProfileId(void);
extern Uint8Type ERM_u8GetOldestUserProfileId();
extern Uint8Type ERM_u8GetOldestPlayerProfileId();
extern Uint8Type ERM_u8GetOldestHeadsetProfileId();
extern BooleanType ERM_vCheckforprofileId(AT_DeviceId id);//RT13844 Test

BooleanType ERM_bIsStartupDone(void);
Uint8Type ERM_u8GetBluetoothUpdateActive (void);

extern BooleanType ERM_bDevConnectedOnBtLevel(AT_DeviceId devId);

extern BooleanType ERM_bSetDeviceName(AT_DeviceId deviceId, const Uint8Type *pName, Uint16Type length);
extern AT_DeviceId ERM_GetDeviceId_BtAddr(const AT_BT_Address *pBtAddress);

extern Uint8Type ERM_u8GetUserProfileIndexForDeviceId(AT_DeviceId deviceId);
extern Uint8Type ERM_u8GetPlayerHeadsetProfileIndexForDeviceId(AT_DeviceId deviceId);


extern APP_COMMON_enResponseCode ERM_enGetBTAddress(
        AT_BT_Address * p_address);

extern APP_COMMON_enResponseCode ERM_rcResetToFactorySetting(
        void (*cb)(APP_COMMON_enResponseCode));
extern APP_COMMON_enResponseCode ERM_eSetTelephoneBasicVolumeComm(
        void (*cb)(APP_COMMON_enResponseCode),
        Uint8Type value);
extern APP_COMMON_enResponseCode ERM_eSetTelephoneBasicVolumeAlc(
        void (*cb)(APP_COMMON_enResponseCode),
        Uint8Type value);
extern APP_COMMON_enResponseCode ERM_eSetVoiceOutputBasicVolume(
        void (*cb)(APP_COMMON_enResponseCode),
        Uint8Type value);
extern APP_COMMON_enResponseCode ERM_eSetMediaDeviceBasicVolume(
        void (*cb)(APP_COMMON_enResponseCode),
        Uint8Type value);
extern APP_COMMON_enResponseCode ERM_eSetMicrophoneSensitivity(
        void (*cb)(APP_COMMON_enResponseCode),
        Uint8Type value);

extern APP_COMMON_enResponseCode ERM_sFSetBluetoothSetPINCode(
        void (*cb)(APP_COMMON_enResponseCode),
        Uint16Type pincode);

extern APP_COMMON_enResponseCode ERM_sFSetBluetoothSetPINCodeDiagnose(
        void (*cb)(APP_COMMON_enResponseCode),
        Uint8Type *pau8Pincode);

extern APP_COMMON_enResponseCode ERM_sFSetUniversalPreparationForMobileTelephone(
        void (*cb)(APP_COMMON_enResponseCode),
        const Uint8Type* data,
        Uint16Type dataLen);

extern APP_COMMON_enResponseCode ERM_sFSetBluetoothAcknowledgementSignal(
        void (*cb)(APP_COMMON_enResponseCode),
        BooleanType value);

extern APP_COMMON_enResponseCode ERM_sFSetReferenceChannelDelay(
        void (*cb)(APP_COMMON_enResponseCode),
        Uint8Type value);

/* Inquiry */
extern APP_COMMON_enResponseCode ERM_rcStartInquiry(
        void (*cb)(APP_COMMON_enResponseCode));

extern APP_COMMON_enResponseCode ERM_rcCancelInquiry(
        void (*cb)(APP_COMMON_enResponseCode));

extern APP_COMMON_enResponseCode ERM_rcGetInquiryResult(
        void (*cb)(APP_COMMON_enResponseCode),
        APP_DEVICE_DeviceType deviceType,
        Uint8Type start,
        Uint8Type number);

extern APP_COMMON_enResponseCode ERM_rcInitiatePairing(
        AT_DeviceId           deviceId,
        ATPairingDeviceType   deviceType,
        const ATPinCodeBT           pin,
        Uint8Type             numPinDigits,
        void (*cb)(APP_COMMON_enResponseCode));

extern APP_COMMON_enResponseCode ERM_rcAcceptPairing(
        AT_DeviceId           deviceId,
        ATPairingDeviceType   deviceType,
        const ATPinCodeBT           pin,
        Uint8Type             numPinDigits,
        void (*cb)(APP_COMMON_enResponseCode));

extern APP_COMMON_enResponseCode ERM_rcRejectPairing(
        AT_DeviceId           deviceId,
        ATPairingDeviceType   deviceType,
        void (*cb)(APP_COMMON_enResponseCode));

extern APP_COMMON_enResponseCode ERM_rcStopHeadsetService(
        Uint16Type deviceId,
        void (*cb)(APP_COMMON_enResponseCode));

extern APP_COMMON_enResponseCode ERM_rcStartHeadsetService(
        Uint16Type deviceId,
        void (*cb)(APP_COMMON_enResponseCode));

extern APP_COMMON_enResponseCode ERM_rcStopPlayerService(
        Uint16Type deviceId,
        void (*cb)(APP_COMMON_enResponseCode));

extern APP_COMMON_enResponseCode ERM_rcStopPhoneService(
        Uint16Type deviceId,
        void (*cb)(APP_COMMON_enResponseCode));

extern APP_COMMON_enResponseCode ERM_rcStartPhoneService(
        Uint16Type deviceId,
        void (*cb)(APP_COMMON_enResponseCode));

extern APP_COMMON_enResponseCode ERM_rcConnectDevice(
        Uint16Type deviceId,
        void (*cb)(APP_COMMON_enResponseCode));

extern APP_COMMON_enResponseCode ERM_rcDisconnectDevice(
        Uint16Type deviceId,
        void (*cb)(APP_COMMON_enResponseCode));

extern APP_COMMON_enResponseCode ERM_rcSetAutoConnMode(
        BooleanType enable,
        void (*cb)(APP_COMMON_enResponseCode));


extern APP_COMMON_enResponseCode ERM_rcDeactivatePhoneServiceAutoConn(
        void (*cb)(APP_COMMON_enResponseCode));

APP_COMMON_enResponseCode ERM_rcStopPlayerAvrcpService(Uint16Type deviceId,
        void (*cb)(APP_COMMON_enResponseCode));

APP_COMMON_enResponseCode ERM_rcStartPlayerAvrcpService(Uint16Type deviceId,
                                        void (*cb)(APP_COMMON_enResponseCode));

APP_COMMON_enResponseCode ERM_rcStopPlayerA2dpSinkService(Uint16Type deviceId,
        void (*cb)(APP_COMMON_enResponseCode));

APP_COMMON_enResponseCode ERM_rcStartPlayerA2dpService(Uint16Type deviceId,
                                        void (*cb)(APP_COMMON_enResponseCode));

ERMUserAudioModeType ERM_enGetActiveUserAudioMode(void);

void ERM_set_AutoConnectionMode(BooleanType bauto);

/* functions for audio tests and test mode */
extern APP_COMMON_enResponseCode ERM_eTestSetMicAdcMode(
        void (*cb)(APP_COMMON_enResponseCode),
        Uint8Type               value);

extern APP_COMMON_enResponseCode ERM_eTestSetMicAdcGain(
        void (*cb)(APP_COMMON_enResponseCode),
        Uint8Type               value);

extern APP_COMMON_enResponseCode ERM_eTestSetLoopback(
        void (*cb)(APP_COMMON_enResponseCode),
        BooleanType loopback_on);

extern APP_COMMON_enResponseCode ERM_eSetAudioLoopTest(
        void (*cb)(APP_COMMON_enResponseCode),
        const AT_BT_Address bt_addr,
        Uint16Type    delay_local,
        Uint16Type    delay_BT );

extern APP_COMMON_enResponseCode ERM_eSetAudioChannel(
        void (*cb)(APP_COMMON_enResponseCode),
        ATReqAlternatingSignalType agsw );

extern APP_COMMON_enResponseCode ERM_eGetExtRevision(
        void (*cb)(APP_COMMON_enResponseCode));

extern APP_COMMON_enResponseCode ERM_eSetCK5050ToTestMode(
        void (*cb)(APP_COMMON_enResponseCode));

extern APP_COMMON_enResponseCode ERM_eSetCK5050ToTxRxTest(
        void (*cb)(APP_COMMON_enResponseCode));

extern APP_COMMON_enResponseCode ERM_rcBTSWUpdateCK5050(
        void (*cb)(APP_COMMON_enResponseCode));

extern APP_COMMON_enResponseCode ERM_rcSetConnectionAuthorization(
        void (*cb)(APP_COMMON_enResponseCode));

extern APP_COMMON_enResponseCode ERM_ePlayRingtone(
        void (*cb)(APP_COMMON_enResponseCode),
        ATPprtActionType action,
        ATRintoneModeEnum ringtone);


extern BooleanType ERM_bIsBluetoothVisible(void);
extern BooleanType ERM_bIsSecurityCheckOK (void);

extern void ERM_vsetActiveCallFlagForSettingWithoutCI(BooleanType bActiveCall);

extern BooleanType ERM_bgetActiveCallFlagForSettingWithoutCI(void);

extern ATBTSWUpdateRecEnum ERM_enGetBluetoothUpdateRequest (void);
extern void ERM_vBluetoothUpdateProcessCK5050Status(ATBTSWUpdateResEnum UpdateEvent);
extern void ERM_vOnErmNotification(const ERMNotificationType *pNotify);
extern void ERM_vBluetoothUpdateStateMachine(ERM_enBTUpdateParams Parameter);
extern void ERM_vBluetoothUpdateSecurityCheck (void);



extern BooleanType ERM_bPlayerIdIsInphoneList(Uint8Type deviceId);

extern Uint8Type ERM_GetLastActiveProfileId( Uint8Type activePos,
                                             APP_DEVICE_DeviceType profileType);


/* new prototypes for user profile access */
extern const ERMBTDeviceTableType        *ERM_pGetDevice(Uint8Type index);
extern const ERMPhysicalBTDeviceType     *ERM_pGetPhyDevice(Uint8Type index);
extern const ERMUserProfileType          *ERM_pGetUserProfile(Uint8Type index);
extern const ERMPlayerHeadsetProfileType *ERM_pGetPlayerHeadsetProfile(Uint8Type index);
extern const ERMUserProfileType          *ERM_pGetUserProfile_DevId(AT_DeviceId index);
extern const ERMPlayerHeadsetProfileType *ERM_pGetPlayerHeadsetProfile_DevId(AT_DeviceId index);

extern const ERMPairReqType              *ERM_pGetPairRequest();
extern const ERMSspReqType               *ERM_pGetSspRequest();


extern APP_COMMON_enResponseCode ERM_rcGetCKSWVersionString(ERMSWVersionStringType * p_version);
extern APP_COMMON_enResponseCode ERM_rcGetPF_CodeString(ERMPF_CodeStringType * p_pfcode);
extern APP_COMMON_enResponseCode ERM_rcGetBtChipVersion(ERMBtChipVersionType * p_btChipVers);



extern Uint8Type  ERM_u8GetUserProfileId(AT_DeviceId index);
extern Uint8Type  ERM_u8GetPlayerHeadsetProfileId(AT_DeviceId index);


extern BooleanType ERM_bDeviceAlreadyKnownAsHeadset(AT_DeviceId deviceId);
extern BooleanType ERM_bDeviceAlreadyKnownAsPlayer(AT_DeviceId deviceId);

extern const ERMUserProfileType          *ERM_pGetActiveUserProfile();
extern const ERMPlayerHeadsetProfileType *ERM_pGetActivePlayerProfile();
extern const ERMPlayerHeadsetProfileType *ERM_pGetActiveHeadsetProfile();

extern BooleanType ERM_bMaxNumberOfUserProfilesReached();
extern BooleanType ERM_bMaxNumberOfPlayerProfilesReached();
extern BooleanType ERM_bMaxNumberOfPlayerHeadsetProfilesReached();
extern BooleanType ERM_bMaxNumberOfHeadsetProfilesReached();

extern Uint16Type ERM_u16GetNumberOfUserProfiles();
extern Uint16Type ERM_u16GetNumberOfPlayerProfiles();
extern Uint16Type ERM_u16GetNumberOfHeadsetProfiles();


void _ERM_vEepromWriteFinished_Cb(CommonResponseCodeType rsp);

/***************************************/
/* functions to handle new UserProfile */
/***************************************/

// check if a given deviceId is used in a UserProfile, This is needed when user
// wants to delete a Player. This shall not be possible, if Player is also used
// as a Phone / UserProfile
extern BooleanType ERM_bDeviceInUserProfiles(AT_DeviceId deviceId);


extern APP_COMMON_enResponseCode ERM_rcAddUserProfile_InEeprom(
        void (*cb)(APP_COMMON_enResponseCode),
        AT_DeviceId deviceId,
        const Uint8Type *pName, Uint16Type length );

extern APP_COMMON_enResponseCode ERM_rcDeleteUserProfile_InEeprom(
        void (*cb)(APP_COMMON_enResponseCode),
        Uint16Type indexOfProfile );

extern APP_COMMON_enResponseCode ERM_rcAddPlayerProfile_InEeprom(
        void (*cb)(APP_COMMON_enResponseCode),
        AT_DeviceId deviceId,
        const Uint8Type *pName, Uint16Type length );

extern APP_COMMON_enResponseCode ERM_rcAddHeadsetProfile_InEeprom(
        void (*cb)(APP_COMMON_enResponseCode),
        AT_DeviceId deviceId,
        const Uint8Type *pName, Uint16Type length );

extern APP_COMMON_enResponseCode ERM_rcDeletePlayerProfile_InEeprom(
        void (*cb)(APP_COMMON_enResponseCode),
        Uint16Type indexOfProfile );

extern APP_COMMON_enResponseCode ERM_rcDeleteHeadsetProfile_InEeprom(
        void (*cb)(APP_COMMON_enResponseCode),
        Uint16Type indexOfProfile );

extern APP_COMMON_enResponseCode ERM_rcDeleteDevice(
        void (*cb)(APP_COMMON_enResponseCode),
        Uint16Type deviceId);

extern APP_COMMON_enResponseCode ERM_rcSetVoiceMailboxNumber_InEeprom(
        void (*cb)(APP_COMMON_enResponseCode),
        Uint8Type profileIndex,
        const Uint8Type *pNumber, Uint16Type length );

extern APP_COMMON_enResponseCode ERM_rcUpdatePhoneActiveProfileList(
                void (*cb)(APP_COMMON_enResponseCode),
                Uint8Type profileIndex);

extern APP_COMMON_enResponseCode ERM_rcUpdatePlayerActiveProfileList(
                void (*cb)(APP_COMMON_enResponseCode),
                Uint8Type profileIndex);

extern APP_COMMON_enResponseCode ERM_rcUpdateHeadsetActiveProfileList(
                void (*cb)(APP_COMMON_enResponseCode),
                Uint8Type profileIndex);

extern APP_COMMON_enResponseCode ERM_rcSetRingtoneAlternate_InEeprom(
                                void (*cb)(APP_COMMON_enResponseCode),
                                Uint8Type profileIndex,
                                BooleanType bAlternate);

extern APP_COMMON_enResponseCode ERM_eSetActiveDeviceRingtone(
        void (*cb)(APP_COMMON_enResponseCode),
        ATRintoneModeEnum ringtone);

extern APP_COMMON_enResponseCode ERM_eSetActiveDeviceRingtoneAlternate(
        void (*cb)(APP_COMMON_enResponseCode),
        BooleanType ringAlternate);

extern APP_COMMON_enResponseCode ERM_rcSetRingtone_InEeprom(
                                void (*cb)(APP_COMMON_enResponseCode),
                                Uint8Type profileIndex,
                                ATRintoneModeEnum ringtone);

extern APP_COMMON_enResponseCode ERM_rcSetUserAssociatedPlayer_InEeprom(
                                void (*cb)(APP_COMMON_enResponseCode),
                                Uint8Type userProfileIndex,
                                Uint8Type playerProfileIndex,
                                BooleanType bConsiderInvalidPlayer);

extern APP_COMMON_enResponseCode ERM_rcSetUserAssociatedHeadset_InEeprom(
                                void (*cb)(APP_COMMON_enResponseCode),
                                Uint8Type userProfileIndex,
                                Uint8Type headsetProfileIndex);


extern APP_COMMON_enResponseCode ERM_rcSetPlayerAuthMode_InEeprom(
                                void (*cb)(APP_COMMON_enResponseCode),
                                Uint8Type profileIndex,
                                BooleanType bAuthNeeded);

extern APP_COMMON_enResponseCode ERM_rcSetHeadsetAuthMode_InEeprom(
                                void (*cb)(APP_COMMON_enResponseCode),
                                Uint8Type profileIndex,
                                BooleanType bAuthNeeded);


extern APP_COMMON_enResponseCode ERM_rcSetPbCompletelyLoaded_InEeprom(
                                void (*cb)(APP_COMMON_enResponseCode),
                                Uint8Type profileIndex);


extern APP_COMMON_enResponseCode ERM_rcChangeDeviceName_InEeprom(
                                void (*cb)(APP_COMMON_enResponseCode),
                                AT_DeviceId deviceId,
                                AT_DeviceNameString name,
                                Uint16Type nameLen);

/* set mode for playing caller name when a contact is calling */
extern APP_COMMON_enResponseCode ERM_rcSetPlayCallerNameMode(
            void (*cb)(APP_COMMON_enResponseCode),
            BooleanType bPlayCallerName);


#if 0
/* get current mode for playing caller name when a contact is calling */
extern APP_COMMON_enResponseCode ERM_rcGetPlayCallerNameMode(
        void (*cb)(APP_COMMON_enResponseCode));
extern BooleanType                  ERM_bGetPlayCallerNameMode();
#endif




/* set audio mode */
extern APP_COMMON_enResponseCode ERM_rcSetAudioMode_InEeprom(
        void (*cb)(APP_COMMON_enResponseCode),
        AT_DeviceId deviceId,
        ERMUserAudioModeType audioMode);

extern void ERM_cbMuteGetAck(APP_COMMON_enResponseCode rsp);

/****************************************/



extern void ERM_vHandleATResponse(const ATRspMessageType * p_msg);


extern BooleanType ERM_bMaxVoiceTagsForUserRecorder(void);

extern APP_COMMON_enResponseCode ERM_rcChangeNumVoiceTagsForActiveUser_InEeprom(
          void (*cb)(APP_COMMON_enResponseCode),
          Sint8Type  value);
/* note : isStart idicate if reset is started or finished,
 *         BT_TRUE indicate reset started
 *         BT_FALSE indicate reset stopped */
extern APP_COMMON_enResponseCode ERM_rcResetNumVoiceTagsForActiveUser_InEeprom(
        void (*cb)(APP_COMMON_enResponseCode),
        BooleanType isStart);

#include "APP_DEVICE.h"
extern void ERM_vOnAppDeviceNotification(APP_DEVICE_enNotificationId notification);

#include "VCR.h"
extern void ERM_vOnVcrNotification(VCRNotificationIdEnum notification);

#include "APP_PHONEBOOK_CE.h"
void ERM_vOnPhonebookNotification(APP_PB_enNotificationId notification);

#include "APP_MDI.h"
extern void ERM_vOnAppMdiNotification(APP_MDI_enNotificationId event);

#include "APP_CSHDL.h"
extern void ERM_vOnAppCshdlNotification(APP_CSHDL_NotificationType notification);

#include "APP_PHONE.h"
extern void ERM_vOnAppPhoneNotification(APP_PHONE_enNotificationId notification);

BooleanType _NetworkOperatorCodeIsVerizonOrAlltelOrSprint(const Uint8Type *imsi, Uint16Type length);

BooleanType ERM_bIsCDMAPhone(void);

Uint16Type ERM_uGetLastExtendedErrorCode(void);

void _ERM_ForceDisconnect_Cb(APP_COMMON_enResponseCode result);

void _ERM_cbGetOperaterDelay();

extern BooleanType ERM_bIsSSPActive(void);
extern APP_COMMON_enResponseCode ERM_rcSendSSPSupportedRequest(AT_DeviceId, void (*cb)(APP_COMMON_enResponseCode));
void ERM_cbPauseSuhutdownDelaytimeOut();//RT14519
/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/


#endif /* _ERM_H */

/* End Of File ERM.h */
