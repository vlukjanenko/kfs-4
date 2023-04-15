ENTRY(_start)
SECTIONS
{
	_gdt_start = 0x00000800;
	.gdt _gdt_start :
	{
		KEEP(*(.gdt))
	}
	. = 1M;
	_ptr = .;
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
	end_of_code = .; /* тут конец нашего кода и дальше память свободна? */
}
