/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block_merge.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 21:32:59 by clorin            #+#    #+#             */
/*   Updated: 2023/06/26 14:45:29 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

static t_block	*merge_prev_block(t_heap *heap, t_block *block)
{
	if (heap && block && block->prev && block->prev->freed)
	{
		block->prev->next = block->next;
		if (block->next)
			block->next->prev = block->prev;
		block->prev->data_size += block->data_size + sizeof(t_block);
		heap->block_count--;
		return (block->prev);
	}
	return (NULL);
}

static void	merge_next_block(t_heap *heap, t_block *block)
{
	if (heap && block && block->next && block->next->freed)
	{
		block->data_size += block->next->data_size ;
		if (block->next && block->next->next)
			block->next->next->prev = block;
		block->next = block->next->next;
		heap->block_count--;
	}
}

t_block	*merge_block(t_heap *heap, t_block *block)
{
	t_block	*ret;

	merge_next_block(heap, block);
	ret = merge_prev_block(heap, block);
	return (ret);
}

void	remove_block(t_heap *heap, t_block *block)
{
	if (block->freed && !block->next)
	{
		if (block->prev)
			block->prev->next = NULL;
		heap->free_size += block->data_size + sizeof(t_block);
		heap->block_count--;
	}
}
