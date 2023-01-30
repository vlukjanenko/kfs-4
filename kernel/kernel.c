/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kernel.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majosue <majosue@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 19:00:59 by majosue           #+#    #+#             */
/*   Updated: 2023/01/29 22:19:18 by majosue          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "stddef.h"
#include "screen.h"
#include "baselib.h"
#include "keyboard.h"
#include "multiboot.h"
#include "memory.h"

extern struct multiboot_info* mbi;

void print_stack(void)
{
	void* top = max_addr;
	void* bottom;
	char stack_indicator1[8] = {'b','o', 't', 't', 'o', 'm' , '.', '.'};
	(void)stack_indicator1;
	asm ("movl %%esp, %0" : "=r" (bottom));
	print_memory(bottom, top - bottom);
}

void print_gdb()
{
	uint64_t *entry = (uint64_t *)0x800;

	printf("\nGDT\n\n");
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
	//printf("\n");
	printf("Descriptors table:\n");
	for (int i = 0; i < 6; i++) {
		printf("%010p: %016llx\n", entry, *entry);
		/* printf("Base: %08x ", (*entry >> 40 & 0xff000000)
							|(*entry >>  8 & 0x00ff0000)
							|(*entry >> 16 & 0x0000ffff));
		printf("Limit: %x ", (*entry >> 32 & 0x000f0000)
									 |(*entry & 0x0000ffff)); */
		/* printf("Flags: %llx\n", (*entry >> 52 & 0xf));
		printf("G: %llx, ", (*entry >> 52 & 0x8) >> 3);
		printf("DB: %llx, ", (*entry >> 52 & 0x4) >> 2);
		printf("L: %llx\n", (*entry >> 52 & 0x2) >> 1);
		printf("Access Byte: %llx\n",  (*entry >> 40 & 0xff));
		printf("P: %llx, DPL: %llx, S: %llx, E: %llx, DC: %llx, RW: %llx, A: %llx\n", 
		(*entry >> 40 & 0x80) >> 7,
		(*entry >> 40 & 0x60) >> 5,
		(*entry >> 40 & 0x10) >> 4,
		(*entry >> 40 & 0x8) >> 3,
		(*entry >> 40 & 0x4) >> 2,
		(*entry >> 40 & 0x2) >> 1,
		(*entry >> 40 & 0x1)); */

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
	
	/* multiboot_memory_map_t *mmap;
	 for (mmap = (multiboot_memory_map_t *) mbi->mmap_addr;
           (unsigned long) mmap < mbi->mmap_addr + mbi->mmap_length;
           mmap = (multiboot_memory_map_t *) ((unsigned long) mmap
                                    + mmap->size + sizeof (mmap->size)))
        printf (" size = %#x, base_addr = %.8p,"
                " length = %#.8x, type = %x\n",
                (unsigned) mmap->size,
                (unsigned) (mmap->addr),
                (unsigned) (mmap->len),
                (unsigned) mmap->type); 
	
 */
	printf("max_addr = %010p\n", max_addr);
	print_stack();
	//print_gdb();
	//disable_cursor();
	enable_cursor(0, 15);
	
	poll_keyboard(NULL);
	return (0);
}