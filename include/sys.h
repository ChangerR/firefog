#ifndef __FIRE_SYS
#define __FIRE_SYS
typedef int (*fn_ptr)(void);	// 定义函数指针类型。

extern int sys_exit ();		// 程序退出。
extern int sys_fork ();		// 创建进程。
extern int sys_time ();		// 取当前时间。 
extern int sys_pause ();	// 暂停进程运行。
extern int sys_kill ();		// 终止一个进程。

extern fn_ptr sys_call_table[];

#endif