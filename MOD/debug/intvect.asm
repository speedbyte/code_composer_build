; file: intvect.asm, automatically generated by GENTMS470.EXE, Version: 3.10
; from: C:\PROJECT_PARROT_SOFTWARE\SYS_PLATTFORM_WORKBENCH\UHV_B1_V002\MOD\MOD_OSEK_DYN\CFG\UHV_NAR.oil
; at  : Mon Sep 22 12:03:54 2008
; License 2400100185 for S1nn GmbH u. Co. KG, Infotainment and Connectivity
; Implementation: TMS470R1
; Version of general code: 3.54b


osdVrmAsmGenMajRelNum     .set 3
osdVrmAsmGenMinRelNum     .set 10

   .state32

   .include "tcb.inc"
   
   ; added for second jump table placed on memory space 0x6000
    .global  _osIRQHandler           
   
   .ref _osUnhandledExceptionWrapper
   .ref _osDirectSWIDispatcher
 .if (!$$defined(osdSuppressDefVectWarning))
   .ref _osDefVectWarning
 .endif


   .ref _osSystemStack
 .if (osdStackCheck == 1)
   .ref _osSystemStackOverflow
osRef_osSystemStackOverflow .word _osSystemStackOverflow
 .endif

 .if ((osdNonPreempt == 0))
   .ref _osLockDispatcher
osRef_osLockDispatcher .word _osLockDispatcher
 .endif

   .ref _osIntNestingDepth
osRef_osIntNestingDepth .word _osIntNestingDepth
   .ref _osSystemStack
osRef_SystemStackTop .word _osSystemStack + osdSystemStackSize
osRef_SystemStack .word _osSystemStack
 .if (!osdNonPreempt)
   .ref _osStartDispatcher
osRef_osStartDispatcher .word _osStartDispatcher
   .ref _osSupervisorStack
osRef_SupervisorStackTop .word _osSupervisorStack + osdSupervisorStackSize
   .ref _osDispatcher
osRef_osDispatcher .word _osDispatcher
 .endif

 .if (osdORTIDebug == 1)
 .if (osdORTIVersion == 200)
   .ref _osORTICurrentServiceId
osRef_osORTICurrentServiceId .word _osORTICurrentServiceId
 .endif

 .endif

osRef_AAAAAAAA .word 0xAAAAAAAA; 

_osEnterCallLeaveISR
   ;registers {r3,r8,r9,ip,lr} already pushed and lr points to ISR function reference
   ldr    r8,[lr]              ; must never run in FIQ mode where r8 is shadowed!
   ldr    lr,[sp,#0x10]        ; for the following code, restore situation compatible w/ old version

   mrs    a1, SPSR
   str    a1, [sp, #-4]!

   ; lock Dispatcher
 .if (!osdNonPreempt)
   ldr    a1, osRef_osLockDispatcher
   ldrb   a2,[a1]
   add    a2,a2,#1
   strb   a2,[a1]
 .endif


   ; increment osIntNestingDepth
   ldr    a1, osRef_osIntNestingDepth
   ldrb   a2,[a1]
   cmp    a2, #0               ; set Z if first interrupt nesting level
   add    a2,a2,#1
   strb   a2,[a1]

   ; switch to system mode
   mrs    a4, CPSR
   orr    a4, a4, #0x1F
   msr    CPSR, a4

   bne    _EnterISR_nested
   ; on first interrupt level: load SYSTEM SP with system stack and save sp
   mov    a1, sp
   ldr    sp, osRef_SystemStackTop
   str    a1, [sp, #-4]!         ; save sp
_EnterISR_nested

   str    lr, [sp, #-4]!         ; save LR
   

 .if (osdORTIDebug == 1)
 .if (osdORTIVersion == 200)
   ldr    a1,osRef_osORTICurrentServiceId
   ldrb   a3, [a1]
   str    a3, [sp, #-4]!
   mov    a3,#osdORTINoService
   strb   a3, [a1]
 .endif

 .endif


   ; !!! a2 must still contain value of osIntNestingDepth !!!

_CallISR
   mov   lr,pc
   bx    r8

_LeaveISR
 .if (osdStackCheck == 1)
   ldr    a1, osRef_SystemStack
   ldr    a2,[a1]
   ldr    a3, osRef_AAAAAAAA;
   cmp    a2, a3
   blne   _osSystemStackOverflow
 .endif


   mrs    a4, CPSR
   orr    a4, a4, #0x80        ; disable interrupt
   msr    CPSR, a4
   

 .if (osdORTIDebug == 1)
 .if (osdORTIVersion == 200)
   ldr    a1, [sp], #+4
   ldr    a2, osRef_osORTICurrentServiceId
   strb   a1, [a2]
 .endif

 .endif


   ; decrement osIntNestingDepth
   ldr    a1, osRef_osIntNestingDepth
   ldrb   a2,[a1]
   subs   a2,a2,#1             ; set Z if first interrupt nesting level
   strb   a2,[a1]

   ldr    lr, [sp], #+4

   ; restore sp (only for first nesting level)
   ldreq  sp, [sp]

   ; switch to irq mode
   bic    a4, a4, #0x1F
   orr    a4, a4, #0x92
   msr    CPSR, a4

   ; decrement osLockDispatcher
 .if (!osdNonPreempt)
   ldr    r0, osRef_osLockDispatcher
   ldrb   r1,[r0]
   subs   r1,r1,#1
   strb   r1,[r0]
 .endif


 .if (!osdNonPreempt)
   ; return from IRQ if dispatcher locked
   bne    _LeaveISR_normal
   ldr    r0, osRef_osStartDispatcher
   ldrb   r1,[r0]
   teq    r1,#0
   ; return from IRQ if dispatcher not started
   beq    _LeaveISR_normal

   ; jump to dispatcher
   ; copy r0-r2,lr to supervisor stack
   mov    ip, sp
   add    ip, ip, #20
   ldr    r9,osRef_SupervisorStackTop
   ldmfd  ip,{r0-r2,r8}
   stmfd  r9,{r0-r2,r8}

   ldr    r0, [sp], #+4        ; r0 = SPSRirq

   ; switch to SVC mode
   orr    r1, r3, #0x93
   ldmfd  sp!,{r3,r8,r9,ip}
   add    sp, sp, #16
   msr    CPSR, r1

   msr    SPSR, r0
   sub    sp,sp,#16
   ldr    lr, [sp], #+4        ; restore LR
   sub    lr, lr,#4            ; convert LRirq to LRsvc

   ldr    r0, osRef_osDispatcher
   mov    pc,r0
 .endif


_LeaveISR_normal
   ; normal return from IRQ
   ldr    r0, [sp], #+4        ; r0 = SPSRirq
   msr    SPSR, r0

   ldmfd  sp!,{r3,r8,r9,ip,lr}
   ldmfd  sp!,{r0-r2}
   subs   pc,lr,#4             ; ENDOF ISRcat2 handler

   .ref _osUndefInstruction
 .if (osdTasksInUserMode==0)
   .ref _osDirectSWIDispatcher
 .else
   .ref _osSWI
 .endif

   .ref _osPrefetchAbort
   .ref _osDataAbort
   .ref _osFIQ
   .ref _Can0Interrupt
   .ref _UART1_TxISR
   .ref _UART1_RxISR
   .ref _UART2_TxISR
   .ref _UART2_RxISR
   .ref _UART_ErrorISR
   .ref _osTimerInt


_Can0Interrupt_cat23
   stmfd  sp!,{r3,r8,r9,ip,lr}
   bl _osEnterCallLeaveISR
   .word _Can0Interrupt


_UART1_TxISR_cat1
   ldmfd  sp!,{r0-r2}
   b _UART1_TxISR

_UART1_RxISR_cat1
   ldmfd  sp!,{r0-r2}
   b _UART1_RxISR

_UART2_TxISR_cat1
   ldmfd  sp!,{r0-r2}
   b _UART2_TxISR

_UART2_RxISR_cat1
   ldmfd  sp!,{r0-r2}
   b _UART2_RxISR

_UART_ErrorISR_cat1
   ldmfd  sp!,{r0-r2}
   b _UART_ErrorISR

_osTimerInt_cat23
   stmfd  sp!,{r3,r8,r9,ip,lr}
   bl _osEnterCallLeaveISR
   .word _osTimerInt





_osDefVect
 .if (!$$defined(osdSuppressDefVectWarning))
   stmfd  sp!,{r3,r8,r9,ip,lr}
   bl _osEnterCallLeaveISR
   .word _osDefVectWarning

 .else
   ldmfd  sp!,{r0-r2}
   subs   pc,lr,#4
 .endif



   .ref _osUnhandledExceptionWrapper
   .sect ".osvtable"
   .global     osIRQTable
osIRQTable     ; interrupt vector table
   .word _osDefVect
   .word _osUnhandledExceptionWrapper     ; ISN 0 (SPI1)
   .word _osUnhandledExceptionWrapper     ; ISN 1 (COMP2)
   .word _osTimerInt_cat23     ; ISN 2 (COMP1)
   .word _osUnhandledExceptionWrapper     ; ISN 3 (TAP)
   .word _osUnhandledExceptionWrapper     ; ISN 4 (SPI2)
   .word _osUnhandledExceptionWrapper     ; ISN 5 (GIOA)
   .word _osUnhandledExceptionWrapper     ; ISN 6 (HWAG_H)
   .word _osUnhandledExceptionWrapper     ; ISN 7 (HET1)
   .word _osUnhandledExceptionWrapper     ; ISN 8 (EXT8)
   .word _UART_ErrorISR_cat1     ; ISN 9 (SCIERR)
   .word _UART1_RxISR_cat1     ; ISN 10 (SCIRX)
   .word _osUnhandledExceptionWrapper     ; ISN 11 (C2SI)
   .word _osUnhandledExceptionWrapper     ; ISN 12 (EXT12)
   .word _Can0Interrupt_cat23     ; ISN 13 (HECCA)
   .word _osUnhandledExceptionWrapper     ; ISN 14 (SCCA)
   .word _osUnhandledExceptionWrapper     ; ISN 15 (EXT15)
   .word _osUnhandledExceptionWrapper     ; ISN 16 (ADCEEC)
   .word _UART2_RxISR_cat1     ; ISN 17 (EXT17)
   .word _osUnhandledExceptionWrapper     ; ISN 18 (DMAA)
   .word _osUnhandledExceptionWrapper     ; ISN 19 (EXT19)
   .word _UART1_TxISR_cat1     ; ISN 20 (SCITX)
   .word _osUnhandledExceptionWrapper     ; ISN 21 (SSI)
   .word _osUnhandledExceptionWrapper     ; ISN 22 (HWAG_L)
   .word _osUnhandledExceptionWrapper     ; ISN 23 (HET2)
   .word _osUnhandledExceptionWrapper     ; ISN 24 (HECCB)
   .word _osUnhandledExceptionWrapper     ; ISN 25 (SCCB)
   .word _UART2_TxISR_cat1     ; ISN 26 (EXT26)
   .word _osUnhandledExceptionWrapper     ; ISN 27 (ADCEG1C)
   .word _osUnhandledExceptionWrapper     ; ISN 28 (DMAB)
   .word _osUnhandledExceptionWrapper     ; ISN 29 (GIOB)
   .word _osUnhandledExceptionWrapper     ; ISN 30 (ADCEG2C)
   .word _osUnhandledExceptionWrapper     ; ISN 31 (EXT31)


osIRQIVEC          .word 0xFFFFff20
osCnstIntTable     .word osIRQTable

_osIRQHandler
   stmfd  sp!,{r0-r2}          ; save scratch register
   ldr    r1, osIRQIVEC        ; r1 = IRQIVEC
   ldr    r0, osCnstIntTable   ; vector table base
   ldr    r2, [r1]             ; r2 = IRQ Index
   and    r2, r2, #0xff        
   ldr    pc, [r0, r2, lsl#2]  ; jump to interrupt handler

   .sect ".osarmvect"
osARMVectTable
    b _osUndefInstruction                    ; Undefined Instruction
 .if (osdTasksInUserMode==0)
    b _osDirectSWIDispatcher                       ; SWI
 .else
    b _osSWI                       ; SWI
 .endif

    b _osPrefetchAbort             ; Prefetch Abort
    b _osDataAbort                 ; Data Abort
    .word 0                        ; reserved
    b _osIRQHandler                ; IRQ
    b _osFIQ                       ; FIQ


   .end


; END OF C:\project_Parrot_software\SYS_Plattform_workbench\UHV_B1_V002\MOD\MOD_OSEK_DYN\src\intvect.asm

