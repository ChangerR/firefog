
	org 7c00h
%define	SYSSIZE 3000h
%define	SETUPLEN 4
%define BOOTSEG  07c0h
%define	INITSEG  9000h
%define	SETUPSEG  9020h
%define	SYSSEG  0x1000
%define	ENDSEG  (SYSSEG + SYSSIZE)

entry:
	 mov ax,BOOTSEG
	 mov ds,ax
	 mov ax,INITSEG
	 mov es,ax
	 mov cx,256
	 sub si,si
	 sub di,di
	 rep movsw
	 jmp WORD INITSEG:(go - BOOTSEG*16)
go:
	mov ax,cs
	mov ds,ax
	mov es,ax
	mov ss,ax
	mov sp,0ffffh
	
load_setup:
	mov dx,0
	mov cx,2h
	mov bx,200h
	mov ax,200h + SETUPLEN
	int 13h
	jnc short ok_load_setup
	mov dx,0
	mov ax,0
	int 13h
	jmp short load_setup
	
ok_load_setup:
	mov dl,0x00
	mov ax,0x0800
	int 13h
	mov ch,0x00
	
	mov WORD[0],cx
	mov ax,INITSEG
	mov es,ax
	
	mov ax,0003h
	int 10h
	;print some message
	mov ah,3h
	xor bh,bh
	int 10h
	
	mov cx,24
	mov bx,7h
	mov bp,(msg - BOOTSEG * 16)
	mov ax,1301h
	int 0x10
	
	mov ax,SYSSEG
	mov es,ax
	call read_it
	call kill_motor
	
	jmp WORD INITSEG:200h

read_it:
	mov ax,es
	test ax,0x0fff
die:
	jne die
	xor bx,bx
	
rp_read:
	mov ax,es
	cmp ax,ENDSEG
	jb ok_read1
	ret
	
ok_read1:
	mov ax,WORD [0]
	sub ax,WORD[sread - BOOTSEG * 16]
	mov cx,ax
	shl cx,9
	add cx,bx
	jnc ok_read2
	je  ok_read2
	
	xor ax,ax
	sub ax,bx
	shr ax,9
ok_read2:
	call read_track
	mov cx,ax
	add ax,WORD [sread - BOOTSEG * 16]
	cmp ax,WORD [0]
	jne ok_read3
	
	mov ax,1
	sub ax,WORD [head- BOOTSEG * 16]
	jne ok_read4
	inc WORD[track- BOOTSEG * 16]
ok_read4:
	mov WORD[head- BOOTSEG * 16],ax
	xor ax,ax
ok_read3:
	mov WORD[sread- BOOTSEG * 16],ax
	shl cx,9
	add bx,cx
	jnc rp_read
	
	mov ax,es
	add ax,0x1000
	mov es,ax
	xor bx,bx
	jmp rp_read
	
read_track:
	push ax
	push bx
	push cx
	push dx
	mov dx,WORD[track - BOOTSEG * 16]
	mov cx,WORD[sread - BOOTSEG * 16]
	inc cx
	mov ch,dl
	mov dx,WORD[head - BOOTSEG * 16]
	mov dh,dl
	mov dl,0
	and dx,0x0100
	mov ah,2
	int 0x13
	jc bad_rt
	pop dx
	pop cx
	pop bx
	pop ax
	ret

bad_rt:
	mov ax,0
	mov dx,0
	int 0x13
	pop dx
	pop cx
	pop bx
	pop ax
	jmp read_track

kill_motor:
	push dx
	mov dx,0x3f2
	mov al,0
	out dx,al
	pop dx
	ret

sread:
	DW 1 + SETUPLEN
head:
	DW 0
track:
	DW 0
msg:
	DB 13,10
	DB "Loading system...",0
	DB 13,10,13,10
	 
	 times 510 - ($-$$) DB 0
	 DW 0aa55h