/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     ERM_Utils.h
*-----------------------------------------------------------------------------
* Module Name:    External Resource Manager Utilities and module global structs
*-----------------------------------------------------------------------------
* Description:    External resource manager global functions.
*                 the ERM is responsible for control of the UART resource
*                 towards the different other application modules
*-----------------------------------------------------------------------------
* $Date: 2011-11-08 12:01:36 +0100 (Di, 08 Nov 2011) $
* $Rev: 23159 $
* $Author: maennel $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_ERM/trunk/inc/ERM_Utils.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _ERM_UTILS_H
#define _ERM_UTILS_H


/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include "ERM_CI.h"
#include "ERM_CE.h"
#include "tms470r1.h"
/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/
#include "osek.h"

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "ATParser.h"

/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Types                                                               */
/*----------------------------------------------------------------------------*/


/* structure for audio values that might be tuned online via diagnose */
typedef struct {
    Uint8Type           mic_gain;          /* range 0..15, default 8         */
    Uint8Type           commVolume;        /* range 0..15, default 15        */
    Uint8Type           promptVolume;      /* range 0..15, default 8         */
    Uint8Type           streamVolume;      /* range 0..63, default 48        */
    Uint8Type           aecDelay;          /* range 0..25, default 0         */
    Uint8Type           alcTarget;         /* range 0(0 dB)..40(-40dB), default 8 */
    Uint8Type           alcMode;           /* range 0..3,  default 0        */
    Uint8Type           slLevel;           /* range 0(0dB)..40(-40dB), default 0 */
} ERMAudioModValuesType;

/* ERM internal messages which are mapped to AT message where required */
typedef enum {
/*  0  */   ERM_REQ_NONE                        = 0x00000000,
            /* NOTE: first ERM_STARTUP_NUM_MESSAGES shall include messages
             *       requested in startup phase */
/*  1  */   ERM_REQ_SET_EXTENDED_ERROR,
/*  2  */   ERM_REQ_MODEL_REV,
/*  3  */   ERM_REQ_MODEL_REV_EXT,
/*  4  */   ERM_REQ_MODEL_REV_PF_CODE,
/*  5  */   ERM_REQ_BT_CHIP_VERSION,
/*  6  */   ERM_REQ_SET_BT_NAME,
/*  7  */   ERM_REQ_BT_VISIBILITY_ON,
/*  9  */   ERM_REQ_SET_MAX_PAIRED_DEVICES, /* has not be the last before ERM_STARTUP_NUM_MESSAGES
                                               otherwise _ERM_sfHandleOkErrorRsp has to be adapted */

            /* audio parameter setting at startup
            list which is sent on change of audio parameter index,
            this list is handled within function _ERM_vIntAudioCallback() */
            /* set microphone algorithm */
/* 10  */   ERM_REQ_MIC_ALGO_SELECT,
            /* set microphone choice    */
/* 11  */   ERM_REQ_MIC_CHOICE_SET,
            /* set noise reduction      */
/* 12  */   ERM_REQ_MIC_TUNE_NR,
            /* set mic equalizer values */
/* 13  */   ERM_REQ_MIC_EQUALIZE,
            /* set microphone ADC gain  */
/* 14  */   ERM_REQ_MIC_ADC_CONF,
            /* set speaker algorithm    */
/* 15  */   ERM_REQ_SPK_RX_ALGO_SELECT,
            /* set speaker euqalizer    */
/* 16  */   ERM_REQ_SPK_EQUALIZE,
            /* set limiter and highpass */
///* 15  */   ERM_REQ_SPK_LIM_HIGHPASS,			// RT 2276
            /* set echo cancellation    */
/* 17  */   ERM_REQ_ECHO_TUNE_CANCELLATION,
            /* set ring tone volume  */
/* 18  */   ERM_REQ_AMGMT_OUT_VOLUME_SET_RING,
            /* set comfort noise */
/* 19  */   ERM_REQ_AMGMT_COMFORT_NOISE,
            /* set AEC delay */
/* 20  */   ERM_REQ_ECHO_DELAY_CANCELLATION,
            /* set RX ALC */
#if defined(USE_AUTOMATIC_LEVEL_CONTROL_IN_RX)
/* 21  */   ERM_REQ_SPK_RX_ALC,
/* 21  */   ERM_REQ_SPK_RX_SL,
/* 21  */   ERM_REQ_SPK_TX_SL,
#endif
/* 22 */    ERM_REQ_ACTIVATE_SW_UPDATE_CAPABILITY,
            /* NOTE : this entry used as indication for the END of the list
                      additional actions have to be inserted before
                      OR the switch within _ERM_vIntAudioCallback has to be adapted */

            /* set AudioGateway mode to disable */
/* 23  */   ERM_REQ_AUDIO_GATEWAY,				// RT 2276

            /* end of CK5050 audio parameter setting at startup */
/* 24  */   ERM_REQ_EXT_CALL_LIST,
/* 25  */   ERM_REQ_EXT_CALL_STACK,
/* 26  */   ERM_REQ_SET_MAX_CALLS,
/* 27  */   ERM_REQ_INIT_GET_PAIRED_DEVICES,
/* 28  */   ERM_REQ_DISABLE_INBAND_RINGTONE,
/* 29  */   ERM_REQ_DISABLE_DTMF_AFTER_DIALING,
/* 30  */   ERM_REQ_AUDIO_LOCK,
/* 31  */   ERM_REQ_SET_OBJECT_PUSH,
            /* the following id markes the end of the init messages
             * IMPORTANT: all messages between ERM_REQ_NONE and
             * ERM_STARTUP_NUM_MESSAGES need a consecutive numbering
             * without gaps !!! */
/* 32  */   ERM_STARTUP_NUM_MESSAGES, /* no mismatch with 256 below */

            /* here starts the init messages for UHV without SD available */
/* 33  */   ERM_REQ_AUTOCONN_ON,
/* 34  */   ERM_REQ_INIT_ENABLE_PHONE_AUTO_CONN,
/* 35  */   ERM_REQ_INIT_ENABLE_A2DP_AUTO_CONN,
/* 36  */   ERM_REQ_INIT_ENABLE_AVRCP_AUTO_CONN,
/* 37  */   ERM_REQ_HDL_PAIR_REQUEST,
//   ERM_REQ_INIT_ENABLE_AUDIO_GW_AUTO_CONN, /* changed on 6.4.2011 by nikolov/ RT: 10180(headset not supportet any more)*/
/* 38  */   ERM_STARTUP_NUM_MESSAGES_WO_SD,

            /* here starts the init messages for UHV with SD available */
/* 39  */   ERM_REQ_AUTOCONN_OFF,
/* 40  */   ERM_REQ_INIT_DISABLE_PHONE_AUTO_CONN,
/* 41  */   ERM_REQ_INIT_DISABLE_A2DP_AUTO_CONN,
/* 42  */   ERM_REQ_INIT_DISABLE_AVRCP_AUTO_CONN,
/* 43  */   ERM_REQ_HDL_PAIR_REQUEST_FORWARD,
//   ERM_REQ_INIT_DISABLE_AUDIO_GW_AUTO_CONN, /* changed on 6.4.2011 by nikolov/ RT: 10180(headset not supportet any more)*/
/* 44  */   ERM_STARTUP_NUM_MESSAGES_WITH_SD,
		
		/* changed on 6.4.2011 by nikolov/ RT: 10180(if auto conn mode and MP disabled, set the right service auto conn. mode)*/
		/* here starts the init messages for UHV with SD available */
/* 45  */   ERM_REQ_AUTOCONN_ON_MP_DISABLED,
/* 46  */   ERM_REQ_INIT_ENABLE_PHONE_AUTO_CONN_MP_DISABLED,
/* 47  */   ERM_REQ_INIT_DISABLE_A2DP_AUTO_CONN_MP_DISABLED,
/* 48  */   ERM_REQ_INIT_DISABLE_AVRCP_AUTO_CONN_MP_DISABLED,
/* 49  */   ERM_REQ_HDL_PAIR_REQUEST_MP_DISABLED,
//   ERM_REQ_INIT_DISABLE_AUDIO_GW_AUTO_CONNMP_DISABLED,
/* 50  */   ERM_STARTUP_NUM_MESSAGES_WO_SD_MP_DISABLED,

            /* now messages follow that do NOT run inside startup phase
             * Note: to be able to "remind" those triggered message if there are
             * more than one msg required, the value is coded with a bitmap
              * where first byte is kept for the init messages above */
/* 51  */   ERM_REQ_FLASH_ERASE_USERDATA,
/* 52  */   ERM_REQ_BT_VISIBILITY_OFF,
/* 53 */   ERM_REQ_GET_PAIRED_DEVICES,
/* 54  */   ERM_REQ_BT_SERVICE_CONN,
/* 55  */   ERM_REQ_DELETE_DEVICE,
           /* set microphone gain      */
/* 56  */   ERM_REQ_MIC_GAIN_SET,
            /* set communication volume  */
/* 57  */   ERM_REQ_AMGMT_OUT_VOLUME_SET_COMM,
/* 58  */   ERM_REQ_AMGMT_SET_RINGTONE,
           /* set streaming volume     */
/* 59  */   ERM_REQ_AMGMT_OUT_VOLUME_SET_STREAM,
            /* set prompt volume     */
/* 60  */   ERM_REQ_AMGMT_OUT_VOLUME_SET_PRMT,
            /* special test commands */
/* 61  */   ERM_REQ_TEST_MIC_ADCGAIN,
/* 62  */   ERM_REQ_TEST_MIC_ADCMODE,
/* 63  */   ERM_REQ_TEST_LOOPBACK,
/* 64  */   ERM_REQ_TEST_AUDIOLOOP,
/* 65  */   ERM_REQ_TEST_TESTMODE_ON, /* NOTE: Testmode can only be switched off via reset!!! */
/* 66  */   ERM_REQ_TEST_DELETE_ALL_DEVICES,
/* 67  */   ERM_REQ_TEST_FREQUENCY,
/* 68  */   ERM_REQ_TEST_AUTOCONN_OFF,
             /* start / cancel inquiry */
/* 69  */   ERM_REQ_START_INQUIRY,
/* 70  */   ERM_REQ_CANCEL_INQUIRY,
/* 71  */   ERM_REQ_INQUIRY_RESULT,
/* 72  */   ERM_REQ_STOP_PHONE_SERVICE,
/* 73  */   ERM_REQ_START_PHONE_SERVICE,
/* 74  */   ERM_REQ_STOP_PLAYER_AVRCP_SERVICE,
/* 75  */   ERM_REQ_START_PLAYER_AVRCP_SERVICE,
/* 76  */   ERM_REQ_STOP_PLAYER_A2DP_SINK_SERVICE,
/* 77  */   ERM_REQ_START_PLAYER_A2DP_SINK_SERVICE,
/* 78  */   ERM_REQ_STOP_HEADSET_SERVICE,
/* 79  */   ERM_REQ_START_HEADSET_SERVICE,
/* 80  */   ERM_REQ_DELETE_DEVICE_ID,
/* 81  */   ERM_REQ_CONNECT_DEVICE,
/* 82  */   ERM_REQ_DISCONNECT_DEVICE,
           /* set / get play caller name */
/* 83  */   ERM_REQ_SET_PLAY_CALLER_NAME,
/* 84  */   ERM_REQ_GET_PLAY_CALLER_NAME,
            /* set / get audio mode */
/* 85  */   ERM_REQ_SET_AUDIO_MODE,
/* 86  */   ERM_REQ_GET_AUDIO_MODE,
            /* pairing */
/* 87  */   ERM_REQ_PAIRING,
            /* disable autoconn for phone service */
/* 88  */   ERM_REQ_DISABLE_PHONE_AUTO_CONN,
            /* add profiles etc. in eeprom */
/* 89  */   ERM_REQ_ADD_PLAYER_HEADSET_PROFILE,
/* 90  */   ERM_REQ_ADD_USER_PROFILE,
/* 91  */   ERM_REQ_CHANGE_USER_PROFILE,
/* 92  */   ERM_REQ_DELETE_USER_PROFILE,
/* 93  */   ERM_REQ_DELETE_PLAYER_HEADSET_PROFILE,
/* 94  */   ERM_REQ_UPDATE_PROFILE_LIST,
/* 95  */   ERM_REQ_CONSISTENCY_CHECK,
/* 96  */   ERM_REQ_CHANGE_PLAYHS_PROFILE,
/* 97  */   ERM_REQ_CHANGE_PHY_TABLE,
           /* other */
/* 98  */   ERM_REQ_ALTERNATING_SIGNAL,
/* 99  */   ERM_REQ_CK5050_SW_RESET,
            /* power off */
/* 100  */   ERM_REQ_CK5050_POWER_OFF,
            /* Bluetooth SW-Update */
/* 101  */   ERM_REQ_BLUETOOTH_SW_UPDATE,
/* 102  */   ERM_REQ_AMGMT_SET_RINGTONE_ALTERNATE,
/* 103  */   ERM_REQ_BT_SET_CONN_AUTH,
/* 104  */   ERM_REQ_RINGTONE_PLAY,
/* 105  */   ERM_REQ_TEST_RX_TX,
/* 106  */   ERM_REQ_SPEAKER_VOLUME_NOTIFICATION,
/* 107  */   ERM_REQ_GET_IMSI,
            /* simple secure pairing messages */
/* 108  */  ERM_REQ_SSP_CONFIG,
/* 109  */  ERM_REQ_SSP_PAIRING_REQUEST,
/* 110  */  ERM_REQ_SSP_SETUP,
/* 111  */  ERM_REQ_SSP_SUPPORT,
/*  8  */   ERM_REQ_INQUIRY_EXT_RESULT,


            /* add all new "real messages" above this comment!!! */
/* 112  */   ERM_REQ_RETRY,

/*==================================*/
/* Do not change this last entry    */
/* 112  */   ERM_MAX_NUM_INTERNAL_MSG
} ERMIntMessageEnum;

typedef enum {
    ERM_BTVIS_NONE = 0,
    ERM_BTVIS_ON,
    ERM_BTVIS_OFF
} ERMBTVisActionEnum;


typedef enum
{
    ERM_EPR_WRITE_STATE_UNDEFINED,
    ERM_EPR_WRITE_STATE_WRITE_PENDING,
    ERM_EPR_WRITE_STATE_WAITRESULT,
    ERM_EPR_WRITE_STATE_FINSIHED
} ERMEpromWriteStateEnum;

typedef enum
{
    ERM_CC_INVALID,
    ERM_CC_ACTIVE,
    ERM_CC_WAIT_FOR_RESPONSE
} ERMConsistCheckStateEnum;


typedef enum {
    ERM_JINGLE_NO_JINGLE_ACTIVE = 0,
    ERM_JINGLE_WAIT_MUTE_ACK_CONNECT_JINGLE_CMD,
    ERM_JINGLE_SEND_CONNECT_JINGLE_CMD,
    ERM_JINGLE_CONNECT_JINGLE_IS_PLAYING,
    ERM_JINGLE_SEND_DISCONNECT_JINGLE_CMD,
    ERM_JINGLE_WAIT_MUTE_ACK_DISCONNECT_JINGLE_CMD,
    ERM_JINGLE_DISCONNECT_JINGLE_IS_PLAYING
} ERMJingleStates;


#define ERM_JINGLE_CMD_DELAY_CONNECT             1500
#define ERM_JINGLE_CMD_DELAY_DISCONNECT          3000  // HU need more time if KL15 is OFF
#define ERM_JINGLE_WAIT_MUTE_ACK_TIMEOUT         6000
#define ERM_JINGLE_PLAY_REQUEST_TIMEOUT          8000



/* ERM module internal data struct */
typedef struct
{   /* parameters for state handling */
    ERMSwVersionType            eCK5050SWVersion;
    Uint8Type                   u8Active;
    BooleanType                 bStartupDone;
    BooleanType                 bReInitAudioParmeter;
    ERMStateType                eState;
    ERMBTVisActionEnum          eBTvisibilityAct;
    BooleanType                 bBTVisible;
    BooleanType                 bBTVisIsInitial;
    Uint8Type                   u8PlayConnectJingle;
    Uint8Type                   u8PlayDisconnectJingle;
    BooleanType                 bEnableDisconnectPlayer;
    BooleanType                 bMuteACK;
    ERMJingleStates             eJingleState;
    TickType                    JingleStateTime;
    Uint8Type                   u8PhoneIdConnected;
    Uint8Type                   u8PlayerIdConnected;

    BooleanType                 bSendA2dpAudioVolume;
    BooleanType                 bSendVcrAudioVolume;
    BooleanType                 bForceDisconnect;
    BooleanType                 bForceDisconnectPlayer;
    BooleanType                 bPrepareCK5050Off;
    /* UART parameters */
    SCI_ST              *       pUartPtr;
    APPSubModuleIdEnum          eUartOwner;

    /* paramters for message handling */
    /* indicator if ERM internal startup is still ongoing */
    Uint32Type                  u32StartupCnt;
    /* if send of request is required regERMRequest is set to required
     * message index and ERM try to send the message via AT. If the message
     * has been send via AT, identifier is copied to lastERMRequest until
     * OK for the request is received. As long as OK is NOT received, ERM
     * will not accept further reqERMRequest. A retry counter is used in
     * case UART is blocked */
    Uint16Type                  reqRetryCnt;     /* UART retry counter */
    ERMIntMessageEnum           reqERMRequest;   /* request for sending*/
    ERMIntMessageEnum           lastERMRequest;  /* wait for OK        */
    /* callback is set if an application send an ERM(AT) request
       callback is processed when AT ERROR or OK has been received
       for the specific request  */
    void (*cb_atOkErrCBReq)(APP_COMMON_enResponseCode);
    void (*cb_atOkErrCBLast)(APP_COMMON_enResponseCode);

    /* parameters for device handling */
    AT_DeviceId                u8ActiveBTId;
    AT_DeviceId                u8ActiveMDIId;
    AT_DeviceId                u8ActiveHeadsetId;
    AT_DeviceId                u8DeviceIdForMsg;
    AT_DeviceId                u8DeleteDeviceId;
    AT_DeviceId                u8LastPairedDeviceId;
    AT_DeviceId                u8LastServiceDeviceId;
    Uint8Type                  pu8ActiveUserList[AT_MAX_DEVICE_ID];

    /* parameters to store information */
    ERMSWVersionStringType     stSWVersion;      /* CK5050 SW Version */
    ERMPF_CodeStringType       stPF_Code;          /* CK5050 PF Code */
    BooleanType                bCKSWVersionStringAvail;
    BooleanType                bPF_CodeStringAvail;
    ATPinCodeBT                stBTPinCode;      /* BT PIN */
    Uint8Type                  stBTName[ERM_BT_DEVICE_NAME_MAXLEN+1];

    /* parameters for special tests */
    Uint8Type                  u8TestMicMode;
    Uint8Type                  u8TestMicGain;
    BooleanType                bTestLoopbackOn;
    ATTestFreqModeEnum         eTestFreqMode;
    AT_BT_Address              btAddrAudioLoop;
    Uint16Type                 u16AudioLoopDelayLocal;
    Uint16Type                 u16AudioLoopDelayBT;
    void (*cb_TestModeExt)(APP_COMMON_enResponseCode);
    ATReqAlternatingSignalType alternatingSignal;

    /* additional params */
    Uint8Type                  u8TotalInquiryResult;
    Uint8Type                  u8PhoneInquiryResult;
    Uint8Type                  u8PlayerInquiryResult;
    Uint8Type                  u8HeadsetInquiryResult;

    ERMResetState              eResetState;

    struct
    {
        ERMIntMessageEnum      enLastId;
        union
        {
            ERMInquiryResultType                inquiry;
            ERMDiscPhoneServiceType             discPhoneServ;
            ERMDiscPlayerAvrcpServiceType       discPlayerAvrcpServ;
            ERMDiscPlayerA2dpSinkServiceType    discPlayerA2dpSinkServ;
            ERMDiscHeadSetServiceType           discHeadsetServ;
            ERMStartHeadSetServiceType          startHeadsetServ;
            ERMConnPhoneServiceType             connPhoneServ;
            ERMConnPlayerAvrcpServiceType       connPlayerAvrcpServ;
            ERMConnPlayerA2dpSinkServiceType    connPlayerA2dpSinkServ;
            ERMDeleteDeviceType                 deleteDevice;
            ERMConnectDeviceType                connectDevice;
            ERMDisconnectDeviceType             disconnectDevice;
            ERMSetPlayCallerNameType            setPlayCallerName;
            ERMGetPlayCallerNameType            getPlayCallerName;
            ERMSetAudioModeType                 setAudioMode;
            ERMGetAudioModeType                 getAudioMode;
            ERMPairingType                      pairing;
            ERMRingtonePlayType                 ringtonePlay;
        }ctx;
    }req;

    /* paramters for EEPROM write */
    ERMIntMessageEnum     ePendingEpromMsg;  /* store pending Eprom Msg */
    void                  (*cb_epromWriteFinished)(APP_COMMON_enResponseCode);
    /* temporary eprom storage */
    struct
    {
        ERMEpromWriteStateEnum      state;

        /* user profile */
        BooleanType                 bUpdateUserEepromNeeded;
        BooleanType                 bUpdateUserRamNeeded;
        Uint8Type                   u8UserIdx;
        /* player / headset profiles */
        BooleanType                 bUpdatePlayHSEepromNeeded;
        BooleanType                 bUpdatePlayHSRamNeeded;
        Uint8Type                   u8PlayHSIdx;
        /* physical device table */
        BooleanType                 bUpdatePhyEepromNeeded;
        BooleanType                 bUpdatePhyRamNeeded;
        Uint8Type                   u8PhyIdx;
        /* active profile table need to be updated */
        BooleanType                 bUpdateActProfileEepromNeeded;
        BooleanType                 bUpdateActProfileRamNeeded;
        BooleanType                 bActUserProfileListChanged;
        BooleanType                 bActPlayerProfileListChanged;
        BooleanType                 bActHeadsetProfileListChanged;
        /* temp structs for new data */
        ERMUserProfileType          user;
        ERMPlayerHeadsetProfileType playHs;
        ERMPhysicalBTDeviceType     phy;
        ERMActiveProfileListType    actList;
    } epr_tmp;

    ERMConsistCheckStateEnum        eConsistCheckState;
    AT_BT_Address                   stBTAddress;
#if defined(ERM_USE_SIGNED_BT_UPDATE)
    AT_Md5KeyType                   stMd5Key;
    AT_Md5KeyEncrytedType           stMd5KeyEncryted;
#endif
    BooleanType                     bSecurityCheckOK;
    ERMBtChipVersionType            stBtChipVersion;
    BooleanType                     bDisconnectWillFollow;
    BooleanType                     bgetImsi;
    BooleanType                     bisCDMAPhone;
    BooleanType                     doPlayDisconnectJingleForPlayer;
    BooleanType                     bgetProvider;
    APP_DEVICE_DeviceType           deviceTypeForPairing;
    ERMExtendedErrorCode            extendedErrorCode;
    BooleanType                     bActiveCallFlagFOrSettingWithoutCI;
    ERMPairReqType                  stPairRequest;
    ERMSspReqType                   stSspRequest;
    ATSSPModeEnum                   eSSPMode;
    ATSSPIoCapParameterEnum         eSSPIoCap;
    BooleanType						bRingtonestatus;//RT14853
}ERMContextType;


/*----------------------------------------------------------------------------*/
/* Global Variables Declarations (avoid as much as possible!)                 */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global const Declarations                                                  */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Methods                                                             */
/*----------------------------------------------------------------------------*/
#ifdef ERM_TEST_AUDIO
void _ERM_vExtAudioCallback(APP_COMMON_enResponseCode rsp);
void _ERM_vMainTestFunction(void);
#endif

extern SuccessFailType _ERM_ReleaseUart( APPSubModuleIdEnum requestingApp );
extern const SCI_ST*   _ERM_GetUart( APPSubModuleIdEnum requestingApp );
extern void            _ERM_vChangeState(ERMStateType new_state);


extern void _ERM_vStartUartSupervision();
extern void _ERM_vStopUartSupervision();
extern ERMJingleStates ERM_GetJingleState (void);
extern void ERM_vSetDeviceTypeForPairing(APP_DEVICE_DeviceType deviceType);
extern void ERM_Execute_HW_Reset (void);
extern void ERM_PWNG4_Execute_HW_Reset (void);



/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Private Enumerations                                                       */
/*----------------------------------------------------------------------------*/


#endif /* _ERM_UTILS_H */

/* End Of File ERM_Utils.h */
