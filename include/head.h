#ifndef __FIRE_HEAD
#define __FIRE_HEAD
#include "firedef.h"

extern qword idt[256];
extern qword gdt[256];
extern DWORD pg_dir[1024];
extern sys_argu_t sys_argu;
extern long plidt;
#endif