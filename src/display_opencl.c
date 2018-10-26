/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_opencl.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/29 00:39:23 by njaber            #+#    #+#             */
/*   Updated: 2018/10/26 09:02:05 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void			set_arg_iter_pos(t_view *view)
{
	t_ivec	iter_pos;

	iter_pos = ivec(0, 0);
	if (view->set.iter & (1 << 0))
		iter_pos.v[0] += 8;
	if (view->set.iter & (1 << 1))
		iter_pos.v[1] += 8;
	if (view->set.iter & (1 << 2))
		iter_pos.v[0] += 4;
	if (view->set.iter & (1 << 3))
		iter_pos.v[1] += 4;
	if (view->set.iter & (1 << 4))
		iter_pos.v[0] += 2;
	if (view->set.iter & (1 << 5))
		iter_pos.v[1] += 2;
	if (view->set.iter & (1 << 6))
		iter_pos.v[0] += 1;
	if (view->set.iter & (1 << 7))
		iter_pos.v[1] += 1;
	view->set.iter_pos = iter_pos;
}

static void			set_args(t_view *view, t_kernel *kernel)
{
	cl_kernel	process;

	process = get_helem(&kernel->cores, "process_image");
	set_arg_iter_pos(view);
	clSetKernelArg(process, 0, sizeof(cl_mem), &view->scene_buf);
	clSetKernelArg(process, 1, sizeof(cl_mem), &view->objbuf);
	clSetKernelArg(process, 2, sizeof(cl_mem), &view->spotbuf);
	clSetKernelArg(process, 3, sizeof(int), &view->nobjs);
	clSetKernelArg(process, 4, sizeof(int), &view->nspots);
	clSetKernelArg(process, 5, sizeof(t_set), &view->set);
	clSetKernelArg(process, 6, sizeof(cl_mem), &get_p()->texture_mem.cl_obj);
	clSetKernelArg(process, 7, sizeof(cl_mem), &get_p()->cl_texture_sizes);
}

static void			sample_image(t_view *view, t_kernel *kernel,
										t_climg dest)
{
	cl_int		err;
	int			g_sz;
	cl_kernel	sampler;

	g_sz = sqrt(WG_SIZE);
	sampler = get_helem(&kernel->cores, "sampler");
	clSetKernelArg(sampler, 0, sizeof(cl_mem), &dest);
	clSetKernelArg(sampler, 1, sizeof(cl_mem), &view->scene_buf);
	clSetKernelArg(sampler, 2, sizeof(int[2]), &view->size);
	clSetKernelArg(sampler, 3, sizeof(int), &view->set.iter);
	if ((err = clEnqueueNDRangeKernel(kernel->opencl->gpu_command_queue,
			sampler, 2, NULL,
			(size_t[2]){(view->size.v[0] / g_sz + 1) * g_sz,
			(view->size.v[1] / g_sz + 1) * g_sz},
			(size_t[2]){g_sz, g_sz}, 0, NULL, NULL)) != CL_SUCCESS)
		ft_error("[Erreur] Echec d'execution du sampler"
				"%<R> (Error code: %<i>%2d)%<0>\n", err);
}

void				process_scene_opencl(t_view *view, t_kernel *kernel)
{
	cl_int		err;
	int			g_sz;
	size_t		sdown;

	g_sz = sqrt(WG_SIZE);
	sdown = view->set.progressive ? 4 : 1;
	set_args(view, kernel);
	if ((err = clEnqueueNDRangeKernel(kernel->opencl->gpu_command_queue,
			get_helem(&kernel->cores, "process_image"), 2, NULL, (size_t[2]){
			(int)ceil((int)(view->size.v[0] / sdown) / g_sz + 1) * g_sz,
			(int)ceil((int)(view->size.v[1] / sdown) / g_sz + 1) * g_sz},
			(size_t[2]){g_sz, g_sz}, 0, NULL, NULL)) != CL_SUCCESS)
		ft_error("[Erreur] Echec d'execution du kernel"
				"%<R> (Error code: %<i>%2d)%<0>\n", err);
}

void				update_scene(t_ptr *p)
{
	clFinish(p->opencl->gpu_command_queue);
	if (p->update)
	{
		generate_cam_matrices(&p->view);
		p->view.set.iter = 0;
		p->update = 0;
		process_scene_opencl(&p->view, p->kernel);
		sample_image(&p->view, p->kernel, p->scene);
	}
	else if (p->view.set.iter < 255)
	{
		p->view.set.iter++;
		process_scene_opencl(&p->view, p->kernel);
		sample_image(&p->view, p->kernel, p->scene);
	}
}
