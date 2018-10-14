/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <njaber@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/14 10:26:26 by njaber            #+#    #+#             */
/*   Updated: 2018/10/14 12:16:22 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

t_vec3		parse3f(char **pos)
{
	t_vec3	ret;
	int		i;

	i = 0;
	ret.v[0] = ft_parse_float(*pos, &i);
	ret.v[1] = ft_parse_float(*pos, &i);
	ret.v[2] = ft_parse_float(*pos, &i);
	return (ret);
}

t_vec2		parse2f(char **pos)
{
	t_vec2	ret;
	int		i;

	i = 0;
	ret.v[0] = ft_parse_float(*pos, &i);
	ret.v[1] = ft_parse_float(*pos, &i);
	return (ret);
}

t_scal		parsef(char **pos)
{
	t_scal	ret;
	int		i;

	i = 0;
	ret = (t_scal)ft_parse_float(*pos, &i);
	return (ret);
}

t_color		parsecolor(char **pos)
{
	t_color	ret;
	int		i;

	i = 0;
	ret = (t_color)ft_parse_hex(*pos, &i);
	return (ret);
}

int			parsetype(char **pos)
{
	int		i;
	char	*type;

	while (ft_isinvis(**pos))
		(*pos)++;
	i = 0;
	while ((*pos)[i] != '<' && (*pos)[i] != '\0')
		i++;
	while (ft_isinvis((*pos)[i] - 1) && i > 1)
		i--;
	if ((type = ft_strldup(*pos, i)) == NULL)
		ft_error("Malloc error\n");
	if (ft_strcmp(type, "sphere") == 0)
		return (SPHERE);
	else if (ft_strcmp(type, "plane") == 0)
		return (PLANE);
	else if (ft_strcmp(type, "cylinder") == 0)
		return (CYLINDER);
	else if (ft_strcmp(type, "cone") == 0)
		return (CONE);
	else if (ft_strcmp(type, "torus") == 0)
		return (TORUS);
	else if (ft_strcmp(type, "moebius") == 0)
		return (MOEBIUS);
	ft_printf("Warning: unkown object \"%s\"\n", type);
	return (UNKOWN);
}
