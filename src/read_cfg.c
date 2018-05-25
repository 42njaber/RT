/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_cfg.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/25 11:20:41 by njaber            #+#    #+#             */
/*   Updated: 2018/05/26 00:30:41 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"
#include "gnl.h"
#include "libft.h"

static void		parse_cam(t_ptr *p, int fd)
{
	char	*line;
	int		i;

	if (get_next_line(fd, &line) != 1)
		ft_error("[Error] Error in the camera parameters\n");
	i = 0;
	p->cam_pos.x = ft_parse_float(line, &i);
	p->cam_pos.y = ft_parse_float(line, &i);
	p->cam_pos.z = ft_parse_float(line, &i);
	free(line);
	if (get_next_line(fd, &line) != 1)
		ft_error("[Error] Error in the camera parameters\n");
	i = 0;
	p->cam_rot.x = ft_parse_float(line, &i);
	p->cam_rot.y = ft_parse_float(line, &i);
	p->cam_rot.z = ft_parse_float(line, &i);
	free(line);
}

static void		parse_spots(t_ptr *p, int fd)
{
	char	*line;
	int		i;
	int		j;

	i = 0;
	if (get_next_line(fd, &line) != 1)
		ft_error("[Error] Error in the spots parameters\n");
	p->nspots = ft_parse_int(line, &i);
	free(line);
	if ((p->spots = ft_memalloc(sizeof(t_spot) * p->nspots)) == NULL)
		ft_error("[Erreur] Failed to allocate memory\n");
	j = -1;
	while (++j < p->nspots)
	{
		if (((i = 0) == 0) && get_next_line(fd, &line) != 1)
			ft_error("[Error] Error in the camera parameters\n");
		p->spots[j].lum = ft_parse_float(line, &i);
		free(line);
		if (((i = 0) == 0) && get_next_line(fd, &line) != 1)
			ft_error("[Error] Error in the camera parameters\n");
		p->spots[j].pos.x = ft_parse_float(line, &i);
		p->spots[j].pos.y = ft_parse_float(line, &i);
		p->spots[j].pos.z = ft_parse_float(line, &i);
		free(line);
	}
}

static void		choose_option(t_ptr *p, char *def, int fd, char *line)
{
	if (ft_strncmp(line, "CAM", 3) == 0)
	{
		if (def[0] == 0)
			ft_error("[Error] This config file tries to define "
					"the camera more then once\n");
		def[0] = 0;
		parse_cam(p, fd);
	}
	if (ft_strncmp(line, "OBJS", 4) == 0)
	{
		if (def[1] == 0)
			ft_error("[Error] This config file tries to define "
					"the objects more then once\n");
		def[1] = 0;
		parse_objs(p, fd);
	}
	if (ft_strncmp(line, "SPOTS", 5) == 0)
	{
		if (def[2] == 0)
			ft_error("[Error] This config file tries to define "
					"the spots more then once\n");
		def[2] = 0;
		parse_spots(p, fd);
	}
}

void			parse_configs(t_ptr *p, int fd)
{
	char	*line;
	char	*def;
	int		ret;

	def = (char[3]){1, 1, 1};
	while ((def[0] == 1 || def[1] == 1 || def[2] == 1) &&
			(ret = get_next_line(fd, &line)) == 1)
	{
		choose_option(p, def, fd, line);
		free(line);
	}
	if (ret == -1)
		ft_error("[Error] Unable to read the config file\n");
	set_default(p, def);
}
