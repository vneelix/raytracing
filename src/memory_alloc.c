/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_alloc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/26 11:38:45 by dgriseld          #+#    #+#             */
/*   Updated: 2020/02/27 17:57:35 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

void		mem_free(t_rt *rt)
{
	if (rt->lt_c != 0 && rt->lht != NULL)
		free(rt->lht);
	if (rt->pl_c != 0 && rt->pln != NULL)
		free(rt->pln);
	if (rt->sp_c != 0 && rt->sph != NULL)
		free(rt->sph);
	if (rt->cr_c != 0 && rt->clr != NULL)
		free(rt->clr);
	if (rt->cn_c != 0 && rt->cne != NULL)
		free(rt->cne);
	SDL_DestroyWindow(rt->sdl.win);
	SDL_Quit();
	exit(0);
}

void		*alloc_mem(t_rt *rt)
{
	if (rt->lt_c != 0)
		rt->lht = (t_light*)malloc(sizeof(t_light) * rt->lt_c);
	if (rt->pl_c != 0)
		rt->pln = (t_plane*)malloc(sizeof(t_plane) * rt->pl_c);
	if (rt->sp_c != 0)
		rt->sph = (t_sphere*)malloc(sizeof(t_sphere) * rt->sp_c);
	if (rt->cr_c != 0)
		rt->clr = (t_cyli*)malloc(sizeof(t_cyli) * rt->cr_c);
	if (rt->cn_c != 0)
		rt->cne = (t_cone *)malloc(sizeof(t_cone) * rt->cn_c);
	if ((rt->lt_c != 0 && rt->lht == NULL)
		|| (rt->pl_c != 0 && rt->pln == NULL)
			|| (rt->sp_c != 0 && rt->sph == NULL)
				|| (rt->cr_c != 0 && rt->clr == NULL)
					|| (rt->cn_c != 0 && rt->cne == NULL))
		mem_free(rt);
	return (rt);
}
