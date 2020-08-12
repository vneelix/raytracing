/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/07 18:41:16 by mdirect           #+#    #+#             */
/*   Updated: 2020/08/12 12:10:54 by vneelix          ###   ########.fr       */
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

int				create_item(t_rt *rt, char *file)
{
	if (get_count(rt, file))
		return (-1);
	if (rt->opt.item_c == 0)
		return (-1);
	if (!(rt->item = (t_item*)ft_memalloc(sizeof(t_item) * rt->opt.item_c)))
		return (-1);
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
