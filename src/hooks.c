/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/20 14:52:32 by njaber            #+#    #+#             */
/*   Updated: 2018/05/27 02:30:53 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"
#include "mlx.h"

/*
** If the escape key is pressed, close the program
*/

static int		key_press_hook(int key_code, void *parm)
{
	(void)parm;
	if (key_code == K_ESC)
		exit(0);
	return (0);
}

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
** Readraws the computed image to the window
*/

static int		expose_hook(void *parm)
{
	t_ptr *p;

	p = (t_ptr*)parm;
	mlx_clear_window(p->mlx, p->win->win);
	mlx_put_image_to_window(p->mlx, p->win->win, p->scene->link, 0, 0);
	return (0);
}

/*
** p: the program's main structure
**
** Sets the hooks for the folowing events:
**  - Key press
**  - Expose
**  - Close button clicked
*/

void			set_hooks(t_ptr *p)
{
	mlx_expose_hook(p->win->win, expose_hook, (void*)p);
	mlx_hook(p->win->win, 2, 0, key_press_hook, (void*)p);
	mlx_hook(p->win->win, 17, 0, close_hook, (void*)p);
}