/******************************************************************************/
/*****************  Copyright (c) 2007 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/
/** 
 * \file 	ADC_PL_C1.c
 * \brief	This file consists of functions that are used internally by 
 * the modules to initialise ADC, to read the ADC channels, trigger to start 
 * the conversion of the configured channels etc. Detailed description found below.
 * \brief	Detailed description of the file.
*/
/*-----------------------------------------------------------------------------
 * $Date: 2009-06-03 16:49:48 +0200 (Mi, 03 Jun 2009) $
 * $Rev: 6457 $
 * $Author: agrawal $
 * $URL: http://subversion-01/svn/SW_HW_System/Plattform/TMS470/MOD/MOD_ADC/trunk/src/ADC_PL_C1.c $
 *--------------------------------------------------------------------------- */
/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include "ADC.h"
#include "ADC_PL.h"
/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* pragma Directives                                                          */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Definitions (avoid as much as possible!)                  */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global const Definitions                                                   */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private Manifest Constant Definitions                                      */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private Types                                                              */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Prototypes of private Methods                                              */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private Code Macros                                                        */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private Variables                                                          */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private const Definitions                                                  */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private Address Calculations                                               */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Function    : ADC_SWResetEnter_V                                      */
/** 
    \brief      Enter Reset Mode of ADC Peripheral
  
    \brief      Description:
                This function enters into the Reset Mode of the peripheral.  
                In the reset mode, the functionality of the ADC is disabled.

    \param      pstAdc
    			ADC_PST
                ADC 
    \return     void
*/
/*----------------------------------------------------------------------------*/
void ADC_SWResetEnter_V(ADC_ST *pstAdc)
{
    /* Reset the ADC */
    pstAdc->ADCR1_UN.ADCR1_UL &=  ~(1<<5);  /* ADCEN */
    // 1 = ENABLE, 0 = DISABLE
    /* Disable EndOfGroup interrupts for all groups */
    pstAdc->ADCR2_UN.ADCR2_UL &= ~((1<<9)|(1<<3)|(1<<0));
    /* disable DMA and FIFO OVR ints and threshold ints */
	pstAdc->ADBCR3_UN.ADBCR3_UL &= 0x000F;
    /* GPx END flags are automatically cleared by the ADC reset */ 
    return;
}

/*----------------------------------------------------------------------------*/
/* Function    : ADC_SWResetExit_V                                      */
/** 
    \brief      Exit Reset Mode of ADC Peripheral
  
    \brief      Description:
                This function exits into the Reset Mode of the peripheral.  
                In the reset exit mode, the functionality of the ADC is enabled .

    \param      pstAdc
    			ADC_PST
                ADC 
    \return     void
*/
/*----------------------------------------------------------------------------*/
void ADC_SWResetExit_V(ADC_ST *pstAdc)
{
    /* Release the HW reset of the module */
    pstAdc->ADCR1_UN.ADCR1_UL |=  (1<<5);  /* ADCEN */
     // 1 = ENABLE, 0 = DISABLE
    return;
}

/*----------------------------------------------------------------------------*/
/* Function    : ADC_ConfigureInitRegister                                      */
/** 
    \brief      Configure Init Register - ADCR1
  
    \brief      Description:
                This function configures the ADC into a working state..  

    \param      pstAdc
    			ADC_PST
                ADC 
    \return     SuccessFailType
*/
/*----------------------------------------------------------------------------*/
SuccessFailType ADC_sfConfigureInitRegister(ADC_ST *pstAdc)
{
    SuccessFailType sf = SUCCESS;
	union madcr1  Reg1_UN;
    if ( pstAdc != ADC1 ){
    	return FAIL;
	}
	Reg1_UN.ADCR1_ST.CALEN_B1     = 0;   /* Avoid bad value in Calibration Enable bit*/
    Reg1_UN.ADCR1_ST.CALSTR_B1    = 0;   /* Avoid bad value in Calibration Start bit */
    Reg1_UN.ADCR1_ST.HILO_B1      = 0;   /* Avoid bad value in HILO bit location     */
    Reg1_UN.ADCR1_ST.BRIDGEN_B1   = 0;   /* Setting correct value in BRIDGEN bit     */
    Reg1_UN.ADCR1_ST.FSTEST_B1    = 0;   /* Avoid bad value in FSTest bit location   */
    Reg1_UN.ADCR1_ST.ADCENA_B1    = 0;   /* Setting correct value in SWRESET bit     */
    Reg1_UN.ADCR1_ST.ACQ_B2       = (REG_32)ADC_ACQUISITION_PRESCALER;
    Reg1_UN.ADCR1_ST.PS_B3        = (REG_32)ADC_CLOCK_PRESCALER;
    pstAdc->ADCR1_UN.ADCR1_UL   =  Reg1_UN.ADCR1_UL;
    return sf;
}

/*----------------------------------------------------------------------------*/
/* Function    : ADC_sfConvStart_PL                                           */
/** 
    \brief      Start Conversion
  
    \brief      Description:
                Start conversion of all channels defined in ADC_CE.h 

    \param      pstAdc
    			ADC_PST
                ADC1 
    \param      SelChn_N
    			Uint32Type
                AdcChn0 to 15, Associate to the specified group channels to be converted.
                For each bit set in ADISR1, one channel is selected for conversion when
				ADISR1 is written. For example, if bits 11, 3, and 1 are to be converted, then
				when the value 0000100000001010 binary (0x080A) is written to ADISR1,
				group 1 conversions start. Channel 1 is converted first, then channel 3, then
				channel 11.
    \param      GrpId_N
    			Uint32Type
                AdcSWGp1 to 2, An Identifier of the selected group 
    \param      SelMode_N
    			MBADCMODESEL
                AdcPoll  , selects one of three modes; interrupt, dma or polling mode                                
    \return     SuccessFailType
                If the function was successful or failed
                SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType ADC_sfConvStart_PL(ADC_PST pstAdc, MBADCCHNSEL  SelChn_N,  ADC_EN_FUNCTIONS GrpId_N, MBADCMODESEL SelMode_N)
{
	SuccessFailType sF = SUCCESS;
	static Uint8Type      tempTimeoutCount = 0;
	static Uint8Type conversionStarted = FALSE;
	if(SelMode_N == AdcPoll) /*-----------------Polling Mode--------------*/
    {
        switch (GrpId_N)
        {
           case Adc_enSWGp1:
        	   if ( FALSE == conversionStarted )
        	   {
        		   pstAdc->ADISR1_UW = (REG_16)SelChn_N;   /* start conversion */
        		   conversionStarted = TRUE;
        	   }
        	   if  (0 == (pstAdc->ADSR_UN.ADSR_UL & (1<<2)))  /*a 0 in this register dentotes conversion is going on*/
	           {
        		   sF = FAIL; // Conversion is going on.
        	        if(tempTimeoutCount++ == ADC_MAXTIMEOUT)
        	        {
        	            conversionStarted = FALSE;   // Try the conversion again after Timeout. 
        	            tempTimeoutCount = 0;
        	            pstAdc->ADISR1_UW = 0; // Abort the current conversion. A new conversion will be started.
        	        }
	           }
        	   else if ( 1 == (pstAdc->ADSR_UN.ADSR_UL & (1<<2))) /*a 1 in this register dentotes end of conversion GPIEND*/
        	   {
        		   conversionStarted = FALSE;
        	   }
        	   break;

           case Adc_enSWGp2:
        	   /* To maintain simplicity, Group2 is not being used as of now. in case of
        	    * use, code has to be written here and corresponding interfaces fixed */ 
                break;
         
	
           case Adc_enEvent:
        	   /* To maintain simplicity, ADC Events are not being used as of now. in case of
        	    * use, code has to be written here and corresponding interfaces fixed */ 
                break;
           default:
                break;
        }
    }
	else if(SelMode_N == AdcWro)/*------------Conversion Only  Mode--------*/
	{
        switch (GrpId_N)
        {
           case Adc_enSWGp1:

              pstAdc->ADCR2_UN.ADCR2_UL &= ~(1<<3); /* disable ENAG1INT */  
              pstAdc->ADISR1_UW = (REG_16)SelChn_N;
              break;
            	
           case Adc_enSWGp2:

              pstAdc->ADCR2_UN.ADCR2_UL &= ~(1<<0); /* disable ENAG2INT */  
              pstAdc->ADISR2_UW = (REG_16)SelChn_N;
              break;
           	
           case Adc_enEvent:

              pstAdc->ADCR2_UN.ADCR2_UL &= ~(1<<9); /* disable ENAEVINT */  
              pstAdc->ADEISR_UW = (REG_16)SelChn_N;
              break;

           default:
              break;
        }
	}	
    return sF;		
}

/*----------------------------------------------------------------------------*/
/* Function    : ADC_u16ReadChannel                                    		  */
/** 
    \brief      Get the converted value from the ADC Hw Buffer.
  
    \brief      Description:
                Initialise the ADC to a correct state. 

    \param      pstAdc
    			ADC_PST
                ADC1 
    \param		SelChn_N
    			Uint32Type
    			Channels from 0 to 15
    
    \return     Uint16Type
                the value at the channel.
*/
/*----------------------------------------------------------------------------*/
Uint16Type ADC_u16ReadChannel(const ADC_PST pstAdc, Uint32Type  SelChn_N)
{
	return ((Uint16Type)(pstAdc->ADDRREG[SelChn_N].ADDR_UL) & (Uint32Type)0x000003FF) ;
}

/*----------------------------------------------------------------------------*/
/* Function    : ADC_vConfigureInterrupt                                */
/** 
    \brief      Enable or Disable interrupt.
  
    \brief      Description:
    			Enable or Disable Interrupts for 
    			Group1 conversion end.
    			Group2 conversion end.
    			Group event conversion end.

    \param      pstAdc
    			ADC_ST
                UART1 / UART2 
    \param      u8Location
    			ADC_EN_FUNCTIONS ( enum )
    			Adc_enSWGp1,    Adc_enSWGp2,    Adc_enEvent
    \param      u8Value
    			Uint8Type
                Value between 0 an 255. 
    \return     void
*/
/*----------------------------------------------------------------------------*/
void ADC_vConfigureInterrupt(ADC_ST *pstAdc, ADC_EN_FUNCTIONS u8Location, Uint8Type u8Value)
{
	if (Adc_enSWGp1 == u8Location)
	{
		pstAdc->ADCR2_UN.ADCR2_ST.ENAGP1INT_B1 = u8Value; /* disable ENAG1INT */
	}
	else if (Adc_enSWGp2 == u8Location)
	{
		pstAdc->ADCR2_UN.ADCR2_ST.ENAGP1INT_B1 = u8Value; /* disable ENAG1INT */
	}
	else if (Adc_enEvent == u8Location)
	{
		pstAdc->ADCR2_UN.ADCR2_ST.ENAEVINT_B1 = u8Value; /* disable ENAG1INT */	
    }
	return;
}
/* End Of File ADC_PL_C1.c */
