/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memalloc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/20 16:20:35 by njaber            #+#    #+#             */
/*   Updated: 2018/10/26 19:00:39 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
# include "stdio.h"

void		*ft_memalloc(size_t size)
{
	void	*ret;

	if ((ret = malloc(size)) == NULL)
		return (NULL);
	ft_bzero(ret, size);
	return (ret);
}
