/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_item.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/12 15:19:54 by vneelix           #+#    #+#             */
/*   Updated: 2020/09/12 15:19:55 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

cl_int			get_int(char *string, size_t *ret)
{
	cl_int		val;

	val = (cl_int)(strtoimax(string, NULL, 10));
	*ret = ft_number(string);
	return (val);
}

char			*get_string(char *string, size_t *ret)
{
	char	*ptr;

	*ret = 0;
	ptr = string;
	while (*ptr != '\0' && *ptr != ' ' && *ptr != '\n' && *ptr != '\t')
		ptr += 1;
	if (!(ptr = ft_strndup(string, ptr - string)))
		return (NULL);
	*ret = ft_strlen(ptr);
	return (ptr);
}

size_t			extract_int(char *keyword, char *string, void *arg)
{
	size_t		i;
	size_t		ret;
	cl_int		value;

	i = 0;
	ret = ft_strlen(keyword);
	if (ft_strncmp(string, keyword, ret) == 0)
	{
		i = ret + ft_splits(string + ret);
		value = get_int(string + i, &ret);
		if (ret == 0)
			return (0);
		if (arg != NULL)
			ft_memcpy(arg, &value, sizeof(int));
		i += ret;
	}
	return (i);
}

size_t			extract_string(t_rt *rt, char *keyword, char *string, void *arg)
{
	size_t		i;
	cl_int		j;
	size_t		ret;
	char		*temp;

	i = 0;
	ret = ft_strlen(keyword);
	if (ft_strncmp(string, keyword, ret) == 0)
	{
		i = ret + ft_splits(string + ret);
		if ((temp = get_string(string + i, &ret)) == NULL)
			return (0);
		j = -1;
		i += ret;
		while (++j < rt->opt.texture_c)
		{
			if (ft_strncmp(temp, rt->file_name[j], ret) == 0)
			{
				ft_memcpy(arg, &j, sizeof(int));
				free(temp);
				return (i);
			}
		}
	}
	return (0);
}
