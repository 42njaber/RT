/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_scene.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/25 11:20:41 by njaber            #+#    #+#             */
/*   Updated: 2018/10/27 02:23:40 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"
#include "libft.h"

static int	read_all_file(int fd, char **stock)
{
	char	buf[4096];
	t_list	*list;
	t_list	**end;
	int		ret;

	end = &list;
	while ((ret = read(fd, buf, 4096)) > 0)
	{
		if ((*end = ft_lstnew(NULL, ret)) == NULL)
			ft_error("Malloc error\n");
		ft_memcpy((*end)->content, buf, ret);
		end = &(*end)->next;
	}
	if ((*end = ft_lstnew(NULL, 1)) == NULL)
		ft_error("Malloc error\n");
	if (ret < 0)
		ret = EXIT_FAILURE;
	else
	{
		ret = EXIT_SUCCESS;
		if ((*stock = (char*)combine_list(list)) == NULL)
			ft_error("Malloc error\n");
	}
	ft_lstdel(&list, free_content);
	return (ret);
}

int			default_check_node(t_scene *scene, t_node *onode, char **pos)
{
	t_node	node;

	if (onode->type == 1)
		return (EXIT_FAILURE);
	if (onode->type == 2)
		return (EXIT_SUCCESS);
	init_hmap(&node.values);
	while (1)
	{
		if (get_next_xml_node(&node, pos, 0) < 1)
			break ;
		if (node.type == 2)
			continue ;
		else if (node.type == 0)
			default_check_node(scene, &node, pos);
		else if (node.type == 1)
			break ;
	}
	destroy_hmap(&node.values, free_and_null);
	if (ft_strcmp(node.name, onode->name) == 0)
		return (EXIT_SUCCESS);
	return (EXIT_FAILURE);
}

static int	parse_node(t_scene *scene, t_node *onode, char **pos)
{
	if (onode->type == 1)
		return (EXIT_FAILURE);
	if (ft_strcmp(onode->name, "config") == 0)
	{
		if (scene->configured)
		{
			ft_printf("%<#FFAA00>[Warning]%<0>"
					"Scene has two configuration blocks\n");
			return (EXIT_FAILURE);
		}
		scene->configured = 1;
		return (parse_config_node(scene, onode, pos));
	}
	else if (ft_strcmp(onode->name, "objlist") == 0)
		return (parse_objlist_node(scene, onode, pos));
	else if (ft_strcmp(onode->name, "spotlist") == 0)
		return (parse_spotlist_node(scene, onode, pos));
	else
		return (default_check_node(scene, onode, pos));
}

static int	assert_scene(t_scene *scene)
{
	if (scene->nobjs == 0)
		ft_printf("%<#FFAA00>[Warning]%<0> Scenes contains no object\n");
	if (scene->nspots == 0)
		ft_printf("%<#FFAA00>[Warning]%<0> Scenes contains no spots\n");
	if (!scene->configured)
		ft_printf("%<#FFAA00>[Warning]%<0>"
				" Scene does not contain any configuration block\n");
	return (1);
}

int			parse_scene_file(t_scene *scene, int fd)
{
	int			ret;
	t_node		node;
	char		*file;
	char		*pos;

	(void)scene;
	if (read_all_file(fd, &file) != EXIT_SUCCESS)
		return (EXIT_FAILURE);
	pos = file;
	init_hmap(&node.values);
	ret = 1;
	while (ret > 0 && (ret = get_next_xml_node(&node, &pos, 1)) > 0)
		if (parse_node(scene, &node, &pos) != EXIT_SUCCESS)
			ret = -1;
	destroy_hmap(&node.values, free_and_null);
	free(file);
	if (ret < 0 || !assert_scene(scene))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
