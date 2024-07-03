	.intel_syntax noprefix
	.text
.m0: .string "CPUID: %x\n"
.m1: .string "Largest basic function number implemented: %i\n"
.m2: .string "Vendor ID: %s\n"

.global testCPUID
.type testCPUID, @function
    

testCPUID:
	mov		eax, 0
	mov		ecx, 0
	cpuid
	mov		eax, ecx
	ret

    .section .note.GNU-stack,"",@progbits