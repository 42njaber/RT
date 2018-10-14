/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <njaber@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/14 01:24:16 by njaber            #+#    #+#             */
/*   Updated: 2018/10/14 12:10:07 by njaber           ###   ########.fr       */
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
	int		__attribute__((aligned(4)))			type;
	t_vec3	__attribute__((aligned(16)))		pos;
	t_vec3	__attribute__((aligned(16)))		rot;
	t_vec3	__attribute__((aligned(16)))		size;
	t_color	__attribute__((aligned(16)))		color;
	float	__attribute__((aligned(4)))			reflect;
	t_mat4	__attribute__((aligned(64)))		transform;
	t_mat4	__attribute__((aligned(64)))		rot_mat;
	t_mat4	__attribute__((aligned(64)))		rev_rot;
} __attribute__((packed))						t_obj;

/*
** A structure defining a spot in the scene
**
** pos: position of the object
** lum: the amount of light emitted by the spot
*/

typedef struct __attribute__((aligned(32)))	s_spot {
	t_vec3	__attribute__((aligned(16)))	pos;
	float	__attribute__((aligned(8)))		lum;
} __attribute__((packed))						t_spot;

#endif
