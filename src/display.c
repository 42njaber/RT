/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/16 05:31:19 by njaber            #+#    #+#             */
/*   Updated: 2018/05/26 00:16:25 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

static int			can_trace(t_ptr *p, t_vec3 v1, t_vec3 v2)
{
	t_vec3	dir;
	double	len;
	double	t;
	int		i;

	dir = vec_sub(v2, v1);
	len = length(dir);
	dir = (t_vec3){dir.x / len, dir.y / len, dir.z / len};
	i = -1;
	while (++i < p->nobjs)
		if (is_hit(&p->objs[i], v1, dir, &t) && t < len)
			return (0);
	return (1);
}

static double		get_luminance(t_ptr *p, t_vec3 v, t_vec3 n, t_vec3 ray)
{
	t_vec3	r_in;
	t_vec3	r_out;
	double	lum;
	double	reflect;
	int		i;

	reflect = 0;
	lum = 0.15;
	i = -1;
	while (++i < p->nspots)
		if (p->shadows == 0 || can_trace(p, p->spots[i].pos, v))
		{
			r_in = vec_sub(v, p->spots[i].pos);
			r_out = vec_sub(vec_mult(n, 2 * scalar(n, r_in) / length(n)), r_in);
			if (p->brillance == 1)
				reflect = pow(fmax(scalar(r_out, ray), 0) /
						(length(r_out) * length(ray)), 299);
			else if (p->brillance == 2)
				reflect = pow(scalar(r_in, n) /
						(length(r_in) * length(n)), 100);
			lum += (scalar(r_in, n) < 0) ? 0 : (p->spots[i].lum *
				scalar(r_in, n) / pow(length(r_in), 3) / length(n) +
				pow(p->spots[i].lum, 0.25) * reflect);
		}
	return (lum);
}

static unsigned int	get_color(t_ptr *p, t_obj *obj, t_vec3 v, t_vec3 ray)
{
	double	lum;

	lum = get_luminance(p, v, get_normal(obj, v), ray);
	if (lum <= 1)
		return (color_gradiant((unsigned int[2])
				{0x00000000, obj->color}, lum));
	else
		return (color_gradiant((unsigned int[2])
				{0x00FFFFFF, obj->color}, 1 / lum));
}

static unsigned int	trace(t_ptr *p, t_vec3 dir)
{
	double	hit;
	double	tmp;
	t_vec3	v;
	int		obj_hit;
	int		i;

	v = apply_mat_vec3(dir, p->cam_mat);
	dir = apply_mat_vec3(dir, p->cam_mat_rot);
	hit = p->far + 1;
	tmp = -1;
	i = -1;
	while (++i < p->nobjs)
		if (is_hit(&p->objs[i], v, dir, &tmp) && tmp < hit)
		{
			hit = tmp;
			obj_hit = i;
		}
	hit -= 0.01;
	if (hit < p->far)
		return (get_color(p, &p->objs[obj_hit],
					vec_add(v, vec_mult(dir, hit)), dir));
	else
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
				(ft_max(p->scene->size.x, p->scene->size.y) / 2) / s * 1,
				(double)(px.y - p->scene->size.y / 2) /
				(ft_max(p->scene->size.x, p->scene->size.y) / 2) / s * 1,
				1});
		img_px(p->scene, color, px);
		px.x = (px.x + 1) % p->scene->size.x;
		if (px.x == 0)
		{
			px.y++;
			ft_printf("\rComputing image... %.1f%%",
					(double)px.y / p->scene->size.y * 100);
		}
	}
	ft_printf("\n");
}
