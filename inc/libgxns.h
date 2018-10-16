/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libgxns.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/25 23:56:43 by njaber            #+#    #+#             */
/*   Updated: 2018/10/16 07:17:20 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBGXNS_H
# define LIBGXNS_H

# include <time.h>
# include <math.h>
# include "ft_printf.h"
# include "libft.h"
# include "types.h"

#  include "qwerty.h"

# include <OpenCL/opencl.h>

typedef struct			s_hmap {
	void	**elements;
	char	**keys;
	size_t	prebuf_size;
	size_t	elem_count;
}						t_hmap;

typedef struct	s_ocl {
	cl_context			gpu_context;
	size_t				gpu_nbr;
	cl_device_id		gpus[16];
	cl_command_queue	gpu_command_queue;
	size_t				gpu_wg_sz;
	size_t				gpu_double;
}				t_ocl;

typedef struct	s_kernel {
	cl_program		program;
	cl_mem			memobjs[16];
	size_t			memobjs_nbr;
	t_hmap			cores;
	t_ocl			*opencl;
}				t_kernel;

typedef struct	s_img {
	void			*link;
	unsigned char	*buf;
	t_ivec			size;
	int				px_size;
	int				line;
	int				endian;
	int				line_draw_mode;
}				t_img;

typedef struct	s_win {
	void				*mlx;
	void				*win;
	t_img				img;
	t_ivec				size;
	int					frame;
	float				fps;
	t_scal				frame_elapsed;
	unsigned long		frames[30];
}				t_win;

void			destroy_hmap(t_hmap *hmap, void (*del)(void**));
void			del_helem(t_hmap *hmap, const char *key, void (*del)(void**));
void			reset_hmap(t_hmap *hmap, void (*del)(void**));
void			init_hmap(t_hmap *hmap);
void			*get_helem(t_hmap *hmap, char *key);
void			add_helem(t_hmap *hmap, char *key, void *data);

t_vec3			apply_mat_vec3(t_vec3 v, t_mat4 m);
void			print_mat(t_mat4 m);

void			identity(t_mat4 m);
void			translate(t_mat4 m, t_vec3 v);
void			scale(t_mat4 m, t_vec3 v);
void			rotate(t_mat4 m, t_vec3 v);
void			multiply(t_mat4 m1, t_mat4 m2, int stock_in_m2);

float			sqr(float n);

t_vec3			vec3(t_scal x, t_scal y, t_scal z);
t_vec2			vec2(t_scal x, t_scal y);
t_ivec			ivec(int x, int y);
t_scal			sqr(t_scal n);
t_scal			vec2_angle(t_vec2 v);
t_scal			vec2_square_distance(t_vec2 v1, t_vec2 v2);
t_vec3			vec2_line(t_vec2 a, t_vec2 b);
t_scal			vec2_dot_line(t_vec3 l, t_vec2 v);

t_mat2			m2identity(void);
t_mat2			m2transpose(t_mat2 m);
t_mat2			m2rotation(t_scal angle);
t_mat2			m2dot(t_mat2 m1, t_mat2 m2);
t_vec2			m2dotv2(t_mat2 m, t_vec2 v);

t_vec3			vec3_sub(t_vec3 v1, t_vec3 v2);
t_vec3			vec3_add(t_vec3 v1, t_vec3 v2);
t_vec3			vec3_mult(t_vec3 v1, t_scal d);
t_scal			vec3_length(t_vec3 v1);
t_scal			vec3_scalar(t_vec3 v1, t_vec3 v2);

t_vec2			vec2_sub(t_vec2 v1, t_vec2 v2);
t_vec2			vec2_add(t_vec2 v1, t_vec2 v2);
t_vec2			vec2_mult(t_vec2 v1, t_scal d);
t_scal			vec2_length(t_vec2 v1);
t_scal			vec2_scalar(t_vec2 v1, t_vec2 v2);

int				init_new_win(void *mlx, t_win *win, t_ivec size, char *title);
void			paint_window(t_win *win, t_kernel *opencl_kernel, int clear);

void			display_data_scal(t_win *win, char *name, t_scal data, int y);
void			display_data_vec2(t_win *win, char *name, t_vec2 data, int y);
void			display_data_vec3(t_win *win, char *name, t_vec3 data, int y);
void			display_data_str(t_win *win, char *name, char *data, int y);

void			init_new_image(void *mlx, t_img *img, t_ivec size);
void			img_px(t_img *img, unsigned int color, t_ivec pixel);
void			clear_img(t_img *img);

t_ocl			*init_opencl(void);
cl_program		create_program_from_file(cl_context context, const char *file);

void			draw_line(t_img *img, unsigned int *color, t_vec2 *verticies);
unsigned int	color_gradiant(unsigned int *colors, float gradiant);

int				get_key_digit(int key_code);
void			free_and_null(void **mem);
void			free_content(void *mem, size_t len);
void			*combine_list(t_list *lst);

#endif
