#ifndef __FIRE_SYSTEM
#define __FIRE_SYSTEM


#define _set_gate(n,type,dpl,addr) \
{\
	__asm push edx \
	__asm push eax \
	__asm push edi \
	__asm mov edx,plidt \
	__asm mov eax,n \
	__asm lea edi,[edx+eax*8] \
	__asm mov edx,(addr) \
	__asm mov ax,0x0008 \
	__asm shl eax,16 \
	__asm mov ax,dx	\
	__asm mov dx,0x8000+((type)<<8) +((dpl)<<13) \
	__asm mov DWORD PTR[edi],eax \
	__asm mov DWORD PTR[edi + 4],edx\
	__asm pop edi \
	__asm pop eax \
	__asm pop edx \
}

#define set_intr_gate(n,addr) \
		_set_gate(n,14,0,addr)
		
#define set_trap_gate(n,addr) \
		_set_gate(n,15,0,addr)
		
#define set_system_gate(n,addr) \
		_set_gate(n,15,3,addr)
		
#define _set_tssldt_desc(n,addr,type)\
{\
	__asm push edx \
	__asm push eax \
	__asm mov edx,(addr)\
	__asm mov ax,dx \
	__asm shl eax,16 \
	__asm mov ax,104 \
	__asm mov DWORD PTR[(n)],eax \
	__asm shr edx,8 \
	__asm mov al,dh\
	__asm mov dl,(type) \
	__asm xor dh,dh \
	__asm shl edx,8 \
	__asm mov dl,al \
	__asm mov DWORD PTR[(n)+4],edx \
	__asm pop eax \
	__asm pop edx \
}


#define set_tss_desc(n,addr) _set_tssldt_desc(n,addr, 0x89)

#define set_ldt_desc(n,addr) _set_tssldt_desc(n,addr, 0x82)
	
	
	

	
#define hlt() {\
	__asm hlt\
	}
	
#define sti() {\
	__asm sti\
	}
	
#define cli() {\
	__asm cli\
	}
	
#define iret() {\
	__asm iret\
	}
	
#define nop() {\
	__asm nop\
	}

#endif