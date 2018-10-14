/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <njaber@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/14 08:02:55 by njaber            #+#    #+#             */
/*   Updated: 2018/10/14 08:04:16 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"
#include "libft.h"

void		free_and_null(void **mem)
{
	if (mem != NULL && *mem != NULL && *mem != g_nul)
	{
		free(*mem);
		*mem = NULL;
	}
}

void		free_content(void *mem, size_t len)
{
	(void)len;
	if (mem != NULL)
		free(mem);
}

void			*combine_list(t_list *lst)
{
	void	*ret;
	t_list	*tmp;
	t_uint	i;

	i = 0;
	tmp = lst;
	while (tmp != NULL)
	{
		i += tmp->content_size;
		tmp = tmp->next;
	}
	if ((ret = (void*)ft_memalloc(i)) == NULL)
		return (NULL);
	i = 0;
	tmp = lst;
	while (tmp != NULL)
	{
		ft_memcpy(ret + i, tmp->content, tmp->content_size);
		i += tmp->content_size;
		tmp = tmp->next;
	}
	return (ret);
}
