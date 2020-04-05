/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   closest_surf.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/24 19:51:25 by vneelix           #+#    #+#             */
/*   Updated: 2020/02/27 15:54:38 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

static void		closest_part2(t_rt *rt, t_obj *obj, double *t_min)
{
	int			i;

	i = -1;
	while ((++i) < rt->cr_c)
		if ((rt->clr + i)->t1 > 0.0 && (rt->clr + i)->t1 < *t_min)
		{
			*t_min = (rt->clr + i)->t1;
			obj->ptr = (rt->clr + i);
			obj->type = 2;
		}
	i = -1;
	while ((++i) < rt->cn_c)
		if ((rt->cne + i)->t1 > 0.0 && (rt->cne + i)->t1 < *t_min)
		{
			*t_min = (rt->cne + i)->t1;
			obj->ptr = (rt->cne + i);
			obj->type = 3;
		}
}

__uint32_t		closest_surface(t_rt *rt)
{
	int			i;
	t_obj		obj;
	double		t_min;

	i = -1;
	t_min = INFINITY;
	while ((++i) < rt->sp_c)
		if ((rt->sph + i)->t1 > 0.0 && (rt->sph + i)->t1 < t_min)
		{
			t_min = (rt->sph + i)->t1;
			obj.ptr = (rt->sph + i);
			obj.type = 0;
		}
	i = -1;
	while ((++i) < rt->pl_c)
		if ((rt->pln + i)->t > 0.0 && (rt->pln + i)->t < t_min)
		{
			t_min = (rt->pln + i)->t;
			obj.ptr = (rt->pln + i);
			obj.type = 1;
		}
	closest_part2(rt, &obj, &t_min);
	if (t_min != INFINITY)
		return (calc_light(rt, &obj));
	return (0);
}
