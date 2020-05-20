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

bool	shadow(__global struct item *item, struct opt *opt, float3 point, float3 *direct)
{
	float t;

	for (int i = 0; i != opt->item_c; i += 1)
	{
		if ((item + i)->type == 0)
			t = plane(item + i, point, *direct);
		else if ((item + i)->type == 1)
			t = sphere(item + i, point, *direct);
		else if ((item + i)->type == 2)
			t = cylinder(item + i, point, *direct);
		else if ((item + i)->type == 3)
			t = cone(item + i, point, *direct);
		else if ((item + i)->type == 4)
			t = paraboloid(item + i, point, *direct);
		else
			t = INFINITY;
		if (t > 0.f && t <= 1.f)
			if ((item + i)->attr.refract == 0.f)
				return (true);
	}
	return (false);
}

float3	calc_light(__global struct item *illu, __global struct item *item, int item_index, struct opt *opt,
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
	for (int i = 0; i != opt->illu_c; i += 1)
	{
		illu_vec = (illu + i)->pref.center - point;
		if (shadow(item, opt, point + normal * 0.001f, &illu_vec))
			continue ;
		if ((ratio = scalar_multiple(normalize(illu_vec), normalize(normal))) > 0.001f)
		{
			diffuse += (ratio * (illu + i)->pref.k);
			if ((item + item_index)->attr.shine != 0.f)
				if ((ratio = scalar_multiple(get_reflect_vec(-illu_vec, normal), normalize(-direct))) > 0.001f)
					shine += (pow(ratio, (item + item_index)->attr.shine) * (illu + i)->pref.k);
		}
	}
	return (((item + item_index)->attr.color * diffuse) + (float3){255, 255, 255} * shine);
}

float3	traceray(__global struct item *illu, __global struct item *item, struct opt *opt,
					float3 center, float3 direct, struct state *state, __global struct item **ptr)
{
	float	t;
	int		item_index;

	t = minimal_param(item, opt->item_c, center, direct, &item_index);
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
			struct opt *opt, float3 *color, struct state *ptr, __global struct item **obj, float *arr)
{
	int f = 0;
	int	num = 2;
	int	obj_id = -1;
	int	color_id = 0;
	struct state state[STE * 2];

	float out;
	state[0] = *ptr;
	for (int i = 0; i != DPH; i += 1)
	{
		for (int j = 0, k = 0; j != num; j += 2, k += 1)
		{
			if (obj[obj_id] != NULL)
				out = outside(state[f + k].direct, state[f + k].normal);
			if (i != (DPH - 1) && obj[obj_id] != NULL && obj[obj_id]->attr.refract != 0.f)
				arr[color_id] = fresnel(state[f + k].direct, state[f + k].normal, obj[obj_id]->attr.refract);
			if (obj[obj_id] != NULL && (obj[obj_id]->attr.reflect != 0.f || obj[obj_id]->attr.refract != 0.f))
				color[color_id] = traceray(illu, item, opt, state[f + k].point + state[f + k].normal * 0.001f * out, get_reflect_vec(
					state[f + k].direct, state[f + k].normal), (i != (DPH - 1) ? state + (f == 0 ? STE : 0) + j : NULL), (i != (DPH - 1) ? obj + color_id : NULL));
			if (obj[obj_id] != NULL && obj[obj_id]->attr.refract != 0.f)
				color[color_id + 1] = traceray(illu, item, opt, state[f + k].point - state[f + k].normal * 0.001f * out, get_refract_vec(state[f + k].direct, state[f 
					+ k].normal, obj[obj_id]->attr.refract), (i != (DPH - 1) ? state + (f == 0 ? STE : 0) + j + 1 : NULL), (i != (DPH - 1) ? obj + color_id + 1 : NULL));
			color_id += 2;
			obj_id += 1;
		}
		f = (f == 0 ? STE : 0);
		num <<= 1;
	}
}

void	collapse_set(float3 *color, __global struct item **obj, float *k)
{
	if (DPH != 1)
	{
		int active, next;
		for (int i = (DPH - 1); i != 0; i -= 1)
		{
			for (int j = 0, active = (1 << i) - 2, next = (1 << (i + 1)) - 2; active != next; j += 2, active += 1)
			{
				if (obj[active] != NULL && obj[active]->attr.reflect != 0.f)
					color[active] = color[active] * (1.f - obj[active]->attr.reflect) + color[next + j] * obj[active]->attr.reflect;
				else if (obj[active] != NULL && obj[active]->attr.refract != 0.f)
					color[active] = color[next + j] * k[active] + color[next + j + 1] * (1.f - k[active]);
			}
		}
	}
	if (obj[-1] != NULL && obj[-1]->attr.reflect != 0.f)
		color[-1] = color[-1] * (1.f - obj[-1]->attr.reflect) + color[0] * obj[-1]->attr.reflect;
	else if (obj[-1] != NULL && obj[-1]->attr.refract != 0.f)
		color[-1] = color[0] * k[-1] + color[1] * (1.f - k[-1]);
}

__kernel void	main_func(__global uint *pixel,
	__global struct item *illu, __global struct item *item, struct opt opt)
{
	int		id = get_global_id(0);
	float3	point = get_point(id, &opt);
	float3	direct = get_direct(&point, &opt);

	struct state state;
	float3	color[CLR + 1] = {0};
	float	k[CLR - (1 << DPH) + 1] = {0};
	__global struct item *obj[CLR - (1 << DPH) + 1] = {NULL};

	color[0] = traceray(illu, item, &opt, opt.center, direct, &state, obj);
	if (obj[0] != NULL)
	{
		create_set(illu, item, &opt, color + 1, &state, obj + 1, k);
		collapse_set(color + 1, obj + 1, k + 1);
	}
	pixel[id] = rgb_to_uint(color[0]);
}
