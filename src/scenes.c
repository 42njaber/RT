/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scenes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <njaber@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/16 17:31:05 by njaber            #+#    #+#             */
/*   Updated: 2018/10/16 17:31:32 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

t_scene		*new_scene(t_ptr *p, char *name)
{
	t_scene	*scene;

	if (get_helem(&p->scenes, name) != NULL)
		return (NULL);
	if ((scene = (t_scene*)ft_memalloc(sizeof(t_scene))) == NULL)
		ft_error("Malloc error\n");
	add_helem(&p->scenes, name, scene);
	return (scene);
}

void		free_scene(void **data)
{
	t_scene		**scene;

	scene = (t_scene**)data;
	free_and_null((void**)scene);
}
