/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/
/** 
 * \file 	GIO_PL_C1.c
 * \brief	This file consists of functions that are used internally by 
 * the modules to initialise GIO Ports, to read the GIO channels, and write the
 * GIO channels. Detailed description found below.
 * \brief	Detailed description of the file.
*/
/*-----------------------------------------------------------------------------
* $Date: 2009-05-29 09:25:06 +0200 (Fr, 29 Mai 2009) $
* $Rev: 6395 $
* $Author: rammelt $
* $URL: http://subversion-01/svn/SW_HW_System/Plattform/TMS470/MOD/MOD_GIO/trunk/src/GIO_PL_C1.c $
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
#include "GIO.h"

/*----------------------------------------------------------------------------*/
/* pragma Directives                                                          */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Definitions (avoid as much as possible!)                  */
/*----------------------------------------------------------------------------*/
static GIO_ST *pstGio    = (GIO_PST)TMS470R1_BASEADDRESS_GIO;
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
/* -------------------------------------------------------------------------  */
/* CallBackLow_PV                                                             */
/*      Pointer to interrupt call back routines, they must be initalised by   */
/*      a call to GIO_IntGlobalInit_V before any of the interrupts are        */
/*      enabled.                                                              */
/*                                                                            */

static void (*CallBackLow_PV)(Uint8Type);


/* -------------------------------------------------------------------------  */
/* CallBackHigh_PV                                                            */
/*      Pointer to interrupt call back routines, they must be initalised by   */
/*      a call to GIO_IntGlobalInit_V before any of the interrupts are        */
/*      enabled.                                                              */
/*                                                                            */

static void (*CallBackHigh_PV)(Uint8Type);
/*----------------------------------------------------------------------------*/
/* File local Code Macros                                                     */
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

/*----------------------------------------------------------------------------*/
/* Private Address Calculations                                               */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Function    : GIO_vPinModify                                */
/** 
    \brief      Modify functionaly of one or more pins of a given port
  
    \brief      Description:\n
				Detail: Effected Register: GIODIR
				0 Output buffer is disabled.
				1 Output buffer is enabled. 

    \param      PinMask_N
    			Uint32Type
    			pins to be modified    
    \param      PinQual_N
    			Uint32Type
    			operation to be performed                          
    \param      Dev_N
    			Uint8Type
				port to be modified
				
    \return     void
*/
/*----------------------------------------------------------------------------*/
void GIO_vPinModify(Uint8Type Dev_N, Uint32Type PinMask_N, PORTQUAL PinQual_N )
{
    Uint8Type u8PortValue;
    u8PortValue = (Uint8Type)pstGio->Port_ST[Dev_N].Dout_UL; 
	if (PortIoIn == PinQual_N)
    {
        pstGio->Port_ST[Dev_N].Dir_UL  &= ~(REG_32)PinMask_N;
    }
    else if (PortIoOut == PinQual_N)
    {
        pstGio->Port_ST[Dev_N].Dir_UL  |= (REG_32)PinMask_N;
    }
    else if (PortIoOutHigh == PinQual_N)
    {
    	u8PortValue |= PinMask_N;
        pstGio->Port_ST[Dev_N].Dir_UL  |= (REG_32)PinMask_N;
        pstGio->Port_ST[Dev_N].Dout_UL = u8PortValue;
    }
    else if (PortIoOutLow == PinQual_N)
    {
    	u8PortValue &= ~PinMask_N;
        pstGio->Port_ST[Dev_N].Dir_UL  |= (REG_32)PinMask_N;
        pstGio->Port_ST[Dev_N].Dout_UL = u8PortValue;
    }
    else
    {
    }
    return;
}
/*----------------------------------------------------------------------------*/
/* Function    : GIO_u32PortRead                                			  */
/** 
    \brief      This function gets the value from the Data
				input Register ( GIODIN )
  
    \brief      Description:\n
				Detail: Effected Register: GIODIN

    \param      Dev_N
    			Uint8Type
				port to be read
				
    \return     Uint32Type
    			TRUE if one or more if the bits is set, otherwise FALSE.
*/
/*----------------------------------------------------------------------------*/
Uint32Type GIO_u32PortRead(Uint8Type Dev_N)
{
    return (pstGio->Port_ST[Dev_N].Din_UL) & PORTMASK;
}
/*----------------------------------------------------------------------------*/
/* Function    : GIO_PinRead_B                                				  */
/** 
    \brief      This function gets the value from the Data
				input Register ( GIODIN )
  
    \brief      Description:\n
				Detail: Effected Register: GIODIN

    \param      PinMask_N
    			Uint8Type
				port to be modified
    \param      Dev_N
    			Uint8Type
				port to be read
				
    \return     Uint8Type
    			TRUE if one or more if the bits is set, otherwise FALSE.
*/
/*----------------------------------------------------------------------------*/
Uint8Type GIO_u8PinRead(Uint32Type PinMask_N, Uint8Type Dev_N)
{
	return pstGio->Port_ST[Dev_N].Din_UL & (REG_32)PinMask_N ? TRUE : FALSE;
}
/*----------------------------------------------------------------------------*/
/* Function    : GIO_vPinWrite                                                */
/** 
    \brief      Write to a single Port pin 
  
    \brief      Description:\n
				Detail: Effected Register: GIODIR
				0 Output buffer is disabled.
				1 Output buffer is enabled. 

    \param      PinMask_N
    			Uint32Type
    			Pin to be written to    
    \param      PinData_B
    			Uint8Type
    			New value for specified pin                          
    \param      Dev_N
    			Uint8Type
				port to be written
				
    \return     void
*/
/*----------------------------------------------------------------------------*/
void GIO_vPinWrite(Uint32Type PinMask_N, Uint8Type PinData_B, Uint8Type Dev_N)
{
	Uint8Type u8PortValue;
	u8PortValue = (Uint8Type)pstGio->Port_ST[Dev_N].Dout_UL; 
	if (PinData_B)
	{
		u8PortValue |= PinMask_N;
	}
	else
	{
		u8PortValue &= ~PinMask_N;
	}
	pstGio->Port_ST[Dev_N].Dout_UL = u8PortValue;
}
/*----------------------------------------------------------------------------*/
/* Function    : GIO_vPortSet                                */
/** 
    \brief      Set the Output port. 
  
    \brief      Description:\n
				This function sets the Data output register
				Effected Reg: GIODSET

    \param      PinMask_N
    			Uint32Type
    			Pin to be written to    
    \param      Dev_N
    			Uint8Type
				port to be written
				
    \return     void
*/
/*----------------------------------------------------------------------------*/
void GIO_vPortSet(Uint32Type PinMask_N, Uint8Type Dev_N)
{
	pstGio->Port_ST[Dev_N].Dset_UL = PinMask_N;
	return;
}
/*----------------------------------------------------------------------------*/
/* Function    : GIO_vPortClear                                */
/** 
    \brief      clears the Output port. 
  
    \brief      Description:\n
				This function sets the Data output register
				Effected Reg: GIODCLR

    \param      PinMask_N
    			Uint32Type
    			Pin to be written to    
    \param      Dev_N
    			Uint8Type
				port to be written
				
    \return     void
*/
/*----------------------------------------------------------------------------*/
void GIO_vPortClear(Uint32Type PinMask_N, Uint8Type Dev_N)
{
	pstGio->Port_ST[Dev_N].Dclr_UL = PinMask_N;
	return;
}
/*----------------------------------------------------------------------------*/
/* Function    : GIO_vIntEnable                                */
/** 
    \brief     	GIOFLG register must be cleared before enabling the interrupt.\n
			   	For multiple interrupts occur simulatneously, GIO must prioritize\n 
				the interrupts through the GIOPRY.\n
				When the Interrupt is disabled, the pin acts like I/O pin. 
  
    \brief      Description:\n
				Detail: Effected Register: GIOENA\n
				Two bits must be set within the CIM (central\n
				interrupt manager) in the interrupt mask register\n
				(REQMASK). The REQMASK register must be configured\n
				to enable the appropriate interrupts. Additionally,\n
				the CPU must be configured to recognize\n
				interrupt requests.\n
				This function provides a SW implemented prioritization scheme.\n
				Low bits have higher priority than high bits if they are of the\n 
				same level.\n
				GIOPOL, Rising = 1 or Falling = 0 edge on the external pins.\n
				Programmers shud make sure that	the level on the pins should\n 
				remain high or low for atlease 1 CLK cycle.				

    \param      IntId_N
    			Uint8Type
    			Which interrupt pin?    
    \param      Edge_B
    			GIOPOLARITY
    			Rising or Falling edge?    
    \param      Priority_B
    			GIOPRIORITY
				Low or High Prio?
				
    \return     void
*/
/*----------------------------------------------------------------------------*/
 void GIO_vIntEnable(Uint8Type IntId_N, GIOPOLARITY Edge_B, GIOPRIORITY Priority_B)
{
    pstGio->Pol_UB  = !Edge_B     ? pstGio->Pol_UB & ~IntId_N 
                                   : pstGio->Pol_UB | IntId_N;
    pstGio->Pry_UB  = !Priority_B ? pstGio->Pry_UB & ~IntId_N 
                                   : pstGio->Pry_UB | IntId_N;
    pstGio->Ena_UB |= IntId_N;
    return;
}
/*----------------------------------------------------------------------------*/
 /* Function    : GIO_vIntDisable                                             */
 /** 
     \brief     GIOFLG register must be cleared before enabling the interrupt.\n
 			   	For multiple interrupts occur simulatneously, GIO must prioritize\n 
 				the interrupts through the GIOPRY.\n
 				When the Interrupt is disabled, the pin acts like I/O pin. 
   
     \brief     Description:\n
 				Detail: Effected Register: GIOENA\n

     \param     IntId_N
     			Uint8Type
     			Which interrupt pin?    
 			
     \return    void
*/
/*----------------------------------------------------------------------------*/
void GIO_vIntDisable(Uint8Type  IntId_N)
{
    pstGio->Ena_UB &= ~IntId_N;
    return;
}
/*----------------------------------------------------------------------------*/
 /* Function    : GIO_IntGlobalInit_V		                                  */
 /** 
     \brief     Initialise the GIO interrupt registers and callback routine.
   
     \brief     Description:\n
 				Detail: Effected Register: GIOENA\n

     \param     pstCnt
     			GIO_INTCNF_PST
     			Which interrupt pin?    
     \param     ReqHigh_PV
     			void
     			pointer to user supplied high priority interrupt callback routine    
     \param     ReqLow_PV
     			void
     			pointer to user supplied low priority interrupt callback routine
     			     			 				
     \return    void
*/
/*----------------------------------------------------------------------------*/
void GIO_IntGlobalInit_V(const GIO_INTCNF_PST pstCnt, void (*ReqHigh_PV)(Uint8Type), void (*ReqLow_PV)(Uint8Type))
{
    CallBackHigh_PV = ReqHigh_PV;  // call back high pv is a function pointer
    CallBackLow_PV  = ReqLow_PV;

    pstGio->Pry_UB = pstCnt->Priority_UB;
    pstGio->Pol_UB = pstCnt->Polarity_UB;
    pstGio->Ena_UB = pstCnt->Enable_UB;
    return;
}
/* End Of File GIO_PL_C1.c */


