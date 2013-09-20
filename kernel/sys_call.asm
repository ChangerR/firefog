[BITS 32]
extern _jiffies,_do_timer,_do_signal,_current,_schedule,_sys_call_table,_task\
		,_find_empty_process,_copy_process
global _timer_interrupt,_system_call,_sys_fork,_coprocessor_error,_device_not_available
[SECTION .text]
; EAX = 0X00
; EBX = 0X04
; ECX = 0X08
; EDX = 0X0C
; FS  = 0X10
; ES  = 0X14
; DS  = 0X18
; EIP = 0X1C
; CS  = 0X20
; ELFAGS = 0X24 
;OLDESP = 0x28
;OLDSS = 0x2c

;task_t struct
; state = 0
; counter = 4
; priority = 8
; signal = 12
; sigcation = 16
; blocked = 33*16

%define nr_system_call 5
	align 4
reschedule:
	push ret_from_sys_call
	jmp _schedule

	align 4
_system_call:
	cmp eax,nr_system_call
	ja bad_system_call
	
	push ds
	push es
	push fs
	push edx
	push ecx
	push ebx
	mov edx,10
	mov ds,dx
	mov es,dx
	mov edx,0x17
	mov fs,dx
	mov ebx,DWORD[_sys_call_table]
	call DWORD[ebx+eax*4]
	push eax
	mov eax,DWORD[_current]
	cmp [eax],0
	jne reschedule
	cmp [eax+4],0
	je reschedule
	
ret_from_sys_call:
	mov eax,DWORD [_current]
	mov ecx,DWORD[_task]
	cmp eax,DWORD[ecx]
	je r_end
	;如果是系统任务则直接退出 不做任何处理
	cmp WORD[esp+0x20],0xf
	jne r_end
	cmp WORD[esp+0x2c],0x17
	jne r_end
	;得到进程信号量
	mov ecx,DWORD[eax+528]
	mov ebx,DWORD[eax+12]
	not ecx
	and ecx,ebx
	bsf ecx,ecx
	
	je r_end
	btr edx,ecx
	mov DWORD[eax+12],edx
	inc ecx
	push ecx
	call _do_signal
	pop eax
r_end:
	pop eax
	pop ebx
	pop ecx
	pop edx
	pop fs
	pop es
	pop ds
	iret
	

 align 4
_timer_interrupt:
	push ds
	push es
	push fs
	push edx
	push ecx
	push ebx
	push eax
	mov eax,0x10
	mov ds,ax
	mov es,ax
	mov eax,0x17
	mov fs,ax
	inc DWORD[_jiffies]
	
	mov al,0x20
	out 0x20,al
	
	mov eax,DWORD[esp+0x20]
	and eax,3
	push eax
	call _do_timer
	add esp,4
	jmp ret_from_sys_call

	align 4
_sys_fork:
	call _find_empty_process
	test eax,eax
	js @1
	push gs
	push esi
	push edi
	push ebp
	push eax
	call _copy_process
	add esp,20
@1:
	ret
	
	
_coprocessor_error:
	push ds
	push es
	push fs
	push edx
	push ecx
	push eax
	mov eax,0x10
	mov ds,ax
	mov es,ax
	mov eax,0x17
	mov fs,ax
	push ret_from_sys_call
	;jmp _math_error
	ret
	
_device_not_available:
	push ds
	push es
	push fs
	push edx
	push ecx
	push eax
	mov eax,0x10
	mov ds,ax
	mov es,ax
	mov eax,0x17
	mov fs,ax
	push ret_from_sys_call
	;jmp _math_error
	ret