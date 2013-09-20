//#include "ports.h"
typedef unsigned int DWORD;
typedef unsigned short WORD;
typedef unsigned char BYTE;

__declspec(naked) BYTE inb_p(WORD port)
{
	__asm{
		push edx
		xor eax,eax
		mov dx,port
		in al,dx
		pop edx
		ret
	}
}

__declspec(naked) WORD inw_p(WORD port)
{
	__asm{
		push edx
		xor eax,eax
		mov dx,port
		in ax,dx
		pop edx
		ret
	}
}

__declspec(naked) DWORD ind_p(WORD port)
{
	__asm{
		push edx
		mov dx,port
		in eax,dx
		pop edx
		ret
	}
}

__declspec(naked) void outb_p(BYTE v,WORD port)
{
	__asm
	{
		push edx
		push eax
		mov dx,port
		mov al,v
		out dx,al
		pop eax
		pop edx
		ret
	}

}

__declspec(naked) void outw_p(WORD v,WORD port)
{
	__asm
	{
		push edx
		push eax
		mov dx,port
		mov ax,v
		out dx,ax
		pop eax
		pop edx
		ret
	}

}

__declspec(naked) void outd_p(DWORD v,WORD port)
{
	__asm
	{
		push edx
		push eax
		mov dx,port
		mov eax,v
		out dx,eax
		pop eax
		pop edx
		ret
	}
}