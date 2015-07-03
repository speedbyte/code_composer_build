; Kernbauer Version: 1.14 Konfiguration: TMS470 Erzeugungsgangnummer: 372 

;*****************************************************************************
; Project Name: OSEK 2.2
;    File Name: osekasm.asm
;
;  Module version: $vv$=1.09
;
;  Description: Assembler functions for OSEK ARM7 (TI compiler version)
;
;-----------------------------------------------------------------------------
;               C O P Y R I G H T
;-----------------------------------------------------------------------------
; Copyright (c) 2000 Vector Informatik GmbH               All rights reserved.
;****************************************************************************/


; some OSEK constants
osRef_AAAAAAAA             .word 0AAAAAAAAh

PRE_READY                  .equ 0
RUNNING                    .equ 1
READY                      .equ 3

STANDARD_STATUS            .equ 0
EXTENDED_STATUS            .equ 1


  .include "tcb.inc"

; Vector release management 
; KB begin vrmAsmReleaseNumber 
; Assembler source release number 
osdVrmAsmMajRelNum           .set 3
osdVrmAsmMinRelNum           .set 10
; KB end vrmAsmReleaseNumber 
  .include "vrm.inc"

  .def _osDispatcher
  .def _osDirectSWIDispatcher
  .def _osStartOSasm

  .def _osPrefetchAbort
  .def _osFIQ
  .def _osUnhandledExceptionWrapper
  .def _osDataAbort
  .def _osSWI
  .def _osUndefInstruction

osExtRef .macro rname
  .ref _:rname:
osRef_:rname:    .word  _:rname:
 .endm


 .if (osdRegisterOpt == 0)
  osExtRef osActiveTaskIndex
  osExtRef osHighReadyTaskPrio
 .endif
  osExtRef osActiveTaskPrio
  osExtRef osHighReadyTaskIndex
  osExtRef osTcbSP
 .if (osdNonPreempt == 0)
  osExtRef osLockDispatcher
 .endif
  osExtRef osTcbTaskState
  osExtRef oskTcbStackTop
  osExtRef oskTcbTaskStartAddress
  osExtRef osStartOSc
  osExtRef osSystemStack
  osExtRef osIrqStack
  osExtRef osSupervisorStack
  osExtRef osStartDispatcher
  osExtRef osUnhandledException
  osExtRef osOccupyInternalResource

  .ref osdStartupStack
osRef_osdStartupStack  .word osdStartupStack


 .if (osOwnApplSWIHandler == 1) 
   .ref osdApplSWIHandler
 .endif

 .if (osdORTIDebug == 1) 
 .if (osdORTIVersion == 200)
  osExtRef osORTICurrentServiceId
  osExtRef osTcbORTIServiceId
 .endif
 .endif

 .if (osdMixedPreempt == 1)
  osExtRef oskTcbTaskPreemption
 .endif

 .if (osdPreTaskHook == 1)
  osExtRef osPreTaskHook
 .endif
 .if (osdPostTaskHook == 1)
  osExtRef osPostTaskHook
 .endif

 .if (STATUS_LEVEL == EXTENDED_STATUS)
 .if (osdThumbMode == 0)
  osExtRef osMissingTerminateError
 .else
  .ref $osMissingTerminateError
osRef_osMissingTerminateError:    .word  $osMissingTerminateError
 .endif
 .endif

 .if (osdStackCheck == 1)
  osExtRef osStackOverflow
  osExtRef oskTcbStackBottom
  osExtRef osFillSystemStack
  osExtRef osLastActiveTaskIndex
 .endif


osREQMASK .equ 0xffffff34
osPCR     .equ 0xfffffd30
osSSIR    .equ 0xfffffffc
osSSIF    .equ 0xfffffff8
osCIMIVEC .equ 0xffffff28
osGLBCTRL .equ 0xffffffdc

osRef_PCR             .word osPCR
osRef_osdNumberOfAllTasks  .word osdNumberOfAllTasks
osRef_SystemStackTop   .word _osSystemStack+osdSystemStackSize
osRef_StartupStackTop   .word osdStartupStack+osdStartupStackSize
osRef_SupervisorStackTop   .word _osSupervisorStack+osdSupervisorStackSize
osRef_IrqStackTop   .word _osIrqStack+osdIrqStackSize

; register mapping for Dispatcher
   .asg r0,  RegCPSRval              
   .asg r1,  RegTaskLR               
   .asg r2,  RegTaskSP               
   .asg r3,  RegAdr_osTcbSP          
   .asg r4,  RegAdr_osTcbTaskState   
  .if (osdRegisterOpt == 0)
   .asg r5,  RegAdr_oskTcbStackTop   
   .asg r6,  RegTmp1      
   .asg r7,  RegTmp2                 
   .asg r8,  RegTmp3                 
   .asg r9,  RegTmp4                 
   .asg r10, RegAdr_osActiveTaskIndex
   .asg r11, Reg_osActiveTaskIndex   
  .else
   .asg r5,  Reg_osActiveTaskIndex   
   .asg r6,  Reg_osHighReadyTaskPrio      
   .asg r7,  RegTmp1                 
   .asg r8,  RegTmp2                 
   .asg r9,  RegTmp3                 
   .asg r10, RegTmp4
   .asg r11, RegAdr_oskTcbStackTop
  .endif
   .asg r12, RegNoActiveTask         


; Stack frame:
;
;    r0
;    r1
;    r2
;    SPSR
;    lr (svc) = return of interrupt
;    lr (task) = saved task context
;    r3
;    r4
;    r5
;    r6
;    r7
;    r8
;    r9
;    r10
;    r11
;    r12


;-+--------------------------------------------------------------------------
; osDispatcher                                                               
; Funktion: osDispatcher                                                       
;   - saves old task kontext
;   - switches to new task
;   - contains IDLE-Loop                                                     
; Parameter:                                                                 
; Returnvalue:                                                               
;----------------------------------------------------------------------------

_osDirectSWIDispatcher    ; called directly by SWI vector
  stmfd  sp!,{r0-r2}

_osDispatcher

  ; switch to system mode (for stack access)
  mrs    RegCPSRval, CPSR                         ; RegCPSRval : saved CPSR
  orr    RegCPSRval, RegCPSRval, #011111b
  msr    CPSR, RegCPSRval

  ; get SP of system/user mode
  mov    RegTaskSP, sp                            ; RegTaskSP : task SP
  mov    RegTaskLR, lr                            ; RegTaskLR : task LR

  ; switch back to SVC mode
  bic    RegCPSRval, RegCPSRval, #01100b
  msr    CPSR, RegCPSRval

 .if (osdRegisterOpt == 0)
  stmfd  RegTaskSP!,{RegAdr_osActiveTaskIndex-RegNoActiveTask}  ; save r10-r12 (on task stack!) 
  ; load osActiveTaskIndex into Reg_osActiveTaskIndex
  ldr    RegAdr_osActiveTaskIndex, osRef_osActiveTaskIndex           ; RegAdr_osActiveTaskIndex : &osActiveTaskIndex
  ldrh   Reg_osActiveTaskIndex, [RegAdr_osActiveTaskIndex]      ; Reg_osActiveTaskIndex : osActiveTaskIndex
 .else
  stmfd  RegTaskSP!,{RegNoActiveTask}                           ; save r12 (on task stack!) 
  ; osActiveTaskIndex is already located in Reg_osActiveTaskIndex
 .endif

;  ldr    RegNoActiveTask, osdNoActiveTask                      ; RegNoActiveTask : osdNoActiveTask
  mov    RegNoActiveTask,#0x10000
  sub    RegNoActiveTask,RegNoActiveTask,#1
  cmp    Reg_osActiveTaskIndex, RegNoActiveTask                 ; osActiveTaskIndex == osdNoActiveTask ?

 .if (osdStackCheck == 1)
  ; check task stack
  bne    osDispNoCheck

  add    sp,sp,#12              ; if not saving old context: restore SP

  ldr    RegTmp2, osRef_oskTcbStackBottom                         ; RegTmp2 : &oskTcbStackBottom
  ldr    RegTmp3, osRef_osLastActiveTaskIndex                     ; RegTmp3 : &osLastActiveTaskIndex
  ldrh   RegTmp4,[RegTmp3]                                   ; RegTmp4 : osLastActiveTaskIndex
  ldr    RegTmp3,[RegTmp2, +RegTmp4, lsl #2]        ; RegTmp3 : oskTcbStackBottom[RegTmp4]
  ldr    RegTmp2, [RegTmp3];
  ldr    RegTmp3, osRef_AAAAAAAA;
  cmp    RegTmp2, RegTmp3
 .if (osdThumbMode == 1)
  ldr    RegTmp2, osRef_osStackOverflow
  mov    lr, pc
  bxne   RegTmp2
 .else
  blne   _osStackOverflow
 .endif
  b      osNextTask

osDispNoCheck
 .else   ; no stack check
  addeq  sp,sp,#12              ; if not saving old context: restore SP
  beq    osNextTask
 .endif

  ; save old task context (on task stack!)
 .if (osdRegisterOpt == 0)
  stmfd  RegTaskSP!,{RegTaskLR, RegAdr_osTcbSP-RegTmp4}  ; save LR, r3-r9
 .else
  stmfd  RegTaskSP!,{RegTaskLR, RegAdr_osTcbSP-RegAdr_osTcbTaskState, RegTmp1-RegAdr_oskTcbStackTop}  ; save LR, r3-r4, r7-r11
 .endif

  mrs    RegTmp4, SPSR
  ldmfd  sp!,{RegTmp1, RegTmp2, RegTmp3}      ; get task registers from SVC stack
  stmfd  RegTaskSP!,{RegTmp1-RegTmp4,lr}      ; save task registers on task stack
 
 .if (osdORTIDebug == 1) 
 .if (osdORTIVersion == 200)
   ; store CurrentService into tcb
   ldr    RegTmp3,osRef_osORTICurrentServiceId
   ldr    RegTmp2,osRef_osTcbORTIServiceId
   ldrb   RegTmp3,[RegTmp3]
   strb   RegTmp3,[RegTmp2, +Reg_osActiveTaskIndex]  
 .endif
 .endif
  
  ldr    RegAdr_osTcbTaskState, osRef_osTcbTaskState                   ; RegAdr_osTcbTaskState : &osTcbTaskState  
  ldrb   RegTmp3,[RegAdr_osTcbTaskState, +Reg_osActiveTaskIndex]  ; RegTmp3 : osTcbTaskState[osActiveTaskIndex]
  cmp    RegTmp3,#1

 .if (osdPostTaskHook == 1)
  bne    osL01
  ; call PostTaskHook
  mov    RegTmp3, sp
  mov    sp, RegTaskSP

 .if (osdThumbMode == 1)
  stmfd  sp!,{a1-a4,r8,r9,ip}
  ldr    a1, osRef_osPostTaskHook
  mov    lr, pc
  bx     a1
  ldmfd  sp!,{a1-a4,r8,r9,ip}
 .else
  stmfd  sp!,{a1-a4,ip}
  bl     _osPostTaskHook
  ldmfd  sp!,{a1-a4,ip}
 .endif

  mov    sp, RegTmp3

  ; set old task state to READY
  mov    RegTmp3,#READY         
  strb   RegTmp3,[RegAdr_osTcbTaskState, +Reg_osActiveTaskIndex]  ; RegTmp3 : osTcbTaskState[osActiveTaskIndex]
 .else
  moveq    RegTmp3,#READY         
  streqb   RegTmp3,[RegAdr_osTcbTaskState, +Reg_osActiveTaskIndex]  ; RegTmp3 : osTcbTaskState[osActiveTaskIndex]
 .endif

  
osL01:
 .if (osdStackCheck == 1)
  ; check task stack
  ldr    RegTmp2, osRef_oskTcbStackBottom                              ; RegTmp2 : &oskTcbStackBottom
  ldr    RegTmp3,[RegTmp2, +Reg_osActiveTaskIndex, lsl #2]        ; RegTmp3 : oskTcbStackBottom[Reg_osActiveTaskIndex]
  ldr    RegTmp2, [RegTmp3];
  ldr    RegTmp3, osRef_AAAAAAAA;
  cmp    RegTmp2, RegTmp3

 .if (osdThumbMode == 1)
  ldr    RegTmp2, osRef_osStackOverflow
  mov    lr, pc
  bxne   RegTmp2
 .else
  blne   _osStackOverflow
 .endif
 .endif

  ; save stack pointer
  ldr    RegAdr_osTcbSP, osRef_osTcbSP                         ; RegAdr_osTcbSP : &osTcbSP
  str    RegTaskSP,[RegAdr_osTcbSP, +Reg_osActiveTaskIndex, lsl #2] ; osTcbSP[osActiveTaskIndex] = SP (task)

osNextTask   ; start next task
  ldr    RegTmp1, osRef_osHighReadyTaskIndex                   ; RegTmp1 : &osHighReadyTaskIndex
  ldrh   Reg_osActiveTaskIndex,[RegTmp1]       
 .if (osdRegisterOpt == 0)               
  strh   Reg_osActiveTaskIndex,[RegAdr_osActiveTaskIndex] ; Reg_osActiveTaskIndex : osActiveTaskIndex = osHighReadyTaskIndex
 .endif

 .if (osdRegisterOpt == 0)
  ldr    RegTmp2, osRef_osHighReadyTaskPrio                    ; RegTmp2 : &osHighReadyTaskPrio
  ldrh   RegTmp3,[RegTmp2]                                ; RegTmp3 : osHighReadyTaskPrio
  ldr    RegTmp2, osRef_osActiveTaskPrio                       ; RegTmp2 : &osActiveTaskPrio  
  strh   RegTmp3,[RegTmp2]                                ; osActiveTaskPrio = osHighReadyTaskPrio
 .else
  ldr    RegTmp2, osRef_osActiveTaskPrio                       ; RegTmp2 : &osActiveTaskPrio  
  strh   Reg_osHighReadyTaskPrio,[RegTmp2]                                ; osActiveTaskPrio = osHighReadyTaskPrio
 .endif

  cmp    Reg_osActiveTaskIndex, RegNoActiveTask           ; (new) osActiveTaskIndex == OS_NO_ACTIVE_TASK ?
  beq    IdleLoop

  ; switch to next task
  ; if there are internal resources check if an internal resource needs to be occupied
 .if (osdNumberOfInternalResources > 0)
  stmfd  sp!,{a1-a4,r8,r9,ip}
  bl _osOccupyInternalResource
  ldmfd  sp!,{a1-a4,r8,r9,ip}
 .endif
 
 .if (osdMixedPreempt == 1)
  ; lock dispatcher for nonpreemptive tasks, unlock for preemptive tasks
  ldr    RegTmp2, osRef_oskTcbTaskPreemption                   ; RegTmp2 : &oskTcbTaskPreemption  
  ldrb   RegTmp3,[RegTmp2, +Reg_osActiveTaskIndex]        ; RegTmp3 : oskTaskPreemption[osActiveTaskIndex]
  eor    RegTmp3,RegTmp3,#1                               ; if RegTmp3 = 1 then RegTmp3 = 0 else RegTmp3 = 1
  ldr    RegTmp2, osRef_osLockDispatcher                       ; RegTmp2 : &osLockDispatcher  
  strb   RegTmp3,[RegTmp2]             
 .endif

  ldr    RegAdr_osTcbTaskState, osRef_osTcbTaskState           ; RegAdr_osTcbTaskState : &osTcbTaskState  
  ldrb   RegTmp3,[RegAdr_osTcbTaskState, +Reg_osActiveTaskIndex]     ; RegTmp3 : osTcbTaskState[osActiveTaskIndex]

  cmp    RegTmp3,#PRE_READY                                ; osTcbTaskState[osActiveTaskIndex] == PRE_READY ?
  bne    RestoreContext             

  ; task is running for the first time: create new stack frame
 .if (osdORTIDebug == 1) 
 .if (osdORTIVersion == 200)
  ; osORTICurrentServiceId=osdORTINoService; 
  ldr    RegTmp1, osRef_osORTICurrentServiceId
  mov    RegTmp2, #osdORTINoService
  strb   RegTmp2,[RegTmp1]
 .endif
 .endif
  ldr    RegTmp2, osRef_oskTcbStackTop                         ; RegTmp2 : &oskTcbStackTop  
  ldr    RegAdr_oskTcbStackTop,[RegTmp2, +Reg_osActiveTaskIndex, lsl #2]  ; RegAdr_oskTcbStackTop : new task SP = oskTcbStackTop[osActiveTaskIndex]

  ; set task state to running
  mov    RegTmp3,#RUNNING                                                  ; RegTmp3 : RUNNING
  strb   RegTmp3,[RegAdr_osTcbTaskState, +Reg_osActiveTaskIndex]          ; osTcbTaskState[osActiveTaskIndex] = RUNNING

  ; reset osStartDispatcher
  ldr    RegTmp2, osRef_osStartDispatcher                                      ; RegTmp2 : &osStartDispatcher  
  eor    RegTmp3, RegTmp3, RegTmp3                                        ; RegTmp3 : 0
  strb   RegTmp3, [RegTmp2]                                               ; osStartDispatcher = 0;

  ; get task start address
  ldr    RegTmp2, osRef_oskTcbTaskStartAddress                                 ; RegTmp2 : &oskTcbTaskStartAddress   
  ldr    RegTmp3,[RegTmp2, +Reg_osActiveTaskIndex, lsl #2]                ; RegTmp3 : oskTcbTaskStartAddress[osActiveTaskIndex]

  ; switch to system mode (for stack access)
  orr    RegCPSRval, RegCPSRval, #0x1F
  msr    CPSR, RegCPSRval

  mov    sp, RegAdr_oskTcbStackTop            ; set task SP of system/user mode

 .if (osdPreTaskHook == 1)
  ; call PreTaskHook
  str    RegCPSRval,[sp, #-4]!                 ; push RegCPSRval
 .if (osdThumbMode == 1)
  stmfd  sp!,{a1-a4,r8,r9,ip}
  ldr    a1, osRef_osPreTaskHook
  mov    lr, pc
  bx   a1
  ldmfd  sp!,{a1-a4,r8,r9,ip}
 .else
  stmfd  sp!,{a1-a4,ip}
  bl     _osPreTaskHook
  ldmfd  sp!,{a1-a4,ip}
 .endif
  ldr    RegCPSRval,[sp],#4                 ; pop RegCPSRval
 .endif

 .if (STATUS_LEVEL == EXTENDED_STATUS)
  ; set lr to error handler
  ldr    lr, osRef_osMissingTerminateError
 .endif

  ; switch back to SVC mode
  bic    RegCPSRval, RegCPSRval, #01100b
  msr    CPSR, RegCPSRval

  ; init status register
  mrs    RegTmp2, SPSR
  bic    RegTmp2, RegTmp2, #10101111b
 .if (osdThumbMode == 1)
  .if (osdTasksInUserMode == 1)
  orr    RegTmp2, RegTmp2, #00110000b
  .else
  orr    RegTmp2, RegTmp2, #00111111b
  .endif
 .else
  .if (osdTasksInUserMode == 1)
  orr    RegTmp2, RegTmp2, #00010000b
  .else
  orr    RegTmp2, RegTmp2, #00011111b
  .endif
 .endif
  msr    SPSR, RegTmp2

  movs   pc, RegTmp3            ; jump to new task


RestoreContext
  ; task was running before: restore old task context

 .if (osdORTIDebug == 1) 
 .if (osdORTIVersion == 200)
   ; restore tcb into CurrentService
   ldr    RegTmp2,osRef_osTcbORTIServiceId
   ldr    RegTmp3,osRef_osORTICurrentServiceId
   ldrb   RegTmp2,[RegTmp2, +Reg_osActiveTaskIndex]  
   strb   RegTmp2,[RegTmp3]
 .endif
 .endif

  ; restore stack pointer
  ldr    RegAdr_osTcbSP, osRef_osTcbSP                                 ; RegAdr_osTcbSP : osTcbSP
  ldr    RegTmp2,[RegAdr_osTcbSP, +Reg_osActiveTaskIndex, lsl #2] ; RegTmp2 = osTcbSP[osActiveTaskIndex]

  ; set task state to RUNNING
  ldr    RegAdr_osTcbTaskState, osRef_osTcbTaskState                   ; RegAdr_osTcbTaskState : &osTcbTaskState  
  mov    RegTmp3,#RUNNING                                          ; RegTmp3 : RUNNING
  strb   RegTmp3,[RegAdr_osTcbTaskState, +Reg_osActiveTaskIndex]  ; osTcbTaskState[osActiveTaskIndex] = RUNNING

  ; reset osStartDispatcher
  ldr    RegTmp1, osRef_osStartDispatcher                              ; RegTmp1 : &osStartDispatcher  
  eor    RegTmp3, RegTmp3, RegTmp3                                ; RegTmp3 : 0
  strb   RegTmp3, [RegTmp1]                                       ; osStartDispatcher = 0;

  ; restore task context

  ldmfd  RegTmp2!,{RegTmp3}   ; pop first register
  stmfd  sp!,{RegTmp3}

  ldmfd  RegTmp2!,{RegTaskLR-RegTaskSP, RegTmp3, lr}   ; pop next 2 registers, SPSR (to RegTmp3) and LR

  msr    SPSR, RegTmp3
  
  ; switch to system mode (for stack access)

  orr    RegCPSRval, RegCPSRval, #0x1F
  msr    CPSR, RegCPSRval

  mov    sp, RegTmp2                                    ; set task SP 

 .if (osdPreTaskHook == 1)
  ; call PreTaskHook

 .if (osdThumbMode == 1)
  stmfd  sp!,{a1-a4,r8,r9,ip}
  ldr    a1, osRef_osPreTaskHook
  mov    lr, pc
  bx     a1
  ldmfd  sp!,{a1-a4,r8,r9,ip}
 .else
  stmfd  sp!,{a1-a4,ip}
  bl     _osPreTaskHook
  ldmfd  sp!,{a1-a4,ip}
 .endif

 .endif

  ldr    lr,[sp],#4                                     ; pop lr
 .if (osdRegisterOpt == 0)
  ldmfd  sp!,{RegAdr_osTcbSP-RegNoActiveTask} 
 .else
  ldmfd  sp!,{RegAdr_osTcbSP-RegAdr_osTcbTaskState, RegTmp1-RegNoActiveTask} 
 .endif

  ; now all registers except RegCPSRval are valid and RegCPSRval ist pushed to SVC stack

  ; switch back to SVC mode
  bic    RegCPSRval, RegCPSRval, #01100b
  msr    CPSR, RegCPSRval

  ldr    RegCPSRval,[sp],#4            ; pop RegCPSRval
  movs   pc,lr                         ; return from dispatcher

IdleLoop
  ; called if no task is READY

  ; switch to system mode (for stack access)
  mrs    RegTmp3, CPSR                               ; switch to SYSTEM mode
  orr    RegTmp3, RegTmp3, #0x1F
  msr    CPSR, RegTmp3
  
  ; set stack pointer to IRQ-Stack to avoid corrupted task stack
  ; when saving scratch registers after entering dispatcher from
  ; idle loop
  ldr sp, osRef_IrqStackTop

  ; switch back to SVC mode 
  bic    RegTmp3, RegTmp3, #01100b
  msr    CPSR, RegTmp3
  
 .if (osdMixedPreempt == 1)
  ; unlock dispatcher
  mov    RegTmp3, #0
  ldr    RegTmp2, osRef_osLockDispatcher    ; RegTmp2 : &osLockDispatcher  
  strb   RegTmp3,[RegTmp2]             
 .endif

  ; simulate end of interrupt
  mrs    r0, SPSR                      ; copy SPSR to CPSR
 .if (osdThumbMode == 1)
  bic    r0, r0, #00100000b;
 .endif
  msr    CPSR, r0

InnerIdleLoop

 .if (osdNonPreempt == 1)
  ; poll osStartDispatcher
  ldr    RegTmp2, osRef_osStartDispatcher   
  ldrb   RegTmp3, [RegTmp2]
  cmp    RegTmp3, #0
  ; if osStartDispatcher == TRUE activate delayed interrupt
  beq    l_nodelayedint
  swi    #0x81    ; start dispatcher
 .endif
l_nodelayedint
  b InnerIdleLoop


; handlers for unhandled exceptions

_osUndefInstruction:
 .if (osdThumbMode == 1)
  ldr    a1, osRef_osUnhandledException
  mov    lr, pc
  bx     a1
 .else
  bl     _osUnhandledException
 .endif

  
_osSWI:
  stmfd  sp!,{r0-r2}

 .if (osdThumbMode == 1)
  ldrb   r1,[lr, #-1]
 .else
  ldrh   r1,[lr, #-2]
 .endif
  mov    r0, #128
  sub    r2, r1, r0
  cmp    r2, #128
  bcs    osSWIL1

  cmp    r1, #0x81
  bne    osSWIL2
  
  ; goto Dispatcher
  b      _osDispatcher

osSWIL2:   ; goto r3 fct
  ldmfd  sp!,{r0-r2}
  mov    pc, r3

osSWIL1:   ; link to appl SWI handler
  ldmfd  sp!,{r0-r2}    

  b      osdApplSWIHandler


_osPrefetchAbort:
 .if (osdThumbMode == 1)
  ldr    a1, osRef_osUnhandledException
  mov    lr, pc
  bx     a1
 .else
  bl     _osUnhandledException
 .endif

  
_osDataAbort:
 .if (osdThumbMode == 1)
  ldr    a1, osRef_osUnhandledException
  mov    lr, pc
  bx     a1
 .else
  bl     _osUnhandledException
 .endif

_osFIQ:
 .if (osdThumbMode == 1)
  ldr    a1, osRef_osUnhandledException
  mov    lr, pc
  bx     a1
 .else
  bl     _osUnhandledException
 .endif


_osUnhandledExceptionWrapper:
 .if (osdThumbMode == 1)
  ldr    a1, osRef_osUnhandledException
  mov    lr, pc
  bx     a1
 .else
  bl     _osUnhandledException
 .endif


;-+--------------------------------------------------------------------------
; osStartOSasm                                                               
; Funktion: Assembler startup code                                                       
;   - assembler initialisation code                                                 
;   - jumps to osStartOSc
;   - never returns
; Parameter:                                                                 
; Returnvalue:                                                               
;----------------------------------------------------------------------------
   
 .if (osdThumbMode == 1)
  .def $osStartOSasm
$osStartOSasm
  .state16
  bx     pc
  nop
 .endif
  .state32

_osStartOSasm
  ; set IRQ mode, disable IRQ
  mrs a1, CPSR
  bic a1, a1, #00101101b
  orr a1, a1, #10010010b
  msr CPSR, a1

  ; set IRQ stack pointer
  ldr sp, osRef_IrqStackTop

 .if (osdSupervisorStackSize > 0)
  ; switch to supervisor mode
  bic a1, a1, #00001100b
  orr a1, a1, #10010011b
  msr CPSR, a1

  ; set SVC stack pointer
  ldr sp, osRef_SupervisorStackTop
 .endif

  ; switch to system mode
  orr a1, a1, #10011111b
  msr CPSR, a1

  ; set stack pointer to startup stack
  ldr sp, osRef_StartupStackTop

 .if (osdStackCheck == 1)
  ; fill system stack with empty pattern
 .if (osdThumbMode == 1)
  ldr   a1, osRef_osFillSystemStack
  mov   lr, pc
  bx    a1
 .else
  bl _osFillSystemStack
 .endif
 .endif

 .if (!$$defined(osdTMS470PVF24))
  ; peripheral enable
  ldr    a1, osRef_PCR
  ldr    a2, [a1]
  orr    a2, a2, #1
  str    a2, [a1]
 .endif

 .if (osdStackCheck == 1)
  ; fill task stacks
  bl    osFillTaskStacks
 .endif

 .if (osdThumbMode == 1)
  ; jump to osStartOSc  (never returns)
  ldr   a1, osRef_osStartOSc
  bx    a1
 .else
  b     _osStartOSc
 .endif
 


 .if (osdStackCheck == 1)
;-+--------------------------------------------------------------------------
; osFillTaskStacks                                                               
; Funktion: Fill task stacks with empty pattern                                                       
; Parameter:                                                                 
; Returnvalue:                                                               
;----------------------------------------------------------------------------
   
osFillTaskStacks:
  mov a1, #0                      ; a1: task index
  ldr a4, osRef_osdNumberOfAllTasks      ; a4: number of tasks
  ldr r12, osRef_AAAAAAAA
  
osFillLoop1                         ; for all tasks
  cmp a1, a4
  bcs osExitFillTaskStack

  ldr a3, osRef_oskTcbStackTop   
  ldr a2,[a3, +a1, lsl #2]          ; a2: oskTcbStackTop[task index]

  tst a2, a2
  beq osNextTaskIndex       

  ldr a3, osRef_StartupStackTop      ; if stack == startup stack fill only from current sp
  cmp a2, a3
  moveq a2, sp                                                                                    

  ldr a3, osRef_oskTcbStackBottom
  ldr a3,[a3, +a1, lsl #2]          ; a3: oskTcbStackBottom[task index]

osFillLoop2                         ; for all stack words
  cmp a3, a2
  bcs osNextTaskIndex

  str r12, [a2, #-4]!                ; fill with empty pattern
  b   osFillLoop2
  
osNextTaskIndex  
  add a1, a1, #1                     ; next task index
  b   osFillLoop1
  
osExitFillTaskStack
  mov pc,lr
 .endif

