#include "signal.h"
#include "string.h"

void do_signal(long signr,long eax,long ebx,long ecx
			,long edx,long fs,long es,long ds,long eip,
			long cs,long eflags,unsigned long* eip,long ss)
{		
	printk("signal %d",signr);			
}