/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare_item.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdirect <mdirect@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/05 20:52:43 by dgriseld          #+#    #+#             */
/*   Updated: 2020/08/07 17:54:34 by mdirect          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static cl_float3	cross_product(cl_float3 a, cl_float3 b)
{
	cl_float3		temp;

	temp.x = a.y * b.z - a.z * b.y;
	temp.y = a.z * b.x - a.x * b.z;
	temp.z = a.x * b.y - a.y * b.x;
	return (temp);
}

static cl_float3	divide_vector_by_num(const cl_float3 v, const float num)
{
	cl_float3 vector;

	vector.x = v.x / num;
	vector.y = v.y / num;
	vector.z = v.z / num;
	return (vector);
}

static float		len_vector(const cl_float3 v)
{
	return (sqrt(v.x * v.x + v.y * v.y + v.z * v.z));
}

static cl_float3	normalize_vec(cl_float3 v)
{
	return (divide_vector_by_num(v, len_vector(v)));
}

size_t				set_item_par(t_item *item, size_t n)
{
	size_t			i;
	cl_float3		temp;

	temp.x = 1;
	temp.y = 1;
	temp.z = 1;
	i = -1;
	while (++i < n)
	{
		item[i].vector = normalize_vec(item[i].vector);
		item[i].textr.vector_e = cross_product(temp, item[i].vector);
		item[i].textr.vector_e = normalize_vec(item[i].textr.vector_e);
		item[i].textr.vector_n = item[i].vector;
		item[i].textr.texture_scale = 6.5;
	}
	return (1);
}
