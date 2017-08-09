
; Sample x64 Assembly Program 
.code 
PUBLIC GetErrorSAD_8x8_MMX64_asm 
GetErrorSAD_8x8_MMX64_asm PROC
	LOCAL	fourOnes : qword

	push rdi
	push rsi
	
	;x32_x64 architecture uses FAST_CALL calling conventions, so...
	mov rsi, rcx	; Current MB (Estimated MB)

	mov rdi, rdx; reference MB

	sub rax, rax	; init rax
	mov eax, r8d	; stride

	;Init locals needed for sum merging
	mov dword ptr fourOnes		, 00010001h 
	mov dword ptr fourOnes + 4	, 00010001h

	pxor	mm6, mm6			; mm6 = sum = 0

	pxor	mm7, mm7			; mm7 = 0

	sub rcx, rcx	; init rcx
	mov		ecx, 4

sad8_mmx_lp:

	movq mm0, qword ptr [rsi]	; ref
	movq mm1, qword ptr [rdi]	; cur

	add	rsi, rax
	add	rdi, rax

	movq mm2, qword ptr [rsi]	; ref2
	movq mm3, qword ptr [rdi]	; cur2

	movq mm4, mm0 
	movq mm5, mm2

	psubusb mm0, mm1
	psubusb mm2, mm3

	psubusb mm1, mm4		; mm1 = mm1 - mm0 < 0 ? 0 : mm1 - mm0
	psubusb mm3, mm5

	por mm0, mm1			; mm0 = |ref - cur|
	por mm2, mm3			; mm2 = |*(ref+stride) - *(cur+stride)|

	movq mm1,mm0
	movq mm3,mm2

	punpcklbw mm0,mm7		;convert u8 to u16
	punpcklbw mm2,mm7		

	punpckhbw mm1,mm7
	punpckhbw mm3,mm7

	paddusw mm0,mm1
	paddusw mm2,mm3

	paddusw mm6,mm0		; sum += mm01
	paddusw mm6,mm2		; sum += mm23

	add	rsi, rax
	add	rdi, rax

	dec	ecx
	jnz	sad8_mmx_lp

	pmaddwd mm6, qword ptr fourOnes	; merge sum
	pop 	rsi

	movq mm7, mm6
	pop 	rdi

	psrlq mm7, 32 

	paddd mm6, mm7

	;Return result
	sub rax, rax
	movd eax, mm6

	emms
	ret

GetErrorSAD_8x8_MMX64_asm ENDP
End

