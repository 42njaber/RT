/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scenes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <njaber@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/16 17:31:05 by njaber            #+#    #+#             */
/*   Updated: 2018/10/26 23:36:36 by njaber           ###   ########.fr       */
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
	scene->start_pos = vec3(0, 0, 0);
	scene->start_rot = vec2(0, 0);
	scene->ambiant_light = 0.15;
	scene->sky_color = 0x000000;
	add_helem(&p->scenes, name, scene);
	return (scene);
}

void		free_scene(void **data)
{
	t_scene		**scene;

	scene = (t_scene**)data;
	free_and_null((void**)scene);
}
