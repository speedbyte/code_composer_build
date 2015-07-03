/**************************************************************
*  File C:\_workspace_\VW_UHV_NAR_trunk\MOD\MOD_CAN_DYN\src\UHV_NAR.c
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
#include "can_inc.h"

#include "UHV_NAR.h"


/* begin Fileversion check */
#if GEN_MAGIC_NUMBER_UHV_NAR !=1303376549
 #error "The magic number of the generated header uhv_nar.h is different! Check time and date of generated files!"
#endif

/* end of Fileversion check */


extern canuint32  nmSendMess[2];






V_MEMROM0 V_MEMROM1 canuint8 V_MEMROM2 kGenMainVersion   = (canuint8)(0xff & ((GEN_VERSION) >> 8));
V_MEMROM0 V_MEMROM1 canuint8 V_MEMROM2 kGenSubVersion    = (canuint8)(0xff & GEN_VERSION);
V_MEMROM0 V_MEMROM1 canuint8 V_MEMROM2 kGenBugfixVersion = (canuint8)(0xff & GEN_BUGFIX_VERSION);

/* start skeleton Texas TMS470  C-File ----------------------------------------*/
/******************************************************************************
| Version check
|*****************************************************************************/
#if( VTOOL_SKELETON_VERSION != 0x01 )
  #error "Header and source skeleton file in tool directory are inconsistent!"
#endif
#if( VTOOL_SKELETON_BUGFIX_VERSION != 0x00 )
  #error "Different versions of bugfix in Header and Source skeleton file used!"
#endif

#ifndef NULL
# define NULL	0
#endif
/* end skeleton Texas TMS470   C-File ----------------------------------------*/



/*************************************************************/
/* Databuffer for send objects                               */
/*************************************************************/
_c_ISO_Telefon_Resp_buf MEMORY_FAR  ISO_Telefon_Resp;
_c_mASR5_String3_buf MEMORY_FAR  mASR5_String3;
_c_mASR4_String2_buf MEMORY_FAR  mASR4_String2;
_c_mASR3_String1_buf MEMORY_FAR  mASR3_String1;
_c_mASR2_String0_buf MEMORY_FAR  mASR2_String0;
_c_mASR1_TypeInfo_buf MEMORY_FAR  mASR1_TypeInfo;
_c_mASR0_Status_buf MEMORY_FAR  mASR0_Status;
_c_BAP_Debug_Data_buf MEMORY_FAR  BAP_Debug_Data;
_c_BAP_Telefon_02_buf MEMORY_FAR  BAP_Telefon_02;
_c_BAP_Telefon_01_buf MEMORY_FAR  BAP_Telefon_01;
_c_BAP_Telefon_04_buf MEMORY_FAR  BAP_Telefon_04;
_c_mTelefon_3_buf MEMORY_FAR  mTelefon_3;
_c_BAP_Telefon_03_buf MEMORY_FAR  BAP_Telefon_03;
_c_BAP_MDI_02_buf MEMORY_FAR  BAP_MDI_02;


/*************************************************************/
/* Databuffer for receive objects                            */
/*************************************************************/
_c_mASR7_Param_Feedback_buf MEMORY_FAR  mASR7_Param_Feedback;
_c_mASR6_Param_Request_buf MEMORY_FAR  mASR6_Param_Request;
_c_BAP_Debug_Cmd_buf MEMORY_FAR  BAP_Debug_Cmd;
_c_BAP_ASG_03_buf MEMORY_FAR  BAP_ASG_03;
_c_BAP_ASG_06_buf MEMORY_FAR  BAP_ASG_06;
_c_BAP_ASG_07_buf MEMORY_FAR  BAP_ASG_07;
_c_mRadio_4_buf MEMORY_FAR  mRadio_4;
_c_mDiagnose_1_buf MEMORY_FAR  mDiagnose_1;
_c_mSollverbau_neu_buf MEMORY_FAR  mSollverbau_neu;
_c_mGateway_3_buf MEMORY_FAR  mGateway_3;
_c_mSysteminfo_1_buf MEMORY_FAR  mSysteminfo_1;
_c_mMFL_Tasten_buf MEMORY_FAR  mMFL_Tasten;
_c_MFL_Tasten_Kon_01_buf MEMORY_FAR  MFL_Tasten_Kon_01;
_c_mBSG_3_buf MEMORY_FAR  mBSG_3;
_c_mBSG_2_buf MEMORY_FAR  mBSG_2;
_c_mGW_Kombi_buf MEMORY_FAR  mGW_Kombi;
_c_mBSG_Kombi_buf MEMORY_FAR  mBSG_Kombi;
_c_mLSM_1_buf MEMORY_FAR  mLSM_1;
_c_mAirbag_1_buf MEMORY_FAR  mAirbag_1;


/*************************************************************/
/* Send structures                                           */
/*************************************************************/
V_MEMROM0 V_MEMROM1 tCanTxId0 V_MEMROM2 CanTxId0[kCanNumberOfTxObjects] = {
	 MK_STDID0(0x7d5)		/* id: 0x7D5, Name: "ISO_Telefon_Resp", Handle: 0, [BC] */
	,MK_STDID0(0x7a5)		/* id: 0x7A5, Name: "mASR5_String3", Handle: 1, [BC] */
	,MK_STDID0(0x7a4)		/* id: 0x7A4, Name: "mASR4_String2", Handle: 2, [BC] */
	,MK_STDID0(0x7a3)		/* id: 0x7A3, Name: "mASR3_String1", Handle: 3, [BC] */
	,MK_STDID0(0x7a2)		/* id: 0x7A2, Name: "mASR2_String0", Handle: 4, [BC] */
	,MK_STDID0(0x7a1)		/* id: 0x7A1, Name: "mASR1_TypeInfo", Handle: 5, [BC] */
	,MK_STDID0(0x7a0)		/* id: 0x7A0, Name: "mASR0_Status", Handle: 6, [BC] */
	,MK_STDID0(0x6d7)		/* id: 0x6D7, Name: "BAP_Debug_Data", Handle: 7, [BC] */
	,MK_STDID0(0x6d5)		/* id: 0x6D5, Name: "BAP_Telefon_02", Handle: 8, [BC] */
	,MK_STDID0(0x6d2)		/* id: 0x6D2, Name: "BAP_Telefon_01", Handle: 9, [BC] */
	,MK_STDID0(0x66f)		/* id: 0x66F, Name: "BAP_Telefon_04", Handle: 10, [BC] */
	,MK_STDID0(0x665)		/* id: 0x665, Name: "mTelefon_3", Handle: 11, [BC] */
	,MK_STDID0(0x63b)		/* id: 0x63B, Name: "BAP_Telefon_03", Handle: 12, [BC] */
	,MK_STDID0(0x62c)		/* id: 0x62C, Name: "BAP_MDI_02", Handle: 13, [BC] */
	,MK_STDID0(0x43a)		/* id: 0x43A, Name: "NWM_Telefon", Handle: 14, [FC] */
};

V_MEMROM0 V_MEMROM1 canuint8 V_MEMROM2 CanTxDLC[kCanNumberOfTxObjects] = {
	 MK_TX_DLC(8)		/* id: 0x7D5, Name: "ISO_Telefon_Resp", Handle: 0, [BC] */
	,MK_TX_DLC(8)		/* id: 0x7A5, Name: "mASR5_String3", Handle: 1, [BC] */
	,MK_TX_DLC(8)		/* id: 0x7A4, Name: "mASR4_String2", Handle: 2, [BC] */
	,MK_TX_DLC(8)		/* id: 0x7A3, Name: "mASR3_String1", Handle: 3, [BC] */
	,MK_TX_DLC(8)		/* id: 0x7A2, Name: "mASR2_String0", Handle: 4, [BC] */
	,MK_TX_DLC(8)		/* id: 0x7A1, Name: "mASR1_TypeInfo", Handle: 5, [BC] */
	,MK_TX_DLC(4)		/* id: 0x7A0, Name: "mASR0_Status", Handle: 6, [BC] */
	,MK_TX_DLC(7)		/* id: 0x6D7, Name: "BAP_Debug_Data", Handle: 7, [BC] */
	,MK_TX_DLC(8)		/* id: 0x6D5, Name: "BAP_Telefon_02", Handle: 8, [BC] */
	,MK_TX_DLC(8)		/* id: 0x6D2, Name: "BAP_Telefon_01", Handle: 9, [BC] */
	,MK_TX_DLC(8)		/* id: 0x66F, Name: "BAP_Telefon_04", Handle: 10, [BC] */
	,MK_TX_DLC(7)		/* id: 0x665, Name: "mTelefon_3", Handle: 11, [BC] */
	,MK_TX_DLC(8)		/* id: 0x63B, Name: "BAP_Telefon_03", Handle: 12, [BC] */
	,MK_TX_DLC(8)		/* id: 0x62C, Name: "BAP_MDI_02", Handle: 13, [BC] */
	,MK_TX_DLC(8)		/* id: 0x43A, Name: "NWM_Telefon", Handle: 14, [FC] */
};

V_MEMROM0 V_MEMROM1 TxDataPtr V_MEMROM2 CanTxDataPtr[kCanNumberOfTxObjects] = {
	 (TxDataPtr)ISO_Telefon_Resp._c		/* id: 0x7D5, Name: "ISO_Telefon_Resp", Handle: 0, [BC] */
	,(TxDataPtr)mASR5_String3._c		/* id: 0x7A5, Name: "mASR5_String3", Handle: 1, [BC] */
	,(TxDataPtr)mASR4_String2._c		/* id: 0x7A4, Name: "mASR4_String2", Handle: 2, [BC] */
	,(TxDataPtr)mASR3_String1._c		/* id: 0x7A3, Name: "mASR3_String1", Handle: 3, [BC] */
	,(TxDataPtr)mASR2_String0._c		/* id: 0x7A2, Name: "mASR2_String0", Handle: 4, [BC] */
	,(TxDataPtr)mASR1_TypeInfo._c		/* id: 0x7A1, Name: "mASR1_TypeInfo", Handle: 5, [BC] */
	,(TxDataPtr)mASR0_Status._c		/* id: 0x7A0, Name: "mASR0_Status", Handle: 6, [BC] */
	,(TxDataPtr)BAP_Debug_Data._c		/* id: 0x6D7, Name: "BAP_Debug_Data", Handle: 7, [BC] */
	,(TxDataPtr)BAP_Telefon_02._c		/* id: 0x6D5, Name: "BAP_Telefon_02", Handle: 8, [BC] */
	,(TxDataPtr)BAP_Telefon_01._c		/* id: 0x6D2, Name: "BAP_Telefon_01", Handle: 9, [BC] */
	,(TxDataPtr)BAP_Telefon_04._c		/* id: 0x66F, Name: "BAP_Telefon_04", Handle: 10, [BC] */
	,(TxDataPtr)mTelefon_3._c		/* id: 0x665, Name: "mTelefon_3", Handle: 11, [BC] */
	,(TxDataPtr)BAP_Telefon_03._c		/* id: 0x63B, Name: "BAP_Telefon_03", Handle: 12, [BC] */
	,(TxDataPtr)BAP_MDI_02._c		/* id: 0x62C, Name: "BAP_MDI_02", Handle: 13, [BC] */
	,(TxDataPtr)nmSendMess		/* id: 0x43A, Name: "NWM_Telefon", Handle: 14, [FC] */
};

#ifdef C_ENABLE_CONFIRMATION_FCT

V_MEMROM0 V_MEMROM1 ApplConfirmationFct V_MEMROM2 CanTxApplConfirmationPtr[kCanNumberOfTxObjects] = {
	 I2V_vCanDataCon		/* id: 0x7D5, Name: "ISO_Telefon_Resp", Handle: 0, [BC] */
	,NULL					/* id: 0x7A5, Name: "mASR5_String3", Handle: 1, [BC] */
	,NULL					/* id: 0x7A4, Name: "mASR4_String2", Handle: 2, [BC] */
	,NULL					/* id: 0x7A3, Name: "mASR3_String1", Handle: 3, [BC] */
	,NULL					/* id: 0x7A2, Name: "mASR2_String0", Handle: 4, [BC] */
	,NULL					/* id: 0x7A1, Name: "mASR1_TypeInfo", Handle: 5, [BC] */
	,NULL					/* id: 0x7A0, Name: "mASR0_Status", Handle: 6, [BC] */
	,NULL					/* id: 0x6D7, Name: "BAP_Debug_Data", Handle: 7, [BC] */
	,NULL					/* id: 0x6D5, Name: "BAP_Telefon_02", Handle: 8, [BC] */
	,NULL					/* id: 0x6D2, Name: "BAP_Telefon_01", Handle: 9, [BC] */
	,NULL					/* id: 0x66F, Name: "BAP_Telefon_04", Handle: 10, [BC] */
	,NULL					/* id: 0x665, Name: "mTelefon_3", Handle: 11, [BC] */
	,NULL					/* id: 0x63B, Name: "BAP_Telefon_03", Handle: 12, [BC] */
	,NULL					/* id: 0x62C, Name: "BAP_MDI_02", Handle: 13, [BC] */
	,NmConfirmation		/* id: 0x43A, Name: "NWM_Telefon", Handle: 14, [FC] */
};
#endif


#ifdef C_ENABLE_PRETRANSMIT_FCT

V_MEMROM0 V_MEMROM1 ApplPreTransmitFct V_MEMROM2 CanTxApplPreTransmitPtr[kCanNumberOfTxObjects] = {
	 NULL					/* id: 0x7D5, Name: "ISO_Telefon_Resp", Handle: 0, [BC] */
	,NULL					/* id: 0x7A5, Name: "mASR5_String3", Handle: 1, [BC] */
	,NULL					/* id: 0x7A4, Name: "mASR4_String2", Handle: 2, [BC] */
	,NULL					/* id: 0x7A3, Name: "mASR3_String1", Handle: 3, [BC] */
	,NULL					/* id: 0x7A2, Name: "mASR2_String0", Handle: 4, [BC] */
	,NULL					/* id: 0x7A1, Name: "mASR1_TypeInfo", Handle: 5, [BC] */
	,NULL					/* id: 0x7A0, Name: "mASR0_Status", Handle: 6, [BC] */
	,NULL					/* id: 0x6D7, Name: "BAP_Debug_Data", Handle: 7, [BC] */
	,NULL					/* id: 0x6D5, Name: "BAP_Telefon_02", Handle: 8, [BC] */
	,NULL					/* id: 0x6D2, Name: "BAP_Telefon_01", Handle: 9, [BC] */
	,NULL					/* id: 0x66F, Name: "BAP_Telefon_04", Handle: 10, [BC] */
	,NULL					/* id: 0x665, Name: "mTelefon_3", Handle: 11, [BC] */
	,NULL					/* id: 0x63B, Name: "BAP_Telefon_03", Handle: 12, [BC] */
	,NULL					/* id: 0x62C, Name: "BAP_MDI_02", Handle: 13, [BC] */
	,NULL					/* id: 0x43A, Name: "NWM_Telefon", Handle: 14, [FC] */
};
#endif


#if defined (C_ENABLE_PART_OFFLINE)
V_MEMROM0 V_MEMROM1 canuint8 V_MEMROM2 CanTxSendMask[kCanNumberOfTxObjects] = {
	 C_SEND_GRP_DIAG		/* id: 0x7D5, Name: "ISO_Telefon_Resp", Handle: 0, [BC] */
	,C_SEND_GRP_USER1		/* id: 0x7A5, Name: "mASR5_String3", Handle: 1, [BC] */
	,C_SEND_GRP_USER1		/* id: 0x7A4, Name: "mASR4_String2", Handle: 2, [BC] */
	,C_SEND_GRP_USER1		/* id: 0x7A3, Name: "mASR3_String1", Handle: 3, [BC] */
	,C_SEND_GRP_USER1		/* id: 0x7A2, Name: "mASR2_String0", Handle: 4, [BC] */
	,C_SEND_GRP_USER1		/* id: 0x7A1, Name: "mASR1_TypeInfo", Handle: 5, [BC] */
	,C_SEND_GRP_USER1		/* id: 0x7A0, Name: "mASR0_Status", Handle: 6, [BC] */
	,C_SEND_GRP_USER1		/* id: 0x6D7, Name: "BAP_Debug_Data", Handle: 7, [BC] */
	,C_SEND_GRP_USER1		/* id: 0x6D5, Name: "BAP_Telefon_02", Handle: 8, [BC] */
	,C_SEND_GRP_USER1		/* id: 0x6D2, Name: "BAP_Telefon_01", Handle: 9, [BC] */
	,C_SEND_GRP_USER1		/* id: 0x66F, Name: "BAP_Telefon_04", Handle: 10, [BC] */
	,C_SEND_GRP_USER1		/* id: 0x665, Name: "mTelefon_3", Handle: 11, [BC] */
	,C_SEND_GRP_USER1		/* id: 0x63B, Name: "BAP_Telefon_03", Handle: 12, [BC] */
	,C_SEND_GRP_USER1		/* id: 0x62C, Name: "BAP_MDI_02", Handle: 13, [BC] */
	,C_SEND_GRP_NM		/* id: 0x43A, Name: "NWM_Telefon", Handle: 14, [FC] */
};
#endif



/*************************************************************/
/* Databuffer for confirmationflags                          */
/*************************************************************/

#ifdef C_ENABLE_CONFIRMATION_FLAG
V_MEMROM0 V_MEMROM1 canuint8 V_MEMROM2 CanConfirmationOffset[kCanNumberOfTxObjects] = {
	   0	,  0	,  0	,  0	,  0	,  0	,  0	,  0
	,  0	,  1	,  1	,  1	,  1	,  1	,  0
};

V_MEMROM0 V_MEMROM1 canuint8 V_MEMROM2 CanConfirmationMask[kCanNumberOfTxObjects] = {
	 0x00	,0x01	,0x02	,0x04	,0x08	,0x10	,0x20	,0x40
	,0x80	,0x01	,0x02	,0x04	,0x08	,0x10	,0x00
};
#endif


/*************************************************************/
/* Receive structures                                        */
/*************************************************************/
V_MEMROM0 V_MEMROM1 tCanRxId0 V_MEMROM2 CanRxId0[kCanNumberOfRxObjects] = {
	 MK_STDID0(0x7a7)		/* id: 0x7A7, Name: "mASR7_Param_Feedback", Handle: 0, [FC] */
	,MK_STDID0(0x7a6)		/* id: 0x7A6, Name: "mASR6_Param_Request", Handle: 1, [FC] */
	,MK_STDID0(0x76b)		/* id: 0x76B, Name: "ISO_Telefon_Req", Handle: 2, [FC] */
	,MK_STDID0(0x700)		/* id: 0x700, Name: "ISO_Funktionaler_Req_All", Handle: 3, [FC] */
	,MK_STDID0(0x6d6)		/* id: 0x6D6, Name: "BAP_Debug_Cmd", Handle: 4, [FC] */
	,MK_STDID0(0x6c7)		/* id: 0x6C7, Name: "BAP_ASG_03", Handle: 5, [FC] */
	,MK_STDID0(0x6c6)		/* id: 0x6C6, Name: "BAP_ASG_06", Handle: 6, [FC] */
	,MK_STDID0(0x67c)		/* id: 0x67C, Name: "BAP_ASG_07", Handle: 7, [FC] */
	,MK_STDID0(0x661)		/* id: 0x661, Name: "mRadio_4", Handle: 8, [FC] */
	,MK_STDID0(0x65d)		/* id: 0x65D, Name: "mDiagnose_1", Handle: 9, [FC] */
	,MK_STDID0(0x655)		/* id: 0x655, Name: "mSollverbau_neu", Handle: 10, [FC] */
	,MK_STDID0(0x653)		/* id: 0x653, Name: "mGateway_3", Handle: 11, [FC] */
	,MK_STDID0(0x651)		/* id: 0x651, Name: "mSysteminfo_1", Handle: 12, [FC] */
	,MK_STDID0(0x5c1)		/* id: 0x5C1, Name: "mMFL_Tasten", Handle: 13, [FC] */
	,MK_STDID0(0x5bf)		/* id: 0x5BF, Name: "MFL_Tasten_Kon_01", Handle: 14, [FC] */
	,MK_STDID0(0x575)		/* id: 0x575, Name: "mBSG_3", Handle: 15, [FC] */
	,MK_STDID0(0x571)		/* id: 0x571, Name: "mBSG_2", Handle: 16, [FC] */
	,MK_STDID0(0x527)		/* id: 0x527, Name: "mGW_Kombi", Handle: 17, [FC] */
	,MK_STDID0(0x470)		/* id: 0x470, Name: "mBSG_Kombi", Handle: 18, [FC] */
	,MK_STDID0(0x2c1)		/* id: 0x2C1, Name: "mLSM_1", Handle: 19, [FC] */
	,MK_STDID0(0x151)		/* id: 0x151, Name: "mAirbag_1", Handle: 20, [FC] */
};

V_MEMROM0 V_MEMROM1 canuint8 V_MEMROM2 CanRxDataLen[kCanNumberOfRxObjects] = {
	 8		/* id: 0x7A7, Name: "mASR7_Param_Feedback", Handle: 0, [FC] */
	,8		/* id: 0x7A6, Name: "mASR6_Param_Request", Handle: 1, [FC] */
	,0		/* id: 0x76B, Name: "ISO_Telefon_Req", Handle: 2, [FC] */
	,0		/* id: 0x700, Name: "ISO_Funktionaler_Req_All", Handle: 3, [FC] */
	,2		/* id: 0x6D6, Name: "BAP_Debug_Cmd", Handle: 4, [FC] */
	,2		/* id: 0x6C7, Name: "BAP_ASG_03", Handle: 5, [FC] */
	,2		/* id: 0x6C6, Name: "BAP_ASG_06", Handle: 6, [FC] */
	,2		/* id: 0x67C, Name: "BAP_ASG_07", Handle: 7, [FC] */
	,1		/* id: 0x661, Name: "mRadio_4", Handle: 8, [FC] */
	,8		/* id: 0x65D, Name: "mDiagnose_1", Handle: 9, [FC] */
	,8		/* id: 0x655, Name: "mSollverbau_neu", Handle: 10, [FC] */
	,2		/* id: 0x653, Name: "mGateway_3", Handle: 11, [FC] */
	,7		/* id: 0x651, Name: "mSysteminfo_1", Handle: 12, [FC] */
	,4		/* id: 0x5C1, Name: "mMFL_Tasten", Handle: 13, [FC] */
	,4		/* id: 0x5BF, Name: "MFL_Tasten_Kon_01", Handle: 14, [FC] */
	,1		/* id: 0x575, Name: "mBSG_3", Handle: 15, [FC] */
	,6		/* id: 0x571, Name: "mBSG_2", Handle: 16, [FC] */
	,3		/* id: 0x527, Name: "mGW_Kombi", Handle: 17, [FC] */
	,7		/* id: 0x470, Name: "mBSG_Kombi", Handle: 18, [FC] */
	,5		/* id: 0x2C1, Name: "mLSM_1", Handle: 19, [FC] */
	,1		/* id: 0x151, Name: "mAirbag_1", Handle: 20, [FC] */
};

V_MEMROM0 V_MEMROM1 RxDataPtr V_MEMROM2 CanRxDataPtr[kCanNumberOfRxObjects] = {
	 (RxDataPtr)mASR7_Param_Feedback._c		/* id: 0x7A7, Name: "mASR7_Param_Feedback", Handle: 0, [FC] */
	,(RxDataPtr)mASR6_Param_Request._c		/* id: 0x7A6, Name: "mASR6_Param_Request", Handle: 1, [FC] */
	,(RxDataPtr)0			/* id: 0x76B, Name: "ISO_Telefon_Req", Handle: 2, [FC] */
	,(RxDataPtr)0			/* id: 0x700, Name: "ISO_Funktionaler_Req_All", Handle: 3, [FC] */
	,(RxDataPtr)BAP_Debug_Cmd._c		/* id: 0x6D6, Name: "BAP_Debug_Cmd", Handle: 4, [FC] */
	,(RxDataPtr)BAP_ASG_03._c		/* id: 0x6C7, Name: "BAP_ASG_03", Handle: 5, [FC] */
	,(RxDataPtr)BAP_ASG_06._c		/* id: 0x6C6, Name: "BAP_ASG_06", Handle: 6, [FC] */
	,(RxDataPtr)BAP_ASG_07._c		/* id: 0x67C, Name: "BAP_ASG_07", Handle: 7, [FC] */
	,(RxDataPtr)mRadio_4._c		/* id: 0x661, Name: "mRadio_4", Handle: 8, [FC] */
	,(RxDataPtr)mDiagnose_1._c		/* id: 0x65D, Name: "mDiagnose_1", Handle: 9, [FC] */
	,(RxDataPtr)mSollverbau_neu._c		/* id: 0x655, Name: "mSollverbau_neu", Handle: 10, [FC] */
	,(RxDataPtr)mGateway_3._c		/* id: 0x653, Name: "mGateway_3", Handle: 11, [FC] */
	,(RxDataPtr)mSysteminfo_1._c		/* id: 0x651, Name: "mSysteminfo_1", Handle: 12, [FC] */
	,(RxDataPtr)mMFL_Tasten._c		/* id: 0x5C1, Name: "mMFL_Tasten", Handle: 13, [FC] */
	,(RxDataPtr)MFL_Tasten_Kon_01._c		/* id: 0x5BF, Name: "MFL_Tasten_Kon_01", Handle: 14, [FC] */
	,(RxDataPtr)mBSG_3._c		/* id: 0x575, Name: "mBSG_3", Handle: 15, [FC] */
	,(RxDataPtr)mBSG_2._c		/* id: 0x571, Name: "mBSG_2", Handle: 16, [FC] */
	,(RxDataPtr)mGW_Kombi._c		/* id: 0x527, Name: "mGW_Kombi", Handle: 17, [FC] */
	,(RxDataPtr)mBSG_Kombi._c		/* id: 0x470, Name: "mBSG_Kombi", Handle: 18, [FC] */
	,(RxDataPtr)mLSM_1._c		/* id: 0x2C1, Name: "mLSM_1", Handle: 19, [FC] */
	,(RxDataPtr)mAirbag_1._c		/* id: 0x151, Name: "mAirbag_1", Handle: 20, [FC] */
};

#ifdef C_ENABLE_PRECOPY_FCT

V_MEMROM0 V_MEMROM1 ApplPrecopyFct V_MEMROM2 CanRxApplPrecopyPtr[kCanNumberOfRxObjects] = {
	 NULL					/* id: 0x7A7, Name: "mASR7_Param_Feedback", Handle: 0, [FC] */
	,NULL					/* id: 0x7A6, Name: "mASR6_Param_Request", Handle: 1, [FC] */
	,I2V_ubCanDataPrecopy	/* id: 0x76B, Name: "ISO_Telefon_Req", Handle: 2, [FC] */
	,I2V_ubCanDataPrecopy	/* id: 0x700, Name: "ISO_Funktionaler_Req_All", Handle: 3, [FC] */
	,NULL					/* id: 0x6D6, Name: "BAP_Debug_Cmd", Handle: 4, [FC] */
	,BAP_BCL_CanRxPreCopy		/* id: 0x6C7, Name: "BAP_ASG_03", Handle: 5, [FC] */
	,BAP_BCL_CanRxPreCopy		/* id: 0x6C6, Name: "BAP_ASG_06", Handle: 6, [FC] */
	,BAP_BCL_CanRxPreCopy		/* id: 0x67C, Name: "BAP_ASG_07", Handle: 7, [FC] */
	,NULL					/* id: 0x661, Name: "mRadio_4", Handle: 8, [FC] */
	,NULL					/* id: 0x65D, Name: "mDiagnose_1", Handle: 9, [FC] */
	,NULL					/* id: 0x655, Name: "mSollverbau_neu", Handle: 10, [FC] */
	,NULL					/* id: 0x653, Name: "mGateway_3", Handle: 11, [FC] */
	,NULL					/* id: 0x651, Name: "mSysteminfo_1", Handle: 12, [FC] */
	,NULL					/* id: 0x5C1, Name: "mMFL_Tasten", Handle: 13, [FC] */
	,NULL					/* id: 0x5BF, Name: "MFL_Tasten_Kon_01", Handle: 14, [FC] */
	,NULL					/* id: 0x575, Name: "mBSG_3", Handle: 15, [FC] */
	,NULL					/* id: 0x571, Name: "mBSG_2", Handle: 16, [FC] */
	,NULL					/* id: 0x527, Name: "mGW_Kombi", Handle: 17, [FC] */
	,NULL					/* id: 0x470, Name: "mBSG_Kombi", Handle: 18, [FC] */
	,NULL					/* id: 0x2C1, Name: "mLSM_1", Handle: 19, [FC] */
	,NULL					/* id: 0x151, Name: "mAirbag_1", Handle: 20, [FC] */
};
#endif


#ifdef C_ENABLE_INDICATION_FCT

V_MEMROM0 V_MEMROM1 ApplIndicationFct V_MEMROM2 CanRxApplIndicationPtr[kCanNumberOfRxObjects] = {
	 NULL					/* id: 0x7A7, Name: "mASR7_Param_Feedback", Handle: 0, [FC] */
	,NULL					/* id: 0x7A6, Name: "mASR6_Param_Request", Handle: 1, [FC] */
	,NULL					/* id: 0x76B, Name: "ISO_Telefon_Req", Handle: 2, [FC] */
	,NULL					/* id: 0x700, Name: "ISO_Funktionaler_Req_All", Handle: 3, [FC] */
	,App_BapDiagnoseSwitch		/* id: 0x6D6, Name: "BAP_Debug_Cmd", Handle: 4, [FC] */
	,NULL					/* id: 0x6C7, Name: "BAP_ASG_03", Handle: 5, [FC] */
	,NULL					/* id: 0x6C6, Name: "BAP_ASG_06", Handle: 6, [FC] */
	,NULL					/* id: 0x67C, Name: "BAP_ASG_07", Handle: 7, [FC] */
	,NULL					/* id: 0x661, Name: "mRadio_4", Handle: 8, [FC] */
	,NULL					/* id: 0x65D, Name: "mDiagnose_1", Handle: 9, [FC] */
	,NULL					/* id: 0x655, Name: "mSollverbau_neu", Handle: 10, [FC] */
	,NULL					/* id: 0x653, Name: "mGateway_3", Handle: 11, [FC] */
	,NULL					/* id: 0x651, Name: "mSysteminfo_1", Handle: 12, [FC] */
	,NULL					/* id: 0x5C1, Name: "mMFL_Tasten", Handle: 13, [FC] */
	,NULL					/* id: 0x5BF, Name: "MFL_Tasten_Kon_01", Handle: 14, [FC] */
	,NULL					/* id: 0x575, Name: "mBSG_3", Handle: 15, [FC] */
	,NULL					/* id: 0x571, Name: "mBSG_2", Handle: 16, [FC] */
	,NULL					/* id: 0x527, Name: "mGW_Kombi", Handle: 17, [FC] */
	,NULL					/* id: 0x470, Name: "mBSG_Kombi", Handle: 18, [FC] */
	,NULL					/* id: 0x2C1, Name: "mLSM_1", Handle: 19, [FC] */
	,NULL					/* id: 0x151, Name: "mAirbag_1", Handle: 20, [FC] */
};
#endif



/*************************************************************/
/* Databuffer for indicationflags                                */
/*************************************************************/
V_MEMROM0 V_MEMROM1 canuint8 V_MEMROM2 CanIndicationOffset[kCanNumberOfRxObjects] = {
	   0	,  0	,  0	,  0	,  0	,  0	,  0	,  0
	,  0	,  0	,  0	,  0	,  0	,  0	,  1	,  1
	,  1	,  1	,  1	,  1	,  1
};

V_MEMROM0 V_MEMROM1 canuint8 V_MEMROM2 CanIndicationMask[kCanNumberOfRxObjects] = {
	 0x01	,0x02	,0x00	,0x00	,0x00	,0x00	,0x00	,0x00
	,0x04	,0x08	,0x10	,0x20	,0x40	,0x80	,0x01	,0x02
	,0x04	,0x08	,0x10	,0x20	,0x40
};


/* CAN buffer configuration */

#ifdef C_ENABLE_TX_FULLCAN_OBJECTS
V_MEMROM0 V_MEMROM1 canuint8 V_MEMROM2 CanTxHwObj[kCanNumberOfTxObjects] = {
	 0x1e		/* id: 0x7D5, Name: "ISO_Telefon_Resp", Handle: 0, [BC] */
	,0x1e		/* id: 0x7A5, Name: "mASR5_String3", Handle: 1, [BC] */
	,0x1e		/* id: 0x7A4, Name: "mASR4_String2", Handle: 2, [BC] */
	,0x1e		/* id: 0x7A3, Name: "mASR3_String1", Handle: 3, [BC] */
	,0x1e		/* id: 0x7A2, Name: "mASR2_String0", Handle: 4, [BC] */
	,0x1e		/* id: 0x7A1, Name: "mASR1_TypeInfo", Handle: 5, [BC] */
	,0x1e		/* id: 0x7A0, Name: "mASR0_Status", Handle: 6, [BC] */
	,0x1e		/* id: 0x6D7, Name: "BAP_Debug_Data", Handle: 7, [BC] */
	,0x1e		/* id: 0x6D5, Name: "BAP_Telefon_02", Handle: 8, [BC] */
	,0x1e		/* id: 0x6D2, Name: "BAP_Telefon_01", Handle: 9, [BC] */
	,0x1e		/* id: 0x66F, Name: "BAP_Telefon_04", Handle: 10, [BC] */
	,0x1e		/* id: 0x665, Name: "mTelefon_3", Handle: 11, [BC] */
	,0x1e		/* id: 0x63B, Name: "BAP_Telefon_03", Handle: 12, [BC] */
	,0x1e		/* id: 0x62C, Name: "BAP_MDI_02", Handle: 13, [BC] */
	,0x1f		/* id: 0x43A, Name: "NWM_Telefon", Handle: 14, [FC] */
};
#endif

/*************************************************************/
/*************************************************************/

#if defined ( C_MULTIPLE_RECEIVE_CHANNEL )
V_MEMROM0 V_MEMROM1 canuint8 V_MEMROM2 CanInitObjectStartIndex[2] = {
	 0
	,1
};
#endif

V_MEMROM0 V_MEMROM1 unsigned long V_MEMROM2 CanInitBTC[kCanNumberOfInitObjects] = { 0x00090252}; 
V_MEMROM0 V_MEMROM1 unsigned long V_MEMROM2 CanInitLAM0[kCanNumberOfInitObjects] = { 0xfc0000}; 
V_MEMROM0 V_MEMROM1 unsigned long V_MEMROM2 CanInitMID1[kCanNumberOfInitObjects] = { 0x10000000}; 

#if defined(C_ENABLE_TX_DIRECT_OBJECTS)
V_MEMROM0 V_MEMROM1 canuint8 V_MEMROM2 CanTxDirectTransmitSupport[kCanNumberOfUsedTxCANObjects] = {
	kCanTxNoDirectObj	/*Standard Tx Object from Channel: 0*/
	,kCanTxNoDirectObj	/*ID: 1082 Name: NWM_Telefon*/
};
#endif



