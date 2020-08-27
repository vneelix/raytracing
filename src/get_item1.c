#include "rt.h"

cl_float		get_float(char *string, size_t *ret)
{
	cl_float	val;

	val = (cl_float)(strtof(string, NULL));
	*ret = ft_number(string);
	return (val);
}

size_t			extract_float(char *keyword, char *string, void *arg)
{
	size_t		i;
	size_t		ret;
	cl_float	value;

	i = 0;
	ret = ft_strlen(keyword);
	if (ft_strncmp(string, keyword, ret) == 0)
	{
		i = ft_splits(string + ret) + ret;
		value = get_float(string + i, &ret);
		if (ret == 0)
			return (0);
		if (arg != NULL)
			ft_memcpy(arg, &value, sizeof(cl_float));
		i += ret;
	}
	return (i);
}

cl_float3		get_float3(char *string, size_t *ret)
{
	cl_float	val[3];
	size_t		index[2];

	*ret = 0;
	ft_bzero(index, sizeof(size_t) * 2);
	if (string[index[0]] != '(')
		return ((cl_float3){{INFINITY, INFINITY, INFINITY}});
	index[0] += ft_splits(string + 1) + 1;
	while (index[1] < 3 && string[index[0]] != '\0')
	{
		val[index[1]] = get_float(string + index[0], ret);
		index[0] += ft_splits(string + index[0] + (*ret)) + (*ret);
		*ret = 0;
		if (string[index[0]] != ',' && index[1] != 2)
			return ((cl_float3){{INFINITY, INFINITY, INFINITY}});
		index[0] += (string[index[0]] == ',' ? 1 : 0);
		index[0] += ft_splits(string + index[0]);
		index[1] += 1;
	}
	if (string[index[0]] != ')')
		return ((cl_float3){{INFINITY, INFINITY, INFINITY}});
	*ret = index[0] + 1;
	return ((cl_float3){{val[0], val[1], val[2]}});
}

size_t			extract_float3(char *keyword, char *string, void *arg)
{
	size_t		i;
	size_t		ret;
	cl_float3	value;

	i = 0;
	ret = ft_strlen(keyword);
	if (ft_strncmp(string, keyword, ret) == 0)
	{
		i = ft_splits(string + ret) + ret;
		value = get_float3(string + i, &ret);
		if (ret == 0)
			return (0);
		if (arg != NULL)
			ft_memcpy(arg, &value, sizeof(cl_float3));
		i += ret;
	}
	return (i);
}

size_t			get_item(t_rt *rt, t_item *item, char *file, t_uint32 t)
{
	size_t		ret;
	__uint32_t	flags;
	size_t		index;

	flags = 0;
	index = ft_splits(file);
	if (file[index] != '{')
		return (0);
	index += (ft_splits(file + index + 1) + 1);
	while (file[index] != '}' && file[index] != '\0')
	{
		ret = extract_param(rt, file + index, item, &flags);
		index += (ft_splits(file + index + ret) + ret);
		if ((flags & ~t.pref) || (ret == 0 && file[index] != '{'))
			return (0);
	}
	if (file[index] != '}' || ((flags & t.req) != t.req))
		return (0);
	return (index + 1);
}
