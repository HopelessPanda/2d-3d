;
; GetErrorSAD_16x16_H264_MMX64_asm(const BYTE* pSrc1, const BYTE* pSrc2, const BYTE* blockRef, const int strideRef, int iStride, int h264_blend);
; params:
;	pSrc1		First source MB				rcx		[rbp + 10H]
;	pSrc2		Second source MB			rdx		[rbp + 18H]
;	blockRef	Reference MB				r8		[rbp + 20H]
;	strideRef	Stride in reference MB		r9d		[rbp + 28H]
;	iStride		Stride in source MB			---		[rbp + 30H]
;	h264_blend	Is H264_BLEND? (flag)		---		[rbp + 38H]

.code 
PUBLIC GetErrorSAD_16x16_H264_MMX64_asm 
GetErrorSAD_16x16_H264_MMX64_asm PROC
	;push	rbp
	;mov	rbp, rsp
	;sub	rsp, 16
	LOCAL 	fourOnes:qword,	eightOnes:qword

	;Init constants needed for sum merging
	mov dword ptr fourOnes		, 00010001h 
	mov dword ptr fourOnes + 4	, 00010001h
	mov dword ptr eightOnes		, 01010101h
	mov dword ptr eightOnes + 4	, 01010101h
	
	;Save registers, sub rsp, 24
	push rdi
	push rsi
	push rbx

	;Test H264_blend condition
	mov eax, dword ptr [rbp + 38H]
    or      eax, eax          ; Is h264_blend = 0?
    jz      no_blend          ; Jump if so


	;H264_BLEND
	;............

	mov rsi, rcx					; pSrc1
	mov rbx, rdx					; pSrc2
	mov rdi, r8						; blockRef
	mov eax, r9d					; strideRef
	mov edx, dword ptr [rbp + 30H]	; iStride

	movq mm6, qword ptr eightOnes
	pxor mm7, mm7

	mov	ecx, 16

lineLoopComplex:
	movq mm1, qword ptr [rsi]			;// 8 pixels for current MB
	movq mm3, qword ptr [rsi + 8]		;// another 8 pixels in the same row of current MB
	add	rsi, rdx

	movq mm0, qword ptr [rbx]
	movq mm2, qword ptr [rbx + 8]
	add	rbx, rdx

	movq mm4, mm1
	movq mm5, mm3

	por mm4, mm0
	por mm5, mm2

	pand mm4, mm6
	pand mm5, mm6

	por mm1, mm6
	por mm3, mm6

	psrlq mm1, 1
	psrlq mm3, 1

	por mm0, mm6
	por mm2, mm6

	psrlq mm0, 1
	psrlq mm2, 1

	pxor mm6, mm6

	paddb mm1, mm0
	paddb mm3, mm2

	paddb mm1, mm4
	paddb mm3, mm5

	movq mm0, qword ptr [rdi]		;	// 8 pixels for reference MB
	movq mm2, qword ptr [rdi + 8]	;	// another 8 pixels in the same row of reference MB
	add	rdi, rax

	movq mm4, mm1
	movq mm5, mm3

	psubusb mm1, mm0
	psubusb mm3, mm2

	psubusb mm0, mm4
	psubusb mm2, mm5

	por mm0, mm1			;// mm0 = |cur - ref|
	por mm2, mm3			;// mm2 = |*(cur+8) - *(ref+8)|

	movq mm1, mm0
	movq mm3, mm2

	punpcklbw mm0, mm6
	punpckhbw mm1, mm6

	punpcklbw mm2, mm6
	punpckhbw mm3, mm6

	movq mm6, qword ptr eightOnes

	paddusw mm0, mm1
	paddusw mm2, mm3		

	paddusw mm7, mm0		;// sum += mm01
	paddusw mm7, mm2		;// sum += mm23

	sub ecx, 1

	jnz	lineLoopComplex

	pmaddwd mm7, qword ptr fourOnes	;// merge sum

	movq mm0, mm7
	psrlq mm7, 32 

	paddd mm0, mm7
	
	;return result
	movd rax, mm0

	emms

	jmp end_label

no_blend:

	;H264_NO_BLEND
	;............

	;(const BYTE* pSrc1, const BYTE* pSrc2, const BYTE* blockRef, const int strideRef, int iStride, int h264_blend);			
	mov rsi, rcx					; pSrc1
	mov rdi, r8						; blockRef
	mov eax, r9d					; strideRef
	mov edx, dword ptr [rbp + 30H]	; iStride

	pxor mm6, mm6
	pxor mm7, mm7

	mov	ecx, 16

lineLoopSimple:
	movq mm1, qword ptr [rsi]			;// 8 pixels for current MB
	movq mm3, qword ptr [rsi + 8]		;// another 8 pixels in the same row of current MB
	add	rsi, rdx

	movq mm0, qword ptr [rdi]			;// 8 pixels for reference MB
	movq mm2, qword ptr [rdi + 8]		;// another 8 pixels in the same row of reference MB
	add	rdi, rax

	movq mm4, mm1
	movq mm5, mm3

	psubusb mm1, mm0
	psubusb mm3, mm2

	psubusb mm0, mm4
	psubusb mm2, mm5

	por mm0, mm1			;// mm0 = |cur - ref|
	por mm2, mm3			;// mm2 = |*(cur+8) - *(ref+8)|

	movq mm1, mm0
	movq mm3, mm2

	punpcklbw mm0, mm6
	punpckhbw mm1, mm6

	punpcklbw mm2, mm6
	punpckhbw mm3, mm6

	paddusw mm0, mm1
	paddusw mm2, mm3		

	paddusw mm7, mm0		;// sum += mm01
	paddusw mm7, mm2		;// sum += mm23

	sub ecx, 1

	jnz	lineLoopSimple

	pmaddwd mm7, qword ptr fourOnes	;// merge sum

	movq mm0, mm7
	psrlq mm7, 32 

	paddd mm0, mm7
	
	;save result
	sub   rax, rax
	movd rax, mm0

	emms

end_label:

	pop		rbx
	pop 	rsi
	pop 	rdi
	
	ret

GetErrorSAD_16x16_H264_MMX64_asm ENDP
End

