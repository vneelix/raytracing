#include "rtv1.h"

void		create_items(t_illum *illum, t_item *item)
{
	illum[0] = (t_illum){(cl_float)(0.6), (cl_float3){0, 0, 0}};
	item[0] = (t_item){(t_pref){
		(cl_float3){0, 10, 50}, (cl_float3){0, -1, 0}, (cl_float){0}, (cl_float)(0)},
			(t_attr){(cl_float3){0, 0, 255}, (cl_float){5000},
				(cl_float){0}, (cl_float){0}}, 0};
}

cl_int		opencl_memobj(t_opencl *cl, void *arg)
{
	cl_int	ret;
	size_t	size;

	//
	t_illum *illum = malloc(sizeof(t_illum) * 1);
	t_item	*item = malloc(sizeof(t_item) * 1);
	create_items(illum, item);
	//
	size = 96 * 1024 * 1024;
	(cl->memobj)[0] = clCreateBuffer(cl->context, CL_MEM_READ_WRITE, size, NULL, &ret);
	size = sizeof(t_illum) * 1;
	(cl->memobj)[1] = clCreateBuffer(cl->context, CL_MEM_READ_ONLY, size, NULL, &ret);
	ret = clEnqueueWriteBuffer(cl->command_queue, (cl->memobj)[1],
		CL_TRUE, 0, size, illum, 0, NULL, NULL);
	size = sizeof(t_item) * 1;
	(cl->memobj)[2] = clCreateBuffer(cl->context, CL_MEM_READ_ONLY, size, NULL, &ret);
	ret = clEnqueueWriteBuffer(cl->command_queue, (cl->memobj)[2],
		CL_TRUE, 0, size, item, 0, NULL, NULL);
	if (ret)
		return (ret);
	return (0);
}

cl_int		opencl_lauch(t_opencl *cl, void *arg)
{
	cl_int	ret;
	size_t	work_size[1] = {800 * 800};

	ret = clSetKernelArg(cl->kernel, 0, sizeof(cl_mem), cl->memobj);
	ret = clSetKernelArg(cl->kernel, 1, sizeof(cl_mem), cl->memobj + 1);
	ret = clSetKernelArg(cl->kernel, 2, sizeof(cl_mem), cl->memobj + 2);
	t_opt options = (t_opt){800, 800, 1, 1};
	ret = clSetKernelArg(cl->kernel, 3, sizeof(t_opt), &options);
	ret = clEnqueueNDRangeKernel(cl->command_queue,
		cl->kernel, 1, NULL, work_size, NULL, 0, NULL, NULL);
	ret = clEnqueueReadBuffer(cl->command_queue, (cl->memobj)[0], CL_TRUE, 0,
		800 * 800 * 4, ((t_sdl*)(arg))->surf->pixels, 0, NULL, NULL);
	return (0);
}

cl_int		opencl_program(t_opencl *cl, char **sources, cl_uint count)
{
	cl_int	ret;

	cl->program = clCreateProgramWithSource(cl->context, count, (const char**)(sources), NULL, &ret);
	if (ret)
		return (ret);
	if ((ret = clBuildProgram(cl->program, 1, &(cl->device_id), NULL, NULL, NULL)))
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

cl_int		opencl_init(t_opencl *cl, char **sources, cl_uint count, void *arg)
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
	if ((ret = opencl_program(cl, sources, count)))
		return (ret);
	if ((ret = opencl_memobj(cl ,arg)))
		return (ret);
	if ((ret = opencl_lauch(cl, arg)))
		return (ret);
	return (0);
}