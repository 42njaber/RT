/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error_callback.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdittric <cdittric@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/19 10:02:04 by cdittric          #+#    #+#             */
/*   Updated: 2018/10/02 03:40:32 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void		ft_get_error_callback(
		void (***dstfunc)(void *, int),
		void ***dstparam)
{
	static void		(*func)(void *, int);
	static void		*param;

	*dstfunc = &func;
	*dstparam = &param;
}

void			ft_set_error_callback(void (*func)(void *, int), void *param)
{
	void	(**dstfunc)(void *, int);
	void	**dstparam;

	ft_get_error_callback(&dstfunc, &dstparam);
	*dstfunc = func;
	*dstparam = param;
}

void			ft_error_callback(void)
{
	void	(**func)(void *, int);
	void	**param;

	ft_get_error_callback(&func, &param);
	if (*func != NULL)
		(*func)(*param, EXIT_FAILURE);
	else
		exit(EXIT_FAILURE);
}
