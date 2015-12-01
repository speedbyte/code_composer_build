/* STARTSINGLE_OF_MULTIPLE */
/*****************************************************************************
| Project Name: CAN Driver
|    File Name: can_inc.h
|
|  Description: definition of can driver include files
|               Please adapt such that the compiler finds the following files:
|                            CAN_CFG.H
|                            CAN_DEF.H
|-----------------------------------------------------------------------------
|               C O P Y R I G H T
|-----------------------------------------------------------------------------
| Copyright (c) 2000-2003 by Vector Informatik GmbH.      All rights reserved.
|
|                    Alle Rechte an der Software verbleiben bei der
|                    Vector Informatik GmbH.
|                    Vector Informatik GmbH räumt dem Lizenznehmer das unwider-
|                    rufliche, geographisch und zeitlich nicht beschränkte,
|                    jedoch nicht ausschließliche Nutzungsrecht innerhalb des
|                    Lizenznehmers ein. Die Weitergabe des Nutzungsrechts
|                    durch den Lizenznehmers ist auf dessen Zulieferer
|                    beschränkt. Die Zulieferer sind zu verpflichten, die
|                    Software nur im Rahmen von Projekten für den Lizenznehmer
|                    zu verwenden; weitere Rechte der Zulieferer sind
|                    auszuschließen.
|-----------------------------------------------------------------------------
|               A U T H O R   I D E N T I T Y
|-----------------------------------------------------------------------------
| Initials     Name                      Company
| --------     ---------------------     ------------------------------------
| Ht           Heike Honert              Vector Informatik GmbH
|-----------------------------------------------------------------------------
|               R E V I S I O N   H I S T O R Y
|-----------------------------------------------------------------------------
| Date         Ver  Author  Description
| ---------    ---  ------  --------------------------------------------------
| 08-Mai-01    1.0   Ht     creation
| 2003-01-28   1.1   Ht     protection from multiple include of this file
|****************************************************************************/
#ifndef _CAN_INC_H
#define _CAN_INC_H
#include "can_cfg.h"               /* dependend configuration of the driver.*/
                                   /* configuration file.                   */

#include "can_def.h"               /* CAN driver header                     */

#include "nm_cfg.h"                /* configuration file of network management */

#if defined ( C_ENABLE_OSEK_OS )
 #include "osek.h"                  /*include of OSEK-Header */
// #error "insert the include for OSEK OS and delete this line"
#endif



/* Include Register header of used controller derivative */
#endif /* _CAN_INC_H */
/* STOPSINGLE_OF_MULTIPLE */
/************   Organi, Version 3.7.0 Vector-Informatik GmbH  ************/
