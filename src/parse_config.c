/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_nodes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <njaber@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/14 09:20:31 by njaber            #+#    #+#             */
/*   Updated: 2018/10/27 00:44:32 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int				parse_config_node(t_scene *scene, t_node *onode, char **pos)
{
	t_node	node;

	init_hmap(&node.values);
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
		if (default_check_node(scene, &node, pos) == EXIT_FAILURE)
			break ;
	}
	destroy_hmap(&node.values, free_and_null);
	if (ft_strcmp(node.name, onode->name) == 0)
		return (EXIT_SUCCESS);
	return (EXIT_FAILURE);
}
