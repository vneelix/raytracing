/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   primitives2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/24 19:14:58 by vneelix           #+#    #+#             */
/*   Updated: 2020/02/27 13:20:18 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

void		param_normalize(double *a, double *b)
{
	double	buff;

	if (a == NULL || b == NULL)
		return ;
	if ((*a > *b && *b > 0.0) || (*a < *b && *b > 0.0))
	{
		buff = *a;
		*a = *b;
		*b = buff;
	}
}

double		scalar_multiple(const t_vector v1, const t_vector v2)
{
	return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

t_vector	multiple_vector_by_num(const t_vector v, const double num)
{
	t_vector vector;

	vector.x = v.x * num;
	vector.y = v.y * num;
	vector.z = v.z * num;
	return (vector);
}

t_vector	divide_vector_by_num(const t_vector v, const double num)
{
	t_vector vector;

	vector.x = v.x / num;
	vector.y = v.y / num;
	vector.z = v.z / num;
	return (vector);
}

double		len_vector(const t_vector v)
{
	return (sqrt(v.x * v.x + v.y * v.y + v.z * v.z));
}
