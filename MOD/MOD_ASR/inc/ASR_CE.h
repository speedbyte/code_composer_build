/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     ASR_CE.h
*-----------------------------------------------------------------------------
* Module Name:    ASR
*-----------------------------------------------------------------------------
* Description:    
*-----------------------------------------------------------------------------
* $Date: 2009-01-20 16:24:15 +0100 (Di, 20 Jan 2009) $
* $Rev: 3917 $
* $Author: rammelt $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_ASR/trunk/inc/ASR_CE.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _ASR_CE_H
#define _ASR_CE_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Import configuration                                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/
#define ASR0_STATUS_ACTIVE                  b_ASR0_Status_Activ_b
#define ASR0_STATUS_RECOGOPEN               b_ASR0_Status_RecogOpen_b
#define ASR0_STATUS_STATE_0                 b_ASR0_Status_StateID_b_0
#define ASR0_STATUS_STATE_1                 b_ASR0_Status_StateID_b_1
#define ASR0_STATUS_STATE_2                 b_ASR0_Status_StateID_b_2
#define ASR0_STATUS_STATE_C                 b_ASR0_Status_StateID_b(c)

#define ASR1_COMMAND_ID                     b_ASR1_Is_Command_ID_b
#define ASR1_IS_STRING_MULTIPLEXED          b_ASR1_Is_String_TimeMultiplexed_b
#define ASR1_IS_MULTIPLEX_LAST_MSG          b_ASR1_Is_Multiplex_Last_Msg_b
#define ASR1_CNT_MULTIPLEX_STRING           b_ASR1_Cnt_Multiplex_String_b
#define ASR1_CONFIDENCE_0                   b_ASR1_Confidence_b_0
#define ASR1_CONFIDENCE_1                   b_ASR1_Confidence_b_1
#define ASR1_CONFIDENCE_C                   b_ASR1_Confidence_b(c)      
#define ASR1_COMMAND_ID_0                   b_ASR1_Command_ID_b_0
#define ASR1_COMMAND_ID_1                   b_ASR1_Command_ID_b_1
#define ASR1_COMMAND_ID_2                   b_ASR1_Command_ID_b_2
#define ASR1_COMMAND_ID_C                   b_ASR1_Command_ID_b(c)           
#define ASR1_IS_SUBCMD_LAST_MSG             b_ASR1_Is_SubCmd_Last_Msg_b
#define ASR1_CNT_SUBCMD                     b_ASR1_Cnt_SubCmd_b
#define ASR1_IS_NBEST_LAST_MSG              b_ASR1_Is_NBest_Last_Msg_b
#define ASR1_CNT_NBEST                      b_ASR1_Cnt_NBest_b

#define ASR2_CHAR_00                        b_ASR2_Char00_c
#define ASR2_CHAR_01                        b_ASR2_Char01_c
#define ASR2_CHAR_02                        b_ASR2_Char02_c
#define ASR2_CHAR_03                        b_ASR2_Char03_c
#define ASR2_CHAR_04                        b_ASR2_Char04_c
#define ASR2_CHAR_05                        b_ASR2_Char05_c
#define ASR2_CHAR_06                        b_ASR2_Char06_c
#define ASR2_CHAR_07                        b_ASR2_Char07_c

#define ASR3_CHAR_08                        b_ASR3_Char08_c
#define ASR3_CHAR_09                        b_ASR3_Char09_c
#define ASR3_CHAR_10                        b_ASR3_Char10_c
#define ASR3_CHAR_11                        b_ASR3_Char11_c
#define ASR3_CHAR_12                        b_ASR3_Char12_c
#define ASR3_CHAR_13                        b_ASR3_Char13_c
#define ASR3_CHAR_14                        b_ASR3_Char14_c
#define ASR3_CHAR_15                        b_ASR3_Char15_c

#define ASR4_CHAR_16                        b_ASR4_Char16_c
#define ASR4_CHAR_17                        b_ASR4_Char17_c
#define ASR4_CHAR_18                        b_ASR4_Char18_c
#define ASR4_CHAR_19                        b_ASR4_Char19_c
#define ASR4_CHAR_20                        b_ASR4_Char20_c
#define ASR4_CHAR_21                        b_ASR4_Char21_c
#define ASR4_CHAR_22                        b_ASR4_Char22_c
#define ASR4_CHAR_23                        b_ASR4_Char23_c

#define ASR5_CHAR_24                        b_ASR5_Char24_c
#define ASR5_CHAR_25                        b_ASR5_Char25_c
#define ASR5_CHAR_26                        b_ASR5_Char26_c
#define ASR5_CHAR_27                        b_ASR5_Char27_c
#define ASR5_CHAR_28                        b_ASR5_Char28_c
#define ASR5_CHAR_29                        b_ASR5_Char29_c
#define ASR5_CHAR_30                        b_ASR5_Char30_c
#define ASR5_CHAR_31                        b_ASR5_Char31_c

/*----------------------------------------------------------------------------*/
/* Public Constants                                                           */
/*----------------------------------------------------------------------------*/

#endif /* End Of File ASR_CE.h */















