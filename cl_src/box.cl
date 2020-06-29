#include "clheader.h"
#include "quaternion.h"

float3	BoxNormal(__global struct item *item, float3 *point) {
	if(fabs(point->x - item->vMin.x) < 1e-4) {
		return (float3){-1, 0, 0};
	}
	else if(fabs(point->x - item->vMax.x) < 1e-4) {
		return (float3){1, 0, 0};
	}
	 else if(fabs(point->y - item->vMin.y) < 1e-4) {
		return (float3){0, -1, 0};
	}
	else if(fabs(point->y - item->vMax.y) < 1e-4) {
		return (float3){0, 1, 0};
	}
	else if(fabs(point->z -item->vMin.z) < 1e-4) {
		return (float3){0, 0, -1};
	}
	else if(fabs(point->z - item->vMax.z) < 1e-4) {
		return (float3){0, 0, 1};
	} else {
		return (float3){0, 0, 0};
	}
}

float	BoxIntersect(__global struct item *item, float3 *orig, float3 *dir) {
	float3 local_dir = Rotation(*dir, item->x, item->y, item->z, 0);
	float3 local_orig = RotationAround(item->center, *orig, item->x, item->y, item->z, 0);
	float3 local_vMin = RotationAround(item->center, item->vMin, item->x, item->y, item->z, 0);
	float3 local_vMax = RotationAround(item->center, item->vMax, item->x, item->y, item->z, 0);

	int sign[3];
	float3 invdir;
	float3 bound[2] = {local_vMin, local_vMax};
	float tmin, tmax, tymin, tymax, tzmin, tzmax;
	{
		invdir = 1.f / local_dir;
		sign[0] = (invdir.x < 0);
		sign[1] = (invdir.y < 0);
		sign[2] = (invdir.z < 0);
	}
	{
		tmin = (bound[sign[0]].x - local_orig.x) * invdir.x;
		tmax = (bound[1-sign[0]].x - local_orig.x) * invdir.x;
		tymin = (bound[sign[1]].y - local_orig.y) * invdir.y;
		tymax = (bound[1-sign[1]].y - local_orig.y) * invdir.y;
	}
	if ((tmin > tymax) || (tymin > tmax)) {
		return INFINITY;
	}
	tmin = max(tmin, tymin);
	tmax = min(tmax, tymax);
	tzmin = (bound[sign[2]].z - local_orig.z) * invdir.z;
	tzmax = (bound[1-sign[2]].z - local_orig.z) * invdir.z;
	if ((tmin > tzmax) || (tzmin > tmax)) {
		return INFINITY;
	}
	tmin = max(tmin, tzmin);
	tmax = min(tmax, tzmax);
	if (tmin < 0) {
		tmin = tmax;
		if (tmin < 0) {
			return INFINITY;
		}
	}
	return tmin;
}
