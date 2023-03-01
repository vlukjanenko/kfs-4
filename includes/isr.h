/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   isr.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majosue <majosue@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 20:04:32 by majosue           #+#    #+#             */
/*   Updated: 2023/02/09 13:57:01 by majosue          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ISR_H
# define ISR_H

struct interrupt_frame;

__attribute__((interrupt))
void	interrupt_handler_stub(struct interrupt_frame* frame);
__attribute__((interrupt))
void	divide_error(struct interrupt_frame* frame);
__attribute__((interrupt))
void	debug(struct interrupt_frame* frame);
__attribute__((interrupt))
void	nmi(struct interrupt_frame* frame);
__attribute__((interrupt))
void	int3(struct interrupt_frame* frame);
__attribute__((interrupt))
void	overflow(struct interrupt_frame* frame);
__attribute__((interrupt))
void	bounds(struct interrupt_frame* frame);
__attribute__((interrupt))
void	invalid_op(struct interrupt_frame* frame);
__attribute__((interrupt))
void	device_not_available(struct interrupt_frame* frame);
__attribute__((interrupt))
void	doublefault_fn(struct interrupt_frame* frame, uint32_t error_code);
__attribute__((interrupt))
void	coprocessor_segment_overrun(struct interrupt_frame* frame);
__attribute__((interrupt))
void	invalid_TSS(struct interrupt_frame* frame, uint32_t error_code);
__attribute__((interrupt))
void	segment_not_present(struct interrupt_frame* frame, uint32_t error_code);
__attribute__((interrupt))
void	stack_segment(struct interrupt_frame* frame, uint32_t error_code);
__attribute__((interrupt))
void	general_protection(struct interrupt_frame* frame, uint32_t error_code);
__attribute__((interrupt))
void	page_fault(struct interrupt_frame* frame, uint32_t error_code);
__attribute__((interrupt))
void	coprocessor_error(struct interrupt_frame* frame);
__attribute__((interrupt))
void	alignment_check(struct interrupt_frame* frame, uint32_t error_code);
__attribute__((interrupt))
void	machine_check(struct interrupt_frame* frame);
__attribute__((interrupt))
void	simd_coprocessor_error(struct interrupt_frame* frame);
__attribute__((interrupt))
void	keyboard_isr(struct interrupt_frame* frame);

#endif