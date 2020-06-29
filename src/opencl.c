#include "rtv1.h"

cl_int		opencl_memobj(t_cl *cl, t_rt *rt)
{
	cl_int	ret;
	size_t	size;

	size = rt->opt.w * rt->opt.h * 4;
	cl->memory[0] = clCreateBuffer(cl->context, CL_MEM_READ_WRITE, size, NULL, &ret);
	size = sizeof(t_item) * rt->opt.illu_c;
	cl->memory[1] = clCreateBuffer(cl->context, CL_MEM_READ_ONLY, size, NULL, &ret);
	ret = clEnqueueWriteBuffer(cl->queue, cl->memory[1],
		CL_TRUE, 0, size, rt->illu, 0, NULL, NULL);
	size = sizeof(t_item) * rt->opt.item_c;
	cl->memory[2] = clCreateBuffer(cl->context, CL_MEM_READ_ONLY, size, NULL, &ret);
	ret = clEnqueueWriteBuffer(cl->queue, cl->memory[2],
		CL_TRUE, 0, size, rt->item, 0, NULL, NULL);
	if (ret)
		return (ret);
	return (0);
}

cl_int		opencl_launch(t_cl *cl, t_rt *rt)
{
	cl_int	ret;
	size_t	work_size;

	work_size = rt->opt.w * rt->opt.h;
	ret = clSetKernelArg(cl->rt_kernel, 3, sizeof(t_opt), &(rt->opt));
	ret = clEnqueueNDRangeKernel(cl->queue,
		cl->rt_kernel, 1, NULL, &(work_size), NULL, 0, NULL, NULL);
	ret = clEnqueueReadBuffer(cl->queue, cl->memory[0], CL_TRUE, 0, work_size * 4, rt->sdl.ptr, 0, NULL, NULL);
	return (ret);
}

cl_int		opencl_program(t_cl *cl, char **sources)
{
	cl_int	ret;
	cl_uint	count;

	count = 0;
	while (sources[count] != NULL)
		count += 1;
	cl->program = clCreateProgramWithSource(cl->context, count, (const char**)(sources), NULL, &ret);
	if (ret)
		return (ret);
	if ((ret = clBuildProgram(cl->program, 1, &(cl->device), "-Icl_inc", NULL, NULL)))
	{
		size_t log_size;
		clGetProgramBuildInfo(cl->program, cl->device, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
		char *log = (char *) malloc(log_size);
		clGetProgramBuildInfo(cl->program, cl->device, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);
		printf("%s\n", log);
	}
	cl->rt_kernel = clCreateKernel(cl->program, "main_func", &ret);
	if (ret)
		return (ret);
	return (0);
}

/*struct __attribute__ ((packed)) item {
	enum IType	Type;
	float3			Center;
	float3			Normal;
	float3			Vector;
	float3			VMin, VMax;
	float				ItemRadius, ItemMin, ItemMax, k;

	float3			Color;
	float				ReflectRatio, RefractRatio, ShineRatio;
};*/

void	figure_init(t_rt *rt) {
	rt->opt.illu_c = 1;
	rt->opt.item_c = 2;

	rt->illu = malloc(sizeof(t_item) * rt->opt.illu_c);
	rt->item = malloc(sizeof(t_item) * rt->opt.item_c);

	rt->illu[0] = (t_item){POINT, {0, -16, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 4, 0, 0, 0.8, {255, 255, 255}, 0, 0, 0};
	rt->item[0] = (t_item){PLANE, {0, 20, 0}, {0, -1, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 0, 0, 0, 0, {0, 255, 153}, 0, 0, 0};
	rt->item[1] = (t_item){BOX, {0, 0, 45}, {0, 0, 0}, {0, 0, 0}, {-10, -10, 35}, {10, 10, 55}, 0, 0, 0, 0, {255, 204, 204}, 0, 0};
}

cl_int	platform_device_init(t_cl *cl)
{
	cl_int	ret;
	cl_uint	num;

	if ((ret = clGetPlatformIDs(0, NULL, &num)) == 0)
		if ((ret = clGetPlatformIDs(num, &cl->platform, NULL)) == 0)
			if ((ret =clGetDeviceIDs(cl->platform, CL_DEVICE_TYPE_GPU, 0, NULL, &num)) == 0)
				if ((ret = clGetDeviceIDs(cl->platform, CL_DEVICE_TYPE_GPU, num, &cl->device, NULL)) == 0)
					return (0);
	return (ret);
}

cl_int	contex_queue_init(t_cl *cl) {
	cl_int	ret;

	cl->context = clCreateContext(NULL, 1, (const cl_device_id*)(&cl->device), NULL, NULL, &ret);
	if (ret)
		return (ret);
	cl->queue = clCreateCommandQueueWithProperties(cl->context, cl->device, NULL, &ret);
	return (ret);
}

void	func(t_cl *cl) {
	cl_int ret, a = 4, b = 8;
	size_t worksize = a * b + 1;

	cl->memory[3] = clCreateBuffer(cl->context, CL_MEM_READ_WRITE, sizeof(cl_float3) * worksize, NULL, &ret);
	cl->genhemisphere_kernel = clCreateKernel(cl->program, "genHemisphere", &ret);
	ret = clSetKernelArg(cl->genhemisphere_kernel, 0, sizeof(cl_mem), &cl->memory[3]);
	ret = clSetKernelArg(cl->genhemisphere_kernel, 1, sizeof(cl_int), &a);
	ret = clSetKernelArg(cl->genhemisphere_kernel, 2, sizeof(cl_int), &b);
	ret = clEnqueueNDRangeKernel(cl->queue, cl->genhemisphere_kernel, 1, NULL, &worksize, NULL, 0, NULL, NULL);
	ret = clSetKernelArg(cl->rt_kernel, 4, sizeof(cl_mem), &cl->memory[3]);
	ret = clSetKernelArg(cl->rt_kernel, 5, sizeof(cl_int), &worksize);
}

cl_int	opencl_init(t_cl *cl, char **sources, t_rt *rt)
{
	cl_int ret;

	if (ret = platform_device_init(cl))
		return (ret);
	if (ret = contex_queue_init(cl))
		return (ret);
	if (ret = opencl_program(cl, sources))
		return (ret);
	figure_init(rt);
	if ((ret = opencl_memobj(cl ,rt)))
		return (ret);
	func(cl);
	ret = clSetKernelArg(cl->rt_kernel, 0, sizeof(cl_mem), &cl->memory[0]);
	ret = clSetKernelArg(cl->rt_kernel, 1, sizeof(cl_mem), &cl->memory[1]);
	ret = clSetKernelArg(cl->rt_kernel, 2, sizeof(cl_mem), &cl->memory[2]);
	if ((ret = opencl_launch(cl, rt)))
		return (ret);
	return (0);
}
