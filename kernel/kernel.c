/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kernel.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majosue <majosue@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 19:00:59 by majosue           #+#    #+#             */
/*   Updated: 2023/04/15 11:59:14 by majosue          ###   ########.fr       */
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
	void* top = &stack_top;
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

	printf("Position of symbol end_of_code = %x\n", &end_of_code); // предоположительно конец кода

	printf("frame addr = %p\n", get_frame());
	printf("frame addr = %p\n", get_frame());
	void *tmp1 = get_frame();
	printf("tmp1 = %p\n", tmp1);
	void *tmp2 = get_frame();
	printf("tmp2 = %p\n", tmp2);
	printf("frame addr = %p\n", get_frame());
	printf("frame addr = %p\n", get_frame());
	printf("frames(8) addr = %p\n", get_frames(8));
	free_frame(tmp1);
	free_frame(tmp2);
	printf("frames(2) addr = %p\n", get_frames(2));

	//turn_on_paging(); //запускаем из ассемблера до перехода в 0xC0000000
	enable_cursor(0, 15);
	poll_keyboard(NULL);
	return (0);
}
