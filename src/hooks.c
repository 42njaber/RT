/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/20 14:52:32 by njaber            #+#    #+#             */
/*   Updated: 2018/06/01 18:07:33 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"
#include "mlx.h"

/*
** Closes the program
*/

static int		close_hook(void *parm)
{
	(void)parm;
	exit(0);
	return (0);
}

/*
** p: the program's main structure
**
** Sets the hooks for the folowing events:
**  - Key press
**  - Key release
**  - Loop
**  - Expose
**  - Close button clicked
*/

void		set_hooks(t_ptr *p)
{
	mlx_loop_hook(p->win->mlx, loop_hook, (void*)p);
	mlx_expose_hook(p->win->win, loop_hook, (void*)p);
	mlx_hook(p->win->win, 6, 0, motion_hook, (void*)p);
	mlx_hook(p->win->win, 4, 0, button_press_hook, (void*)p);
	mlx_hook(p->win->win, 5, 0, button_release_hook, (void*)p);
	mlx_hook(p->win->win, 2, 0, key_press_hook, (void*)p);
	mlx_hook(p->win->win, 3, 0, key_release_hook, (void*)p);
	mlx_hook(p->win->win, 17, 0, close_hook, (void*)p);
}
