/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/24 20:17:14 by vneelix           #+#    #+#             */
/*   Updated: 2020/07/24 12:00:01 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

int		main(int argc, char **argv)
{
	t_rt	*rt;

	if (argc != 2)
		return (-1);
	rt = ft_memalloc(sizeof(t_rt));
	/*if (get_scene(rt, argv[1]))
	{
		printf("Scene file error\n");
		return (-1);
	}*/
	if (sdl_init(&(rt->sdl)))
		return (-1);
	rt->opt.w = W;
	rt->opt.h = H;
	rt->opt.spin_x = 0;
	rt->opt.center = (cl_float3){{0, 0, 0}};
	rt->sdl.ptr = malloc(rt->opt.w * rt->opt.h * 4);
	if (opencl_create_infrastructure(&rt->cl,
		"cl_src/", "cl_inc/") || opencl_init(&rt->cl, rt))
		return (-1);
	SDL_UpdateTexture(rt->sdl.tex, NULL, rt->sdl.ptr, rt->opt.w * 4);
	SDL_RenderCopy(rt->sdl.ren, rt->sdl.tex, NULL, NULL);
	SDL_RenderPresent(rt->sdl.ren);
	sdl_loop(&(rt->sdl), rt);
	return (0);
}
