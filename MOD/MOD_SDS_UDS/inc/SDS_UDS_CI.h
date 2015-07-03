/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/
/** 
 * \file 	SDS_UDS_CI.h
 * \brief	CI - Configuration Internal File.  Detailed description found below.
 * \brief	This file mainly contains of:
 * 			1. constants to switch on or off the internal interfaces. 
 * 			2. module internal constants that can be configured by the module owner.
*/
/*-----------------------------------------------------------------------------
* Description:
*-----------------------------------------------------------------------------
* $Date: 2011-09-15 17:19:23 +0200 (Do, 15 Sep 2011) $
* $Rev: 21758 $
* $Author: butz $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_SDS_UDS/trunk/inc/SDS_UDS_CI.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _SDS_UDS_CI_H
#define _SDS_UDS_CI_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include "version.h"
#include "deh.h"
/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/
#include "EEPROM_LAYOUT_generated_ADR.h"
/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/
/** 
    \brief	interface definition to xxx.
*/

/* ----------------------------------------------------------------------- */
/*  - T y p e d e f s ---------------------------------------------------- */
/* ----------------------------------------------------------------------- */

/* return values of the function SDSext_WriteUserAndIOData() */
typedef enum
{
SDS_IO_USER_WRITE_SUCCESSFUL=0,            /* data available */
SDS_IO_USER_WRITE_FAILED,                  /* data not available */
SDS_IO_USER_WRITE_NOT_ALLOWED,             /* write isn't allowed, because security isn't active */
SDS_IO_USER_WRITE_OUT_OF_RANGE,            /* write isn't allowed, because out of range */
SDS_IO_USER_WRITE_REQUEST_SEQUENCE_ERROR,   /* on valid fingerprint available, necessary for Coding or Calibration */
SDS_IO_USER_WRITE_NO_RESPONSE_FROM_SUB_COMPONENT,
SDS_IO_USER_WRITE_TRY_AGAIN,                /* E2P busy, try again via RESPONSE PENDING handling  */
SDS_IO_USER_REPEAT_FOR_COMPLETE_E2P_WRITE   /* Repeat E2P request until write to E2P is finish - via RESPONSE PENDING handling  */
} tSDSextWrite_Return;

/* return values of the function SDSext_ReadUserAndIOData() */
typedef enum
{
SDSext_IO_USER_READ_SUCCESSFUL=0,        /* data available */
SDSext_IO_USER_READ_FAILED,              /* data not available */
SDSext_IO_USER_READ_NOT_ALLOWED,         /* read isn't allowed, because security isn't active */
SDSext_IO_USER_READ_IN_PROGRESS          /* still reading */
} tSDSextRead_Return;


typedef enum
{
    SDS_UDS_PROG_DATE = 0,
    SDS_UDS_REPAIR_SHOP_CODE,
    SDS_UDS_PROG_STATE
}extFingerprint;

typedef enum {
    DEH_EE_WAIT_TIME_IDLE = 0,
    DEH_EE_WAIT_TIME_START,
    DEH_EE_WAIT_TIME_STOP
} TriStateType_DEH_EE_Waiting_Time;

// Defaultwerte für Messwertböcke
#define DEFAULTMESSWERT                               0
#define DEFAULTTELEPHONECONNECTIVITYMESSWERT          0

// Offset and logicals Block size for $F15B
#define SDS_UDS_F15B_LOGICAL_BLOCK_SIZE_MAX             2

#define SDS_UDS_F15B_OFFSET_PROG_DATE                   3
#define SDS_UDS_F15B_OFFSET_REPAIR_SHOP_CODE            6
#define SDS_UDS_F15B_OFFSET_PROG_STATE                  1

#define SDS_UDS_F15B_LOGICAL_BLOCK_MAX                  (SDS_UDS_F15B_LOGICAL_BLOCK_SIZE_MAX - 1)
#define SDS_UDS_F15B_LOGICAL_BLOCK_MIN                  0

// calculated and 40 means 400ms
#define MAX_RESPONSE_FROM_AP_TASK                   5000                          // ca. 50 Sekunde warten
#define MAX_BUSY_RESPONSE_FROM_AP_TASK_AT_VAS_REQUEST 8000                       


// Mask for ubSecState (security access)
#define MASK_SEED_COUNTER        0x03
#define MASK_HIDE_SEED_COUNTER   0xFC
#define MASK_SEED_ALREADY_SEND   0x04
#define MASK_SEC_STATUS_LOCKED   0x08
#define MASK_SEC_STATUS_UNLOCKED 0xF4  
#define MASK_SEC_ACTIVE          0x40                      // user Login error
#define MASK_SEC_DEACTIVE        0xBF
#define MASK_SEC_DD_ACTIVE       0x80                      // Datensatz Download error
#define MASK_SEC_DD_DEACTIVE     0x7F
#define MASK_SEC_ALL_DEACTIVE    0x3B

/* 3 and more Bootloader is locked  */
#define BOOTLOADER_TIMELOC_STATE_LOCKED      0x03

/*
In UHV there are 2 login methods implemented
1) SAS2 Login (0x11/0x12) - Bootloader
2) Seed&Key (0x03/0x04)  - Normal Login
*/
#define NUMBER_OF_LOGIN_METHODS_IMPLEMETED    0x02

#define DTC_MASK_BIT                          0x80
#define DTC_CLEAR_MASK_BIT                    0x00

/*
  xxxxxxxx
  ||||||||-----   MASK_CODING_READY       
  |||||||------   MASK_CODING_RUNNING    
  ||||||-------   MASK_CALIBRATION_READY   
  |||||--------   MASK_CALIBRATION_RUNNING 
  ||||---------      
  |||----------      
  ||-----------      
  |------------                      
*/

// Mask for ubReadInitValue (Coding and Calibration
#define MASK_CODING_READY                     0x01
#define MASK_CODING_RUNNING                   0x02
#define MASK_CALIBRATION_READY                0x04
#define MASK_CALIBRATION_RUNNING              0x08
#define MASK_READ_INIT_READY                  0x0F

#define SEC_TIMER_DELAY          (10 * 60 * 100)     // 10min via 10ms Timer      

#define MAX_NUMBER_OF_DTC_MASKIERUNG          8

#ifdef SDSEXT_SERVICE86_ALLOWED  
// Index for u8Service86DataValues
#define Service86EventType                    0
#define Service86EventWindowTime              1
#define Service86EventTypeRecord              2
#define Service86ServiceToRespondToRecord     3

// states for u8Service86DataValuesInitStatus
#define Service86inactive                     0
#define Service86init                         1
#define Service86running                      2

#define Service86EventTypeCAN                                        RxTxBuf[0]
#define Service86EventType_StopResponseOnEvent                       0
#define Service86EventType_OnDTCStatusChange                         1
#define Service86EventType_StartResponseOnEvent                      5

#define Service86EventWindowTimeCAN                                  RxTxBuf[1]
#define Service86EventWindowTime_InfiniteTimeToResponse              2
#define Service86EventWindowTime_ActualCycle                         3

#define Service86EventTypeRecordCAN                                  RxTxBuf[2]
#define Service86EventTypeRecord_TestFailed                          1
#define Service86EventTypeRecord_ConfirmedDTC                        8        // BIT 3
#define Service86ServiceToRespondToRecordCAN                         RxTxBuf[5]
#define Service86ServiceToRespondToRecord_ReportFirstTestFailed      0x09
#define Service86ServiceToRespondToRecord_ReportFirstPending         0x0C
#endif

#define Service34IDplusBSN                     2             // ServiceIdentifier und BlockSequenceNumber (BSN)
#define Service34MaxNumberOfBlockLength        32             // die maximale Anzahl der in jeder TransferData-Message enthaltenen Nutz-Datenmenge

#define BASIC_SETTINGS_NOT_RUNNING     0x00
#define BASIC_SETTINGS_ABORTED         0x60
#define BASIC_SETTINGS_FINISHED        0x10
#define BASIC_SETTINGS_RUNNING         0xC0
#define BASIC_SETTINGS_CANNOT_BE_STOPPED         0x80

#define MAX_EEP_WRITE_RETRY_COUNTER    100                 // so oft soll maximal versucht werden ins EEPROM zu schreiben

#define FIX_LOGIN_OFFSET         9281              // Secirty code for normal login

#define MASK_INVALIDEEPROM_DATA                0x01

#define SDSext_EUVarianteSupport            SDS_ENABLED  // EU Varianten unterstützen?

#define FLASH_REQUEST_PADDERN_BOOT          0x55AA0FF0UL
#define FLASH_REQUEST_PADDERN_APP           0x55AAF00FUL
#define FLASH_REQUEST_PADDERN_DEF_SESSION   0x55AA0AA0UL

/* negative response codes, that are not present in standard SW */
#define SDS_UDS_NORESPONSEFROMSUBNETCOMPONENT     (byte)0x25

#if( (SDS_A_START_DIAGNOSTIC_SESSION == SDSext_ENABLED) && (SDS_ECU_PROGRAMABLE != 0) )
extern void SDSext_vSetProgRequest( void );
#endif

#define DEFAULT_USER_PROFILES                          0           // not delete
#define DEFAULT_RESET_TO_FACTORY_SETTING               1           // off

extern void SDSext_vDoReset(Uint8Type ubResetControl);

extern void SDSext_vResetSecurityAccessEffected(void);

extern Uint8Type SDSext_ubComCtrl(Uint8Type ControlType, Uint8Type CommNetworkType);

extern tSDSextRead_Return SDSext_eReadUserAndIOData(Uint8Type* data,
		const t_ReadDataIdCfgItem* DataIdItem);
/*----------------------------------------------------------------------------*/
/* Import configuration                                                       */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of configured interface
*/
#define IUART_SDS_UDS_CFG 110

/*----------------------------------------------------------------------------*/
/* Synonym adaptions                                    */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Prototypes of module global methods                                        */
/*----------------------------------------------------------------------------*/
extern tSDSextWrite_Return SDSext_EEPWriteUniversalData(uint16_t EEP_Position,
		uint16_t BlockSize_u16, uint8_t *Buffer_u8);

tSDSextWrite_Return SDSext_EEPWriteUniversalData_WithRetryMechanism(uint16_t EEP_Position,	uint16_t BlockSize_u16, uint8_t *Buffer_u8);

extern tSDSextRead_Return SDSext_EEPReadUniversalData(uint16_t EEP_Position,
		uint16_t BlockSize_u16, uint8_t *Buffer_u8);

extern void _SDS_UDS_vResetOfAdaptionValues(BooleanType start,
		BooleanType BasicSetting);

extern Uint8Type SDS_UDS_u8ProcessorStartUpDiagnoseSequence(Uint8Type ubReadInitValue);

extern void SDS_UDS_vTimerHandlingStellgliedTests(void);

extern tSDSextWrite_Return _SDS_UDS_vService2F_CallApp_Diag(Uint16Type ID, Uint8Type data);

extern void SDS_UDS_vInitialiseService2E(void);

extern void SDS_UDS_vInitialiseService2F(void);

extern void SDS_UDS_vInitialiseService22(void);

extern Uint8Type _SDS_UDS_u8GetStatusBasicSettings(void);

extern BooleanType _SDS_UDS_bDiagnosisCodingConnectCheck(const Uint8Type* data);

extern void SDS_UDS_vInitialiseServiceDataDownload(void);

extern void SDS_UDS_vInitialiseServiceSecurityCheck(void);

extern void SDS_UDS_vInitialiseServiceDTC(void);

extern void SDS_UDS_vInitialiseService31(void);

extern BooleanType _SDS_UDS_bResetOfAdaptionValuesStatus(void);

extern void SDS_UDS_vChecksumStart(void);

extern Uint8Type _SDS_UDS_ubChecksumStatus(void);

extern Uint16Type SDS_UDS_u16CalculateDtcMaskierung(Uint16Type index, Uint8Type *RxTxBuf );

/*----------------------------------------------------------------------------*/
/* Module global code macros                                                         */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the macros
*/
#define sqrt(x) ((x)*(x))

/*----------------------------------------------------------------------------*/
/* Module global variables                                                    */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the module global variable
*/

extern APP_DIAG_ST_DIAG_RESPONSE _SDS_UDS_stResponsePacketToVas;
extern APP_DIAG_ST_DIAG_REQUEST _SDS_UDS_stRequestPacketFromVas;

#ifdef SDSEXT_SERVICE86_ALLOWED  
extern Uint8Type u8Service86DataValuesInitStatus;
extern Uint8Type u8Service86DataValuesDTC[DEH_MAX_NUMBER_OF_EVENTPATHS*3];
extern Uint8Type u8Service86DataValues [VWDB_EE_ADDDRESS_Service86ResponseOnEvent_LEN];
#endif
extern Uint8Type ubSecState;
extern Uint8Type DTC_Maskiert[MAX_NUMBER_OF_DTC_MASKIERUNG * 8] [3];

extern Uint8Type u8DTC_Maskierung[MAX_NUMBER_OF_DTC_MASKIERUNG] ;
extern Uint8Type SDS_UDS_u8DTC_Mask_Counter;
extern Uint16Type           TimerSecurity                           ;

/*    Service 2E specific */
extern Uint8Type
		_SDS_UDS_au8FingerprintValue[ID_2E_F198_WERKSTATTCODE_DAS_ANGESCHLOSSENEN_TESTERS_MAX_LEN];
extern Uint8Type
		_SDS_UDS_au8ProgrammingDate[ID_2E_F199_PROGRAMMING_DATE_MAX_LEN];
extern Uint8Type ub_RequestPending;
extern Uint8Type ubResponseFromAppTask;
extern Uint16Type ubCounterResponseFromAppTask;
extern Uint16Type uwBufSize;
extern Uint8Type _SDS_UDS_u8UserProfile;

extern Uint8Type _SDS_UDS_u8ResetToFactory;

extern Uint16Type TimerSecurity;
extern Uint8Type RandomCounter[4]; // wird "pseudo komplex" in SDSext_vTask manipuliert;


extern const Uint8Type DTC_Mask[MAX_NUMBER_OF_DTC_MASKIERUNG * 8] [3];

extern const t_ReadDataIdCfgItem cs_ReadDataIdItem[85];

extern BooleanType _SDS_UDS_bStopCommunicationMessages;

extern BooleanType _SDS_UDS_bResetOfAdaptionValuesState;

extern Uint8Type ub_RequestPending22;

extern extFingerprint logBlockState;

// $F181: Identifikation der Standardsoftware: VW Standard Software Modul
extern const Uint8Type
		VERSION_VALUES[ID_22_F181_IDENTIFIKATION_DER_STANDARDSOFTWARE_MAX_LEN];


// Status for IO Basic Settings Status   0 = OFF, 1 = ON
extern Uint8Type
		_SDS_UDS_u8Service31_BasicSettingsStatus;
// Timer for Basic Setting
extern Uint16Type _SDS_UDS_u16TimerBasicSettings;

extern Uint32Type Set_FlashRequest_Paddern;

extern Uint8Type u8PhoneNumberLength;

extern tSDSextRead_Return eReadInProgress;
/*----------------------------------------------------------------------------*/
/* Module global const definitions                                                  */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the #define
*/
#define	EXAMPLE_DEFINE		1


#endif /* _SDS_UDS_CI_H */

/* End Of File SDS_UDS_CI.h */
