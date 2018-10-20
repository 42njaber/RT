/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/18 17:01:19 by njaber            #+#    #+#             */
/*   Updated: 2018/10/20 10:47:02 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include "rt.h"

static void		end_environement(void *data, int status)
{
	t_ptr	*p;

	p = (t_ptr*)data;
	glfwDestroyWindow(p->win->win);
	glfwTerminate();
	exit(status);
}

static void		glfw_error_callback(int err, const char *strerr)
{
	(void)err;
	ft_printf("GLFW Error: %s\n", strerr);
}

/*
** p: the program's main structure
**
** Creates the window structure, sets the event hooks, copies the processed
** image to the newly created window, and then launches the main loop
*/

static void		launch_window(t_ptr *p)
{
	glfwShowWindow(p->win->win);
	glfwFocusWindow(p->win->win);
	glViewport(0, 0, p->win->size.v[0], p->win->size.v[1]);
	glfwSwapInterval(1);
	set_hooks(p);
	loop_hook(p);
	end_environement(p, 0);
}

t_ptr			*get_p(void)
{
	static t_ptr	p;
	static int		init;

	if (!init)
	{
		init = 1;
		ft_bzero(&p, sizeof(p));
	}
	return (&p);
}

/*
** p: the program's main structure, which is going to passed to most functions
** fd: the file descriptor of the config file
**
** The main function processes the first argument, acordingly displaying
** the usage and exiting, taking a file that describes the scene as input and
** calling the parser to read it, or setting up the default scene.
** It then initalizes p, reads the following arguments and creates the window
** to start the display
*/

int				main(int argc, char **argv)
{
	int		i;
	t_ptr	*p;

	p = get_p();
	ft_set_error_callback(end_environement, p);
	if (argc < 2 || ft_strcmp(argv[1], "help") == 0)
	{
		ft_printf("Usage : rt <scene.xml>");
		return (0);
	}
	init_hmap(&p->scenes);
	i = 0;
	while (++i < argc)
		read_path(p, argv[i]);
	if (p->scenes.elem_count < 1)
		ft_error("%<R,!>[Error]%<0> Could not read any map, qutting...\n");
	argc = 1;
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
		ft_error("Failed to initialize glut\n");
	init_struct(p);
	launch_window(p);
}
