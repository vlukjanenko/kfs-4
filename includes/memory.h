/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majosue <majosue@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/29 20:57:49 by majosue           #+#    #+#             */
/*   Updated: 2023/04/15 11:23:00 by majosue          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MEMORY_H
# define MEMORY_H

# include "stdint.h"
# define PAGE_OFFSET 0xC0000000
# define PAGE_SIZE 4096

extern uint32_t			max_addr;
extern unsigned char	stack_top;
extern unsigned char	end_of_code;
extern unsigned char	start_of_code;

extern uint32_t pd_first_entry;
extern uint32_t pt_first_entry;

void *get_frame(void);
void *get_frames(uint32_t frames);
void free_frame(void* addr);
void load_page_directory(void *page_directory);
void enable_paging(void);
void turn_on_paging(void);
int frame_status(uint32_t addr);
void refresh_map(void);
void memory_init();
unsigned char* 	get_bitmask();
uint32_t 		get_bitmask_size();


#endif
