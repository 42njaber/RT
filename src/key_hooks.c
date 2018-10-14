/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_hooks.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/20 14:52:32 by njaber            #+#    #+#             */
/*   Updated: 2018/10/14 05:26:59 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"
#include <math.h>

#define SPEED 20

void			move(t_ptr *p)
{
	t_vec2		rot;
	t_scal		fspeed;

	fspeed = SPEED * p->win->frame_elapsed;
	rot = vec2(p->rot.v[0] * M_PI / 180, p->rot.v[1] * M_PI / 180);
	if (p->keys[KEY_SPACE])
		p->pos = vec3(p->pos.v[0], p->pos.v[1] - fspeed, p->pos.v[2]);
	if (p->keys[KEY_LSHIFT])
		p->pos = vec3(p->pos.v[0], p->pos.v[1] + fspeed, p->pos.v[2]);
	if (p->keys[KEY_UP] || p->keys[KEY_W])
		p->pos = vec3(p->pos.v[0] - fspeed * sin(rot.v[1]), p->pos.v[1],
			p->pos.v[2] + fspeed * cos(rot.v[1]));
	if (p->keys[KEY_DOWN] || p->keys[KEY_S])
		p->pos = vec3(p->pos.v[0] + fspeed * sin(rot.v[1]), p->pos.v[1],
			p->pos.v[2] - fspeed * cos(rot.v[1]));
	if (p->keys[KEY_LEFT] || p->keys[KEY_A])
		p->pos = vec3(p->pos.v[0] - fspeed * cos(rot.v[1]), p->pos.v[1],
			p->pos.v[2] - fspeed * sin(rot.v[1]));
	if (p->keys[KEY_RIGHT] || p->keys[KEY_D])
		p->pos = vec3(p->pos.v[0] + fspeed * cos(rot.v[1]), p->pos.v[1],
			p->pos.v[2] + fspeed * sin(rot.v[1]));
	if (p->keys[KEY_LEFT] || p->keys[KEY_RIGHT] || p->keys[KEY_UP] ||
			p->keys[KEY_DOWN] || p->keys[KEY_SPACE] || p->keys[KEY_LSHIFT] ||
			p->keys[KEY_W] || p->keys[KEY_S] || p->keys[KEY_A] ||
			p->keys[KEY_D])
		p->update = 1;
}

int				key_press_hook(int key_code, void *parm)
{
	t_ptr	*p;

	p = (t_ptr*)parm;
	p->keys[key_code] = 1;
	if (key_code == KEY_ESCAPE)
		exit(0);
	if (key_code == KEY_PAGEUP && p->max_reflections < 10)
	{
		p->max_reflections++;
		p->update = 1;
	}
	if (key_code == KEY_PAGEDOWN && p->max_reflections > 1)
	{
		p->max_reflections--;
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
