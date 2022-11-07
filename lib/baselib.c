/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   baselib.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majosue <majosue@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 19:05:49 by majosue           #+#    #+#             */
/*   Updated: 2022/11/04 16:51:33 by majosue          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "baselib.h"

size_t	strlen(const char* str)
{
	size_t len = 0;
	
	while (str[len])
		len++;
	return len;
}

void    *memcpy(void *dst, const void *src, size_t n)
{
	size_t  i;

	if (n == 0)
		return (dst);
	if (!dst && !src)
		return (NULL);
	i = 0;
	while (i < n)
	{
		((unsigned char *)dst)[i] = ((unsigned char *)src)[i];
		i++;
	}
	return (dst);
}

void    *memmove(void *dst, const void *src, size_t len)
{
	if (len == 0)
		return (dst);
	if (!dst && !src)
		return (NULL);
	if (dst < src)
		memcpy(dst, src, len);
	else
	{
		while (len > 0)
		{
			((unsigned char *)dst)[len - 1] = ((unsigned char *)src)[len - 1];
			len--;
		}
	}
	return (dst);
}

int	isalpha(int c)
{
	if ((c >= 0101 && c <= 0132) || (c >= 0141 && c <= 0172))
		return (1);
	else
		return (0);
}

int	isdigit(int c)
{
	return (c >= '0' && c <= '9');
}

int	isprint(int c)
{
	if (c >= 0040 && c <= 0176)
		return (1);
	else
		return (0);
}

void	bzero(void *s, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
		((unsigned char *)s)[i++] = 0;
}

char	*strchr(const char *s, int c)
{
	while (*s != '\0')
	{
		if (*s == (char)c)
			return ((char *)s);
		s++;
	}
	return (NULL);
}

int	atoi(const char *str)
{
	long	result;
	int		sign;
	long	over;

	result = 0;
	sign = 1;
	while (*str == '\t' || *str == '\n' || *str == '\v' ||\
*str == '\f' || *str == '\r' || *str == ' ')
		str++;
	if (*str == '-' || *str == '+')
	{
		sign = 44 - *str;
		str++;
	}
	while (*str != '\0' && *str >= '0' && *str <= '9')
	{
		over = result;
		result = result * 10 + *str - '0';
		if (over > result && sign == -1)
			return (0);
		if (over > result && sign == 1)
			return (-1);
		str++;
	}
	return (sign * result);
}

char	*strcpy(char *dst, const char *src)
{
	char	*tmp;

	tmp = dst;
	while (*src != '\0')
		*dst++ = *src++;
	*dst = '\0';
	return (tmp);
}

int	strnequ(char const *s1, char const *s2, size_t n)
{
	size_t	i;

	if (!s1 || !s2)
		return (0);
	i = 0;
	while (i < n && (s1[i] != '\0' || s2[i] != '\0'))
	{
		if (s1[i] != s2[i])
			return (0);
		i++;
	}
	return (1);
}

void	*memset(void *b, int c, size_t len)
{
	size_t	i;

	i = 0;
	while (i < len)
	{
		((unsigned char *)b)[i] = (unsigned char)c;
		i++;
	}
	return (b);
}

static int	rn(unsigned long long value, int base)
{
	int	rn;

	rn = 0;
	while (value > 0)
	{
		value = value / base;
		rn++;
	}
	if (rn == 0)
		return (1);
	return (rn);
}

void	sitoa_base_u(unsigned long long int value, int base, char *buffer)
{
	int		rezult_size;

	rezult_size = rn(value, base);
	buffer[rezult_size] = '\0';
	while ((rezult_size - 1) >= 0)
	{
		if ((value % base) >= 10)
		{
			buffer[rezult_size - 1] = value % base - 10 + 'A';
		}
		else
		{
			buffer[rezult_size - 1] = value % base + '0';
		}
		value /= base;
		rezult_size--;
	}
}

void	sitoa_base(long long int value, int base, char *buffer)
{
	unsigned long long int	uvalue;

	if (value < 0)
	{
		uvalue = -value;
		if (base == 10) {
			buffer[0] = '-';
			buffer++;
		}
	}
	else
		uvalue = value;
	sitoa_base_u(uvalue, base, buffer);
}

int	strequ(char const *s1, char const *s2)
{
	if (!s1 || !s2)
		return (0);
	if (strlen(s1) != (strlen(s2)) || !s1 || !s2)
		return (0);
	while (*s1 != '\0')
		if (*s1++ != *s2++)
			return (0);
	return (1);
}

int	tolower(int c)
{
	if (c >= 'A' && c <= 'Z')
		return (c + 32);
	else
		return (c);
}