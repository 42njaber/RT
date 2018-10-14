/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_scene.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/25 11:20:41 by njaber            #+#    #+#             */
/*   Updated: 2018/10/14 11:58:57 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"
#include "libft.h"

int			read_all_file(int fd, char **stock)
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

int			default_check_node(t_ptr *p, t_node *onode, char **pos)
{
	t_node	node;

	init_hmap(&node.values);
	if (onode->type == 1)
		return (EXIT_FAILURE);
	while (1)
	{
		if (get_next_xml_node(&node, pos, 0) < 1)
			break ;
		if (node.type == 2)
			continue ;
		else if (node.type == 0)
			default_check_node(p, &node, pos);
		else if (node.type == 1)
			break ;
	}
	destroy_hmap(&node.values, free_and_null);
	if (ft_strcmp(node.name, onode->name) == 0)
		return (EXIT_SUCCESS);
	return (EXIT_FAILURE);
}

static int	parse_node(t_ptr *p, t_node *onode, char **pos, int *config)
{
	if (onode->type == 1)
		return (EXIT_FAILURE);
	ft_printf("Reading node: %s\n", onode->name);
	if (ft_strcmp(onode->name, "config") == 0)
	{
		if (*config)
		{
			ft_printf("Scene has two config blocks\n");
			return (EXIT_FAILURE);
		}
		*config = 1;
		return (parse_config_node(p, onode, pos));
	}
	else if (ft_strcmp(onode->name, "objlist") == 0)
		return (parse_objlist_node(p, onode, pos));
	else if (ft_strcmp(onode->name, "spotlist") == 0)
		return (parse_spotlist_node(p, onode, pos));
	else
		return (default_check_node(p, onode, pos));
}

int			parse_scene_file(t_ptr *p, int fd)
{
	int			ret;
	int			config;
	t_node		node;
	char		*file;
	char		*pos;

	(void)p;
	if (read_all_file(fd, &file) != EXIT_SUCCESS)
		return (EXIT_FAILURE);
	pos = file;
	init_hmap(&node.values);
	config = 0;
	ret = 1;
	while (ret > 0 && (ret = get_next_xml_node(&node, &pos, 1)) > 0)
		if (parse_node(p, &node, &pos, &config) != EXIT_SUCCESS)
			ret = -1;
	destroy_hmap(&node.values, free_and_null);
	free(file);
	if (ret < 0 || config == 0)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
