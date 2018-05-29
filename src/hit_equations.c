/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_equations.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/16 20:35:52 by njaber            #+#    #+#             */
/*   Updated: 2018/05/29 02:40:11 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

/*
** ori: a point in space describing the origin of the ray
** dir: a direction vector describing the ray
** t: the location to stock the solution to the equation, which coressponds
**    to the distance from the origin to the closest intersection point, mesured
**    by the length of dir
**
** a, b, c, d: temporary variables to resolve the equation
**
** Resolves the equation sqr(ori.x + t * dir.x) + sqr(ori.y + t * dir.y) +
** sqr(ori.z + t * dir.z) = sqr(radius), where radius is the radius of the
** object.
** To do that, we first first put the equation in the form
** a * sqr(t) + b * t + c = 0, and calculate the delta (sqr(b) - 4 * a * c)
** If the delta is negative, the equation has no solution, else, calculate
** the 2 intersection points (which can be ovelaped if the ray is tangent to the
** shpere), and then take the smallest positive result as solution, or non
** if both are negative, and put it in t.
**
** returns: 1 if a positive solution to the equation was found, 0 otherwise
*/

static int	sphere_hit(t_vec3 ori, t_vec3 dir, float *t)
{
	float	a;
	float	b;
	float	c;
	float	d;

	a = sqr(dir.x) + sqr(dir.y) + sqr(dir.z);
	b = 2 * (ori.x * dir.x + ori.y * dir.y + ori.z * dir.z);
	c = sqr(ori.x) + sqr(ori.y) + sqr(ori.z) - 1;
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

/*
** ori: a point in space describing the origin of the ray
** dir: a direction vector describing the ray
** t: the location to stock the solution to the equation, which coressponds
**    to the distance from the origin to the closest intersection point, mesured
**    by the length of dir
**
** a: temporary variable to resolve the equation
**
** Resolves the equation ori.z + t * dir.z = 0
** if dir.z is not null, this equation always has a solution, if it's positive,
** take it.
**
** returns: 1 if a positive solution to the equation was found, 0 otherwise
*/

static int	plane_hit(t_vec3 ori, t_vec3 dir, float *t)
{
	float	a;

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

/*
** ori: a point in space describing the origin of the ray
** dir: a direction vector describing the ray
** t: the location to stock the solution to the equation, which coressponds
**    to the distance from the origin to the closest intersection point, mesured
**    by the length of dir
**
** a, b, c, d: temporary variables to resolve the equation
**
** Resolves the equation sqr(ori.x + t * dir.x) + sqr(ori.y + t * dir.y) =
** sqr(radius), where radius is the radius of the object.
** To do that, we first first put the equation in the form
** a * sqr(t) + b * t + c = 0, and calculate the delta (sqr(b) - 4 * a * c)
** If the delta is negative, the equation has no solution, else, calculate
** the 2 intersection points (which can be ovelaped if the ray is tangent to the
** shpere), and then take the smallest positive result as solution, or non
** if both are negative, and put it in t.
**
** returns: 1 if a positive solution to the equation was found, 0 otherwise
*/

static int	cylindre_hit(t_vec3 ori, t_vec3 dir, float *t)
{
	float	a;
	float	b;
	float	c;
	float	d;

	a = sqr(dir.x) + sqr(dir.z);
	b = 2 * (ori.x * dir.x + ori.z * dir.z);
	c = sqr(ori.x) + sqr(ori.z) - 1;
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

/*
** ori: a point in space describing the origin of the ray
** dir: a direction vector describing the ray
** t: the location to stock the solution to the equation, which coressponds
**    to the distance from the origin to the closest intersection point, mesured
**    by the length of dir
**
** a, b, c, d: temporary variables to resolve the equation
**
** Resolves the equation sqr(ori.x + t * dir.x) + sqr(ori.y + t * dir.y) =
** sqr(radius * y), where radius is the radius of the object.
** To do that, we first first put the equation in the form
** a * sqr(t) + b * t + c = 0, and calculate the delta (sqr(b) - 4 * a * c)
** If the delta is negative, the equation has no solution, else, calculate
** the 2 intersection points (which can be ovelaped if the ray is tangent to the
** shpere), and then take the smallest positive result as solution, or non
** if both are negative, and put it in t.
**
** returns: 1 if a positive solution to the equation was found, 0 otherwise
*/

static int	cone_hit(t_vec3 ori, t_vec3 dir, float *t)
{
	float	a;
	float	b;
	float	c;
	float	d;

	a = sqr(dir.x) + sqr(dir.z) - sqr(dir.y);
	b = 2 * (ori.x * dir.x + ori.z * dir.z - ori.y * dir.y);
	c = sqr(ori.x) + sqr(ori.z) - sqr(ori.y);
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

/*
** obj: the object with which the intersection is being tested
** ori: a point in space describing the origin of the ray
** dir: a direction vector describing the ray
** t: the location to stock the solution to the equation, which coressponds
**    to the distance from the origin to the closest intersection point, mesured
**    by the length of dir
**
** Finds the closest intersection between the ray and the object, if any exists,
** by first transforming the ray to put it in the object's coordinate system,
** and then redirecting to the right equation-solving function depending on the
** type of the object
**
** returns: 1 if an intersection was found, 0 otherwise
*/

int			does_intersect(t_obj *obj, t_vec3 origin, t_vec3 dir, float *t)
{
	origin = apply_mat_vec3(origin, obj->transform);
	dir = apply_mat_vec3(dir, obj->rot_mat);
	if (obj->type == SPHERE)
		return (sphere_hit(origin, dir, t));
	else if (obj->type == PLANE)
		return (plane_hit(origin, dir, t));
	else if (obj->type == CYLINDER)
		return (cylindre_hit(origin, dir, t));
	else if (obj->type == CONE)
		return (cone_hit(origin, dir, t));
	else
		return (0);
}