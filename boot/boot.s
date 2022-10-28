MBALIGN equ 1 << 0
MEMINFO equ 1 << 1
FLAGS equ MBALIGN | MEMINFO
MAGIC equ 0x1BADB002
CHECKSUM equ -(MAGIC + FLAGS)

section .multiboot
	align 4
	dd MAGIC
	dd FLAGS
	dd CHECKSUM

section .bss
	align 16
	stack_bottom:
	resb 16384
	stack_top:

section .text
global _start:function (_start.end - _start)
_start:
	extern main
	mov esp, stack_top
	call main
	cli
.hang: hlt
	jmp .hang
.end:

global status_port:function
status_port:
	in al, 0x64
	ret
.end:

global read_key:function
read_key:
	in al, 0x60
	ret
.end: