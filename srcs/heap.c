/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 20:18:36 by clorin            #+#    #+#             */
/*   Updated: 2023/07/09 11:34:57 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include <errno.h>

static rlim_t	get_system_limit(void)
{
	struct rlimit	rpl;

	if (getrlimit(RLIMIT_DATA, &rpl) < 0)
		return (-1);
	return (rpl.rlim_max);
}

static t_heap	*get_available_heap(const t_heap *list_start,
	const t_heap_group group, const size_t required_size)
{
	t_heap	*heap_el;

	heap_el = (t_heap *)list_start;
	while (heap_el)
	{
		if ((heap_el->group == group) && (heap_el->free_size >= required_size))
			return (heap_el);
		heap_el = heap_el->next;
	}
	return (NULL);
}

size_t	get_heap_size(size_t size)
{
	t_heap_group	heap_group;
	size_t			pagesize;
	size_t			heap_size;
	size_t			multiple;

	heap_group = get_heap_group(size);
	if (heap_group == TINY)
		return ((size_t)TINY_HEAP_ALLOCATION_SIZE);
	else if (heap_group == SMALL)
		return ((size_t)SMALL_HEAP_ALLOCATION_SIZE);
	pagesize = getpagesize();
	heap_size = size + sizeof(t_heap) + sizeof(t_block);
	multiple = heap_size / pagesize;
	if (heap_size % pagesize != 0)
		multiple++;
	return (multiple * pagesize);
}

static t_heap	*create_heap(t_heap_group group, size_t block_size)
{
	size_t	heap_size;
	t_heap	*heap;

	heap_size = get_heap_size(block_size);
	if (heap_size > get_system_limit())
		return (NULL);
	heap = (t_heap *)mmap(NULL, heap_size, PROT_READ | PROT_WRITE,
			MAP_PRIVATE | MAP_ANON, -1, 0);
	if (heap == MAP_FAILED)
		return (NULL);
	ft_bzero(heap, sizeof(heap));
	heap->group = group;
	heap->total_size = heap_size;
	heap->free_size = heap_size - sizeof(t_heap);
	log_report(MMAP, heap, block_size);
	return (heap);
}

t_heap_group	get_heap_group(size_t size)
{
	if (size <= (size_t)TINY_BLOCK_SIZE)
		return (TINY);
	else if (size <= (size_t)SMALL_BLOCK_SIZE)
		return (SMALL);
	return (LARGE);
}

t_heap	*get_heap(const size_t size)
{
	t_heap			*default_heap;
	t_heap_group	heap_group;
	t_heap			*heap;

	default_heap = g_heap;
	heap_group = get_heap_group(size);
	heap = get_available_heap(default_heap,
			heap_group, size + sizeof(t_block));
	if (heap == NULL)
	{
		heap = create_heap(heap_group, size);
		if (!heap)
			return (NULL);
		heap->next = (t_heap *)default_heap;
		if (heap->next)
			heap->next->prev = heap;
		g_heap = heap;
	}
	return (heap);
}
