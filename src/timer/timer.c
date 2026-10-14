#include <stdint.h>
#include <stdbool.h>

uint64_t timer_read_sys_count() {
  uint64_t sys_count;
  __asm__ volatile("mrs %0, CNTPCT_EL0" : "=r"(sys_count));

  return sys_count;
}

uint64_t timer_read_sys_frq() {
  uint64_t sys_frq;
  __asm__ volatile("mrs %0, CNTFRQ_EL0" : "=r"(sys_frq));

  return sys_frq;
}

uint64_t timer_read_ctl() {
  uint64_t ctl;
  __asm__ volatile("mrs %0, CNTP_CTL_EL0" : "=r"(ctl));
  
  return ctl;
}

void timer_enable() {
  uint64_t ctl = 0b1;
  __asm__ volatile("msr CNTP_CTL_EL0, %0\nisb" :: "r"(ctl));
}

void timer_disable() {
  uint64_t ctl = 0b0;
  __asm__ volatile("msr CNTP_CTL_EL0, %0\nisb" :: "r"(ctl));
}

void timer_set_duration(uint32_t s) {
  uint32_t tval = s * timer_read_sys_frq();
  __asm__ volatile("msr CNTP_TVAL_EL0, %0" :: "r"(tval));
  timer_enable();
}

bool timer_is_finished() {
  uint64_t cval;
  __asm__ volatile("mrs %0, CNTP_CVAL_EL0":"=r"(cval));

  return cval <= timer_read_sys_count();
}
