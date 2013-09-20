#include "int.h"
#include "system.h"
#include "firedef.h"
#include "head.h"
void die(const char* str,long esp,long error_code);

static void die(const char* str,long esp,long error_code)
{
	printk("%s esp:%x error_code:%d",str,esp,error_code);
}

void do_divide_error(long esp,long error_code)
{
	die("diivide_error",esp,error_code);
}
void do_debug(long esp,long error_code)
{
	die("debug_error",esp,error_code);
}
void do_nmi(long esp,long error_code)
{
	die("nmi_error",esp,error_code);
}
void do_int3(long esp,long error_code)
{
	die("int3_error",esp,error_code);
}
void do_overflow(long esp,long error_code)
{
	die("overflow_error",esp,error_code);
}
void do_bounds(long esp,long error_code)
{
	die("bounds_error",esp,error_code);
}
void do_invalid_op(long esp,long error_code)
{
	die("invalid_op_error",esp,error_code);
}
void do_coprocessor_segment_overrun(long esp,long error_code)
{
	die("coprocessor_segment_overrun_error",esp,error_code);
}
void do_reserved(long esp,long error_code)
{
	die("reserved_error",esp,error_code);
}
void do_double_fault(long esp,long error_code)
{
	die("double_fault_error",esp,error_code);
}
void do_invalid_TSS(long esp,long error_code)
{
	die("invalid_TSS_error",esp,error_code);
}
void do_segment_not_present(long esp,long error_code)
{
	die("segment_not_present_error",esp,error_code);
}
void do_stack_segment(long esp,long error_code)
{
	die("stack_segment_error",esp,error_code);
}
void do_general_protection(long esp,long error_code)
{
	die("general_protection_error",esp,error_code);
}

void trip_int(void)
{
	int i;
	
	set_trap_gate(0,divide_error);
	set_trap_gate(1,debug);
	set_trap_gate(2,nmi);
	set_system_gate(3,int3);
	set_system_gate(4,overflow);
	set_system_gate(5,bounds);
	set_trap_gate(6,invalid_op);
	set_trap_gate(7,device_not_available);
	set_trap_gate(8,double_fault);
	set_trap_gate(9,coprocessor_segment_overrun);
	set_trap_gate(10,invalid_TSS);
	set_trap_gate(11,segment_not_present);
	set_trap_gate(12,stack_segment);
	set_trap_gate(13,general_protection);
	set_trap_gate(14,coprocessor_error);
	set_trap_gate(15,reserved);
	set_trap_gate(16,coprocessor_error);
	for(i = 17;i<48;i++)
		set_trap_gate(i,reserved);
}