#include "openclbuilder.h"

void		*release_t_cl_builder(t_cl_builder *cl)
{
	if (cl->program)
		clReleaseProgram(cl->program);
	if (cl->queue)
		clReleaseCommandQueue(cl->queue);
	if (cl->context)
		clReleaseContext(cl->context);
	if (cl->device)
		clReleaseDevice(cl->device);
	return (NULL);
}

cl_int		opencl_platform_device_init(t_cl_builder *cl)
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

cl_int		opencl_contex_queue_init(t_cl_builder *cl)
{
	cl_int	ret;

	cl->context = clCreateContext(NULL, 1,
		(const cl_device_id*)(&cl->device), NULL, NULL, &ret);
	if (ret)
		return (ret);
	cl->queue = clCreateCommandQueue(cl->context, cl->device, 0, &ret);
	return (ret);
}

void		opencl_print_log(t_cl_builder *cl,
				cl_program *prog, const char *file)
{
	size_t	size;
	char	*log;

	if (clGetProgramBuildInfo(*prog, cl->device,
			CL_PROGRAM_BUILD_LOG, 0, NULL, &size))
	{
		if ((log = ft_strjoin(file, ": failure when getting a build info\n")))
		{
			write(2, log, ft_strlen(log));
			free(log);
		}
		return ;
	}
	if (!(log = (char*)ft_memalloc(size)))
		return ;
	if (clGetProgramBuildInfo(*prog, cl->device,
		CL_PROGRAM_BUILD_LOG, size, log, &size))
	{
		free(log);
		return ;
	}
	ft_putendl_fd(file, 2);
	ft_putendl_fd(log, 2);
	free(log);
}

cl_program	*opencl_source_to_program(t_cl_builder *cl,
	const char **src, const char **src_file, size_t src_counter)
{
	size_t		i;
	cl_int		ret;
	char		*log;
	cl_program	*prog;

	i = 0;
	ret = 0;
	if (!(prog = (cl_program*)ft_memalloc(sizeof(cl_program) * src_counter)))
		return (NULL);
	while (i != src_counter && !ret)
	{
		prog[i] = clCreateProgramWithSource(
			cl->context, 1, src_file + i, NULL, &ret);
		i += 1;
	}
	if (ret && (log = ft_strjoin(src[i - 1],
			": failure when creating a program\n")))
	{
		write(2, log, ft_strlen(log));
		free(log);
	}
	while (ret && (i != 0))
		clReleaseProgram(prog[--i]);
	ft_memdel(ret ? (void**)&prog : NULL);
	return (!ret ? prog : NULL);
}
