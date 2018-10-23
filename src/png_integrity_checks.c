/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   png_integrity_checks.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <njaber@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/22 16:51:35 by njaber            #+#    #+#             */
/*   Updated: 2018/10/20 11:32:01 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int				checksum(t_uchar *buf, t_uint len, t_uchar *alder)
{
	t_uint		i;
	t_uint		s1;
	t_uint		s2;
	t_uint		sum;

	s1 = 1;
	s2 = 0;
	i = 0;
	while (i < len)
	{
		s1 = (s1 + buf[i]) % 65521;
		s2 = (s2 + s1) % 65521;
		i++;
	}
	sum = (alder[0] << 24) | (alder[1] << 16) |
		(alder[2] << 8) | alder[3];
	if (sum != (s2 << 16 | s1))
		return (-10);
	return (0);
}

static void		gen_tab(t_uint *tab)
{
	t_uint		c;
	int			n;
	int			k;

	n = -1;
	while (++n < 256)
	{
		c = (t_uint)n;
		k = -1;
		while (++k < 8)
		{
			if (c & 0x1)
				c = 0xedb88320 ^ (c >> 1);
			else
				c = c >> 1;
		}
		tab[n] = c;
	}
}

static int		crc_check(int fd, t_uint computed_crc)
{
	t_uchar		crc_buf[4];
	t_uint		crc;

	if (read(fd, crc_buf, 4) != 4)
		return (-3);
	crc = (crc_buf[0] << 24) | (crc_buf[1] << 16) |
		(crc_buf[2] << 8) | crc_buf[3];
	if (~crc != computed_crc)
		return (-10);
	return (0);
}

int				validate_crc(int fd, t_uchar *data, t_png *png)
{
	static t_uint	crc_tab[256];
	static int		computed_tab = 0;
	t_uint			crc;
	int				i;

	(void)png;
	if (!computed_tab)
	{
		gen_tab(crc_tab);
		computed_tab = 1;
	}
	crc = 0xFFFFFFFF;
	i = -1;
	while (++i < 4)
		crc = crc_tab[(crc ^ png->block_type_[i]) & 0xFF] ^ (crc >> 8);
	i = -1;
	while (++i < (int)png->block_len_)
		crc = crc_tab[(crc ^ data[i]) & 0xFF] ^ (crc >> 8);
	return (crc_check(fd, crc));
}
