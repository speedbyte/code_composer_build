/*----------------------------------------------------------------------------*/
/* lld_SPI.c                                                11/07/06 10:40:29 */
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
| $Id: lld_SPI.c 4508 2008-11-28 12:28:20Z maennel $
| $HeadURL: http://smartsvn/svn/adlatusrepository/auslieferungen/S1NN/086350-S1NN-ADLATUS-TMS470R-MucSuc/20081201_S1NN_TMS470R_BlfIso-Source_V1.00/01_SW_Projekt/01_adlatus/02_src/lld_SPI.c $
|
*/                                                              

#include "dtyp.h"
#include "lld.h"
#include "lld_SPI.h"

#include "ADLATUS_Types_tdf.h"

#include "tms470regs.h"

#include "ADLATUS_TMS470_SPI.h"
#include "ADLATUS_Eep.h"
#include "ADLATUS_Timer_tdf.h"

/*----------------------------------------------------------------------------*/

/* USER CODE BEGIN (0) */

/* ========================================================================== */
/* (9)  G L O B A L   V A R I A B L E S                                       */
/* -------------------------------------------------------------------------- */
/*      D E F I N E D   W I T H I N   T H I S   M O D U L                     */
/* ========================================================================== */
/* NO ENTRY                                                                   */
/* constant structure to initialize the EEPROM Mode */
const tSpiMode SPI2_st_EepromMode =
{
	SPI_cuw_LENGTH_8       |\
	((SPI_PRESCALER(SPI2_cul_EEPROM_BAUDRATE,ICLK)) << 5u),

	SPI_cuw_INTERNAL_CLOCK |\
	SPI_cuw_SPI_ENABLE     |\
	SPI_cuw_SPI_MASTER     |\
	SPI_cuw_NO_POWER_DOWN  |\
	SPI_cuw_RISING_EDGE    |\
	SPI_cuw_WITH_DELAY,
	
	SPI_cuw_TRISTATE_DIS   |\
	SPI_cuw_DMA_REQ_DIS    |\
	SPI_cuw_OVR_INT_DIS    |\
	SPI_cuw_RX_INT_DIS
}; 

const tSpiPins  SPI2_st_Pins =
{
	SPI_PIN_OUTPUT( SPI_cuw_ENABLE_PIN )   |\
	SPI_PIN_OUTPUT( SPI_cuw_CLK_PIN )      |\
	SPI_PIN_OUTPUT( SPI_cuw_SIMO_PIN )     |\
	SPI_PIN_INPUT ( SPI_cuw_SOMI_PIN )     |\
	SPI_PIN_OUTPUT( SPI_cuw_SCS_PIN ),
	
	SPI_PIN_SET( SPI_cuw_ENABLE_PIN )      |\
	SPI_PIN_SET( SPI_cuw_CLK_PIN )         |\
	SPI_PIN_SET( SPI_cuw_SIMO_PIN )        |\
	SPI_PIN_SET( SPI_cuw_SOMI_PIN )        |\
	SPI_PIN_SET( SPI_cuw_SCS_PIN ),
	
	SPI_PIN_GIO( SPI_cuw_ENABLE_PIN )      |\
	SPI_PIN_FUNCTIONAL( SPI_cuw_CLK_PIN )  |\
	SPI_PIN_FUNCTIONAL( SPI_cuw_SIMO_PIN ) |\
	SPI_PIN_FUNCTIONAL( SPI_cuw_SOMI_PIN ) |\
	SPI_PIN_GIO( SPI_cuw_SCS_PIN)
};    
/* ========================================================================== */
/* (10) F U N C T I O N   D E F I N I T I O N S                               */
/* -------------------------------------------------------------------------- */
/*      F O R   T H I S   M O D U L                                           */
/* ========================================================================== */
inline void SPI_v_SetMode(const tSpiMode *pst_SpiMode );

inline void SPI_v_SetMode(const tSpiMode *pst_SpiMode )
{
	/* set the baud rate and the character size */
	e_SPI2_ST.ctrl1_UN.ctrl1_UW.uw_Ctrl1 = pst_SpiMode->uw_Ctrl1;
	/* Set SPI working mode */
	e_SPI2_ST.ctrl2_UN.ctrl2_UW.uw_Ctrl2 = pst_SpiMode->uw_Ctrl2;
}
/* USER CODE END */

 T_U8 g_buf_UB;
/*----------------------------------------------------------------------------*/
/* Global Variables                                                           */

/*----------------------------------------------------------------------------*/
/* OCSPI_InitSync                                                             */

/* USER CODE BEGIN (1) */
/* USER CODE END */

IO_ErrorType OCSPI_InitSync(OCSPI_ConfigType config)
{
/* USER CODE BEGIN (2) */
/* USER CODE END */

  /* reset the hardware */
  e_SPI2_ST.ctrl2_UN.ctrl2_UW.uw_Ctrl2 &= ~SPI_cuw_SPIEN;

  /* Disable interrupts during registers setup */
  e_SPI2_ST.ctrl3_UN.ctrl3_UW.uw_Ctrl3 &= ~SPI_cuw_RX_INT_ENABLE;

  /* set special funitionality for the following pins */
  e_SPI2_ST.pc6_UN.pc6_UW.uw_PC6 = SPI2_st_Pins.uw_PC6;
  /* set the output pins with 1 level*/
  e_SPI2_ST.pc3_UN.pc3_UW.uw_PC3 = SPI2_st_Pins.uw_PC3;
  /* initialise the functions of the port pins */
  e_SPI2_ST.pc1_UN.pc1_UW.uw_PC1 = SPI2_st_Pins.uw_PC1;

  /* set the baud rate and the character size */
  e_SPI2_ST.ctrl1_UN.ctrl1_UW.uw_Ctrl1 = SPI2_st_EepromMode.uw_Ctrl1;
  /* set the interrupt status */
  e_SPI2_ST.ctrl3_UN.ctrl3_UW.uw_Ctrl3 = SPI2_st_EepromMode.uw_Ctrl3;
  /* Set SPI operation mode */
  e_SPI2_ST.ctrl2_UN.ctrl2_UW.uw_Ctrl2 = SPI2_st_EepromMode.uw_Ctrl2; 
  /* finaly exit from the reset state */
  e_SPI2_ST.ctrl2_UN.ctrl2_UW.uw_Ctrl2 |= SPI_cuw_SPIEN;

/* USER CODE BEGIN (3) */
/* USER CODE END */

    return IO_E_OK;
}


/*----------------------------------------------------------------------------*/
/* OCSPI_DeInitSync                                                           */

/* USER CODE BEGIN (4) */
/* USER CODE END */

void OCSPI_DeInitSync(void)
{
/* USER CODE BEGIN (5) */
/* USER CODE END */

    /* reset SPI module 0 */
    e_SPI2_ST.ctrl2_UN.ctrl2_UW.uw_Ctrl2 &= ~SPI_cuw_SPIEN;


/* USER CODE BEGIN (6) */
/* USER CODE END */
}

IO_ErrorType OCSPI_Busy(void)
{
  IO_ErrorType t_Temp_UB;

  SPI_v_SetMode(&SPI2_st_EepromMode );
  /* activate EEPROM Chip Select */
  e_SPI2_ST.pc5_UN.pc5_UW.uw_PC5 = SPI_cuw_SCS_PIN;


  /* clear flags */
  g_buf_UB = e_SPI2_ST.uw_Buf;

  /* write instruction to send in register */
  e_SPI2_ST.uw_Dat0 = EEPROM_COM_STATUS_READ;
  
  /* waiting as long as the charater has not been send */
  while ((e_SPI2_ST.ctrl3_UN.ctrl3_UW.uw_Ctrl3 & 0x0001u) == 0u)
  {
    FUN_CCtim_SystemTimerCheck_V ();
  }

  /* clear flags */
  g_buf_UB = e_SPI2_ST.uw_Buf;
  
  /* write dummy */
  e_SPI2_ST.uw_Dat0 = 0u;
  
  /* waiting as long as the charater has not been send */
  while ((e_SPI2_ST.ctrl3_UN.ctrl3_UW.uw_Ctrl3 & 0x0001u) == 0u)
  {
    FUN_CCtim_SystemTimerCheck_V ();
  }

  /* clear flags */
  g_buf_UB = e_SPI2_ST.uw_Buf;
  if ((0x01 & g_buf_UB) == 0x01)
  {
    t_Temp_UB = IO_E_BUSY;
  }
  else
  {
    t_Temp_UB = IO_E_OK;
  }  

  /* deactivate EEPROM Chip Select */
  e_SPI2_ST.pc4_UN.pc4_UW.uw_PC4 = SPI_cuw_SCS_PIN;

  /* Return write in progress bit of EEPROM; it is set when EEPROM is busy writing data */
  return(t_Temp_UB);
}

/*----------------------------------------------------------------------------*/
/* OCSPI_WriteSync                                                        */

/* USER CODE BEGIN (13) */
/* USER CODE END */

IO_ErrorType OCSPI_ReadSync(IO_ChannelType channel, OCSPI_MemPtrType readAddress, OCSPI_SizeType size)
{
  SPI_v_SetMode(&SPI2_st_EepromMode );
  /* activate EEPROM Chip Select */
  e_SPI2_ST.pc5_UN.pc5_UW.uw_PC5 = SPI_cuw_SCS_PIN;

  /* clear flags */
  g_buf_UB = e_SPI2_ST.uw_Buf;

  /* write instruction to send in register */
  e_SPI2_ST.uw_Dat0 = *readAddress;
  
  readAddress++;  
  size--;

  /* waiting as long as the charater has not been send */
  while ((e_SPI2_ST.ctrl3_UN.ctrl3_UW.uw_Ctrl3 & 0x0001u) == 0u) 
  {
    FUN_CCtim_SystemTimerCheck_V ();
  }

  /* clear flags */
  g_buf_UB = e_SPI2_ST.uw_Buf;
  
  /* write Address(es) */
  e_SPI2_ST.uw_Dat0 = *readAddress;
  
  /* waiting as long as the charater has not been send */
  while ((e_SPI2_ST.ctrl3_UN.ctrl3_UW.uw_Ctrl3 & 0x0001u) == 0u)  
  {
    FUN_CCtim_SystemTimerCheck_V ();
  }

  /* clear flags */
  g_buf_UB = e_SPI2_ST.uw_Buf;
    
  readAddress++;  
  size--;    

  /* write Address(es) */
  e_SPI2_ST.uw_Dat0 = *readAddress;
  
  /* waiting as long as the charater has not been send */
  while ((e_SPI2_ST.ctrl3_UN.ctrl3_UW.uw_Ctrl3 & 0x0001u) == 0u)
  {   
    FUN_CCtim_SystemTimerCheck_V ();
  }

  /* clear flags */
  g_buf_UB = e_SPI2_ST.uw_Buf;
    
  readAddress++;  
  size--;    

  for(;size>0;size--)
  {
    /* initialise the register */
    /* write dummy */

    e_SPI2_ST.uw_Dat0 = 0u;

    /* wait as long as the reception is not ended */
    while ((e_SPI2_ST.ctrl3_UN.ctrl3_UW.uw_Ctrl3 & 0x0001u) == 0u)  
    {
      FUN_CCtim_SystemTimerCheck_V ();
    }

    /* clear interrupts, read buffer */
    *readAddress = e_SPI2_ST.uw_Buf;
        
    readAddress++;
  }
         
  /* deactivate EEPROM Chip Select */
	e_SPI2_ST.pc4_UN.pc4_UW.uw_PC4 = SPI_cuw_SCS_PIN;

  return IO_E_OK;
}
/*************************************************************************/
IO_ErrorType OCSPI_WriteSync(IO_ChannelType channel, OCSPI_MemPtrType writeAddress, OCSPI_SizeType size)
{

  SPI_v_SetMode(&SPI2_st_EepromMode );
  /* activate EEPROM Chip Select */
  e_SPI2_ST.pc5_UN.pc5_UW.uw_PC5 = SPI_cuw_SCS_PIN;
      
  /* clear flags */
  g_buf_UB = e_SPI2_ST.uw_Buf;
  
  for(;size > (UBYTE) 0x00u;size--)
  {
    /* write character to send in register */
    e_SPI2_ST.uw_Dat0 = *writeAddress;

    writeAddress++;

    /* waiting as long as the charater has not been send */
    while ((e_SPI2_ST.ctrl3_UN.ctrl3_UW.uw_Ctrl3 & 0x0001u) == 0u)
    { 
      FUN_CCtim_SystemTimerCheck_V ();
    }

    /* clear flags */
    g_buf_UB = e_SPI2_ST.uw_Buf;
  }
  /* deactivate EEPROM Chip Select */
	e_SPI2_ST.pc4_UN.pc4_UW.uw_PC4 = SPI_cuw_SCS_PIN;

  return IO_E_OK;
}


/*----------------------------------------------------------------------------*/

