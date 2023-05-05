/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vmalloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majosue <majosue@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 00:14:34 by majosue           #+#    #+#             */
/*   Updated: 2023/05/05 13:17:28 by majosue          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "memory.h"
#include "stddef.h"
#include "stdint.h"
#include "baselib.h"

typedef struct s_list
{
	void			*content;
	size_t			content_size;
	struct s_list	*next;
	struct s_list	*prev;
}	t_list;

typedef struct s_vmalloc {
	void		*ptr;
	uint32_t	size;
} t_vmalloc;

static t_list *allocations;

t_list	*lstnew(void const *content, size_t content_size)
{
	t_list	*new;

	new = kmalloc(sizeof(*new));
	if (new == NULL)
		return (NULL);
	if (content == NULL)
	{
		new->content = NULL;
		new->content_size = 0;
		new->next = NULL;
		return (new);
	}
	new->content = kmalloc(content_size);
	if (new->content == NULL)
	{
		kfree(new);
		new = (NULL);
		return (new);
	}
	new->content_size = content_size;
	memmove(new->content, content, new->content_size);
	new->next = NULL;
	new->prev = NULL;
	return (new);
}

void	lstadd(t_list **alst, t_list *new)
{
	if (!(*alst)) {
		*alst = new;
		return;
	}
	new->next = (*alst);
	(*alst)->prev = new;
	(*alst) = new;
}

/* размер выделения выравнивается по PAGE_SIZE */
void *vmalloc(uint32_t size)
{
	t_vmalloc	new_alloc;
	t_list		*new_node;
	uint32_t	pages_nbr = align(size, PAGE_SIZE) / PAGE_SIZE;

	new_alloc.ptr = get_page(VMALLOC, pages_nbr);
	if (!new_alloc.ptr)
		return (NULL);
	new_alloc.size = size;

	new_node = lstnew(&new_alloc, sizeof(new_alloc));
	if (!new_node)
		return (NULL);
	lstadd(&allocations, new_node);
	return (new_alloc.ptr);
}

static t_list	*find_alloc(void *ptr)
{
	t_list *head = allocations;

	while (head) {
		t_vmalloc *alloc = head->content;
		if (alloc->ptr == ptr) {
			return (head);
		}
		head = head->next;
	}
	return (NULL);
}

uint32_t vsize(void *ptr)
{
	t_list		*node = find_alloc(ptr);
	t_vmalloc	*alloc;
	size_t		size;

	if (node) {
		alloc = node->content;
			size = alloc->size;
	} else {
		size = -1;
	}
	return (size);
}

void	lstdelone(t_list **alst, void (*del) (void*, size_t))
{
	del((*alst)->content, (*alst)->content_size);
	if((*alst)->prev && (*alst)->next) {
		//printf("come to del middle\n");
		(*alst)->next->prev = (*alst)->prev;
		(*alst)->prev->next = (*alst)->next;
		kfree(*alst);
		*alst = NULL;
	} else if ((*alst)->prev) {
		//printf("come to del end\n");
		(*alst)->prev->next = NULL;
		kfree(*alst);
		*alst = NULL;
	} else if ((*alst)->next) {
		//printf("come to del head\n");
		(*alst) = (*alst)->next;
		kfree((*alst)->prev);
		(*alst)->prev = NULL;
	} else {
		//printf("come to del single element list\n");
		kfree(*alst);
		*alst = NULL;
	}
}

static void clean_node(void* content, size_t size)
{
	t_vmalloc *alloc = content;

	free_page(alloc->ptr, align(alloc->size, PAGE_SIZE) / PAGE_SIZE);
	bzero(content, size);
	kfree(content);
}

void vfree(void *ptr)
{
	t_list *node;

	if (!ptr)
		return;

	node = find_alloc(ptr);
	if (node == allocations) {
	//	printf("come to del frist node\n");
		lstdelone(&allocations, clean_node);
	} else {
		lstdelone(&node, clean_node);
	}
}

void print_vmalloc_list(void)
{
	t_list *head = allocations;

	printf("==vmalloc list==\n");
	while (head) {
		t_vmalloc *alloc = head->content;
		printf("addr = %x, size = %u\n", alloc->ptr, alloc->size);
		head = head->next;
	}
	printf("================\n");
}

