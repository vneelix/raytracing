/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard_control.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/12 15:20:23 by vneelix           #+#    #+#             */
/*   Updated: 2020/09/13 09:47:58 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static int	change_post(SDL_Keycode keycode, t_rt *rt)
{
	if (keycode == SDLK_1)
		rt->ppf = rt->ppf == 1 ? 0 : 1;
	else if (keycode == SDLK_2)
		rt->ppf = rt->ppf == 2 ? 0 : 2;
	else if (keycode == SDLK_3)
		rt->ppf = rt->ppf == 4 ? 0 : 4;
	else if (keycode == SDLK_4)
		rt->ppf = rt->ppf == 8 ? 0 : 8;
	else if (keycode == SDLK_5)
		rt->ppf = rt->ppf == 16 ? 0 : 16;
	else
		return (0);
	if (rt->ppf && post_processing_kernel_launch(&rt->cl, rt))
		close_programm(rt, "Post-process kernel error.");
	if (rt->ppf)
		present_win(rt);
	return (1);
}

static int	change_mode(SDL_Keycode keycode, t_rt *rt)
{
	if (!(rt->opt.flags & FAST_RENDER) && keycode == SDLK_q)
		rt->opt.flags = rt->opt.flags & AMBIENT
			? rt->opt.flags & ~AMBIENT : rt->opt.flags | AMBIENT;
	else if (!(rt->opt.flags & FAST_RENDER) && keycode == SDLK_w)
		rt->opt.flags = rt->opt.flags & ALIASING
			? rt->opt.flags & ~ALIASING : rt->opt.flags | ALIASING;
	else if (!(rt->opt.flags & FAST_RENDER) && keycode == SDLK_e)
		rt->opt.flags = rt->opt.flags & RECURSION
			? rt->opt.flags & ~RECURSION : rt->opt.flags | RECURSION;
	else if (!(rt->opt.flags & FAST_RENDER) && keycode == SDLK_r)
		rt->opt.flags = rt->opt.flags & SOFT_SHADOWS
			? rt->opt.flags & ~SOFT_SHADOWS : rt->opt.flags | SOFT_SHADOWS;
	else if (keycode == SDLK_t)
		rt->opt.flags = rt->opt.flags & FAST_RENDER
			? rt->opt.flags & ~FAST_RENDER : rt->opt.flags | FAST_RENDER;
	return (1);
}

static int	sdl_iskey(SDL_Keycode c)
{
	if (c == SDLK_UP || c == SDLK_DOWN
		|| c == SDLK_LEFT || c == SDLK_RIGHT
			|| c == SDLK_RETURN || c == SDLK_BACKSPACE
				|| c == SDLK_h || c == SDLK_5
					|| c == SDLK_c || c == SDLK_q
						|| c == SDLK_w || c == SDLK_e
							|| c == SDLK_r || c == SDLK_t
								|| c == SDLK_1 || c == SDLK_2
									|| c == SDLK_3 || c == SDLK_4)
		return (c);
	return (0);
}

static void	fill_flags(SDL_Keycode keycode, t_rt *rt, cl_uint *flags)
{
	rt = NULL;
	if (keycode == SDLK_UP)
		*flags |= (cl_uint)1 << 0;
	else if (keycode == SDLK_DOWN)
		*flags |= (cl_uint)1 << 1;
	else if (keycode == SDLK_LEFT)
		*flags |= (cl_uint)1 << 2;
	else if (keycode == SDLK_RIGHT)
		*flags |= (cl_uint)1 << 3;
	else if (keycode == SDLK_RETURN)
		*flags |= (cl_uint)1 << 4;
	else if (keycode == SDLK_BACKSPACE)
		*flags |= (cl_uint)1 << 5;
	else
		return ;
}

int			sdl_keyhook(SDL_Keycode keycode, t_rt *rt)
{
	cl_uint flags;

	flags = 0;
	if (!sdl_iskey(keycode) || change_post(keycode, rt))
		return (0);
	if (keycode == SDLK_h || keycode == SDLK_c)
	{
		if (keycode == SDLK_c)
			rt->flag.col = !rt->flag.col;
		if (keycode == SDLK_h)
			rt->flag.help = !rt->flag.help;
		present_win(rt);
		return (0);
	}
	fill_flags(keycode, rt, &flags);
	if (flags)
	{
		rt->opt.flags |= rt->opt.flags & FAST_RENDER ? 0 : FAST_RENDER;
		if (move_origin_kernel_launch(&rt->cl, flags))
			close_programm(rt, "Move kernel error.");
	}
	else
		change_mode(keycode, rt);
	return (1);
}
