/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/20 01:17:25 by njaber            #+#    #+#             */
/*   Updated: 2018/10/12 00:45:44 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libgxns.h"

t_vec2	vec2_sub(t_vec2 v1, t_vec2 v2)
{
	return (vec2(v1.v[0] - v2.v[0], v1.v[1] - v2.v[1]));
}

t_vec2	vec2_add(t_vec2 v1, t_vec2 v2)
{
	return (vec2(v1.v[0] + v2.v[0], v1.v[1] + v2.v[1]));
}

t_vec2	vec2_mult(t_vec2 v1, t_scal d)
{
	return (vec2(v1.v[0] * d, v1.v[1] * d));
}

t_scal	vec2_length(t_vec2 v1)
{
	return (sqrt(sqr(v1.v[0]) + sqr(v1.v[1])));
}

t_scal	vec2_scalar(t_vec2 v1, t_vec2 v2)
{
	return (v1.v[0] * v2.v[0] + v1.v[1] * v2.v[1]);
}
