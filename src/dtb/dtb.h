#ifndef DTB_H
#define DTB_H

#include <stdint.h>

#define DTB_MAGIC_LITTLE_ENDIAN 0xEDFE0DD0
#define FDT_BEGIN_NODE 0x00000001
#define FDT_END_NODE 0x00000002
#define FDT_PROP 0x00000003
#define FDT_NOP 0x00000004
#define FDT_END 0x00000009

#define DTB_REG_MAX_REGIONS 16

typedef struct dtb_header{
	uint32_t magic;
	uint32_t total_size;
	uint32_t off_dt_struct;
	uint32_t off_dt_strings;
	uint32_t off_mem_rsvmap;
	uint32_t version;
	uint32_t last_comp_version;
	uint32_t boot_cpuid_phys;
	uint32_t size_dt_strings;
	uint32_t size_dt_struct;
} dtb_header;

typedef struct dtb_node_state{
	uint32_t address_cells;
	uint32_t size_cells;
	const uint8_t* ranges_data;
	uint32_t ranges_len;
	uint8_t ranges_present;
	uint8_t compatible_match;
	const void* reg_data;
	uint32_t reg_len;
} dtb_node_state;

typedef struct dtb_reg{
	uint64_t address;
	uint64_t size;
} dtb_reg;

typedef struct dtb_regs{
	/* TODO: should probably just make this a raw pointer instead of statically allocating space for DTB_REG_MAX_REGIONS in the future */
	dtb_reg reg_regions[DTB_REG_MAX_REGIONS];
	uint32_t count;
} dtb_regs;

typedef struct dtb_interrupts{
	uint32_t interrupt_parent;
	uint32_t interrupt_cells;
	uint32_t interrupt_count;
	const uint32_t* interrupt_specifiers;
} dtb_interrupts;

typedef struct dtb_interrupt_node_state{
	uint32_t interrupt_parent;
	uint8_t interrupt_parent_present;
	const void* interrupts_data;
	uint32_t interrupts_len;
	uint32_t compatible_match;
} dtb_interrupt_node_state;

uint8_t dtb_correct_magic(const void* raw_dtb);

/**
  * Finds the "reg' property information for the specified target_compat or target_device_type. 
  * This function will find the first compatible device and instance of "reg" that satisfy either target_compat
  * or target_device_type. target_compat targets the "compatible" property and target_device_type targets the "device_type" property.  
  * NOTE: IT IS RECOMMENDED TO ONLY USE ONE OF EITHER TARGET_COMPAT OR TARGET_DEVICE_TYPE AT A TIME AND LEAVE THE OTHER STRING NULL!!!
  **/
uint8_t find_device_regs(const void* raw_dtb, const char* target_compat, const char* target_device_type, dtb_regs* regs);

uint8_t find_device_interrupts(const void* raw_dtb, const char* target_compat, dtb_interrupts* result);

#endif
