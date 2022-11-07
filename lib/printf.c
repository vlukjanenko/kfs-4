/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printf.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majosue <majosue@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 22:05:20 by majosue           #+#    #+#             */
/*   Updated: 2022/11/04 18:25:00 by majosue          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "printf.h"
#include "baselib.h"

static const char* g_flag_tab = " #0-+";
static const char* g_modi_tab = "cspdiouxX%";

static int (*const g_print[10])(struct s_modi *params, \
			va_list *valist, int* result) = {
	print_char,
	print_str,
	print_number_x,
	print_number,
	print_number,
	print_number_o,
	print_number_u,
	print_number_x,
	print_number_x,
	print_char
};

static void get_sign(struct s_modi *params)
{
	if (params->buffer[0] == '-') {
		params->buffer++;
		params->len--;
		strcpy(params->prefix, "-");
	} else if (params->flags[PLUS]) {
		strcpy(params->prefix, "+");
	} else if (params->flags[SPACE]) {
		strcpy(params->prefix, " ");
	}
}

static void fill_buffer(int *result, const char c, int number)
{
	for (int i = 0; i < number; i++) {
		terminal_putchar(c);
		(*result)++;
	}
}

static void write_buffer(int *result, void *data, int len)
{
	terminal_write(data, len);
	(*result) += len;
}

static void correct_sharp_for_o(struct s_modi *params)
{
	if (params->mod[0] != 'o')
		return;
	if (params->prefix[0] == '0' && params->buffer[0] == '0')
		params->prefix[0] = '\0';
	if (params->prefix[0] == '0' && params->prec[1] > params->len)
		params->prefix[0] = '\0';
}

static void formated_print(struct s_modi *params, int *result)
{
	size_t filler_prec;
	size_t filler_width;

	correct_sharp_for_o(params);
	if (params->prec[1] < params->len)
		params->prec[1] = params->len;
	if (params->widt[1] < params->prec[1] + strlen(params->prefix))
		params->widt[1] = params->prec[1] + strlen(params->prefix);
	filler_prec = params->prec[1] - params->len;
	filler_width = params->widt[1] - params->prec[1] - \
		strlen(params->prefix);
	if (params->flags[MINUS]) {
		write_buffer(result, params->prefix, strlen(params->prefix));
		fill_buffer(result, '0', filler_prec);
		write_buffer(result, params->buffer, params->len);
		fill_buffer(result, ' ', filler_width);
	} else if (params->flags[ZERO]) {
		write_buffer(result, params->prefix, strlen(params->prefix));
		fill_buffer(result, '0', filler_width);
		write_buffer(result, params->buffer, params->len);
	} else {
		fill_buffer(result, ' ', filler_width);
		write_buffer(result, params->prefix, strlen(params->prefix));
		fill_buffer(result, '0', filler_prec);
		write_buffer(result, params->buffer, params->len);
	}
}

static int format_number(struct s_modi *params, int *result)
{
	get_sign(params);
	if (params->prec[0] || params->flags[MINUS])
		params->flags[ZERO] = 0;
	if (params->prec[0] && !params->prec[1] && params->buffer[0] == '0') {
		params->buffer++;
		params->len = 0;
	}
	formated_print(params, result);
	return (1);
}

static int print_str(struct s_modi *params, va_list *valist, int *result)
{
	params->buffer = va_arg(*valist, char *);
	if (params->buffer == NULL)
		params->buffer = "(null)";
	params->len = strlen(params->buffer);
	if (params->prec[0] && params->prec[1] < params->len)
		params->len = params->prec[1];
	if (params->prec[0] && params->prec[1] > params->len)
		params->prec[1] = params->len;
	formated_print(params, result);
	return (1);
}

static int print_char(struct s_modi *params, va_list *valist, int *result)
{
	int		s1;

	if (*(params->mod) == '%')
		s1 = '%';
	else
		s1 = va_arg(*valist, int);
	params->buffer[0]= (char)s1;
	params->len = 1;
	params->prec[0] = 0;
	params->prec[1] = 0;
	formated_print(params, result);
	return (1);
}

static void take_signed(long long int *number, \
	const char *len_mod, va_list *valist)
{
	if (strequ(len_mod, "ll"))
		*number = va_arg(*valist, long long int);
	else if (strequ(len_mod, "hh"))
		*number = (char)va_arg(*valist, int);
	else if (strequ(len_mod, "h"))
		*number = (short int)va_arg(*valist, int);
	else if (strequ(len_mod, "l"))
		*number = (long int)va_arg(*valist, long int);
	else
		*number = (int)va_arg(*valist, int);
}

static void take_unsigned(unsigned long long int *number, \
	const char *len_mod, va_list *valist)
{
	if (strequ(len_mod, "ll"))
		*number = va_arg(*valist, unsigned long long int);
	else if (strequ(len_mod, "hh"))
		*number = (unsigned char)va_arg(*valist, unsigned int);
	else if (strequ(len_mod, "h"))
		*number = (unsigned short int)va_arg(*valist, unsigned int);
	else if (strequ(len_mod, "l"))
		*number = (unsigned long int)va_arg(*valist, unsigned long int);
	else
		*number = (unsigned int)va_arg(*valist, unsigned int);
}

static int	print_number(struct s_modi *params, va_list *valist, int *result)
{
	long long int	d;

	take_signed(&d, params->len_mod, valist);
	sitoa_base(d, 10, params->buffer);
	params->len = strlen(params->buffer);
	format_number(params, result);
	return (1);
}

static int	print_number_u(struct s_modi *params, va_list *valist, int *result)
{
	unsigned long long int	d;

	take_unsigned(&d, params->len_mod, valist);
	sitoa_base_u(d, 10, params->buffer);
	params->len = strlen(params->buffer);
	format_number(params, result);
	return (1);
}

static int	print_number_o(struct s_modi *params, va_list *valist, int *result)
{
	unsigned long long int	d;

	take_unsigned(&d, params->len_mod, valist);
	sitoa_base_u(d, 8, params->buffer);
	if (params->flags[SHARP])
		params->prefix[0] = '0';
	params->len = strlen(params->buffer);
	format_number(params, result);
	return (1);
}

static void to_lower_in_place(char *buffer)
{
	while (*buffer) {
		*buffer = tolower(*buffer);
		buffer++;
	}
}

static int	print_number_x(struct s_modi *params, va_list *valist, int *result)
{
	unsigned long long int	d;
	if (params->mod[0] == 'p')
		strcpy(params->len_mod, "l"); // depends on address size?
	take_unsigned(&d, params->len_mod, valist);
	sitoa_base_u(d, 16, params->buffer);
	if ((params->flags[SHARP] && params->buffer[0] != '0') \
		|| params->mod[0] == 'p')
		strcpy(params->prefix, "0X");
	if (params->mod[0] == 'x' || params->mod[0] == 'p') {
		to_lower_in_place(params->buffer);
		to_lower_in_place(params->prefix);
	}
	params->len = strlen(params->buffer);
	format_number(params, result);
	return (1);
}

static void read_flags(struct s_modi *params, const char *restrict *format)
{
	const char* flag;

	while ((flag = strchr(g_flag_tab, **format))) {
		params->flags[flag - g_flag_tab] = 1;
		(*format)++;
	}
}

static void read_width(struct s_modi *params, \
	const char *restrict *format, va_list *valist)
{
	while (isdigit(**format) || **format == '*') {
		params->widt[0] = 1;
		params->widt[1] = atoi(*format);
		while (isdigit(**format)) {
			(*format)++;
		}
		while (**format == '*') {
			int value = va_arg(*valist, int);
			if (value < 0) {
				params->flags[MINUS] = 1;
				params->widt[1] = -value;
			} else {
				params->widt[1] = value;
			}
			(*format)++;
		}
	}
}

static void read_prec(struct s_modi *params, \
	const char *restrict *format, va_list *valist)
{
	while (isdigit(**format) || **format == '*') {
		params->prec[0] = 1;
		params->prec[1] = atoi(*format);
		while (isdigit(**format)) {
			(*format)++;
		}
		while (**format == '*') {
			int value = va_arg(*valist, int);
			if (value < 0) {
				params->prec[0] = 0;
				params->prec[1] = 0;
			} else {
				params->prec[1] = value;
			}
			(*format)++;
		}
	}
}

static void read_lmod(struct s_modi *params, const char *restrict *format)
{
	if (strnequ(*format, "hh", 2)) {
		strcpy(params->len_mod, "hh");
		(*format) += 2;
	} else if (strnequ(*format, "ll", 2)) {
		strcpy(params->len_mod, "ll");
		(*format) += 2;
	} else if (strnequ(*format, "l", 1)) {
		strcpy(params->len_mod, "l");
		(*format) += 1;
	} else if (strnequ(*format, "h", 1)) {
		strcpy(params->len_mod, "h");
		(*format) += 1;
	}
}

static void print_arg(const char *restrict *format, \
	int *result, va_list *valist)
{
	struct s_modi params;
	char buffer[256];

	bzero(&params, sizeof(params));
	bzero(buffer, sizeof(buffer));
	params.buffer = buffer;
	(*format)++;							// skip %
	read_flags(&params, format);			// read flags
	read_width(&params, format, valist);	// read width
	while (**format == '.') {				// read precision
		params.prec[0] = 1;
		(*format)++;
		read_prec(&params, format, valist);
	}
	read_lmod(&params, format);				// read length modifier
	if ((params.mod = strchr(g_modi_tab, **format))) {	// read modifier
		(*format)++;
		g_print[params.mod - g_modi_tab](&params, valist, result);
	}
}

int	printf(const char *restrict format, ...)
{
	va_list valist;
	int  result = 0;

	va_start(valist, format);
	while (result >=0 && *format) {
		if (*format == '%') {
			print_arg(&format, &result, &valist);
			continue;
		}
		terminal_putchar(*format);
		result++;
		format++;
	}
	va_end(valist);
	return(result);
}
