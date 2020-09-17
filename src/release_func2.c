/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   release_func2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/12 15:13:55 by vneelix           #+#    #+#             */
/*   Updated: 2020/09/12 15:28:32 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	release_program(cl_program program)
{
	cl_uint i;

	if (clGetProgramInfo(program,
		CL_PROGRAM_REFERENCE_COUNT, sizeof(cl_uint), &i, NULL))
		return ;
	while (i)
	{
		clReleaseProgram(program);
		i -= 1;
	}
}

void	release_context(cl_context context)
{
	cl_uint i;

	if (clGetContextInfo(context,
		CL_CONTEXT_REFERENCE_COUNT, sizeof(cl_uint), &i, NULL))
		return ;
	while (i)
	{
		clReleaseContext(context);
		i -= 1;
	}
}

void	release_command_queue(cl_command_queue queue)
{
	cl_uint i;

	if (clGetCommandQueueInfo(queue,
		CL_QUEUE_REFERENCE_COUNT, sizeof(cl_uint), &i, NULL))
		return ;
	while (i)
	{
		clReleaseCommandQueue(queue);
		i -= 1;
	}
}

void	release_device_id(cl_device_id device)
{
	cl_uint i;

	if (clGetDeviceInfo(device,
		CL_DEVICE_REFERENCE_COUNT, sizeof(cl_uint), &i, NULL))
		return ;
	while (i)
	{
		clReleaseDevice(device);
		i -= 1;
	}
}

void	release_sdl(t_sdl *sdl, t_interface *ife)
{
	if (ife && ife->spectr)
		SDL_FreeSurface(ife->spectr);
	if (ife && ife->menu)
		SDL_DestroyTexture(ife->menu);
	if (ife && ife->image)
		SDL_DestroyTexture(ife->image);
	if (sdl && sdl->ptr)
		free(sdl->ptr);
	if (sdl && sdl->tex)
		SDL_DestroyTexture(sdl->tex);
	if (sdl && sdl->ren)
		SDL_DestroyRenderer(sdl->ren);
	if (sdl && sdl->win)
		SDL_DestroyWindow(sdl->win);
	IMG_Quit();
	SDL_Quit();
}
