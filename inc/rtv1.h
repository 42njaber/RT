/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rtv1.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/20 15:19:13 by njaber            #+#    #+#             */
/*   Updated: 2018/05/26 00:32:05 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RTV1_H
# define RTV1_H

# include "libgxns.h"

typedef struct	s_obj {
	int				type;
	t_vec3			pos;
	t_vec3			rot;
	double			size;
	unsigned int	color;
	t_mat4			transform;
	t_mat4			rot_mat;
	t_mat4			rev_rot;
}				t_obj;

typedef struct	s_spot {
	t_vec3			pos;
	double			lum;
}				t_spot;

typedef struct	s_ptr {
	t_win	*win;
	void	*mlx;
	char	keys[512];
	double	fov;
	double	near;
	double	far;
	int		nobjs;
	int		nspots;
	t_vec3	cam_pos;
	t_vec3	cam_rot;
	t_mat4	cam_mat;
	t_mat4	cam_mat_rot;
	t_obj	*objs;
	t_spot	*spots;
	t_img	*scene;
	char	brillance;
	char	shadows;
}				t_ptr;

void			init_struct(t_ptr *p);
void			set_hooks(t_ptr *p);
void			parse_configs(t_ptr *p, int fd);
void			parse_objs(t_ptr *p, int fd);
void			set_default(t_ptr *p, char *def);

void			draw_scene(t_ptr *p);

int				is_hit(t_obj *obj, t_vec3 origin, t_vec3 dir, double *t);
t_vec3			get_normal(t_obj *obj, t_vec3 v);

#endif
