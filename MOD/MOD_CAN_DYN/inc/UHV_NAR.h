/**************************************************************
*  File C:\_workspace_\VW_UHV_NAR_trunk\MOD\MOD_CAN_DYN\inc\UHV_NAR.h
*  generated at Thu Apr 21 11:02:30 2011
*             Toolversion:   415
*               Bussystem:   Can
*
*  generated out of CANdb:   C:\_workspace_\VW_UHV_NAR_trunk\MOD\MOD_CAN_DYN\cfg\PQ35_46_ICAN_V3_6_9_F_20081104_ASR_V1_2.dbc
*            Manufacturer:   Volkswagen AG
*                for node:   UHV_NAR
*   Generation parameters:   Target system = TMS470
*                            Compiler      = Texas Instruments
*
* License information:       
*   -    Serialnumber:       CBD0500058
*   - Date of license:       26.7.2005
*
***************************************************************
Software is licensed for:    
Volkswagen AG
EESN/1
VAG
TMS470
Texas Instruments
**************************************************************/

#ifndef CAN_UHV_NAR_H
#define CAN_UHV_NAR_H

#define __UHV_NAR__

#define VERSIONYEAR 0x08
#define VERSIONMONTH 0x11
#define VERSIONWEEK 0x45
#define VERSIONDAY 0x04

#define VERSIONNUMBER 0x1

#define GEN_MAGIC_NUMBER_UHV_NAR 1303376549

#ifndef TOOLVERSION
#define TOOLVERSION 415
#endif
#ifndef DLLVERSION
#define DLLVERSION 415
#endif


#ifndef GEN_VERSION
#define GEN_VERSION 0x0415
#define GEN_BUGFIX_VERSION 0x50

V_MEMROM0 extern V_MEMROM1 canuint8 V_MEMROM2 kGenMainVersion;
V_MEMROM0 extern V_MEMROM1 canuint8 V_MEMROM2 kGenSubVersion;
V_MEMROM0 extern V_MEMROM1 canuint8 V_MEMROM2 kGenBugfixVersion;

#endif
/* start skeleton Texas Tms470   H-File ----------------------------------------*/
/******************************************************************************
| Version check
|*****************************************************************************/
# define VTOOL_SKELETON_VERSION 0x01
# define VTOOL_SKELETON_BUGFIX_VERSION 0x00 
/* ende  skeleton Texas Tms470   H-File ----------------------------------------*/


/*************************************************************/
/* Signal structures                                          */
/*************************************************************/

/* Send messages */

typedef struct {
	canbittype 	ISO_Telefon_Resp_Data_0: 8;
	canbittype 	ISO_Telefon_Resp_Data_1: 8;
	canbittype 	ISO_Telefon_Resp_Data_2: 8;
	canbittype 	ISO_Telefon_Resp_Data_3: 8;
	canbittype 	ISO_Telefon_Resp_Data_4: 8;
	canbittype 	ISO_Telefon_Resp_Data_5: 8;
	canbittype 	ISO_Telefon_Resp_Data_6: 8;
	canbittype 	ISO_Telefon_Resp_Data_7: 8;
} _c_ISO_Telefon_Resp_msgType;

typedef struct {
	canbittype 	ASR5_Char24: 8;
	canbittype 	ASR5_Char25: 8;
	canbittype 	ASR5_Char26: 8;
	canbittype 	ASR5_Char27: 8;
	canbittype 	ASR5_Char28: 8;
	canbittype 	ASR5_Char29: 8;
	canbittype 	ASR5_Char30: 8;
	canbittype 	ASR5_Char31: 8;
} _c_mASR5_String3_msgType;

typedef struct {
	canbittype 	ASR4_Char16: 8;
	canbittype 	ASR4_Char17: 8;
	canbittype 	ASR4_Char18: 8;
	canbittype 	ASR4_Char19: 8;
	canbittype 	ASR4_Char20: 8;
	canbittype 	ASR4_Char21: 8;
	canbittype 	ASR4_Char22: 8;
	canbittype 	ASR4_Char23: 8;
} _c_mASR4_String2_msgType;

typedef struct {
	canbittype 	ASR3_Char08: 8;
	canbittype 	ASR3_Char09: 8;
	canbittype 	ASR3_Char10: 8;
	canbittype 	ASR3_Char11: 8;
	canbittype 	ASR3_Char12: 8;
	canbittype 	ASR3_Char13: 8;
	canbittype 	ASR3_Char14: 8;
	canbittype 	ASR3_Char15: 8;
} _c_mASR3_String1_msgType;

typedef struct {
	canbittype 	ASR2_Char00: 8;
	canbittype 	ASR2_Char01: 8;
	canbittype 	ASR2_Char02: 8;
	canbittype 	ASR2_Char03: 8;
	canbittype 	ASR2_Char04: 8;
	canbittype 	ASR2_Char05: 8;
	canbittype 	ASR2_Char06: 8;
	canbittype 	ASR2_Char07: 8;
} _c_mASR2_String0_msgType;

typedef struct {
	canbittype 	unused0: 1;
	canbittype 	ASR1_Cnt_Multiplex_String: 4;
	canbittype 	ASR1_Is_Multiplex_Last_Msg: 1;
	canbittype 	ASR1_Is_String_TimeMultiplexed: 1;
	canbittype 	ASR1_Is_Command_ID: 1;
	canbittype 	ASR1_Confidence_0: 8;
	canbittype 	ASR1_Confidence_1: 8;
	canbittype 	ASR1_Command_ID_0: 8;
	canbittype 	ASR1_Command_ID_1: 8;
	canbittype 	ASR1_Command_ID_2: 8;
	canbittype 	unused1: 3;
	canbittype 	ASR1_Cnt_SubCmd: 4;
	canbittype 	ASR1_Is_SubCmd_Last_Msg: 1;
	canbittype 	unused2: 3;
	canbittype 	ASR1_Cnt_NBest: 4;
	canbittype 	ASR1_Is_NBest_Last_Msg: 1;
} _c_mASR1_TypeInfo_msgType;

typedef struct {
	canbittype 	unused0: 6;
	canbittype 	ASR0_Status_RecogOpen: 1;
	canbittype 	ASR0_Status_Activ: 1;
	canbittype 	ASR0_Status_StateID_0: 8;
	canbittype 	ASR0_Status_StateID_1: 8;
	canbittype 	ASR0_Status_StateID_2: 8;
} _c_mASR0_Status_msgType;

typedef struct {
	canbittype 	BAP_Debug_Data_0: 8;
	canbittype 	BAP_Debug_Data_1: 8;
	canbittype 	BAP_Debug_Data_2: 8;
	canbittype 	BAP_Debug_Data_3: 8;
	canbittype 	BAP_Debug_Data_4: 8;
	canbittype 	BAP_Debug_Data_5: 8;
	canbittype 	BAP_Debug_Data_6: 8;
} _c_BAP_Debug_Data_msgType;

typedef struct {
	canbittype 	BAP_Data_Telefon_2_0: 8;
	canbittype 	BAP_Data_Telefon_2_1: 8;
	canbittype 	unused0: 8;
	canbittype 	unused1: 8;
	canbittype 	unused2: 8;
	canbittype 	unused3: 8;
	canbittype 	unused4: 8;
	canbittype 	unused5: 8;
} _c_BAP_Telefon_02_msgType;

typedef struct {
	canbittype 	BAP_Data_Telefon_1_0: 8;
	canbittype 	BAP_Data_Telefon_1_1: 8;
	canbittype 	unused0: 8;
	canbittype 	unused1: 8;
	canbittype 	unused2: 8;
	canbittype 	unused3: 8;
	canbittype 	unused4: 8;
	canbittype 	unused5: 8;
} _c_BAP_Telefon_01_msgType;

typedef struct {
	canbittype 	BAP_Data_Telefon_4_0: 8;
	canbittype 	BAP_Data_Telefon_4_1: 8;
	canbittype 	unused0: 8;
	canbittype 	unused1: 8;
	canbittype 	unused2: 8;
	canbittype 	unused3: 8;
	canbittype 	unused4: 8;
	canbittype 	unused5: 8;
} _c_BAP_Telefon_04_msgType;

typedef struct {
	canbittype 	TF3_Phone_Error_flag: 1;
	canbittype 	TF3_Phone_pin_OK: 1;
	canbittype 	TF3_Phone_sim_OK: 1;
	canbittype 	TF3_Phone_ring: 1;
	canbittype 	TF3_Phone_active_call: 1;
	canbittype 	TF3_Phone_mute: 1;
	canbittype 	TF3_Show_Phone: 1;
	canbittype 	TF3_Phone_on: 1;
	canbittype 	TF3_Phone_Init: 1;
	canbittype 	TF3_Phone_Display_Vol: 1;
	canbittype 	TF3_Phone_Level: 6;
	canbittype 	unused0: 1;
	canbittype 	TF3_BT_ConnectionState: 1;
	canbittype 	TF3_Radio_Access: 2;
	canbittype 	TF3_Wake_Up_Komfort: 1;
	canbittype 	TF3_Mute_Msg_Code: 3;
	canbittype 	TF3_Provider_0: 8;
	canbittype 	TF3_Feldstaerke: 4;
	canbittype 	TF3_Provider_1: 4;
	canbittype 	TF3_Laendercode_0: 8;
	canbittype 	TF3_Registrierungsstatus: 3;
	canbittype 	TF3_Feldstaerke_Fehler: 1;
	canbittype 	TF3_Laendercode_1: 4;
} _c_mTelefon_3_msgType;

typedef struct {
	canbittype 	BAP_Data_Telefon_3_0: 8;
	canbittype 	BAP_Data_Telefon_3_1: 8;
	canbittype 	unused0: 8;
	canbittype 	unused1: 8;
	canbittype 	unused2: 8;
	canbittype 	unused3: 8;
	canbittype 	unused4: 8;
	canbittype 	unused5: 8;
} _c_BAP_Telefon_03_msgType;

typedef struct {
	canbittype 	BAP_Data_MDI_02_0: 8;
	canbittype 	BAP_Data_MDI_02_1: 8;
	canbittype 	unused0: 8;
	canbittype 	unused1: 8;
	canbittype 	unused2: 8;
	canbittype 	unused3: 8;
	canbittype 	unused4: 8;
	canbittype 	unused5: 8;
} _c_BAP_MDI_02_msgType;

/* Receive messages */

typedef struct {
	canbittype 	unused0: 6;
	canbittype 	ASR7_Write_Success: 1;
	canbittype 	ASR7_Is_Read_Feedback: 1;
	canbittype 	ASR7_Param_ID_0: 8;
	canbittype 	ASR7_Param_ID_1: 8;
	canbittype 	ASR7_Param_GetValue_0: 8;
	canbittype 	ASR7_Param_GetValue_1: 8;
	canbittype 	ASR7_Param_GetValue_2: 8;
	canbittype 	ASR7_Param_GetValue_3: 8;
	canbittype 	ASR7_Param_GetValue_4: 8;
} _c_mASR7_Param_Feedback_msgType;

typedef struct {
	canbittype 	unused0: 6;
	canbittype 	ASR7_Write_Success: 1;
	canbittype 	ASR7_Is_Read_Feedback: 1;
	canbittype 	ASR7_Param_ID_0: 8;
	canbittype 	ASR7_Param_ID_1: 8;
	canbittype 	ASR7_Param_GetValue_0: 8;
	canbittype 	ASR7_Param_GetValue_1: 8;
	canbittype 	ASR7_Param_GetValue_2: 8;
	canbittype 	ASR7_Param_GetValue_3: 8;
	canbittype 	ASR7_Param_GetValue_4: 8;
} _c_mASR7_Param_Feedback_RDS_msgType;

typedef struct {
	canbittype 	unused0: 7;
	canbittype 	ASR6_Is_Read_Request: 1;
	canbittype 	ASR6_Param_ID_0: 8;
	canbittype 	ASR6_Param_ID_1: 8;
	canbittype 	ASR6_Param_SetValue_0: 8;
	canbittype 	ASR6_Param_SetValue_1: 8;
	canbittype 	ASR6_Param_SetValue_2: 8;
	canbittype 	ASR6_Param_SetValue_3: 8;
	canbittype 	ASR6_Param_SetValue_4: 8;
} _c_mASR6_Param_Request_msgType;

typedef struct {
	canbittype 	unused0: 7;
	canbittype 	ASR6_Is_Read_Request: 1;
	canbittype 	ASR6_Param_ID_0: 8;
	canbittype 	ASR6_Param_ID_1: 8;
	canbittype 	ASR6_Param_SetValue_0: 8;
	canbittype 	ASR6_Param_SetValue_1: 8;
	canbittype 	ASR6_Param_SetValue_2: 8;
	canbittype 	ASR6_Param_SetValue_3: 8;
	canbittype 	ASR6_Param_SetValue_4: 8;
} _c_mASR6_Param_Request_RDS_msgType;

typedef struct {
	canbittype 	BAP_Data_Debug_Cmd_0: 8;
	canbittype 	BAP_Data_Debug_Cmd_1: 8;
} _c_BAP_Debug_Cmd_msgType;

typedef struct {
	canbittype 	BAP_Data_Debug_Cmd_0: 8;
	canbittype 	BAP_Data_Debug_Cmd_1: 8;
} _c_BAP_Debug_Cmd_RDS_msgType;

typedef struct {
	canbittype 	BAP_Data_ASG_03_0: 8;
	canbittype 	BAP_Data_ASG_03_1: 8;
} _c_BAP_ASG_03_msgType;

typedef struct {
	canbittype 	BAP_Data_ASG_03_0: 8;
	canbittype 	BAP_Data_ASG_03_1: 8;
} _c_BAP_ASG_03_RDS_msgType;

typedef struct {
	canbittype 	BAP_Data_ASG_06_0: 8;
	canbittype 	BAP_Data_ASG_06_1: 8;
} _c_BAP_ASG_06_msgType;

typedef struct {
	canbittype 	BAP_Data_ASG_06_0: 8;
	canbittype 	BAP_Data_ASG_06_1: 8;
} _c_BAP_ASG_06_RDS_msgType;

typedef struct {
	canbittype 	BAP_Data_ASG_07_0: 8;
	canbittype 	BAP_Data_ASG_07_1: 8;
} _c_BAP_ASG_07_msgType;

typedef struct {
	canbittype 	BAP_Data_ASG_07_0: 8;
	canbittype 	BAP_Data_ASG_07_1: 8;
} _c_BAP_ASG_07_RDS_msgType;

typedef struct {
	canbittype 	unused1: 5;
	canbittype 	RA4_Radio_mute: 1;
	canbittype 	unused0: 1;
	canbittype 	RA4_Radio_on: 1;
} _c_mRadio_4_msgType;

typedef struct {
	canbittype 	unused1: 5;
	canbittype 	RA4_Radio_mute: 1;
	canbittype 	unused0: 1;
	canbittype 	RA4_Radio_on: 1;
} _c_mRadio_4_RDS_msgType;

typedef struct {
	canbittype 	DN1_Verlernzaehler: 8;
	canbittype 	DN1_KM_Stand_0: 8;
	canbittype 	DN1_KM_Stand_1: 8;
	canbittype 	DN1_Jahr_0: 4;
	canbittype 	DN1_KM_Stand_2: 4;
	canbittype 	DN1_Tag_0: 1;
	canbittype 	DN1_Monat: 4;
	canbittype 	DN1_Jahr_1: 3;
	canbittype 	DN1_Stunde_0: 4;
	canbittype 	DN1_Tag_1: 4;
	canbittype 	DN1_Sekunde_0: 1;
	canbittype 	DN1_Minute: 6;
	canbittype 	DN1_Stunde_1: 1;
	canbittype 	DN1_alt_Zeit: 1;
	canbittype 	DN1_alt_Kilometerstand: 1;
	canbittype 	unused0: 1;
	canbittype 	DN1_Sekunde_1: 5;
} _c_mDiagnose_1_msgType;

typedef struct {
	canbittype 	DN1_Verlernzaehler: 8;
	canbittype 	DN1_KM_Stand_0: 8;
	canbittype 	DN1_KM_Stand_1: 8;
	canbittype 	DN1_Jahr_0: 4;
	canbittype 	DN1_KM_Stand_2: 4;
	canbittype 	DN1_Tag_0: 1;
	canbittype 	DN1_Monat: 4;
	canbittype 	DN1_Jahr_1: 3;
	canbittype 	DN1_Stunde_0: 4;
	canbittype 	DN1_Tag_1: 4;
	canbittype 	DN1_Sekunde_0: 1;
	canbittype 	DN1_Minute: 6;
	canbittype 	DN1_Stunde_1: 1;
	canbittype 	DN1_alt_Zeit: 1;
	canbittype 	DN1_alt_Kilometerstand: 1;
	canbittype 	unused0: 1;
	canbittype 	DN1_Sekunde_1: 5;
} _c_mDiagnose_1_RDS_msgType;

typedef struct {
	canbittype 	unused0: 8;
	canbittype 	unused1: 8;
	canbittype 	VBN_Soll_Ist_OK: 1;
	canbittype 	unused2: 7;
	canbittype 	unused3: 8;
	canbittype 	unused5: 5;
	canbittype 	VBN_SMLS_K: 1;
	canbittype 	unused4: 2;
	canbittype 	unused6: 8;
	canbittype 	unused8: 4;
	canbittype 	VBN_Tastenmodul_I: 1;
	canbittype 	unused7: 3;
	canbittype 	VBN_Telefon_I: 1;
	canbittype 	VBN_Radio_I: 1;
	canbittype 	unused10: 2;
	canbittype 	VBN_Navigation_I: 1;
	canbittype 	VBN_Telematik_I: 1;
	canbittype 	unused9: 1;
	canbittype 	VBN_DSP_I: 1;
} _c_mSollverbau_neu_msgType;

typedef struct {
	canbittype 	unused0: 8;
	canbittype 	unused1: 8;
	canbittype 	VBN_Soll_Ist_OK: 1;
	canbittype 	unused2: 7;
	canbittype 	unused3: 8;
	canbittype 	unused5: 5;
	canbittype 	VBN_SMLS_K: 1;
	canbittype 	unused4: 2;
	canbittype 	unused6: 8;
	canbittype 	unused8: 4;
	canbittype 	VBN_Tastenmodul_I: 1;
	canbittype 	unused7: 3;
	canbittype 	VBN_Telefon_I: 1;
	canbittype 	VBN_Radio_I: 1;
	canbittype 	unused10: 2;
	canbittype 	VBN_Navigation_I: 1;
	canbittype 	VBN_Telematik_I: 1;
	canbittype 	unused9: 1;
	canbittype 	VBN_DSP_I: 1;
} _c_mSollverbau_neu_RDS_msgType;

typedef struct {
	canbittype 	GW3_Land_Sprach_empf: 1;
	canbittype 	GW3_Alt_3_Kombi: 1;
	canbittype 	GW3_Laendervariante: 6;
	canbittype 	GW3_Sprachvariante: 8;
} _c_mGateway_3_msgType;

typedef struct {
	canbittype 	GW3_Land_Sprach_empf: 1;
	canbittype 	GW3_Alt_3_Kombi: 1;
	canbittype 	GW3_Laendervariante: 6;
	canbittype 	GW3_Sprachvariante: 8;
} _c_mGateway_3_RDS_msgType;

typedef struct {
	canbittype 	SY1_Verbauliste_gueltig: 1;
	canbittype 	SY1_Infotainment: 1;
	canbittype 	SY1_Diag_Infotainment: 1;
	canbittype 	unused1: 2;
	canbittype 	SY1_Sleep_Komfort: 1;
	canbittype 	unused0: 2;
	canbittype 	SY1_Marke: 4;
	canbittype 	SY1_Klasse: 4;
	canbittype 	SY1_Generation: 4;
	canbittype 	SY1_Derivat: 4;
	canbittype 	unused3: 1;
	canbittype 	SY1_Transportmode: 1;
	canbittype 	unused2: 1;
	canbittype 	SY1_Rechtslenker: 1;
	canbittype 	SY1_Erweiterung: 4;
	canbittype 	unused4: 8;
	canbittype 	unused5: 8;
	canbittype 	SY1_NWDF: 1;
	canbittype 	SY1_NWDF_gueltig: 1;
	canbittype 	unused6: 6;
} _c_mSysteminfo_1_msgType;

typedef struct {
	canbittype 	SY1_Verbauliste_gueltig: 1;
	canbittype 	SY1_Infotainment: 1;
	canbittype 	SY1_Diag_Infotainment: 1;
	canbittype 	unused1: 2;
	canbittype 	SY1_Sleep_Komfort: 1;
	canbittype 	unused0: 2;
	canbittype 	SY1_Marke: 4;
	canbittype 	SY1_Klasse: 4;
	canbittype 	SY1_Generation: 4;
	canbittype 	SY1_Derivat: 4;
	canbittype 	unused3: 1;
	canbittype 	SY1_Transportmode: 1;
	canbittype 	unused2: 1;
	canbittype 	SY1_Rechtslenker: 1;
	canbittype 	SY1_Erweiterung: 4;
	canbittype 	unused4: 8;
	canbittype 	unused5: 8;
	canbittype 	SY1_NWDF: 1;
	canbittype 	SY1_NWDF_gueltig: 1;
	canbittype 	unused6: 6;
} _c_mSysteminfo_1_RDS_msgType;

typedef struct {
	canbittype 	MLT_Symbol: 8;
	canbittype 	unused0: 8;
	canbittype 	unused1: 4;
	canbittype 	MLT_Tickzaehler_1: 4;
	canbittype 	MLT_Variante: 4;
	canbittype 	MLT_Marke: 4;
} _c_mMFL_Tasten_msgType;

typedef struct {
	canbittype 	MLT_Symbol: 8;
	canbittype 	unused0: 8;
	canbittype 	unused1: 4;
	canbittype 	MLT_Tickzaehler_1: 4;
	canbittype 	MLT_Variante: 4;
	canbittype 	MLT_Marke: 4;
} _c_mMFL_Tasten_RDS_msgType;

typedef struct {
	canbittype 	MFL_Tastencode_1: 8;
	canbittype 	MFL_Tastencode_2: 8;
	canbittype 	MFL_Eventcode_2: 4;
	canbittype 	MFL_Eventcode_1: 4;
	canbittype 	MFL_Variante: 4;
	canbittype 	MFL_Marke: 4;
} _c_MFL_Tasten_Kon_01_msgType;

typedef struct {
	canbittype 	MFL_Tastencode_1: 8;
	canbittype 	MFL_Tastencode_2: 8;
	canbittype 	MFL_Eventcode_2: 4;
	canbittype 	MFL_Eventcode_1: 4;
	canbittype 	MFL_Variante: 4;
	canbittype 	MFL_Marke: 4;
} _c_MFL_Tasten_Kon_01_RDS_msgType;

typedef struct {
	canbittype 	unused0: 6;
	canbittype 	BS3_Klemme_15: 1;
	canbittype 	BS3_Klemme_S: 1;
} _c_mBSG_3_msgType;

typedef struct {
	canbittype 	unused0: 6;
	canbittype 	BS3_Klemme_15: 1;
	canbittype 	BS3_Klemme_S: 1;
} _c_mBSG_3_RDS_msgType;

typedef struct {
	canbittype 	unused0: 8;
	canbittype 	unused1: 8;
	canbittype 	unused2: 8;
	canbittype 	unused4: 1;
	canbittype 	BS2_aus_Infotainment: 1;
	canbittype 	unused3: 6;
	canbittype 	unused5: 8;
	canbittype 	unused7: 6;
	canbittype 	BS2_Warn_Infotainment: 1;
	canbittype 	unused6: 1;
} _c_mBSG_2_msgType;

typedef struct {
	canbittype 	unused0: 8;
	canbittype 	unused1: 8;
	canbittype 	unused2: 8;
	canbittype 	unused4: 1;
	canbittype 	BS2_aus_Infotainment: 1;
	canbittype 	unused3: 6;
	canbittype 	unused5: 8;
	canbittype 	unused7: 6;
	canbittype 	BS2_Warn_Infotainment: 1;
	canbittype 	unused6: 1;
} _c_mBSG_2_RDS_msgType;

typedef struct {
	canbittype 	unused1: 5;
	canbittype 	GWK_Alt_1_Kombi: 1;
	canbittype 	unused0: 2;
	canbittype 	GWK_FzgGeschw_0: 7;
	canbittype 	GWK_FzgGeschw_Quelle: 1;
	canbittype 	GWK_FzgGeschw_1: 8;
} _c_mGW_Kombi_msgType;

typedef struct {
	canbittype 	unused1: 5;
	canbittype 	GWK_Alt_1_Kombi: 1;
	canbittype 	unused0: 2;
	canbittype 	GWK_FzgGeschw_0: 7;
	canbittype 	GWK_FzgGeschw_Quelle: 1;
	canbittype 	GWK_FzgGeschw_1: 8;
} _c_mGW_Kombi_RDS_msgType;

typedef struct {
	canbittype 	unused0: 8;
	canbittype 	unused1: 8;
	canbittype 	unused2: 8;
	canbittype 	unused3: 8;
	canbittype 	unused4: 8;
	canbittype 	unused5: 8;
	canbittype 	BCM_Remotestart_Betrieb: 1;
	canbittype 	unused6: 7;
} _c_mBSG_Kombi_msgType;

typedef struct {
	canbittype 	unused0: 8;
	canbittype 	unused1: 8;
	canbittype 	unused2: 8;
	canbittype 	unused3: 8;
	canbittype 	unused4: 8;
	canbittype 	unused5: 8;
	canbittype 	BCM_Remotestart_Betrieb: 1;
	canbittype 	unused6: 7;
} _c_mBSG_Kombi_RDS_msgType;

typedef struct {
	canbittype 	unused0: 8;
	canbittype 	unused1: 8;
	canbittype 	unused2: 8;
	canbittype 	unused3: 8;
	canbittype 	unused6: 2;
	canbittype 	LS1_MFL_Typ: 1;
	canbittype 	unused5: 1;
	canbittype 	LS1_MFA_Tasten: 1;
	canbittype 	LS1_MFA_vorhanden: 1;
	canbittype 	LS1_MFL_vorhanden: 1;
	canbittype 	unused4: 1;
} _c_mLSM_1_msgType;

typedef struct {
	canbittype 	unused0: 8;
	canbittype 	unused1: 8;
	canbittype 	unused2: 8;
	canbittype 	unused3: 8;
	canbittype 	unused6: 2;
	canbittype 	LS1_MFL_Typ: 1;
	canbittype 	unused5: 1;
	canbittype 	LS1_MFA_Tasten: 1;
	canbittype 	LS1_MFA_vorhanden: 1;
	canbittype 	LS1_MFL_vorhanden: 1;
	canbittype 	unused4: 1;
} _c_mLSM_1_RDS_msgType;

typedef struct {
	canbittype 	AB1_CrashStaerke: 3;
	canbittype 	AB1_Rollover: 1;
	canbittype 	AB1_Crash_BT: 1;
	canbittype 	AB1_Crash_FT: 1;
	canbittype 	AB1_HeckCrash: 1;
	canbittype 	AB1_FrontCrash: 1;
} _c_mAirbag_1_msgType;

typedef struct {
	canbittype 	AB1_CrashStaerke: 3;
	canbittype 	AB1_Rollover: 1;
	canbittype 	AB1_Crash_BT: 1;
	canbittype 	AB1_Crash_FT: 1;
	canbittype 	AB1_HeckCrash: 1;
	canbittype 	AB1_FrontCrash: 1;
} _c_mAirbag_1_RDS_msgType;

/*************************************************************/
/* Message unions              */
/*************************************************************/

/* Send messages */

typedef union {
	canuint8 _c[8];
	_c_ISO_Telefon_Resp_msgType	iso_telefon_resp;
} _c_ISO_Telefon_Resp_buf;

typedef union {
	canuint8 _c[8];
	_c_mASR5_String3_msgType	masr5_string3;
} _c_mASR5_String3_buf;

typedef union {
	canuint8 _c[8];
	_c_mASR4_String2_msgType	masr4_string2;
} _c_mASR4_String2_buf;

typedef union {
	canuint8 _c[8];
	_c_mASR3_String1_msgType	masr3_string1;
} _c_mASR3_String1_buf;

typedef union {
	canuint8 _c[8];
	_c_mASR2_String0_msgType	masr2_string0;
} _c_mASR2_String0_buf;

typedef union {
	canuint8 _c[8];
	_c_mASR1_TypeInfo_msgType	masr1_typeinfo;
} _c_mASR1_TypeInfo_buf;

typedef union {
	canuint8 _c[4];
	_c_mASR0_Status_msgType	masr0_status;
} _c_mASR0_Status_buf;

typedef union {
	canuint8 _c[7];
	_c_BAP_Debug_Data_msgType	bap_debug_data;
} _c_BAP_Debug_Data_buf;

typedef union {
	canuint8 _c[8];
	_c_BAP_Telefon_02_msgType	bap_telefon_02;
} _c_BAP_Telefon_02_buf;

typedef union {
	canuint8 _c[8];
	_c_BAP_Telefon_01_msgType	bap_telefon_01;
} _c_BAP_Telefon_01_buf;

typedef union {
	canuint8 _c[8];
	_c_BAP_Telefon_04_msgType	bap_telefon_04;
} _c_BAP_Telefon_04_buf;

typedef union {
	canuint8 _c[7];
	_c_mTelefon_3_msgType	mtelefon_3;
} _c_mTelefon_3_buf;

typedef union {
	canuint8 _c[8];
	_c_BAP_Telefon_03_msgType	bap_telefon_03;
} _c_BAP_Telefon_03_buf;

typedef union {
	canuint8 _c[8];
	_c_BAP_MDI_02_msgType	bap_mdi_02;
} _c_BAP_MDI_02_buf;

/* Receive messages */

typedef union {
	canuint8 _c[8];
	_c_mASR7_Param_Feedback_msgType	masr7_param_feedback;
} _c_mASR7_Param_Feedback_buf;

typedef union {
	canuint8 _c[8];
	_c_mASR6_Param_Request_msgType	masr6_param_request;
} _c_mASR6_Param_Request_buf;

typedef union {
	canuint8 _c[2];
	_c_BAP_Debug_Cmd_msgType	bap_debug_cmd;
} _c_BAP_Debug_Cmd_buf;

typedef union {
	canuint8 _c[2];
	_c_BAP_ASG_03_msgType	bap_asg_03;
} _c_BAP_ASG_03_buf;

typedef union {
	canuint8 _c[2];
	_c_BAP_ASG_06_msgType	bap_asg_06;
} _c_BAP_ASG_06_buf;

typedef union {
	canuint8 _c[2];
	_c_BAP_ASG_07_msgType	bap_asg_07;
} _c_BAP_ASG_07_buf;

typedef union {
	canuint8 _c[1];
	_c_mRadio_4_msgType	mradio_4;
} _c_mRadio_4_buf;

typedef union {
	canuint8 _c[8];
	_c_mDiagnose_1_msgType	mdiagnose_1;
} _c_mDiagnose_1_buf;

typedef union {
	canuint8 _c[8];
	_c_mSollverbau_neu_msgType	msollverbau_neu;
} _c_mSollverbau_neu_buf;

typedef union {
	canuint8 _c[2];
	_c_mGateway_3_msgType	mgateway_3;
} _c_mGateway_3_buf;

typedef union {
	canuint8 _c[7];
	_c_mSysteminfo_1_msgType	msysteminfo_1;
} _c_mSysteminfo_1_buf;

typedef union {
	canuint8 _c[4];
	_c_mMFL_Tasten_msgType	mmfl_tasten;
} _c_mMFL_Tasten_buf;

typedef union {
	canuint8 _c[4];
	_c_MFL_Tasten_Kon_01_msgType	mfl_tasten_kon_01;
} _c_MFL_Tasten_Kon_01_buf;

typedef union {
	canuint8 _c[1];
	_c_mBSG_3_msgType	mbsg_3;
} _c_mBSG_3_buf;

typedef union {
	canuint8 _c[6];
	_c_mBSG_2_msgType	mbsg_2;
} _c_mBSG_2_buf;

typedef union {
	canuint8 _c[3];
	_c_mGW_Kombi_msgType	mgw_kombi;
} _c_mGW_Kombi_buf;

typedef union {
	canuint8 _c[7];
	_c_mBSG_Kombi_msgType	mbsg_kombi;
} _c_mBSG_Kombi_buf;

typedef union {
	canuint8 _c[5];
	_c_mLSM_1_msgType	mlsm_1;
} _c_mLSM_1_buf;

typedef union {
	canuint8 _c[1];
	_c_mAirbag_1_msgType	mairbag_1;
} _c_mAirbag_1_buf;

/* CAN buffer */

typedef union {
	canuint8 _c[8];
}_c_RDSTx_buf;

typedef union {
	canuint8 _c[8];
	/*Tx ID: 0x43a*/
}_c_RDS31_buf;

typedef union {
	canuint8 _c[8];
}_c_RDSBasic_buf;

typedef union {
	canuint8 _c[8];
	/*Rx ID: 0x7a7*/
	_c_mASR7_Param_Feedback_RDS_msgType	masr7_param_feedback;
}_c_RDS2_buf;

typedef union {
	canuint8 _c[8];
	/*Rx ID: 0x7a6*/
	_c_mASR6_Param_Request_RDS_msgType	masr6_param_request;
}_c_RDS3_buf;

typedef union {
	canuint8 _c[8];
	/*Rx ID: 0x76b*/
}_c_RDS4_buf;

typedef union {
	canuint8 _c[8];
	/*Rx ID: 0x700*/
}_c_RDS5_buf;

typedef union {
	canuint8 _c[8];
	/*Rx ID: 0x6d6*/
	_c_BAP_Debug_Cmd_RDS_msgType	bap_debug_cmd;
}_c_RDS6_buf;

typedef union {
	canuint8 _c[8];
	/*Rx ID: 0x6c7*/
	_c_BAP_ASG_03_RDS_msgType	bap_asg_03;
}_c_RDS7_buf;

typedef union {
	canuint8 _c[8];
	/*Rx ID: 0x6c6*/
	_c_BAP_ASG_06_RDS_msgType	bap_asg_06;
}_c_RDS8_buf;

typedef union {
	canuint8 _c[8];
	/*Rx ID: 0x67c*/
	_c_BAP_ASG_07_RDS_msgType	bap_asg_07;
}_c_RDS9_buf;

typedef union {
	canuint8 _c[8];
	/*Rx ID: 0x661*/
	_c_mRadio_4_RDS_msgType	mradio_4;
}_c_RDS10_buf;

typedef union {
	canuint8 _c[8];
	/*Rx ID: 0x65d*/
	_c_mDiagnose_1_RDS_msgType	mdiagnose_1;
}_c_RDS11_buf;

typedef union {
	canuint8 _c[8];
	/*Rx ID: 0x655*/
	_c_mSollverbau_neu_RDS_msgType	msollverbau_neu;
}_c_RDS12_buf;

typedef union {
	canuint8 _c[8];
	/*Rx ID: 0x653*/
	_c_mGateway_3_RDS_msgType	mgateway_3;
}_c_RDS13_buf;

typedef union {
	canuint8 _c[8];
	/*Rx ID: 0x651*/
	_c_mSysteminfo_1_RDS_msgType	msysteminfo_1;
}_c_RDS14_buf;

typedef union {
	canuint8 _c[8];
	/*Rx ID: 0x5c1*/
	_c_mMFL_Tasten_RDS_msgType	mmfl_tasten;
}_c_RDS15_buf;

typedef union {
	canuint8 _c[8];
	/*Rx ID: 0x5bf*/
	_c_MFL_Tasten_Kon_01_RDS_msgType	mfl_tasten_kon_01;
}_c_RDS16_buf;

typedef union {
	canuint8 _c[8];
	/*Rx ID: 0x575*/
	_c_mBSG_3_RDS_msgType	mbsg_3;
}_c_RDS17_buf;

typedef union {
	canuint8 _c[8];
	/*Rx ID: 0x571*/
	_c_mBSG_2_RDS_msgType	mbsg_2;
}_c_RDS18_buf;

typedef union {
	canuint8 _c[8];
	/*Rx ID: 0x527*/
	_c_mGW_Kombi_RDS_msgType	mgw_kombi;
}_c_RDS19_buf;

typedef union {
	canuint8 _c[8];
	/*Rx ID: 0x470*/
	_c_mBSG_Kombi_RDS_msgType	mbsg_kombi;
}_c_RDS20_buf;

typedef union {
	canuint8 _c[8];
	/*Rx ID: 0x2c1*/
	_c_mLSM_1_RDS_msgType	mlsm_1;
}_c_RDS21_buf;

typedef union {
	canuint8 _c[8];
	/*Rx ID: 0x151*/
	_c_mAirbag_1_RDS_msgType	mairbag_1;
}_c_RDS22_buf;

/*************************************************************/
/* Signal groups                                             */
/*************************************************************/

/*************************************************************/
/* Message masks (receive messages)                      */
/*************************************************************/


#define c1_mASR7_Param_Feedback_Mask_c 0x3
#define c2_mASR7_Param_Feedback_Mask_c 0xff
#define c3_mASR7_Param_Feedback_Mask_c 0xff
#define c4_mASR7_Param_Feedback_Mask_c 0xff
#define c5_mASR7_Param_Feedback_Mask_c 0xff
#define c6_mASR7_Param_Feedback_Mask_c 0xff
#define c7_mASR7_Param_Feedback_Mask_c 0xff
#define c8_mASR7_Param_Feedback_Mask_c 0xff

#define c1_mASR6_Param_Request_Mask_c 0x1
#define c2_mASR6_Param_Request_Mask_c 0xff
#define c3_mASR6_Param_Request_Mask_c 0xff
#define c4_mASR6_Param_Request_Mask_c 0xff
#define c5_mASR6_Param_Request_Mask_c 0xff
#define c6_mASR6_Param_Request_Mask_c 0xff
#define c7_mASR6_Param_Request_Mask_c 0xff
#define c8_mASR6_Param_Request_Mask_c 0xff

#define c1_mRadio_4_Mask_c 0x5

#define c1_mDiagnose_1_Mask_c 0xff
#define c2_mDiagnose_1_Mask_c 0xff
#define c3_mDiagnose_1_Mask_c 0xff
#define c4_mDiagnose_1_Mask_c 0xff
#define c5_mDiagnose_1_Mask_c 0xff
#define c6_mDiagnose_1_Mask_c 0xff
#define c7_mDiagnose_1_Mask_c 0xff
#define c8_mDiagnose_1_Mask_c 0xdf

#define c1_mSollverbau_neu_Mask_c 0x0
#define c2_mSollverbau_neu_Mask_c 0x0
#define c3_mSollverbau_neu_Mask_c 0x80
#define c4_mSollverbau_neu_Mask_c 0x0
#define c5_mSollverbau_neu_Mask_c 0x4
#define c6_mSollverbau_neu_Mask_c 0x0
#define c7_mSollverbau_neu_Mask_c 0x8
#define c8_mSollverbau_neu_Mask_c 0xcd

#define c1_mGateway_3_Mask_c 0xff
#define c2_mGateway_3_Mask_c 0xff

#define c1_mSysteminfo_1_Mask_c 0xe4
#define c2_mSysteminfo_1_Mask_c 0xff
#define c3_mSysteminfo_1_Mask_c 0xff
#define c4_mSysteminfo_1_Mask_c 0x5f
#define c5_mSysteminfo_1_Mask_c 0x0
#define c6_mSysteminfo_1_Mask_c 0x0
#define c7_mSysteminfo_1_Mask_c 0xc0

#define c1_mMFL_Tasten_Mask_c 0xff
#define c2_mMFL_Tasten_Mask_c 0x0
#define c3_mMFL_Tasten_Mask_c 0xf
#define c4_mMFL_Tasten_Mask_c 0xff

#define c1_MFL_Tasten_Kon_01_Mask_c 0xff
#define c2_MFL_Tasten_Kon_01_Mask_c 0xff
#define c3_MFL_Tasten_Kon_01_Mask_c 0xff
#define c4_MFL_Tasten_Kon_01_Mask_c 0xff

#define c1_mBSG_3_Mask_c 0x3

#define c1_mBSG_2_Mask_c 0x0
#define c2_mBSG_2_Mask_c 0x0
#define c3_mBSG_2_Mask_c 0x0
#define c4_mBSG_2_Mask_c 0x40
#define c5_mBSG_2_Mask_c 0x0
#define c6_mBSG_2_Mask_c 0x2

#define c1_mGW_Kombi_Mask_c 0x4
#define c2_mGW_Kombi_Mask_c 0xff
#define c3_mGW_Kombi_Mask_c 0xff

#define c1_mBSG_Kombi_Mask_c 0x0
#define c2_mBSG_Kombi_Mask_c 0x0
#define c3_mBSG_Kombi_Mask_c 0x0
#define c4_mBSG_Kombi_Mask_c 0x0
#define c5_mBSG_Kombi_Mask_c 0x0
#define c6_mBSG_Kombi_Mask_c 0x0
#define c7_mBSG_Kombi_Mask_c 0x80

#define c1_mLSM_1_Mask_c 0x0
#define c2_mLSM_1_Mask_c 0x0
#define c3_mLSM_1_Mask_c 0x0
#define c4_mLSM_1_Mask_c 0x0
#define c5_mLSM_1_Mask_c 0x2e

#define c1_mAirbag_1_Mask_c 0xff

/* Pretransmit functions */

/* Confirmation functions */
extern void I2V_vCanDataCon(CanTransmitHandle txObject);
extern void NmConfirmation(CanTransmitHandle txObject);


/* Precopy functions */
extern canuint8 I2V_ubCanDataPrecopy(CanRxInfoStructPtr preParam);
extern canuint8 BAP_BCL_CanRxPreCopy(CanRxInfoStructPtr preParam);


/* Indication functions */
extern void App_BapDiagnoseSwitch(CanReceiveHandle rxObject);

/* Hook function prototypes */

#define RDS31 (*((_c_RDS31_buf MEMORY_CAN *)(canRDSTxPtr[1])))
#define RDSTx (*((_c_RDSTx_buf MEMORY_CAN *)(canRDSTxPtr[0])))
#define RDS2 (*((_c_RDS2_buf MEMORY_CAN *)(canRDSRxPtr[0])))
#define RDS3 (*((_c_RDS3_buf MEMORY_CAN *)(canRDSRxPtr[0])))
#define RDS4 (*((_c_RDS4_buf MEMORY_CAN *)(canRDSRxPtr[0])))
#define RDS5 (*((_c_RDS5_buf MEMORY_CAN *)(canRDSRxPtr[0])))
#define RDS6 (*((_c_RDS6_buf MEMORY_CAN *)(canRDSRxPtr[0])))
#define RDS7 (*((_c_RDS7_buf MEMORY_CAN *)(canRDSRxPtr[0])))
#define RDS8 (*((_c_RDS8_buf MEMORY_CAN *)(canRDSRxPtr[0])))
#define RDS9 (*((_c_RDS9_buf MEMORY_CAN *)(canRDSRxPtr[0])))
#define RDS10 (*((_c_RDS10_buf MEMORY_CAN *)(canRDSRxPtr[0])))
#define RDS11 (*((_c_RDS11_buf MEMORY_CAN *)(canRDSRxPtr[0])))
#define RDS12 (*((_c_RDS12_buf MEMORY_CAN *)(canRDSRxPtr[0])))
#define RDS13 (*((_c_RDS13_buf MEMORY_CAN *)(canRDSRxPtr[0])))
#define RDS14 (*((_c_RDS14_buf MEMORY_CAN *)(canRDSRxPtr[0])))
#define RDS15 (*((_c_RDS15_buf MEMORY_CAN *)(canRDSRxPtr[0])))
#define RDS16 (*((_c_RDS16_buf MEMORY_CAN *)(canRDSRxPtr[0])))
#define RDS17 (*((_c_RDS17_buf MEMORY_CAN *)(canRDSRxPtr[0])))
#define RDS18 (*((_c_RDS18_buf MEMORY_CAN *)(canRDSRxPtr[0])))
#define RDS19 (*((_c_RDS19_buf MEMORY_CAN *)(canRDSRxPtr[0])))
#define RDS20 (*((_c_RDS20_buf MEMORY_CAN *)(canRDSRxPtr[0])))
#define RDS21 (*((_c_RDS21_buf MEMORY_CAN *)(canRDSRxPtr[0])))
#define RDS22 (*((_c_RDS22_buf MEMORY_CAN *)(canRDSRxPtr[0])))
#define RDSBasic (*((_c_RDSBasic_buf MEMORY_CAN *)(canRDSRxPtr[0])))

/*************************************************************/
/* Databuffer for send objects                               */
/*************************************************************/
extern _c_ISO_Telefon_Resp_buf MEMORY_FAR  ISO_Telefon_Resp;
extern _c_mASR5_String3_buf MEMORY_FAR  mASR5_String3;
extern _c_mASR4_String2_buf MEMORY_FAR  mASR4_String2;
extern _c_mASR3_String1_buf MEMORY_FAR  mASR3_String1;
extern _c_mASR2_String0_buf MEMORY_FAR  mASR2_String0;
extern _c_mASR1_TypeInfo_buf MEMORY_FAR  mASR1_TypeInfo;
extern _c_mASR0_Status_buf MEMORY_FAR  mASR0_Status;
extern _c_BAP_Debug_Data_buf MEMORY_FAR  BAP_Debug_Data;
extern _c_BAP_Telefon_02_buf MEMORY_FAR  BAP_Telefon_02;
extern _c_BAP_Telefon_01_buf MEMORY_FAR  BAP_Telefon_01;
extern _c_BAP_Telefon_04_buf MEMORY_FAR  BAP_Telefon_04;
extern _c_mTelefon_3_buf MEMORY_FAR  mTelefon_3;
extern _c_BAP_Telefon_03_buf MEMORY_FAR  BAP_Telefon_03;
extern _c_BAP_MDI_02_buf MEMORY_FAR  BAP_MDI_02;

/*************************************************************/
/* Byte access to databuffer of send objects                 */
/*************************************************************/
#define c1_mASR5_String3_c		mASR5_String3._c[0]
#define c2_mASR5_String3_c		mASR5_String3._c[1]
#define c3_mASR5_String3_c		mASR5_String3._c[2]
#define c4_mASR5_String3_c		mASR5_String3._c[3]
#define c5_mASR5_String3_c		mASR5_String3._c[4]
#define c6_mASR5_String3_c		mASR5_String3._c[5]
#define c7_mASR5_String3_c		mASR5_String3._c[6]
#define c8_mASR5_String3_c		mASR5_String3._c[7]

#define c1_mASR4_String2_c		mASR4_String2._c[0]
#define c2_mASR4_String2_c		mASR4_String2._c[1]
#define c3_mASR4_String2_c		mASR4_String2._c[2]
#define c4_mASR4_String2_c		mASR4_String2._c[3]
#define c5_mASR4_String2_c		mASR4_String2._c[4]
#define c6_mASR4_String2_c		mASR4_String2._c[5]
#define c7_mASR4_String2_c		mASR4_String2._c[6]
#define c8_mASR4_String2_c		mASR4_String2._c[7]

#define c1_mASR3_String1_c		mASR3_String1._c[0]
#define c2_mASR3_String1_c		mASR3_String1._c[1]
#define c3_mASR3_String1_c		mASR3_String1._c[2]
#define c4_mASR3_String1_c		mASR3_String1._c[3]
#define c5_mASR3_String1_c		mASR3_String1._c[4]
#define c6_mASR3_String1_c		mASR3_String1._c[5]
#define c7_mASR3_String1_c		mASR3_String1._c[6]
#define c8_mASR3_String1_c		mASR3_String1._c[7]

#define c1_mASR2_String0_c		mASR2_String0._c[0]
#define c2_mASR2_String0_c		mASR2_String0._c[1]
#define c3_mASR2_String0_c		mASR2_String0._c[2]
#define c4_mASR2_String0_c		mASR2_String0._c[3]
#define c5_mASR2_String0_c		mASR2_String0._c[4]
#define c6_mASR2_String0_c		mASR2_String0._c[5]
#define c7_mASR2_String0_c		mASR2_String0._c[6]
#define c8_mASR2_String0_c		mASR2_String0._c[7]

#define c1_mASR1_TypeInfo_c		mASR1_TypeInfo._c[0]
#define c2_mASR1_TypeInfo_c		mASR1_TypeInfo._c[1]
#define c3_mASR1_TypeInfo_c		mASR1_TypeInfo._c[2]
#define c4_mASR1_TypeInfo_c		mASR1_TypeInfo._c[3]
#define c5_mASR1_TypeInfo_c		mASR1_TypeInfo._c[4]
#define c6_mASR1_TypeInfo_c		mASR1_TypeInfo._c[5]
#define c7_mASR1_TypeInfo_c		mASR1_TypeInfo._c[6]
#define c8_mASR1_TypeInfo_c		mASR1_TypeInfo._c[7]

#define c1_mASR0_Status_c		mASR0_Status._c[0]
#define c2_mASR0_Status_c		mASR0_Status._c[1]
#define c3_mASR0_Status_c		mASR0_Status._c[2]
#define c4_mASR0_Status_c		mASR0_Status._c[3]

#define c1_mTelefon_3_c		mTelefon_3._c[0]
#define c2_mTelefon_3_c		mTelefon_3._c[1]
#define c3_mTelefon_3_c		mTelefon_3._c[2]
#define c4_mTelefon_3_c		mTelefon_3._c[3]
#define c5_mTelefon_3_c		mTelefon_3._c[4]
#define c6_mTelefon_3_c		mTelefon_3._c[5]
#define c7_mTelefon_3_c		mTelefon_3._c[6]


/*************************************************************/
/* Handles of send objects                                   */
/*************************************************************/

#define CanTxISO_Telefon_Resp	0
#define CanTxmASR5_String3	1
#define CanTxmASR4_String2	2
#define CanTxmASR3_String1	3
#define CanTxmASR2_String0	4
#define CanTxmASR1_TypeInfo	5
#define CanTxmASR0_Status	6
#define CanTxBAP_Debug_Data	7
#define CanTxBAP_Telefon_02	8
#define CanTxBAP_Telefon_01	9
#define CanTxBAP_Telefon_04	10
#define CanTxmTelefon_3	11
#define CanTxBAP_Telefon_03	12
#define CanTxBAP_MDI_02	13
#define CanTxNWM_Telefon	14

#if ! defined (CanStartFlagWriteSync)
	#error "Incompatible CAN driver - CanStartFlagWriteSync is missing."
#endif
#if ! defined (CanEndFlagWriteSync)
	#error "Incompatible CAN driver - CanEndFlagWriteSync is missing."
#endif

/*************************************************************/
/* Databuffer for confirmationflags                          */
/*************************************************************/

#ifdef C_ENABLE_CONFIRMATION_FLAG

#define mASR5_String3_conf_b 	CanConfirmationFlags.w[0].b0
#define CanWriteSyncmASR5_String3_conf_b(x)\
		CanStartFlagWriteSync();\
		mASR5_String3_conf_b = x;\
		CanEndFlagWriteSync();

#define mASR4_String2_conf_b 	CanConfirmationFlags.w[0].b1
#define CanWriteSyncmASR4_String2_conf_b(x)\
		CanStartFlagWriteSync();\
		mASR4_String2_conf_b = x;\
		CanEndFlagWriteSync();

#define mASR3_String1_conf_b 	CanConfirmationFlags.w[0].b2
#define CanWriteSyncmASR3_String1_conf_b(x)\
		CanStartFlagWriteSync();\
		mASR3_String1_conf_b = x;\
		CanEndFlagWriteSync();

#define mASR2_String0_conf_b 	CanConfirmationFlags.w[0].b3
#define CanWriteSyncmASR2_String0_conf_b(x)\
		CanStartFlagWriteSync();\
		mASR2_String0_conf_b = x;\
		CanEndFlagWriteSync();

#define mASR1_TypeInfo_conf_b 	CanConfirmationFlags.w[0].b4
#define CanWriteSyncmASR1_TypeInfo_conf_b(x)\
		CanStartFlagWriteSync();\
		mASR1_TypeInfo_conf_b = x;\
		CanEndFlagWriteSync();

#define mASR0_Status_conf_b 	CanConfirmationFlags.w[0].b5
#define CanWriteSyncmASR0_Status_conf_b(x)\
		CanStartFlagWriteSync();\
		mASR0_Status_conf_b = x;\
		CanEndFlagWriteSync();

#define BAP_Debug_Data_conf_b 	CanConfirmationFlags.w[0].b6
#define CanWriteSyncBAP_Debug_Data_conf_b(x)\
		CanStartFlagWriteSync();\
		BAP_Debug_Data_conf_b = x;\
		CanEndFlagWriteSync();

#define BAP_Telefon_02_conf_b 	CanConfirmationFlags.w[0].b7
#define CanWriteSyncBAP_Telefon_02_conf_b(x)\
		CanStartFlagWriteSync();\
		BAP_Telefon_02_conf_b = x;\
		CanEndFlagWriteSync();

#define BAP_Telefon_01_conf_b 	CanConfirmationFlags.w[0].b10
#define CanWriteSyncBAP_Telefon_01_conf_b(x)\
		CanStartFlagWriteSync();\
		BAP_Telefon_01_conf_b = x;\
		CanEndFlagWriteSync();

#define BAP_Telefon_04_conf_b 	CanConfirmationFlags.w[0].b11
#define CanWriteSyncBAP_Telefon_04_conf_b(x)\
		CanStartFlagWriteSync();\
		BAP_Telefon_04_conf_b = x;\
		CanEndFlagWriteSync();

#define mTelefon_3_conf_b 	CanConfirmationFlags.w[0].b12
#define CanWriteSyncmTelefon_3_conf_b(x)\
		CanStartFlagWriteSync();\
		mTelefon_3_conf_b = x;\
		CanEndFlagWriteSync();

#define BAP_Telefon_03_conf_b 	CanConfirmationFlags.w[0].b13
#define CanWriteSyncBAP_Telefon_03_conf_b(x)\
		CanStartFlagWriteSync();\
		BAP_Telefon_03_conf_b = x;\
		CanEndFlagWriteSync();

#define BAP_MDI_02_conf_b 	CanConfirmationFlags.w[0].b14
#define CanWriteSyncBAP_MDI_02_conf_b(x)\
		CanStartFlagWriteSync();\
		BAP_MDI_02_conf_b = x;\
		CanEndFlagWriteSync();

#endif


/*************************************************************/
/* Databuffer for receive objects                            */
/*************************************************************/
extern _c_mASR7_Param_Feedback_buf MEMORY_FAR  mASR7_Param_Feedback;
extern _c_mASR6_Param_Request_buf MEMORY_FAR  mASR6_Param_Request;
extern _c_BAP_Debug_Cmd_buf MEMORY_FAR  BAP_Debug_Cmd;
extern _c_BAP_ASG_03_buf MEMORY_FAR  BAP_ASG_03;
extern _c_BAP_ASG_06_buf MEMORY_FAR  BAP_ASG_06;
extern _c_BAP_ASG_07_buf MEMORY_FAR  BAP_ASG_07;
extern _c_mRadio_4_buf MEMORY_FAR  mRadio_4;
extern _c_mDiagnose_1_buf MEMORY_FAR  mDiagnose_1;
extern _c_mSollverbau_neu_buf MEMORY_FAR  mSollverbau_neu;
extern _c_mGateway_3_buf MEMORY_FAR  mGateway_3;
extern _c_mSysteminfo_1_buf MEMORY_FAR  mSysteminfo_1;
extern _c_mMFL_Tasten_buf MEMORY_FAR  mMFL_Tasten;
extern _c_MFL_Tasten_Kon_01_buf MEMORY_FAR  MFL_Tasten_Kon_01;
extern _c_mBSG_3_buf MEMORY_FAR  mBSG_3;
extern _c_mBSG_2_buf MEMORY_FAR  mBSG_2;
extern _c_mGW_Kombi_buf MEMORY_FAR  mGW_Kombi;
extern _c_mBSG_Kombi_buf MEMORY_FAR  mBSG_Kombi;
extern _c_mLSM_1_buf MEMORY_FAR  mLSM_1;
extern _c_mAirbag_1_buf MEMORY_FAR  mAirbag_1;

/*************************************************************/
/* Byte access to databuffer of receive objects              */
/*************************************************************/
#define c1_mASR7_Param_Feedback_c		mASR7_Param_Feedback._c[0]
#define c2_mASR7_Param_Feedback_c		mASR7_Param_Feedback._c[1]
#define c3_mASR7_Param_Feedback_c		mASR7_Param_Feedback._c[2]
#define c4_mASR7_Param_Feedback_c		mASR7_Param_Feedback._c[3]
#define c5_mASR7_Param_Feedback_c		mASR7_Param_Feedback._c[4]
#define c6_mASR7_Param_Feedback_c		mASR7_Param_Feedback._c[5]
#define c7_mASR7_Param_Feedback_c		mASR7_Param_Feedback._c[6]
#define c8_mASR7_Param_Feedback_c		mASR7_Param_Feedback._c[7]

#define c1_mASR6_Param_Request_c		mASR6_Param_Request._c[0]
#define c2_mASR6_Param_Request_c		mASR6_Param_Request._c[1]
#define c3_mASR6_Param_Request_c		mASR6_Param_Request._c[2]
#define c4_mASR6_Param_Request_c		mASR6_Param_Request._c[3]
#define c5_mASR6_Param_Request_c		mASR6_Param_Request._c[4]
#define c6_mASR6_Param_Request_c		mASR6_Param_Request._c[5]
#define c7_mASR6_Param_Request_c		mASR6_Param_Request._c[6]
#define c8_mASR6_Param_Request_c		mASR6_Param_Request._c[7]

#define c1_mRadio_4_c		mRadio_4._c[0]

#define c1_mDiagnose_1_c		mDiagnose_1._c[0]
#define c2_mDiagnose_1_c		mDiagnose_1._c[1]
#define c3_mDiagnose_1_c		mDiagnose_1._c[2]
#define c4_mDiagnose_1_c		mDiagnose_1._c[3]
#define c5_mDiagnose_1_c		mDiagnose_1._c[4]
#define c6_mDiagnose_1_c		mDiagnose_1._c[5]
#define c7_mDiagnose_1_c		mDiagnose_1._c[6]
#define c8_mDiagnose_1_c		mDiagnose_1._c[7]

#define c1_mSollverbau_neu_c		mSollverbau_neu._c[0]
#define c2_mSollverbau_neu_c		mSollverbau_neu._c[1]
#define c3_mSollverbau_neu_c		mSollverbau_neu._c[2]
#define c4_mSollverbau_neu_c		mSollverbau_neu._c[3]
#define c5_mSollverbau_neu_c		mSollverbau_neu._c[4]
#define c6_mSollverbau_neu_c		mSollverbau_neu._c[5]
#define c7_mSollverbau_neu_c		mSollverbau_neu._c[6]
#define c8_mSollverbau_neu_c		mSollverbau_neu._c[7]

#define c1_mGateway_3_c		mGateway_3._c[0]
#define c2_mGateway_3_c		mGateway_3._c[1]

#define c1_mSysteminfo_1_c		mSysteminfo_1._c[0]
#define c2_mSysteminfo_1_c		mSysteminfo_1._c[1]
#define c3_mSysteminfo_1_c		mSysteminfo_1._c[2]
#define c4_mSysteminfo_1_c		mSysteminfo_1._c[3]
#define c5_mSysteminfo_1_c		mSysteminfo_1._c[4]
#define c6_mSysteminfo_1_c		mSysteminfo_1._c[5]
#define c7_mSysteminfo_1_c		mSysteminfo_1._c[6]

#define c1_mMFL_Tasten_c		mMFL_Tasten._c[0]
#define c2_mMFL_Tasten_c		mMFL_Tasten._c[1]
#define c3_mMFL_Tasten_c		mMFL_Tasten._c[2]
#define c4_mMFL_Tasten_c		mMFL_Tasten._c[3]

#define c1_MFL_Tasten_Kon_01_c		MFL_Tasten_Kon_01._c[0]
#define c2_MFL_Tasten_Kon_01_c		MFL_Tasten_Kon_01._c[1]
#define c3_MFL_Tasten_Kon_01_c		MFL_Tasten_Kon_01._c[2]
#define c4_MFL_Tasten_Kon_01_c		MFL_Tasten_Kon_01._c[3]

#define c1_mBSG_3_c		mBSG_3._c[0]

#define c1_mBSG_2_c		mBSG_2._c[0]
#define c2_mBSG_2_c		mBSG_2._c[1]
#define c3_mBSG_2_c		mBSG_2._c[2]
#define c4_mBSG_2_c		mBSG_2._c[3]
#define c5_mBSG_2_c		mBSG_2._c[4]
#define c6_mBSG_2_c		mBSG_2._c[5]

#define c1_mGW_Kombi_c		mGW_Kombi._c[0]
#define c2_mGW_Kombi_c		mGW_Kombi._c[1]
#define c3_mGW_Kombi_c		mGW_Kombi._c[2]

#define c1_mBSG_Kombi_c		mBSG_Kombi._c[0]
#define c2_mBSG_Kombi_c		mBSG_Kombi._c[1]
#define c3_mBSG_Kombi_c		mBSG_Kombi._c[2]
#define c4_mBSG_Kombi_c		mBSG_Kombi._c[3]
#define c5_mBSG_Kombi_c		mBSG_Kombi._c[4]
#define c6_mBSG_Kombi_c		mBSG_Kombi._c[5]
#define c7_mBSG_Kombi_c		mBSG_Kombi._c[6]

#define c1_mLSM_1_c		mLSM_1._c[0]
#define c2_mLSM_1_c		mLSM_1._c[1]
#define c3_mLSM_1_c		mLSM_1._c[2]
#define c4_mLSM_1_c		mLSM_1._c[3]
#define c5_mLSM_1_c		mLSM_1._c[4]

#define c1_mAirbag_1_c		mAirbag_1._c[0]


/*************************************************************/
/* Handles of  receive objects                               */
/*************************************************************/

#define CanRxmASR7_Param_Feedback	0
#define CanRxmASR6_Param_Request	1
#define CanRxISO_Telefon_Req	2
#define CanRxISO_Funktionaler_Req_All	3
#define CanRxBAP_Debug_Cmd	4
#define CanRxBAP_ASG_03	5
#define CanRxBAP_ASG_06	6
#define CanRxBAP_ASG_07	7
#define CanRxmRadio_4	8
#define CanRxmDiagnose_1	9
#define CanRxmSollverbau_neu	10
#define CanRxmGateway_3	11
#define CanRxmSysteminfo_1	12
#define CanRxmMFL_Tasten	13
#define CanRxMFL_Tasten_Kon_01	14
#define CanRxmBSG_3	15
#define CanRxmBSG_2	16
#define CanRxmGW_Kombi	17
#define CanRxmBSG_Kombi	18
#define CanRxmLSM_1	19
#define CanRxmAirbag_1	20


/*************************************************************/
/* Databuffer for indicationflags                                */
/*************************************************************/

#define mASR7_Param_Feedback_ind_b 	CanIndicationFlags.w[0].b0
#define CanWriteSyncmASR7_Param_Feedback_ind_b(x)\
		CanStartFlagWriteSync();\
		mASR7_Param_Feedback_ind_b = x;\
		CanEndFlagWriteSync();

#define mASR6_Param_Request_ind_b 	CanIndicationFlags.w[0].b1
#define CanWriteSyncmASR6_Param_Request_ind_b(x)\
		CanStartFlagWriteSync();\
		mASR6_Param_Request_ind_b = x;\
		CanEndFlagWriteSync();

#define mRadio_4_ind_b 	CanIndicationFlags.w[0].b2
#define CanWriteSyncmRadio_4_ind_b(x)\
		CanStartFlagWriteSync();\
		mRadio_4_ind_b = x;\
		CanEndFlagWriteSync();

#define mDiagnose_1_ind_b 	CanIndicationFlags.w[0].b3
#define CanWriteSyncmDiagnose_1_ind_b(x)\
		CanStartFlagWriteSync();\
		mDiagnose_1_ind_b = x;\
		CanEndFlagWriteSync();

#define mSollverbau_neu_ind_b 	CanIndicationFlags.w[0].b4
#define CanWriteSyncmSollverbau_neu_ind_b(x)\
		CanStartFlagWriteSync();\
		mSollverbau_neu_ind_b = x;\
		CanEndFlagWriteSync();

#define mGateway_3_ind_b 	CanIndicationFlags.w[0].b5
#define CanWriteSyncmGateway_3_ind_b(x)\
		CanStartFlagWriteSync();\
		mGateway_3_ind_b = x;\
		CanEndFlagWriteSync();

#define mSysteminfo_1_ind_b 	CanIndicationFlags.w[0].b6
#define CanWriteSyncmSysteminfo_1_ind_b(x)\
		CanStartFlagWriteSync();\
		mSysteminfo_1_ind_b = x;\
		CanEndFlagWriteSync();

#define mMFL_Tasten_ind_b 	CanIndicationFlags.w[0].b7
#define CanWriteSyncmMFL_Tasten_ind_b(x)\
		CanStartFlagWriteSync();\
		mMFL_Tasten_ind_b = x;\
		CanEndFlagWriteSync();

#define MFL_Tasten_Kon_01_ind_b 	CanIndicationFlags.w[0].b10
#define CanWriteSyncMFL_Tasten_Kon_01_ind_b(x)\
		CanStartFlagWriteSync();\
		MFL_Tasten_Kon_01_ind_b = x;\
		CanEndFlagWriteSync();

#define mBSG_3_ind_b 	CanIndicationFlags.w[0].b11
#define CanWriteSyncmBSG_3_ind_b(x)\
		CanStartFlagWriteSync();\
		mBSG_3_ind_b = x;\
		CanEndFlagWriteSync();

#define mBSG_2_ind_b 	CanIndicationFlags.w[0].b12
#define CanWriteSyncmBSG_2_ind_b(x)\
		CanStartFlagWriteSync();\
		mBSG_2_ind_b = x;\
		CanEndFlagWriteSync();

#define mGW_Kombi_ind_b 	CanIndicationFlags.w[0].b13
#define CanWriteSyncmGW_Kombi_ind_b(x)\
		CanStartFlagWriteSync();\
		mGW_Kombi_ind_b = x;\
		CanEndFlagWriteSync();

#define mBSG_Kombi_ind_b 	CanIndicationFlags.w[0].b14
#define CanWriteSyncmBSG_Kombi_ind_b(x)\
		CanStartFlagWriteSync();\
		mBSG_Kombi_ind_b = x;\
		CanEndFlagWriteSync();

#define mLSM_1_ind_b 	CanIndicationFlags.w[0].b15
#define CanWriteSyncmLSM_1_ind_b(x)\
		CanStartFlagWriteSync();\
		mLSM_1_ind_b = x;\
		CanEndFlagWriteSync();

#define mAirbag_1_ind_b 	CanIndicationFlags.w[0].b16
#define CanWriteSyncmAirbag_1_ind_b(x)\
		CanStartFlagWriteSync();\
		mAirbag_1_ind_b = x;\
		CanEndFlagWriteSync();


/*************************************************************/
/* Signals of send messages                                  */
/*************************************************************/

#define b_ASR5_Char24_c	 mASR5_String3.masr5_string3.ASR5_Char24
#define b_ASR5_Char25_c	 mASR5_String3.masr5_string3.ASR5_Char25
#define b_ASR5_Char26_c	 mASR5_String3.masr5_string3.ASR5_Char26
#define b_ASR5_Char27_c	 mASR5_String3.masr5_string3.ASR5_Char27
#define b_ASR5_Char28_c	 mASR5_String3.masr5_string3.ASR5_Char28
#define b_ASR5_Char29_c	 mASR5_String3.masr5_string3.ASR5_Char29
#define b_ASR5_Char30_c	 mASR5_String3.masr5_string3.ASR5_Char30
#define b_ASR5_Char31_c	 mASR5_String3.masr5_string3.ASR5_Char31

#define b_ASR4_Char16_c	 mASR4_String2.masr4_string2.ASR4_Char16
#define b_ASR4_Char17_c	 mASR4_String2.masr4_string2.ASR4_Char17
#define b_ASR4_Char18_c	 mASR4_String2.masr4_string2.ASR4_Char18
#define b_ASR4_Char19_c	 mASR4_String2.masr4_string2.ASR4_Char19
#define b_ASR4_Char20_c	 mASR4_String2.masr4_string2.ASR4_Char20
#define b_ASR4_Char21_c	 mASR4_String2.masr4_string2.ASR4_Char21
#define b_ASR4_Char22_c	 mASR4_String2.masr4_string2.ASR4_Char22
#define b_ASR4_Char23_c	 mASR4_String2.masr4_string2.ASR4_Char23

#define b_ASR3_Char08_c	 mASR3_String1.masr3_string1.ASR3_Char08
#define b_ASR3_Char09_c	 mASR3_String1.masr3_string1.ASR3_Char09
#define b_ASR3_Char10_c	 mASR3_String1.masr3_string1.ASR3_Char10
#define b_ASR3_Char11_c	 mASR3_String1.masr3_string1.ASR3_Char11
#define b_ASR3_Char12_c	 mASR3_String1.masr3_string1.ASR3_Char12
#define b_ASR3_Char13_c	 mASR3_String1.masr3_string1.ASR3_Char13
#define b_ASR3_Char14_c	 mASR3_String1.masr3_string1.ASR3_Char14
#define b_ASR3_Char15_c	 mASR3_String1.masr3_string1.ASR3_Char15

#define b_ASR2_Char00_c	 mASR2_String0.masr2_string0.ASR2_Char00
#define b_ASR2_Char01_c	 mASR2_String0.masr2_string0.ASR2_Char01
#define b_ASR2_Char02_c	 mASR2_String0.masr2_string0.ASR2_Char02
#define b_ASR2_Char03_c	 mASR2_String0.masr2_string0.ASR2_Char03
#define b_ASR2_Char04_c	 mASR2_String0.masr2_string0.ASR2_Char04
#define b_ASR2_Char05_c	 mASR2_String0.masr2_string0.ASR2_Char05
#define b_ASR2_Char06_c	 mASR2_String0.masr2_string0.ASR2_Char06
#define b_ASR2_Char07_c	 mASR2_String0.masr2_string0.ASR2_Char07

#define b_ASR1_Is_Command_ID_b	 mASR1_TypeInfo.masr1_typeinfo.ASR1_Is_Command_ID
#define b_ASR1_Is_String_TimeMultiplexed_b	 mASR1_TypeInfo.masr1_typeinfo.ASR1_Is_String_TimeMultiplexed
#define b_ASR1_Is_Multiplex_Last_Msg_b	 mASR1_TypeInfo.masr1_typeinfo.ASR1_Is_Multiplex_Last_Msg
#define b_ASR1_Cnt_Multiplex_String_b	 mASR1_TypeInfo.masr1_typeinfo.ASR1_Cnt_Multiplex_String
#define b_ASR1_Confidence_b_0	 mASR1_TypeInfo.masr1_typeinfo.ASR1_Confidence_0
#define b_ASR1_Confidence_b_1	 mASR1_TypeInfo.masr1_typeinfo.ASR1_Confidence_1
#define                                                                                                                                                                                                                                                            b_ASR1_Confidence_b(c)   { b_ASR1_Confidence_b_0 = ((canuint8)(((canuint16)(c) & 0xff)));\
                                                                                                                                                                                                                                                                                              b_ASR1_Confidence_b_1 = ((canuint8)(((canuint16)(c) & 0xff00) >> 8));\
                                                                                                                                                                                                                                                                                            }
#define b_ASR1_Command_ID_b_0	 mASR1_TypeInfo.masr1_typeinfo.ASR1_Command_ID_0
#define b_ASR1_Command_ID_b_1	 mASR1_TypeInfo.masr1_typeinfo.ASR1_Command_ID_1
#define b_ASR1_Command_ID_b_2	 mASR1_TypeInfo.masr1_typeinfo.ASR1_Command_ID_2
#define                                                                                                                                                                                                                                                            b_ASR1_Command_ID_b(c)   { b_ASR1_Command_ID_b_0 = ((canuint8)(((canuint32)(c) & 0xff)));\
                                                                                                                                                                                                                                                                                              b_ASR1_Command_ID_b_1 = ((canuint8)(((canuint32)(c) & 0xff00) >> 8));\
                                                                                                                                                                                                                                                                                              b_ASR1_Command_ID_b_2 = ((canuint8)(((canuint32)(c) & 0xff0000) >> 16));\
                                                                                                                                                                                                                                                                                            }
#define b_ASR1_Is_SubCmd_Last_Msg_b	 mASR1_TypeInfo.masr1_typeinfo.ASR1_Is_SubCmd_Last_Msg
#define b_ASR1_Cnt_SubCmd_b	 mASR1_TypeInfo.masr1_typeinfo.ASR1_Cnt_SubCmd
#define b_ASR1_Is_NBest_Last_Msg_b	 mASR1_TypeInfo.masr1_typeinfo.ASR1_Is_NBest_Last_Msg
#define b_ASR1_Cnt_NBest_b	 mASR1_TypeInfo.masr1_typeinfo.ASR1_Cnt_NBest

#define b_ASR0_Status_Activ_b	 mASR0_Status.masr0_status.ASR0_Status_Activ
#define b_ASR0_Status_RecogOpen_b	 mASR0_Status.masr0_status.ASR0_Status_RecogOpen
#define b_ASR0_Status_StateID_b_0	 mASR0_Status.masr0_status.ASR0_Status_StateID_0
#define b_ASR0_Status_StateID_b_1	 mASR0_Status.masr0_status.ASR0_Status_StateID_1
#define b_ASR0_Status_StateID_b_2	 mASR0_Status.masr0_status.ASR0_Status_StateID_2
#define                                                                                                                                                                                                                                                        b_ASR0_Status_StateID_b(c)   { b_ASR0_Status_StateID_b_0 = ((canuint8)(((canuint32)(c) & 0xff)));\
                                                                                                                                                                                                                                                                                              b_ASR0_Status_StateID_b_1 = ((canuint8)(((canuint32)(c) & 0xff00) >> 8));\
                                                                                                                                                                                                                                                                                              b_ASR0_Status_StateID_b_2 = ((canuint8)(((canuint32)(c) & 0xff0000) >> 16));\
                                                                                                                                                                                                                                                                                            }

#define b_TF3_Phone_on_b	 mTelefon_3.mtelefon_3.TF3_Phone_on
#define b_TF3_Show_Phone_b	 mTelefon_3.mtelefon_3.TF3_Show_Phone
#define b_TF3_Phone_mute_b	 mTelefon_3.mtelefon_3.TF3_Phone_mute
#define b_TF3_Phone_active_call_b	 mTelefon_3.mtelefon_3.TF3_Phone_active_call
#define b_TF3_Phone_ring_b	 mTelefon_3.mtelefon_3.TF3_Phone_ring
#define b_TF3_Phone_sim_OK_b	 mTelefon_3.mtelefon_3.TF3_Phone_sim_OK
#define b_TF3_Phone_pin_OK_b	 mTelefon_3.mtelefon_3.TF3_Phone_pin_OK
#define b_TF3_Phone_Error_flag_b	 mTelefon_3.mtelefon_3.TF3_Phone_Error_flag
#define b_TF3_Phone_Level_b	 mTelefon_3.mtelefon_3.TF3_Phone_Level
#define b_TF3_Phone_Display_Vol_b	 mTelefon_3.mtelefon_3.TF3_Phone_Display_Vol
#define b_TF3_Phone_Init_b	 mTelefon_3.mtelefon_3.TF3_Phone_Init
#define b_TF3_Mute_Msg_Code_b	 mTelefon_3.mtelefon_3.TF3_Mute_Msg_Code
#define b_TF3_Wake_Up_Komfort_b	 mTelefon_3.mtelefon_3.TF3_Wake_Up_Komfort
#define b_TF3_Radio_Access_b	 mTelefon_3.mtelefon_3.TF3_Radio_Access
#define b_TF3_BT_ConnectionState_b	 mTelefon_3.mtelefon_3.TF3_BT_ConnectionState
#define b_TF3_Provider_b_0	 mTelefon_3.mtelefon_3.TF3_Provider_0
#define b_TF3_Provider_b_1	 mTelefon_3.mtelefon_3.TF3_Provider_1
#define                                                                                                                                                                                                                                                               b_TF3_Provider_b(c)   { b_TF3_Provider_b_0 = ((canuint8)(((canuint16)(c) & 0xff)));\
                                                                                                                                                                                                                                                                                              b_TF3_Provider_b_1 = ((canuint8)(((canuint16)(c) & 0xff00) >> 8));\
                                                                                                                                                                                                                                                                                            }
#define b_TF3_Feldstaerke_b	 mTelefon_3.mtelefon_3.TF3_Feldstaerke
#define b_TF3_Laendercode_b_0	 mTelefon_3.mtelefon_3.TF3_Laendercode_0
#define b_TF3_Laendercode_b_1	 mTelefon_3.mtelefon_3.TF3_Laendercode_1
#define                                                                                                                                                                                                                                                            b_TF3_Laendercode_b(c)   { b_TF3_Laendercode_b_0 = ((canuint8)(((canuint16)(c) & 0xff)));\
                                                                                                                                                                                                                                                                                              b_TF3_Laendercode_b_1 = ((canuint8)(((canuint16)(c) & 0xff00) >> 8));\
                                                                                                                                                                                                                                                                                            }
#define b_TF3_Feldstaerke_Fehler_b	 mTelefon_3.mtelefon_3.TF3_Feldstaerke_Fehler
#define b_TF3_Registrierungsstatus_b	 mTelefon_3.mtelefon_3.TF3_Registrierungsstatus

/*************************************************************/
/* Signals of receive messages                               */
/*************************************************************/

#define b_ASR7_Is_Read_Feedback_b	 mASR7_Param_Feedback.masr7_param_feedback.ASR7_Is_Read_Feedback
#define b_ASR7_Write_Success_b	 mASR7_Param_Feedback.masr7_param_feedback.ASR7_Write_Success
#define b_ASR7_Param_ID_b_0	 mASR7_Param_Feedback.masr7_param_feedback.ASR7_Param_ID_0
#define b_ASR7_Param_ID_b_1	 mASR7_Param_Feedback.masr7_param_feedback.ASR7_Param_ID_1
#define                                                                                                                                                                                                                                                                 b_ASR7_Param_ID_b  (canuint16)(((canuint16)(b_ASR7_Param_ID_b_1) << 8)\
                                                                                                                                                                                                                                                                                         | (b_ASR7_Param_ID_b_0))
#define b_ASR7_Param_GetValue_b_0	 mASR7_Param_Feedback.masr7_param_feedback.ASR7_Param_GetValue_0
#define b_ASR7_Param_GetValue_b_1	 mASR7_Param_Feedback.masr7_param_feedback.ASR7_Param_GetValue_1
#define b_ASR7_Param_GetValue_b_2	 mASR7_Param_Feedback.masr7_param_feedback.ASR7_Param_GetValue_2
#define b_ASR7_Param_GetValue_b_3	 mASR7_Param_Feedback.masr7_param_feedback.ASR7_Param_GetValue_3
#define b_ASR7_Param_GetValue_b_4	 mASR7_Param_Feedback.masr7_param_feedback.ASR7_Param_GetValue_4
#define b_CAN_ASR7_Is_Read_Feedback_b	 RDS2.masr7_param_feedback.ASR7_Is_Read_Feedback
#define b_CAN_ASR7_Write_Success_b	 RDS2.masr7_param_feedback.ASR7_Write_Success
#define b_CAN_ASR7_Param_ID_b_0	 RDS2.masr7_param_feedback.ASR7_Param_ID_0
#define b_CAN_ASR7_Param_ID_b_1	 RDS2.masr7_param_feedback.ASR7_Param_ID_1
#define                                                                                                                                                                                                                                                             b_CAN_ASR7_Param_ID_b  (canuint16)(((canuint16)(b_CAN_ASR7_Param_ID_b_1) << 8)\
                                                                                                                                                                                                                                                                                         | (b_CAN_ASR7_Param_ID_b_0))
#define b_CAN_ASR7_Param_GetValue_b_0	 RDS2.masr7_param_feedback.ASR7_Param_GetValue_0
#define b_CAN_ASR7_Param_GetValue_b_1	 RDS2.masr7_param_feedback.ASR7_Param_GetValue_1
#define b_CAN_ASR7_Param_GetValue_b_2	 RDS2.masr7_param_feedback.ASR7_Param_GetValue_2
#define b_CAN_ASR7_Param_GetValue_b_3	 RDS2.masr7_param_feedback.ASR7_Param_GetValue_3
#define b_CAN_ASR7_Param_GetValue_b_4	 RDS2.masr7_param_feedback.ASR7_Param_GetValue_4

#define b_ASR6_Is_Read_Request_b	 mASR6_Param_Request.masr6_param_request.ASR6_Is_Read_Request
#define b_ASR6_Param_ID_b_0	 mASR6_Param_Request.masr6_param_request.ASR6_Param_ID_0
#define b_ASR6_Param_ID_b_1	 mASR6_Param_Request.masr6_param_request.ASR6_Param_ID_1
#define                                                                                                                                                                                                                                                                 b_ASR6_Param_ID_b  (canuint16)(((canuint16)(b_ASR6_Param_ID_b_1) << 8)\
                                                                                                                                                                                                                                                                                         | (b_ASR6_Param_ID_b_0))
#define b_ASR6_Param_SetValue_b_0	 mASR6_Param_Request.masr6_param_request.ASR6_Param_SetValue_0
#define b_ASR6_Param_SetValue_b_1	 mASR6_Param_Request.masr6_param_request.ASR6_Param_SetValue_1
#define b_ASR6_Param_SetValue_b_2	 mASR6_Param_Request.masr6_param_request.ASR6_Param_SetValue_2
#define b_ASR6_Param_SetValue_b_3	 mASR6_Param_Request.masr6_param_request.ASR6_Param_SetValue_3
#define b_ASR6_Param_SetValue_b_4	 mASR6_Param_Request.masr6_param_request.ASR6_Param_SetValue_4
#define b_CAN_ASR6_Is_Read_Request_b	 RDS3.masr6_param_request.ASR6_Is_Read_Request
#define b_CAN_ASR6_Param_ID_b_0	 RDS3.masr6_param_request.ASR6_Param_ID_0
#define b_CAN_ASR6_Param_ID_b_1	 RDS3.masr6_param_request.ASR6_Param_ID_1
#define                                                                                                                                                                                                                                                             b_CAN_ASR6_Param_ID_b  (canuint16)(((canuint16)(b_CAN_ASR6_Param_ID_b_1) << 8)\
                                                                                                                                                                                                                                                                                         | (b_CAN_ASR6_Param_ID_b_0))
#define b_CAN_ASR6_Param_SetValue_b_0	 RDS3.masr6_param_request.ASR6_Param_SetValue_0
#define b_CAN_ASR6_Param_SetValue_b_1	 RDS3.masr6_param_request.ASR6_Param_SetValue_1
#define b_CAN_ASR6_Param_SetValue_b_2	 RDS3.masr6_param_request.ASR6_Param_SetValue_2
#define b_CAN_ASR6_Param_SetValue_b_3	 RDS3.masr6_param_request.ASR6_Param_SetValue_3
#define b_CAN_ASR6_Param_SetValue_b_4	 RDS3.masr6_param_request.ASR6_Param_SetValue_4

#define b_BAP_Data_Debug_Cmd_b_0	 BAP_Debug_Cmd.bap_debug_cmd.BAP_Data_Debug_Cmd_0
#define b_BAP_Data_Debug_Cmd_b_1	 BAP_Debug_Cmd.bap_debug_cmd.BAP_Data_Debug_Cmd_1
#define                                                                                                                                                                                                                                                            b_BAP_Data_Debug_Cmd_b  (canuint16)(((canuint16)(b_BAP_Data_Debug_Cmd_b_1) << 8)\
                                                                                                                                                                                                                                                                                         | (b_BAP_Data_Debug_Cmd_b_0))
#define b_CAN_BAP_Data_Debug_Cmd_b_0	 RDS6.bap_debug_cmd.BAP_Data_Debug_Cmd_0
#define b_CAN_BAP_Data_Debug_Cmd_b_1	 RDS6.bap_debug_cmd.BAP_Data_Debug_Cmd_1
#define                                                                                                                                                                                                                                                        b_CAN_BAP_Data_Debug_Cmd_b  (canuint16)(((canuint16)(b_CAN_BAP_Data_Debug_Cmd_b_1) << 8)\
                                                                                                                                                                                                                                                                                         | (b_CAN_BAP_Data_Debug_Cmd_b_0))

#define b_BAP_Data_ASG_03_b_0	 BAP_ASG_03.bap_asg_03.BAP_Data_ASG_03_0
#define b_BAP_Data_ASG_03_b_1	 BAP_ASG_03.bap_asg_03.BAP_Data_ASG_03_1
#define                                                                                                                                                                                                                                                               b_BAP_Data_ASG_03_b  (canuint16)(((canuint16)(b_BAP_Data_ASG_03_b_1) << 8)\
                                                                                                                                                                                                                                                                                         | (b_BAP_Data_ASG_03_b_0))
#define b_CAN_BAP_Data_ASG_03_b_0	 RDS7.bap_asg_03.BAP_Data_ASG_03_0
#define b_CAN_BAP_Data_ASG_03_b_1	 RDS7.bap_asg_03.BAP_Data_ASG_03_1
#define                                                                                                                                                                                                                                                           b_CAN_BAP_Data_ASG_03_b  (canuint16)(((canuint16)(b_CAN_BAP_Data_ASG_03_b_1) << 8)\
                                                                                                                                                                                                                                                                                         | (b_CAN_BAP_Data_ASG_03_b_0))

#define b_BAP_Data_ASG_06_b_0	 BAP_ASG_06.bap_asg_06.BAP_Data_ASG_06_0
#define b_BAP_Data_ASG_06_b_1	 BAP_ASG_06.bap_asg_06.BAP_Data_ASG_06_1
#define                                                                                                                                                                                                                                                               b_BAP_Data_ASG_06_b  (canuint16)(((canuint16)(b_BAP_Data_ASG_06_b_1) << 8)\
                                                                                                                                                                                                                                                                                         | (b_BAP_Data_ASG_06_b_0))
#define b_CAN_BAP_Data_ASG_06_b_0	 RDS8.bap_asg_06.BAP_Data_ASG_06_0
#define b_CAN_BAP_Data_ASG_06_b_1	 RDS8.bap_asg_06.BAP_Data_ASG_06_1
#define                                                                                                                                                                                                                                                           b_CAN_BAP_Data_ASG_06_b  (canuint16)(((canuint16)(b_CAN_BAP_Data_ASG_06_b_1) << 8)\
                                                                                                                                                                                                                                                                                         | (b_CAN_BAP_Data_ASG_06_b_0))

#define b_BAP_Data_ASG_07_b_0	 BAP_ASG_07.bap_asg_07.BAP_Data_ASG_07_0
#define b_BAP_Data_ASG_07_b_1	 BAP_ASG_07.bap_asg_07.BAP_Data_ASG_07_1
#define                                                                                                                                                                                                                                                               b_BAP_Data_ASG_07_b  (canuint16)(((canuint16)(b_BAP_Data_ASG_07_b_1) << 8)\
                                                                                                                                                                                                                                                                                         | (b_BAP_Data_ASG_07_b_0))
#define b_CAN_BAP_Data_ASG_07_b_0	 RDS9.bap_asg_07.BAP_Data_ASG_07_0
#define b_CAN_BAP_Data_ASG_07_b_1	 RDS9.bap_asg_07.BAP_Data_ASG_07_1
#define                                                                                                                                                                                                                                                           b_CAN_BAP_Data_ASG_07_b  (canuint16)(((canuint16)(b_CAN_BAP_Data_ASG_07_b_1) << 8)\
                                                                                                                                                                                                                                                                                         | (b_CAN_BAP_Data_ASG_07_b_0))

#define b_RA4_Radio_on_b	 mRadio_4.mradio_4.RA4_Radio_on
#define b_RA4_Radio_mute_b	 mRadio_4.mradio_4.RA4_Radio_mute
#define b_CAN_RA4_Radio_on_b	 RDS10.mradio_4.RA4_Radio_on
#define b_CAN_RA4_Radio_mute_b	 RDS10.mradio_4.RA4_Radio_mute

#define b_DN1_Verlernzaehler_c	 mDiagnose_1.mdiagnose_1.DN1_Verlernzaehler
#define b_DN1_KM_Stand_b_0	 mDiagnose_1.mdiagnose_1.DN1_KM_Stand_0
#define b_DN1_KM_Stand_b_1	 mDiagnose_1.mdiagnose_1.DN1_KM_Stand_1
#define b_DN1_KM_Stand_b_2	 mDiagnose_1.mdiagnose_1.DN1_KM_Stand_2
#define                                                                                                                                                                                                                                                                  b_DN1_KM_Stand_b  (canuint32)(((canuint32)(b_DN1_KM_Stand_b_2) << 16)\
                                                                                                                                                                                                                                                                                         | ((canuint32)(b_DN1_KM_Stand_b_1) << 8)\
                                                                                                                                                                                                                                                                                         | (b_DN1_KM_Stand_b_0))
#define b_DN1_Jahr_b_0	 mDiagnose_1.mdiagnose_1.DN1_Jahr_0
#define b_DN1_Jahr_b_1	 mDiagnose_1.mdiagnose_1.DN1_Jahr_1
#define                                                                                                                                                                                                                                                                      b_DN1_Jahr_b  (canuint16)(((canuint16)(b_DN1_Jahr_b_1) << 4)\
                                                                                                                                                                                                                                                                                         | (b_DN1_Jahr_b_0))
#define b_DN1_Monat_b	 mDiagnose_1.mdiagnose_1.DN1_Monat
#define b_DN1_Tag_b_0	 mDiagnose_1.mdiagnose_1.DN1_Tag_0
#define b_DN1_Tag_b_1	 mDiagnose_1.mdiagnose_1.DN1_Tag_1
#define                                                                                                                                                                                                                                                                       b_DN1_Tag_b  (canuint16)(((canuint16)(b_DN1_Tag_b_1) << 1)\
                                                                                                                                                                                                                                                                                         | (b_DN1_Tag_b_0))
#define b_DN1_Stunde_b_0	 mDiagnose_1.mdiagnose_1.DN1_Stunde_0
#define b_DN1_Stunde_b_1	 mDiagnose_1.mdiagnose_1.DN1_Stunde_1
#define                                                                                                                                                                                                                                                                    b_DN1_Stunde_b  (canuint16)(((canuint16)(b_DN1_Stunde_b_1) << 4)\
                                                                                                                                                                                                                                                                                         | (b_DN1_Stunde_b_0))
#define b_DN1_Minute_b	 mDiagnose_1.mdiagnose_1.DN1_Minute
#define b_DN1_Sekunde_b_0	 mDiagnose_1.mdiagnose_1.DN1_Sekunde_0
#define b_DN1_Sekunde_b_1	 mDiagnose_1.mdiagnose_1.DN1_Sekunde_1
#define                                                                                                                                                                                                                                                                   b_DN1_Sekunde_b  (canuint16)(((canuint16)(b_DN1_Sekunde_b_1) << 1)\
                                                                                                                                                                                                                                                                                         | (b_DN1_Sekunde_b_0))
#define b_DN1_alt_Kilometerstand_b	 mDiagnose_1.mdiagnose_1.DN1_alt_Kilometerstand
#define b_DN1_alt_Zeit_b	 mDiagnose_1.mdiagnose_1.DN1_alt_Zeit
#define b_CAN_DN1_Verlernzaehler_c	 RDS11.mdiagnose_1.DN1_Verlernzaehler
#define b_CAN_DN1_KM_Stand_b_0	 RDS11.mdiagnose_1.DN1_KM_Stand_0
#define b_CAN_DN1_KM_Stand_b_1	 RDS11.mdiagnose_1.DN1_KM_Stand_1
#define b_CAN_DN1_KM_Stand_b_2	 RDS11.mdiagnose_1.DN1_KM_Stand_2
#define                                                                                                                                                                                                                                                              b_CAN_DN1_KM_Stand_b  (canuint32)(((canuint32)(b_CAN_DN1_KM_Stand_b_2) << 16)\
                                                                                                                                                                                                                                                                                         | ((canuint32)(b_CAN_DN1_KM_Stand_b_1) << 8)\
                                                                                                                                                                                                                                                                                         | (b_CAN_DN1_KM_Stand_b_0))
#define b_CAN_DN1_Jahr_b_0	 RDS11.mdiagnose_1.DN1_Jahr_0
#define b_CAN_DN1_Jahr_b_1	 RDS11.mdiagnose_1.DN1_Jahr_1
#define                                                                                                                                                                                                                                                                  b_CAN_DN1_Jahr_b  (canuint16)(((canuint16)(b_CAN_DN1_Jahr_b_1) << 4)\
                                                                                                                                                                                                                                                                                         | (b_CAN_DN1_Jahr_b_0))
#define b_CAN_DN1_Monat_b	 RDS11.mdiagnose_1.DN1_Monat
#define b_CAN_DN1_Tag_b_0	 RDS11.mdiagnose_1.DN1_Tag_0
#define b_CAN_DN1_Tag_b_1	 RDS11.mdiagnose_1.DN1_Tag_1
#define                                                                                                                                                                                                                                                                   b_CAN_DN1_Tag_b  (canuint16)(((canuint16)(b_CAN_DN1_Tag_b_1) << 1)\
                                                                                                                                                                                                                                                                                         | (b_CAN_DN1_Tag_b_0))
#define b_CAN_DN1_Stunde_b_0	 RDS11.mdiagnose_1.DN1_Stunde_0
#define b_CAN_DN1_Stunde_b_1	 RDS11.mdiagnose_1.DN1_Stunde_1
#define                                                                                                                                                                                                                                                                b_CAN_DN1_Stunde_b  (canuint16)(((canuint16)(b_CAN_DN1_Stunde_b_1) << 4)\
                                                                                                                                                                                                                                                                                         | (b_CAN_DN1_Stunde_b_0))
#define b_CAN_DN1_Minute_b	 RDS11.mdiagnose_1.DN1_Minute
#define b_CAN_DN1_Sekunde_b_0	 RDS11.mdiagnose_1.DN1_Sekunde_0
#define b_CAN_DN1_Sekunde_b_1	 RDS11.mdiagnose_1.DN1_Sekunde_1
#define                                                                                                                                                                                                                                                               b_CAN_DN1_Sekunde_b  (canuint16)(((canuint16)(b_CAN_DN1_Sekunde_b_1) << 1)\
                                                                                                                                                                                                                                                                                         | (b_CAN_DN1_Sekunde_b_0))
#define b_CAN_DN1_alt_Kilometerstand_b	 RDS11.mdiagnose_1.DN1_alt_Kilometerstand
#define b_CAN_DN1_alt_Zeit_b	 RDS11.mdiagnose_1.DN1_alt_Zeit

#define b_VBN_Soll_Ist_OK_b	 mSollverbau_neu.msollverbau_neu.VBN_Soll_Ist_OK
#define b_VBN_SMLS_K_b	 mSollverbau_neu.msollverbau_neu.VBN_SMLS_K
#define b_VBN_Tastenmodul_I_b	 mSollverbau_neu.msollverbau_neu.VBN_Tastenmodul_I
#define b_VBN_DSP_I_b	 mSollverbau_neu.msollverbau_neu.VBN_DSP_I
#define b_VBN_Telematik_I_b	 mSollverbau_neu.msollverbau_neu.VBN_Telematik_I
#define b_VBN_Navigation_I_b	 mSollverbau_neu.msollverbau_neu.VBN_Navigation_I
#define b_VBN_Radio_I_b	 mSollverbau_neu.msollverbau_neu.VBN_Radio_I
#define b_VBN_Telefon_I_b	 mSollverbau_neu.msollverbau_neu.VBN_Telefon_I
#define b_CAN_VBN_Soll_Ist_OK_b	 RDS12.msollverbau_neu.VBN_Soll_Ist_OK
#define b_CAN_VBN_SMLS_K_b	 RDS12.msollverbau_neu.VBN_SMLS_K
#define b_CAN_VBN_Tastenmodul_I_b	 RDS12.msollverbau_neu.VBN_Tastenmodul_I
#define b_CAN_VBN_DSP_I_b	 RDS12.msollverbau_neu.VBN_DSP_I
#define b_CAN_VBN_Telematik_I_b	 RDS12.msollverbau_neu.VBN_Telematik_I
#define b_CAN_VBN_Navigation_I_b	 RDS12.msollverbau_neu.VBN_Navigation_I
#define b_CAN_VBN_Radio_I_b	 RDS12.msollverbau_neu.VBN_Radio_I
#define b_CAN_VBN_Telefon_I_b	 RDS12.msollverbau_neu.VBN_Telefon_I

#define b_GW3_Laendervariante_b	 mGateway_3.mgateway_3.GW3_Laendervariante
#define b_GW3_Alt_3_Kombi_b	 mGateway_3.mgateway_3.GW3_Alt_3_Kombi
#define b_GW3_Land_Sprach_empf_b	 mGateway_3.mgateway_3.GW3_Land_Sprach_empf
#define b_GW3_Sprachvariante_c	 mGateway_3.mgateway_3.GW3_Sprachvariante
#define b_CAN_GW3_Laendervariante_b	 RDS13.mgateway_3.GW3_Laendervariante
#define b_CAN_GW3_Alt_3_Kombi_b	 RDS13.mgateway_3.GW3_Alt_3_Kombi
#define b_CAN_GW3_Land_Sprach_empf_b	 RDS13.mgateway_3.GW3_Land_Sprach_empf
#define b_CAN_GW3_Sprachvariante_c	 RDS13.mgateway_3.GW3_Sprachvariante

#define b_SY1_Sleep_Komfort_b	 mSysteminfo_1.msysteminfo_1.SY1_Sleep_Komfort
#define b_SY1_Diag_Infotainment_b	 mSysteminfo_1.msysteminfo_1.SY1_Diag_Infotainment
#define b_SY1_Infotainment_b	 mSysteminfo_1.msysteminfo_1.SY1_Infotainment
#define b_SY1_Verbauliste_gueltig_b	 mSysteminfo_1.msysteminfo_1.SY1_Verbauliste_gueltig
#define b_SY1_Klasse_b	 mSysteminfo_1.msysteminfo_1.SY1_Klasse
#define b_SY1_Marke_b	 mSysteminfo_1.msysteminfo_1.SY1_Marke
#define b_SY1_Derivat_b	 mSysteminfo_1.msysteminfo_1.SY1_Derivat
#define b_SY1_Generation_b	 mSysteminfo_1.msysteminfo_1.SY1_Generation
#define b_SY1_Erweiterung_b	 mSysteminfo_1.msysteminfo_1.SY1_Erweiterung
#define b_SY1_Rechtslenker_b	 mSysteminfo_1.msysteminfo_1.SY1_Rechtslenker
#define b_SY1_Transportmode_b	 mSysteminfo_1.msysteminfo_1.SY1_Transportmode
#define b_SY1_NWDF_gueltig_b	 mSysteminfo_1.msysteminfo_1.SY1_NWDF_gueltig
#define b_SY1_NWDF_b	 mSysteminfo_1.msysteminfo_1.SY1_NWDF
#define b_CAN_SY1_Sleep_Komfort_b	 RDS14.msysteminfo_1.SY1_Sleep_Komfort
#define b_CAN_SY1_Diag_Infotainment_b	 RDS14.msysteminfo_1.SY1_Diag_Infotainment
#define b_CAN_SY1_Infotainment_b	 RDS14.msysteminfo_1.SY1_Infotainment
#define b_CAN_SY1_Verbauliste_gueltig_b	 RDS14.msysteminfo_1.SY1_Verbauliste_gueltig
#define b_CAN_SY1_Klasse_b	 RDS14.msysteminfo_1.SY1_Klasse
#define b_CAN_SY1_Marke_b	 RDS14.msysteminfo_1.SY1_Marke
#define b_CAN_SY1_Derivat_b	 RDS14.msysteminfo_1.SY1_Derivat
#define b_CAN_SY1_Generation_b	 RDS14.msysteminfo_1.SY1_Generation
#define b_CAN_SY1_Erweiterung_b	 RDS14.msysteminfo_1.SY1_Erweiterung
#define b_CAN_SY1_Rechtslenker_b	 RDS14.msysteminfo_1.SY1_Rechtslenker
#define b_CAN_SY1_Transportmode_b	 RDS14.msysteminfo_1.SY1_Transportmode
#define b_CAN_SY1_NWDF_gueltig_b	 RDS14.msysteminfo_1.SY1_NWDF_gueltig
#define b_CAN_SY1_NWDF_b	 RDS14.msysteminfo_1.SY1_NWDF

#define b_MLT_Symbol_c	 mMFL_Tasten.mmfl_tasten.MLT_Symbol
#define b_MLT_Tickzaehler_1_b	 mMFL_Tasten.mmfl_tasten.MLT_Tickzaehler_1
#define b_MLT_Marke_b	 mMFL_Tasten.mmfl_tasten.MLT_Marke
#define b_MLT_Variante_b	 mMFL_Tasten.mmfl_tasten.MLT_Variante
#define b_CAN_MLT_Symbol_c	 RDS15.mmfl_tasten.MLT_Symbol
#define b_CAN_MLT_Tickzaehler_1_b	 RDS15.mmfl_tasten.MLT_Tickzaehler_1
#define b_CAN_MLT_Marke_b	 RDS15.mmfl_tasten.MLT_Marke
#define b_CAN_MLT_Variante_b	 RDS15.mmfl_tasten.MLT_Variante

#define b_MFL_Tastencode_1_c	 MFL_Tasten_Kon_01.mfl_tasten_kon_01.MFL_Tastencode_1
#define b_MFL_Tastencode_2_c	 MFL_Tasten_Kon_01.mfl_tasten_kon_01.MFL_Tastencode_2
#define b_MFL_Eventcode_1_b	 MFL_Tasten_Kon_01.mfl_tasten_kon_01.MFL_Eventcode_1
#define b_MFL_Eventcode_2_b	 MFL_Tasten_Kon_01.mfl_tasten_kon_01.MFL_Eventcode_2
#define b_MFL_Marke_b	 MFL_Tasten_Kon_01.mfl_tasten_kon_01.MFL_Marke
#define b_MFL_Variante_b	 MFL_Tasten_Kon_01.mfl_tasten_kon_01.MFL_Variante
#define b_CAN_MFL_Tastencode_1_c	 RDS16.mfl_tasten_kon_01.MFL_Tastencode_1
#define b_CAN_MFL_Tastencode_2_c	 RDS16.mfl_tasten_kon_01.MFL_Tastencode_2
#define b_CAN_MFL_Eventcode_1_b	 RDS16.mfl_tasten_kon_01.MFL_Eventcode_1
#define b_CAN_MFL_Eventcode_2_b	 RDS16.mfl_tasten_kon_01.MFL_Eventcode_2
#define b_CAN_MFL_Marke_b	 RDS16.mfl_tasten_kon_01.MFL_Marke
#define b_CAN_MFL_Variante_b	 RDS16.mfl_tasten_kon_01.MFL_Variante

#define b_BS3_Klemme_S_b	 mBSG_3.mbsg_3.BS3_Klemme_S
#define b_BS3_Klemme_15_b	 mBSG_3.mbsg_3.BS3_Klemme_15
#define b_CAN_BS3_Klemme_S_b	 RDS17.mbsg_3.BS3_Klemme_S
#define b_CAN_BS3_Klemme_15_b	 RDS17.mbsg_3.BS3_Klemme_15

#define b_BS2_aus_Infotainment_b	 mBSG_2.mbsg_2.BS2_aus_Infotainment
#define b_BS2_Warn_Infotainment_b	 mBSG_2.mbsg_2.BS2_Warn_Infotainment
#define b_CAN_BS2_aus_Infotainment_b	 RDS18.mbsg_2.BS2_aus_Infotainment
#define b_CAN_BS2_Warn_Infotainment_b	 RDS18.mbsg_2.BS2_Warn_Infotainment

#define b_GWK_Alt_1_Kombi_b	 mGW_Kombi.mgw_kombi.GWK_Alt_1_Kombi
#define b_GWK_FzgGeschw_Quelle_b	 mGW_Kombi.mgw_kombi.GWK_FzgGeschw_Quelle
#define b_GWK_FzgGeschw_b_0	 mGW_Kombi.mgw_kombi.GWK_FzgGeschw_0
#define b_GWK_FzgGeschw_b_1	 mGW_Kombi.mgw_kombi.GWK_FzgGeschw_1
#define                                                                                                                                                                                                                                                                 b_GWK_FzgGeschw_b  (canuint16)(((canuint16)(b_GWK_FzgGeschw_b_1) << 7)\
                                                                                                                                                                                                                                                                                         | (b_GWK_FzgGeschw_b_0))
#define b_CAN_GWK_Alt_1_Kombi_b	 RDS19.mgw_kombi.GWK_Alt_1_Kombi
#define b_CAN_GWK_FzgGeschw_Quelle_b	 RDS19.mgw_kombi.GWK_FzgGeschw_Quelle
#define b_CAN_GWK_FzgGeschw_b_0	 RDS19.mgw_kombi.GWK_FzgGeschw_0
#define b_CAN_GWK_FzgGeschw_b_1	 RDS19.mgw_kombi.GWK_FzgGeschw_1
#define                                                                                                                                                                                                                                                             b_CAN_GWK_FzgGeschw_b  (canuint16)(((canuint16)(b_CAN_GWK_FzgGeschw_b_1) << 7)\
                                                                                                                                                                                                                                                                                         | (b_CAN_GWK_FzgGeschw_b_0))

#define b_BCM_Remotestart_Betrieb_b	 mBSG_Kombi.mbsg_kombi.BCM_Remotestart_Betrieb
#define b_CAN_BCM_Remotestart_Betrieb_b	 RDS20.mbsg_kombi.BCM_Remotestart_Betrieb

#define b_LS1_MFL_vorhanden_b	 mLSM_1.mlsm_1.LS1_MFL_vorhanden
#define b_LS1_MFA_vorhanden_b	 mLSM_1.mlsm_1.LS1_MFA_vorhanden
#define b_LS1_MFA_Tasten_b	 mLSM_1.mlsm_1.LS1_MFA_Tasten
#define b_LS1_MFL_Typ_b	 mLSM_1.mlsm_1.LS1_MFL_Typ
#define b_CAN_LS1_MFL_vorhanden_b	 RDS21.mlsm_1.LS1_MFL_vorhanden
#define b_CAN_LS1_MFA_vorhanden_b	 RDS21.mlsm_1.LS1_MFA_vorhanden
#define b_CAN_LS1_MFA_Tasten_b	 RDS21.mlsm_1.LS1_MFA_Tasten
#define b_CAN_LS1_MFL_Typ_b	 RDS21.mlsm_1.LS1_MFL_Typ

#define b_AB1_FrontCrash_b	 mAirbag_1.mairbag_1.AB1_FrontCrash
#define b_AB1_HeckCrash_b	 mAirbag_1.mairbag_1.AB1_HeckCrash
#define b_AB1_Crash_FT_b	 mAirbag_1.mairbag_1.AB1_Crash_FT
#define b_AB1_Crash_BT_b	 mAirbag_1.mairbag_1.AB1_Crash_BT
#define b_AB1_Rollover_b	 mAirbag_1.mairbag_1.AB1_Rollover
#define b_AB1_CrashStaerke_b	 mAirbag_1.mairbag_1.AB1_CrashStaerke
#define b_CAN_AB1_FrontCrash_b	 RDS22.mairbag_1.AB1_FrontCrash
#define b_CAN_AB1_HeckCrash_b	 RDS22.mairbag_1.AB1_HeckCrash
#define b_CAN_AB1_Crash_FT_b	 RDS22.mairbag_1.AB1_Crash_FT
#define b_CAN_AB1_Crash_BT_b	 RDS22.mairbag_1.AB1_Crash_BT
#define b_CAN_AB1_Rollover_b	 RDS22.mairbag_1.AB1_Rollover
#define b_CAN_AB1_CrashStaerke_b	 RDS22.mairbag_1.AB1_CrashStaerke


/* Initialization configurations */
#define kCanInitObj1 0



/*************************************************************/
/* Support for diagnostics/transport protocol            */
/*************************************************************/

#define kTpTxHandle	CanTxISO_Telefon_Resp
#define tpCanTxData	ISO_Telefon_Resp._c


V_MEMROM0 V_MEMROM1 extern CanReceiveHandle V_MEMROM2 kTpRxHandle_Field[1];

#define kTpRxHandle	CanRxISO_Telefon_Req

extern unsigned char DiagBuffer[22];


#endif

