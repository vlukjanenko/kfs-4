#include "baselib.h"

size_t strlen(const char* str)
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

int	isprint(int c)
{
	if (c >= 0040 && c <= 0176)
		return (1);
	else
		return (0);
}
