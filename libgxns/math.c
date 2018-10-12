/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/23 01:35:36 by njaber            #+#    #+#             */
/*   Updated: 2018/10/12 01:20:39 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libgxns.h"

float			sqr(float n)
{
	return (n * n);
}

t_vec2			c_mult(t_vec2 z1, t_vec2 z2)
{
	return (vec2(z1.v[0] * z2.v[0] - z1.v[1] * z2.v[1],
			z1.v[0] * z2.v[1] + z1.v[1] * z2.v[0]));
}

t_vec2			c_pow(t_vec2 z1, int pow)
{
	t_vec2 ret;

	ret = z1;
	while (--pow > 0)
		ret = c_mult(ret, z1);
	return (ret);
}

t_vec2			c_add(t_vec2 z1, t_vec2 z2)
{
	return (vec2(z1.v[0] + z2.v[0], z1.v[1] + z2.v[1]));
}
