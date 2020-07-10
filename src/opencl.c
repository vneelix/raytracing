#include "rtv1.h"

cl_int		opencl_memobj(t_cl *cl, t_rt *rt)
{
	cl_int	ret;
	size_t	size;

	size = rt->opt.w * rt->opt.h * 4;
	cl->memory[0] = clCreateBuffer(cl->context, CL_MEM_READ_WRITE, size, NULL, &ret);
	size = sizeof(t_item) * rt->opt.illu_c;
	cl->memory[1] = clCreateBuffer(cl->context, CL_MEM_READ_WRITE, size, NULL, &ret);
	ret = clEnqueueWriteBuffer(cl->queue, cl->memory[1],
		CL_TRUE, 0, size, rt->illu, 0, NULL, NULL);
	size = sizeof(t_item) * rt->opt.item_c;
	cl->memory[2] = clCreateBuffer(cl->context, CL_MEM_READ_WRITE, size, NULL, &ret);
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
	rt->opt.illu_c = 0;
	rt->opt.item_c = 6;

	rt->illu = malloc(sizeof(t_item) * rt->opt.illu_c);
	rt->item = malloc(sizeof(t_item) * rt->opt.item_c);

	//rt->illu[0] = (t_item){SPHERE, {0, -16, 62}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 4, 0, 0, 0.45, {255, 255, 255}, 0, 0, 0};
	rt->item[0] = (t_item){PLANE, {0, 20, 48}, {0, -1, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 0, 60, 40, 0, {255, 255, 255}, 0, 0, 0};
	rt->item[1] = (t_item){PLANE, {-30, 0, 48}, {1, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 0, 40, 40, 0, {224, 133, 156}, 0, 0, 0};
	rt->item[2] = (t_item){PLANE, {30, 0, 48}, {-1, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 0, 40, 40, 0, {125, 125, 232}, 0, 0, 0};
	rt->item[3] = (t_item){PLANE, {0, 0, 68}, {0, 0, -1}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 0, 60, 40, 0, {255, 255, 255}, 0, 0, 0};
	rt->item[4] = (t_item){SPHERE, {-12, 10, 48}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 8, 0, 0, 0, {245, 163, 224}, 0, 0, 0};
	rt->item[5] = (t_item){SPHERE, {12, 8, 48}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 10, 0, 0, 0, {153, 204, 255}, 0, 0, 0};
}

void	func(t_cl *cl) {
	cl_int ret, a = 8, b = 8;
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

cl_int	opencl_create_infrastructure(t_cl *cl, char *src_dir, char *inc_dir)
{
	cl_int			ret;
	t_cl_builder	*cl_builder;

	if (!(cl_builder = (t_cl_builder*)ft_memalloc(sizeof(t_cl_builder))))
		return (-1);
	if ((ret = opencl_platform_device_init(cl_builder))
		|| (ret = opencl_contex_queue_init(cl_builder)))
	{
		release_t_cl_builder(cl_builder);
		free(cl_builder);
		return (ret);
	}
	if ((ret = openclbuilder(cl_builder, src_dir, inc_dir)))
	{
		free(cl_builder);
		return (ret);
	}
	cl->queue = cl_builder->queue;
	cl->device = cl_builder->device;
	cl->context = cl_builder->context;
	cl->program = cl_builder->program;
	cl->platform = cl_builder->platform;
	free(cl_builder);
	return (ret);
}

cl_int	opencl_init(t_cl *cl, t_rt *rt)
{
	figure_init(rt);
	opencl_memobj(cl, rt);
	cl_int ret;
	cl->rt_kernel = clCreateKernel(cl->program, "main_func", &ret);
	ret = clSetKernelArg(cl->rt_kernel, 0, sizeof(cl_mem), &cl->memory[0]);
	ret = clSetKernelArg(cl->rt_kernel, 1, sizeof(cl_mem), &cl->memory[1]);
	ret = clSetKernelArg(cl->rt_kernel, 2, sizeof(cl_mem), &cl->memory[2]);
	func(cl);
	opencl_launch(cl, rt);
	return (0);
}
