/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/26 00:14:34 by njaber            #+#    #+#             */
/*   Updated: 2018/10/27 02:42:36 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libgxns.h"

static void		draw_texture(t_win *win)
{
	glEnable(GL_TEXTURE_2D);
	glClear(GL_COLOR_BUFFER_BIT);
	glBindTexture(GL_TEXTURE_2D, win->img.id);
	glLoadIdentity();
	glBegin(GL_QUADS);
	glTexCoord2f(0., 0.);
	glVertex2f(-1, 1);
	glTexCoord2f(1., 0.);
	glVertex2f(1., 1);
	glTexCoord2f(1., 1.);
	glVertex2f(1., -1.);
	glTexCoord2f(0., 1.);
	glVertex2f(-1., -1.);
	glDisable(GL_TEXTURE_2D);
	glEnd();
	glFlush();
	glFinish();
}

void			paint_window(t_win *win, t_ocl *opencl)
{
	uint64_t	time;

	(void)opencl;
	time = clock_gettime_nsec_np(CLOCK_UPTIME_RAW);
	if (win->frame > 0)
		win->frame_elapsed = (t_scal)(time - win->frames[win->frame % 30]) /
			1000000000.;
	win->frame++;
	if (win->frame % 5 == 0)
	{
		win->fps = (float)30000000000 / (time - win->frames[win->frame % 30]);
		ft_printf("Fps: %.3g\n", win->fps);
	}
	win->frames[win->frame % 30] = time;
	draw_texture(win);
}

int				init_new_win(t_win *win, t_ivec size, char *title)
{
	(void)title;
	glfwWindowHint(GLFW_SAMPLES, 1);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	if ((win->win = glfwCreateWindow(size.v[0], size.v[1], title,
				NULL, NULL)) == NULL)
		return (EXIT_FAILURE);
	glfwMakeContextCurrent(win->win);
	win->size = size;
	win->frame = 0;
	win->frame_elapsed = 0;
	win->fps = 0;
	ft_bzero(win->frames, 30 * sizeof(uint64_t));
	return (EXIT_SUCCESS);
}
