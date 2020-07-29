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

void	clear_queue(void)
{
	SDL_EventState(SDL_KEYDOWN, SDL_DISABLE);
	SDL_EventState(SDL_KEYDOWN, SDL_ENABLE);
	SDL_EventState(SDL_MOUSEMOTION, SDL_DISABLE);
	SDL_EventState(SDL_MOUSEMOTION, SDL_ENABLE);
}

int		sdl_mousehook(t_sdl *sdl, t_rt *rt)
{
	rt->opt.x = atanf(sdl->event.motion.yrel) * 0.016;
	rt->opt.y = atanf(sdl->event.motion.xrel) * 0.016;
	rotate_kernel_launch(&rt->cl, rt->opt.x, rt->opt.y, 0, 0);
	return (1);
}

int		sdl_keyhook(SDL_Keycode keycode, t_rt *rt)
{
	cl_uchar flags = 0;

	if (!(keycode == SDLK_UP || keycode == SDLK_DOWN
		|| keycode == SDLK_LEFT || keycode == SDLK_RIGHT
			|| keycode == SDLK_RETURN || keycode == SDLK_BACKSPACE))
		return (0);
	if (keycode == SDLK_UP)
		flags |= (cl_uchar)1 << 0;
	else if (keycode == SDLK_DOWN)
		flags |= (cl_uchar)1 << 1;
	else if (keycode == SDLK_LEFT)
		flags |= (cl_uchar)1 << 2;
	else if (keycode == SDLK_RIGHT)
		flags |= (cl_uchar)1 << 3;
	else if (keycode == SDLK_RETURN)
		flags |= (cl_uchar)1 << 4;
	else if (keycode == SDLK_BACKSPACE)
		flags |= (cl_uchar)1 << 5;
	if (flags) {
		cl_int	ret = 0;
		size_t	workSize = 1;

		ret = clSetKernelArg(rt->cl.move_origin_kernel, 2, sizeof(t_opt), &rt->opt);
		ret = clSetKernelArg(rt->cl.move_origin_kernel, 1, sizeof(cl_uchar), &flags);
		ret = clEnqueueNDRangeKernel(rt->cl.queue, rt->cl.move_origin_kernel, 1, NULL, &workSize, NULL, 0, NULL, NULL);
	}
	return (1);
}

int		sdl_loop(t_sdl *sdl, t_rt *rt)
{
	int	ret;
	int	left_click = 0;
	int right_click = 0;

	ret = 0;
	while (SDL_WaitEvent(&(sdl->event)))
	{
		if (sdl->event.type == SDL_QUIT
			|| (sdl->event.key.type == SDL_KEYUP
				&& sdl->event.key.keysym.sym == SDLK_ESCAPE))
			break;

		if (sdl->event.button.type == SDL_MOUSEBUTTONDOWN
				&& sdl->event.button.button == SDL_BUTTON_LEFT) {
			left_click = 1;
		}
		else if (sdl->event.button.type == SDL_MOUSEBUTTONUP
				&& sdl->event.button.button == SDL_BUTTON_LEFT) {
			left_click = 0;
		}

		if (sdl->event.button.type == SDL_MOUSEBUTTONDOWN
				&& sdl->event.button.button == SDL_BUTTON_RIGHT) {
			right_click = 1;
		}
		else if (sdl->event.button.type == SDL_MOUSEBUTTONUP
				&& sdl->event.button.button == SDL_BUTTON_RIGHT) {
			right_click = 0;
		}

		if (right_click && sdl->event.type == SDL_MOUSEMOTION) {
			ret = sdl_mousehook(sdl, rt);
		}

		if (left_click && !(sdl->event.type == SDL_MOUSEMOTION)) {
			ret = find_item_kernel_launch(&rt->cl, sdl->event.motion.x, sdl->event.motion.y, &rt->opt);
			ret = !ret ? 1 : 0;
		}

		if (sdl->event.type == SDL_KEYDOWN)
			ret = sdl_keyhook(sdl->event.key.keysym.sym, rt);

		if (ret)
		{
			ret = opencl_launch(&(rt->cl), rt);
			SDL_UpdateTexture(sdl->tex, NULL, sdl->ptr, rt->opt.w * 4);
			SDL_RenderCopy(sdl->ren, sdl->tex, NULL, NULL);
			SDL_RenderPresent(sdl->ren);
		}
		clear_queue();
	}
	return (0);
}
