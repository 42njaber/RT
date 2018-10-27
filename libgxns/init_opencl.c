/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_opencl.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/15 01:57:02 by njaber            #+#    #+#             */
/*   Updated: 2018/10/27 02:42:29 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libgxns.h"

static void		create_context(t_ocl *opencl)
{
	cl_context_properties	props[3];
	int						err;

	if ((opencl->gl_context = CGLGetCurrentContext()) == 0)
		ft_error("No current OpenGL context\n");
	props[0] = CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE;
	props[1] = (cl_context_properties)CGLGetShareGroup(opencl->gl_context);
	props[2] = 0;
	opencl->gpu_context = clCreateContext(props, 0, 0, NULL, 0, &err);
	if (err != CL_SUCCESS)
		ft_error("Could not launch shared context between CL/GL\n");
	err = clGetGLContextInfoAPPLE(opencl->gpu_context, opencl->gl_context,
			CL_CGL_DEVICE_FOR_CURRENT_VIRTUAL_SCREEN_APPLE,
				sizeof(cl_device_id), &opencl->gpu, NULL);
	if (err != CL_SUCCESS)
		ft_error("Could get shared device\n");
}

static int		get_shared_context(t_ocl *opencl)
{
	char	*ext;
	size_t	ext_len;
	int		ret;

	create_context(opencl);
	ret = clGetDeviceInfo(opencl->gpu, CL_DEVICE_EXTENSIONS, 0, NULL, &ext_len);
	if (ret != CL_SUCCESS)
		ft_error("Could not get device infos\n");
	if ((ext = (char*)ft_memalloc(ext_len)) == NULL)
		ft_error("Malloc error\n");
	clGetDeviceInfo(opencl->gpu, CL_DEVICE_EXTENSIONS, ext_len, ext, NULL);
	ft_printf("Extensions: %.*s\n", ext_len, ext);
	ret = 0;
	if (ft_strstr(ext, "cl_khr_gl_sharing") != NULL)
		ret = 1;
	if (ft_strstr(ext, "cl_APPLE_gl_sharing") != NULL)
		ret = 1;
	free(ext);
	return (ret);
}

void			get_configs(t_ocl *opencl)
{
	size_t	tmp[1];
	char	tmp2[128];
	size_t	tmp3;

	clGetDeviceInfo(opencl->gpu, CL_DEVICE_MAX_WORK_GROUP_SIZE,
			sizeof(tmp), tmp, NULL);
	opencl->gpu_wg_sz = *tmp;
	ft_printf("Max work-group size: %d\n", opencl->gpu_wg_sz);
	clGetDeviceInfo(opencl->gpu, CL_DEVICE_MAX_COMPUTE_UNITS,
			sizeof(tmp), tmp, NULL);
	ft_printf("Max compute units: %d\n", *tmp);
	clGetDeviceInfo(opencl->gpu, CL_DEVICE_VERSION,
			sizeof(tmp2), tmp2, &tmp3);
	ft_printf("Version: %.*s\n", tmp3, tmp2);
	*tmp = 0;
	clGetDeviceInfo(opencl->gpu, CL_DEVICE_DOUBLE_FP_CONFIG,
			sizeof(tmp), tmp, &tmp3);
	ft_printf("Double support: %lu\n", *tmp);
	opencl->gpu_double = *tmp;
}

t_ocl			*init_opencl(void)
{
	t_ocl			*opencl;
	int				err;

	if ((opencl = (t_ocl*)ft_memalloc(sizeof(t_ocl))) == NULL)
		ft_error("[Erreur] Echec d'allocation mémoire.\n");
	get_shared_context(opencl);
	get_configs(opencl);
	opencl->gpu_command_queue = clCreateCommandQueue(opencl->gpu_context,
			opencl->gpu, 0, &err);
	if (err != CL_SUCCESS)
		ft_error("Could not create command queue\n", err);
	return (opencl);
}
