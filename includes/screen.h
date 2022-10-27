#ifndef SCREEN_H
# define SCREEN_H
# include "stddef.h"
# include "stdint.h"

enum vga_color {
	VGA_COLOR_BLACK,
	VGA_COLOR_BLUE,
	VGA_COLOR_GREEN,
	VGA_COLOR_CYAN,
	VGA_COLOR_RED,
	VGA_COLOR_MAGENTA,
	VGA_COLOR_BROWN,
	VGA_COLOR_LIGHT_GREY,
	VGA_COLOR_DARK_GREY,
	VGA_COLOR_LIGHT_BLUE,
	VGA_COLOR_LIGHT_GREEN,
	VGA_COLOR_LIGHT_CYAN,
	VGA_COLOR_LIGHT_RED,
	VGA_COLOR_LIGHT_MAGENTA,
	VGA_COLOR_LIGHT_BROWN,
	VGA_COLOR_WHITE
};

void    terminal_initialize(enum vga_color fg, enum vga_color bg);
void    terminal_setcolor(enum vga_color fg, enum vga_color bg);
void    terminal_putchar(char c);
void    terminal_writestring(const char* data);
void    terminal_write(const char* data, size_t size);

#endif