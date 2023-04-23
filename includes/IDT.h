/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IDT.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majosue <majosue@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 18:58:31 by majosue           #+#    #+#             */
/*   Updated: 2023/02/09 15:50:53 by majosue          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IDT_H
# define IDT_H

# include "stdint.h"

#define IDTBASE	0xC0000000
#define IDTSIZE	0xFF

typedef struct {
	uint16_t	limit;
	uint32_t	base;
} __attribute__((packed)) idtr_t;

typedef struct {
	uint16_t    isr_low;      // The lower 16 bits of the ISR's address
	uint16_t    kernel_cs;    // The GDT segment selector that the CPU will load into CS before calling the ISR
	uint8_t     reserved;     // Set to zero
	uint8_t     attributes;   // Type and attributes; see the IDT page
	uint16_t    isr_high;     // The higher 16 bits of the ISR's address
} __attribute__((packed)) idt_entry_t;

void			idt_init(void);

#endif