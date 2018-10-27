/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   png_deflate_code_reader.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <njaber@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/04 14:17:43 by njaber            #+#    #+#             */
/*   Updated: 2018/10/27 02:58:19 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static t_uint	lencode(short val)
{
	t_uint	len;

	if (val == 285)
		len = 258;
	else
	{
		len = 3 + val - 257
			+ (val < 266 ? 0 : 1 * (val - 265))
			+ (val < 270 ? 0 : 2 * (val - 269))
			+ (val < 274 ? 0 : 4 * (val - 273))
			+ (val < 278 ? 0 : 8 * (val - 277))
			+ (val < 282 ? 0 : 16 * (val - 281));
	}
	return (len);
}

static t_uint	distcode(short val)
{
	t_uint	dist;

	if (val > 29)
		return (0);
	dist = 1 + val
		+ (val < 5 ? 0 : 1 * (val - 4))
		+ (val < 7 ? 0 : 2 * (val - 6))
		+ (val < 9 ? 0 : 4 * (val - 8))
		+ (val < 11 ? 0 : 8 * (val - 10))
		+ (val < 13 ? 0 : 16 * (val - 12))
		+ (val < 15 ? 0 : 32 * (val - 14))
		+ (val < 17 ? 0 : 64 * (val - 16))
		+ (val < 19 ? 0 : 128 * (val - 18))
		+ (val < 21 ? 0 : 256 * (val - 20))
		+ (val < 23 ? 0 : 512 * (val - 22))
		+ (val < 25 ? 0 : 1024 * (val - 24))
		+ (val < 27 ? 0 : 2048 * (val - 26))
		+ (val < 29 ? 0 : 4096 * (val - 28));
	return (dist);
}

static int		get_dist(t_png *png, t_uint *pos, t_uchar *stream)
{
	short	code;
	t_uint	tmp;
	int		dist;

	if (png->codes_len_ >= png->data_size_)
		return (-2);
	code = read_next(png, stream, pos, png->dtree);
	if (code < 0)
		return (code);
	dist = distcode((t_uchar)code);
	if (code > 3)
	{
		if ((*pos + (code - 4) / 2) / 8 >= png->zlib_len_ - 6)
			return (-2);
		tmp = get_next_bits(stream, pos, (code - 2) / 2);
		dist += tmp;
	}
	png->codes_[png->codes_len_++] = dist;
	return (0);
}

static int		code_tobuf(t_png *png, short val, t_uint *pos, t_uchar *stream)
{
	t_uint	stockval;
	t_uint	tmp;

	if (png->codes_len_ >= png->data_size_)
		return (-2);
	else if (val > 256 && val < 286)
	{
		stockval = lencode(val);
		if (val > 264 && val < 285)
		{
			if ((*pos + (val - 265) / 4) / 8 >= png->zlib_len_ - 6)
				return (-2);
			tmp = get_next_bits(stream, pos, 1 + (val - 265) / 4);
			stockval += tmp;
		}
		png->codes_[png->codes_len_++] = -stockval;
		return (get_dist(png, pos, stream));
	}
	else
		png->codes_[png->codes_len_++] = val;
	return (0);
}

int				read_codes(t_png *png, t_uchar *stream, t_uint *pos)
{
	short		val;
	int			ret;

	val = read_next(png, stream, pos, png->ltree);
	while (val >= 0 && val != 256)
	{
		if ((ret = code_tobuf(png, val, pos, stream)) < 0)
			return (ret);
		val = read_next(png, stream, pos, png->ltree);
	}
	if (val != 256)
		return (val);
	return (0);
}
