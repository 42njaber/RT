/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_hooks.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/20 14:52:32 by njaber            #+#    #+#             */
/*   Updated: 2018/10/16 16:36:38 by njaber           ###   ########.fr       */
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
		if (p->keys[KEY_SPACE])
			p->view.pos = vec3(p->view.pos.v[0], p->view.pos.v[1] + fspeed,
					p->view.pos.v[2]);
		if (p->keys[KEY_LSHIFT])
			p->view.pos = vec3(p->view.pos.v[0], p->view.pos.v[1] - fspeed,
					p->view.pos.v[2]);
		if (p->keys[KEY_UP] || p->keys[KEY_W])
			p->view.pos = vec3(p->view.pos.v[0] - fspeed * sin(rot.v[1]),
					p->view.pos.v[1], p->view.pos.v[2] + fspeed * cos(rot.v[1]));
		if (p->keys[KEY_DOWN] || p->keys[KEY_S])
			p->view.pos = vec3(p->view.pos.v[0] + fspeed * sin(rot.v[1]),
					p->view.pos.v[1], p->view.pos.v[2] - fspeed * cos(rot.v[1]));
		if (p->keys[KEY_LEFT] || p->keys[KEY_A])
			p->view.pos = vec3(p->view.pos.v[0] - fspeed * cos(rot.v[1]),
					p->view.pos.v[1], p->view.pos.v[2] - fspeed * sin(rot.v[1]));
		if (p->keys[KEY_RIGHT] || p->keys[KEY_D])
			p->view.pos = vec3(p->view.pos.v[0] + fspeed * cos(rot.v[1]),
					p->view.pos.v[1], p->view.pos.v[2] + fspeed * sin(rot.v[1]));
		if (p->keys[KEY_LEFT] || p->keys[KEY_RIGHT] || p->keys[KEY_UP] ||
				p->keys[KEY_DOWN] || p->keys[KEY_SPACE] || p->keys[KEY_LSHIFT] ||
				p->keys[KEY_W] || p->keys[KEY_S] || p->keys[KEY_A] ||
				p->keys[KEY_D])
			p->update = 1;
	}
}

int				key_press_hook(int key_code, void *parm)
{
	t_ptr	*p;

	p = (t_ptr*)parm;
	p->keys[key_code] = 1;
	if (key_code == KEY_ESCAPE)
	{
		if (p->gui.state == MENU)
			exit(0);
		else if (p->gui.state == SCENE)
		{
			p->gui.zoom = 1;
			p->gui.state = UNZOOM_SCENE;
		}
	}
	if (key_code == KEY_PAGEUP && p->view.set.max_rays < 10)
	{
		p->view.set.max_rays++;
		p->update = 1;
	}
	if (key_code == KEY_PAGEDOWN && p->view.set.max_rays > 1)
	{
		p->view.set.max_rays--;
		p->update = 1;
	}
	//ft_printf("Key pressed: %d\n", key_code);
	return (0);
}

int				key_release_hook(int key_code, void *parm)
{
	t_ptr	*p;

	p = (t_ptr*)parm;
	p->keys[key_code] = 0;
	return (0);
}
