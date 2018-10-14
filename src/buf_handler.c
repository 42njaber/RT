/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buf_handler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/29 03:42:23 by njaber            #+#    #+#             */
/*   Updated: 2018/10/14 11:34:34 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void			create_obj_memobjs(t_ptr *p)
{
	if (p->kernel->memobjs[1] != 0)
		clReleaseMemObject(p->kernel->memobjs[1]);
	p->kernel->memobjs[1] = clCreateBuffer(p->opencl->gpu_context,
			CL_MEM_COPY_HOST_PTR, sizeof(t_obj) * p->nobjs, p->objs, NULL);
}

void			create_spot_memobjs(t_ptr *p)
{
	if (p->kernel->memobjs[2] != 0)
		clReleaseMemObject(p->kernel->memobjs[2]);
	p->kernel->memobjs[2] = clCreateBuffer(p->opencl->gpu_context,
			CL_MEM_COPY_HOST_PTR, sizeof(t_spot) * p->nspots, p->spots, NULL);
}
