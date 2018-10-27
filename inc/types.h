/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   types.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <njaber@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/14 01:31:08 by njaber            #+#    #+#             */
/*   Updated: 2018/10/27 15:35:54 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPES_H
# define TYPES_H
# include <OpenCL/opencl.h>

typedef float			t_scal;

typedef unsigned int	t_uint;
typedef unsigned char	t_uchar;
typedef unsigned short	t_ushort;
typedef unsigned int	t_color;
typedef cl_mem			t_climg;

typedef struct			s_vec2 {
	t_scal	v[2];
}						t_vec2;

typedef struct			s_vec3 {
	t_scal	v[3];
}						t_vec3;

typedef struct			s_vec4 {
	t_scal	v[4];
}						t_vec4;

typedef struct			s_mat2 {
	t_scal	m[4];
}						t_mat2;

typedef struct			s_ivec {
	int		v[2];
}						t_ivec;

typedef t_scal			t_mat4[16];

#endif
