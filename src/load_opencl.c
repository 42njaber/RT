/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_opencl.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/16 20:23:49 by njaber            #+#    #+#             */
/*   Updated: 2018/10/16 14:38:10 by njaber           ###   ########.fr       */
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

static int		set_init_args(t_ptr *p, t_ocl *opencl,
											t_kernel *kernel, t_img *img)
{
	int err;

	(void)p;
	kernel->memobjs[0] = clCreateBuffer(opencl->gpu_context, CL_MEM_WRITE_ONLY,
			img->line * img->size.v[1], NULL, &err);
	err |= clSetKernelArg(get_helem(&kernel->cores, "clear_buf"), 0,
			sizeof(cl_mem), (void*)&kernel->memobjs[0]);
	return (err);
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

#define		FILE_NUMBER 5

static int		compile_files(t_ocl *opencl, cl_program *objects)
{
	const char		*files[FILE_NUMBER] = {"kernel.cl", "math.cl",
		"hit_equations.cl", "solvers.cl", "debug.cl"};
	cl_int			err;
	int				i;

	i = -1;
	err = 0;
	while (++i < FILE_NUMBER)
	{
		ft_printf("Compiling file %<G,i>%s%<0>...\n", files[i]);
		objects[i] = create_program_from_file(opencl->gpu_context, files[i]);
		if (objects[i] == NULL)
			err = -100;
		else
		{
			err = clCompileProgram(objects[i], opencl->gpu_nbr, opencl->gpus,
				NULL , 0, NULL, NULL, NULL, NULL);
		}
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
	ft_printf("%<i>OpenCL leaks here...%<0>\n");
	i = -1;
	while (++i < FILE_NUMBER)
	{
		clReleaseProgram(objects[i]);
		ft_printf("%<i>and here...%<0>\n");
	}
	if (err != CL_SUCCESS && kernel->program != NULL)
		clReleaseProgram(kernel->program);
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
