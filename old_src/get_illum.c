#include "rtv1.h"

float		get_ratio(char **scene)
{
	float		value;

	value = INFINITY;
	if (ft_isdigit(**scene) == 0)
		return (value);
	value = strtof(*scene, NULL);
	while (ft_isdigit(**scene))
	{
		*scene += 1;
		if (**scene == '.')
			*scene += 1;
	}
	if (value > 0 && value <= 1)
		return (value);
	else if (value > 0 && value <= 100 && **scene == '%')
	{
		*scene += 1;
		return (value);
	}
	return (INFINITY);
}

cl_float3	get_center(char **scene)
{
	float		val[3];
	cl_float3	center;

	if (ft_isdigit(**scene) == 0)
		return ((cl_float3){INFINITY, INFINITY, INFINITY});
	val[0] = strtof(*scene, NULL);
	while (ft_isdigit(**scene))
	{
		*scene += 1;
		if (**scene == '.')
			*scene += 1;
	}
	while (**scene == ' ' || **scene == '\t' || **scene)
	if (*(*scene++) != ',')
		return ((cl_float3){INFINITY, INFINITY, INFINITY});
	return (center);
}
