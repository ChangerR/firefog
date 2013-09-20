LINKFLAGS=/SUBSYSTEM:CONSOLE	/ENTRY:startup_32	/NOLOGO	/NODEFAULTLIB:LIBCMT.lib
OBJS=head.obj	main.obj	memory.obj	string.obj	vsprintf.obj\
		ports.obj
CCFLAG=/nologo	/I../include	/X	/Zl	/GS-
CC = cl /c
LK = link
ASM = nasm -f	win32

all:kernel.bin

kernel.bin:kernel.exe
	e2b	kernel.exe	$@
	
kernel.exe:$(OBJS)
	$(LK)	$(LINKFLAGS)	/OUT:$@	$(OBJS)
	
main.obj:main.c
	$(CC)	$(CCFLAG)	main.c	/Fo$@

head.obj:head.asm
	$(ASM)	-o $@	head.asm

memory.obj:memory.asm
	$(ASM)	-o	$@	memory.asm

string.obj:string.asm
	$(ASM)	-o	$@	string.asm
	
vsprintf.obj:vsprintf.c
	$(CC)	$(CCFLAG)	vsprintf.c	/Fo$@
	
asm.obj:asm.asm
	$(ASM)	-o	$@	asm.asm
	
sys_call.obj:sys_call.asm
	$(ASM)	-o	$@	sys_call.asm
	
sched.obj:sched.c
	$(CC)	$(CCFLAG)	sched.c	/Fo$@
	
traps.obj:traps.c
	$(CC)	$(CCFLAG)	traps.c	/Fo$@
	
ports.obj:ports.c
	$(CC)	$(CCFLAG)	ports.c	/Fo$@
	
sys.obj:sys.c
	$(CC)	$(CCFLAG)	sys.c	/Fo$@
	
clean:
	del	*.exe
	del	*.bin
	del	*.obj