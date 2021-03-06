/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opencl.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/12 15:21:04 by vneelix           #+#    #+#             */
/*   Updated: 2020/09/12 15:26:30 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static cl_int	active_item_address_init(t_cl *cl)
{
	size_t	val;
	cl_uint	address_bits;

	val = 0;
	if (clGetDeviceInfo(cl->device,
		CL_DEVICE_ADDRESS_BITS, sizeof(cl_uint), &address_bits, NULL))
		return (-1);
	address_bits /= 8;
	if (!(cl->active_item = clCreateBuffer(cl->context,
			CL_MEM_READ_WRITE, address_bits, NULL, NULL)))
		return (-1);
	if (clEnqueueWriteBuffer(cl->queue,
		cl->active_item, CL_TRUE, 0, address_bits, &val, 0, NULL, NULL))
		return (-1);
	return (0);
}

static cl_int	camera_init(t_cl *cl, t_rt *rt)
{
	t_camera	camera;
	cl_float3	reper[4];

	reper[0] = rt->opt.center;
	reper[1] = (cl_float3){{1, 0, 0}};
	reper[2] = (cl_float3){{0, 1, 0}};
	reper[3] = vector_len(rt->cam.vector) != 0.f
		? rt->cam.vector : (cl_float3){{0, 0, 1}};
	camera = (t_camera){
		0, 0, 0,
		{reper[0], reper[1], reper[2], reper[3]},
		{reper[0], reper[1], reper[2], reper[3]}
	};
	if (clEnqueueWriteBuffer(cl->queue, cl->camera,
		CL_TRUE, 0, sizeof(t_camera), &camera, 0, NULL, NULL))
		return (-1);
	if (vector_len(rt->cam.vector) != 0.f)
		if (rotate_kernel_launch(cl, (cl_float3){{0, 0, 0}}, 0))
			return (-1);
	return (0);
}

static cl_int	opencl_memobj(t_cl *cl, t_rt *rt)
{
	if (!(cl->pixel_buffer = clCreateBuffer(cl->context,
		CL_MEM_READ_WRITE, rt->opt.w * rt->opt.h * 4, NULL, NULL))
		|| !(cl->item_buffer = clCreateBuffer(cl->context,
			CL_MEM_READ_WRITE, sizeof(t_item) * rt->opt.item_c, NULL, NULL)))
		return (-1);
	if (rt->illu && !(cl->illu_buffer = clCreateBuffer(cl->context,
		CL_MEM_READ_WRITE, sizeof(t_item) * rt->opt.illu_c, NULL, NULL)))
		return (-1);
	if (rt->texture && !(cl->texture_buffer = clCreateBuffer(cl->context,
		CL_MEM_READ_ONLY, rt->opt.texture_size * sizeof(uint), NULL, NULL)))
		return (-1);
	if (clEnqueueWriteBuffer(cl->queue, cl->item_buffer, CL_TRUE,
		0, sizeof(t_item) * rt->opt.item_c, rt->item, 0, NULL, NULL))
		return (-1);
	if (cl->illu_buffer && clEnqueueWriteBuffer(cl->queue, cl->illu_buffer,
		CL_TRUE, 0, sizeof(t_item) * rt->opt.illu_c, rt->illu, 0, NULL, NULL))
		return (-1);
	if (cl->texture_buffer && clEnqueueWriteBuffer(
		cl->queue, cl->texture_buffer, CL_TRUE, 0, rt->opt.texture_size
								* sizeof(uint), rt->texture, 0, NULL, NULL))
		return (-1);
	if (active_item_address_init(cl) || !(cl->camera = clCreateBuffer(
			cl->context, CL_MEM_READ_WRITE, sizeof(t_camera), NULL, NULL)))
		return (-1);
	return (0);
}

cl_int			opencl_create_infrastructure(t_cl *cl)
{
	cl_int			ret;

	if ((ret = opencl_platform_device_init(cl))
		|| (ret = opencl_contex_queue_init(cl)))
		return (ret);
	if ((ret = opencl_build(cl)))
		return (ret);
	return (0);
}

cl_int			opencl_init(t_cl *cl, t_rt *rt)
{
	if (opencl_memobj(cl, rt))
		return (-1);
	if (!(cl->renderer_kernel = clCreateKernel(cl->program, "Renderer", NULL)))
		return (-1);
	if (clSetKernelArg(cl->renderer_kernel, 3, sizeof(cl_mem), &cl->camera)
	|| clSetKernelArg(cl->renderer_kernel, 6, sizeof(cl_mem), &cl->active_item)
	|| clSetKernelArg(cl->renderer_kernel, 2, sizeof(cl_mem), &cl->item_buffer)
	|| clSetKernelArg(cl->renderer_kernel, 1, sizeof(cl_mem), &cl->illu_buffer)
	|| clSetKernelArg(cl->renderer_kernel, 0, sizeof(cl_mem), &cl->pixel_buffer)
	|| clSetKernelArg(cl->renderer_kernel,
		7, sizeof(cl_mem), &cl->texture_buffer))
		return (-1);
	if (move_origin_kernel_init(cl) || find_item_kernel_init(cl)
			|| rotate_kernel_init(cl) || change_color_kernel_init(cl)
				|| post_processing_kernel_init(cl) || genhemisphere_kernel(cl,
					(cl_uint2){{16, 32}}, (cl_uint2){{32, 32}}))
		return (-1);
	if (camera_init(cl, rt))
		return (-1);
	rt->opt.soft_shadow_buffer_size = 16 * 32 + 1;
	rt->opt.ambient_occlusion_buffer_size = 32 * 32 + 1;
	rt->opt.flags |= SOFT_SHADOWS | RECURSION;
	if (opencl_launch(cl, rt))
		return (-1);
	return (0);
}
