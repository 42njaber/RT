/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cfg_default.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/25 14:19:58 by njaber            #+#    #+#             */
/*   Updated: 2018/05/29 02:26:47 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

/*
** p: the program's main structure
** def: a char[3] describing which elements should be set, index 0 corresponds
**      to the camera, 1 to the objects, and 2 to the spots
**
** Sets up a default scene composed of 2 planes, 1 sphere, and 2 spots.
*/

void		set_default_scene(t_ptr *p, char *def)
{
	if (def[0] == 1)
	{
		p->pos = (t_vec3){0, 0, 0};
		p->rot = (t_vec3){0, 0, 0};
	}
	if (def[1] == 1)
	{
		p->nobjs = 3;
		if ((p->objs = ft_memalloc(sizeof(t_obj) * 3)) == NULL)
			ft_error("[Erreur] Failed to allocate memory\n");
		p->objs[0] = (t_obj){0, (t_vec3){20, 0, 50}, (t_vec3){0, 0, 0},
			(t_vec3){10, 10, 10}, 0x00FFAA00, {0}, {0}, {0}};
		p->objs[1] = (t_obj){1, (t_vec3){0, 0, 70}, (t_vec3){0, 0, 0},
			(t_vec3){1, 1, 1}, 0x00FF22FF, {0}, {0}, {0}};
		p->objs[2] = (t_obj){1, (t_vec3){0, 20, 0}, (t_vec3){90, 0, 0},
			(t_vec3){1, 1, 1}, 0x0000AA00, {0}, {0}, {0}};
	}
	if (def[2] == 1)
	{
		p->nspots = 1;
		if ((p->spots = ft_memalloc(sizeof(t_spot) * p->nspots)) == NULL)
			ft_error("[Erreur] Failed to allocate memory\n");
		p->spots[0] = (t_spot){(t_vec3){0, -20, -10}, 2000};
	}
}