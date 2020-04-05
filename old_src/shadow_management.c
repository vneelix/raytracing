/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shadow_management.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/19 11:58:07 by vneelix           #+#    #+#             */
/*   Updated: 2020/02/25 16:09:24 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

t_point	correct_point(t_point p, t_vector n)
{
	return (add_vector_to_point(multiple_vector_by_num(n, 0.000001), p));
}

int		shadow_management(t_rt *rt, t_vector *l, t_point *p)
{
	if (rt->pln != NULL && plane_intersect_shadow(rt, *l, *p))
		return (1);
	else if (rt->sph != NULL && sphere_intersect_shadow(rt, *l, *p))
		return (1);
	else if (rt->clr != NULL && cyli_intersect_shadow(rt, *l, *p))
		return (1);
	else if (rt->cne != NULL && cone_intersect_shadow(rt, *l, *p))
		return (1);
	else
		return (0);
}
