#include "clheader.h"

float	PlaneIntersect(__global struct item *item, float3 *orig, float3 *dir) {
	float denom = DotProduct(item->normal, *dir);
	if (denom != 0) {
		float3 p0l0 = item->center - *orig;
		float t = DotProduct(p0l0, item->normal) / denom;
		return (t > 0 ? t : INFINITY);
	}
	return (INFINITY);
}
