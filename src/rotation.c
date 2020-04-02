/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotation.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 16:41:02 by vneelix           #+#    #+#             */
/*   Updated: 2020/02/27 17:27:25 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

static t_vector	rotation_y(double alpha, t_vector v)
{
	t_vector		tmp;

	tmp.x = v.x * cos(alpha) + v.z * sin(alpha);
	tmp.y = v.y;
	tmp.z = v.x * -(sin(alpha)) + v.z * cos(alpha);
	return (tmp);
}

static t_vector	rotation_x(double alpha, t_vector v)
{
	t_vector		tmp;

	tmp.x = v.x;
	tmp.y = v.y * cos(alpha) - v.z * sin(alpha);
	tmp.z = v.y * sin(alpha) + v.z * cos(alpha);
	return (tmp);
}

void			ft_rotation(t_rt *rt)
{
	if (rt->angle_x != 0)
		rt->view.dir = rotation_x((double)(rt->angle_x)
			* (M_PI / 180.0), rt->view.dir);
	if (rt->angle_y != 0)
		rt->view.dir = rotation_y((double)(rt->angle_y)
			* (M_PI / 180.0), rt->view.dir);
}
