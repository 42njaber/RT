/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <njaber@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/26 17:49:16 by njaber            #+#    #+#             */
/*   Updated: 2018/10/27 02:42:17 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void		destroy_hmap(t_hmap *hmap, void (*del)(void**))
{
	size_t	i;

	i = 0;
	while (i < hmap->elem_count)
	{
		if (hmap->keys[i] != NULL)
			free(hmap->keys[i]);
		if (del != NULL && hmap->elements[i] != NULL)
			del(&hmap->elements[i]);
		i++;
	}
	if (hmap->keys != NULL)
	{
		free(hmap->keys);
		hmap->keys = NULL;
	}
	if (hmap->elements != NULL)
	{
		free(hmap->elements);
		hmap->elements = NULL;
	}
	hmap->prebuf_size = 0;
	hmap->elem_count = 0;
}

static void	realloc_buf(t_hmap *hmap)
{
	void	**tmp_elem;
	char	**tmp_key;

	hmap->prebuf_size *= 2;
	if ((tmp_elem = (void**)ft_memalloc(sizeof(void*) *
					hmap->prebuf_size)) == NULL)
		ft_error("Malloc error\n");
	if ((tmp_key = (void*)ft_memalloc(sizeof(char*) *
					hmap->prebuf_size)) == NULL)
	{
		free(tmp_elem);
		ft_error("Malloc error\n");
	}
	ft_memcpy(tmp_elem, hmap->elements, hmap->elem_count * sizeof(void*));
	ft_memcpy(tmp_key, hmap->keys, hmap->elem_count * sizeof(char*));
	free(hmap->elements);
	free(hmap->keys);
	hmap->elements = tmp_elem;
	hmap->keys = tmp_key;
}

size_t		add_helem(t_hmap *hmap, char *key, void *data)
{
	if (hmap->elem_count >= hmap->prebuf_size)
		realloc_buf(hmap);
	if ((hmap->keys[hmap->elem_count] = ft_strdup(key)) == NULL)
		ft_error("Malloc error\n");
	hmap->elements[hmap->elem_count] = data;
	return (hmap->elem_count++);
}

void		*get_helem(t_hmap *hmap, char *key)
{
	size_t	i;

	i = 0;
	while (i < hmap->elem_count)
	{
		if (ft_strcmp(hmap->keys[i], key) == 0)
			return (hmap->elements[i]);
		i++;
	}
	return (NULL);
}

void		init_hmap(t_hmap *hmap)
{
	hmap->prebuf_size = 8;
	hmap->elem_count = 0;
	if ((hmap->elements = (void**)ft_memalloc(sizeof(void*) *
					hmap->prebuf_size)) == NULL)
		ft_error("Malloc error\n");
	if ((hmap->keys = (char**)ft_memalloc(sizeof(char*) *
					hmap->prebuf_size)) == NULL)
		ft_error("Malloc error\n");
}
