#include <stdint.h>
#include "uart.h"
#include "timer.h"
#include "machine_specific_dtb_bindings.h"
#include "dtb.h"
#include "utils.h"

#define GICD_CTLR_OFFSET 0x0000
#define GICD_ISENABLER_OFFSET(N) (0x0100 + 0x20 * (N))
#define GICD_SGIR_OFFSET 0x0F00
#define GICC_CTLR_OFFSET 0x0000
#define GICC_PMR_OFFSET 0x0004
#define GICC_IAR_OFFSET 0x000C
#define GICC_EOIR_OFFSET 0x0010

static uintptr_t gicd_base;
static uintptr_t gicc_base;
static uint16_t el1_ns_timer_interrupt_id;

static uint16_t timer_triggered_count;

void GIC_init(void* dtb, uint64_t gicd_base_address, uint64_t gicc_base_address) {
	gicd_base = gicd_base_address;
	gicc_base = gicc_base_address;

	volatile uint32_t* gicd_iseenabler = (volatile uint32_t*) (gicd_base + GICD_ISENABLER_OFFSET(0));
	volatile uint32_t* gicd_ctlr = (volatile uint32_t*) (gicd_base + GICD_CTLR_OFFSET);
	volatile uint32_t* gicc_ctlr = (volatile uint32_t*) (gicc_base + GICC_CTLR_OFFSET);
	volatile uint32_t* gicc_pmr = (volatile uint32_t*) (gicc_base + GICC_PMR_OFFSET);

	/* Enable Timer */
	dtb_interrupts timer_int;
	if(find_device_interrupts(dtb, "arm,armv7-timer", &timer_int)){
		el1_ns_timer_interrupt_id = TIMER_PPI_IID_OFFSET + be32tole32(timer_int.interrupt_specifiers[
			QEMU_VIRT_V8_TIMER_DTB_INTERRUPTS_PROP_ENTRY_NS_EL1_INTERRUPTS_INDEX * timer_int.interrupt_cells + 
			QEMU_VIRT_V8_TIMER_DTB_INTERRUPTS_CELL_INTERRUPT_LOCAL_ID]);
	}

	*gicd_iseenabler = 1U << el1_ns_timer_interrupt_id;

	/* Start GIC */
	*gicd_ctlr = 0x01;
	*gicc_ctlr = 0x01;

	/* Allow all priorities */
	*gicc_pmr = 0xFF;
	
	/* Clear interrupt mask */
	__asm__ volatile("msr daifclr, #2");
}

void irq_handler() {
	volatile uint32_t* gicc_iar = (volatile uint32_t*) (gicc_base + GICC_IAR_OFFSET);

	uint32_t iar_snapshot = *gicc_iar;

	/* Lower 10 bits (0x03FF) is for the interrupt ID field of GICC_IAR */
	uint16_t interrupt_id = iar_snapshot & 0x03FF;
	
	if (interrupt_id == el1_ns_timer_interrupt_id) {
		uart_puts("Timer went off! Count: ");
		uart_putu(timer_triggered_count);
		uart_puts("\n");
		timer_triggered_count++;
		if(timer_triggered_count >= 5){
			uart_puts("Disabling timer interrupt...\n");
			timer_triggered_count = 0;
			timer_disable();
		}else{
			timer_set_duration(1);
		}
	} else {
		uart_puts("Something (uncaccounted for currently) went off!\n");
	}

	volatile uint32_t* gicc_eoir = (volatile uint32_t*) (gicc_base + GICC_EOIR_OFFSET);

	*gicc_eoir = iar_snapshot;
}
