/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kmalloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majosue <majosue@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/01 02:39:38 by majosue           #+#    #+#             */
/*   Updated: 2023/05/01 03:10:42 by majosue          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "memory.h"
#include "stddef.h"
#include "stdint.h"
#include "baselib.h"

typedef struct s_block {
	void		*next;
	void		*prev;
	uint32_t	status;
	uint32_t	size;
} t_block;

static t_block *block_list;

uint32_t ksize(void *ptr)
{
	t_block *block;

	block = ptr - sizeof(t_block);
	return (block->size);
}

static void print_block(t_block *blk)
{
	printf("===block: addr %p\nprev %p\nnext %p\nstatus %u\nsize %u\n",
blk, blk->prev, blk->next, blk->status, blk->size);
}

void print_memory_list()
{
	t_block *head;

	head = block_list;
	while (head) {
		print_block(head);
		head = head->next;
	}
}

static void *find_free_block(t_block *lst, uint32_t size_with_header, uint32_t size)
{
	t_block *new_block;

	if (lst == NULL)
		return (NULL);
	while (lst) {
		if (lst->size == size_with_header - sizeof(t_block)) { // если 1:1 к свободному блоку просто меняем статус
			lst->status = 1;
			lst->size = size; // при освобождении выровнять по заголовку!!!
			return (lst + 1);
		}
		if (lst->size > size_with_header) {
			new_block = (void *)lst + lst->size + sizeof(t_block) - size_with_header;
			new_block->status = 1;
			new_block->size = size; // при освобождении выровнять по заголовку!!!
			lst->size -= size_with_header;
			new_block->next = lst->next;
			new_block->prev = lst;
			lst->next = new_block;
			return (new_block + 1);
		}
		lst = lst->next;
	}
	return (NULL);
}

static void add_block_to_list(void *pages, uint32_t size_with_header)
{
	t_block	*new_block;
	t_block *head;

	new_block = (t_block *)pages;
	new_block->size = size_with_header - sizeof(t_block);
	new_block->prev = NULL;
	new_block->next = NULL;
	new_block->status = 0;
	if (!block_list) {
		block_list = new_block;
	} else {
		head = block_list;
		while (head->next)
			head = head->next;
		head->next = new_block;
		new_block->prev = head;
	}
}

static t_block*	request_more_memory(uint32_t size_with_header)
{
	void		*pages;

	pages = get_page(LOW_MEM, align(size_with_header, PAGE_SIZE) / PAGE_SIZE);
	if (!pages)
		return (NULL);
	add_block_to_list(pages, align(size_with_header, PAGE_SIZE));
	return (pages);
}

void *kmalloc(uint32_t size)
{
	void		*result = NULL;
	t_block		*new_block;
	uint32_t	size_with_header;

	size_with_header = align(size + sizeof(t_block), sizeof(t_block));
	result = find_free_block(block_list, size_with_header, size);
	if (!result) {
		new_block = request_more_memory(size_with_header);
		result = find_free_block(new_block, size_with_header, size);
	}
	return (result);
}
