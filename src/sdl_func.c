#include "rt.h"

SDL_Surface	*texture_convert(SDL_Surface *surf)
{
	SDL_Surface	*template;

	if (!surf)
		return (NULL);
	template = SDL_ConvertSurfaceFormat(surf, SDL_PIXELFORMAT_BGRA32, 0);
	SDL_FreeSurface(surf);
	return (template);
}

int			sdl_image_init(t_rt *rt)
{
	if (!(rt->intrf.spectr = texture_convert(IMG_Load("menu.png"))))
		return (-1);
	if (!(rt->intrf.menu = SDL_CreateTextureFromSurface(rt->sdl.ren,
	rt->intrf.spectr)))
		return (-1);
	SDL_FreeSurface(rt->intrf.spectr);
	if (!(rt->intrf.spectr = texture_convert(IMG_Load("spectrum.png"))))
		return (-1);
	if (!(rt->intrf.image = SDL_CreateTextureFromSurface(rt->sdl.ren,
	rt->intrf.spectr)))
		return (-1);
	return (0);
}

int			sdl_init(t_sdl *sdl)
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
	if ((sdl->ren = SDL_CreateRenderer(sdl->win, -1, SDL_RENDERER_ACCELERATED))
	== NULL)
	{
		SDL_DestroyWindow(sdl->win);
		SDL_Quit();
		return (-1);
	}
	if ((sdl->tex = SDL_CreateTexture(sdl->ren, SDL_PIXELFORMAT_BGRA32,
	SDL_TEXTUREACCESS_STREAMING, W, H)) == NULL)
	{
		SDL_DestroyRenderer(sdl->ren);
		SDL_DestroyWindow(sdl->win);
		return (-1);
	}
	return (0);
}
