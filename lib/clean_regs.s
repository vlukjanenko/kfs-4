; clear regs and halt
global clean_regs_hlt:function
clean_regs_hlt:
	xor eax, eax
    xor ebx, ebx
    xor ecx, ecx
    xor edx, edx
    xor esi, esi
    xor edi, edi
    xor ebp, ebp
    xor esp, esp
 
    hlt