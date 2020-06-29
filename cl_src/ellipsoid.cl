#include "clheader.h"

float3 EllipsoidNormal(__global struct item *item, float3 *orig, float3 *dir, float3 *point, float t) {
	float3 Cmid = item->center + item->vector * item->k / 2.f;
	float3 R = *point - Cmid;
	float b = sqrt(pow(item->radius / 2, 2) - pow(item->k, 2) / 4);
	float a = item->k / 2 + item->radius - item->k;
	return (normalize(R - item->vector * (1.f - (b * b) / (a * a)) * DotProduct(R, item->vector)));
}

float	EllipsoidIntersect(__global struct item *item, float3 *orig, float3 *dir) {
	float3 OC = *orig - item->center;
	float A1 = 2 * item->k * DotProduct(*dir, item->vector);
	float A2 = pow(item->radius, 2) + 2 * item->k * DotProduct(OC, item->vector) - item->k;
	float a = 4 * pow(item->radius, 2) * DotProduct(*dir, *dir) - pow(A1, 2);
	float b = 2 * (4 * pow(item->radius, 2) * DotProduct(*dir, OC) - A1 * A2);
	float c = 4 * pow(item->radius, 2) * DotProduct(OC, OC) - pow(A2, 2);

	float t0, t1;
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
