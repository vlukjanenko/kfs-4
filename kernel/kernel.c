#include "stddef.h"
#include "screen.h"
#include "GDT.h"
#include "baselib.h"
#include "keyboard.h"

/* with delay to see scroll in progress */

void terminal_writestring_with_delay(const char* data)
{
	for (size_t i = 0; i < 10000000; i++);
	terminal_writestring(data);
}
 
void main(void) 
{
	struct s_terminal second_terminal;

	terminal_initialize(VGA_COLOR_WHITE, VGA_COLOR_BLACK); 
	terminal_writestring("                                  :::      ::::::::\n");
	terminal_writestring("                                :+:      :+:    :+:\n");
	terminal_writestring("                              +:+ +:+         +:+\n");
	terminal_writestring("                            +#+  +:+       +#+\n");
	terminal_writestring("                          +#+#+#+#+#+   +#+\n");
	terminal_writestring("                               #+#    #+#\n");
	terminal_writestring("                              ###   ########.fr\n");
	terminal_save(&second_terminal);
	terminal_setcolor(VGA_COLOR_GREEN, VGA_COLOR_BLACK);
	enable_cursor(0, 15);
	terminal_writestring("Welcome!!!\n");
	for (size_t i = 0; i < 15; i++) {
		terminal_writestring_with_delay("Welcome!!! ");
	}
	terminal_putchar('\n');
	terminal_setcolor(VGA_COLOR_RED, VGA_COLOR_BLACK);
	terminal_writestring_with_delay("                                  :::      ::::::::\n");
	terminal_setcolor(VGA_COLOR_GREEN, VGA_COLOR_BLACK);
	terminal_writestring_with_delay("                                :+:      :+:    :+:\n");
	terminal_setcolor(VGA_COLOR_CYAN, VGA_COLOR_BLACK);
	terminal_writestring_with_delay("                              +:+ +:+         +:+\n");
	terminal_setcolor(VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK);
	terminal_writestring_with_delay("                            +#+  +:+       +#+\n");
	terminal_setcolor(VGA_COLOR_BLUE, VGA_COLOR_BLACK);
	terminal_writestring_with_delay("                          +#+#+#+#+#+   +#+\n");
	terminal_setcolor(VGA_COLOR_LIGHT_BLUE, VGA_COLOR_BLACK);
	terminal_writestring_with_delay("                               #+#    #+#\n");
	terminal_setcolor(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
	terminal_writestring_with_delay("                              ###   ########.fr\n");
	for (size_t i = 0; i < 17; i++) {
		terminal_writestring_with_delay("\n");
	}
	poll_keyboard(&second_terminal);
}