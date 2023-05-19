/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kernel.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majosue <majosue@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 19:00:59 by majosue           #+#    #+#             */
/*   Updated: 2023/05/19 11:35:40 by majosue          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "stddef.h"
#include "screen.h"
#include "baselib.h"
#include "keyboard.h"
#include "multiboot.h"
#include "memory.h"
#include "IDT.h"
#include "isr.h"

void	print_stack(void)
{
	void* top = &stack_top + 0xc0000000;
	void* bottom;
	char stack_indicator1[8] = {'b','o', 't', 't', 'o', 'm' , '.', '.'};
	(void)stack_indicator1;
	asm ("movl %%esp, %0" : "=r" (bottom));
	printf("Stack:\n");
	print_memory(bottom, top - bottom);
}

int 	divide_by_zero()
{
	int a = 0;

	return (1 / a);
}

int	main(void)
{
	memory_init();
	terminal_initialize(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
	enable_cursor(0, 15);

	set_intr_gate(0, interrupt_handler_stub);
	set_intr_gate(32, timer_isr);
	//divide_by_zero();
	irq_disable();
	irq_enable();
	irq_set_mask(1);
	irq_clear_mask(1);
	irq_clear_mask(0); // timer
	irq_set_mask(0);
	printf("$ ");
	while(1) {
		asm("hlt\n\t");
	};
	//poll_keyboard(NULL);
	return (0);
}
