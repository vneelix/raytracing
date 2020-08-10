/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   menu.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/24 18:44:58 by mdirect           #+#    #+#             */
/*   Updated: 2020/08/08 17:13:10 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	change_color(t_rt *rt)
{
	SDL_Rect	rectangle;

	rectangle = (SDL_Rect){10, 10, 210, 110};
	if (SDL_SetRenderDrawBlendMode(rt->sdl.ren, SDL_BLENDMODE_MOD)
	|| SDL_SetRenderDrawColor(rt->sdl.ren, 70, 70, 70, 0)
	|| SDL_RenderFillRect(rt->sdl.ren, &rectangle)
	|| SDL_SetRenderDrawBlendMode(rt->sdl.ren, SDL_BLENDMODE_NONE)
	|| SDL_SetRenderDrawColor(rt->sdl.ren, 0, 0, 0, 0)
	|| SDL_RenderDrawRect(rt->sdl.ren, &rectangle))
		close_programm(rt, "Change color menu error.");
	rectangle = COL_MENU;
	if (SDL_RenderCopy(rt->sdl.ren, rt->intrf.image, NULL, &rectangle))
		close_programm(rt, "Change color menu error.");
}

void	present_win(t_rt *rt)
{
	if (SDL_UpdateTexture(rt->sdl.tex, NULL, rt->sdl.ptr, rt->opt.w * 4))
		close_programm(rt, "Present win error.");
	if (SDL_RenderCopy(rt->sdl.ren, rt->sdl.tex, NULL, NULL))
		close_programm(rt, "Present win error.");
	if (rt->flag.help && !rt->flag.col)
		help(rt);
	if (rt->flag.col && !rt->flag.help)
		change_color(rt);
	SDL_SetRenderDrawColor(rt->sdl.ren, 0, 0, 0, 0);
	SDL_RenderPresent(rt->sdl.ren);
}

void	help(t_rt *rt)
{
	SDL_Rect rectangle;

	rectangle = (SDL_Rect){W * D_W, H * D_H,
	W * (1 - 2 * D_W), H * (1 - 2 * D_H)};
	if (SDL_SetRenderDrawBlendMode(rt->sdl.ren, SDL_BLENDMODE_MOD)
	|| SDL_SetRenderDrawColor(rt->sdl.ren, 70, 70, 70, 0)
	|| SDL_RenderFillRect(rt->sdl.ren, &rectangle)
	|| SDL_SetRenderDrawBlendMode(rt->sdl.ren, SDL_BLENDMODE_NONE)
	|| SDL_SetRenderDrawColor(rt->sdl.ren, 0, 0, 0, 0)
	|| SDL_RenderDrawRect(rt->sdl.ren, &rectangle)
	|| SDL_RenderCopy(rt->sdl.ren, rt->intrf.menu, NULL, &rectangle))
		close_programm(rt, "Help print error.");
}
