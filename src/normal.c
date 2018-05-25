/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   normal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/16 20:35:52 by njaber            #+#    #+#             */
/*   Updated: 2018/05/25 21:41:02 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

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
		ret = (t_vec3){-v.x, v.y * sqr(obj->size), -v.z};
	else
		ret = (t_vec3){0, 0, 1};
	ret = apply_mat_vec3(ret, obj->rev_rot);
	return (ret);
}
