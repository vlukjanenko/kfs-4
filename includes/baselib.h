/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   baselib.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majosue <majosue@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 18:58:15 by majosue           #+#    #+#             */
/*   Updated: 2022/11/01 18:58:20 by majosue          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BASELIB_H
# define BASELIB_H

# include "stddef.h"
# include "stdint.h"

size_t	strlen(const char* str);
void	*memcpy(void *dst, const void *src, size_t n);
void	*memmove(void *dst, const void *src, size_t len);
uint8_t	inb(uint16_t port);
void	outb(uint16_t port, uint8_t value);
int		isalpha(int c);
int		isprint(int c);

#endif