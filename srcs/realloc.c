/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 10:09:29 by clorin            #+#    #+#             */
/*   Updated: 2023/06/30 12:14:14 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

static void	*realloc_main(void *ptr, size_t size)
{
	t_heap	*heap;
	t_block	*block;
	void	*new_ptr;

	if (!ptr)
		return (malloc(size));
	else if (size == 0)
	{
		free(ptr);
		return (NULL);
	}
	search_ptr(&heap, &block, ptr);
	if (!heap || !block)
    {
        log_report(REALLOC, (void*)ptr, (size_t)0);
		return (NULL);
    }
	else if (size == block->data_size)
		return (ptr);
    log_report(REALLOC, ptr, size);
	new_ptr = malloc(size);
	ft_memmove(new_ptr, ptr, block->data_size > size ? size : block->data_size);
	free(ptr);
	return (new_ptr);
}

void*realloc(void *ptr, size_t size)
{
	void *res;

	res = realloc_main(ptr, size);
	return (res);
}
