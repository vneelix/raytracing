#include "clheader.h"

float3	EllipsoidNormal(__global struct item *item, float3 *orig, float3 *dir, float3 *point, float t) {
	float3	Cmid = item->center + item->vector * item->k / 2.f;
	float3	R = *point - Cmid;
	float	b = sqrt(pow(item->radius / 2.f, 2.f) - pow(item->k, 2.f) / 4.f);
	float	a = item->k / 2.f + item->radius - item->k;
	return (normalize(R - item->vector * (1.f - (b * b) / (a * a)) * dot(R, item->vector)));
}

float	EllipsoidIntersect(__global struct item *item, float3 *orig, float3 *dir) {
	float3	OC = *orig - item->center;
	float	A1 = 2.f * item->k * dot(*dir, item->vector);
	float	A2 = pow(item->radius, 2.f) + 2.f * item->k * dot(OC, item->vector) - item->k;
	float	a = 4.f * pow(item->radius, 2.f) * dot(*dir, *dir) - pow(A1, 2.f);
	float	b = 2.f * (4.f * pow(item->radius, 2.f) * dot(*dir, OC) - A1 * A2);
	float	c = 4.f * pow(item->radius, 2.f) * dot(OC, OC) - pow(A2, 2.f);

	float	t0, t1;
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
