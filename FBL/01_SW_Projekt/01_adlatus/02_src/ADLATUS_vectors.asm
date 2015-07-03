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
	B _ISR_Undef   ; UNDEFINED INSTRUCTION INTERRUPT
	B _SwiDisp2    ; SOFTWARE INTERRUPT
	B _ISR_Pre	   ; ABORT (PREFETCH) INTERRUPT
	B _ISR_DA	   ; ABORT (DATA) INTERRUPT
;	B #-8	       ; RESERVED
	B _ISR_RESEVED ; RESERVED
	B _IrqDisp2    ; IRQ INTERRUPT
	B _FiqDisp2    ; FIQ INTERRUPT
    B #-8

	.end

;/**************************** END of boot_vectors.asm ***********************/