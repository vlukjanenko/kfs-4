/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kernel.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majosue <majosue@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 19:00:59 by majosue           #+#    #+#             */
/*   Updated: 2022/12/23 17:34:17 by majosue          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "stddef.h"
#include "screen.h"
#include "baselib.h"
#include "keyboard.h"
#include "multiboot.h"

extern struct multiboot_info* regholder;

void print_values(void *ptr, void *end, size_t group, size_t groups)
{
	if (ptr == end)
		return;
	printf("%08p: ", ptr);
	for (size_t i = 0; i < groups; i++) {
		printf(" ");
		for (size_t j = 0; j < group; j++) {
			if (ptr < end) {
				printf("%02x", *(unsigned char *)ptr);
				ptr++;
			} else {
				printf("  ");
			}
		}
	}

}

void print_chars(void *ptr, void *end, size_t group, size_t groups)
{
	size_t nbr = group * groups;
	
	if (ptr == end)
		return;
	printf(" ");
	for (size_t i = 0; i < nbr && ptr < end; i++, ptr++) {
		if (isprint(*(unsigned char *)ptr)) {
			printf("%c", *(unsigned char *)ptr);
		} else {
			printf(".");
		}
	}
	printf("\n");
}

void print_memory(void *ptr, void *end)
{
	size_t 	groups = 4; // групп
	size_t	group = 4; // байт в группе
	size_t	h = 0;
	
	for (; ptr < end; ptr += group * groups) // по 4 байта
	{
		print_values(ptr, end, group, groups);
		print_chars(ptr, end, group, groups); // тут всегда пееревод строки в конце
		if (h == VGA_HEIGHT - 1) {
			printf("Press Enter to continue");
			while (1) {
				if ((inb(0x64) & 1) && inb(0x60) == 0x1C) {
					printf("\n");
					break ;
				}
			}
			h = 0;
		}
		h++;
	}
}

void print_stack(void)
{
	char stack_indicator1[5] = {'H','e', 'l', 'l', 'o'};
	uint32_t stack_indicator2 = 0xC0DE;
	(void)stack_indicator1;
	(void)stack_indicator2;
	uint32_t* top = get_stack()->top;
	uint32_t* bottom = get_stack()->bottom;
	print_memory(bottom, top);
	/* int h = 0;
	int t = 0;

	while (top > bottom) {
		t = t % 4;
		if (t == 0)
			printf("%08p:", top);
		printf(" %08x", *top);
		if (t == 3) {
			printf("\n");
			h++;
		}
		if (h == VGA_HEIGHT - 1) {
			printf("Press Enter to continue");
			while (1) {
				if ((inb(0x64) & 1) && inb(0x60) == 0x1C) {
					printf("\n");
					break ;
				}
			}
			h = 0;
		}
		t++;
		top--;
	} */
	//printf("\n");
	//printf("topstack -> %x, bottomstack -> %x, size = %u\n", top, bottom, top - bottom);

}

void	main(void) 
{
	//struct s_terminal	second_terminal;

	disable_cursor();
	/* terminal_initialize(VGA_COLOR_WHITE, VGA_COLOR_BLACK); 
	printf(
	"                                  :::      ::::::::\n"
	"                                :+:      :+:    :+:\n"
	"                              +:+ +:+         +:+\n"
	"                            +#+  +:+       +#+\n"
	"                          +#+#+#+#+#+   +#+\n"
	"                               #+#    #+#\n"
	"                              ###   ########.fr\n"); */
	//terminal_save(&second_terminal);
	terminal_initialize(VGA_COLOR_WHITE, VGA_COLOR_BLACK); 
	
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
/* 	printf("Printf demo:\n");
	printf("printf(\"%%c\", \'C\'): %c\n", 'C');
	printf("printf(\"%%d\", 42): %d\n", 42);
	printf("printf(\"%%o\", 042): %o\n", 042);
	printf("printf(\"%%#x\", 0x42): %#x\n", 0x42);
	printf("printf(\"%%u\", 42): %u\n", 42);
	printf("printf(\"%%s\", \"Hello world!\"): %s\n", "Hello world!");
	printf("printf(\"%%.8p\", printf): %.8p\n", printf);
	printf("\n");
	printf("Use alt+tab to switch screen\n");
	enable_cursor(0, 15);
	printf("flags %#.4x\n", regholder->flags);
	printf("boot device %#.4x\n", regholder->boot_device);
	printf("mmap_length %d\n", regholder->mmap_length);
	printf("mmap_addr %p\n", regholder->mmap_addr);
	 */
	//struct multiboot_mmap_entry *mbi = (struct multiboot_mmap_entry *)regholder->mmap_addr;
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
	print_stack();
	print_stack();
	poll_keyboard(NULL);
}