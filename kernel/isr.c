/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   isr.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majosue <majosue@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 17:21:42 by majosue           #+#    #+#             */
/*   Updated: 2023/05/19 12:55:01 by majosue          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "baselib.h"
#include "isr.h"
#include "pic.h"
#include "keyboard.h"

void panic(char* str)
{
	printf("Kernel panic: %s\n", str);
	save_stack();
	__asm__ volatile ("cli; hlt");
}

void	interrupt_handler_stub(struct interrupt_frame* frame)
{
	(void)frame;
	printf("\nReserved interrupt captured\n");
}

void divide_error(struct interrupt_frame* frame)
{
	(void)frame;
	printf("\nDivision by zero\n");
	panic("Fatal exception");
}

void	debug(struct interrupt_frame* frame)
{
	(void)frame;
	printf("\nDebug\n");
	panic("Fatal exception");
}

void	nmi(struct interrupt_frame* frame)
{
	printf("\nNMI\n");
	(void)frame;
}

void	int3(struct interrupt_frame* frame)
{
	(void)frame;
	printf("\nint3\n");
}

void	overflow(struct interrupt_frame* frame)
{
	(void)frame;
	printf("\nOverflow\n");
}

void	bounds(struct interrupt_frame* frame)
{
	(void)frame;
	printf("\nBounds\n");
}

void	invalid_op(struct interrupt_frame* frame)
{
	(void)frame;
	printf("\nInvalid op\n");
}

void	device_not_available(struct interrupt_frame* frame)
{
	(void)frame;
	printf("\nDevice not available\n");
}

void	doublefault_fn(struct interrupt_frame* frame, uint32_t error_code)
{
	(void)frame;
	(void)error_code;

	printf("\nDouble fault\n");
}

void	coprocessor_segment_overrun(struct interrupt_frame* frame)
{
	(void)frame;
	printf("\nCoprocessor Segment Overrun\n");
}

void	invalid_TSS(struct interrupt_frame* frame, uint32_t error_code)
{
	(void)frame;
	(void)error_code;

	printf("\nInvalid TSS\n");
}

void	segment_not_present(struct interrupt_frame* frame, uint32_t error_code)
{
	(void)frame;
	printf("\nSegment not present %x\n", error_code);
	panic("Fatal exception");
}

void	stack_segment(struct interrupt_frame* frame, uint32_t error_code)
{
	(void)frame;
	printf("\nStack segment fault %x\n", error_code);
	panic("Fatal exception");
}

void	general_protection(struct interrupt_frame* frame, uint32_t error_code)
{
	(void)frame;
	(void)error_code;
	printf("\nGeneral protection fault %x\n", error_code);
	panic("Fatal exception");
}

void	page_fault(struct interrupt_frame* frame, uint32_t error_code)
{
	(void)frame;
	(void)error_code;
	printf("\nPage fault\n");
	printf("Error code = %x\n", error_code);
	__asm__ volatile ("cli; hlt");
}

void	coprocessor_error(struct interrupt_frame* frame)
{
	(void)frame;
	printf("\nCoprocessor error\n");
}

void	alignment_check(struct interrupt_frame* frame, uint32_t error_code)
{
	(void)frame;
	(void)error_code;
	printf("\nAlignment check\n");
}

void	machine_check(struct interrupt_frame* frame)
{
	(void)frame;
	printf("\nMachine check\n");
}

void	simd_coprocessor_error(struct interrupt_frame* frame)
{
	(void)frame;
	printf("\nSIMD coprocessor error\n");
}

void	keyboard_isr(struct interrupt_frame* frame)
{
	(void)frame;
	do_keyboard();
	PIC_sendEOI();
}

void	timer_isr(struct interrupt_frame* frame)
{
	(void)frame;
	printf("timer tick\n");
	PIC_sendEOI();
}
