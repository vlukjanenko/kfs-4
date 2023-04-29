#include "stdint.h"
#include "memory.h"
#include "baselib.h"
#include "stddef.h"

#define LOW_MEM	0
#define VMALOC	1


static void *get_page_in_lowmem(uint32_t nbr)
{
	void frames = get_frames(heap_start, heap_end);

	if (frames)
		return (frames + PAGE_OFFSET);
	return (NULL);
}

void *get_page(uint32_t flags, uint32_t nbr)
{
	if (flags == LOW_MEM)
		return (get_page_in_lowmem(nbr));
	return (NULL);
}
