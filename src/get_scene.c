#include "rtv1.h"

size_t	ft_len(char *s)
{
	size_t	i;

	i = 0;
	while (s[i] != '\0')
		i += 1;
	return (i);
}

int		ft_strcmp(char *s1, char *s2, size_t size)
{
	size_t	i;

	i = 0;
	while (s1[i] != '\0' && s2[i] != '\0' &&  i != size)
	{
		if (s1[i] != s2[i])
			return (1);
		i += 1;
	}
	return (0);
}

int		ft_isdigit(char ch)
{
	if (ch >= '0' && ch <= '9')
		return ((int)(ch));
	return (0);
}

size_t	ft_splits(char *s)
{
	size_t	i;

	i = 0;
	while (s[i] != '\0' && (s[i] == ' ' || s[i] == '\t' || s[i] == '\n'))
			i += 1;
	return (i);
}

size_t	ft_number(char *s)
{
	size_t	i;

	i = 0;
	if (s[i] == '-' || s[i] == '+')
		i += 1;
	while (ft_isdigit(s[i]))
	{
		i += 1;
		if (s[i] == '.')
			i += 1;
	}
	return (i);
}

cl_float	get_float(char *string, size_t	*ret)
{
	cl_float	val;

	val = strtof(string, NULL);
	*ret = ft_number(string);
	return (val);
}

cl_float3	get_float3(char *string, size_t *ret)
{
	int			i;
	size_t		j;
	float		val[3];

	i = -1;
	j = 0;
	*ret = 0;
	if (string[j++] != '(')
		return ((cl_float3){INFINITY, INFINITY, INFINITY});
	j += ft_splits(string + j);
	while (++i < 3)
	{
		val[i] = get_float(string + j, ret);
		j += *ret;
		if (i != 2 && string[j++] != ',')
		{
			*ret = 0;
			return((cl_float3){INFINITY, INFINITY, INFINITY});
		}
		j += ft_splits(string + j);
	}
	if (string[j] != ')')
		return((cl_float3){INFINITY, INFINITY, INFINITY});
	*ret = j + 1;
	return((cl_float3){val[0], val[1], val[2]});
}

size_t		extract_float3(char *keyword, char *string, void *arg);
size_t		extract_float(char *keyword, char *string, void *arg);

size_t		get_illum(t_illum *illum, char *file)
{
	size_t	index[2];
	size_t	variable[2];

	ft_bzero(index, sizeof(size_t) * 2);
	ft_bzero(variable, sizeof(size_t) * 3);
	if (file[index[0]] != '{')
		return (0);
	index[0] += 1;
	index[0] += ft_splits(file + index[0]);
	while (index[1] < 2)
	{
		if (ft_strcmp(file + index[0], "center", 6) == 0)
		{
			variable[0] = index[0] + 6 + ft_splits(file + index[0] + 6);
			if (illum != NULL)
				illum->center = get_float3(file + variable[0], variable + 1);
			else
				get_float3(file + variable[0], variable + 1);
			if (variable[1] == 0)
				return (0);
			index[0] = (variable[0] + variable[1]);
		}
		else if (ft_strcmp(file + index[0], "ratio", 5) == 0)
		{
			variable[0] = index[0] + 5 + ft_splits(file + index[0] + 5);
			if (illum != NULL)
				illum->ratio = get_float(file + variable[0], variable + 1);
			else
				get_float(file + variable[0], variable + 1);
			if (variable[1] == 0)
				return (0);
			index[0] = (variable[0] + variable[1]);
		}
		if (variable[1] == 0 || (file[index[0]] != ',' && index[1] != 1))
			return (0);
		index[0] += (ft_splits(file + index[0] + 1) + 1);
		index[1] += 1;
	}
	if (file[index[0]] != '}')
		return (0);
	return (index[0] + 1);
}

size_t		get_plane(t_item *item, char *file)
{
	size_t	ret;
	size_t	index[2];

	ft_bzero(index, sizeof(size_t) * 2);
	if (file[index[0]] != '{')
		return (0);
	index[0] += (ft_splits(file + index[0] + 1) + 1);
	while (index[1] < 6)
	{
		if ((ret = extract_float3("center", file + index[0], &(item->pref.center))) != 0)
			index[0] += ret;
		else if ((ret = extract_float3("vector", file + index[0], &(item->pref.vector))) != 0)
			index[0] += ret;
		else if ((ret = extract_float3("rgb", file + index[0], &(item->attr.color))) != 0)
			index[0] += ret;
		else if ((ret = extract_float("shine", file + index[0], &(item->attr.shine))) != 0)
			index[0] += ret;
		else if ((ret = extract_float("reflect", file + index[0], &(item->attr.reflect))) != 0)
			index[0] += ret;
		else if ((ret = extract_float("refract", file + index[0], &(item->attr.refract))) != 0)
			index[0] += ret;
		if (ret == 0 || (file[index[0]] != ',' && index[1] != 5))
			return (0);
		index[0] += (ft_splits(file + index[0] + 1) + 1);
		index[1] += 1;
	}
	if (file[index[0]] != '}')
		return (0);
	return (index[0] + 1);
}

size_t		extract_float3(char *keyword, char *string, void *arg)
{
	size_t		i;
	size_t		var;
	cl_float3	float3;

	i = 0;
	var = ft_len(keyword);
	if (ft_strcmp(string, keyword, var) == 0)
	{
		i = ft_splits(string + var) + var;
		if (arg != NULL)
		{
			float3 = get_float3(string + i, &var);
			if (var != 0)
				ft_memcpy(arg, &float3, sizeof(cl_float3));
		}
		else
			get_float3(string + i, &var);
		if (var == 0)
			return (0);
		i += var;
	}
	return (i);
}

size_t		extract_float(char *keyword, char *string, void *arg)
{
	size_t		i;
	size_t		var;
	cl_float	value;

	i = 0;
	var = ft_len(keyword);
	if (ft_strcmp(string, keyword, var) == 0)
	{
		i = ft_splits(string + var) + var;
		if (arg != NULL)
		{
			value = get_float(string + i, &var);
			if (var != 0)
				ft_memcpy(arg, &value, sizeof(cl_float));
		}
		else
			get_float(string + i, &var);
		if (var == 0)
			return (0);
		i += var;
	}
	return (i);
}

int			get_scene(char *file_name)
{
	int		ret;
	char	*file;

	if ((ret = open(file_name, O_RDONLY)) < 0)
		return (-1);
	file = ft_getfile(ret);
	close(ret);
	if (file == NULL)
		return (-1);
	t_item item;
	size_t n = get_plane(&item, file + 5);
	return (0);
}