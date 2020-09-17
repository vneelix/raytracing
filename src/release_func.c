/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   release_func.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/12 15:15:09 by vneelix           #+#    #+#             */
/*   Updated: 2020/09/12 15:29:49 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void		release_kernel(cl_kernel kernel)
{
	cl_uint i;

	if (clGetKernelInfo(kernel,
		CL_KERNEL_REFERENCE_COUNT, sizeof(cl_uint), &i, NULL))
		return ;
	while (i)
	{
		clReleaseKernel(kernel);
		i -= 1;
	}
}

void		release_memory(cl_mem memory)
{
	cl_uint i;

	if (clGetMemObjectInfo(memory,
		CL_MEM_REFERENCE_COUNT, sizeof(cl_uint), &i, NULL))
		return ;
	while (i)
	{
		clReleaseMemObject(memory);
		i -= 1;
	}
}

static void	release_cl2(t_cl *cl)
{
	if (cl->illu_buffer)
		release_memory(cl->illu_buffer);
	if (cl->pixel_buffer)
		release_memory(cl->pixel_buffer);
	if (cl->texture_buffer)
		release_memory(cl->texture_buffer);
	if (cl->soft_shadow_buffer)
		release_memory(cl->soft_shadow_buffer);
	if (cl->ambient_occlusion_buffer)
		release_memory(cl->ambient_occlusion_buffer);
	if (cl->program)
		release_program(cl->program);
	if (cl->queue)
		release_command_queue(cl->queue);
	if (cl->context)
		release_context(cl->context);
	if (cl->device)
		release_device_id(cl->device);
	if (cl->platform)
		clUnloadPlatformCompiler(cl->platform);
}

void		release_cl(t_cl *cl)
{
	if (!cl)
		return ;
	if (cl->rotate_kernel)
		release_kernel(cl->rotate_kernel);
	if (cl->renderer_kernel)
		release_kernel(cl->renderer_kernel);
	if (cl->find_item_kernel)
		release_kernel(cl->find_item_kernel);
	if (cl->move_origin_kernel)
		release_kernel(cl->move_origin_kernel);
	if (cl->change_color_kernel)
		release_kernel(cl->change_color_kernel);
	if (cl->genhemisphere_kernel)
		release_kernel(cl->genhemisphere_kernel);
	if (cl->post_processing_kernel)
		release_kernel(cl->post_processing_kernel);
	if (cl->camera)
		release_memory(cl->camera);
	if (cl->active_item)
		release_memory(cl->active_item);
	if (cl->item_buffer)
		release_memory(cl->item_buffer);
	release_cl2(cl);
}

void		free_rt(t_rt *rt)
{
	release_cl(&rt->cl);
	release_sdl(&rt->sdl, &rt->intrf);
	if (rt->illu)
		free(rt->illu);
	if (rt->item)
		free(rt->item);
	if (rt->texture)
		free(rt->texture);
	if (rt->file_name)
		ft_nptr_del((void**)rt->file_name);
	free(rt);
}
