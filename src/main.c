/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/24 20:17:14 by vneelix           #+#    #+#             */
/*   Updated: 2020/06/27 18:35:55 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

char	**cl_source(void)
{
	size_t	i;
	char	*ptr;
	char	**ret;
	char	**file;
	size_t	num_files;

	if ((file = ft_directory("cl_src", &num_files)) == NULL)
		return (NULL);
	if ((ret = (char**)ft_memalloc(sizeof(char*) * (num_files + 1))) == NULL)
		return (nptr_del((void**)file));
	i = 0;
	while (i != num_files)
	{
		if ((ptr = ft_strjoin("cl_src/", file[i])) == NULL)
			break ;
		if ((ret[i++] = ft_getfile(ptr, 0)) == NULL)
			break ;
		ft_memdel(ptr);
	}
	ptr = (i == num_files ? NULL : ptr);
	ft_memdel(ptr);
	nptr_del((void**)file);
	if (i != num_files)
		return (nptr_del((void**)ret));
	return (ret);
}

int		main(int argc, char **argv)
{
	t_rt	*rt;
	char	**source;

	argv = argv;
	if (argc != 2)
		return (-1);
	rt = malloc(sizeof(t_rt));
	ft_bzero(rt, sizeof(t_rt));
	/*if (get_scene(rt, argv[1]))
	{
		printf("Scene file error\n");
		return (-1);
	}*/
	if (sdl_init(&(rt->sdl)))
		return (-1);
	SDL_SetRelativeMouseMode(SDL_ENABLE);
	rt->opt.w = W;
	rt->opt.h = H;
	rt->opt.center = (cl_float3){{0, 0, 0}};
	rt->sdl.ptr = malloc(rt->opt.w * rt->opt.h * 4);
	if ((source = cl_source()) == NULL)
		return (-1);
	if (opencl_init(&(rt->cl), source, rt))
	{
		perror(NULL);
		return (-1);
	}
	SDL_UpdateTexture(rt->sdl.tex, NULL, rt->sdl.ptr, rt->opt.w * 4);
	SDL_RenderCopy(rt->sdl.ren, rt->sdl.tex, NULL, NULL);
	SDL_RenderPresent(rt->sdl.ren);
	sdl_loop(&(rt->sdl), rt);
	return (0);
}
