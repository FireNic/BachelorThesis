	.intel_syntax noprefix
	.text
.global testCPUID
.type testCPUID, @function
    

testCPUID:
	push 	rbx
	mov		eax, 7
	mov		ecx, 0
	cpuid
	mov		eax, ecx
	pop		rbx
	ret

    .section .note.GNU-stack,"",@progbits