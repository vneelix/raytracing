/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/24 20:17:14 by vneelix           #+#    #+#             */
/*   Updated: 2020/08/12 13:18:06 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void		close_programm(t_rt *rt, char *message)
{
	if (message)
		ft_putendl(message);
	free_rt(rt);
	exit(0);
}

static t_rt	*init_main(char *s)
{
	t_rt	*rt;

	if (!(rt = ft_memalloc(sizeof(t_rt))))
		return (NULL);
	if ((rt->opt.texture_c = get_file_name(rt)) > -1)
	{
		if (read_texture(rt))
			close_programm(rt, "Read textr failure.");
	}
	else
		close_programm(rt, "Get textr failure.");
	if (get_scene(rt, s))
		close_programm(rt, "Scene file error.");
	set_item_par(rt->item, rt->opt.item_c);
	return (rt);
}

int			main(int argc, char **argv)
{
	t_rt	*rt;

	if (argc != 2)
		return (-1);
	rt = init_main(argv[1]);
	if (sdl_init(&(rt->sdl)))
		close_programm(rt, "SDL init error.");
	if ((IMG_Init(IMG_INIT_PNG) < 0) || (sdl_image_init(rt) == -1))
		close_programm(rt, "SDL image error.");
	rt->opt.w = W;
	rt->opt.h = H;
	rt->opt.center = rt->cam.center;
	if (!(rt->sdl.ptr = ft_memalloc(sizeof(__uint32_t) * W * H)))
		close_programm(rt, "SDL image error.");
	if (opencl_create_infrastructure(&rt->cl,
		"cl_src/", "cl_inc/") || opencl_init(&rt->cl, rt))
		close_programm(rt, "OpenCL error.");
	present_win(rt);
	sdl_loop(&(rt->sdl), rt);
	close_programm(rt, NULL);
	return (0);
}
