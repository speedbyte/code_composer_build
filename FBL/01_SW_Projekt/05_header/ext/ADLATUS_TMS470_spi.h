/*-----------------------------------------------------------------------------+
|            ____    ___            ____    _____           ____               |
|           |    |  |   \   |      |    |     |    |    |  |                   |
|           |____|  |    |  |      |____|     |    |    |  |____               |
|           |    |  |    |  |      |    |     |    |    |       |              |
|           |    |  |___/   |____  |    |     |    |____|   ____|              |
|                                                                              |
|           T H E   S M A R T   E M B E D D E D   S O L U T I O N              |
|                                                                              |
+------------------------------------------------------------------------------+
| F I L E   I N F O R M A T I O N                                              |
+------------------------------------------------------------------------------+
|                                                                              |
|  Filename:  ADLATUS_TMS470_spi.h                                             |
|                                                                              |
|  Comment:   This file contains all for SPI access                            |
|                                                                              |
+------------------------------------------------------------------------------+
| A U T H O R   I D E N T I T Y                                                |
+------------------------------------------------------------------------------+
|                                                                              |
| Initials     Name                      Company                               |
| --------     ---------------------     ----------------------------------    |
|   JW         Juergen Werner            SMART Electronic Development GmbH     |
|                                                                              |
+------------------------------------------------------------------------------+
| R E V I S I O N   H I S T O R Y                                              |
+------------------------------------------------------------------------------+
|
| Subversion Info:
| $Id: ADLATUS_TMS470_spi.h 4396 2008-11-04 09:26:32Z maennel $
| $HeadURL: http://smartsvn/svn/adlatusrepository/auslieferungen/S1NN/086350-S1NN-ADLATUS-TMS470R-MucSuc/20081201_S1NN_TMS470R_BlfIso-Source_V1.00/01_SW_Projekt/05_header/ext/ADLATUS_TMS470_spi.h $
|
+------------------------------------------------------------------------------+
|                                                                              |
|    Date       Author   Description                                           |
| -----------   ------   --------------------------------------------------    +
|  28-Jan-04      RS     Creation                                              |
|                                                                              |
+------------------------------------------------------------------------------+
|      (c) SMART ELECTRONIC DEVELOPMENT GMBH - all rights reserved             |
+-----------------------------------------------------------------------------*/

/* ========================================================================== */
/*      O P E N   M U L T I P L E   I N C L U D E   P R O T E C T I O N       */
/* ========================================================================== */
#ifndef __ADLATUS_TMS470_SPI_
#define __ADLATUS_TMS470_SPI_


/*************************************************************************/
/*  - T y p e d e f                                                      */
/*************************************************************************/
/* data type to set the working mode of the SPI interface */
typedef struct st_Mode
{
	UWORD	uw_Ctrl1;
	UWORD	uw_Ctrl2;
	UWORD	uw_Ctrl3;
}tSpiMode;

/* data type to set the working mode of the IO pins of the SPI interface */
typedef struct st_SPI_Pins
{
	UWORD	uw_PC1;
	UWORD	uw_PC3;
	UWORD	uw_PC6;
}tSpiPins;

/*************************************************************************/
/* define proccesor option */
#define PJ_PROCESSOR TMS470R1Vx348

/* define oscillator frequency options */
#define PJ_OSCI_4MHz         1u
#define PJ_OSCI_6MHz         2u
#define PJ_OSCI_8MHz         3u
#define PJ_OSCI_12MHz        4u 


/* define oscilator option */
#define PJ_OSCI  PJ_OSCI_12MHz

/* define FCPU options */
#define PJ_FCPU_16MHz   1u
#define PJ_FCPU_24MHz   2u
#define PJ_FCPU_32MHz   3u
#define PJ_FCPU_48MHz   4u
#define PJ_FCPU_64MHz   5u

/* define ICLK options */
#define PJ_ICLK_16MHz   1u
#define PJ_ICLK_24MHz   2u 
#define PJ_ICLK_32MHz   3u

#if (PJ_OSCI==PJ_OSCI_8MHz)
/* settings for 8 MHz oscilator */
#  define PJ_FCPU              PJ_FCPU_24MHz
#  define PJ_ICLK              PJ_ICLK_24MHz
#else /* (PJ_OSCI == PJ_OSCI_8MHz) */
#  if (PJ_OSCI==PJ_OSCI_12MHz)
/* settings for 12 MHz oscilator */
#    define PJ_FCPU              PJ_FCPU_24MHz
#    define PJ_ICLK              PJ_ICLK_24MHz
#  else /* (PJ_OSCI == PJ_OSCI_12MHz) */
#    error "unsupported oscillator!"
#  endif /* (PJ_OSCI == PJ_OSCI_12MHz) */
#endif /* (PJ_OSCI == PJ_OSCI_8MHz) */

/* define FCPU values */
#define FCPU_16MHz   16000000UL
#define FCPU_24MHz   24000000UL
#define FCPU_32MHz   32000000UL
#define FCPU_48MHz   48000000UL
#define FCPU_64MHz   64000000UL

/* define the FCPU */
#if (PJ_FCPU==PJ_FCPU_16MHz)
#  define FCPU FCPU_16MHz
#else /* (PJ_FCPU == PJ_FCPU_16MHz) */
#  if (PJ_FCPU==PJ_FCPU_24MHz)
#    define FCPU FCPU_24MHz
#  else /* (PJ_FCPU == PJ_FCPU_24MHz) */
#    if (PJ_FCPU==PJ_FCPU_32MHz)
#      define FCPU FCPU_32MHz
#    else /* (PJ_FCPU == PJ_FCPU_32MHz) */
#      if (PJ_FCPU==PJ_FCPU_48MHz)
#        define FCPU FCPU_48MHz
#      else /* (PJ_FCPU == PJ_FCPU_48MHz) */
#        if (PJ_FCPU==PJ_FCPU_64MHz)
#          define FCPU FCPU_64MHz
#        else /* (PJ_FCPU == PJ_FCPU_64MHz) */
#          error "unsupported system clock!"
#        endif /* (PJ_FCPU == PJ_FCPU_64MHz)*/
#      endif /* (PJ_FCPU == PJ_FCPU_48MHz) */
#    endif /* (PJ_FCPU == PJ_FCPU_32MHz) */
#  endif /* (PJ_FCPU == PJ_FCPU_24MHz) */
#endif /* (PJ_FCPU == PJ_FCPU_16MHz) */

/* define the ICLKs values */
#define ICLK_16MHz   16000000UL
#define ICLK_24MHz   24000000UL 
#define ICLK_32MHz   32000000UL

/* define the ICLK */
#if (PJ_ICLK == PJ_ICLK_16MHz)
#  define ICLK ICLK_16MHz
#else /* (PJ_ICLK == PJ_ICLK_16MHz) */
#  if (PJ_ICLK == PJ_ICLK_24MHz)
#    define ICLK ICLK_24MHz
#  else /* (PJ_ICLK == PJ_ICLK_24MHz) */
#    if (PJ_ICLK == PJ_ICLK_32MHz)
#      define ICLK ICLK_32MHz
#    else /* (PJ_ICLK == PJ_ICLK_32MHz) */
#      error "unsupported perifheral clock!"
#    endif /* (PJ_ICLK == PJ_ICLK_32MHz) */
#  endif /* (PJ_ICLK == PJ_ICLK_24MHz)*/
#endif /* (PJ_ICLK == PJ_ICLK_16MHz) */

#define SPI2_cul_EEPROM_BAUDRATE 2000000UL 

/* macro to calculate the PRESCALER value based on the ICLK and SPI baud rate */
#define SPI_PRESCALER(spi_br,iclk)     ((UWORD)(((iclk)/(spi_br)) - 1UL))

/* macros for bitfield description */
#define BIT_MASK_16(pos)   (1u<<(pos))
#define BIT_MASK_32(pos)   (1UL<<(pos)) 
/*************************************************************************/
/*  - C o n s t a n t s                                                  */
/*************************************************************************/
/* SPI Registers Bits definitions */

/* SPICTRL1 mask definition */

#define SPI_cuw_CHARLEN_MASK      0x001Fu
#define SPI_cuw_PRESCALE_MASK     0x1FE0u

/* SPI CHAR LENGTH definition */

#define SPI_cuw_LENGTH_3          0x0003u
#define SPI_cuw_LENGTH_4          0x0004u
#define SPI_cuw_LENGTH_5          0x0005u
#define SPI_cuw_LENGTH_6          0x0006u
#define SPI_cuw_LENGTH_7          0x0007u
#define SPI_cuw_LENGTH_8          0x0008u
#define SPI_cuw_LENGTH_9          0x0009u
#define SPI_cuw_LENGTH_10         0x000Au
#define SPI_cuw_LENGTH_11         0x000Bu
#define SPI_cuw_LENGTH_12         0x000Cu
#define SPI_cuw_LENGTH_13         0x000Du
#define SPI_cuw_LENGTH_14         0x000Eu
#define SPI_cuw_LENGTH_15         0x000Fu
#define SPI_cuw_LENGTH_16         0x0010u

/*************************************************************************/
/*  - M a c r o s                                                        */
/*************************************************************************/
/* SPI pins functions configuration. Default are configured as GIO */

#define SPI_cuw_SCS_PIN           BIT_MASK_16(4u)
#define SPI_cuw_SOMI_PIN          BIT_MASK_16(3u)
#define SPI_cuw_SIMO_PIN          BIT_MASK_16(2u)
#define SPI_cuw_CLK_PIN           BIT_MASK_16(1u)
#define SPI_cuw_ENABLE_PIN        BIT_MASK_16(0u)


#if (PJ_PROCESSOR == TMS470R1Vx448)
/* All definitions and settings are only valid 
   if using MIBSPI in TMS470 standard SPI compatibilty mode
   Means no MIB function is activated, MIBSPI behaves as SPI1 at an 348 device */
# define SPI_cuw_SCS0_PIN          SPI_uw_SCS_PIN
# define SPI_cuw_SCS1_PIN          BIT_MASK_16(5u)
# define SPI_cuw_SCS2_PIN          BIT_MASK_16(6u)
# define SPI_cuw_SCS3_PIN          BIT_MASK_16(7u)
# define SPI_cuw_SCS4_PIN          BIT_MASK_16(8u)
# define SPI_cuw_SCS5_PIN          BIT_MASK_16(9u)
# define SPI_cuw_SCS6_PIN          BIT_MASK_16(10u)
# define SPI_cuw_SCS7_PIN          BIT_MASK_16(11u)
#endif

/*************************************************************************/
/*  - C o n s t a n t s                                                  */
/*************************************************************************/

/* SPICTRL2 bit definition */
#define SPI_cuw_CLKMODE           BIT_MASK_16(5u)
#define SPI_cuw_SPIEN             BIT_MASK_16(4u)
#define SPI_cuw_MASTER            BIT_MASK_16(3u)
#define SPI_cuw_PWRDN             BIT_MASK_16(2u)
#define SPI_cuw_POLARITY          BIT_MASK_16(1u)
#define SPI_cuw_PHASE             BIT_MASK_16(0u)

/* SPICTRL3 bit definition */
#define SPI_cuw_ENABLE_HIGHZ      BIT_MASK_16(5u)
#define SPI_cuw_DMA_REQ           BIT_MASK_16(4u)
#define SPI_cuw_OVRN_INT          BIT_MASK_16(3u)
#define SPI_cuw_RCVR_OVRN         BIT_MASK_16(2u)
#define SPI_cuw_RX_INT            BIT_MASK_16(1u)
#define SPI_cuw_RX_INT_FLAG       BIT_MASK_16(0u)

/* Bits from SPIBUF (the MSB part - uw_Img_Reg) */
#define SPI_cuw_IMG_RCVR_OVRN     BIT_MASK_16(1u)
#define SPI_cuw_IMG_INT_FLAG      BIT_MASK_16(0u)


/* SPI pins functions configuration. Default are configured as GIO */
#define SPI_cuw_SCS_PIN           BIT_MASK_16(4u)
#define SPI_cuw_SOMI_PIN          BIT_MASK_16(3u)
#define SPI_cuw_SIMO_PIN          BIT_MASK_16(2u)
#define SPI_cuw_CLK_PIN           BIT_MASK_16(1u)
#define SPI_cuw_ENABLE_PIN        BIT_MASK_16(0u)

#if (PJ_PROCESSOR == TMS470R1Vx448)
/* All definitions and settings are only valid if using 
   MIBSPI in TMS470 standard SPI compatibilty mode
   Means no MIB function is activated, MIBSPI behaves as SPI1 at an 348 device */
# define SPI_cuw_SCS0_PIN          SPI_uw_SCS_PIN
# define SPI_cuw_SCS1_PIN          BIT_MASK_16(5u)
# define SPI_cuw_SCS2_PIN          BIT_MASK_16(6u)
# define SPI_cuw_SCS3_PIN          BIT_MASK_16(7u)
# define SPI_cuw_SCS4_PIN          BIT_MASK_16(8u)
# define SPI_cuw_SCS5_PIN          BIT_MASK_16(9u)
# define SPI_cuw_SCS6_PIN          BIT_MASK_16(10u)
# define SPI_cuw_SCS7_PIN          BIT_MASK_16(11u)
#endif

/* SPICTRL2 bits mask */
#define SPI_cuw_INTERNAL_CLOCK    (1u*SPI_cuw_CLKMODE)
#define SPI_cuw_EXTERNAL_CLOCK    (0u*SPI_cuw_CLKMODE)

#define SPI_cuw_SPI_ENABLE        (1u*SPI_cuw_SPIEN)
#define SPI_cuw_SPI_DISABLE       (0u*SPI_cuw_SPIEN)

#define SPI_cuw_SPI_MASTER        (1u*SPI_cuw_MASTER)
#define SPI_cuw_SPI_SLAVE         (0u*SPI_cuw_MASTER)

#define SPI_cuw_POWER_DOWN        (1u*SPI_cuw_PWRDN)
#define SPI_cuw_NO_POWER_DOWN     (0u*SPI_cuw_PWRDN)

#define SPI_cuw_FALLING_EDGE      (1u*SPI_cuw_POLARITY)
#define SPI_cuw_RISING_EDGE       (0u*SPI_cuw_POLARITY)

#define SPI_cuw_WITH_DELAY        (1u*SPI_cuw_PHASE)
#define SPI_cuw_NO_DELAY          (0u*SPI_cuw_PHASE)

/* SPICTRL3 bits mask */
#define SPI_cuw_TRISTATE_EN       (1u*SPI_cuw_ENABLE_HIGHZ)
#define SPI_cuw_TRISTATE_DIS      (0u*SPI_cuw_ENABLE_HIGHZ)

#define SPI_cuw_DMA_REQ_EN        (1u*SPI_cuw_DMA_REQ)
#define SPI_cuw_DMA_REQ_DIS       (0u*SPI_cuw_DMA_REQ)

#define SPI_cuw_OVR_INT_EN        (1u*SPI_cuw_OVRN_INT)
#define SPI_cuw_OVR_INT_DIS       (0u*SPI_cuw_OVRN_INT)

#define SPI_cuw_RX_INT_EN         (1u*SPI_cuw_RX_INT)
#define SPI_cuw_RX_INT_DIS        (0u*SPI_cuw_RX_INT)

/* SPI reset/disable bits */
#define SPI_cuw_SW_RESET          SPI_cuw_SPI_ENABLE

#define SPI_cuw_RX_INT_ENABLE     SPI_cuw_RX_INT_EN

/* macro to define one pin as input or output */
#define SPI_PIN_INPUT(pin)        (0u*(pin))
#define SPI_PIN_OUTPUT(pin)       (1u*(pin))

/* macros to ser or clear a pin */
#define SPI_PIN_CLEAR(pin)        (0u*(pin))
#define SPI_PIN_SET(pin)          (1u*(pin))

/* macros to define pin as GOI or as functional */
#define SPI_PIN_GIO(pin)          (0u*(pin))
#define SPI_PIN_FUNCTIONAL(pin)   (1u*(pin))



/*************************************************************************/
/*  - V a r i a b l e s                                                  */
/*************************************************************************/





/*************************************************************************/
/*  - F l a g s / B i t  M a s k s                                       */
/*************************************************************************/



/*************************************************************************/
/*  - F u n c t i o n  P r o t o t y p e s                               */
/*************************************************************************/

/* exported function prototypes */
extern void SPI_v_Init( void );

UBYTE SpiEepromBusy(void);

void SpiEepromReceive(UBYTE ubBlockSize, UBYTE *ubResponse);

void SpiEepromSend(UBYTE ubBlockSize, const UBYTE *ubMessage);

#endif

/* ========================================================================== */
/*      E N D   O F   F I L E                                                 */
/* ========================================================================== */

