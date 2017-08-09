
; Sample x64 Assembly Program 
.code 
PUBLIC GetErrorSAD_Int_8x8_MMX64_asm 
GetErrorSAD_Int_8x8_MMX64_asm PROC
	LOCAL	fourOnes : qword

	push rdi
	push rsi
	
	;x32_x64 architecture uses FAST_CALL calling conventions, so...
	mov rsi, rcx	; Current MB (Estimated MB)
	sub rcx, rcx	; init rcx

	mov rdi, rdx; reference MB

	sub rax, rax	; init rax
	mov eax, r8d	; stride

	;Init locals needed for sum merging
	mov dword ptr fourOnes		, 00010001h 
	mov dword ptr fourOnes + 4	, 00010001h

	add eax, r8d

	pxor	mm6, mm6			; mm6 = sum = 0

	pxor	mm7, mm7			; mm7 = 0
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

	psubusb mm1, mm4
	psubusb mm3, mm5

	por mm0, mm1			; mm0 = |ref - cur|
	por mm2, mm3			; mm2 = |*(ref+stride) - *(cur+stride)|

	movq mm1,mm0
	movq mm3,mm2

	punpcklbw mm0,mm7
	punpcklbw mm2,mm7

	punpckhbw mm1,mm7
	punpckhbw mm3,mm7

	paddusw mm0,mm1
	paddusw mm2,mm3

	paddusw mm6,mm0			; sum += mm01
	add	rsi, rax

	add	rdi, rax
	paddusw mm6,mm2			; sum += mm23

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
	movd rax, mm6

	emms

	ret

GetErrorSAD_Int_8x8_MMX64_asm ENDP
End

