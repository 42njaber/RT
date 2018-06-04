/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/20 15:19:13 by njaber            #+#    #+#             */
/*   Updated: 2018/06/04 23:42:13 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RT_H
# define RT_H

# include "libgxns.h"

enum e_shape
{
	SPHERE,
	PLANE,
	CYLINDER,
	CONE
};

/*
** A structure defining an object in a scene
**
** type: the shape of the object
** pos: position of the object
** rot: rotation of the object subsquently around the x, y, and z axis
** size: size of the object, can mean deiffrent things depending on the shape
** color: RBG color of the object
** transform: matrix that transforms a point into the object's coordinate
**            system
** rot_mat: matrix that transforms a vector into the object's coordinate
**          system
** rev_rot: matrix that transforms a vector back into the mains coordinate
**          system
*/

typedef struct	s_obj {
	int				type;
	t_vec3			pos;
	t_vec3			rot;
	t_vec3			size;
	unsigned int	color;
	float			reflect;
	t_mat4			transform;
	t_mat4			rot_mat;
	t_mat4			rev_rot;
}				t_obj;

/*
** A structure defining a spot in the scene
**
** pos: position of the object
** lum: the amount of light emitted by the spot
*/

typedef struct	s_spot {
	t_vec3			pos;
	float			lum;
}				t_spot;

#ifdef OPENCL

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
	int			timer;
	float		fov;
	float		near;
	float		far;
	int			res;
	int			nobjs;
	int			nspots;
	t_vec3		pos;
	t_vec3		origin_rot;
	t_vec3		rot;
	t_mat4		cam_mat;
	t_mat4		cam_mat_rot;
	t_obj		*objs;
	t_spot		*spots;
	char		brilliance;
	char		shadows;
	int			tmp;
}				t_ptr;

void			process_image_opencl(t_ptr *p);

void			create_spot_memobjs(t_ptr *p);
void			create_obj_memobjs(t_ptr *p);
t_kernel		*create_kernel(t_ptr *p);

#else

typedef struct	s_ptr {
	t_win		*win;
	void		*mlx;
	char		keys[512];
	int			button;
	t_ivec		mouse_pos;
	t_ivec		tmp_mouse_pos;
	int			update;
	int			timer;
	float		fov;
	float		near;
	float		far;
	int			res;
	int			nobjs;
	int			nspots;
	t_vec3		pos;
	t_vec3		rot;
	t_vec3		origin_rot;
	t_mat4		cam_mat;
	t_mat4		cam_mat_rot;
	t_obj		*objs;
	t_spot		*spots;
	char		brilliance;
	char		shadows;
}				t_ptr;

#endif

void			init_struct(t_ptr *p);
void			set_hooks(t_ptr *p);
void			parse_scene_file(t_ptr *p, int fd);
void			parse_objs(t_ptr *p, int fd);
void			set_default_scene(t_ptr *p, char *def);

int				loop_hook(void *parm);

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
