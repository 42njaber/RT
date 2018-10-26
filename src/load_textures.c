/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_textures.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <njaber@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/25 00:56:37 by njaber            #+#    #+#             */
/*   Updated: 2018/10/26 15:31:26 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void	fill_buffers(t_ptr *p, t_ivec size_max)
{
	int		i;
	int		j;
	int		err;
	t_png	*png;
	t_uint	*buf;
	size_t	img_size;

	img_size = sizeof(t_uint) * size_max.v[0] * size_max.v[1];
	if ((buf = ft_memalloc(img_size * p->textures.elem_count)) == NULL)
		ft_error("Malloc error\n");
	i = -1;
	while (++i < (int)p->textures.elem_count)
	{
		png = (t_png*)p->textures.elements[i];
		if ((void*)png == (void*)g_nul)
			continue ;
		j = -1;
		while (++j < png->dim.v[1])
			ft_memcpy(buf + size_max.v[0] * size_max.v[1] * i +
					size_max.v[0] * j, png->buf + png->dim.v[0] * j,
					sizeof(t_uint) * png->dim.v[0]);
	}
	glGenTextures(1, &p->texture_mem.id);
	glBindTexture(GL_TEXTURE_3D, p->texture_mem.id);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, size_max.v[0], size_max.v[1],
			p->textures.elem_count, 0, GL_RGBA, GL_UNSIGNED_BYTE, buf);
	glFinish();
	p->texture_mem.cl_obj = clCreateFromGLTexture(p->opencl->gpu_context,
			CL_MEM_READ_WRITE, GL_TEXTURE_3D, 0, p->texture_mem.id, &err);
	clFinish(p->opencl->gpu_command_queue);
	free(buf);
	if (err != CL_SUCCESS)
		ft_error("Could not create textures buffer (err: %d)\n", err);
}

static void	create_texture_buffer(t_ptr *p)
{
	int		i;
	int		err;
	t_png	*png;
	t_ivec	size_max;
	t_ivec	*sizes;

	size_max = ivec(1, 1);
	if ((sizes = (t_ivec*)ft_memalloc(sizeof(t_ivec) *
					p->textures.elem_count)) == NULL)
		ft_error("Malloc error\n");
	i = -1;
	while (++i < (int)p->textures.elem_count)
	{
		png = (t_png*)p->textures.elements[i];
		if ((void*)png == (void*)g_nul)
			continue ;
		size_max.v[0] = size_max.v[0] >= png->dim.v[0] ?
			size_max.v[0] : png->dim.v[0];
		size_max.v[1] = size_max.v[1] >= png->dim.v[1] ?
			size_max.v[1] : png->dim.v[1];
		sizes[i] = png->dim;
	}
	p->cl_texture_sizes = clCreateBuffer(p->opencl->gpu_context,
			CL_MEM_COPY_HOST_PTR, sizeof(t_ivec) * p->textures.elem_count,
			sizes, &err);
	free(sizes);
	fill_buffers(p, size_max);
}

static int	get_texture_id(t_ptr *p, char *name)
{
	t_png	*png;
	char	*file;
	char	*path;
	int		ret;

	file = NULL;
	path = NULL;
	if ((file = ft_strjoin(name, ".png")) == NULL ||
		(path = ft_strjoin("textures/", file)) == NULL)
		ft_error("ft_strjoin error\n");
	ft_printf("Reading png: %s\n", path);
	if ((png = get_helem(&p->textures, path)) == NULL)
	{
		if ((png = decode_png(path)) == NULL)
		{
			add_helem(&p->textures, path, (void*)g_nul);
			ret = -1;
		}
		else
			ret = add_helem(&p->textures, path, png);
	}
	else if (png == (void*)g_nul)
		ret = -1;
	else 
		ret = get_helem_id(&p->textures, path);
	free(path);
	free(file);
	return (ret);
}

void		init_textures(t_ptr *p)
{
	int		i;
	int		j;
	t_scene	*scene;

	init_hmap(&p->textures);
	i = -1;
	while (++i < (int)p->scenes.elem_count)
	{
		scene = (t_scene*)p->scenes.elements[i];
		j = -1;
		while (++j < scene->nobjs)
		{
			if (scene->objs[j].texture != NULL)
			{
				scene->objs[j].texture_id =
					get_texture_id(p, scene->objs[j].texture);
			}
		}
	}
	create_texture_buffer(p);
}
