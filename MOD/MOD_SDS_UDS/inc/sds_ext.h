/******************************************************************************/
/*****************  Copyright (c) 2010 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:	sds_ext.h
*-----------------------------------------------------------------------------
* Module Name:	MOD_SDS
*-----------------------------------------------------------------------------
* Description:	
*-----------------------------------------------------------------------------
* $Date: 2010-11-17 16:11:46 +0100 (Mi, 17 Nov 2010) $
* $Rev: 16349 $
* $Author: butz $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_SDS_UDS/trunk/inc/sds_ext.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef  _SDS_EXT_H_
#define  _SDS_EXT_H_

#include "i2s_types.h"
#include "uds.h"
#include "tp.h"
#include "EEPROM.h"
/*--------------------------------------------------------------------------*/
/* Identifications SG for Service $22                                       */
/*--------------------------------------------------------------------------*/
#define SWVersion                           SoftwareVersion

#define ASAMODXFILEVERSION                  AsamOdxFileVersion /* EV Bezeichnung */


#define SDS_UDS_DEVELOPMENT_FUNCTION_START_ADDRESS                       EEPROM_ADDR_MIN
#define SDS_UDS_DEVELOPMENT_FUNCTION_END_ADDRESS                         EEPROM_ADDR_MAX + SDS_UDS_MAX_CHECKSUM_BYTES

#define SDS_22_F181_TheftProtectionGetVersionInfoAvailable			  SDS_DISABLED
#define SDS_22_F181_BAP_GetVersionInfoAvailable								    SDS_ENABLED
#define SDS_22_F181_UDS_GetVersionInfoAvailable								    SDS_ENABLED
#define SDS_22_F181_DEH_GetVersionInfoAvailable								    SDS_ENABLED
#define SDS_22_F181_CANDriverGetVersionInfoAvailable						  SDS_ENABLED
#define SDS_22_F181_VW_TP20GetVersionInfoAvailable							  SDS_DISABLED
#define SDS_22_F181_VW_TP16GetVersionInfoAvailable							  SDS_DISABLED
#define SDS_22_F181_TP_GetVersionInfoAvailable								    SDS_ENABLED
#define SDS_22_F181_KWP2000GetVersionInfoAvailable							  SDS_DISABLED
#define SDS_22_F181_OSEK_OperatingSystemGetVersionInfoAvailable	  SDS_ENABLED
#define SDS_22_F181_OSEK_NetworkmanagementGetVersionInfoAvailable	SDS_DISABLED
#define SDS_22_F181_HighSpeedNetmanagementGetVersionInfoAvailable	SDS_DISABLED
#define SDS_22_F181_OSEK_CommunikationGetVersionInfoAvailable			SDS_DISABLED
#define SDS_22_F181_LIN13GetVersionInfoAvailable							    SDS_DISABLED
#define SDS_22_F181_LIN20GetVersionInfoAvailable							    SDS_DISABLED

/* supported security access type */ 
#define UDScub_SECURITY_ACCESS_SEED                               0x03
#define UDScub_SECURITY_ACCESS_KEY                                0x04
#define UDScub_SECURITY_ACCESS_SEED_SYSTEMSPEZIFISCH_VAS5163      0x09

/* general negative response codes for sercives */
#define SDS_UDS_GENERAL_REJECT                                  (byte)0x10
#define SDS_UDS_REQUEST_SEQUENCE_ERROR                          (byte)0x24
#define SDS_UDS_NO_RESPONSE_FROM_SUBNET_COMPONENT               (byte)0x25
#define SDS_UDS_FAILURE_PREVENTS_EXECUTION_OF_REQUESTED_ACTION  (byte)0x26
#define SDS_UDS_EXCEED_NUMBER_OF_ATTEMPTS                       (byte)0x36
#define SDS_UDS_UPLOAD_DOWNLOAD_NOT_ACCEPTED                    (byte)0x70
#define SDS_UDS_TRANSFER_DATA_SUSPENDED                         (byte)0x71
#define SDS_UDS_GENERAL_PROGRAMMING_FAILURE                     (byte)0x72
#define SDS_UDS_WRONG_BLOCK_SEQUENCE_NUMBER                     (byte)0x73
#define SDS_UDS_RPM_TOO_LOW                                     (byte)0x82
#define SDS_UDS_ENGINE_IS_RUNNING                               (byte)0x83
#define SDS_UDS_ENGINE_IS_NOT_RUNNING                           (byte)0x84
#define SDS_UDS_ENGINE_RUN_TIME_Too_Low                         (byte)0x85
#define SDS_UDS_TEMPERATURE_TOO_HIGH                            (byte)0x86
#define SDS_UDS_TEMPERATURE_TOO_LOW                             (byte)0x87
#define SDS_UDS_VEHICLE_SPEED_TOO_LOW                           (byte)0x89
#define SDS_UDS_THROTTLE_PEDAL_TOO_HIGH                         (byte)0x8A
#define SDS_UDS_THROTTLE_PEDAL_TOO_LOW                          (byte)0x8B
#define SDS_UDS_TRANSMISSION_RANGE_NOT_IN_NEUTRAL               (byte)0x8C
#define SDS_UDS_TRANSMISSION_RANGE_NOT_IN_GEAR                  (byte)0x8D
#define SDS_UDS_BRAKE_SWITCH_NOT_CLOSED                         (byte)0x8F
#define SDS_UDS_VOLTAGE_TOO_HIGH                                (byte)0x92
#define SDS_UDS_VOLTAGE_TOO_LOW                                 (byte)0x93

#define CODING_VALUES                            7

// Watchdog paddern mask
#define WD_PADDERN_POWER_UP                      0xAA
#define WD_PADDERN_POWER_DOWN                    0x55
#define WD_PADDERN_LOWVOLTAGE                    0x66
#define WD_PADDERN_HIGHVOLTAGE                   0x99

// Stellgliedtest Testtöne
#define SDS_UDS_SIGNAL_FREQUENCY_1KHZ                      1000
#define SDS_UDS_SIGNAL_FREQUENCY_333HZ                      333
#define SDS_UDS_SIGNAL_CHANNEL_ON                          AT_TRUE
#define SDS_UDS_SIGNAL_CHANNEL_OFF                         AT_FALSE
#define SDS_UDS_SIGNAL_OUTPUT_LEVEL_1VEFF                 16384


#define SERVICE_2F_FKT_START_EIN                                          0x01
#define SERVICE_2F_FKT_START_AUS                                          0x02
#define SERVICE_2F_FKT_STOP_LAST_STATE                                    0x03

/* ----------------------------------------------------------------------- */
/*  - E x t e r n a l s -------------------------------------------------- */
/* ----------------------------------------------------------------------- */
extern void SDSext_vTask( void );
extern void SDSext_vInit( word bufsize );

SuccessFailType SDSext_DIAG_vHandleEvents(EventMaskType event);

boolean SDSext_GetUhvInRestrictedMode(void);

void SDS_UDS_vDiagTimeout(void);

extern Uint8Type SDS_UDS_u8GetStatusActuatorTest(void);

BooleanType SDS_UDS_GetAdditionalDataDownloadStatus( void );

Uint8Type SDS_UDS_u8GetKundenDienstBit(void);
Uint8Type SDS_UDS_u8GetBootloaderTimeLockState(void);
void SDS_UDS_vSetBootloaderTimeLockState( Uint8Type SetBootloaderTimeLockState);

/* ----------------------------------------------------------------------- */
/*  - G l o b a l s ------------------------------------------------------ */
/* ----------------------------------------------------------------------- */

/* ----------------------------------------------------------------------- */
/*  - T y p e d e f s ---------------------------------------------------- */
/* ----------------------------------------------------------------------- */


typedef struct
{
  word LID;                        /* Low id */
  byte ModeAvailability;           /* supported in session */
} t_DiagServiceSubItem;


typedef struct
{
  void (*func)(Uint8Type);         /* address of the callback function, 0 = not supported */ 
} t_DiagCodingItem;


static const t_DiagServiceSubItem cs_Sercive11_LID_Session[] = {
/*  |-------------------- LID 
    |    |-------- valid in session(s) */
{ 0x01, 0x02 },
{ 0x02, 0x2E },
{ 0x03, 0x2E }
};

/* structure for data ID configuration array cs_DataIdItem: see sds_cfg.h */
typedef struct
{
  word DataId;                   /* data id */
  enum
  {
    SDSext_ASCII = 0,               /* read ascii data from config table */
    SDSext_BINARY,                  /* read binary data from config table */
    SDSext_RAM,                     /* read data from RAM by address */
    SDSext_ROM,                     /* read data from ROM by address */
    SDSext_EEPROM,                  /* read data from EEPROM */
    SDSext_SPECIAL_EEPROM,          /* read data from EEPROM by more addresses */
    SDSext_USER                     /* user define read of data */
  } extMem;                         /* data access type */
  word Len;                      /* length of data */
  void* Address;                 /* data address or index in data table */
} t_ReadDataIdCfgItem;

#define ACTUATOR_TEST_NOT_RUNNING     0x00
#define ACTUATOR_TEST_ABORTED         0x40
#define ACTUATOR_TEST_FINISHED        0x80
#define ACTUATOR_TEST_RUNNING         0xC0

//ECUDataProgrammingInfo handling
#define SET_UNGULTIGE_EEPROM_DATEN	0x40
#define SET_FEHLER_EEPROM_DATEN		0x80
#define RESET_GULTIGE_EEPROM_DATEN	0x3F

void * sdsMemCpy( void * dest, const void * src , size_t count );
#endif
