/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_frame.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majosue <majosue@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/15 10:50:09 by majosue           #+#    #+#             */
/*   Updated: 2023/04/30 14:08:23 by majosue          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "memory.h"
#include "stdint.h"
#include "baselib.h"
#include "stddef.h"

static unsigned char	*bitmask;
static uint32_t			bm_size;
static uint32_t			max_pfn;

static void frame_allocator_init()
{
	bitmask = get_bitmask();
	bm_size = get_bitmask_size();
	max_pfn = align(MAX_ADDR, PAGE_SIZE) / PAGE_SIZE;
}

static void pfn_to_idxs(uint32_t pfn, uint32_t *i, uint32_t *j)
{
	*i = pfn / 8;
	*j = pfn % 8;
}

int frame_status(uint32_t addr)
{
	uint32_t pfn =  (uint32_t)addr / PAGE_SIZE;
	uint32_t i;
	uint32_t j;

	pfn_to_idxs(pfn, &i, &j);
	return (bitmask[i] & (1 << j));
}

void free_frame(void* addr)
{
	uint32_t pfn = (uint32_t)addr / PAGE_SIZE;
	uint32_t i;
	uint32_t j;

	pfn_to_idxs(pfn, &i, &j);
	bitmask[i] = bitmask[i] & ~((bitmask[i] & (1 << j)));
}

static void *mark_frames_occupied(uint32_t i, int j, uint32_t frames)
{
	void* ret_value = (void *)(PAGE_SIZE * (i * 8 + j));
	uint32_t nbr = 0;

	for ( ; nbr < frames; i++) {
		for (; j < 8 && nbr < frames; j++) {
			bitmask[i] = bitmask[i] | (1 << j);
			nbr++;
		}
		j = 0;
	}
	return (ret_value);
}

static int in_range(uint32_t end_frame, uint32_t i, uint32_t j)
{
	uint32_t pfn = i * 8 + j;

	if (pfn < max_pfn && pfn < end_frame)
		return (1);
	return (0);
}

static void *__get_frames(uint32_t end_frame, uint32_t *i, \
							uint32_t *j, uint32_t frames)
{
	uint32_t nbr = 0;
	uint32_t i_begin = *i;
	uint32_t j_begin = *j;

	for (; in_range(end_frame, *i, *j); (*i)++) {
		for (; in_range(end_frame, *i, *j); (*j)++) {
			if (!(bitmask[*i] & (1 << *j)))
				nbr++;
			else
				return (NULL);
			if (nbr == frames)
				return mark_frames_occupied(i_begin, j_begin, frames);
		}
		*j = 0;
	}
	return (NULL);
}

void *get_frames(uint32_t start_frame, uint32_t end_frame, uint32_t frames)
{
	void	*ret_value = NULL;
	uint32_t i;
	uint32_t j;

	if (!bitmask) {
		frame_allocator_init();
	}
	pfn_to_idxs(start_frame, &i, &j);
	for (; in_range(end_frame, i, j) && !ret_value; i++) {
		if (bitmask[i] == 0xFF)
			continue;
		for (; in_range(end_frame, i, j) && !ret_value; j++) {
			if (!(bitmask[i] & (1 << j))) {
				ret_value = __get_frames(end_frame, &i, &j, frames);
			}
		}
		j = 0;
	}
	return (ret_value);
}

void *get_frame(uint32_t start_frame, uint32_t end_frame)
{
	return (get_frames(start_frame, end_frame, 1));
}
