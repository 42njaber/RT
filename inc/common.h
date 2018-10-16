/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <njaber@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/14 01:24:16 by njaber            #+#    #+#             */
/*   Updated: 2018/10/16 15:52:43 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMON_H
# define COMMON_H
# ifndef __OPENCL_VERSION__
#  include "types.h"
# endif

enum		e_shape
{
	SPHERE,
	PLANE,
	CYLINDER,
	CONE,
	TORUS,
	MOEBIUS,
	UNKOWN
};

enum		e_guistate
{
	MENU,
	SCENE,
	INIT_SCENE,
	ZOOM_SCENE,
	UNZOOM_SCENE,
	QUIT_SCENE
};

/*
** A structure defining an object in a scene
**
** type: the shape of the object
** pos: position of the object
** rot: rotation of the object subsquently around the x, y, and z axis
** size: scaling of the object along the 3 different axis
** color: RBG color of the object
** transform: matrix that transforms a point into the object's coordinate
**            system
** rot_mat: matrix that transforms a vector into the object's coordinate
**          system
** rev_rot: matrix that transforms a vector back into the mains coordinate
**          system
*/

typedef struct __attribute__((aligned(512)))	s_obj {
	int		__attribute__((aligned(8)))			type;
	t_vec3	__attribute__((aligned(16)))		pos;
	t_vec3	__attribute__((aligned(16)))		rot;
	t_vec3	__attribute__((aligned(16)))		size;
	t_color	__attribute__((aligned(16)))		color;
	float	__attribute__((aligned(8)))			reflect;
	t_mat4	__attribute__((aligned(64)))		transform;
	t_mat4	__attribute__((aligned(64)))		rot_mat;
	t_mat4	__attribute__((aligned(64)))		rev_norm;
} __attribute__((packed))						t_obj;

/*
** A structure defining a spot in the scene
**
** pos: position of the object
** lum: the amount of light emitted by the spot
*/

typedef struct __attribute__((aligned(32)))		s_spot {
	t_vec3	__attribute__((aligned(16)))	pos;
	float	__attribute__((aligned(8)))		lum;
} __attribute__((packed))						t_spot;

typedef struct __attribute__((aligned(64)))	s_gui {
	int		__attribute__((aligned(8)))		state;
	int		__attribute__((aligned(8)))		scene_id;
	t_ivec	__attribute__((aligned(16)))	mouse_pos;
	t_climg	__attribute__((aligned(8)))		thumbnails;
	float	__attribute__((aligned(8)))		zoom;
} __attribute__((packed))						t_gui;

typedef struct __attribute__((aligned(256)))	s_set
{
	t_mat4	 __attribute__((aligned(64)))	cam_mat;
	t_mat4	 __attribute__((aligned(64)))	cam_mat_rot;
	t_color	 __attribute__((aligned(16)))	sky_color;
	int		 __attribute__((aligned(8)))	max_rays;
	t_scal	 __attribute__((aligned(8)))	fov;
	t_scal	 __attribute__((aligned(8)))	ambiant_light;
	int		 __attribute__((aligned(8)))	progressive;
	int		 __attribute__((aligned(8)))	iter;
	t_ivec	 __attribute__((aligned(16)))	iter_pos;
}												t_set;

#endif
