/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opencl_kernel_func2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/07 21:04:26 by vneelix           #+#    #+#             */
/*   Updated: 2020/08/07 21:04:32 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

cl_int	opencl_launch(t_cl *cl, t_rt *rt)
{
	cl_int	ret;
	size_t	work_size;

	work_size = rt->opt.w * rt->opt.h;
	ret = clSetKernelArg(cl->renderer_kernel, 8, sizeof(t_opt), &(rt->opt));
	ret = clEnqueueNDRangeKernel(cl->queue,
		cl->renderer_kernel, 1, NULL, &(work_size), NULL, 0, NULL, NULL);
	ret = clEnqueueReadBuffer(cl->queue, cl->pixel_buffer,
						CL_TRUE, 0, work_size * 4, rt->sdl.ptr, 0, NULL, NULL);
	return (ret);
}

cl_int	move_origin_kernel_init(t_cl *cl)
{
	cl_int	ret;

	cl->move_origin_kernel = clCreateKernel(cl->program, "MoveOrigin", &ret);
	if (ret)
		return (-1);
	if (clSetKernelArg(
			cl->move_origin_kernel, 0, sizeof(cl_mem), &cl->camera) ||
			clSetKernelArg(
				cl->move_origin_kernel, 1, sizeof(cl_mem), &cl->active_item))
		return (-1);
	return (0);
}

cl_int	move_origin_kernel_launch(t_cl *cl, cl_uint flags)
{
	size_t	worksize;

	worksize = 1;
	if (clSetKernelArg(cl->move_origin_kernel, 2, sizeof(cl_uint), &flags))
		return (-1);
	if (clEnqueueNDRangeKernel(cl->queue, cl->move_origin_kernel,
								1, NULL, &worksize, NULL, 0, NULL, NULL))
		return (-1);
	return (0);
}

cl_int	find_item_kernel_init(t_cl *cl)
{
	cl_int	ret;

	cl->find_item_kernel = clCreateKernel(cl->program, "FindItem", &ret);
	if (ret)
		return (-1);
	if (clSetKernelArg(
			cl->find_item_kernel, 0, sizeof(cl_mem), &cl->camera) ||
			clSetKernelArg(
				cl->find_item_kernel, 1, sizeof(cl_mem), &cl->item_buffer) ||
				clSetKernelArg(
					cl->find_item_kernel, 2, sizeof(cl_mem), &cl->active_item))
		return (-1);
	return (0);
}

cl_int	find_item_kernel_launch(t_cl *cl, cl_int x, cl_int y, t_opt *opt)
{
	size_t	worksize;

	worksize = 1;
	if (clSetKernelArg(
			cl->find_item_kernel, 3, sizeof(cl_int), &x) ||
			clSetKernelArg(
				cl->find_item_kernel, 4, sizeof(cl_int), &y) ||
				clSetKernelArg(
					cl->find_item_kernel, 5, sizeof(t_opt), opt))
		return (-1);
	if (clEnqueueNDRangeKernel(cl->queue,
		cl->find_item_kernel, 1, NULL, &worksize, NULL, 0, NULL, NULL))
		return (-1);
	return (0);
}
