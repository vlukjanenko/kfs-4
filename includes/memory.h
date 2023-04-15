/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majosue <majosue@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/29 20:57:49 by majosue           #+#    #+#             */
/*   Updated: 2023/04/15 10:34:12 by majosue          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MEMORY_H
# define MEMORY_H

#include "stdint.h"

extern uint32_t  max_addr;
extern void*    stack_top;
extern uint32_t end_of_code;

void *get_frame(void);
void free_frame(void* addr);
void load_page_directory(void *page_directory);
void enable_paging(void);

#endif