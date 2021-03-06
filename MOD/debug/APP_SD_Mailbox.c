/******************************************************************************/
/*****************  Copyright (c) 2010 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     APP_SD_Mailbox.c
*-----------------------------------------------------------------------------
* Module Name:
*-----------------------------------------------------------------------------
* Description: !!! THIS IS A GENERATED FILE. PLEASE DO NOT CHANGE IT MANUALLY. !!!
*-----------------------------------------------------------------------------
* $Date: 2008-12-23 13:32:44 +0100 (Di, 23 Dez 2008) $
* $Rev: 3480 $
* $Author: agrawal $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_SD/trunk/src/APP_SD_FrameDataBase.c $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include "common.h"
#include <osek.h>           // vector

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "APP_SD_Mailbox.h"

/*----------------------------------------------------------------------------*/
/* pragma Directives                                                          */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Definitions (avoid as much as possible!)                  */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private Manifest Constant Definitions                                      */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private Types                                                              */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the type
*/

/*----------------------------------------------------------------------------*/
/* Prototypes of file local Methods                                           */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the method
*/

/*----------------------------------------------------------------------------*/
/* File local Code Macros                                                        */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the macros
*/

/*----------------------------------------------------------------------------*/
/* File local Variables                                                       */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the private variable
*/

/*----------------------------------------------------------------------------*/
/* File local const Definitions                                               */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the private constant
*/


/* Voicemailbox */
const Uint8Type APP_SD_au8English_ta_find_mailbox_1[0xc] = {
0x56, 0x6f, 0x69, 0x63, 0x65, 0x6d, 0x61, 0x69, 0x6c, 0x62, 0x6f, 0x78
};
/* mailbox */
const Uint8Type APP_SD_au8English_ta_find_mailbox_2[0x7] = {
0x6d, 0x61, 0x69, 0x6c, 0x62, 0x6f, 0x78
};
/* voice messages */
const Uint8Type APP_SD_au8English_ta_find_mailbox_3[0xe] = {
0x76, 0x6f, 0x69, 0x63, 0x65, 0x20, 0x6d, 0x65, 0x73, 0x73, 0x61, 0x67, 0x65, 0x73
};
/* voicemailbox */
const Uint8Type APP_SD_au8English_ta_find_mailbox_4[0xc] = {
0x76, 0x6f, 0x69, 0x63, 0x65, 0x6d, 0x61, 0x69, 0x6c, 0x62, 0x6f, 0x78
};

/* Sprachmailbox */
const Uint8Type APP_SD_au8German_ta_find_mailbox_1[0xd] = {
0x53, 0x70, 0x72, 0x61, 0x63, 0x68, 0x6d, 0x61, 0x69, 0x6c, 0x62, 0x6f, 0x78
};
/* Mailbox */
const Uint8Type APP_SD_au8German_ta_find_mailbox_2[0x7] = {
0x4d, 0x61, 0x69, 0x6c, 0x62, 0x6f, 0x78
};
/* Sprachnachrichten */
const Uint8Type APP_SD_au8German_ta_find_mailbox_3[0x11] = {
0x53, 0x70, 0x72, 0x61, 0x63, 0x68, 0x6e, 0x61, 0x63, 0x68, 0x72, 0x69, 0x63, 0x68, 0x74, 0x65, 0x6e
};
/* Anrufbeantworter */
const Uint8Type APP_SD_au8German_ta_find_mailbox_4[0x10] = {
0x41, 0x6e, 0x72, 0x75, 0x66, 0x62, 0x65, 0x61, 0x6e, 0x74, 0x77, 0x6f, 0x72, 0x74, 0x65, 0x72
};

/* Hlasov� schr�nka */
const Uint8Type APP_SD_au8Czech_ta_find_mailbox_1[0x12] = {
0x48, 0x6c, 0x61, 0x73, 0x6f, 0x76, 0xc3, 0xa1, 0x20, 0x73, 0x63, 0x68, 0x72, 0xc3, 0xa1, 0x6e, 0x6b, 0x61
};
/* Hlasova schranka */
const Uint8Type APP_SD_au8Czech_ta_find_mailbox_2[0x10] = {
0x48, 0x6c, 0x61, 0x73, 0x6f, 0x76, 0x61, 0x20, 0x73, 0x63, 0x68, 0x72, 0x61, 0x6e, 0x6b, 0x61
};
/* Schr�nka */
const Uint8Type APP_SD_au8Czech_ta_find_mailbox_3[0x9] = {
0x53, 0x63, 0x68, 0x72, 0xc3, 0xa1, 0x6e, 0x6b, 0x61
};
/* Schranka */
const Uint8Type APP_SD_au8Czech_ta_find_mailbox_4[0x8] = {
0x53, 0x63, 0x68, 0x72, 0x61, 0x6e, 0x6b, 0x61
};
/* Mailbox */
const Uint8Type APP_SD_au8Czech_ta_find_mailbox_5[0x7] = {
0x4d, 0x61, 0x69, 0x6c, 0x62, 0x6f, 0x78
};

/* Buz�n de Voz */
const Uint8Type APP_SD_au8Spanish_ta_find_mailbox_1[0xd] = {
0x42, 0x75, 0x7a, 0xc3, 0xb3, 0x6e, 0x20, 0x64, 0x65, 0x20, 0x56, 0x6f, 0x7a
};
/* Mensajes de Voz */
const Uint8Type APP_SD_au8Spanish_ta_find_mailbox_2[0xf] = {
0x4d, 0x65, 0x6e, 0x73, 0x61, 0x6a, 0x65, 0x73, 0x20, 0x64, 0x65, 0x20, 0x56, 0x6f, 0x7a
};

/* Casella vocale */
const Uint8Type APP_SD_au8Italian_ta_find_mailbox_1[0xe] = {
0x43, 0x61, 0x73, 0x65, 0x6c, 0x6c, 0x61, 0x20, 0x76, 0x6f, 0x63, 0x61, 0x6c, 0x65
};

/* messagerie vocale */
const Uint8Type APP_SD_au8French_ta_find_mailbox_1[0x11] = {
0x6d, 0x65, 0x73, 0x73, 0x61, 0x67, 0x65, 0x72, 0x69, 0x65, 0x20, 0x76, 0x6f, 0x63, 0x61, 0x6c, 0x65
};
/* Bo�te vocale */
const Uint8Type APP_SD_au8French_ta_find_mailbox_2[0xd] = {
0x42, 0x6f, 0xc3, 0xae, 0x74, 0x65, 0x20, 0x76, 0x6f, 0x63, 0x61, 0x6c, 0x65
};

/* caixa correio voz */
const Uint8Type APP_SD_au8Portugese_ta_find_mailbox_1[0x11] = {
0x63, 0x61, 0x69, 0x78, 0x61, 0x20, 0x63, 0x6f, 0x72, 0x72, 0x65, 0x69, 0x6f, 0x20, 0x76, 0x6f, 0x7a
};

/* Voicemailbox */
const Uint8Type APP_SD_au8USEnglish_ta_find_mailbox_1[0xc] = {
0x56, 0x6f, 0x69, 0x63, 0x65, 0x6d, 0x61, 0x69, 0x6c, 0x62, 0x6f, 0x78
};
/* mailbox */
const Uint8Type APP_SD_au8USEnglish_ta_find_mailbox_2[0x7] = {
0x6d, 0x61, 0x69, 0x6c, 0x62, 0x6f, 0x78
};
/* voice messages */
const Uint8Type APP_SD_au8USEnglish_ta_find_mailbox_3[0xe] = {
0x76, 0x6f, 0x69, 0x63, 0x65, 0x20, 0x6d, 0x65, 0x73, 0x73, 0x61, 0x67, 0x65, 0x73
};
/* voicemailbox */
const Uint8Type APP_SD_au8USEnglish_ta_find_mailbox_4[0xc] = {
0x76, 0x6f, 0x69, 0x63, 0x65, 0x6d, 0x61, 0x69, 0x6c, 0x62, 0x6f, 0x78
};

/* ????????? ???? */
const Uint8Type APP_SD_au8Russian_ta_find_mailbox_1[0x1b] = {
0xd0, 0x93, 0xd0, 0xbe, 0xd0, 0xbb, 0xd0, 0xbe, 0xd1, 0x81, 0xd0, 0xbe, 0xd0, 0xb2, 0xd0, 0xbe, 0xd0, 0xb9, 0x20, 0xd1, 0x8f, 0xd1, 0x89, 0xd0, 0xb8, 0xd0, 0xba
};

/* messagerie vocale */
const Uint8Type APP_SD_au8CanadianFrench_ta_find_mailbox_1[0x11] = {
0x6d, 0x65, 0x73, 0x73, 0x61, 0x67, 0x65, 0x72, 0x69, 0x65, 0x20, 0x76, 0x6f, 0x63, 0x61, 0x6c, 0x65
};
/* Bo�te vocale */
const Uint8Type APP_SD_au8CanadianFrench_ta_find_mailbox_2[0xd] = {
0x42, 0x6f, 0xc3, 0xae, 0x74, 0x65, 0x20, 0x76, 0x6f, 0x63, 0x61, 0x6c, 0x65
};

/* Voicemail */
const Uint8Type APP_SD_au8Dutch_ta_find_mailbox_1[0x9] = {
0x56, 0x6f, 0x69, 0x63, 0x65, 0x6d, 0x61, 0x69, 0x6c
};
/* mailbox */
const Uint8Type APP_SD_au8Dutch_ta_find_mailbox_2[0x7] = {
0x6d, 0x61, 0x69, 0x6c, 0x62, 0x6f, 0x78
};
/* voice messages */
const Uint8Type APP_SD_au8Dutch_ta_find_mailbox_3[0xe] = {
0x76, 0x6f, 0x69, 0x63, 0x65, 0x20, 0x6d, 0x65, 0x73, 0x73, 0x61, 0x67, 0x65, 0x73
};
/* voicemailbox */
const Uint8Type APP_SD_au8Dutch_ta_find_mailbox_4[0xc] = {
0x76, 0x6f, 0x69, 0x63, 0x65, 0x6d, 0x61, 0x69, 0x6c, 0x62, 0x6f, 0x78
};

/* R�stbrevl�da */
const Uint8Type APP_SD_au8Swedish_ta_find_mailbox_1[0xe] = {
0x52, 0xc3, 0xb6, 0x73, 0x74, 0x62, 0x72, 0x65, 0x76, 0x6c, 0xc3, 0xa5, 0x64, 0x61
};
/* Brevl�da */
const Uint8Type APP_SD_au8Swedish_ta_find_mailbox_2[0x9] = {
0x42, 0x72, 0x65, 0x76, 0x6c, 0xc3, 0xa5, 0x64, 0x61
};
/* R�stmeddelanden */
const Uint8Type APP_SD_au8Swedish_ta_find_mailbox_3[0x10] = {
0x52, 0xc3, 0xb6, 0x73, 0x74, 0x6d, 0x65, 0x64, 0x64, 0x65, 0x6c, 0x61, 0x6e, 0x64, 0x65, 0x6e
};
/* Telefonsvarare */
const Uint8Type APP_SD_au8Swedish_ta_find_mailbox_4[0xe] = {
0x54, 0x65, 0x6c, 0x65, 0x66, 0x6f, 0x6e, 0x73, 0x76, 0x61, 0x72, 0x61, 0x72, 0x65
};

/* Telesekreter */
const Uint8Type APP_SD_au8Turkish_ta_find_mailbox_1[0xc] = {
0x54, 0x65, 0x6c, 0x65, 0x73, 0x65, 0x6b, 0x72, 0x65, 0x74, 0x65, 0x72
};
/* Mesaj kutusu */
const Uint8Type APP_SD_au8Turkish_ta_find_mailbox_2[0xc] = {
0x4d, 0x65, 0x73, 0x61, 0x6a, 0x20, 0x6b, 0x75, 0x74, 0x75, 0x73, 0x75
};
/* Sesli mesajlar */
const Uint8Type APP_SD_au8Turkish_ta_find_mailbox_3[0xe] = {
0x53, 0x65, 0x73, 0x6c, 0x69, 0x20, 0x6d, 0x65, 0x73, 0x61, 0x6a, 0x6c, 0x61, 0x72
};
/* Telesekreter */
const Uint8Type APP_SD_au8Turkish_ta_find_mailbox_4[0xc] = {
0x54, 0x65, 0x6c, 0x65, 0x73, 0x65, 0x6b, 0x72, 0x65, 0x74, 0x65, 0x72
};

/* Talepostkasse */
const Uint8Type APP_SD_au8Norwegian_ta_find_mailbox_1[0xd] = {
0x54, 0x61, 0x6c, 0x65, 0x70, 0x6f, 0x73, 0x74, 0x6b, 0x61, 0x73, 0x73, 0x65
};
/* Postkasse */
const Uint8Type APP_SD_au8Norwegian_ta_find_mailbox_2[0x9] = {
0x50, 0x6f, 0x73, 0x74, 0x6b, 0x61, 0x73, 0x73, 0x65
};
/* Talebeskjeder */
const Uint8Type APP_SD_au8Norwegian_ta_find_mailbox_3[0xd] = {
0x54, 0x61, 0x6c, 0x65, 0x62, 0x65, 0x73, 0x6b, 0x6a, 0x65, 0x64, 0x65, 0x72
};
/* Telefonsvarer */
const Uint8Type APP_SD_au8Norwegian_ta_find_mailbox_4[0xd] = {
0x54, 0x65, 0x6c, 0x65, 0x66, 0x6f, 0x6e, 0x73, 0x76, 0x61, 0x72, 0x65, 0x72
};

/* poczta glosowa */
const Uint8Type APP_SD_au8Polish_ta_find_mailbox_1[0xf] = {
0x70, 0x6f, 0x63, 0x7a, 0x74, 0x61, 0x20, 0x67, 0xc5, 0x82, 0x6f, 0x73, 0x6f, 0x77, 0x61
};
/* mailbox */
const Uint8Type APP_SD_au8Polish_ta_find_mailbox_2[0x7] = {
0x6d, 0x61, 0x69, 0x6c, 0x62, 0x6f, 0x78
};
/* wiadomosci glosowe */
const Uint8Type APP_SD_au8Polish_ta_find_mailbox_3[0x14] = {
0x77, 0x69, 0x61, 0x64, 0x6f, 0x6d, 0x6f, 0xc5, 0x9b, 0x63, 0x69, 0x20, 0x67, 0xc5, 0x82, 0x6f, 0x73, 0x6f, 0x77, 0x65
};
/* undecided */
const Uint8Type APP_SD_au8Polish_ta_find_mailbox_4[0x9] = {
0x75, 0x6e, 0x64, 0x65, 0x63, 0x69, 0x64, 0x65, 0x64
};

/* Buz�n de Voz */
const Uint8Type APP_SD_au8USSpanish_ta_find_mailbox_1[0xd] = {
0x42, 0x75, 0x7a, 0xc3, 0xb3, 0x6e, 0x20, 0x64, 0x65, 0x20, 0x56, 0x6f, 0x7a
};
/* Mensajes de Voz */
const Uint8Type APP_SD_au8USSpanish_ta_find_mailbox_2[0xf] = {
0x4d, 0x65, 0x6e, 0x73, 0x61, 0x6a, 0x65, 0x73, 0x20, 0x64, 0x65, 0x20, 0x56, 0x6f, 0x7a
};

/* End Of File APP_SD_Mailbox.c */
