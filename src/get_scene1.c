/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_scene1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/07 18:55:28 by mdirect           #+#    #+#             */
/*   Updated: 2020/08/08 16:48:16 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

size_t		extract_param_part4(t_rt *rt, char *str, t_item *item, uint32_t *f)
{
	size_t ret;

	ret = 0;
	if ((ret = extract_float("refract", str, (item != NULL) ?
											&(item->refractratio) : NULL)))
	{
		if (*f & REFRACT)
			return (0);
		*f |= REFRACT;
	}
	else if ((ret = extract_string(rt, "normal_map_file", str, (item != NULL) ?
											&(item->textr.normal_map) : NULL)))
	{
		if (*f & NORMAL_MAP_FILE)
			return (0);
		*f |= NORMAL_MAP_FILE;
	}
	else if ((ret = extract_string(rt, "texture_file", str, (item != NULL) ?
											&(item->textr.n_texture) : NULL)))
	{
		if (*f & TEXTURE_FILE)
			return (0);
		*f |= TEXTURE_FILE;
	}
	return (ret);
}

size_t		extract_param_part5(char *str, t_item *item, uint32_t *f)
{
	size_t	ret;

	ret = 0;
	if ((ret = extract_float("texture_tilling", str, (item != NULL) ?
										&(item->textr.texture_tilling) : NULL)))
	{
		if (*f & TEXTURE_TILLING)
			return (0);
		*f |= TEXTURE_TILLING;
	}
	else if ((ret = extract_float("texture_scale", str, (item != NULL) ?
										&(item->textr.texture_scale) : NULL)))
	{
		if (*f & TEXTURE_SCALE)
			return (0);
		*f |= TEXTURE_SCALE;
	}
	return (ret);
}

size_t		extract_param(t_rt *rt, char *str, t_item *item, uint32_t *f)
{
	size_t	ret;
	size_t	temp;

	ret = 0;
	temp = extract_param_part1(str, item, f);
	ret = temp ? temp : ret;
	temp = extract_param_part2(str, item, f);
	ret = temp ? temp : ret;
	temp = extract_param_part3(str, item, f);
	ret = temp ? temp : ret;
	temp = extract_param_part4(rt, str, item, f);
	ret = temp ? temp : ret;
	temp = extract_param_part5(str, item, f);
	ret = temp ? temp : ret;
	if (!ret)
	{
		return (0);
	}
	return (ret);
}
