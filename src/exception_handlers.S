/* exception_handlers.S - Aarch64 Exception Handlers */

/* TODO - NO handlers are functional or currently used! */

/* Everything below is placed in .text */
.section .text

.macro save_processor_state

	stp     x0,  x1,  [sp, #-16]!
	stp     x2,  x3,  [sp, #-16]!
	stp     x4,  x5,  [sp, #-16]!
	stp     x6,  x7,  [sp, #-16]!
	stp     x8,  x9,  [sp, #-16]!
	stp     x10, x11, [sp, #-16]!
	stp     x12, x13, [sp, #-16]!
	stp     x14, x15, [sp, #-16]!
	stp     x16, x17, [sp, #-16]!
	stp     x18, x19, [sp, #-16]!
	stp     x20, x21, [sp, #-16]!
	stp     x22, x23, [sp, #-16]!
	stp     x24, x25, [sp, #-16]!
	stp     x26, x27, [sp, #-16]!
	stp     x28, x29, [sp, #-16]!
	str     x30,      [sp, #-16]!  

	mrs 		x0, ELR_EL1
	mrs 		x1, SPSR_EL1
	stp 		x0,	x1, 	[sp, #-16]!
	
.endm

.macro load_processor_state

	ldp 		x0, x1,   [sp], #16
	msr 		SPSR_EL1, x1
	msr     ELR_EL1,  x0

	ldr			x30, 			[sp], #16
	ldp			x28, x29, [sp], #16
	ldp     x26, x27, [sp], #16
	ldp     x24, x25, [sp], #16
	ldp     x22, x23, [sp], #16
	ldp     x20, x21, [sp], #16
	ldp     x18, x19, [sp], #16
	ldp     x16, x17, [sp], #16
	ldp     x14, x15, [sp], #16
	ldp     x12, x13, [sp], #16
	ldp     x10, x11, [sp], #16
	ldp     x8,  x9,  [sp], #16
	ldp     x6,  x7,  [sp], #16
	ldp     x4,  x5,  [sp], #16
	ldp     x2,  x3,  [sp], #16
	ldp     x0,  x1,  [sp], #16

.endm

.global synchronous_handler_sp0
.type synchronous_handler_sp0 STT_FUNC
synchronous_handler_sp0:
	/* TODO */
	eret

.global irq_handler_sp0
.type irq_handler_sp0 STT_FUNC
irq_handler_sp0:
	/* TODO */
	eret

.global fiq_handler_sp0
.type fiq_handler_sp0 STT_FUNC
fiq_handler_sp0:
	/* TODO */
	eret

.global serror_handler_sp0
.type serror_handler_sp0 STT_FUNC
serror_handler_sp0:
	/* TODO */
	eret

.global synchronous_handler_spx
.type synchronous_handler_spx STT_FUNC
synchronous_handler_spx:
	save_processor_state
	
	mrs 		x0, ESR_EL1
	bl 			handler

	load_processor_state
	eret

.global irq_handler_spx
.type irq_handler_spx STT_FUNC
irq_handler_spx:
	/* TODO */
	eret

.global fiq_handler_spx
.type fiq_handler_spx STT_FUNC
fiq_handler_spx:
	/* TODO */
	eret

.global serror_handler_spx
.type serror_handler_spx STT_FUNC
serror_handler_spx:
	/* TODO */
	eret

.global synchronous_handler_64
.type synchronous_handler_64 STT_FUNC
synchronous_handler_64:
	/* TODO */
	eret

.global irq_handler_64
.type irq_handler_64 STT_FUNC
irq_handler_64:
	/* TODO */
	eret

.global fiq_handler_64
.type fiq_handler_64 STT_FUNC
fiq_handler_64:
	/* TODO */
	eret

.global serror_handler_64
.type serror_handler_64 STT_FUNC
serror_handler_64:
	/* TODO */
	eret

.global synchronous_handler_32
.type synchronous_handler_32 STT_FUNC
synchronous_handler_32:
	/* TODO */
	eret

.global irq_handler_32
.type irq_handler_32 STT_FUNC
irq_handler_32:
	/* TODO */
	eret

.global fiq_handler_32
.type fiq_handler_32 STT_FUNC
fiq_handler_32:
	/* TODO */
	eret

.global serror_handler_32
.type serror_handler_32 STT_FUNC
serror_handler_32:
	/* TODO */
	eret

