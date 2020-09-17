#include "./cl_inc/clheader.clh"

float	CylinderRestrict(__global struct item *item, float3 *orig, float3 *dir, float3 *OC, float t0, float t1);

float3	CylinderNormal(__global struct item *item, float3 *orig, float3 *dir, float3 *point, float t) {
	float m = dot(*dir, item->vector) * t + dot(*orig - item->center, item->vector);
	return (normalize(*point - item->center - item->vector * m));
}

float	CylinderRestrict(__global struct item *item, float3 *orig, float3 *dir, float3 *OC, float t0, float t1) {
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

float	CylinderIntersect(__global struct item *item, float3 *orig, float3 *dir) {
	float3	OC = *orig - item->center;
	float	a = dot(*dir, *dir) - pow(dot(*dir, item->vector), 2.f);
	float	b = 2.f * (dot(*dir, OC) - dot(*dir, item->vector) * dot(OC, item->vector));
	float	c = dot(OC, OC) - pow(dot(OC, item->vector), 2.f) - pow(item->radius, 2.f);

	float	t0, t1;
	if (!SolveQuadratic(a, b, c, &t0, &t1)) {
		return (INFINITY);
	} else {
		return (CylinderRestrict(item, orig, dir, &OC, t0, t1));
	}
}
