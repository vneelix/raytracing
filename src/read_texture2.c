/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_texture2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/12 15:21:27 by vneelix           #+#    #+#             */
/*   Updated: 2020/09/12 15:21:28 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static int			calc_len(SDL_Surface **surf, int n)
{
	int				i;
	int				summ;

	if (!surf)
		return (0);
	summ = 0;
	i = -1;
	while (++i != n)
		summ += surf[i]->h * surf[i]->w + 2;
	return (summ);
}

static int			surface_to_uint(t_rt *rt, SDL_Surface **surf)
{
	int				size;
	int				i;
	int				tsize;

	size = rt->opt.texture_c;
	i = -1;
	while (++i != rt->opt.texture_c)
	{
		rt->texture[i] = size;
		rt->texture[size] = surf[i]->w;
		rt->texture[size + 1] = surf[i]->h;
		SDL_LockSurface(surf[i]);
		tsize = surf[i]->w * surf[i]->h;
		SDL_memcpy(rt->texture + (size + 2), surf[i]->pixels,
											sizeof(uint) * tsize);
		SDL_UnlockSurface(surf[i]);
		size += tsize + 2;
	}
	return (size);
}

int					read_texture(t_rt *rt)
{
	SDL_Surface		**surf;
	int				n;

	if ((surf = prepare_read_texture(rt)) == 0)
		return (-1);
	n = calc_len(surf, rt->opt.texture_c);
	if ((rt->texture = (uint*)malloc(sizeof(uint) *
							(n + rt->opt.texture_c))) == NULL)
	{
		free_surfaces(surf);
		return (-1);
	}
	rt->opt.texture_size = surface_to_uint(rt, surf);
	free_surfaces(surf);
	return (0);
}
