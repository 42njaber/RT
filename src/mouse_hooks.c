/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_hooks.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/26 01:32:56 by njaber            #+#    #+#             */
/*   Updated: 2018/10/16 16:24:41 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int		button_press_hook(int button, int x, int y, void *parms)
{
	t_ptr	*p;
	int		i;
	t_vec2	pos;

	p = (t_ptr*)parms;
	if (button == 1 && p->gui.state == MENU)
	{
		pos = vec2((float)x / p->win->size.v[0], (float)y / p->win->size.v[1]);
		i = -1;
		while ((t_uint)(++i) < p->scenes.elem_count)
		{
			if (fabs(pos.v[0] - 0.2 * (i + 1)) < 0.075 &&
					fabs(pos.v[1] -0.2) < 0.075)
			{
				p->gui.state = INIT_SCENE;
				p->gui.scene_id = i;
			}
		}
	}
	if (button < 4)
		p->button = button;
	return (0);
}

int		button_release_hook(int button, int x, int y, void *parms)
{
	t_ptr	*p;

	p = (t_ptr*)parms;
	(void)(button + x + y);
	if (button == p->button)
		p->button = -1;
	return (0);
}

int		motion_hook(int x, int y, void *parm)
{
	t_ptr	*p;

	p = (t_ptr*)parm;
	if (p->gui.state == SCENE && p->button == 1)
	{
		p->view.rot = vec2(fmax(-90, fmin(90, p->view.rot.v[0] -
				(y - p->mouse_pos.v[1]) * -0.25)), fmod(p->view.rot.v[1] +
				(x - p->mouse_pos.v[0]) * -0.25, 360));
		p->update = 1;
	}
	p->mouse_pos = ivec(x, y);
	return (0);
}
