/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pointer.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 21:07:57 by clorin            #+#    #+#             */
/*   Updated: 2023/06/21 21:24:40 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

void	search_ptr(t_heap **ptr_heap, t_block **ptr_block, void *ptr)
{
	t_block	*block;
	t_heap	*heap;

	block = NULL;
	heap = g_heap;
	while (heap)
	{
		block = (t_block *)HEAP_OFFSET(heap);
		while (block)
		{
			if (BLOCK_OFFSET(block) == ptr)
			{
				*ptr_heap = heap;
				*ptr_block = block;
				return ;
			}
			block = block->next;
		}
		heap = heap->next;
	}
	*ptr_heap = NULL;
	*ptr_block = NULL;
}
