#include "clheader.h"

float	ConeRestrict(__global struct item *item, float3 *orig, float3 *dir, float3 *OC, float t0, float t1);

float3	ConeNormal(__global struct item *item, float3 *orig, float3 *dir, float3 *point, float t) {
	float m = dot(*dir, item->vector) * t + dot(*orig - item->center, item->vector);
	return (normalize(*point - item->center - (1 + pow(item->k, 2.f)) * item->vector * m));
}

float	ConeRestrict(__global struct item *item, float3 *orig, float3 *dir, float3 *OC, float t0, float t1) {
	if (t0 < 0.f && t1 < 0.f) {
		return (INFINITY);
	}
	if (t0 > 0.f) {
		float	m = dot(*dir, item->vector) * t0 + dot(*OC, item->vector);
		if (m >= item->min && m <= item->max) {
			return (t0);
		}
	}
	if (t1 > 0.f) {
		float	m = dot(*dir, item->vector) * t1 + dot(*OC, item->vector);
		if (m >= item->min && m <= item->max) {
			return (t1);
		}
	}
	return (INFINITY);
}

float	ConeIntersect(__global struct item *item, float3 *orig, float3 *dir) {
	float3	OC = *orig - item->center;
	float	a = dot(*dir, *dir) - (1.f + pow(item->k, 2.f)) * pow(dot(*dir, item->vector), 2.f);
	float	b = 2.f * (dot(*dir, OC) - (1.f + pow(item->k, 2.f)) * dot(*dir, item->vector) * dot(OC, item->vector));
	float	c = dot(OC, OC) - (1.f + pow(item->k, 2.f)) * pow(dot(OC, item->vector), 2.f);

	float	t0, t1;
	if (!SolveQuadratic(a, b, c, &t0, &t1)) {
		return INFINITY;
	} else {
		return ConeRestrict(item, orig, dir, &OC, t0, t1);
	}
}
