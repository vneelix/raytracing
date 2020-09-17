/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   control.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/12 15:12:23 by vneelix           #+#    #+#             */
/*   Updated: 2020/09/12 15:12:24 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void	correct_queue(void)
{
	SDL_EventState(SDL_KEYDOWN, SDL_DISABLE);
	SDL_EventState(SDL_KEYDOWN, SDL_ENABLE);
	SDL_EventState(SDL_MOUSEMOTION, SDL_DISABLE);
	SDL_EventState(SDL_MOUSEMOTION, SDL_ENABLE);
}

static void	check_ret(t_rt *rt, cl_int ret)
{
	if (ret)
	{
		if (opencl_launch(&(rt->cl), rt))
			close_programm(rt, "Renderer kernel error.");
		if (rt->ppf && post_processing_kernel_launch(&rt->cl, rt))
			close_programm(rt, "Post-process kernel error.");
		present_win(rt);
	}
	correct_queue();
}

int			sdl_loop(t_sdl *sdl, t_rt *rt)
{
	int	ret;

	while (SDL_WaitEvent(&(sdl->event)))
	{
		ret = 0;
		if (sdl->event.type == SDL_QUIT
			|| (sdl->event.key.type == SDL_KEYUP
				&& sdl->event.key.keysym.sym == SDLK_ESCAPE))
			break ;
		else if (sdl->event.type == SDL_KEYDOWN)
			ret = sdl_keyhook(sdl->event.key.keysym.sym, rt);
		else if (sdl->event.type == SDL_MOUSEBUTTONDOWN
		|| sdl->event.type == SDL_MOUSEBUTTONUP)
			ret = push_mouse(&rt->sdl, rt);
		else if (sdl->event.type == SDL_MOUSEMOTION)
			ret = sdl_mousehook(&rt->sdl, rt);
		check_ret(rt, ret);
	}
	return (0);
}
