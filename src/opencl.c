#include "rtv1.h"

cl_int		opencl_memobj(t_opencl *cl, t_rt *rt)
{
	cl_int	ret;
	size_t	size;

	size = 96 * 1024 * 1024;
	(cl->memobj)[0] = clCreateBuffer(cl->context, CL_MEM_READ_WRITE, size, NULL, &ret);
	size = sizeof(t_item) * rt->opt.illu_c;
	(cl->memobj)[1] = clCreateBuffer(cl->context, CL_MEM_READ_ONLY, size, NULL, &ret);
	ret = clEnqueueWriteBuffer(cl->command_queue, (cl->memobj)[1],
		CL_TRUE, 0, size, rt->illu, 0, NULL, NULL);
	size = sizeof(t_item) * rt->opt.item_c;
	(cl->memobj)[2] = clCreateBuffer(cl->context, CL_MEM_READ_ONLY, size, NULL, &ret);
	ret = clEnqueueWriteBuffer(cl->command_queue, (cl->memobj)[2],
		CL_TRUE, 0, size, rt->item, 0, NULL, NULL);
	if (ret)
		return (ret);
	return (0);
}

cl_int		opencl_launch(t_opencl *cl, t_rt *rt)
{
	cl_int	ret;
	size_t	work_size;

	work_size = rt->opt.w * rt->opt.h;
	ret = clSetKernelArg(cl->kernel, 3, sizeof(t_opt), &(rt->opt));
	ret = clEnqueueNDRangeKernel(cl->command_queue,
		cl->kernel, 1, NULL, &(work_size), NULL, 0, NULL, NULL);
	ret = clEnqueueReadBuffer(cl->command_queue, (cl->memobj)[0], CL_TRUE, 0,
		rt->opt.w * rt->opt.h * 4, rt->sdl.surf->pixels, 0, NULL, NULL);
	return (0);
}

cl_int		opencl_program(t_opencl *cl, char **sources)
{
	cl_int	ret;
	cl_uint	count;

	count = 0;
	while (sources[count] != NULL)
		count += 1;
	cl->program = clCreateProgramWithSource(cl->context, count, (const char**)(sources), NULL, &ret);
	if (ret)
		return (ret);
	if ((ret = clBuildProgram(cl->program, 1, &(cl->device_id), "-Icl_inc", NULL, NULL)))
	{
		size_t log_size;
		clGetProgramBuildInfo(cl->program, cl->device_id, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
		char *log = (char *) malloc(log_size);
		clGetProgramBuildInfo(cl->program, cl->device_id, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);
		printf("%s\n", log);
	}
	cl->kernel = clCreateKernel(cl->program, "main_func", &ret);
	if (ret)
		return (ret);
	return (0);
}

cl_int		opencl_init(t_opencl *cl, char **sources, t_rt *rt)
{
	cl_int	ret;
	cl_uint	num;

	if ((ret = clGetPlatformIDs(1, &(cl->platform_id), &num)))
		return (ret);
	if ((ret = clGetDeviceIDs(cl->platform_id, CL_DEVICE_TYPE_GPU, 1, &(cl->device_id), &num)))
		return (ret);
	cl->context = clCreateContext(NULL, 1, &(cl->device_id), NULL, NULL, &ret);
	if (ret)
		return (ret);
	cl->command_queue = clCreateCommandQueueWithProperties(cl->context, cl->device_id, NULL, &ret);
	if (ret)
		return (ret);
	if ((ret = opencl_program(cl, sources)))
		return (ret);
	if ((ret = opencl_memobj(cl ,rt)))
		return (ret);
	ret = clSetKernelArg(cl->kernel, 0, sizeof(cl_mem), cl->memobj);
	ret = clSetKernelArg(cl->kernel, 1, sizeof(cl_mem), cl->memobj + 1);
	ret = clSetKernelArg(cl->kernel, 2, sizeof(cl_mem), cl->memobj + 2);
	if ((ret = opencl_launch(cl, rt)))
		return (ret);
	return (0);
}
