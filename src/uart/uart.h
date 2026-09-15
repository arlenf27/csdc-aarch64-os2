#ifndef UART_H
#define UART_H

#include <stdint.h>

void uart_init(uint64_t base);

void uart_putc(char c);

void uart_puts(const char* s); 

void uart_putu(uint64_t u);

void uart_putd(int64_t d);

#endif
