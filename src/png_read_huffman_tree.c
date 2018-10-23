/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   png_read_huffman_tree.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <njaber@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/05 18:40:41 by njaber            #+#    #+#             */
/*   Updated: 2018/10/20 11:32:17 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

short			read_next(t_png *png, t_uchar *stream,
													t_uint *pos, t_btree *tree)
{
	while (tree != NULL && tree->val == -1)
	{
		if (*pos / 8 >= png->block_len_ - 4)
			return (-2);
		else if (get_next_bits(stream, pos, 1))
			tree = tree->b1;
		else
			tree = tree->b0;
	}
	if (tree == NULL)
		return (-6);
	return (tree->val);
}

static int		read_clens(t_png *png, t_uchar *stream,
													t_uint *pos, t_ushort max)
{
	t_uchar	clens[19];
	t_uchar	order[19];
	int		i;
	int		ret;

	ft_bzero(clens, sizeof(t_uchar) * 19);
	if (max > 19)
		return (-6);
	if ((*pos + max * 3 - 1) / 8 >= png->zlib_len_ - 6)
		return (-2);
	ft_memcpy(order, (t_uchar[19]){16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11,
			4, 12, 3, 13, 2, 14, 1, 15}, 19);
	i = -1;
	while (++i < max)
		clens[order[i]] = get_next_bits(stream, pos, 3);
	ret = create_cltree(&png->cltree, clens);
	return (ret);
}

static int		copy_all_codes(t_png *png, t_uint *pos,
													t_uchar *lens, t_ushort max)
{
	t_uchar	*stream;
	int		i;
	t_uchar	len;
	short	val;

	stream = png->zlib_stream_ + 2;
	i = 0;
	while (i < max)
	{
		if ((val = read_next(png, stream, pos, png->cltree)) < 0)
			return (val);
		else if (val < 16)
			lens[i++] = val;
		else
		{
			if ((*pos + (val == 18 ? 7 : val - 14)) / 8 >= png->zlib_len_ - 6)
				return (-2);
			if ((val == 16 && i == 0) || (len = get_next_bits(stream, pos,
				(val == 18 ? 7 : val - 14)) + (val == 18 ? 11 : 3)) + i > max)
				return (-2);
			ft_memset(lens + i, val == 16 ? lens[i - 1] : 0, len);
			i += len;
		}
	}
	return (0);
}

static int		read_trees(t_png *png, t_uint *pos, t_ushort *hlens)
{
	t_uchar	lens[318];
	int		ret;

	if (hlens[0] > 286 || hlens[1] > 32)
		return (-6);
	if ((ret = copy_all_codes(png, pos, lens, hlens[0] + hlens[1])) >= 0)
	{
		if (lens[256] == 0)
			return (-7);
		if ((ret = create_tree(&png->ltree, lens, hlens[0])) >= 0)
			ret = create_tree(&png->dtree, lens + hlens[0], hlens[1]);
	}
	return (ret);
}

int				read_tree(t_png *png, t_uchar *stream, t_uint *pos)
{
	t_ushort	hclen;
	t_ushort	hlens[2];
	int			ret;

	if ((*pos + 13) / 8 >= png->zlib_len_ - 6)
		return (-2);
	hlens[0] = get_next_bits(stream, pos, 5) + 257;
	hlens[1] = get_next_bits(stream, pos, 5) + 1;
	hclen = get_next_bits(stream, pos, 4) + 4;
	if ((ret = read_clens(png, stream, pos, hclen)) >= 0)
		ret = read_trees(png, pos, hlens);
	free_tree(&png->cltree);
	return (ret);
}
