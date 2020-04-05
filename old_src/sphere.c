/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/16 14:44:17 by vneelix           #+#    #+#             */
/*   Updated: 2020/02/26 15:11:51 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

t_vector		sphere_normal(t_sphere *s, t_point *p)
{
	t_vector	n;

	n = subtract_point_from_point(s->center, *p);
	n = divide_vector_by_num(n, len_vector(n));
	return (n);
}

int				sphere_intersect_shadow(t_rt *rt, t_vector dir, t_point center)
{
	double		m[4];
	double		t[2];
	t_vector	v;
	int			i;

	i = -1;
	m[0] = scalar_multiple(dir, dir);
	while ((++i) < rt->sp_c)
	{
		if ((rt->sph + i)->att.refract != 0.0)
			continue ;
		v = subtract_point_from_point((rt->sph + i)->center, center);
		m[1] = 2 * scalar_multiple(v, dir);
		m[2] = scalar_multiple(v, v) - (rt->sph + i)->r * (rt->sph + i)->r;
		m[3] = m[1] * m[1] - 4 * m[0] * m[2];
		if (m[3] < 0)
			continue ;
		m[3] = sqrt(m[3]);
		t[0] = (-m[1] + m[3]) / (2 * m[0]);
		t[1] = (-m[1] - m[3]) / (2 * m[0]);
		param_normalize(t, t + 1);
		if (t[0] > 0.000001 && t[0] <= 1.0)
			return (1);
	}
	return (0);
}

void			sphere_intersect(t_rt *rt, t_vector dir, t_point center)
{
	double		m[4];
	t_vector	v;
	int			i;

	i = -1;
	m[0] = scalar_multiple(dir, dir);
	while ((++i) < rt->sp_c)
	{
		v = subtract_point_from_point((rt->sph + i)->center, center);
		m[1] = 2 * scalar_multiple(v, dir);
		m[2] = scalar_multiple(v, v) - (rt->sph + i)->r * (rt->sph + i)->r;
		m[3] = m[1] * m[1] - 4 * m[0] * m[2];
		if (m[3] < 0)
		{
			(rt->sph + i)->t1 = INFINITY;
			(rt->sph + i)->t2 = INFINITY;
			continue ;
		}
		m[3] = sqrt(m[3]);
		(rt->sph + i)->t1 = (-m[1] + m[3]) / (2 * m[0]);
		(rt->sph + i)->t2 = (-m[1] - m[3]) / (2 * m[0]);
		param_normalize(&((rt->sph + i)->t1), &((rt->sph + i)->t2));
	}
}
