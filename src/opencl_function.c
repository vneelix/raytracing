/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opencl_function.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/12 15:20:48 by vneelix           #+#    #+#             */
/*   Updated: 2020/09/12 15:23:33 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

cl_int		opencl_platform_device_init(t_cl *cl)
{
	cl_int	ret;

	ret = clGetPlatformIDs(1,
			&cl->platform, NULL);
	if (ret)
		return (ret);
	ret = clGetDeviceIDs(cl->platform,
		CL_DEVICE_TYPE_GPU, 1, &cl->device, NULL);
	return (ret);
}

cl_int		opencl_contex_queue_init(t_cl *cl)
{
	cl_int	ret;

	cl->context = clCreateContext(NULL, 1,
		(const cl_device_id*)(&cl->device), NULL, NULL, &ret);
	if (ret)
		return (ret);
	cl->queue = clCreateCommandQueue(cl->context, cl->device, 0, &ret);
	return (ret);
}

cl_int		opencl_build(t_cl *cl)
{
	size_t	count;
	char	**dir;
	char	**file;

	if (ft_get_directory("cl_src/", (void**)&dir, (void**)&file, &count))
		return (-1);
	cl->program = clCreateProgramWithSource(cl->context,
									count, (const char**)file, NULL, NULL);
	ft_nptr_del((void**)dir);
	ft_nptr_del((void**)file);
	if (!cl->program)
		return (-1);
	if (clBuildProgram(cl->program, 1, &cl->device, "-Werror", NULL, NULL))
		return (-1);
	return (0);
}
