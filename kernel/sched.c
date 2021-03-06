#include "sched.h"
#include "memory.h"
#include "system.h"
#include "head.h"
#include "int.h"

#define LATCH (1193180/HZ)
#define _S(nr) (1<<((nr)-1)
#define _BLOCKABLE (~(_S(SIGKILL)|_S(SIGSTOP)))
long plidt = (long)idt;
union task_union {
	task_t task;
	char stack[PAGE_SIZE];
	};
	
union task_union init_task0;

long volatile jiffies = 0;
long startup_time = 0;

task_t* current = &(init_task0.task);
task_t* task[NR_TASKS] = {0,};

task_t* last_task_used_math = NULL;

long user_stack[1024];

struct {
	long* a;
	short b;
	} stack_start = {&user_stack[1024],0x10};


static void Init_Task0(task_t* t)
{
	memset(t,0,sizeof(task_t));
	t->counter = 15;
	t->priority = 15;
	t->father = -1;
	t->ldt[1] = 54318073435390111ll;
	t->ldt[2] = 54309277342367903ll;

	t->tss.esp0 = PAGE_SIZE+(long)&init_task0;
	t->tss.ss0 = 0x10l;
	t->tss.cr3 = 0;
	t->tss.ss = 0x17;
	t->tss.cs = 0x17;
	t->tss.ds = 0x17;
	t->tss.es = 0x17;
	t->tss.fs = 0x17;
	t->tss.gs = 0x17;
	t->tss.ldt = _LDT(0);
	t->tss.trace_bitmap = 0x80000000;
}
/*
 * 作者觉得这段代码移植的太尼玛烂了.....
*/
void switch_to(int n)
{
	task_t* ptask;
	DWORD tss_des;
	ptask = task[n];
	tss_des = _TSS(n);
	__asm{
		mov ecx,ptask
		cmp ecx,current
		je endasm
		mov edx,tss_des
		xchg ecx,current
		mov es,dx
		jmp DWORD PTR es:[0x0]
		cmp ecx,last_task_used_math
		jne endasm
		clts
	}
endasm:
	return;
}
void schedule(void)
{
	int i,next,c;
	task_t** p;
	long mask;
	for(p = &LAST_TASK;p > &FIRST_TASK; --p)
	{
		if(*p)
		{
			if((*p)->alarm&&(*p)->alarm <jiffies)
			{
				(*p)->signal |= (1 << (SIGALRM-1));
				(*p)->alarm = 0;
			}
			mask = 1<<(SIGKILL-1);
			mask |= 1<<(SIGSTOP-1);
			mask &= (*p)->blocked;
			mask = ~mask;
			mask &= (*p)->signal;
			if(mask&&(*p)->state == TASK_INTERRUPTIBLE)
			{
				(*p)->state = TASK_RUNNING;
			}
		}
	}
	
	while(1)
	{
		c = -1;
		next = 0;
		i = NR_TASKS;
		p = &task[NR_TASKS];
		while(--i)
		{
			if(!*--p)
				continue;
			if((*p)->state == TASK_RUNNING&&(*p)->counter >c)
			{
				c = (*p)->counter;
				next = i;
			}
		}
		if(c)
			break;
		for(p = &LAST_TASK;p > &FIRST_TASK; --p)
		{
			if(*p)
			{
				(*p)->counter = ((*p)->counter >> 1) +(*p)->priority;
			}
		}	
	}
	switch_to(next);
}

void do_timer(long cpl)
{
	if(cpl)
	{
		current->utime++;
	}
	else
	{
		current->stime++;
	}
	current->counter--;
	if(current->counter > 0)
		return;
		
	current->counter = 0;
	if(!cpl)
		return;
		
	schedule();
}

void sched_init()
{
	int i;
	qword* p;
	long addr;
	Init_Task0(current);
	task[0] = current;
	
	addr = (long)&(init_task0.task.tss);
	set_tss_desc(gdt+FIRST_TSS_ENTRY,addr);
	addr = (long)&(init_task0.task.ldt[0]);
	set_ldt_desc(gdt+FIRST_LDT_ENTRY,addr);
	
	p = gdt + FIRST_TSS_ENTRY +2;
	for(i = 1;i<NR_TASKS;i++)
	{
		task[i] = NULL;
		*p++ = 0ll;
		*p++ = 0ll;
	}
	__asm pushf
	__asm and DWORD PTR[esp],0xffffbfff
	__asm popf
	
	ltr(0);
	ldt(0);
	
	outb_p(0x36,0x43);
	outb_p(LATCH&0xff,0x40);
	outb_p(LATCH>>8,0x40);
	
	set_intr_gate(0x20,timer_interrupt);
	outb_p(inb_p(0x21)&0xFE,0x21);
	set_system_gate(0x80,system_call);
}

































