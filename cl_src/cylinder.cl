#include "clheader.h"

float3 CylinderNormal(__global struct item *item, float3 *orig, float3 *dir, float3 *point, float t) {
	float m = DotProduct(*dir, item->vector) * t + DotProduct(*orig - item->center, item->vector);
	return (normalize(*point - item->center - item->vector * m));
}

float CylinderRestrict(__global struct item *item, float3 *orig, float3 *dir, float t0, float t1) {
	if (t0 < 0.f && t1 < 0.f) {
		return (INFINITY);
	}
	float3 OC = *orig - item->center;
	if (t0 > 0.f) {
		float m = DotProduct(*dir, item->vector) * t0 + DotProduct(OC, item->vector);
		if (m >= item->min && m <= item->max) {
			return (t0);
		}
	}
	if (t1 > 0.f) {
		float m = DotProduct(*dir, item->vector) * t1 + DotProduct(OC, item->vector);
		if (m >= item->min && m <= item->max) {
			return (t1);
		}
	}
	return (INFINITY);
}

float	CylinderIntersect(__global struct item *item, float3 *orig, float3 *dir) {
	float3 OC = *orig - item->center;
	float a = DotProduct(*dir, *dir) - pow(DotProduct(*dir, item->vector), 2);
	float b = 2 * (DotProduct(*dir, OC) - DotProduct(*dir, item->vector) * DotProduct(OC, item->vector));
	float	c = DotProduct(OC, OC) - pow(DotProduct(OC, item->vector), 2) - pow(item->radius, 2);

	float t0, t1;
	if (!SolveQuadratic(a, b, c, &t0, &t1)) {
		return (INFINITY);
	} else {
		return (CylinderRestrict(item, orig, dir, t0, t1));
	}
}
