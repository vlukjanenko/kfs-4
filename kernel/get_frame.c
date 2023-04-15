#include "memory.h"
#include "stdint.h"
#include "baselib.h"
#include "stddef.h"

static void frame_allocator_init(unsigned char **arr, uint32_t *size)
{
	uint32_t    frames_in_use;
	// find size of array for bitmask
	*size = max_addr / 0x1000 / 8; 
	frames_in_use = (uint32_t)&end_of_code / 0x1000 + *size / 0x1000;

	printf("Frames in use: %u\n", frames_in_use);
	printf("Bitmask array size: %u\n", *size);

	*arr = (unsigned char *)&end_of_code;
	bzero(*arr, *size);

	// mark used frames
	for (uint32_t i = 0; i < frames_in_use; i++) {
		(*arr)[i / 8] = (*arr)[i / 8] | (1 << (i % 8));
	}
}

void free_frame(void* addr)
{
	unsigned char *arr = (unsigned char *)&end_of_code;
	arr[(uint32_t)addr / 0x1000 / 8] = arr[(uint32_t)addr / 0x1000 / 8] & \
		~((arr[(uint32_t)addr / 0x1000 / 8] & \
			(1 << (uint32_t)addr / 0x1000 % 8)));
}

void *get_frame()
{
	static uint32_t			size;
	static unsigned char	*arr;

	if (!arr)
		frame_allocator_init(&arr, &size);
	for (uint32_t i = 0; i < size; i++) {
		if (arr[i] == 0xFF)
			continue;
		for (int j = 0; j < 8; j++) {
			if (!(arr[i] & (1 << j))) {
				arr[i] = arr[i] | (1 << j);
				return ((void *)(0x1000 * (i * 8 + j)));
			}
		}
	}
	return (NULL);
}