#include "rt.h"

cl_int	rotate_kernel_init(t_cl *cl)
{
	cl_int	ret;

	cl->rotate_kernel = clCreateKernel(cl->program, "Rotate", &ret);
	if (ret)
		return (-1);
	if (clSetKernelArg(
		cl->rotate_kernel, 0, sizeof(cl_mem), &cl->camera) ||
			clSetKernelArg(
				cl->rotate_kernel, 1, sizeof(cl_mem), &cl->active_item))
		return (-1);
	return (0);
}

cl_int	rotate_kernel_launch(t_cl *cl, cl_float3 euler_angle, cl_uint flags)
{
	size_t	worksize;

	worksize = 1;
	if (clSetKernelArg(cl->rotate_kernel,
		5, sizeof(cl_uint), &flags) ||
			clSetKernelArg(cl->rotate_kernel,
				2, sizeof(cl_float), &euler_angle.x) ||
				clSetKernelArg(cl->rotate_kernel,
					3, sizeof(cl_float), &euler_angle.y) ||
					clSetKernelArg(cl->rotate_kernel,
						4, sizeof(cl_float), &euler_angle.z))
		return (-1);
	if (clEnqueueNDRangeKernel(cl->queue, cl->rotate_kernel,
								1, NULL, &worksize, NULL, 0, NULL, NULL))
		return (-1);
	return (0);
}

cl_int	change_color_kernel_init(t_cl *cl)
{
	cl_int	ret;

	cl->change_color_kernel = clCreateKernel(cl->program, "ChangeColor", &ret);
	if (ret)
		return (-1);
	if (clSetKernelArg(cl->change_color_kernel,
			0, sizeof(cl_mem), &cl->active_item))
		return (-1);
	return (0);
}

cl_int	change_color_kernel_launch(t_cl *cl, cl_float3 color)
{
	size_t	worksize;

	worksize = 1;
	if (clSetKernelArg(cl->change_color_kernel, 1, sizeof(cl_float3), &color))
		return (-1);
	if (clEnqueueNDRangeKernel(cl->queue, cl->change_color_kernel,
								1, NULL, &worksize, NULL, 0, NULL, NULL))
		return (-1);
	return (0);
}
