/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_view.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <njaber@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/16 13:56:12 by njaber            #+#    #+#             */
/*   Updated: 2018/10/16 16:50:30 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void		copy_scene_data(t_view *view, t_scene *scene)
{
	view->pos = scene->start_pos;
	view->rot = scene->start_rot;
	view->set.sky_color = scene->sky_color;
	view->set.ambiant_light = scene->ambiant_light;
	view->nobjs = scene->nobjs;
	view->nspots = scene->nspots;
	if ((view->objs =
				(t_obj*)ft_memalloc(sizeof(t_obj) * scene->nobjs)) == NULL)
		ft_error("Malloc error\n");
	if ((view->spots =
				(t_spot*)ft_memalloc(sizeof(t_spot) * scene->nspots)) == NULL)
		ft_error("Malloc error\n");
	ft_memcpy(view->objs, scene->objs, sizeof(t_obj) * scene->nobjs);
	ft_memcpy(view->spots, scene->spots, sizeof(t_spot) * scene->nspots);
}

void		cleanup_view(t_view *view)
{
	if (view->objbuf != 0)
	{
		clReleaseMemObject(view->objbuf);
		view->objbuf = 0;
	}
	if (view->spotbuf != 0)
	{
		clReleaseMemObject(view->spotbuf);
		view->spotbuf = 0;
	}
	free_and_null((void**)&view->objs);
	free_and_null((void**)&view->spots);
}
