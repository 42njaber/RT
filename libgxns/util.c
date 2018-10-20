/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/15 08:35:05 by njaber            #+#    #+#             */
/*   Updated: 2018/10/19 03:36:43 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int			get_key_digit(int key_code)
{
	if (key_code == GLFW_KEY_0)
		return (0);
	if (key_code == GLFW_KEY_1)
		return (1);
	if (key_code == GLFW_KEY_2)
		return (2);
	if (key_code == GLFW_KEY_3)
		return (3);
	if (key_code == GLFW_KEY_4)
		return (4);
	if (key_code == GLFW_KEY_5)
		return (5);
	if (key_code == GLFW_KEY_6)
		return (6);
	if (key_code == GLFW_KEY_7)
		return (7);
	if (key_code == GLFW_KEY_8)
		return (8);
	if (key_code == GLFW_KEY_9)
		return (9);
	return (-1);
}
