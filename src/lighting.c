/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lighting.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/17 15:59:22 by dgriseld          #+#    #+#             */
/*   Updated: 2020/02/27 13:57:16 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

static double	compute_shine(t_vector v, t_vector *n, t_light *l, t_obj_p *p)
{
	t_vector	r;
	double		ise;
	double		scalar;

	r = subtract_vector_from_vector(l->v,
		multiple_vector_by_num(*n, 2 * scalar_multiple(*n, l->v)));
	if ((scalar = scalar_multiple(r, v)) < 0.000001)
		return (0);
	ise = l->ise * pow((scalar / ((len_vector(r)
		* len_vector(v)))), p->att->shine);
	if (ise > 1.0)
		ise = 1.0;
	return (ise);
}

static double	light_ratio(t_light *src, t_vector *n, t_vector *l)
{
	double		result;
	double		scalar;

	scalar = scalar_multiple(*n, *l);
	if (scalar < 0.0)
		return (0);
	result = ((src->ise) * scalar) / (len_vector(*n) * len_vector(*l));
	if (result > 1.0)
		return (1.0);
	return (result);
}

static t_vector	get_normalvector(t_rt *rt, t_obj *obj, t_point *point)
{
	t_vector	n;

	if (obj->type == 0)
		n = sphere_normal(obj->ptr, point);
	else if (obj->type == 1)
		n = ((t_plane*)(obj->ptr))->n;
	else if (obj->type == 2)
		n = cyli_normal(rt, obj->ptr, point);
	else if (obj->type == 3)
		n = cone_normal(rt, obj->ptr, point);
	else
		return ((t_vector){0, 0, 0});
	if (scalar_multiple(rt->view.dir, n) > 0.001)
		n = multiple_vector_by_num(n, -1.0);
	return (n);
}

__uint32_t		calc_light(t_rt *rt, t_obj *obj)
{
	double		l_ratio[2];
	t_obj_p		p;
	t_vector	n;
	int			i;

	i = -1;
	l_ratio[1] = 0.0;
	l_ratio[0] = DIFFUSE;
	p = get_point(rt, obj);
	n = get_normalvector(rt, obj, &(p.p));
	while ((++i) != rt->lt_c)
	{
		if ((rt->lht + i)->t == 0)
			(rt->lht + i)->v =
				subtract_point_from_point(p.p, ((rt->lht) + i)->c);
		if (shadow_management(rt, &((rt->lht + i)->v), &p.p))
			continue ;
		l_ratio[0] += light_ratio((rt->lht) + i, &n, &((rt->lht + i)->v));
		if (p.att->shine != 0)
			l_ratio[1] += compute_shine(subtract_point_from_point(
				p.p, rt->view.c), &n, rt->lht + i, &p);
	}
	return (0);
}
