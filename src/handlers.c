#include <stdint.h>
#include "uart.h"

void handler(uint64_t esr) {
	uint8_t ec = esr >> 26;

	switch (ec) {
		case 0x15:
			uart_puts("SVC INSTRUCTION");
			break;
		default:
			uart_puts("idk bro");
	}

  uart_puts("\n");
}