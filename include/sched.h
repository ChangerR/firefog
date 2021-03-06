#ifndef __FIRE_SCHED
#define __FIRE_SCHED
#include "firedef.h"
#include "signal.h"
//...由于作者能力有限 这里大部分是从linux0.11中copy下来的
// 这里定义了进程运行可能处的状态。
#define TASK_RUNNING 0		
#define TASK_INTERRUPTIBLE 1	
#define TASK_UNINTERRUPTIBLE 2	
#define TASK_ZOMBIE 3		
#define TASK_STOPPED 4	


#define NR_TASKS 64
#define HZ 100			

#define FIRST_TASK task[0]	
#define LAST_TASK task[NR_TASKS-1]	


extern void sched_init (void);

typedef struct i387_struct
{
  long cwd;			// 控制字(Control word)。
  long swd;			// 状态字(Status word)。
  long twd;			// 标记字(Tag word)。
  long fip;			// 协处理器代码指针。
  long fcs;			// 协处理器代码段寄存器。
  long foo;
  long fos;
  long st_space[20];		/* 8*10 bytes for each FP-reg = 80 bytes */
}fpu_unit;

// 任务状态段数据结构。TSS描述段
typedef struct tss_struct
{
  long back_link;		/* 16 high bits zero */
  long esp0;
  long ss0;			/* 16 high bits zero */
  long esp1;
  long ss1;			/* 16 high bits zero */
  long esp2;
  long ss2;			/* 16 high bits zero */
  long cr3;
  long eip;
  long eflags;
  long eax, ecx, edx, ebx;
  long esp;
  long ebp;
  long esi;
  long edi;
  long es;			/* 16 high bits zero */
  long cs;			/* 16 high bits zero */
  long ss;			/* 16 high bits zero */
  long ds;			/* 16 high bits zero */
  long fs;			/* 16 high bits zero */
  long gs;			/* 16 high bits zero */
  long ldt;			/* 16 high bits zero */
  long trace_bitmap;		/* bits: trace 0, bitmap 16-31 */
  fpu_unit i387;
}_tss_t;


// 这里是任务（进程）数据结构，或称为进程描述符。
// ==========================
// long state 任务的运行状态（-1 不可运行，0 可运行(就绪)，>0 已停止）。
// long counter 任务运行时间计数(递减)（滴答数），运行时间片。
// long priority 运行优先数。任务开始运行时counter = priority，越大运行越长。
// long signal 信号。是位图，每个比特位代表一种信号，信号值=位偏移值+1。
// struct sigaction sigaction[32] 信号执行属性结构，对应信号将要执行的操作和标志信息。
// long blocked 进程信号屏蔽码（对应信号位图）。
// --------------------------
// int exit_code 任务执行停止的退出码，其父进程会取。
// unsigned long start_code 代码段地址。
// unsigned long end_code 代码长度（字节数）。
// unsigned long end_data 代码长度 + 数据长度（字节数）。
// unsigned long brk 总长度（字节数）。
// unsigned long start_stack 堆栈段地址。
// long pid 进程标识号(进程号)。
// long father 父进程号。
// long pgrp 父进程组号。
// long session 会话号。
// long leader 会话首领。
// unsigned short uid 用户标识号（用户id）。
// unsigned short euid 有效用户id。
// unsigned short suid 保存的用户id。
// unsigned short gid 组标识号（组id）。
// unsigned short egid 有效组id。
// unsigned short sgid 保存的组id。
// long alarm 报警定时值（滴答数）。
// long utime 用户态运行时间（滴答数）。
// long stime 系统态运行时间（滴答数）。
// long cutime 子进程用户态运行时间。
// long cstime 子进程系统态运行时间。
// long start_time 进程开始运行时刻。
// unsigned short used_math 标志：是否使用了协处理器。
// --------------------------
// int tty 进程使用tty 的子设备号。-1 表示没有使用。
// unsigned short umask 文件创建属性屏蔽位。
// struct m_inode * pwd 当前工作目录i 节点结构。
// struct m_inode * root 根目录i 节点结构。
// struct m_inode * executable 执行文件i 节点结构。
// unsigned long close_on_exec 执行时关闭文件句柄位图标志。（参见include/fcntl.h）
// struct file * filp[NR_OPEN] 进程使用的文件表结构。
// --------------------------
// struct desc_struct ldt[3] 本任务的局部表描述符。0-空，1-代码段cs，2-数据和堆栈段ds&ss。
// --------------------------
// struct tss_struct tss 本进程的任务状态段信息结构。
// ==========================
typedef struct task_struct
{
  long state;			/* -1 unrunnable, 0 runnable, >0 stopped */
  long counter;
  long priority;
  long signal;
  struct sigaction sigaction[32];
  long blocked;			/* bitmap of masked signals */
  int exit_code;
  unsigned long start_code, end_code, end_data, brk, start_stack;
  long pid, father, pgrp, session, leader;
  unsigned short uid, euid, suid;
  unsigned short gid, egid, sgid;
  long alarm;
  long utime, stime, cutime, cstime, start_time;
  unsigned short used_math;
  //int tty;			/* -1 if no tty, so it must be signed */
  //unsigned short umask;
 // struct m_inode *pwd;
  //struct m_inode *root;
 // struct m_inode *executable;
 // unsigned long close_on_exec;
 // struct file *filp[NR_OPEN];
/* ldt for this task 0 - zero 1 - cs 2 - ds&ss */
  qword ldt[3];
/* tss for this task */
  struct tss_struct tss;
}task_t;

void switch_to(int n);
extern struct task_struct *task[NR_TASKS];	// 任务数组。
extern struct task_struct *last_task_used_math;	// 上一个使用过协处理器的进程。
extern struct task_struct *current;	// 当前进程结构指针变量。
extern long volatile jiffies;	// 从开机开始算起的滴答数（10ms/滴答）。
extern long startup_time;	// 开机时间。从1970:0:0:0 开始计时的秒数。

#define CURRENT_TIME (startup_time+jiffies/HZ)	// 当前时间（秒数）


/*
* 寻找第1 个TSS 在全局表中的入口。0-没有用nul，1-代码段cs，2-数据段ds，3-系统段syscall
* 4-任务状态段TSS0，5-局部表LTD0，6-任务状态段TSS1，等。
*/
// 全局表中第1 个任务状态段(TSS)描述符的选择符索引号。
#define FIRST_TSS_ENTRY 4
// 全局表中第1 个局部描述符表(LDT)描述符的选择符索引号。
#define FIRST_LDT_ENTRY (FIRST_TSS_ENTRY+1)
// 宏定义，计算在全局表中第n 个任务的TSS 描述符的索引号（选择符）。
#define _TSS(n) ((((unsigned long) n)<<4)+(FIRST_TSS_ENTRY<<3))
// 宏定义，计算在全局表中第n 个任务的LDT 描述符的索引号。
#define _LDT(n) ((((unsigned long) n)<<4)+(FIRST_LDT_ENTRY<<3))
// 宏定义，加载第n 个任务的任务寄存器tr。
#define ltr(n) {\
		__asm push ecx \
		__asm push eax \
		__asm xor ecx,ecx \
		__asm mov ch,n \
		__asm lea eax,[ecx+(FIRST_TSS_ENTRY<<3)] \
		__asm ltr ax \
		__asm pop eax \
		__asm pop ecx \
		}
		
#define lldt(n) {\
		__asm push ecx \
		__asm push eax \
		__asm xor ecx,ecx \
		__asm mov ch,n \
		__asm lea eax,[ecx+(FIRST_LDT_ENTRY<<3)] \
		__asm lldt ax \
		__asm pop eax \
		__asm pop ecx \
		}


#endif