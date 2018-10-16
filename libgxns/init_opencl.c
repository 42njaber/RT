/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_opencl.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/15 01:57:02 by njaber            #+#    #+#             */
/*   Updated: 2018/10/16 09:13:56 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libgxns.h"

char			*join_free(char *s1, char *s2, size_t src_len)
{
	size_t	dst_len;
	char	*ret;

	dst_len = ft_strlen(s1);
	ret = ft_strnew(dst_len + src_len);
	ft_memcpy(ret, s1, dst_len);
	ft_memcpy(ret + dst_len, s2, src_len);
	free(s1);
	return (ret);
}

cl_program		create_program_from_file(cl_context context, const char *file)
{
	cl_program	ret;
	char		*kernel;
	char		buf[4096];
	int			fd;
	int			rd;

	if ((fd = open(file, O_RDONLY)) < 0)
		return (NULL);
	rd = read(fd, buf, 4096);
	kernel = ft_strnew(rd);
	ft_memcpy(kernel, buf, rd);
	while ((rd = read(fd, buf, 4096)) > 0)
		kernel = join_free(kernel, buf, rd);
	if (rd == -1)
		return (NULL);
	ret = clCreateProgramWithSource(context, 1,
			(const char**)&kernel, NULL, NULL);
	free(kernel);
	return (ret);
}

void			get_configs(t_ocl *opencl)
{
	size_t	tmp[1];
	char	tmp2[128];
	size_t	tmp3;

	clGetContextInfo(opencl->gpu_context, CL_CONTEXT_NUM_DEVICES,
			sizeof(unsigned int), &opencl->gpu_nbr, NULL);
	ft_printf("Number of Gpus: %u\n", opencl->gpu_nbr);
	clGetContextInfo(opencl->gpu_context, CL_CONTEXT_DEVICES,
			sizeof(cl_device_id[16]), opencl->gpus, NULL);
	clGetDeviceInfo(opencl->gpus[0], CL_DEVICE_MAX_WORK_GROUP_SIZE,
			sizeof(tmp), tmp, NULL);
	opencl->gpu_wg_sz = pow(2, trunc(log2(*tmp)));
	ft_printf("Max work-group size: %d\n", opencl->gpu_wg_sz);
	clGetDeviceInfo(opencl->gpus[0], CL_DEVICE_MAX_COMPUTE_UNITS,
			sizeof(tmp), tmp, NULL);
	ft_printf("Max compute units: %d\n", *tmp);
	clGetDeviceInfo(opencl->gpus[0], CL_DEVICE_VERSION,
			sizeof(tmp2), tmp2, &tmp3);
	ft_printf("Version: %.*s\n", tmp3, tmp2);
	*tmp = 0;
	clGetDeviceInfo(opencl->gpus[0], CL_DEVICE_DOUBLE_FP_CONFIG,
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
	opencl->gpu_context = clCreateContextFromType(NULL, CL_DEVICE_TYPE_GPU,
			NULL, NULL, NULL);
	get_configs(opencl);
	opencl->gpu_command_queue = clCreateCommandQueue(opencl->gpu_context,
			opencl->gpus[0], 0, &err);
	if (err != CL_SUCCESS)
		ft_error("Could not create command queue\n", err);
	return (opencl);
}
