/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blocks.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 19:30:43 by clorin            #+#    #+#             */
/*   Updated: 2023/06/26 14:46:21 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

static void	init_block(t_block *block, size_t size)
{
	block->prev = NULL;
	block->next = NULL;
	block->data_size = size;
	block->freed = false;
}

static t_block	*last_block(t_block *block)
{
	while (block->next)
		block = block->next;
	return (block);
}

static void	split_block(t_block *block, size_t size, t_heap *heap)
{
	t_block	*free_block;

	free_block = BLOCK_OFFSET(block) + size;
	init_block(free_block, (size_t)block->next - (size_t)free_block);
	free_block->freed = true;
	free_block->prev = block;
	free_block->next = block->next;
	heap->block_count++;
	block->next = free_block;
	block->data_size = size;
	block->freed = false;
}

static void	find_available_block(size_t size,
	t_heap **res_heap, t_block **res_block)
{
	t_heap			*heap;
	t_block			*block;
	t_heap_group	group;

	heap = g_heap;
	group = get_heap_group(size);
	while (heap)
	{
		block = (t_block *)HEAP_OFFSET(heap);
		while (heap->group == group && block)
		{
			if (block->freed && (block->data_size >= size + sizeof(t_block)))
			{
				*res_heap = heap;
				*res_block = block;
				return ;
			}
			block = block->next;
		}
		heap = heap->next;
	}
	*res_heap = NULL;
	*res_block = NULL;
}

void	*append_block(t_heap *heap, size_t size)
{
	t_block	*new_block;
	t_block	*l_block;

	new_block = (t_block *)HEAP_OFFSET(heap);
	l_block = NULL;
	if (heap->block_count)
	{
		l_block = last_block(new_block);
		new_block = (t_block *)(BLOCK_OFFSET(l_block) + l_block->data_size);
	}
	init_block(new_block, size);
	if (heap->block_count)
	{
		l_block->next = new_block;
		new_block->prev = l_block;
	}
	heap->block_count++;
	heap->free_size -= (new_block->data_size + sizeof(t_block));
	return ((void *)BLOCK_OFFSET(new_block));
}

t_block	*fill_block(size_t size)
{
	t_block	*block;
	t_heap	*heap;

	find_available_block(size, &heap, &block);
	if (block && heap)
	{
		split_block(block, size, heap);
		return (block);
	}
	return (NULL);
}
