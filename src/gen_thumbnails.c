/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gen_thumbnails.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <njaber@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/16 11:51:56 by njaber            #+#    #+#             */
/*   Updated: 2018/10/27 02:43:03 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

cl_mem			create_image_array(t_ocl *opencl, int len,
											t_ivec size, int *err)
{
	cl_image_format		format;
	cl_image_desc		desc;

	ft_bzero(&format, sizeof(format));
	ft_bzero(&desc, sizeof(desc));
	format.image_channel_order = CL_RGBA;
	format.image_channel_data_type = CL_UNSIGNED_INT8;
	desc.image_type = CL_MEM_OBJECT_IMAGE2D_ARRAY;
	desc.image_width = (size_t)size.v[0];
	desc.image_height = (size_t)size.v[1];
	desc.image_array_size = len;
	return (clCreateImage(opencl->gpu_context, CL_MEM_READ_WRITE |
				CL_MEM_HOST_WRITE_ONLY, &format, &desc, NULL, err));
}

static void		init_thumbnails_view(t_view *view, t_ivec size, t_ocl *ocl)
{
	int		err;

	view->size = size;
	view->scene_buf = cl_img2d(ocl, CL_MEM_READ_WRITE, size, &err);
	if (err != CL_SUCCESS)
		ft_error("Error while creating thumbnail buffer\n");
	view->set.progressive = 0;
	view->set.max_rays = 4;
	view->set.fov = 90;
}

static void		sample_thumbnail(t_view *view, t_kernel *kernel,
										t_climg dest, int id)
{
	cl_int		err;
	int			g_sz;
	cl_kernel	sampler;

	g_sz = sqrt(kernel->opencl->gpu_wg_sz / 16);
	sampler = get_helem(&kernel->cores, "thumbnail_sampler");
	clSetKernelArg(sampler, 0, sizeof(cl_mem), &dest);
	clSetKernelArg(sampler, 1, sizeof(cl_mem), &view->scene_buf);
	clSetKernelArg(sampler, 2, sizeof(int[2]), &view->size);
	clSetKernelArg(sampler, 3, sizeof(int), &id);
	if ((err = clEnqueueNDRangeKernel(kernel->opencl->gpu_command_queue,
			sampler, 2, NULL, (size_t[2]){(view->size.v[0] / g_sz + 1) * g_sz,
			(view->size.v[1] / g_sz + 1) * g_sz},
			(size_t[2]){g_sz, g_sz}, 0, NULL, NULL)) != CL_SUCCESS)
		ft_error("[Erreur] Echec d'execution du sampler"
				"%<R> (Error code: %<i>%2d)%<0>\n", err);
}

void			gen_thumbnails(t_ptr *p)
{
	int		err;
	int		i;
	t_ivec	size;
	t_view	view;

	glFinish();
	size = ivec(p->win->img.size.v[0] / 4, p->win->img.size.v[1] / 4);
	p->gui.thumbnails =
		create_image_array(p->opencl, p->scenes.elem_count, size, &err);
	if (err != CL_SUCCESS)
		ft_error("Error while creating image array\n");
	init_thumbnails_view(&view, size, p->opencl);
	i = -1;
	while ((t_uint)(++i) < p->scenes.elem_count)
	{
		copy_scene_data(&view, ((t_scene**)p->scenes.elements)[i]);
		generate_cam_matrices(&view);
		generate_obj_matrices(&view);
		create_scene_memobjs(&view, p->opencl->gpu_context);
		process_scene_opencl(&view, p->kernel);
		sample_thumbnail(&view, p->kernel, p->gui.thumbnails, i);
		clFinish(p->opencl->gpu_command_queue);
		cleanup_view(&view);
	}
}
