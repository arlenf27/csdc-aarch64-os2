#include <stdint.h>

#define BITMAP_PAGE_FREE 0
#define BITMAP_PAGE_NOT_FREE 1

void pmm_init(uint64_t physical_usable_memory_end_address);

uint64_t pmm_malloc();

uint8_t pmm_free(uint64_t address);

uint64_t pmm_get_start_address();

uint64_t pmm_get_max_size();

uint64_t pmm_get_total_page_count();
