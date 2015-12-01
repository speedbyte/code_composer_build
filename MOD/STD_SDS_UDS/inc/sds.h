/**************************************************************************
 *  COPYRIGHT (C) Volkswagen AG, 2006-2007                                *
 *  ALLE RECHTE VORBEHALTEN. ALL RIGHTS RESERVED.                         *
 *                                                                        *
 **************************************************************************
 *  FILE: SDS.H
 **************************************************************************
 *  PURPOSE:
 *  Application H-File for SDS Module
 *
 *************************************************************************
 *  CHANGE HISTORY: In2Soft GmbH
 *
 *  Version    Name     Date   Description
 **************************************************************************
 *  V_00_01_00 Ki 12.10.2006 - first version 
 *  V_00_01_01 Ki 02.11.2006 - new function: SDS_GetVersionInfo 
 *  V_00_01_02 Ki 12.12.2006 - SDS_28_Sub-Network1_14 --> SDS_28_Sub_Network1_14
 *                           - i2s_types.h included
 *                           - use of i2s type instead of C types
 *  V_00_01_03 Ki 19.12.2006 - no change
 *  V_00_01_04 Ki 10.01.2007 - no change
 *  V_00_01_05 Ki 12.01.2007 - no change
 *  V_00_01_06 Ki 02.03.2007 - general revision, review by VW
 *  V_01_00_00 Ki 30.03.2007 - SDS_ENABLED, SDS_DISABLED moved into sds.h
 *  V_01_00_01 Ki 11.04.2007 - new: SDS_85_On, SDS_85_Off, SDSext_vSetProgRequest
 *  V_01_00_02 Ki 24.04.2007 - new: SDSext_vResetSecurityAccessEffected
 *  V_01_00_03 Ki 02.07.2007 - SDS_22_FFFF_TerminationValue deleted
 *  V_01_00_04 Ki 11.07.2007 - no change
 *  V_01_00_05 Ki 23.07.2007 - no change
 *  V_01_00_06 Ki 02.11.2007 - #include "sds_cfg.h" after SDS_EN/DISABLED definition
 *  V_01_00_07 Ki 04.02.2008 - no change
 *  V_01_00_08 Ki 18.06.2008 - byte SDSext_ubComCtrl() --> void SDSext_vComCtrl()
 *  V_01_00_09 Ki 24.06.2008 - no change 
 *  V_01_00_10 Ki 23.07.2008 - no change 
 *  V_01_01_00 Ki 08.08.2008 - Preprocessor defines for SDSext... revised 
 *                           - void SDSext_vComCtrl() --> byte SDSext_ubComCtrl()
*************************************************************************/

#ifndef  _I2S_SDS_H_
#define  _I2S_SDS_H_

#include "i2s_types.h"

/* ----------------------------------------------------------------------- */
/*  - T y p e d e f s  --------------------------------------------------- */
/* ----------------------------------------------------------------------- */

/* return values of the function SDSext_ReadUserAndIOData() */
typedef enum
{
SDS_IO_USER_READ_SUCCESSFUL=0,        /* data available */
SDS_IO_USER_READ_FAILED,              /* data not available */
SDS_IO_USER_READ_IN_PROGRESS          /* still reading */
} tSDS_Return;

/* structure for data ID configuration array cs_DataIdItem: see sds_cfg.h */
typedef struct 
{
  word DataId;                   /* data id */
  enum
  {
    SDS_ASCII = 0,               /* read ascii data from config table */
    SDS_BINARY,                  /* read binary data from config table */
    SDS_RAM,                     /* read data from RAM by address */
    SDS_ROM,                     /* read data from ROM by address */
    SDS_EEPROM,                  /* read data from EEPROM */
    SDS_USER                     /* user define read of data */
  } Mem;                         /* data access type */
  word Len;                      /* length of data */
  void* Address;                 /* data address or index in data table */
} t_DataIdCfgItem;

/* ----------------------------------------------------------------------- */
/*  - C o n s t a n t s -------------------------------------------------- */
/* ----------------------------------------------------------------------- */
#define SDS_ENABLED     1
#define SDS_DISABLED    0

#include "sds_cfg.h"             /* do not move this line */

/* Service 19 sub IDs */
#define SDS_19_ReportNumberOfDTCByStatusMask                            0x01
#define SDS_19_ReportDTCByStatusMask                                    0x02
#define SDS_19_ReportDTCSnapshotRecordByDTCNumber                       0x04
#define SDS_19_ReportDTCExtendedDataRecordByDTCNumber                   0x06
#define SDS_19_ReportNumberOfEmissionRelatedOBDDTCByStatusMask          0x12
#define SDS_19_ReportEmissionRelatedOBDDTCByStatusMask                  0x13
#define SDS_19_StandardDTCInformation                                   0x01
#define SDS_19_StandardFreezeFrame_1                                    0x02
#define SDS_19_AllStandardFreezeFrameAndStandardDTCInformation          0x8F
#define SDS_19_AllOBDExtendedDataRecordNumbers                          0xFE
#define SDS_19_AllDTCExtendedDataRecordNumbers                          0xFF

/* Service 11 ResetType */
#define SDS_11_HardReset                                                0x01
#define SDS_11_KeyOffOnReset                                            0x02
#define SDS_11_SoftReset                                                0x03
#define SDS_11_EnableRapidPowerShutDownSequence                         0x04
#define SDS_11_DisableRapidPowerShutDownSequence                        0x05

/* Service 28 CommunicationControl */
#define SDS_28_EnableRxAndTx                                            0x00
#define SDS_28_EnableRxAndDisableTx                                     0x01
#define SDS_28_DisableRxAndEnableTx                                     0x02
#define SDS_28_DisableRxAndTx                                           0x03
#define SDS_28_GatewayVehicleWakeUp                                     0x40
#define SDS_28_NormalCommunicationMessagesOnly                          0x01
#define SDS_28_NetworkManagementCommunicationMessagesOnly               0x02
#define SDS_28_NormalAndNetworkManagmentCommunicationMessages           0x03
#define SDS_28_AllNetworks                                              0x00
#define SDS_28_Sub_Network1                                             0x10
#define SDS_28_Sub_Network14                                            0xE0
#define SDS_28_MainNetwork                                              0xF0

/* Service 85 ControlDTCSetting */
#define SDS_85_On                                                       0x01
#define SDS_85_Off                                                      0x02

/* ----------------------------------------------------------------------- */
/*  - G l o b a l s ------------------------------------------------------ */
/* ----------------------------------------------------------------------- */
extern void SDS_vTask( void );
extern void SDS_vInit( word bufsize );
extern void SDS_vGetVersionInfo( I2S_VersionInfoType *versioninfo );

/* CallBacks */
#if( SDS_A_RESET_OR_DISCONNECT == SDS_ENABLED )
extern void SDSext_vDoReset( byte ubResetControl );
#if( SDS_11_POWER_DOWN == SDS_ENABLED )
extern byte SDSext_ubPowerDown( byte ResetType );
#endif
#endif

#if( SDS_A_START_DIAGNOSTIC_SESSION == SDS_ENABLED )
extern void SDSext_vResetSecurityAccessEffected( void );
#endif

#if( (SDS_A_START_DIAGNOSTIC_SESSION == SDS_ENABLED) || (SDS_A_COMMUNICATION_CONTROL == SDS_ENABLED) )
extern byte SDSext_ubComCtrl( byte ControlType, byte CommNetworkType ); 
#endif

#if( SDS_A_READ_DATA_BY_IDENTIFIER == SDS_ENABLED )
extern tSDS_Return SDSext_eReadUserAndIOData(byte* data, const t_DataIdCfgItem* DataIdItem);
#endif

#if( (SDS_A_START_DIAGNOSTIC_SESSION == SDS_ENABLED) && (SDS_ECU_PROGRAMABLE != 0) )
extern void SDSext_vSetProgRequest( void );
#if( SDS_10_ZeroVehicleOrEngineSpeed == SDS_ENABLED ) 
extern byte SDSext_ubZeroVehicleOrEngineSpeed(byte ServiceID);
#endif
#endif

#if( (SDS_A_COMMUNICATION_CONTROL == SDS_ENABLED) && (SDS_28_ZeroVehicleOrEngineSpeed == SDS_ENABLED) ) 
extern byte SDSext_ubZeroVehicleOrEngineSpeed(byte ServiceID);
#elif( (SDS_A_CTRL_DTC_SETTINGS == SDS_ENABLED) && (SDS_85_ZeroVehicleOrEngineSpeed == SDS_ENABLED) ) 
extern byte SDSext_ubZeroVehicleOrEngineSpeed(byte ServiceID);
#endif

#endif
