#ifndef __FIRE_STRING
#define __FIRE_STRING
#include "stdarg.h"
int strlen(const char* s);
int strlen2(const char*s,int maxlen);
char* strcpy( char* dest,const char* source);
char* strncpy(char* dest,const char* source,int len);
int strcmp(const char* dest,const char* source);
int strncmp(const char* dest,const char* source,int len);
char* strcat(const char* dest,const char* source);
void printk(const char* string,...);

int vsprintf(char* dest,const char* fmt,va_list args);
#endif