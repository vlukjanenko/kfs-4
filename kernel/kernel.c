/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kernel.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majosue <majosue@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 19:00:59 by majosue           #+#    #+#             */
/*   Updated: 2022/11/07 12:25:32 by majosue          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "stddef.h"
#include "screen.h"
#include "baselib.h"
#include "keyboard.h"

void	main(void) 
{
	struct s_terminal	second_terminal;

	disable_cursor();
	terminal_initialize(VGA_COLOR_WHITE, VGA_COLOR_BLACK); 
	printf(
	"                                  :::      ::::::::\n"
	"                                :+:      :+:    :+:\n"
	"                              +:+ +:+         +:+\n"
	"                            +#+  +:+       +#+\n"
	"                          +#+#+#+#+#+   +#+\n"
	"                               #+#    #+#\n"
	"                              ###   ########.fr\n");
	terminal_save(&second_terminal);
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
	printf("Printf demo:\n");
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
	poll_keyboard(&second_terminal);
}