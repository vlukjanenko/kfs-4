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
//#include "baselib.h"

/*
	https://wiki.osdev.org/Setting_Up_Paging#Basic_Paging
*/

void turn_on_paging()
{
	uint32_t *page_directory = &pd_first_entry; // pd_first_entry выделено в boot.s
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
	uint32_t *first_page_table = &pt_first_entry; // pt_first_entry выделено в boot.s
	for(int i = 0; i < 1024; i++) // мапим первые 4M физической памяти (когда ядро подрастет нужно будет добавить страниц)
	{
		// As the address is page aligned, it will always leave 12 bits zeroed.
		// Those bits are used by the attributes ;)
		first_page_table[i] = (i * 0x1000) | 3; // attributes: supervisor level, read/write, present.
	}
	page_directory[0] = (uint32_t)first_page_table | 3;
	page_directory[768] = (uint32_t)first_page_table | 3; // мапим таблицу в 0xC000000
	load_page_directory(page_directory);
	enable_paging();
}
