/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_hooks.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/26 01:32:56 by njaber            #+#    #+#             */
/*   Updated: 2018/10/20 10:43:32 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void		button_press_hook(int button, t_ptr *p)
{
	int		i;
	t_vec2	pos;
	int		x;
	int		y;

	x = p->mouse_pos.v[0];
	y = p->mouse_pos.v[1];
	if (button == GLFW_MOUSE_BUTTON_LEFT && p->gui.state == MENU)
	{
		pos = vec2((float)x / p->win->size.v[0], (float)y / p->win->size.v[1]);
		i = -1;
		while ((t_uint)(++i) < p->scenes.elem_count)
		{
			if (fabs(pos.v[0] - 0.2 * (i % 4 + 1)) < 0.075 &&
					fabs(pos.v[1] - 0.2 * (i / 4 + 1)) < 0.075)
			{
				p->gui.state = INIT_SCENE;
				p->gui.scene_id = i;
			}
		}
	}
	p->button = button;
}

static void		button_release_hook(int button, t_ptr *p)
{
	if (button == p->button)
		p->button = -1;
}

void			mouse_callback(GLFWwindow *win,
								int button, int action, int mods)
{
	t_ptr	*p;

	(void)mods;
	(void)win;
	p = get_p();
	if (action == GLFW_PRESS)
		button_press_hook(button, p);
	else if (action == GLFW_RELEASE)
		button_release_hook(button, p);
}

void			motion_callback(GLFWwindow *win, double xpos, double ypos)
{
	t_ptr	*p;
	int		x;
	int		y;

	(void)win;
	p = get_p();
	x = (int)floor(xpos);
	y = (int)floor(ypos);
	if (p->gui.state == SCENE && p->button != -1)
	{
		p->view.rot = vec2(fmax(-90, fmin(90, p->view.rot.v[0] -
				(y - p->mouse_pos.v[1]) * -0.25)), fmod(p->view.rot.v[1] +
				(x - p->mouse_pos.v[0]) * -0.25, 360));
		p->update = 1;
	}
	p->mouse_pos = ivec(x, y);
}
