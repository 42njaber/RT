/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_objs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/25 19:22:52 by njaber            #+#    #+#             */
/*   Updated: 2018/05/26 00:20:44 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"
#include "gnl.h"

static void		parse_single_obj2(t_ptr *p, int fd, int j)
{
	int		i;
	char	*line;

	i = 0;
	if (get_next_line(fd, &line) != 1)
		ft_error("[Error] Error in the camera parameters\n");
	p->objs[j].rot.x = ft_parse_float(line, &i);
	p->objs[j].rot.y = ft_parse_float(line, &i);
	p->objs[j].rot.z = ft_parse_float(line, &i);
	free(line);
	i = 0;
	if (get_next_line(fd, &line) != 1)
		ft_error("[Error] Error in the camera parameters\n");
	p->objs[j].color = ft_parse_hex(line, &i);
	free(line);
	i = 0;
	if (get_next_line(fd, &line) != 1)
		ft_error("[Error] Error in the camera parameters\n");
	p->objs[j].size = ft_parse_float(line, &i);
	free(line);
}

static void		parse_single_obj1(t_ptr *p, int fd, int j)
{
	int		i;
	char	*line;

	if (get_next_line(fd, &line) != 1)
		ft_error("[Error] Error in the objects parameters\n");
	if (ft_strncmp(line, "PLANE", 5) == 0)
		p->objs[j].type = 1;
	else if (ft_strncmp(line, "CYLINDER", 4) == 0)
		p->objs[j].type = 2;
	else if (ft_strncmp(line, "CONE", 8) == 0)
		p->objs[j].type = 3;
	else
		p->objs[j].type = 0;
	free(line);
	i = 0;
	if (get_next_line(fd, &line) != 1)
		ft_error("[Error] Error in the camera parameters\n");
	p->objs[j].pos.x = ft_parse_float(line, &i);
	p->objs[j].pos.y = ft_parse_float(line, &i);
	p->objs[j].pos.z = ft_parse_float(line, &i);
	free(line);
	parse_single_obj2(p, fd, j);
}

void			parse_objs(t_ptr *p, int fd)
{
	char	*line;
	int		i;
	int		j;

	i = 0;
	if (get_next_line(fd, &line) != 1)
		ft_error("[Error] Error in the objects parameters\n");
	p->nobjs = ft_parse_int(line, &i);
	free(line);
	if ((p->objs = ft_memalloc(sizeof(t_obj) * p->nobjs)) == NULL)
		ft_error("[Erreur] Failed to allocate memory\n");
	j = -1;
	while (++j < p->nobjs)
		parse_single_obj1(p, fd, j);
}
