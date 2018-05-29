/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buf_handler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/29 03:42:23 by njaber            #+#    #+#             */
/*   Updated: 2018/05/29 21:31:06 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void		**create_obj_data(t_ptr *p)
{
	void	**tmp;
	int		i;

	if ((tmp = ft_memalloc(sizeof(void*) * 5)) == NULL ||
		(tmp[0] = ft_memalloc(sizeof(int) * p->nobjs)) == NULL ||
		(tmp[1] = ft_memalloc(sizeof(int) * p->nobjs)) == NULL ||
		(tmp[2] = ft_memalloc(sizeof(t_mat4) * p->nobjs)) == NULL ||
		(tmp[3] = ft_memalloc(sizeof(t_mat4) * p->nobjs)) == NULL ||
		(tmp[4] = ft_memalloc(sizeof(t_mat4) * p->nobjs)) == NULL)
		ft_error("[Erreur] Failed to allocate memory\n");
	i = -1;
	while (++i < p->nobjs)
	{
		((int*)tmp[0])[i] = p->objs[i].type;
		((unsigned int*)tmp[1])[i] = p->objs[i].color;
		ft_memcpy(((t_mat4*)tmp[2])[i], p->objs[i].transform, sizeof(t_mat4));
		ft_memcpy(((t_mat4*)tmp[3])[i], p->objs[i].rot_mat, sizeof(t_mat4));
		ft_memcpy(((t_mat4*)tmp[4])[i], p->objs[i].rev_rot, sizeof(t_mat4));
	}
	return (tmp);
}

void			create_obj_memobjs(t_ptr *p)
{
	void	**tmp;
	int		i;

	tmp = create_obj_data(p);
	i = 0;
	while (++i < 6)
		if (p->kernel->memobjs[i] != 0)
			clReleaseMemObject(p->kernel->memobjs[i]);
	p->kernel->memobjs[1] = clCreateBuffer(p->opencl->gpu_context,
			CL_MEM_COPY_HOST_PTR, sizeof(int) * p->nobjs, tmp[0], NULL);
	free(tmp[0]);
	p->kernel->memobjs[2] = clCreateBuffer(p->opencl->gpu_context,
			CL_MEM_COPY_HOST_PTR, sizeof(int) * p->nobjs, tmp[1], NULL);
	free(tmp[1]);
	p->kernel->memobjs[3] = clCreateBuffer(p->opencl->gpu_context,
			CL_MEM_COPY_HOST_PTR, sizeof(t_mat4) * p->nobjs, tmp[2], NULL);
	free(tmp[2]);
	p->kernel->memobjs[4] = clCreateBuffer(p->opencl->gpu_context,
			CL_MEM_COPY_HOST_PTR, sizeof(t_mat4) * p->nobjs, tmp[3], NULL);
	free(tmp[3]);
	p->kernel->memobjs[5] = clCreateBuffer(p->opencl->gpu_context,
			CL_MEM_COPY_HOST_PTR, sizeof(t_mat4) * p->nobjs, tmp[4], NULL);
	free(tmp[4]);
	free(tmp);
}

void			create_spot_memobjs(t_ptr *p)
{
	void	**tmp;
	int		i;

	if ((tmp = ft_memalloc(sizeof(void*) * 5)) == NULL ||
		(tmp[0] = ft_memalloc(sizeof(float) * p->nspots)) == NULL ||
		(tmp[1] = ft_memalloc(sizeof(float[4]) * p->nspots)) == NULL)
		ft_error("[Erreur] Failed to allocate memory\n");
	i = -1;
	while (++i < p->nspots)
	{
		((float*)tmp[0])[i] = p->spots[i].lum;
		ft_memcpy((void*)tmp[1] + sizeof(float[4]) * i, &p->spots[i].pos,
				sizeof(float[3]));
	}
	if (p->kernel->memobjs[6] != 0)
		clReleaseMemObject(p->kernel->memobjs[6]);
	p->kernel->memobjs[6] = clCreateBuffer(p->opencl->gpu_context,
			CL_MEM_COPY_HOST_PTR, sizeof(float) * p->nspots, tmp[0], NULL);
	free(tmp[0]);
	if (p->kernel->memobjs[7] != 0)
		clReleaseMemObject(p->kernel->memobjs[7]);
	p->kernel->memobjs[7] = clCreateBuffer(p->opencl->gpu_context,
			CL_MEM_COPY_HOST_PTR, sizeof(float[4]) * p->nspots, tmp[1], NULL);
	free(tmp[1]);
	free(tmp);
}
