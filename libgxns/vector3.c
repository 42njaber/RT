/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/20 01:17:25 by njaber            #+#    #+#             */
/*   Updated: 2018/10/12 00:46:02 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

t_vec3	vec3_sub(t_vec3 v1, t_vec3 v2)
{
	return (vec3(v1.v[0] - v2.v[0], v1.v[1] - v2.v[1], v1.v[2] - v2.v[2]));
}

t_vec3	vec3_add(t_vec3 v1, t_vec3 v2)
{
	return (vec3(v1.v[0] + v2.v[0], v1.v[1] + v2.v[1], v1.v[2] + v2.v[2]));
}

t_vec3	vec3_mult(t_vec3 v1, t_scal d)
{
	return (vec3(v1.v[0] * d, v1.v[1] * d, v1.v[2] * d));
}

t_scal	vec3_length(t_vec3 v1)
{
	return (sqrt(sqr(v1.v[0]) + sqr(v1.v[1]) + sqr(v1.v[2])));
}

t_scal	vec3_scalar(t_vec3 v1, t_vec3 v2)
{
	return (v1.v[0] * v2.v[0] + v1.v[1] * v2.v[1] + v1.v[2] * v2.v[2]);
}
