/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   images.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/19 10:10:08 by njaber            #+#    #+#             */
/*   Updated: 2018/10/21 12:14:49 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libgxns.h"

int		init_new_image(t_img *img, t_ivec size, t_ocl *opencl, void *data)
{
	int err;

	img->size = size;
	glGenTextures(1, &img->id);
	glBindTexture(GL_TEXTURE_2D, img->id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, size.v[0], size.v[1],
			0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
	img->cl_obj = clCreateFromGLTexture(opencl->gpu_context,
			CL_MEM_READ_WRITE, GL_TEXTURE_2D, 0, img->id, &err);
	return (err);
}
