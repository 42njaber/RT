/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/21 21:11:02 by njaber            #+#    #+#             */
/*   Updated: 2018/05/30 03:21:41 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static char		*convert(char *format, va_list args, va_list arg_t, t_buf *buf)
{
	int		i;
	int		nb;
	va_list	arg_c;

	i = 0;
	if (ft_isdigit(*format))
	{
		while (ft_isdigit(*(format + i)))
			i++;
		if (*(format + i) == '$')
		{
			nb = ft_atoi(format);
			va_copy(arg_c, args);
			while (nb-- > 1)
				va_arg(arg_c, long);
			format = ft_mapper(format + i + 1, arg_c, buf);
			va_end(arg_c);
			return (format);
		}
	}
	format = ft_mapper(format, arg_t, buf);
	return (format);
}

int				ft_printf(const char *format, ...)
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
	return (buf.printed);
}

void			ft_error(const char *format, ...)
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
	exit(1);
}

static void		fill_buf(t_buf *buf)
{
	if (buf->ret == NULL)
		buf->ret = ft_strldup(buf->buf, B_SIZE);
	else
		buf->ret = ft_strljoin(buf->ret, buf->buf, buf->printed % B_SIZE, 1);
}

char			*ft_printb(const char *format, ...)
{
	char	*tmp;
	va_list	args;
	va_list	arg_t;
	t_buf	buf;
	char	tbuf[B_SIZE];

	ft_bzero(&buf, sizeof(buf));
	buf.tbuf = (t_list*)ft_memalloc(sizeof(t_list));
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
	fill_buf(&buf);
	free(buf.tbuf);
	return (buf.ret);
}
