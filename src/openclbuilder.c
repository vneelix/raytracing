#include "openclbuilder.h"

static void		*release_t_cl_program(t_cl_program *prog)
{
	if (prog->src)
		ft_nptr_del((void**)prog->src);
	if (prog->program)
	{
		while (prog->size != 0)
			clReleaseProgram(prog->program[--prog->size]);
		free(prog->program);
	}
	return (NULL);
}

static cl_int	build_prog(t_cl_builder *cl,
					t_cl_program *src, t_cl_program *inc)
{
	size_t	i;
	cl_int	ret;

	i = 0;
	while (i != src->size)
	{
		if ((ret = clCompileProgram(
			src->program[i],
			1,
			(const cl_device_id*)&cl->device,
			"-Werror",
			inc->size,
			(const cl_program*)inc->program,
			(const char**)inc->src, NULL, NULL)))
		{
			opencl_print_log(cl, src->program + i, (const char*)src->src[i]);
			return (ret);
		}
		i += 1;
	}
	cl->program = clLinkProgram(cl->context, 1, &cl->device,
		NULL, src->size, (const cl_program*)src->program, NULL, NULL, &ret);
	return (ret);
}

static cl_int	t_cl_program_init(t_cl_builder *cl,
						t_cl_program *prog, char *dir)
{
	char	**file;

	if (ft_get_directory(dir, (void**)&prog->src, (void**)&file, &prog->size))
		return (-1);
	prog->program = opencl_source_to_program(cl,
		(const char**)prog->src, (const char**)file, prog->size);
	ft_nptr_del((void**)file);
	if (!prog->program)
	{
		release_t_cl_program(prog);
		return (-1);
	}
	return (0);
}

cl_int			openclbuilder(t_cl_builder *cl, char *src_dir, char *inc_dir)
{
	cl_int			ret;
	t_cl_program	src;
	t_cl_program	inc;

	ft_bzero(&src, sizeof(t_cl_program));
	ft_bzero(&inc, sizeof(t_cl_program));
	if (t_cl_program_init(cl, &src, src_dir))
		return (-1);
	if ((inc_dir != NULL) && t_cl_program_init(cl, &inc, inc_dir))
	{
		release_t_cl_program(&src);
		return (-1);
	}
	ret = build_prog(cl, &src, &inc);
	release_t_cl_program(&src);
	release_t_cl_program(&inc);
	if (ret)
		release_t_cl_builder(cl);
	return (ret);
}
