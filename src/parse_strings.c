/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_strings.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <njaber@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/27 02:51:10 by njaber            #+#    #+#             */
/*   Updated: 2018/10/27 02:51:22 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int			parsetype(char **pos)
{
	int		i;
	char	*type;
	int		ret;

	while (ft_isinvis(**pos))
		(*pos)++;
	i = 0;
	while ((*pos)[i] != '<' && (*pos)[i] != '\0')
		i++;
	while (ft_isinvis((*pos)[i] - 1) && i > 1)
		i--;
	if ((type = ft_strldup(*pos, i)) == NULL)
		ft_error("Malloc error\n");
	ret = UNKOWN;
	if (ft_strcmp(type, "sphere") == 0)
		ret = SPHERE;
	else if (ft_strcmp(type, "plane") == 0)
		ret = PLANE;
	else if (ft_strcmp(type, "cylinder") == 0)
		ret = CYLINDER;
	else if (ft_strcmp(type, "cone") == 0)
		ret = CONE;
	else if (ft_strcmp(type, "torus") == 0)
		ret = TORUS;
	else if (ft_strcmp(type, "moebius") == 0)
		ret = MOEBIUS;
	free(type);
	return (ret);
}

char		*parsestr(char **pos)
{
	int		i;
	char	*ret;

	while (ft_isinvis(**pos))
		(*pos)++;
	i = 0;
	while ((*pos)[i] != '<' && (*pos)[i] != '\0')
		i++;
	while (ft_isinvis((*pos)[i] - 1) && i > 1)
		i--;
	if ((ret = ft_strldup(*pos, i)) == NULL)
		ft_error("Malloc error\n");
	return (ret);
}
