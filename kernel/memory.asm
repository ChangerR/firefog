[BITS 32]
global _memset,_memcpy
[SECTION .text]
_memset:
	mov ecx,[esp+12]
	movzx eax,BYTE [esp+8]
	mov edi,[esp+4]
	cld
	rep stosb
	ret
_memcpy:
	mov ecx,[esp+12]
	mov esi,[esp+8]
	mov edi,[esp+4]
	cld
	rep movsb
	ret
