#ifndef SCREEN_H
# define SCREEN_H
# include "stddef.h"
# include "stdint.h"

# define VGA_WIDTH	80
# define VGA_HEIGHT	25

struct s_terminal {
	size_t		row;
	size_t		column;
	uint8_t		color;
	uint16_t	buffer[VGA_HEIGHT * VGA_WIDTH];
};

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
void	disable_cursor();
void	enable_cursor(uint8_t cursor_start, uint8_t cursor_end);
void	terminal_del(void);
void	terminal_save(struct s_terminal *term);
void	terminal_restore(struct s_terminal *term);



#endif