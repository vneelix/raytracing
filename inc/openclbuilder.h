#ifndef OPENCLBUILDER_H
# define OPENCLBUILDER_H
# include "libft.h"
# ifdef __APPLE__
#  include <OpenCL/opencl.h>
# else
#  include <CL/cl.h>
# endif

typedef struct	s_cl_builder{
	cl_platform_id		platform;
	cl_device_id		device;
	cl_context			context;
	cl_command_queue	queue;
	cl_program			program;
}				t_cl_builder;

typedef struct	s_cl_program {
	size_t				size;
	char				**src;
	cl_program			*program;
}				t_cl_program;

void			*release_t_cl_builder(t_cl_builder *cl);
cl_int			opencl_platform_device_init(t_cl_builder *cl);
cl_int			opencl_contex_queue_init(t_cl_builder *cl);
void			opencl_print_log(t_cl_builder *cl,
				cl_program *prog, const char *file);
cl_program		*opencl_source_to_program(t_cl_builder *cl,
				const char **src, const char **src_file, size_t src_counter);
cl_int			openclbuilder(t_cl_builder *cl, char *src_dir, char *inc_dir);
#endif
