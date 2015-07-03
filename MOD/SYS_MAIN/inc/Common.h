/****************************************************************************/
/********************  Copyright 2006 s1nn GmbH & Co. KG  *******************/
/**************************  All Rights Reserved  ***************************/
/****************************************************************************/

/****************************************************************************/
/* File:        Common.h                                                    */
/*--------------------------------------------------------------------------*/
/* Module Name: Not Applicable                                              */
/*--------------------------------------------------------------------------*/
/* Description: Common definitions header file                              */
/*--------------------------------------------------------------------------*/
/* Methods:     Not Applicable                                              */
/*--------------------------------------------------------------------------*/
/*  Module Interfaces:                                                      */
/*              Not Applicable                                              */              
/*--------------------------------------------------------------------------*/
/* Revision History:                                                        */
/* Date     Who   Description                                               */
/* yy/mm/dd                                                                 */
/*--------------------------------------------------------------------------*/
/* 06/02/20 VWD   Created                                                   */
/*--------------------------------------------------------------------------*/
/* 06/06/16 HB    ADD #define BACK, RET, ESC, HEX, DEC                      */
/*--------------------------------------------------------------------------*/
/*                                                                          */
/****************************************************************************/
#ifndef _COMMON_H
#define _COMMON_H 

#include <string.h>

//#include <osekcom.h>  /* JW nur eine Behelfsloesung fuer redine FlagType */



extern int sprintf(char *out, const char *format, ...);
extern int isOsStarted(void);

/*--------------------------------------------------------------------------*/
/* Macros to access registers in the peripheral blocks                      */
/*--------------------------------------------------------------------------*/
/* IO may reference a long word (32-bits), word (16-bits), byte (8-bits),   */
/* bit (1-bit) or bit field (N-bits). USe of these macros improves          */
/* readability in that the reader knows that the symbol is an IO register.  */
#define WRITE_IO(io,val)    io = val
#define READ_IO(sfr)        sfr

#define _CONST          const

/*--------------------------------------------------------------------------*/
/* Type definitions                                                         */
/*--------------------------------------------------------------------------*/ 
//#ifndef __OSEKCOM__
typedef unsigned char   FlagType;
//#endif

typedef signed char     Sint8Type;
typedef signed short    Sint16Type;
typedef signed long     Sint32Type;
typedef unsigned char   Uint8Type;
typedef unsigned short  Uint16Type;
typedef unsigned long   Uint32Type;
typedef float 		    float32Type;     

typedef enum
{
    SUCCESS =0,
    FAIL    =1
} SuccessFailType;

typedef enum {
    BT_FALSE = 0,
    BT_TRUE  = 1
} BooleanType;

typedef enum {
    TS_FALSE = 0,
    TS_TRUE  = 1,
    TS_UNKNOWN = 0xFF
} TriStateType;

typedef enum
{
    IO_POLL,
    IO_INT,
    IO_DMA,
    IO_EI2OS
} IOTransferType;

typedef enum
{
	COMMON_RSP_OK,
    COMMON_RSP_ERROR,
    COMMON_RSP_ERROR_BUSY,
    COMMON_RSP_ERROR_BUFFER_TOO_SHORT,
    COMMON_RSP_ERROR_ADDR_OUT_OF_RANGE,
    COMMON_RSP_ERROR_NOT_INITIALIZED 
}CommonResponseCodeType;

//typedef enum
//{
//    COMMON_RSP_OK=0,
//    EEPROM_RSP_WORKING,
//    COMMON_RSP_ERROR,
//    EEPROM_RSP_ERROR_EEPROM_BLOCKED,
//    COMMON_RSP_ERROR_BUSY,
//    EEPROM_RSP_ERROR_SEND,
//    EEPROM_RSP_ERROR_BUFFER_TOO_SHORT,
//    EEPROM_RSP_ERROR_ADDR_OUT_OF_RANGE,
//    EEPROM_RSP_ERROR_NOT_INITIALIZED 
//}EEPROM_enResponseCode;

/*--------------------------------------------------------------------------*/
/* Endianess Definitions                                                    */
/*--------------------------------------------------------------------------*/
#define U16U8MSB        1
#define U16U8LSB        0

#define U32U8MSB        3
#define U32U8MMSB       2
#define U32U8LLSB       1
#define U32U8LSB        0

typedef union
{
    Uint16Type          u16;
    Sint16Type          s16;
    Uint8Type           u8[2];
    Sint8Type           s8[2];
} Union16Type;

typedef union
{
    Uint32Type          u32;
    Sint32Type          s32;
    Uint16Type          u16[2];
    Sint16Type          s16[2];
    Uint8Type           u8[4];
    Sint8Type           s8[4];
} Union32Type;

/*--------------------------------------------------------------------------*/
/* Miscellaneous Definitions                                                */
/*--------------------------------------------------------------------------*/
#define _CONST          const

#define HIGH            1
#define LOW             0

#ifndef TRUE
	#define TRUE        1
#endif

#ifndef false
	#define false       0
#endif

#ifndef true
	#define true        1
#endif

#ifndef FALSE
	#define FALSE       0
#endif

#define YES             1
#define NO              0

#define ON              1
#define OFF             0

#define SET             1
#define CLEAR           0

#define ENABLE          1
#define DISABLE         0

#define DONE            TRUE
#define CONTINUE        FALSE

#define MUTE            1
#define DEMUTE          0

#define NOTIFY          TRUE
#define NOT_NOTIFY      FALSE

#define BACK 0x08	/* Backspace-Eingabe (HEX-Wert)	 	*/
#define RET  0x0d	/* Return-Eingabe (HEX-Wert)		  */
#define ESC  0x1b /* Escape-Eingabe (HEX-Wert)	   	*/
#define HEX	 0
#define DEC	 1

#define semP(semaphore) if (isOsStarted()) GetResource(semaphore)
#define semV(semaphore) if (isOsStarted()) ReleaseResource(semaphore)
    
#endif
