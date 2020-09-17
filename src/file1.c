/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/12 15:12:55 by vneelix           #+#    #+#             */
/*   Updated: 2020/09/12 15:12:56 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

size_t			get_cone(t_rt *rt, t_item *item, char *file)
{
	size_t		len;
	size_t		ret;

	len = ft_strlen("cone");
	if (ft_strncmp("cone", file, len))
		return (0);
	if (!(ret = len + get_item(rt, item, file + len,
											(t_uint32){CONE_PREF, CONE_REQ})))
		return (0);
	if (item != NULL)
	{
		item->type = CONE;
		item->k = tanf(item->k / 2.f * (CL_M_PI / 180.f));
		item->vector = normalize(item->vector);
	}
	return (ret);
}

size_t			get_paraboloid(t_rt *rt, t_item *item, char *file)
{
	size_t		len;
	size_t		ret;

	len = ft_strlen("paraboloid");
	if (ft_strncmp("paraboloid", file, len))
		return (0);
	if (!(ret = len + get_item(rt, item, file + len,
											(t_uint32){CONE_PREF, CONE_REQ})))
		return (0);
	if (item != NULL)
	{
		item->type = PARABOLOID;
		item->k = tanf(item->k / 2.f * (CL_M_PI / 180.f));
	}
	return (ret);
}

int				get_count(t_rt *rt, char *file)
{
	size_t		i;
	size_t		ret;

	i = 0;
	i += ft_splits(file + i);
	while (file[i] != '\0')
	{
		if ((ret = get_illum(rt, NULL, file + i)))
			rt->opt.illu_c += 1;
		else if ((ret = get_plane(rt, NULL, file + i)) ||
		(ret = get_sphere(rt, NULL, file + i)) ||
		(ret = get_cylinder(rt, NULL, file + i)) ||
		(ret = get_cone(rt, NULL, file + i)) ||
		(ret = get_paraboloid(rt, NULL, file + i)))
			rt->opt.item_c += 1;
		else if ((ret = get_sc(rt, NULL, file + i)))
		{
		}
		if (ret == 0)
			return (-1);
		i += ft_splits(file + i + ret) + ret;
	}
	return (0);
}

int				extract_item(t_rt *rt, char *file)
{
	size_t		ret;
	size_t		index[3];

	ft_bzero(index, sizeof(size_t) * 3);
	index[0] += ft_splits(file + index[0]);
	while (file[index[0]] != '\0')
	{
		if ((ret = get_illum(rt, rt->illu + index[1], file + index[0])))
			index[1] += 1;
		else if ((ret = get_plane(rt, rt->item + index[2], file + index[0]))
		|| (ret = get_sphere(rt, rt->item + index[2], file + index[0])) ||
		(ret = get_cylinder(rt, rt->item + index[2], file + index[0])) ||
		(ret = get_cone(rt, rt->item + index[2], file + index[0])) ||
		(ret = get_paraboloid(rt, rt->item + index[2], file + index[0])))
			index[2] += 1;
		else if ((ret = get_sc(rt, &rt->cam, file + index[0])))
		{
		}
		if (ret == 0)
			return (-1);
		index[0] += ft_splits(file + index[0] + ret) + ret;
	}
	return (0);
}
