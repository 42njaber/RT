/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gen_matricies.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/30 01:22:13 by njaber            #+#    #+#             */
/*   Updated: 2018/10/12 01:10:13 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

/*
** p: the program's main structure
**
** Generates the matricies for the camera and all the objects of the scene
*/

void		generate_cam_matricies(t_ptr *p)
{
	identity(p->cam_mat);
	rotate(p->cam_mat, vec3(p->rot.v[0], 0, 0));
	rotate(p->cam_mat, vec3(0, p->rot.v[1], 0));
	translate(p->cam_mat, vec3(p->pos.v[0], p->pos.v[1], p->pos.v[2]));
	identity(p->cam_mat_rot);
	rotate(p->cam_mat_rot, vec3(p->rot.v[0], 0, 0));
	rotate(p->cam_mat_rot, vec3(0, p->rot.v[1], 0));
}

/*
** p: the program's main structure
**
** Generates the matricies for the all the objects of the scene
*/

void		generate_obj_matricies(t_ptr *p)
{
	int		i;

	i = -1;
	while (++i < p->nobjs)
	{
		identity(p->objs[i].transform);
		translate(p->objs[i].transform, vec3(-p->objs[i].pos.v[0],
				-p->objs[i].pos.v[1], -p->objs[i].pos.v[2]));
		rotate(p->objs[i].transform, vec3(-p->objs[i].rot.v[0],
				-p->objs[i].rot.v[1], -p->objs[i].rot.v[2]));
		scale(p->objs[i].transform, vec3(1 / p->objs[i].size.v[0],
				1 / p->objs[i].size.v[1], 1 / p->objs[i].size.v[2]));
		identity(p->objs[i].rot_mat);
		rotate(p->objs[i].rot_mat, vec3(-p->objs[i].rot.v[0],
				-p->objs[i].rot.v[1], -p->objs[i].rot.v[2]));
		scale(p->objs[i].rot_mat, vec3(1 / p->objs[i].size.v[0],
				1 / p->objs[i].size.v[1], 1 / p->objs[i].size.v[2]));
		identity(p->objs[i].rev_rot);
		rotate(p->objs[i].rev_rot, vec3(p->objs[i].rot.v[0],
				p->objs[i].rot.v[1], p->objs[i].rot.v[2]));
	}
}
