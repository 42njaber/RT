/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/16 05:31:19 by njaber            #+#    #+#             */
/*   Updated: 2018/05/17 01:31:26 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

#define PREC 0.4

static int			can_trace(t_ptr *p, t_vec3 v1, t_vec3 v2)
{
	t_vec3	v;
	t_vec3	step;
	double	dst;
	double	len;
	int		i;

	v = v1;
	dst = sqrt(sqr(v1.x - v2.x) + sqr(v1.y - v2.y) + sqr(v1.z + v2.z));
	step = (t_vec3){(v2.x - v1.x) / dst * PREC, (v2.y - v1.y) / dst * PREC,
		(v2.z - v1.z) / dst * PREC};
	len = 0;
	while (len < dst)
	{
		i = -1;
		while (++i < p->nobjs)
			if (is_hit(&p->objs[i], v))
				return (0);
		v = (t_vec3){v.x + step.x, v.y + step.y, v.z + step.z};
		len += PREC;
	}
	return (1);
}

static double		get_luminosity(t_ptr *p, t_vec3 v, t_vec3 n)
{
	t_vec3	r_pos;
	double	dst_sqr;
	double	scalar;
	double	r_cos;

	if (!can_trace(p, p->spot.pos, v))
		return (0.2);
	r_pos = (t_vec3){v.x - p->spot.pos.x, v.y - p->spot.pos.y,
		v.z - p->spot.pos.z};
	dst_sqr = sqr(r_pos.x) + sqr(r_pos.y) + sqr(r_pos.z);
	scalar = r_pos.x * n.x + r_pos.y * n.y + r_pos.z * n.z;
	r_cos = scalar / (sqrt(dst_sqr) * sqrt(sqr(n.x) + sqr(n.y) + sqr(n.z)));
	return (0.2 + (scalar < 0 ? 0 : p->spot.lum * r_cos / dst_sqr));
}

static unsigned int	trace(t_ptr *p, t_vec3 step)
{
	t_vec3	v;
	int		i;
	double	lum;

	v = (t_vec3){0, 0, 0};
	while (v.z < p->far)
	{
		i = -1;
		while (++i < p->nobjs)
			if (is_hit(&p->objs[i], v))
			{
				v = (t_vec3){v.x - step.x, v.y - step.y, v.z - step.z};
				lum = get_luminosity(p, v, get_normal(&p->objs[i], v));
				if (lum <= 1)
					return (color_gradiant((unsigned int[2])
							{0x00000000, p->objs[i].color}, lum));
				else
					return (color_gradiant((unsigned int[2])
							{0x00FFFFFF, p->objs[i].color}, 1 / lum));
			}
		v = (t_vec3){v.x + step.x, v.y + step.y, v.z + step.z};
	}
	return (0x000000);
}

void				draw_scene(t_ptr *p)
{
	t_ivec			px;
	unsigned int	color;
	double			s;

	px = (t_ivec){0, 0};
	while (px.y < p->scene->size.y)
	{
		s = 1 / tan(p->fov * 0.5 * M_PI / 180);
		color = trace(p, (t_vec3){
				(double)(px.x - p->scene->size.x / 2) /
				(ft_max(p->scene->size.x, p->scene->size.y) / 2) * s * PREC,
				(double)(px.y - p->scene->size.y / 2) /
				(ft_max(p->scene->size.x, p->scene->size.y) / 2) * s * PREC,
				PREC});
		img_px(p->scene, color, px);
		px.x = (px.x + 1) % p->scene->size.x;
		if (px.x == 0)
			px.y++;
		ft_printf("\rComputing image... %.1f%%",
				(double)px.y / p->scene->size.y * 100);
	}
	ft_printf("\n");
}
