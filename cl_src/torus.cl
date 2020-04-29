#include "clheader.h"

float	torus(__global struct item *item, float3 center, float3 direct)
{
	float3	x;
	float	m, n, o, p, q;
	float	a, b, c, d, e;

	x = center - item->pref.center;
	o = scalar_multiple(x, x);
	n = scalar_multiple(direct, x);
	m = scalar_multiple(direct, direct);
	q = scalar_multiple(x, item->pref.vector);
	p = scalar_multiple(direct, item->pref.vector);

	float	R = item->pref.radius * item->pref.radius;
	float	r = item->pref.sm_rad * item->pref.sm_rad;

	a = m * m;
	b = 4 * m * n;
	c = 4 * a + 2 * m * o - 2 * (R + r) * m + 4 * R * p * p;
	d = 4 * n * o - 4 * (R + r) * n + 8 * R * p * q;
	e = o * o - 2 * (R + r) * o + 4 * R * q * q + (R - r) * (R - r);
	return (0);
}
