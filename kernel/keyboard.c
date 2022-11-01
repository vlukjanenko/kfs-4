#include "baselib.h"
#include "stdint.h"
#include "screen.h"
#include "keyboard.h"

static uint8_t shift_state = 0;
static uint8_t lalt_state = 0;
static struct s_terminal *second_terminal;

static uint8_t symbol_table[256] = { 0, 0,
    '1', '2', '3','4','5', '6', '7', '8', '9', '0',
    '-', '=', 0, 0, 'q', 'w', 'e', 'r', 't', 'y', 'u',
    'i', 'o', 'p', '[', ']', 0, 0, 'a', 's', 'd', 'f',
    'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\',
    'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.','/', 0,
    0, 0, ' '};

static void process_key(uint8_t code)
{
    if (isprint(symbol_table[code])) {
		terminal_putchar(symbol_table[code] - \
            isalpha(symbol_table[code]) * shift_state * 32);
    }
    if (code == 0x1C) {
        terminal_putchar('\n');
    }
    if (code == 0x2A || code == 0x36) {
        shift_state = 1;
    }
    if (code == 0xB6 || code == 0xAA) {
        shift_state = 0;
    }
    if (code == 0x0E) {
        terminal_del();
    }
    if (code == 0x38) {
        lalt_state = 1;
    }
    if (code == 0xB8) {
        lalt_state = 0;
    }
    if (lalt_state && code == 0x0F) {
        struct s_terminal t;
        
        t.color = second_terminal->color;
        t.column = second_terminal->column;
        t.row = second_terminal->row;
        memmove(t.buffer, second_terminal->buffer, sizeof(uint16_t) * VGA_HEIGHT * VGA_WIDTH);
        terminal_save(second_terminal);
        terminal_restore(&t);
    }

}

void poll_keyboard(void* val)
{
    second_terminal = val;
    while (1) {
            if ((inb(0x64) & 1)) {
                uint8_t code = inb(0x60);
                process_key(code);
        }
    }
}
