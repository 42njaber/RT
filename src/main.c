/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/18 17:01:19 by njaber            #+#    #+#             */
/*   Updated: 2018/06/01 22:14:25 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include "rt.h"
#include "mlx.h"

/*
** p: the program's main structure
** argc, argv: the program's arguments, as received by the main function
**
** i: a counter for the argument being processed
**
** Reads the arguments, and sets the fov, brilliance ** and shadows
** options accordingly
** The first argument is ignored since it's processed by the main function
*/

static void		parse_arguments(t_ptr *p, int argc, char **argv)
{
	int		i;

	p->fov = 90;
	p->brilliance = 1;
	p->shadows = 1;
	i = 1;
	while (++i < argc)
	{
		if (ft_strcmp(argv[i], "-brilliance=off") == 0)
			p->brilliance = 0;
		else if (ft_strcmp(argv[i], "-brilliance=reflected-camera") == 0)
			p->brilliance = 1;
		else if (ft_strcmp(argv[i], "-brilliance=incident-normal") == 0)
			p->brilliance = 2;
		else if (ft_strcmp(argv[i], "-shadows=off") == 0)
			p->shadows = 0;
		else if (ft_strcmp(argv[i], "-shadows=on") == 0)
			p->shadows = 1;
		else if (ft_strncmp(argv[i], "-fov=", 5) == 0)
			p->fov = ft_atoi(argv[i] + 5);
	}
	if (p->fov < 10 || p->fov > 170)
		ft_error("[Error] Please enter an fov value between 10 and 170\n");
}

/*
** p: the program's main structure, which is going to passed to most functions
** fd: the file descriptor of the config file
**
** The main function processes the first argument, acordingly displaying
** the usage and exiting, taking a file that describes the scene as input and
** calling the parser to read it, or setting up the default scene.
** It then initalizes p, reads the following arguments, runs the algorithm to
** process the image, and once this is done, creates the window to display
** the computed image.
*/

#ifdef OPENCL

/*
** p: the program's main structure
**
** Creates the window structure, sets the event hooks, copies the processed
** image to the newly created window, and then launches the main loop
*/

static void		launch_window(t_ptr *p)
{
	if (p->kernel == NULL)
		process_image_opencl(p);
	if ((init_new_win(p->mlx, p->win, (t_ivec){1200, 800}, "RT")) == 0)
		ft_error("[Erreur] Failed to initialize window\n");
	set_hooks(p);
	paint_window(p->win, NULL);
	mlx_loop(p->mlx);
}

int				main(int argc, char **argv)
{
	t_ptr	p;
	int		fd;

	if (argc < 2 || ft_strcmp(argv[1], "help") == 0)
	{
		ft_printf("Usage : rt <help|default|scene-description-file>"
				"[-brilliance=off|reflected-camera|incdent-normal]"
				"[-shadows=on|off] [-fov=number]\n");
		return (0);
	}
	else if (ft_strcmp(argv[1], "default") == 0)
		set_default_scene(&p, (char[3]){1, 1, 1});
	else
	{
		if ((fd = open(argv[1], O_RDONLY)) < 0)
			ft_error("[Error] Could not open the configuration file\n");
		parse_scene_file(&p, fd);
		close(fd);
	}
	if ((p.mlx = mlx_init()) == 0)
		ft_error("[Error] Failed to initialize mlx\n");
	init_struct(&p);
	parse_arguments(&p, argc, argv);
	launch_window(&p);
}

#else

/*
** p: the program's main structure
**
** Creates the window structure, sets the event hooks, copies the processed
** image to the newly created window, and then launches the main loop
*/

static void		launch_window(t_ptr *p)
{
	if ((init_new_win(p->mlx, p->win, (t_ivec){1200, 800}, "RT")) == 0)
		ft_error("[Erreur] Failed to initialize window\n");
	set_hooks(p);
	process_image(p);
	paint_window(p->win, NULL);
	mlx_loop(p->mlx);
}

int				main(int argc, char **argv)
{
	t_ptr	p;
	int		fd;

	if (argc < 2 || ft_strcmp(argv[1], "help") == 0)
	{
		ft_printf("Usage : rt <help|default|scene-description-file>"
				"[-brilliance=off|reflected-camera|indicent-normal]"
				"[-shadows=on|off] [-fov=number]\n");
		return (0);
	}
	else if (ft_strcmp(argv[1], "default") == 0)
		set_default_scene(&p, (char[3]){1, 1, 1});
	else
	{
		if ((fd = open(argv[1], O_RDONLY)) < 0)
			ft_error("[Error] Could not open the configuration file\n");
		parse_scene_file(&p, fd);
		close(fd);
	}
	if ((p.mlx = mlx_init()) == 0)
		ft_error("[Error] Failed to initialize mlx\n");
	init_struct(&p);
	parse_arguments(&p, argc, argv);
	launch_window(&p);
}

#endif
