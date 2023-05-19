/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IDT.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majosue <majosue@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/05 18:25:01 by majosue           #+#    #+#             */
/*   Updated: 2023/05/19 11:19:25 by majosue          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IDT.h"
#include "baselib.h"
#include "pic.h"
#include "isr.h"

void idt_set_descriptor(void* isr, uint8_t flags, idt_entry_t* descriptor)
{
	descriptor->isr_low = (uint32_t)isr & 0xFFFF;
	descriptor->kernel_cs = 0x08; // this value can be whatever offset your kernel code selector is in your GDT
	descriptor->attributes = flags;
	descriptor->isr_high = (uint32_t)isr >> 16;
	descriptor->reserved = 0;
}

void idt_init()
{
	idt_entry_t		idt[256];
	static idtr_t	idtr;

	idtr.base = IDTBASE;
	idtr.limit = IDTSIZE * 8 - 1;
	for (uint16_t vector = 0; vector < 256; vector++) {
		idt_set_descriptor(interrupt_handler_stub, 0x8E, &idt[vector]);
	}
	idt_set_descriptor(divide_error, 0x8E, &idt[0]);
	idt_set_descriptor(debug, 0x8E, &idt[1]);
	idt_set_descriptor(nmi, 0x8E, &idt[2]);
	idt_set_descriptor(int3, 0x8E, &idt[3]);
	idt_set_descriptor(overflow, 0x8E, &idt[4]);
	idt_set_descriptor(bounds, 0x8E, &idt[5]);
	idt_set_descriptor(invalid_op, 0x8E, &idt[6]);
	idt_set_descriptor(device_not_available, 0x8E, &idt[7]);
	idt_set_descriptor(doublefault_fn, 0x8E, &idt[8]);
	idt_set_descriptor(coprocessor_segment_overrun, 0x8E, &idt[9]);
	idt_set_descriptor(invalid_TSS, 0x8E, &idt[10]);
	idt_set_descriptor(segment_not_present, 0x8E, &idt[11]);
	idt_set_descriptor(stack_segment, 0x8E, &idt[12]);
	idt_set_descriptor(general_protection, 0x8E, &idt[13]);
	idt_set_descriptor(page_fault, 0x8E, &idt[14]);
	idt_set_descriptor(coprocessor_error, 0x8E, &idt[16]);
	idt_set_descriptor(alignment_check, 0x8E, &idt[17]);
	idt_set_descriptor(machine_check, 0x8E, &idt[18]);
	idt_set_descriptor(simd_coprocessor_error, 0x8E, &idt[19]);
	idt_set_descriptor(keyboard_isr, 0x8E, &idt[33]);
	memcpy((char *) idtr.base, (char *) idt, idtr.limit + 1);
	__asm__ volatile ("lidt %0" : : "m"(idtr)); // load the new IDT
	PIC_remap(0x20, 0x70);
	__asm__ volatile ("sti"); // set the interrupt flag
}

void			set_intr_gate(uint8_t n, void* addr) // interrunt DPL = 0
{
	idt_set_descriptor(addr, 0x8E, (idt_entry_t *)(IDTBASE + n * 8));
}

void			set_system_gate(uint8_t n, void* addr) // trap DPL = 3
{
	idt_set_descriptor(addr, 0xEF, (idt_entry_t *)(IDTBASE + n * 8));
}

void			set_system_intr_gate(uint8_t n, void* addr) // interrupt DPL = 3
{
	idt_set_descriptor(addr, 0xEE, (idt_entry_t *)(IDTBASE + n * 8));
}

void			set_trap_gate(uint8_t n, void* addr) // trap DPL = 0
{
	idt_set_descriptor(addr, 0x8F, (idt_entry_t *)(IDTBASE + n * 8));
}

void irq_disable()
{
	__asm__ volatile ("cli"); // unset the interrupt flag
}

void irq_enable()
{
	__asm__ volatile ("sti"); // set the interrupt flag
}
