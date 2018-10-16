/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gen_matrices.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/30 01:22:13 by njaber            #+#    #+#             */
/*   Updated: 2018/10/16 15:01:39 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

/*
** p: the program's main structure
**
** Generates the matricies for the camera and all the objects of the scene
*/

void		generate_cam_matrices(t_view *view)
{
	identity(view->set.cam_mat);
	rotate(view->set.cam_mat, vec3(view->rot.v[0], 0, 0));
	rotate(view->set.cam_mat, vec3(0, view->rot.v[1], 0));
	translate(view->set.cam_mat, vec3(view->pos.v[0], view->pos.v[1],
									view->pos.v[2]));
	identity(view->set.cam_mat_rot);
	rotate(view->set.cam_mat_rot, vec3(view->rot.v[0], 0, 0));
	rotate(view->set.cam_mat_rot, vec3(0, view->rot.v[1], 0));
}

/*
** view: the program's main structure
**
** Generates the matricies for the all the objects of the scene
*/

void		generate_obj_matrices(t_view *view)
{
	int		i;

	i = -1;
	while (++i < view->nobjs)
	{
		identity(view->objs[i].transform);
		translate(view->objs[i].transform, vec3(-view->objs[i].pos.v[0],
				-view->objs[i].pos.v[1], -view->objs[i].pos.v[2]));
		rotate(view->objs[i].transform, vec3(-view->objs[i].rot.v[0],
				-view->objs[i].rot.v[1], -view->objs[i].rot.v[2]));
		scale(view->objs[i].transform, vec3(1 / view->objs[i].size.v[0],
				1 / view->objs[i].size.v[1], 1 / view->objs[i].size.v[2]));
		identity(view->objs[i].rot_mat);
		rotate(view->objs[i].rot_mat, vec3(-view->objs[i].rot.v[0],
				-view->objs[i].rot.v[1], -view->objs[i].rot.v[2]));
		scale(view->objs[i].rot_mat, vec3(1 / view->objs[i].size.v[0],
				1 / view->objs[i].size.v[1], 1 / view->objs[i].size.v[2]));
		identity(view->objs[i].rev_norm);
		scale(view->objs[i].rev_norm, vec3(1 / view->objs[i].size.v[0],
				1 / view->objs[i].size.v[1], 1 / view->objs[i].size.v[2]));
		rotate(view->objs[i].rev_norm, vec3(view->objs[i].rot.v[0],
				view->objs[i].rot.v[1], view->objs[i].rot.v[2]));
	}
}
