/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/20 14:52:32 by njaber            #+#    #+#             */
/*   Updated: 2018/10/27 03:02:13 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void	refresh_callback(GLFWwindow *win)
{
	t_ptr	*p;

	(void)win;
	p = get_p();
	paint_window(p->win, p->opencl);
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
	glfwSetWindowRefreshCallback(p->win->win, refresh_callback);
	glfwSetMouseButtonCallback(p->win->win, mouse_callback);
	glfwSetCursorPosCallback(p->win->win, motion_callback);
	glfwSetKeyCallback(p->win->win,
			(void(*)(GLFWwindow*, int, int, int, int))key_callback);
}
