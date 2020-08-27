#include "rt.h"

static void	free_rt_part0(t_rt *rt)
{
	if (rt->intrf.image)
		SDL_DestroyTexture(rt->intrf.image);
	if (rt->intrf.menu)
		SDL_DestroyTexture(rt->intrf.menu);
	if (rt->intrf.spectr)
		SDL_FreeSurface(rt->intrf.spectr);
	ft_nptr_del((void**)rt->file_name);
	if (rt->texture)
		free(rt->texture);
	if (rt->item)
		free(rt->item);
	if (rt->illu)
		free(rt->illu);
	if (rt->sdl.ren)
		SDL_DestroyRenderer(rt->sdl.ren);
	if (rt->sdl.tex)
		SDL_DestroyTexture(rt->sdl.tex);
	if (rt->sdl.ptr)
		free(rt->sdl.ptr);
	if (rt->sdl.win)
		SDL_DestroyWindow(rt->sdl.win);
}

static void	free_rt_part1(t_rt *rt)
{
	if (rt->cl.genhemisphere_kernel)
		clReleaseKernel(rt->cl.genhemisphere_kernel);
	if (rt->cl.change_color_kernel)
		clReleaseKernel(rt->cl.change_color_kernel);
	if (rt->cl.move_origin_kernel)
		clReleaseKernel(rt->cl.move_origin_kernel);
	if (rt->cl.find_item_kernel)
		clReleaseKernel(rt->cl.find_item_kernel);
	if (rt->cl.renderer_kernel)
		clReleaseKernel(rt->cl.renderer_kernel);
	if (rt->cl.rotate_kernel)
		clReleaseKernel(rt->cl.rotate_kernel);
	if (rt->cl.ambient_occlusion_buffer)
		clReleaseMemObject(rt->cl.ambient_occlusion_buffer);
	if (rt->cl.soft_shadow_buffer)
		clReleaseMemObject(rt->cl.soft_shadow_buffer);
	if (rt->cl.texture_buffer)
		clReleaseMemObject(rt->cl.texture_buffer);
	if (rt->cl.pixel_buffer)
		clReleaseMemObject(rt->cl.pixel_buffer);
	if (rt->cl.item_buffer)
		clReleaseMemObject(rt->cl.item_buffer);
}

static void	free_rt_part2(t_rt *rt)
{
	if (rt->cl.active_item)
		clReleaseMemObject(rt->cl.active_item);
	if (rt->cl.camera)
		clReleaseMemObject(rt->cl.camera);
	if (rt->cl.program)
		clReleaseProgram(rt->cl.program);
	if (rt->cl.context)
		clReleaseContext(rt->cl.context);
	if (rt->cl.queue)
		clReleaseCommandQueue(rt->cl.queue);
}

void		free_rt(t_rt *rt)
{
	if (rt)
	{
		free_rt_part0(rt);
		free_rt_part1(rt);
		free_rt_part2(rt);
		free(rt);
	}
}
