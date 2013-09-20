#ifndef __FIREFOG_FIRE
#define __FIREFOG_FIRE
typedef unsigned char pixel_t;
typedef unsigned char BYTE;
typedef unsigned int u_int;
typedef unsigned long u_long;
typedef unsigned short u_short;
typedef u_short WORD;
typedef u_long DWORD;
typedef unsigned int size_t;
typedef long long qword;
typedef enum qboolean
{ 
	qfalse,qtrue
}boolean;
typedef struct sys_argument
{
	pixel_t xpos,ypos;
	WORD  ext_mem;
	WORD display_page;
	WORD video_mode;
	WORD cols;
	WORD video_memsize;
	WORD video_state;
	WORD video_argus;
}sys_argu_t;

#ifndef NULL
#define NULL ((void*)0)
#endif
#define PAGE_SIZE 0x1000

#endif