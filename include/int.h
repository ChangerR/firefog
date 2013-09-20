#ifndef _FIRE_INTERRUPT
#define _FIRE_INTERRUPT
void divide_error(void);
void debug(void);
void nmi(void);
void int3(void);
void overflow(void);
void bounds(void);
void invalid_op(void);
void coprocessor_segment_overrun(void);
void reserved(void);
void double_fault(void);
void invalid_TSS(void);
void segment_not_present(void);
void stack_segment(void);
void general_protection(void);

void coprocessor_error(void);
void device_not_available(void);

void system_call(void);
void timer_interrupt(void);

void trip_int(void);
#endif