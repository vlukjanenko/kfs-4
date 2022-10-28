#ifndef GDT_H
# define GDT_H

# include "stdint.h"

struct __attribute__((packed)) GDTR
{
	uint16_t limit;
	uint32_t base;
};

extern uint64_t g_table[5];
extern struct GDTR gdtr;

void init_gdt();

#endif