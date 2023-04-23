# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    boot.s                                             :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: majosue <majosue@student.21-school.ru>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/01 19:07:23 by majosue           #+#    #+#              #
#    Updated: 2023/03/01 12:19:50 by majosue          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

global pd_first_entry:data
global pt_first_entry:data

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

section .bootstrap.bss nobits alloc noexec write align=4
	align 16
	resb 16384					;; temp stack
	global stack_top:data
	stack_top:
	global max_addr:data
	max_addr:
	resb 4

section .tables nobits alloc noexec write align=4096
pd_first_entry:
	resb 4096
pt_first_entry:
	resb 4096

section .boot.text progbits alloc exec nowrite align=16
global _start:function
_start:
	mov esp, stack_top
	push eax					;; save magic number multiboot
	push ebx					;; save address of struct multiboot_info
	extern get_memory_map
	call get_memory_map
	add esp, 8
	cmp eax, 0
	jnz pre_start.hang
	call load_segment_registers
	;extern idt_init
	;call idt_init
	;mov esp, [max_addr]
	;push 0x6b636174
	;push 0x7320666f
	;push 0x20706f74

	;; тут включаем пейджинг
	extern turn_on_paging
	call turn_on_paging
	;; правим стек
	mov esp, 0xC0000000 + stack_top
	;; не забываем про адрес вга буфера
	jmp pre_start


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

section .bss

section .text
pre_start:	;; тут пойдет после включение пейджинга
	;; удаляем из таблицы замапленную нулевую
	mov [pd_first_entry], DWORD 0
	mov ecx, cr3
	mov cr3, ecx
	extern idt_init
	call idt_init
	push 0
	push 15
	extern terminal_initialize
	call terminal_initialize
	add esp, 8
	extern main
	call main
	cli
.hang: hlt
	jmp .hang
.end:

