/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     ATParser_Utils.h
*-----------------------------------------------------------------------------
* Module Name:    AT Parser Utilities
*-----------------------------------------------------------------------------
* Description:    The AT Parser search for carrage returns und line feeds.
                  Analysis the incomming data from the UART minterface.
*-----------------------------------------------------------------------------
* $Date: 2011-10-04 15:35:52 +0200 (Di, 04 Okt 2011) $
* $Rev: 22027 $
* $Author: maennel $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_AT_Parser/trunk/inc/ATParser_Utils.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _ATPARSER_UTILS_H
#define _ATPARSER_UTILS_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include "Common.h"
#include "ATParser.h"
/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Types                                                               */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Declarations (avoid as much as possible!)                 */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global const Declarations                                                  */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Methods                                                             */
/*----------------------------------------------------------------------------*/
extern Uint16Type       _ATParser_FirstToken(Uint8Type *p_msg);
extern Uint16Type       _ATParser_OtherToken(Uint8Type *p_msg);
extern Uint8Type        _ATParser_u8GetFromAsciiAndCheck(Uint8Type *p_msg,
                                                         Uint8Type max,
                                                         Uint8Type set_default);

/* note: function check implicit if value > max, if so value set to 0 */
extern SuccessFailType  _ATParser_GetDeviceAndValue(Uint8Type  *p_msg,
                                                    Uint8Type  *device_id,
                                                    Uint32Type *value,
                                                    Uint8Type  maxvalue,
                                                    Uint32Type string_len);

extern SuccessFailType _ATParser_ExtPlpdInfo(ATRspPlpdInfoType * p_info,
                                             Uint8Type         * p_msg,
                                             Uint32Type         string_len);

extern SuccessFailType _ATParser_ExtPlccInfo(ATRspCallStatValType * p_info,
                                             Uint8Type            * p_msg,
                                             Uint32Type           string_len);

extern SuccessFailType _ATParser_ExtPindVal(ATRspPhoneStatusIndValueType * p_info,
                                            Uint8Type         * p_msg,
                                            Uint32Type         string_len);

extern SuccessFailType _ATParser_ExtPindSup(ATRspPhoneStatusIndSupportType * p_info,
                                            Uint8Type         * p_msg,
                                            Uint32Type         string_len);

extern SuccessFailType _ATParser_ExtPievInfo(ATRspPhoneStatusIndEventType * p_info,
                                             Uint8Type         * p_msg,
                                             Uint32Type         string_len);

extern SuccessFailType _ATParser_ExtPmrdNum(ATRspSmsReceiveNumType * p_info,
                                            Uint8Type              * p_msg,
                                            Uint32Type               string_len);

extern SuccessFailType _ATParser_ExtPmrdMsg(ATRspSmsReceiveMsgType * p_info,
                                            Uint8Type              * p_msg,
                                            Uint32Type               string_len);

extern SuccessFailType _ATParser_ExtPpbp(ATRspPBReadNumberType * p_info,
                                         Uint8Type             * p_msg,
                                         Uint32Type              string_len);

extern SuccessFailType _ATParser_ExtPpbr(ATRspPBReadEntryType * p_info,
                                         Uint8Type            * p_msg,
                                         Uint32Type             string_len);

extern SuccessFailType _ATParser_ExtPbsc(ATResBTServiceConnType * p_info,
                                         Uint8Type              * p_msg,
                                         Uint32Type             string_len);

extern SuccessFailType _ATParser_ExtPgmt(ATResGetMetadataType   * p_info,
                                         Uint8Type              * p_msg,
                                         Uint32Type             string_len);

extern SuccessFailType _ATParser_ExtPbsn(ATResBTServiceNotType  * p_info,
                                         Uint8Type              * p_msg,
                                         Uint32Type             string_len);

extern SuccessFailType _ATParser_ExtPamu(ATResMetaDataUpdateType  * p_info,
                                         Uint8Type              * p_msg,
                                         Uint32Type             string_len);

extern SuccessFailType _ATParser_ExtPsbd(ATRspSelectBTDeviceType * p_info,
                                         Uint8Type               * p_msg,
                                         Uint32Type              string_len);

extern SuccessFailType _ATParser_ExtDgps(ATRspGetPlayerStatusType * p_info,
                                         Uint8Type                * p_msg,
                                         Uint32Type               string_len);

extern SuccessFailType _ATParser_ExtDged(ATResPlayerGetEntDataType * p_info,
                                         Uint8Type                * p_msg,
                                         Uint32Type               string_len);

extern SuccessFailType _ATParser_ExtPasa(ATResAudioSrcActType * p_info,
                                         Uint8Type            * p_msg,
                                         Uint32Type           string_len);

extern SuccessFailType _ATParser_ExtPcms(ATResBTAutoConnType * p_info,
                                         Uint8Type           * p_msg,
                                         Uint32Type          string_len);

extern SuccessFailType _ATParser_ExtRres(ATResSIVRResultType * p_info,
                                         Uint8Type           * p_msg,
                                         Uint32Type          string_len);

extern SuccessFailType _ATParser_ExtRscl(ATResSIVRSetSupLangType * p_info,
                                         Uint8Type               * p_msg,
                                         Uint32Type              string_len);

extern SuccessFailType _ATParser_ExtRsrf(ATResSIVRSetResFormatType * p_info,
                                         Uint8Type                 * p_msg,
                                         Uint32Type                string_len);

extern SuccessFailType _ATParser_ExtRgsl(ATResSIVRGetSupLangType * p_info,
                                         Uint8Type               * p_msg,
                                         Uint32Type              string_len);

extern SuccessFailType _ATParser_ExtPalgoselect(ATResPalgoSelectType * p_info,
                                                Uint8Type            * p_msg,
                                                Uint32Type           string_len);

extern SuccessFailType _ATParser_ExtPalgoSetMic(ATResPalgoSetMicType * p_info,
                                                Uint8Type            * p_msg,
                                                Uint32Type           string_len);

extern SuccessFailType _ATParser_ExtMicGain(ATResMicGainType * p_info,
                                           Uint8Type            * p_msg,
                                           Uint32Type           string_len);

extern SuccessFailType _ATParser_ExtPalgoAEC(ATResPalgoAECType * p_info,
                                             Uint8Type            * p_msg,
                                             Uint32Type           string_len);

extern SuccessFailType _ATParser_ExtVolumeSet(ATResVolumeSetType * p_info,
                                              Uint8Type            * p_msg,
                                              Uint32Type           string_len);

extern SuccessFailType _ATParser_ExtPwng(ATRspWarningType * p_info,
                                  Uint8Type         * p_msg,
                                  Uint32Type         string_len);


extern SuccessFailType _ATParser_ExtDeviceId(Uint8Type       ** p_msg,
                                             Uint8Type       * p_device_id);

extern SuccessFailType _ATParser_ExtDeviceIdType(Uint8Type         ** p_msg,
                                                 ATDeviceTypeEnum  * p_device_type,
                                                 Uint8Type         * p_device_id);

extern SuccessFailType _ATParser_ExtPairingDeviceIdType( 
                                            Uint8Type           ** p_msg,
                                            ATPairingDeviceType * p_device_type,
                                            Uint8Type           * p_device_id);

extern SuccessFailType _ATParser_CheckNumberParam(Uint8Type   * p_msg,
                                                  Uint8Type     req_num_param,
                                                  Uint8Type    *avail_param,
                                                  Uint32Type    string_len    );

extern SuccessFailType _ATParser_ExtName(Uint8Type   ** p_msg,
                                         AT_DeviceNameString   * p_device_name,
                                         Uint8Type   * p_name_len,
                                         Uint32Type    max_len    );
                                         
extern SuccessFailType _ATParser_ExtCallerID(Uint8Type   ** p_msg,
                                         AT_CallerId   * p_device_name,
                                         Uint8Type   * p_name_len,
                                         Uint32Type    max_len    );
                                         
extern SuccessFailType _ATParser_ExtLastName(Uint8Type   ** p_msg,
                                         AT_DeviceNameString   * p_device_name,
                                         Uint8Type   * p_name_len,
                                         Uint32Type    max_len    ); 

 extern SuccessFailType _ATParser_ExtLastCallerID(Uint8Type   ** p_msg,
                                         AT_CallerId   * p_device_name,
                                         Uint8Type   * p_name_len,
                                         Uint32Type    max_len    );                                          

extern SuccessFailType _ATParser_ExtBTAddress(Uint8Type   ** p_msg,
                                              AT_BT_Address   * p_bt_addr    );

extern SuccessFailType _ATParser_ExtLastBTAddress(Uint8Type       ** p_msg,
                                              AT_BT_Address   * p_bt_addr    );

extern SuccessFailType _ATParser_ExtCoD(Uint8Type   ** p_msg,
                                        AT_BTClassOfDevice   * p_bt_cod    );

extern SuccessFailType _ATParser_ExtLastCoD(Uint8Type            ** p_msg,
                                        Uint32Type            * p_bt_cod    );

extern SuccessFailType _ATParser_ExtSSPPin(Uint8Type            ** p_msg,
                                           ATSSPPinCodeBT        * p_pin    );

extern SuccessFailType _ATParser_ExtSupport(Uint8Type   ** p_msg,
                                            ATSupportedEnum   * p_support   );

extern SuccessFailType _ATParser_ExtLastSupport(Uint8Type         ** p_msg,
                                            ATSupportedEnum   * p_support   );

extern SuccessFailType _ATParser_ExtBool(Uint8Type   ** p_msg,
                                         ATBooleanEnum   * p_bool   );
                                         
                                         
extern SuccessFailType _ATParser_ExtLastBool(Uint8Type       ** p_msg,
                                         ATBooleanEnum   * p_bool   );                                         

extern SuccessFailType _ATParser_ExtBoolSpecial(Uint8Type       ** p_msg,
                                        ATBooleanEnum   * p_bool   );

extern SuccessFailType _ATParser_ExtCallState(Uint8Type        ** p_msg,
                                          ATCallStateEnum   * p_call_state   );

extern SuccessFailType _ATParser_sFExtu8RangeValue(Uint8Type   ** p_msg,
                                                   Uint8Type   * p_value,
                                                   Uint8Type   min,
                                                   Uint8Type   max);

extern SuccessFailType _ATParser_sFExtu16RangeValue(Uint8Type   ** p_msg,
                                                    Uint16Type   * p_value,
                                                    Uint16Type   min,
                                                    Uint16Type   max);

extern SuccessFailType _ATParser_sFExtu32RangeValue(Uint8Type   ** p_msg,
                                                    Uint32Type   * p_value,
                                                    Uint32Type   min,
                                                    Uint32Type   max);

extern SuccessFailType _ATParser_sFExtLastu32RangeValue(Uint8Type   ** p_msg,
                                                     Uint32Type   * p_value,
                                                     Uint32Type   min,
                                                     Uint32Type   max);


extern SuccessFailType _ATParser_sFExtMicAlgoValue(Uint8Type   ** p_msg,
        ATMicAlgoEnum   * p_value,
        ATMicAlgoEnum   min,
        ATMicAlgoEnum   max);

extern SuccessFailType _ATParser_sFExtPairingResultValue(Uint8Type   ** p_msg,
        ATPairingResultEnum   * p_value,
        ATPairingResultEnum   min,
        ATPairingResultEnum   max);

extern SuccessFailType _ATParser_sFExtSIVRCmdValue(Uint8Type   ** p_msg,
        ATSIVRCmdIdEnum   * p_value,
        ATSIVRCmdIdEnum   min,
        ATSIVRCmdIdEnum   max);

extern SuccessFailType _ATParser_sFExtSIVRResultValue(Uint8Type   ** p_msg,
        ATSIVRResultEnum   * p_value,
        ATSIVRResultEnum   min,
        ATSIVRResultEnum   max);

extern SuccessFailType _ATParser_sFExtSIVRPromptValue(Uint8Type   ** p_msg,
        ATSIVRPromptEnum   * p_value,
        ATSIVRPromptEnum   min,
        ATSIVRPromptEnum   max);

extern SuccessFailType _ATParser_sFExtSIVRLanguageValue(Uint8Type   ** p_msg,
        ATSIVRLanguagesEnum   * p_value,
        ATSIVRLanguagesEnum   min,
        ATSIVRLanguagesEnum   max);

extern SuccessFailType _ATParser_sFExtAutoConnStatusValue(Uint8Type   ** p_msg,
        ATAutoConnStatusEnum   * p_value,
        ATAutoConnStatusEnum   min,
        ATAutoConnStatusEnum   max);

extern SuccessFailType _ATParser_sFExtASourceActStateValue(Uint8Type   ** p_msg,
        ATAudioSourceActStateEnum   * p_value,
        ATAudioSourceActStateEnum   min,
        ATAudioSourceActStateEnum   max);

extern SuccessFailType _ATParser_sFExtEntryFieldMaskValue(Uint8Type   ** p_msg,
        ATEntryFieldMaskEnum   * p_value,
        ATEntryFieldMaskEnum   min,
        ATEntryFieldMaskEnum   max);

extern SuccessFailType _ATParser_sFExtDeviceplayerCodedValue(Uint8Type   ** p_msg,
        ATDevicePlayerCodedEnum   * p_value,
        ATDevicePlayerCodedEnum   min,
        ATDevicePlayerCodedEnum   max);

extern SuccessFailType _ATParser_sFExtDeviceStatusValue(Uint8Type   ** p_msg,
        ATDGPSStatusEnum   * p_value,
        ATDGPSStatusEnum   min,
        ATDGPSStatusEnum   max);

extern SuccessFailType _ATParser_sFExtBTServiceValue(Uint8Type   ** p_msg,
        ATBTServiceEnum   * p_value,
        ATBTServiceEnum   min,
        ATBTServiceEnum   max);

extern SuccessFailType _ATParser_sFExtBTServiceResultValue(Uint8Type   ** p_msg,
        ATBTServiceResultEnum   * p_value,
        ATBTServiceResultEnum   min,
        ATBTServiceResultEnum   max);

extern SuccessFailType _ATParser_sFExtMetadataTypeValue(Uint8Type   ** p_msg,
        ATMetadataTypeEnum   * p_value,
        ATMetadataTypeEnum   min,
        ATMetadataTypeEnum   max);

extern SuccessFailType _ATParser_sFExtKeyRangeValue(Uint8Type   ** p_msg,
        ATRspKeyboardKeysEnum   * p_value,
        ATRspKeyboardKeysEnum   min,
        ATRspKeyboardKeysEnum   max);

extern SuccessFailType _ATParser_sFExtDiscoverModeValue(Uint8Type   ** p_msg,
        ATBTDiscoverModeEnum   * p_value,
        ATBTDiscoverModeEnum   min,
        ATBTDiscoverModeEnum   max);

extern SuccessFailType _ATParser_sFExtSIVREngStatusValue(Uint8Type   ** p_msg,
        ATSIVREngStatusEnum   * p_value,
        ATSIVREngStatusEnum   min,
        ATSIVREngStatusEnum   max);

extern SuccessFailType _ATParser_sFExtATSupportedValue(Uint8Type   ** p_msg,
        ATSupportedEnum   * p_value,
        ATSupportedEnum   min,
        ATSupportedEnum   max);

extern SuccessFailType _ATParser_sFExtPNTypeValue(Uint8Type   ** p_msg,
        ATPhoneNumberTypeEnum   * p_value,
        ATPhoneNumberTypeEnum   min,
        ATPhoneNumberTypeEnum   max);

extern SuccessFailType _ATParser_sFExtLastPNTypeValue(Uint8Type           ** p_msg,
                                 ATPhoneNumberTypeEnum   * p_value,
                                 ATPhoneNumberTypeEnum   min,
                                 ATPhoneNumberTypeEnum   max   );

extern SuccessFailType _ATParser_sFExtPNTypeSpecialValue(Uint8Type           ** p_msg,
                                 ATPhoneNumberTypeEnum   * p_value,
                                 ATPhoneNumberTypeEnum   min,
                                 ATPhoneNumberTypeEnum   max   );

extern SuccessFailType _ATParser_sFExtRintoneModeValue(Uint8Type   ** p_msg,
        ATRintoneModeEnum   * p_value,
        ATRintoneModeEnum   min,
        ATRintoneModeEnum   max);

extern SuccessFailType _ATParser_sFExtCodProfileValue(Uint8Type   ** p_msg,
        ATCoDProfileEnum   * p_value,
        ATCoDProfileEnum   min,
        ATCoDProfileEnum   max);

extern SuccessFailType _ATParser_sFExtVTResultValue(Uint8Type   ** p_msg,
        ATVTRecResultEnum   * p_value,
        ATVTRecResultEnum   min,
        ATVTRecResultEnum   max);

extern SuccessFailType _ATParser_sFExtUnquotedString(Uint8Type   ** p_msg,
                                                     Uint8Type   * p_string,
                                                     Uint16Type  * p_string_len,
                                                     Uint32Type    max_len    );

extern SuccessFailType _ATParser_sFExtQuotedString(Uint8Type   ** p_msg,
                                                   Uint8Type   * p_string,
                                                   Uint16Type  * p_string_len,
                                                   Uint32Type    max_len    );

extern SuccessFailType _ATParser_sFExtLastQuotedString(Uint8Type   ** p_msg,
                                            Uint8Type   * p_string,
                                            Uint16Type  * p_string_len,
                                            Uint32Type    max_len    );
                                                   
extern SuccessFailType _ATParser_sFExtQuotedLeftPosition(Uint8Type    ** p_msg,
                                                    ATRspPBReadEntryType ** p_string);
                                                   
extern SuccessFailType _ATParser_ExtPbdi(ATRspInquiryType * p_info,
        Uint8Type            * p_msg,
        Uint32Type             string_len);

extern SuccessFailType _ATParser_ExtPgir(ATRspGetInquiryResType * p_info,
        Uint8Type            * p_msg,
        Uint32Type             string_len);

extern SuccessFailType _ATParser_ExtPprq(ATRspInPairRequestType * p_info,
        Uint8Type            * p_msg,
        Uint32Type             string_len);

extern SuccessFailType _ATParser_ExtPbcd(ATRspClassOfDeviceRequestType * p_info,
        Uint8Type              * p_msg,
        Uint32Type             string_len);  
        
extern SuccessFailType _ATParser_ExtRstu(ATRspRstuexType * p_info,
        Uint8Type            * p_msg,
        Uint32Type             string_len) ;
        
extern SuccessFailType _ATParser_sfExtPbcv(ATRspPbcvType * p_info,
        Uint8Type            * p_msg,
        Uint32Type             string_len);

extern SuccessFailType _ATParser_ExtPmsdexMsg(ATRspSmsConcReceiveType * p_info,
                                       Uint8Type               * p_msg,
                                       Uint32Type              string_len);

extern SuccessFailType _ATParser_ExtPprs(ATResPairingResultType * p_pprs,
                                  Uint8Type              * p_msg,
                                  Uint32Type             string_len);
extern SuccessFailType _ATParser_sfExtPsspInfo(ATRspSSPSetupType * , Uint8Type * , Uint32Type );
extern SuccessFailType _ATParser_sfExtPbsfInfo(ATResBtFeatureSupportType * pstPbsf,
                                      Uint8Type         * pu8Msg,
                                      Uint32Type        u32StringLen);
extern SuccessFailType _ATParser_sfExtPsprInfo(ATRspSSPRequestType *,
                                        Uint8Type           *,
                                        Uint32Type          );
/*----------------------------------------------------------------------------*/
/* Public Enumerations                                                        */
/*----------------------------------------------------------------------------*/



#endif /* _ATPARSER_UTILS_H */

/* End Of File ATParser_Utils.h */
