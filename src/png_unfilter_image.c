/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   png_unfilter_image.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <njaber@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/04 21:33:19 by njaber            #+#    #+#             */
/*   Updated: 2018/10/20 11:32:22 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static int		stock_colors(t_png *png, t_uchar *colors)
{
	int		i;

	i = -1;
	while (++i < png->dim.v[0] * png->dim.v[1])
	{
		png->buf[i] = (colors[i * png->pxsize] << 16) |
			(colors[i * png->pxsize + 1] << 8) | colors[i * png->pxsize + 2];
		if ((png->ctype & 0x4) != 0)
			png->buf[i] |= ~colors[i * png->pxsize + png->pxsize - 1] << 24;
	}
	return (0);
}

static t_uchar	paeth_predictor(t_uchar a, t_uchar b, t_uchar c)
{
	int		p;
	int		pa;
	int		pb;
	int		pc;

	p = (int)a + (int)b - (int)c;
	pa = ft_abs(p - (int)a);
	pb = ft_abs(p - (int)b);
	pc = ft_abs(p - (int)c);
	if (pa <= pb && pa <= pc)
		return (a);
	else if (pb <= pc)
		return (b);
	else
		return (c);
}

static int		unfilter_byte(t_png *png, t_uchar *buf,
													t_ivec pos, t_uchar filter)
{
	t_uint	buf_pos;
	t_uchar	x;
	t_uchar	a;
	t_uchar	b;
	t_uchar	c;

	buf_pos = pos.v[0] + png->dim.v[0] * png->pxsize * pos.v[1];
	x = png->data_[buf_pos + pos.v[1] + 1];
	a = pos.v[0] < png->pxsize ? 0 : buf[buf_pos - png->pxsize];
	b = pos.v[1] == 0 ? 0 : buf[buf_pos - png->dim.v[0] * png->pxsize];
	c = pos.v[0] < png->pxsize || pos.v[1] == 0 ? 0 :
		buf[buf_pos - (png->dim.v[0] + 1) * png->pxsize];
	if (filter == 1)
		buf[buf_pos] = (t_uchar)(x + a);
	else if (filter == 2)
		buf[buf_pos] = (t_uchar)(x + b);
	else if (filter == 3)
		buf[buf_pos] = (t_uchar)(x + ((t_uint)a + (t_uint)b) / 2);
	else if (filter == 4)
		buf[buf_pos] = (t_uchar)(x + paeth_predictor(a, b, c));
	else
		buf[buf_pos] = x;
	return (0);
}

static int		unfilter_data(t_png *png, t_uchar *buf)
{
	t_ivec	pos;
	t_uchar	filter;
	int		ret;

	pos = ivec(0, 0);
	filter = png->data_[0];
	while (pos.v[1] < png->dim.v[1])
	{
		if ((ret = unfilter_byte(png, buf, pos, filter)) < 0)
			return (ret);
		pos.v[0]++;
		if (pos.v[0] >= png->dim.v[0] * png->pxsize)
		{
			pos = ivec(0, pos.v[1] + 1);
			filter = png->data_[(png->dim.v[0] * png->pxsize + 1) * pos.v[1]];
		}
	}
	return (0);
}

int				recompose_image(t_png *png)
{
	t_uchar		*color_data;
	int			ret;

	color_data = NULL;
	if ((color_data = (t_uchar*)ft_memalloc(sizeof(t_uchar) * png->dim.v[0] *
					png->dim.v[1] * png->pxsize)) == NULL)
		return (-666);
	if ((ret = unfilter_data(png, color_data)) < 0)
	{
		free(color_data);
		return (ret);
	}
	ret = stock_colors(png, color_data);
	free(color_data);
	return (ret);
}
