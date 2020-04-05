#include "rtv1.h"

size_t	get_illum(t_item *item, char *file)
{
	size_t	len;
	size_t	ret;

	ret = 0;
	len = ft_strlen("illum");
	if (ft_strncmp("illum", file, len) == 0)
		ret = len + get_item(item, file + len, ILLUM_PREF, ILLUM_REQ);
	if (item != NULL)
		item->type = -1;
	return (ret);
}

size_t	get_plane(t_item *item, char *file)
{
	size_t	len;
	size_t	ret;

	len = ft_strlen("plane");
	if (ft_strncmp("plane", file, len) == 0)
		ret = len + get_item(item, file + len, PLANE_PREF, PLANE_REQ);
	else
		return (0);
	if (item != NULL)
		item->type = 0;
	return (ret);
}

size_t	get_sphere(t_item *item, char *file)
{
	size_t	len;
	size_t	ret;

	len = ft_strlen("sphere");
	if (ft_strncmp("sphere", file, len) == 0)
		ret = len + get_item(item, file + len, SPHERE_PREF, SPHERE_REQ);
	else
		return (0);
	if (item != NULL)
		item->type = 1;
	return (ret);
}

int		get_count(t_rt *rt, char *file)
{
	size_t	i;
	size_t	ret;
	size_t	len;

	i = 0;
	i += ft_splits(file + i);
	while (file[i] != '\0')
	{
		if ((ret = get_illum(NULL, file + i)))
			rt->opt.illu_c += 1;
		else if ((ret = get_plane(NULL, file + i)))
			rt->opt.item_c += 1;
		else if ((ret = get_sphere(NULL, file + i)))
			rt->opt.item_c += 1;
		if (ret == 0)
			return (-1);
		i += ft_splits(file + i + ret) + ret;
	}
	return (0);
}

int		extract_item(t_rt *rt, char *file)
{
	size_t	ret;
	size_t	len;
	size_t	index[3];

	ft_bzero(index, sizeof(size_t) * 3);
	index[0] += ft_splits(file + index[0]);
	while (file[index[0]] != '\0')
	{
		if ((ret = get_illum(rt->illu + index[1], file + index[0])))
			index[1] += 1;
		else if ((ret = get_plane(rt->item + index[2], file + index[0])))
			index[2] += 1;
		else if ((ret = get_sphere(rt->item + index[2], file + index[0])))
			index[2] += 1;
		if (ret == 0)
			return (-1);
		index[0] += ft_splits(file + index[0] + ret) + ret;
	}
	return (0);
}

int		create_item(t_rt *rt, char *file)
{
	if (get_count(rt, file))
		return (-1);
	if (rt->opt.item_c == 0)
		return (-1);
	if ((rt->illu = (t_item*)malloc(sizeof(
			t_item) * rt->opt.illu_c)) == NULL)
		return (-1);
	if ((rt->item = (t_item*)malloc(sizeof(
			t_item) * rt->opt.item_c)) == NULL)
	{
		free(rt->illu);
		return (-1);
	}
	ft_bzero(rt->illu, sizeof(t_item) * rt->opt.illu_c);
	ft_bzero(rt->item, sizeof(t_item) * rt->opt.item_c);
	if (extract_item(rt, file))
	{
		if (rt->illu)
			free(rt->illu);
		free(rt->item);
		return (-1);
	}
	return (0);
}

int		get_scene(t_rt *rt, char *file_name)
{
	int		ret;
	char	*file;

	if ((ret = open(file_name, O_RDONLY)) < 0)
		return (-1);
	file = ft_getfile(ret);
	close(ret);
	if (file == NULL || file[0] == '\0')
	{
		if (file != NULL)
			free(file);
		return (-1);
	}
	ret = create_item(rt, file);
	free(file);
	if (ret)
		return (-1);
	return (0);
}