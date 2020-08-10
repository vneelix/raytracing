/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   func.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/05 20:54:15 by dgriseld          #+#    #+#             */
/*   Updated: 2020/08/10 12:51:16 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

size_t		ft_splits(char *s)
{
	size_t	i;

	i = 0;
	while (s[i] != '\0' && (s[i] == ' '
		|| s[i] == '\t' || s[i] == '\n'))
		i += 1;
	return (i);
}

size_t		ft_number(char *s)
{
	size_t	i;

	i = 0;
	if (s[i] == '-' || s[i] == '+')
		i += 1;
	while (ft_isdigit(s[i]))
	{
		i += 1;
		if (s[i] == '.')
			i += 1;
	}
	return (i);
}

float		vector_len(const cl_float3 vect)
{
	return (sqrt(
		vect.x * vect.x + vect.y * vect.y + vect.z * vect.z));
}

cl_float3	normalize(cl_float3 vect)
{
	float	len;

	len = vector_len(vect);
	if (len == 0.f || len == 1.f)
		return (vect);
	return (
		(cl_float3){{
			vect.x / len,
			vect.y / len,
			vect.z / len
	}});
}
