#include <stdint.h>
#include <stddef.h>
#include "uart.h"

#define PMM_PAGE_SIZE 4096

extern uint8_t __pmm_start;

uintptr_t pmm_start_address;
uint64_t pmm_max_size;
uint64_t pmm_page_count;
uint8_t* freelist;

void pmm_init(uint64_t physical_usable_memory_end_address){
	/* Align to page size */
	pmm_start_address = (((uintptr_t) &__pmm_start) + (PMM_PAGE_SIZE - 1)) & ~(PMM_PAGE_SIZE - 1);
	pmm_max_size = physical_usable_memory_end_address - (uint64_t) pmm_start_address;
	pmm_page_count = pmm_max_size / PMM_PAGE_SIZE;

	freelist = (uint8_t*) pmm_start_address;
	uint64_t* first_node = (uint64_t*) freelist;
	*first_node = (uintptr_t) 0;
	uint8_t* freelist_it = freelist;

	/* TODO: TEMPORARY debug prints, remove later */
	uart_puts("PMM start address: ");
	uart_putu(pmm_start_address);
	uart_puts("\nPMM freelist node 0 address: ");
	uart_putu(*first_node);
	uart_puts("\nPMM max size: ");
	uart_putu(pmm_max_size);
	uart_puts("\nPMM page count: ");
	uart_putu(pmm_page_count);
	uart_puts("\n");

	/* All pages are free to start, so every PMM_PAGE_SIZE bytes, the first 64 bits of each free page denotes
	   address of next free page. At init, every page is free, so there's PMM_PAGE_SIZE bytes between every
	   free list "node" */
	for(int i = 1; i < pmm_page_count; i++){
		uint64_t* current_node = (uint64_t*) freelist_it;
		freelist_it += PMM_PAGE_SIZE;
		*current_node = (uintptr_t) freelist_it;
		uint64_t* next_node = (uint64_t*) (((uint8_t*) pmm_start_address) + (i * PMM_PAGE_SIZE));
		*next_node = (uintptr_t) 0;

		/* TODO: TEMPORARY debug prints, remove later */
		uart_puts("PMM freelist node ");
		uart_putu(i);
		uart_puts(" address: ");
		uart_putu(*current_node);
		uart_puts("\n");
	}
}

uint64_t pmm_malloc(){
	/* TODO */
}

uint8_t pmm_free(uint64_t address){
	/* TODO */
}

uint64_t pmm_get_start_address(){
	return (uint64_t) pmm_start_address;
}

uint64_t pmm_get_max_size(){
	return pmm_max_size;
}

uint64_t pmm_get_total_page_count(){
	return pmm_page_count;
}
