/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/16 17:11:47 by vneelix           #+#    #+#             */
/*   Updated: 2020/02/26 21:46:04 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

static void	cyli_calc(double *m,
					t_cyli *cl, t_vector *v, t_vector *dir)
{
	m[0] = scalar_multiple(*dir, *dir)
		- scalar_multiple(*dir, cl->dir)
			* scalar_multiple(*dir, cl->dir);
	m[1] = 2 * (scalar_multiple(*dir, *v)
			- scalar_multiple(*dir, cl->dir)
				* scalar_multiple(*v, cl->dir));
	m[2] = scalar_multiple(*v, *v)
		- scalar_multiple(*v, cl->dir)
			* scalar_multiple(*v, cl->dir)
							- cl->r * cl->r;
	m[3] = m[1] * m[1] - 4 * m[0] * m[2];
}

t_vector	cyli_normal(t_rt *rt, t_cyli *cl, t_point *p)
{
	double		m;
	t_vector	n;

	m = scalar_multiple(rt->view.dir,
		multiple_vector_by_num(cl->dir, cl->t1))
			+ scalar_multiple(subtract_point_from_point(
				cl->center, rt->view.c), cl->dir);
	n = subtract_vector_from_vector(
		multiple_vector_by_num(cl->dir, m),
			subtract_point_from_point(cl->center, *p));
	n = divide_vector_by_num(n, len_vector(n));
	return (n);
}

int			cyli_intersect_shadow(t_rt *rt, t_vector dir, t_point center)
{
	int			i;
	double		m[4];
	double		t[2];
	t_vector	v;

	i = -1;
	while ((++i) < rt->cr_c)
	{
		if ((rt->clr + i)->att.refract != 0.0)
			continue ;
		v = subtract_point_from_point((rt->clr + i)->center, center);
		cyli_calc(m, rt->clr + i, &v, &dir);
		if (m[3] < 0)
			continue ;
		m[3] = sqrt(m[3]);
		t[0] = (-m[1] + m[3]) / (2 * m[0]);
		t[1] = (-m[1] - m[3]) / (2 * m[0]);
		if ((t[0] > 0.000001 && t[0] <= 1.0)
			|| (t[1] > 0.000001 && t[1] <= 1.0))
			return (1);
	}
	return (0);
}

void		cyli_intersect(t_rt *rt, t_vector dir, t_point center)
{
	int			i;
	double		m[4];
	t_vector	v;

	i = -1;
	while ((++i) < rt->cr_c)
	{
		v = subtract_point_from_point((rt->clr + i)->center, center);
		cyli_calc(m, rt->clr + i, &v, &dir);
		if (m[3] < 0)
		{
			(rt->clr + i)->t1 = INFINITY;
			(rt->clr + i)->t2 = INFINITY;
			continue ;
		}
		m[3] = sqrt(m[3]);
		(rt->clr + i)->t1 = (-m[1] + m[3]) / (2 * m[0]);
		(rt->clr + i)->t2 = (-m[1] - m[3]) / (2 * m[0]);
		param_normalize(&((rt->clr + i)->t1), &((rt->clr + i)->t2));
	}
}
