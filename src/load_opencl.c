/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_opencl.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/16 20:23:49 by njaber            #+#    #+#             */
/*   Updated: 2018/06/18 19:15:42 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static int		set_init_args(t_ptr *p, t_ocl *opencl,
		t_kernel *kernel, t_img *img)
{
	int err;

	(void)p;
	if ((kernel->memobjs = (cl_mem*)ft_memalloc(sizeof(cl_mem) * 16)) == NULL)
		ft_error("[Erreur] Echec d'allocation mémoire.");
	kernel->memobjs[0] = clCreateBuffer(opencl->gpu_context, CL_MEM_READ_WRITE,
			img->line * img->size.y, NULL, &err);
	kernel->memobjs[15] = clCreateBuffer(opencl->gpu_context, CL_MEM_READ_WRITE,
			img->line * img->size.y * 16, NULL, &err);
	err |= clSetKernelArg(kernel->cores[0], 0,
			sizeof(cl_mem), (void*)&kernel->memobjs[15]);
	err |= clSetKernelArg(kernel->cores[1], 0,
			sizeof(cl_mem), (void*)&kernel->memobjs[0]);
	err |= clSetKernelArg(kernel->cores[2], 0,
			sizeof(cl_mem), (void*)&kernel->memobjs[0]);
	err |= clSetKernelArg(kernel->cores[2], 1,
			sizeof(cl_mem), (void*)&kernel->memobjs[15]);
	err |= clSetKernelArg(kernel->cores[2], 2, sizeof(int[2]), &img->size);
	return (err);
}

static int		make_kernels(t_kernel *kernel)
{
	int			err;

	kernel->cores[0] = clCreateKernel(kernel->program, "process_image", &err);
	if (err != CL_SUCCESS)
		return (err);
	kernel->cores[1] = clCreateKernel(kernel->program, "clear_buf", &err);
	if (err != CL_SUCCESS)
		return (err);
	kernel->cores[2] = clCreateKernel(kernel->program, "sampler", &err);
	return (err);
}

static int		build_program(t_ocl *opencl, t_kernel *kernel)
{
	char		tmp[4096];
	size_t		tmp2;
	char		*tmp3;
	int			err;

	kernel->program = create_program_from_file(opencl->gpu_context,
			"src/kernel.cl");
	if (kernel->program == NULL)
		return (-100);
	tmp3 = ft_printb("-cl-single-precision-constant -cl-opt-disable");
	err = clBuildProgram(kernel->program, 0, NULL, tmp3, NULL, NULL);
	free(tmp3);
	clGetProgramBuildInfo(kernel->program, opencl->gpus[0],
			CL_PROGRAM_BUILD_LOG, 4096, tmp, &tmp2);
	if (tmp2 < 4096)
		ft_printf("Build log :\n%.*s\n", tmp2, tmp);
	return (err);
}

t_kernel		*create_kernel(t_ptr *p)
{
	t_kernel	*kernel;
	t_img		*img;
	int			err;

	p->kernel = NULL;
	if ((kernel = (t_kernel*)ft_memalloc(sizeof(t_kernel))) == NULL)
		ft_error("[Erreur] Echec d'allocation mémoire.");
	img = &p->win->img;
	kernel->opencl = p->opencl;
	if ((err = build_program(p->opencl, kernel)) != CL_SUCCESS ||
		(err = make_kernels(kernel)) != CL_SUCCESS ||
		(err = set_init_args(p, p->opencl, kernel, img)) != CL_SUCCESS)
	{
		ft_printf("[Error] Could not build kernel program"
				"%<R>  (Error code: %<i>%2d)%<0>\n", err);
		if (kernel->program != NULL)
			clReleaseProgram(kernel->program);
		free(kernel);
		return (NULL);
	}
	p->kernel = kernel;
	return (kernel);
}
