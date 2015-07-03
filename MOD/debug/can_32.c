/******   STARTSINGLE_OF_MULTIPLE_COMMENT    ******/


 /*****************************************************************************
| Project Name: C A N - D R I V E R
|    File Name: CAN_32.C
|
|
|  Description: Implementation of a routine that has to run in 32 bit
|               Target system: Texas Instruments TMS470
|                              Texas Instruments OMAP   
|               Compiler:      Texas Instruments
|
|-----------------------------------------------------------------------------
|               C O P Y R I G H T
|-----------------------------------------------------------------------------
| Copyright (c) 1999 by Vector Informatik GmbH.           All rights reserved.
|
| Alle Rechte an der Software verbleiben bei der Vector Informatik GmbH.
| Vector Informatik GmbH räumt dem Lizenznehmer das unwiderrufliche, 
| geographisch und zeitlich nicht beschränkte, jedoch nicht ausschließliche 
| Nutzungsrecht innerhalb des Lizenznehmers ein. Die Weitergabe des Nutzungs-
| rechts durch den Lizenznehmers ist auf dessen Zulieferer beschränkt. Die 
| Zulieferer sind zu verpflichten, die Software nur im Rahmen von Projekten 
| für den Lizenznehmer zu verwenden. Weitere Rechte der Zulieferer sind
| auszuschließen.
|-----------------------------------------------------------------------------
|               A U T H O R   I D E N T I T Y
|-----------------------------------------------------------------------------
| Initials     Name                       Company
| --------     ---------------------      ------------------------------------
| Fr           Günther Förstner           Vector Informatik GmbH
| Ml           Patrick Markl              Vector Informatik GmbH
|-----------------------------------------------------------------------------
|               R E V I S I O N   H I S T O R Y
|-----------------------------------------------------------------------------
| Date         Version   Author  Description
| ----------   --------  ------  ----------------------------------------------
| 2000-01-27   01.00.00    Fr     - initial
| 2000-03-21   01.01.00    Fr     - Bugfix: Push/pop added around inline assembler
| 2000-11-13   01.02.00    Fr     - Bugfix: Switch to privileged mode if access to 
|                                   INT / FIQ-flags is needed
| 2000-12-20   01.03.00    Fr     - No interrupt handling any more in this file
| 2001-02-14   01.04.00    Fr     - CPSR directly read into R0, no static var any more
| 2001-07-27   01.05.00    Fr     - Just comment added
| 2002-10-09   01.06.00    Ml     - ESCAN00003885: ARM toolchain is supported
| 2003-12-04   01.07.00    Ml     - ESCAN00007131: added support for OMAP
|****************************************************************************/
#include "can_inc.h"


#if defined(C_COMP_TI_TMS470) 
/************************************************************************
 Name         : CanGetCpsr
 Called by    : can_drv.c
 Preconditions: -
 Parameter    : -
 Return value : int: value of CPSR register
 Description  : This function returns the value of the CPSR register. The value
                is needed to restore properly the interrupts.

  IMPORTANT: The value of CPSR is copied into the return register by the
             assembler line. Since the formal return is missing the compiler
             will report a warning!
             This warning is not critical and can be ignored!
*************************************************************************/
int CanGetCpsr(void) 
{
  asm(" mrs r0,cpsr"); 
}
#endif

/******   STOPSINGLE_OF_MULTIPLE_COMMENT    ******/
/************   Organi, Version 3.7.0 Vector-Informatik GmbH  ************/
