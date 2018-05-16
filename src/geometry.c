/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   geometry.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/16 20:35:52 by njaber            #+#    #+#             */
/*   Updated: 2018/05/17 01:19:02 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

int			is_hit(t_obj *obj, t_vec3 v)
{
	v = (t_vec3){v.x - obj->pos.x, v.y - obj->pos.y, v.z - obj->pos.z};
	if (obj->type == 0 && sqr(v.x) + sqr(v.y) + sqr(v.z) <= sqr(obj->size))
		return (1);
	else if (obj->type == 1 && v.z > 0)
		return (1);
	else if (obj->type == 2 && sqr(v.x) + sqr(v.z) <= sqr(obj->size))
		return (1);
	else if (obj->type == 3 && sqr(v.x) + sqr(v.z) <= sqr(obj->size * v.y))
		return (1);
	else
		return (0);
}

t_vec3		get_normal(t_obj *obj, t_vec3 v)
{
	v = (t_vec3){v.x - obj->pos.x, v.y - obj->pos.y, v.z - obj->pos.z};
	if (obj->type == 0)
		return ((t_vec3){-v.x, -v.y, -v.z});
	else if (obj->type == 1)
		return ((t_vec3){0, 0, 1});
	else if (obj->type == 2)
		return ((t_vec3){-v.x, 0, -v.z});
	else if (obj->type == 3)
		return ((t_vec3){-v.x, v.y * obj->size, -v.z});
	else
		return ((t_vec3){0, 0, 1});
}
