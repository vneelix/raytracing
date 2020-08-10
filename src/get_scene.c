/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_scene.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/07 17:44:20 by mdirect           #+#    #+#             */
/*   Updated: 2020/08/08 16:47:27 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

size_t		extract_param_part1(char *str, t_item *item, uint32_t *f)
{
	size_t ret;

	ret = 0;
	if ((ret = extract_float("k", str, (item != NULL) ? &(item->k) : NULL)))
	{
		if (*f & K)
			return (0);
		*f |= K;
	}
	else if ((ret = extract_float("min", str, (item != NULL) ?
													&(item->min) : NULL)))
	{
		if (*f & MIN)
			return (0);
		*f |= MIN;
	}
	else if ((ret = extract_float("max", str, (item != NULL) ?
													&(item->max) : NULL)))
	{
		if (*f & MAX)
			return (0);
		*f |= MAX;
	}
	return (ret);
}

size_t		extract_param_part2(char *str, t_item *item, uint32_t *f)
{
	size_t ret;

	ret = 0;
	if ((ret = extract_float3("rgb", str, (item != NULL) ?
													&(item->color) : NULL)))
	{
		if (*f & RGB)
			return (0);
		*f |= RGB;
	}
	else if ((ret = extract_float("shine", str, (item != NULL) ?
												&(item->shineratio) : NULL)))
	{
		if (*f & SHINE)
			return (0);
		*f |= SHINE;
	}
	else if ((ret = extract_float("radius", str, (item != NULL) ?
													&(item->radius) : NULL)))
	{
		if (*f & RADIUS)
			return (0);
		*f |= RADIUS;
	}
	return (ret);
}

size_t		extract_param_part3(char *str, t_item *item, uint32_t *f)
{
	size_t ret;

	ret = 0;
	if ((ret = extract_float3("center", str, (item != NULL) ?
													&(item->center) : NULL)))
	{
		if (*f & CENTER)
			return (0);
		*f |= CENTER;
	}
	else if ((ret = extract_float3("vector", str, (item != NULL) ?
													&(item->vector) : NULL)))
	{
		if (*f & VECTOR)
			return (0);
		*f |= VECTOR;
	}
	else if ((ret = extract_float("reflect", str, (item != NULL) ?
												&(item->reflectratio) : NULL)))
	{
		if (*f & REFLECT)
			return (0);
		*f |= REFLECT;
	}
	return (ret);
}
