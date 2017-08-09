
; Sample x64 Assembly Program 
.code 
PUBLIC GetErrorSAD_Int_16x16_MMX64_asm 
GetErrorSAD_Int_16x16_MMX64_asm PROC
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

	pxor mm7, mm7			; mm7 = sum = 0
	pxor mm6, mm6			; mm6 = 0

	movq mm1, qword ptr [rsi]			; 8 pixels for current MB
	movq mm3, qword ptr [rsi+8]		; another 8 pixels in the same row of current MB

	mov	ecx, 16

sad16_mmx_loop:
	movq mm0, qword ptr [rdi]			; 8 pixels for reference MB
	movq mm2, qword ptr [rdi+8]		; another 8 pixels in the same row of reference MB

	movq mm4, mm1
	movq mm5, mm3

	psubusb mm1, mm0
	psubusb mm3, mm2

	add	rsi, rax
	add	rdi, rax

	psubusb mm0, mm4
	psubusb mm2, mm5

	por mm0, mm1			; mm0 = |cur - ref|
	por mm2, mm3			; mm2 = |*(cur+8) - *(ref+8)|

	movq mm1,mm0
	movq mm3,mm2

	punpcklbw mm0,mm6
	punpckhbw mm1,mm6

	punpcklbw mm2,mm6
	punpckhbw mm3,mm6

	paddusw mm0,mm1
	paddusw mm2,mm3		

	paddusw mm7,mm0		; sum += mm01
	movq mm1, qword ptr [rsi]		; 8 pixels for current MB

	paddusw mm7,mm2		; sum += mm23
	movq mm3, qword ptr [rsi+8]	; another 8 pixels in the same row of current MB

	;// start next row's processing
	movq mm0, qword ptr [rdi]			; 8 pixels for reference MB
	movq mm2, qword ptr [rdi+8]		; another 8 pixels in the same row of reference MB

	movq mm4, mm1
	movq mm5, mm3

	psubusb mm1, mm0
	psubusb mm3, mm2

	add	rsi, rax
	add	rdi, rax

	psubusb mm0, mm4
	psubusb mm2, mm5

	por mm0, mm1			; mm0 = |cur - ref|
	por mm2, mm3			; mm2 = |*(cur+8) - *(ref+8)|

	movq mm1,mm0
	movq mm3,mm2

	punpcklbw mm0,mm6
	punpckhbw mm1,mm6

	punpcklbw mm2,mm6
	punpckhbw mm3,mm6

	paddusw mm0,mm1
	paddusw mm2,mm3		

	paddusw mm7,mm0		; sum += mm01
	movq mm1, qword ptr [rsi]		; 8 pixels for current MB

	paddusw mm7,mm2		; sum += mm23
	sub		ecx, 2		; unlooped two rows' processing

	movq mm3, qword ptr [rsi+8]	; another 8 pixels in the same row of current MB

	jnz	sad16_mmx_loop

	pmaddwd mm7, qword ptr fourOnes	; merge sum
	pop 	rsi

	movq mm0, mm7
	pop 	rdi
	psrlq mm7, 32 

	paddd mm0, mm7

	;Return result
	sub rax, rax
	movd rax, mm0
	
	emms

	ret

GetErrorSAD_Int_16x16_MMX64_asm ENDP
End

