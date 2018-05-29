/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/16 05:31:19 by njaber            #+#    #+#             */
/*   Updated: 2018/05/27 02:01:22 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** TODO
** Create a function to normalize vectors in the library, and update all the
** functions in this file acordingly
*/

#include <time.h>
#include "rt.h"

/*
** p: the program's main structure
** v1, v2: 2 points in space
**
** dir: a normalized direction vector pointing form v1 to v2
** len: the distance between v1 and v2
** i: a counter for the object which is currently being tested for intersection
**
** This functions tests if it's possible to trace a straight line between v1
** and v2 without intersecting any object of the scene.
**
** return: 0 if the line intersects an object, or 1 otherwise
*/

static int			can_trace(t_ptr *p, t_vec3 v1, t_vec3 v2)
{
	t_vec3	dir;
	float	len;
	float	t;
	int		i;

	dir = vec_sub(v2, v1);
	len = length(dir);
	dir = (t_vec3){dir.x / len, dir.y / len, dir.z / len};
	i = -1;
	while (++i < p->nobjs)
		if (does_intersect(&p->objs[i], v1, dir, &t) && t < len)
			return (0);
	return (1);
}

/*
** p: the program's main structure
** v: a point on the surface of an object (preferably a little bit closer to the
**    camera to prevent float imprecisions)
** n: a non-normalized vector representing the normal of the surface
**    at that point
** ray: a non-normalized direction vector representing a light ray that comes
**      from v to the camera
**
** r_in: a non-normalized direction vector representing a light ray going from
**       a spot to v (incident ray)
** r_out: a non-normalized direction vector representing the light ray that was
**        reflected from r_in by the object
** lum: the total amount of luminosity coming from v to the camera
**      ranging from 0 to +INF
** reflected: if brilliance in on, the amount of light reflected into the camera
**            by the object at v
** i: a counter for the spot which light's is currently being processed
**
** Calculates the amount of light emmited by the point v, this is done by taking
** the ambiant light, which is set to an arbitrary value (currently 0.15), and
** adding it with a light value calculated independently for each spot.
** The light from a spot is calulated by adding the dispersed light with,
** if the brilliance option is on, the light reflected by the object.
** The dispersed light is calculated by dividing the light emmited by the spot
** by the distance from the spot to v squared, and then multiplying it by the
** cos of the angle between r_in and n (if the cos is negative, this value is
** considered to be 0).
** The brilliance value is calculated by getting the cos of the angle between
** 2 vectors, which vectors are taken for the calculations depends on the
** brilliance option. The 2 possible options are:
** [r_in, n] which creates a reflection that doesn't depend on the way
** the camera is placed
** [r_out, ray] which creates a more realistic reflection that depends on the
** camera placement.
** The cos is then taken to an arbitrary power (the higher the power,
** the smaller the reflection is) and multiplied by the light value of the spot.
** If the shadows option is on, before calculating the value for each spot,
** checks wether the light ray from the spot is obstructed by an object, if it
** is, the value for this particular spot is considered to be 0
**
** returns: lum
*/

static float		get_luminance(t_ptr *p, t_vec3 v, t_vec3 n, t_vec3 ray)
{
	t_vec3	r_in;
	t_vec3	r_out;
	float	lum;
	float	reflected;
	int		i;

	reflected = 0;
	lum = 0.15;
	i = -1;
	while (++i < p->nspots)
		if (p->shadows == 0 || can_trace(p, p->spots[i].pos, v))
		{
			r_in = vec_sub(v, p->spots[i].pos);
			r_out = vec_sub(r_in, vec_mult(n, 2 * scalar(n, r_in) / length(n)));
			if (p->brilliance == 1)
				reflected = pow(fmax(scalar(r_out, ray), 0) /
						(length(r_out) * length(ray)), 1200);
			else if (p->brilliance == 2)
				reflected = pow(scalar(r_in, n) /
						(length(r_in) * length(n)), 400);
			lum += (scalar(r_in, n) < 0) ? 0 : (p->spots[i].lum *
				scalar(r_in, n) / pow(length(r_in), 3) / length(n) +
				p->spots[i].lum * reflected);
		}
	return (lum);
}

/*
** p: the program's main structure
** obj: the structure describing the object which was hit by the ray
** v: a point on the surface of the object
** ray: a non-normalized direction vector representing a ray that was traced
**      from the camera to v
**
** lum: the total amount of luminosity coming from v to the camera
**      ranging from 0 to +INF
**
** Calculates the color of a pixel by getting its luminosity, and mixing the
** object's color accordingly.
** If the luminosity is lesser then 1, the color is darkened acordingly, if it's
** greater then 1, the color is lightened accordingly
**
** returns: the final color of the pixel
*/

static unsigned int	get_color(t_ptr *p, t_obj *obj, t_vec3 v, t_vec3 ray)
{
	float	lum;

	lum = get_luminance(p, v, get_normal(obj, v),
			(t_vec3){-ray.x, -ray.y, -ray.z});
	if (lum <= 1)
		return (color_gradiant((unsigned int[2])
				{0x00000000, obj->color}, lum));
	else
		return (color_gradiant((unsigned int[2])
				{0x00FFFFFF, obj->color}, 1 / lum));
}

/*
** p: the program's main structure
** dir: a non-normalized direction vector describing a ray that needs to be
**      traced from the camera
**
** hit: how far away from the camera the intersection with a given object is,
**      where 1 is the lengthof dir
** v: a point in space describing the origin of the ray
** obj_hit: the index of the closest object that was found to intersect
**          with the ray
** i: a counter for the object currently being tested
**
** Transforms the ray to put it in the camera's position, then tests the
** intersection of the ray with each object of the scene, and gets the color
** of the closest intersection found
**
** returns: the computed color of the closest intersection found, or black
**          if no intersection were found
*/

static unsigned int	trace(t_ptr *p, t_vec3 dir)
{
	float	hit;
	float	tmp;
	t_vec3	v;
	int		obj_hit;
	int		i;

	v = apply_mat_vec3(dir, p->cam_mat);
	dir = apply_mat_vec3(dir, p->cam_mat_rot);
	hit = p->far + 1;
	tmp = -1;
	i = -1;
	while (++i < p->nobjs)
		if (does_intersect(&p->objs[i], v, dir, &tmp) && tmp < hit)
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

/*
** p: the program's main structure
**
** px: the pixel currently being processed
** color: then computed color of the pixel being processed
** s: the number by which to multiply the spacement of the rays in order to
**    acount for fov
**
** Processes the image of the scene by creating a ray for each pixel and
** then processing the color of that pixel
*/

void				process_image(t_ptr *p)
{
	t_ivec			px;
	unsigned int	color;
	float			s;

	px = (t_ivec){0, 0};
	while (px.y < p->scene->size.y)
	{
		s = 1 / tan(p->fov * 0.5 * M_PI / 180);
		color = trace(p, (t_vec3){
				(float)(px.x - p->scene->size.x / 2) /
				(ft_max(p->scene->size.x, p->scene->size.y) / 2) / s * 1,
				(float)(px.y - p->scene->size.y / 2) /
				(ft_max(p->scene->size.x, p->scene->size.y) / 2) / s * 1,
				1});
		img_px(p->scene, color, px);
		px.x = (px.x + 1) % p->scene->size.x;
		if (px.x == 0)
			ft_printf("\rComputing image... %.1f%%",
					(float)px.y++ / p->scene->size.y * 100);
	}
	ft_printf("\nFinished\n");
}