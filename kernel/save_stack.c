/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_stack.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majosue <majosue@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 12:49:08 by majosue           #+#    #+#             */
/*   Updated: 2023/05/19 12:52:44 by majosue          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "baselib.h"
#include "memory.h"

void	save_stack(void)
{
	void *bottom;

	asm ("movl %%esp, %0" : "=r" (bottom));
	printf("Stack:\n");
	print_memory(bottom, (void*)(&stack_top + 0xc0000000) - bottom);
}
