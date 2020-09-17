/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/12 15:13:07 by vneelix           #+#    #+#             */
/*   Updated: 2020/09/12 15:13:28 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void			create_item_prep(t_rt *rt)
{
	int	i;

	i = -1;
	while (++i < rt->opt.item_c)
	{
		rt->item[i].textr.n_texture = -1;
		rt->item[i].textr.normal_map = -1;
	}
}

void			prepare_item(t_item *item, size_t count)
{
	while (count--)
	{
		item[count].max = INFINITY;
		item[count].min = -INFINITY;
	}
}

void			correct_item(t_item *item, size_t count)
{
	while (count--)
	{
		item[count].k = clamp(0, INFINITY, item[count].k);
		item[count].radius = clamp(0, INFINITY, item[count].radius);
		item[count].shineratio = clamp(0, 5000, item[count].shineratio);
		item[count].reflectratio = clamp(0, 1, item[count].reflectratio);
		item[count].refractratio = clamp(0, 1, item[count].refractratio);
		if (item[count].refractindex != 0.f)
			item[count].refractindex = clamp(1, INFINITY,
									item[count].refractindex);
	}
}

int				create_item(t_rt *rt, char *file)
{
	if (get_count(rt, file))
		return (-1);
	if (rt->opt.item_c == 0)
		return (-1);
	if (!(rt->item = (t_item*)ft_memalloc(sizeof(t_item) * rt->opt.item_c)))
		return (-1);
	prepare_item(rt->item, rt->opt.item_c);
	if (rt->opt.illu_c
		&& !(rt->illu = (t_item*)ft_memalloc(sizeof(t_item) * rt->opt.illu_c)))
	{
		ft_memdel((void**)rt->item);
		return (-1);
	}
	create_item_prep(rt);
	if (extract_item(rt, file))
	{
		ft_memdel((void**)&rt->item);
		ft_memdel((void**)&rt->illu);
		return (-1);
	}
	correct_item(rt->item, rt->opt.item_c);
	correct_item(rt->illu, rt->opt.illu_c);
	return (0);
}

int				get_scene(t_rt *rt, char *file_name)
{
	int			ret;
	char		*file;

	if ((ret = open(file_name, O_RDONLY)) < 0)
		return (-1);
	file = ft_getfile(NULL, ret);
	close(ret);
	if (file == NULL || file[0] == '\0')
	{
		ft_memdel((void**)&file);
		return (-1);
	}
	ret = create_item(rt, file);
	ft_memdel((void**)&file);
	if (ret)
		return (-1);
	return (0);
}
