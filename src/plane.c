/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/18 17:46:51 by vneelix           #+#    #+#             */
/*   Updated: 2020/02/27 20:52:18 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

int				plane_intersect_shadow(t_rt *rt, t_vector dir, t_point center)
{
	int			i;
	double		t;
	double		s;

	i = -1;
	while ((++i) < rt->pl_c)
	{
		if ((rt->pln + i)->att.refract != 0.0)
			continue ;
		s = scalar_multiple(dir, (rt->pln + i)->n);
		if (s == 0)
			continue ;
		t = scalar_multiple(subtract_point_from_point(
			center, (rt->pln + i)->a), (rt->pln + i)->n) / s;
		if (t > 0.000001 && t <= 1.0)
			return (1);
	}
	return (0);
}

void			plane_intersect(t_rt *rt, t_vector dir, t_point center)
{
	double		s;
	int			i;

	i = -1;
	while ((++i) < rt->pl_c)
	{
		s = scalar_multiple(dir, (rt->pln + i)->n);
		if (s == 0)
		{
			(rt->pln + i)->t = INFINITY;
			continue ;
		}
		(rt->pln + i)->t =
			scalar_multiple(subtract_point_from_point(
				center, (rt->pln + i)->a), (rt->pln + i)->n) / s;
	}
}
