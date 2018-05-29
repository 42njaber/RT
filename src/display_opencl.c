/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_opencl.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/29 00:39:23 by njaber            #+#    #+#             */
/*   Updated: 2018/05/29 20:25:49 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void				set_args(t_ptr *p)
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
	clSetKernelArg(p->kernel->cores[0], 11, sizeof(int), &p->nobjs);
	clSetKernelArg(p->kernel->cores[0], 12, sizeof(int), &p->nspots);
	clSetKernelArg(p->kernel->cores[0], 13, sizeof(float), &p->near);
	clSetKernelArg(p->kernel->cores[0], 14, sizeof(float), &p->far);
	clSetKernelArg(p->kernel->cores[0], 15, sizeof(float), &p->fov);
}

void				process_image_opencl(t_ptr *p)
{
	cl_event	event;
	cl_int		err;

	if (p->kernel == NULL)
		process_image(p);
		//exit(0);
	else
	{
		set_args(p);
		if ((err = clEnqueueNDRangeKernel(p->opencl->gpu_command_queue,
				p->kernel->cores[0], 1, NULL, (size_t[1]){
				(((p->scene->size.x * p->scene->size.y)
				/ p->opencl->gpu_wg_sz) + 1) * p->opencl->gpu_wg_sz},
				(size_t[1]){p->opencl->gpu_wg_sz}, 0, NULL, &event))
				!= CL_SUCCESS)
			ft_error("[Erreur] Echec d'execution du kernel"
					"%<R>  (Error code: %<i>%2d)%<0>\n", err);
		if ((err = clEnqueueReadBuffer(p->opencl->gpu_command_queue,
				p->kernel->memobjs[0], CL_TRUE, 0,
				p->scene->line * p->scene->size.y,
				p->scene->buf, 1, &event, NULL)) != CL_SUCCESS)
			ft_error("[Erreur] Echec durant la lecture du buffer"
					"%<R>  (Error code: %<i>%2d)%<0>\n", err);
		clFinish(p->opencl->gpu_command_queue);
	}
}
