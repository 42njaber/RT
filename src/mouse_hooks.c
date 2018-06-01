/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_hooks.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/26 01:32:56 by njaber            #+#    #+#             */
/*   Updated: 2018/06/01 19:05:51 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int		button_press_hook(int button, int x, int y, void *parms)
{
	t_ptr	*p;

	(void)(x + y);
	p = (t_ptr*)parms;
	if (button == 1)
	{
		p->tmp_mouse_pos = (t_ivec){x, y};
		p->mouse_pos = (t_ivec){x, y};
		p->origin_rot = p->rot;
	}
	if (button == 4 || button == 5)
	{
		p->fov *= (button == 5) ? 0.96 : 1.04;
		if (p->fov < 10)
			p->fov = 10;
		if (p->fov > 170)
			p->fov = 170;
	}
	else
		p->button = button;
	return (0);
}

int		button_release_hook(int button, int x, int y, void *parms)
{
	t_ptr	*p;

	p = (t_ptr*)parms;
	(void)(button + x + y);
	if (button == 1)
	{
		p->rot = (t_vec3){fmax(-90, fmin(90, p->origin_rot.x + (p->mouse_pos.y
					- p->tmp_mouse_pos.y) * -0.25)), fmod(p->origin_rot.y +
					(p->mouse_pos.x - p->tmp_mouse_pos.x) * -0.25, 360), 0};
	}
	if (button == p->button)
		p->button = -1;
	return (0);
}

int		motion_hook(int x, int y, void *parm)
{
	t_ptr	*p;

	p = (t_ptr*)parm;
	p->mouse_pos = (t_ivec){x, y};
	if (p->button == 1)
	{
		p->rot = (t_vec3){fmax(-90, fmin(90, p->origin_rot.x + (p->mouse_pos.y
					- p->tmp_mouse_pos.y) * -0.25)), fmod(p->origin_rot.y +
					(p->mouse_pos.x - p->tmp_mouse_pos.x) * -0.25, 360), 0};
		p->update = 1;
	}
	return (0);
}
