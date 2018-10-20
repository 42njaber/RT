/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop_hook.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/20 16:00:11 by njaber            #+#    #+#             */
/*   Updated: 2018/10/20 10:25:17 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void			set_guiargs(t_ptr *p)
{
	cl_kernel	painter;

	painter = get_helem(&p->kernel->cores, "gui_painter");
	clSetKernelArg(painter, 0, sizeof(cl_mem), &p->win->img.cl_obj);
	clSetKernelArg(painter, 1, sizeof(t_gui), &p->gui);
	clSetKernelArg(painter, 2, sizeof(int[2]), &p->win->size);
	clSetKernelArg(painter, 3, sizeof(cl_mem), &p->scene);
	clSetKernelArg(painter, 4, sizeof(cl_mem), &p->gui.thumbnails);
	clSetKernelArg(painter, 5, sizeof(t_set), &p->view.set);
}

static void			paint_gui(t_ptr *p)
{
	cl_int		err;
	cl_kernel	painter;
	int			g_sz;

	if ((err = clEnqueueAcquireGLObjects(p->opencl->gpu_command_queue, 1,
					&p->win->img.cl_obj, 0, 0, NULL)) != CL_SUCCESS)
		ft_error("[Error] Failed to acquire OpenGl object"
				"%<R>  (Error code: %<i>%2d)%<0>\n", err);
	if (p->gui.state == SCENE || p->gui.state == ZOOM_SCENE)
		update_scene(p);
	g_sz = sqrt(WG_SIZE);
	set_guiargs(p);
	painter = get_helem(&p->kernel->cores, "gui_painter");
	clFlush(p->opencl->gpu_command_queue);
	clFinish(p->opencl->gpu_command_queue);
	if ((err = clEnqueueNDRangeKernel(p->opencl->gpu_command_queue, painter,
			2, NULL, (size_t[2]){
			(int)ceil((int)(p->win->size.v[0]) / g_sz + 1) * g_sz,
			(int)ceil((int)(p->win->size.v[1]) / g_sz + 1) * g_sz},
			(size_t[2]){g_sz, g_sz}, 0, NULL, NULL)) != CL_SUCCESS)
		ft_error("[Error] Gailed to launchkernel kernel"
				"%<R>  (Error code: %<i>%2d)%<0>\n", err);
	clFlush(p->opencl->gpu_command_queue);
	clFinish(p->opencl->gpu_command_queue);
	if ((err = clEnqueueReleaseGLObjects(p->opencl->gpu_command_queue, 1,
					&p->win->img.cl_obj, 0, NULL, NULL)) != CL_SUCCESS)
		ft_error("[Error] Failed to release OpenGl object"
				"%<R>  (Error code: %<i>%2d)%<0>\n", err);
	clFinish(p->opencl->gpu_command_queue);
}

static void		update_gui(t_ptr *p)
{
	p->gui.mouse_pos = p->mouse_pos;
	if (p->gui.state == INIT_SCENE)
		init_scene(p);
	else if (p->gui.state == ZOOM_SCENE)
	{
		p->gui.zoom += 0.25;
		if (p->gui.zoom >= 1)
			p->gui.state = SCENE;
	}
	else if (p->gui.state == UNZOOM_SCENE)
	{
		p->gui.zoom -= 0.25;
		if (p->gui.zoom <= 0)
			p->gui.state = QUIT_SCENE;
	}
	if (p->gui.state == QUIT_SCENE)
	{
		cleanup_view(&p->view);
		p->gui.state = MENU;
	}
}

void				*loop_hook(t_ptr *p)
{
	glfwMakeContextCurrent(p->win->win);
	while (!glfwWindowShouldClose(p->win->win))
	{
		glfwPollEvents();
		move(p);
		update_gui(p);
		clFinish(p->opencl->gpu_command_queue);
		paint_window(p->win, p->opencl);
		paint_gui(p);
		glfwSwapBuffers(p->win->win);
	}
	return (NULL);
}
