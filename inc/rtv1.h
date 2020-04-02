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

# include <stdlib.h>
# include <fcntl.h>
# include <math.h>
# include <pthread.h>
# include <unistd.h>
# include <SDL2/SDL.h>
# include <SDL2/SDL_keyboard.h>
#ifdef __APPLE__
# include <OpenCL/opencl.h>
#else
# include <CL/cl.h>
#endif
# include "primitives.h"

typedef struct	s_sdl{
	SDL_Window			*win;
	SDL_Surface			*surf;
	SDL_Event			event;
}				t_sdl;

typedef struct	s_opencl{
	cl_platform_id		platform_id;
	cl_device_id		device_id;
	cl_context			context;
	cl_command_queue	command_queue;
	cl_program			program;
	cl_mem				memobj[3];
	cl_kernel			kernel;
}				t_opencl;

typedef struct __attribute__ ((aligned)) s_pref{
	cl_float3			center;
	cl_float3			vector;
	cl_float			radius;
	cl_float			min;
	cl_float			max;
	cl_float			k;
}				t_pref;

typedef struct __attribute__ ((aligned)) s_item{
	t_pref				pref;
	t_attr				attr;
	cl_int				type;
}				t_item;

typedef struct __attribute__ ((aligned)) s_illum{
	cl_float			ratio;
	cl_float3			center;
}				t_illum;

typedef struct __attribute__ ((aligned)) s_opt{
	cl_int				w;
	cl_int				h;
	cl_int				illum_c;
	cl_int				item_c;
}				t_opt;

typedef struct	s_rt
{
	t_sdl		sdl;
	t_opencl	cl;
	t_viewp		view;
	__uint16_t	lt_c;
	__uint16_t	sp_c;
	__uint16_t	pl_c;
	__uint16_t	cr_c;
	__uint16_t	cn_c;
	t_light		*lht;
	t_plane		*pln;
	t_sphere	*sph;
	t_cyli		*clr;
	t_cone		*cne;
	int			depth;
	int			angle_x;
	int			angle_y;
}				t_rt;

typedef struct	s_thread{
	t_rt		rt;
	int			min;
	int			max;
}				t_thread;

/*
** main func
*/
int				key_func(int key, void *arg);
int				thread_manager(t_rt *rt);
void			ft_rotation(t_rt *rt);
char			*ft_getfile(int fd);

/*
** trace functions
*/
__uint32_t		calc_light(t_rt *rt, t_obj *obj);
t_obj_p			get_point(t_rt *rt, t_obj *obj);
int				shadow_management(t_rt *rt, t_vector *l, t_point *p);
t_point			correct_point(t_point p, t_vector n);
__uint32_t		get_color(t_rt *rt, t_obj_p *p, t_vector *n, double l_ratio[2]);
__uint32_t		closest_surface(t_rt *rt);
void			param_normalize(double *a, double *b);
__uint32_t		raytrace(t_rt *rt);

/*
** rgb func
*/
__uint32_t		multiple_color(__uint32_t color, double l_ratio);
__uint32_t		add_color_to_color(__uint32_t c1, __uint32_t c2);

/*
** sphere
*/
void			sphere_intersect(t_rt *rt, t_vector dir, t_point center);
int				sphere_intersect_shadow(t_rt *rt, t_vector dir, t_point center);
t_vector		sphere_normal(t_sphere *s, t_point *p);

/*
** plane
*/
void			plane_intersect(t_rt *rt, t_vector dir, t_point center);
int				plane_intersect_shadow(t_rt *rt, t_vector dir, t_point center);

/*
** cyli
*/
void			cyli_intersect(t_rt *rt, t_vector dir, t_point center);
int				cyli_intersect_shadow(t_rt *rt, t_vector dir, t_point center);
t_vector		cyli_normal(t_rt *rt, t_cyli *clr, t_point *p);

/*
** cone
*/
void			cone_intersect(t_rt *rt, t_vector dir, t_point center);
int				cone_intersect_shadow(t_rt *rt, t_vector dir, t_point center);
t_vector		cone_normal(t_rt *rt, t_cone *cn, t_point *p);

/*
** other
*/
void			*ft_memcpy(void *dst, const void *src, size_t n);
void			*ft_memalloc(size_t size);
void			*alloc_mem(t_rt *rt);
void			mem_free(t_rt *rt);
void			ft_bzero(void *memory, size_t size);

/*
** sdl
*/
int				sdl_init(t_sdl *sdl);
int				sdl_loop(t_sdl *sdl, t_rt *rt);

int				ft_isdigit(char ch);

# define DIFFUSE 0.06
# define W 800
# define H 800
# define T 16
# define S 2

#endif
