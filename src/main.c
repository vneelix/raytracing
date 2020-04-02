/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/24 20:17:14 by vneelix           #+#    #+#             */
/*   Updated: 2020/02/27 17:48:19 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

/*int				init_scene1(t_rt *rt)
{
	rt->lt_c = 1;
	rt->pl_c = 1;
	rt->sp_c = 3;
	rt->cr_c = 0;
	rt->cn_c = 0;

	rt = alloc_mem(rt);
	(rt->lht)[0] = (t_light){0, {0, 0, 0}, {0, 0, 0}, 0.6};
	(rt->pln)[0] = (t_plane){{0, 25, 0}, {0, -1, 0}, {0x0000ff, 5000, 0.0}, 0};
	(rt->sph)[0] = (t_sphere){{0, -5, 100}, 10, {0x0000ff, 5000, 0.5, 0.0}, 0, 0};
	(rt->sph)[1] = (t_sphere){{-50, -10, 200}, 30, {0xff6699, 5000, 0.0}, 0, 0};
	(rt->sph)[2] = (t_sphere){{50, -10, 200}, 30, {0xff6699, 5000, 0.75}, 0, 0};
	return (0);
}

static void			init_scene2_1(t_rt *rt)
{
	rt->lt_c = 1;
	rt->sp_c = 13;
	rt->pl_c = 3;
	rt->cr_c = 6;
	rt->cn_c = 0;
	rt = alloc_mem(rt);
	(rt->lht)[0] = (t_light){0, {0, -5, 0}, {0, 0, 0}, 0.6};
	(rt->pln)[0] = (t_plane){{0, 25, 0}, {0, -1, 0}, {0xe0ddd2, 5000, 0.6}, 0};
	(rt->pln)[1] = (t_plane){{0, -25, 0}, {0, 1, 0}, {0xe0ddd2, 5000, 0.6}, 0};
	(rt->pln)[2] = (t_plane){{0, 0, 150}, {0, 0, -1}, {0x0000ff, 5000, 0.6}, 0};
	(rt->clr)[0] = (t_cyli){{-20, 0, 50}, {0, -1, 0}, 5,
		{0xe0ddd2, 5000, 0.7}, 0, 0};
	(rt->clr)[1] = (t_cyli){{20, 0, 50}, {0, -1, 0}, 5,
		{0xe0ddd2, 5000, 0.7}, 0, 0};
	(rt->clr)[2] = (t_cyli){{-20, 0, 90}, {0, -1, 0}, 5,
		{0xe0ddd2, 5000, 0.7}, 0, 0};
	(rt->clr)[3] = (t_cyli){{20, 0, 90}, {0, -1, 0}, 5,
		{0xe0ddd2, 5000, 0.7}, 0, 0};
	(rt->clr)[4] = (t_cyli){{-20, 0, 130}, {0, -1, 0}, 5,
		{0xe0ddd2, 5000, 0.7}, 0, 0};
}

static void			init_scene2_2(t_rt *rt)
{
	(rt->clr)[5] = (t_cyli){{20, 0, 130}, {0, -1, 0}, 5,
							{0xe0ddd2, 5000, 0.7}, 0, 0};
	(rt->sph)[0] = (t_sphere){{-20, 25, 90}, 8.0, {0xe0ddd2, 5000, 0.7}, 0, 0};
	(rt->sph)[1] = (t_sphere){{20, 25, 90}, 8, {0xe0ddd2, 5000, 0.7}, 0, 0};
	(rt->sph)[2] = (t_sphere){{-20, -25, 90}, 8, {0xe0ddd2, 5000, 0.7}, 0, 0};
	(rt->sph)[3] = (t_sphere){{20, -25, 90}, 8, {0xe0ddd2, 5000, 0.7}, 0, 0};
	(rt->sph)[4] = (t_sphere){{-20, 25, 130}, 8, {0xe0ddd2, 5000, 0.7}, 0, 0};
	(rt->sph)[5] = (t_sphere){{20, 25, 130}, 8, {0xe0ddd2, 5000, 0.7}, 0, 0};
	(rt->sph)[6] = (t_sphere){{-20, -25, 130}, 8, {0xe0ddd2, 5000, 0.7}, 0, 0};
	(rt->sph)[7] = (t_sphere){{20, -25, 130}, 8, {0xe0ddd2, 5000, 0.7}, 0, 0};
	(rt->sph)[8] = (t_sphere){{-20, 25, 50}, 8, {0xe0ddd2, 5000, 0.7}, 0, 0};
}

void				init_scene2(t_rt *rt)
{
	init_scene2_1(rt);
	init_scene2_2(rt);
	(rt->sph)[9] = (t_sphere){{20, 25, 50}, 8, {0xe0ddd2, 5000, 0.7}, 0, 0};
	(rt->sph)[10] = (t_sphere){{-20, -25, 50}, 8, {0xe0ddd2, 5000, 0.7}, 0, 0};
	(rt->sph)[11] = (t_sphere){{20, -25, 50}, 8, {0xe0ddd2, 5000, 0.7}, 0, 0};
	(rt->sph)[12] = (t_sphere){{0, 0, 70}, 8, {0x0000ff, 5000, 0.5}, 0, 0};
}

void		init_scene4(t_rt *rt)
{
	rt->lt_c = 2;
	rt->sp_c = 4;
	rt->pl_c = 0;
	rt->cr_c = 1;
	rt->cn_c = 0;
	rt = alloc_mem(rt);
	(rt->lht)[0] = (t_light){0, {2, -1, 0}, {0, 0, 0}, 0.55};
	(rt->lht)[1] = (t_light){0, {-2, -1, 0}, {0, 0, 0}, 0.7};
	(rt->sph)[0] = (t_sphere){{0, 1, 3}, 1, {0xff0000, 5000, 0.0, 1.3}, 0, 0};
	(rt->sph)[1] = (t_sphere){{2, 0, 4}, 1, {0x0000ff, 5000, 0.0}, 0, 0};
	(rt->sph)[2] = (t_sphere){{-2, 0, 4}, 1, {0x00ff00, 5000, 0.0}, 0, 0};
	(rt->sph)[3] = (t_sphere){{0, 5001, 0}, 5000, {0x00d9d9, 5000, 0.0}, 0, 0};
	(rt->clr)[0] = (t_cyli){{0, 0, 8}, {0, -1, 0}, 1, {0x0000ff, 5000, 0.0, 0.0}, 0, 0};
}*/

void*		opencl_scan(char **files, cl_uint count)
{
	cl_uint		i;
	cl_int		ret;
	char**		sources;

	if ((sources = (char**)malloc(sizeof(char*) * count)) == NULL)
		return (NULL);
	i = 0;
	while (i < count)
	{
		if ((ret = open(files[i], O_RDONLY)) < 0)
			break ;
		if ((sources[i] = ft_getfile(ret)) == NULL)
		{
			close(ret);
			break ;
		}
		close(ret);
		i += 1;
	}
	if (i != count)
	{
		i -= (i != 0 ? 1 : 0);
		while (i > 0)
			free(files[i--]);
		free(files[i--]);
		free(files);
	}
	return (sources);
}

cl_int		opencl_init(t_opencl *cl, char **sources, cl_uint count, void *arg);
int			get_scene(char *file_name);

int				main(void)
{
	t_rt	*rt;
	cl_uint	count = 5;
	char	*files[] = {"main_func.cl", "plane.cl", "sphere.cl", "cylinder.cl", "cone.cl"};
	char	**sources;

	get_scene("file");
	/*rt = malloc(sizeof(t_rt));
	if (sdl_init(&(rt->sdl)))
		return (-1);
	sources = (char**)(opencl_scan(files, count));
	if (opencl_init(&rt->cl, sources, count, &(rt->sdl)))
		return (-1);
	SDL_UpdateWindowSurface(rt->sdl.win);
	sdl_loop(&(rt->sdl), rt);*/
	return (0);
}
