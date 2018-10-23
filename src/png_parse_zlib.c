/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   png_parse_zlib.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <njaber@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/21 15:00:14 by njaber            #+#    #+#             */
/*   Updated: 2018/10/20 11:32:11 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

t_uint			get_next_bits(t_uchar *stream, t_uint *pos, t_uint len)
{
	t_uint	ret;
	t_uchar	offset;

	if (len > 32)
		len = 32;
	ret = stream[*pos / 8] >> (*pos % 8);
	offset = 8 - *pos % 8;
	while (offset < len)
	{
		ret |= stream[(*pos + offset) / 8] << offset;
		offset += 8;
	}
	ret &= 0xFFFFFFFF >> (32 - len);
	*pos += len;
	return (ret);
}

static int		check_flags(t_uchar *stream)
{
	t_uchar	cmf;
	t_uchar	flg;

	cmf = stream[0];
	flg = stream[1];
	if ((cmf & 0x0F) != 8 || (flg & 0x20) != 0)
		return (-5);
	if ((((t_uint)cmf << 8) | flg) % 31 != 0)
		return (-10);
	return (0);
}

static int		parse_deflate(t_png *png, t_uchar *stream)
{
	t_uint	pos;
	int		final;
	int		ret;

	pos = 0;
	final = 0;
	ret = 0;
	while (!final)
	{
		if (pos / 8 >= png->zlib_len_ - 6)
			return (-2);
		final = get_next_bits(stream, &pos, 1);
		ret = decompress_block(png, stream, &pos);
		if (ret < 0)
			break ;
	}
	return (ret);
}

static int		expand_length_codes(t_png *png)
{
	int		i;
	int		j;
	int		len;
	int		pos;

	i = -1;
	j = 0;
	while ((size_t)++i < png->codes_len_ && (size_t)j < png->data_size_)
	{
		if (png->codes_[i] >= 0)
			png->data_[j++] = (t_uchar)png->codes_[i];
		else
		{
			len = -png->codes_[i++];
			if ((size_t)i >= png->codes_len_ || (pos = j - png->codes_[i]) < 0)
				return (-2);
			while (len-- > 0)
			{
				if ((size_t)j >= png->data_size_)
					return (-2);
				png->data_[j++] = png->data_[pos++];
			}
		}
	}
	return ((size_t)i < png->codes_len_ ? -2 : 0);
}

int				parse_zlib(t_png *png)
{
	int		ret;

	if (png->zlib_len_ < 6)
		return (-2);
	if (png->filter != 0)
		return (-5);
	if (png->compression != 0)
		return (-4);
	png->data_size_ = png->dim.v[0] * png->dim.v[1] *
		png->pxsize + png->dim.v[1];
	if ((png->codes_ = (int*)ft_memalloc(sizeof(t_uint) *
					png->data_size_)) == NULL)
		return (-666);
	if ((png->data_ = (t_uchar*)ft_memalloc(sizeof(t_uint) *
					png->data_size_)) == NULL)
		return (-666);
	if ((ret = check_flags(png->zlib_stream_)) >= 0)
		if ((ret = parse_deflate(png, png->zlib_stream_ + 2)) >= 0)
			if ((ret = expand_length_codes(png)) >= 0)
				ret = checksum(png->data_, png->data_size_,
						png->zlib_stream_ + png->zlib_len_ - 4);
	free(png->codes_);
	png->codes_ = NULL;
	return (ret);
}
