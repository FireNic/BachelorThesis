.intel_syntax noprefix
.text
.global readPKRU
.type readPKRU, @function
    

readPKRU:
    mov     ecx, 0
    RDPKRU
	ret


.global writePKRU
.type writePKRU, @function

writePKRU:
    mov     eax, edi
    mov     edx, 0
    mov     ecx, 0
    WRPKRU
    ret