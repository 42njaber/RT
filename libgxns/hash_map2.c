/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_map2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <njaber@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/14 06:24:57 by njaber            #+#    #+#             */
/*   Updated: 2018/10/16 07:37:04 by njaber           ###   ########.fr       */
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

void		del_helem(t_hmap *hmap, const char *key, void (*del)(void**))
{
	size_t	i;

	i = 0;
	while (i < hmap->elem_count)
	{
		if (hmap->keys[i] == key)
		{
			free(hmap->keys[i]);
			del(&hmap->elements[i]);
			hmap->elem_count--;
			hmap->keys[i] = hmap->keys[hmap->elem_count];
			hmap->elements[i] = hmap->elements[hmap->elem_count];
			hmap->keys[hmap->elem_count] = NULL;
			hmap->elements[hmap->elem_count] = NULL;
		}
		else
			i++;
	}
}
