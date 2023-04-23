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

	.bootstrap.data ALIGN(4K) : {
		*(.multiboot)
		*(.bootstrap.data)
	}
	.bootstrap.bss ALIGN(4k) : {
		*(.tables)
		*(.bootstrap.bss)
	}
	.boot.text ALIGN(4k) : {
		*(.boot.text)
		boot/get_memory_map.o
		boot/paging.o
		kernel/paging.o
	}
	. += 0xC0000000;
	.text ALIGN (4K) : AT (ADDR (.text) - 0xC0000000)
	{
		*(.text)
	}
	.rodata ALIGN (4K) : AT (ADDR (.rodata) - 0xC0000000)
	{
		*(.rodata)
	}
	.data ALIGN (4K) : AT (ADDR (.data) - 0xC0000000)
	{
		*(.data)
	}
	.bss ALIGN (4K) : AT (ADDR (.bss) - 0xC0000000)
	{
		*(COMMON)
		*(.bss)
	}
	. = ALIGN(4K);
	end_of_code = .; /* тут конец нашего кода и дальше память свободна? */
}
