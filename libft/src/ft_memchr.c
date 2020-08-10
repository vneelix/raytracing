/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/04 21:37:53 by vneelix           #+#    #+#             */
/*   Updated: 2019/09/08 16:08:49 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		if (((__uint8_t*)s)[i] == (__uint8_t)(c))
			break ;
		i += 1;
	}
	if (((__uint8_t*)s)[i] == (__uint8_t)(c))
		return ((void*)(s + i));
	return (NULL);
}
