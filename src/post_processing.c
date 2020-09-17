/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   post_processing.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/12 15:21:10 by vneelix           #+#    #+#             */
/*   Updated: 2020/09/12 15:26:06 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

cl_int	post_processing_kernel_init(t_cl *cl)
{
	cl_int	ret;

	cl->post_processing_kernel = clCreateKernel(cl->program,
											"PostProcessing", &ret);
	if (ret)
		return (-1);
	if (clSetKernelArg(cl->post_processing_kernel,
				0, sizeof(cl_mem), &cl->pixel_buffer))
		return (-1);
	return (0);
}

cl_int	post_processing_kernel_launch(t_cl *cl, t_rt *rt)
{
	size_t	worksize;

	if (clSetKernelArg(cl->post_processing_kernel,
						1, sizeof(cl_uint), &rt->opt.w)
		|| clSetKernelArg(cl->post_processing_kernel,
							2, sizeof(cl_uint), &rt->opt.h)
			|| clSetKernelArg(cl->post_processing_kernel,
								3, sizeof(cl_uchar), &rt->ppf))
		return (-1);
	worksize = rt->opt.w * rt->opt.h;
	if (clEnqueueNDRangeKernel(cl->queue, cl->post_processing_kernel,
								1, NULL, &worksize, NULL, 0, NULL, NULL))
		return (-1);
	if (clEnqueueReadBuffer(cl->queue, cl->pixel_buffer,
						CL_TRUE, 0, worksize * 4, rt->sdl.ptr, 0, NULL, NULL))
		return (-1);
	return (0);
}
