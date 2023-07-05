/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/26 15:19:21 by clorin            #+#    #+#             */
/*   Updated: 2023/07/05 10:37:31 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

void	*calloc(size_t count, size_t size)
{
	size_t	total_size;
	void	*result;

	total_size = size * count;
	if (!count || !size || (total_size / count != size))
		return (NULL);
	result = malloc(total_size);
	if (result)
		ft_bzero(result, total_size);
	return (result);
}
