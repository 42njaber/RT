/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_clprogram.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <njaber@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/17 15:38:42 by njaber            #+#    #+#             */
/*   Updated: 2018/10/26 21:55:52 by njaber           ###   ########.fr       */
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

static int		concat_file(char **str, const char *file)
{
	char		buf[4096];
	int			fd;
	int			rd;

	if ((fd = open(file, O_RDONLY)) < 0)
		return (EXIT_FAILURE);
	while ((rd = read(fd, buf, 4096)) > 0)
		*str = join_free(*str, buf, rd);
	close(fd);
	if (rd == -1)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

cl_program		create_program_from_files(cl_context context, int file_nb,
															const char **files)
{
	cl_program	ret;
	char	*sources;
	int		i;

	if ((sources = (char*)ft_memalloc(1)) == NULL)
		ft_error("Malloc error\n");
	i = -1;
	while (++i < file_nb)
	{
		if (concat_file(&sources, files[i]) != EXIT_SUCCESS)
			ft_error("Could not read kernel file: %s\n", files[i]);
	}
	ret = clCreateProgramWithSource(context, 1,
			(const char**)&sources, NULL, NULL);
	free(sources);
	return (ret);
}
