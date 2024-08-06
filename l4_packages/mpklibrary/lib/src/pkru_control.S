.intel_syntax noprefix
.text
.global pkru_lib_read_PKRU
.type pkru_lib_read_PKRU, @function
    

pkru_lib_read_PKRU:
    mov     ecx, 0
    RDPKRU
	ret


.global pkru_lib_write_PKRU
.type pkru_lib_write_PKRU, @function

pkru_lib_write_PKRU:
    mov     eax, edi
    mov     edx, 0
    mov     ecx, 0
    WRPKRU
    ret