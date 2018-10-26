/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_opencl.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/16 20:23:49 by njaber            #+#    #+#             */
/*   Updated: 2018/10/26 22:36:58 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

cl_mem			cl_img2d(t_ocl *opencl, cl_mem_flags flags,
														t_ivec size, int *err)
{
	cl_image_format		format;
	cl_image_desc		desc;

	ft_bzero(&format, sizeof(format));
	ft_bzero(&desc, sizeof(desc));
	format.image_channel_order = CL_RGBA;
	format.image_channel_data_type = CL_UNSIGNED_INT8;
	desc.image_type = CL_MEM_OBJECT_IMAGE2D;
	desc.image_width = (size_t)size.v[0];
	desc.image_height = (size_t)size.v[1];
	return (clCreateImage(opencl->gpu_context, flags, &format,
												&desc, NULL, err));
}

static int		build_program(t_ocl *opencl, t_kernel *kernel)
{
	cl_int		err;

	chdir("opencl/");
	kernel->program = create_program_from_files(opencl->gpu_context, 4,
			(const char*[4]){"math.cl", "solvers.cl",
			"hit_equations.cl", "kernel.cl"});
	if (kernel->program == NULL)
		err = EXIT_FAILURE;
	else
	{
		ft_printf("%<#FFAA00>Compiling opencl program...%<0>\n");
		err = clBuildProgram(kernel->program, 1, &opencl->gpu, "", NULL, NULL);
	}
	chdir("../");
	if (err != CL_SUCCESS)
		return (err);
	return (EXIT_SUCCESS);
}

static int		make_kernels(t_kernel *kernel)
{
	int			err;

	init_hmap(&kernel->cores);
	add_helem(&kernel->cores, "process_image",
			clCreateKernel(kernel->program, "process_image", &err));
	if (err != CL_SUCCESS)
		return (err);
	add_helem(&kernel->cores, "clear_buf",
			clCreateKernel(kernel->program, "clear_buf", &err));
	if (err != CL_SUCCESS)
		return (err);
	add_helem(&kernel->cores, "sampler",
			clCreateKernel(kernel->program, "sampler256", &err));
	if (err != CL_SUCCESS)
		return (err);
	add_helem(&kernel->cores, "thumbnail_sampler",
			clCreateKernel(kernel->program, "sampler1", &err));
	if (err != CL_SUCCESS)
		return (err);
	add_helem(&kernel->cores, "gui_painter",
			clCreateKernel(kernel->program, "paint_gui", &err));
	return (err);
}

t_kernel		*create_kernel(t_ptr *p)
{
	t_kernel	*kernel;
	t_img		*img;
	int			err;

	if (p->kernel != NULL)
		ft_error("Trying to override kernel\n");
	if ((kernel = (t_kernel*)ft_memalloc(sizeof(t_kernel))) == NULL)
		ft_error("[Erreur] Echec d'allocation mémoire.");
	img = &p->win->img;
	kernel->opencl = p->opencl;
	if ((err = build_program(p->opencl, kernel)) != EXIT_SUCCESS ||
		(err = make_kernels(kernel)) != CL_SUCCESS)
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
