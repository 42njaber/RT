/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/18 17:01:19 by njaber            #+#    #+#             */
/*   Updated: 2018/10/16 17:43:42 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include "rt.h"
#include "mlx.h"

/*
** p: the program's main structure
**
** Creates the window structure, sets the event hooks, copies the processed
** image to the newly created window, and then launches the main loop
*/

static void		launch_window(t_ptr *p)
{
	if ((init_new_win(p->mlx, p->win,
					ivec(DEFAULT_WIDTH, DEFAULT_HEIGHT), "RT")) == 0)
		ft_error("[Erreur] Failed to initialize window\n");
	set_hooks(p);
	mlx_loop(p->mlx);
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

static void		error_callback(void *data, int status)
{
	(void)data;
	exit(status);
}

int				main(int argc, char **argv)
{
	t_ptr	p;
	int		i;

	ft_set_error_callback(error_callback, &p);
	if (argc < 2 || ft_strcmp(argv[1], "help") == 0)
	{
		ft_printf("Usage : rt <scene.xml>");
		return (0);
	}
	if ((p.mlx = mlx_init()) == 0)
		ft_error("[Error] Failed to initialize mlx\n");
	init_hmap(&p.scenes);
	i = 0;
	while (++i < argc)
		read_path(&p, argv[i]);
	if (p.scenes.elem_count < 1)
		ft_error("%<R,!>[Error]%<0> Could not read any map, qutting...\n");
	init_struct(&p);
	launch_window(&p);
}
