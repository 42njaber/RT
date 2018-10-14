/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop_hook.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/20 16:00:11 by njaber            #+#    #+#             */
/*   Updated: 2018/10/14 05:26:09 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void		print_data(t_ptr *p)
{
	display_data_scal(p->win, "FPS:", p->win->fps, 10);
	display_data_vec3(p->win, "Positon:", p->pos, 30);
	display_data_vec2(p->win, "Rotation:", p->rot, 50);
	display_data_scal(p->win, "Resolution:", (float)p->res / 2.55, 70);
	display_data_scal(p->win, "Reflections:", (float)p->max_reflections, 90);
}

static void		update_tranform(t_ptr *p)
{
	(void)p;
	generate_cam_matricies(p);
}

static void		read_buffer(t_ptr *p)
{
	cl_int		err;

	clFinish(p->opencl->gpu_command_queue);
	if ((err = clEnqueueReadBuffer(p->opencl->gpu_command_queue,
			p->kernel->memobjs[0], CL_TRUE, 0,
			p->win->img.line * p->win->img.size.v[1],
			p->win->img.buf, 0, NULL, NULL)) != CL_SUCCESS)
		ft_error("[Erreur] Echec durant la lecture du buffer"
				"%<R>  (Error code: %<i>%2d)%<0>\n", err);
}

void			update_image(t_ptr *p)
{
	clFinish(p->opencl->gpu_command_queue);
	if (p->update)
	{
		p->res = 0;
		p->update = 0;
		process_image_opencl(p);
	}
	else if (p->res < 255)
	{
		p->res++;
		process_image_opencl(p);
	}
}

int				loop_hook(void *parm)
{
	t_ptr		*p;

	p = (t_ptr*)parm;
	move(p);
	update_tranform(p);
	read_buffer(p);
	paint_window(p->win, p->kernel, 0);
	update_image(p);
	print_data(p);
	return (0);
}
