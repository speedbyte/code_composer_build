/*----------------------------------------------------------------------------*/
/* lld_SPI.h                                                11/07/06 10:40:29 */
/*                                                                            */
/* (c) Texas Instruments 2003-2006, All rights reserved.                      */
/*                                                                            */
/* Versions:                                                                  */
/* HALCOGEN         2.04.016                                                  */
/* OCSPIx470        2.00.010                                                  */
/* TICGEN           1.04.002                                                  */
/*                                                                            
|
| Subversion Info:
| $Id: lld_SPI.h 4396 2008-11-04 09:26:32Z maennel $
| $HeadURL: http://smartsvn/svn/adlatusrepository/auslieferungen/S1NN/086350-S1NN-ADLATUS-TMS470R-MucSuc/20081201_S1NN_TMS470R_BlfIso-Source_V1.00/01_SW_Projekt/05_header/ext/lld_SPI.h $
|
*/ 

#ifndef __lld_SPI_h__
#define __lld_SPI_h__

/* USER CODE BEGIN (0) */
/* USER CODE END */

/*----------------------------------------------------------------------------*/
/* OCSPI configuration                                                        */

#define OCSPI_MAX_PORT    1U
#define OCSPI_MAX_CHANNEL 1U

/* USER CODE BEGIN (1) */
/* USER CODE END */


/*----------------------------------------------------------------------------*/
/* Definitions                                                                */

#define OCSPI_0 0U

#define OCSPI_FORMAT_0 0U
#define OCSPI_FORMAT_1 1U
#define OCSPI_FORMAT_2 2U
#define OCSPI_FORMAT_3 3U

#define OCSPI_NORMAL   0U
#define OCSPI_LOOPBACK 1U

#define OCSPI_S_DATA_LENGTH_ERROR 0x01000000U
#define OCSPI_S_TIMEOUT           0x02000000U
#define OCSPI_S_PARITY_ERROR      0x04000000U
#define OCSPI_S_DESYNC            0x08000000U
#define OCSPI_S_BIT_ERROR         0x10000000U
#define OCSPI_S_TX_FULL           0x20000000U
#define OCSPI_S_RX_ERROR          0x40000000U
#define OCSPI_S_RX_EMPTY          0x80000000U

/* USER CODE BEGIN (2) */
/* USER CODE END */


/*----------------------------------------------------------------------------*/
/* Data Types                                                                 */

typedef const void * OCSPI_ConfigType;

typedef T_U16  OCSPI_SizeType;
typedef T_U32  OCSPI_ModeType;
typedef T_U32  OCSPI_ValueType;
typedef T_U32  OCSPI_DeviceStateType;
typedef T_U8 * OCSPI_MemPtrType;

/* USER CODE BEGIN (3) */
/* USER CODE END */


/*----------------------------------------------------------------------------*/
/* SPI Register Definition                                                    */

typedef volatile struct
{
    T_U32  GCR0;
    T_U8   ENA;
    T_U8   LB;
    T_U8   PD;
    T_U8   GCR1;
    T_U32  INT0;
    T_U32  LVL;
    T_U32  FLG;
    T_U32  PC0;
    T_U32  PC1;
    T_U32  PC2;
    T_U32  PC3;
    T_U32  PC4;
    T_U32  PC5;
    T_U32  PC6;
    T_U32  PC7;
    T_U32  PC8;
    T_U32  DAT0;
    T_U16  CSNR;
    T_U16  DAT1;
    T_U32  BUF;
    T_U32  EMU;
    T_U32  DELAY;
    T_U32  CSDEF;
    T_U32  FMT[4];
    T_U32  INTVECT0;
    T_U32  INTVECT1;
} T_OCSPI;

/* #define OCSPI0 ((T_OCSPI *)0xFFF7F400U) */

/*extern T_OCSPI e_SPI1_ST;*/

/* USER CODE BEGIN (4) */
/* USER CODE END */


/*----------------------------------------------------------------------------*/
/* Global Variables                                                           */

extern T_U16 g_OCSPI_size;

/* USER CODE BEGIN (5) */
/* USER CODE END */


/*----------------------------------------------------------------------------*/
/* Inline Functions                                                           */

/* OCSPI_GetVersionOfDriver */
/*static inline IO_U32 OCSPI_GetVersionOfDriver()
{
    return 0x00140100U;
}*/


/* USER CODE BEGIN (6) */
/* USER CODE END */


/*----------------------------------------------------------------------------*/
/* SPI Interface Functions                                                    */

IO_ErrorType OCSPI_InitSync(OCSPI_ConfigType config);
void OCSPI_DeInitSync(void);
IO_ErrorType OCSPI_ReadSync(IO_ChannelType channel, OCSPI_MemPtrType address, OCSPI_SizeType size);
IO_ErrorType OCSPI_WriteSync(IO_ChannelType channel, OCSPI_MemPtrType address, OCSPI_SizeType size);
IO_ErrorType OCSPI_ReadWriteSync(IO_ChannelType channel, OCSPI_MemPtrType readAddress, OCSPI_MemPtrType writeAddress, OCSPI_SizeType size);
void OCSPI_SuspendSync(IO_ChannelType channel);
IO_ErrorType OCSPI_ReadAsync(IO_ChannelType channel, OCSPI_MemPtrType address, OCSPI_SizeType size);
IO_ErrorType OCSPI_WriteAsync(IO_ChannelType channel, OCSPI_MemPtrType address, OCSPI_SizeType size);
IO_ErrorType OCSPI_ReadWriteAsync(IO_ChannelType channel, OCSPI_MemPtrType readAddress, OCSPI_MemPtrType writeAddress, OCSPI_SizeType size);
void OCSPI_SuspendAsync(IO_ChannelType channel);
OCSPI_DeviceStateType OCSPI_GetStateSync(IO_ChannelType channel);
IO_ErrorType OCSPI_SetModeSync(IO_ChannelType channel, OCSPI_ModeType mode);
IO_ErrorType OCSPI_SetFormatSync(IO_ChannelType module, IO_ChannelType format, OCSPI_ValueType value);
void OCSPI_EnableNotification(IO_ChannelType channel, OCSPI_ValueType notifType);
void OCSPI_DisableNotification(IO_ChannelType channel, OCSPI_ValueType notifType);
void OCSPI_Notification(IO_ChannelType channel, OCSPI_ValueType notifType, OCSPI_SizeType size);

IO_ErrorType OCSPI_Busy(void);

/* USER CODE BEGIN (7) */
/* USER CODE END */

#endif
/*----------------------------------------------------------------------------*/

