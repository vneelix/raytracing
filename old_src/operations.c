/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   primitives.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/05 11:40:52 by vneelix           #+#    #+#             */
/*   Updated: 2020/02/24 19:16:18 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

t_point		add_vector_to_point(const t_vector v, const t_point p)
{
	t_point point;

	point.x = p.x + v.x;
	point.y = p.y + v.y;
	point.z = p.z + v.z;
	return (point);
}

t_point		subtract_vector_from_point(const t_vector v, const t_point p)
{
	t_point point;

	point.x = p.x - v.x;
	point.y = p.y - v.y;
	point.z = p.z - v.z;
	return (point);
}

t_vector	subtract_point_from_point(const t_point p1, const t_point p2)
{
	t_vector vector;

	vector.x = p2.x - p1.x;
	vector.y = p2.y - p1.y;
	vector.z = p2.z - p1.z;
	return (vector);
}

t_vector	add_vector_to_vector(const t_vector v1, t_vector v2)
{
	t_vector vector;

	vector.x = v1.x + v2.x;
	vector.y = v1.y + v2.y;
	vector.z = v1.z + v2.z;
	return (vector);
}

t_vector	subtract_vector_from_vector(const t_vector v1, t_vector v2)
{
	t_vector vector;

	vector.x = v2.x - v1.x;
	vector.y = v2.y - v1.y;
	vector.z = v2.z - v1.z;
	return (vector);
}
