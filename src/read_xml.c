/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_xml.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/25 11:20:41 by njaber            #+#    #+#             */
/*   Updated: 2018/10/27 02:54:25 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"
#include "libft.h"

const char		*g_nul = "";

static int		read_node_name(t_node *node, char **pos)
{
	int		i;

	i = 0;
	while (i < 32 && (ft_isalnum((*pos)[i]) || (*pos)[i] == '_'))
	{
		node->name[i] = (*pos)[i];
		i++;
	}
	if (i >= 32)
		return (ft_printret(EXIT_FAILURE, "Node name too long"
"(max 32 characters)\n"));
	node->name[i] = '\0';
	*pos += i;
	return (EXIT_SUCCESS);
}

/*
** static int		read_node_vals(t_node *node, char **pos)
** {
** 	int		i;
** 	char	*name;
** 	char	*val;
** 
** 	while (1)
** 	{
** 		while (ft_isblank(**pos))
** 			(*pos)++;
** 		i = 0;
** 		while (ft_isalnum((*pos)[i]) || (*pos)[i] == '_')
** 			i++;
** 		if (i <= 0)
** 			break ;
** 		name = ft_strldup(*pos, i);
** 		*pos += i;
** 		if (**pos == '=')
** 		{
** 			if (*(++(*pos)) != '"')
** 				return (ft_printret(EXIT_FAILURE, "Expected '\"'\n"));
** 			while ((*pos)[i] != '"' && (*pos)[i] != '\0')
** 				i++;
** 			if (((*pos)[i++]) != '"')
** 				return (ft_printret(EXIT_FAILURE, "Expected '\"'\n"));
** 			val = ft_strldup(*pos, i);
** 			*pos += i;
** 			add_helem(&node->values, name, val);
** 		}
** 		else
** 			add_helem(&node->values, name, (void*)g_nul);
** 		free(name);
** 	}
** 	return (EXIT_SUCCESS);
** }
*/

static int		read_node(t_node *node, char **pos)
{
	node->type = 0;
	if (**pos == '/')
	{
		node->type = 1;
		(*pos)++;
	}
	if (read_node_name(node, pos) != EXIT_SUCCESS)
		return (EXIT_FAILURE);
	if (*(*pos) == '/')
	{
		if (node->type == 1)
			return (ft_printret(EXIT_FAILURE, "Extraneous '/' in xml node\n"));
		node->type = 2;
		(*pos)++;
	}
	if (*((*pos)++) != '>')
		return (ft_printret(EXIT_FAILURE, "Expected '>' (%s)\n", node->name));
	return (EXIT_SUCCESS);
}

int				get_next_xml_node(t_node *node, char **pos, char strict)
{
	reset_hmap(&node->values, free_and_null);
	while (1)
	{
		while (**pos != '<')
		{
			if (**pos == '\0')
				return (0);
			else if (strict && !ft_isinvis(**pos))
				return (ft_printret(-1, "Unexpected character"
							"between nodes: %c\n", **pos));
			(*pos)++;
		}
		(*pos)++;
		if (ft_strncmp("!--", *pos, 3) == 0)
		{
			if ((*pos = ft_strstr(*pos, "-->")) == NULL)
				return (ft_printret(-1, "Unterminated comment\n"));
			(*pos) += 3;
		}
		else
			break ;
	}
	return (read_node(node, pos) == EXIT_SUCCESS ? 1 : -1);
}
