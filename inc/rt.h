/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/20 15:19:13 by njaber            #+#    #+#             */
/*   Updated: 2018/10/14 11:24:46 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RT_H
# define RT_H

# include "libgxns.h"
# include "common.h"

extern const char	*g_nul;

typedef struct			s_hmap {
	void	**elements;
	char	**keys;
	size_t	prebuf_size;
	size_t	elem_count;
}						t_hmap;

typedef struct			s_node {
	char	name[32];
	t_hmap	values;
	char	type;
}						t_node;

typedef struct	s_ptr {
	t_win		*win;
	void		*mlx;
	t_ocl		*opencl;
	t_kernel	*kernel;
	char		keys[512];
	int			button;
	t_ivec		mouse_pos;
	t_ivec		tmp_mouse_pos;
	int			update;
	float		fov;
	float		near;
	float		far;
	float		ambiant_light;
	int			res;
	int			nobjs;
	int			nspots;
	int			obj_pbufsize;
	int			spot_pbufsize;
	t_obj		*objs;
	t_spot		*spots;
	t_vec3		pos;
	t_vec2		origin_rot;
	t_vec2		rot;
	t_mat4		cam_mat;
	t_mat4		cam_mat_rot;
	char		brilliance;
	char		shadows;
	char		max_reflections;
}				t_ptr;

void			process_image_opencl(t_ptr *p);

void			create_spot_memobjs(t_ptr *p);
void			create_obj_memobjs(t_ptr *p);
t_kernel		*create_kernel(t_ptr *p);

int				parse_scene_file(t_ptr *p, int fd);
t_scal			parsef(char **pos);
t_vec2			parse2f(char **pos);
t_vec3			parse3f(char **pos);
t_color			parsecolor(char **pos);
int				parsetype(char **pos);

int				get_next_xml_node(t_node *tag, char **pos, char strict);
void			xml_set_read_buf(char *file, int len);

int				default_check_node(t_ptr *p, t_node *onode, char **pos);
int				parse_config_node(t_ptr *p, t_node *onode, char **pos);
int				parse_objlist_node(t_ptr *p, t_node *onode, char **pos);
int				parse_spotlist_node(t_ptr *p, t_node *onode, char **pos);

t_spot			*default_spot(t_ptr *p);
t_obj			*default_obj(t_ptr *p);

void			init_struct(t_ptr *p);
void			set_hooks(t_ptr *p);

int				loop_hook(void *parm);

void			destroy_hmap(t_hmap *hmap, void (*del)(void**));
void			reset_hmap(t_hmap *hmap, void (*del)(void**));
void			init_hmap(t_hmap *hmap);
void			*get_helem(t_hmap *hmap, char *key);
void			add_helem(t_hmap *hmap, char *key, void *data);

int				button_press_hook(int button, int x, int y, void *parms);
int				button_release_hook(int button, int x, int y, void *parms);
int				motion_hook(int x, int y, void *parm);
int				key_press_hook(int key_code, void *parm);
int				key_release_hook(int key_code, void *parm);
void			move(t_ptr *p);

void			process_image(t_ptr *p);

int				does_intersect(t_obj *obj, t_vec3 origin, t_vec3 dir, float *t);
t_vec3			get_normal(t_obj *obj, t_vec3 v);

void			generate_cam_matricies(t_ptr *p);
void			generate_obj_matricies(t_ptr *p);

#endif
