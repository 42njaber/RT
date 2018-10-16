/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   switch_scene.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <njaber@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/16 15:40:14 by njaber            #+#    #+#             */
/*   Updated: 2018/10/16 16:08:00 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void		init_scene_view(t_view *view, t_ivec size, t_ocl *ocl)
{
	int		err;

	view->size = size;
	size = ivec(size.v[0] * 4, size.v[1] * 4);
	view->scene_buf = cl_img2d(ocl, CL_MEM_READ_WRITE, size, &err);
	if (err != CL_SUCCESS)
		ft_error("Error while creating thumbnail buffer\n");
	view->set.progressive = 1;
	view->set.max_rays = 4;
	view->set.fov = 90;
}

static void		create_memobjs(t_view *view, cl_context context)
{
	int		err;

	view->objbuf = clCreateBuffer(context, CL_MEM_COPY_HOST_PTR,
			sizeof(t_obj) * view->nobjs, view->objs, &err);
	if (err != CL_SUCCESS)
		ft_error("Error while creating view buffers\n");
	view->spotbuf = clCreateBuffer(context, CL_MEM_COPY_HOST_PTR,
			sizeof(t_spot) * view->nspots, view->spots, &err);
	if (err != CL_SUCCESS)
		ft_error("Error while creating view buffers\n");
}

void		init_scene(t_ptr *p)
{
	init_scene_view(&p->view, p->win->size, p->opencl);
	copy_scene_data(&p->view, ((t_scene**)p->scenes.elements)[p->gui.scene_id]);
	generate_cam_matrices(&p->view);
	generate_obj_matrices(&p->view);
	create_memobjs(&p->view, p->opencl->gpu_context);
	p->gui.state = ZOOM_SCENE;
	p->gui.zoom = 0;
	p->update = 1;
}
