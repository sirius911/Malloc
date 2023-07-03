/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_alloc_mem.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 21:35:34 by clorin            #+#    #+#             */
/*   Updated: 2023/07/03 14:45:55 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include "colors.h"

size_t	print_block(t_block *block)
{
	char	*start_address;
	char	*end_address;

	start_address = NULL;
	end_address = NULL;
	start_address = (char *)BLOCK_OFFSET(block);
	end_address = start_address + block->data_size;
	ft_putnbr_fd((size_t)start_address, 16, 1, true);
	ft_putstr(" - ");
	ft_putnbr_fd((size_t)end_address, 16, 1, true);
	ft_putstr(" : ");
	if (!block->freed)
	{
		ft_putstr(BLUE);
		ft_putnbr_fd(block->data_size, 10, 1, false);
		ft_putstr(" bytes\n");
		ft_putstr(RESET);
		return (block->data_size);
	}
	else
	{
		ft_putstr(GREEN);
		ft_putstr("FREED (");
		ft_putnbr_fd(block->data_size, 10, 1, false);
		ft_putstr(" bytes)\n");
		ft_putstr(RESET);
		return ((size_t)0);
	}
}

static size_t	print_block_list(t_block *block)
{
	size_t	total;

	total = 0;
	while (block)
	{
		total += print_block(block);
		block = block->next;
	}
	return (total);
}

void	print_heap_header(char *name, t_heap *heap, bool verbose)
{
	ft_putstr(name);
	ft_putnbr_fd((size_t)heap, 16, 1, true);
	if (verbose)
	{
		ft_putstr(" (free :");
		ft_putnbr_fd((size_t)heap->free_size, 10, 1, false);
		ft_putstr(" - ");
		ft_putnbr_fd((size_t)heap->block_count, 10, 1, false);
		ft_putstr(" block(s))");
	}
	ft_putstr("\n");
}

t_heap	*get_last_heap(t_heap *heap)
{
	if (!heap)
		return (NULL);
	while (heap->next)
		heap = heap->next;
	return (heap);
}

static void	show_mem(bool verbose)
{
	t_heap	*first_heap;
	t_heap	*last_heap;
	size_t	total;

	total = 0;
	first_heap = g_heap;
	last_heap = get_last_heap(first_heap);
	while (last_heap)
	{
		if (last_heap->group == TINY)
			print_heap_header("TINY : ", last_heap, verbose);
		else if (last_heap->group == SMALL)
			print_heap_header("SMALL : ", last_heap, verbose);
		else
			print_heap_header("LARGE : ", last_heap, verbose);
		if (last_heap->block_count)
			total += print_block_list((t_block *)HEAP_OFFSET(last_heap));
		last_heap = last_heap->prev;
	}
	ft_putstr("Total : ");
	ft_putnbr_fd(total, 10, 1, false);
	ft_putstr(" bytes\n");
}

void	show_alloc_mem(bool verbose)
{
	pthread_mutex_lock(&g_malloc_mutex);
	show_mem(verbose);
	pthread_mutex_unlock(&g_malloc_mutex);
}
