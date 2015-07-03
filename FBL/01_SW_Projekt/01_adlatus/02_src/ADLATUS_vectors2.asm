;/****************************************************************************/
;/* boot_vectors2.asm	 TMS470	INTERRUPT VECTORS SETUP	FILE		         */
;/****************************************************************************/

  .state32

  .global  _FUN_CCmain_Main_V     ; RESET INTERRUPT
  .global  _ISR_Undef
  .global  _IrqDisp2
  .global  _ISR_Pre
  .global  _ISR_DA
  .global  _SwiDisp2
  .global  _IrqDisp2
  .global  _FiqDisp2
  .global  _ISR_UndefinedInstr_V ; UNDEFINED INSTRUCTION INTERRUPT
  .global  _swiHandler	         ; SOFTWARE INTERRUPT
  .global  _ISR_PrefetchAbort_V  ; ABORT (PREFETCH) INTERRUPT
  .global  _ISR_DataAbort_V      ; ABORT (DATA) INTERRUPT
  .global  _externIntHandler	 ; IRQ INTERRUPT
  .global  _ISR_FIQHandler_V     ; FIQ INTERRUPT
  .global  _ISR_RESEVED       ; RESERVED
 

	.sect ".intvecs"

;/****************************************************************************/
;/*                                                                          */
;/* The 2nd Jumptable is located in the FSW section and guides an interrupt  */
;/* to the final address                                                     */
;/*                                                                          */
;/****************************************************************************/
	
  B  _FUN_CCmain_Main_V        ; RESET INTERRUPT
_ISR_Undef:
;  B  _ISR_UndefinedInstr_V   ; UNDEFINED INSTRUCTION INTERRUPT
  B  _FUN_CCmain_Main_V        ; RESET INTERRUPT

_SwiDisp2:
;  B  _swiHandler	         ; SOFTWARE INTERRUPT
  B  _FUN_CCmain_Main_V        ; RESET INTERRUPT

_ISR_Pre:  
;  B  _ISR_PrefetchAbort_V    ; ABORT (PREFETCH) INTERRUPT
  B  _FUN_CCmain_Main_V        ; RESET INTERRUPT

_ISR_DA:
;  B  _ISR_DataAbort_V        ; ABORT (DATA) INTERRUPT
;  B  #-8				     ; RESERVED
  B  _FUN_CCmain_Main_V        ; RESET INTERRUPT

_ISR_RESEVED:
;  B  _ISR_DataAbort_V        ; ABORT (DATA) INTERRUPT
;  B  #-8				     ; RESERVED
  B  _FUN_CCmain_Main_V        ; RESET INTERRUPT

_IrqDisp2:
;  B  _externIntHandler	     ; IRQ INTERRUPT
  B  _FUN_CCmain_Main_V        ; RESET INTERRUPT

_FiqDisp2:
;  B  _ISR_FIQHandler_V       ; FIQ INTERRUPT
;  B  #-8				     ; RESERVED
  B  _FUN_CCmain_Main_V        ; RESET INTERRUPT
	
  .end

;/**************************** END of boot_vectors2.asm **********************/

