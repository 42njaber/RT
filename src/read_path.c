/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <njaber@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/16 00:33:51 by njaber            #+#    #+#             */
/*   Updated: 2018/10/20 12:52:52 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <dirent.h>
#include <sys/stat.h>
#include "rt.h"

static int		read_file(t_ptr *p, char *path)
{
	int		fd;
	int		err;
	t_scene	*scene;

	if ((fd = open(path, O_RDONLY)) < 0)
		return (ft_printret(EXIT_FAILURE,
					"Could not open file %s\n", path));
	if ((scene = new_scene(p, path)) == NULL)
	{
		close(fd);
		return (EXIT_FAILURE);
	}
	err = parse_scene_file(scene, fd);
	close(fd);
	if (err != EXIT_SUCCESS)
		del_helem(&p->scenes, path, free_scene);
	return (err);
}

static int		read_if_file(t_ptr *p, char *path)
{
	struct stat		pathinfo;
	int		err;

	if ((err = stat(path, &pathinfo)) < 0)
		return (ft_printret(EXIT_FAILURE,
					"Could not read properties of %s\n", path));
	if ((pathinfo.st_mode & S_IFMT) != S_IFDIR)
		read_file(p, path);
	return (EXIT_FAILURE);
}

static int		read_dir(t_ptr *p, char *path)
{
	DIR				*dir;
	struct dirent	*file;
	char			*fpath;
	char			*dpath;

	if ((dir = opendir(path)) == NULL)
		return (ft_printret(EXIT_FAILURE,
					"Could not open dir %s\n", path));
	if ((dpath = ft_strjoin(path, "/")) == NULL)
		ft_error("Malloc error\n");
	while ((file = readdir(dir)))
	{
		if (ft_strrchr(file->d_name, '.') == NULL ||
				ft_strcmp(ft_strrchr(file->d_name, '.'), ".xml") != 0)
			continue ;
		if ((fpath = ft_strjoin(dpath, file->d_name)) == NULL)
			ft_error("Malloc error\n");
		read_if_file(p, fpath);
		free(fpath);
	}
	free(file);
	free(dpath);
	closedir(dir);
	return (EXIT_SUCCESS);
}

int			read_path(t_ptr *p, char *path)
{
	struct stat		pathinfo;
	int		err;

	if ((err = stat(path, &pathinfo)) < 0)
		return (ft_printret(EXIT_FAILURE,
					"Could not read properties of %s\n", path));
	if ((pathinfo.st_mode & S_IFMT) == S_IFDIR)
		return (read_dir(p, path));
	else
		return (read_file(p, path));
}
