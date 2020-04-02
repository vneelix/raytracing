/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memalloc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/05 15:18:07 by vneelix           #+#    #+#             */
/*   Updated: 2020/02/26 17:38:28 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

void	*ft_memalloc(size_t size)
{
	void		*memory;

	if ((memory = (void*)malloc(size)) == NULL)
		return (NULL);
	while (size != 0)
	{
		size -= 1;
		((__uint8_t*)memory)[size] = 0;
	}
	return (memory);
}
