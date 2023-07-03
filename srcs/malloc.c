/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/05 21:23:17 by clorin            #+#    #+#             */
/*   Updated: 2023/07/03 13:22:08 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

static void	*malloc_main(size_t size)
{
	t_heap	*heap;
	t_block	*block;
	void	*result;

	if (!size)
		return (NULL);
	block = fill_block(size);
	if (block)
		return (BLOCK_OFFSET(block));
	heap = get_heap((const size_t)size);
	if (!heap)
		return ((void *)NULL);
	result = append_block(heap, size);
	return (result);
}

void	*malloc(size_t size)
{
	void	*result;

	pthread_mutex_lock(&g_malloc_mutex);
	result = malloc_main(size);
	log_report(MALLOC, result, size);
	pthread_mutex_unlock(&g_malloc_mutex);
	return (result);
}
