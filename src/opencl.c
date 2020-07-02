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

cl_int		opencl_program(t_cl *cl, char **sources)
{
	cl_int	ret;
	cl_uint	count;

	/* count = 0;
	while (sources[count] != NULL)
		count += 1;
	cl->program = clCreateProgramWithSource(cl->context, count, (const char**)(sources), NULL, &ret);
	if (ret)
		return (ret);
	size_t	header_count = 0;
	cl_program *header_program;
	char **header_file = ft_directory("cl_inc/", &header_count);
	printf("%zu\n", header_count);
	{
		header_program = malloc(sizeof(cl_program) * header_count);
		for (cl_uint i = 0; i != header_count; i += 1) {
			char *file = ft_getfile(header_file[i], 0);
			header_program[i] = clCreateProgramWithSource(cl->context, 1, (const char**)(&file), NULL, &ret);
			if (ret) {
				return ret;
			}
			free(file);
		}
	}
	ret = clCompileProgram(cl->program, 1, &cl->device, NULL, header_count, header_program, (const char**)header_file, NULL, NULL); */
	/* if ((ret = clBuildProgram(cl->program, 1, &(cl->device), "-Icl_inc", NULL, NULL)))
	{
		size_t log_size;
		clGetProgramBuildInfo(cl->program, cl->device, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
		char *log = (char *) malloc(log_size);
		clGetProgramBuildInfo(cl->program, cl->device, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);
		printf("%s\n", log);
	} */
	{
		const char *src[7] = {
			ft_getfile("cl_src/add_func.cl", 0),
			//ft_getfile("cl_src/cylinder.cl", 0),
			//ft_getfile("cl_src/paraboloid.cl", 0),
			ft_getfile("cl_src/affine_transform.cl", 0),
			//ft_getfile("cl_src/ellipsoid.cl", 0),
			ft_getfile("cl_src/plane.cl", 0),
			//ft_getfile("cl_src/box.cl", 0),
			ft_getfile("cl_src/generate_hemisphere.cl", 0),
			ft_getfile("cl_src/quaternion.cl", 0),
			//ft_getfile("cl_src/cone.cl", 0),
			ft_getfile("cl_src/main_func.cl", 0),
			ft_getfile("cl_src/sphere.cl", 0)
		};
		const char *hdr[3] = {
			ft_getfile("cl_inc/affine_transform.h", 0),
			ft_getfile("cl_inc/clheader.h", 0),
			ft_getfile("cl_inc/quaternion.h", 0)
		};
		cl_program input_header[3] = {
			clCreateProgramWithSource(cl->context, 1, hdr, NULL, &ret),
			clCreateProgramWithSource(cl->context, 1, hdr + 1, NULL, &ret),
			clCreateProgramWithSource(cl->context, 1, hdr + 2, NULL, &ret)
		};
		char *input_header_name[3] = {
			"affine_transform.h",
			"clheader.h",
			"quaternion.h"
		};
		cl_program input_program[7];
		ft_bzero(input_program, sizeof(cl_program) * 6);
		for (int i = 0; i != 7; i += 1) {
			input_program[i] = clCreateProgramWithSource(cl->context, 1, src + i, NULL, &ret);
			ret = clCompileProgram(input_program[i], 0, NULL, "-Werror", 3, input_header, (const char**)input_header_name, NULL, NULL);
			{
				size_t log_size = 0;
				clGetProgramBuildInfo(input_program[i], cl->device, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
				char *log = (char *) malloc(log_size);
				clGetProgramBuildInfo(input_program[i], cl->device, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);
				if (log_size != 1) {
					printf("File %d\n", i);
					printf("%s\n", log);
					exit(0);
				}
			}
			if (ret) {
				exit(0);
			}
		}
		cl->program = clLinkProgram(cl->context, 1, &cl->device, NULL, 7, input_program, NULL, NULL, &ret);
		if (ret) {
			exit(0);
		}
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
	rt->opt.item_c = 5;

	rt->illu = malloc(sizeof(t_item) * rt->opt.illu_c);
	rt->item = malloc(sizeof(t_item) * rt->opt.item_c);

	rt->illu[0] = (t_item){SPHERE, {0, -32, 64}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 4, 0, 0, 0.6, {255, 255, 255}, 0, 0, 0};
	rt->item[0] = (t_item){SPHERE, {0, 0, 64}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 8, 0, 0, 0, {255, 204, 204}, 0, 0, 0};
	rt->item[1] = (t_item){PLANE, {0, 12, 64}, {0, -1, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 0, 40, 40, 0, {153, 179, 255}, 0, 0, 0};
	rt->item[2] = (t_item){PLANE, {-20, -8, 64}, {1, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 0, 40, 40, 0, {204, 255, 255}, 0, 0, 0};
	rt->item[3] = (t_item){PLANE, {20, -8, 64}, {-1, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 0, 40, 40, 0, {204, 204, 255}, 0, 0, 0};
	rt->item[4] = (t_item){PLANE, {0, -8, 84}, {0, 0, -1}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 0, 40, 40, 0, {255, 204, 230}, 0, 0, 0};
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
	cl->queue = clCreateCommandQueue(cl->context, cl->device, 0, &ret);
	return (ret);
}

void	func(t_cl *cl) {
	cl_int ret, a = 4, b = 16;
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
	if (platform_device_init(cl))
		return (-1);
	if (contex_queue_init(cl))
		return (-1);
	opencl_program(cl, NULL);
	figure_init(rt);
	opencl_memobj(cl, rt);
	cl_int ret;
	ret = clSetKernelArg(cl->rt_kernel, 0, sizeof(cl_mem), &cl->memory[0]);
	ret = clSetKernelArg(cl->rt_kernel, 1, sizeof(cl_mem), &cl->memory[1]);
	ret = clSetKernelArg(cl->rt_kernel, 2, sizeof(cl_mem), &cl->memory[2]);
	func(cl);
	opencl_launch(cl, rt);
	return (0);
}
