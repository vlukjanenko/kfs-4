#include "stddef.h"
#include "screen.h"
#include "GDT.h"

uint8_t read_port(int port);
void write_port(int port, int value);

/* with delay to see scroll in progress */

void terminal_writestring_with_delay(const char* data)
{
	
	for (size_t i = 0; i < 10000000; i++);
	terminal_writestring(data);
}
 
void main(void) 
{
	terminal_initialize(VGA_COLOR_DARK_GREY, VGA_COLOR_BLACK); 
	terminal_writestring("                                  :::      ::::::::\n");
	terminal_writestring("                                :+:      :+:    :+:\n");
	terminal_writestring("                              +:+ +:+         +:+\n");
	terminal_writestring("                            +#+  +:+       +#+\n");
	terminal_writestring("                          +#+#+#+#+#+   +#+\n");
	terminal_writestring("                               #+#    #+#\n");
	terminal_writestring("                              ###   ########.fr\n");
	terminal_setcolor(VGA_COLOR_GREEN, VGA_COLOR_BLACK);
	terminal_writestring("Scroll demo:\n");
	for (size_t i = 0; i < 15; i++) {
		terminal_writestring_with_delay("Next str\n");
	}
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
	terminal_setcolor(VGA_COLOR_LIGHT_MAGENTA, VGA_COLOR_BLACK);
	terminal_writestring_with_delay("                              ###   ########.fr\n");
	for (size_t i = 0; i < 17; i++) {
		terminal_writestring_with_delay("\n");
	}
	write_port(0x3D4, 0x0A);
	write_port(0x3D5, 0x20);
	while (1) {
		if ((read_port(0x64) & 1)) {
			uint8_t code = read_port(0x60);
			if (code == 0x2c) {
			terminal_writestring("Z pressed\n");
			}
		}
	}
}