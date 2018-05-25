/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/20 13:17:27 by njaber            #+#    #+#             */
/*   Updated: 2018/05/26 00:26:12 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"
#include "mlx.h"

static void		generate_matricies(t_ptr *p)
{
	int		i;

	i = -1;
	while (++i < p->nobjs)
	{
		identity(p->objs[i].transform);
		translate(p->objs[i].transform, (t_vec3){-p->objs[i].pos.x,
				-p->objs[i].pos.y, -p->objs[i].pos.z});
		rotate(p->objs[i].transform, (t_vec3){-p->objs[i].rot.x,
				-p->objs[i].rot.y, -p->objs[i].rot.z});
		identity(p->objs[i].rev_rot);
		rotate(p->objs[i].rev_rot, (t_vec3){p->objs[i].rot.x,
				p->objs[i].rot.y, p->objs[i].rot.z});
		identity(p->objs[i].rot_mat);
		rotate(p->objs[i].rot_mat, (t_vec3){-p->objs[i].rot.x,
				-p->objs[i].rot.y, -p->objs[i].rot.z});
	}
	identity(p->cam_mat);
	rotate(p->cam_mat, (t_vec3){p->cam_rot.x, p->cam_rot.y, p->cam_rot.z});
	translate(p->cam_mat, (t_vec3){p->cam_pos.x, p->cam_pos.y, p->cam_pos.z});
	identity(p->cam_mat_rot);
	rotate(p->cam_mat_rot, (t_vec3){p->cam_rot.x, p->cam_rot.y, p->cam_rot.z});
}

void			init_struct(t_ptr *p)
{
	if ((p->scene = ft_memalloc(sizeof(t_img))) == NULL)
		ft_error("[Erreur] Failed to allocate memory\n");
	init_new_image(p->mlx, p->scene, (t_ivec){1200, 800});
	generate_matricies(p);
	p->fov = 90;
	p->brillance = 1;
	p->shadows = 1;
	p->near = 0.1;
	p->far = 400;
}
