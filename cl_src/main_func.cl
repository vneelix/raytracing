#include "clheader.h"

struct state
{
	float3	point;
	float3	normal;
	float3	direct;
};

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

float3	calc_light(__global struct item *illu, __global struct item *item, int item_index, struct opt opt,
					float3 center, float3 direct, float t, struct state *state, __global struct item **ptr)
{
	float3		point, normal, illu_vec;
	float		diffuse = 0.06, shine = 0, ratio = 0;

	point = center + direct * t;
	normal = get_normal(item + item_index, point, center, direct, t);
	if (state != NULL && ptr != NULL)
	{
		state->point = point;
		state->normal = normal;
		state->direct = direct;
		*ptr = item + item_index;
	}
	normal *= outside(direct, normal);
	point += normal * 0.001f;
	for (int i = 0; i != opt.illu_c; i += 1)
	{
		illu_vec = (illu + i)->pref.center - point;
		t = minimal_param(item, opt.item_c, point, illu_vec, NULL);
		if (t > 0.f && t <= 1.f)
			continue ;
		if ((ratio = scalar_multiple(normalize(illu_vec), normalize(normal))) > 0.001f)
		{
			diffuse += (ratio * (illu + i)->pref.k);
			if ((item + item_index)->attr.shine != 0.f)
				if ((ratio = scalar_multiple(get_reflect_vec(-illu_vec, normal), normalize(-direct))) > 0.001)
					shine += (pow(ratio, (item + item_index)->attr.shine) * (illu + i)->pref.k);
		}
	}
	return (((item + item_index)->attr.color * diffuse) + (float3){255, 255, 255} * shine);
}

float3	traceray(__global struct item *illu, __global struct item *item, struct opt opt,
					float3 center, float3 direct, struct state *state, __global struct item **ptr)
{
	float	t;
	int		item_index;

	t = minimal_param(item, opt.item_c, center, direct, &item_index);
	if (t != INFINITY)
		return (calc_light(illu, item, item_index, opt, center, direct, t, state, ptr));
	if (ptr != NULL)
		*ptr = NULL;
	return ((float3){0, 0, 0});
}

float3	get_point(int id, struct opt *opt)
{
	float3	point;

	point = (float3){(id - (id / opt->w) * opt->w - (opt->w / 2)) * (
		(float)(1) / opt->w), (id / opt->h - (opt->h / 2)) * ((float)(1) / opt->h), 1};
	point.x += opt->center.x;
	point.y += opt->center.y;
	point.z += opt->center.z;
	return (point);
}

float3	get_direct(float3 *point, struct opt *opt)
{
	float3	direct;

	direct = normalize(*point - opt->center);
	direct = rotation_x(opt->spin_x * M_PI / 180.f, direct);
	direct = rotation_y(opt->spin_y * M_PI / 180.f, direct);
	return (direct);
}

void	create_set(__global struct item *illu, __global struct item *item,
			struct opt opt, float3 *color, struct state *ptr, __global struct item **obj)
{
	int f = 0;
	int	num = 2;
	int	obj_id = -1;
	int	color_id = 0;
	struct state state[STE * 2];

	state[0] = *ptr;
	for (int i = 0; i != DPH; i += 1)
	{
		for (int j = 0, k = 0; j != num; j += 2, k += 1)
		{
			if (obj[obj_id] != NULL && obj[obj_id]->attr.reflect != 0.f)
				color[color_id] = traceray(illu, item, opt, state[f + k].point + state[f + k].normal * 0.001f, get_reflect_vec(
					state[f + k].direct, state[f + k].normal), (i != (DPH - 1) ? state + (f == 0 ? STE : 0) + j : NULL), obj + color_id);
			if (obj[obj_id] != NULL && obj[obj_id]->attr.refract != 0.f)
				color[color_id + 1] = traceray(illu, item, opt, state[f + k].point - state[f + k].normal * 0.001f, get_refract_vec(state[f + k].direct, state[f 
					+ k].normal, obj[obj_id]->attr.refract), (i != (DPH - 1) ? state + (f == 0 ? STE : 0) + j + 1 : NULL), obj + color_id + 1);
			color_id += 2;
			obj_id += 1;
		}
		f = (f == 0 ? STE : 0);
		num <<= 1;
	}
}

void	collapse_set(float3 *color, __global struct item **obj)
{
	if (DPH != 1)
	{
		int active, next;
		for (int i = (DPH - 1); i != 0; i -= 1)
		{
			for (active = (1 << i) - 2, next = (1 << (i + 1)) - 2; active != next; active += 2)
			{
				if (obj[active] != NULL)
					color[active] = color[active] * (1.f - obj[active]->attr.reflect) + color[next] * obj[active]->attr.reflect;
			}
		}
	}
	color[-1] = color[-1] * (1.f - obj[-1]->attr.reflect) + color[0] * obj[-1]->attr.reflect;
}

__kernel void	main_func(__global uint *pixel,
	__global struct item *illu, __global struct item *item, struct opt opt)
{
	int		id = get_global_id(0);
	float3	point = get_point(id, &opt);
	float3	direct = get_direct(&point, &opt);

	struct state state;
	float3 color[CLR + 1] = {0};
	__global struct item *obj[CLR + 1] = {NULL};

	color[0] = traceray(illu, item, opt, opt.center, direct, &state, obj);
	if (obj[0] != NULL)
	{
		create_set(illu, item, opt, color + 1, &state, obj + 1);
		collapse_set(color + 1, obj + 1);
	}
	pixel[id] = rgb_to_uint(color[0]);
}
