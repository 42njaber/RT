/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/20 14:52:32 by njaber            #+#    #+#             */
/*   Updated: 2018/10/20 10:35:15 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void		events_loop(t_ptr *p)
{
	while (!glfwWindowShouldClose(p->win->win))
	{
		glfwWaitEvents();
	}
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
	//mlx_expose_hook(p->win->win, loop_hook, (void*)p);
	glfwSetMouseButtonCallback(p->win->win, mouse_callback);
	glfwSetCursorPosCallback(p->win->win, motion_callback);
	glfwSetKeyCallback(p->win->win,
			(void(*)(GLFWwindow*, int, int, int, int))key_callback);
}
