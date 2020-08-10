/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nptrdel.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <vneelix@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/08 18:51:04 by vneelix           #+#    #+#             */
/*   Updated: 2020/07/08 18:51:16 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_nptr_del(void **nptr)
{
	void *temp;

	if (!nptr)
		return (NULL);
	temp = nptr;
	while (*nptr != NULL)
	{
		if (*nptr)
			free(*nptr);
		nptr += 1;
	}
	free(temp);
	return (NULL);
}
