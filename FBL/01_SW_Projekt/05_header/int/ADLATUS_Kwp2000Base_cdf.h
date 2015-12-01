/*-----------------------------------------------------------------------------+
|            ____    ___            ____    _____           ____               |
|           |    |  |   \   |      |    |     |    |    |  |                   |
|           |____|  |    |  |      |____|     |    |    |  |____               |
|           |    |  |    |  |      |    |     |    |    |       |              |
|           |    |  |___/   |____  |    |     |    |____|   ____|              |
|                                                                              |
|           T H E   S M A R T   E M B E D D E D   S O L U T I O N              |
|                                                                              |
+------------------------------------------------------------------------------+
| F I L E   I N F O R M A T I O N                                              |
+------------------------------------------------------------------------------+
|                                                                              |
|  Filename:  ADLATUS_Kwp2000Base_cdf.h                                        |
|                                                                              |
|  Comment:   This file includes all basic defines for the KWP2000-Protokol    |
|             handling.                                                        |
|                                                                              |
+------------------------------------------------------------------------------+
| R E V I S I O N   H I S T O R Y                                              |
+------------------------------------------------------------------------------+
|
| Subversion Info:
| $Id: ADLATUS_Kwp2000Base_cdf.h 4396 2008-11-04 09:26:32Z maennel $
| $HeadURL: http://smartsvn/svn/adlatusrepository/auslieferungen/S1NN/086350-S1NN-ADLATUS-TMS470R-MucSuc/20081201_S1NN_TMS470R_BlfIso-Source_V1.00/01_SW_Projekt/05_header/int/ADLATUS_Kwp2000Base_cdf.h $
|
+------------------------------------------------------------------------------+
|      (c) SMART ELECTRONIC DEVELOPMENT GMBH - all rights reserved             |
+-----------------------------------------------------------------------------*/

/* ========================================================================== */
/*      O P E N   M U L T I P L E   I N C L U D E   P R O T E C T I O N       */
/* ========================================================================== */
#ifndef __CDF_KWP2000BASE_
#define __CDF_KWP2000BASE_
/* ========================================================================== */

/* ========================================================================== */
/*                                                                            */
/*                                                                            */
/*      K W P 2 0 0 0   D E F I N E S                                         */
/*      -----------------------------------------------------------           */
/*      ( I S O   1 4 2 3 0  /  1 5 7 6 5 )                                   */
/*                                                                            */
/* ========================================================================== */

/* ========================================================================== */
/*                                                                            */
/*      S E R V I C E   I D E N T I F I E R   ( S I D )                       */
/*                                                                            */
/* ========================================================================== */

/*----------------------------------------------------------------------------*/
/* $00 .. $0F --> SAE J1979 DiagTestModes                    (ISO 15031-5)    */
/*---------------------------------------+------------------------------------*/
#define  d_CCkwpbase_SIDStartDiagSession              (UBYTE)   0x10u
#define  d_CCkwpbase_SIDEcuReset                      (UBYTE)   0x11u
#define  d_CCkwpbase_SIDReadFreezeFrameData           (UBYTE)   0x12u
#define  d_CCkwpbase_SIDReadDTC                       (UBYTE)   0x13u
#define  d_CCkwpbase_SIDClearDTCInfo                  (UBYTE)   0x14u
#define  d_CCkwpbase_SIDReadStatusOfDTC               (UBYTE)   0x17u
#define  d_CCkwpbase_SIDReadDTCByStatus               (UBYTE)   0x18u
#define  d_CCkwpbase_SIDReadEcuIdentification         (UBYTE)   0x1Au
#define  d_CCkwpbase_SIDStopDiagSession               (UBYTE)   0x20u
#define  d_CCkwpbase_SIDReadDataByLocalId             (UBYTE)   0x21u
#define  d_CCkwpbase_SIDReadDataByCommonId            (UBYTE)   0x22u
#define  d_CCkwpbase_SIDReadMemoryByAddress           (UBYTE)   0x23u
#define  d_CCkwpbase_SIDSetDataRates                  (UBYTE)   0x26u
#define  d_CCkwpbase_SIDSecurityAccess                (UBYTE)   0x27u
#define  d_CCkwpbase_SIDDisableNormalMsgTransmission  (UBYTE)   0x28u
#define  d_CCkwpbase_SIDEnableNormalMsgTransmission   (UBYTE)   0x29u
#define  d_CCkwpbase_SIDDynamicallyDefineLid          (UBYTE)   0x2Cu
#define  d_CCkwpbase_SIDWriteDataByLocalId            (UBYTE)   0x2Eu
#define  d_CCkwpbase_SID_IOControlByCid               (UBYTE)   0x2Fu
#define  d_CCkwpbase_SID_IOControlByLid               (UBYTE)   0x30u
#define  d_CCkwpbase_SIDStartRoutineByLid             (UBYTE)   0x31u
#define  d_CCkwpbase_SIDStopRoutineByLid              (UBYTE)   0x32u
#define  d_CCkwpbase_SIDReqRResultsByLid              (UBYTE)   0x33u
#define  d_CCkwpbase_SIDRequestDownload               (UBYTE)   0x34u
#define  d_CCkwpbase_SIDRequestUpload                 (UBYTE)   0x35u
#define  d_CCkwpbase_SIDTransferData                  (UBYTE)   0x36u
#define  d_CCkwpbase_SIDRequestTransferExit           (UBYTE)   0x37u
#define  d_CCkwpbase_SIDStartRoutineByAddress         (UBYTE)   0x38u
#define  d_CCkwpbase_SIDStopRoutineByAddress          (UBYTE)   0x39u
#define  d_CCkwpbase_SIDReqRResultsByAddress          (UBYTE)   0x3Au
#define  d_CCkwpbase_SIDWriteDataByLid                (UBYTE)   0x3Bu
#define  d_CCkwpbase_SIDWriteMemoryByAddress          (UBYTE)   0x3Du
#define  d_CCkwpbase_SIDTesterPresent                 (UBYTE)   0x3Eu
/*----------------------------------------------------------------------------*/
/* $3F --> Reserved                                                           */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* $40 .. $4F                                                   (ISO 15031-5) */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* $50 .. $7E positive Responses                                (ISO 15765-3) */
/*----------------------------------------------------------------------------*/
#define  d_CCkwpbase_SIDNegativResponseID             (UBYTE)   0x7Fu
#define  d_CCkwpbase_SIDEscapeCode                    (UBYTE)   0x80u
#define  d_CCkwpbase_SIDStartCommunication            (UBYTE)   0x81u
#define  d_CCkwpbase_SIDStopCommunication             (UBYTE)   0x82u
#define  d_CCkwpbase_SIDAccessTimingParameters        (UBYTE)   0x83u
#define  d_CCkwpbase_ControlDTC                       (UBYTE)   0x85u
/*----------------------------------------------------------------------------*/
/* $86 .. $9F --> reserved for future exp.                                    */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* $A0 .. $B9 --> vehicle manufacturer defines                                */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* $BA .. $BE --> system supplier defines                                     */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* $BF        --> Reserved                                                    */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* $C0        --> Reserved                                      (ISO 14230-2) */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* $C1 .. $C3 --> Reserved                                      (ISO 14230-3) */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* $C4 .. $C5 --> Reserved                                      (ISO 15765-3) */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* $C6 .. $DF --> reserved for future exp.                                    */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* $E0 .. $F9 --> pos. Resp. vehicle manufacturer defines                     */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* $FA .. $FE --> pos. Resp. system supplier defines                          */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* $FF --> Reserved                                                           */
/*                                                                            */
/* NOTE: $FF will be used by SMART in the LookForCmd-Fkt!                     */
/*----------------------------------------------------------------------------*/



/* ========================================================================== */
/*                                                                            */
/*      L O C A L   I D E N T I F I E R   ( L I D )                           */
/*                                                                            */
/* ========================================================================== */

/*============================================================================*/
/*                                                                            */
/* LID for Service:  $10 StartDiagnosticSession - STDS                        */
/*                                                                            */
/*============================================================================*/
/*----------------------------------------------------------------------------*/
/* $00 .. $80 --> reserved                                                    */
/*-------------------------------------+--------------------------------------*/
#define d_CCkwpbase_LIDStandardMode                (UBYTE)   0x81u
#define d_CCkwpbase_LIDPeriodicTransmissions       (UBYTE)   0x82u
#define d_CCkwpbase_LIDEndOfLineVehicleManuMode    (UBYTE)   0x83u
#define d_CCkwpbase_LIDEndOfLineSysSupMode         (UBYTE)   0x84u
#define d_CCkwpbase_LIDProgMode                    (UBYTE)   0x85u
#define d_CCkwpbase_LIDDevelopmentMode             (UBYTE)   0x86u
#define d_CCkwpbase_LIDAdjustmentMode              (UBYTE)   0x87u
#define d_CCkwpbase_LIDVariantCodingMode           (UBYTE)   0x88u
/*----------------------------------------------------------------------------*/
/* $89 .. $F9 --> vehicle manufacturer specific defines                       */
/*----------------------------------------------------------------------------*/
/* $FA .. $FE --> system supplier specific defines                            */
/*----------------------------------------------------------------------------*/
/* $FF        --> reserved                                                    */
/*----------------------------------------------------------------------------*/

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* Positive Return Code (PRC) of "StartDiagnosticSession $10"                 */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* No entry */



/*=========================================================================*/
/*                                                                         */
/* $11 ECUReset - ER                                                       */
/*                                                                         */
/*=========================================================================*/

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Logical ID's (LID)                                                      */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/*-----------------------------------------------------------*/
/* $00 --> reserved                                          */
/*-------------------------------------+---------------------*/
#define d_CCkwpbase_LIDPowerOn                              (UBYTE)   0x01u
#define d_CCkwpbase_LIDPowerOnWhileMaintainingCommunication (UBYTE)   0x02u
/*-----------------------------------------------------------*/
/* $03 .. $3F --> reserved                                   */
/*-----------------------------------------------------------*/
#define d_CCkwpbase_LID_ResetStatus                         (UBYTE)   0x80u
/*-----------------------------------------------------------*/
/* $81 .. $F9 --> vehicle manufacturer specific defines      */
/*-----------------------------------------------------------*/
/*-----------------------------------------------------------*/
/* $FA .. $FE --> system supplier specific defines           */
/*-----------------------------------------------------------*/
/*-----------------------------------------------------------*/
/* $FF        --> reserved                                   */
/*-----------------------------------------------------------*/



/*=========================================================================*/
/*                                                                         */
/* $18 Read DTC By Status - RDTCBS                                         */
/*                                                                         */
/*=========================================================================*/

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Logical ID's (LID)                                                      */
/*-------------------------------------+-----------------------------------*/
#define d_CCkwpbase_LIDReqIdentifiedDTCAndStatus   (UBYTE)   0x00u
#define d_CCkwpbase_LIDReqSupportedDTCAndStatus    (UBYTE)   0x01u
/*-----------------------------------------------------------*/
/* $02 .. $F9 --> vehicle manufacturer specific defines      */
/*-----------------------------------------------------------*/
/*-----------------------------------------------------------*/
/* $FA .. $FE --> system supplier specific defines           */
/*-----------------------------------------------------------*/
/*-----------------------------------------------------------*/
/* $FF        --> reserved by document                       */
/*-----------------------------------------------------------*/

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Groups of DTC  ( Parameter 2)                             */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#define d_CCkwpbase_PowertrainGroup                     0x0000u
#define d_CCkwpbase_ChassisGroup                        0x4000u
#define d_CCkwpbase_BodyGroup                           0x8000u
#define d_CCkwpbase_UndefinedGroup                      0xC000u
#define d_CCkwpbase_AllGroups                           0xFF00u



/*=========================================================================*/
/*                                                                         */
/* $1A ReadECUIdentification - REI                                         */
/* $3B WriteDataByLocalId    - WDBLI                                       */
/*                                                                         */
/*=======================================================================*/

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Logical ID's (LID)                                                      */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#define d_CCkwpbase_LIDDataTable                        (UBYTE)     0x80u
#define d_CCkwpbase_LIDScalingTable                     (UBYTE)     0x81u
/*-----------------------------------------------------------*/
/* $82 .. $89 --> vehicle manufacturer specific defines      */
/*-----------------------------------------------------------*/
/* $8A .. $8F --> system supplier specific defines           */
/*-----------------------------------------------------------*/
/* $90 .. $99 --> KWP 2000 Norm 14230                        */
/*------+------------------------------+---------------------*/
#define d_CCkwpbase_LIDVehicleIdentificationNbr          (UBYTE)    0x90u
#define d_CCkwpbase_LIDVehicleManufacturerEcuHWNbr       (UBYTE)    0x91u
#define d_CCkwpbase_LIDSysSupplHWNbr                     (UBYTE)    0x92u
#define d_CCkwpbase_LIDSysSupplEcuHWVersionNbr           (UBYTE)    0x93u
#define d_CCkwpbase_LIDSysSupplEcuSWNbr                  (UBYTE)    0x94u
#define d_CCkwpbase_LIDSysSupplEcuSWVersionNbr           (UBYTE)    0x95u
#define d_CCkwpbase_LIDExRegOrTypeApprovalNbr            (UBYTE)    0x96u
#define d_CCkwpbase_LIDSysNameOrEngineType               (UBYTE)    0x97u
#define d_CCkwpbase_LIDRepShopCodeOrTesterSerialNbr      (UBYTE)    0x98u
#define d_CCkwpbase_LIDProgDate                          (UBYTE)    0x99u

/*-----------------------------------------------------------*/
/* $9A .. $9F --> vehicle manufacturer specific defines      */
/*-----------------------------------------------------------*/
#define d_CCkwpbase_LIDECUIdentification                 (UBYTE)    0x9Bu
#define d_CCkwpbase_LIDFlashStatus                       (UBYTE)    0x9Cu



/*=========================================================================*/
/*                                                                         */
/* $27 Security Access (SA)                                                */
/*                                                                         */
/*=========================================================================*/

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Logical ID's (LID)                                                      */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#define d_CCkwpbase_LIDMode1                               (UBYTE)   0x01u
#define d_CCkwpbase_LIDMode2                               (UBYTE)   0x02u
#define d_CCkwpbase_LIDMode3                               (UBYTE)   0x03u
#define d_CCkwpbase_LIDMode4                               (UBYTE)   0x04u

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Positive Return Code (PRC) of "Security Access $27"                     */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#define d_CCkwpbase_PRCSecurityAccessGranded               (UBYTE)   0x34u






/*=========================================================================*/
/*                                                                         */
/* $28 Disable Normal Message Transmission (DNMT)                          */
/*                                                                         */
/*=========================================================================*/

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Logical ID's (LID)                                                      */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#define d_CCkwpbase_LIDWithAnswer                        (UBYTE)   0x01u
#define d_CCkwpbase_LIDWithoutAnswer                     (UBYTE)   0x02u





/*=========================================================================*/
/*                                                                         */
/* $83 AccessTimingParameters - ATP                                        */
/*                                                                         */
/*=========================================================================*/

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Logical ID's (LID)                                                      */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#define d_CCkwpbase_LIDReadLimOfPosTimingPara             (UBYTE)   0x00u
#define d_CCkwpbase_LIDSetTimingParaToDefVal              (UBYTE)   0x01u
#define d_CCkwpbase_LIDReadCurActTimingPara               (UBYTE)   0x02u
#define d_CCkwpbase_LIDSetTimingParaToGivenVal            (UBYTE)   0x03u



/* ========================================================================== */
/*                                                                            */
/*      N E G A T I V E   R E S P O N S E   C O D E S   ( N R C )             */
/*                                                                            */
/* ========================================================================== */
/*------+------------------------------+---------------------*/
#define d_CCkwpbase_NRCGeneralReject                           (UBYTE)   0x10u
#define d_CCkwpbase_NRCServiceNotSupported                     (UBYTE)   0x11u
#define d_CCkwpbase_NRCSubFunctionNotSupported                 (UBYTE)   0x12u
#define d_CCkwpbase_NRCInvalidFormat                           (UBYTE)   0x12u
#define d_CCkwpbase_NRCincorrectMessageLengthOrInvalidFormat   (UBYTE)   0x13u
#define d_CCkwpbase_NRCBusyRepeatRequest                       (UBYTE)   0x21u
#define d_CCkwpbase_NRCConditionsNotCorrect                    (UBYTE)   0x22u
#define d_CCkwpbase_NRCRequestSequenceError                    (UBYTE)   0x22u
#define d_CCkwpbase_NRCRoutineNotComplete                      (UBYTE)   0x23u
#define d_CCkwpbase_NRCRequestOutOfRange                       (UBYTE)   0x31u
#define d_CCkwpbase_NRCSecurityAccessDenied                    (UBYTE)   0x33u
#define d_CCkwpbase_NRCSecurityAccessRequested                 (UBYTE)   0x33u
#define d_CCkwpbase_NRCInvalidKey                              (UBYTE)   0x35u
#define d_CCkwpbase_NRCExceedNumberOfAttempts                  (UBYTE)   0x36u
#define d_CCkwpbase_NRCRequiredTimeDelayNotExpired             (UBYTE)   0x37u
#define d_CCkwpbase_NRCDownloadNotAccepted                     (UBYTE)   0x40u
#define d_CCkwpbase_NRCImproperDownloadType                    (UBYTE)   0x41u
#define d_CCkwpbase_NRCCanNotDownloadToSpecifiedAddress        (UBYTE)   0x42u
#define d_CCkwpbase_NRCCanNotDownloadNumberOfBytesRequested    (UBYTE)   0x43u
#define d_CCkwpbase_NRCUploadNotAccepted                       (UBYTE)   0x50u
#define d_CCkwpbase_NRCImproperUploadType                      (UBYTE)   0x51u
#define d_CCkwpbase_NRCCanNotUploadFromSpecifiedAddress        (UBYTE)   0x52u
#define d_CCkwpbase_NRCCanNotUploadNumberOfBytesRequested      (UBYTE)   0x53u
#define d_CCkwpbase_NRCTransferSuspended                       (UBYTE)   0x71u
#define d_CCkwpbase_NRCTransferAborted                         (UBYTE)   0x72u
/*------+------------------------------+---------------------*/
#define d_CCkwpbase_NRCIllegalAddressInBlockTransfer           (UBYTE)   0x74u
#define d_CCkwpbase_NRCIllegalByteCountInBlockTransfer         (UBYTE)   0x75u
#define d_CCkwpbase_NRCIllegalBlockTransferType                (UBYTE)   0x76u
#define d_CCkwpbase_NRCBlockTransferDataChecksumError          (UBYTE)   0x77u
#define d_CCkwpbase_NRCRequestCorrrectReceive_RespPending      (UBYTE)   0x78u
#define d_CCkwpbase_NRCIncorrectByteCountDuringBlockTransfer   (UBYTE)   0x79u
#define d_CCkwpbase_NRC_SubSrvNSInActSession                   (UBYTE)   0x7Eu
#define d_CCkwpbase_NRC_SrvNSInActSession                      (UBYTE)   0x7Fu
#define d_CCkwpbase_NRC_SrvNSInActDiagMode                     (UBYTE)   0x80u

/*------------------------------------------------------------*/
/* $90 .. $F9 --> vehicle manufacturer specific defines       */
/*------------------------------------------------------------*/
/*------------------------------------------------------------*/
/* $FA .. $FE --> system supplier specific defines            */
/*------------------------------------------------------------*/


/* ========================================================================== */
/*                                                                            */
/*                                                                            */
/*      K W P 2 0 0 0   D E F I N E S                                         */
/*      -----------------------------------------------------------           */
/*      ( I S O   1 4 2 2 9   U D S  /  1 5 7 6 5 )                           */
/*                                                                            */
/* ========================================================================== */

/* ========================================================================== */
/*                                                                            */
/*      S E R V I C E   I D E N T I F I E R   ( S I D )                       */
/*                                                                            */
/* ========================================================================== */
#define  d_CCkwpbase_SIDDiagSessionControl            (UBYTE)   0x10u
#define  d_CCkwpbase_SIDCommunicationControl          (UBYTE)   0x28u
#define  d_CCkwpbase_SIDReadDataById                  (UBYTE)   0x22u
#define  d_CCkwpbase_SIDWriteDataById                 (UBYTE)   0x2Eu
#define  d_CCkwpbase_SIDRoutineControl                (UBYTE)   0x31u
#define  d_CCkwpbase_SIDControlDTCSetting             (UBYTE)   0x85u

/* ========================================================================== */
/*                                                                            */
/*      N E G A T I V E   R E S P O N S E   C O D E S   ( N R C )             */
/*                                                                            */
/* ========================================================================== */
#define d_CCkwpbase_NRCGeneralProgFailure             (UBYTE)   0x72u
#define d_CCkwpbase_NRCwrongBlockSequenceCounter      (UBYTE)   0x73u
#define d_CCkwpbase_NRCRequestSequenceError0x24       (UBYTE)   0x24u

/* ========================================================================== */
/*                                                                            */
/*      L O C A L   I D E N T I F I E R   ( L I D )                           */
/*                                                                            */
/* ========================================================================== */

/*============================================================================*/
/*                                                                            */
/* LID for Service:  $10 DiagnosticSessionControl                             */
/*                                                                            */
/*============================================================================*/
#define d_CCkwpbase_LIDDefaultSession              (UBYTE)   0x01u
#define d_CCkwpbase_LIDProgSession                 (UBYTE)   0x02u
#define d_CCkwpbase_LIDExtendedSession             (UBYTE)   0x03u

/*============================================================================*/
/*                                                                            */
/* LID for Service:  $11 ECUReset                                             */
/*                                                                            */
/*============================================================================*/
#define d_CCkwpbase_LIDhardReset                   (UBYTE)   0x01u

/*============================================================================*/
/*                                                                            */
/* LID for Service:  $27 SecurityAccess                                       */
/*                                                                            */
/*============================================================================*/
#define d_CCkwpbase_LIDGetSeed                     (UBYTE)   0x11u
#define d_CCkwpbase_LIDSendKey                     (UBYTE)   0x12u

/*============================================================================*/
/*                                                                            */
/* LID for Service:  $28 CommunicationControl                                 */
/*                                                                            */
/*============================================================================*/
#define d_CCkwpbase_LIDenableRxAndDisableTx        (UBYTE)   0x01u
#define d_CCkwpbase_LIDenableRxAndTx               (UBYTE)   0x00u

/*============================================================================*/
/*                                                                            */
/* LID for Service:  $31 RoutineControl                                       */
/*                                                                            */
/*============================================================================*/
#define d_CCkwpbase_LIDStartRoutine                (UBYTE)   0x01u
#define d_CCkwpbase_LIDStopRoutine                 (UBYTE)   0x02u
#define d_CCkwpbase_LIDReqRoutineResults           (UBYTE)   0x03u

/*============================================================================*/
/*                                                                            */
/* LID for Service:  $3E TesterPresent                                        */
/*                                                                            */
/*============================================================================*/
#define d_CCkwpbase_LIDzeroSubFunction             (UBYTE)   0x00u

/*============================================================================*/
/*                                                                            */
/* LID for Service:  $85 ControlDTCSetting                                    */
/*                                                                            */
/*============================================================================*/
#define d_CCkwpbase_LIDDTCOn                       (UBYTE)   0x01u
#define d_CCkwpbase_LIDDTCOff                      (UBYTE)   0x02u



/* ========================================================================== */
/*      C L O S E   M U L T I P L E   I N C L U D E   P R O T E C T I O N     */
/* ========================================================================== */
#endif
/* ========================================================================== */

/* ========================================================================== */
/*      E N D   O F   F I L E                                                 */
/* ========================================================================== */
