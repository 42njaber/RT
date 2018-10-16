/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop_hook.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/20 16:00:11 by njaber            #+#    #+#             */
/*   Updated: 2018/10/16 16:49:03 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void		print_data(t_ptr *p)
{
	display_data_vec3(p->win, "Positon:", p->view.pos, 30);
	display_data_vec2(p->win, "Rotation:", p->view.rot, 50);
	display_data_scal(p->win, "Resolution:", (t_scal)p->view.set.iter / 2.55, 70);
	display_data_scal(p->win, "Reflections:", (t_scal)p->view.set.max_rays, 90);
}

static void			set_guiargs(t_ptr *p)
{
	cl_kernel	painter;

	painter = get_helem(&p->kernel->cores, "gui_painter");
	clSetKernelArg(painter, 0, sizeof(cl_mem), &p->kernel->memobjs[0]);
	clSetKernelArg(painter, 1, sizeof(t_gui), &p->gui);
	clSetKernelArg(painter, 2, sizeof(int[2]), &p->win->size);
	clSetKernelArg(painter, 3, sizeof(cl_mem), &p->scene);
	clSetKernelArg(painter, 4, sizeof(cl_mem), &p->gui.thumbnails);
}

static void			paint_gui(t_ptr *p)
{
	cl_int		err;
	cl_kernel	painter;
	int			g_sz;

	if (p->gui.state == SCENE || p->gui.state == ZOOM_SCENE)
		update_scene(p);
	g_sz = sqrt(p->opencl->gpu_wg_sz / 16);
	set_guiargs(p);
	painter = get_helem(&p->kernel->cores, "gui_painter");
	if ((err = clEnqueueNDRangeKernel(p->opencl->gpu_command_queue, painter,
			2, NULL, (size_t[2]){
			(int)ceil((int)(p->win->size.v[0]) / g_sz + 1) * g_sz,
			(int)ceil((int)(p->win->size.v[1]) / g_sz + 1) * g_sz},
			(size_t[2]){g_sz, g_sz}, 0, NULL, NULL)) != CL_SUCCESS)
		ft_error("[Erreur] Echec d'execution du kernel"
				"%<R>  (Error code: %<i>%2d)%<0>\n", err);
	if ((err = clEnqueueReadBuffer(p->opencl->gpu_command_queue,
			p->kernel->memobjs[0], CL_TRUE, 0,
			p->win->img.line * p->win->img.size.v[1],
			p->win->img.buf, 0, NULL, NULL)) != CL_SUCCESS)
		ft_error("[Erreur] Echec durant la lecture du buffer"
				"%<R>  (Error code: %<i>%2d)%<0>\n", err);
}

static void		update_gui(t_ptr *p)
{
	p->gui.mouse_pos = p->mouse_pos;
	if (p->gui.state == INIT_SCENE)
		init_scene(p);
	else if (p->gui.state == ZOOM_SCENE)
	{
		p->gui.zoom += 0.05;
		if (p->gui.zoom >= 1)
			p->gui.state = SCENE;
	}
	else if (p->gui.state == UNZOOM_SCENE)
	{
		p->gui.zoom -= 0.05;
		if (p->gui.zoom <= 0)
			p->gui.state = QUIT_SCENE;
	}
	if (p->gui.state == QUIT_SCENE)
	{
		cleanup_view(&p->view);
		p->gui.state = MENU;
	}
}

int				loop_hook(void *parm)
{
	t_ptr		*p;

	p = (t_ptr*)parm;
	move(p);
	clFinish(p->opencl->gpu_command_queue);
	update_gui(p);
	paint_window(p->win, p->kernel, 0);
	paint_gui(p);
	display_data_scal(p->win, "FPS:", p->win->fps, 10);
	if (p->gui.state == SCENE)
		print_data(p);
	return (0);
}
