/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_alloc_mem_hex.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 21:35:34 by clorin            #+#    #+#             */
/*   Updated: 2023/07/05 10:55:26 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

static size_t	print_block_hex(t_block *block)
{
	char	*start_address;
	char	*end_address;
	int		i;
	int		j;

	start_address = NULL;
	end_address = NULL;
	start_address = (char *)BLOCK_OFFSET(block);
	end_address = start_address + block->data_size;
	ft_putnbr_fd((size_t)start_address, 16, 1, true);
	ft_putstr(" - ");
	ft_putnbr_fd((size_t)end_address, 16, 1, true);
	if (!block->freed)
	{
		ft_putstr("\n");
		i = 0;
		while (start_address + i < end_address)
		{
			ft_putstr_fd("\t", 1);
			j = 0;
			while (j < 16 && start_address + i + j < end_address)
			{
				ft_putstr(" ");
				ft_putnbr_fd((size_t) *(start_address + j), 16, 1, false);
				j++;
			}
			ft_putstr_fd("\n", 1);
			i += 16;
		}
		return (block->data_size);
	}
	else
	{
		ft_putstr(" -> FREED (");
		ft_putnbr_fd(block->data_size, 10, 1, false);
		ft_putstr(" bytes)\n");
		return ((size_t)0);
	}
}

static size_t	print_block_list(t_block *block)
{
	size_t	total;

	total = 0;
	while (block)
	{
		total += print_block_hex(block);
		block = block->next;
	}
	return (total);
}

static void	show_mem(void)
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
			print_heap_header("TINY : ", last_heap, true);
		else if (last_heap->group == SMALL)
			print_heap_header("SMALL : ", last_heap, true);
		else
			print_heap_header("LARGE : ", last_heap, true);
		if (last_heap->block_count)
			total += print_block_list((t_block *)HEAP_OFFSET(last_heap));
		last_heap = last_heap->prev;
	}
	ft_putstr("Total : ");
	ft_putnbr_fd(total, 10, 1, false);
	ft_putstr(" bytes\n");
}

void	show_alloc_mem_hex(void)
{
	pthread_mutex_lock(&g_malloc_mutex);
	show_mem();
	pthread_mutex_unlock(&g_malloc_mutex);
}
