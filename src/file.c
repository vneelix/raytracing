/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/12 15:12:36 by vneelix           #+#    #+#             */
/*   Updated: 2020/09/12 15:12:37 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

size_t			get_sc(t_rt *rt, t_item *item, char *file)
{
	size_t		len;
	size_t		ret;

	ret = 0;
	len = ft_strlen("scene");
	if (ft_strncmp("scene", file, len))
		return (0);
	if (!(ret = len + get_item(rt, item, file + len,
									(t_uint32){CENTER | VECTOR | RGB | K, 0})))
		return (0);
	if (item != NULL)
		item->vector = normalize(item->vector);
	return (ret);
}

size_t			get_illum(t_rt *rt, t_item *item, char *file)
{
	size_t		len;
	size_t		ret;

	ret = 0;
	len = ft_strlen("illum");
	if (ft_strncmp("illum", file, len))
		return (0);
	if (!(ret = len + get_item(rt, item, file + len,
			(t_uint32){CENTER | RADIUS | K | RGB, CENTER | K})))
		return (0);
	if (item != NULL)
		item->type = item->radius ? SPHERE : POINT;
	return (ret);
}

size_t			get_plane(t_rt *rt, t_item *item, char *file)
{
	size_t		len;
	size_t		ret;

	len = ft_strlen("plane");
	if (ft_strncmp("plane", file, len))
		return (0);
	if (!(ret = len + get_item(rt, item, file + len,
									(t_uint32){PLANE_PREF, PLANE_REQ})))
		return (0);
	if (item != NULL)
	{
		item->type = PLANE;
		item->normal = normalize(item->vector);
	}
	return (ret);
}

size_t			get_sphere(t_rt *rt, t_item *item, char *file)
{
	size_t		len;
	size_t		ret;

	len = ft_strlen("sphere");
	if (ft_strncmp("sphere", file, len))
		return (0);
	if (!(ret = len + get_item(rt, item, file + len,
										(t_uint32){SPHERE_PREF, SPHERE_REQ})))
		return (0);
	if (item != NULL)
	{
		item->type = SPHERE;
	}
	return (ret);
}

size_t			get_cylinder(t_rt *rt, t_item *item, char *file)
{
	size_t		len;
	size_t		ret;

	len = ft_strlen("cylinder");
	if (ft_strncmp("cylinder", file, len))
		return (0);
	if (!(ret = len + get_item(rt, item, file + len,
									(t_uint32){CYLINDER_PREF, CYLINDER_REQ})))
		return (0);
	if (item != NULL)
	{
		item->type = CYLINDER;
		item->vector = normalize(item->vector);
	}
	return (ret);
}
