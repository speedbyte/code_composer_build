/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     APP_CAN_CE.h
*-----------------------------------------------------------------------------
* Module Name:    APP_CAN 
*-----------------------------------------------------------------------------
* Description:    Handle CAN related doings for the applications
*-----------------------------------------------------------------------------
* $Date: 2011-05-13 09:39:35 +0200 (Fr, 13 Mai 2011) $
* $Rev: 19667 $
* $Author: maennel $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_CAN/trunk/inc/APP_CAN_CE.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _APP_CAN_CE_H
#define _APP_CAN_CE_H


/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Import configuration                                                       */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Constants                                                           */
/*----------------------------------------------------------------------------*/
#define GW3_MESSAGE_RECEIVED        mGateway_3_ind_b
#define GW3_KFZ_ALT_SPEED           b_GWB_Alt_FzgGeschw_b
#define GW3_LAENDERVARIANTE         b_GW3_Laendervariante_b
#define GW3_SPRACHVARIANTE          b_GW3_Sprachvariante_c
#define GW3_LAND_SPRACH_EMPF        b_GW3_Land_Sprach_empf_b

#define GWK_MESSAGE_RECEIVE         mGW_Kombi_ind_b
#define GWK_FZG_GESCHWIND_0         b_GWK_FzgGeschw_b_0
#define GWK_FZG_GESCHWIND_1         b_GWK_FzgGeschw_b_1
#define GWK_ALT_KOMBI               b_GWK_Alt_1_Kombi_b

#define SY1_MESSAGE_RECEIVE         mSysteminfo_1_ind_b
#define SY1_SLEPP_KOMFORT           b_SY1_Sleep_Komfort_b
#define SY1_DIAG_INFOTAINMENT       b_SY1_Diag_Infotainment_b
#define SY1_INFOTAINMENT            b_SY1_Infotainment_b
#define SY1_VERBAULISTE_GUELTIG     b_SY1_Verbauliste_gueltig_b	 
#define SY1_KLASSE                  b_SY1_Klasse_b
#define SY1_MARKE                   b_SY1_Marke_b
#define SY1_DERIVAT                 b_SY1_Derivat_b
#define SY1_GENERATION              b_SY1_Generation_b
#define SY1_ERWEITERUNG             b_SY1_Erweiterung_b
#define SY1_RECHTSLENKER            b_SY1_Rechtslenker_b
#define SY1_TRANSPORTMODE           b_SY1_Transportmode_b
#define SY1_NWDF_GUELTIG            b_SY1_NWDF_gueltig_b
#define SY1_NWDF_BIT                    b_SY1_NWDF_b

#define VBN_MESSAGE_RECEIVE         mSollverbau_neu_ind_b
#define VBN_SOLL_IST                b_VBN_Soll_Ist_OK_b	 
#define VBN_SMLS_K                  b_VBN_SMLS_K_b
#define VBN_TASTENMODUL             b_VBN_Tastenmodul_I_b
#define VBN_DSP_I                   b_VBN_DSP_I_b
#define VBN_TELMATIC_I              b_VBN_Telematik_I_b	 
#define VBN_NAVIGATION_I            b_VBN_Navigation_I_b	
#define VBN_RADIO_I                 b_VBN_Radio_I_b
#define VBN_TELEFON_I               b_VBN_Telefon_I_b	 


#define MFL_TASTEN_MESSAGE_RECEIVED mMFL_Tasten_ind_b
#define MLT_TICKZAEHLER             b_MLT_Tickzaehler_1_b
#define MLT_SYMBOL                  b_MLT_Symbol_c
#define MLT_VARIANTE                b_MLT_Variante_b
#define MLT_MARKE                   b_MLT_Marke_b

#define MFL_TASTEN_KON_1_MESSAGE_RECEIVED MFL_Tasten_Kon_01_ind_b
#define MFL_TASTENCODE_1            b_MFL_Tastencode_1_c	
#define MFL_TASTENCODE_2            b_MFL_Tastencode_2_c	
#define MFL_EVENTCODE_1             b_MFL_Eventcode_1_b
#define MFL_EVENTCODE_2             b_MFL_Eventcode_2_b
#define MFL_MARKE                   b_MFL_Marke_b
#define MFL_VARIANTE                b_MFL_Variante_b

#define BSG2_MESSAGE_RECEIVED       mBSG_2_ind_b
#define BSG2_BS2AUSINFO             b_BS2_aus_Infotainment_b
#define BSG2_BS2WARNINFO            b_BS2_Warn_Infotainment_b

#define BSG3_MESSAGE_RECEIVE        mBSG_3_ind_b
#define BSG3_KLEMME_15              b_BS3_Klemme_15_b
#define BSG3_KLEMME_S               b_BS3_Klemme_S_b

#define BSGKOMBI_MESSAGE_RECEIVE    mBSG_Kombi_ind_b
#define BSGKOMBI_REMOTESTART        b_BCM_Remotestart_Betrieb_b

#define LSM1_MESSAGE_RECEIVED       mLSM_1_ind_b
#define LSM1_MFA_VORHANDEN          b_LS1_MFA_vorhanden_b
#define LSM1_MFL_VORHANDEN          b_LS1_MFL_vorhanden_b
#define LSM1_MFA_TASTEN             b_LS1_MFA_Tasten_b
#define LSM1_MFL_TYP                b_LS1_MFL_Typ_b

#define RADIO4_MESSAGE_RECEIVE      mRadio_4_ind_b
#define RADIO4_ONOFF                b_RA4_Radio_on_b
#define RADIO4_MUTE                 b_RA4_Radio_mute_b


#define TF3_PHONE_ON                b_TF3_Phone_on_b
#define TF3_PHONE_INIT              b_TF3_Phone_Init_b
#define TF3_PHONE_MUTE              b_TF3_Phone_mute_b
#define TF3_MUTE_MSG_CODE           b_TF3_Mute_Msg_Code_b	
#define TF3_ACTIVE_CALL             b_TF3_Phone_active_call_b 
#define TF3_PHONE_RING              b_TF3_Phone_ring_b
#define TF3_PROVIDER(c)             b_TF3_Provider_b(c)
#define TF3_FELDSTAERKE             b_TF3_Feldstaerke_b
#define TF3_FELDSTAERKE_FEHLER      b_TF3_Feldstaerke_Fehler_b
#define TF3_REGISTRIERUNGSSTATUS    b_TF3_Registrierungsstatus_b
#define TF3_PHONE_ERROR_FLAG        b_TF3_Phone_Error_flag_b
#define TF3_BT_CONNECTIONSTATE      b_TF3_BT_ConnectionState_b


#define DIAGNOSE_MESSAGE_RECEIVED   mDiagnose_1_ind_b
#define DIAGNOSE_ALT_ZEIT           b_DN1_alt_Zeit_b
#define DIAGNOSE_ALT_KILOMETER      b_DN1_alt_Kilometerstand_b
#define DIAGNOSE_SEKUNDE_0          b_DN1_Sekunde_b_0
#define DIAGNOSE_SEKUNDE_1          b_DN1_Sekunde_b_1
#define DIAGNOSE_MINUTE             b_DN1_Minute_b
#define DIAGNOSE_STUNDEN_0          b_DN1_Stunde_b_0
#define DIAGNOSE_STUNDEN_1          b_DN1_Stunde_b_1
#define DIAGNOSE_TAG_0              b_DN1_Tag_b_0        
#define DIAGNOSE_TAG_1              b_DN1_Tag_b_1
#define DIAGNOSE_MONAT              b_DN1_Monat_b
#define DIAGNOSE_JAHR_0             b_DN1_Jahr_b_0
#define DIAGNOSE_JAHR_1             b_DN1_Jahr_b_1
#define DIAGNOSE_KM_STAND_0         b_DN1_KM_Stand_b_0
#define DIAGNOSE_KM_STAND_1         b_DN1_KM_Stand_b_1
#define DIAGNOSE_KM_STAND_2         b_DN1_KM_Stand_b_2
#define DIAGNOSE_VERLERNZAEHLER     b_DN1_Verlernzaehler_c

#endif /* _APP_CAN_CE_H */

/* End Of File APP_CAN_CE.h */
