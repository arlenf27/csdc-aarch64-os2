/* linker.ld - simple memory layout for QEMU virt */ 

ENTRY(_start)

/* Define ELF segments */
PHDRS{
	read_seg PT_LOAD FLAGS(4); /* read(4) */
	read_execute_seg PT_LOAD FLAGS(5); /* 5 = read (4) + execute (1) */
	read_write_seg PT_LOAD FLAGS(6); /* 6 = read(4) + write(2) */
}

SECTIONS{ 
	
	/* QEMU RAM base: 0x40000000, will reserve for DTB and leave 2 MiB */
	. = 0x40000000;
	__dtb_start = .;
	
	. = 0x40200000;

	/* 16 entries * 128 bytes = 2048 as defined by Aarch64 */
	.vectors : ALIGN(2048) { *(.vectors) *(.vectors.*) } : read_execute_seg
	
	.text : ALIGN(16) { *(.text) *(.text.*) } : read_execute_seg

	.rodata : ALIGN(16) { *(.rodata) *(.rodata.*) } : read_seg

	.data : ALIGN(16) { *(.data) *(.data.*) } : read_write_seg

	. = ALIGN(16);
	__bss_start = .; 
	
	.bss (NOLOAD) : ALIGN(16) { *(.bss) *(.bss.*) *(COMMON) } : read_write_seg
	
	. = ALIGN(16);
	__bss_end = .;

	/* Reserve stack space at top of image */  
	
	. = ALIGN(16);  
	
	stack_top = . + 0x20000; /* 128 KiB stack */ 

	. = stack_top; 
	
	__pmm_start = .;
}
