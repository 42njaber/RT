/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_objects.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <njaber@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/26 23:40:18 by njaber            #+#    #+#             */
/*   Updated: 2018/10/27 15:26:03 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void		read_param(t_obj *obj, t_node *node, char **pos)
{
	if (ft_strcmp(node->name, "type") == 0)
		obj->type = parsetype(pos);
	else if (ft_strcmp(node->name, "pos") == 0)
		obj->pos = parse3f(pos);
	else if (ft_strcmp(node->name, "rot") == 0)
		obj->rot = parse3f(pos);
	else if (ft_strcmp(node->name, "scale") == 0)
		obj->size = parse3f(pos);
	else if (ft_strcmp(node->name, "color") == 0)
		obj->color = parsecolor(pos);
	else if (ft_strcmp(node->name, "reflect") == 0)
		obj->reflect = parsef(pos);
	else if (ft_strcmp(node->name, "transparency") == 0)
		obj->transparency = parsef(pos);
	else if (ft_strcmp(node->name, "refractive_index") == 0)
		obj->ref_index = parsef(pos);
	else if (ft_strcmp(node->name, "brillance") == 0)
		obj->brillance = (int)parsef(pos);
	else if (ft_strcmp(node->name, "texture") == 0)
		if ((obj->texture = parsestr(pos)) == NULL)
			ft_error("Malloc error\n");
}

static int		assert_obj(t_obj *obj)
{
	if (obj->type == UNKOWN)
		return (EXIT_FAILURE);
	if (obj->size.v[0] <= 0 || obj->size.v[1] <= 0 || obj->size.v[2] <= 0)
		return (EXIT_FAILURE);
	if (obj->reflect < 0 || obj->reflect > 1)
		return (EXIT_FAILURE);
	if (obj->transparency < 0 || obj->transparency > 1)
		return (EXIT_FAILURE);
	if (obj->ref_index <= 0)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

static void		free_obj(t_obj *obj)
{
	if (obj->texture != NULL)
		free_and_null((void**)&obj->texture);
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
		read_param(obj, &node, pos);
		if (default_check_node(scene, &node, pos) == EXIT_FAILURE)
			break ;
	}
	destroy_hmap(&node.values, free_and_null);
	if (ft_strcmp(node.name, onode->name) == 0 &&
			assert_obj(obj) == EXIT_SUCCESS)
		return (EXIT_SUCCESS);
	ft_printf("%<#FFAA00>[Warning]%<0> Invalide object\n");
	free_obj(obj);
	scene->nobjs--;
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
