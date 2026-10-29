#include <stdint.h>
#include "uart.h"

#define PMM_PAGE_SIZE 4096

extern uint8_t __pmm_start;

uintptr_t pmm_start_address;
uint64_t pmm_max_size;

void pmm_init(uint64_t physical_usable_memory_end_address){
	pmm_start_address = (uintptr_t) &__pmm_start;
	pmm_max_size = physical_usable_memory_end_address - pmm_start_address;

	/* TEMPORARY debug prints */
	uart_puts("PMM start address: ");
	uart_putd(pmm_start_address);
	uart_puts("\nPMM max size: ");
	uart_putd(pmm_max_size);
	uart_puts("\n");

	/* TODO: Finish rest of init! pmm_max_size is theoretically the space we have available for PMM. 
	   I'm thinking of possibly a bitmap reserved at the start of
	   PMM memory section where each bit indicates if that page corresponding to that page index
	   if free or allocated? But feel free to try better ideas :) */
}

uint64_t pmm_malloc(){

}

uint8_t pmm_free(uint64_t address){

}
