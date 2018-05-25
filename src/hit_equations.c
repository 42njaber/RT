/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_equations.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/16 20:35:52 by njaber            #+#    #+#             */
/*   Updated: 2018/05/26 00:30:21 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

static int	sphere_hit(t_obj *obj, t_vec3 ori, t_vec3 dir, double *t)
{
	double	a;
	double	b;
	double	c;
	double	d;

	a = sqr(dir.x) + sqr(dir.y) + sqr(dir.z);
	b = 2 * (ori.x * dir.x + ori.y * dir.y + ori.z * dir.z);
	c = sqr(ori.x) + sqr(ori.y) + sqr(ori.z) - sqr(obj->size);
	d = sqr(b) - 4 * a * c;
	if (d < 0)
		return (0);
	c = (-b - sqrt(d)) / (2 * a);
	d = (-b + sqrt(d)) / (2 * a);
	if (c <= 0 && d <= 0)
		return (0);
	if (c < 0 || d < 0)
		*t = fmax(c, d);
	else
		*t = fmin(c, d);
	return (1);
}

static int	plane_hit(t_vec3 ori, t_vec3 dir, double *t)
{
	double	a;

	if (ori.z == 0)
	{
		*t = 0;
		return (1);
	}
	if (dir.z == 0)
		return (0);
	a = -ori.z / dir.z;
	if (a < 0)
		return (0);
	*t = a;
	return (1);
}

static int	cylindre_hit(t_obj *obj, t_vec3 ori, t_vec3 dir, double *t)
{
	double	a;
	double	b;
	double	c;
	double	d;

	a = sqr(dir.x) + sqr(dir.z);
	b = 2 * (ori.x * dir.x + ori.z * dir.z);
	c = sqr(ori.x) + sqr(ori.z) - sqr(obj->size);
	d = sqr(b) - 4 * a * c;
	if (d < 0)
		return (0);
	c = (-b - sqrt(d)) / (2 * a);
	d = (-b + sqrt(d)) / (2 * a);
	if (c <= 0 && d <= 0)
		return (0);
	if (c < 0 || d < 0)
		*t = fmax(c, d);
	else
		*t = fmin(c, d);
	return (1);
}

static int	cone_hit(t_obj *obj, t_vec3 ori, t_vec3 dir, double *t)
{
	double	a;
	double	b;
	double	c;
	double	d;

	a = sqr(dir.x) + sqr(dir.z) - sqr(dir.y * obj->size);
	b = 2 * (ori.x * dir.x + ori.z * dir.z - ori.y * dir.y * sqr(obj->size));
	c = sqr(ori.x) + sqr(ori.z) - sqr(ori.y * obj->size);
	d = sqr(b) - 4 * a * c;
	if (d < 0)
		return (0);
	c = (-b - sqrt(d)) / (2 * a);
	d = (-b + sqrt(d)) / (2 * a);
	if (c <= 0 && d <= 0)
		return (0);
	if (c < 0 || d < 0)
		*t = fmax(c, d);
	else
		*t = fmin(c, d);
	return (1);
}

int			is_hit(t_obj *obj, t_vec3 origin, t_vec3 dir, double *t)
{
	origin = apply_mat_vec3(origin, obj->transform);
	dir = apply_mat_vec3(dir, obj->rot_mat);
	if (obj->type == 0)
		return (sphere_hit(obj, origin, dir, t));
	else if (obj->type == 1)
		return (plane_hit(origin, dir, t));
	else if (obj->type == 2)
		return (cylindre_hit(obj, origin, dir, t));
	else if (obj->type == 3)
		return (cone_hit(obj, origin, dir, t));
	else
		return (0);
}
