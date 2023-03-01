/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_memory_map.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majosue <majosue@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 20:11:35 by majosue           #+#    #+#             */
/*   Updated: 2023/02/05 18:28:01 by majosue          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "stddef.h"
#include "screen.h"
#include "baselib.h"
#include "multiboot.h"
#include "memory.h"

int	get_memory_map(struct multiboot_info *mbd, uint32_t magic)
{
	/* Make sure the magic number matches for memory mapping*/
	if(magic != MULTIBOOT_BOOTLOADER_MAGIC) {
		printf("invalid magic number!");
		return (1);
	}
 
	/* Check bit 6 to see if we have a valid memory map */
	if(!(mbd->flags >> 6 & 0x1)) {
		printf("invalid memory map given by GRUB bootloader");
		return (1);
	}
 
	/* Loop through the memory map and display the values */
	unsigned int i;
	for(i = 0; i < mbd->mmap_length; 
		i += sizeof(multiboot_memory_map_t)) 
	{
		multiboot_memory_map_t* mmmt = 
			(multiboot_memory_map_t*) (mbd->mmap_addr + i);
 
		printf("Start Addr: %llx | Length: %llx | Size: %x | Type: %d\n",
			mmmt->addr, mmmt->len, mmmt->size, mmmt->type);
 
		if(mmmt->type == MULTIBOOT_MEMORY_AVAILABLE) {
			/* 
			 * Do something with this memory block!
			 * BE WARNED that some of memory shown as availiable is actually 
			 * actively being used by the kernel! You'll need to take that
			 * into account before writing to memory!
			 */
			max_addr = (void*)((uint32_t)(mmmt->addr + mmmt->len - 1) > \
			 	(uint32_t)max_addr ? (uint32_t)(mmmt->addr + mmmt->len - 1) : \
				(uint32_t)max_addr);
		}      
	}
	max_addr = (void *)((uint32_t)max_addr - (uint32_t)max_addr % 16);
	return (0);
}