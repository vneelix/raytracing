/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/12 15:20:31 by vneelix           #+#    #+#             */
/*   Updated: 2020/09/13 10:12:15 by vneelix          ###   ########.fr       */
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

void		func_rt(t_rt *rt)
{
	if (!rt)
		return ;
	rt->opt.diffuse = rt->cam.k;
	rt->opt.background = rt->cam.color;
}

int			main(int argc, char **argv)
{
	t_rt	*rt;

	if (argc != 2)
		return (-1);
	rt = init_main(argv[1]);
	func_rt(rt);
	if (sdl_init(&(rt->sdl)))
		close_programm(rt, "SDL init error.");
	if ((IMG_Init(IMG_INIT_PNG) < 0) || (sdl_image_init(rt) == -1))
		close_programm(rt, "SDL image error.");
	rt->opt.w = W;
	rt->opt.h = H;
	rt->opt.center = rt->cam.center;
	if (!(rt->sdl.ptr = ft_memalloc(sizeof(uint32_t) * W * H)))
		close_programm(rt, "SDL image error.");
	if (opencl_create_infrastructure(&rt->cl) || opencl_init(&rt->cl, rt))
		close_programm(rt, "OpenCL error.");
	present_win(rt);
	sdl_loop(&(rt->sdl), rt);
	close_programm(rt, NULL);
	return (0);
}
