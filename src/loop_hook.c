/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop_hook.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/20 16:00:11 by njaber            #+#    #+#             */
/*   Updated: 2018/05/30 23:44:02 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void		print_data(t_ptr *p)
{
	display_data_float(p->win, "FPS:", p->win->fps, 10);
	display_data_vec3(p->win, "Positon:", (t_vec3){p->pos.x * 10,
			p->pos.y * 10, p->pos.z}, 30);
}

#ifdef OPENCL

static void		update_tranform(t_ptr *p)
{
	(void)p;
}

int				loop_hook(void *parm)
{
	t_ptr	*p;

	p = (t_ptr*)parm;
	update_tranform(p);
	paint_window(p->win, p->kernel, 0);
	if (p->kernel != NULL)
		process_image_opencl(p);
	print_data(p);
	return (0);
}

#else

int				loop_hook(void *parm)
{
	t_ptr	*p;

	p = (t_ptr*)parm;
	paint_window(p->win, NULL, 0);
	print_data(p);
	return (0);
}

#endif
