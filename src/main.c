/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/18 17:01:19 by njaber            #+#    #+#             */
/*   Updated: 2018/05/26 00:07:03 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include "rtv1.h"
#include "mlx.h"

static void		launch_window(t_ptr *p)
{
	if ((p->win = (t_win*)ft_memalloc(sizeof(t_win))) == NULL)
		ft_error("[Erreur] Failed to allocate memory\n");
	if ((init_new_win(p->mlx, p->win, (t_ivec){1200, 800}, "fractol")) == 0)
		ft_error("[Erreur] Failed to initialize window\n");
	set_hooks(p);
	mlx_put_image_to_window(p->mlx, p->win->win, p->scene->link, 0, 0);
	mlx_loop(p->mlx);
}

static void		parse_arguments(t_ptr *p, int argc, char **argv)
{
	int		i;

	i = 1;
	while (++i < argc)
	{
		if (ft_strcmp(argv[i], "-brillance=off") == 0)
			p->brillance = 0;
		else if (ft_strcmp(argv[i], "-brillance=reflected-camera") == 0)
			p->brillance = 1;
		else if (ft_strcmp(argv[i], "-brillance=incident-normal") == 0)
			p->brillance = 2;
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

int				main(int argc, char **argv)
{
	t_ptr	p;
	int		fd;

	if (argc < 2 || ft_strcmp(argv[1], "help") == 0)
	{
		ft_printf("Usage : rtv1 <help|default|config-file> [-brillance=off|"
				"reflected-camera|indicent-normal] [-shadows=on|off] "
				"[-fov=number]\n");
		return (0);
	}
	else if (ft_strcmp(argv[1], "default") == 0)
		set_default(&p, (char[3]){1, 1, 1});
	else
	{
		if ((fd = open(argv[1], O_RDONLY)) < 0)
			ft_error("[Error] Could not open the configuration file\n");
		parse_configs(&p, fd);
		close(fd);
	}
	if ((p.mlx = mlx_init()) == 0)
		ft_error("[Error] Failed to initialize mlx\n");
	init_struct(&p);
	parse_arguments(&p, argc, argv);
	draw_scene(&p);
	launch_window(&p);
}
