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

#include "stdint.h"

extern uint32_t			max_addr;
extern void*			stack_top;
extern unsigned char	end_of_code;
extern unsigned char	_ptr;

extern uint32_t pd_first_entry;
extern uint32_t pt_first_entry;

void *get_frame(void);
void *get_frames(uint32_t frames);
void free_frame(void* addr);
void load_page_directory(void *page_directory);
void enable_paging(void);
void turn_on_paging(void);
int frame_status(uint32_t addr);


#endif
