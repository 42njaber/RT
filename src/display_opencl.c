/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_opencl.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/29 00:39:23 by njaber            #+#    #+#             */
/*   Updated: 2018/06/05 23:06:02 by njaber           ###   ########.fr       */
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
	clSetKernelArg(p->kernel->cores[0], 8,
			sizeof(cl_mem), &p->kernel->memobjs[8]);
	clSetKernelArg(p->kernel->cores[0], 9, sizeof(int[2]),
			(int[2]){p->win->size.x * res, p->win->size.y * res});
	clSetKernelArg(p->kernel->cores[0], 10, sizeof(float[16]), &p->cam_mat);
	clSetKernelArg(p->kernel->cores[0], 11, sizeof(float[16]), &p->cam_mat_rot);
	clSetKernelArg(p->kernel->cores[0], 12, sizeof(int), &p->nobjs);
	clSetKernelArg(p->kernel->cores[0], 13, sizeof(int), &p->nspots);
	clSetKernelArg(p->kernel->cores[0], 14, sizeof(int), (int[1]){4});
	clSetKernelArg(p->kernel->cores[0], 15, sizeof(float), &p->far);
	clSetKernelArg(p->kernel->cores[0], 16, sizeof(float), &p->fov);
}

static void			sampler_callback(cl_event event,
		cl_int complete, void *parm)
{
	t_ptr	*p;

	(void)complete;
	p = (t_ptr*)parm;
	p->tmp = 0;
	clReleaseEvent(event);
}

static void			sample_buffer(t_ptr *p)
{
	cl_int		err;
	cl_event	event;
	int			g_sz;
	float		res;

	g_sz = sqrt(p->opencl->gpu_wg_sz);
	res = pow(2, fmin(p->res, 2));
	clSetKernelArg(p->kernel->cores[2], 3, sizeof(int[2]),
			(int[2]){p->win->size.x * res, p->win->size.y * res});
	if ((err = clEnqueueNDRangeKernel(p->opencl->gpu_command_queue,
			p->kernel->cores[2], 2, NULL, (size_t[2]){(p->win->size.x / g_sz
				+ 1) * g_sz, (p->win->size.y / g_sz + 1) * g_sz},
			(size_t[2]){g_sz, g_sz}, 0, NULL, &event)) != CL_SUCCESS)
		ft_error("[Erreur] Echec d'execution du sampler"
				"%<R>  (Error code: %<i>%2d)%<0>\n", err);
	clSetEventCallback(event, CL_COMPLETE, &sampler_callback, (void*)p);
	clFlush(p->opencl->gpu_command_queue);
}

static void			image_callback(cl_event event, cl_int complete, void *parm)
{
	t_ptr	*p;

	(void)complete;
	p = (t_ptr*)parm;
	p->tmp = 1;
	clReleaseEvent(event);
	sample_buffer(p);
}

void				process_image_opencl(t_ptr *p)
{
	cl_int		err;
	cl_event	event;
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
		p->tmp = 2;
		if ((err = clEnqueueNDRangeKernel(p->opencl->gpu_command_queue,
				p->kernel->cores[0], 2, NULL, (size_t[2]){
				(int)floor(p->win->size.x * res / g_sz + 1) * g_sz,
				(int)floor(p->win->size.y * res / g_sz + 1) * g_sz},
				(size_t[2]){g_sz, g_sz}, 0, NULL, &event)) != CL_SUCCESS)
			ft_error("[Erreur] Echec d'execution du kernel"
					"%<R>  (Error code: %<i>%2d)%<0>\n", err);
		clSetEventCallback(event, CL_COMPLETE, &image_callback,(void*)p);
		clFlush(p->opencl->gpu_command_queue);
	}
}
