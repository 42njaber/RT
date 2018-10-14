/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_nodes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <njaber@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/14 09:20:31 by njaber            #+#    #+#             */
/*   Updated: 2018/10/14 11:46:11 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int				parse_config_node(t_ptr *p, t_node *onode, char **pos)
{
	t_node	node;

	init_hmap(&node.values);
	p->pos = vec3(0, 0, 0);
	p->rot = vec2(0, 0);
	p->ambiant_light = 0.15;
	while (get_next_xml_node(&node, pos, 0) > 0)
	{
		if (node.type == 1 && ft_strcmp(node.name, onode->name) == 0)
			break ;
		else if (ft_strcmp(node.name, "start_pos") == 0)
			p->pos = parse3f(pos);
		else if (ft_strcmp(node.name, "start_rotation") == 0)
			p->rot = parse2f(pos);
		else if (ft_strcmp(node.name, "ambiant_light") == 0)
			p->ambiant_light = parsef(pos);
		default_check_node(p, &node, pos);
	}
	destroy_hmap(&node.values, free_and_null);
	if (ft_strcmp(node.name, onode->name) == 0)
		return (EXIT_SUCCESS);
	return (EXIT_FAILURE);
}

static int		parse_obj_node(t_ptr *p, t_node *onode, char **pos)
{
	t_node	node;
	t_obj	*obj;

	obj = default_obj(p);
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
		default_check_node(p, &node, pos);
	}
	destroy_hmap(&node.values, free_and_null);
	if (ft_strcmp(node.name, onode->name) == 0)
		return (EXIT_SUCCESS);
	return (EXIT_FAILURE);
}

int				parse_objlist_node(t_ptr *p, t_node *onode, char **pos)
{
	t_node	node;

	p->obj_pbufsize = 8;
	p->nobjs = 0;
	if ((p->objs = (t_obj*)ft_memalloc(sizeof(t_obj)
										* p->obj_pbufsize)) == NULL)
		ft_error("Malloc error\n");
	init_hmap(&node.values);
	while (get_next_xml_node(&node, pos, 0) > 0)
	{
		if (node.type == 1 && ft_strcmp(node.name, onode->name) == 0)
			break ;
		else if (ft_strcmp(node.name, "obj") == 0)
			parse_obj_node(p, &node, pos);
		else
			default_check_node(p, &node, pos);
	}
	destroy_hmap(&node.values, free_and_null);
	if (ft_strcmp(node.name, onode->name) == 0)
		return (EXIT_SUCCESS);
	return (EXIT_FAILURE);
}

static int		parse_spot_node(t_ptr *p, t_node *onode, char **pos)
{
	t_node	node;
	t_spot	*spot;

	spot = default_spot(p);
	init_hmap(&node.values);
	while (get_next_xml_node(&node, pos, 0) > 0)
	{
		if (node.type == 1 && ft_strcmp(node.name, onode->name) == 0)
			break ;
		else if (ft_strcmp(node.name, "lum") == 0)
			spot->lum = parsef(pos);
		else if (ft_strcmp(node.name, "pos") == 0)
			spot->pos = parse3f(pos);
		default_check_node(p, &node, pos);
	}
	destroy_hmap(&node.values, free_and_null);
	if (ft_strcmp(node.name, onode->name) == 0)
		return (EXIT_SUCCESS);
	return (EXIT_FAILURE);
}

int				parse_spotlist_node(t_ptr *p, t_node *onode, char **pos)
{
	t_node	node;

	p->spot_pbufsize = 8;
	p->nspots = 0;
	if ((p->spots = (t_spot*)ft_memalloc(sizeof(t_spot)
										* p->spot_pbufsize)) == NULL)
		ft_error("Malloc error\n");
	init_hmap(&node.values);
	while (get_next_xml_node(&node, pos, 0) > 0)
	{
		if (node.type == 1 && ft_strcmp(node.name, onode->name) == 0)
			break ;
		else if (ft_strcmp(node.name, "spot") == 0)
			parse_spot_node(p, &node, pos);
		else
			default_check_node(p, &node, pos);
	}
	destroy_hmap(&node.values, free_and_null);
	if (ft_strcmp(node.name, onode->name) == 0)
		return (EXIT_SUCCESS);
	return (EXIT_FAILURE);
}
