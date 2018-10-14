/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_map2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <njaber@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/14 06:24:57 by njaber            #+#    #+#             */
/*   Updated: 2018/10/14 06:26:58 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void		reset_hmap(t_hmap *hmap, void (*del)(void**))
{
	size_t	i;

	i = 0;
	while (i < hmap->elem_count)
	{
		if (hmap->keys[i] != NULL)
		{
			free(hmap->keys[i]);
			hmap->keys[i] = NULL;
		}
		if (del != NULL && hmap->elements[i] != NULL)
			del(&hmap->elements[i]);
		i++;
	}
	hmap->elem_count = 0;
}

