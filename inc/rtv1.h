/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rtv1.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/20 15:19:13 by njaber            #+#    #+#             */
/*   Updated: 2018/05/16 23:29:16 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRACTOL_H
# define FRACTOL_H

# include "libgxns.h"

typedef struct	s_obj {
	int				type;
	t_vec3			pos;
	t_vec3			rot;
	double			size;
	unsigned int	color;
	t_mat4			transform;
}				t_obj;

typedef struct	s_spot {
	t_vec3			pos;
	double			lum;
}				t_spot;

typedef struct	s_ptr {
	t_win	*win;
	void	*mlx;
	char	keys[512];
	int		button;
	t_ivec	mouse_pos;
	double	fov;
	double	near;
	double	far;
	int		nobjs;
	t_obj	*objs;
	t_spot	spot;
	t_img	*scene;
}				t_ptr;

void			init_struct(t_ptr *p);
void			set_hooks(t_ptr *p);

void			draw_scene(t_ptr *p);

int				is_hit(t_obj *obj, t_vec3 v);
t_vec3			get_normal(t_obj *obj, t_vec3 v);

int				loop_hook(void *parm);
int				key_press_hook(int key_code, void *parm);
int				key_release_hook(int key_code, void *parm);

int				button_press_hook(int button, int x, int y, void *parms);
int				button_release_hook(int button, int x, int y, void *parms);
int				motion_hook(int x, int y, void *parm);

#endif
