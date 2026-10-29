#include <stdint.h>

void pmm_init(uint64_t physical_usable_memory_end_address);

uint64_t pmm_malloc();

uint8_t pmm_free(uint64_t address);
