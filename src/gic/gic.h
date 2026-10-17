#ifndef GIC_H
#define GIC_H

#include <stdint.h>

void GIC_init(void* dtb, uint64_t gicd_base_address, uint64_t gicc_base_address);
void irq_handler();

#endif
