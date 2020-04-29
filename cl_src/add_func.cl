#include "clheader.h"

float	scalar_multiple(float3 a, float3 b)
{
	float ret;

	ret = a.x * b.x + a.y * b.y + a.z * b.z;
	if (ret > -0.001 && ret < 0.001)
		return (0);
	return (ret);
}

float	get_rate(float a, float b)
{
	if (a < 0 && b < 0)
		return (INFINITY);
	if ((a < b && a > 0 && b > 0) || (a > b && a > 0 && b < 0))
		return (a);
	return (b);
}

float	angle(float3 a, float3 b)
{
	return (acos(scalar_multiple(a, b)));
}

float3	get_reflect_vec(float3 vec, float3 normal)
{
	float3	ret;
	float	num;

	normal *= scalar_multiple(-vec, normal);
	return (normalize(2*normal + vec));
}

float3	get_refract_vec(float3 vec, float3 normal, float n1, float n2)
{
	float	a, b;
	float3	ret;

	a = angle(-vec, normal);
	b = asin((n1 / n2) * sin(a));
	ret = normalize(vec + normal * scalar_multiple(-vec, normal)) * tan(b);
	ret = normalize(-normal + ret);
	return (ret);
}

float3 rotation_x(float rad, float3 vec)
{
  float3	ret;

  ret.x = vec.x;
  ret.y = vec.y * cos(rad) - vec.z * sin(rad);
  ret.z = vec.y * sin(rad) + vec.z * cos(rad);
  return (ret);
}

float3 rotation_y(float rad, float3 vec)
{
  float3	ret;

  ret.x = vec.x * cos(rad) + vec.z * sin(rad);
  ret.y = vec.y;
  ret.z = vec.x * -(sin(rad)) + vec.z * cos(rad);
  return (ret);
}

float	minimal_param(__global struct item *item,
	int count, float3 center, float3 direct, int *item_index)
{
	float t, min;

	min = INFINITY;
	while (--count != -1)
	{
		if ((item + count)->type == 0)
			t = plane(item + count, center, direct);
		else if ((item + count)->type == 1)
			t = sphere(item + count, center, direct);
		else if ((item + count)->type == 2)
			t = cylinder(item + count, center, direct);
		else if ((item + count)->type == 3)
			t = cone(item + count, center, direct);
		else if ((item + count)->type == 4)
			t = paraboloid(item + count, center, direct);
		else
			t = INFINITY;
		if (t < min)
		{
			min = t;
			if (item_index != NULL)
				*item_index = count;
		}
	}
	return (min);
}
