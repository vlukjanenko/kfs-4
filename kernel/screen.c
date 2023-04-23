/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   screen.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majosue <majosue@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 19:03:14 by majosue           #+#    #+#             */
/*   Updated: 2023/01/31 14:46:06 by majosue          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "screen.h"
#include "stddef.h"
#include "stdint.h"
#include "baselib.h"

static size_t       terminal_row;
static size_t       terminal_column;
static uint8_t      terminal_color;
static uint16_t*    terminal_buffer;
static char			terminal_input_buffer[SCREEN_SIZE];
static size_t		terminal_i_b_pos;

/* for scroll
	in normal mode
	Data printed to terminal_buffer also saved to terminal_whole_buffer
	in scroll mode
	we change current_top_line and copy saved data to terminal_buffer
*/
static uint16_t		terminal_whole_buffer[SCREEN_SIZE * BUFF_SIZE]; // here we hold screens * number
static uint16_t		* const screen_top_line = &terminal_whole_buffer[SCREEN_SIZE * (BUFF_SIZE - 1)]; // line in buffer that correspond screen first line
static uint16_t		*current_top_line = &terminal_whole_buffer[SCREEN_SIZE * (BUFF_SIZE - 1)];		// line in buffer that correspond screen first line
static uint16_t		*buffer_top = &terminal_whole_buffer[SCREEN_SIZE * (BUFF_SIZE - 1)]; // hold oldest (most top) line

void disable_cursor()
{
	outb(0x3D4, 0x0A);
	outb(0x3D5, 0x20);
}

void update_cursor(int x, int y)
{
	uint16_t pos = y * VGA_WIDTH + x;

	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t) (pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

void enable_cursor(uint8_t cursor_start, uint8_t cursor_end)
{
	outb(0x3D4, 0x0A);
	outb(0x3D5, (inb(0x3D5) & 0xC0) | cursor_start);

	outb(0x3D4, 0x0B);
	outb(0x3D5, (inb(0x3D5) & 0xE0) | cursor_end);
}


void scroll_up()
{
	if (current_top_line > buffer_top) {
		disable_cursor();
		current_top_line -= VGA_WIDTH;
		memmove(terminal_buffer, current_top_line, SCREEN_SIZE_BYTES);
	}
}

void scroll_down()
{
	if (current_top_line < screen_top_line) {
		current_top_line += VGA_WIDTH;
		memmove(terminal_buffer, current_top_line, SCREEN_SIZE_BYTES);
		if (current_top_line == screen_top_line) {
			enable_cursor(0, 15);
		}
	}
}

void reset_scroll()
{
	if (current_top_line < screen_top_line) {
		current_top_line = screen_top_line;
		memmove(terminal_buffer, current_top_line, SCREEN_SIZE_BYTES);
		enable_cursor(0, 15);
	}
}

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg)
{
	return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color)
{
	return (uint16_t) uc | (uint16_t) color << 8;
}

void terminal_initialize(enum vga_color fg, enum vga_color bg)
{
	terminal_row = 0;
	terminal_column = 0;
	terminal_i_b_pos = 0;
	terminal_setcolor(fg, bg);
	terminal_buffer = (uint16_t*) 0xC00B8000;
	buffer_top = screen_top_line;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
			screen_top_line[index] = vga_entry(' ', terminal_color);
		}
	}
	update_cursor(0, 0);
}

void terminal_clear()
{
	terminal_initialize(terminal_color & 0xf, terminal_color >> 4);
}

void terminal_setcolor(enum vga_color fg, enum vga_color bg)
{
	terminal_color = vga_entry_color(fg, bg);
}

static void terminal_scroll()
{
	memmove(terminal_whole_buffer,
		terminal_whole_buffer + VGA_WIDTH,
			(VGA_WIDTH * VGA_HEIGHT * BUFF_SIZE * sizeof(uint16_t)) -
					(VGA_WIDTH * sizeof(uint16_t)));
	buffer_top = buffer_top == terminal_whole_buffer ? buffer_top : buffer_top - VGA_WIDTH;

	/* for (size_t y = 0; y < VGA_HEIGHT - 1; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = terminal_buffer[index + VGA_WIDTH];
		}
	} */

	memmove (terminal_buffer, terminal_buffer + VGA_WIDTH, (VGA_WIDTH * VGA_HEIGHT * sizeof(uint16_t)) -
					(VGA_WIDTH * sizeof(uint16_t)));

	for (size_t x = 0; x < VGA_WIDTH; x++) {	// wipe last line;
		const size_t index = (VGA_HEIGHT - 1) * VGA_WIDTH + x;
		terminal_buffer[index] = vga_entry(' ', terminal_color);
		screen_top_line[index] = vga_entry(' ', terminal_color);
	}
	terminal_column = 0;
}

static void terminal_cr()
{
	terminal_column = 0;
	if (terminal_row == VGA_HEIGHT - 1) {
		terminal_scroll();
		return;
	}
	terminal_row++;
}

static void terminal_putentryat(char c, uint8_t color, \
								size_t x, size_t y)
{
	const size_t index = y * VGA_WIDTH + x;

	terminal_buffer[index] = vga_entry(c, color);
	screen_top_line[index] = terminal_buffer[index];
}

void terminal_putchar(char c)
{
	if (c == '\n') {
		terminal_cr();
		update_cursor(terminal_column, terminal_row);
		return;
	}
	terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
	if (++terminal_column == VGA_WIDTH) {
		terminal_column = 0;
		if (terminal_row == VGA_HEIGHT - 1) {
			terminal_scroll();
		} else {
			terminal_row++;
		}
	}
	update_cursor(terminal_column, terminal_row);
}

void terminal_putchar_to_buffer(char c)
{
	if (terminal_i_b_pos == VGA_WIDTH * VGA_HEIGHT - 1) { // need last byte for null-terminator;
		terminal_writestring("Error: input buffer overflow\n");
		return;
	}
	terminal_input_buffer[terminal_i_b_pos++] = c;
	terminal_putchar(c);
}

char *terminal_get_input_buffer()
{
	terminal_input_buffer[terminal_i_b_pos] = '\0';
	return (terminal_input_buffer);
}

void terminal_reset_input_buffer() {
	terminal_i_b_pos = 0;
}


void terminal_write(const char* data, size_t size)
{
	for (size_t i = 0; i < size; i++)
		terminal_putchar(data[i]);
}

void terminal_writestring(const char* data)
{
	terminal_write(data, strlen(data));
}

void terminal_del(void) {
	int pos = terminal_row * VGA_WIDTH + terminal_column;

	if (pos > 0 && terminal_i_b_pos > 0) {
		pos--;
		terminal_i_b_pos--;
		terminal_row = pos / VGA_WIDTH;
		terminal_column = pos % VGA_WIDTH;
		terminal_buffer[pos] = vga_entry(' ', terminal_color);
		screen_top_line[pos] = vga_entry(' ', terminal_color);
		update_cursor(terminal_column, terminal_row);
	}
}

void terminal_save(struct s_terminal *term)
{
	term->row = terminal_row;
	term->column = terminal_column;
	term->color = terminal_color;
	memmove(term->buffer, terminal_buffer, \
		sizeof(*terminal_buffer) * VGA_WIDTH * VGA_HEIGHT);
}

void terminal_restore(struct s_terminal *term)
{
	terminal_row = term->row;
	terminal_column = term->column;
	terminal_color = term->color;
	memmove(terminal_buffer, term->buffer, \
		sizeof(*terminal_buffer) * VGA_WIDTH * VGA_HEIGHT);
	update_cursor(terminal_column, terminal_row);
}
