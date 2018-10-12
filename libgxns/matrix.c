/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/12 18:12:18 by njaber            #+#    #+#             */
/*   Updated: 2018/10/12 01:19:18 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libgxns.h"

void	multiply(t_mat4 m1, t_mat4 m2, int stock_in_m2)
{
	int		i;
	t_mat4	tmp;

	i = -1;
	while (++i < 16)
		tmp[i] = m1[i / 4 * 4] * m2[i % 4] +
				m1[i / 4 * 4 + 1] * m2[i % 4 + 4] +
				m1[i / 4 * 4 + 2] * m2[i % 4 + 8] +
				m1[i / 4 * 4 + 3] * m2[i % 4 + 12];
	if (stock_in_m2)
		ft_memcpy(m2, tmp, sizeof(tmp));
	ft_memcpy(m1, tmp, sizeof(tmp));
}

void	identity(t_mat4 m)
{
	int	i;

	i = -1;
	while (++i < 16)
		if (i % 4 == i / 4)
			m[i] = 1;
		else
			m[i] = 0;
}

void	translate(t_mat4 m, t_vec3 v)
{
	m[3] += v.v[0] * m[15];
	m[7] += v.v[1] * m[15];
	m[11] += v.v[2] * m[15];
}

void	scale(t_mat4 m, t_vec3 v)
{
	m[0] *= v.v[0];
	m[1] *= v.v[0];
	m[2] *= v.v[0];
	m[3] *= v.v[0];
	m[4] *= v.v[1];
	m[5] *= v.v[1];
	m[6] *= v.v[1];
	m[7] *= v.v[1];
	m[8] *= v.v[2];
	m[9] *= v.v[2];
	m[10] *= v.v[2];
	m[11] *= v.v[2];
}

void	rotate(t_mat4 m, t_vec3 v)
{
	v = vec3(v.v[0] / 180 * M_PI, v.v[1] / 180 * M_PI, v.v[2] / 180 * M_PI);
	multiply((t_mat4){cos(v.v[2]), -sin(v.v[2]), 0, 0,
						sin(v.v[2]), cos(v.v[2]), 0, 0,
						0, 0, 1, 0,
						0, 0, 0, 1}, m, 1);
	multiply((t_mat4){cos(v.v[1]), 0, -sin(v.v[1]), 0,
						0, 1, 0, 0,
						sin(v.v[1]), 0, cos(v.v[1]), 0,
						0, 0, 0, 1}, m, 1);
	multiply((t_mat4){1, 0, 0, 0,
						0, cos(v.v[0]), -sin(v.v[0]), 0,
						0, sin(v.v[0]), cos(v.v[0]), 0,
						0, 0, 0, 1}, m, 1);
}
