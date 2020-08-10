/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard_control.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/31 10:20:42 by mdirect           #+#    #+#             */
/*   Updated: 2020/08/08 17:22:19 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int		sdl_iskey(SDL_Keycode c)
{
	return (!(c == SDLK_UP || c == SDLK_DOWN || c == SDLK_LEFT
	|| c == SDLK_RIGHT || c == SDLK_RETURN || c == SDLK_BACKSPACE
	|| c == SDLK_h || c == SDLK_l || c == SDLK_c));
}

int		sdl_keyhook(SDL_Keycode keycode, t_rt *rt)
{
	cl_uint flags;

	flags = 0;
	if (sdl_iskey(keycode))
		return (0);
	if (keycode == SDLK_h && !rt->flag.col)
		rt->flag.help = !rt->flag.help;
	if (keycode == SDLK_c && !rt->flag.help)
		rt->flag.col = !rt->flag.col;
	else if (keycode == SDLK_UP)
		flags |= (cl_uint)1 << 0;
	else if (keycode == SDLK_DOWN)
		flags |= (cl_uint)1 << 1;
	else if (keycode == SDLK_LEFT)
		flags |= (cl_uint)1 << 2;
	else if (keycode == SDLK_RIGHT)
		flags |= (cl_uint)1 << 3;
	else if (keycode == SDLK_RETURN)
		flags |= (cl_uint)1 << 4;
	else if (keycode == SDLK_BACKSPACE)
		flags |= (cl_uint)1 << 5;
	if (flags)
		if (move_origin_kernel_launch(&rt->cl, flags))
			close_programm(rt, "Move kernel error.");
	return (1);
}
