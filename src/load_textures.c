/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_textures.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <njaber@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/25 00:56:37 by njaber            #+#    #+#             */
/*   Updated: 2018/10/27 02:48:48 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static t_uint	*fill_buffer(t_ptr *p, t_ivec size_max)
{
	int		i;
	int		j;
	t_png	*png;
	t_uint	*buf;

	if (p->textures.elem_count < 1 && (buf = ft_memalloc(4)) == NULL)
		ft_error("Malloc error\n");
	else if (p->textures.elem_count >= 1)
	{
		if ((buf = ft_memalloc(size_max.v[0] * size_max.v[1] * sizeof(t_uint) *
						p->textures.elem_count)) == NULL)
			ft_error("Malloc error\n");
		i = -1;
		while (++i < (int)p->textures.elem_count)
		{
			if ((void*)(png = (t_png*)p->textures.elements[i]) == (void*)g_nul)
				continue ;
			j = -1;
			while (++j < png->dim.v[1])
				ft_memcpy(buf + size_max.v[0] * size_max.v[1] * i +
						size_max.v[0] * j, png->buf + png->dim.v[0] * j,
						sizeof(t_uint) * png->dim.v[0]);
		}
	}
	return (buf);
}

static void		create_mem_objects(t_ptr *p, t_ivec size_max)
{
	int		img_number;
	int		err;
	t_uint	*buf;

	img_number = p->textures.elem_count;
	buf = fill_buffer(p, size_max);
	if (img_number < 1)
		img_number = 1;
	glGenTextures(1, &p->texture_mem.id);
	glBindTexture(GL_TEXTURE_3D, p->texture_mem.id);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, size_max.v[0], size_max.v[1],
		img_number, 0, GL_RGBA, GL_UNSIGNED_BYTE, buf);
	glFinish();
	p->texture_mem.cl_obj = clCreateFromGLTexture(p->opencl->gpu_context,
			CL_MEM_READ_WRITE, GL_TEXTURE_3D, 0, p->texture_mem.id, &err);
	clFinish(p->opencl->gpu_command_queue);
	free(buf);
	if (err != CL_SUCCESS)
		ft_error("Could not create textures buffer (err: %d)\n", err);
}

static void		create_texture_buffer(t_ptr *p)
{
	int		i;
	int		err;
	t_png	*png;
	t_ivec	size_max;
	t_ivec	*sizes;

	size_max = ivec(1, 1);
	if ((sizes = (t_ivec*)ft_memalloc(sizeof(t_ivec) *
					ft_max(1, p->textures.elem_count))) == NULL)
		ft_error("Malloc error\n");
	i = -1;
	while (++i < (int)p->textures.elem_count)
	{
		png = (t_png*)p->textures.elements[i];
		if ((void*)png == (void*)g_nul)
			continue ;
		size_max.v[0] = ft_max(size_max.v[0], png->dim.v[0]);
		size_max.v[1] = ft_max(size_max.v[1], png->dim.v[1]);
		sizes[i] = png->dim;
	}
	p->cl_texture_sizes = clCreateBuffer(p->opencl->gpu_context,
			CL_MEM_COPY_HOST_PTR, sizeof(t_ivec) *
			ft_max(1, p->textures.elem_count), sizes, &err);
	free(sizes);
	create_mem_objects(p, size_max);
}

static int		get_texture_id(t_ptr *p, char *name)
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
	ft_printf("Reading texture: %<G>%s%<0>\n", path);
	png = get_helem(&p->textures, path);
	ret = -1;
	if (png != NULL && (void*)png != (void*)g_nul)
		ret = get_helem_id(&p->textures, path);
	else if ((png = decode_png(path)) == NULL)
		add_helem(&p->textures, path, (void*)g_nul);
	else
		ret = add_helem(&p->textures, path, png);
	free(path);
	free(file);
	return (ret);
}

void			init_textures(t_ptr *p)
{
	int		i;
	int		j;
	t_scene	*scene;

	init_hmap(&p->textures);
	i = -1;
	ft_printf("%<y>Loading textures...%<0>\n");
	while (++i < (int)p->scenes.elem_count)
	{
		scene = (t_scene*)p->scenes.elements[i];
		j = -1;
		while (++j < scene->nobjs)
			if (scene->objs[j].texture != NULL)
			{
				scene->objs[j].texture_id =
					get_texture_id(p, scene->objs[j].texture);
			}
	}
	ft_printf("%<y>Creating texture buffer...%<0>\n");
	create_texture_buffer(p);
	ft_printf("%<y>Done!%<0>\n");
}
