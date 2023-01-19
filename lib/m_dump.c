/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m_dump.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majosue <majosue@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 16:08:53 by majosue           #+#    #+#             */
/*   Updated: 2023/01/19 16:11:12 by majosue          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "baselib.h"
#include "stdint.h"

static const char *skip_hex_prefix(const char *str)
{
	if (str[0] && str[0] == '0' && str[1] && tolower(str[1]) == 'x')
		str += 2;
	return (str);
}

static int is_hex(const char *str)
{
	while (isdigit(*str) || (tolower(*str) >= 'a' && tolower(*str) <= 'f'))
		str++;
	return (!*str || *str == ' ');
}

static int is_number(const char *str)
{
	if (str != skip_hex_prefix(str))
		return (is_hex(skip_hex_prefix(str)));
	while (isdigit(*str))
		str++;
	return (!*str || *str == ' ');
}

static const char *skip_space(const char *str)
{
	while (*str && *str == ' ')
		str++;
	return (str);
}

static const char *skip_word(const char *str)
{
	while (*str && *str != ' ')
		str++;
	return (str);
}

int words_nbr(const char *params)
{
	int nbr = 0;

	params = skip_space(params);
	while (*params) {
		params = skip_word(params);
		params = skip_space(params);
		nbr++;
	}
	return (nbr);
}

static uint32_t	atoh(const char *str)
{
	uint32_t result = 0;

	while (*str && *str != ' ') {
		if (isdigit(*str)) {
			result = (result << 4) + (*str - '0');
		} else {
			result = (result << 4) + 10 + (tolower(*str) - 'a');
		}
		str++;
	}
	return (result);
}

static uint32_t	atou(const char *str)
{
	uint32_t result = 0;
	
	if (str != skip_hex_prefix(str))
		return (atoh(skip_hex_prefix(str)));
	while (isdigit(*str)) {
		result = result * 10 + (*str - '0');
		str++;
	}
	return (result);	
}

void m_dump(const char *params)
{
	void    *start;
	size_t  size;

	if (words_nbr(params) != 2) {
		printf("\nUsage: mdump addr size\n");
		return;
	}
	params = skip_space(params);
	if (!is_number(params)) {
		printf("\nUsage: mdump addr size\n");
		return;
	}
	start = (void *)atou(params);
	params = skip_space(skip_word(params));
	if (!is_number(params)) {
		printf("\nUsage: mdump addr size\n");
		return;
	}
	size = atou(params);
	printf("\n");
	print_memory(start, size);
}
