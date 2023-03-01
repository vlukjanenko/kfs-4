# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    boot.s                                             :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: majosue <majosue@student.21-school.ru>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/01 19:07:23 by majosue           #+#    #+#              #
#    Updated: 2023/03/01 11:06:51 by majosue          ###   ########.fr        #
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
	resb 16384					;; temp stack
	global stack_top:data
	stack_top:
	global max_addr:data
	max_addr:
	resb 4
	
section .text

global _start:function (_start.end - _start)
_start:
	mov esp, stack_top			
	push eax					;; save magic number multiboot
	push ebx					;; save address of struct multiboot_info
	push 0
	push 15
	extern terminal_initialize
	call terminal_initialize
	add esp, 8
	extern get_memory_map		
	call get_memory_map											
	cmp eax, 0
	jnz .hang
	call load_segment_registers
	extern idt_init
	call idt_init
	;mov esp, [max_addr]
	mov esp, stack_top			;; cleanup stack for demo
	push 0x6b636174  
	push 0x7320666f   
	push 0x20706f74
	extern main
	call main
	cli
.hang: hlt
	jmp .hang
.end:

load_segment_registers:
	extern gdtr
	lgdt [gdtr]
	jmp 0x08:complete_flush
	ret

complete_flush:
    mov dx, 0x10				;; data segment descriptor
    mov ds, dx	
    mov es, dx
    mov fs, dx
    mov gs, dx
	mov dx, 0x18				;; stack segment descriptor
    mov ss, dx
	ret

