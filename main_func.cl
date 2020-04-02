struct __attribute__ ((aligned)) pref{
	float3		center;
	float3		vector;
	float		radius;
	float		min;
	float		max;
	float		k;
};
struct __attribute__ ((aligned)) attr{
	float3		color;
	float		shine;
	float		reflect;
	float		refract;
};
struct __attribute__ ((aligned)) item{
	struct pref	pref;
	struct attr	attr;
	int			type;
};
struct __attribute__ ((aligned)) illum{
	float		ratio;
	float3		center;
};

struct __attribute__ ((aligned)) opt{
	int			w;
	int			h;
	int			illum_c;
	int			item_c;
};

float	plane(__global struct item *item, float3 center, float3 direct);
float	sphere(__global struct item *item, float3 center, float3 direct);
float	cylinder(__global struct item *item, float3 center, float3 direct);
float3	cylinder_normal(__global struct item *item, float3 point, float3 center, float3 direct, float t);
float	cone(__global struct item *item, float3 center, float3 direct);
float3	cone_normal(__global struct item *item, float3 point, float3 center, float3 direct, float t);

float	scalar_multiple(float3 a, float3 b)
{
	return(a.x * b.x + a.y * b.y + a.z * b.z);
}

float	angle(float3 a, float3 b)
{
	return (acos(scalar_multiple(normalize(a), normalize(b))));
}

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
	else
		normal = (float3){1, 0, 0};
	if (scalar_multiple(normal, direct) > 0)
		return (-normal);
	return (normal);
}

float	get_rate(float a, float b)
{
	if (a < 0 && b < 0)
		return (INFINITY);
	if ((a < b && a > 0 && b > 0) || (a > b && a > 0 && b < 0))
		return (a);
	return (b);
}

float	calc_ratio(float3 a, float3 b)
{
	return (scalar_multiple(normalize(a), normalize(b)));
}

float3	get_reflect_vec(float3 vec, float3 normal)
{
	float		k;
	float3		refl_vec;

	k = (-1.0) / scalar_multiple(vec, normal);
	refl_vec = vec * k;
	refl_vec = refl_vec + normal;
	refl_vec = normal + refl_vec;
	return (normalize(refl_vec));
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

uint	calc_light(__global struct illum *illum, __global struct item *item, struct opt opt, float3 center, float3 direct, float t)
{
	int			i = -1;
	float3		point, normal, illum_vec;
	float		diffuse = 0.03, shine = 0, ratio;

	point = center + direct * t;
	normal = get_normal(item, point, center, direct, t);
	while ((++i) < opt.illum_c)
	{
		illum_vec = (illum + i)->center - point;
		if ((ratio = calc_ratio(illum_vec, normal)) > 0)
		{
			diffuse += (ratio * (illum + i)->ratio);
			if ((ratio = calc_ratio(get_reflect_vec(illum_vec, normal), -direct)) > 0)
				shine += (pow(ratio, item->attr.shine) * illum->ratio);
		}
	}
	return (rgb_to_uint(item->attr.color, diffuse, shine));
}

uint	traceray(__global struct illum *illum, __global struct item *item, struct opt opt, float3 center, float3 direct)
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
		return (calc_light(illum, item + item_num, opt, center, direct, t_min));
	return (0);
}

__kernel void	main_func(__global uint *pixels,
	__global struct illum *illum, __global struct item *item, struct opt opt)
{
	int			pixel;
	float3		point, direct;

	pixel = get_global_id(0);
	point = (float3){(pixel - (pixel / opt.w) * opt.w - (opt.w / 2)) * ((float)(1) / opt.w),
		(pixel / opt.h - (opt.h / 2)) * ((float)(1) / opt.h), 1};
	direct = normalize(point - (float3){0, 0, 0});
	item->pref.vector = normalize(item->pref.vector);
	pixels[pixel] = traceray(illum, item, opt, (float3){0, 0, 0}, direct);
}
