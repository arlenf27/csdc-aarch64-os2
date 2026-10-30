/* kernel.c for Aarch64 */ 

#include <stdint.h> 
#include <stddef.h>
#include "dtb.h"
#include "uart.h"
#include "gic.h"
#include "timer.h"
#include "utils.h"
#include "machine_specific_dtb_bindings.h"
#include "pmm.h"

/* TODO: Move these macros to machine_specific_dtb_bindings.h */
#define QEMU_VIRT_UART_DTB_DEVICE_NAME "arm,pl011"
#define QEMU_VIRT_GIC_DTB_DEVICE_NAME "arm,cortex-a15-gic"

/* Global .data test */
char global_data_test = 'e';

/** Special function to read exception level */
static unsigned int read_el(){
	unsigned long el; /* CurrentEL is a 64-bit register */
	/* Reads Aarch64 register CurrentEL and then places it into the C variable el */
	__asm__ volatile("mrs %0, CurrentEL" : "=r"(el));
	/* CurrentEL register in Aarch64 is encoded as EL<<2 so shift right */
	return (unsigned int) ((el >> 2) & 0x3);
}

/* Exported symbol called by boot.S */ 
void kernel_main(void* dtb){
	dtb_regs uart_regs;
	if(dtb_correct_magic((const void*) dtb) && find_device_regs(dtb, QEMU_VIRT_UART_DTB_DEVICE_NAME, NULL, &uart_regs) && uart_regs.count > 0){
		uart_init(uart_regs.reg_regions[QEMU_VIRT_V8_UART_DTB_REG_PROP_ENTRY_MAIN_BASE_ADDRESS_INDEX].address);
		uart_puts("Hello from AArch64 bare-metal kernel!\n");
		uart_puts("DTB magic number is correct. \n");
		unsigned int el = read_el();
		uart_puts("Current exception level is: ");
		uart_putc('0' + (el & 0xF));
		uart_puts("\n");
		uart_puts("Global .data test: ");
		uart_putc(global_data_test);
		uart_puts("\n");
		uart_puts("QEMU Virt UART base registers address: ");
		uart_putd(uart_regs.reg_regions[QEMU_VIRT_V8_UART_DTB_REG_PROP_ENTRY_MAIN_BASE_ADDRESS_INDEX].address);
		uart_puts("\n");
		dtb_regs memory_regs;
		if(find_device_regs(dtb, NULL, "memory", &memory_regs)){
			uart_puts("Memory address: ");
			/* TODO: Add a macro for the array index and stop hardcoding this in machine_specific_dtb_bindings.h */
			uart_putd(memory_regs.reg_regions[0].address);
			uart_puts("\nMemory size: ");
			uart_putd(memory_regs.reg_regions[0].size);
			uart_puts("\n");
		}
		dtb_regs gic_regs;
		if(find_device_regs(dtb, QEMU_VIRT_GIC_DTB_DEVICE_NAME, NULL, &gic_regs) && gic_regs.count > 1){
			uart_puts("QEMU Virt GIC base registers address index 0 (GICD): ");
			uart_putd(gic_regs.reg_regions[QEMU_VIRT_V8_GICV2_DTB_REG_PROP_ENTRY_GICD_BASE_ADDRESS_INDEX].address);
			uart_puts("\n");
			uart_puts("QEMU Virt GIC base registers address index 1 (GICC): ");
			uart_putd(gic_regs.reg_regions[QEMU_VIRT_V8_GICV2_DTB_REG_PROP_ENTRY_GICC_BASE_ADDRESS_INDEX].address);
			uart_puts("\n");
		}else{
			uart_puts(QEMU_VIRT_GIC_DTB_DEVICE_NAME);
			uart_puts(" not found or unexpected DTB registers setup. \n");
		}
		dtb_interrupts timer_int;
		/* TODO: Create macro for the target_compat name for the timer and put it in machine_specific_dtb_bindings.h */
		if(find_device_interrupts(dtb, "arm,armv7-timer", &timer_int)){
			uart_puts("armv7-timer DTB interrupt results: ");
			uart_putd(timer_int.interrupt_parent);
			uart_puts(", ");
			uart_putd(timer_int.interrupt_cells);
			uart_puts(", ");
			uart_putd(timer_int.interrupt_count);
			for(int i = 0; i < timer_int.interrupt_cells * timer_int.interrupt_count; i++){
				uart_puts(", ");
				uart_putd(be32tole32(timer_int.interrupt_specifiers[i]));
			}
			uart_puts("\n");
		}
		dtb_interrupts uart_int;
		if(find_device_interrupts(dtb, QEMU_VIRT_UART_DTB_DEVICE_NAME, &uart_int)){
			uart_puts("UART DTB interrupt results: ");
			uart_putd(uart_int.interrupt_parent);
			uart_puts(", ");
			uart_putd(uart_int.interrupt_cells);
			uart_puts(", ");
			uart_putd(uart_int.interrupt_count);
			for(int i = 0; i < uart_int.interrupt_cells * uart_int.interrupt_count; i++){
				uart_puts(", ");
				uart_putd(be32tole32(uart_int.interrupt_specifiers[i]));
			}
			uart_puts("\n");
		}
		uart_puts("TEMP test for synchronous exception\n");
		__asm__ volatile("svc #0");
		uart_puts("Start the GIC\n");
		GIC_init(dtb, gic_regs.reg_regions[QEMU_VIRT_V8_GICV2_DTB_REG_PROP_ENTRY_GICD_BASE_ADDRESS_INDEX].address, 
				gic_regs.reg_regions[QEMU_VIRT_V8_GICV2_DTB_REG_PROP_ENTRY_GICC_BASE_ADDRESS_INDEX].address);
		uart_puts("Timer interrupt starting now!\n");
		timer_set_duration(1);

		pmm_init(memory_regs.reg_regions[0].address + memory_regs.reg_regions[0].size);
		void* allocated_page_address = pmm_malloc();
		if(allocated_page_address != NULL){
			uart_puts("Allocated PMM page at: ");
			uart_putu((uint64_t) allocated_page_address);
			uart_puts("\n");
			void* another_allocated_page_address = pmm_malloc();
			if(another_allocated_page_address != NULL){
				uart_puts("Allocated another PMM page at: ");
				uart_putu((uint64_t) another_allocated_page_address);
				uart_puts("\n");
				pmm_free(allocated_page_address);
				uart_puts("Attempted a PMM free on the first allocated PMM page. \n");
				void* a_third_allocated_page_address = pmm_malloc();
				if(a_third_allocated_page_address != NULL){
					uart_puts("Allocated a third PMM page at: ");
					uart_putu((uint64_t) a_third_allocated_page_address);
					uart_puts("\n");
				}
			}
		}
	}
	/* simple idle: wait for interrupts (low power) */ 
	while(1){
		__asm__ volatile("wfe");
	}

}
