
;now wo are in 32-bit mode
[BITS 32]
extern _main,_stack_start
global _pg_dir,_pg0,_pg1,_pg2,_pg3,_idt,_gdt,_startup_32
[SECTION  .text]
_pg_dir:
	
_startup_32:
	mov eax,10h
	
	mov ds,ax
	mov es,ax
	mov fs,ax
	mov gs,ax
	lss esp,[_stack_start]
	call setup_idt
	call setup_gdt
	mov eax,10h
	mov ds,ax
	mov es,ax
	mov fs,ax
	mov gs,ax
	lss esp,[_stack_start]
	xor eax,eax
@1:	mov eax,0x00ff0000
	mov DWORD[0],eax
	cmp eax,DWORD[0x100000]
	je l6
	
	
	mov eax,cr0
	and eax,0x80000011
	or eax,2
	mov cr0,eax
	call check_x87
	jmp after_page_tables
	
	align 32
	
check_x87:
	fninit
	fstsw ax
	cmp al,0
	je short @2
	
	
@2:
	DB 0xDB,0xE4
	ret
	
setup_idt:
	lea edx,[ignore_idt]
	mov eax,0x00080000
	mov ax,dx
	
	mov dx,0x8e00
	lea edi,[_idt]
	mov ecx,256
rp_sidt:
	mov [edi],eax
	mov [edi + 4],edx
	add edi ,8
	dec ecx
	jne rp_sidt
	lidt [idt_descr]
	ret
	
setup_gdt:
	lgdt [gdt_descr]
	ret
	
	
	times (0x1000 - ($-$$) ) DB 0
_pg0:
	times (0x2000 - ($-$$) ) DB 0
_pg1:
	times (0x3000 - ($-$$) ) DB 0
_pg2:
	times (0x4000 - ($-$$) ) DB 0
_pg3:
	times (0x5000 - ($-$$) ) DB 0
	
_tmp_floppy_area:
	times 1024 DB 0
after_page_tables:
	push 0
	push 0
	push 0
	push l6
	push _main
	jmp setup_paging
l6:
	hlt
	jmp l6
	
ignore_idt:
	push eax
	push ecx
	push edx
	push ds
	
	push es
	push fs
	
	mov eax,10h
	mov ds,ax
	mov es,ax
	mov fs,ax
	
	pop eax
	pop fs
	pop es
	pop ds
	pop edx
	pop ecx
	pop eax
	iret
	
setup_paging:
	mov ecx,1024*5
	xor eax,eax
	xor edi,edi
	rep stosd
	
	mov DWORD[_pg_dir],_pg0+7
	mov DWORD[_pg_dir + 4],_pg1+7
	mov DWORD[_pg_dir + 8],_pg2+7
	mov DWORD[_pg_dir + 12],_pg3+7
	
	mov edi,_pg3 + 4092
	mov eax,0xfff007

	std
pagesetloop:
	stosd
	sub eax,0x001000
	jge pagesetloop
	
	xor eax,eax
	mov cr3,eax
	
	mov eax,cr0
	or eax,0x80000000
	mov cr0,eax
	ret
	
	align 32
	dw 0
idt_descr:
	dw 256*8 -1
	DD _idt
	
	align 32
	dw 0
gdt_descr:
	dw 256*8 - 1
	DD _gdt
	
_idt:
	times 256 DQ 0
	
_gdt:
	DQ 0x0000000000000000
	DQ 0x00c09a0000000fff
	DQ 0x00c0920000000fff
	DQ 0x0000000000000000
	times 252 DQ 0
	
	
	
	