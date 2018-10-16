/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_nodes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <njaber@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/14 09:20:31 by njaber            #+#    #+#             */
/*   Updated: 2018/10/16 08:08:37 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int				parse_config_node(t_scene *scene, t_node *onode, char **pos)
{
	t_node	node;

	init_hmap(&node.values);
	scene->start_pos = vec3(0, 0, 0);
	scene->start_rot = vec2(0, 0);
	scene->ambiant_light = 0.15;
	scene->sky_color = 0x000000;
	while (get_next_xml_node(&node, pos, 0) > 0)
	{
		if (node.type == 1 && ft_strcmp(node.name, onode->name) == 0)
			break ;
		else if (ft_strcmp(node.name, "start_pos") == 0)
			scene->start_pos = parse3f(pos);
		else if (ft_strcmp(node.name, "start_rotation") == 0)
			scene->start_rot = parse2f(pos);
		else if (ft_strcmp(node.name, "ambiant_light") == 0)
			scene->ambiant_light = parsef(pos);
		else if (ft_strcmp(node.name, "sky_color") == 0)
			scene->sky_color = parsecolor(pos);
		default_check_node(scene, &node, pos);
	}
	destroy_hmap(&node.values, free_and_null);
	if (ft_strcmp(node.name, onode->name) == 0)
		return (EXIT_SUCCESS);
	return (EXIT_FAILURE);
}

static int		parse_obj_node(t_scene *scene, t_node *onode, char **pos)
{
	t_node	node;
	t_obj	*obj;

	obj = default_obj(scene);
	init_hmap(&node.values);
	while (get_next_xml_node(&node, pos, 0) > 0)
	{
		if (node.type == 1 && ft_strcmp(node.name, onode->name) == 0)
			break ;
		else if (ft_strcmp(node.name, "type") == 0)
			obj->type = parsetype(pos);
		else if (ft_strcmp(node.name, "pos") == 0)
			obj->pos = parse3f(pos);
		else if (ft_strcmp(node.name, "rot") == 0)
			obj->rot = parse3f(pos);
		else if (ft_strcmp(node.name, "scale") == 0)
			obj->size = parse3f(pos);
		else if (ft_strcmp(node.name, "color") == 0)
			obj->color = parsecolor(pos);
		else if (ft_strcmp(node.name, "reflect") == 0)
			obj->reflect = parsef(pos);
		default_check_node(scene, &node, pos);
	}
	destroy_hmap(&node.values, free_and_null);
	if (ft_strcmp(node.name, onode->name) == 0)
		return (EXIT_SUCCESS);
	return (EXIT_FAILURE);
}

int				parse_objlist_node(t_scene *scene, t_node *onode, char **pos)
{
	t_node	node;

	scene->obj_pbufsize = 8;
	scene->nobjs = 0;
	if ((scene->objs = (t_obj*)ft_memalloc(sizeof(t_obj)
										* scene->obj_pbufsize)) == NULL)
		ft_error("Malloc error\n");
	init_hmap(&node.values);
	while (get_next_xml_node(&node, pos, 0) > 0)
	{
		if (node.type == 1 && ft_strcmp(node.name, onode->name) == 0)
			break ;
		else if (ft_strcmp(node.name, "obj") == 0)
			parse_obj_node(scene, &node, pos);
		else
			default_check_node(scene, &node, pos);
	}
	destroy_hmap(&node.values, free_and_null);
	if (ft_strcmp(node.name, onode->name) == 0)
		return (EXIT_SUCCESS);
	return (EXIT_FAILURE);
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
		else if (ft_strcmp(node.name, "lum") == 0)
			spot->lum = parsef(pos);
		else if (ft_strcmp(node.name, "pos") == 0)
			spot->pos = parse3f(pos);
		default_check_node(scene, &node, pos);
	}
	destroy_hmap(&node.values, free_and_null);
	if (ft_strcmp(node.name, onode->name) == 0)
		return (EXIT_SUCCESS);
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
