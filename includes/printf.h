/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printf.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majosue <majosue@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 22:04:42 by majosue           #+#    #+#             */
/*   Updated: 2022/11/04 16:45:38 by majosue          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRINTF_H
# define PRINTF_H

# include <stdarg.h>
# include "screen.h"
# include "stdint.h"
# include "stddef.h"

# define SPACE			0
# define SHARP			1
# define ZERO			2
# define MINUS			3
# define PLUS			4

struct s_modi {
	char		prefix[3];	// - + 0 0x holder for numbers
	uint8_t		flags[5];
	size_t		widt[2];	// 0 - is set, 1 - value
	size_t		prec[2];	// 0 - is set, 1 - value
	char		len_mod[3];	// hh, h, l, ll
	const char* mod;		// modifier (pointer to modifiers string)
	char		*buffer;	// itoa result holder or stirng pointer
	size_t		len;		// result str len
};

int	printf(const char *restrict format, ...);

static int	print_char(struct s_modi *params, va_list *valist, int *result);
static int	print_str(struct s_modi *params, va_list *valist, int *result);
static int	print_number(struct s_modi *params, va_list *valist, int *result);
static int	print_number_u(struct s_modi *params, va_list *valist, int *result);
static int	print_number_o(struct s_modi *params, va_list *valist, int *result);
static int	print_number_x(struct s_modi *params, va_list *valist, int *result);

#endif
