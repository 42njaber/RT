/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/26 00:14:34 by njaber            #+#    #+#             */
/*   Updated: 2018/06/01 01:15:26 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libgxns.h"
#include "mlx.h"

#ifdef OPENCL

void	paint_window(t_win *win, t_kernel *opencl_kernel)
{
	uint64_t	time;

	(void)opencl_kernel;
	time = clock_gettime_nsec_np(CLOCK_UPTIME_RAW);
	win->frame++;
	if (win->frame % 5 == 0)
		win->fps = (float)30000000000 / (time - win->frames[win->frame  % 30]);
	win->frames[win->frame % 30] = time;
	mlx_clear_window(win->mlx, win->win);
	mlx_put_image_to_window(win->mlx, win->win, win->img.link, 0, 0);
}

#else

void	paint_window(t_win *win, t_kernel *opencl_kernel)
{
	uint64_t	time;

	(void)opencl_kernel;
	time = clock_gettime_nsec_np(CLOCK_UPTIME_RAW);
	win->frame++;
	if (win->frame % 5 == 0)
		win->fps = (float)30000000000 / (time - win->frames[win->frame % 30]);
	win->frames[win->frame % 30] = time;
	win->frame++;
	mlx_clear_window(win->mlx, win->win);
	mlx_put_image_to_window(win->mlx, win->win, win->img.link, 0, 0);
}

#endif

int		init_new_win(void *mlx, t_win *win, t_ivec size, char *title)
{
	if (mlx != NULL)
		win->mlx = mlx;
	else if ((win->mlx = mlx_init()) == NULL)
		return (0);
	if ((win->win = mlx_new_window(win->mlx, size.x, size.y, title)) == NULL)
		return (0);
	if (win->img.link == NULL)
		init_new_image(win->mlx, &win->img, size);
	win->size = size;
	win->frame = 0;
	win->fps = 0;
	ft_bzero(win->frames, 30 * sizeof(uint64_t));
	return (1);
}
