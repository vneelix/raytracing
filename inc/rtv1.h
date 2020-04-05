/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rtv1.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/03 12:47:38 by vneelix           #+#    #+#             */
/*   Updated: 2020/02/27 17:54:16 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RTV1_H
# define RTV1_H

# include <math.h>
# include <pthread.h>
# include <SDL2/SDL.h>
# include <SDL2/SDL_keyboard.h>
#ifdef __APPLE__
# include <OpenCL/opencl.h>
#else
# include <CL/cl.h>
#endif
# include "libft.h"

typedef struct	s_opencl{
	cl_platform_id		platform_id;
	cl_device_id		device_id;
	cl_context			context;
	cl_command_queue	command_queue;
	cl_program			program;
	cl_mem				memobj[3];
	cl_kernel			kernel;
}				t_opencl;

typedef struct	s_sdl{
	SDL_Window	*win;
	SDL_Surface	*surf;
	SDL_Event	event;
}				t_sdl;

typedef struct __attribute__ ((packed)) s_pref{
	cl_float3	center;
	cl_float3	vector;
	cl_float	radius;
	cl_float	min;
	cl_float	max;
	cl_float	k;
}	t_pref;

typedef struct	__attribute__ ((packed)) s_attr{
	cl_float3	color;
	cl_float	shine;
	cl_float	reflect;
	cl_float	refract;
}	t_attr;

typedef struct __attribute__ ((packed)) s_item{
	t_pref	pref;
	t_attr	attr;
	cl_int	type;
}	t_item;

typedef struct __attribute__ ((packed)) s_opt{
	cl_int		w;
	cl_int		h;
	cl_int		illu_c;
	cl_int		item_c;
	cl_float	spin_x;
	cl_float	spin_y;
	cl_float3	center;
}	t_opt;

typedef struct	s_rt
{
	t_opencl	cl;
	t_sdl		sdl;
	t_opt		opt;
	t_item		*illu;
	t_item		*item;
}				t_rt;

#include <stdio.h>

cl_int		opencl_launch(t_opencl *cl, t_rt *rt);
cl_int		opencl_init(t_opencl *cl, char **sources, cl_uint count, t_rt *rt);

size_t		ft_splits(char *s);
size_t		ft_number(char *s);
int			sdl_init(t_sdl *sdl);
int			sdl_loop(t_sdl *sdl, t_rt *rt);
int			get_scene(t_rt *rt, char *file_name);
cl_float	get_float(char *string, size_t	*ret);
cl_float3	get_float3(char *string, size_t	*ret);
size_t		extract_float(char *keyword, char *string, void *arg);
size_t		extract_float3(char *keyword, char *string, void *arg);
size_t		extract_param(char *string, t_item *item, __uint32_t *flags);
size_t		get_item(t_item *item, char *file, __uint32_t pref, __uint32_t req);

# define DIFFUSE 0.06
# define W 1024
# define H 1024

#define	CENTER	1
#define	VECTOR	2
#define	RADIUS	4
#define	MIN		8
#define	MAX		16
#define	K		32
#define RGB		64
#define	SHINE	128
#define	REFLECT	256
#define	REFRACT	512

#define	ILLUM_REQ	(CENTER | K)
#define	PLANE_REQ	(CENTER | VECTOR | RGB)
#define	SPHERE_REQ	(CENTER | RADIUS | RGB)
#define	ILLUM_PREF	(CENTER | RGB | K)
#define	PLANE_PREF	(CENTER | VECTOR | RGB | SHINE | REFRACT | REFLECT)
#define	SPHERE_PREF	(CENTER | RADIUS | RGB | SHINE | REFRACT | REFLECT)
#endif
