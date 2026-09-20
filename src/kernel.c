/* kernel.c for Aarch64 */ 

#include <stdint.h> 
#include <stddef.h>
#include "dtb.h"
#include "uart.h"

#define QEMU_VIRT_UART_DTB_DEVICE_NAME "arm,pl011"

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
	uint64_t uart_pl011_ram_address;
	if(dtb_correct_magic((const void*) dtb) && find_device_reg(dtb, QEMU_VIRT_UART_DTB_DEVICE_NAME, &uart_pl011_ram_address)){
		uart_init(uart_pl011_ram_address);
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
		uart_putd(uart_pl011_ram_address);
		uart_puts("\n");
		uart_puts("wfe...no interrupts set up yet... \n");
		
		__asm__ volatile("svc     #0");

	}
	/* simple idle: wait for interrupts (low power) */ 
	while(1){
		__asm__ volatile("wfe");
	}

}
