#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include <stdbool.h>

#define TIMER_PPI_IID_OFFSET 0x10

uint64_t timer_read_sys_count();
uint64_t timer_read_sys_frq();
uint64_t timer_read_ctl();

void timer_enable();
void timer_disable();
void timer_set_duration(uint32_t s);
bool timer_is_finished();

#endif
