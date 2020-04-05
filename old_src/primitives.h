/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   primitives.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/05 11:29:31 by vneelix           #+#    #+#             */
/*   Updated: 2020/02/27 14:24:28 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRIMITIVES_H
# define PRIMITIVES_H

/*
** structs
*/

typedef struct	s_point{
	double		x;
	double		y;
	double		z;
}				t_point;

typedef struct	s_vector{
	double		x;
	double		y;
	double		z;
}				t_vector;

typedef struct	s_light{
	int			t;
	t_point		c;
	t_vector	v;
	double		ise;
}				t_light;

typedef struct	s_viewp{
	double		d;
	t_point		v;
	t_point		c;
	t_vector	dir;
}				t_viewp;

typedef struct	s_sphere{
	t_point		center;
	double		r;
	t_attr		att;
	double		t1;
	double		t2;
}				t_sphere;

typedef struct	s_plane{
	t_point		a;
	t_vector	n;
	t_attr		att;
	double		t;
}				t_plane;

typedef struct	s_cyli{
	t_point		center;
	t_vector	dir;
	double		r;
	t_attr		att;
	double		t1;
	double		t2;
}				t_cyli;

typedef struct	s_cone{
	t_point		center;
	t_vector	dir;
	double		angle;
	double		k;
	t_attr		att;
	double		t1;
	double		t2;
}				t_cone;

typedef struct	s_obj
{
	void		*ptr;
	int			type;
}				t_obj;

typedef	struct	s_obj_p{
	t_point		p;
	t_attr		*att;
}				t_obj_p;

/*
** operations with points
*/
t_point			add_vector_to_point(const t_vector v, const t_point p);
t_point			subtract_vector_from_point(const t_vector v, const t_point p);
t_vector		subtract_point_from_point(const t_point p1, const t_point p2);

/*
** operations with vectors
*/
t_vector		normalize_vec(t_vector v);
t_vector		add_vector_to_vector(const t_vector v1, t_vector v2);
t_vector		subtract_vector_from_vector(const t_vector v1, t_vector v2);
double			scalar_multiple(const t_vector v1, t_vector v2);
t_vector		multiple_vector_by_num(const t_vector v, const double num);
t_vector		divide_vector_by_num(const t_vector v, const double num);
double			len_vector(const t_vector v);
double			angle_vec(t_vector v1, t_vector v2);
t_vector		get_reflect_vec(t_vector d, t_vector n);
t_vector		get_refract_vec(t_vector d, t_vector n, double n1, double n2);
#endif
