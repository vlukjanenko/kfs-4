# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    boot.s                                             :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: majosue <majosue@student.21-school.ru>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/01 19:07:23 by majosue           #+#    #+#              #
#    Updated: 2022/12/22 09:26:54 by majosue          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

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
	global regholder:data
	regholder:
		resb 4
	stackholder:
		resb 8

section .text

global _start:function (_start.end - _start)
_start:
	mov [regholder], ebx
	extern main
	mov esp, stack_top
	push 0xBA0BAB
	push 0xBABE
	push 0xBE
	push 0xACE

	call main
	cli
.hang: hlt
	jmp .hang
.end:

test:
	extern gdtr
	lgdt [gdtr]
	jmp 0x08:complete_flush	
	ret

complete_flush:
    mov dx, 0x10
    mov ds, dx
    mov es, dx
    mov fs, dx
    mov gs, dx
    mov ss, dx
    ret
	
global get_stack:function
get_stack:
	mov [stackholder], DWORD stack_top - 4
	mov [stackholder + 4], DWORD esp
	mov eax, stackholder

	ret

