/* NOTE: This file is used for any custom machine-specific DTB information that is specified by
   the hardware/virtual machine itself and not by Aarch64 or the official device tree specifications. 
   For instance, the order of multiple entries of properties like "interrupts" and what they point
   to is specified by this file. Since this file is for the QEMU Aarch64 virt-v8.0 machine, all 
   information is gained from their codebase, specifically: 
   https://gitlab.com/qemu-project/qemu/-/blob/stable-8.0/hw/arm/virt.c?ref_type=heads
   Specific examples of functions where this information was gained from include fdt_add_timer_nodes
   and fdt_add_gic_node in hw/arm/virt.c in QEMU's codebase. */

/*** "reg" property entries - these specify the indexes of the resulting array that specifies what each 
  entry corresponds to where each entry is a dtb_reg instance when using find_device_regs ***/

/* PL011 UART - only one "reg" entry */
#define QEMU_VIRT_V8_UART_DTB_REG_PROP_ENTRY_MAIN_BASE_ADDRESS_INDEX 0

/* GICv2 - two "reg" entries */
#define QEMU_VIRT_V8_GICV2_DTB_REG_PROP_ENTRY_GICD_BASE_ADDRESS_INDEX 0
#define QEMU_VIRT_V8_GICV2_DTB_REG_PROP_ENTRY_GICC_BASE_ADDRESS_INDEX 1

/*** "interrupts" property entries - these specify the indexes of the resulting array that specifies what 
  each entry corresponds to where each entry is a dtb_interrupts instance when using find_device_interrupts ***/

/* PL011 UART - only one "interrupts" entry */
#define QEMU_VIRT_V8_UART_DTB_INTERRUPTS_PROP_ENTRY_MAIN_INTERRUPTS_INDEX 0

/* armv7,v8 timer - four "interrupts" entries */
#define QEMU_VIRT_V8_TIMER_DTB_INTERRUPTS_PROP_ENTRY_S_EL1_INTERRUPTS_INDEX 0
#define QEMU_VIRT_V8_TIMER_DTB_INTERRUPTS_PROP_ENTRY_NS_EL1_INTERRUPTS_INDEX 1
#define QEMU_VIRT_V8_TIMER_DTB_INTERRUPTS_PROP_ENTRY_VIRT_INTERRUPTS_INDEX 2
#define QEMU_VIRT_V8_TIMER_DTB_INTERRUPTS_PROP_ENTRY_NS_EL2_INTERRUPTS_INDEX 3

/* armv7,v8 timer - 3 cells per interrupt entry */
#define QEMU_VIRT_V8_TIMER_DTB_INTERRUPTS_CELL_INTERRUPT_TYPE 0
#define QEMU_VIRT_V8_TIMER_DTB_INTERRUPTS_CELL_INTERRUPT_LOCAL_ID 1
#define QEMU_VIRT_V8_TIMER_DTB_INTERRUPTS_CELL_INTERRUPT_FLAGS 2

/*** Timers available for armv7,v8 timer for QEMU virt v8.0 ***/
typedef enum qemu_virt_v8_timers{
	qemu_virt_v8_timer_s_el1 = 0,
	qemu_virt_v8_timer_ns_el1 = 1,
	qemu_virt_v8_timer_virt = 2,
	qemu_virt_v8_timer_ns_el2 = 3
} qemu_virt_v8_timers;

