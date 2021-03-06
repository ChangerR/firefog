#include "firedef.h"
#include "memory.h"
#include "time.h"
#include "string.h"

sys_argu_t sys_argu;
char printbuf[1024];
static long memory_end = 0;	// 机器具有的内存（字节数）。
static long buffer_memory_end = 0;	// 高速缓冲区末端地址。
static long main_memory_start = 0;	// 主内存（将用于分页）开始的位置。
#define BCD_TO_BIN(val) ((val) = (val)&15 + ((val)>>4)*10)
void InitSysArgu(sys_argu_t* p);


__declspec( naked ) int cmos_read(int addr)
{
	__asm{
		mov al,BYTE PTR[esp+4]
		out 0x70,al
		in  al,0x71
		ret
	}
}
void _time_init (void)	// 该子程序取CMOS 时钟，并设置开机时间??startup_time(秒)。
{
     
	struct tm time;
	time.tm_sec = cmos_read (0);
   do{
	   time.tm_sec = cmos_read (0);	// 参见后面CMOS 内存列表。
	   time.tm_min = cmos_read (2);
	   time.tm_hour = cmos_read (4);
	   time.tm_mday = cmos_read (7);
	   time.tm_mon = cmos_read (8);
	   time.tm_year = cmos_read (9);
	}while (time.tm_sec != cmos_read (0));
	
   BCD_TO_BIN (time.tm_sec);
   BCD_TO_BIN (time.tm_min);
   BCD_TO_BIN (time.tm_hour);
   BCD_TO_BIN (time.tm_mday);
   BCD_TO_BIN (time.tm_mon);
   BCD_TO_BIN (time.tm_year);
   time.tm_mon--;
   startup_time = kernel_mktime (&time);
   
}
void printk(const char* string,...)
{
	static int rows = 0;
	short* buf;
	int index;
	char* str;
	va_list args;
	va_start(args,string);
	vsprintf(printbuf,string,args);
	va_end(args);
	str = printbuf;
	if(rows == 0)
	{
		memset((void*)0xb8000,0,50*sys_argu.cols);
		rows = 0;
	}
checkrows:
	if(rows >25)
	{
		memcpy((void*)0xb8000,(void*)(0xb8000+sys_argu.cols*2),48*sys_argu.cols);
		rows--;
	}
	buf = (short*)(0xb8000+sys_argu.cols*rows*2);
	index = 0;
	while(*str)
	{
		if(*str=='\n'||index>=sys_argu.cols)
		{
			buf+=sys_argu.cols;
			rows++;
			++str;
			index = 0;
			goto checkrows;
		}
		buf[index] = *str+0x700;
		++index;
		++str;
	}
	
}
void main(void)
{
	char* videob;
	InitSysArgu(&sys_argu);
	
	memory_end = (1 << 20) + (sys_argu.ext_mem << 10);	// 内存大小=1Mb 字节+扩展内存(k)*1024 字节。
	memory_end &= 0xfffff000;	// 忽略不到4Kb（1 页）的内存数。
	if (memory_end > 16 * 1024 * 1024)	// 如果内存超过16Mb，则按16Mb 计。
		memory_end = 16 * 1024 * 1024;
	if (memory_end > 12 * 1024 * 1024)	// 如果内存>12Mb，则设置缓冲区末端=4Mb
		buffer_memory_end = 4 * 1024 * 1024;
	else if (memory_end > 6 * 1024 * 1024)	// 否则如果内存>6Mb，则设置缓冲区末端=2Mb
			buffer_memory_end = 2 * 1024 * 1024;
	else
		buffer_memory_end = 1 * 1024 * 1024;	// 否则则设置缓冲区末端=1Mb
	main_memory_start = buffer_memory_end;	// 主内存起始位置=缓冲区末端；
	
	
	_time_init();
	printk("Hello!!\nHello World");
	printk("ext_mem:%x\n",(int)(u_long)sys_argu.ext_mem);
endloop:
	__asm HLT
	goto endloop;
}
void InitSysArgu(sys_argu_t* p)
{
	p->xpos = *(pixel_t*)0x90000;
	p->ypos = *(pixel_t*)0x90001;
	p->ext_mem = *(WORD*)0x90002;
	p->display_page = *(pixel_t*)0x90005;
	p->video_mode = *(pixel_t*)0x90006;
	p->cols = *(pixel_t*)0x90007;
	p->video_memsize=*(pixel_t*)0x9000a;
	p->video_state = *(pixel_t*)0x9000b;
	p->video_argus = *(WORD*)0x9000c;
}

#define MINUTE 60		// 1 分钟的秒数。
#define HOUR (60*MINUTE)	// 1 小时的秒数。
#define DAY (24*HOUR)		// 1 天的秒数。
#define YEAR (365*DAY)		// 1 年的秒数。

/* interestingly, we assume leap-years */
/* 有趣的是我们考虑进了闰年 */
// 下面以年为界限，定义了每个月开始时的秒数时间数组。
static int month[12] = {
  0,
  DAY * (31),
  DAY * (31 + 29),
  DAY * (31 + 29 + 31),
  DAY * (31 + 29 + 31 + 30),
  DAY * (31 + 29 + 31 + 30 + 31),
  DAY * (31 + 29 + 31 + 30 + 31 + 30),
  DAY * (31 + 29 + 31 + 30 + 31 + 30 + 31),
  DAY * (31 + 29 + 31 + 30 + 31 + 30 + 31 + 31),
  DAY * (31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30),
  DAY * (31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31),
  DAY * (31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30)
};

// 该函数计算从1970 年1 月1 日0 时起到开机当日经过的秒数，作为开机时间。

long
kernel_mktime (struct tm *tm)
{
  long res;
  int year;

  year = tm->tm_year - 70;	// 从70 年到现在经过的年数(2 位表示方式)，
  if(year < 0)
	year+=100;
  res = YEAR * year + DAY * ((year + 1) / 4);	// 这些年经过的秒数时间 + 每个闰年时多1 天
  res += month[tm->tm_mon];	// 的秒数时间，在加上当年到当月时的秒数。
 
  if (tm->tm_mon > 1 && ((year + 2) % 4))
    res -= DAY;
  res += DAY * (tm->tm_mday - 1);	// 再加上本月过去的天数的秒数时间。
  res += HOUR * tm->tm_hour;	// 再加上当天过去的小时数的秒数时间。
  res += MINUTE * tm->tm_min;	// 再加上1 小时内过去的分钟数的秒数时间。
  res += tm->tm_sec;		// 再加上1 分钟内已过的秒数。
  return res;			// 即等于从1970 年以来经过的秒数时间。
}
