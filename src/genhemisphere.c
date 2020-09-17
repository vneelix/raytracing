/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   genhemisphere.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/12 15:19:49 by vneelix           #+#    #+#             */
/*   Updated: 2020/09/12 15:19:50 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

cl_int	genhemisphere_kernel_launch(t_cl *cl, cl_mem buffer, cl_uint2 map_size)
{
	size_t	size;

	size = map_size.x * map_size.y + 1;
	if (clSetKernelArg(cl->genhemisphere_kernel, 1,
						sizeof(cl_int), &map_size.x) ||
			clSetKernelArg(cl->genhemisphere_kernel, 2,
							sizeof(cl_int), &map_size.y) ||
				clSetKernelArg(cl->genhemisphere_kernel, 0,
								sizeof(cl_mem), &buffer))
		return (-1);
	if (clEnqueueNDRangeKernel(cl->queue, cl->genhemisphere_kernel,
								1, NULL, &size, NULL, 0, NULL, NULL))
		return (-1);
	return (0);
}

cl_int	genhemisphere_kernel(t_cl *cl,
	cl_uint2 shadow_map_size, cl_uint2 ambient_map_size)
{
	cl_int	ret;

	if (!(cl->soft_shadow_buffer = clCreateBuffer(cl->context,
		CL_MEM_READ_WRITE, (shadow_map_size.x * shadow_map_size.y + 1)
						* sizeof(cl_float3), NULL, NULL))
			|| !(cl->ambient_occlusion_buffer =
				clCreateBuffer(cl->context, CL_MEM_READ_WRITE,
					(ambient_map_size.x * ambient_map_size.y + 1)
						* sizeof(cl_float3) * sizeof(cl_float3), NULL, NULL)))
		return (-1);
	cl->genhemisphere_kernel =
		clCreateKernel(cl->program, "GenHemisphere", &ret);
	if (ret)
		return (-1);
	if (genhemisphere_kernel_launch(cl,
		cl->soft_shadow_buffer, shadow_map_size)
		|| genhemisphere_kernel_launch(cl,
			cl->ambient_occlusion_buffer, ambient_map_size))
		return (-1);
	if ((clSetKernelArg(cl->renderer_kernel,
		4, sizeof(cl_mem), &cl->soft_shadow_buffer))
		|| clSetKernelArg(cl->renderer_kernel,
			5, sizeof(cl_mem), &cl->ambient_occlusion_buffer))
		return (-1);
	return (0);
}
