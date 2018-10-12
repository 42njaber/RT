/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_opencl.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/16 20:23:49 by njaber            #+#    #+#             */
/*   Updated: 2018/10/12 01:49:36 by njaber           ###   ########.fr       */
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
			img->line * img->size.v[1], NULL, &err);
	kernel->memobjs[15] = clCreateBuffer(opencl->gpu_context, CL_MEM_READ_WRITE,
			img->line * img->size.v[1] * 16, NULL, &err);
	err |= clSetKernelArg(kernel->cores[0], 0,
			sizeof(cl_mem), (void*)&kernel->memobjs[15]);
	err |= clSetKernelArg(kernel->cores[0], 9, sizeof(int[2]),
			(int[2]){img->size.v[0] * 4, img->size.v[1] * 4});
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

#define		FILE_NUMBER 5

static int		compile_files(t_ocl *opencl, cl_program *objects)
{
	const char		*files[FILE_NUMBER] = {"kernel.cl", "math.cl",
		"hit_equations.cl", "solvers.cl", "debug.cl"};
	cl_int			err;
	int				i;

	i = -1;
	while (++i < FILE_NUMBER)
	{
		ft_printf("Compiling file %<G,i>%s%<0>...\n", files[i]);
		objects[i] = create_program_from_file(opencl->gpu_context, files[i]);
		if (objects[i] == NULL)
			err = -100;
		else
			err = clCompileProgram(objects[i], opencl->gpu_nbr, opencl->gpus,
				"-cl-single-precision-constant -cl-opt-disable"
				, 0, NULL, NULL, NULL, NULL);
		if (err != CL_SUCCESS)
		{
			i++;
			while (--i >= 0)
				clReleaseProgram(objects[i]);
			return (err);
		}
	}
	return (err);
}

static int		build_program(t_ocl *opencl, t_kernel *kernel)
{
	cl_program	objects[FILE_NUMBER];
	cl_int		err;
	int			i;

	chdir("opencl/");
	err = compile_files(opencl, objects);
	if (err != CL_SUCCESS)
		return (err);
	ft_putendl("Linking program...");
	kernel->program = clLinkProgram(opencl->gpu_context, opencl->gpu_nbr,
			opencl->gpus, "", FILE_NUMBER, objects, NULL, NULL, &err);
	i = -1;
	while (++i < FILE_NUMBER)
		clReleaseProgram(objects[i]);
	if (err != CL_SUCCESS && kernel->program != NULL)
		clReleaseProgram(kernel->program);
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
