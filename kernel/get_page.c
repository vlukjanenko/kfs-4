/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_page.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majosue <majosue@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/29 23:50:36 by majosue           #+#    #+#             */
/*   Updated: 2023/05/04 14:50:14 by majosue          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "stdint.h"
#include "memory.h"
#include "baselib.h"
#include "stddef.h"

static void	*heap_start;	// сделать глобальными?
static void	*heap_end;		//
static void *vmalloc_end;

static void	get_page_init()
{
	heap_start = get_heap_start();
	heap_end = get_heap_end();
	vmalloc_end = get_vmalloc_end();
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

/* https://wiki.osdev.org/Paging */
uint32_t *get_page_table_entry(void *virtualaddr)
{
	uint32_t pdindex = (uint32_t)virtualaddr >> 22;
	uint32_t ptindex = (uint32_t)virtualaddr >> 12 & 0x03FF;
	// Here you need to check whether the PD entry is present.
	uint32_t *pt = ((uint32_t *)0xFFC00000) + (0x400 * pdindex);
	// Here you need to check whether the PT entry is present.
	uint32_t *pt_entry = &pt[ptindex];
	return (pt_entry);
}

int is_page_maped(void *ptr)
{
	return (*get_page_table_entry(ptr) & 1);
}

static void *__get_virt_page(void **start, void* end, uint32_t nbr)
{
	uint32_t i = 0;
	void *continuous_pages = *start;

	while (*start < end && !is_page_maped(*start)) {
		if (i >= nbr) {
			return (continuous_pages);
		}
		i++;
		(*start) += PAGE_SIZE;
	}
	return (NULL);
}

uint32_t *get_non_continuous_frames(uint32_t nbr)
{
	uint32_t *frames = kmalloc(sizeof(uint32_t) * nbr);
	if (!frames)
		return (NULL);
	for (uint32_t i = 0; i < nbr; i++) {
		frames[i] = (uint32_t)get_frame((uint32_t)(heap_end - PAGE_OFFSET) / PAGE_SIZE, align(MAX_ADDR, PAGE_SIZE) / PAGE_SIZE);
		if (!frames[i]) {
			kfree(frames);
			frames = NULL;
			// надо бы по хорошему и странички обратно пометить cвободными
			break;
		}
	}
	return (frames);
}

static void map_pages(void *pages, uint32_t *frames, uint32_t nbr)
{
	for (uint32_t i = 0; i < nbr; i++, pages += PAGE_SIZE) {
		*get_page_table_entry(pages) = frames[i] | 3;
	}
	refresh_map();
}

static void	*get_page_in_highmem(uint32_t nbr) // делаем так чтоб она не брала странички в lowmem зоне
{
	void		*start;
	void		*end;
	void		*pages;
	uint32_t	*frames;

	if (!vmalloc_end)
		get_page_init();
	start = heap_end;
	end = vmalloc_end;
	pages = NULL;
	frames = NULL;
	while (start < end && pages == NULL) {
		if (!is_page_maped(start)) {
			pages = __get_virt_page(&start, end, nbr);
		}
		start += PAGE_SIZE;
	}
	if (pages == NULL)
		return (NULL);
	frames = get_non_continuous_frames(nbr);
	if (frames == NULL)
		return (NULL);
	map_pages(pages, frames, nbr);
	kfree(frames);
	return (pages);
}

void *get_page(uint32_t flags, uint32_t nbr)
{
	if (flags == LOW_MEM)
		return (get_page_in_lowmem(nbr));
	if (flags == VMALLOC)
		return (get_page_in_highmem(nbr));
	return (NULL);
}

static void free_page_in_highmem(void *page, uint32_t nbr)
{
	for (uint32_t i = 0; i < nbr; i++, page += PAGE_SIZE)
	{
		uint32_t *table_entry = get_page_table_entry(page);

		free_frame((void*)(*table_entry));
		*table_entry = 0;
	}
	refresh_map();
}

void free_page(void *page, uint32_t nbr)
{
	if (page >= heap_start && page < heap_end)
		free_page_in_lowmem(page, nbr);
	if (page >= heap_end && page < vmalloc_end)
		free_page_in_highmem(page, nbr);
}

void print_pages(void *page, uint32_t nbr)
{
	for (uint32_t i = 0; i < nbr; i++, page += PAGE_SIZE) {
		printf("%x => %x\n", page, *get_page_table_entry(page) & ~0xFFF);
	}
}
