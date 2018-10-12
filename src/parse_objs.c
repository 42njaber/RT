/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_objs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/25 19:22:52 by njaber            #+#    #+#             */
/*   Updated: 2018/10/12 00:58:38 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"
#include "gnl.h"

static void		parse_single_obj2(t_ptr *p, int fd, int j)
{
	int		i;
	char	*line;

	line = NULL;
	if ((i = 0) == 0 && get_next_line(fd, &line) != 1)
		ft_error("[Error] Error in the camera parameters\n");
	p->objs[j].rot.v[0] = ft_parse_float(line, &i);
	p->objs[j].rot.v[1] = ft_parse_float(line, &i);
	p->objs[j].rot.v[2] = ft_parse_float(line, &i);
	free(line);
	if ((i = 0) == 0 && get_next_line(fd, &line) != 1)
		ft_error("[Error] Error in the camera parameters\n");
	p->objs[j].color = ft_parse_hex(line, &i);
	free(line);
	if ((i = 0) == 0 && get_next_line(fd, &line) != 1)
		ft_error("[Error] Error in the camera parameters\n");
	p->objs[j].size.v[0] = ft_parse_float(line, &i);
	p->objs[j].size.v[1] = ft_parse_float(line, &i);
	p->objs[j].size.v[2] = ft_parse_float(line, &i);
	free(line);
	if ((i = 0) == 0 && get_next_line(fd, &line) != 1)
		ft_error("[Error] Error in the camera parameters\n");
	p->objs[j].reflect = ft_parse_float(line, &i);
	free(line);
}

/*
** p: the program's main structure
** fd: the file descriptor of the file describing the scene
** j: the index of the object being read
**
** line: a buffer for reading lines
** i: a counter for the psotion in the line
**
** Reads the type of the object and its position, if the type of object is not
** recognised, default to a sphere
*/

static void		parse_single_obj1(t_ptr *p, int fd, int j)
{
	int		i;
	char	*line;

	if (get_next_line(fd, &line) != 1)
		ft_error("[Error] Error in the objects parameters\n");
	if (ft_strncmp(line, "PLANE", 5) == 0)
		p->objs[j].type = PLANE;
	else if (ft_strncmp(line, "CYLINDER", 8) == 0)
		p->objs[j].type = CYLINDER;
	else if (ft_strncmp(line, "CONE", 4) == 0)
		p->objs[j].type = CONE;
	else if (ft_strncmp(line, "TORUS", 5) == 0)
		p->objs[j].type = TORUS;
	else if (ft_strncmp(line, "MOEBIUS", 7) == 0)
		p->objs[j].type = MOEBIUS;
	else
		p->objs[j].type = SPHERE;
	free(line);
	i = 0;
	if (get_next_line(fd, &line) != 1)
		ft_error("[Error] Error in the camera parameters\n");
	p->objs[j].pos.v[0] = ft_parse_float(line, &i);
	p->objs[j].pos.v[1] = ft_parse_float(line, &i);
	p->objs[j].pos.v[2] = ft_parse_float(line, &i);
	free(line);
	parse_single_obj2(p, fd, j);
}

/*
** p: the program's main structure
** fd: the file descriptor of the file describing the scene
**
** line: a buffer for reading lines
** i: a counter for the psotion in the line and for the object being read
**
** Reads the number of objects scene, then reads each object in order
*/

void			parse_objs(t_ptr *p, int fd)
{
	char	*line;
	int		i;

	i = 0;
	if (get_next_line(fd, &line) != 1)
		ft_error("[Error] Error in the objects parameters\n");
	p->nobjs = ft_parse_int(line, &i);
	free(line);
	if ((p->objs = ft_memalloc(sizeof(t_obj) * p->nobjs)) == NULL)
		ft_error("[Erreur] Failed to allocate memory\n");
	i = -1;
	while (++i < p->nobjs)
		parse_single_obj1(p, fd, i);
}
