/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/20 13:17:27 by njaber            #+#    #+#             */
/*   Updated: 2018/05/17 01:18:51 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"
#include "mlx.h"

void		init_struct(t_ptr *p)
{
	int		i;

	if ((p->scene = ft_memalloc(sizeof(t_img))) == NULL)
		ft_error("[Erreur] Echec d'allocation mémoire.\n");
	init_new_image(p->mlx, p->scene, (t_ivec){1200, 800});
	p->nobjs = 4;
	if ((p->objs = ft_memalloc(sizeof(t_obj) * 4)) == NULL)
		ft_error("[Erreur] Echec d'allocation mémoire.\n");
	p->objs[0] = (t_obj){0, (t_vec3){30, 0, 30}, (t_vec3){0, 0, 0},
		10, 0x00FFAA00, {0}};
	p->objs[1] = (t_obj){1, (t_vec3){0, 0, 60}, (t_vec3){0, 0, 0},
		1, 0x00FF22FF, {0}};
	p->objs[2] = (t_obj){2, (t_vec3){-40, 0, 30}, (t_vec3){0, 0, 0},
		5, 0x0000AA00, {0}};
	p->objs[3] = (t_obj){3, (t_vec3){0, 0, 40}, (t_vec3){0, 0, 0},
		1, 0x000022FF, {0}};
	i = -1;
	while (++i < p->nobjs)
	{
		identity(p->objs[i].transform);
		translate(p->objs[i].transform, (t_vec3){-p->objs[i].pos.x,
				-p->objs[i].pos.y, -p->objs[i].pos.z});
		rotate(p->objs[i].transform, (t_vec3){-p->objs[i].rot.x,
				-p->objs[i].rot.y, -p->objs[i].rot.z});
		print_mat(p->objs[i].transform);
	}
	p->spot = (t_spot){(t_vec3){0, 0, -5}, 1000};
	p->mouse_pos = (t_ivec){0, 0};
	p->fov = 45;
	p->near = 0.1;
	p->far = 100;
}

static int	close_hook(void *parm)
{
	(void)parm;
	exit(0);
}

void		set_hooks(t_ptr *p)
{
	/*
	mlx_loop_hook(p->win->mlx, loop_hook, (void*)p);
	mlx_expose_hook(p->win->win, loop_hook, (void*)p);
	mlx_hook(p->win->win, 6, 0, motion_hook, (void*)p);
	mlx_hook(p->win->win, 4, 0, button_press_hook, (void*)p);
	mlx_hook(p->win->win, 5, 0, button_release_hook, (void*)p);
	*/
	mlx_hook(p->win->win, 2, 0, key_press_hook, (void*)p);
	mlx_hook(p->win->win, 3, 0, key_release_hook, (void*)p);
	mlx_hook(p->win->win, 17, 0, close_hook, (void*)p);
}
