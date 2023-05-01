/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   baselib.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majosue <majosue@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 18:58:15 by majosue           #+#    #+#             */
/*   Updated: 2023/04/30 14:10:34 by majosue          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BASELIB_H
# define BASELIB_H

# include "stddef.h"
# include "stdint.h"
# include "multiboot.h"

size_t	strlen(const char* str);
void	*memcpy(void *dst, const void *src, size_t n);
void	*memmove(void *dst, const void *src, size_t len);
uint8_t	inb(uint16_t port);
void	outb(uint16_t port, uint8_t value);
void	outw(uint16_t port, uint16_t value);
void	print_stack(void);
int		isalpha(int c);
int		isprint(int c);
void	bzero(void *s, size_t n);
int		printf(const char *restrict format, ...);
char	*strchr(const char *s, int c);
int		isdigit(int c);
int		atoi(const char *str);
char	*strcpy(char *dst, const char *src);
int     strnequ(char const *s1, char const *s2, size_t n);
void	*memset(void *b, int c, size_t len);
void	sitoa_base(long long int value, int base, char *buffer);
void	sitoa_base_u(unsigned long long int value, int base, char *buffer);
int		strequ(char const *s1, char const *s2);
int		tolower(int c);
void	print_memory(void *ptr, size_t size);
void	m_dump(const char *params);
int		get_memory_map(struct multiboot_info *mbd, uint32_t magic);

#endif
