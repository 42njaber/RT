/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_hooks.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/20 14:52:32 by njaber            #+#    #+#             */
/*   Updated: 2018/10/23 11:20:36 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"
#include <math.h>

#define SPEED 20

void			move(t_ptr *p)
{
	t_vec2		rot;
	t_scal		fspeed;

	if (p->gui.state == SCENE)
	{
		fspeed = SPEED * p->win->frame_elapsed;
		rot = vec2(p->view.rot.v[0] * M_PI / 180, p->view.rot.v[1] * M_PI / 180);
		if (p->keys[GLFW_KEY_SPACE])
			p->view.pos = vec3(p->view.pos.v[0], p->view.pos.v[1] + fspeed,
					p->view.pos.v[2]);
		if (p->keys[GLFW_KEY_LEFT_SHIFT])
			p->view.pos = vec3(p->view.pos.v[0], p->view.pos.v[1] - fspeed,
					p->view.pos.v[2]);
		if (p->keys[GLFW_KEY_UP] || p->keys[GLFW_KEY_W])
			p->view.pos = vec3(p->view.pos.v[0] - fspeed * sin(rot.v[1]),
				p->view.pos.v[1], p->view.pos.v[2] + fspeed * cos(rot.v[1]));
		if (p->keys[GLFW_KEY_DOWN] || p->keys[GLFW_KEY_S])
			p->view.pos = vec3(p->view.pos.v[0] + fspeed * sin(rot.v[1]),
				p->view.pos.v[1], p->view.pos.v[2] - fspeed * cos(rot.v[1]));
		if (p->keys[GLFW_KEY_LEFT] || p->keys[GLFW_KEY_A])
			p->view.pos = vec3(p->view.pos.v[0] - fspeed * cos(rot.v[1]),
				p->view.pos.v[1], p->view.pos.v[2] - fspeed * sin(rot.v[1]));
		if (p->keys[GLFW_KEY_RIGHT] || p->keys[GLFW_KEY_D])
			p->view.pos = vec3(p->view.pos.v[0] + fspeed * cos(rot.v[1]),
				p->view.pos.v[1], p->view.pos.v[2] + fspeed * sin(rot.v[1]));
		if (p->keys[GLFW_KEY_LEFT] || p->keys[GLFW_KEY_RIGHT] ||
				p->keys[GLFW_KEY_UP] || p->keys[GLFW_KEY_DOWN] ||
				p->keys[GLFW_KEY_SPACE] || p->keys[GLFW_KEY_LEFT_SHIFT] ||
				p->keys[GLFW_KEY_W] || p->keys[GLFW_KEY_S] ||
				p->keys[GLFW_KEY_A] || p->keys[GLFW_KEY_D])
			p->update = 1;
	}
}

static int		key_press_hook(GLFWwindow *win, int key_code, t_ptr *p)
{
	p->keys[key_code] = 1;
	if (key_code == GLFW_KEY_ESCAPE)
	{
		if (p->gui.state == MENU)
			glfwSetWindowShouldClose(win, GLFW_TRUE);
		else if (p->gui.state == SCENE)
		{
			p->gui.zoom = 1;
			p->gui.state = UNZOOM_SCENE;
		}
	}
	if (key_code == GLFW_KEY_PAGE_UP && p->view.set.max_rays < 20)
	{
		p->view.set.max_rays++;
		p->update = 1;
	}
	if (key_code == GLFW_KEY_PAGE_DOWN && p->view.set.max_rays > 1)
	{
		p->view.set.max_rays--;
		p->update = 1;
	}
	//ft_printf("Key pressed: %d\n", key_code);
	return (0);
}

int				key_release_hook(int key_code, t_ptr *p)
{
	p->keys[key_code] = 0;
	return (0);
}

void			key_callback(GLFWwindow *win, int key, int scancode, int action)
{
	t_ptr	*p;
	(void)win;
	(void)scancode;

	p = get_p();
	if (action == GLFW_PRESS)
		key_press_hook(win, key, p);
	else if (action == GLFW_RELEASE)
		key_release_hook(key, p);
}
