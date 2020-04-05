#include "rtv1.h"

size_t		extract_param(char *string, t_item *item, __uint32_t *flags)
{
	size_t	ret;

	ret = 0;
	if ((ret = extract_float("k", string, (item != NULL) ? &(item->pref.k) : NULL)))
	{
		if (*flags & K)
			return (0);
		*flags |= K;
	}
	else if ((ret = extract_float("min", string, (item != NULL) ? &(item->pref.min) : NULL)))
	{
		if (*flags & MIN)
			return (0);
		*flags |= MIN;
	}
	else if ((ret = extract_float("max", string, (item != NULL) ? &(item->pref.max) : NULL)))
	{
		if (*flags & MAX)
			return (0);
		*flags |= MAX;
	}
	else if ((ret = extract_float3("rgb", string, (item != NULL) ? &(item->attr.color) : NULL)))
	{
		if (*flags & RGB)
			return (0);
		*flags |= RGB;
	}
	else if ((ret = extract_float("shine", string, (item != NULL) ? &(item->attr.shine) : NULL)))
	{
		if (*flags & SHINE)
			return (0);
		*flags |= SHINE;
	}
	else if ((ret = extract_float("radius", string, (item != NULL) ? &(item->pref.radius) : NULL)))
	{
		if (*flags & RADIUS)
			return (0);
		*flags |= RADIUS;
	}
	else if ((ret = extract_float3("center", string, (item != NULL) ? &(item->pref.center) : NULL)))
	{
		if (*flags & CENTER)
			return (0);
		*flags |= CENTER;
	}
	else if ((ret = extract_float3("vector", string, (item != NULL) ? &(item->pref.vector) : NULL)))
	{
		if (*flags & VECTOR)
			return (0);
		*flags |= VECTOR;
	}
	else if ((ret = extract_float("reflect", string, (item != NULL) ? &(item->attr.reflect) : NULL)))
	{
		if (*flags & REFLECT)
			return (0);
		*flags |= REFLECT;
	}
	else if ((ret = extract_float("refract", string, (item != NULL) ? &(item->attr.refract) : NULL)))
	{
		if (*flags & REFRACT)
			return (0);
		*flags |= REFRACT;
	}
	return (ret);
}
