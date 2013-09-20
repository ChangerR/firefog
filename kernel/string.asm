[BITS 32]

global _strlen,_strcmp,_strcpy,_strcat,_strlen2,_strncpy,_strncmp
[SECTION .text]
_strlen:
	push edi
	mov edi,[esp+8]
	xor eax,eax
L1:
	cmp BYTE[edi],0
	je L2
	inc eax
	inc edi
	jmp L1
L2:
	pop edi
	ret
	
_strlen2:
	push edi
	push esi
	push ecx
	cld
	mov edi,[esp+16]
	mov esi,edi
	mov ecx,[esp+20]
	xor eax,eax
	repnz scasb
	sub edi,esi
	dec edi
	mov eax,edi
	pop ecx
	pop esi
	pop edi
	ret
	
_strcpy:
	push edi
	push esi
	mov edi,[esp+12]
	mov esi,[esp+16]
@1:
	mov al,BYTE[esi]
	mov BYTE[edi],al
	cmp al,0
	je @2
	inc edi
	inc esi
	jmp @1
@2:
	mov eax,[esp+12]
	pop esi
	pop edi
	ret
	
_strncpy:
	push edi
	push esi
	push ecx
	mov edi,[esp+16]
	mov esi,[esp+20]
	mov ecx,[esp+24]
@3:
	mov al,BYTE[esi]
	cmp al,0
	je @5
	cmp ecx,0
	je @4
	mov BYTE[edi],al
	inc edi
	inc esi
	dec ecx
	jmp @3
@5:
	cmp ecx,0
	je @4
	mov BYTE[edi],0
@4:
	mov eax,[esp+16]
	pop ecx
	pop esi
	pop edi
	ret
	
_strcmp:
	push edi
	push esi
	push ebx
	mov edi,[esp+16]
	mov esi,[esp+20]
@6:
	movzx eax,BYTE[edi]
	movzx ebx,BYTE[esi]
	cmp eax,0
	je @end
	cmp eax,ebx
	ja @above
	jb @below
	inc edi
	inc esi
	jmp @6
@above:
	xor eax,eax
	inc eax
	jmp @end
@below:
	xor eax,eax
	dec eax
	jmp @end	
@end:
	pop ebx
	pop esi
	pop edi
	ret
	
_strncmp:
	push edi
	push esi
	push ecx
	push ebx
	mov edi,[esp+20]
	mov esi,[esp+24]
	mov ecx,[esp+28]
@7:
	cmp ecx,0
	jz @10
	movzx eax,BYTE[edi]
	movzx ebx,BYTE[esi]
	cmp eax,0
	je @11
	cmp eax,ebx
	ja @8
	jb @9
	inc edi
	inc esi
	dec ecx
	jmp @7
@8:
	mov eax,1
	jmp @11
@9:
	mov eax,-1
	jmp @11
@10:
	xor eax,eax
@11:
	pop ebx
	pop ecx
	pop esi
	pop edi
	ret
	
_strcat:
	push edi
	push esi
	mov edi,[esp+12]
	mov esi,[esp+16]
@12:
	cmp BYTE[edi],0
	jz @13
	inc edi
	jmp @12
@13:
	push esi
	push edi
	call _strcpy
	add esp,8
	mov eax,[esp+12]
	pop esi
	pop edi
	ret