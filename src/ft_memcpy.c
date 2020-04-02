/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/13 13:01:04 by vneelix           #+#    #+#             */
/*   Updated: 2020/02/27 17:19:24 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

void	*ft_memcpy(void *dst, const void *src, size_t size)
{
	size_t	size_char;
	size_t	offset;

	if (dst == NULL || src == NULL)
		return (NULL);
	size_char = size % 4;
	offset = size - size_char;
	size /= 4;
	while (size_char != 0)
	{
		size_char -= 1;
		((__uint8_t*)(dst))[offset + size_char] =
			((__uint8_t*)(src))[offset + size_char];
	}
	while (size != 0)
	{
		size -= 1;
		((__uint32_t*)(dst))[size] = ((__uint32_t*)(src))[size];
	}
	return (dst);
}
