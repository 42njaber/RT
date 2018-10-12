/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/12 17:34:43 by njaber            #+#    #+#             */
/*   Updated: 2018/10/12 01:19:39 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libgxns.h"

t_vec3		apply_mat_vec3(t_vec3 v, t_mat4 m)
{
	t_vec3	ret;
	float	w;

	w = m[12] * v.v[0] + m[13] * v.v[1] + m[14] * v.v[2] + m[15];
	ret.v[0] = (m[0] * v.v[0] + m[1] * v.v[1] + m[2] * v.v[2] + m[3]) / w;
	ret.v[1] = (m[4] * v.v[0] + m[5] * v.v[1] + m[6] * v.v[2] + m[7]) / w;
	ret.v[2] = (m[8] * v.v[0] + m[9] * v.v[1] + m[10] * v.v[2] + m[11]) / w;
	return (ret);
}

void		print_mat(t_mat4 m)
{
	int		i;

	i = -1;
	while (++i < 16)
	{
		ft_printf("%5g", m[i]);
		if (i % 4 == 3)
			ft_printf("\n");
		else
			ft_printf(" | ");
	}
}
