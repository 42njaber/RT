/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gen_matricies.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/30 01:22:13 by njaber            #+#    #+#             */
/*   Updated: 2018/06/01 18:34:34 by njaber           ###   ########.fr       */
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
	rotate(p->cam_mat, (t_vec3){p->rot.x, 0, 0});
	rotate(p->cam_mat, (t_vec3){0, p->rot.y, 0});
	translate(p->cam_mat, (t_vec3){p->pos.x, p->pos.y, p->pos.z});
	identity(p->cam_mat_rot);
	rotate(p->cam_mat_rot, (t_vec3){p->rot.x, 0, 0});
	rotate(p->cam_mat_rot, (t_vec3){0, p->rot.y, 0});
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
		translate(p->objs[i].transform, (t_vec3){-p->objs[i].pos.x,
				-p->objs[i].pos.y, -p->objs[i].pos.z});
		rotate(p->objs[i].transform, (t_vec3){-p->objs[i].rot.x,
				-p->objs[i].rot.y, -p->objs[i].rot.z});
		scale(p->objs[i].transform, (t_vec3){1 / p->objs[i].size.x,
				1 / p->objs[i].size.y, 1 / p->objs[i].size.z});
		identity(p->objs[i].rot_mat);
		rotate(p->objs[i].rot_mat, (t_vec3){-p->objs[i].rot.x,
				-p->objs[i].rot.y, -p->objs[i].rot.z});
		scale(p->objs[i].rot_mat, (t_vec3){1 / p->objs[i].size.x,
				1 / p->objs[i].size.y, 1 / p->objs[i].size.z});
		identity(p->objs[i].rev_rot);
		rotate(p->objs[i].rev_rot, (t_vec3){p->objs[i].rot.x,
				p->objs[i].rot.y, p->objs[i].rot.z});
	}
}
