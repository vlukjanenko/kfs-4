ENTRY(_start)
SECTIONS
{
	_gdt_start = 0x00000800;
	. = 1M;

	.gdt _gdt_start :
	{
		KEEP(*(.gdt))
	}
	.text BLOCK(4K) : ALIGN(4K)
	{
		*(.multiboot)
		*(.text)			
	}
	.rodata BLOCK(4K) : ALIGN(4K)
	{
		*(.rodata)
	}
	.data BLOCK(4K) : ALIGN(4K)
	{
		*(.data)
	}
	.bss BLOCK(4K) : ALIGN(4K)
	{
		*(COMMON)
		*(.bss)
	}
	. = ALIGN(4K);
	end_of_code = ABSOLUTE(.); /* тут конец нашего кода и дальше память свободна? */
}