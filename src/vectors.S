/* vectors.S - Aarch64 vector tables */

/* "ax" explicitly means allocatable and executable, putting this here because
   I am using .vectors and not .text */
.section .vectors, "ax"
.global exception_vectors
.type exception_vectors STT_FUNC

exception_vectors:
	/* Current EL with SP_EL0 */
	b synchronous_handler_sp0
	.balign 128
	b irq_handler_sp0
	.balign 128
	b fiq_handler_sp0
	.balign 128
	b serror_handler_sp0
	/* Current EL with SP_ELX */
	.balign 128
	b synchronous_handler_spx
	.balign 128
	b irq_handler_spx
	.balign 128
	b fiq_handler_spx
	.balign 128
	b serror_handler_spx
	/* Lower EL, and at least one lower EL is Aarch64 */
	.balign 128
	b synchronous_handler_64
	.balign 128
	b irq_handler_64
	.balign 128
	b fiq_handler_64
	.balign 128
	b serror_handler_64
	/* Lower EL, all lower ELs are Aarch32 */
	.balign 128
	b synchronous_handler_32
	.balign 128
	b irq_handler_32
	.balign 128
	b fiq_handler_32
	.balign 128
	b serror_handler_32

