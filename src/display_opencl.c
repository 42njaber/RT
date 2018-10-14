/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_opencl.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/29 00:39:23 by njaber            #+#    #+#             */
/*   Updated: 2018/10/14 05:54:08 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void			set_arg_iter_pos(t_ptr *p)
{
	t_ivec	iter_pos;

	iter_pos = ivec(0, 0);
	if (p->res & (1 << 0))
		iter_pos.v[0] += 8;
	if (p->res & (1 << 1))
		iter_pos.v[1] += 8;
	if (p->res & (1 << 2))
		iter_pos.v[0] += 4;
	if (p->res & (1 << 3))
		iter_pos.v[1] += 4;
	if (p->res & (1 << 4))
		iter_pos.v[0] += 2;
	if (p->res & (1 << 5))
		iter_pos.v[1] += 2;
	if (p->res & (1 << 6))
		iter_pos.v[0] += 1;
	if (p->res & (1 << 7))
		iter_pos.v[1] += 1;
	clSetKernelArg(p->kernel->cores[0], 12, sizeof(int[2]), iter_pos.v);
}

static void			set_args(t_ptr *p)
{
	cl_kernel	prog;

	prog = p->kernel->cores[0];
	clSetKernelArg(prog, 1, sizeof(cl_mem), &p->kernel->memobjs[1]);
	clSetKernelArg(prog, 2, sizeof(cl_mem), &p->kernel->memobjs[2]);
	clSetKernelArg(prog, 4, sizeof(float[16]), &p->cam_mat);
	clSetKernelArg(prog, 5, sizeof(float[16]), &p->cam_mat_rot);
	clSetKernelArg(prog, 6, sizeof(int), &p->nobjs);
	clSetKernelArg(prog, 7, sizeof(int), &p->nspots);
	clSetKernelArg(prog, 8, sizeof(int), (int[1]){p->max_reflections});
	clSetKernelArg(prog, 9, sizeof(float), &p->fov);
	clSetKernelArg(prog, 10, sizeof(float), &p->ambiant_light);
	clSetKernelArg(prog, 11, sizeof(int), &p->res);
	clSetKernelArg(p->kernel->cores[2], 3, sizeof(int), &p->res);
	set_arg_iter_pos(p);
}

void				process_image_opencl(t_ptr *p)
{
	cl_int		err;
	cl_event	event;
	int			g_sz;

	g_sz = sqrt(p->opencl->gpu_wg_sz / 16);
	if (p->kernel == NULL)
		ft_error("Could not launch OpenCL kernel, stopping program...\n");
	set_args(p);
	if ((err = clEnqueueNDRangeKernel(p->opencl->gpu_command_queue,
			p->kernel->cores[0], 2, NULL, (size_t[2]){
			(int)ceil((int)(p->win->size.v[0] / 4.) / g_sz + 1) * g_sz,
			(int)ceil((int)(p->win->size.v[1] / 4.) / g_sz + 1) * g_sz},
			(size_t[2]){g_sz, g_sz}, 0, NULL, &event)) != CL_SUCCESS)
		ft_error("[Erreur] Echec d'execution du kernel"
				"%<R>  (Error code: %<i>%2d)%<0>\n", err);
	if ((err = clEnqueueNDRangeKernel(p->opencl->gpu_command_queue,
			p->kernel->cores[2], 2, NULL, (size_t[2]){(p->win->size.v[0] / g_sz
				+ 1) * g_sz, (p->win->size.v[1] / g_sz + 1) * g_sz},
			(size_t[2]){g_sz, g_sz}, 1, &event, NULL)) != CL_SUCCESS)
		ft_error("[Erreur] Echec d'execution du sampler"
				"%<R>  (Error code: %<i>%2d)%<0>\n", err);
	clFlush(p->opencl->gpu_command_queue);
	clReleaseEvent(event);
}
