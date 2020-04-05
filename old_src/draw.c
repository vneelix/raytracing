/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/24 20:00:46 by vneelix           #+#    #+#             */
/*   Updated: 2020/02/26 16:26:11 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

__uint32_t		raytrace(t_rt *rt)
{
	if (rt->sp_c != 0)
		sphere_intersect(rt, rt->view.dir, rt->view.c);
	if (rt->pl_c != 0)
		plane_intersect(rt, rt->view.dir, rt->view.c);
	if (rt->cr_c != 0)
		cyli_intersect(rt, rt->view.dir, rt->view.c);
	if (rt->cn_c != 0)
		cone_intersect(rt, rt->view.dir, rt->view.c);
	return (closest_surface(rt));
}
