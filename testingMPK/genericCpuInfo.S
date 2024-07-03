	.intel_syntax noprefix
	.text
.m0: .string "CPUID: %x\n"
.m1: .string "Largest basic function number implemented: %i\n"
.m2: .string "Vendor ID: %s\n"

    .globl main

main:
	push    r12
	mov	    eax, 1
	sub	    rsp, 16
    cpuid
    lea	    rdi, .m0[rip]
	mov	    esi, eax
	call	printf
	mov     eax, 0
    cpuid
	lea	    rdi, .m1[rip]
	mov	    esi, eax
	mov	    r12d, edx
	mov	    ebp, ecx
	call    printf
	mov     3[rsp], ebx
	lea	    rsi, 3[rsp]
    lea	    rdi, .m2[rip]
    mov     7[rsp], r12d
    mov     11[rsp], ebp
	call	printf
	add	    rsp, 16
	pop	    r12
	ret

    .section .note.GNU-stack,"",@progbits