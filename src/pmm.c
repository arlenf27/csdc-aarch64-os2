#include <stdint.h>
#include "uart.h"

#define PMM_PAGE_SIZE 4096

extern uint8_t __pmm_start;

uintptr_t pmm_start_address;
uint64_t pmm_max_size;
uint64_t pmm_page_count;

void pmm_init(uint64_t physical_usable_memory_end_address){
	pmm_start_address = (uintptr_t) &__pmm_start;
	pmm_max_size = physical_usable_memory_end_address - (uint64_t) pmm_start_address;

	/* TODO: Below is the start of a bitmap approach, but perhaps we might want to switch to a free list algorithm instead? 
	   A free list is basically a linked list that only contains free pages, so finding a free page is O(1), but of course
	   memory overhead is larger. If we do decide to do the bitmap approach first, can someone verify that this math below
	   is correct? I'm doing this at 3 am and I can't think straight lol */

	/* pmm_max_size * 8 is total bits available, 4096 * 8 is size in bits per page, we add one to that to account for an
	   additional bit needed for the bitmap that specifies whether each page is free or not */
	pmm_page_count = (pmm_max_size * 8) / (4096 * 8 + 1);
	/* Round size of bitmap up */
	uint64_t pmm_bitmap_size = (pmm_page_count + 7) / 8;

	/* Initialize bitmap */
	uint8_t* bitmap_it = (uint8_t*) pmm_start_address;
	uint64_t bitmap_bytes_to_initialize_to_free = pmm_page_count / 8;
	uint64_t i;
	for(i = 0; i < bitmap_bytes_to_initialize_to_free; i++){
		*bitmap_it = 0;
		bitmap_it++;
	}
	uint8_t remaining_bits_to_initialize_to_free = pmm_page_count - (bitmap_bytes_to_initialize_to_free * 8);
	if(bitmap_bytes_to_initialize_to_free < pmm_bitmap_size){
		*bitmap_it = 0xFF;
		(*bitmap_it) <<= remaining_bits_to_initialize_to_free;
	}

	/* TODO: TEMPORARY debug prints for math verification, delete later! */
	uart_puts("PMM start address: ");
	uart_putd(pmm_start_address);
	uart_puts("\nPMM max size: ");
	uart_putd(pmm_max_size);
	uart_puts("\nPMM page count: ");
	uart_putd(pmm_page_count);
	uart_puts("\nPMM bitmap size: ");
	uart_putd(pmm_bitmap_size);
	uart_puts("\nPMM whole bytes to initialize to free: ");
	uart_putd(bitmap_bytes_to_initialize_to_free);
	uart_puts("\nPMM remaining bits to initialize to free: ");
	uart_putd(remaining_bits_to_initialize_to_free);
	uart_puts("\n");
}

uint64_t pmm_malloc(){
	/* TODO: depends on what algorithm we use */
}

uint8_t pmm_free(uint64_t address){
	/* TODO: depends on what algorithm we use */
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
