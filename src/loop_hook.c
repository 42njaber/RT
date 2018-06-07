/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop_hook.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/20 16:00:11 by njaber            #+#    #+#             */
/*   Updated: 2018/06/02 22:08:01 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void		print_data(t_ptr *p)
{
	display_data_float(p->win, "FPS:", p->win->fps, 10);
	display_data_vec3(p->win, "Positon:", p->pos, 30);
	display_data_vec3(p->win, "Rotation:", p->rot, 50);
}

#ifdef OPENCL

static void		update_tranform(t_ptr *p)
{
	(void)p;
	generate_cam_matricies(p);
}

void			update_image(t_ptr *p)
{
	cl_int		err;

    clFinish(p->opencl->gpu_command_queue);
	if (p->update)
	{
		p->update = 0;
		p->timer = 0;
		p->res = -3;
	}
	if (p->tmp == 0)
	{
		if ((err = clEnqueueReadBuffer(p->opencl->gpu_command_queue,
				p->kernel->memobjs[0], CL_TRUE, 0,
				p->win->img.line * p->win->img.size.y,
				p->win->img.buf, 0, NULL, NULL)) != CL_SUCCESS)
			ft_error("[Erreur] Echec durant la lecture du buffer"
					"%<R>  (Error code: %<i>%2d)%<0>\n", err);
		clFinish(p->opencl->gpu_command_queue);
		p->tmp = -1;
	}
	if (p->timer-- <= 0 && p->tmp == -1 && p->res < 3)
	{
		p->res++;
		p->timer = 30;
		process_image_opencl(p);
	}
}

int				loop_hook(void *parm)
{
	t_ptr		*p;

	p = (t_ptr*)parm;
	move(p);
	update_tranform(p);
	paint_window(p->win, p->kernel, 0);
	if (p->kernel != NULL)
		update_image(p);
	print_data(p);
	return (0);
}

#else

int				loop_hook(void *parm)
{
	t_ptr	*p;

	p = (t_ptr*)parm;
	paint_window(p->win, NULL, 1);
	print_data(p);
	return (0);
}

#endif
