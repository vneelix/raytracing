/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/12 15:12:04 by vneelix           #+#    #+#             */
/*   Updated: 2020/09/12 19:17:53 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RT_H
# define RT_H

# include <errno.h>
# include <math.h>
# include <stdlib.h>
# ifdef __APPLE__
#  include <OpenCL/opencl.h>
# else
#  include <CL/cl.h>
# endif
# include "SDL2/SDL.h"
# include "SDL2/SDL_image.h"
# include "SDL2/SDL_keyboard.h"
# include "libft.h"

typedef struct	s_control
{
	int					press_lm;
	int					press_rm;
	int					help;
	int					col;
}				t_control;

typedef struct	s_interface
{
	SDL_Surface			*spectr;
	SDL_Texture			*image;
	SDL_Texture			*menu;
}				t_interface;

typedef struct	s_cl{
	cl_command_queue	queue;
	cl_device_id		device;
	cl_context			context;
	cl_program			program;
	cl_platform_id		platform;
	cl_mem				camera;
	cl_mem				active_item;
	cl_mem				item_buffer;
	cl_mem				illu_buffer;
	cl_mem				pixel_buffer;
	cl_mem				texture_buffer;
	cl_mem				soft_shadow_buffer;
	cl_mem				ambient_occlusion_buffer;
	cl_kernel			rotate_kernel;
	cl_kernel			renderer_kernel;
	cl_kernel			find_item_kernel;
	cl_kernel			move_origin_kernel;
	cl_kernel			change_color_kernel;
	cl_kernel			genhemisphere_kernel;
	cl_kernel			post_processing_kernel;
}				t_cl;

typedef struct	s_sdl{
	uint32_t			*ptr;
	SDL_Window			*win;
	SDL_Texture			*tex;
	SDL_Renderer		*ren;
	SDL_Event			event;
}				t_sdl;

typedef enum	e_itype{
	POINT,
	PLANE,
	SPHERE,
	CYLINDER,
	CONE,
	ELLIPSOID,
	PARABOLOID,
}				t_itype;

typedef struct	s_textr{
	cl_float3	vector_n;
	cl_float3	vector_e;
	cl_int		n_texture;
	cl_int		normal_map;
	cl_float	texture_scale;
	cl_float	texture_tilling;
}				t_textr;

typedef struct	s_item
{
	t_itype				type;
	cl_float3			center;
	cl_float3			normal;
	cl_float3			vector;
	cl_float			radius;
	cl_float			min;
	cl_float			max;
	cl_float			k;
	cl_float3			color;
	cl_float			reflectratio;
	cl_float			refractratio;
	cl_float			refractindex;
	cl_float			shineratio;
	t_textr				textr;
}				t_item;

typedef struct	s_opt
{
	cl_int				w;
	cl_int				h;
	cl_int				illu_c;
	cl_int				item_c;
	cl_float3			center;
	cl_int				texture_c;
	cl_int				texture_size;
	cl_uint				soft_shadow_buffer_size;
	cl_uint				ambient_occlusion_buffer_size;
	cl_uint				flags;
	cl_float3			background;
	cl_float			diffuse;
}				t_opt;

typedef struct	s_camera
{
	cl_float			x;
	cl_float			y;
	cl_float			z;
	cl_float3			temp[4];
	cl_float3			reper[4];
}				t_camera;

typedef struct	s_rt
{
	t_cl				cl;
	t_sdl				sdl;
	t_opt				opt;
	t_item				*illu;
	t_item				*item;
	t_control			flag;
	uint				*texture;
	char				**file_name;
	t_interface			intrf;
	t_item				cam;
	uint8_t				ppf;
}				t_rt;

typedef struct	s_uint32
{
	__uint32_t			pref;
	__uint32_t			req;
}				t_uint32;

float			clamp(float l, float r, float val);
int				sdl_loop(t_sdl *sdl, t_rt *rt);
size_t			get_sc(t_rt *rt, t_item *item, char *file);
size_t			get_illum(t_rt *rt, t_item *item, char *file);
size_t			get_plane(t_rt *rt, t_item *item, char *file);
size_t			get_sphere(t_rt *rt, t_item *item, char *file);
size_t			get_cylinder(t_rt *rt, t_item *item, char *file);
size_t			get_cone(t_rt *rt, t_item *item, char *file);
size_t			get_paraboloid(t_rt *rt, t_item *item, char *file);
int				get_count(t_rt *rt, char *file);
int				extract_item(t_rt *rt, char *file);
void			create_item_prep(t_rt *rt);
int				create_item(t_rt *rt, char *file);
int				get_scene(t_rt *rt, char *file_name);
size_t			ft_splits(char *s);
size_t			ft_number(char *s);
void			*nptr_del(void **nptr);
cl_float3		normalize(cl_float3 vect);
cl_int			get_int(char *string, size_t *ret);
char			*get_string(char *string, size_t *ret);
size_t			extract_int(char *keyword, char *string, void *arg);
size_t			extract_string(t_rt *rt, char *keyword, char *string,
				void *arg);
cl_float		get_float(char *string, size_t *ret);
size_t			extract_float(char *keyword, char *string, void *arg);
cl_float3		get_float3(char *string, size_t *ret);
size_t			extract_float3(char *keyword, char *string, void *arg);
size_t			get_item(t_rt *rt, t_item *item, char *file, t_uint32 t);
size_t			extract_param_part1(char *str, t_item *item,
				uint32_t *f);
size_t			extract_param_part2(char *str, t_item *item,
				uint32_t *f);
size_t			extract_param_part3(char *str, t_item *item,
				uint32_t *f);
size_t			extract_param_part4(t_rt *rt, char *str, t_item *item,
				uint32_t *f);
size_t			extract_param_part5(char *str, t_item *item,
				uint32_t *f);
size_t			extract_param(t_rt *rt, char *str, t_item *item, uint32_t *f);
int				sdl_keyhook(SDL_Keycode keycode, t_rt *rt);
void			change_color(t_rt *rt);
void			present_win(t_rt *rt);
void			help(t_rt *rt);
int				sdl_mousehook(t_sdl *sdl, t_rt *rt);
int				push_mouse(t_sdl *sdl, t_rt *rt);
cl_int			opencl_launch(t_cl *cl, t_rt *rt);
void			figure_init(t_rt *rt);
cl_int			opencl_build(t_cl *cl);
cl_int			opencl_create_infrastructure(t_cl *cl);
cl_int			opencl_init(t_cl *cl, t_rt *rt);
cl_int			opencl_platform_device_init(t_cl *cl);
cl_int			opencl_contex_queue_init(t_cl *cl);
cl_int			move_origin_kernel_init(t_cl *cl);
cl_int			move_origin_kernel_launch(t_cl *cl, cl_uint flags);
cl_int			find_item_kernel_init(t_cl *cl);
cl_int			find_item_kernel_launch(t_cl *cl, cl_int x, cl_int y,
				t_opt *opt);
cl_int			rotate_kernel_init(t_cl *cl);
cl_int			rotate_kernel_launch(t_cl *cl, cl_float3 euler_angle,
				cl_uint flags);
cl_int			change_color_kernel_init(t_cl *cl);
cl_int			change_color_kernel_launch(t_cl *cl, cl_float3 color);
cl_int			genhemisphere_kernel(t_cl *cl,
				cl_uint2 shadow_map_size, cl_uint2 ambient_map_size);
size_t			set_item_par(t_item *item, size_t n);
int				get_file_name(t_rt *rt);
int				read_texture(t_rt *rt);
int				sdl_image_init(t_rt *rt);
int				sdl_init(t_sdl *sdl);
SDL_Surface		**prepare_read_texture(t_rt *rt);
void			free_surfaces(SDL_Surface **surf);
void			free_rt(t_rt *rt);
void			close_programm(t_rt *rt, char *message);
SDL_Surface		*texture_convert(SDL_Surface *surf);
float			vector_len(const cl_float3 vect);

cl_int			post_processing_kernel_init(t_cl *cl);
cl_int			post_processing_kernel_launch(t_cl *cl, t_rt *rt);

void			release_program(cl_program program);
void			release_context(cl_context context);
void			release_command_queue(cl_command_queue queue);
void			release_device_id(cl_device_id device);
void			release_sdl(t_sdl *sdl, t_interface *ife);
void			release_kernel(cl_kernel kernel);
void			release_memory(cl_mem memory);
void			release_cl(t_cl *cl);

# define W 1024
# define H 768
# define CENTER	1
# define VECTOR	2
# define RADIUS	4
# define MIN 8
# define MAX 16
# define K 32
# define RGB 64
# define SHINE 128
# define REFLECT 256
# define REFRACT 512
# define TEXTURE_FILE 1024
# define NORMAL_MAP_FILE 2048
# define TEXTURE_TILLING 4096
# define TEXTURE_SCALE 8192
# define REFRACT_I 16384
# define BLACK (SDL_Color){0, 0, 0}
# define WHITE (SDL_Color){255, 255, 255}
# define COL_MENU (SDL_Rect){15, 15, 200, 100}
# define D_W 0.1
# define D_H 0.1
# define ATTR (ATTR1 | ATTR2)
# define ATTR1 (RGB | SHINE | REFLECT | REFRACT | REFRACT_I | TEXTURE_FILE)
# define ATTR2 (NORMAL_MAP_FILE | TEXTURE_TILLING | TEXTURE_SCALE)
# define REST (MIN | MAX | K)
# define PLANE_REQ	(CENTER | VECTOR | RGB)
# define PLANE_PREF	(CENTER | VECTOR | ATTR | REST)
# define SPHERE_REQ	(CENTER | RADIUS | RGB | VECTOR)
# define SPHERE_PREF	(CENTER | RADIUS | ATTR | VECTOR)
# define CONE_REQ (CENTER | VECTOR | K | RGB)
# define CONE_PREF (CENTER | VECTOR | ATTR | REST)
# define CYLINDER_REQ (CENTER | RADIUS | VECTOR | RGB)
# define CYLINDER_PREF (CENTER | RADIUS | VECTOR | ATTR | (REST - K))

# define AMBIENT 1
# define ALIASING 2
# define RECURSION 4
# define SOFT_SHADOWS 8
# define FAST_RENDER 16
#endif
