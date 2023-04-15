/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kernel.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majosue <majosue@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 19:00:59 by majosue           #+#    #+#             */
/*   Updated: 2023/04/14 16:49:54 by majosue          ###   ########.fr       */
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
	disable_cursor();
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
	printf("                              ###   ########.fr\n");
	//print_stack();
	//print_gdt();
	/* 
	printf("Position of symbol end_of_code = %x\n", &end_of_code); // предоположительно конец кода
	printf("frame addr = %p\n", get_frame());
	printf("frame addr = %p\n", get_frame());
	void *tmp = get_frame();
	printf("frame addr = %p\n", tmp);
	printf("frame addr = %p\n", get_frame());
	printf("frame addr = %p\n", get_frame());
	free_frame(tmp);
	printf("frame addr = %p\n", get_frame()); 
	*/
	uint32_t *page_directory = (uint32_t *)get_frame();
	for(int i = 0; i < 1024; i++) {
		// This sets the following flags to the pages:
		//   Supervisor: Only kernel-mode can access them
		//   Write Enabled: It can be both read from and written to
		//   Not Present: The page table is not present
		page_directory[i] = 0x00000002;
	}
	page_directory[1023] = (uint32_t)page_directory | 3; // to self
	printf("page dir address %x\n", (uint32_t)page_directory);
	uint32_t *first_page_table = (uint32_t *)get_frame();
	for(int i = 0; i < 1024; i++)
	{
		// As the address is page aligned, it will always leave 12 bits zeroed.
		// Those bits are used by the attributes ;)
		first_page_table[i] = (i * 0x1000) | 3; // attributes: supervisor level, read/write, present.
	}
	page_directory[0] = (uint32_t)first_page_table | 3;
	load_page_directory(page_directory);
	enable_paging();

	enable_cursor(0, 15);
	poll_keyboard(NULL);
	return (0);
}