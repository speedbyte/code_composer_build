; Kernbauer Version: 1.14 Konfiguration: TMS470 Erzeugungsgangnummer: 372 

;*****************************************************************************
; Project Name: OSEK 2.2
;    File Name: osekint.asm
;
;  Module version: $vv$=1.05
;
;  Description: Interrupt handling functions for OSEK ARM7 (TI compiler version)
;
;-----------------------------------------------------------------------------
;               C O P Y R I G H T
;-----------------------------------------------------------------------------
; Copyright (c) 2000 Vector Informatik GmbH               All rights reserved.
;****************************************************************************/


  .include "tcb.inc"

; Vector release management 
; KB begin vrmAsmReleaseNumber 
; Assembler source release number 
osdVrmAsmMajRelNum           .set 3
osdVrmAsmMinRelNum           .set 10
; KB end vrmAsmReleaseNumber 
  .include "vrm.inc"

  .def _osCheckInterruptsEnabled
  .def _osCheckInterruptsDisabled
 
  .def _osDisableGlobal
  .def _osEnableGlobal
  .def _osSaveDisableLevel
  .def _osRestoreEnableLevel
  .def _osDisableLevel           
  .def _osEnableLevel      
 .if (osdUseAsmIntFuncs == 1)      
  .def _osSaveDisableLevelNested
  .def _osSaveDisableGlobal
  .def _osRestoreEnableGlobal 
  .def _osSaveDisableGlobalNested
  .def _osRestoreEnableGlobalNested
  .def _osRestoreEnableLevelNested
 .endif
 .if (osdTasksInUserMode==1)
  .def _osSWIDisableGlobal
  .def _osSWIEnableGlobal
  .def _osSWISaveDisableLevel
  .def _osSWISaveDisableLevelNested
  .def _osSWISaveDisableGlobal
  .def _osSWIRestoreEnableGlobal 
  .def _osSWIRestoreEnableLevelNested
  .def _osSWIRestoreEnableLevel
 .endif

 .if (osdTasksInUserMode == 1)
osRef_osSWIEnableLevel    .field  _osSWIEnableLevel,32
osRef_osSWIDisableLevel    .field  _osSWIDisableLevel,32
osRef_osSWISaveDisableLevel    .field  _osSWISaveDisableLevel,32
osRef_osSWIDisableGlobal    .field  _osSWIDisableGlobal,32
osRef_osSWIEnableGlobal    .field  _osSWIEnableGlobal,32
osRef_osSWISaveDisableGlobal    .field  _osSWISaveDisableGlobal,32
osRef_osSWIRestoreEnableGlobal    .field  _osSWIRestoreEnableGlobal,32
osRef_osSWISaveDisableGlobalNested    .field  _osSWISaveDisableGlobalNested,32
osRef_osSWIRestoreEnableGlobalNested    .field  _osSWIRestoreEnableGlobalNested,32
osRef_osSWISaveDisableLevelNested    .field  _osSWISaveDisableLevelNested,32
osRef_osSWIRestoreEnableLevelNested    .field  _osSWIRestoreEnableLevelNested,32
osRef_osSWIRestoreEnableLevel    .field  _osSWIRestoreEnableLevel,32
osRef_osSWIEnterSysMode    .field  _osSWIEnterSysMode,32
osRef_osSWIEnterUserMode    .field  _osSWIEnterUserMode,32

  .def _osSWIEnterSysMode      
  .def _osSWIEnterUserMode      
 .endif

  .ref _osSavedIntLevelNested
osRef_osSavedIntLevelNested    .field  _osSavedIntLevelNested,32
  .ref _osIntSaveDisableCounter
osRef_osIntSaveDisableCounter  .field  _osIntSaveDisableCounter,32
  .ref _osSavedIntLevel
osRef_osSavedIntLevel             .field  _osSavedIntLevel,32
  .ref _osSavedGlobalInt
osRef_osSavedGlobalInt         .field  _osSavedGlobalInt,32
  .ref _osIntSaveDisableCounterGlobal
osRef_osIntSaveDisableCounterGlobal .field _osIntSaveDisableCounterGlobal,32
  .ref _osSavedIntGlobalNested
osRef_osSavedIntGlobalNested  .field _osSavedIntGlobalNested,32


; Header for THUMB mode
osThumbHeader .macro fname
 .if (osdThumbMode == 1)
  .def $:fname:

$:fname:
  .state16
  bx     pc
  nop
 .endif
  .state32
  .endm


;-+--------------------------------------------------------------------------
; osCheckInterruptsEnabled                                                               
; Funktion: check if interrupts are enabled                                                       
; Parameter:                                                                 
; Returnvalue: 1: interrupts enabled    0: interrupts disabled                                                              
;----------------------------------------------------------------------------
  osThumbHeader osCheckInterruptsEnabled
_osCheckInterruptsEnabled
  mrs    a2, CPSR
  tst    a2, #0x80     ; check IRQ flag
  moveq  a1, #1
  movne  a1, #0

  bx     lr


;-+--------------------------------------------------------------------------
; osCheckInterruptsDisabled                                                               
; Funktion: check if interrupts are disabled                                                       
; Parameter:                                                                 
; Returnvalue: 1: interrupts disabled    0: interrupts enabled                                                              
;----------------------------------------------------------------------------
  osThumbHeader osCheckInterruptsDisabled
_osCheckInterruptsDisabled
  mrs    a2, CPSR
  tst    a2, #0x80     ; check IRQ flag
  moveq  a1, #0
  movne  a1, #1

  bx     lr


;-+--------------------------------------------------------------------------
; osDisableLevel                                                               
; Funktion: save level and disable up to system level, no nesting                                                       
; Parameter:                                                                 
; Returnvalue:                                                             
;----------------------------------------------------------------------------
  osThumbHeader osDisableLevel
_osDisableLevel           
  mrs a1, CPSR
 .if (osdTasksInUserMode==1)
  tst a1,#0x0f
  ldreq a4,osRef_osSWIDisableLevel
  swieq #0x80
  beq osLblDL
 .endif
  orr a1, a1, #10000000b
  msr CPSR, a1
  nop
osLblDL:
  bx     lr


 .if (osdTasksInUserMode==1)
_osSWIDisableLevel           
  mrs a1, SPSR
  orr a1, a1, #10000000b
  msr SPSR, a1
  movs   pc, lr
 .endif


;-+--------------------------------------------------------------------------
; osEnableLevel                                                               
; Funktion: Enable task level, no nesting                                                       
; Parameter:                                                                 
; Returnvalue:                                                             
;----------------------------------------------------------------------------
  osThumbHeader osEnableLevel
_osEnableLevel            ;  Enable task level, no nesting 
  mrs a1, CPSR
 .if (osdTasksInUserMode==1)
  tst a1,#0x0f
  ldreq a4,osRef_osSWIEnableLevel
  swieq #0x80
  beq osLblEL
 .endif
  bic a1, a1, #10000000b
  msr CPSR, a1
  nop
osLblEL:
  bx     lr

 .if (osdTasksInUserMode==1)
_osSWIEnableLevel            ;  Enable task level, no nesting 
  mrs a1, SPSR
  bic a1, a1, #10000000b
  msr SPSR, a1
  movs   pc, lr
 .endif


;-+--------------------------------------------------------------------------
; osDisableGlobal                                                               
; Funktion: disable all interrupts, no nesting                                                       
; Parameter:                                                                 
; Returnvalue:                                                             
;----------------------------------------------------------------------------
  osThumbHeader osDisableGlobal
_osDisableGlobal
  mrs a1, CPSR
 .if (osdTasksInUserMode==1)
  tst a1,#0x0f
  ldreq a4,osRef_osSWIDisableGlobal
  swieq #0x80
  beq osLblDG
 .endif
  orr a1, a1, #11000000b     ; set IRQ and FIQ flag
  msr CPSR, a1
  nop
osLblDG:
  bx     lr
  
 .if (osdTasksInUserMode==1)
_osSWIDisableGlobal
  mrs a1, SPSR
  orr a1, a1, #11000000b     ; set IRQ and FIQ flag
  msr SPSR, a1
  movs   pc, lr
 .endif  


;-+--------------------------------------------------------------------------
; osEnableGlobal                                                               
; Funktion: Enable all interrupts, no nesting                                                       
; Parameter:                                                                 
; Returnvalue:                                                             
;----------------------------------------------------------------------------
  osThumbHeader osEnableGlobal
_osEnableGlobal
  mrs a1, CPSR
 .if (osdTasksInUserMode==1)
  tst a1,#0x0f
  ldreq a4,osRef_osSWIEnableGlobal
  swieq #0x80
  beq osLblEG
 .endif
  bic a1, a1, #11000000b     ; reset IRQ and FIQ flag
  msr CPSR, a1
osLblEG:
  bx     lr

 .if (osdTasksInUserMode==1)
_osSWIEnableGlobal
  mrs a1, SPSR
  bic a1, a1, #11000000b     ; reset IRQ and FIQ flag
  msr SPSR, a1
  movs   pc, lr
 .endif


;-+--------------------------------------------------------------------------
; osSaveDisableLevel                                                               
; Funktion: save level and disable up to system level, no nesting                                                       
; Parameter:                                                                 
; Returnvalue:                                                             
;----------------------------------------------------------------------------
  osThumbHeader osSaveDisableLevel
_osSaveDisableLevel
  mrs a1, CPSR
 .if (osdTasksInUserMode==1)
  tst a1,#0x0f
  ldreq a4,osRef_osSWISaveDisableLevel
  swieq #0x80
  beq osLblSDL
 .endif
  ldr a2, osRef_osSavedIntLevel
  orr a3, a1, #0x80
  msr CPSR, a3
  strb a1, [a2]
  nop
osLblSDL:
  bx     lr

 .if (osdTasksInUserMode==1)
_osSWISaveDisableLevel
  mrs a1, SPSR
  ldr a2, osRef_osSavedIntLevel
  orr a3, a1, #0x80
  msr SPSR, a3
  strb a1, [a2]
  movs   pc, lr
 .endif
  

;-+--------------------------------------------------------------------------
; osSaveDisableGlobal                                                              
; Funktion: save and disable all interrupts, no nesting                                                       
; Parameter:                                                                 
; Returnvalue:                                                             
;----------------------------------------------------------------------------
  .if (osdUseAsmIntFuncs == 1)

  osThumbHeader osSaveDisableGlobal
_osSaveDisableGlobal  
  mrs a1, CPSR
 .if (osdTasksInUserMode==1)
  tst a1,#0x0f
  ldreq a4,osRef_osSWISaveDisableGlobal
  swieq #0x80
  beq osLblSDG
 .endif
  ldr a2, osRef_osSavedGlobalInt
  orr a3, a1, #11000000b
  msr CPSR, a3      ; set IRQ and FIQ flag
  strb a1, [a2]       ; store old status of IRQ and FIQ
  nop
osLblSDG:
  bx     lr

 .if (osdTasksInUserMode==1)
_osSWISaveDisableGlobal  
  mrs a1, SPSR
  ldr a2, osRef_osSavedGlobalInt
  orr a3, a1, #11000000b
  msr SPSR, a3      ; set IRQ and FIQ flag
  strb a1, [a2]       ; store old status of IRQ and FIQ
  movs   pc, lr
 .endif
 .endif
  

;-+--------------------------------------------------------------------------
; osRestoreEnableGlobal                                                              
; Funktion: restore and enable all interrupts, no nesting                                                       
; Parameter:                                                                 
; Returnvalue:                                                             
;----------------------------------------------------------------------------
  .if (osdUseAsmIntFuncs == 1)

  osThumbHeader osRestoreEnableGlobal
_osRestoreEnableGlobal 
  mrs a1, CPSR
 .if (osdTasksInUserMode==1)
  tst a1,#0x0f
  ldreq a4,osRef_osSWIRestoreEnableGlobal
  swieq #0x80
  beq osLblREG
 .endif
  ldr a2, osRef_osSavedGlobalInt 
  ldrb a3, [a2]              
  bic a1, a1, #11000000b    ; restore status of IRQ and FIQ
  bic a3, a3, #00111111b
  orr a1, a1, a3
  msr CPSR, a1
osLblREG:
  bx     lr

 .if (osdTasksInUserMode==1)
_osSWIRestoreEnableGlobal 
  mrs a1, SPSR
  ldr a2, osRef_osSavedGlobalInt 
  ldrb a3, [a2]              
  bic a1, a1, #11000000b    ; restore status of IRQ and FIQ
  bic a3, a3, #00111111b
  orr a1, a1, a3
  msr SPSR, a1
  movs   pc, lr
 .endif
 .endif


;-+--------------------------------------------------------------------------
; osSaveDisableLevelNested                                                              
; Funktion: save level and disable up to system level, with nesting                                                       
; Parameter:                                                                 
; Returnvalue:                                                             
;----------------------------------------------------------------------------
  .if (osdUseAsmIntFuncs == 1)

  osThumbHeader osSaveDisableLevelNested
_osSaveDisableLevelNested  
  ldr a3, osRef_osIntSaveDisableCounter  ; a3 = &osIntSaveDisableCounter
  ldrb a2, [a3]                  ; a2 = osIntSaveDisableCounter
  tst a2, a2
  ; if osIntSaveDisableCounter == 0 save level
  bne osLblSDLN

  mrs a1, CPSR
 .if (osdTasksInUserMode==1)
  tst a1,#0x0f
  ldreq a4,osRef_osSWISaveDisableLevelNested
  swieq #0x80
  beq osLblSDLN
 .endif
  orr a4, a1, #0x80
  msr CPSR, a4  
  ldr a4, osRef_osSavedIntLevelNested
  strb a1, [a4]         

osLblSDLN:
  ; increment osIntSaveDisableCounter
  add a2,a2,#1
  strb a2,[a3]
  bx     lr

 .if (osdTasksInUserMode==1)
_osSWISaveDisableLevelNested  
  mrs a1, SPSR
  orr a4, a1, #0x80
  msr SPSR, a4  
  ldr a4, osRef_osSavedIntLevelNested
  strb a1, [a4]         
  movs   pc, lr
 .endif
 .endif


;-+--------------------------------------------------------------------------
; osRestoreEnableLevelNested                                                              
; Funktion: restore level, with nesting                                                       
; Parameter:                                                                 
; Returnvalue:                                                             
;----------------------------------------------------------------------------
  .if (osdUseAsmIntFuncs == 1)
  osThumbHeader osRestoreEnableLevelNested
_osRestoreEnableLevelNested        
  ldr a3, osRef_osIntSaveDisableCounter  ; a3 = &osIntSaveDisableCounter
  ldrb a4, [a3]                  ; a4 = osIntSaveDisableCounter
  subs a4,a4,#1                    ; decrement osIntSaveDisableCounter
  strb a4, [a3]
  bne osLblRELN

  ; osIntSaveDisableCounter == 0 restore level0
  mrs a1, CPSR
 .if (osdTasksInUserMode==1)
  tst a1,#0x0f
  ldreq a4,osRef_osSWIRestoreEnableLevelNested
  swieq #0x80
  beq osLblRELN
 .endif
  ldr a2, osRef_osSavedIntLevelNested 
  ldrb a3, [a2]

  bic a1, a1, #10000000b
  bic a3, a3, #01111111b
  orr a1, a1, a3
  msr CPSR, a1

osLblRELN:
  bx     lr

 .if (osdTasksInUserMode==1)
_osSWIRestoreEnableLevelNested        
  mrs a1, SPSR
  ldr a2, osRef_osSavedIntLevelNested 
  ldrb a3, [a2]

  bic a1, a1, #10000000b
  bic a3, a3, #01111111b
  orr a1, a1, a3
  msr SPSR, a1
  movs   pc, lr
 .endif
 .endif


;-+--------------------------------------------------------------------------
; osSaveDisableGlobalNested                                                              
; Funktion: save and disable all interrupts, with nesting                                                       
; Parameter:                                                                 
; Returnvalue:                                                             
;----------------------------------------------------------------------------
  .if (osdUseAsmIntFuncs == 1)
  osThumbHeader osSaveDisableGlobalNested
_osSaveDisableGlobalNested  
  ldr a3, osRef_osIntSaveDisableCounterGlobal  ; a3 = &osIntSaveDisableCounterGlobal
  ldrb a2, [a3]                  ; a2 = osIntSaveDisableCounterGlobal
  tst a2, a2
  ; if osIntSaveDisableCounterGlobal == 0 save ints
  bne osLblSDGN

  mrs a1, CPSR
 .if (osdTasksInUserMode==1)
  tst a1,#0x0f
  ldreq a4,osRef_osSWISaveDisableGlobalNested
  swieq #0x80
  beq osLblSDGN
 .endif
  orr a4, a1, #0xC0
  msr CPSR, a4  
  ldr a4, osRef_osSavedIntGlobalNested
  strb a1, [a4]         

osLblSDGN:
  ; increment osIntSaveDisableCounterGlobal
  add a2,a2,#1
  strb a2,[a3]
  bx     lr

 .if (osdTasksInUserMode==1)
_osSWISaveDisableGlobalNested  
  mrs a1, SPSR
  orr a4, a1, #0xC0
  msr SPSR, a4  
  ldr a4, osRef_osSavedIntGlobalNested
  strb a1, [a4]         
  movs   pc, lr
 .endif
 .endif


;-+--------------------------------------------------------------------------
; osRestoreEnableGlobalNested                                                              
; Funktion: restore ints, with nesting                                                       
; Parameter:                                                                 
; Returnvalue:                                                             
;----------------------------------------------------------------------------
  .if (osdUseAsmIntFuncs == 1)
  osThumbHeader osRestoreEnableGlobalNested
_osRestoreEnableGlobalNested        
  ldr a3, osRef_osIntSaveDisableCounterGlobal  ; a3 = &osIntSaveDisableCounterGlobal
  ldrb a4, [a3]                  ; a4 = osIntSaveDisableCounterGlobal
  subs a4,a4,#1                    ; decrement osIntSaveDisableCounterGlobal
  strb a4, [a3]
  bne osLblREGN

  ; osRef_osIntSaveDisableCounterGlobal == 0 restore ints
  mrs a1, CPSR
 .if (osdTasksInUserMode==1)
  tst a1,#0x0f
  ldreq a4,osRef_osSWIRestoreEnableGlobalNested
  swieq #0x80
  beq osLblREGN
 .endif
  ldr a2, osRef_osSavedIntGlobalNested 
  ldrb a3, [a2]

  bic a1, a1, #11000000b
  bic a3, a3, #00111111b
  orr a1, a1, a3
  msr CPSR, a1

osLblREGN:
  bx     lr

 .if (osdTasksInUserMode==1)
_osSWIRestoreEnableGlobalNested        
  mrs a1, SPSR
  ldr a2, osRef_osSavedIntGlobalNested 
  ldrb a3, [a2]

  bic a1, a1, #11000000b
  bic a3, a3, #00111111b
  orr a1, a1, a3
  msr SPSR, a1
  movs   pc, lr
 .endif
 .endif


;-+--------------------------------------------------------------------------
; osRestoreEnableLevel                                                              
; Funktion: restore level, no nesting                                                       
; Parameter:                                                                 
; Returnvalue:                                                             
;----------------------------------------------------------------------------
  osThumbHeader osRestoreEnableLevel
_osRestoreEnableLevel      
  mrs a1, CPSR
 .if (osdTasksInUserMode==1)
  tst a1,#0x0f
  ldreq a4,osRef_osSWIRestoreEnableLevel
  swieq #0x80
  beq osLblSDL
 .endif
  ldr a2, osRef_osSavedIntLevel 
  ldrb a3, [a2]              
  bic a1, a1, #10000000b
  bic a3, a3, #01111111b
  orr a1, a1, a3
  msr CPSR, a1
  bx     lr

 .if (osdTasksInUserMode==1)
_osSWIRestoreEnableLevel      
  mrs a1, SPSR
  ldr a2, osRef_osSavedIntLevel 
  ldrb a3, [a2]              
  bic a1, a1, #10000000b
  bic a3, a3, #01111111b
  orr a1, a1, a3
  msr SPSR, a1
  movs   pc, lr


  osThumbHeader osSWIEnterSysMode
_osSWIEnterSysMode      
  mrs a1, SPSR
  orr a1, a1, #00001111b
  msr SPSR, a1
  movs   pc, lr

  osThumbHeader osSWIEnterUserMode
_osSWIEnterUserMode      
  mrs a1, SPSR
  bic a1, a1, #00001111b
  msr SPSR, a1
  movs   pc, lr
 .endif
