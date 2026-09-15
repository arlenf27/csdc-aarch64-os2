#include <stdint.h>
#include <stddef.h>
#include "uart.h"

#define UART_DR_OFFSET 0x00
#define UART_FR_OFFSET 0x18
#define UART_FR_TXFF (1u << 5)

#define MAX_DIGITS_UINT64 20

static uintptr_t uart_base;

void uart_init(uint64_t base){
	uart_base = (uintptr_t) base;
}

void uart_putc(char c){
	volatile uint32_t* fr = (volatile uint32_t*) (uart_base + UART_FR_OFFSET);
	volatile uint32_t* dr = (volatile uint32_t*) (uart_base + UART_DR_OFFSET);
	/* wait while TX FIFO full */ 
	while (*fr & UART_FR_TXFF){
		__asm__ volatile("nop"); 
	} 
	*dr = (uint32_t) c; 
}

void uart_puts(const char* s){ 
	for (size_t i = 0; s[i] != '\0'; i++){
		uart_putc(s[i]);
	}	
}

void uart_putu(uint64_t u) {
	char buf[MAX_DIGITS_UINT64];
	int i = 0;

	do {
    buf[i++] = (u % 10) + '0';
    u /= 10;
  } while (u > 0);

	while (i > 0) {
		uart_putc(buf[--i]);
	}
}

void uart_putd(int64_t d){
	uint64_t abs_val;
	if(d < 0){
		uart_putc('-');
		abs_val = (uint64_t) (-(d+1)) + 1;
	}else{
		abs_val = (uint64_t) d;
	}

	uart_putu(abs_val);
}

