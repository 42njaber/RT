
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/20 13:17:27 by njaber            #+#    #+#             */
/*   Updated: 2018/06/01 00:09:31 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

/*
** p: the program's main structure
**
** Initializes the structure's value and creates the buffer for the computed
** image */

#ifdef OPENCL

void			init_struct(t_ptr *p)
{
	if ((p->win = (t_win*)ft_memalloc(sizeof(t_win))) == NULL)
		ft_error("[Erreur] Failed to allocate memory\n");
	init_new_image(p->mlx, &p->win->img, (t_ivec){1200, 800});
	generate_cam_matricies(p);
	generate_obj_matricies(p);
	p->near = 0.1;
	p->far = 50000;
	p->res = -2;
	p->res_min = -2;
	p->res_max = 2;
	p->update = 30;
	p->opencl = init_opencl();
	p->kernel = create_kernel(p);
	p->tmp = 0;
	if (p->kernel != NULL)
	{
		create_obj_memobjs(p);
		create_spot_memobjs(p);
	}
}

#else

void			init_struct(t_ptr *p)
{
	if ((p->win = (t_win*)ft_memalloc(sizeof(t_win))) == NULL)
		ft_error("[Erreur] Failed to allocate memory\n");
	init_new_image(p->mlx, &p->win->img, (t_ivec){1200, 800});
	generate_cam_matricies(p);
	generate_obj_matricies(p);
	p->near = 0.1;
	p->far = 400;
}

#endif
