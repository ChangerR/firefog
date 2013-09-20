#include "string.h"
static char numchar[]="0123456789ABCDEF";
void sReverse(char* str)
{
	char* p = str;
	register char c;
	while(*p)
		p++;
	--p;
	while(str<p)
	{
		c = *str;
		*str = *p;
		*p = c;
		str++;
		p--;
	}
}
char *itoa(int value, char **string, int radix)
{
	register int t = value;
	char* s = *string;
	while(t)
	{
		**string = numchar[t % radix];
		++(*string);
		t /= radix;
	}
	**string = 0;
	sReverse(s);
	return s;
}

int vsprintf(char* dest,const char* fmt,va_list args)
{
	char* s;
	char* p;
	s = dest;
	while(*fmt)
	{
		if(*fmt == '%')
		{
			++fmt;
			switch(*fmt)
			{
				case 'd':
					itoa(va_arg(args,int),&dest,10);
					goto ctu;
				case 'x':
				case 'X':
					*dest++ = '0';
					*dest++ = 'x';
					itoa(va_arg(args,int),&dest,16);
					goto ctu;
				case 'S':
				case 's':
					p = va_arg(args,char*);
					while (*p)
						*dest++=*p++;
					goto ctu;
				default:
					*dest++ = '%';
					break;
			}
		}
		*dest++ = *fmt;
	ctu:
		if(*fmt)
			++fmt;
	}
	*dest = 0;
	return dest-s;
}