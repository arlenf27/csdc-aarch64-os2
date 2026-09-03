/* boot.S - Aarch64 entry */ 

.section .text 
.global _start 
.type _start STT_FUNC

/* Start function */
_start: 
	/* Save DTB pointer */
	ldr x20, =__dtb_start

	/* Load address of stack_top (from linker script) into x0 and set SP */ 
	ldr x0, =stack_top 
	mov sp, x0

	/* Zero .bss: fill 8 bytes at a time from __bss_start to __bss_end */
	/* x1 = address of bss start */
	ldr    x1, =__bss_start
	/* x2 = address of bss end */
	ldr    x2, =__bss_end

step_1:	
	/* Compare x1 and x2 */
	cmp x1, x2
	/* Branch to step 2 if x1 > x2 */  
	b.ge check_el 
	/* Store 0 (xzr) at [x1], then x1 += 8 */
	str xzr, [x1], #8
	/* Go back to start of step 1 */   
	b step_1

check_el:
	/* Exception level 1 is kernel mode where most OS kernel runs normally, so EL1's where we'll try to go
	   We will try to switch into EL1 starting from here */
	/* Detect current exception level */
	mrs x3, CurrentEL
	/* CurrentEL is encoded, we shift right by 2 to get actual exception level 0-3 */
	lsr x3, x3, #2
	/* Now, x3 is EL number, we first check if it's EL2, and try to move to EL1 if it is */
	cmp x3, #2
	beq el2_to_el1
	/* Check to see if we're in EL3 */
	cmp x3, #3
	beq el3_to_el1
	/* Check to see if we're in EL1 already */
	cmp x3, #1
	beq in_el1
	/* EL0 or unknown: hang — cannot safely elevate from EL0 */
	b _hang

el2_to_el1:
	/* Set SP_EL1 to stack_top so EL1 has a valid stack after ERET */
	ldr x0, =stack_top
	/* SP_EL1 is a special register, so msr is used to move data here */
	msr sp_el1, x0
	/* Set return address to in_el1 (ELR_EL2) */
	adr x1, in_el1
	msr elr_el2, x1
	/* Set SPSR_EL2 (saved PSTATE for ERET, controls target mode and interrupt mask): 
		- mask interrupts (DAIF) and select EL1h on return
		- 0x3C5 commonly used: DAIF masked + M = 0101 (EL1h) 
	   Typically works at least for QEMU virt testing */
	ldr x2, =0x3c5
	msr spsr_el2, x2
	/* Return to EL1, starting at kernel_main */
	eret

el3_to_el1:
	/* Set SP_EL1 to stack_top so EL1 has a valid stack after ERET */
	ldr x0, =stack_top
	/* SP_EL1 is a special register, so msr is used to move data here */
	msr sp_el1, x0
	/* Set return address to in_el1 (ELR_EL3) */
	adr x1, in_el1
	msr elr_el3, x1
	/* Set SPSR_EL3 (saved PSTATE for ERET, controls target mode and interrupt mask): 
		- mask interrupts (DAIF) and select EL1h on return
		- 0x3C5 commonly used: DAIF masked + M = 0101 (EL1h)
	   Typically works at least for QEMU virt testing */
	ldr x2, =0x3c5
	msr spsr_el3, x2
	/* Return to EL1, starting at kernel_main */
	eret

in_el1:
	/* Move DTB pointer to x0 */
	mov x0, x20
	/* Call C kernel entry */
	bl kernel_main		
 	
/* If kernel_main returns, loop forever */
_hang: 
	wfe 
	b _hang
