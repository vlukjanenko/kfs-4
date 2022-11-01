/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   screen.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majosue <majosue@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 19:03:14 by majosue           #+#    #+#             */
/*   Updated: 2022/11/01 19:05:35 by majosue          ###   ########.fr       */
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
	terminal_color = vga_entry_color(fg, bg);
	terminal_buffer = (uint16_t*) 0xB8000;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
}
 
void terminal_setcolor(enum vga_color fg, enum vga_color bg) 
{
	terminal_color = vga_entry_color(fg, bg);
}

static void terminal_scroll()
{
	for (size_t y = 0; y < VGA_HEIGHT - 1; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = terminal_buffer[index + VGA_WIDTH];
		}
	}
	for (size_t x = 0; x < VGA_WIDTH; x++) {
		const size_t index = (VGA_HEIGHT - 1) * VGA_WIDTH + x;
		terminal_buffer[index] = vga_entry(' ', terminal_color);
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
	if (pos > 0) {
		pos--;
		terminal_row = pos / VGA_WIDTH;
		terminal_column = pos % VGA_WIDTH;
		terminal_buffer[pos] = vga_entry(' ', terminal_color);
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