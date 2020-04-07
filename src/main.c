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
			free(sources[i--]);
		free(sources[i]);
		free(sources);
	}
	return (sources);
}

int				main(void)
{
	t_rt	*rt;
	cl_uint	count = 7;
	char	*files[] = {"main_func.cl", "plane.cl",
		"sphere.cl", "cylinder.cl", "cone.cl", "func.cl", "paraboloid.cl"};
	char	**sources;

	rt = malloc(sizeof(t_rt));
	ft_bzero(rt, sizeof(t_rt));
	if (get_scene(rt, "file"))
	{
		printf("Scene file error\n");
		return (-1);
	}
	if (sdl_init(&(rt->sdl)))
		return (-1);
	//SDL_ShowCursor(SDL_DISABLE);
	SDL_SetRelativeMouseMode(SDL_ENABLE);
	rt->opt.w = rt->sdl.surf->w;
	rt->opt.h = rt->sdl.surf->h;
	rt->opt.center = (cl_float3){{0, 0, 0}};
	sources = opencl_scan(files, count);
	if (opencl_init(&(rt->cl), sources, count, rt))
		return (-1);
	SDL_UpdateWindowSurface(rt->sdl.win);
	sdl_loop(&(rt->sdl), rt);
	return (0);
}
