
;this module setup the machine to 32
%define INITSEG 0x9000
%define SYSSEG  0x1000
%define SETUPSEG 0x9020


start:
	mov ax,INITSEG
	mov ds,ax
	;存储光标的xy坐标
	mov ah,0x03
	xor bh,bh
	int 0x10
	mov WORD[0],dx
	;读取扩展内存字节数
	mov ah,0x88
	int 0x15
	mov WORD[2],ax
	;读取显示模式ah=屏幕字符的列数al=显示模式bh=显示的页码
	mov ah,0x0f
	int 10h
	mov WORD[4],bx
	mov WORD[6],ax
	;获取显示卡信息
	mov ah,0x12
	mov bl,0x10
	int 0x10
	mov WORD[8],ax
	mov WORD[10],bx
	mov WORD[12],cx
	;得到第一个硬盘参数表
	mov ax,0x0000
	mov ds,ax
	lea si,[4 * 0x41]
	mov ax,INITSEG
	mov es,ax
	mov di,0x0080
	mov cx,0x10
	rep movsb
	;得到第二个硬盘参数表
	mov ax,0x0000
	mov ds,ax
	lds si,[4 * 0x46]
	mov ax,INITSEG
	mov es,ax
	mov di,0x0090
	mov cx,0x10
	rep movsb
	;call showmsg
	
no_disk1:
	mov ax,INITSEG
	mov es,ax
	mov di,0x0090
	mov cx,0x10
	mov ax,0x0000
	rep stosb
	
	;I want to set display mode 640*480*256
	;just a test
	;mov ax,0x0013
	;int 10h
is_disk1:
	call showmsg
	;now we want to move to protect mode
	cli
	
	mov ax,0x00
	cld
do_move:
	mov es,ax
	add ax,0x1000
	cmp ax,0x9000
	jz end_move
	mov ds,ax
	sub di,di
	sub si,si
	mov cx,0x8000
	rep movsw
	jmp do_move
end_move:
	mov ax,SETUPSEG
	mov ds,ax
	lidt [idt_48]
	lgdt [gdt_48]
	
	;now we enable a20
	call empty_8042
	mov al,0xd1
	out 0x64,al
	
	call empty_8042
	mov al,0xdf
	out 0x60,al
	call empty_8042
	
	;发送ICW1字节 表示中断时边沿促发 多片 需要ICW4
	mov al,0x11
	out 0x20,al
	DW 0x00eb,0x00eb
	out 0xa0,al
	DW 0x00eb,0x00eb
	;设置中断号0x20~0x27 0x28~0x2f
	mov al,0x20
	out 0x21,al
	DW 0x00eb,0x00eb
	mov al,0x28
	out 0xa1,al
	DW 0x00eb,0x00eb
	;设置ICW3
	mov al,0x04
	out 0x21,al
	DW 0x00eb,0x00eb
	mov al,0x02
	out 0xa1,al
	DW 0x00eb,0x00eb
	;设置ICW4 完成设置
	mov al,0x01
	out 0x21,al
	DW 0x00eb,0x00eb
	out 0xa1,al
	DW 0x00eb,0x00eb
	;屏蔽所以中断
	mov al,0xff
	out 0x21,al
	DW 0x00eb,0x00eb
	out 0xa1,al
	DW 0x00eb,0x00eb
	
	mov ax,0x0001
	lmsw ax
	;we goto 32 model
	jmp DWORD 8:0x00000000
empty_8042:
	dw 0x00eb,0x00eb
	in al,0x64
	test al,2
	jnz empty_8042
	ret
gdt:
	DW 0,0,0,0
	DW 0x07ff,0x0000,0x9a00,0x00c0  ;code section
	DW 0x07ff,0x0000,0x9200,0x00c0	;data section
	
idt_48:
	DW 0
	DW 0,0
gdt_48:
	DW 0x800
	DW 512 + gdt,0x9

fin:
	HLT
	jmp fin
showmsg:
	mov cx,28
	mov bx,7h
	mov ax,SETUPSEG
	mov es,ax
	lea bp,[msg]
	mov ax,1301h
	int 0x10
	ret
msg:
	DB 13,10
	DB "Now we go to 32 mode",0
	DB 13,10,13,10
	times 512*4 - ($ - $$) DB 0
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	