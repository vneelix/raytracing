#include "rtv1.h"

int		sdl_init(t_sdl *sdl)
{
	if (SDL_Init(SDL_INIT_EVERYTHING))
		return (-1);
	if ((sdl->win = SDL_CreateWindow("RTv1",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			W, H, SDL_WINDOW_SHOWN)) == NULL)
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

int		sdl_keyhook(SDL_KeyCode key, t_rt *rt)
{
	if (key == SDLK_UP)
		(rt->sph + 0)->center.z += S;
	else if (key == SDLK_DOWN)
		(rt->sph + 0)->center.z -= S;
	else if (key == SDLK_LEFT)
		(rt->sph + 0)->center.x -= S;
	else if (key == SDLK_RIGHT)
		(rt->sph + 0)->center.x += S;
	else if (key == SDLK_RSHIFT)
		(rt->sph + 0)->center.y += S;
	else if (key == SDLK_RETURN)
		(rt->sph + 0)->center.y -= S;
	else
		return (0);
	if (key == SDLK_UP || key == SDLK_DOWN
		|| key == SDLK_LEFT || key == SDLK_RIGHT
			|| key == SDLK_RSHIFT || key == SDLK_RETURN)
		thread_manager(rt);
	return (0);
}

int		sdl_loop(t_sdl *sdl, t_rt *rt)
{
	int	quit;

	quit = 0;
	while (quit == 0)
	{
		if (SDL_PollEvent(&(sdl->event)))
		{
			if (sdl->event.type == SDL_QUIT
				|| (sdl->event.key.type == SDL_KEYUP
					&& sdl->event.key.keysym.sym == SDLK_ESCAPE))
			{
				mem_free(rt);
				quit = 1;
			}
			if (sdl->event.key.type == SDL_KEYDOWN)
				sdl_keyhook(sdl->event.key.keysym.sym, rt);
			correct_queue();
		}
	}
	return (0);
}