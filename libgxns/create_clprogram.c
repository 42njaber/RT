/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_clprogram.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <njaber@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/17 15:38:42 by njaber            #+#    #+#             */
/*   Updated: 2018/10/20 12:57:23 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libgxns.h"

char			*join_free(char *s1, char *s2, size_t src_len)
{
	size_t	dst_len;
	char	*ret;

	dst_len = ft_strlen(s1);
	ret = ft_strnew(dst_len + src_len);
	ft_memcpy(ret, s1, dst_len);
	ft_memcpy(ret + dst_len, s2, src_len);
	free(s1);
	return (ret);
}

cl_program		create_program_from_file(cl_context context, const char *file)
{
	cl_program	ret;
	char		*kernel;
	char		buf[4096];
	int			fd;
	int			rd;

	if ((fd = open(file, O_RDONLY)) < 0)
		return (NULL);
	rd = read(fd, buf, 4096);
	kernel = ft_strnew(rd);
	ft_memcpy(kernel, buf, rd);
	while ((rd = read(fd, buf, 4096)) > 0)
		kernel = join_free(kernel, buf, rd);
	if (rd == -1)
	{
		close(fd);
		return (NULL);
	}
	ret = clCreateProgramWithSource(context, 1,
			(const char**)&kernel, NULL, NULL);
	close(fd);
	free(kernel);
	return (ret);
}
