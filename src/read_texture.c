/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_texture.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/07 19:39:15 by dgriseld          #+#    #+#             */
/*   Updated: 2020/08/08 16:54:57 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int						get_file_name(t_rt *rt)
{
	size_t				num_files;

	if (!(rt->file_name = ft_directory("texture/", &num_files)))
		return (-1);
	return (num_files);
}

void					free_surfaces(SDL_Surface **surf)
{
	void				*temp;

	if (!surf)
		return ;
	temp = surf;
	while (*surf)
		SDL_FreeSurface(*surf++);
	free(temp);
}

static SDL_Surface		**prepare_read_texture0(t_rt *rt)
{
	SDL_Surface			**surf;

	if (!(surf = (SDL_Surface**)ft_memalloc(sizeof(SDL_Surface*) *
	(rt->opt.texture_c + 1))))
	{
		write(2, "texture files don't read/n", 24);
		return (NULL);
	}
	return (surf);
}

SDL_Surface				**prepare_read_texture(t_rt *rt)
{
	SDL_Surface			**surf;
	int					i;
	char				*ptr;

	surf = prepare_read_texture0(rt);
	i = -1;
	while (++i != (int)rt->opt.texture_c)
	{
		if ((ptr = ft_strjoin("texture/", rt->file_name[i])) == NULL)
		{
			free_surfaces(surf);
			return (NULL);
		}
		if ((surf[i] = texture_convert(IMG_Load(ptr))) == NULL)
		{
			ft_memdel((void**)&ptr);
			free_surfaces(surf);
			return (NULL);
		}
		ft_memdel((void**)&ptr);
	}
	return (surf);
}
