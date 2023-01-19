/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kernel.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majosue <majosue@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 19:00:59 by majosue           #+#    #+#             */
/*   Updated: 2023/01/19 16:05:35 by majosue          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "stddef.h"
#include "screen.h"
#include "baselib.h"
#include "keyboard.h"
#include "multiboot.h"

extern struct multiboot_info* regholder;

void print_stack(void)
{
	char stack_indicator1[5] = {'H','e', 'l', 'l', 'o'};
	uint32_t stack_indicator2 = 0xC0DE;
	(void)stack_indicator1;
	(void)stack_indicator2;
	void* top = get_stack()->top;
	void* bottom = get_stack()->bottom;
	print_memory(bottom, top - bottom);
}

void	main(void) 
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
	
	/* 
	multiboot_memory_map_t *mmap;
	 for (mmap = (multiboot_memory_map_t *) regholder->mmap_addr;
           (unsigned long) mmap < regholder->mmap_addr + regholder->mmap_length;
           mmap = (multiboot_memory_map_t *) ((unsigned long) mmap
                                    + mmap->size + sizeof (mmap->size)))
        printf (" size = %#x, base_addr = %.8p,"
                " length = %#.8x, type = %x\n",
                (unsigned) mmap->size,
                (unsigned) (mmap->addr),
                (unsigned) (mmap->len),
                (unsigned) mmap->type); 
	*/

	print_stack();
	//disable_cursor();
	enable_cursor(0, 15);
	
	poll_keyboard(NULL);
}