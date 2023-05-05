/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majosue <majosue@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/29 20:57:49 by majosue           #+#    #+#             */
/*   Updated: 2023/05/05 12:19:01 by majosue          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MEMORY_H
# define MEMORY_H

# include "stdint.h"
# define PAGE_OFFSET	0xC0000000
# define PAGE_SIZE		4096
# define LOW_MEM		0
# define VMALLOC		1
# define MAX_ADDR *(uint32_t *)((void *)&max_addr + PAGE_OFFSET) // max_addr остался в загрузочной части

extern uint32_t			max_addr;
extern unsigned char	stack_top;
extern unsigned char	end_of_code;
extern unsigned char	start_of_code;

extern uint32_t pd_first_entry;
extern uint32_t pt_first_entry;

void			*get_frame(void *start, void *end);
void			*get_frames(void *start, void *end, uint32_t frames);
void			free_frame(void* addr);
void			load_page_directory(void *page_directory);
void			enable_paging(void);
void			turn_on_paging(void);
int				frame_status(uint32_t addr);
void			refresh_map(void);
void 			memory_init();
unsigned char*	get_bitmask();
uint32_t		get_bitmask_size();
uint32_t		align(uint32_t addr, uint32_t boundary);
void			*get_heap_start();
void			*get_heap_end();
void			*get_page(uint32_t flags, uint32_t nbr);
void			free_page(void *page, uint32_t nbr);
void			*kmalloc(uint32_t size);
void			*vmalloc(uint32_t size);
uint32_t		ksize(void *ptr);
uint32_t		vsize(void *ptr);
void			print_memory_list(); // for kmalloc debug
void			kfree(void *ptr);
void			vfree(void *ptr);
void			*get_vmalloc_end();
void			print_pages(void *page, uint32_t nbr); // map pages debug
void			print_vmalloc_list(void); // for vmalloc debug


#endif
