#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include <stdbool.h>

uint64_t read_sys_count();
uint64_t read_sys_frq();
uint64_t read_ctl();

void enable_timer();
void disable_timer();
void set_timer(uint32_t s);
bool check_timer();

#endif
