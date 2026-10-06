#include <stdint.h>
#include "uart.h"
#include "timer.h"

// Hard coded for now
#define GICD_BASE 0x8000000UL

#define GICD_CTLR (*(volatile uint32_t *) (GICD_BASE + 0x0000))
#define GICD_ISENABLER(n) (*(volatile uint32_t *) (GICD_BASE + 0x0100 + 0x20 * n))
#define GICD_SGIR (*(volatile uint32_t *) (GICD_BASE + 0x0F00))

// Hard coded for now
#define GICC_BASE 0x8010000UL

#define GICC_CTLR (*(volatile uint32_t *) (GICC_BASE + 0x0000))
#define GICC_PMR (*(volatile uint32_t *) (GICC_BASE + 0x0004))
#define GICC_IAR (*(volatile uint32_t *) (GICC_BASE + 0x000C))
#define GICC_EOIR (*(volatile uint32_t *) (GICC_BASE + 0x0010))


void GIC_init() {
	// Enable Timer
  GICD_ISENABLER(0) = 1u << 30;

	// Start GIC
	GICD_CTLR = 0x01;
	GICC_CTLR = 0x01;

	// Allow all priorities
	GICC_PMR = 0xFF;
	
	// Clear interrupt mask
	__asm__ volatile("msr daifclr, #2");
}

void irq_handler() {
	uint32_t iar = GICC_IAR;
	uint16_t IID = iar & 0x03FF;
	
	if (IID == 30) {
		uart_puts("Timer went off!");
		disable_timer();
	} else {
		uart_puts("Something went off!");
	}

	GICC_EOIR = iar;
}