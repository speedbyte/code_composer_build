/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/
/** 
 * \file 	ADC_C1.c
 * \brief	This file consists of functions to initialise ADC, to read the ADC 
 * channels, trigger to start the conversion of the configured channels. Detailed 
 * description found below.
 * \brief	Detailed description of the file.
*/
/*-----------------------------------------------------------------------------
* $Date: 2009-06-03 16:49:48 +0200 (Mi, 03 Jun 2009) $
* $Rev: 6457 $
* $Author: agrawal $
* $URL: http://subversion-01/svn/SW_HW_System/Plattform/TMS470/MOD/MOD_ADC/trunk/src/ADC_C1.c $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "ADC.h"
#include "ADC_PL.h"

/*----------------------------------------------------------------------------*/
/* pragma Directives                                                          */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Definitions (avoid as much as possible!)                  */
/*----------------------------------------------------------------------------*/
/** 
    \brief	static block containing ADC converted values.
*/
static 	Uint16Type ADC_au16Block[ADC_NUMBER_OF_CHANNELS];
/*----------------------------------------------------------------------------*/
/* Private Manifest Constant Definitions                                      */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private Types                                                              */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Prototypes of file local Methods                                           */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* File local Code Macros                                                     */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* File local Variables                                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* File local const Definitions                                               */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private Address Calculations                                               */
/*----------------------------------------------------------------------------*/
/* Function    : ADC_sfInit                                         		  */
/** 
    \brief      Initialise the ADC
  
    \brief      Description:
                Initialise the ADC to a correct state. 

    \param		pstAdc
    			ADC_PST
    			
    \return     SuccessFailType
                If the function was successful or failed
                SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType ADC_sfInit(ADC_PST pstAdc)
{
	SuccessFailType sf = SUCCESS;
	
	ADC_SWResetEnter_V(pstAdc);

	sf = ADC_sfConfigureInitRegister(pstAdc);
	
    ADC_vConfigureInterrupt(pstAdc, Adc_enSWGp1, DISABLE);
    ADC_vConfigureInterrupt(pstAdc, Adc_enSWGp2, DISABLE);
    ADC_vConfigureInterrupt(pstAdc, Adc_enEvent, DISABLE);
 
    ADC_SWResetExit_V(pstAdc);
    
    (void)memset(ADC_au16Block,  0x0, ADC_NUMBER_OF_CHANNELS*2);

    return sf;
}
/*----------------------------------------------------------------------------*/
/* Function    : ADC_u16GetAdcValue                                    		  */
/** 
    \brief      Get digital value on the channel.
  
    \brief      Description:
                Get the converted ADC Value. 

    \param      pstAdc
    			ADC_PST
                ADC1 
    \param		SelChn_N
    			Uint32Type
    			Channels from 0 to 15
				For each bit set in ADISR1, one channel is selected for conversion when
				ADISR1 is written. For example, if bits 11, 3, and 1 are to be converted, then
				when the value 0000100000001010 binary (0x080A) is written to ADISR1,
				group 1 conversions start. Channel 1 is converted first, then channel 3, then
				channel 11.    			
    \return     Uint16Type
                the value at the channel.
*/
/*----------------------------------------------------------------------------*/
Uint16Type ADC_u16GetAdcValue(const ADC_PST pstAdc, Uint32Type SelChn_N)
{
	Uint8Type counter = 0;
	if (SelChn_N == 0)
		return 0xFFFF;
	while ( (SelChn_N >> (++counter)) != 0 ) { // to find out which channel was
	}
                                              // requested to be read.
	if (counter > ADC_NUMBER_OF_CHANNELS)
		return 0xFFFF;
	return ADC_au16Block[counter-1];
}

/*----------------------------------------------------------------------------*/
/* Function    : ADC_sfConvStart                                         */
/** 
    \brief      Start Conversion
  
    \brief      Description:
                Start conversion of all channels defined in ADC_CE.h 
                The converted value is stored in ADC_au16Block.

    \param      pstAdc
    			ADC_PST
                ADC1 
    \return     SuccessFailType
                If the function was successful or failed
                SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType ADC_sfConvStart(ADC_PST pstAdc)
{
	SuccessFailType sf = SUCCESS;
	Uint8Type count = ADC_NUMBER_OF_CHANNELS; // no of channels in ADC
	if ( ADC_CONFIGURED_CHANNELS == 0 )
		return FAIL;
	else
	{
		sf = ADC_sfConvStart_PL(pstAdc, ADC_CONFIGURED_CHANNELS, Adc_enSWGp1, AdcPoll);
		if ( sf == SUCCESS)
		{
			while ( count-- > 0 )    
			{
				if ( (ADC_CONFIGURED_CHANNELS >> count) & 0x0001)
					ADC_au16Block[count] = ADC_u16ReadChannel(pstAdc, count);
			}
		}
	}
    return SUCCESS;		
}

/* End Of File ADC_C1.c */
