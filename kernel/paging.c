/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paging.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majosue <majosue@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/15 10:52:41 by majosue           #+#    #+#             */
/*   Updated: 2023/04/15 11:57:41 by majosue          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "memory.h"
#include "stdint.h"
#include "stddef.h"
#include "baselib.h"

/*
	https://wiki.osdev.org/Setting_Up_Paging#Basic_Paging
*/

void turn_on_paging()
{
	uint32_t *page_directory = (uint32_t *)get_frame();
	for(int i = 0; i < 1024; i++) {
		// This sets the following flags to the pages:
		//   Supervisor: Only kernel-mode can access them
		//   Write Enabled: It can be both read from and written to
		//   Not Present: The page table is not present
		page_directory[i] = 0x00000002;
	}

	// refer last entry to page_directory to have access to it after paging enabled
	// page_directory access at 0xFFFFF000
	page_directory[1023] = (uint32_t)page_directory | 3;

	printf("page dir address %p\n", page_directory);

	uint32_t *first_page_table = (uint32_t *)get_frame();

	printf("first table address %p\n", first_page_table);

	bzero(first_page_table, 1024);

	for(int i = 0; i < 1024; i++)
	{

		if (!frame_status(i * 0x1000))
			break;

		 // As the address is page aligned, it will always leave 12 bits zeroed.
		// Those bits are used by the attributes ;)
		first_page_table[i] = (i * 0x1000) | 3; // attributes: supervisor level, read/write, present.
	}
	page_directory[0] = (uint32_t)first_page_table | 3;
	load_page_directory(page_directory);
	enable_paging();
}
