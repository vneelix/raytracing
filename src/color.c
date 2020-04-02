/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/18 18:07:15 by vneelix           #+#    #+#             */
/*   Updated: 2020/03/16 18:28:06 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

/*__uint32_t		multiple_color(__uint32_t color, double l_ratio)
{
	int			i;
	int			s;
	__uint32_t	channel;
	__uint32_t	n_color;

	i = -1;
	s = 16;
	n_color = 0;
	while ((++i) < 3)
	{
		channel = ((color & (255 << s)) >> s);
		channel = (channel * l_ratio);
		if (channel > 255)
			channel = 255;
		n_color |= (channel << s);
		s -= 8;
	}
	return (n_color);
}

__uint32_t		add_color_to_color(__uint32_t c1, __uint32_t c2)
{
	int			i;
	int			s;
	__uint32_t	channel;
	__uint32_t	n_color;

	i = -1;
	s = 16;
	n_color = 0;
	while ((++i) < 3)
	{
		channel = ((c1 & (255 << s)) >> s);
		channel += ((c2 & (255 << s)) >> s);
		if (channel > 255)
			channel = 255;
		n_color |= (channel << s);
		s -= 8;
	}
	return (n_color);
}

__uint32_t		get_color(t_rt *rt, t_obj_p *p, t_vector *n, double l_ratio[2])
{
	t_rt		*n_rt;
	__uint32_t	color;
	__uint32_t	refl_c = 0;
	__uint32_t	refr_c = 0;

	n_rt = malloc(sizeof(t_rt));
	color = add_color_to_color(multiple_color(p->att->color,
				l_ratio[0]), multiple_color(0xffffff, l_ratio[1]));
	if (rt->depth > 0 && (p->att->reflect != 0.0 || p->att->refract != 0))
	{
		*n_rt = *rt;
		n_rt->depth -= 1;
		if (p->att->reflect != 0.0)
		{
			n_rt->view.c = correct_point(p->p, *n);
			n_rt->view.dir = get_reflect_vec(rt->view.dir, *n);
			refl_c = multiple_color(raytrace(n_rt), p->att->reflect);
			color = add_color_to_color(multiple_color(color, 1.0 - p->att->reflect),
					multiple_color(0xffffff, l_ratio[1]));
			color = add_color_to_color(color, refl_c);
		}
		if (p->att->refract != 0.0)
		{
			double k = 0.6;
			n_rt->view.c = correct_point(p->p,
				multiple_vector_by_num(*n, -1.0));
			n_rt->view.dir = get_refract_vec(
				rt->view.dir, *n, 1.0, p->att->refract);
			if (n_rt->view.dir.x == 0.0
				&& n_rt->view.dir.y == 0.0
					&& n_rt->view.dir.z == 0.0)
			{
				free(n_rt);
				return (multiple_color(color, 1.0 - k));
			}
			t_point	t = n_rt->view.c;
			n_rt->view.c = add_vector_to_point(
				multiple_vector_by_num(*n, -0.001), p->p);
			refr_c = raytrace(n_rt);
			color = add_color_to_color(
				multiple_color(color, 1.0 - k), multiple_color(refr_c, k));
		}
	}
	free(n_rt);
	return (color);
}*/
