/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_memory.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majosue <majosue@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 16:03:49 by majosue           #+#    #+#             */
/*   Updated: 2023/01/19 16:05:31 by majosue          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "stddef.h"
#include "screen.h"
#include "baselib.h"

static void print_values(void *ptr, void *end, size_t group, size_t groups)
{
	if (ptr == end)
		return;
	printf("%08p: ", ptr);
	for (size_t i = 0; i < groups; i++) {
		printf(" ");
		for (size_t j = 0; j < group; j++) {
			if (ptr < end) {
				printf("%02x", *(unsigned char *)ptr);
				ptr++;
			} else {
				printf("  ");
			}
		}
	}

}

static void print_chars(void *ptr, void *end, size_t group, size_t groups)
{
	size_t nbr = group * groups;
	
	if (ptr == end)
		return;
	printf(" ");
	for (size_t i = 0; i < nbr && ptr < end; i++, ptr++) {
		if (isprint(*(unsigned char *)ptr)) {
			printf("%c", *(unsigned char *)ptr);
		} else {
			printf(".");
		}
	}
	printf("\n");
}

void print_memory(void *ptr, size_t size)
{
	void *end = ptr + size;
	size_t 	groups = 4;	//
	size_t	group = 4;	// bytes in group
	size_t	h = 0;
	
	for (; ptr < end; ptr += group * groups) // по 4 байта
	{
		print_values(ptr, end, group, groups);
		print_chars(ptr, end, group, groups);
		if (h == VGA_HEIGHT - 1) {
			printf("Press Enter to continue / Escape to stop");
			while (1) {
				if ((inb(0x64) & 1) && inb(0x60) == 0x1C) {
					printf("\n");
					break ;
				} else if ((inb(0x64) & 1) && inb(0x60) == 0x1) {
					printf("\n");
					return;
				}
			}
			h = 0;
		}
		h++;
	}
}