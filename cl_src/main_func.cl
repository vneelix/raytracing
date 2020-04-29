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
	if (scalar_multiple(direct, normal) > 0)
		return (-normal);
	return (normal);
}

uint	rgb_to_uint(float3 rgb)
{
	uint		color = 0;

	rgb.x = rgb.x > 255 ? 255 : rgb.x;
	rgb.y = rgb.y > 255 ? 255 : rgb.y;
	rgb.z = rgb.z > 255 ? 255 : rgb.z;
	color |= (uint)(rgb.z);
	color |= (uint)(rgb.y) << 8;
	color |= (uint)(rgb.x) << 16;
	return (color);
}

float3	calc_light(__global struct item *illu, __global struct item *item,
		int item_index, struct opt opt, float3 center, float3 direct, float t)
{
	int			i = -1;
	float3		point, normal, illu_vec;
	float		diffuse = 0.06, shine = 0, ratio = 0;

	point = center + direct * t;
	normal = get_normal(item + item_index, point, center, direct, t);
	point += normal * 0.1f;
	while (++i != opt.illu_c)
	{
		illu_vec = (illu + i)->pref.center - point;
		t = minimal_param(item, opt.item_c, point, illu_vec, NULL);
		if (t >= 0.f && t <= 1.f)
			continue ;
		if ((ratio = scalar_multiple(normalize(illu_vec), normalize(normal))) > 0.001)
		{
			diffuse += (ratio * (illu + i)->pref.k);
			if ((ratio = scalar_multiple(get_reflect_vec(-illu_vec, normal), normalize(-direct))) > 0.001)
				shine += (pow(ratio, (item + item_index)->attr.shine) * (illu + i)->pref.k);
		}
	}
	return (((item + item_index)->attr.color * diffuse) + (float3){255, 255, 255} * shine);
}

float3	traceray(__global struct item *illu, __global struct item *item, struct opt opt, float3 center, float3 direct)
{
	float	t;
	int		item_index;

	t = minimal_param(item, opt.item_c, center, direct, &item_index);
	if (t != INFINITY)
		return (calc_light(illu, item, item_index, opt, center, direct, t));
	return ((float3){0, 0, 0});
}

__kernel void	main_func(__global uint *pixel,
	__global struct item *illu, __global struct item *item, struct opt opt)
{
	int		id;
	float3	point;
	float3	direct;

	id = get_global_id(0);
	point = (float3){(id - (id / opt.w) * opt.w - (opt.w / 2))
		* ((float)(1) / opt.w), (id / opt.h - (opt.h / 2)) * ((float)(1) / opt.h), 1};
	point.x += opt.center.x;
	point.y += opt.center.y;
	point.z += opt.center.z;
	direct = normalize(point - opt.center);
	direct = rotation_x(opt.spin_x * M_PI / 180.f, direct);
	direct = rotation_y(opt.spin_y * M_PI / 180.f, direct);
	pixel[id] = rgb_to_uint(traceray(illu, item, opt, opt.center, direct));
}
