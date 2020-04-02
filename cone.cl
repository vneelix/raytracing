float3	cone_normal(__global struct item *item, float3 point, float3 center, float3 direct, float t)
{
	float		m;
	float3		vec;

	vec = center - item->pref.center;
	m = scalar_multiple(direct, item->pref.vector) * t + scalar_multiple(vec, item->pref.vector);
	return (normalize(point - item->pref.center - (1 + item->pref.k * item->pref.k) * m * item->pref.vector));
}

float	cone_restrict(__global struct item *item, float3 center, float3 direct, float t1, float t2)
{
	float		m;
	float3		vec;

	vec = center - item->pref.center;
	if (t1 >= 0)
	{
		m = scalar_multiple(direct, item->pref.vector) * t1 + scalar_multiple(vec, item->pref.vector);
		if (m >= -INFINITY && m <= INFINITY)
			return (t1);
	}
	if (t2 >= 0)
	{
		m = scalar_multiple(direct, item->pref.vector) * t2 + scalar_multiple(vec, item->pref.vector);
		if (m >= -INFINITY && m <= INFINITY)
			return (t2);
	}
	return (INFINITY);
}

float	cone(__global struct item *item, float3 center, float3 direct)
{
	float3		vec;
	float		a, b, c, d, t1, t2;

	vec = center - item->pref.center;
	a = scalar_multiple(direct, direct) - (1 + pow(item->pref.k, 2)) * pow(scalar_multiple(direct, item->pref.vector), 2);
	b = 2 * (scalar_multiple(direct, vec) - (1 + pow(item->pref.k, 2)) * scalar_multiple(direct, item->pref.vector) * scalar_multiple(vec, item->pref.vector));
	c = scalar_multiple(vec, vec) - (1 + pow(item->pref.k, 2)) * pow(scalar_multiple(vec, item->pref.vector), 2);
	if ((d = b * b - 4 * a * c) < 0)
		return (INFINITY);
	d = sqrt(d);
	t1 = (-b - d) / (2 * a);
	t2 = (-b + d) / (2 * a);
	if (t1 < 0 && t2 < 0)
		return (INFINITY);
	return (cone_restrict(item, center, direct, t1, t2));
}
