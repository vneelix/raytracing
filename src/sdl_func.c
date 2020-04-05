#include "rtv1.h"

int		sdl_init(t_sdl *sdl)
{
	if (SDL_Init(SDL_INIT_EVERYTHING))
		return (-1);
	if ((sdl->win = SDL_CreateWindow("RTv1",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			W, H, SDL_WINDOW_SHOWN | SDL_WINDOW_INPUT_GRABBED)) == NULL)
	{
		SDL_Quit();
		return (-1);
	}
	if ((sdl->surf = SDL_GetWindowSurface(sdl->win)) == NULL)
	{
		SDL_DestroyWindow(sdl->win);
		SDL_Quit();
		return (-1);
	}
	return (0);
}

void	correct_queue(void)
{
	SDL_EventState(SDL_KEYDOWN, SDL_DISABLE);
	SDL_EventState(SDL_KEYDOWN, SDL_ENABLE);
	SDL_EventState(SDL_MOUSEWHEEL, SDL_DISABLE);
	SDL_EventState(SDL_MOUSEWHEEL, SDL_ENABLE);
	SDL_EventState(SDL_MOUSEMOTION, SDL_DISABLE);
	SDL_EventState(SDL_MOUSEMOTION, SDL_ENABLE);
}

int		sdl_keyhook(SDL_Keycode key, t_rt *rt)
{
	SDL_Keycode available_keys = SDLK_UP | SDLK_DOWN | SDLK_LEFT | SDLK_RIGHT;

	if (key == SDLK_UP)
		(rt->opt.center.s)[2] += 1;
	else if (key == SDLK_DOWN)
		(rt->opt.center.s)[2] -= 1;
	else if (key == SDLK_LEFT)
		(rt->opt.center.s)[0] -= 1;
	else if (key == SDLK_RIGHT)
		(rt->opt.center.s)[0] += 1;
	if (key & available_keys)
		return (1);
	return (0);
}

int		sdl_mousehook(t_sdl *sdl, t_rt *rt)
{
	rt->opt.spin_y += atanf(0.05 * sdl->event.motion.xrel) * 180.f / CL_M_PI;
	rt->opt.spin_x -= atanf(0.05 * sdl->event.motion.yrel) * 180.f / CL_M_PI;
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
			{
				quit = 1;
			}
			else if (sdl->event.type == SDL_KEYDOWN)
				ret = sdl_keyhook(sdl->event.key.keysym.sym, rt);
			/*else if (sdl->event.type == SDL_MOUSEMOTION)
				ret = sdl_mousehook(sdl, rt);*/
			if (ret)
			{
				ret = opencl_launch(&(rt->cl), rt);
				SDL_UpdateWindowSurface(rt->sdl.win);
			}
			correct_queue();
		}
	}
	return (0);
}