/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 21:12:41 by clorin            #+#    #+#             */
/*   Updated: 2023/07/05 10:39:22 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

static int nb_heap(t_heap_group group)
{
	int		nb;
	t_heap	*heap;

	nb = 0;
	heap = g_heap;
	while (heap)
	{
		if (heap->group == group)
			nb++;
		heap = heap->next;
	}
	return nb;
}

static void	delete_heap(t_heap *heap)
{
	if (heap->block_count)
		return ;
	if (heap->prev)
		heap->prev->next = heap->next;
	if (heap->next)
		heap->next->prev = heap->prev;
	if (nb_heap(heap->group) > 1)
	{	
		if (heap == g_heap)
			g_heap = heap->next;
		munmap(heap, heap->total_size);
		log_report(MUNMAP, (void *)0, (size_t)heap);
	}
}

static void	free_main(void *ptr)
{
	t_heap	*heap;
	t_block	*block;
	t_block	*ret;

	heap = g_heap;
	if (!ptr || !heap)
		return ;
	search_ptr(&heap, &block, ptr);
	if (block && heap)
	{
		block->freed = true;
		ft_memset(ptr, 0x42, block->data_size);
		ret = merge_block(heap, block);
		block = ret ? ret : block;
		remove_block(heap, block);
		log_report(FREE, (void *)0, (size_t)ptr);
		delete_heap(heap);
	}
	else
		log_report(FREE, (void *)1, (size_t)ptr);
}

void	free(void *ptr)
{
	pthread_mutex_lock(&g_malloc_mutex);
	free_main(ptr);
	pthread_mutex_unlock(&g_malloc_mutex);
}
