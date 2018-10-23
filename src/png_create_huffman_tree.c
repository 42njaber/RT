/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   png_create_huffman_tree.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <njaber@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/23 15:53:14 by njaber            #+#    #+#             */
/*   Updated: 2018/10/20 11:31:35 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static int		place_node(t_btree **tree, t_ushort bits,
														t_uchar len, short val)
{
	while (len-- > 0)
	{
		if (*tree != NULL && (*tree)->val != -1)
			return (-7);
		if (*tree == NULL &&
				(*tree = (t_btree*)ft_memalloc(sizeof(t_btree))) == NULL)
			return (-666);
		(*tree)->val = -1;
		tree = (bits >> len) & 0x1 ? &(*tree)->b1 : &(*tree)->b0;
	}
	if (*tree != NULL)
		return (-7);
	if ((*tree = (t_btree*)ft_memalloc(sizeof(t_btree))) == NULL)
		return (-666);
	(*tree)->val = val;
	return (0);
}

int				create_cltree(t_btree **cltree, t_uchar *clens)
{
	t_uchar		i;
	int			j;
	t_uchar		bits;
	int			ret;

	i = 0;
	bits = 0;
	while (++i < 8)
	{
		j = -1;
		while (++j < 19)
		{
			if (clens[j] == i)
			{
				if ((ret = place_node(cltree, bits, i, j)) < 0)
					return (ret);
				bits += 1;
			}
		}
		bits <<= 1;
	}
	return (0);
}

int				create_tree(t_btree **tree, t_uchar *lens, t_ushort max)
{
	t_uchar		i;
	int			j;
	t_ushort	bits;
	int			ret;

	i = 0;
	bits = 0;
	while (++i < 16)
	{
		j = -1;
		while (++j < max)
		{
			if (lens[j] == i)
			{
				if ((ret = place_node(tree, bits, i, j)) < 0)
					return (ret);
				bits += 1;
			}
		}
		bits <<= 1;
	}
	return (0);
}

void			free_tree(t_btree **tree)
{
	if (*tree != NULL)
	{
		free_tree(&(*tree)->b0);
		free_tree(&(*tree)->b1);
		free(*tree);
		*tree = NULL;
	}
}

int				gen_default_tree(t_png *png)
{
	short		val;
	int			ret;

	val = -1;
	while (++val < 288)
	{
		if (val < 144)
			ret = place_node(&png->ltree, 0x30 + val, 8, val);
		else if (val < 256)
			ret = place_node(&png->ltree, 0x190 + val - 144, 9, val);
		else if (val < 280)
			ret = place_node(&png->ltree, val - 256, 7, val);
		else
			ret = place_node(&png->ltree, 0xC0 + val - 280, 8, val);
		if (ret < 0)
			return (ret);
	}
	val = -1;
	while (++val < 31)
		if ((ret = place_node(&png->dtree, val, 5, val)) < 0)
			return (ret);
	return (0);
}
