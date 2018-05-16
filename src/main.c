/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/18 17:01:19 by njaber            #+#    #+#             */
/*   Updated: 2018/05/16 18:41:46 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"
#include "mlx.h"

static void		launch_window(t_ptr *p)
{
	if ((p->win = (t_win*)ft_memalloc(sizeof(t_win))) == NULL)
		ft_error("[Erreur] Echec d'allocation mémoire.\n");
	if ((init_new_win(p->mlx, p->win, (t_ivec){1200, 800}, "fractol")) == 0)
		ft_error("[Erreur] Echec de l'intialization de la fenètre\n");
	set_hooks(p);
	mlx_put_image_to_window(p->mlx, p->win->win, p->scene->link, 0, 0);
	mlx_loop(p->mlx);
}

int				main(int argc, char **argv)
{
	t_ptr	p;

	(void)argv;
	(void)argc;
	p.mlx = mlx_init();
	init_struct(&p);
	draw_scene(&p);
	launch_window(&p);
}
