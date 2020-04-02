/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/17 17:12:29 by vneelix           #+#    #+#             */
/*   Updated: 2020/02/27 14:05:41 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

static void		cone_calc(double *m, t_cone *cn, t_vector *v, t_vector *dir)
{
	m[0] = scalar_multiple(*dir, *dir) - (1.0 + cn->k * cn->k)
		* scalar_multiple(*dir, cn->dir) * scalar_multiple(*dir, cn->dir);
	m[1] = 2 * (scalar_multiple(*dir, *v) - (1.0 + cn->k * cn->k)
		* scalar_multiple(*dir, cn->dir) * scalar_multiple(*v, cn->dir));
	m[2] = scalar_multiple(*v, *v) - (1.0 + cn->k * cn->k)
		* scalar_multiple(*v, cn->dir) * scalar_multiple(*v, cn->dir);
	m[3] = m[1] * m[1] - 4 * m[0] * m[2];
}

t_vector		cone_normal(t_rt *rt, t_cone *cn, t_point *p)
{
	double		m;
	t_vector	n;

	m = scalar_multiple(rt->view.dir,
		multiple_vector_by_num(cn->dir, cn->t1))
			+ scalar_multiple(subtract_point_from_point(
				cn->center, rt->view.c), cn->dir);
	n = subtract_point_from_point(cn->center, *p);
	n = subtract_vector_from_vector(
		multiple_vector_by_num(cn->dir, m * (1.0 + cn->k * cn->k)), n);
	n = divide_vector_by_num(n, len_vector(n));
	return (n);
}

int				cone_intersect_shadow(t_rt *rt, t_vector dir, t_point center)
{
	int			i;
	double		m[4];
	double		t[2];
	t_vector	v;

	i = -1;
	while ((++i) < rt->cn_c)
	{
		if ((rt->cne + i)->att.refract != 0.0)
			continue ;
		v = subtract_point_from_point((rt->cne + i)->center, center);
		cone_calc(m, rt->cne + i, &v, &dir);
		if (m[3] < 0)
			continue ;
		m[3] = sqrt(m[3]);
		t[0] = (-m[1] + m[3]) / (2 * m[0]);
		t[1] = (-m[1] - m[3]) / (2 * m[0]);
		if ((t[0] > 0.000001 && t[0] <= 1.0) || (t[1] > 0.000001 && t[1] <= 1.0))
			return (1);
	}
	return (0);
}

void			cone_intersect(t_rt *rt, t_vector dir, t_point center)
{
	int			i;
	double		m[4];
	t_vector	v;

	i = -1;
	while ((++i) < rt->cn_c)
	{
		v = subtract_point_from_point((rt->cne + i)->center, center);
		cone_calc(m, rt->cne + i, &v, &dir);
		if (m[3] < 0)
		{
			(rt->cne + i)->t1 = INFINITY;
			(rt->cne + i)->t2 = INFINITY;
			continue ;
		}
		m[3] = sqrt(m[3]);
		(rt->cne + i)->t1 = (-m[1] + m[3]) / (2 * m[0]);
		(rt->cne + i)->t2 = (-m[1] - m[3]) / (2 * m[0]);
		param_normalize(&((rt->cne + i)->t1), &((rt->cne + i)->t2));
	}
}
