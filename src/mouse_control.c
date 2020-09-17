/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_control.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/12 15:20:42 by vneelix           #+#    #+#             */
/*   Updated: 2020/09/12 15:20:43 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static int	choose_color(t_rt *rt, int x, int y)
{
	int			i;
	Uint32		*p;
	SDL_Color	color;

	if (!(x >= COL_MENU.x && x <= COL_MENU.w + COL_MENU.x
	&& y >= COL_MENU.y && y <= COL_MENU.h + COL_MENU.y))
		return (0);
	p = rt->intrf.spectr->pixels;
	i = (int)((x - COL_MENU.x) * rt->intrf.spectr->w / (float)COL_MENU.w) +
		(int)((y - COL_MENU.y) * rt->intrf.spectr->h / (float)COL_MENU.h) *
		rt->intrf.spectr->w;
	color.r = (p[i] << 8) >> 24;
	color.g = (p[i] << 16) >> 24;
	color.b = (p[i] << 24) >> 24;
	if (change_color_kernel_launch(&rt->cl,
		(cl_float3){{color.r, color.g, color.b}}))
		close_programm(rt, "Change color kernel error.");
	rt->opt.flags |= rt->opt.flags & FAST_RENDER ? 0 : FAST_RENDER;
	return (1);
}

static int	push_mouse_down(t_sdl *sdl, t_rt *rt)
{
	if (sdl->event.button.button == SDL_BUTTON_RIGHT)
		rt->flag.press_rm = 1;
	else if (sdl->event.button.button == SDL_BUTTON_LEFT)
	{
		rt->flag.press_lm = 1;
		if (rt->flag.col == 1)
			return (choose_color(rt, sdl->event.button.x,
			sdl->event.button.y));
		else
		{
			if (find_item_kernel_launch(&rt->cl, sdl->event.motion.x,
			sdl->event.motion.y, &rt->opt))
				close_programm(rt, "Change color menu error.");
			else
			{
				rt->opt.flags |= rt->opt.flags & FAST_RENDER ? 0 : FAST_RENDER;
				return (1);
			}
		}
	}
	return (0);
}

static int	push_mouse_up(t_sdl *sdl, t_rt *rt)
{
	if (sdl->event.button.button == SDL_BUTTON_RIGHT)
		rt->flag.press_rm = 0;
	else if (sdl->event.button.button == SDL_BUTTON_LEFT)
		rt->flag.press_lm = 0;
	return (0);
}

int			push_mouse(t_sdl *sdl, t_rt *rt)
{
	if (sdl->event.type == SDL_MOUSEBUTTONUP)
		return (push_mouse_up(sdl, rt));
	if (sdl->event.type == SDL_MOUSEBUTTONDOWN)
		return (push_mouse_down(sdl, rt));
	return (0);
}

int			sdl_mousehook(t_sdl *sdl, t_rt *rt)
{
	if (rt->flag.press_rm)
	{
		rotate_kernel_launch(
			&rt->cl,
			(cl_float3){{
				atanf(sdl->event.motion.yrel) * 0.024f,
				atanf(sdl->event.motion.xrel) * 0.024f,
				0
			}},
			0);
		rt->opt.flags |= rt->opt.flags & FAST_RENDER ? 0 : FAST_RENDER;
		return (1);
	}
	return (0);
}
