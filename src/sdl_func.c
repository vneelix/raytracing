#include "rtv1.h"

int		sdl_init(t_sdl *sdl)
{
	if (SDL_Init(SDL_INIT_EVERYTHING))
		return (-1);
	if ((sdl->win = SDL_CreateWindow("RT",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			W, H, SDL_WINDOW_SHOWN)) == NULL)
	{
		SDL_Quit();
		return (-1);
	}
	if ((sdl->ren = SDL_CreateRenderer(sdl->win, -1, SDL_RENDERER_ACCELERATED)) == NULL)
	{
		SDL_DestroyWindow(sdl->win);
		SDL_Quit();
		return (-1);
	}
	if ((sdl->tex = SDL_CreateTexture(sdl->ren, SDL_PIXELFORMAT_BGRA32, SDL_TEXTUREACCESS_STREAMING, W, H)) == NULL)
	{
		SDL_DestroyRenderer(sdl->ren);
		SDL_DestroyWindow(sdl->win);
		return (-1);
	}
	return (0);
}

void	correct_queue(void)
{
	SDL_EventState(SDL_KEYDOWN, SDL_DISABLE);
	SDL_EventState(SDL_KEYDOWN, SDL_ENABLE);
	SDL_EventState(SDL_MOUSEMOTION, SDL_DISABLE);
	SDL_EventState(SDL_MOUSEMOTION, SDL_ENABLE);
}

int		sdl_mousehook(t_sdl *sdl, t_rt *rt)
{
	rt->opt.spin_x -= atanf(sdl->event.motion.yrel) * 0.02;
	rt->opt.spin_y += atanf(sdl->event.motion.xrel) * 0.02;
	return (1);
}

int		sdl_keyhook(SDL_Keycode keycode, t_rt *rt)
{
	/*if (!(keycode == SDLK_UP || keycode == SDLK_DOWN
		|| keycode == SDLK_LEFT || keycode == SDLK_RIGHT))
		return (0);*/
	if (keycode == SDLK_UP)
		(rt->opt.center.s)[2] += 0.5;
	else if (keycode == SDLK_DOWN)
		(rt->opt.center.s)[2] -= 0.5;
	else if (keycode == SDLK_LEFT)
		(rt->opt.center.s)[0] -= 0.5;
	else if (keycode == SDLK_RIGHT)
		(rt->opt.center.s)[0] += 0.5;
	else if (keycode == SDLK_RETURN)
		(rt->opt.center.s)[1] -= 0.5;
	else if (keycode == SDLK_BACKSPACE)
		(rt->opt.center.s)[1] += 0.5;
	return (1);
}

int		sdl_loop(t_sdl *sdl, t_rt *rt)
{
	int	ret;
	int	quit;

	ret = 0;
	quit = 0;
	while (quit == 0)
	{
		if (SDL_PollEvent(&(sdl->event)))
		{
			if (sdl->event.type == SDL_QUIT
				|| (sdl->event.key.type == SDL_KEYUP
					&& sdl->event.key.keysym.sym == SDLK_ESCAPE))
				quit = 1;
			else if (sdl->event.type == SDL_MOUSEMOTION)
				ret = sdl_mousehook(sdl, rt);
			else if (sdl->event.type == SDL_KEYDOWN)
				ret = sdl_keyhook(sdl->event.key.keysym.sym, rt);
			if (ret)
			{
				ret = opencl_launch(&(rt->cl), rt);
				SDL_UpdateTexture(sdl->tex, NULL, sdl->ptr, rt->opt.w * 4);
				SDL_RenderCopy(sdl->ren, sdl->tex, NULL, NULL);
				SDL_RenderPresent(sdl->ren);
			}
			correct_queue();
		}
	}
	return (0);
}