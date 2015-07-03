/* STARTSINGLE_OF_MULTIPLE */
/*****************************************************************************
| Project Name: V E C T O R   -  Common module type definition header
|    File Name: v_def.h
|
|  Description: Declares types and definitions common to all 
|               VECTOR CANbedded modules.
|               If some of the definitions collides with Company-internal
|               type definitions, modifications could be made here.
|               It must be ensured, that own types are compliant 
|               with Vector internal definitions 
|
|               Note:
|               =====
|               that some hardware dependent settings included in this file.
|               Never mix up files with same namings but intended for 
|               other hardware platforms.
|
|
|     This file is intended for the following hardware platform:
|   ---------------------------------------------------------- 
|
|            Compiler:   see module file
|            Controller:
|
|-----------------------------------------------------------------------------
|               C O P Y R I G H T
|-----------------------------------------------------------------------------
| Copyright (c) 2002 by Vector Informatik GmbH.       All rights reserved.
|
| This software is copyright protected and proprietary 
| to Vector Informatik GmbH. Vector Informatik GmbH 
| grants to you only those rights as set out in the 
| license conditions. All other rights remain with 
| Vector Informatik GmbH.
|
|-----------------------------------------------------------------------------
|               A U T H O R   I D E N T I T Y
|-----------------------------------------------------------------------------
| Initials     Name                      Company
| --------     ---------------------     ------------------------------------
|   Hp         Armin Happel              Vector Informatik GmbH
|   Ht         Heike Honert              Vector Informatik GmbH
|   dH         Gunnar de Haan            Vector Informatik GmbH
|   Et         Thomas Ebert              Vector Informatik GmbH
|   Wr         Hans Waaser               Vector Informatik GmbH
|   Vg         Frank Voorburg            Vector CANtech, Inc.
|   Ds         Hussain Darwish           Vector CANtech, Inc.
|   RAP        Rebecca Pipkorn           Vector CANtech, Inc.
|   BWR        Brandon Root              Vector CANtech, Inc.
|   Ml         Patrick Markl             Vector Informatik GmbH
|   Pl         Georg Pfluegel            Vector Informatik GmbH
|   LS         Konrad Lazarus            Vector Informatik GmbH
|   Zw         Werner Ziegler            Vector Informatik GmbH
|   Bs         Thomas Balster            Vector Informatik GmbH
|   Stu        Bernd Stumpf              Vector Informatik GmbH
|   Pet        Thomas Petrus             Vector Informatik GmbH
|   To         Torsten Schmidt           Vector Informatik GmbH
|   Ap         Andreas Pick              Vector Informatik GmbH
|   Tri        Frank Triem               Vector Informatik GmbH
|   WM         Marco Wierer              Vector Informatik GmbH
|   Bir        Holger Birke              Vector Informatik GmbH
|   Ces        Senol Cendere             Vector Informatik GmbH
|   Ms         Gunnar Meiss              Vector Informatik GmbH
|   Ths        Thomas Sommer             Vector Informatik GmbH
|   Krt        Kerstin Thim              Vector Informatik GmbH
|-----------------------------------------------------------------------------
|               R E V I S I O N   H I S T O R Y
|-----------------------------------------------------------------------------
| Date       Ver    Author  Description
| ---------  ----   ------  --------------------------------------------------
| 13-Mar-01  1.00    Hp     Creation
| 04-Apr-01  1.01    Ht     general rework 
| 05-Apr-01  1.02    dH     added Fujitsu FFMC16LX
| 05-Apr-01  1.03    HH     added COMP_GHS_V85X
| 11-Apr-01  1.04    Et     added LIN components
| 23-Apr-01  1.05    Si     corrections for C_COMP_IAR_78K0
|                           lint comments added
| 29-Mai-01  1.06    Pl     added ARM7 STMICRO
| 22-Jun-01  1.07    Ht     added ANSI CANoe
| 03-Jul-01          Wr     added C_COMP_OKI_CC665S
| 12-Jul-01          Et     support combination of "old" CAN driver and "new" LIN driver
| 13-Jul-01  1.08    Ht     error check for supported systems and switches added
| 16-Jul-01          dH     added C_COMP_HEW_SH7055 / C_COMP_HEW_H8S
| 17-Aug-01  1.09    Et     deleted keywords __near and __far for C_COMP_FUJITSU_8L
| 21-Aug-01  1.10    Ht/Vg  added C_COMP_DIABDATA_PPC
| 25-Aug-01  1.11    Ml     added #pragma MESSAGE DISABLE C1106 for Hiware HC08 compiler
| 06-Sep-01  1.12    Ht/Vg  added C_COMP_GNU_ST9
| 11-Sep-01  1.13    Fr     added C_COMP_ANSI_TMS_470
| 21-Sep-01  1.14    Pl     added C_COMP_MCP18_PIC_INT
| 25-Sep-01  1.15    Vg     added C_COMP_MICROTEC_MC376 and C_COMP_TASKING_MC376
| 28-Sep-01  1.16    Hp     added C_COMP_GHS_TX39 and C_COMP_TOSHIBA_TLC900
| 23-Oct-01  1.17    Js     added C_COMP_DIABDATA_MCORE
| 23-Oct-01  1.18    dH     changed ..._SH705X to ..._SH705X
| 05-Nov-01  1.19    dH     added M32R
| 19-Nov-01  1.20    Ht     added C_COMP_xxx_XC16X
| 22-Nov-01  1.21    dH     added C_COMP_IAR_H8S
| 11-Nov-01  1.22    Fz     added C_COMP_MITSUBISHI_M32C/C_COMP_NEC_V85X
| 19-Dec-01  1.23    dH     changed Fujitsu FFMC16LX
| 28-Jan-02  1.24    Ml     ESCAN00002160: added paging keywords for HC12
| 31-Jan-02  1.25    Pl     added C_COMP_KEIL_T89C51C
| 07-Mar-02  1.26    Vg/Rn  added C_COMP_METROWERKS_PPC
| 27-Mar-02  1.27    Fz     added C_COMP_IAR_V85X
| 09-Apr-02  1.28    Ht     addaption to LI 1.3
| 02-Apr-02  1.29    Ml     added C_COMP_METROWERKS_MGT5100
|                           added C_COMP_METROWERKS_DSP56F80X
| 13-May-02  1.30    LS     MEMORY_HUGE added for C_COMP_TASKING_C16X
| 21-May-02  1.31    Ml     canbittype for HC12 is now unsigned short
| 10-Jul-02  1.32    Ml     canbittype for HC12 changed to unsigned char
| 16-Jul-02  1.33a   Fz     canbittype for V850 changed to unsigend int
|                           _c_bits32 added  
| 31-Jul-02          Ts     added C_COMP_HEW_SH70XX_HCAN2
| 31-Jul-02  1.33    Ht     _c_bits16 added for C_CPUTYPE_BITORDER_MSB2LSB
| 31-Jul-02  1.34    Si     pc-lint comments for 78K0 changed
| 02-Aug-02  1.33    Sf     ESCAN00003192  Memory qualifier for M16C changed
| 08-Aug-02  1.34    Zw     ESCAN00003456  added STmicro ST7 (beCAN) / Hiware
| 18-Aug-02          Pet    ESCAN00003530  delete keyword near for HC08
| 06-Sep-02  1.35    Zw     ESCAN00003688: MEMORY_NEAR defined to nothing for ST7_beCAN
| 09-Sep-02  1.36    Bs     controller OKI MSM9225B (C_COMP_GHS_ARM7TM) added (needed for FBL)
| 10-Sep-02  1.37    Ds     added C_COMP_COSMIC_ST7_BECAN
|                           and   C_COMP_HIWARE_ST7_BECAN
| 13-Sep-02  1.38    Ht     rework the module
| 17-Sep-02  1.39    Stu    add the C_COMP_NEC_78K0
| 17-Sep-02  1.40    Vg     ESCAN00003614 added __declspec() to place const 
|                           variables into ROM for Metrowerks
| 18-Sep-02  1.41    Stu    added C_COMP_NEC_78K0
| 07-Oct-02  1.42    dH     added C_COMP_GAIO_SH705X
| 11-Oct-02  1.43    dH     added C_COMP_FUJITSU_16LX_HL
| 14-Oct-02  1.44    Ml     changed vbittype for MGT5100 into unsigned short
| 14-Oct-02  1.45    Stu    changed vbittype for 78k0 into unsigned char
| 18-Oct-02  1.46    Pl     added C_COMP_TASKING_ST10_CCAN 
|                           added C_COMP_ARM_ST30_CCAN 
| 30-Oct-02  1.47    Ml     added C_COMP_ARM_470 and C_COMP_TI_470
| 06-Nov-02  1.48    Wr     added C_COMP_NATIONAL_CR16 
| 07-Nov-02  1.49    RAP    added C_COMP_DIABDATA_MPC823
|                    RAP    added C_COMP_GHS_STARFISH
| 14-Nov-02  1.50    An     corrected the banking defines for Cosmic and Hiware
| 27-Nov-02  1.50    Pl     added C_COMP_FUJITSU_FR50
| 02-Dec-02  1.51    Si     MEMORY_NEAR changed for C_COMP_NEC_78K0
| 11-Dec-02          Pet    added C_COMP_IAR_CEVF
| 18-Dec-02  1.52    Si     added C_COMP_NEC_78K0
| 04-Jan-03          Et     added memory qualifier for C_COMP_IAR_CEVF
| 22-Jan-03          Ap     added C_COMP_ARM_EASYCAN
| 28-Jan-03  1.53    Pl     added C_COMP_KEIL_ST10_CCAN
|                           delete C_COMP_ARM_ARM7STM
| 03-Apr-04  1.54    To     added C_COMP_METROWERKS_DSP5683X
|                    ZW     canbittype for CEVG is now unsigned char
|                    Hp     Add MEMORY_HUGE for XC16x
|                    Fz     added  C_COMP_IAR_CR16 and C_COMP_NATIONAL_CR16 changed
|                    Ml     added  C_COMP_TI_TMS320
| 2003-05-13 1.60    Ht     support V_MEMROM0
| 2003-05-14 1.61    Tri    added V_MEMROM0 definition for MGT5100
| 2003-05-15 1.62    Pl     added C_COMP_FUJITSU_FR60_CCAN
| 2003-05-20 1.63    WM     added near/far memory defines for C_COMP_HIWARE_12
| 2003-06-11 1.64    CB     MEMORY_HUGE added for C_COMP_TASKING_ST10_CCAN    
| 2003-06-18 1.65    Bir    added C_COMP_MICROCHIP_DSPIC30
| 2003-07-08 1.66    Bir    no local lint command
| 2003-08-15 1.67    Rr     changed MEMORY_FAR for M32C
| 2003-08-18 1.68    BWR    added support for MAC710X
| 2003-09-02 1.69    BWR    added support for MC332 with external Intel 82527 
| 2003-09-09 1.70    Bir    added C_COMP_TASKING_TRICORE_MULTICAN & C_COMP_GNU_TRICORE_MULTICAN
| 2003-10-30 1.71    RAP    added C_COMP_MICROCHIP_PIC18
| 2003-11-13 2.00    Ht     memory qualifier changed to new definition
| 2004-01-08 2.01    BWR    added support for MPC55XX 
| 2004-01-16 2.02    Ml     added cast macros
| 2004-01-22 2.03    dH     added C_COMP_HEW_SH2_HCAN1 and C_COMP_HEW_H8S_HCAN1
| 2004-02-13 2.04    WM     ESCAN00007465: Added banking support for C_COMP_IAR_12
| 2004-02-18 2.05    RAP    Update for C_COMP_MICROCHIP_PIC18
| 2004-03-12 2.06    Ml     changed canbittype for TMS470
| 2004-03-13 2.07    dH     added C_COMP_HEW_H8TINY
| 2004-04-20 2.08    dH     added C_COMP_HEW_H8SX_HCAN1
| 2004-04-08 2.09    Rna    Memoryclassifier V_MEMROM0 and MEMORY_ROM for Metrowerks PPC changed
| 2004-05-14 2.10    Ms     added Release Version define
|                           added include for vstdlib.h
| 2004-05-18 2.11    Ml     changed plattform check to work in any case (NO_ELSE)
|                    Ml     added V_NULL
| 2004-06-17 2.12    Ms     changed C_COMP_SUPPORTED_PLATTFORM -> V_COMP_SUPPORTED_PLATTFORM
| 2004-06-26 2.13    RAP    added C_COMP_GHS_SJA2020
| 2004-06-30 2.14    Ml     added C_COMP_COSMIC_MCS12X_MSCAN12
| 2004-08-17 2.15    Pl     Memoryclassifier V_MEMROM0/1/2 for the C_COMP_TOSHIBA_TLC900 changed
| 2004-08-31 2.16    Ml     changed bitfieldtype for MCS12X
| 2004-09-08 2.17    RAP    changed vbittype for C_COMP_GHS_SJA2020
| 2004-09-12 2.18    BWR    added C_COMP_GHS_PPC
| 2004-09-20 2.19    Ces    added C_COMP_GHS_ST30_CCAN
| 2004-09-27 2.20    Ms     -ESCAN00009636: Naming Conventions
| 2004-09-28 2.21    Bir    changed bitfieldtype for TriCore MultiCAN
| 2004-09-30 2.22    Ht     Review
| 2004-10-06 2.23    Ml     Added C_COMP_MTRWRKS_MCS12X_MSCAN12  
| 2004-10-27 2.24    Ml     changed V_ENABLE_VSTDLIB -> VGEN_ENABLE_VSTDLIB
| 2004-10-29 2.25    Pl     Added C_COMP_GHS_TX19
| 2004-11-05 2.26    Ml     Added C_COMP_COSMIC_MCS12X_MSCAN12 and C_COMP_COSMIC_MCS12_MSCAN12
| 2004-11-08 2.27    Et     support compiler option -fNP (C_COMP_MITSUBISHI_M32C)
| 2004-11-10 2.28    Ml     Added compatibility for VStdLib
| 2004-11-15 2.29    dH     new memoryclassifier for C_COMP_FUJITSU_16LX/C_COMP_FUJITSU_16LX_HL
| 2004-11-15 2.30    Ms     ESCAN00010228: Support C_COMP specific V_NULL definition
| 2004-11-19 2.31    Ml     bitfields for TMS470 are now int
|                    Ths    ESCAN00010435: Due to compatibility V_DEF_VERSION added
| 2005-01-31 2.32    Ml     Added C_COMP_IAR_TMS430_SCCHECC
| 2005-02-09 2.33    Ht     support V_NULL for C_COMP_KEIL_C5X5C
| 2005-03-10 2.34    Ces    Memory qualifier MEMORY_SADDR and MEMORY_NEAR for IAR 78K0 compiler
| 2005-03-22 2.35    Pl     support V_NULL for C_COMP_TASKING_ST10_CCAN
| 2005-03-22 2.36    Ces    support C_COMP_IAR_78K0_AFCAN
|                           support V_NULL for Tasking C16x/ST10, Mitsubishi M16C, IAR 78K0,
|                           Metrowerks DSP5683x and GreenHills ST30
| 2005-03-24 2.37    Krt    Set cast macros for M32C controller, to avoid compiler warnings
|                    Bir    support V_NULL for C_COMP_TASKING_C16X, C_COMP_TASKING_XC16X, C_COMP_GNU_TRICORE_MULTICAN
|                    Ces    MEMORY_NEAR corrections for IAR 78K0 AFCAN
| 2005-06-17 2.38    Ht     remove duplicated compiler switch
|                    dH     added C_COMP_HEW_SH2_RCAN
|                    Pl     added C_COMP_GNU_ST10_CCAN
|                    Ds     Added support for ADI Blackfin (BF5xx)
|                    Ml     Added QNX platforms
|                    Fz     MEMORY_HUGE changed for V85X IAR compiler in case tiny memory model
|                    Ht     default definition of NULL
| 2005-06-20 2.39    BWR    Added support for MAC7100 IAR compiler
|                    dH     changed bittype for C_COMP_HEW_SH2_RCAN to unsigned char
|****************************************************************************/

#ifndef  V_DEF_H
#define  V_DEF_H

/***************************************************************************/
/* Version                  (abcd: Main version=ab, Sub Version=cd )       */
/***************************************************************************/

/* ##V_CFG_MANAGEMENT ##CQProject : Common_Vdef CQComponent : Implementation */
#define COMMON_VDEF_VERSION            0x0239
#define COMMON_VDEF_RELEASE_VERSION    0x00

/* compatibility for IL versions < 3.52 */
#define V_DEF_VERSION                  COMMON_VDEF_VERSION

/***************************************************************************/
/* Supported Systems                                                       */
/***************************************************************************/
#if defined( C_COMP_COSMIC_08 )                 || \
    defined( C_COMP_COSMIC_12 )                 || \
    defined( C_COMP_COSMIC_ST7 )                || \
    defined( C_COMP_DIABDATA_PPC )              || \
    defined( C_COMP_FUJITSU_16LX_HL )           || \
    defined( C_COMP_GHS_V85X )                  || \
    defined( C_COMP_HIWARE_12 )                 || \
    defined( C_COMP_IAR_12 )                    || \
    defined( C_COMP_IAR_78K0 )                  || \
    defined( C_COMP_IAR_78K0_AFCAN )            || \
    defined( C_COMP_IAR_M16C )                  || \
    defined( C_COMP_KEIL_C16X )                 || \
    defined( C_COMP_KEIL_C5X5C )                || \
    defined( C_COMP_TASKING_C16X )              || \
    defined( C_COMP_TI_TMS470 )                 || \
    defined( C_COMP_COSMIC_ST7_BECAN )          || \
    defined( C_COMP_TASKING_ST10_CCAN )         || \
    defined( C_COMP_TASKING_TRICORE_MULTICAN )  || \
    defined( C_COMP_HEW_H8S_HCAN1 )             
# define V_COMP_SUPPORTED_PLATTFORM
#endif

#if !defined V_COMP_SUPPORTED_PLATTFORM
# error "driver not supported or C_COMP_xxx_yyy not defined"
#endif



/***************************************************************************/
/***************************************************************************/
/****  Hardware independent settings  **************************************/
/***************************************************************************/
/***************************************************************************/
/*--- standard memory qualifier definition --------------------------------*/

/* 8-Bit qualifier */
typedef unsigned char  vuint8;
typedef signed char    vsint8;
# define canuint8 vuint8
# define cansint8 vsint8

/* 16-Bit qualifier */
typedef unsigned short vuint16;
typedef signed short   vsint16;
#define canuint16 vuint16
#define cansint16 vsint16

/* 32-Bit qualifier */
typedef unsigned long  vuint32;
typedef signed long    vsint32;
#define canuint32 vuint32
#define cansint32 vsint32


#if defined( C_COMP_KEIL_C5X5C)
# if defined( C_BUFFER_XDATA )
  typedef unsigned char xdata *TxDataPtr;      /* ptr to data in canTransmit   */
  typedef unsigned char xdata *RxDataPtr;      /* ptr to receiving data buffer */
# elif defined( C_BUFFER_IDATA )
  typedef unsigned char idata *TxDataPtr;      /* ptr to data in canTransmit   */
  typedef unsigned char idata *RxDataPtr;      /* ptr to receiving data buffer */
# endif
#else
typedef unsigned char *TxDataPtr;              /* ptr to transmission data buffer */
typedef unsigned char *RxDataPtr;              /* ptr to receiving data buffer    */
#endif

/***************************************************************************/
/*  Defines                                                                */
/***************************************************************************/

/***************************************************************************/
/*  V_NULL Definition                                                      */
/***************************************************************************/

#if defined( C_COMP_FUJITSU_16LX_HL )      || \
    defined( C_COMP_KEIL_C5X5C )           || \
    defined( C_COMP_TASKING_ST10_CCAN )    || \
    defined( C_COMP_TASKING_C16X )         
# define V_NULL 0
#endif


#if !defined(V_NULL)
# define V_NULL ((void*)0)
#endif

#ifndef NULL
# define NULL V_NULL
#endif


/***************************************************************************/
/***************************************************************************/
/****  Hardware/Compiler dependent settings   ******************************/
/***************************************************************************/
/***************************************************************************/

/*  data type according to the CPU type */
#if defined( C_CPUTYPE_8BIT )
/* 8-Bit qualifier */
# ifndef vuintx
#  define vuintx vuint8
# endif
# ifndef vsintx
#  define vsintx vsint8
# endif
#else
# if defined( C_CPUTYPE_16BIT )
/* 16-Bit qualifier */
#  ifndef vuintx
#   define vuintx vuint16
#  endif
#  ifndef vsintx
#   define vsintx vsint16
#  endif
# else
#  if defined( C_CPUTYPE_32BIT )
/* 32-Bit qualifier */
#   ifndef vuintx
#    define vuintx vuint32
#   endif
#   ifndef vsintx
#    define vsintx vsint32
#   endif
#  endif
# endif
#endif


/* qualifier for bitfield definition */

#if defined( C_COMP_COSMIC_08 )                 || \
    defined( C_COMP_IAR_12 )                    || \
    defined( C_COMP_HIWARE_12 )                 || \
    defined( C_COMP_COSMIC_12 )                 || \
    defined( C_COMP_IAR_M16C )                  || \
    defined( C_COMP_KEIL_C5X5C )                || \
    defined( C_COMP_IAR_78K0 )                  || \
    defined( C_COMP_IAR_78K0_AFCAN )            || \
    defined( C_COMP_COSMIC_ST7 )                || \
    defined( C_COMP_HEW_H8S_HCAN1 )             || \
    defined( C_COMP_COSMIC_ST7_BECAN )          
typedef unsigned char    vbittype;
#else
#if defined( C_COMP_DIABDATA_PPC )              
typedef unsigned short   vbittype;
# else     
typedef unsigned int     vbittype;
# endif
#endif
#define canbittype       vbittype


#if defined( C_COMP_IAR_78K0 )        || \
    defined( C_COMP_IAR_78K0_AFCAN )  
/*lint -save -e46 */ /* ignore error: field type should be int */
#endif

#if defined( C_SUPPORT_MIXED_CAN_LIN )
  /* support combination of "old" CAN driver and "new" LIN driver */
#else
/* Structure for bit accessed memory.           */
/* The bit-order is hardware/compiler dependent */
#  if defined( C_CPUTYPE_BITORDER_LSB2MSB )
struct _c_bits8
{
  canbittype  b0:1;  /* LSB-Bits (lower adr!) */
  canbittype  b1:1;
  canbittype  b2:1;
  canbittype  b3:1;
  canbittype  b4:1;
  canbittype  b5:1;
  canbittype  b6:1;
  canbittype  b7:1;  /* MSB-Bits (higher address) unused */
 };

struct _c_bits16 
{
  canbittype  b0:1;  /* lower adr */
  canbittype  b1:1;
  canbittype  b2:1;
  canbittype  b3:1;
  canbittype  b4:1;
  canbittype  b5:1;
  canbittype  b6:1;
  canbittype  b7:1;

  canbittype  b10:1; /* higher adr */
  canbittype  b11:1;
  canbittype  b12:1;
  canbittype  b13:1;
  canbittype  b14:1;
  canbittype  b15:1;
  canbittype  b16:1;
  canbittype  b17:1;
};

struct _c_bits32 
{
  canbittype  b0:1;  /* lower adr */
  canbittype  b1:1;
  canbittype  b2:1;
  canbittype  b3:1;
  canbittype  b4:1;
  canbittype  b5:1;
  canbittype  b6:1;
  canbittype  b7:1;

  canbittype  b10:1; /* mid low adr */
  canbittype  b11:1;
  canbittype  b12:1;
  canbittype  b13:1;
  canbittype  b14:1;
  canbittype  b15:1;
  canbittype  b16:1;
  canbittype  b17:1;

  canbittype  b20:1; /* mid high adr */
  canbittype  b21:1;
  canbittype  b22:1;
  canbittype  b23:1;
  canbittype  b24:1;
  canbittype  b25:1;
  canbittype  b26:1;
  canbittype  b27:1;

  canbittype  b30:1; /* higher adr */
  canbittype  b31:1;
  canbittype  b32:1;
  canbittype  b33:1;
  canbittype  b34:1;
  canbittype  b35:1;
  canbittype  b36:1;
  canbittype  b37:1;
};

# else /* MSB2LSB */
#  if defined( C_CPUTYPE_BITORDER_MSB2LSB )
struct _c_bits8         
{
  canbittype  b7:1;  /* MSB-Bits (Higher addr!) */
  canbittype  b6:1;
  canbittype  b5:1;
  canbittype  b4:1;
  canbittype  b3:1;
  canbittype  b2:1;
  canbittype  b1:1;
  canbittype  b0:1;  /* LSB-Bits (higher address) unused */
};

struct _c_bits16 
{
  canbittype  b7:1;   /* lower adr */
  canbittype  b6:1;
  canbittype  b5:1;
  canbittype  b4:1;
  canbittype  b3:1;
  canbittype  b2:1;
  canbittype  b1:1;
  canbittype  b0:1;

   canbittype b17:1; /* higher adr */
  canbittype  b16:1;
  canbittype  b15:1;
  canbittype  b14:1;
  canbittype  b13:1;
  canbittype  b12:1;
  canbittype  b11:1;
  canbittype  b10:1;
};


struct _c_bits32 
{
  canbittype  b7:1;   /* lower adr */
  canbittype  b6:1;
  canbittype  b5:1;
  canbittype  b4:1;
  canbittype  b3:1;
  canbittype  b2:1;
  canbittype  b1:1;
  canbittype  b0:1;

  canbittype  b17:1;  /* mid low adr */
  canbittype  b16:1;
  canbittype  b15:1;
  canbittype  b14:1;
  canbittype  b13:1;
  canbittype  b12:1;
  canbittype  b11:1;
  canbittype  b10:1;

  canbittype  b27:1;  /* mid high adr */
  canbittype  b26:1;
  canbittype  b25:1;
  canbittype  b24:1;
  canbittype  b23:1;
  canbittype  b22:1;
  canbittype  b21:1;
  canbittype  b20:1;

  canbittype  b37:1;  /* higher adr */
  canbittype  b36:1;
  canbittype  b35:1;
  canbittype  b34:1;
  canbittype  b33:1;
  canbittype  b32:1;
  canbittype  b31:1;
  canbittype  b30:1;
};
#  else
#  error "Bitorder unknown: C_CPUTYPE_BITORDER_MSB2LSB or C_CPUTYPE_BITORDER_LSB2MSB has to be defined"
#  endif /* C_CPUTYPE_BITORDER_MSB2LSB */
# endif  /* C_CPUTYPE_BITORDER_LSB2MSB */
#endif

#if defined( C_COMP_IAR_78K0 )        || \
    defined( C_COMP_IAR_78K0_AFCAN )  
/*lint -restore */ /* re-enable error: field type should be int */
#endif



/***************************************************************************/
/* Memory qualifier                                                        */
/***************************************************************************/


#if defined (  C_COMP_IAR_M16C )   
/* for M16C the default qualifier is now MEMORY_NORMAL
   Therefore MEMORY_FAR can be set to far           */
# define MEMORY_ROM    const        /* for IAR, this may be directed to a separate segment */
# define MEMORY_NEAR   near         /* whole RAM is near       */
# define MEMORY_NORMAL near         /* whole RAM is near       */
# define MEMORY_FAR    far          /* far memory (e.g. flash) */
# define MEMORY_RAM    near         /* whole RAM is near       */
# define V_MEMROM3     P_MEM_ROM
/* C_COMP_IAR_M16C || C_COMP_MITSUBISHI_M16C */
#endif

#if defined( C_COMP_KEIL_C5X5C )
/* this is necessary, because generating tool is referencing on referencing
   on memory qualifier MEMORY_NEAR and MEMORY_FAR */
# if defined( C_BUFFER_IDATA )
#  define MEMORY_NEAR  idata
#  define MEMORY_FAR   idata
# endif
# if defined( C_BUFFER_XDATA )
#  define MEMORY_NEAR  xdata
#  define MEMORY_FAR   xdata
# endif
# define MEMORY_ROM    code
#endif


#if defined( C_COMP_FUJITSU_16LX_HL )
# define  V_MEMROM2_NEAR   const __near
# ifndef V_MEMROM2
#  define V_MEMROM2        const
# endif
# define  V_MEMROM2_FAR    const __far

# define  V_MEMRAM2_NEAR   __near
# define  V_MEMRAM2
# define  V_MEMRAM2_FAR    __far
#endif

#if defined( C_COMP_COSMIC_08 )
# define MEMORY_NEAR   @tiny
#endif                         

#if defined( C_COMP_COSMIC_ST7 ) || \
    defined( C_COMP_COSMIC_ST7_BECAN )
# define MEMORY_NEAR   @tiny
#endif

#if defined( C_COMP_IAR_78K0 ) || defined( C_COMP_IAR_78K0_AFCAN )
# if defined( __VER__ )
#  if ( __VER__ < 400 )
#   define MEMORY_NEAR   saddr
#   define MEMORY_SADDR  saddr
#  else
#   define MEMORY_NEAR   __saddr
#   define MEMORY_SADDR  __saddr
#  endif
#  define V_MEMROM2_NEAR  MEMORY_NEAR
# endif
#endif




#if defined(C_COMP_COSMIC_12) 
/* for HC12 paging support */
# define V_API_FAR           @far
# define V_CALLBACK_FAR      @far
# define V_INTERNAL_FAR      @far
# define V_API_NEAR          @near
# define V_CALLBACK_NEAR     @near
# define V_INTERNAL_NEAR     @near
# define VUINT8_CAST         (vuint8)
# define VSINT8_CAST         (vsint8)
# define VUINT16_CAST        (vuint16)
# define VSINT16_CAST        (vsint16)
# define CANBITTYPE_CAST     (canbittype)
# define CANSINTCPUTYPE_CAST (int)
# define CANUINTCPUTYPE_CAST (unsigned int)
#endif

#if defined(C_COMP_HIWARE_12) 
# define V_API_FAR        far
# define V_CALLBACK_FAR   far
# define V_INTERNAL_FAR   far
# define V_API_NEAR       near
# define V_CALLBACK_NEAR  near
# define V_INTERNAL_NEAR  near
#endif

#if defined(C_COMP_IAR_12) 
# define V_API_FAR        banked
# define V_CALLBACK_FAR   banked
# define V_INTERNAL_FAR   banked
# define V_API_NEAR       non_banked
# define V_CALLBACK_NEAR  non_banked
# define V_INTERNAL_NEAR  non_banked
#endif


#ifndef MEMORY_HUGE
# if defined(C_COMP_TASKING_C16X) || \
     defined(C_COMP_TASKING_ST10_CCAN)
#  define MEMORY_HUGE   huge        /* far functions need huge pointer  */
# else
#  define MEMORY_HUGE               /* no entry                         */
# endif
#endif





/* default defines */
          /* used to store permanent data  */
#ifndef V_MEMROM0
# define V_MEMROM0                  /* addition qualifier data access in ROM  */
#endif

#ifndef V_MEMROM1_NEAR
# define V_MEMROM1_NEAR             /* fast data access in ROM */
#endif

#ifndef V_MEMROM1
# define V_MEMROM1                 /* fast data access in ROM */
#endif

#ifndef V_MEMROM1_FAR
# define V_MEMROM1_FAR             /* slow addressing mode in ROM */
#endif


#ifndef MEMORY_ROM_NEAR
# ifndef V_MEMROM2_NEAR
#  define V_MEMROM2_NEAR   const    /* fast data access in ROM */
# endif
  /* compatibility for modules which use old definition of memory qualifer */
# define MEMORY_ROM_NEAR   V_MEMROM2_NEAR
#else
# define V_MEMROM2_NEAR    MEMORY_ROM_NEAR
#endif

#ifndef MEMORY_ROM
# ifndef V_MEMROM2
#  define V_MEMROM2        const    /* fast data access in ROM */
# endif
  /* compatibility for modules which use old definition of memory qualifer */
# define MEMORY_ROM        V_MEMROM2       
#else
# define V_MEMROM2         MEMORY_ROM
#endif

#ifndef MEMORY_ROM_FAR
# ifndef V_MEMROM2_FAR
#  define V_MEMROM2_FAR    const    /* slow addressing mode in ROM */
# endif
  /* compatibility for modules which use old definition of memory qualifer */
# define MEMORY_ROM_FAR    V_MEMROM2_FAR
#else
# define V_MEMROM2_FAR     MEMORY_ROM_FAR
#endif

#ifndef V_MEMROM3
# define V_MEMROM3        
#endif


/* default defines */
           /* used to store volatile data  */
#ifndef V_MEMRAM0
# define V_MEMRAM0                  /* addition qualifier data access in RAM  */
#endif

#ifndef V_MEMRAM1_NEAR
# define V_MEMRAM1_NEAR             /* fast data access in RAM */
#endif

#ifndef V_MEMRAM1
# define V_MEMRAM1                 /* fast data access in RAM */
#endif

#ifndef V_MEMRAM1_FAR
# define V_MEMRAM1_FAR             /* slow addressing mode in RAM */
#endif


#ifndef MEMORY_NEAR
# ifndef V_MEMRAM2_NEAR
#  define V_MEMRAM2_NEAR           /* fast data access in RAM */
# endif
  /* compatibility for modules which use old definition of memory qualifer */
# define MEMORY_NEAR   V_MEMRAM2_NEAR
#else
# define V_MEMRAM2_NEAR    MEMORY_NEAR
#endif

#ifndef MEMORY_NORMAL
# ifndef V_MEMRAM2
#  define V_MEMRAM2                 /* fast data access in RAM */
# endif
  /* compatibility for modules which use old definition of memory qualifer */
# define MEMORY_NORMAL   V_MEMRAM2       
#else
# define V_MEMRAM2           MEMORY_NORMAL
#endif

#ifndef MEMORY_FAR
# ifndef V_MEMRAM2_FAR
#  define V_MEMRAM2_FAR            /* slow addressing mode in RAM */
# endif
  /* compatibility for modules which use old definition of memory qualifer */
# define MEMORY_FAR    V_MEMRAM2_FAR
#else
# define V_MEMRAM2_FAR     MEMORY_FAR
#endif

#ifndef V_MEMRAM3
# define V_MEMRAM3        
#endif

#if !defined(VUINT8_CAST)
# define VUINT8_CAST
#endif 
#if !defined(VSINT8_CAST)
# define VSINT8_CAST
#endif
#if !defined(VUINT16_CAST)
# define VUINT16_CAST
#endif
# if !defined(VSINT16_CAST)
# define VSINT16_CAST
#endif
#if !defined(CANBITTYPE_CAST)
# define CANBITTYPE_CAST
#endif
#if !defined(CANSINTCPUTYPE_CAST)
# define CANSINTCPUTYPE_CAST
#endif
#if !defined(CANUINTCPUTYPE_CAST)
# define CANUINTCPUTYPE_CAST
#endif

/***************************************************************************/
/*  Common_VStdLib                                                         */
/***************************************************************************/
# if defined (VGEN_ENABLE_VSTDLIB) || defined (V_ENABLE_VSTDLIB)
#  include "vstdlib.h"
# endif

#endif /* V_DEF_H */


/* STOPSINGLE_OF_MULTIPLE */
/************   Organi, Version 3.2.4 Vector-Informatik GmbH  ************/
