/*  *****   STARTSINGLE_OF_MULTIPLE_COMMENT    *****  */



/******************************************************************************
| Project Name: O S E K   N e t w o r k   M a n a g e m e n t
|    File Name: N_VNM.C
|
| Description: Implementation of Vector OSEK Network Management
|
|------------------------------------------------------------------------------
|               C O P Y R I G H T
|------------------------------------------------------------------------------
| Copyright (c) 1999-2007 by Vector Informatik GmbH.      All rights reserved.
|
|                    Alle Rechte an der Software verbleiben bei der
|                    Vector Informatik GmbH.
|                    Vector Informatik GmbH raeumt dem Lizenznehmer das unwider-
|                    rufliche, geographisch und zeitlich nicht beschraenkte,
|                    jedoch nicht ausschliessliche Nutzungsrecht innerhalb des
|                    Lizenznehmers ein. Die Weitergabe des Nutzungsrechts
|                    durch den Lizenznehmers ist auf dessen Zulieferer
|                    beschraenkt. Die Zulieferer sind zu verpflichten, die
|                    Software nur im Rahmen von Projekten fuer den Lizenznehmer
|                    zu verwenden; weitere Rechte der Zulieferer sind
|                    auszuschliessen.
|------------------------------------------------------------------------------
|              A U T H O R   I D E N T I T Y
|------------------------------------------------------------------------------
| Initials     Name                      Company
| --------     ---------------------     --------------------------------------
| Bs           Thomas Balster            Vector Informatik GmbH
| Ba           Bernd Baudermann          Vector Informatik GmbH
| Bd           Steffen Braendle          Vector Informatik GmbH
| Be           Christian Baeuerle        Vector Informatik GmbH
| dH           Gunnar de Haan            Vector Informatik GmbH
| Et           Thomas Ebert              Vector Informatik GmbH 
| Fr           Guenther Foerstner        Vector Informatik GmbH
| Fz           Ralf Fritz                Vector Informatik GmbH
| Gu           Alexander Gutlederer      Vector Informatik GmbH 
| Gz           Oliver Garnatz            Vector Informatik GmbH
| HH           Hartmut Hoerner           Vector Informatik GmbH
| Ht           Heike Honert              Vector Informatik GmbH
| Js           Bernd Jesse               Vector Informatik GmbH
| Mas          Markus Schwarz            Vector Informatik GmbH
| Ml           Patrick Markl             Vector Informatik GmbH
| Mr           Rainer Mueller            Vector Informatik GmbH
| Pl           Georg Pfluegel            Vector Informatik GmbH
| Rn           Jochen Rein               Vector Informatik GmbH
| Sa           Mishel Shishmanyan        Vector Informatik GmbH
| Si           Bernd Sigle               Vector Informatik GmbH
| Th           Jochen Theuer             Vector Informatik GmbH
|------------------------------------------------------------------------------
|               R E V I S I O N   H I S T O R Y
|------------------------------------------------------------------------------
| Date        Version  Author  Description
| ----------  -------  ------  -------------------------------------------------
| 1999-01-25  0.01.00  dH      Creation
| 1999-01-26  0.02.00  dH      first tests with C167 and VECTOR_NM succesfull
| 1999-01-27  0.03.00  dH      changes according to code review with Ba
| 1999-01-28  0.04.00  dH      optimizations
| 1999-02-01  0.05.00  HH      unchanged
| 1999-02-02  0.06.00  dH      added _0 (first step to multichannel version)
| 1999-02-03  0.08.00  dH      multichannel NM
| 1999-04-27  1.00.00  Fr      Mod. for new definition of multichanneling
| 1999-06-18  3.00.00  dH      some bugfixing within processor defines
|                              deTAB
|                      dH      bugfixing: moved some code from single to multiple
|                              -> removed all C_CLIENT_XXX
|                              C_COMP_TASKING_C16x -> C_COMP_TASKING_C16X
|                      dH      merged several branches into single source
|                              corrected some errors in the NM algorithm
|                      Ba      first official version
| 1999-07-02  3.01.00  dH      changed NmPrecopy: no statement before variable declarations
|                              added channel at CanEcuNumber&=0x1f within NmPrecopy
|                              added nmSendmess to multican define section
|                      Ba      ApplCanAddCanInterruptDisable/Restore channel dependent
|                      Si      changes in prototype of ApplWriteWakeUpInfo
|                      Ba      CANNWM channel 0 also added for nmRecvMess macro
| 1999-12-06  3.10.00  HH      adapted to automatic gentool
| 1999-12-07  3.11.00  HH      enable selective disabling of channels by means of
|                              NM_TYPE_NONE
| 2000-02-28  3.12.00  Be      Compatibility for SINGLE_/MULTIPLE_RECEIVE_BUFFER
|                      dH      adaptation to new GenTool CANgen
|                              renamed callback functions to ApplNm...
|                              renamed timing parameters to NM_COUNT...
|                              no more #ifndef
|                              added C_COMP_HEW_SH7055
|                              added some casts to avoid compiler warnings
|                              changed C_ENABLE_MULTICAN and CANNWM to NM_CAN_CHANNEL
|                              changed NM_NumberOfConfigNodes to NM_NUMBER_OF_CONFIG_NODES
|                              changed NM_NumberOfConfigBytes to NM_NUMBER_OF_CONFIG_BYTES
| 2000-02-15  3.15.00  Mr      changes according to DC-ONM ver. 3.14d
|                      Mr      StateNetSleepInd, PreNetSleepInd (DC-ONM) implemented
|                      Mr      Userdata-Protocol (UDP for DC-ONM) implemented
| 2000-04-10  3.16.00  Fz      adaptation to new GenTool CANgen
|                              nmEcuNr->kNmEcuNr, nmTxObj->kNmTxObj, nmCanPara->kNmCanPara
|                      Rn      conditional namechanges of includefiles
| 2000-04-12           Fz      added C_COMP_NEC_V85X
| 2000-04-26           Bd      bugfix for NM_BROADCAST_ACCESS_USED with C_MULTIPLE_RECEIVE_BUFFER
| 2000-05-30  3.17.00  Gz      adaptation CanEcuNumber -> canEcuNumber
|                              changed switch for FJVGC 
| 2000-06-02  3.18.00  Et      inserted dummy assignment to avoid compiler
|                              warnings for all C_COMP_...
|                              access to SleepInd and SleepAck both by DB and by VAG
| 2000-08-17  3.19.00  Rn      delete C_SPECIAL_HC08AGATE 
|                      Gu      new CUSTOMER_SWITCH
|                      Rn      support C_MULTIPLE_RECEIVE_CHANNEL 
|                      Gu      added C_COMP_IAR_78K0 and C_COMP_HEW_H8S
|                      Gu      divided ApplCanErrorPin() into ApplCanErrorPinReceive()/Send()
|                      Js      C_COMP_MITSUBISHI_M32R added
| 2000-09-01  3.20.00  Js, Ba  Correction: ResetConfig now also works with 128 nodes
|                              bus diagnostic added
| 2000-09-04           Fz      changed NmMessType for NEC CA850 compiler  
| 2000-09-04           Js      bus diagnostics added and ringdata mechanism changed
| 2000-09-08           Gu      moved initialisation of SendMsgUser1 and SendMsgUser2
|                              added C_COMP_IAR_12 in CopyErrorBitToMess
|                              condensed union NmMessType for C_COMP_NEC_V85X
|                              added plausibility check of NM configuration
| 2000-09-20  3.21.00  Js      Ring Address (nmSendMess.c[0]) and
|                              Control Byte (nmSendMess.c[1]) initialized with 0
| 2000-09-25           Th      added support for GNU ST9
| 2000-09-27           Rn      ApplCanAddCanInterruptDisable without Paramater with index-Can-driver
| 2000-10-09  3.22.00  Gu      Updated versions of supported compiler
| 2000-10-19           Fr      added C_COMP_ANSI_TMS_470
| 2000-11-13           Bd      added support for DIABDATA MCORE
| 2000-11-15           Gu      added variable kNmBugfixVersion Bugfix support
|                              bugfix for direct repetition in NmCanError
| 2000-11-20  3.23.00  Gu      no special treatment for C5x5C in CanError needed anymore
| 2000-11-21           Th      added support  for HIWARE ST7
| 2000-11-21           Gz      C_COMP_MITSUBISHI_NC30 -> C_COMP_MITSUBISHI_M16C 
|                              added NM_DUMMY_STATEMENT to avoid compiler warnings
| 2000-12-18  3.24.00  Gu      C_COMP_KEIL_C16x -> C_COMP_KEIL_C16X
| 2000-01-08           Gu      changed instruction in state NM_ACTION_START to avoid compiler warnings 
|                              NM_DBAG_... -> NM_TYPE_DBAG
|                              added condition for old DC_ONM define to ensure compilation of header
| 2001-02-14           Rn      added channel 4, correction in channel 1,2,3
|                      Gu      changed instruction in function NmTask to avoid compiler warnings 
| 2001-02-22  3.25.00  Rn      bugfix: CanReset... for indexed-Can-driver with paramter 
|                              NM_CAN_CHANNEL (ESCAN00000463)
| 2001-02-23  3.26.00  Bs      Greenhils MULTI2000 for OKI ARM7TM added / C_COMP_GHS_ARM7TM
| 2001-03-01           Gu      Deleted MEMORY_ROM assigment for multiple ECU declaration
| 2001-03-19  3.27.00  Gu      no special treatment for C5x5C in NmTask needed anymore
|                              changed dummy statement for M16C in NmConfirmation
|                              added plausibility check for include structure
|                              deleted old DC define
|                              moved compatibility mode of nmEcuNr->kNmEcuNr, nmTxObj->kNmTxObj, 
|                              nmCanPara->kNmCanPara into NM_CFG.H, Source also changed
| 2001-04-06           Gu      added new Customer switch
|                              added extended NM Callbacks
| 2001-04-06           Sa      added Dummy Statement to avoid compiler warnings for NEC 78K0 
|                              and nested comment fix.
| 2001-04-09           Pl      added support for ARM7TDMI
|                      Gu      changed compatibility mode for older versions of Gen-Tool
| 2001-04-19  3.28.00  Gu      added Dummy Statement to avoid compiler warnings
|                              for KEIL C16X Compiler
| 2001-04-26  3.29.00  Gu      XPAGE setting for C_COMP_KEIL_C5x5 removed due to CAN LI1.2 support
|                      Bs      ESCAN00000677: support of C_COMP_OKI_CC665S added in source file
| 2001-06-08  3.30.00  Ba, Js  ESCAN00000792: Implementation for indexed usage of the NM
|                              ESCAN00000560: wrong param for callback for indexed
|                                             CAN-Driver fixed
| 2001-07-16  3.31.00  Ht      ESCAN00000867: support for C_SINGLE_RECEIVE_CHANNEL added
|                      Gu      ESCAN00000835: Changed pChipDataPtr to CanChipDataPtr which
|                                             was changed in reference implementation 1.1
|                                             of the CAN Driver
|                              ESCAN00000875: Wrong assigment of #define TargetErrorConfig changed
|                              ESCAN00000876: changed instruction in function ReadBroadcastData
|                                             to avoid compiler warnings
|                              ESCAN00000927: refer next line
|                              ESCAN00000930: changed instruction in function ResetConfig 
|                                             and ResetErrorConfig to avoid compiler warnings
|                              ESCAN00000928: code for time-measurement added
|                              ESCAN00000931: Missing define for PAG added when using 128 nodes
|                              ESCAN00000570: Vector NM now also works with 128 nodes
|                              ESCAN00001039: tempCanRxActualDLC dependens not only
|                                             of NM_BROADCAST_ACCESS_USED
|                              ESCAN00000980: Adaptions for Ford
| 2001-07-27  3.32.00  Gu      ESCAN00001055: DLC-check in tempCanRxActualDLC optimized
|                      Rn      ESCAN00001055: tempCanRxActualDLC deleted
|                      Ml      ESCAN00001069: added cast to avoid HC12 compiler errors
|                                             when using -pck option
|                      Rn      ESCAN00001068: fixed compiler error in case of indexed
|                                             CAN-Driver and DC Userdata
|                      Gu      ESCAN00001072: Added some casts to avoid warning when
|                                             using strict typechecking
| 2001-08-03  3.33.00  Gu                     no changes
|                      Js      ESCAN00001229: C_COMP_TASKING_MC376 added
| 2001-08-23  3.34.00  Gu      ESCAN00001284: Bugfix in case of using the Ford-NM
| 2001-08-30  3.35.00  Ba      ESCAN00001358: Support of Hiware compiler with Motorola HC08
| 2001-09-03  3.36.00  Gu      ESCAN00001375: Special inquiry in case
|                                             NM_NUMBER_OF_CONFIG_NODES is not used
| 2001-09-12  3.37.00  Gu      ESCAN00001446: Incomplete statement in case of using
|                                             STM ST7 and indexed NM corrected
|                      Gu      ESCAN00001463: Support for extended Userdata Access
| 2001-09-21  3.38.00  Gu      ESCAN00001480: added new and deleted some old compiler switches
|                      Gu, Js  ESCAN00001468: temporary buffer for userdata access
|                                             used for BMW
|                      Gu      ESCAN00001516: udinit and udtask changed to UdInit and UdTask
| 2001-10-25  3.39.00  Gu      ESCAN00001688: Change in bus-off handling for Ford NOS
| 2001-11-01  3.40.00  Rn      ESCAN00001731: added missing brackets for userdata DC
| 2001-11-05           Gu      ESCAN00001733: Fixed another incomplete code line
|                                             using STM ST7
|                              ESCAN00001712: changed switch from C_COMP_<xxx>_SH7055
|                                             to  C_COMP_<xxx>_SH705X
|                              ESCAN00001736: Additional Micro Support for NM C_COMP_COSMIC_ST7_BECAN
|                              ESCAN00001737: Corrected some comments
|                              ESCAN00001738: no changes
| 2001-11-07           Rn      ESCAN00001748: DC-Userdata: clear nmpdu in case of limphome
|                              ESCAN00001749: GW-Support: code before definition
| 2001-11-29           Gu      ESCAN00001842: DC-Userdata: notification of userdata
|                                             service in case ECU is not addressed
| 2001-12-13           Gu      ESCAN00001901: Remove compiler specific definition
|                                             of bit fields
|                              ESCAN00001917: Adapted call of function ApplWriteWakeUpInfo
|                                             in C-File according to Header-File
| 2002-01-03  3.41.00  Ml      ESCAN00001855: Added keywords for paging support
| 2002-01-15           Gu      ESCAN00002062: ApplWriteWakeUpInfo does not support
|                                             MEMORY_NEAR for Hiware Compiler
| 2002-01-22  3.42.00  Gu      ESCAN00002111: New features according to the new
|                                             specification of a car manufacturer
|                      Js      ESCAN00002210: no changes
| 2002-03-04  3.43.00  Js      ESCAN00002413: no changes
|                              ESCAN00002757: no changes
|                      Gu      ESCAN00001900: Added typecast's for NM send buffer to
|                                             avoid linker warnings
|                              ESCAN00004445: During execution of GotoMode(): Possible loss
|                                             of sleep / wakeup information
|                              ESCAN00002759: Corrected bus-off handling for Ford NOS
|                              ESCAN00002760: Type nmAction has been changed from
|                                             enum to vuint8 to avoid compiler warnings
|                              ESCAN00002807: Insert interrupt protection in API
|                                             functions to ensure data consistency
| 2002-05-24  3.44.00  Fz      ESCAN00002926: nmSendMess array aligned to two bytes
|                                             for V850 with NEC compiler
| 2002-06-18           Et      ESCAN00003089: support of physical multiple ECUs
| 2002-07-17                   ESCAN00002926: nmSendMess array aligned to two bytes
|                                             for 16-/32-bit MCUs
| 2002-07-25           Js      ESCAN00002906: UdTask() and UdInit() renamed to
|                                             ApplNmUserDataTask() and ApplUserDataInit()
| 2002-11-11  3.45.00  Js      ESCAN00004146: C-Library functions not longer used
|                              ESCAN00004247: Rx/Tx Inversion resolution
|                              ESCAN00004259: Delayed state transition or during execution of
|                                             GotoMode(): Possible loss of sleep information
|                              ESCAN00004328: CanOnline() added to NM_ACTION_START in NmTask()
|                              ESCAN00004376: Formal changes
|                              ESCAN00004286: kNmEcuNr handling for indexed NM
|                                             according to implementation of
|                                             ESCAN00003089 corrected
|                              ESCAN00003323: Lint warnings removed
|                              ESCAN00004441: Wrong access to UD receive message command
|                              ESCAN00002357  AND
|                              ESCAN00004163: Some compiler produces warning on dummy statements
|                                             V_ENABLE_USE_DUMMY_STATEMENT inserted
|                              ESCAN00004500: Further status functions according to ESCAN00004259
|                              ESCAN00004601: To less copy and initialization of User Data for
|                                             NM_TYPE_VECTOR_OSEK
|                              ESCAN00004607: No Skipped Alive for a special Ring Message
| 2003-02-22  3.46.00  Js      ESCAN00004844: DC Userdata support will not compile in a code
|                                             doubled system
| 2003-03-17  3.47.00  Js      ESCAN00005108: Wrong definition of SendMessFlag and NmStateTypeDcUd
|                                             for C5X5
| 2003-03-28  3.48.00  Js      ESCAN00005245: No ApplCanAddCanInterruptRestore(0) in ReadRingData()
|                                             for special OEM
|                              ESCAN00005246: MEMORY_CAN inserted
|                                             SINGLE_RECEIVE_BUFFER and MULTIPLE_RECEIVE_BUFFER
|                                             deleted
|                              ESCAN00003524: Memory qualifier for zero page mapping
|                                             for C_COMP_HIWARE_08
| 2003-05-06  3.49.00  Js      ESCAN00005573: Warning removed
| 2003-06-17  3.50.00  Js      ESCAN00004601: To less copy and initialization of User Data for
|                                             NM_TYPE_VECTOR_OSEK
| 2003-09-03  3.51.00  Js      ESCAN00006420: OSEK NM send message has incorrect data
|                                             due to alignment problem
| 2003-10-06  3.52.00  Js      ESCAN00006646: New features (2nd step) according to the new
|                                             specification of a car manufacturer
|                              ESCAN00006727: Error Pin detection does not work on
|                                             indexed multi channel NM for special OEM
|                              ESCAN00006769: No WakeUp after BusSleep for Ring Message
|                                             with set Sleep Indication
|                              ESCAN00005970: canChannel for Multiple ECUs with
|                                             indirection not defined
|                              ESCAN00006771: Amount of User Data changed for
|                                             special OEM
|                              ESCAN00006891: nmSendMess now fix on 8 byte
|                              ESCAN00006916: canEcuNumber not longer used
| 2003-11-27  3.53.00  Js      ESCAN00007008: WakeUp Type Indication for a car manufacturer added
|                              ESCAN00007091: Changes according GENy introduction
|                              ESCAN00007216: User Data extracted for special car manufacturer
|                              ESCAN00007217: Generic NM adaptation API introduced
|                              ESCAN00006970: Warning is generated "canChannel not used"
|                              ESCAN00006361: Prepare Sleep Counter for Indexed Systems
|                              ESCAN00007275: Possible compiler report for truncating assignment
|                              ESCAN00007287: Error Configuration mismatch for special
|                                             car manufacturer
| 2004-01-28  3.54.00  Js      ESCAN00007369: ONMX send data access changed
|                              ESCAN00007382: Cosmic HC(S)12 issue fix
| 2004-02-13  3.55.00  Rn      ESCAN00007561: Include v_inc.h instead of can_inc.h in case of GENy
| 2004-02-17  3.56.00  Js      ESCAN00007430: Remote Sleep Indication adapted
|                              ESCAN00007436: ESCAN00006769 moved to NmPrecopy
|                              ESCAN00007437: ESCAN00007008 reworked for wait bus sleep
| 2004-03-11  3.57.00  Js      ESCAN00007856: Faster solution for ESCAN00004247
|                              ESCAN00007535: Multiple calls of ApplNmWaitBusSleep when
|                                             multiply receiving RING messages with set SleepAck
|                              ESCAN00007530: Compiler Warning for GetConfig() and CmpConfig()
|                              ESCAN00007822: Warning about unreferenced variable 'channel'
|                              ESCAN00007866: Usage of Multiple ECU for special configuration
| 2004-04-05  3.58.00  Js      ESCAN00007954: no changes
|                              ESCAN00008104: Unexpected copy of User Data
|                              ESCAN00008229: Optimization of UD handling for special
|                                             car manufacturer
|                              ESCAN00008463: Tx path activation to late after cancel
|                                             of TwaitBusSleep
|                              ESCAN00008466: Feature review and rework
|                              ESCAN00008489: Formal adaptations for specific car manufacturer
| 2004-06-29  3.59.00  Js      ESCAN00008585: no changes
|                              ESCAN00008665: Usage of temp. buffer for indexed C16x driver
| 2004-10-01  3.60.00  Js      ESCAN00009420: The ECU does not transmit any message
|                                             except NM messages
|                              ESCAN00009728: New Version Defines introduced
|                              ESCAN00009711: BusOff Flag remains active
| 2004-10-14  3.61.00  Js      ESCAN00009728: New Version Defines introduced
| 2004-10-29  3.62.00  Js      ESCAN00010055: Wrong context of CanOffline
| 2004-11-29  3.63.00  Js      ESCAN00010415: Support of 256 nodes
|                              ESCAN00010454: Config algorithm equal for all OEMs
|                              ESCAN00010455: no changes
|                              ESCAN00010456: C_COMP_HITECH_05 removed
|                              ESCAN00010463: Fast BusOff recovery introduced
|                              ESCAN00010468: Extensions for special OEM removed
|                              ESCAN00010471: no changes
|                              ESCAN00010498: Wrong Fast BusOff recovery time
| 2005-01-18  3.64.00  Js      ESCAN00010855: CarWakeUp for special OEM introduced
| 2005-03-29  3.65.00  Js      ESCAN00011622: Extensions for special OEM re-integrated
|                              ESCAN00011925: Compile issue in special configuration
|                                             for indexed NM
|                              ESCAN00011970: Standard vector type qualifier implemented
|                              ESCAN00011997: ECU's NM does not participate on a
|                                             bus in a valid way
| 2005-07-11  3.66.00  Js      ESCAN00012831: Extensions for special OEM re-integrated
|                              ESCAN00013626: Fixed by ESCAN00012831
|                              ESCAN00012842: Include structure adapted
|                              ESCAN00012901: Unexpected user data for special OEM
|                              ESCAN00012928: kNmBugfixVersion renamed to kNmReleaseVersion
| 2006-01-23  3.67.00  Js      ESCAN00014119: changed implementation of SkippedAlive
|                      Mas     ESCAN00014635: Re-integrated ErrorPin in NmPrecopy
|                      Mas     ESCAN00014458: Fixed access on TX object for multiple ECUs
|                      Mas     ESCAN00015016: added ORGANI filter checks
| 2006-03-07  3.68.00  Mas     ESCAN00015469: corrected layout of bus diagnostic for some OEMs
| 2006-03-16  3.69.00  Mas     ESCAN00015700: changed byte order within NmGetCanErrorConfig()
| 2006-09-04  3.70.00  Mas     ESCAN00017526: - changed variable assignment in NmPreTransmit()
|                              ESCAN00017787: - extended feature CarWakeUp for certain OEM 
|                              <internal>     - moved local functions to the end of the file and 
|                                               added prototypes
|                              <internal>     - adapted comments and applied changes for MISRA
| 2007-01-16  3.70.01  Mas     ESCAN00017962: added reset of limphome status
| 2007-07-02  3.70.02  Mas     ESCAN00021051: added BusOff end handling for RX of SleepAck in LimpHome
| 2007-07-23  3.70.03  Mas     ESCAN00020303: no changes in this file
|                              ESCAN00020580: removed compiler warnings for kNmEcuNr == 0x00
|                              ESCAN00020990: changed handling of solution for ESCAN00020580
|*****************************************************************************/

/******************************************************************************
 * Include files
 ******************************************************************************/
/* used for nm internal */
#define OSEK_NM_MODULE

#include "n_vnmdef.h"
#include "GIO.h"
#include "PORT.h"
/*lint -e539*/ /* Did not expect positive indentation from line ... */
/*lint -e655*/ /* Warning 655: bit-wise operation uses (compatible) enum's */
/*lint -e750*/ /* local macro ... not referenced */

/*****************************************************************************
 * Version check
 *****************************************************************************/
/* ##V_CFG_MANAGEMENT ##CQProject : Nm_DirOsek CQComponent : Implementation */
#if ( NM_DIROSEK_VERSION != 0x0370 )
  #error "Configuration error in NM_DIROSEK: Header and Source file are inconsistent!"
#endif
#if ( NM_DIROSEK_RELEASE_VERSION != 0x03 )
  #error "Configuration error in NM_DIROSEK: Header and Source file are inconsistent!"
#endif

/*****************************************************************************
 * ORGANI check
 *****************************************************************************/
#if defined ( VGEN_GENY ) && !defined ( VGEN_GENY )
/* __ORGANI_ERROR__ "VGEN_GENY must be filtered in or out!" */
#endif


/*****************************************************************************
 * Timing Measurement Module
 *****************************************************************************/
  #if defined ( StartMeasure )
  #else
    #define StartMeasure(i)
  #endif

  #if defined ( StopMeasure )
  #else
    #define StopMeasure(i)
  #endif

/*****************************************************************************
 * Defines
 *****************************************************************************/
#if defined ( C_CPUTYPE_8BIT )
  #define kNmSendMessSize 8
  #define kNmSendMessType vuint8
#else
  #if defined ( C_CPUTYPE_16BIT )
    #define kNmSendMessSize 4
    #define kNmSendMessType vuint16
  #else
    #if defined ( C_CPUTYPE_32BIT )
      #define kNmSendMessSize 2
      #define kNmSendMessType vuint32
    #else
      #error "Nm_DirOsek: Missing/Unknown setting for C_CPUTYPE_xBIT. Please check v_cfg.h"
    #endif
  #endif
#endif

/* multi channel can driver */
#if defined ( C_SINGLE_RECEIVE_BUFFER ) || defined ( C_MULTIPLE_RECEIVE_BUFFER )
 #if defined ( NM_CAN_CHANNEL )
  #if ( NM_CAN_CHANNEL == 0 )
    #define CanRxActualId          CanRxActualId_0
    #define CanTransmit            CanTransmit_0
    #define CanResetBusOffStart(a) CanResetBusOffStart_0(a)
    #define CanResetBusOffEnd(a)   CanResetBusOffEnd_0(a)
    #define CanResetBusSleep(a)    CanResetBusSleep_0(a)
    #define CanOffline             CanOffline_0
    #define CanOnline              CanOnline_0
    #define CanInterruptDisable    CanInterruptDisable_0
    #define CanInterruptRestore    CanInterruptRestore_0
    #define CanRxActualDLC         CanRxActualDLC_0
    #define NmConfirmation         NmConfirmation_0
    #define NmPrecopy              NmPrecopy_0
    #define NmCanError             NmCanError_0
    #define nmSendMess             nmSendMess_0
  #endif /* #if ( NM_CAN_CHANNEL == 0 ) */
  #if ( NM_CAN_CHANNEL == 1 )
    #define CanRxActualId          CanRxActualId_1
    #define CanTransmit            CanTransmit_1
    #define CanResetBusOffStart(a) CanResetBusOffStart_1(a)
    #define CanResetBusOffEnd(a)   CanResetBusOffEnd_1(a)
    #define CanResetBusSleep(a)    CanResetBusSleep_1(a)
    #define CanOffline             CanOffline_1
    #define CanOnline              CanOnline_1
    #define CanInterruptDisable    CanInterruptDisable_1
    #define CanInterruptRestore    CanInterruptRestore_1
    #define CanRxActualDLC         CanRxActualDLC_1
    #define NmConfirmation         NmConfirmation_1
    #define NmPrecopy              NmPrecopy_1
    #define NmCanError             NmCanError_1
    #define nmSendMess             nmSendMess_1
  #endif /* #if ( NM_CAN_CHANNEL == 1 ) */
  #if ( NM_CAN_CHANNEL == 2 )
    #define CanRxActualId          CanRxActualId_2
    #define CanTransmit            CanTransmit_2
    #define CanResetBusOffStart(a) CanResetBusOffStart_2(a)
    #define CanResetBusOffEnd(a)   CanResetBusOffEnd_2(a)
    #define CanResetBusSleep(a)    CanResetBusSleep_2(a)
    #define CanOffline             CanOffline_2
    #define CanOnline              CanOnline_2
    #define CanInterruptDisable    CanInterruptDisable_2
    #define CanInterruptRestore    CanInterruptRestore_2
    #define CanRxActualDLC         CanRxActualDLC_2
    #define NmConfirmation         NmConfirmation_2
    #define NmPrecopy              NmPrecopy_2
    #define NmCanError             NmCanError_2
    #define nmSendMess             nmSendMess_2
  #endif /* #if ( NM_CAN_CHANNEL == 2 ) */
  #if ( NM_CAN_CHANNEL == 3 )
    #define CanRxActualId          CanRxActualId_3
    #define CanTransmit            CanTransmit_3
    #define CanResetBusOffStart(a) CanResetBusOffStart_3(a)
    #define CanResetBusOffEnd(a)   CanResetBusOffEnd_3(a)
    #define CanResetBusSleep(a)    CanResetBusSleep_3(a)
    #define CanOffline             CanOffline_3
    #define CanOnline              CanOnline_3
    #define CanInterruptDisable    CanInterruptDisable_3
    #define CanInterruptRestore    CanInterruptRestore_3
    #define CanRxActualDLC         CanRxActualDLC_3
    #define NmConfirmation         NmConfirmation_3
    #define NmPrecopy              NmPrecopy_3
    #define NmCanError             NmCanError_3
    #define nmSendMess             nmSendMess_3
  #endif /* #if ( NM_CAN_CHANNEL == 3 ) */
  #if ( NM_CAN_CHANNEL == 4 )
    #define CanRxActualId          CanRxActualId_4
    #define CanTransmit            CanTransmit_4
    #define CanResetBusOffStart(a) CanResetBusOffStart_4(a)
    #define CanResetBusOffEnd(a)   CanResetBusOffEnd_4(a)
    #define CanResetBusSleep(a)    CanResetBusSleep_4(a)
    #define CanOffline             CanOffline_4
    #define CanOnline              CanOnline_4
    #define CanInterruptDisable    CanInterruptDisable_4
    #define CanInterruptRestore    CanInterruptRestore_4
    #define CanRxActualDLC         CanRxActualDLC_4
    #define NmConfirmation         NmConfirmation_4
    #define NmPrecopy              NmPrecopy_4
    #define NmCanError             NmCanError_4
    #define nmSendMess             nmSendMess_4
  #endif /* #if ( NM_CAN_CHANNEL == 4 ) */
 #endif /* defined ( NM_CAN_CHANNEL ) */
#endif /* defined ( C_SINGLE_RECEIVE_BUFFER ) || defined ( C_MULTIPLE_RECEIVE_BUFFER ) */

#if defined ( NM_ENABLE_CONFIG ) || defined ( NM_ENABLE_VAG_LAH_130 )
#define BYTE_OFFSET_MASK              ((vuint8)0x07) /* Used for config */
#endif

/* used for handling rx and tx counter */
#define RX_CNT_MASK                   ((vuint8)0x0F) /* Low-Nibble  of nmRxTxCnt */
#define TX_CNT_MASK                   ((vuint8)0xF0) /* High-Nibble of nmRxTxCnt */
#define TX_CNT_STEP                   ((vuint8)0x10)
#define RX_CNT_STEP                   ((vuint8)0x01)
#define RX_CNT_MAX                    ((vuint8)4)
#define TX_CNT_MAX                    ((vuint8)8)

/*---- Variable for storing the current state for the Network Management state machine ----------------------*/
typedef vuint8 NmActionType;

/*---- Definition of the different states of Network Management state machine  ----------------------*/
#define NO_ACTION                     ((NmActionType)0x00)
#define NM_ACTION_START               ((NmActionType)0x01)
#define NM_ACTION_WAIT_SENDTOKEN      ((NmActionType)0x02)
#define NM_ACTION_WAIT_RINGMSG        ((NmActionType)0x03)
#define NM_ACTION_LH_WAIT_LIMPHOME_TX ((NmActionType)0x04)
#define NM_ACTION_LH_PREPARE_SLEEP    ((NmActionType)0x05)
#define NM_ACTION_GO_BUSSLEEP         ((NmActionType)0x06)
#define NM_ACTION_BUS_SLEEP           ((NmActionType)0x07)
#define NM_ACTION_REPEAT              ((NmActionType)0x80)


#if defined ( C_COMP_IAR_M16C )
  #if defined ( _NEAR_ )
  /* Set const data to const segment, don´t initialize */
  #pragma memory=constseg(CONST_DATA) :far
  #endif
#endif

V_MEMROM0 V_MEMROM1 vuint8 V_MEMROM2 kNmMainVersion    = (vuint8)(NM_DIROSEK_VERSION >> 8);
V_MEMROM0 V_MEMROM1 vuint8 V_MEMROM2 kNmSubVersion     = (vuint8)(NM_DIROSEK_VERSION & 0xFF);
V_MEMROM0 V_MEMROM1 vuint8 V_MEMROM2 kNmReleaseVersion = (vuint8)(NM_DIROSEK_RELEASE_VERSION);

#if defined ( C_COMP_IAR_M16C )
  #if defined ( _NEAR_ )
  /* Normal segment usage */
  #pragma memory=default
  #endif
#endif


#if defined ( NM_ENABLE_INDEXED_NM ) || \
    defined ( NM_ENABLE_MULTIPLE_NODES )
  #define NM_INT_DISABLE_OPTIMIZED_SKIPPED_DETECTION
#else
  #if ( kNmEcuNr == 0x00 )
    #define NM_INT_ENABLE_OPTIMIZED_SKIPPED_DETECTION
  #else
    #define NM_INT_DISABLE_OPTIMIZED_SKIPPED_DETECTION
  #endif
#endif

/*  *****   STOPSINGLE_OF_MULTIPLE_COMMENT    *****  */


/*****************************************************************************
*
*
*      HEADER-Part for NM channel 
*
*
******************************************************************************/



/*---- Defines for NM message bits ------------------------------------------*/
  #define SLEEP_IND                 ((vuint8)0x10)  /* bit 4 byte 1 */
  #define SLEEP_ACK                 ((vuint8)0x20)  /* bit 5 byte 1 */

  #define CTRL_INIT                 ((vuint8)0x00)

    #define DATA_INIT               ((vuint8)0x00)

  #if defined ( NM_TYPE_VAG_OSEK ) 
    #define ALIVE                   ((vuint8)0x02)
    #define RING                    ((vuint8)0x01)
    #define LIMPHOME                ((vuint8)0x04)
    #define CAR_WUP                 ((vuint8)0x40)  /* ESCAN00010855 */
  #endif  /* NM_TYPE_VAG_OSEK || NM_TYPE_PAG2_OSEK */

/***************************************************************************/
/* Defines for Receive Handling                                            */
/***************************************************************************/

/* Masks for Receiver CAN-ID Handling - only valid in NmPrecopy() */
#if defined ( VGEN_GENY )
  #if defined ( NM_ENABLE_INDEXED_NM )
    #if ( kNmNumberOfRanges > 1 )
      #define kNmSourceMask (NmRangeMask_Field[channel])  /* Dynamic node mask */
    #else
      #define kNmSourceMask ((vuint8)NM_RANGE_MASK)   /* Dynamic node mask */
    #endif
  #else
    #define   kNmSourceMask ((vuint8)NM_RANGE_MASK)   /* Dynamic node mask */
  #endif
#else
  #if defined ( NM_TYPE_VAG_OSEK ) 
    #define   kNmSourceMask ((vuint8)0x1F)                /*  32 nodes */
  #endif  /* NM_TYPE_VAG_OSEK || NM_TYPE_PAG2_OSEK */
#endif

/* Receiver CAN-ID Handling - only valid in NmPrecopy() */
#if defined ( C_MULTIPLE_RECEIVE_CHANNEL ) || defined ( C_SINGLE_RECEIVE_CHANNEL )
  #define GetRecvNmSourceAdr ((vuint8)((vuint8)(CanRxActualId(rxStruct)) & kNmSourceMask))
#else
  #define GetRecvNmSourceAdr ((vuint8)((vuint8)(CanRxActualId      ) & kNmSourceMask))
#endif

/* Define Rx buffer access */ 
  #define RecvNmDestAdr   ((vuint8)(nmRecvMess[0]))
  #define RecvNmCmd       ((vuint8)(nmRecvMess[1] &  (vuint8)0x0F)) /* bit 3...0 byte 1 */
  #define RecvNmSleepInd  ((vuint8)(nmRecvMess[1] & SLEEP_IND)) /* bit 4     byte 1 */
  #define RecvNmSleepAck  ((vuint8)(nmRecvMess[1] & SLEEP_ACK)) /* bit 5     byte 1 */

  #define RecvUdStartAdr          (&nmRecvMess[2])                  /* Raw CAN data     */

  #define RecvMsgUser0    ((vuint8)(nmRecvMess[2]))
  #define RecvMsgUser1    ((vuint8)(nmRecvMess[3]))
  #define RecvMsgUser2    ((vuint8)(nmRecvMess[4]))
  #define RecvMsgUser3    ((vuint8)(nmRecvMess[5]))
  #define RecvMsgUser4    ((vuint8)(nmRecvMess[6]))
  #define RecvMsgUser5    ((vuint8)(nmRecvMess[7]))


/*---- Receive buffer for NM messages (on - chip receive buffer ) -----------*/

#if defined ( C_SINGLE_RECEIVE_BUFFER )
  #if defined ( NM_CAN_CHANNEL )
    #if ( NM_CAN_CHANNEL == 0 )
      #define nmRecvMess    ((CanChipDataPtr)(CanRDSPtr_0))
    #endif
    #if ( NM_CAN_CHANNEL == 1 )
      #define nmRecvMess    ((CanChipDataPtr)(CanRDSPtr_1))
    #endif
    #if ( NM_CAN_CHANNEL == 2 )
      #define nmRecvMess    ((CanChipDataPtr)(CanRDSPtr_2))
    #endif
    #if ( NM_CAN_CHANNEL == 3 )
      #define nmRecvMess    ((CanChipDataPtr)(CanRDSPtr_3))
    #endif
  #else /* NM_CAN_CHANNEL */
    #define nmRecvMess  ((CanChipDataPtr)(CanRDSPtr))
  #endif /* NM_CAN_CHANNEL */
#endif /* C_SINGLE_RECEIVE_BUFFER */

#if defined ( C_MULTIPLE_RECEIVE_BUFFER )
  #define nmRecvMess    ((CanChipDataPtr)(rxDataPtr))
#endif /* C_MULTIPLE_RECEIVE_BUFFER */

#if defined ( C_MULTIPLE_RECEIVE_CHANNEL ) || defined ( C_SINGLE_RECEIVE_CHANNEL )
  #define nmRecvMess    ((CanChipDataPtr)(rxStruct->pChipData))
#endif


/*****************************************************************************
 * Data types / enumerations
 *****************************************************************************/

/*---- NM  state  in byte 0; flags in byte 1 --------------------------------*/
typedef union               /* c[0]:  07 06 04 03 02 01 00 :mem    net state */
{                           /* c[1]:  17 16 14 13 12 11 10 :mem+1  flags     */
  vuint8 c[2];

  #if defined ( C_CPUTYPE_BITORDER_MSB2LSB )
  struct
  {
    vbittype    b_BusSleepInd     :1;     /* byte 0 bit 7 */
    #if defined ( NM_TYPE_VAG_OSEK ) 
      #if defined ( NM_ENABLE_VAG_LAH_130 )
    vbittype    b_ErrorBit        :1;     /* byte 0 bit 6 */
      #endif
      #if defined ( NM_ENABLE_VAG_LAH_141 )
    vbittype    b_TxRingDataAllow :1;     /* byte 0 bit 6 */
      #endif
    #endif  /* NM_TYPE_VAG_OSEK || NM_TYPE_PAG2_OSEK */
    vbittype    b_WaitBusSleep    :1;     /* byte 0 bit 5 */
    vbittype    b_BusSleep        :1;     /* byte 0 bit 4 */
    vbittype    b_LimpHome        :1;     /* byte 0 bit 3 */
    vbittype    b_Active          :1;     /* byte 0 bit 2 */
    vbittype    b_BusError        :1;     /* byte 0 bit 1 */
    vbittype    b_RingStable      :1;     /* byte 0 bit 0 */

    vbittype    b_PrepSleep       :1;     /* byte 1 bit 7 */
    vbittype    b_RingPreStable   :1;     /* byte 1 bit 6 */
    vbittype    b_RxTxLockUd      :1;     /* byte 1 bit 5 */  /* ESCAN00008104 */
    vbittype    b_NmMsgRecvRemind :1;     /* byte 1 bit 4 */
    vbittype    b_ImSkippedOver   :1;     /* byte 1 bit 3 */
    vbittype    b_ImAddressed     :1;     /* byte 1 bit 2 */
    vbittype    b_RingMsgRecv     :1;     /* byte 1 bit 1 */
    vbittype    b_NmMsgRecv       :1;     /* byte 1 bit 0 */
  }b;
  #else
    #if defined ( C_CPUTYPE_BITORDER_LSB2MSB )
  struct
  {
    vbittype    b_RingStable      :1;     /* byte 0 bit 0 */
    vbittype    b_BusError        :1;     /* byte 0 bit 1 */
    vbittype    b_Active          :1;     /* byte 0 bit 2 */
    vbittype    b_LimpHome        :1;     /* byte 0 bit 3 */
    vbittype    b_BusSleep        :1;     /* byte 0 bit 4 */
    vbittype    b_WaitBusSleep    :1;     /* byte 0 bit 5 */
    #if defined ( NM_TYPE_VAG_OSEK ) 
      #if defined ( NM_ENABLE_VAG_LAH_130 )
    vbittype    b_ErrorBit        :1;     /* byte 0 bit 6 */
      #endif
      #if defined ( NM_ENABLE_VAG_LAH_141 )
    vbittype    b_TxRingDataAllow :1;     /* byte 0 bit 6 */
      #endif
    #endif  /* NM_TYPE_VAG_OSEK || NM_TYPE_PAG2_OSEK */
    vbittype    b_BusSleepInd     :1;     /* byte 0 bit 7 */

    vbittype    b_NmMsgRecv       :1;     /* byte 1 bit 0 */
    vbittype    b_RingMsgRecv     :1;     /* byte 1 bit 1 */
    vbittype    b_ImAddressed     :1;     /* byte 1 bit 3 */
    vbittype    b_ImSkippedOver   :1;     /* byte 1 bit 4 */
    vbittype    b_NmMsgRecvRemind :1;     /* byte 1 bit 6 */
    vbittype    b_RxTxLockUd      :1;     /* byte 1 bit 5 */  /* ESCAN00008104 */
    vbittype    b_RingPreStable   :1;     /* byte 1 bit 6 */
    vbittype    b_PrepSleep       :1;     /* byte 1 bit 7 */
  }b;
    #else 
      #error "Configuration error in NM_DIROSEK: Missing bitorder for typedef NmStateType!"
    #endif
  #endif
}NmStateType;

#if defined ( NM_ENABLE_SLEEPACK_NOTIFICATION ) || defined ( NM_ENABLE_REMOTESLEEP_INDICATION )
typedef union
{
  vuint8 c[2];

  #if defined ( C_CPUTYPE_BITORDER_MSB2LSB )
  struct
  {
    vbittype    b_Dummy0_7            :1;     /* byte 0 bit 7 */
    vbittype    b_Dummy0_6            :1;     /* byte 0 bit 6 */
    vbittype    b_Dummy0_5            :1;     /* byte 0 bit 5 */
    vbittype    b_Dummy0_4            :1;     /* byte 0 bit 4 */
    vbittype    b_Dummy0_3            :1;     /* byte 0 bit 3 */
    vbittype    b_PrepareSendSleepAck :1;     /* byte 0 bit 2 */
    vbittype    b_RemoteSleepInd      :1;     /* byte 0 bit 1 */
    vbittype    b_RemotePrepareSleep  :1;     /* byte 0 bit 0 */

    vbittype    b_Dummy1_7            :1;     /* byte 1 bit 7 */
    vbittype    b_Dummy1_6            :1;     /* byte 1 bit 6 */
    vbittype    b_Dummy1_5            :1;     /* byte 1 bit 5 */
    vbittype    b_Dummy1_4            :1;     /* byte 1 bit 4 */
    vbittype    b_Dummy1_3            :1;     /* byte 1 bit 3 */
    vbittype    b_Dummy1_2            :1;     /* byte 1 bit 2 */
    vbittype    b_Dummy1_1            :1;     /* byte 1 bit 1 */
    vbittype    b_Dummy1_0            :1;     /* byte 1 bit 0 */
  }b;
  #else
    #if defined ( C_CPUTYPE_BITORDER_LSB2MSB )
  struct
  {
    vbittype    b_RemotePrepareSleep  :1;     /* byte 0 bit 0 */
    vbittype    b_RemoteSleepInd      :1;     /* byte 0 bit 1 */
    vbittype    b_PrepareSendSleepAck :1;     /* byte 0 bit 2 */
    vbittype    b_Dummy0_3            :1;     /* byte 0 bit 3 */
    vbittype    b_Dummy0_4            :1;     /* byte 0 bit 4 */
    vbittype    b_Dummy0_5            :1;     /* byte 0 bit 5 */
    vbittype    b_Dummy0_6            :1;     /* byte 0 bit 6 */
    vbittype    b_Dummy0_7            :1;     /* byte 0 bit 7 */

    vbittype    b_Dummy1_0            :1;     /* byte 1 bit 0 */
    vbittype    b_Dummy1_1            :1;     /* byte 1 bit 1 */
    vbittype    b_Dummy1_2            :1;     /* byte 1 bit 2 */
    vbittype    b_Dummy1_3            :1;     /* byte 1 bit 3 */
    vbittype    b_Dummy1_4            :1;     /* byte 1 bit 4 */
    vbittype    b_Dummy1_5            :1;     /* byte 1 bit 5 */
    vbittype    b_Dummy1_6            :1;     /* byte 1 bit 6 */
    vbittype    b_Dummy1_7            :1;     /* byte 1 bit 7 */
  }b;
      #else 
        #error "Configuration error in NM_DIROSEK: Missing bitorder for typedef NmStateTypeGwMaster!"
      #endif
    #endif
}NmStateTypeGwMaster;
#endif  /* NM_ENABLE_SLEEPACK_NOTIFICATION || NM_ENABLE_REMOTESLEEP_INDICATION */

typedef union
{
  vuint8 c[8];

  #if defined ( C_CPUTYPE_BITORDER_MSB2LSB )
  struct
  {
    vbittype cDest          :8;   /* bit  7...0 ;           byte 0          */ 
    vbittype bReserved0     :1;   /* bit 15     ; bit 7     byte 1          */
    vbittype bConfig        :1;   /* bit 14     ; bit 6                     */
    vbittype bSleepAck      :1;   /* bit 13     ; bit 5                     */
    vbittype bSleepInd      :1;   /* bit 12     ; bit 4                     */
    vbittype cCmd           :4;   /* bit 11...8 ; bit 3...0 byte 1          */
                                    /*
                                                  0x01: ALIVE - VAG: RING
                                                  0x02: RING  - VAG: ALIVE
                                                  0x04: LIMPHOME
                                                                            */
    /* 1 to 6 user bytes follow. Access is done by array element of union.  */
  }b;
  #else
    #if defined ( C_CPUTYPE_BITORDER_LSB2MSB )
  struct
  {
    vbittype cDest          :8;   /* bit  7...0 ;           byte 0          */ 
    vbittype cCmd           :4;   /* bit 11...8 ; bit 3...0 byte 1          */
                                  /*
                                                  0x01: ALIVE - VAG: RING
                                                  0x02: RING  - VAG: ALIVE
                                                  0x04: LIMPHOME
                                                                            */
    vbittype bSleepInd      :1;   /* bit 12     ; bit 4                     */
    vbittype bSleepAck      :1;   /* bit 13     ; bit 5                     */
    vbittype bConfig        :1;   /* bit 14     ; bit 6                     */
    vbittype bReserved0     :1;   /* bit 15     ; bit 7     byte 1          */
    /* 1 to 6 user bytes follow. Access is done by array element of union.  */
  }b;
    #else
      #error "Configuration error in NM_DIROSEK: Missing bitorder for typedef SendMsgTypeUnion!"
    #endif
  #endif
}SendMsgTypeUnion;


/****************************************************************************/
/* Definition of variables depends on number of channels                    */
/****************************************************************************/


#if defined ( NM_ENABLE_INDEXED_NM )

  /****************************************************************************/
  /* Page0 data                                                               */
  /****************************************************************************/
  /*---- transmit buffer nm message ------------------------------------------*/

  #if defined ( C_COMP_KEIL_C5X5C )
static idata      NmStateType nmState[kNmNumberOfChannels];
static idata volatile vuint8 SendMessFlag[kNmNumberOfChannels]; /* ESCAN00004259 */

    #if defined ( NM_ENABLE_MULTIPLE_NODES )
/* new array, because of complex statement */
static idata vuint8 NmEcuNr_FieldMultipleNodes[kNmNumberOfChannels];
static idata vuint8 NmTxObj_FieldMultipleNodes[kNmNumberOfChannels];
    #endif

    #if defined ( C_BUFFER_XDATA )
xdata kNmSendMessType nmSendMess[kNmNumberOfChannels][kNmSendMessSize];
    #endif

    #if defined ( C_BUFFER_IDATA )
idata kNmSendMessType nmSendMess[kNmNumberOfChannels][kNmSendMessSize];
    #endif

    #if defined ( NM_ENABLE_SLEEPACK_NOTIFICATION ) || defined ( NM_ENABLE_REMOTESLEEP_INDICATION )
static idata NmStateTypeGwMaster nmStateGwMaster[kNmNumberOfChannels];
    #endif

  #else /* C_COMP_KEIL_C5X5C */

static      NmStateType MEMORY_NEAR nmState[kNmNumberOfChannels];
static volatile vuint8 MEMORY_NEAR SendMessFlag[kNmNumberOfChannels]; /* ESCAN00004259 */

    #if defined ( NM_ENABLE_MULTIPLE_NODES )
/* new array, because of complex statement */
static vuint8 MEMORY_NEAR NmEcuNr_FieldMultipleNodes[kNmNumberOfChannels];
static vuint8 MEMORY_NEAR NmTxObj_FieldMultipleNodes[kNmNumberOfChannels];
    #endif

    #if defined ( C_COMP_COSMIC_ST7 )
    #else
kNmSendMessType MEMORY_NEAR nmSendMess[kNmNumberOfChannels][kNmSendMessSize];
    #endif

    #if defined ( NM_ENABLE_SLEEPACK_NOTIFICATION ) || defined ( NM_ENABLE_REMOTESLEEP_INDICATION )
static NmStateTypeGwMaster MEMORY_NEAR nmStateGwMaster[kNmNumberOfChannels];
    #endif

  #endif /* C_COMP_KEIL_C5X5C */


/****************************************************************************/
/* Extended page variables                                                  */
/****************************************************************************/
static vuint8          TimeCnt[kNmNumberOfChannels];
static NmActionType   nmAction[kNmNumberOfChannels];
static vuint8  RecvNmSourceAdr[kNmNumberOfChannels];
 /*--- counters to detect failures of rx/tx functionality of the NM */
static vuint8        nmRxTxCnt[kNmNumberOfChannels];
static vuint8 logicalSuccessor[kNmNumberOfChannels];

  #if defined ( NM_ENABLE_CONFIG )
/*---- Net management configuration ----------------------------------------*/
static NmConfigType nmConfig;
  #endif

  #if defined ( NM_ENABLE_COPY_RINGDATA_TO_TEMPBUFFER)
/* Received ringdata are copied to global buffer for application usage */
/* but they are not automatically copied to the send message buffer */
static vuint8 nmRecvCopyMsgUser[kNmNumberOfChannels][6];
  #endif




  #if defined ( NM_TYPE_VAG_OSEK )
    #if defined ( NM_ENABLE_VAG_LAH_141 ) && defined ( NM_ENABLE_CAR_WUP_REQUEST )
static volatile tNmCommStateType nmCarWupState[kNmNumberOfChannels];  /* ESCAN00010855 */
static          vuint8        nmCarWupSendMess[kNmNumberOfChannels];  /* ESCAN00010855 */
    #endif
  #endif  /* NM_TYPE_VAG_OSEK */

  #if defined ( NM_TYPE_VAG_OSEK ) 
    #if defined ( NM_ENABLE_VAG_LAH_130 )
static NmConfigType nmCanErrorConfig;
    #endif
  #endif  /* NM_TYPE_VAG_OSEK || NM_TYPE_PAG2_OSEK */

#else /* NM_ENABLE_INDEXED_NM */

  /****************************************************************************/
  /* Page0 data                                                               */
  /****************************************************************************/
  /*---- transmit buffer nm message ------------------------------------------*/

  #if defined ( C_COMP_KEIL_C5X5C )
static idata NmStateType nmState;
static idata volatile vuint8 SendMessFlag; /* ESCAN00004259 */

    #if defined ( C_BUFFER_XDATA )
xdata kNmSendMessType nmSendMess[kNmSendMessSize];
    #endif
    #if defined ( C_BUFFER_IDATA )
idata kNmSendMessType nmSendMess[kNmSendMessSize];
    #endif

    #if defined ( NM_ENABLE_SLEEPACK_NOTIFICATION ) || defined ( NM_ENABLE_REMOTESLEEP_INDICATION )
static idata NmStateTypeGwMaster nmStateGwMaster;
    #endif

  #else /* C_COMP_KEIL_C5X5C */

static NmStateType MEMORY_NEAR nmState;
static volatile vuint8 MEMORY_NEAR SendMessFlag; /* ESCAN00004259 */

    #if defined ( C_COMP_COSMIC_ST7 )
    #else
kNmSendMessType MEMORY_NEAR nmSendMess[kNmSendMessSize];
    #endif

    #if defined ( NM_ENABLE_SLEEPACK_NOTIFICATION ) || defined ( NM_ENABLE_REMOTESLEEP_INDICATION )
static NmStateTypeGwMaster MEMORY_NEAR nmStateGwMaster;
    #endif

  #endif /* C_COMP_KEIL_C5X5C */


/****************************************************************************/
/* Extended page variables                                                  */
/****************************************************************************/
static vuint8          TimeCnt;
static NmActionType     nmAction;
static vuint8  RecvNmSourceAdr;
/*--- counters to detect failures of rx/tx functionality of the NM */
static vuint8        nmRxTxCnt;
static vuint8 logicalSuccessor;

  #if defined ( NM_ENABLE_CONFIG )
  /*---- Net management configuration ----------------------------------------*/
static NmConfigType nmConfig;
  #endif

  #if defined ( NM_ENABLE_COPY_RINGDATA_TO_TEMPBUFFER )
/* Received ringdata are copied to global buffer for application usage */
/* but they are not automatically copied to the send message buffer */
static vuint8 nmRecvCopyMsgUser[6];
  #endif




  #if defined ( NM_TYPE_VAG_OSEK )
    #if defined ( NM_ENABLE_VAG_LAH_141 ) && defined ( NM_ENABLE_CAR_WUP_REQUEST )
static volatile tNmCommStateType  nmCarWupState;  /* ESCAN00010855 */
static          vuint8         nmCarWupSendMess;  /* ESCAN00010855 */
    #endif
  #endif  /* NM_TYPE_VAG_OSEK */

  #if defined ( NM_TYPE_VAG_OSEK ) 
    #if defined ( NM_ENABLE_VAG_LAH_130 )
static NmConfigType nmCanErrorConfig;
    #endif
  #endif  /* NM_TYPE_VAG_OSEK || NM_TYPE_PAG2_OSEK */

#endif /* NM_ENABLE_INDEXED_NM */



/******************************************************************************/
/* Defines for nmState, tx and rx buffer access depends on number of channels */
/******************************************************************************/

/* Reset Low Nibble of NmStatLocal */
#define ResetMessRecvFlags()         (NmStatLocal &= (vuint8)0xF0)

#if defined ( NM_ENABLE_INDEXED_NM )

  /****************************************************************************/
  /* Additional defines for required indexed arrays                           */
  /****************************************************************************/

  #define TimeCnt                     TimeCnt[channel]
  #define nmAction                    nmAction[channel]
  #define SendMessFlag                SendMessFlag[channel] /* ESCAN00004259 */
  #define nmRxTxCnt                   nmRxTxCnt[channel]
  #define logicalSuccessor            logicalSuccessor[channel]
  #define RecvNmSourceAdr             RecvNmSourceAdr[channel]

  #define NM_COUNT_RING_TYP           NmCountRingTyp_Field[channel]
  #define NM_COUNT_RING_MAX           NmCountRingMax_Field[channel]
  #define NM_COUNT_LIMPHOME           NmCountLimphome_Field[channel]
  #define NM_COUNT_WAIT_BUSSLEEP      NmCountWaitBussleep_Field[channel]

  #define kNmCanPara                  NmCanPara_Field[channel]

  #if defined ( NM_ENABLE_MULTIPLE_NODES )
    /* new array, because of complex statement */
    #define kNmEcuNr                  NmEcuNr_FieldMultipleNodes[channel]
    #define kNmTxObj                  NmTxObj_FieldMultipleNodes[channel]
  #else
    #define kNmEcuNr                  NmEcuNr_Field[channel]
    #define kNmTxObj                  NmTxObj_Field[channel]
  #endif




  #if defined ( NM_TYPE_VAG_OSEK )
    #if defined ( NM_ENABLE_VAG_LAH_141 ) && defined ( NM_ENABLE_CAR_WUP_REQUEST )
      #define nmCarWupState           nmCarWupState[channel] /* ESCAN00010855 */
      #define nmCarWupSendMess        nmCarWupSendMess[channel] /* ESCAN00010855 */
    #endif
  #endif  /* NM_TYPE_VAG_OSEK */

  /****************************************************************************/
  /* Common with code redoubled section, but channel dependend                */
  /****************************************************************************/

  #define NmStatGlobal                nmState[channel].c[0]
  #define NmStatLocal                 nmState[channel].c[1]

  #define NmStatRingStable            nmState[channel].b.b_RingStable
  #define NmStatBusError              nmState[channel].b.b_BusError
  #define NmStatActive                nmState[channel].b.b_Active
  #define NmStatLimpHome              nmState[channel].b.b_LimpHome
  #define NmStatBusSleep              nmState[channel].b.b_BusSleep
  #define NmStatWaitBusSleep          nmState[channel].b.b_WaitBusSleep
  #if defined ( NM_TYPE_VAG_OSEK ) 
    #if defined ( NM_ENABLE_VAG_LAH_130 )
      #define NmStatErrorBit          nmState[channel].b.b_ErrorBit
    #endif
    #if defined ( NM_ENABLE_VAG_LAH_141 )
      #define NmStatTxRingDataAllow   nmState[channel].b.b_TxRingDataAllow
    #endif
  #endif  /* NM_TYPE_VAG_OSEK || NM_TYPE_PAG2_OSEK */
  #define NmStatBusSleepInd           nmState[channel].b.b_BusSleepInd
 
  #define NmStatNmMsgRecv             nmState[channel].b.b_NmMsgRecv
  #define NmStatRingMsgRecv           nmState[channel].b.b_RingMsgRecv
  #define NmStatImAddressed           nmState[channel].b.b_ImAddressed
  #define NmStatImSkippedOver         nmState[channel].b.b_ImSkippedOver
  #define NmStatNmMsgRecvRemind       nmState[channel].b.b_NmMsgRecvRemind
  #define NmStatRxTxLockUd            nmState[channel].b.b_RxTxLockUd  /* ESCAN00008104 */
  #define NmStatRingPreStable         nmState[channel].b.b_RingPreStable
  #define NmStatPrepSleep             nmState[channel].b.b_PrepSleep

  #if defined ( NM_ENABLE_SLEEPACK_NOTIFICATION ) || defined ( NM_ENABLE_REMOTESLEEP_INDICATION )
    #define NmStatGwMasterFlags       nmStateGwMaster[channel].c[0]
    #define NmStatGwMasterDummy       nmStateGwMaster[channel].c[1]
  #endif

  #if defined( NM_ENABLE_SLEEPACK_NOTIFICATION )
    #define PrepareSendSleepAck       nmStateGwMaster[channel].b.b_PrepareSendSleepAck
  #endif

  #if defined( NM_ENABLE_REMOTESLEEP_INDICATION )
    #define RemotePrepareSleep        nmStateGwMaster[channel].b.b_RemotePrepareSleep
    #define RemoteSleepInd            nmStateGwMaster[channel].b.b_RemoteSleepInd
  #endif

  #define SendMsgByte0           (*(SendMsgTypeUnion*)nmSendMess[channel]).c[0]
  #define SendMsgByte1           (*(SendMsgTypeUnion*)nmSendMess[channel]).c[1]
  #define SendMsgByte2           (*(SendMsgTypeUnion*)nmSendMess[channel]).c[2]

  #define SendSleepAck           (*(SendMsgTypeUnion*)nmSendMess[channel]).b.bSleepAck
  #define SendSleepInd           (*(SendMsgTypeUnion*)nmSendMess[channel]).b.bSleepInd
  #define SendMsgConfig          (*(SendMsgTypeUnion*)nmSendMess[channel]).b.bConfig

    #define SendMsgCmdSet           SendMsgCmd
    #define SendMsgCmdIf            SendMsgCmd

    #define SendMsgEcuNr         (*(SendMsgTypeUnion*)nmSendMess[channel]).c[0]
    #define SendMsgCmd           (*(SendMsgTypeUnion*)nmSendMess[channel]).b.cCmd
    #define SendMsgUser0         (*(SendMsgTypeUnion*)nmSendMess[channel]).c[2]
    #define SendMsgUser1         (*(SendMsgTypeUnion*)nmSendMess[channel]).c[3]
    #define SendMsgUser2         (*(SendMsgTypeUnion*)nmSendMess[channel]).c[4]
    #define SendMsgUser3         (*(SendMsgTypeUnion*)nmSendMess[channel]).c[5]
    #define SendMsgUser4         (*(SendMsgTypeUnion*)nmSendMess[channel]).c[6]
    #define SendMsgUser5         (*(SendMsgTypeUnion*)nmSendMess[channel]).c[7]

  #if defined ( NM_ENABLE_COPY_RINGDATA_TO_TEMPBUFFER )
    #define RecvCopyMsgUser0        nmRecvCopyMsgUser[channel][0]
    #define RecvCopyMsgUser1        nmRecvCopyMsgUser[channel][1]
    #define RecvCopyMsgUser2        nmRecvCopyMsgUser[channel][2]
    #define RecvCopyMsgUser3        nmRecvCopyMsgUser[channel][3]
    #define RecvCopyMsgUser4        nmRecvCopyMsgUser[channel][4]
    #define RecvCopyMsgUser5        nmRecvCopyMsgUser[channel][5]
  #endif

  #if defined ( NM_TYPE_VAG_OSEK ) 
    #if defined ( NM_ENABLE_VAG_LAH_130 )
      #define CanErrorConfig0       nmCanErrorConfig.nmConfigChar[(channel*(vuint8)((NM_NUMBER_OF_CONFIG_NODES)/8))+0]
      #define CanErrorConfig1       nmCanErrorConfig.nmConfigChar[(channel*(vuint8)((NM_NUMBER_OF_CONFIG_NODES)/8))+1]
      #define CanErrorConfig2       nmCanErrorConfig.nmConfigChar[(channel*(vuint8)((NM_NUMBER_OF_CONFIG_NODES)/8))+2]
      #define CanErrorConfig3       nmCanErrorConfig.nmConfigChar[(channel*(vuint8)((NM_NUMBER_OF_CONFIG_NODES)/8))+3]
    #endif
  #endif  /* NM_TYPE_VAG_OSEK || NM_TYPE_PAG2_OSEK */

  #if defined ( NM_TYPE_VAG_OSEK ) 
    #if defined ( NM_ENABLE_VAG_LAH_130 )
      #define TargetErrorConfig0                dataPtr->nmConfigChar[(channel*(vuint8)((NM_NUMBER_OF_CONFIG_NODES)/8))+0]
      #define TargetErrorConfig1                dataPtr->nmConfigChar[(channel*(vuint8)((NM_NUMBER_OF_CONFIG_NODES)/8))+1]
      #define TargetErrorConfig2                dataPtr->nmConfigChar[(channel*(vuint8)((NM_NUMBER_OF_CONFIG_NODES)/8))+2]
      #define TargetErrorConfig3                dataPtr->nmConfigChar[(channel*(vuint8)((NM_NUMBER_OF_CONFIG_NODES)/8))+3]
    #endif
  #endif  /* NM_TYPE_VAG_OSEK || NM_TYPE_PAG2_OSEK */

#else /* NM_ENABLE_INDEXED_NM */

  /****************************************************************************/
  /* Additional defines for required for special implementation               */
  /****************************************************************************/

  #if defined ( NM_ENABLE_MULTIPLE_NODES )
    #if defined ( NM_CAN_CHANNEL )
      #define kNmEcuNr                NmEcuNr_Field[comMultipleECUCurrent[NM_CAN_CHANNEL]]
      #define kNmTxObj                NmTxObj_Field[comMultipleECUCurrent[NM_CAN_CHANNEL]]
    #else
      #define kNmEcuNr                NmEcuNr_Field[comMultipleECUCurrent]
      #define kNmTxObj                NmTxObj_Field[comMultipleECUCurrent]
    #endif
  #endif

  /****************************************************************************/
  /* Common with indexed section, but only code redoubled                     */
  /****************************************************************************/

  #define NmStatGlobal                nmState.c[0]
  #define NmStatLocal                 nmState.c[1]

  #define NmStatRingStable            nmState.b.b_RingStable
  #define NmStatBusError              nmState.b.b_BusError
  #define NmStatActive                nmState.b.b_Active
  #define NmStatLimpHome              nmState.b.b_LimpHome
  #define NmStatBusSleep              nmState.b.b_BusSleep
  #define NmStatWaitBusSleep          nmState.b.b_WaitBusSleep
  #if defined ( NM_TYPE_VAG_OSEK ) 
    #if defined ( NM_ENABLE_VAG_LAH_130 )
      #define NmStatErrorBit          nmState.b.b_ErrorBit
    #endif
    #if defined ( NM_ENABLE_VAG_LAH_141 )
      #define NmStatTxRingDataAllow   nmState.b.b_TxRingDataAllow
    #endif
  #endif  /* NM_TYPE_VAG_OSEK || NM_TYPE_PAG2_OSEK */
  #define NmStatBusSleepInd           nmState.b.b_BusSleepInd

  #define NmStatNmMsgRecv             nmState.b.b_NmMsgRecv
  #define NmStatRingMsgRecv           nmState.b.b_RingMsgRecv
  #define NmStatImAddressed           nmState.b.b_ImAddressed
  #define NmStatImSkippedOver         nmState.b.b_ImSkippedOver
  #define NmStatNmMsgRecvRemind       nmState.b.b_NmMsgRecvRemind
  #define NmStatRxTxLockUd            nmState.b.b_RxTxLockUd  /* ESCAN00008104 */
  #define NmStatRingPreStable         nmState.b.b_RingPreStable
  #define NmStatPrepSleep             nmState.b.b_PrepSleep

  #if defined ( NM_ENABLE_SLEEPACK_NOTIFICATION ) || defined ( NM_ENABLE_REMOTESLEEP_INDICATION )
    #define NmStatGwMasterFlags       nmStateGwMaster.c[0]
    #define NmStatGwMasterDummy       nmStateGwMaster.c[1]
  #endif

  #if defined( NM_ENABLE_SLEEPACK_NOTIFICATION )
    #define PrepareSendSleepAck       nmStateGwMaster.b.b_PrepareSendSleepAck
  #endif

  #if defined( NM_ENABLE_REMOTESLEEP_INDICATION )
    #define RemotePrepareSleep        nmStateGwMaster.b.b_RemotePrepareSleep
    #define RemoteSleepInd            nmStateGwMaster.b.b_RemoteSleepInd
  #endif

  /* Define tx buffer access */
  #define SendSleepAck           (*(SendMsgTypeUnion*)nmSendMess).b.bSleepAck
  #define SendSleepInd           (*(SendMsgTypeUnion*)nmSendMess).b.bSleepInd
  #define SendMsgConfig          (*(SendMsgTypeUnion*)nmSendMess).b.bConfig

  #define SendMsgByte0           (*(SendMsgTypeUnion*)nmSendMess).c[0]
  #define SendMsgByte1           (*(SendMsgTypeUnion*)nmSendMess).c[1]
  #define SendMsgByte2           (*(SendMsgTypeUnion*)nmSendMess).c[2]

    #define SendMsgCmdSet           SendMsgCmd
    #define SendMsgCmdIf            SendMsgCmd

    #define SendMsgEcuNr         (*(SendMsgTypeUnion*)nmSendMess).c[0]
    #define SendMsgCmd           (*(SendMsgTypeUnion*)nmSendMess).b.cCmd
    #define SendMsgUser0         (*(SendMsgTypeUnion*)nmSendMess).c[2]
    #define SendMsgUser1         (*(SendMsgTypeUnion*)nmSendMess).c[3]
    #define SendMsgUser2         (*(SendMsgTypeUnion*)nmSendMess).c[4]
    #define SendMsgUser3         (*(SendMsgTypeUnion*)nmSendMess).c[5]
    #define SendMsgUser4         (*(SendMsgTypeUnion*)nmSendMess).c[6]
    #define SendMsgUser5         (*(SendMsgTypeUnion*)nmSendMess).c[7]

  #if defined ( NM_ENABLE_COPY_RINGDATA_TO_TEMPBUFFER )
    #define RecvCopyMsgUser0        nmRecvCopyMsgUser[0]
    #define RecvCopyMsgUser1        nmRecvCopyMsgUser[1]
    #define RecvCopyMsgUser2        nmRecvCopyMsgUser[2]
    #define RecvCopyMsgUser3        nmRecvCopyMsgUser[3]
    #define RecvCopyMsgUser4        nmRecvCopyMsgUser[4]
    #define RecvCopyMsgUser5        nmRecvCopyMsgUser[5]
  #endif

  #if defined ( NM_TYPE_VAG_OSEK ) 
    #if defined ( NM_ENABLE_VAG_LAH_130 )
      #define CanErrorConfig0       nmCanErrorConfig.nmConfigChar[0]
      #define CanErrorConfig1       nmCanErrorConfig.nmConfigChar[1]
      #define CanErrorConfig2       nmCanErrorConfig.nmConfigChar[2]
      #define CanErrorConfig3       nmCanErrorConfig.nmConfigChar[3]
    #endif
  #endif  /* NM_TYPE_VAG_OSEK || NM_TYPE_PAG2_OSEK */

  #if defined ( NM_TYPE_VAG_OSEK ) 
    #if defined ( NM_ENABLE_VAG_LAH_130 )
      #define TargetErrorConfig0                dataPtr->nmConfigChar[0]
      #define TargetErrorConfig1                dataPtr->nmConfigChar[1]
      #define TargetErrorConfig2                dataPtr->nmConfigChar[2]
      #define TargetErrorConfig3                dataPtr->nmConfigChar[3]
    #endif
  #endif  /* NM_TYPE_VAG_OSEK || NM_TYPE_PAG2_OSEK */

#endif /* NM_ENABLE_INDEXED_NM */


/*****************************************************************************
 * Local function prototypes
 *****************************************************************************/
static void TransmitSkippedAlive( NM_CHANNEL_NMTYPE_ONLY );

static void ResetConfig( NM_CHANNEL_NMTYPE_ONLY );

#if defined ( NM_TYPE_VAG_OSEK ) 
  #if defined ( NM_ENABLE_VAG_LAH_130 )
static void ResetErrorConfig( NM_CHANNEL_NMTYPE_ONLY );
  #endif
#endif


/******************************************************************************
 * Global functions
 ******************************************************************************/
/************************************************************************
| NAME:               NmCanError
| PROTOTYPE:          void NmCanError( void ) OR
|                     void NmCanError ( CanChannelHandle canChannel )
| CALLED BY:          CAN driver (Error Interrupt)
| PRECONDITIONS:      Error interrupts have to be enabled
| INPUT PARAMETERS:   -
| RETURN VALUE:       -
| DESCRIPTION:        Handles detected CAN error ( Bus off detected )
| GLOBAL DATA:        -
| STACK:              -
| RUNTIME:            -
|*************************************************************************/
#if defined ( C_COMP_KEIL_C5X5C )
  #pragma NOAREGS
#endif
/* Call back function of the CAN driver depends on driver type */
void NM_INTERNAL_CALL_TYPE NmCanError(NM_CHANNEL_CANTYPE_ONLY)
{
  #if defined ( NM_ENABLE_INDEXED_NM )
    #if defined ( NM_ENABLE_CHANNEL_INDIRECTION )
  CanChannelHandle channel = NmCanToNmIndirection[canChannel];
    #endif
  #else
    #if defined ( C_MULTIPLE_RECEIVE_CHANNEL )
      #if defined ( NM_CAN_CHANNEL )
        #if defined ( V_ENABLE_USE_DUMMY_STATEMENT )
  canChannel = canChannel; /* avoid compiler warnings */
        #endif
      #endif
    #endif
  #endif

  CanOffline(NM_CHANNEL_CANPARA_ONLY);

  NmStatBusError = 1;

  #if defined ( C_MULTIPLE_RECEIVE_CHANNEL )
  /* makros must be encapsulated in this way */
  CanResetBusOffStart(NM_CHANNEL_CANPARA_ONLY, kNmCanPara);
  #else
  CanResetBusOffStart(kNmCanPara);
  #endif

  if((NmStatPrepSleep == 1) && ((NmStatBusSleep == 1) || (NmStatWaitBusSleep == 1)))
  {
    /*
      ESCAN00009420: BusOff after SleepAck:
                     Hold the Sleep state regardless of BusOff - restart always with Reset Alive.
                     The call of ApplNmBusOff() and ApplNmBusOffEnd() is not symmetric.
                     The Application has to decide, depending on the WaitBusSleep-State within
                     ApplNmBusOff(), if it is neccessary to stop the communication (IL)
                     or not if already in this state.
    */

    #if defined ( C_MULTIPLE_RECEIVE_CHANNEL )
    /* makros must be encapsulated in this way */
    CanResetBusOffEnd(NM_CHANNEL_CANPARA_ONLY, kNmCanPara);
    #else
    CanResetBusOffEnd(kNmCanPara);
    #endif
  }
  else
  {
    ResetMessRecvFlags();
    NmStatNmMsgRecvRemind = 0;
    NmStatRxTxLockUd      = 0; /* ESCAN00008104 */
    SendMessFlag          = 0;

    nmRxTxCnt = TX_CNT_STEP; /* clear rx counter; set tx counter = 1*/

    if((nmAction & ((NmActionType)~NM_ACTION_REPEAT)) != NM_ACTION_LH_PREPARE_SLEEP)
    {
      TimeCnt   = NM_COUNT_LIMPHOME;
      nmAction  = NM_ACTION_LH_WAIT_LIMPHOME_TX;    /* start state limphome */
    }
  }


  ApplNmBusOff(NM_CHANNEL_APPLPARA_ONLY);
} /* end of NmCanError() */


/************************************************************************
| NAME:               NmConfirmation
| PROTOTYPE:          void NmConfirmation(CanTransmitHandle tmtObject)
| CALLED BY:          CAN driver (Tx Interrupt)
| PRECONDITIONS:      CAN tx interrupts have to be enabled
| INPUT PARAMETERS:   Index on TX object (not used)
| RETURN VALUE:       -
| DESCRIPTION:        Confirmation routine for net management CAN messages
| GLOBAL DATA:        -
| STACK:              -
| RUNTIME:            -
|*************************************************************************/
#if defined ( C_COMP_KEIL_C5X5C )
  #pragma NOAREGS
#endif
void NM_INTERNAL_CALL_TYPE NmConfirmation(CanTransmitHandle tmtObject)
{
  #if defined ( NM_ENABLE_ONM_EXT_API )
  NmMsgType onmStateParam = NM_MSG_INVALID; 
  #endif

  #if defined ( NM_TYPE_VAG_OSEK ) 
    #if defined ( NM_ENABLE_VAG_LAH_130 ) && defined ( NM_ENABLE_ERRORPIN_SEND )
  vuint8 configByte = 0;
    #endif
  #endif  /* NM_TYPE_VAG_OSEK || NM_TYPE_PAG2_OSEK */

  #if defined ( C_MULTIPLE_RECEIVE_CHANNEL ) && defined ( NM_ENABLE_INDEXED_NM )
  CanChannelHandle channel;

  /* Calculate logical channel of nm based on physical channel (tmtObject) of the CAN driver */
    #if defined( NM_ENABLE_MULTIPLE_NODES )
  for(channel = 0; tmtObject != NmTxObj_Field[channel][comMultipleECUCurrent[channel]]; channel++ ){;} /* ESCAN00014458 */
    #else
  for(channel = 0; tmtObject != NmTxObj_Field[channel]; channel++ ){;}
    #endif
  #else
    #if defined ( V_ENABLE_USE_DUMMY_STATEMENT )
  tmtObject = tmtObject;
    #endif
  #endif

  StartMeasure(NmConfirmationTimings);

  NmStatRxTxLockUd = 0; /* ESCAN00008104 - release waiting for NmConfirmation() */

  nmRxTxCnt &= ((vuint8)~TX_CNT_MASK);   /* Reset transmit counter  */


/*---------------------------------------------------------------------------*/
  if(SendMsgCmdIf == RING)
/*---------------------------------------------------------------------------*/
  {
    #if defined ( NM_ENABLE_ONM_EXT_API )
    onmStateParam |= NM_MSG_RING; 
    #endif

    /* ESCAN00007856 */

    if((NmStatPrepSleep == 1) && (SendSleepAck == 1))  /* check Sleep Prepare */
    {
      CanOffline(NM_CHANNEL_CANPARA_ONLY);

      #if defined ( NM_ENABLE_ONM_EXT_API )
      onmStateParam |= NM_MSG_SLEEP_ACK; 
      #endif

      NmStatWaitBusSleep = 1;
      SendMessFlag       = 0;  /* cancel nm tx request, NmStatRxTxLockUd is already 0 */


      TimeCnt  = NM_COUNT_WAIT_BUSSLEEP;
      nmAction = NM_ACTION_GO_BUSSLEEP;
    }
    else
    {
      if((NmStatBusSleepInd == 1) && (SendSleepInd == 1))
      {
        #if defined ( NM_ENABLE_ONM_EXT_API )
        onmStateParam |= NM_MSG_SLEEP_IND; 
        #endif

        NmStatPrepSleep = 1;

        #if defined( NM_ENABLE_REMOTESLEEP_INDICATION )
        /* Remote sleep indication depends only on received sleep indication flags */                 
        #endif
      }

      /*
        Do not wait for any Ring message if we are currently go to sleep.
        This could happen, if we receive a Sleep Acknowledge (NmPrecopy()) while waiting for
        our own Confirmation for the Sleep Indication message.
        Then, this last retrigger and state transition could be discarded.
        Leave the NmStatImAddressed and NmStatPrepSleep information too.
        These flags got it's latest update also in NmPrecopy().
      */
      if(NmStatWaitBusSleep == 0) /* ESCAN00007856 */
      {
        NmStatImAddressed = 0;
        NmStatLimpHome = 0; /* ESCAN00017962 */

        /* Retrigger timer to avoid duplicate rings */
        TimeCnt  = NM_COUNT_RING_MAX;
        nmAction = NM_ACTION_WAIT_RINGMSG;
      }
    }
  }
  else
  {
/*---------------------------------------------------------------------------*/
    if(SendMsgCmdIf == LIMPHOME)
/*---------------------------------------------------------------------------*/
    {
      #if defined ( NM_ENABLE_ONM_EXT_API )
      onmStateParam |= NM_MSG_LIMPHOME; 
      #endif

      if((NmStatActive == 1) && (NmStatNmMsgRecvRemind == 1))
      {
        nmAction = NM_ACTION_START;
      }
    }
/*---------------------------------------------------------------------------*/
    else
    {
/*---------------------------------------------------------------------------*/
      if(SendMsgCmdIf == ALIVE)
/*---------------------------------------------------------------------------*/
      {
        #if defined ( NM_ENABLE_ONM_EXT_API )
        if(SendMsgEcuNr == kNmEcuNr)  /* address is own nodenr. ? */
        {
          onmStateParam |= NM_MSG_RESET_ALIVE; 
        }
        else
        {
          onmStateParam |= NM_MSG_SKIPPED_ALIVE; 
        }
        #endif

        if((nmAction ==  NM_ACTION_LH_WAIT_LIMPHOME_TX)||
           (nmAction ==  NM_ACTION_LH_PREPARE_SLEEP   )  )
        {
          /* set tx counter, use as Limphome tx mark, reset only on limphome msg */
          nmRxTxCnt = TX_CNT_STEP; 
        }
      }
    }
  } /* endif (SendMsgCmdIf == RING) */

  #if defined ( NM_TYPE_VAG_OSEK )
    #if defined ( NM_ENABLE_VAG_LAH_141 ) && defined ( NM_ENABLE_CAR_WUP_REQUEST ) /* ESCAN00010855 */
  /* CarWakeUp message was sent? */

  if((SendMsgByte1 & CAR_WUP) == CAR_WUP)
  {
    #if defined ( NM_ENABLE_ONM_EXT_API )
    onmStateParam |= NM_MSG_CAR_WUP;  /* notify application */
    #endif

    SendMsgByte1 &= ((vuint8)~CAR_WUP);
  }
    #endif
  #endif  /* NM_TYPE_VAG_OSEK */

  #if defined ( NM_TYPE_VAG_OSEK ) 
    #if defined ( NM_ENABLE_VAG_LAH_130 ) && defined ( NM_ENABLE_ERRORPIN_SEND )
  /* calculate the index for the config table */
      #if defined ( NM_ENABLE_INDEXED_NM )
  configByte = (vuint8)((vuint8)(kNmEcuNr >> 3) + (channel * (vuint8)((NM_NUMBER_OF_CONFIG_NODES)/8)));
      #else
  configByte = (vuint8)(kNmEcuNr >> 3);
      #endif

  /*
    read status of bus transceiver error pin and Set/reset own bit
    in error configuration depending on current error pin value
  */

  if(ApplNmCanErrorPinSend(NM_CHANNEL_APPLPARA_ONLY) != 0)
  {
    /* Set Bits in Errorpinbyte Buffer */
    nmCanErrorConfig.nmConfigChar[configByte] |= (vuint8)(1 << (kNmEcuNr & BYTE_OFFSET_MASK));
  }
  else
  {
    /* Delete Bits in Errorpinbyte Buffer */
    nmCanErrorConfig.nmConfigChar[configByte] &= (vuint8)(~((vuint8)(1 << (kNmEcuNr & BYTE_OFFSET_MASK))));
  }
    #endif
  #endif /* NM_TYPE_VAG_OSEK || NM_TYPE_PAG2_OSEK */

  #if defined ( NM_TYPE_VAG_OSEK ) 
    #if defined ( NM_ENABLE_VAG_LAH_130 )
  /* State of NM status error bit depends on previous error configuration and current state of error pin */
  if((CanErrorConfig0 != 0) || (CanErrorConfig1 != 0) ||
     (CanErrorConfig2 != 0) || (CanErrorConfig3 != 0))
  {
    if(NmStatErrorBit == 0)
    {
      NmStatErrorBit = 1;
      #if defined ( NM_ENABLE_CAN_ERRORBIT )
      ApplNmErrorBitToOn(NM_CHANNEL_APPLPARA_ONLY);
      #endif
    }
  }
  else
  {
    if(NmStatErrorBit == 1)
    {
      NmStatErrorBit = 0;
      #if defined ( NM_ENABLE_CAN_ERRORBIT )
      ApplNmErrorBitToOff(NM_CHANNEL_APPLPARA_ONLY);
      #endif
    }
  }
    #endif
  #endif  /* NM_TYPE_VAG_OSEK || NM_TYPE_PAG2_OSEK */

  #if defined ( NM_ENABLE_ONM_EXT_API )
  OnmExtConfirmation(NM_CHANNEL_NMPARA_FIRST onmStateParam); 
  #endif

  StopMeasure(NmConfirmationTimings);

} /* end of NmConfirmation() */


/************************************************************************
| NAME:               NmPrecopy
| PROTOTYPE:          vuint8 NmPrecopy(CanTransmitHandle rcvObject) or
|                     vuint8 NmPrecopy(CanChipDataPtr ReceivedMessage)
| CALLED BY:          CAN driver (Rx Interrupt)
| PRECONDITIONS:      CAN Rx interrupts have to be enabled
| INPUT PARAMETERS:   Index on RX object (not used)   OR
|                     Pointer to RX object
| RETURN VALUE:       kCanNoCopyData: Do not copy data, because of no
|                                     RX buffer for NM messages
| DESCRIPTION:        Precopy routine for net management CAN messages
| GLOBAL DATA:
| STACK:
| RUNTIME:
|*************************************************************************/
#if defined ( C_COMP_KEIL_C5X5C )
  #pragma NOAREGS
#endif
#if defined ( C_SINGLE_RECEIVE_BUFFER )
vuint8 NM_INTERNAL_CALL_TYPE NmPrecopy(CanReceiveHandle rcvObject)
{
#endif
#if defined ( C_MULTIPLE_RECEIVE_BUFFER )
vuint8 NM_INTERNAL_CALL_TYPE NmPrecopy(CanChipDataPtr rxDataPtr)
{
#endif
#if defined ( C_MULTIPLE_RECEIVE_CHANNEL ) || defined ( C_SINGLE_RECEIVE_CHANNEL )
vuint8 NM_INTERNAL_CALL_TYPE NmPrecopy(CanRxInfoStructPtr rxStruct)
{
#endif

  #if defined ( NM_TYPE_VAG_OSEK ) 
    #if defined ( NM_ENABLE_VAG_LAH_130 )
  /* ESCAN00014635 */
  vuint8 ErrorPin = 0;
    #endif
  #endif

  #if defined ( NM_ENABLE_ONM_EXT_API )
  NmMsgType onmStateParam = NM_MSG_INVALID; 
  #endif

  /*-------------------- start of function -----------------------*/
  #if defined ( NM_ENABLE_INDEXED_NM )
    #if defined ( NM_ENABLE_CHANNEL_INDIRECTION )
  CanChannelHandle channel = NmCanToNmIndirection[rxStruct->Channel];
    #else
  CanChannelHandle channel = rxStruct->Channel;
    #endif
  #endif

  #if defined ( NM_ENABLE_CONFIG ) || defined ( NM_ENABLE_VAG_LAH_130 )
  vuint8 configByte = 0;
  #endif

  StartMeasure(NmPrecopyTimings);

  #if defined ( C_SINGLE_RECEIVE_BUFFER )
    #if defined ( V_ENABLE_USE_DUMMY_STATEMENT )
  rcvObject = rcvObject; /* avoid compiler warnings */
    #endif
  #endif

  /* Get the source address valid until the next message will be received */
  RecvNmSourceAdr = GetRecvNmSourceAdr;

  #if defined ( C_COMP_KEIL_C5X5C )
    #if defined ( C_CAN_CHIP_PDATA )
  XPAGE = 0xF7; /* 8 bit addressing of CAN */
    #endif
  #endif


/*---------------------------------------------------------------------------*/
  if((RecvNmCmd == RING ) || (RecvNmCmd == ALIVE))
/*---------------------------------------------------------------------------*/
  {
    /* ESCAN00007856 */

    NmStatNmMsgRecv = 1;

    #if defined ( NM_ENABLE_CONFIG )
    /* set current configuration bit -------------------------------------*/
    if(RecvNmSourceAdr <= ((vuint8)((NM_NUMBER_OF_CONFIG_NODES) - 1)))
    {
      /* calculate the index for the config table */
      #if defined ( NM_ENABLE_INDEXED_NM )
      configByte = (vuint8)((vuint8)(RecvNmSourceAdr >> 3) + (channel * (vuint8)((NM_NUMBER_OF_CONFIG_NODES)/8)));
      #else
      configByte = (vuint8)(RecvNmSourceAdr >> 3);
      #endif

      nmConfig.nmConfigChar[configByte] |= (vuint8)(1 << (RecvNmSourceAdr & BYTE_OFFSET_MASK)); /* set the received ECU number as present */
    }
    #endif /* NM_ENABLE_CONFIG */

    /* calculate new log successor ------------------------------------- */
    if(logicalSuccessor > kNmEcuNr)
    {
      if((RecvNmSourceAdr < logicalSuccessor)&&
         (RecvNmSourceAdr > kNmEcuNr        )  )
      {
        logicalSuccessor = RecvNmSourceAdr;
      }
    }
    else
    {
      if((RecvNmSourceAdr < logicalSuccessor)||
         (RecvNmSourceAdr > kNmEcuNr        )  )
      {
        logicalSuccessor = RecvNmSourceAdr;
      }
    }

/*---------------------------------------------------------------------------*/
    if(RecvNmCmd == RING)                   /* RING message received */
/*---------------------------------------------------------------------------*/
    {
      #if defined ( NM_ENABLE_ONM_EXT_API )
      onmStateParam |= NM_MSG_RING; 
      #endif

      NmStatRingMsgRecv = 1;

/*---- check for own node was addressed --------------------------------------*/
      if((RecvNmDestAdr == kNmEcuNr       )||   /*I'm addressed or    */
         (RecvNmDestAdr == RecvNmSourceAdr)  )  /*sender was addressed*/
      {
        #if defined ( NM_ENABLE_ONM_EXT_API )
        onmStateParam |= NM_MSG_WAIT_TX; 
        #endif

        NmStatImAddressed = 1;

        #if defined ( NM_TYPE_VAG_OSEK ) 
        /* do not copy user data */
        #endif  /* NM_TYPE_VAG_OSEK || NM_TYPE_PAG2_OSEK */
      }
      else
      {
/*---- own node not addressed , check for skipped ---------------------------*/
        NmStatImAddressed = 0;

      #if defined ( NM_INT_ENABLE_OPTIMIZED_SKIPPED_DETECTION )
        /* Code: Special use case: In this configuration, the kNmCfg_EcuNr is a preprocessor define with the value 0.
         *       This leads to some compiler warnings for the standard algorithm. 
         *       The warnings can be removed and the code can be optimized for this use case.*/
        /* ESCAN00020580 */
        if( RecvNmSourceAdr >= RecvNmDestAdr )
        {
          NmStatImSkippedOver = 1;
        }
      #else

        if((vsint16)RecvNmDestAdr < (vsint16)kNmEcuNr) /* ESCAN00007275 */
        {
          if((         RecvNmSourceAdr >= RecvNmDestAdr     )&& 
             ((vsint16)RecvNmSourceAdr <  (vsint16)kNmEcuNr))
          {
            NmStatImSkippedOver = 1;
          }
        }
        else
        {
          if(RecvNmSourceAdr < RecvNmDestAdr)
          {
            if((vsint16)RecvNmSourceAdr < (vsint16)kNmEcuNr)
            {
              NmStatImSkippedOver = 1;
            }
          }
          else
          {
            NmStatImSkippedOver = 1;
          }
        }
      #endif
      }

      #if defined ( NM_TYPE_VAG_OSEK ) 
        #if defined ( NM_ENABLE_VAG_LAH_130 )
      /* Set Error Pin only if in a valid range */
      if(RecvNmSourceAdr <= ((vuint8)((NM_NUMBER_OF_CONFIG_NODES) - 1)))
      {
        /* calculate the index for the config table */
        #if defined ( NM_ENABLE_INDEXED_NM )
        configByte = (vuint8)((vuint8)(RecvNmSourceAdr >> 3) + (channel * (vuint8)((NM_NUMBER_OF_CONFIG_NODES)/8)));
        #else
        configByte = (vuint8)(RecvNmSourceAdr >> 3);
        #endif

        /*
          read status of bus transceiver error pin and Set/reset own bit
          in error configuration depending on current error pin value
        */

        /* ESCAN00014635 */
        if(ApplNmCanErrorPinReceive(NM_CHANNEL_APPLPARA_ONLY) != 0)
        {
          ErrorPin = 1;
          /* Set Bits in Errorpinbyte Buffer */
          nmCanErrorConfig.nmConfigChar[configByte] |= (vuint8)(1 << (RecvNmSourceAdr & BYTE_OFFSET_MASK));
        }
        else
        {
          ErrorPin = 0;
          /* Delete Bits in Errorpinbyte Buffer */
          nmCanErrorConfig.nmConfigChar[configByte] &= (vuint8)(~(vuint8)(1 << (RecvNmSourceAdr & BYTE_OFFSET_MASK)));
        }
      }
      /* State of NM status error bit depends on previous error configuration and current state of error pin */
      /* ESCAN00014635 */
      if((RecvMsgUser0 != 0) || (RecvMsgUser1 != 0) ||
         (RecvMsgUser2 != 0) || (RecvMsgUser3 != 0) || (ErrorPin != 0))
      {
        if(NmStatErrorBit == 0)
        {
          NmStatErrorBit = 1;
          #if defined ( NM_ENABLE_CAN_ERRORBIT )
          ApplNmErrorBitToOn(NM_CHANNEL_APPLPARA_ONLY);
          #endif
        }
      }
      else
      {
        if(NmStatErrorBit == 1)
        {
          NmStatErrorBit = 0;
          #if defined ( NM_ENABLE_CAN_ERRORBIT )
          ApplNmErrorBitToOff(NM_CHANNEL_APPLPARA_ONLY);
          #endif
        }
      }
        #endif
      #endif  /* NM_TYPE_VAG_OSEK || NM_TYPE_PAG2_OSEK */
    }
/*---------------------------------------------------------------------------*/
    else                                           /* ALIVE message received */
/*---------------------------------------------------------------------------*/
    {
      #if defined ( NM_ENABLE_ONM_EXT_API )
      if(RecvNmDestAdr == RecvNmSourceAdr)  /* address is own nodenr. ? */
      {
        onmStateParam |= NM_MSG_RESET_ALIVE; 
      }
      else
      {
        onmStateParam |= NM_MSG_SKIPPED_ALIVE; 
      }
      #endif

      NmStatRingStable    = 0;
      NmStatRingPreStable = 0;

      #if defined ( NM_ENABLE_REMOTESLEEP_INDICATION )  /* ESCAN00007430 */
      /* Remote sleep indication depends only on received sleep indication flags */

      /*
        Any NM message without set Sleep Indication resets the Remote Sleep Indication.
        This will be done a few lines later ->  if(RecvNmSleepInd != SLEEP_IND) ... .
        That means, a Skipped Alive with a set Sleep Ind does not influence the network remote sleep state.
        A Startup Alive has ever! no set Sleep Ind.
      */
      #endif
    }
  }
/*---------------------------------------------------------------------------*/
  else                                          /* LIMPHOME message received */
/*---------------------------------------------------------------------------*/
  {
    if(RecvNmCmd == LIMPHOME)
    {
      #if defined ( NM_ENABLE_ONM_EXT_API )
      onmStateParam |= NM_MSG_LIMPHOME; 
      #endif

      NmStatNmMsgRecv = 1;
    }
  }

/*---------------------------------------------------------------------------*/
  if(NmStatNmMsgRecv == 1)                        /* VALID NM message received */
/*---------------------------------------------------------------------------*/
  {
    #if defined ( NM_ENABLE_ONM_EXT_API )
    if(RecvNmSleepInd == SLEEP_IND)
    {
      onmStateParam |= NM_MSG_SLEEP_IND;  
    }
    #endif

    nmRxTxCnt &= (vuint8)~RX_CNT_MASK;  /* receive counter reset */

    /* Check for sleep prepare */
    if((RecvNmSleepInd != SLEEP_IND) || (NmStatBusSleep == 1)) /* ESCAN00007436 */
    {
      NmStatPrepSleep = 0;

      #if defined ( NM_ENABLE_SLEEPACK_NOTIFICATION )
      PrepareSendSleepAck = 0;
      SendSleepAck        = 0;
      #endif

      #if defined ( NM_ENABLE_REMOTESLEEP_INDICATION )
      RemotePrepareSleep = 0;
      RemoteSleepInd     = 0;
      #endif

    }

    if(RecvNmSleepAck == SLEEP_ACK)
    {
      #if defined ( NM_ENABLE_ONM_EXT_API )
      onmStateParam |= NM_MSG_SLEEP_ACK;  
      #endif

      if((NmStatBusSleep == 0) && (NmStatWaitBusSleep == 0))  /* ESCAN00007535 */
      {
        /* Start transition to BUSSLEEP */

        if(NmStatBusSleepInd == 1)
        {
          /* Sleep-Acknowlege received a n d  ready to go to BusSleep */

          /* Stop transmission immediately, GO BUSSLEEP */
          CanOffline(NM_CHANNEL_CANPARA_ONLY);

          NmStatPrepSleep    = 1;
          NmStatWaitBusSleep = 1;
          NmStatRxTxLockUd   = 0; /* ESCAN00008104 */
          SendMessFlag       = 0; /* cancel nm tx request */

          #if defined ( NM_ENABLE_RINGDATA_ACCESS )
          NmStatTxRingDataAllow = 0;  /* disable user(ring)data access */
          #endif

          #if defined ( NM_ENABLE_REMOTESLEEP_INDICATION )
          RemotePrepareSleep = 1;
          RemoteSleepInd     = 1;
          #endif


          TimeCnt  = NM_COUNT_WAIT_BUSSLEEP;
          nmAction = NM_ACTION_GO_BUSSLEEP;

          /*
            ESCAN00009711: SleepAck after BusOff:
                           Hold the Sleep state regardless of BusOff - restart always with Reset Alive.
                           The call of ApplNmBusOff() and ApplNmBusOffEnd() is not symmetric.
                           ApplNmWaitBusSleep(), has always a higher priority to stop the
                           communication (IL).
          */
        }
        else
        {
          /* Sleep-Acknowlege received but  n o t  ready to go to BusSleep */

          /*ESCAN00021051*/
          if( (nmAction == NM_ACTION_LH_WAIT_LIMPHOME_TX) || 
              (nmAction == NM_ACTION_LH_PREPARE_SLEEP) )
          {
            if( NmStatBusError == 1 )
            {
              NmStatBusError = 0;

              #if defined( C_MULTIPLE_RECEIVE_CHANNEL )
              /* macros must be encapsulated in this way */
              CanResetBusOffEnd(NM_CHANNEL_CANPARA_ONLY, kNmCanPara);
              #else
              CanResetBusOffEnd(kNmCanPara);
              #endif

              #if defined( NM_ENABLE_EXTENDED_CALLBACK )
              ApplNmBusOffEnd(NM_CHANNEL_APPLPARA_ONLY);
              #endif
            }
          }

          #if defined ( NM_ENABLE_IMMEDIATE_ALIVE )
          NmStatPrepSleep = 0;
          nmAction = NM_ACTION_START; /* restart Network Management immediately */
          #endif

          #if defined ( NM_ENABLE_SLEEPACK_NOTIFICATION )
          PrepareSendSleepAck = 0;
          SendSleepAck        = 0;
          #endif

          #if defined ( NM_ENABLE_REMOTESLEEP_INDICATION )
          RemotePrepareSleep = 0;
          RemoteSleepInd     = 0;
          #endif

        }
      }
    }
  } /* endif NmStatNmMsgRecv */

  #if defined ( NM_ENABLE_ONM_EXT_API )
  OnmExtPrecopy(NM_CHANNEL_NMPARA_FIRST onmStateParam, RecvUdStartAdr); 
  #endif

  StopMeasure(NmPrecopyTimings);

  return(kCanNoCopyData);
} /* end of NmPrecopy() */


/************************************************************************
| NAME:               NmOsekInit
| PROTOTYPE:          void NmOsekInit(NmInitType InitMode)
| CALLED BY:          StartNM / StopNM (internal)
|                     Station Management / Application
| PRECONDITIONS:      Bus Transceiver and CAN driver have to be initialized
| INPUT PARAMETERS:   mode: Init parameter
| RETURN VALUE:       -
| DESCRIPTION:        Init of network management
| GLOBAL DATA:
| STACK:
| RUNTIME:
|*************************************************************************/
#if defined ( C_COMP_KEIL_C5X5C )
  #pragma NOAREGS
#endif
void NM_API_CALL_TYPE NmOsekInit(NM_CHANNEL_NMTYPE_FIRST NmInitType InitMode)
{
  #if defined ( NM_ENABLE_INDEXED_NM ) && defined ( NM_ENABLE_CHANNEL_INDIRECTION )
    #if defined ( NM_ENABLE_MULTIPLE_NODES )
  CanChannelHandle canChannel = channel;
    #endif
  channel = NmCanToNmIndirection[channel];
  #endif

  nmAction = (NmActionType)InitMode; /* use nmAction as temporary variable */

  /*---- Disable and Save Interrupt Mask ------------*/
  ApplCanAddCanInterruptDisable(0);

  #if defined ( NM_ENABLE_INDEXED_NM ) && defined ( NM_ENABLE_MULTIPLE_NODES )
    #if defined ( NM_ENABLE_CHANNEL_INDIRECTION )
  kNmEcuNr = NmEcuNr_Field[channel][comMultipleECUCurrent[canChannel]];
  kNmTxObj = NmTxObj_Field[channel][comMultipleECUCurrent[canChannel]];
    #else
  kNmEcuNr = NmEcuNr_Field[channel][comMultipleECUCurrent[channel]];
  kNmTxObj = NmTxObj_Field[channel][comMultipleECUCurrent[channel]];
    #endif
  #endif

  NmStatGlobal    = 0;
  NmStatLocal     = 0;
  SendMessFlag    = 0; /* ESCAN00004259 */

  TimeCnt         = 0;
  nmRxTxCnt       = 0; /* reset watch counter */
  RecvNmSourceAdr = kNmEcuNr;  /* initial value - self addressed */

  #if defined ( NM_TYPE_VAG_OSEK )
    #if defined ( NM_ENABLE_VAG_LAH_141 ) && defined ( NM_ENABLE_CAR_WUP_REQUEST )
  nmCarWupState    = CarWupOff; /* ESCAN00010855 */
  nmCarWupSendMess = 0;         /* ESCAN00010855 */
    #endif
  #endif  /* NM_TYPE_VAG_OSEK */

  #if defined ( NM_ENABLE_SLEEPACK_NOTIFICATION ) || defined ( NM_ENABLE_REMOTESLEEP_INDICATION )
  NmStatGwMasterFlags = 0;
  NmStatGwMasterDummy = 0;
  #endif


  #if defined ( NM_ENABLE_SLEEPACK_NOTIFICATION )
  if(InitMode != NM_SENDSLEEPACK)
  #endif
  {
    /*
       Standard default initialization.
       Else, leave NM message configuration for asynchron Sleep Ack.
       Reset only Sleep Acknowledge information, for next retrigger.
    */
    SendMsgByte0 = CTRL_INIT; /* initialize Ring Address and Control Byte */
    SendMsgByte1 = CTRL_INIT;


    ResetConfig(NM_CHANNEL_NMPARA_ONLY); /* internal function */
  }


  #if defined ( NM_TYPE_VAG_OSEK ) 
    #if defined ( NM_ENABLE_VAG_LAH_130 )
  ResetErrorConfig(NM_CHANNEL_NMPARA_ONLY); /* internal function */
    #endif
  #endif  /* NM_TYPE_VAG_OSEK || NM_TYPE_PAG2_OSEK */

  switch((NmInitType)nmAction)
  {
    /*---------------------------------------------------------------------------*/
    case NM_NORMAL:
      NmStatActive = 1;
      break;
    /*---------------------------------------------------------------------------*/
    case NM_NMPASSIVE:
      break;
    /*---------------------------------------------------------------------------*/
    case NM_SLEEPIND:
      NmStatActive      = 1;
      NmStatBusSleepInd = 1;

      break;
    /*---------------------------------------------------------------------------*/
    case NM_SLEEPIND_NMPASSIVE:
      NmStatBusSleepInd = 1;
      break;

      #if defined ( NM_ENABLE_SLEEPACK_NOTIFICATION )
    /*---------------------------------------------------------------------------*/
    case NM_SENDSLEEPACK:
      NmStatActive        = 1;
      NmStatBusSleepInd   = 1;
      NmStatPrepSleep     = 1;
      nmPrepareSleepCnt   = 0;
      PrepareSendSleepAck = 1;
      SendSleepAck        = 0;
      #if defined ( NM_ENABLE_REMOTESLEEP_INDICATION )
      RemotePrepareSleep  = 1;
      RemoteSleepInd      = 1;
      #endif

      /*
        NmStatImSkippedOver and/or NmStatRingMsgRecv could be cleared
        while re-initialization
       */

      if(logicalSuccessor == kNmEcuNr)
      {
        logicalSuccessor = (vuint8)(~kNmEcuNr);
      }
      /* else, leave the successor */

      nmAction = NM_ACTION_WAIT_SENDTOKEN;

      ApplCanAddCanInterruptRestore(0);
      return;
      #endif /* NM_ENABLE_SLEEPACK_NOTIFICATION */
    /*---------------------------------------------------------------------------*/
    case NM_CANSLEEP:
      CanOffline(NM_CHANNEL_CANPARA_ONLY);

      NmStatActive      = 1;
      NmStatBusSleepInd = 1;
      NmStatPrepSleep   = 1;
      NmStatBusSleep    = 1;

      nmAction = NM_ACTION_BUS_SLEEP;


      #if defined ( NM_ENABLE_SLEEPACK_NOTIFICATION )
      PrepareSendSleepAck = 1;
      SendSleepAck        = 1;
      #endif

      #if defined ( NM_ENABLE_REMOTESLEEP_INDICATION )
      RemotePrepareSleep = 1;
      RemoteSleepInd     = 1;
      #endif

      #if defined ( NM_ENABLE_ONM_EXT_API )
      OnmExtInit(NM_CHANNEL_NMPARA_ONLY); 
      #endif

      ApplNmCanSleep(NM_CHANNEL_APPLPARA_ONLY);
      ApplNmCanBusSleep(NM_CHANNEL_APPLPARA_ONLY);

      ApplCanAddCanInterruptRestore(0);
      return;
    /*---------------------------------------------------------------------------*/
    case NM_DISABLE:
    /*---------------------------------------------------------------------------*/
    default :
      #if defined ( NM_ENABLE_ONM_EXT_API )
      OnmExtInit(NM_CHANNEL_NMPARA_ONLY); 
      #endif

      nmAction = NO_ACTION;

      /* restore old interrupt mask */
      ApplCanAddCanInterruptRestore(0);
      return;
  } /* switch((NmInitType)nmAction) */

  #if defined ( NM_ENABLE_ONM_EXT_API )
  OnmExtInit(NM_CHANNEL_NMPARA_ONLY); 
  #endif

  ApplNmCanNormal(NM_CHANNEL_APPLPARA_ONLY);  /* wakeup CAN-Chip; switch Tranceiver to normal state */
  CanOnline(NM_CHANNEL_CANPARA_ONLY); /* switch CAN driver to 'Online' */
  #if defined (NM_ENABLE_EXTENDED_CALLBACK)
  ApplNmBusStart(NM_CHANNEL_APPLPARA_ONLY);
  #endif

  nmAction = NM_ACTION_START;

  ApplCanAddCanInterruptRestore(0); /*---- Restore old Interrupt Mask ----*/
} /* end of NmOsekInit() */


/************************************************************************
| NAME:               NmTask
| PROTOTYPE:          void NmTask(void)
| CALLED BY:          Station Management / Application
| PRECONDITIONS:      NM has to be initialized
| INPUT PARAMETERS:   -
| RETURN VALUE:       -
| DESCRIPTION:        Main task of NM, contains state machine and has to be
|                     called by frequently
| GLOBAL DATA:
| STACK:
| RUNTIME:
|*************************************************************************/
#if defined ( C_COMP_KEIL_C5X5C )
  #pragma NOAREGS
#endif
void NM_API_CALL_TYPE NmTask(NM_CHANNEL_NMTYPE_ONLY)
{
  #if defined ( NM_ENABLE_ONM_EXT_API )
  NmMsgType onmStateParam; 
  #endif

  StartMeasure(NmTaskTimings);

  #if defined ( NM_ENABLE_INDEXED_NM ) && defined ( NM_ENABLE_CHANNEL_INDIRECTION )
  channel = NmCanToNmIndirection[channel];
  #endif

  ApplCanAddCanInterruptDisable(0);
  if(TimeCnt > 0)
  {
    TimeCnt--;
  }

  ApplCanAddCanInterruptRestore(0);

/*----------------------------------------------------------------------------
 * Loop for an immediate switch to the next state in time-critical cases.
 *  If source code is changed, pay attention that NM_ACTION_REPEAT does not
 *  provoke a multiple repetition (deadlock!).
 *---------------------------------------------------------------------------*/

  do
  {
    #if defined ( NM_ENABLE_ONM_EXT_API )
    /* collect new states always, because of REPEAT flag */
    onmStateParam = NM_MSG_INVALID; 
    #endif
/*---- Disable and Save Interrupt -------------------------------------------*/
    //GIO_vPinWrite(PORT_NOTUSED_PIN7_GIOA, LOW, GIOA);
    ApplCanAddCanInterruptDisable(0);
/*---------------------------------------------------------------------------*/
    nmAction &= (NmActionType)~NM_ACTION_REPEAT;
 
/*---- Check transmit counter, if not ok, start limp home mode -----------*/
    if((vuint8)((nmRxTxCnt & TX_CNT_MASK)>>4) > TX_CNT_MAX)
    {                              /* tx counter run off-> state limp home */
      nmRxTxCnt = TX_CNT_STEP; /* set tx counter; use as Limphome tx mark*/
      TimeCnt = NM_COUNT_LIMPHOME;
      nmAction = NM_ACTION_LH_WAIT_LIMPHOME_TX; /* start state limp home */
    }

/*---- State machine for NM states ------------------------------------------*/
    switch(nmAction)
    {
/*---------------------------------------------------------------------------*/
      case    NM_ACTION_START:
/*---------------------------------------------------------------------------*/
        #if defined ( NM_ENABLE_ONM_EXT_API )
        /* Use SendMessFlag to detect a valid Tx entry */
        onmStateParam |= (NmMsgType)(NM_MSG_RESET_ALIVE | NM_MSG_WAIT_TX); 
        #endif

        ResetConfig( NM_CHANNEL_NMPARA_ONLY ); /* internal function */

        /* Reset all NmStatLocal and all non application dependend Bits of NmStatGlobal */
        NmStatLocal           = 0;
        #if defined (NM_TYPE_VAG_OSEK) 
          #if defined ( NM_ENABLE_VAG_LAH_130 )
        /* leave NmStatErrorBit as it is */
          #endif
        #endif  /* NM_TYPE_VAG_OSEK || NM_TYPE_PAG2_OSEK */
        NmStatWaitBusSleep    = 0;  /* ESCAN00009420 */
        NmStatBusSleep        = 0; 
        NmStatLimpHome        = 0;
        NmStatRingStable      = 0; 
        NmStatBusError        = 0;  /* ESCAN00009711 */
        SendMessFlag          = 0;  /* ESCAN00004259 */

        RecvNmSourceAdr = kNmEcuNr; /* initial value - self addressed */
        nmRxTxCnt += RX_CNT_STEP; /* increment receive counter */

        CanOnline(NM_CHANNEL_CANPARA_ONLY); /* unlock tx path */  /* ESCAN00004328 */

        #if defined ( NM_ENABLE_SLEEPACK_NOTIFICATION )
        PrepareSendSleepAck = 0;
        SendSleepAck        = 0;
        #endif

        #if defined ( NM_ENABLE_REMOTESLEEP_INDICATION )
        RemotePrepareSleep = 0;
        RemoteSleepInd     = 0;
        #endif

        #if defined ( NM_TYPE_VAG_OSEK ) 
          #if defined ( NM_ENABLE_VAG_LAH_130 )
        ResetErrorConfig(NM_CHANNEL_NMPARA_ONLY); /* internal function */
          #endif
        #endif  /* NM_TYPE_VAG_OSEK || NM_TYPE_PAG2_OSEK */

        if(NmStatActive == 1)
        { 
          #if defined ( NM_ENABLE_ONM_EXT_API )
          /* Use SendMessFlag to detect a valid Tx entry */
          onmStateParam |= NM_MSG_START_TX; 
          #endif

          NmStatRxTxLockUd = 1; /* ESCAN00008104 - wait for NmConfirmation() */
          SendMessFlag     = 1; /* send alive message */

          nmRxTxCnt += TX_CNT_STEP; /* increment tx counter */

          /* build Alive-Msg -------------------------------------------*/

          SendMsgEcuNr     = kNmEcuNr;
          logicalSuccessor = kNmEcuNr;
          SendMsgCmdSet    = ALIVE;
          SendSleepAck     = 0;
          SendSleepInd     = 0;

            #if defined ( NM_TYPE_VAG_OSEK ) 
              #if defined ( NM_ENABLE_VAG_LAH_130 )
          ApplNmWriteWakeUpInfo(NM_CHANNEL_APPLPARA_FIRST &(SendMsgUser0));
              #endif
            #endif  /* NM_TYPE_VAG_OSEK || NM_TYPE_PAG2_OSEK */
        }

        if((nmRxTxCnt & RX_CNT_MASK) <= RX_CNT_MAX)
        {
          TimeCnt  = (vuint8)(NM_COUNT_RING_TYP + 1);
          nmAction = NM_ACTION_WAIT_SENDTOKEN;  /* start normal mode */
        }
        else  /* Receive counter expired  --> LIMPHOME*/
        { /* set tx counter; use as Limphome tx mark */
          nmRxTxCnt = TX_CNT_STEP;

          TimeCnt   = NM_COUNT_LIMPHOME;
          nmAction  = NM_ACTION_LH_WAIT_LIMPHOME_TX;  /* start LIMPHOME */
        }

        break;

/*---------------------------------------------------------------------------*/
      case    NM_ACTION_WAIT_SENDTOKEN:
/*---------------------------------------------------------------------------*/
        #if defined ( NM_ENABLE_ONM_EXT_API )
        onmStateParam |= (NmMsgType)(NM_MSG_RING | NM_MSG_WAIT_TX); 
        #endif

        if(NmStatActive == 0)  /* NMPassive: no tx */
        {
          NmStatPrepSleep = NmStatBusSleepInd;

          #if defined ( NM_ENABLE_REMOTESLEEP_INDICATION )
          /* Remote sleep indication is not defined in passive mode */                 
          #endif


          TimeCnt  = NM_COUNT_RING_MAX;
          nmAction = NM_ACTION_WAIT_RINGMSG;
        }
        else
        {
          if(NmStatImSkippedOver == 1)
          {
            /*
              Don't touch the counter, Token should be transmitted now or by the next task call,
              if NM_COUNT_RING_TYP expires. This message might be overwritten.
            */
            #if defined ( NM_ENABLE_ONM_EXT_API )
            onmStateParam |= (NmMsgType)(NM_MSG_SKIPPED_ALIVE | NM_MSG_START_TX); 
            #endif

            /* transmit SKIPPED ALIVE message ------------------------ */
            TransmitSkippedAlive(NM_CHANNEL_NMPARA_ONLY);
          }

          if(NmStatRingMsgRecv == 1)
          {
            if(NmStatImAddressed == 1) /* Refresh Timer - state not changed */
            {
              TimeCnt = NM_COUNT_RING_TYP;
            }
            else
            {
              /* cancel transmit token */
              TimeCnt  = NM_COUNT_RING_MAX;
              nmAction = NM_ACTION_WAIT_RINGMSG;
            }
          }
          else
          {
            if(TimeCnt == 0)
            {
              /* transmit RING message ------------------------ */

              #if defined ( NM_ENABLE_ONM_EXT_API )
              /* Use SendMessFlag to detect a valid Tx entry */
              onmStateParam |= NM_MSG_START_TX; 
              #endif

              #if defined ( NM_ENABLE_RINGDATA_ACCESS )
              NmStatTxRingDataAllow = 0;  /* disable user(ring)data access */
              #endif

              NmStatRxTxLockUd      = 1; /* ESCAN00008104 - wait for NmConfirmation() */
              SendMessFlag          = 1; /* tx-ring-msg-flag */

              nmRxTxCnt += TX_CNT_STEP; /* Increment transmit counter */
                              
              if(logicalSuccessor != kNmEcuNr)
              {
                if((NmStatPrepSleep == 1) && (NmStatBusSleepInd == 1))
                {
                  #if defined ( NM_ENABLE_SLEEPACK_NOTIFICATION )
                  if(ApplNmSendSleepAck(NM_CHANNEL_APPLPARA_ONLY))
                  {
                    SendSleepAck = 1;  
                  }
                  else
                  {
                    SendSleepAck = 0;
                  }

                  PrepareSendSleepAck = 1;
                  #else
                  SendSleepAck = 1;
                  #endif
                }
                else
                {
                  SendSleepAck = 0;

                }

                SendSleepInd = NmStatBusSleepInd;
              }
              else
              {
                SendSleepAck = 0;
                SendSleepInd = 0;
              }

              if(NmStatRingPreStable == 1) /* check config  */
              {
                NmStatRingStable = 1; /* set config bit */

                #if defined ( NM_TYPE_VAG_OSEK ) 
                  #if defined ( NM_ENABLE_VAG_LAH_130 ) && defined ( NM_ENABLE_CAN_ERRORBIT )
                ApplNmRingtimeElapsed(NM_CHANNEL_APPLPARA_ONLY);
                  #endif
                #endif  /* NM_TYPE_VAG_OSEK || NM_TYPE_PAG2_OSEK */
              }
              else
              {
                NmStatRingPreStable = 1;
              }

              #if defined ( NM_ENABLE_REMOTESLEEP_INDICATION )
              if(RemotePrepareSleep == 1)
              {
                RemoteSleepInd = 1;
              }
              else
              {
                RemotePrepareSleep = 1;
              }
              #endif

              /* Build NM tx message */
              SendMsgCmdSet = RING;
              SendMsgEcuNr  = logicalSuccessor;

              #if defined ( NM_TYPE_VAG_OSEK ) 
                #if defined ( NM_ENABLE_VAG_LAH_130 )
              /* Copy received errorpin information and own detected errorpin in CAN transmit buffer */
              /* ESCAN00015469 */
              SendMsgUser0 = CanErrorConfig3;
              SendMsgUser1 = CanErrorConfig2;
              SendMsgUser2 = CanErrorConfig1;
              SendMsgUser3 = CanErrorConfig0;
                #endif
              #endif  /* NM_TYPE_VAG_OSEK || NM_TYPE_PAG2_OSEK */

              TimeCnt  = NM_COUNT_RING_MAX;
              nmAction = NM_ACTION_WAIT_RINGMSG;
            }
          }
        }
        break;

/*---------------------------------------------------------------------------*/
      case    NM_ACTION_WAIT_RINGMSG:
/*---------------------------------------------------------------------------*/
        #if defined ( NM_ENABLE_ONM_EXT_API )
        onmStateParam |= (NmMsgType)(NM_MSG_RING | NM_MSG_WAIT_RX); 
        #endif

        if(NmStatRingMsgRecv == 1)
        {
          #if defined ( NM_ENABLE_ONM_EXT_API )
          onmStateParam |= NM_MSG_RECEIVED; 
          #endif

          if((NmStatActive == 1) && (NmStatImAddressed == 1))
          {
            #if defined ( NM_ENABLE_ONM_EXT_API )
            /* 1st entry to switch the state to RING transmit -> Token received */
            onmStateParam |= NM_MSG_TOKEN_RECEIVED; 
            #endif

            #if defined ( NM_ENABLE_RINGDATA_ACCESS )
            if(NmStatRingStable == 1)
            {
              ApplNmIndRingData(NM_CHANNEL_APPLPARA_ONLY);  /* indicate ring data access valid */
            }
            #endif

            TimeCnt  = NM_COUNT_RING_TYP;
            nmAction = NM_ACTION_WAIT_SENDTOKEN;
          }
          else
          {
            /* Wait for the next RING message */
            TimeCnt = NM_COUNT_RING_MAX;
          }
        }

        if(NmStatActive == 1)
        {
          if(NmStatImSkippedOver == 1)
          {
            /*
              Don't touch the counter, Token should be transmitted by the next task call,
              if NM_COUNT_RING_TYP expires.
              If there is no Token present for the node, NM_COUNT_RING_MAX expires.
            */

            #if defined ( NM_ENABLE_ONM_EXT_API )
            onmStateParam |= (NmMsgType)(NM_MSG_SKIPPED_ALIVE | NM_MSG_WAIT_TX | NM_MSG_START_TX); 
            #endif

            /* transmit SKIPPED ALIVE message ------------------------ */
            TransmitSkippedAlive(NM_CHANNEL_NMPARA_ONLY);
          }
        }
        else
        {
          /* set Ring Stable on nmOff */

          if(NmStatRingPreStable == 1)  /* check config */
          {
            NmStatRingStable = 1;       /* set config bit */
          }
          else
          {
            NmStatRingPreStable = 1;
          }

          NmStatPrepSleep = NmStatBusSleepInd;

          #if defined ( NM_ENABLE_REMOTESLEEP_INDICATION )
          /* Remote sleep indication is not defined in passive mode */                 
          #endif

        }

        if(TimeCnt == 0)
        { 
          /* receive timeout NM_COUNT_RING_MAX, goto start state */

          #if defined ( NM_ENABLE_ONM_EXT_API )
          onmStateParam |= NM_MSG_TIMEOUT_RX; 
          #endif

          nmAction = NM_ACTION_START;
        }
        break;
/*---------------------------------------------------------------------------*/
      case    NM_ACTION_LH_WAIT_LIMPHOME_TX:
      case    NM_ACTION_LH_PREPARE_SLEEP:
/*---------------------------------------------------------------------------*/
        ResetConfig(NM_CHANNEL_NMPARA_ONLY); /* internal function */
        NmStatLimpHome = 1;

        nmRxTxCnt &= (vuint8)~RX_CNT_MASK;  /* reset receive counter */

        #if defined ( NM_ENABLE_REMOTESLEEP_INDICATION )
        /* If GW is in limphome mode, other networks should */
        /* not be hindered to enter sleep mode */
        RemotePrepareSleep = 1;
        RemoteSleepInd     = 1;
        #endif

        if(NmStatNmMsgRecv == 1)
        {
          NmStatNmMsgRecvRemind = 1;
        }

        if((NmStatNmMsgRecvRemind == 1) && ((NmStatActive == 0) || (nmRxTxCnt == 0)))
        {
          nmRxTxCnt = 0;
          /* time critical case: work immediately on new state */
          nmAction = (NmActionType)(NM_ACTION_START | NM_ACTION_REPEAT);
        }
        else
        {
          if(nmAction == NM_ACTION_LH_WAIT_LIMPHOME_TX)
          {
            #if defined ( NM_ENABLE_ONM_EXT_API )
            onmStateParam |= (NmMsgType)(NM_MSG_LIMPHOME | NM_MSG_WAIT_TX); 
            #endif
/*---------------------------------------------------------------------------*/
/*                      NM_ACTION_LH_WAIT_LIMPHOME_TX only                   */
/*---------------------------------------------------------------------------*/
            if(TimeCnt == 0)
            {
              if(NmStatBusError == 1)
              {
                NmStatBusError = 0;

                #if defined ( C_MULTIPLE_RECEIVE_CHANNEL )
                /* makros must be encapsulated in this way */
                CanResetBusOffEnd(NM_CHANNEL_CANPARA_ONLY, kNmCanPara);
                #else
                CanResetBusOffEnd(kNmCanPara);
                #endif

                #if defined ( NM_ENABLE_EXTENDED_CALLBACK )
                ApplNmBusOffEnd(NM_CHANNEL_APPLPARA_ONLY);
                #endif
              }

              CanOnline(NM_CHANNEL_CANPARA_ONLY);

              if(NmStatActive == 1)
              {
                /* Send LIMPHOME message */

                #if defined ( NM_ENABLE_ONM_EXT_API )
                onmStateParam |= NM_MSG_START_TX; 
                #endif

                NmStatPrepSleep  = NmStatBusSleepInd;
                NmStatRxTxLockUd = 1; /* ESCAN00008104 - wait for NmConfirmation() */
                SendMessFlag     = 1; /* transmit LimpHome msg */

                nmRxTxCnt = TX_CNT_STEP;  /* set tx counter; use as Limphome tx mark */

                #if defined ( NM_ENABLE_REMOTESLEEP_INDICATION )
                /* Remote sleep indication depends only on received sleep indication flags */                 
                #endif


                /* Build LIMPHOME message */
                SendMsgCmdSet = LIMPHOME;
                SendMsgEcuNr  = logicalSuccessor;
                SendSleepAck  = 0; /* ESCAN00007382 */
                SendSleepInd  = NmStatBusSleepInd;

                #if defined ( NM_TYPE_VAG_OSEK ) 
                  #if defined ( NM_ENABLE_VAG_LAH_130 )
                SendMsgUser0 = DATA_INIT;
                SendMsgUser1 = DATA_INIT;
                SendMsgUser2 = DATA_INIT;
                SendMsgUser3 = DATA_INIT;
                  #endif
                #endif  /* NM_TYPE_VAG_OSEK || NM_TYPE_PAG2_OSEK */
              }

              TimeCnt = NM_COUNT_LIMPHOME;
            }

            if(NmStatActive == 0)
            {
              NmStatPrepSleep = NmStatBusSleepInd;

              #if defined ( NM_ENABLE_REMOTESLEEP_INDICATION )
              /* Remote sleep indication is not defined in passive mode */                 
              #endif

            }

            if(NmStatPrepSleep == 1)
            {

              TimeCnt = NM_COUNT_RING_MAX;  /* start Timeout-Timer */
              /* time critical case: work immediately on new state  */
              nmAction = (NmActionType)(NM_ACTION_LH_PREPARE_SLEEP | NM_ACTION_REPEAT);
            }
          }
/*---------------------------------------------------------------------------*/
/*                      NM_ACTION_LH_PREPARE_SLEEP                           */
/*---------------------------------------------------------------------------*/
          else
          {
            if(NmStatPrepSleep == 1)
            {

              if(NmStatRingMsgRecv == 1)
              {
                TimeCnt = NM_COUNT_RING_MAX;  /* reset timer */
              }
              else
              {
                if(TimeCnt == 0)  /* start BusSleep */
                {
                  CanOffline(NM_CHANNEL_CANPARA_ONLY);

                  NmStatWaitBusSleep = 1;
                  NmStatRxTxLockUd   = 0; /* ESCAN00008104 */
                  SendMessFlag       = 0; /* cancel nm tx request */


                  if(NmStatBusError == 1)
                  {
                    NmStatBusError = 0;

                    #if defined ( C_MULTIPLE_RECEIVE_CHANNEL )
                    /* makros must be encapsulated in this way */
                    CanResetBusOffEnd(NM_CHANNEL_CANPARA_ONLY, kNmCanPara);
                    #else
                    CanResetBusOffEnd(kNmCanPara);
                    #endif

                    #if defined (NM_ENABLE_EXTENDED_CALLBACK)
                    ApplNmBusOffEnd(NM_CHANNEL_APPLPARA_ONLY);
                    #endif
                  }

                  TimeCnt  = NM_COUNT_WAIT_BUSSLEEP;
                  nmAction = NM_ACTION_GO_BUSSLEEP;
                }
              }
            }
            else
            {
              /* Application-WakeUp */
              NmStatNmMsgRecvRemind = 0;

              TimeCnt  = NM_COUNT_LIMPHOME;  /* stop PREPARE_SLEEP */
              nmAction = NM_ACTION_LH_WAIT_LIMPHOME_TX;
            }
          }

/*---------------------------------------------------------------------------*/
        }
        break;
/*---------------------------------------------------------------------------*/
      case    NM_ACTION_GO_BUSSLEEP:
/*---------------------------------------------------------------------------*/
        ResetConfig(NM_CHANNEL_NMPARA_ONLY); /* internal function */
        NmStatNmMsgRecvRemind = 0;

        nmRxTxCnt = 0;

        /*
          Check counter for entry of BusSleep state here. This will cause a
          latency of up to one call cycle. Do not compare with NM_COUNT_WAIT_BUSSLEEP
          due to the TimeCnt decrement at top of this task with a free interrupt
          between the decrement and the check!
        */

        #if defined ( NM_ENABLE_EXTENDED_CALLBACK )
        if(TimeCnt == ((vuint8)(NM_COUNT_WAIT_BUSSLEEP - 1)))
        {
          /*
            Just call the callback here on task level.
            Do it in each case for a symmetric call of start and cancel callback.
          */
          ApplNmWaitBusSleep(NM_CHANNEL_APPLPARA_ONLY);
        }
        #endif

        if(NmStatPrepSleep == 0)   /* BusWakeUp called or NM msg without SleepInd received -> stop PrepareSleep */
        {
          NmStatWaitBusSleep = 0;  /* Leave the context, because of possible WakeUp detection in GotoMode() */

          CanOnline(NM_CHANNEL_CANPARA_ONLY); /* ESCAN00008463 - unlock CAN-Transmit path immediately */


          if(NmStatLimpHome == 1)
          {
            nmRxTxCnt = TX_CNT_STEP;  /* set tx counter, use as LIMPHOME */

            TimeCnt   = NM_COUNT_LIMPHOME;

            nmAction  = NM_ACTION_LH_WAIT_LIMPHOME_TX;  /* start state LIMPHOME */
          }
          else
          {
            nmAction  = (NmActionType)(NM_ACTION_START | NM_ACTION_REPEAT);
          }

          #if defined ( NM_ENABLE_EXTENDED_CALLBACK )
          /*
            WaitBusSleep leaved for LIMPHOME and for normal state.
            For LIMPHOME CAN is offline till next call of the NM task.
          */
          ApplNmWaitBusSleepCancel(NM_CHANNEL_APPLPARA_ONLY);
          #endif
        }
        else
        {
        	/*if(TimeCnt <= 5)
        	{
        		volatile Uint8Type i;
        	    if (TRUE == GIO_u8PinRead(PORT_NOTUSED_PIN7_GIOA, GIOA))
        	    {
        	        GIO_vPinWrite(PORT_NOTUSED_PIN7_GIOA, LOW, GIOA);
        	    }
        	    else
        	    {
        	        GIO_vPinWrite(PORT_NOTUSED_PIN7_GIOA, HIGH, GIOA);
        	    }
        	}*/
        	if(TimeCnt == 1)
          {
            CanOffline(NM_CHANNEL_CANPARA_ONLY);

            NmStatRxTxLockUd = 0; /* ESCAN00008104 */
            SendMessFlag     = 0; /* cancel nm tx request */

            /* init CAN-Chip; clear Tx requets */
            #if defined ( C_MULTIPLE_RECEIVE_CHANNEL )
            /* makros must be encapsulated in this way */
            CanResetBusSleep(NM_CHANNEL_CANPARA_ONLY, kNmCanPara);
            #else
            CanResetBusSleep(kNmCanPara);
            #endif
          }
          else
          {
            if(TimeCnt == 0)
            {
              NmStatWaitBusSleep = 0;
              NmStatBusSleep     = 1;

              ApplNmCanSleep(NM_CHANNEL_APPLPARA_ONLY);    /* switch off CAN path */
              ApplNmCanBusSleep(NM_CHANNEL_APPLPARA_ONLY); /* indicate bus sleep */

              nmAction = NM_ACTION_BUS_SLEEP;
            }
          }
        }
        break;
/*---------------------------------------------------------------------------*/
      case    NM_ACTION_BUS_SLEEP:
/*---------------------------------------------------------------------------*/
        NmStatLimpHome = 0;

	   /* if (TRUE == GIO_u8PinRead(PORT_NOTUSED_PIN7_GIOA, GIOA))
	    {
	        GIO_vPinWrite(PORT_NOTUSED_PIN7_GIOA, LOW, GIOA);
	    }
	    else
	    {
	        GIO_vPinWrite(PORT_NOTUSED_PIN7_GIOA, HIGH, GIOA);
	    }*/
        /* Application called wakeup or any NM message received */
        if(NmStatPrepSleep == 0) /* ESCAN00007436 */
        {
          NmStatBusSleep = 0;


          ApplNmCanNormal(NM_CHANNEL_APPLPARA_ONLY); /* wakeup CAN-chip and transceiver */
          CanOnline(NM_CHANNEL_CANPARA_ONLY); /* unlock tx path */
          #if defined ( NM_ENABLE_EXTENDED_CALLBACK )
          ApplNmBusStart(NM_CHANNEL_APPLPARA_ONLY);
          #endif

          nmAction = (NmActionType)(NM_ACTION_START | NM_ACTION_REPEAT);
        }
        break;
/*---------------------------------------------------------------------------*/
      case    NO_ACTION:
      case    NM_ACTION_REPEAT:
      default :
/*---------------------------------------------------------------------------*/
        ResetConfig( NM_CHANNEL_NMPARA_ONLY ); /* internal function */
        NmStatGlobal    = 0;
        NmStatLocal     = 0;
        SendMessFlag    = 0;  /* ESCAN00004259 */

        TimeCnt         = 0;
        nmRxTxCnt       = 0;  /* Reset watch counter */
        RecvNmSourceAdr = kNmEcuNr;  /* initial value - self addressed */

        #if defined ( NM_ENABLE_SLEEPACK_NOTIFICATION )
        PrepareSendSleepAck = 0;
        SendSleepAck        = 0;
        #endif

        #if defined ( NM_ENABLE_REMOTESLEEP_INDICATION )
        RemotePrepareSleep = 0;
        RemoteSleepInd     = 0;
        #endif
        break;
    }/* switch(nmAction) */

    #if defined ( NM_TYPE_VAG_OSEK )
      #if defined ( NM_ENABLE_VAG_LAH_141 ) && defined ( NM_ENABLE_CAR_WUP_REQUEST )
    if(nmCarWupState == CarWupOn1stReq) /* ESCAN00010855 */
    {
      nmCarWupState    = CarWupOn;  /* set async request to sync request */
      nmCarWupSendMess = 1;         /* push async request */
    }
      #endif
    #endif  /* NM_TYPE_VAG_OSEK */

    #if defined ( NM_ENABLE_ONM_EXT_API )
    OnmExtTask(NM_CHANNEL_NMPARA_FIRST onmStateParam); 
    #endif    

    ResetMessRecvFlags();

/*---- Restore old Interrupt Mask -------------------------------------------*/
    //GIO_vPinWrite(PORT_NOTUSED_PIN7_GIOA, HIGH, GIOA);
    ApplCanAddCanInterruptRestore(0);

/*---------------------------------------------------------------------------*/
  }while( (nmAction & NM_ACTION_REPEAT) != 0 ); /*repeat immediately on new state*/
/*---------------------------------------------------------------------------*/

  /*---- Send if there is s.th to send -------------------------------------*/
  #if defined ( NM_TYPE_VAG_OSEK )
    #if defined ( NM_ENABLE_VAG_LAH_141 ) && defined ( NM_ENABLE_CAR_WUP_REQUEST )
  if(((SendMessFlag == 1) || (nmCarWupSendMess == 1)) && (NmStatActive == 1)) /* ESCAN00010855 */
    #else
  if((SendMessFlag == 1) && (NmStatActive == 1))
    #endif
  #endif  /* NM_TYPE_VAG_OSEK */
  {
    if(CanTransmit(kNmTxObj) == kCanTxOk)
    {
      SendMessFlag = 0; /* tx accepted, wait for NmConfirmation() */
      #if defined ( NM_TYPE_VAG_OSEK )
        #if defined ( NM_ENABLE_VAG_LAH_141 ) && defined ( NM_ENABLE_CAR_WUP_REQUEST )
      nmCarWupSendMess = 0; /* ESCAN00010855 */
        #endif
      #endif  /* NM_TYPE_VAG_OSEK */
    }
  }

  StopMeasure(NmTaskTimings);

} /* end of NmTask() */


/************************************************************************
| NAME:               GotoMode
| PROTOTYPE:          void GotoMode( NMModeName mode )
| CALLED BY:          Station Manager, Application
| PRECONDITIONS:      -
| INPUT PARAMETERS:   BusSleep: SM / Application is ready to sleep
|                     Awake:    CAN bus is required by SM / Application
| RETURN VALUE:       -
| DESCRIPTION:        Set/clear status bit 'BusSleepInd' and
|                     clear 'PrepareSleep'
| GLOBAL DATA:
| STACK:
| RUNTIME:
|*************************************************************************/
#if defined ( C_COMP_KEIL_C5X5C )
  #pragma NOAREGS
#endif
void NM_API_CALL_TYPE GotoMode(NM_CHANNEL_NMTYPE_FIRST NMModeName mode)
{
  #if defined ( NM_ENABLE_INDEXED_NM ) && defined ( NM_ENABLE_CHANNEL_INDIRECTION )
  channel = NmCanToNmIndirection[channel];
  #endif

  ApplCanAddCanInterruptDisable(0);

  if(mode == BusSleep)
  {
    NmStatBusSleepInd = 1;
  }
  else /* mode == Awake */
  {

    #if defined ( NM_ENABLE_SLEEPACK_NOTIFICATION )
    PrepareSendSleepAck = 0;
    SendSleepAck        = 0;
    #endif

    #if defined ( NM_ENABLE_REMOTESLEEP_INDICATION )
    /* Change of local bus sleep indication state */
    /* does not effect remote sleep */
    #endif

    NmStatBusSleepInd = 0;
    NmStatPrepSleep   = 0;
  }

  ApplCanAddCanInterruptRestore(0);
} /* end of GotoMode() */


/************************************************************************
| NAME:               GetStatus
| PROTOTYPE:          void GetStatus (NmStatusType* dest)
| CALLED BY:          Application
| PRECONDITIONS:      -
| INPUT PARAMETERS:   Pointer to application specific variable
| RETURN VALUE:       -
| DESCRIPTION:        Copies the current NM state into an application
|                     specific variable
| GLOBAL DATA:        -
| STACK:              -
| RUNTIME:            -
|*************************************************************************/
#if defined ( C_COMP_KEIL_C5X5C )
  #pragma NOAREGS
#endif
void NM_API_CALL_TYPE GetStatus(NM_CHANNEL_NMTYPE_FIRST NmStatusType *dest)
{
  #if defined ( NM_ENABLE_INDEXED_NM ) && defined ( NM_ENABLE_CHANNEL_INDIRECTION )
  channel = NmCanToNmIndirection[channel];
  #endif

  *dest = (NmStatusType)(NmStatGlobal);
} /* end of GetStatus() */


/************************************************************************
| NAME:               NmGetStatus
| PROTOTYPE:          NmStatusType NmGetStatus(void)
| CALLED BY:          Application
| PRECONDITIONS:      -
| INPUT PARAMETERS:   -
| RETURN VALUE:       Current NM status
| DESCRIPTION:        -
| GLOBAL DATA:        -
| STACK:              -
| RUNTIME:            -
|*************************************************************************/
#if defined ( C_COMP_KEIL_C5X5C )
  #pragma NOAREGS
#endif
NmStatusType NM_API_CALL_TYPE NmGetStatus(NM_CHANNEL_NMTYPE_ONLY)
{
  #if defined ( NM_ENABLE_INDEXED_NM ) && defined ( NM_ENABLE_CHANNEL_INDIRECTION )
  channel = NmCanToNmIndirection[channel];
  #endif

  return((NmStatusType)(NmStatGlobal));
} /* end of NmGetStatus()*/


/************************************************************************
| NAME:               StartNM
| PROTOTYPE:          void StartNM(void)
| CALLED BY:          Application
| PRECONDITIONS:      -
| INPUT PARAMETERS:   -
| RETURN VALUE:       -
| DESCRIPTION:        Starts the local network management
| GLOBAL DATA:
| STACK:
| RUNTIME:
|*************************************************************************/
#if defined ( C_COMP_KEIL_C5X5C )
  #pragma NOAREGS
#endif
void NM_API_CALL_TYPE StartNM ( NM_CHANNEL_NMTYPE_ONLY )
{
  #if defined ( NM_ENABLE_INDEXED_NM ) && defined ( NM_ENABLE_CHANNEL_INDIRECTION )
  /*
    !!! Not used, because direct usage of CAN channel for NmOsekInit() call
    channel = NmCanToNmIndirection[channel];
  */
  #endif
  /* only here routing the physical channels directly */
  NmOsekInit(NM_CHANNEL_NMPARA_FIRST NM_NORMAL);
} /* end of StartNM() */


/************************************************************************
| NAME:               StopNM
| PROTOTYPE:          void StopNM(void)
| CALLED BY:          Application
| PRECONDITIONS:      -
| INPUT PARAMETERS:   -
| RETURN VALUE:       -
| DESCRIPTION:        Stops the local network management
| GLOBAL DATA:
| STACK:
| RUNTIME:
|*************************************************************************/
#if defined ( C_COMP_KEIL_C5X5C )
  #pragma NOAREGS
#endif
void NM_API_CALL_TYPE StopNM(NM_CHANNEL_NMTYPE_ONLY)
{
  #if defined ( NM_ENABLE_INDEXED_NM ) && defined ( NM_ENABLE_CHANNEL_INDIRECTION )
  /*
    !!! Not used, because direct usage of CAN channel for NmOsekInit() call
    channel = NmCanToNmIndirection[channel];
  */
  #endif
  /* only here routing the physical channels directly */
  NmOsekInit(NM_CHANNEL_NMPARA_FIRST NM_DISABLE);
} /* end of StopNM() */


/************************************************************************
| NAME:               TalkNM
| PROTOTYPE:          void TalkNM(void)
| CALLED BY:          Application
| PRECONDITIONS:      -
| INPUT PARAMETERS:   -
| RETURN VALUE:       -
| DESCRIPTION:        Switching on of net management
| GLOBAL DATA:
| STACK:
| RUNTIME:
|*************************************************************************/
#if defined ( C_COMP_KEIL_C5X5C )
  #pragma NOAREGS
#endif
void NM_API_CALL_TYPE TalkNM(NM_CHANNEL_NMTYPE_ONLY)
{
  #if defined ( NM_ENABLE_INDEXED_NM ) && defined ( NM_ENABLE_CHANNEL_INDIRECTION )
  channel = NmCanToNmIndirection[channel];
  #endif

  ApplCanAddCanInterruptDisable(0);
  NmStatActive = 1;
  ApplCanAddCanInterruptRestore(0);
} /* end of TalkNM() */


/************************************************************************
| NAME:               SilentNM
| PROTOTYPE:          void SilentNM(void)
| CALLED BY:          Application
| PRECONDITIONS:      -
| INPUT PARAMETERS:   -
| RETURN VALUE:       -
| DESCRIPTION:        Switching off of net management
| GLOBAL DATA:
| STACK:
| RUNTIME:
|*************************************************************************/
#if defined ( C_COMP_KEIL_C5X5C )
  #pragma NOAREGS
#endif
void NM_API_CALL_TYPE SilentNM(NM_CHANNEL_NMTYPE_ONLY)
{
  #if defined ( NM_ENABLE_INDEXED_NM ) && defined ( NM_ENABLE_CHANNEL_INDIRECTION )
  channel = NmCanToNmIndirection[channel];
  #endif

  ApplCanAddCanInterruptDisable(0);
  NmStatActive = 0;
  ApplCanAddCanInterruptRestore(0);
} /* end of SilentNM() */


#if defined ( NM_ENABLE_RINGDATA_ACCESS )
/************************************************************************
| NAME:               ReadRingData
| PROTOTYPE:          StatusType ReadRingData(vuint8* ringData)
| CALLED BY:          Application
| PRECONDITIONS:      -
| INPUT PARAMETERS:   DataPtr: Pointer to a target field, there the
|                              current user data are copied to
| RETURN VALUE:       -
| DESCRIPTION:        copies the current user data into an
|                     application specific buffer
| GLOBAL DATA:
| STACK:
| RUNTIME:
|*************************************************************************/
#if defined ( C_COMP_KEIL_C5X5C )
  #pragma NOAREGS
#endif
StatusType NM_API_CALL_TYPE ReadRingData(NM_CHANNEL_NMTYPE_FIRST vuint8* ringData)
{
  #if defined ( NM_ENABLE_INDEXED_NM ) && defined ( NM_ENABLE_CHANNEL_INDIRECTION )
  channel = NmCanToNmIndirection[channel];
  #endif

  if(NmStatRingStable == 0)
  {
    return(E_NotOK);
  }
  else
  {
    #if defined ( NM_ENABLE_COPY_RINGDATA )
    ApplCanAddCanInterruptDisable(0);
    /* normal OSEK mechanism - received ring data are in send buffer */
    *(ringData    ) = SendMsgUser0;
    *(ringData + 1) = SendMsgUser1;
    *(ringData + 2) = SendMsgUser2;
    *(ringData + 3) = SendMsgUser3;
    *(ringData + 4) = SendMsgUser4;
    *(ringData + 5) = SendMsgUser5;
    ApplCanAddCanInterruptRestore(0);

    return(E_OK);
    #else
      #if defined ( NM_ENABLE_COPY_RINGDATA_TO_TEMPBUFFER )
    ApplCanAddCanInterruptDisable(0);
    /* Read the received ring data, which have been copied to global buffer */
    *(ringData    ) = RecvCopyMsgUser0;
    *(ringData + 1) = RecvCopyMsgUser1;
    *(ringData + 2) = RecvCopyMsgUser2;
    *(ringData + 3) = RecvCopyMsgUser3;
    *(ringData + 4) = RecvCopyMsgUser4;
    *(ringData + 5) = RecvCopyMsgUser5;
    ApplCanAddCanInterruptRestore(0);

    return(E_OK);
      #else
        #if defined ( V_ENABLE_USE_DUMMY_STATEMENT )
    *ringData = *ringData;
        #endif
    /* The received data are not copied - there is nothing to read !*/
    return(E_NotOK);
      #endif
    #endif 
  }
} /* end of ReadRingData()*/


/************************************************************************
| NAME:               TransmitRingData
| PROTOTYPE:          StatusType TransmitRingData(vuint8* ringData)
| CALLED BY:          Application
| PRECONDITIONS:      -
| INPUT PARAMETERS:   DataPtr: Pointer to a source field, there the
|                              current user data are copied from
| RETURN VALUE:       -
| DESCRIPTION:        copies the current user data from application
|                     specific buffer into user data of nm message
| GLOBAL DATA:
| STACK:
| RUNTIME:
|*************************************************************************/
#if defined ( C_COMP_KEIL_C5X5C )
  #pragma NOAREGS
#endif
StatusType NM_API_CALL_TYPE TransmitRingData( NM_CHANNEL_NMTYPE_FIRST vuint8* ringData )
{
  #if defined ( NM_ENABLE_INDEXED_NM ) && defined ( NM_ENABLE_CHANNEL_INDIRECTION )
  channel = NmCanToNmIndirection[channel];
  #endif

  if(NmStatTxRingDataAllow == 0) /* user(ring)data access disabled? */
  {
    return(E_NotOK);
  }
  else
  {
    ApplCanAddCanInterruptDisable(0);
    SendMsgUser0  = *(ringData    );
    SendMsgUser1  = *(ringData + 1);
    SendMsgUser2  = *(ringData + 2);
    SendMsgUser3  = *(ringData + 3);
    SendMsgUser4  = *(ringData + 4);
    SendMsgUser5  = *(ringData + 5);
    ApplCanAddCanInterruptRestore(0);

    return(E_OK);
  }
} /* end of TransmitRingData()*/
#endif /* NM_ENABLE_RINGDATA_ACCESS ) */


#if defined ( NM_ENABLE_CONFIG )
/************************************************************************
| NAME:               CmpConfig
| PROTOTYPE:          NmReturnType CmpConfig(vuint8 idx)
| CALLED BY:          Application
| PRECONDITIONS:      -
| INPUT PARAMETERS:   idx: Index in ConfigMaskTable and TargetConfigTable
| RETURN VALUE:       1: Current configuration is matching
|                     0: Current Configuration is wrong
| DESCRIPTION:        compares current configuration ( registered CAN nodes )
|                     with a expected config stored in memory
| GLOBAL DATA:
| STACK:
| RUNTIME:
|*************************************************************************/
#if defined ( C_COMP_KEIL_C5X5C )
  #pragma NOAREGS
#endif
NmReturnType CmpConfig(NM_CHANNEL_NMTYPE_FIRST vuint8 idx)
{
  vuint8 configByte;

  #if defined ( NM_ENABLE_INDEXED_NM ) && defined ( NM_ENABLE_CHANNEL_INDIRECTION )
  channel = NmCanToNmIndirection[channel];
  #endif

  ApplCanAddCanInterruptDisable(0);

  #if defined ( NM_ENABLE_INDEXED_NM )
  for(configByte =   channel      * (vuint8)((NM_NUMBER_OF_CONFIG_NODES)/8);
      configByte < ((channel + 1) * (vuint8)((NM_NUMBER_OF_CONFIG_NODES)/8));
      configByte++)
  #else
  for(configByte = 0; configByte <  (vuint8)((NM_NUMBER_OF_CONFIG_NODES)/8); configByte++)
  #endif
  {
    if((TargetConfigTable[idx].nmConfigChar[configByte] ^ nmConfig.nmConfigChar[configByte])
        & ConfigMaskTable[idx].nmConfigChar[configByte])
    {
      ApplCanAddCanInterruptRestore(0);
      return(0);
    }
  }

  ApplCanAddCanInterruptRestore(0);
  return(1);
} /* end of CmpConfig() */


/************************************************************************
| NAME:               GetConfig
| PROTOTYPE:          void GetConfig(NmConfigType *DataPtr)
| CALLED BY:          Application
| PRECONDITIONS:      -
| INPUT PARAMETERS:   DataPtr: Pointer to a target field, there the
|                              current configuration is copied to
| RETURN VALUE:       -
| DESCRIPTION:        copies the current configuration ( registered CAN nodes )
|                     into an application specific buffer
| GLOBAL DATA:
| STACK:
| RUNTIME:
|*************************************************************************/
#if defined ( C_COMP_KEIL_C5X5C )
  #pragma NOAREGS
#endif
void NM_API_CALL_TYPE GetConfig(NM_CHANNEL_NMTYPE_FIRST NmConfigType *dataPtr)
{
  vuint8 configByte;

  #if defined ( NM_ENABLE_INDEXED_NM ) && defined ( NM_ENABLE_CHANNEL_INDIRECTION )
  channel = NmCanToNmIndirection[channel];
  #endif

  ApplCanAddCanInterruptDisable(0);

  #if defined ( NM_ENABLE_INDEXED_NM )
  for(configByte =   channel      * (vuint8)((NM_NUMBER_OF_CONFIG_NODES)/8);
      configByte < ((channel + 1) * (vuint8)((NM_NUMBER_OF_CONFIG_NODES)/8));
      configByte++)
  #else
  for(configByte = 0; configByte <  (vuint8)((NM_NUMBER_OF_CONFIG_NODES)/8); configByte++)
  #endif
  {
    dataPtr->nmConfigChar[configByte] = nmConfig.nmConfigChar[configByte];
  }

  ApplCanAddCanInterruptRestore(0);
} /* end of GetConfig() */
#endif /* NM_ENABLE_CONFIG */


#if defined ( NM_ENABLE_SLEEPACK_NOTIFICATION )
/************************************************************************
| NAME:               NmGetPrepareSendSleepAck
| PROTOTYPE:          vuint8 NmGetPrepareSendSleepAck(void)
| CALLED BY:          Application
| PRECONDITIONS:      -
| INPUT PARAMETERS:   -
| RETURN VALUE:       Actual prepare sleep ack state
| DESCRIPTION:        Returns actual prepare sleep ack state of net management
| GLOBAL DATA:
| STACK:
| RUNTIME:
|*************************************************************************/
#if defined ( C_COMP_KEIL_C5X5C )
  #pragma NOAREGS
#endif
vuint8 NM_API_CALL_TYPE NmGetPrepareSendSleepAck(NM_CHANNEL_NMTYPE_ONLY)
{
  #if defined ( NM_ENABLE_INDEXED_NM ) && defined ( NM_ENABLE_CHANNEL_INDIRECTION )
  channel = NmCanToNmIndirection[channel];
  #endif

  if(PrepareSendSleepAck == 1)
  {
    return(1);
  }
  else
  {
    return(0);
  }
} /* end of NmGetPrepareSendSleepAck()*/


/************************************************************************
| NAME:               NmGetSendSleepAck
| PROTOTYPE:          vuint8 NmGetSendSleepAck(void)
| CALLED BY:          Application
| PRECONDITIONS:      Prepare Send Sleep Ack has to be set
| INPUT PARAMETERS:   -
| RETURN VALUE:       Actual state of sleep ack flag in send buffer
| DESCRIPTION:        Returns actual state of sleep ack flag in send buffer of net management
| GLOBAL DATA:
| STACK:
| RUNTIME:
|*************************************************************************/
#if defined ( C_COMP_KEIL_C5X5C )
  #pragma NOAREGS
#endif
vuint8 NM_API_CALL_TYPE NmGetSendSleepAck(NM_CHANNEL_NMTYPE_ONLY)
{
  #if defined ( NM_ENABLE_INDEXED_NM ) && defined ( NM_ENABLE_CHANNEL_INDIRECTION )
  channel = NmCanToNmIndirection[channel];
  #endif

  if(SendSleepAck == 1)
  {
    return(1);
  }
  else
  {
    return(0);
  }
} /* end of NmGetSendSleepAck()*/
#endif /* defined ( NM_ENABLE_SLEEPACK_NOTIFICATION ) */


#if defined ( NM_ENABLE_REMOTESLEEP_INDICATION )
/************************************************************************
| NAME:               NmGetRemoteSleepInd
| PROTOTYPE:          vuint8 NmGetRemoteSleepInd(void)
| CALLED BY:          Application
| PRECONDITIONS:      -
| INPUT PARAMETERS:   -
| RETURN VALUE:       Actual remote sleep indication state
| DESCRIPTION:        Returns actual remote sleep indication state of net management
| GLOBAL DATA:
| STACK:
| RUNTIME:
|*************************************************************************/
#if defined ( C_COMP_KEIL_C5X5C )
  #pragma NOAREGS
#endif
vuint8 NM_API_CALL_TYPE NmGetRemoteSleepInd(NM_CHANNEL_NMTYPE_ONLY)
{
  #if defined ( NM_ENABLE_INDEXED_NM ) && defined ( NM_ENABLE_CHANNEL_INDIRECTION )
  channel = NmCanToNmIndirection[channel];
  #endif

  if(RemoteSleepInd == 1)
  {
    return(1);
  }
  else
  {
    return(0);
  }
} /* end of NmGetRemoteSleepInd()*/
#endif /* NM_ENABLE_REMOTESLEEP_INDICATION */


#if defined ( NM_ENABLE_ONM_EXT_API )
/************************************************************************
| NAME:               NmGetUdStartAddress
| PROTOTYPE:          vuint8* NmGetUdStartAddress(void)
| CALLED BY:          OnmExt functions
| PRECONDITIONS:      -
| INPUT PARAMETERS:   -
| RETURN VALUE:       Pointer to first User Data byte
| DESCRIPTION:        The pointer is valid after initialization for the
|                     whole running time.
| GLOBAL DATA:        -
| STACK:              -
| RUNTIME:            -
|*************************************************************************/
#if defined ( C_COMP_KEIL_C5X5C )
  #pragma NOAREGS
#endif
vuint8* NM_API_CALL_TYPE NmGetUdStartAddress(NM_CHANNEL_NMTYPE_ONLY)
{
  #if defined ( NM_ENABLE_INDEXED_NM ) && defined ( NM_ENABLE_CHANNEL_INDIRECTION )
  /*
    !!! Not used, because only for module internal usage on NM cannel allowed
    channel = NmCanToNmIndirection[channel];
  */
  #endif

  return(&(SendMsgUser0));
} /* end of NmGetUdStartAddress() */


/************************************************************************
| NAME:               NmReadRecvSourceAddress
| PROTOTYPE:          StatusType NmReadRecvSourceAddress(NmNodeType* source)
| CALLED BY:          OnmExt functions
| PRECONDITIONS:      Interrupts disabled
| INPUT PARAMETERS:   source address by reference
| RETURN VALUE:       E_NotOK, E_OK, current received source address by reference
| DESCRIPTION:        -
| GLOBAL DATA:        -
| STACK:              -
| RUNTIME:            -
|*************************************************************************/
#if defined ( C_COMP_KEIL_C5X5C )
  #pragma NOAREGS
#endif
StatusType NM_API_CALL_TYPE NmReadRecvSourceAddress(NM_CHANNEL_NMTYPE_FIRST NmNodeType* source)
{
  StatusType rval;

  #if defined ( NM_ENABLE_INDEXED_NM ) && defined ( NM_ENABLE_CHANNEL_INDIRECTION )
  /*
    !!! Not used, because only for module internal usage on NM cannel allowed
    channel = NmCanToNmIndirection[channel];
  */
  #endif

  if((NmStatBusError     == 1) ||
     (NmStatWaitBusSleep == 1) ||
     (NmStatBusSleep     == 1))
  {
    /* Source address undefined */
    rval = E_NotOK;
  }
  else
  {
    /* Interrupts already blocked, because usage only allowed in ONMX */
    *source = (NmNodeType)RecvNmSourceAdr;
    /* Interrupts already blocked, because usage only allowed in ONMX */

    rval = E_OK;
  }
  return ( rval );
} /* end of NmReadRecvSourceAddress() */
#endif  /* NM_ENABLE_ONM_EXT_API */






#if defined ( NM_TYPE_VAG_OSEK ) 
  #if defined ( NM_ENABLE_VAG_LAH_130 )
/************************************************************************
| NAME:               NmGetCanErrorConfig
| PROTOTYPE:          void NmGetCanErrorConfig(NmConfigType *DataPtr)
| CALLED BY:          Application
| PRECONDITIONS:      -
| INPUT PARAMETERS:   DataPtr: Pointer to a target field, there the
|                              current error configuration is copied to
| RETURN VALUE:       -
| DESCRIPTION:        copies the current error configuration into an
|                     application specific buffer
| GLOBAL DATA:
| STACK:
| RUNTIME:
|*************************************************************************/
#if defined ( C_COMP_KEIL_C5X5C )
  #pragma NOAREGS
#endif
void NM_API_CALL_TYPE NmGetCanErrorConfig(NM_CHANNEL_NMTYPE_FIRST NmConfigType *dataPtr)
{
  #if defined ( NM_ENABLE_INDEXED_NM ) && defined ( NM_ENABLE_CHANNEL_INDIRECTION )
  channel = NmCanToNmIndirection[channel];
  #endif

  ApplCanAddCanInterruptDisable(0);

  /* ESCAN00015469, ESCAN00015700 */
  TargetErrorConfig0 = CanErrorConfig0;
  TargetErrorConfig1 = CanErrorConfig1;
  TargetErrorConfig2 = CanErrorConfig2;
  TargetErrorConfig3 = CanErrorConfig3;

  ApplCanAddCanInterruptRestore(0);
} /* end of NmGetCanErrorConfig() */
  #endif
#endif  /* NM_TYPE_VAG_OSEK || NM_TYPE_PAG2_OSEK */


#if defined ( NM_TYPE_VAG_OSEK )
  #if defined ( NM_ENABLE_VAG_LAH_141 ) && defined ( NM_ENABLE_CAR_WUP_REQUEST )
/************************************************************************
| NAME:               NmCarWupRequest
| PROTOTYPE:          void NmCarWupRequest(NM_CHANNEL_NMTYPE_FIRST tNmCommStateType)
| CALLED BY:          Application
| PRECONDITIONS:      -
| INPUT PARAMETERS:   -
| RETURN VALUE:       -
| DESCRIPTION:        Sets the CarWakeUp Flag within the NM control byte.
| GLOBAL DATA:        -
| STACK:              -
| RUNTIME:            -
|*************************************************************************/
#if defined ( C_COMP_KEIL_C5X5C )
  #pragma NOAREGS
#endif
void NM_API_CALL_TYPE NmCarWupRequest(NM_CHANNEL_NMTYPE_FIRST tNmCommStateType comState)  /* ESCAN00010855 */
{
  #if defined ( NM_ENABLE_INDEXED_NM ) && defined ( NM_ENABLE_CHANNEL_INDIRECTION )
  channel = NmCanToNmIndirection[channel];
  #endif

  /*
    Set CarWakeUp Flag to on / off.
    The real CarWakeUp message will be built within NmPreTransmit().
    The command flags will be  c l e a r e d  there finally, if the CarWakeUp Flag
    is already set and should be transmitted  a s y n c h r o n o u s l y .
    The command flags will be  l e f t, if the CarWakeUp Flag
    is already set and should be transmitted  s y n c h r o n o u s l y .
  */

  if(comState == CarWupOn)
  {
    nmCarWupState = CarWupOn1stReq; /* 1st trigger */
  }
  else
  {
    nmCarWupState = CarWupOff;
  }
} /* end of NmCarWupRequest() */
  #endif
#endif  /* NM_TYPE_VAG_OSEK */


#if defined( C_MULTIPLE_RECEIVE_CHANNEL ) || defined( C_SINGLE_RECEIVE_CHANNEL )
  #define txRegPtr  (txStruct.pChipData)
  #define tmtObject (txStruct.Handle)
#endif

#if defined ( NM_TYPE_VAG_OSEK ) && \
    defined ( NM_ENABLE_VAG_LAH_141 ) && \
    defined ( NM_ENABLE_CAR_WUP_REQUEST )
/************************************************************************
| NAME:               NmPreTransmit
| PROTOTYPE:          vuint8 NmPreTransmit(CanTxInfoStruct txStruct)
| CALLED BY:          CAN Driver within transmission using CanTransmit()
| PRECONDITIONS:      -
| INPUT PARAMETERS:   txStruct
| RETURN VALUE:       - kCanNoCopyData
|                     - kCanCopyData
| DESCRIPTION:        Sets or cleares the CarWakeUp Flag within the NM control byte.
| GLOBAL DATA:        -
| STACK:              -
| RUNTIME:            -
|*************************************************************************/
#if defined ( C_COMP_KEIL_C5X5C )
  #pragma NOAREGS
#endif
#if defined ( C_MULTIPLE_RECEIVE_CHANNEL ) || defined ( C_SINGLE_RECEIVE_CHANNEL )
vuint8 NM_INTERNAL_CALL_TYPE NmPreTransmit( CanTxInfoStruct txStruct )
{
#endif
#if defined ( C_SINGLE_RECEIVE_BUFFER ) || defined ( C_MULTIPLE_RECEIVE_BUFFER )
vuint8 NM_INTERNAL_CALL_TYPE NmPreTransmit( CanChipDataPtr txRegPtr )
{
#endif

  #if defined ( C_MULTIPLE_RECEIVE_CHANNEL ) && defined ( NM_ENABLE_INDEXED_NM )
  CanChannelHandle channel;
  /* Calculate logical channel of nm based on physical channel (tmtObject) of the CAN driver */
  for(channel = 0; tmtObject != NmTxObj_Field[channel]; channel++ ){;}
  #endif

  if( nmCarWupState == CarWupOn ) /* ESCAN00017787 */
  {
    /* ESCAN00017526 */

    if( NmStatRxTxLockUd == 0 )
    {
      /* This is an asynchronous transmission.
       * => transmit only CarWakeup information */
      SendMsgByte1 = CAR_WUP;
    }
    else
    {
      /* This is a synchronous transmission.
       * => transmit normal NM information (command flags) together with the CarWakeup information */
      SendMsgByte1 |= CAR_WUP;
    }

    txRegPtr[0] = SendMsgByte0;
    txRegPtr[1] = SendMsgByte1;
    txRegPtr[2] = SendMsgUser0;
    txRegPtr[3] = SendMsgUser1;
    txRegPtr[4] = SendMsgUser2;
    txRegPtr[5] = SendMsgUser3;
    txRegPtr[6] = SendMsgUser4;
    txRegPtr[7] = SendMsgUser5;

    /* Note: There is no automatic release of the CarWaekup condition.
     *       This must be done by the CarWup application. */

    /* NM has copied the data => DrvCan may not copy it again */
    return( kCanNoCopyData ); 
  }
  else
  {
    /* DrvCan has to copy the data */
    return( kCanCopyData ); 
  }
} /* end of NmPreTransmit() */
#endif  /* NM_TYPE_VAG_OSEK */

#if defined( C_MULTIPLE_RECEIVE_CHANNEL ) || defined( C_SINGLE_RECEIVE_CHANNEL )
  #undef txRegPtr
  #undef tmtObject
#endif





/*****************************************************************************
 * Local functions
 *****************************************************************************/
/************************************************************************
| NAME:               TransmitSkippedAlive
| PROTOTYPE:          void TransmitSkippedAlive(void/channel)
| CALLED BY:          NmTask (internal)
| PRECONDITIONS:      -
| INPUT PARAMETERS:   -
| RETURN VALUE:       -
| DESCRIPTION:        Prepare Skipped Alive message for asynchronous
|                     transmission, the transmission should be pushed
|                     at the appropriate situation
|                     for indexed version, function works
|                     always with NM internal channel
| GLOBAL DATA:        -
| STACK:              -
| RUNTIME:            -
|*************************************************************************/
#if defined ( C_COMP_KEIL_C5X5C )
  #pragma NOAREGS
#endif
static void TransmitSkippedAlive(NM_CHANNEL_NMTYPE_ONLY)
{
  NmStatRxTxLockUd    = 1; /* ESCAN00008104 - wait for NmConfirmation() */
  SendMessFlag        = 1; /* send alive message */

  nmRxTxCnt          += TX_CNT_STEP; /* increment transmit counter */

  NmStatRingStable    = 0;
  NmStatRingPreStable = 0;

  #if defined ( NM_ENABLE_REMOTESLEEP_INDICATION )
  RemotePrepareSleep  = 0;
  RemoteSleepInd      = 0;
  #endif

  SendMsgCmdSet = ALIVE;
  SendMsgEcuNr  = logicalSuccessor;
  SendSleepInd  = NmStatBusSleepInd;
  SendSleepAck  = 0;

  #if defined ( NM_TYPE_VAG_OSEK ) 
    #if defined ( NM_ENABLE_VAG_LAH_130 )
  SendMsgUser0  = DATA_INIT;
  SendMsgUser1  = DATA_INIT;
  SendMsgUser2  = DATA_INIT;
  SendMsgUser3  = DATA_INIT;
    #endif
  #endif  /* NM_TYPE_VAG_OSEK || NM_TYPE_PAG2_OSEK */
} /* end of TransmitSkippedAlive() */


/************************************************************************
| NAME:               ResetConfig
| PROTOTYPE:          void ResetConfig(void)
| CALLED BY:          NmOsekInit, NmTask (internal)
| PRECONDITIONS:      -
| INPUT PARAMETERS:   -
| RETURN VALUE:       -
| DESCRIPTION:        reset all configurations in memory
|                     for indexed version, function works
|                     always with NM internal channel
| GLOBAL DATA:        -
| STACK:              -
| RUNTIME:            -
|*************************************************************************/
#if defined ( C_COMP_KEIL_C5X5C )
  #pragma NOAREGS
#endif
static void ResetConfig(NM_CHANNEL_NMTYPE_ONLY)
{
  #if defined ( NM_ENABLE_CONFIG )
  vuint8 configByte;
  #endif

  #if defined ( NM_ENABLE_INDEXED_NM ) && defined ( NM_ENABLE_CHANNEL_INDIRECTION )
  /*
    !!! Not used, because only for module internal usage on NM cannel allowed
    channel = NmCanToNmIndirection[channel];
  */
  #endif

  NmStatRingStable    = 0;
  NmStatRingPreStable = 0;

  #if defined ( NM_ENABLE_CONFIG )
    #if defined ( NM_ENABLE_INDEXED_NM )
  for(configByte =   channel      * (vuint8)((NM_NUMBER_OF_CONFIG_NODES)/8);
      configByte < ((channel + 1) * (vuint8)((NM_NUMBER_OF_CONFIG_NODES)/8));
      configByte++)
  {
    nmConfig.nmConfigChar[configByte] = 0; /* clear whole configuration */
  }

  /* calculate the index for the config table */
  configByte = (vuint8)((vuint8)(kNmEcuNr >> 3) + (channel * (vuint8)((NM_NUMBER_OF_CONFIG_NODES)/8)));
    #else
  for(configByte = 0; configByte < ((vuint8)((NM_NUMBER_OF_CONFIG_NODES)/8)); configByte++)
  {
    nmConfig.nmConfigChar[configByte] = 0; /* clear whole configuration */
  }

  /* calculate the index for the config table */
  configByte = (vuint8)(kNmEcuNr >> 3);
    #endif

  /* set own ECU number as present */
  nmConfig.nmConfigChar[configByte] |= (vuint8)(1 << (kNmEcuNr & BYTE_OFFSET_MASK));
  #endif /* NM_ENABLE_CONFIG */
} /* end of ResetConfig() */

#if defined ( NM_TYPE_VAG_OSEK ) 
  #if defined ( NM_ENABLE_VAG_LAH_130 )
/************************************************************************
| NAME:               ResetErrorConfig
| PROTOTYPE:          void ResetErrorConfig(void)
| CALLED BY:          NmOsekInit (internal)
| PRECONDITIONS:      -
| INPUT PARAMETERS:   -
| RETURN VALUE:       -
| DESCRIPTION:        reset all error configurations in memory
|                     for indexed version, function works
|                     always with NM internal channel
| GLOBAL DATA:        -
| STACK:              -
| RUNTIME:            -
|*************************************************************************/
#if defined ( C_COMP_KEIL_C5X5C )
  #pragma NOAREGS
#endif
static void ResetErrorConfig(NM_CHANNEL_NMTYPE_ONLY)
{
  #if defined ( NM_ENABLE_INDEXED_NM ) && defined ( NM_ENABLE_CHANNEL_INDIRECTION )
  /*
    !!! Not used, because only for module internal usage on NM cannel allowed
    channel = NmCanToNmIndirection[channel];
  */
  #endif

  CanErrorConfig0 = 0;
  CanErrorConfig1 = 0;
  CanErrorConfig2 = 0;
  CanErrorConfig3 = 0;

  SendMsgUser0 = DATA_INIT;
  SendMsgUser1 = DATA_INIT;
  SendMsgUser2 = DATA_INIT;
  SendMsgUser3 = DATA_INIT;
} /* end of ResetErrorConfig() */
  #endif
#endif /* NM_TYPE_VAG_OSEK || NM_TYPE_PAG2_OSEK */









/****************************************************************************/
/* End of File                                                              */
/****************************************************************************/
/************   Organi, Version 3.6.8 Vector-Informatik GmbH  ************/
