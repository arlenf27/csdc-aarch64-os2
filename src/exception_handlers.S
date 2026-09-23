/* exception_handlers.S - Aarch64 Exception Handlers */

/* TODO - NO handlers are functional or currently used! */

/* Everything below is placed in .text */
.section .text

/* Align 16 bytes in accordance with Aarch64 requirements */
.set STACK_SAVED_REGISTERS_SIZE, 272

.macro save_processor_state
    sub sp, sp, #STACK_SAVED_REGISTERS_SIZE

	stp x0, x1, [sp]
	stp x2, x3, [sp, #16]
	stp x4, x5, [sp, #32]
	stp x6, x7, [sp, #48]
	stp x8, x9, [sp, #64]
	stp x10, x11, [sp, #80]
	stp x12, x13, [sp, #96]
	stp x14, x15, [sp, #112]
	stp x16, x17, [sp, #128]
	stp x18, x19, [sp, #144]
	stp x20, x21, [sp, #160]
	stp x22, x23, [sp, #176]
	stp x24, x25, [sp, #192]
	stp x26, x27, [sp, #208]
	stp x28, x29, [sp, #224]

	/* calculate original stack ptr */
	add x21, sp, #STACK_SAVED_REGISTERS_SIZE
	stp x30, x21, [sp, #240]

	/* save ELR and SPSR at offset 256 */
	mrs x22, ELR_EL1
	mrs x23, SPSR_EL1

	stp x22, x23, [sp, #256]
.endm

.macro load_processor_state
    /* restore special purpose registers first */
	ldp x22, x23, [sp, #256]
	msr ELR_EL1, x22
    msr SPSR_EL1, x23

	/* restore link register */
	ldr x30, [sp, #240]

    ldp x0, x1, [sp]
    ldp x2, x3, [sp, #16]
    ldp x4, x5, [sp, #32]
    ldp x6, x7, [sp, #48]
    ldp x8, x9, [sp, #64]
    ldp x10, x11, [sp, #80]
    ldp x12, x13, [sp, #96]
    ldp x14, x15, [sp, #112]
    ldp x16, x17, [sp, #128]
    ldp x18, x19, [sp, #144]
    ldp x20, x21, [sp, #160]
    ldp x22, x23, [sp, #176]
    ldp x24, x25, [sp, #192]
    ldp x26, x27, [sp, #208]
    ldp x28, x29, [sp, #224]

    add sp, sp, #STACK_SAVED_REGISTERS_SIZE
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

