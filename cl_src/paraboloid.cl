#include "clheader.h"

float3 ParaboloidNormal(__global struct item *item, float3 *orig, float3 *dir, float3 *point, float t) {
	float m = dot(*dir, item->vector) * t + dot(*orig - item->center, item->vector);
	return (normalize(*point - item->center - item->vector * (m + item->k)));
}

float ParaboloidRestrict(__global struct item *item, float3 *orig, float3 *dir, float t0, float t1) {
	if (t0 < 0.f && t1 < 0.f) {
		return (INFINITY);
	}
	float3 OC = *orig - item->center;
	if (t0 > 0.f) {
		float m = dot(*dir, item->vector) * t0 + dot(OC, item->vector);
		if (m >= item->min && m <= item->max) {
			return (t0);
		}
	}
	if (t1 > 0.f) {
		float m = dot(*dir, item->vector) * t1 + dot(OC, item->vector);
		if (m >= item->min && m <= item->max) {
			return (t1);
		}
	}
	return (INFINITY);
}

float	ParaboloidIntersect(__global struct item *item, float3 *orig, float3 *dir) {
	float3 OC = *orig - item->center;
	float a = dot(*dir, *dir) - pow(dot(*dir, item->vector), 2);
	float b = 2 * (dot(*dir, OC) - dot(*dir, item->vector) * (dot(OC, item->vector) + 2 * item->k));
	float	c = dot(OC, OC) - dot(OC, item->vector) * (dot(OC, item->vector) + 4 * item->k);

	float t0, t1;
	if (!SolveQuadratic(a, b, c, &t0, &t1)) {
		return (INFINITY);
	} else {
		return (ParaboloidRestrict(item, orig, dir, t0, t1));
	}
}
