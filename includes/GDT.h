/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GDT.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majosue <majosue@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 18:58:31 by majosue           #+#    #+#             */
/*   Updated: 2023/01/31 21:05:18 by majosue          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GDT_H
# define GDT_H

# include "stdint.h"

struct __attribute__((packed)) GDTR
{
	uint16_t limit;
	uint32_t base;
};

extern uint64_t		g_table[7];
extern struct GDTR	gdtr __attribute__ ((section (".bootstrap.data")));
extern struct GDTR	gdtr_post;

#endif
