/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   normal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/16 20:35:52 by njaber            #+#    #+#             */
/*   Updated: 2018/05/29 02:41:18 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

/*
** obj: the object
** v: a point on the surface of the object
**
** Calculates the normal at a given point of the object's surface, depending
** on the type of object
** The functions first transform the points to put it in the object's coordinate
** system, then transforms the vector to put it back in the main coordinate
** system
**
** returns: a non-normalized vector representing the normal
*/

t_vec3		get_normal(t_obj *obj, t_vec3 v)
{
	t_vec3	ret;

	v = apply_mat_vec3(v, obj->transform);
	if (obj->type == 0)
		ret = (t_vec3){-v.x, -v.y, -v.z};
	else if (obj->type == 1)
		ret = (t_vec3){0, 0, copysign(1, -v.z)};
	else if (obj->type == 2)
		ret = (t_vec3){-v.x, 0, -v.z};
	else if (obj->type == 3)
		ret = (t_vec3){-v.x, v.y, -v.z};
	else
		ret = (t_vec3){0, 0, 1};
	ret = apply_mat_vec3(ret, obj->rev_rot);
	return (ret);
}
