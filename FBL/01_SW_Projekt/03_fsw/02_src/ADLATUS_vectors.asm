;/****************************************************************************/
;/* boot_vectors.asm	 TMS470	INTERRUPT VECTORS SETUP	FILE		         */
;/****************************************************************************/
    
	.state32
    .global  _c_int00
    .global  _ISR_Undef
    .global _IrqDisp2
    .global _FiqDisp2
    .global _SwiDisp2
    .global  _ISR_Pre
    .global  _ISR_DA
    .global  _ISR_RESEVED
    
    .sect ".vectors"
	
;/****************************************************************************/
;/*                                                                          */
;/* Jumptable to the jumptable located in the FSW section                    */
;/*                                                                          */
;/****************************************************************************/

	B _c_int00        ; RESET INTERRUPT
_ISR_Undef:
	B _ISR_Undef   ; UNDEFINED INSTRUCTION INTERRUPT
_SwiDisp2:
	B _SwiDisp2    ; SOFTWARE INTERRUPT
_ISR_Pre:
	B _ISR_Pre	   ; ABORT (PREFETCH) INTERRUPT
_ISR_DA:
	B _ISR_DA	   ; ABORT (DATA) INTERRUPT
;	B #-8	       ; RESERVED
_ISR_RESEVED:
	B _ISR_RESEVED ; RESERVED
_IrqDisp2:
	B _IrqDisp2    ; IRQ INTERRUPT
_FiqDisp2:
	B _FiqDisp2    ; FIQ INTERRUPT
    B #-8

	.end

;/**************************** END of boot_vectors.asm ***********************/