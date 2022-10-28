#include "GDT.h"

uint64_t g_table[5] = {
	0x0000000000000000,
	0x00CF9A000000FFFF,
	0x00CF92000000FFFF,
	0x00CFFA000000FFFF,
	0x00CFF2000000FFFF
};

struct GDTR gdtr = {
	0x4,
	(uint32_t)g_table
};