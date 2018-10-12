/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   images.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/19 10:10:08 by njaber            #+#    #+#             */
/*   Updated: 2018/10/12 01:18:35 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libgxns.h"
#include "mlx.h"

void	clear_img(t_img *img)
{
	int		i;

	i = 0;
	while (i < img->line * img->size.v[1])
		img->buf[i++] = 0;
}

void	img_px(t_img *img, unsigned int color, t_ivec pixel)
{
	int		px_bytes;

	px_bytes = (img->px_size + 7) / 8;
	if (pixel.v[0] < 0 || pixel.v[1] < 0 ||
			pixel.v[0] >= img->size.v[0] || pixel.v[1] >= img->size.v[1])
		return ;
	img->buf[pixel.v[0] * px_bytes + 0 + pixel.v[1] * img->line] =
		color & 0xFF;
	img->buf[pixel.v[0] * px_bytes + 1 + pixel.v[1] * img->line] =
		(color >> 8) & 0xFF;
	img->buf[pixel.v[0] * px_bytes + 2 + pixel.v[1] * img->line] =
		(color >> 16) & 0xFF;
	img->buf[pixel.v[0] * px_bytes + 3 + pixel.v[1] * img->line] =
		(color >> 24);
}

void	init_new_image(void *mlx, t_img *img, t_ivec size)
{
	img->link = mlx_new_image(mlx, size.v[0], size.v[1]);
	img->size = size;
	img->buf = (unsigned char*)mlx_get_data_addr(img->link, &(img->px_size),
			&(img->line), &(img->endian));
	img->line_draw_mode = 1;
}
