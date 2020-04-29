#include "clheader.h"

float	plane(__global struct item *item, float3 center, float3 direct)
{
	float	t;

	if ((t = scalar_multiple(item->pref.center - center, item->pref.vector)) == 0)
		return (INFINITY);
	if ((t = t / scalar_multiple(direct, item->pref.vector)) < 0)
		return (INFINITY);
	return (t);
}
