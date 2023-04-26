#include "stdint.h"

struct s_pages {
	void*		addr;
	uint32_t	nbr;
	uint32_t	flags;
};

void *get_page(uint32_t nbr)
{
	// изначально всё пространство 0xE000000 - end_of_heap доступно
	// берем страницу
	frame = get_frames(nbr);


}
