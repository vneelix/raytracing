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
	cl->cam_reper = clCreateBuffer(cl->context, CL_MEM_READ_WRITE, sizeof(cl_float3) * 4, NULL, &ret);
	ret = clEnqueueWriteBuffer(cl->queue, cl->cam_reper,
		CL_TRUE, 0, sizeof(cl_float3), &rt->opt.center, 0, NULL, NULL);
	cl_int val = 0;
	cl_uint	address_bits = 0;
	ret = clGetDeviceInfo(cl->device, CL_DEVICE_ADDRESS_BITS, sizeof(cl_uint), &address_bits, NULL);
	address_bits /= 8;
	cl->active_item = clCreateBuffer(cl->context, CL_MEM_READ_WRITE, address_bits, NULL, &ret);
	ret = clEnqueueWriteBuffer(cl->queue, cl->active_item,
		CL_TRUE, 0, address_bits, &val, 0, NULL, NULL);
	if (ret)
		return (ret);
	return (0);
}

cl_int		opencl_launch(t_cl *cl, t_rt *rt)
{
	cl_int	ret;
	size_t	work_size;

	work_size = rt->opt.w * rt->opt.h;
	ret = clSetKernelArg(cl->rt_kernel, 7, sizeof(t_opt), &(rt->opt));
	ret = clEnqueueNDRangeKernel(cl->queue,
		cl->rt_kernel, 1, NULL, &(work_size), NULL, 0, NULL, NULL);
	ret = clEnqueueReadBuffer(cl->queue, cl->memory[0], CL_TRUE, 0, work_size * 4, rt->sdl.ptr, 0, NULL, NULL);
	return (ret);
}

cl_int	find_item_kernel_init(t_cl *cl) {
	cl_int	ret;

	cl->find_item_kernel = clCreateKernel(cl->program, "FindItem", &ret);
	if (ret) {
		return -1;
	}
	if (clSetKernelArg(cl->find_item_kernel, 0, sizeof(cl_mem), &cl->cam_reper) ||
			clSetKernelArg(cl->find_item_kernel, 1, sizeof(cl_mem), &cl->memory[2]) ||
				clSetKernelArg(cl->find_item_kernel, 2, sizeof(cl_mem), &cl->active_item)) {
		return -1;
	}
	return 0;
}

cl_int	find_item_kernel_launch(t_cl *cl, cl_int x, cl_int y, t_opt *opt) {
	cl_int	ret;
	size_t	worksize;

	ret = 0;
	worksize = 1;
	if (clSetKernelArg(cl->find_item_kernel, 3, sizeof(cl_int), &x) ||
			clSetKernelArg(cl->find_item_kernel, 4, sizeof(cl_int), &y) ||
				clSetKernelArg(cl->find_item_kernel, 5, sizeof(t_opt), opt)) {
		return -1;
	}
	if (clEnqueueNDRangeKernel(cl->queue, cl->find_item_kernel, 1, NULL, &worksize, NULL, 0, NULL, NULL)) {
		return -1;
	}
	return 0;
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
	rt->opt.item_c = 5;

	rt->illu = malloc(sizeof(t_item) * rt->opt.illu_c);
	rt->item = malloc(sizeof(t_item) * rt->opt.item_c);

	rt->illu[0] = (t_item){POINT, {0, -16, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 4, 0, 0, 0.45, {255, 255, 255}, 0, 0, 0};
	rt->item[0] = (t_item){PLANE, {0, 20, 48}, {0, -1, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 0, 60, 40, 0, {0, 0, 255}, 0, 0, 0};
	rt->item[1] = (t_item){PLANE, {-30, 0, 48}, {1, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 0, 40, 40, 0, {224, 133, 156}, 0, 0, 0};
	rt->item[2] = (t_item){PLANE, {30, 0, 48}, {-1, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 0, 40, 40, 0, {125, 125, 232}, 0, 0, 0};
	rt->item[3] = (t_item){PLANE, {0, 0, 68}, {0, 0, -1}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 0, 60, 40, 0, {0, 0, 255}, 0, 0, 0};
	rt->item[4] = (t_item){CYLINDER, {0, -8, 48}, {0, 0, 0}, {0, -1, 0}, {0, 0, 0}, {0, 0, 0}, 8, -10, 10, 0, {245, 163, 224}, 0, 0, 0};

	/* rt->illu[0] = (t_item){POINT, {0, -16, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 4, 0, 0, 0.45, {255, 255, 255}, 0, 0, 0};
	rt->item[0] = (t_item){PLANE, {0, 20, 48}, {0, -1, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 0, 60, 40, 0, {0, 0, 255}, 0, 0, 0};
	rt->item[1] = (t_item){PLANE, {-30, 0, 48}, {1, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 0, 40, 40, 0, {224, 133, 156}, 0, 0, 0};
	rt->item[2] = (t_item){PLANE, {30, 0, 48}, {-1, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 0, 40, 40, 0, {125, 125, 232}, 0, 0, 0};
	rt->item[3] = (t_item){PLANE, {0, 0, 68}, {0, 0, -1}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 0, 60, 40, 0, {0, 0, 255}, 0, 0, 0};
	rt->item[4] = (t_item){SPHERE, {-12, 10, 48}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 8, 0, 0, 0, {245, 163, 224}, 0, 0, 0};
	rt->item[5] = (t_item){SPHERE, {12, 8, 48}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 10, 0, 0, 0, {153, 204, 255}, 0, 0, 0}; */
}

void	func(t_cl *cl) {
	cl_int ret, a = 8, b = 8;
	size_t worksize = a * b + 1;

	cl->memory[3] = clCreateBuffer(cl->context, CL_MEM_READ_WRITE, sizeof(cl_float3) * worksize, NULL, &ret);
	cl->genhemisphere_kernel = clCreateKernel(cl->program, "GenHemisphere", &ret);
	ret = clSetKernelArg(cl->genhemisphere_kernel, 0, sizeof(cl_mem), &cl->memory[3]);
	ret = clSetKernelArg(cl->genhemisphere_kernel, 1, sizeof(cl_int), &a);
	ret = clSetKernelArg(cl->genhemisphere_kernel, 2, sizeof(cl_int), &b);
	ret = clEnqueueNDRangeKernel(cl->queue, cl->genhemisphere_kernel, 1, NULL, &worksize, NULL, 0, NULL, NULL);
	ret = clSetKernelArg(cl->rt_kernel, 4, sizeof(cl_mem), &cl->memory[3]);
	ret = clSetKernelArg(cl->rt_kernel, 5, sizeof(cl_int), &worksize);

	cl->move_origin_kernel = clCreateKernel(cl->program, "MoveOrigin", &ret);
	ret = clSetKernelArg(cl->move_origin_kernel, 0, sizeof(cl_mem), &cl->cam_reper);
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
	ret = clSetKernelArg(cl->rt_kernel, 3, sizeof(cl_mem), &cl->cam_reper);
	ret = clSetKernelArg(cl->rt_kernel, 6, sizeof(cl_mem), &cl->active_item);
	func(cl);
	find_item_kernel_init(cl);
	opencl_launch(cl, rt);
	return (0);
}
