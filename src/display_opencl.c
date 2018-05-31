/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_opencl.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/29 00:39:23 by njaber            #+#    #+#             */
/*   Updated: 2018/05/31 05:25:09 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void			set_args(t_ptr *p, float res)
{
	clSetKernelArg(p->kernel->cores[0], 1,
			sizeof(cl_mem), &p->kernel->memobjs[1]);
	clSetKernelArg(p->kernel->cores[0], 2,
			sizeof(cl_mem), &p->kernel->memobjs[2]);
	clSetKernelArg(p->kernel->cores[0], 3,
			sizeof(cl_mem), &p->kernel->memobjs[3]);
	clSetKernelArg(p->kernel->cores[0], 4,
			sizeof(cl_mem), &p->kernel->memobjs[4]);
	clSetKernelArg(p->kernel->cores[0], 5,
			sizeof(cl_mem), &p->kernel->memobjs[5]);
	clSetKernelArg(p->kernel->cores[0], 6,
			sizeof(cl_mem), &p->kernel->memobjs[6]);
	clSetKernelArg(p->kernel->cores[0], 7,
			sizeof(cl_mem), &p->kernel->memobjs[7]);
	clSetKernelArg(p->kernel->cores[0], 8, sizeof(int[2]),
			(int[2]){p->win->size.x * res, p->win->size.y * res});
	clSetKernelArg(p->kernel->cores[0], 9, sizeof(float[16]), &p->cam_mat);
	clSetKernelArg(p->kernel->cores[0], 10, sizeof(float[16]), &p->cam_mat_rot);
	clSetKernelArg(p->kernel->cores[0], 11, sizeof(int), &p->nobjs);
	clSetKernelArg(p->kernel->cores[0], 12, sizeof(int), &p->nspots);
	clSetKernelArg(p->kernel->cores[0], 13, sizeof(float), &p->far);
	clSetKernelArg(p->kernel->cores[0], 14, sizeof(float), &p->fov);
}

static void			sample_buffer(t_ptr *p, cl_event event, int g_sz, float res)
{
	cl_event	event2;
	cl_int		err;

	clSetKernelArg(p->kernel->cores[2], 3, sizeof(int[2]),
			(int[2]){p->win->size.x * res, p->win->size.y * res});
	if ((err = clEnqueueNDRangeKernel(p->opencl->gpu_command_queue,
			p->kernel->cores[2], 2, NULL, (size_t[2]){(p->win->size.x / g_sz
				+ 1) * g_sz, (p->win->size.y / g_sz + 1) * g_sz},
			(size_t[2]){g_sz, g_sz}, 1, &event, &event2)) != CL_SUCCESS)
		ft_error("[Erreur] Echec d'execution du sampler"
				"%<R>  (Error code: %<i>%2d)%<0>\n", err);
	if ((err = clEnqueueReadBuffer(p->opencl->gpu_command_queue,
			p->kernel->memobjs[0], CL_TRUE, 0,
			p->win->img.line * p->win->img.size.y,
			p->win->img.buf, 1, &event2, NULL)) != CL_SUCCESS)
		ft_error("[Erreur] Echec durant la lecture du buffer"
				"%<R>  (Error code: %<i>%2d)%<0>\n", err);
}

void				process_image_opencl(t_ptr *p)
{
	cl_event	event;
	cl_int		err;
	int			g_sz;
	float		res;

	g_sz = sqrt(p->opencl->gpu_wg_sz);
	res = pow(2, fmin(p->res, 2));
	if (p->kernel == NULL)
		process_image(p);
		//exit(0);
	else
	{
		set_args(p, res);
		if ((err = clEnqueueNDRangeKernel(p->opencl->gpu_command_queue,
				p->kernel->cores[0], 2, NULL, (size_t[2]){
				(int)floor(p->win->size.x * res / g_sz + 1) * g_sz,
				(int)floor(p->win->size.y * res / g_sz + 1) * g_sz},
				(size_t[2]){g_sz, g_sz}, 0, NULL, &event)) != CL_SUCCESS)
			ft_error("[Erreur] Echec d'execution du kernel"
					"%<R>  (Error code: %<i>%2d)%<0>\n", err);
		sample_buffer(p, event, g_sz, res);
	}
}
