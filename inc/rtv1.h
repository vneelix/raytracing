/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rtv1.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/03 12:47:38 by vneelix           #+#    #+#             */
/*   Updated: 2020/07/03 22:58:38 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RTV1_H
# define RTV1_H

# include <stdio.h>
# include <errno.h>
# include <math.h>
# include "SDL2/SDL.h"
# include "SDL2/SDL_keyboard.h"
#ifdef __APPLE__
# include <OpenCL/opencl.h>
#else
# include <CL/cl.h>
#endif
# include "libft.h"

typedef struct	s_cl{
	cl_platform_id		platform;
	cl_device_id			device;
	cl_context				context;
	cl_command_queue	queue;
	cl_program				program;
	cl_mem						memory[4];
	cl_kernel					rt_kernel, genhemisphere_kernel;
}				t_cl;

typedef struct	s_sdl{
	uint32_t		*ptr;
	SDL_Window		*win;
	SDL_Texture		*tex;
	SDL_Renderer	*ren;
	SDL_Event		event;
}				t_sdl;

typedef enum m_itype {
	POINT,
	PLANE,
	SPHERE,
	CYLINDER,
	CONE,
	ELLIPSOID,
	PARABOLOID,
	BOX
} t_itype;

typedef struct item {
	/*
	** Item params
	*/
	t_itype		type;
	cl_float3	center;
	cl_float3	normal;
	cl_float3	vector;
	cl_float3	vMin, vMax;
	cl_float	radius, min, max, k;
	/*
	** Item properties
	*/
	cl_float3	color;
	cl_float	reflectRatio, refractRatio, shineRatio;
	/*
	** Euler angles
	*/
	cl_float	x, y, z;
} t_item;

typedef struct s_opt{
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
	t_cl		cl;
	t_sdl		sdl;
	t_opt		opt;
	t_item	*illu;
	t_item	*item;
}				t_rt;

cl_int		opencl_launch(t_cl *cl, t_rt *rt);
cl_int		opencl_init(t_cl *cl, char **sources, t_rt *rt);

size_t		ft_splits(char *s);
size_t		ft_number(char *s);
void		*nptr_del(void **nptr);
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
# define W 800
# define H 800

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

#define	ATTR	(RGB | SHINE | REFLECT |REFRACT)
#define	REST	(MIN | MAX | K)
#define	PLANE_REQ	(CENTER | VECTOR | RGB)
#define	PLANE_PREF	(CENTER | VECTOR | ATTR)
#define	SPHERE_REQ	(CENTER | RADIUS | RGB)
#define	SPHERE_PREF	(CENTER | RADIUS | ATTR | VECTOR)
#define CONE_REQ (CENTER | VECTOR | K | RGB)
#define	CONE_PREF (CENTER | VECTOR | ATTR | REST)
#define	CYLINDER_REQ (CENTER | RADIUS | VECTOR | RGB)
#define	CYLINDER_PREF (CENTER | RADIUS | VECTOR | ATTR | (REST - K))
#endif
