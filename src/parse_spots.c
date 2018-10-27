/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_spots.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <njaber@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/27 00:44:44 by njaber            #+#    #+#             */
/*   Updated: 2018/10/27 00:49:39 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void		read_param(t_spot *spot, t_node *node, char **pos)
{
	if (ft_strcmp(node->name, "lum") == 0)
		spot->lum = parsef(pos);
	else if (ft_strcmp(node->name, "pos") == 0)
		spot->pos = parse3f(pos);
	else if (ft_strcmp(node->name, "color") == 0)
		spot->color = parsecolor(pos);
}

static int		assert_spot(t_spot *spot)
{
	(void)spot;
	return (EXIT_SUCCESS);
}

static int		parse_spot_node(t_scene *scene, t_node *onode, char **pos)
{
	t_node	node;
	t_spot	*spot;

	spot = default_spot(scene);
	init_hmap(&node.values);
	while (get_next_xml_node(&node, pos, 0) > 0)
	{
		if (node.type == 1 && ft_strcmp(node.name, onode->name) == 0)
			break ;
		read_param(spot, &node, pos);
		if (default_check_node(scene, &node, pos) == EXIT_FAILURE)
			break ;
	}
	destroy_hmap(&node.values, free_and_null);
	if (ft_strcmp(node.name, onode->name) == 0 &&
			assert_spot(spot) == EXIT_SUCCESS)
		return (EXIT_SUCCESS);
	ft_printf("%<#FFAA00>[Warning]%<0> Invalide spot\n");
	scene->nspots--;
	return (EXIT_FAILURE);
}

int				parse_spotlist_node(t_scene *scene, t_node *onode, char **pos)
{
	t_node	node;

	scene->spot_pbufsize = 8;
	scene->nspots = 0;
	if ((scene->spots = (t_spot*)ft_memalloc(sizeof(t_spot)
										* scene->spot_pbufsize)) == NULL)
		ft_error("Malloc error\n");
	init_hmap(&node.values);
	while (get_next_xml_node(&node, pos, 0) > 0)
	{
		if (node.type == 1 && ft_strcmp(node.name, onode->name) == 0)
			break ;
		else if (ft_strcmp(node.name, "spot") == 0)
			parse_spot_node(scene, &node, pos);
		else
			default_check_node(scene, &node, pos);
	}
	destroy_hmap(&node.values, free_and_null);
	if (ft_strcmp(node.name, onode->name) == 0)
		return (EXIT_SUCCESS);
	return (EXIT_FAILURE);
}
