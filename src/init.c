/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/20 13:17:27 by njaber            #+#    #+#             */
/*   Updated: 2018/10/16 16:01:00 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void		init_gui(t_ptr *p)
{
	int		err;

	p->scene = cl_img2d(p->opencl, CL_MEM_READ_WRITE,
			ivec(p->win->img.size.v[0], p->win->img.size.v[1]), &err);
	if (err != CL_SUCCESS)
		ft_error("Error while creationg OpenCL images %<i,R>"
					"(errcode: %d)%<0>\n", err);
	p->view.scene_buf = cl_img2d(p->opencl, CL_MEM_READ_WRITE,
			ivec(p->win->img.size.v[0] * 4, p->win->img.size.v[1] * 4), &err);
	if (err != CL_SUCCESS)
		ft_error("Error while creating OpenCL images %<i,R>"
					"(errcode: %d)%<0>\n", err);
	p->gui.state = MENU;
}

static void		init_view(t_ptr *p)
{
	(void)p;
}

void			init_struct(t_ptr *p)
{
	if ((p->win = (t_win*)ft_memalloc(sizeof(t_win))) == NULL)
		ft_error("[Erreur] Failed to allocate memory\n");
	init_new_image(p->mlx, &p->win->img,
			ivec(DEFAULT_WIDTH, DEFAULT_HEIGHT));
	init_view(p);
	p->update = 1;
	if ((p->opencl = init_opencl()) == NULL || 
		(p->kernel = create_kernel(p)) == NULL)
		ft_error("Could not initialize OpenCL, quiting...\n");
	gen_thumbnails(p);
	init_gui(p);
}

t_obj			*default_obj(t_scene *scene)
{
	t_obj	*ret;

	scene->nobjs++;
	if (scene->nobjs > scene->obj_pbufsize)
		ft_realloc((void**)&scene->objs, scene->obj_pbufsize * sizeof(t_obj),
				(scene->obj_pbufsize *= 2) * sizeof(t_obj));
	ret = scene->objs + scene->nobjs - 1;
	ret->type = PLANE;
	ret->pos = vec3(0, 0, 0);
	ret->rot = vec3(0, 0, 0);
	ret->color = 0xFFFFFF;
	ret->size = vec3(1, 1, 1);
	ret->reflect = 0;
	return (ret);
}

t_spot			*default_spot(t_scene *scene)
{
	t_spot	*ret;

	scene->nspots++;
	if (scene->nspots > scene->spot_pbufsize)
		ft_realloc((void**)&scene->spots, scene->spot_pbufsize * sizeof(t_spot),
				(scene->spot_pbufsize *= 2) * sizeof(t_spot));
	ret = scene->spots + scene->nspots - 1;
	ret->lum = 0;
	ret->pos = vec3(0, 0, 0);
	return (ret);
}
