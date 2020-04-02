/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/26 13:12:25 by vneelix           #+#    #+#             */
/*   Updated: 2020/02/27 17:25:13 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

static void	*thread_draw(void *arg)
{
	int			i;
	int			j;
	t_thread	*obj;

	obj = (t_thread*)arg;
	i = obj->min - (W / 2);
	j = -W / 2;
	obj->rt.view.v.z = obj->rt.view.d + obj->rt.view.c.z;
	while (i != obj->max - (W / 2))
	{
		obj->rt.view.v.y = i * (obj->rt.view.d / H) + obj->rt.view.c.y;
		while (j != W / 2)
		{
			obj->rt.view.v.x = j * (obj->rt.view.d / W) + obj->rt.view.c.x;
			obj->rt.view.dir =
				subtract_point_from_point(obj->rt.view.c, obj->rt.view.v);
			ft_rotation(&(obj->rt));
			((__uint32_t*)(obj->rt.sdl.surf->pixels))
			[(j + W / 2) + W * (i + H / 2)] = raytrace(&(obj->rt));
			j += 1;
		}
		j = -W / 2;
		i += 1;
	}
	pthread_exit(NULL);
}

static void	ft_exit(t_rt *rt)
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
}

static int	struct_copy(t_rt *n_rt, t_rt *rt)
{
	*n_rt = *rt;
	if (rt->lt_c != 0)
		n_rt->lht = ft_memcpy(ft_memalloc(
			sizeof(t_light) * rt->lt_c), rt->lht, sizeof(t_light) * rt->lt_c);
	if (rt->pl_c != 0)
		n_rt->pln = ft_memcpy(ft_memalloc(
			sizeof(t_plane) * rt->pl_c), rt->pln, sizeof(t_plane) * rt->pl_c);
	if (rt->sp_c != 0)
		n_rt->sph = ft_memcpy(ft_memalloc(
			sizeof(t_sphere) * rt->sp_c), rt->sph, sizeof(t_sphere) * rt->sp_c);
	if (rt->cr_c != 0)
		n_rt->clr = ft_memcpy(ft_memalloc(
			sizeof(t_cyli) * rt->cr_c), rt->clr, sizeof(t_cyli) * rt->cr_c);
	if (rt->cn_c != 0)
		n_rt->cne = ft_memcpy(ft_memalloc(
			sizeof(t_cone) * rt->cn_c), rt->cne, sizeof(t_cone) * rt->cn_c);
	if ((n_rt->lt_c != 0 && n_rt->lht == NULL) || (n_rt->pl_c != 0
		&& n_rt->pln == NULL) || (n_rt->sp_c != 0 && n_rt->sph == NULL)
			|| (n_rt->cr_c != 0 && n_rt->clr == NULL)
				|| (n_rt->cn_c != 0 && n_rt->cne == NULL))
	{
		ft_exit(n_rt);
		return (1);
	}
	return (0);
}

void		thread_start(t_thread *obj, pthread_t *tid, t_rt *rt, int *i)
{
	*i = 0;
	while (*i != T)
	{
		if (struct_copy(&((obj + *i)->rt), rt))
			break ;
		(obj + *i)->min = H / T * *i;
		(obj + *i)->max = (obj + *i)->min + H / T;
		if (pthread_create(tid + *i, NULL, thread_draw, obj + *i))
		{
			ft_exit(&((obj + *i)->rt));
			break ;
		}
		*i += 1;
	}
}

int			thread_manager(t_rt *rt)
{
	int			i;
	pthread_t	*tid;
	t_thread	*obj;

	if ((tid = (pthread_t*)malloc(sizeof(*tid) * T)) == NULL)
		return (0);
	if ((obj = (t_thread*)malloc(sizeof(*obj) * T)) == NULL)
	{
		free(tid);
		return (0);
	}
	thread_start(obj, tid, rt, &i);
	while (i != 0)
	{
		i -= 1;
		pthread_join(tid[i], NULL);
		ft_exit(&((obj + i)->rt));
	}
	SDL_UpdateWindowSurface(rt->sdl.win);
	free(tid);
	free(obj);
	return (0);
}
