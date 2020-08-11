/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard_control.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/31 10:20:42 by mdirect           #+#    #+#             */
/*   Updated: 2020/08/11 13:51:42 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int		change_mode(SDL_KeyCode keycode, t_rt *rt)
{
	if (keycode == SDLK_q)
		rt->opt.flags = rt->opt.flags & AMBIENT
			? rt->opt.flags & ~AMBIENT : rt->opt.flags | AMBIENT;
	else if (keycode == SDLK_w)
		rt->opt.flags = rt->opt.flags & ALIASING
			? rt->opt.flags & ~ALIASING : rt->opt.flags | ALIASING;
	else if (keycode == SDLK_e)
		rt->opt.flags = rt->opt.flags & RECURSION
			? rt->opt.flags & ~RECURSION : rt->opt.flags | RECURSION;
	else if (keycode == SDLK_r)
		rt->opt.flags = rt->opt.flags & SOFT_SHADOWS
			? rt->opt.flags & ~SOFT_SHADOWS : rt->opt.flags | SOFT_SHADOWS;
	else if (keycode == SDLK_t)
		rt->opt.flags = rt->opt.flags & FAST_RENDER
			? rt->opt.flags & ~FAST_RENDER : rt->opt.flags | FAST_RENDER;
	return (1);
}

int		sdl_iskey(SDL_Keycode c)
{
	if (c == SDLK_UP || c == SDLK_DOWN || c == SDLK_LEFT
		|| c == SDLK_RIGHT || c == SDLK_RETURN || c == SDLK_BACKSPACE
			|| c == SDLK_h || c == SDLK_l || c == SDLK_c || c == SDLK_q
				|| c == SDLK_w || c == SDLK_e || c == SDLK_r || c == SDLK_t)
		return (c);
	return (0);
}

int		sdl_keyhook(SDL_Keycode keycode, t_rt *rt)
{
	cl_uint flags;

	flags = 0;
	if (!sdl_iskey(keycode))
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
	{
		if (move_origin_kernel_launch(&rt->cl, flags))
			close_programm(rt, "Move kernel error.");
	}
	else
		change_mode(keycode, rt);
	return (1);
}
