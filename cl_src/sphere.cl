#include "clheader.h"

float	SphereIntersect(global struct item *item, float3 *orig, float3 *dir) {
	float3	L = *orig - item->center;
	float	a = dot(*dir, *dir);
	float	b = 2 * dot(*dir, L);
	float	c = dot(L, L) - pow(item->radius, 2);

	float 	t0, t1;
	if (!SolveQuadratic(a, b, c, &t0, &t1)) {
		return (INFINITY);
	} else {
		if (t0 < 0) {
			t0 = t1;
			if (t0 < 0) {
				return (INFINITY);
			}
		}
		return (t0);
	}
}
