#include "clheader.h"

void	swap(float *a, float *b)
{
	float buff;

	buff = *a;
	*a = *b;
	*b = buff;
}

float	outside(float3 direct, float3 normal)
{
	if (scalar_multiple(direct, normal) > 0.001)
		return (-1);
	return (1);
}

float	scalar_multiple(float3 a, float3 b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
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
	return (acos(scalar_multiple(normalize(a), normalize(b))));
}

float3	get_reflect_vec(float3 direct, float3 normal)
{
	if (scalar_multiple(direct, normal) > 0.001)
		normal = -normal;
	return (normalize(2 * normal * scalar_multiple(-direct, normal) + direct));
}

float3	get_refract_vec(float3 direct, float3 normal, float etat)
{
	float cosi, eta, etai, k;

	etai = 1;
	cosi = scalar_multiple(normalize(direct), normalize(normal));
	if (cosi < 0.001)
		cosi = -cosi;
	else
	{
		normal = -normal;
		swap(&etai, &etat);
	}
	eta = etai/ etat;
	k = 1 - eta * eta * (1 - cosi * cosi); 
    return (k < 0 ? 0 : eta * direct + (eta * cosi - sqrt(k)) * normal);
}

float	fresnel(float3 direct, float3 normal, float etat)
{
	float sint, cosi, etai;

	etai = 1;
	cosi = scalar_multiple(normalize(direct), normalize(normal));
	if (cosi > 0.001)
		swap(&etai, &etat);
	sint = etai / etat * sqrt(max(0.f, 1.f - cosi * cosi));
	if (sint >= 1.f)
		return (1);
	else
	{
		float cost, Rs, Rp;

		cost = sqrt(max(0.f, 1.f - sint * sint));
		cosi = fabs(cosi);
		Rs = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
		Rp = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
		return ((Rs * Rs + Rp * Rp) / 2.f);
	}
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
	for (int i = 0; i != count; i += 1)
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
		if (t < min)
		{
			min = t;
			if (item_index != NULL)
				*item_index = i;
		}
	}
	return (min);
}
