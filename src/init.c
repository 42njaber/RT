/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/20 13:17:27 by njaber            #+#    #+#             */
/*   Updated: 2018/10/14 12:02:52 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

/*
** p: the program's main structure
**
** Initializes the structure's value and creates the buffer for the computed
** image
*/

void			init_struct(t_ptr *p)
{
	if ((p->win = (t_win*)ft_memalloc(sizeof(t_win))) == NULL)
		ft_error("[Erreur] Failed to allocate memory\n");
	init_new_image(p->mlx, &p->win->img, ivec(1200, 800));
	generate_cam_matricies(p);
	generate_obj_matricies(p);
	p->near = 0.1;
	p->far = 50000;
	p->res = 0;
	p->max_reflections = 4;
	p->update = 1;
	p->opencl = init_opencl();
	p->kernel = create_kernel(p);
	if (p->kernel != NULL)
	{
		create_obj_memobjs(p);
		create_spot_memobjs(p);
	}
}

t_obj			*default_obj(t_ptr *p)
{
	t_obj	*ret;

	p->nobjs++;
	if (p->nobjs > p->obj_pbufsize)
		ft_realloc((void**)&p->objs, p->obj_pbufsize * sizeof(t_obj),
				(p->obj_pbufsize *= 2) * sizeof(t_obj));
	ret = p->objs + p->nobjs - 1;
	ret->type = PLANE;
	ret->pos = vec3(0, 0, 0);
	ret->rot = vec3(0, 0, 0);
	ret->color = 0xFFFFFF;
	ret->size = vec3(1, 1, 1);
	ret->reflect = 0;
	return (ret);
}

t_spot			*default_spot(t_ptr *p)
{
	t_spot	*ret;

	p->nspots++;
	if (p->nspots > p->spot_pbufsize)
		ft_realloc((void**)&p->spots, p->spot_pbufsize * sizeof(t_spot),
				(p->spot_pbufsize *= 2) * sizeof(t_spot));
	ret = p->spots + p->nspots - 1;
	ret->lum = 0;
	ret->pos = vec3(0, 0, 0);
	return (ret);
}
