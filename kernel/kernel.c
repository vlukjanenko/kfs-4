/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kernel.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majosue <majosue@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 19:00:59 by majosue           #+#    #+#             */
/*   Updated: 2023/05/02 08:17:51 by majosue          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "stddef.h"
#include "screen.h"
#include "baselib.h"
#include "keyboard.h"
#include "multiboot.h"
#include "memory.h"
#include "IDT.h"

void	print_stack(void)
{
	void* top = &stack_top + 0xc0000000;
	void* bottom;
	char stack_indicator1[8] = {'b','o', 't', 't', 'o', 'm' , '.', '.'};
	(void)stack_indicator1;
	asm ("movl %%esp, %0" : "=r" (bottom));
	printf("Stack:\n");
	print_memory(bottom, top - bottom);
}

void	print_gdt()
{
	uint64_t *entry = (uint64_t *)0x800;

	printf("Segment descriptor:\n");
	terminal_setcolor(VGA_COLOR_WHITE, VGA_COLOR_LIGHT_MAGENTA);
	printf("Flags ");
	terminal_setcolor(VGA_COLOR_WHITE, VGA_COLOR_GREEN);
	printf("Access Byte\n");
	terminal_setcolor(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
	printf("0x");
	terminal_setcolor(VGA_COLOR_WHITE, VGA_COLOR_LIGHT_BLUE);
	printf("00");
	terminal_setcolor(VGA_COLOR_WHITE, VGA_COLOR_LIGHT_MAGENTA);
	printf("0");
	terminal_setcolor(VGA_COLOR_WHITE, VGA_COLOR_RED);
	printf("0");
	terminal_setcolor(VGA_COLOR_WHITE, VGA_COLOR_GREEN);
	printf("00");
	terminal_setcolor(VGA_COLOR_WHITE, VGA_COLOR_LIGHT_BLUE);
	printf("000000");
	terminal_setcolor(VGA_COLOR_WHITE, VGA_COLOR_RED);
	printf("0000\n");
	terminal_setcolor(VGA_COLOR_WHITE, VGA_COLOR_LIGHT_BLUE);
	printf("Base ");
	terminal_setcolor(VGA_COLOR_WHITE, VGA_COLOR_RED);
	printf("Limit ");
	terminal_setcolor(VGA_COLOR_WHITE, VGA_COLOR_LIGHT_BLUE);
	printf("Base ");
	terminal_setcolor(VGA_COLOR_WHITE, VGA_COLOR_RED);
	printf("Limit\n");
	terminal_setcolor(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
	printf("GDT:\n");
	for (int i = 0; i < 7; i++) {
		printf("%010p: %016llx\n", entry, *entry);
		printf("Base: %08x\n", (*entry >> 40 & 0xff000000)
							|(*entry >>  8 & 0x00ff0000)
							|(*entry >> 16 & 0x0000ffff));
		printf("Limit: %x\n", (*entry >> 32 & 0x000f0000)
									 |(*entry & 0x0000ffff));
		printf("Flags (%llx): ", (*entry >> 52 & 0xf));
		printf("G: %llx ", (*entry >> 52 & 0x8) >> 3);
		printf("DB: %llx ", (*entry >> 52 & 0x4) >> 2);
		printf("L: %llx\n", (*entry >> 52 & 0x2) >> 1);
		printf("Access Byte (%llx): ",  (*entry >> 40 & 0xff));
		printf("P: %llx DPL: %llx S: %llx E: %llx DC: %llx RW: %llx A: %llx\n",
		(*entry >> 40 & 0x80) >> 7,
		(*entry >> 40 & 0x60) >> 5,
		(*entry >> 40 & 0x10) >> 4,
		(*entry >> 40 & 0x8) >> 3,
		(*entry >> 40 & 0x4) >> 2,
		(*entry >> 40 & 0x2) >> 1,
		(*entry >> 40 & 0x1));
		entry++;
	}
}

int	main(void)
{
	memory_init();
	terminal_initialize(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
/* 	disable_cursor();
	terminal_setcolor(VGA_COLOR_RED, VGA_COLOR_BLACK);
	printf("                                  :::      ::::::::\n");
	terminal_setcolor(VGA_COLOR_GREEN, VGA_COLOR_BLACK);
	printf("                                :+:      :+:    :+:\n");
	terminal_setcolor(VGA_COLOR_CYAN, VGA_COLOR_BLACK);
	printf("                              +:+ +:+         +:+\n");
	terminal_setcolor(VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK);
	printf("                            +#+  +:+       +#+\n");
	terminal_setcolor(VGA_COLOR_BLUE, VGA_COLOR_BLACK);
	printf("                          +#+#+#+#+#+   +#+\n");
	terminal_setcolor(VGA_COLOR_LIGHT_BLUE, VGA_COLOR_BLACK);
	printf("                               #+#    #+#\n");
	terminal_setcolor(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
	printf("                              ###   ########.fr\n"); */
	//print_stack();
	//print_gdt();
/*
	printf("Position of symbol end_of_code = %x\n", &end_of_code); // предоположительно конец кода

	printf("frame addr = %p\n", get_frame(0x10000, 0x11000));
	printf("frame addr = %p\n", get_frame(0x10000, 0x11000));
	void *tmp1 = get_frame(0x10000, 0x11000);
	printf("tmp1 = %p\n", tmp1);
	void *tmp2 = get_frame(0x10000, 0x11000);
	printf("tmp2 = %p\n", tmp2);
	printf("frame addr = %p\n", get_frame(0x10000, 0x11000));
	printf("frame addr = %p\n", get_frame(0x10000, 0x11000));
	printf("frames(8) addr = %p\n", get_frames(0x10000, 0x11000, 8));
	printf("frames(1) addr = %p\n", get_frames(0x10000, 0x11000, 1));
	free_frame(tmp1);
	free_frame(tmp2);
	printf("frames(2) addr = %p\n", get_frames(0x10000, 0x11000, 2));
	void *tmp3 = get_page(LOW_MEM, 2);
	printf("get_page addr = %p\n", tmp3);
	free_page(tmp3, 2);
	printf("get_page addr = %p\n", get_page(LOW_MEM, 2));

	void *var0 = kmalloc(4080);
	(void)var0;
	print_memory_list();

	uint32_t *var = kmalloc(sizeof(uint32_t));
	uint32_t *var2 = kmalloc(sizeof(uint32_t));
	uint32_t *var3 = kmalloc(PAGE_SIZE * 2);
	(void)var2;
	(void)var3;

	if (var) {
	*var = 194;

	printf("var = %u\n", *var);
	printf("sizeof var = %u\n", ksize(var));
	} else {
		printf("kmalloc return NULL\n");
	}
	print_memory_list();
	printf("================\n");
	kfree(var0);
	kfree(var);
	kfree(var2);
	kfree(var3);
	print_memory_list();
	var0 = kmalloc(sizeof(long));
	printf("================\n");
	print_memory_list(); */
	get_page(VMALLOC, 10);
	enable_cursor(0, 15);
	poll_keyboard(NULL);
	return (0);
}
