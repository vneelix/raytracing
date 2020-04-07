#include "clheader.h"

float3	get_normal(__global struct item *item, float3 point, float3 center, float3 direct, float t)
{
	float3		normal;

	if (item->type == 0)
		normal = item->pref.vector;
	else if (item->type == 1)
		normal = normalize(point - item->pref.center);
	else if (item->type == 2)
		normal = cylinder_normal(item, point, center, direct, t);
	else if (item->type == 3)
		normal = cone_normal(item, point, center, direct, t);
	else if (item->type == 4)
		normal = paraboloid_normal(item, point, center, direct, t);
	else
		normal = (float3){0, 0, 0};
	if (angle(normal, direct) < M_PI_2)
			normal *= -1;
	return (normal);
}

uint	rgb_to_uint(float3 rgb, float diffuse, float shine)
{
	uint		color = 0;

	rgb = (rgb * diffuse) + (float3){255, 255, 255} * shine;
	rgb.x = rgb.x > 255 ? 255 : rgb.x;
	rgb.y = rgb.y > 255 ? 255 : rgb.y;
	rgb.z = rgb.z > 255 ? 255 : rgb.z;
	color |= (uint)(rgb.z);
	color |= (uint)(rgb.y) << 8;
	color |= (uint)(rgb.x) << 16;
	return (color);
}

uint	calc_light(__global struct item *illu, __global struct item *item,
		int item_num, struct opt opt, float3 center, float3 direct, float t)
{
	int			i = 0;
	float3		point, normal, illu_vec;
	float		diffuse = 0.06, shine = 0, ratio;

	point = center + direct * t;
	normal = get_normal(item + item_num, point, center, direct, t);
	while (i < opt.illu_c)
	{
		illu_vec = (illu + i)->pref.center - point;
		if ((ratio = calc_ratio(illu_vec, normal)) > 0.001)
		{
			diffuse += (ratio * (illu + i)->pref.k);
			if ((ratio = calc_ratio(get_reflect_vec(-illu_vec, normal), -direct)) > 0.001)
				shine += (pow(ratio, (item + item_num)->attr.shine) * (illu + i)->pref.k);
		}
		i += 1;
	}
	return (rgb_to_uint((item + item_num)->attr.color, diffuse, shine));
}

uint	traceray(__global struct item *illu, __global struct item *item, struct opt opt, float3 center, float3 direct)
{
	int			i, item_num;
	float		t, t_min;

	i = 0;
	t_min = INFINITY;
	while (i < opt.item_c)
	{
		if ((item + i)->type == 0)
			t = plane(item + i, center, direct);
		else if ((item + i)->type == 1)
			t = sphere(item + i, center, direct);
		else if ((item + i)->type == 2)
			t = cylinder(item + i, center, direct);
		else if ((item + i)->type == 3)
			t = cone(item + i, center, direct);
		else if ((item + i)->type == 4)
			t = paraboloid(item + i, center, direct);
		else
			t = INFINITY;
		if (t < t_min)
		{
			t_min = t;
			item_num = i;
		}
		i += 1;
	}
	if (t_min != INFINITY)
		return (calc_light(illu, item, item_num, opt, center, direct, t_min));
	return (0);
}

__kernel void	main_func(__global uint *pixels,
	__global struct item *illu, __global struct item *item, struct opt opt)
{
	int			pixel;
	float3		point, direct;

	pixel = get_global_id(0);
	point = (float3){(pixel - (pixel / opt.w) * opt.w - (opt.w / 2)) * ((float)(1) / opt.w),
		(pixel / opt.h - (opt.h / 2)) * ((float)(1) / opt.h), 1};
	point.x += opt.center.x;
	point.y += opt.center.y;
	point.z += opt.center.z;
	direct = normalize(point - opt.center);
	direct = rotation_x(opt.spin_x * M_PI / 180.f, direct);
	direct = rotation_y(opt.spin_y * M_PI / 180.f, direct);
	item->pref.vector = normalize(item->pref.vector);
	pixels[pixel] = traceray(illu, item, opt, opt.center, direct);
}
