/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_data.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/10 12:10:10 by njaber            #+#    #+#             */
/*   Updated: 2018/10/17 18:19:38 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libgxns.h"

void	display_data_scal(t_win *win, char *name, t_scal data, int y)
{
	char	*tmp;

	(void)win;
	(void)y;
	tmp = ft_asprint("%s %.3g", name, data);
	// TODO mlx_string_put(win->mlx, win->win, 10, y, 0x777777, tmp);
	free(tmp);
}

void	display_data_vec2(t_win *win, char *name, t_vec2 data, int y)
{
	char	*tmp;

	(void)win;
	(void)y;
	tmp = ft_asprint("%s %g, %g", name, data.v[0], data.v[1]);
	// TODO mlx_string_put(win->mlx, win->win, 10, y, 0x777777, tmp);
	free(tmp);
}

void	display_data_vec3(t_win *win, char *name, t_vec3 data, int y)
{
	char	*tmp;

	(void)win;
	(void)y;
	tmp = ft_asprint("%s %g, %g, %g", name, data.v[0], data.v[1], data.v[2]);
	// TODO mlx_string_put(win->mlx, win->win, 10, y, 0x777777, tmp);
	free(tmp);
}

void	display_data_str(t_win *win, char *name, char *data, int y)
{
	char	*tmp;

	(void)win;
	(void)y;
	tmp = ft_asprint("%s %s", name, data);
	// TODO mlx_string_put(win->mlx, win->win, 10, y, 0x777777, tmp);
	free(tmp);
}
