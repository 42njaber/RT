/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   png_block_parser.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <njaber@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/20 14:19:30 by njaber            #+#    #+#             */
/*   Updated: 2018/10/20 11:31:27 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int		parse_ihdr(int fd, t_png *png)
{
	t_uchar	buf[13];

	if (png->init)
		return (-1);
	png->init = 1;
	if (png->block_len_ != 13)
		return (-2);
	if (read(fd, buf, 13) != 13)
		return (-3);
	png->dim.v[0] = (buf[0] << 24) | (buf[1] << 16) | (buf[2] << 8) | buf[3];
	png->dim.v[1] = (buf[4] << 24) | (buf[5] << 16) | (buf[6] << 8) | buf[7];
	png->bdepth = buf[8];
	png->ctype = buf[9];
	png->compression = buf[10];
	png->filter = buf[11];
	png->interlace = buf[12];
	if (png->bdepth != 8 || !(png->ctype == 2 || png->ctype == 6) ||
			png->compression != 0 || png->filter != 0 || png->interlace != 0)
		return (-4);
	png->pxsize = png->ctype == 2 ? 3 : 4;
	ft_printf("Image size: %d, %d\nBit depth: %hu\nColortype: %hu\nCompression "
			"methode: %hu\nFilter methode: %hu\nInterlace method: %hu\n",
			png->dim.v[0], png->dim.v[1], png->bdepth, png->ctype,
			png->compression, png->filter, png->interlace);
	return (validate_crc(fd, buf, png));
}

int		parse_phys(int fd, t_png *png)
{
	t_uchar	buf[9];

	if (!png->init || png->zlib_stream_ != NULL)
		return (-1);
	if (png->block_len_ != 9)
		return (-2);
	if (read(fd, buf, 9) != 9)
		return (-3);
	png->aratio.v[0] = (buf[0] << 24) | (buf[1] << 16) | (buf[2] << 8) | buf[3];
	png->aratio.v[1] = (buf[4] << 24) | (buf[5] << 16) | (buf[6] << 8) | buf[7];
	png->aunit = buf[8];
	return (validate_crc(fd, buf, png));
}

int		parse_idat(int fd, t_png *png)
{
	t_list	*buf;
	int		ret;

	png->block_nb_++;
	if (!png->init)
		return (-1);
	if ((buf = ft_lstnew(NULL, png->block_len_)) == NULL)
		return (-666);
	if (read(fd, buf->content, png->block_len_) != png->block_len_)
	{
		ft_lstdelone(&buf, &free_content);
		return (-3);
	}
	*png->idat_last_ = buf;
	png->idat_last_ = &buf->next;
	ret = validate_crc(fd, buf->content, png);
	png->zlib_len_ += png->block_len_;
	return (ret);
}

int		parse_iend(int fd, t_png *png)
{
	int		ret;

	if (!png->init || png->zlib_len_ == 0)
		return (-1);
	if ((png->zlib_stream_ = (t_uchar*)
					combine_list(png->idat_blocks_)) == NULL)
		return (-666);
	ft_lstdel(&png->idat_blocks_, &free_content);
	png->idat_blocks_ = NULL;
	if (png->block_len_ != 0)
		return (-2);
	if ((png->buf = (t_uint*)ft_memalloc(sizeof(t_uint) *
					png->dim.v[0] * png->dim.v[1])) == NULL)
		return (-666);
	if ((ret = parse_zlib(png)) >= 0)
		if ((ret = recompose_image(png)) >= 0)
			ret = validate_crc(fd, NULL, png);
	free(png->zlib_stream_);
	png->zlib_stream_ = NULL;
	free(png->data_);
	png->data_ = NULL;
	if (ret < 0)
		return (ret);
	return (1);
}

int		parse_unkown(int fd, t_png *png)
{
	t_uchar	*buf;
	int		ret;

	if ((png->block_type_[0] & 0x20) == 0)
		return (-4);
	if (!png->init)
		return (-1);
	if ((buf = (t_uchar*)ft_memalloc(sizeof(char) * png->block_len_)) == NULL)
		return (-666);
	if (read(fd, buf, png->block_len_) != png->block_len_)
	{
		free(buf);
		return (-3);
	}
	ret = validate_crc(fd, buf, png);
	free(buf);
	return (ret);
}
