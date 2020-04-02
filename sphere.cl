float	sphere(__global struct item *item, float3 center, float3 direct)
{
	float3		vec;
	float		a, b, c, d, t1, t2;

	vec = center - item->pref.center;
	a = scalar_multiple(direct, direct);
	b = 2 * scalar_multiple(vec, direct);
	c = scalar_multiple(vec, vec) - (item->pref.radius * item->pref.radius);
	if ((d = b * b - 4 * a * c) < 0)
		return (INFINITY);
	d = sqrt(d);
	t1 = (-b - d) / (2 * a);
	t2 = (-b + d) / (2 * a);
	return (get_rate(t1, t2));
}
