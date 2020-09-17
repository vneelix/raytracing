#include "./cl_inc/clheader.clh"

float	QuadRangle(global struct item *item, float3 *orig, float3 *dir, float t);

float	QuadRangle(global struct item *item, float3 *orig, float3 *dir, float t) {
	float3 vec3f = (*orig + *dir * t) - item->center;
	float3 basis[3] = {
		item->normal,
		(float3){0, 0, 0},
		(float3){0, 0, 0}
	};
	BuildBasis(basis);
	basis[1] *= item->min * 0.5f;
	basis[2] *= item->max * 0.5f;
	if ((fabs(dot(vec3f, basis[1]) / length(basis[1])) <= (length(basis[1]) + (float)1e-4))
		&& (fabs(dot(vec3f, basis[2]) / length(basis[2])) <= (length(basis[2]) + (float)1e-4))) {
		return t;
	} else {
		return INFINITY;
	}
}

float	PlaneIntersect(global struct item *item, float3 *orig, float3 *dir) {
	float	denom = dot(item->normal, *dir);
	if (denom != 0) {
		float3	p0l0 = item->center - *orig;
		float	t = dot(p0l0, item->normal) / denom;
		if (t < 0) {
			return INFINITY;
		}
		if (item->min > 0.f && item->max > 0.f) {
			return QuadRangle(item, orig, dir, t);
		} else {
			return t;
		}

	} else {
		return INFINITY;
	}
}
