/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <njaber@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/14 09:56:18 by njaber            #+#    #+#             */
/*   Updated: 2018/10/14 10:05:30 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int				ft_printret(int ret, const char *format, ...)
{
	char	*tmp;
	va_list	args;
	va_list	arg_t;
	t_buf	buf;
	char	tbuf[B_SIZE];

	buf.tbuf = (t_list*)ft_memalloc(sizeof(t_list));
	buf.write = 1;
	buf.printed = 0;
	buf.tbuf->content = tbuf;
	va_start(args, format);
	va_copy(arg_t, args);
	while ((tmp = ft_strchr(format, '%')))
	{
		str_tobuf((char *)format, tmp ?
			(size_t)(tmp++ - format) : ft_strlen(format), &buf);
		format = convert(tmp, args, arg_t, &buf);
	}
	str_tobuf((char *)format, tmp ?
		(size_t)(tmp - format) : ft_strlen(format), &buf);
	va_end(arg_t);
	va_end(args);
	write(1, buf.buf, buf.printed % B_SIZE);
	free(buf.tbuf);
	return (ret);
}

