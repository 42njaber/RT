/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_numbers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <njaber@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/14 10:26:26 by njaber            #+#    #+#             */
/*   Updated: 2018/10/27 02:51:21 by njaber           ###   ########.fr       */
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
