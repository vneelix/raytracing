/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsplit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/07 15:05:26 by vneelix           #+#    #+#             */
/*   Updated: 2020/07/29 11:15:53 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	**memory_alloc(const char *string, const char separator)
{
	size_t	size;

	size = sizeof(char*);
	while (*string)
	{
		size += (*string && *string != separator) ? sizeof(char*) : 0;
		while (*string && *string != separator)
			string += 1;
		string += *string ? 1 : 0;
	}
	return (ft_memalloc(size));
}

char		**ft_strsplit(const char *string, const char separator)
{
	size_t		i;
	char		**ret;
	const char	*temp;

	if (!(ret = memory_alloc(string, separator)))
		return (NULL);
	i = 0;
	while (*string)
	{
		while (*string && *string == separator)
			string += 1;
		temp = string;
		while (*string && *string != separator)
			string += 1;
		if (temp != string)
			if (!(ret[i++] = ft_strndup(temp, string - temp)))
				return (ft_nptr_del((void**)ret));
		string += *string ? 1 : 0;
	}
	return (ret);
}
