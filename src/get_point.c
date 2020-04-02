/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_point.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/24 20:04:31 by vneelix           #+#    #+#             */
/*   Updated: 2020/02/24 20:15:00 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

void			getp_part2(t_rt *rt, t_obj *obj, t_obj_p *p, t_vector *v)
{
	if (obj->type == 2)
	{
		*v = multiple_vector_by_num(rt->view.dir, ((t_cyli*)(obj->ptr))->t1);
		p->att = &(((t_cyli*)(obj->ptr))->att);
	}
	else if (obj->type == 3)
	{
		*v = multiple_vector_by_num(rt->view.dir, ((t_cone*)(obj->ptr))->t1);
		p->att = &(((t_cone*)(obj->ptr))->att);
	}
	else
		*v = (t_vector){0, 1, 0};
}

t_obj_p			get_point(t_rt *rt, t_obj *obj)
{
	t_obj_p		p;
	t_vector	v;

	if (obj->type == 0)
	{
		v = multiple_vector_by_num(rt->view.dir, ((t_sphere*)(obj->ptr))->t1);
		p.att = &(((t_sphere*)(obj->ptr))->att);
	}
	else if (obj->type == 1)
	{
		v = multiple_vector_by_num(rt->view.dir, ((t_plane*)(obj->ptr))->t);
		p.att = &(((t_plane*)(obj->ptr))->att);
	}
	else
		getp_part2(rt, obj, &p, &v);
	p.p = add_vector_to_point(v, rt->view.c);
	return (p);
}
