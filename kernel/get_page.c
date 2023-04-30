/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_page.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majosue <majosue@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/29 23:50:36 by majosue           #+#    #+#             */
/*   Updated: 2023/04/30 00:19:41 by majosue          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "stdint.h"
#include "memory.h"
#include "baselib.h"
#include "stddef.h"

static void	*heap_start;	// сделать глобальными?
static void	*heap_end;		//

static void	get_page_init()
{
	heap_start = get_heap_start();
	heap_end = get_heap_end();
}

static void	*get_page_in_lowmem(uint32_t nbr)
{
	void	*frames;

	if (!heap_start)
		get_page_init();
	frames = get_frames((uint32_t)(heap_start - PAGE_OFFSET) / PAGE_SIZE, \
						(uint32_t)(heap_end - PAGE_OFFSET) / PAGE_SIZE, nbr);
	if (frames)
		return (frames + PAGE_OFFSET);
	return (NULL);
}

static void free_page_in_lowmem(void *page, uint32_t nbr)
{
	for (uint32_t i = 0; i < nbr; i++)
	{
		free_frame(page - PAGE_OFFSET);
		page = page + PAGE_SIZE;
	}
}

void *get_page(uint32_t flags, uint32_t nbr)
{
	if (flags == LOW_MEM)
		return (get_page_in_lowmem(nbr));
	return (NULL);
}

void free_page(void *page, uint32_t nbr)
{
	if (page >= heap_start && page < heap_end)
		free_page_in_lowmem(page, nbr);
}
